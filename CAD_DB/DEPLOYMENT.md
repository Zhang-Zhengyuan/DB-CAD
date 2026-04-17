# DBCAD 验收部署指南（服务器 + 客户机）

## 目标
- 服务器：运行 FastAPI 统一后端（可接 Neo4j）。
- 客户机：运行打包后的 Qt EXE，配置后即可远程访问。

---

## 一、服务器部署（推荐 uv）

### 1. 前置
- Windows Server / Windows 10+
- 可访问 Neo4j（若使用 `neo4j` 存储后端）

### 2. 一键启动
在仓库根目录执行：

```powershell
.\CAD_DB\deploy\server\deploy_backend_uv.ps1 \
  -HostAddress 0.0.0.0 \
  -Port 8000 \
  -StorageBackend neo4j \
  -Neo4jUri bolt://127.0.0.1:7687 \
  -Neo4jUser neo4j \
  -Neo4jPassword "your_password"
```

### 3. 验证
```powershell
.\CAD_DB\deploy\server\check_backend.ps1 -BaseUrl http://127.0.0.1:8000
```

通过后可访问：
- `http://服务器IP:8000/health`
- `http://服务器IP:8000/docs`

---

## 二、客户机部署（Qt EXE 打包）

### 1. 一键打包
在开发机执行：

```powershell
.\CAD_DB\deploy\client\package_client.ps1 \
  -Configuration Release \
  -Platform x64 \
  -ServerBaseUrl http://服务器IP:8000 \
  -Author customer-user
```

输出：
- `dist/DBCAD-client-Release.zip`

### 2. 客户机安装
- 解压 zip 到任意目录
- 双击 EXE 运行
- 如需本地 neo4j 直连模式，编辑同目录 `neo4j_connect_info.conf`

---

## 三、验收清单
- [ ] 客户端可打开 FastAPI 模式并加载项目。
- [ ] 客户端可保存模型，后端版本号递增。
- [ ] 两台客户机同时打开同一项目，一台保存后另一台收到实时同步。
- [ ] 切换到 ACIS/neo4j 本地模式仍可正常保存加载。

---

## 四、通信说明
当前 C++ 与 Python 后端通信方式：
- HTTP REST（保存/加载）
- WebSocket（实时订阅）

不依赖 pybind。该方式天然支持跨电脑部署与运维分层。
