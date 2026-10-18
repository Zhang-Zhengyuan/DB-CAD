"""几何 worker 网关（V3_PLAN.md ADR-1：服务端权威，网关按需拉起 C++ worker）。

P1 形态：批处理子进程（import / merge / export）。worker 是 Qt-free 控制台
（CAD_DB/worker/main.cpp，构建目标 DBCADWorker）。P2 命令引擎升级为常驻
进程 + 行协议时，本模块的 ImportResult 契约不变。

manifest 契约（dbcad.worker.import/1）由 worker 自检与 tests 锁定。
"""

from __future__ import annotations

import json
import subprocess
import tempfile
from dataclasses import dataclass, field
from pathlib import Path

WORKER_TIMEOUT_SECONDS = 300.0
MANIFEST_SCHEMA = "dbcad.worker.import/1"


class WorkerError(RuntimeError):
    """worker 缺失、退出码非零或产物不符合契约。"""


@dataclass
class ImportedComponent:
    sat: bytes
    glb: bytes
    bbox_min: list[float] = field(default_factory=lambda: [0.0, 0.0, 0.0])
    bbox_max: list[float] = field(default_factory=lambda: [0.0, 0.0, 0.0])
    color: list[float] = field(default_factory=lambda: [0.8, 0.8, 0.8])
    triangles: int = 0
    vertices: int = 0


@dataclass
class ImportResult:
    components: list[ImportedComponent]


class SubprocessWorkerGateway:
    def __init__(self, exe: Path) -> None:
        self.exe = Path(exe)

    def _run(self, *args: str) -> None:
        if not self.exe.exists():
            raise WorkerError(f"geometry worker not found: {self.exe}")
        completed = subprocess.run(  # noqa: S603 - fixed argv, no shell
            [str(self.exe), *args],
            capture_output=True,
            timeout=WORKER_TIMEOUT_SECONDS,
            check=False,
        )
        if completed.returncode != 0:
            detail = completed.stderr.decode("utf-8", "replace").strip()[-500:]
            raise WorkerError(f"worker failed ({completed.returncode}): {detail}")

    def import_sat(self, sat_bytes: bytes) -> ImportResult:
        with tempfile.TemporaryDirectory(prefix="dbcad_worker_") as tmp:
            tmp_dir = Path(tmp)
            source = tmp_dir / "input.sat"
            source.write_bytes(sat_bytes)
            out_dir = tmp_dir / "components"
            self._run("import", "--sat", str(source), "--outdir", str(out_dir))
            return self._parse_manifest(out_dir)

    @staticmethod
    def _parse_manifest(out_dir: Path) -> ImportResult:
        manifest_path = out_dir / "manifest.json"
        if not manifest_path.exists():
            raise WorkerError("worker produced no manifest.json")
        try:
            manifest = json.loads(manifest_path.read_text(encoding="utf-8"))
        except json.JSONDecodeError as exc:
            raise WorkerError(f"worker manifest is not JSON: {exc}") from exc
        if manifest.get("schema") != MANIFEST_SCHEMA:
            raise WorkerError(f"worker manifest schema mismatch: {manifest.get('schema')!r}")
        components: list[ImportedComponent] = []
        for entry in manifest.get("components", []):
            sat_path = out_dir / entry.get("sat", "")
            glb_path = out_dir / entry.get("glb", "")
            if not sat_path.exists() or not glb_path.exists():
                raise WorkerError(f"worker component files missing: {entry}")
            bbox = entry.get("bbox", {})
            components.append(
                ImportedComponent(
                    sat=sat_path.read_bytes(),
                    glb=glb_path.read_bytes(),
                    bbox_min=[float(v) for v in bbox.get("min", [0.0, 0.0, 0.0])],
                    bbox_max=[float(v) for v in bbox.get("max", [0.0, 0.0, 0.0])],
                    color=[float(v) for v in entry.get("color", [0.8, 0.8, 0.8])],
                    triangles=int(entry.get("triangles", 0)),
                    vertices=int(entry.get("vertices", 0)),
                )
            )
        if not components:
            raise WorkerError("worker manifest contains no components")
        return ImportResult(components=components)

    def merge_sat(self, parts: list[bytes]) -> bytes:
        return self._collect("merge", parts, "--out", ".sat")

    def export_glb(self, parts: list[bytes]) -> bytes:
        return self._collect("export", parts, "--glb-out", ".glb")

    def export_stl(self, parts: list[bytes]) -> bytes:
        return self._collect("export", parts, "--stl-out", ".stl")

    def _collect(self, command: str, parts: list[bytes], out_flag: str, suffix: str) -> bytes:
        if not parts:
            raise WorkerError("no components to export")
        with tempfile.TemporaryDirectory(prefix="dbcad_worker_") as tmp:
            tmp_dir = Path(tmp)
            names = []
            for index, payload in enumerate(parts):
                part = tmp_dir / f"part-{index}.sat"
                part.write_bytes(payload)
                names.append(str(part))
            output = tmp_dir / f"output{suffix}"
            self._run(command, "--sat-list", ",".join(names), out_flag, str(output))
            if not output.exists():
                raise WorkerError(f"worker produced no output for {command}")
            return output.read_bytes()


_gateway: SubprocessWorkerGateway | None = None


def get_worker_gateway() -> SubprocessWorkerGateway:
    global _gateway
    if _gateway is None:
        from .config import settings

        _gateway = SubprocessWorkerGateway(Path(settings.worker_exe))
    return _gateway


def set_worker_gateway(gateway: SubprocessWorkerGateway | None) -> None:
    global _gateway
    _gateway = gateway
