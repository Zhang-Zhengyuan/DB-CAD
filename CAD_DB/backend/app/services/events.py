"""Wire event builders shared by REST saves and WebSocket submissions."""

from typing import Any


def _model_saved_event(
    project_id: str,
    version: Any,
    *,
    trigger: str,
    request_id: str | None = None,
    source_client_id: str | None = None,
    include_content: bool = True,
) -> dict[str, Any]:
    event = {
        "type": "model_saved",
        "project_id": project_id,
        "version": version.version,
        "author": version.author,
        "created_at": version.created_at.isoformat(),
        "trigger": trigger,
    }
    if include_content:
        event["content"] = version.content
    if request_id:
        event["request_id"] = request_id
    if source_client_id:
        event["source_client_id"] = source_client_id
    return event


def _entity_graph_saved_event(
    project_id: str,
    version: Any,
    *,
    trigger: str,
    request_id: str | None = None,
    source_client_id: str | None = None,
) -> dict[str, Any]:
    """增量 entity_graph 提交事件。

    content 里包含 {entity_graph, changes, sat, sat_format}：
    - entity_graph: 提交方的完整图（含全部 uuid 节点，便于接收端重建对齐）
    - changes: 本次相对 base_version 的增量变更（ADD/REMOVE/MODIFY）
    - sat: 兜底的 SAT 文本（便于接收端在 ACIS 层面做几何重建）

    接收端应基于 changes 做增量合并（ADD/REMOVE/MODIFY），
    而不是用 sat 整体替换本地 ACIS 模型——这正是"以服务端最新一次
    提交的完整图"为基准的协作语义。
    """
    content = dict(version.content or {})
    event = {
        "type": "entity_graph_saved",
        "project_id": project_id,
        "version": version.version,
        "author": version.author,
        "created_at": version.created_at.isoformat(),
        "trigger": trigger,
        "content": content,
    }
    if request_id:
        event["request_id"] = request_id
    if source_client_id:
        event["source_client_id"] = source_client_id
    return event
