"""常驻几何 worker 池（V3_PLAN.md ADR-1/ADR-2 · P2 命令引擎）。

每项目一个 worker 进程（say ACIS：进程级 modeller 状态、不支持进程内并发
建模）+ 每项目一把 asyncio 锁——命令串行全序恰好就是版本链。会话同步到
head 版本：组件清单变化即重建会话（恢复 SAT，代价毫秒级，版本切换/回退后
自动对齐）。LRU 上限回收空闲进程。
"""

from __future__ import annotations

import asyncio
import json
import subprocess
import tempfile
from collections import OrderedDict
from pathlib import Path

from .component_store import get_component_store
from .config import settings
from .v1_store import SceneComponent


class WorkerError(RuntimeError):
    """worker 不可用、协议失败或命令执行失败。"""


class _Process:
    """线程模型持有 worker：Popen + 阻塞行读写，经 asyncio.to_thread 驱动。

    不用 asyncio.subprocess——transport 绑定创建它的 event loop，TestClient
    之类的逐请求换 loop 场景会炸（proactor None）。Popen + to_thread 对单
    loop 生产部署与多 loop 测试同样成立；串行性由池的每项目锁保证。
    """

    def __init__(self, exe: Path) -> None:
        self.exe = exe
        self.proc: subprocess.Popen | None = None
        self._id = 0

    def start(self) -> None:
        if not self.exe.exists():
            raise WorkerError(f"geometry worker not found: {self.exe}")
        if self.proc is not None and self.proc.poll() is None:
            return
        self.proc = subprocess.Popen(
            [str(self.exe), "serve"],
            stdin=subprocess.PIPE,
            stdout=subprocess.PIPE,
            stderr=subprocess.DEVNULL,
        )

    def _request_sync(self, op: str, payload: dict) -> dict:
        if self.proc is None or self.proc.stdin is None or self.proc.stdout is None:
            raise WorkerError("worker process not running")
        self._id += 1
        message = {"id": self._id, "op": op, **payload}
        try:
            self.proc.stdin.write((json.dumps(message) + "\n").encode())
            self.proc.stdin.flush()
            line = self.proc.stdout.readline()
        except (OSError, ValueError) as exc:
            raise WorkerError(f"worker pipe broken: {exc}") from exc
        if not line:
            raise WorkerError("worker closed the connection")
        response = json.loads(line)
        if not response.get("ok"):
            raise WorkerError(str(response.get("error", "worker error")))
        return response

    async def request(self, op: str, **payload) -> dict:
        return await asyncio.to_thread(self._request_sync, op, payload)

    def _stop_sync(self) -> None:
        if self.proc is None:
            return
        try:
            if self.proc.stdin is not None and self.proc.poll() is None:
                self._id += 1
                self.proc.stdin.write(
                    (json.dumps({"id": self._id, "op": "close"}) + "\n").encode()
                )
                self.proc.stdin.flush()
        except (OSError, ValueError):
            pass
        try:
            self.proc.wait(timeout=5)
        except subprocess.TimeoutExpired:
            self.proc.kill()
            try:
                self.proc.wait(timeout=5)
            except subprocess.TimeoutExpired:
                pass
        self.proc = None

    async def stop(self) -> None:
        await asyncio.to_thread(self._stop_sync)


class _Session:
    def __init__(self, project_id: str) -> None:
        self.project_id = project_id
        self.version = -1
        self.component_sats: list[str] = []  # hash_sat 序列，与版本清单一致
        self.spool = Path(tempfile.mkdtemp(prefix=f"dbcad_session_{project_id[:8]}_"))
        self.process = _Process(Path(settings.worker_exe))

    async def sync(self, version: int, components: list[SceneComponent]) -> None:
        sats = [c.hash_sat for c in components]
        if self.version == version and sats == self.component_sats:
            return
        assets = get_component_store()
        manifest = []
        for component in components:
            data = assets.read("sat", component.hash_sat)
            if data is None:
                raise WorkerError(f"component asset missing: {component.hash_sat}")
            name = f"{component.hash_sat}.sat"
            (self.spool / name).write_bytes(data)
            manifest.append({"sid": component.hash_sat, "file": name})
        await self.process.stop()
        self.process.start()
        await self.process.request("open", spool=str(self.spool), components=manifest)
        self.version = version
        self.component_sats = sats


