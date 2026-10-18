// 工具栏（显示模式/标准视图/建模操作）+ 属性面板（工业 CAD 入门配置）。
// 建模操作全部走 POST /commands（服务端权威）；布尔需要恰好两个选中
// （首=目标，次=工具），gizmo 只作用于主选组件。
import { useViewer, type DisplayMode, type GizmoMode } from "./store";

const DISPLAY_MODES: { id: DisplayMode; label: string }[] = [
  { id: "shaded", label: "着色" },
  { id: "shadedEdges", label: "带边着色" },
  { id: "wireframe", label: "线框" },
  { id: "edges", label: "仅边" },
];

const GIZMO_MODES: { id: GizmoMode; label: string }[] = [
  { id: "off", label: "浏览" },
  { id: "translate", label: "平移" },
  { id: "rotate", label: "旋转" },
  { id: "scale", label: "缩放" },
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
  const gizmoMode = useViewer((s) => s.gizmoMode);
  const setGizmoMode = useViewer((s) => s.setGizmoMode);
  const setView = useViewer((s) => s.setView);
  const selected = useViewer((s) => s.selected);
  const runCommand = useViewer((s) => s.runCommand);
  const undo = useViewer((s) => s.undo);
  const redo = useViewer((s) => s.redo);
  const active = useViewer((s) => !!s.activeProject);

  const boolean = async (op: "unite" | "subtract" | "intersect") => {
    if (selected.length !== 2) return;
    await runCommand({
      action: `boolean.${op}`,
      params: { targets: [selected[0]], tool: selected[1] },
    });
  };

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
      <div className="segmented">
        {GIZMO_MODES.map((mode) => (
          <button
            key={mode.id}
            className={gizmoMode === mode.id ? "active" : ""}
            disabled={!active || selected.length < 1}
            onClick={() => setGizmoMode(mode.id)}
          >
            {mode.label}
          </button>
        ))}
      </div>
      <div className="segmented">
        <button disabled={!active} onClick={() => void undo()} title="撤销（树状）">
          ↩ 撤销
        </button>
        <button disabled={!active} onClick={() => void redo()} title="重做">
          ↪ 重做
        </button>
        <button
          disabled={selected.length !== 2}
          onClick={() => void boolean("unite")}
          title="布尔并：首选=目标，次选=工具"
        >
          ∪ 并
        </button>
        <button disabled={selected.length !== 2} onClick={() => void boolean("subtract")}>
          ∖ 差
        </button>
        <button disabled={selected.length !== 2} onClick={() => void boolean("intersect")}>
          ∩ 交
        </button>
        <button
          disabled={selected.length < 1}
          onClick={() =>
            void runCommand({ action: "component.delete", params: { component: selected[0] } })
          }
        >
          ✕ 删除
        </button>
      </div>
    </div>
  );
}

export function Properties() {
  const scene = useViewer((s) => s.scene);
  const selectedIds = useViewer((s) => s.selected);
  const intents = useViewer((s) => s.intents);
  const component =
    scene?.components.find((c) => c.component_id === selectedIds[0]) ?? null;
  if (!component) {
    return (
      <aside className="properties">
        <h3>属性</h3>
        <p className="empty">{scene ? "点击组件查看属性" : "未打开场景"}</p>
      </aside>
    );
  }
  const size = component.bbox.max.map((v, i) => v - component.bbox.min[i]);
  const editor = Object.entries(intents).find(([, id]) => id === component.component_id)?.[0];
  return (
    <aside className="properties">
      <h3>属性{selectedIds.length > 1 ? `（${selectedIds.length} 选）` : ""}</h3>
      <dl>
        <dt>名称</dt>
        <dd>
          {component.name}
          {editor && <span className="intent-badge">✏ {editor}</span>}
        </dd>
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
