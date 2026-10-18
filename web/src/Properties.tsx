// 工具栏（显示模式/标准视图/适配）+ 属性面板（工业 CAD 入门配置）。
import { useViewer, type DisplayMode } from "./store";

const DISPLAY_MODES: { id: DisplayMode; label: string }[] = [
  { id: "shaded", label: "着色" },
  { id: "shadedEdges", label: "带边着色" },
  { id: "wireframe", label: "线框" },
  { id: "edges", label: "仅边" },
];

const VIEWS = ["fit", "front", "back", "left", "right", "top", "bottom", "iso"] as const;
const VIEW_LABELS: Record<(typeof VIEWS)[number], string> = {
  fit: "适配",
  front: "前",
  back: "后",
  left: "左",
  right: "右",
  top: "顶",
  bottom: "底",
  iso: "等轴测",
};

export function Toolbar() {
  const displayMode = useViewer((s) => s.displayMode);
  const setDisplayMode = useViewer((s) => s.setDisplayMode);
  const setView = useViewer((s) => s.setView);
  return (
    <div className="toolbar">
      <div className="segmented">
        {DISPLAY_MODES.map((mode) => (
          <button
            key={mode.id}
            className={displayMode === mode.id ? "active" : ""}
            onClick={() => setDisplayMode(mode.id)}
          >
            {mode.label}
          </button>
        ))}
      </div>
      <div className="segmented">
        {VIEWS.map((view) => (
          <button key={view} onClick={() => setView(view)}>
            {VIEW_LABELS[view]}
          </button>
        ))}
      </div>
    </div>
  );
}

export function Properties() {
  const scene = useViewer((s) => s.scene);
  const selectedId = useViewer((s) => s.selected);
  const component = scene?.components.find((c) => c.component_id === selectedId) ?? null;
  if (!component) {
    return (
      <aside className="properties">
        <h3>属性</h3>
        <p className="empty">{scene ? "点击组件查看属性" : "未打开场景"}</p>
      </aside>
    );
  }
  const size = component.bbox.max.map((v, i) => v - component.bbox.min[i]);
  return (
    <aside className="properties">
      <h3>属性</h3>
      <dl>
        <dt>名称</dt>
        <dd>{component.name}</dd>
        <dt>三角形 / 顶点</dt>
        <dd>
          {component.triangles} / {component.vertices}
        </dd>
        <dt>包围盒最小</dt>
        <dd>{component.bbox.min.map((v) => v.toFixed(2)).join(", ")}</dd>
        <dt>包围盒最大</dt>
        <dd>{component.bbox.max.map((v) => v.toFixed(2)).join(", ")}</dd>
        <dt>尺寸 (L×W×H)</dt>
        <dd>{size.map((v) => v.toFixed(2)).join(" × ")}</dd>
        <dt>颜色</dt>
        <dd>{component.color.map((v) => v.toFixed(2)).join(", ")}</dd>
        <dt className="hash">SAT 哈希</dt>
        <dd className="hash">{component.sat.slice(0, 16)}…</dd>
        <dt className="hash">GLB 哈希</dt>
        <dd className="hash">{component.glb.slice(0, 16)}…</dd>
      </dl>
    </aside>
  );
}
