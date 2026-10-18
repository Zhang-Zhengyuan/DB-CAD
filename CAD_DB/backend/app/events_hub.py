"""v1 事件总线：WS /api/v1/events 的进程内发布/订阅（V3_PLAN.md §2）。

P1 单进程语义：版本提交/定版/分支事件即时广播给订阅该项目的连接。
多实例部署时替换为 Redis pub/sub 之类的外部总线——发布接口 publish()
 不变，路由代码无感。
"""

from __future__ import annotations

import asyncio
from datetime import UTC, datetime


class EventHub:
    def __init__(self) -> None:
        self._subscribers: dict[str, set[asyncio.Queue]] = {}

    def subscribe(self, project_ids: list[str]) -> asyncio.Queue:
        queue: asyncio.Queue = asyncio.Queue(maxsize=256)
        for project_id in project_ids:
            self._subscribers.setdefault(project_id, set()).add(queue)
        return queue

    def unsubscribe(self, queue: asyncio.Queue, project_ids: list[str]) -> None:
        for project_id in project_ids:
            self._subscribers.get(project_id, set()).discard(queue)

    def publish(self, project_id: str, event_type: str, payload: dict) -> None:
        event = {
            "type": event_type,
            "project_id": project_id,
            "time": datetime.now(UTC).isoformat(),
            **payload,
        }
        for queue in list(self._subscribers.get(project_id, ())):
            try:
                queue.put_nowait(event)
            except asyncio.QueueFull:  # 慢消费者丢事件，重连后以 /versions 对账
                pass


hub = EventHub()
