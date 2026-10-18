// 左侧栏：项目列表 / 版本时间线（微版本+定版标签）/ 组件树 / 参数化插入。
// 插入表单由 /api/v1/schema/commands 驱动生成（契约即表单）。
import { useEffect, useState } from "react";
import { api, type CommandSchemas } from "./api";
import { useViewer } from "./store";

function Section({ title, children }: { title: string; children: React.ReactNode }) {
  return (
    <section className="panel-section">
      <h3>{title}</h3>
      {children}
    </section>
  );
}

function InsertForm() {
  const active = useViewer((s) => s.activeProject);
  const runCommand = useViewer((s) => s.runCommand);
  const [schemas, setSchemas] = useState<CommandSchemas | null>(null);
  const [kind, setKind] = useState("cube");
  const [values, setValues] = useState<Record<string, string>>({});
  const [position, setPosition] = useState<Record<string, string>>({});

  useEffect(() => {
    api
      .schemas()
      .then((data) => setSchemas(data))
      .catch(() => setSchemas(null));
  }, []);

  if (!active) return null;
  const create = schemas?.commands["component.create"];
  const params = create?.params ?? {};
  const numericFields = Object.entries(params).filter(
    ([name, spec]) =>
      name !== "kind" && name !== "position" && spec.type.startsWith("number"),
  );

  const submit = async () => {
    const numeric: Record<string, number | number[]> = {};
    for (const [name] of numericFields) {
      const raw = values[name];
      if (raw !== undefined && raw !== "") numeric[name] = Number(raw);
    }
    const pos = ["x", "y", "z"].map((axis) => Number(position[axis] || 0));
    if (pos.some((v) => Number.isFinite(v) && v !== 0)) {
      numeric.position = pos;
    }
    const ok = await runCommand({
      action: "component.create",
      params: { kind, ...numeric },
    });
    if (ok) {
      setValues({});
      setPosition({});
    }
  };

  return (
    <Section title="插入图元">
      <div className="insert-form">
        <select value={kind} onChange={(e) => setKind(e.target.value)}>
          {(params.kind?.enum ?? ["cube"]).map((name) => (
            <option key={name} value={name}>
              {name}
            </option>
          ))}
        </select>
        <div className="param-grid">
          {numericFields.map(([name, spec]) => (
            <label key={name}>
              <span title={spec.type}>{name}</span>
              <input
                type="number"
                step="any"
                placeholder="默认"
                value={values[name] ?? ""}
                onChange={(e) => setValues((prev) => ({ ...prev, [name]: e.target.value }))}
              />
            </label>
          ))}
          {(["x", "y", "z"] as const).map((axis) => (
            <label key={axis}>
              <span>{axis}</span>
              <input
                type="number"
                step="any"
                placeholder="0"
                value={position[axis] ?? ""}
                onChange={(e) => setPosition((prev) => ({ ...prev, [axis]: e.target.value }))}
              />
            </label>
          ))}
        </div>
        <button className="primary" onClick={() => void submit()}>
          插入
        </button>
      </div>
    </Section>
  );
}

export function Sidebar() {
  const projects = useViewer((s) => s.projects);
  const activeProject = useViewer((s) => s.activeProject);
  const versions = useViewer((s) => s.versions);
  const tags = useViewer((s) => s.tags);
  const scene = useViewer((s) => s.scene);
  const selected = useViewer((s) => s.selected);
  const intents = useViewer((s) => s.intents);
  const loadProjects = useViewer((s) => s.loadProjects);
  const openProject = useViewer((s) => s.openProject);
  const showVersion = useViewer((s) => s.showVersion);
  const select = useViewer((s) => s.select);
  const toggleSelect = useViewer((s) => s.toggleSelect);

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
        <Section title={`组件 · v${scene.version}（Ctrl+点多选）`}>
          <ul className="component-list">
            {scene.components.map((component) => {
              const editor = Object.entries(intents).find(
                ([, id]) => id === component.component_id,
              )?.[0];
              return (
                <li
                  key={component.component_id}
                  className={selected.includes(component.component_id) ? "active" : ""}
                  onClick={(event) => {
                    if (event.ctrlKey || event.metaKey)
                      toggleSelect(component.component_id);
                    else
                      select(
                        selected[0] === component.component_id
                          ? null
                          : component.component_id,
                      );
                  }}
                >
                  <span className="swatch" style={{ background: rgbCss(component.color) }} />
                  <span className="name">
                    {component.name}
                    {editor && <span className="intent-badge">✏ {editor}</span>}
                  </span>
                  <span className="meta">{component.triangles} △</span>
                </li>
              );
            })}
            {scene.components.length === 0 && <li className="empty">空场景</li>}
          </ul>
        </Section>
      )}

      <InsertForm />
    </aside>
  );
}

function rgbCss(color: number[]): string {
  const to255 = (value: number) => Math.round(Math.min(1, Math.max(0, value)) * 255);
  return `rgb(${to255(color[0] ?? 0.8)},${to255(color[1] ?? 0.8)},${to255(color[2] ?? 0.8)})`;
}
