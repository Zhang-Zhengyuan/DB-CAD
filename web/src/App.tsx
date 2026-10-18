import { Sidebar } from "./Sidebar";
import { Properties, Toolbar } from "./Properties";
import { Viewer } from "./Viewer";
import { useViewer } from "./store";

export default function App() {
  const activeProject = useViewer((s) => s.activeProject);
  const scene = useViewer((s) => s.scene);
  const error = useViewer((s) => s.error);
  const loading = useViewer((s) => s.loading);

  return (
    <div className="app">
      <header className="header">
        <span className="brand">DB-CAD</span>
        <span className="project">
          {activeProject ? `${activeProject.name}${scene ? ` · v${scene.version}` : ""}` : "未打开项目"}
        </span>
        {loading && <span className="loading">加载中…</span>}
      </header>
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
