# 作业 2：WebSocket 连接管理

## 作业目标

你需要补全 WebSocket 连接管理器的 3 个函数，让它能：

- 正确移除断开的连接
- 返回在线成员快照
- 广播消息时跳过指定客户端，并清理失效连接

这个作业的重点是：

- 理解异步代码中的共享状态保护
- 理解连接池 / 会话表的基本数据结构
- 学会写最小可用的广播逻辑

## 你只需要改哪里

只允许修改这个文件：

- [backend/app/sync.py](<C:/Users/17126/source/repos/cad-db-store-develop/CAD_DB/backend/app/sync.py:19>)

只需要完成这 3 个函数：

1. [disconnect](<C:/Users/17126/source/repos/cad-db-store-develop/CAD_DB/backend/app/sync.py:42>)
2. [list_members](<C:/Users/17126/source/repos/cad-db-store-develop/CAD_DB/backend/app/sync.py:68>)
3. [broadcast](<C:/Users/17126/source/repos/cad-db-store-develop/CAD_DB/backend/app/sync.py:81>)

## 你需要先看什么

先按下面顺序阅读：

1. [backend/app/sync.py](<C:/Users/17126/source/repos/cad-db-store-develop/CAD_DB/backend/app/sync.py:1>)
   重点看 `ProjectSyncManager.__init__` 和 `connect`。
2. [backend/app/main.py](<C:/Users/17126/source/repos/cad-db-store-develop/CAD_DB/backend/app/main.py:309>)
   看这些函数最终怎么被调用，但不用修改。
3. [backend/tests/test_assignment_02_sync_manager.py](<C:/Users/17126/source/repos/cad-db-store-develop/CAD_DB/backend/tests/test_assignment_02_sync_manager.py:1>)
   这是判题标准。

## 数据结构说明

`self._connections` 的结构是：

```python
{
    "project-id-1": {
        "client-id-a": ClientConnection(...),
        "client-id-b": ClientConnection(...),
    },
    "project-id-2": {
        ...
    }
}
```

其中 `ClientConnection` 有：

- `websocket`
- `client_id`
- `author`
- `connected_at`

## 具体要求

### 1. `disconnect`

要求：

- 在 `async with self._lock:` 下操作共享状态。
- 找到与传入 `websocket` 对应的连接。
- 删除这个连接。
- 如果该项目没有剩余连接，要把整个项目键也删掉。
- 返回：

```python
{
    "client_id": "...",
    "author": "...",
}
```

- 如果没找到，返回 `None`。
- 如果 `author` 为空，返回 `"unknown"`。

你需要学到的点：

- 如何安全地修改共享字典
- 如何在删除后顺手做“空容器清理”

### 2. `list_members`

要求：

- 在锁内拿到当前项目所有连接
- 返回一个 `list[dict]`
- 每个成员字典要有：
  `client_id`、`author`、`connected_at`
- `connected_at` 要转成字符串：`client.connected_at.isoformat()`

你需要学到的点：

- 面向 API 的输出格式构造
- 为什么要先复制，再在锁外组装返回值

### 3. `broadcast`

要求：

- 先把 `event` 转成 JSON 文本：
  `json.dumps(event, ensure_ascii=False)`
- 在锁内复制当前项目的连接列表
- 如果没有连接，直接返回
- 广播时：
  - 如果 `exclude_client_id` 命中，跳过该客户端
  - 否则调用 `await client.websocket.send_text(message)`
- 如果发送失败：
  - 不要立刻崩溃
  - 把这个 `websocket` 记到 `disconnected`
- 广播结束后，对所有失败的连接调用：
  `await self.disconnect(project_id, socket)`

你需要学到的点：

- 为什么不能在持锁状态下做网络 IO
- 广播失败后如何做“延迟清理”

## 不要改什么

- 不要改 `connect`
- 不要改 `_connections` 的结构
- 不要改测试文件

## 自测方法

在 `CAD_DB/backend` 目录运行：

```powershell
uv run pytest tests/test_assignment_02_sync_manager.py
```

通过标准：

- 3 个测试全部通过

## 老师用评分点

- 是否正确使用了 `self._lock`
- 是否在广播失败后清理死连接
- 是否能正确处理“空项目”和“排除自己”的情况