class GeometryPool:
    """每项目一会话；execute() 内部串行——外部无需再加锁。"""

    def __init__(self, max_sessions: int = 8) -> None:
        self.max_sessions = max_sessions
        self._sessions: OrderedDict[str, _Session] = OrderedDict()
        self._locks: dict[str, asyncio.Lock] = {}

    def _lock(self, project_id: str) -> asyncio.Lock:
        return self._locks.setdefault(project_id, asyncio.Lock())

    def _touch(self, project_id: str) -> None:
        if project_id in self._sessions:
            self._sessions.move_to_end(project_id)

    async def _evict_locked(self) -> None:
        while len(self._sessions) > self.max_sessions:
            _, victim = self._sessions.popitem(last=False)
            await victim.process.stop()

    async def execute(
        self,
        project_id: str,
        head_version: int,
        components: list[SceneComponent],
        command: dict,
    ) -> dict:
        """在 head 版本几何上执行命令；返回 worker 的 added/modified/removed。"""
        async with self._lock(project_id):
            session = self._sessions.get(project_id)
            if session is None:
                session = _Session(project_id)
                self._sessions[project_id] = session
            self._touch(project_id)
            try:
                await session.sync(head_version, components)
                response = await session.process.request("exec", command=command)
            except WorkerError:
                # 命令失败或会话损坏：直接重建，下条命令按 head 重开。
                self._sessions.pop(project_id, None)
                await session.process.stop()
                raise
            await self._evict_locked()
            result = response.get("result", {})
            self._collect(project_id, result)
            return result

    def _collect(self, project_id: str, result: dict) -> None:
        """把 worker 发射的组件文件搬进内容寻址组件库（含实体图投影）。"""
        assets = get_component_store()
        session = self._sessions.get(project_id)
        if session is None:
            return
        for entry in (*result.get("added", ()), *result.get("modified", ())):
            sat_name = entry.get("sat", "")
            glb_name = entry.get("glb", "")
            sat_path = session.spool / sat_name
            glb_path = session.spool / glb_name
            if sat_path.exists():
                entry["sat_bytes"] = assets.put("sat", sat_path.read_bytes())
            if glb_path.exists():
                entry["glb_bytes"] = assets.put("glb", glb_path.read_bytes())
            graph_path = session.spool / entry.get("graph", "")
            if graph_path.exists() and entry.get("sat_bytes"):
                from .entity_projection import normalize_entity_projection

                # 资产与投影统一存规范化形态（root=组件 SAT 哈希）；
                # normalize 幂等，store 侧再次 normalize 为 no-op。
                try:
                    normalized = normalize_entity_projection(
                        str(entry["sat_bytes"]), json.loads(graph_path.read_text(encoding="utf-8"))
                    )
                except ValueError as exc:
                    raise WorkerError(f"entity graph payload invalid: {exc}") from exc
                entry["graph_json"] = normalized
                entry["graph_bytes"] = assets.put(
                    "graph", json.dumps(normalized, ensure_ascii=False).encode("utf-8")
                )

    async def shutdown(self) -> None:
        for session in list(self._sessions.values()):
            await session.process.stop()
        self._sessions.clear()


_pool: GeometryPool | None = None


def get_worker_pool() -> GeometryPool:
    global _pool
    if _pool is None:
        _pool = GeometryPool()
    return _pool


def set_worker_pool(pool: GeometryPool | None) -> None:
    global _pool
    _pool = pool
