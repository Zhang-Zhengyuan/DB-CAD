// 左侧栏：项目列表 / 版本时间线（微版本+定版标签）/ 组件树。
import { useEffect } from "react";
import { useViewer } from "./store";

function Section({ title, children }: { title: string; children: React.ReactNode }) {
  return (
    <section className="panel-section">
      <h3>{title}</h3>
      {children}
    </section>
  );
}

export function Sidebar() {
  const projects = useViewer((s) => s.projects);
  const activeProject = useViewer((s) => s.activeProject);
  const versions = useViewer((s) => s.versions);
  const tags = useViewer((s) => s.tags);
  const scene = useViewer((s) => s.scene);
  const selected = useViewer((s) => s.selected);
  const loadProjects = useViewer((s) => s.loadProjects);
  const openProject = useViewer((s) => s.openProject);
  const showVersion = useViewer((s) => s.showVersion);
  const select = useViewer((s) => s.select);

  useEffect(() => {
    void loadProjects();
  }, [loadProjects]);

  const tagsByVersion = new Map(tags.map((tag) => [tag.version, tag.name]));

  return (
    <aside className="sidebar">
      <Section title="项目">
        <ul className="project-list">
          {projects.map((project) => (
            <li
              key={project.id}
              className={project.id === activeProject?.id ? "active" : ""}
              onClick={() => void openProject(project.id)}
            >
              <span className="name">{project.name}</span>
              <span className="meta">v{project.head_version ?? "—"}</span>
            </li>
          ))}
          {projects.length === 0 && <li className="empty">暂无项目（导入 SAT 或 API 创建）</li>}
        </ul>
      </Section>

      {activeProject && (
        <Section title={`版本时间线 · ${activeProject.name}`}>
          <ul className="version-list">
            {[...versions].reverse().map((version) => {
              const active = scene?.version === version.version;
              return (
                <li
                  key={version.version}
                  className={active ? "active" : ""}
                  onClick={() => void showVersion(version.version)}
                >
                  <span className="version-number">v{version.version}</span>
                  {tagsByVersion.get(version.version) && (
                    <span className="tag">{tagsByVersion.get(version.version)}</span>
                  )}
                  <span className="message">{version.message}</span>
                  <span className="meta">
                    {version.author} · {new Date(version.created_at).toLocaleString()}
                  </span>
                </li>
              );
            })}
            {versions.length === 0 && <li className="empty">暂无版本</li>}
          </ul>
        </Section>
      )}

      {scene && (
        <Section title={`组件 · v${scene.version}`}>
          <ul className="component-list">
            {scene.components.map((component) => (
              <li
                key={component.component_id}
                className={selected === component.component_id ? "active" : ""}
                onClick={() => select(selected === component.component_id ? null : component.component_id)}
              >
                <span className="swatch" style={{ background: rgbCss(component.color) }} />
                <span className="name">{component.name}</span>
                <span className="meta">{component.triangles} △</span>
              </li>
            ))}
            {scene.components.length === 0 && <li className="empty">空场景</li>}
          </ul>
        </Section>
      )}
    </aside>
  );
}

function rgbCss(color: number[]): string {
  const to255 = (value: number) => Math.round(Math.min(1, Math.max(0, value)) * 255);
  return `rgb(${to255(color[0] ?? 0.8)},${to255(color[1] ?? 0.8)},${to255(color[2] ?? 0.8)})`;
}
