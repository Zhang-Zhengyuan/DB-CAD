# DBCAD 后端三份独立作业

- 基本的业务逻辑与异常处理
- WebSocket 连接状态管理
- WebSocket 协议消息解析与错误返回

每份作业都满足：

- 只需要改 `3` 个函数
- 只涉及 `1` 个 Python 文件
- 可以独立评测
- 不需要碰 Qt / C++ / Neo4j / Bridge

## 使用方式

1. 从当前仓库复制一个干净副本给学生。
2. 进入 `CAD_DB` 目录。
3. 对应某一份作业，应用它的补丁：

```powershell
git apply assignments\patches\assignment-01-student.patch
```

或：

```powershell
git apply assignments\patches\assignment-02-student.patch
git apply assignments\patches\assignment-03-student.patch
```

4. 学生完成后，运行对应测试：

```powershell
cd backend
uv run pytest tests/test_assignment_01_crud_logic.py
```

## 三份作业

- `assignment_01_crud.md`
  业务层：项目创建、版本保存规则、对象反序列化
- `assignment_02_sync_manager.md`
  连接管理层：断开连接、成员快照、广播清理
- `assignment_03_ws_protocol.md`
  协议层：构造事件、返回拒绝消息、分发 WebSocket 指令

## 总评测

在 `CAD_DB` 目录运行：

```powershell
.\assignments\run_assignment_checks.ps1
```

这个脚本会顺序跑三份测试。
