"""v1 事件总线：WS /api/v1/events 的进程内发布/订阅（V3_PLAN.md §2）。

P1 单进程语义：版本提交/定版/分支事件即时广播给订阅该项目的连接。
多实例部署时替换为 Redis pub/sub 之类的外部总线——发布接口 publish()
不变，路由代码无感。

P4 增加双向通道：客户端在同一 WS 上报 presence（谁在看这个项目）与
edit_intent（谁正要编辑哪个组件），服务端即时广播给同项目其他连接。
进程内 Map 记录在场名单；连接断开即清除并广播 leave。
"""

from __future__ import annotations

import asyncio
from datetime import UTC, datetime


class EventHub:
    def __init__(self) -> None:
        self._subscribers: dict[str, set[asyncio.Queue]] = {}
        self._presence: dict[str, dict[str, str]] = {}  # project -> user -> connection marker
        self._intents: dict[str, dict[str, str]] = {}  # project -> user -> component_id

    def subscribe(self, project_id: str, queue: asyncio.Queue) -> None:
        self._subscribers.setdefault(project_id, set()).add(queue)

    def unsubscribe(self, project_id: str, queue: asyncio.Queue) -> None:
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

    # ---- 在场 / 编辑意图（P4） ---------------------------------------------
    def presence_join(self, project_id: str, user: str, marker: str) -> list[str]:
        users = self._presence.setdefault(project_id, {})
        users[user] = marker
        self._intents.setdefault(project_id, {})
        return sorted(users)

    def presence_leave(self, project_id: str, marker: str) -> list[str]:
        users = self._presence.get(project_id, {})
        for user, value in list(users.items()):
            if value == marker:
                del users[user]
                self._intents.get(project_id, {}).pop(user, None)
        return sorted(users)

    def set_intent(self, project_id: str, user: str, component_id: str | None) -> dict:
        intents = self._intents.setdefault(project_id, {})
        if component_id:
            intents[user] = component_id
        else:
            intents.pop(user, None)
        return dict(intents)

    def roster(self, project_id: str) -> dict:
        return {
            "users": sorted(self._presence.get(project_id, {})),
            "intents": dict(self._intents.get(project_id, {})),
        }


hub = EventHub()
