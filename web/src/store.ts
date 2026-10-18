// 应用状态（zustand）：项目/版本链/场景缓存/显示模式/选择/协同。
// 服务端权威（ADR-1）：所有几何变更经 POST /commands；本地不做几何编辑。
// 协同：WS /api/v1/events 推送版本提交→静默刷新；在场/编辑意图双向上报。
// 409（stale_base_version 几何冲突）→ 冲突横幅 + 拉取 head。
import { create } from "zustand";
import { api, ApiError, type ProjectInfo, type Scene, type VersionInfo } from "./api";

export type DisplayMode = "shaded" | "shadedEdges" | "wireframe" | "edges";
export type GizmoMode = "off" | "translate" | "rotate" | "scale";

interface ViewerState {
  projects: ProjectInfo[];
  activeProject: ProjectInfo | null;
  versions: VersionInfo[];
  tags: { name: string; version: number }[];
  scene: Scene | null;
  sceneCache: Map<number, Scene>;
  selected: string[]; // 首个为主选（gizmo 目标）；布尔取前两个
  displayMode: DisplayMode;
  gizmoMode: GizmoMode;
  viewRequest: { view: string; nonce: number } | null;
  presence: string[];
  intents: Record<string, string>;
  conflict: string | null;
  error: string | null;
  loading: boolean;
  ws: WebSocket | null;

  loadProjects: () => Promise<void>;
  openProject: (id: string) => Promise<void>;
  showVersion: (version: number | null) => Promise<void>;
  select: (componentId: string | null) => void;
  toggleSelect: (componentId: string) => void;
  setDisplayMode: (mode: DisplayMode) => void;
  setGizmoMode: (mode: GizmoMode) => void;
  setView: (view: string) => void;
  runCommand: (payload: Record<string, unknown>) => Promise<boolean>;
  undo: () => Promise<void>;
  redo: () => Promise<void>;
  sendIntent: (component: string | null) => void;
  closeProject: () => void;
}

export const useViewer = create<ViewerState>((set, get) => {
  const refreshHead = async () => {
    const project = get().activeProject;
    if (!project) return;
    const fresh = await api.openProject(project.id);
    const list = await api.versions(project.id);
    set({ activeProject: fresh, versions: list.versions, tags: list.tags });
    await get().showVersion(null);
  };

  const connectEvents = (projectId: string) => {
    get().ws?.close();
    const ws = new WebSocket(api.eventsUrl([projectId]));
    ws.onmessage = (message) => {
      try {
        const event = JSON.parse(message.data as string) as {
          type: string;
          version?: number;
          users?: string[];
          intents?: Record<string, string>;
        };
        if (event.type === "version.committed" && typeof event.version === "number") {
          const current = get().scene?.version ?? 0;
          if (event.version > current) void refreshHead();
        } else if (event.type === "presence" && event.users) {
          set({ presence: event.users.filter((u) => u !== "") });
        } else if (event.type === "edit_intent" && event.intents) {
          set({ intents: event.intents });
        }
      } catch {
        /* 非 JSON 帧忽略 */
      }
    };
    ws.onclose = () => {
      if (get().activeProject?.id === projectId) set({ ws: null });
    };
    set({ ws });
  };

  return {
    projects: [],
    activeProject: null,
    versions: [],
    tags: [],
    scene: null,
    sceneCache: new Map(),
    selected: [],
    displayMode: "shadedEdges",
    gizmoMode: "off",
    viewRequest: null,
    presence: [],
    intents: {},
    conflict: null,
    error: null,
    loading: false,
    ws: null,

    loadProjects: async () => {
      try {
        const projects = await api.projects();
        set({ projects, error: null });
      } catch (e) {
        set({ error: String(e) });
      }
    },

    openProject: async (id) => {
      set({
        loading: true,
        sceneCache: new Map(),
        scene: null,
        selected: [],
        versions: [],
        tags: [],
        presence: [],
        intents: {},
        conflict: null,
      });
      try {
        const project = await api.openProject(id);
        const list = await api.versions(id);
        set({ activeProject: project, versions: list.versions, tags: list.tags });
        connectEvents(id);
        await get().showVersion(null);
      } catch (e) {
        set({ error: String(e), loading: false });
      }
    },

    showVersion: async (version) => {
      const project = get().activeProject;
      if (!project) return;
      const wanted = version ?? project.head_version;
      if (wanted == null) {
        set({ scene: null, loading: false });
        return;
      }
      const cached = get().sceneCache.get(wanted);
      if (cached) {
        set({ scene: cached, selected: [], loading: false });
        return;
      }
      set({ loading: true });
      try {
        const scene = await api.scene(project.id, wanted);
        get().sceneCache.set(wanted, scene);
        set({ scene, selected: [], error: null, loading: false });
      } catch (e) {
        set({ error: String(e), loading: false });
      }
    },

    select: (componentId) => {
      set({ selected: componentId ? [componentId] : [], conflict: null });
      get().sendIntent(componentId);
    },
    toggleSelect: (componentId) => {
      const current = get().selected;
      const next = current.includes(componentId)
        ? current.filter((id) => id !== componentId)
        : [...current, componentId];
      set({ selected: next.slice(0, 3) });
      get().sendIntent(next[0] ?? null);
    },
    setDisplayMode: (displayMode) => set({ displayMode }),
    setGizmoMode: (gizmoMode) => set({ gizmoMode }),
    setView: (view) =>
      set((state) => ({ viewRequest: { view, nonce: (state.viewRequest?.nonce ?? 0) + 1 } })),

    runCommand: async (payload) => {
      const project = get().activeProject;
      if (!project) return false;
      const currentHead = project.head_version;
      try {
        await api.command(project.id, { base_version: currentHead ?? undefined, ...payload });
        await refreshHead();
        set({ conflict: null });
        return true;
      } catch (e) {
        if (e instanceof ApiError && e.status === 409) {
          set({ conflict: "版本冲突：组件已被他人修改，已刷新到最新版本" });
          await refreshHead();
        } else {
          set({ error: `命令失败：${e instanceof Error ? e.message : String(e)}` });
        }
        return false;
      }
    },

    undo: async () => {
      const project = get().activeProject;
      if (!project) return;
      try {
        await api.undo(project.id);
        await refreshHead();
      } catch (e) {
        set({ error: `撤销失败：${e instanceof Error ? e.message : String(e)}` });
      }
    },
    redo: async () => {
      const project = get().activeProject;
      if (!project) return;
      try {
        await api.redo(project.id);
        await refreshHead();
      } catch (e) {
        set({ error: `重做失败：${e instanceof Error ? e.message : String(e)}` });
      }
    },

    sendIntent: (component) => {
      const ws = get().ws;
      if (ws && ws.readyState === WebSocket.OPEN) {
        ws.send(JSON.stringify({ type: "edit_intent", component }));
      }
    },

    closeProject: () => {
      get().ws?.close();
      set({ activeProject: null, scene: null, versions: [], tags: [], ws: null, presence: [], intents: {} });
    },
  };
});
