# 作业 3：WebSocket 协议与消息分发

## 作业目标

你需要补全 WebSocket 协议层的 3 个函数，让后端能：

- 构造标准的 `model_saved` 事件
- 构造标准的 `submit_rejected` 事件
- 按消息内容分发 `sync_now` / `ping` / `submit_model` / 错误消息

这个作业的重点是：

- 理解“协议层”与“业务层”的分离
- 学会把内部对象转换成对外 JSON 协议
- 学会处理字符串命令、JSON 消息和错误分支

## 你只需要改哪里

只允许修改这个文件：

- [backend/app/main.py](<C:/Users/17126/source/repos/cad-db-store-develop/CAD_DB/backend/app/main.py:120>)

只需要完成这 3 个函数：

1. [_model_saved_event](<C:/Users/17126/source/repos/cad-db-store-develop/CAD_DB/backend/app/main.py:120>)
2. [_send_submit_rejected](<C:/Users/17126/source/repos/cad-db-store-develop/CAD_DB/backend/app/main.py:218>)
3. [_handle_project_ws_message](<C:/Users/17126/source/repos/cad-db-store-develop/CAD_DB/backend/app/main.py:309>)

## 你需要先看什么

先按下面顺序阅读：

1. [backend/app/main.py](<C:/Users/17126/source/repos/cad-db-store-develop/CAD_DB/backend/app/main.py:120>)
2. [backend/app/crud.py](<C:/Users/17126/source/repos/cad-db-store-develop/CAD_DB/backend/app/crud.py:50>)
3. [backend/tests/test_assignment_03_ws_protocol.py](<C:/Users/17126/source/repos/cad-db-store-develop/CAD_DB/backend/tests/test_assignment_03_ws_protocol.py:1>)

## 具体要求

### 1. `_model_saved_event`

要求：

- 返回一个 `dict`
- 必须包含这些字段：
  - `type = "model_saved"`
  - `project_id`
  - `version`
  - `author`
  - `created_at`
  - `trigger`
  - `content`
- `created_at` 需要是 `version.created_at.isoformat()`
- 如果 `request_id` 非空，附加 `request_id`
- 如果 `source_client_id` 非空，附加 `source_client_id`

你需要学到的点：

- 协议对象和内部对象之间的映射
- 可选字段的构造方式

### 2. `_send_submit_rejected`

要求：

- 先构造一个基础事件：

```python
{
    "type": "submit_rejected",
    "project_id": project_id,
    "reason": reason,
    "detail": detail,
}
```

- 如果有 `request_id`，加进去
- 然后尝试调用：
  `crud.get_latest_version_or_404(project_id)`
- 如果拿到了最新版本，再追加：
  - `latest_version`
  - `author`
  - `created_at`
  - `content`
- 如果 `crud.get_latest_version_or_404` 抛异常，不要继续抛，直接忽略最新版本信息
- 最后用：
  `await websocket.send_json(event)`

你需要学到的点：

- 组装协议响应
- “尽力返回更多信息，但失败也不能把原错误覆盖掉”

### 3. `_handle_project_ws_message`

要求：

- 先做 `normalized = message.strip()`
- 再做 `lowered = normalized.lower()`
- 若是 `"sync_now"`：
  调用 `_send_latest_model_saved_event(..., trigger="sync_now")`
- 若是 `"ping"`：
  返回 `{"type": "pong", "project_id": project_id}`
- 否则尝试 `json.loads(normalized)`
- 如果 JSON 解析失败：
  返回 `{"type": "error", "project_id": project_id, "detail": "Unsupported WebSocket message"}`
- 如果 JSON 不是对象：
  返回 `{"type": "error", "project_id": project_id, "detail": "WebSocket JSON message must be an object"}`
- 如果 `message_type == "submit_model"`：
  调用 `_handle_submit_model_message(...)`
- 如果 `message_type == "sync_now"`：
  调用 `_send_latest_model_saved_event(..., trigger="sync_now")`
- 如果 `message_type == "ping"`：
  返回 `pong`
- 否则返回：
  `Unsupported WebSocket message type: {message_type}`

你需要学到的点：

- 协议分发器怎么写
- 纯文本命令和 JSON 命令如何共存
- 错误消息如何统一格式

## 不要改什么

- 不要改 `_handle_submit_model_message`
- 不要改路由函数 `ws_project_sync`
- 不要改测试文件

## 自测方法

在 `CAD_DB/backend` 目录运行：

```powershell
uv run pytest tests/test_assignment_03_ws_protocol.py
```

通过标准：

- 4 个测试全部通过

## 老师用评分点

- 协议字段是否完整
- 错误分支是否覆盖到位
- 是否能正确区分字符串命令和 JSON 命令
