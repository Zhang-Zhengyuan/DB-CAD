# 作业 1：业务逻辑与异常处理

## 作业目标

你需要补全后端业务层的 3 个函数，让项目创建、版本保存规则、对象反序列化符合测试要求。

这个作业的重点是：

- 理解“业务层”和“存储层”的职责边界
- 学会使用 `HTTPException` 表达业务错误
- 学会把存储对象转换成 API 输出对象

## 你只需要改哪里

只允许修改这个文件：

- [backend/app/crud.py](<C:/Users/17126/source/repos/cad-db-store-develop/CAD_DB/backend/app/crud.py:19>)

只需要完成这 3 个函数：

1. [create_project](<C:/Users/17126/source/repos/cad-db-store-develop/CAD_DB/backend/app/crud.py:19>)
2. [create_model_version](<C:/Users/17126/source/repos/cad-db-store-develop/CAD_DB/backend/app/crud.py:40>)
3. [deserialize_version](<C:/Users/17126/source/repos/cad-db-store-develop/CAD_DB/backend/app/crud.py:71>)

## 你需要先看什么

先按下面顺序阅读：

1. [backend/app/schemas.py](<C:/Users/17126/source/repos/cad-db-store-develop/CAD_DB/backend/app/schemas.py:1>)
   看清楚 `ProjectCreate`、`ModelVersionCreate`、`ModelVersionRead` 的字段结构。
2. [backend/app/storage_bridge.py](<C:/Users/17126/source/repos/cad-db-store-develop/CAD_DB/backend/app/storage_bridge.py:1>)
   只需要知道 `VersionRecord` 和 `StorageBridgeClient` 返回什么。
3. [backend/app/crud.py](<C:/Users/17126/source/repos/cad-db-store-develop/CAD_DB/backend/app/crud.py:1>)
   这是你真正要改的地方。
4. [backend/tests/test_assignment_01_crud_logic.py](<C:/Users/17126/source/repos/cad-db-store-develop/CAD_DB/backend/tests/test_assignment_01_crud_logic.py:1>)
   这是判题标准。

## 具体要求

### 1. `create_project`

要求：

- 先通过 `storage_bridge.get_project_by_name_or_none(payload.name)` 查询同名项目。
- 如果已经存在，抛出 `HTTPException(status_code=409, detail="Project name already exists")`。
- 如果不存在，调用 `storage_bridge.create_project(payload.name)` 并返回结果。

你需要学到的点：

- 业务层先做校验，再调用底层存储。
- 409 表示“冲突”，适合“重名不能创建”这种场景。

### 2. `create_model_version`

要求：

- 先查当前项目的最新版本：`storage_bridge.get_latest_version_or_none(project_id)`。
- 如果已经存在最新版本，但 `payload.base_version is None`，抛出 409。
- 错误文案格式必须是：
  `base_version is required: latest version is X`
- 其余情况直接调用：
  `storage_bridge.create_model_version(project_id, payload.author, payload.content, payload.base_version)`

注意：

- 这里不需要自己处理“stale base_version”的逻辑。
- 旧版本冲突由底层存储对象自己负责。

你需要学到的点：

- 哪些规则在业务层做，哪些规则在存储层做。
- `None` 和整数版本号的语义区别。

### 3. `deserialize_version`

要求：

- 如果 `entity` 是 `VersionRecord`，返回一个 `schemas.ModelVersionRead(...)`。
- 字段必须一一正确映射。
- 如果不是 `VersionRecord`，抛出：
  `HTTPException(status_code=500, detail="Invalid version entity type")`

你需要学到的点：

- DTO / Schema 映射
- “输入类型不符合预期”如何显式失败

## 不要改什么

- 不要改 `tests/`
- 不要改 `schemas.py`
- 不要改 `storage_bridge.py`
- 不要改函数名和参数列表

## 自测方法

在 `CAD_DB/backend` 目录运行：

```powershell
uv run pytest tests/test_assignment_01_crud_logic.py
```

通过标准：

- 4 个测试全部通过

## 老师用评分点

- 是否正确区分“项目不存在”“项目重名”“版本缺少 base_version”“类型错误”
- 是否使用了正确的 HTTP 状态码
- 是否保持了函数接口和返回结构不变
