import { Sidebar } from "./Sidebar";
import { Properties, Toolbar } from "./Properties";
import { Viewer } from "./Viewer";
import { useViewer } from "./store";
import { api, ApiError, setApiPassword, userName } from "./api";
import { useEffect, useState } from "react";

export default function App() {
  const [connected, setConnected] = useState(false);
  const [checking, setChecking] = useState(true);
  const [password, setPassword] = useState("");
  const [loginError, setLoginError] = useState("");
  const connect = async () => {
    setChecking(true);
    try {
      await api.projects();
      setConnected(true);
      setLoginError("");
      setPassword("");
    } catch (e) {
      setLoginError(e instanceof ApiError && e.status === 401 ? "请输入正确的访问密码" : `连接失败：${String(e)}`);
    } finally {
      setChecking(false);
    }
  };
  useEffect(() => { void connect(); }, []);
  const activeProject = useViewer((s) => s.activeProject);
  const scene = useViewer((s) => s.scene);
  const error = useViewer((s) => s.error);
  const conflict = useViewer((s) => s.conflict);
  const loading = useViewer((s) => s.loading);
  const presence = useViewer((s) => s.presence);
  const measureCount = useViewer((s) => s.measurePoints.length);

  if (!connected) return (
    <main className="login-screen">
      <form className="login-form" onSubmit={(event) => {
        event.preventDefault();
        setApiPassword(password);
        void connect();
      }}>
        <h1>DB-CAD</h1>
        <label>访问密码<input type="password" autoComplete="current-password" value={password}
          onChange={(event) => setPassword(event.target.value)} /></label>
        {loginError && <p role="alert">{loginError}</p>}
        <button type="submit" disabled={checking}>{checking ? "连接中…" : "连接"}</button>
      </form>
    </main>
  );

  return (
    <div className="app" data-measure={measureCount}>
      <header className="header">
        <span className="brand">DB-CAD</span>
        <span className="project">
          {activeProject ? `${activeProject.name}${scene ? ` · v${scene.version}` : ""}` : "未打开项目"}
        </span>
        {loading && <span className="loading">加载中…</span>}
        <span className="presence">
          {presence.map((user) => (
            <span key={user} className={`user-chip ${user === userName ? "self" : ""}`}>
              {user === userName ? `${user}（我）` : user}
            </span>
          ))}
        </span>
      </header>
      {conflict && <div className="conflict-banner">⚠ {conflict}</div>}
      {error && <div className="error-banner">{error}</div>}
      <div className="main">
        <Sidebar />
        <div className="viewport">
          <Toolbar />
          <Viewer />
        </div>
        <Properties />
      </div>
    </div>
  );
}
