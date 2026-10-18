"""不可变组件资产的内容寻址存储（V3_PLAN.md §1：组件 SAT 源 + GLB 双写）。

资产以 sha256 为名，一次写入永不更改——这是"任意 CDN 语义"的基础：
`/api/v1/assets/{hash}` 可配 `Cache-Control: immutable` 强缓存，worker 缓存
与版本去重都以内容寻址为键（P0 基准结论：内核无 facet 缓存，worker 侧
内容寻址缓存是必须的设计）。
"""

from __future__ import annotations

import hashlib
from uuid import uuid4
from pathlib import Path

KINDS = ("sat", "glb")
KIND_MEDIA_TYPES = {"sat": "application/octet-stream", "glb": "model/gltf-binary"}


class ComponentStore:
    def __init__(self, root: Path) -> None:
        self.root = Path(root)
        for kind in KINDS:
            (self.root / kind).mkdir(parents=True, exist_ok=True)

    def digest(self, payload: bytes) -> str:
        return hashlib.sha256(payload).hexdigest()

    def _path(self, kind: str, digest: str) -> Path:
        if kind not in KINDS:
            raise ValueError(f"unsupported asset kind: {kind}")
        return self.root / kind / digest[:2] / f"{digest}.{kind}"

    def put(self, kind: str, payload: bytes) -> str:
        """写入并返回内容哈希；并发写同内容时收敛为一次落盘（幂等）。"""
        digest = self.digest(payload)
        target = self._path(kind, digest)
        if target.exists():
            return digest
        target.parent.mkdir(parents=True, exist_ok=True)
        # 唯一临时名避免并发互踩；摘要相同 ⇒ 输给 replace 竞态的一方直接采用
        # 赢方产物（字节必然一致）。
        tmp = target.with_name(f"{target.name}.{uuid4().hex}.tmp")
        tmp.write_bytes(payload)
        try:
            tmp.replace(target)
        except OSError:
            if not target.exists():
                raise
            tmp.unlink(missing_ok=True)
        return digest

    def has(self, kind: str, digest: str) -> bool:
        return self._path(kind, digest).exists()

    def read(self, kind: str, digest: str) -> bytes | None:
        target = self._path(kind, digest)
        if not target.exists():
            return None
        return target.read_bytes()


_store: ComponentStore | None = None


def get_component_store() -> ComponentStore:
    global _store
    if _store is None:
        from .config import settings

        _store = ComponentStore(Path(settings.component_root))
    return _store


def set_component_store(store: ComponentStore | None) -> None:
    global _store
    _store = store
