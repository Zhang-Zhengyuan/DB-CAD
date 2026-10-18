// 应用状态（zustand）：项目/版本链/场景缓存/显示模式/选择。
// 场景按版本缓存——版本切换只补拉未缓存清单（P3 验收：增量加载），
// GLB 资产按内容哈希天然缓存（R3F loader 缓存 + 浏览器强缓存）。
import { create } from "zustand";
import { api, type ProjectInfo, type Scene, type VersionInfo } from "./api";

export type DisplayMode = "shaded" | "shadedEdges" | "wireframe" | "edges";

interface ViewerState {
  projects: ProjectInfo[];
  activeProject: ProjectInfo | null;
  versions: VersionInfo[];
  tags: { name: string; version: number }[];
  scene: Scene | null;
  sceneCache: Map<number, Scene>;
  selected: string | null;
  displayMode: DisplayMode;
  viewRequest: { view: string; nonce: number } | null;
  error: string | null;
  loading: boolean;

  loadProjects: () => Promise<void>;
  openProject: (id: string) => Promise<void>;
  showVersion: (version: number | null) => Promise<void>;
  select: (componentId: string | null) => void;
  setDisplayMode: (mode: DisplayMode) => void;
  setView: (view: string) => void;
  refreshAfterCommand: () => Promise<void>;
}

export const useViewer = create<ViewerState>((set, get) => ({
  projects: [],
  activeProject: null,
  versions: [],
  tags: [],
  scene: null,
  sceneCache: new Map(),
  selected: null,
  displayMode: "shadedEdges",
  viewRequest: null,
  error: null,
  loading: false,

  loadProjects: async () => {
    try {
      const projects = await api.projects();
      set({ projects, error: null });
    } catch (e) {
      set({ error: String(e) });
    }
  },

  openProject: async (id) => {
    set({ loading: true, sceneCache: new Map(), scene: null, selected: null, versions: [], tags: [] });
    try {
      const project = await api.openProject(id);
      const list = await api.versions(id);
      set({ activeProject: project, versions: list.versions, tags: list.tags });
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
      set({ scene: cached, selected: null, loading: false });
      return;
    }
    set({ loading: true });
    try {
      const scene = await api.scene(project.id, wanted);
      const cache = get().sceneCache;
      cache.set(wanted, scene);
      set({ scene, selected: null, error: null, loading: false });
    } catch (e) {
      set({ error: String(e), loading: false });
    }
  },

  select: (componentId) => set({ selected: componentId }),
  setDisplayMode: (displayMode) => set({ displayMode }),
  setView: (view) =>
    set((state) => ({ viewRequest: { view, nonce: (state.viewRequest?.nonce ?? 0) + 1 } })),

  refreshAfterCommand: async () => {
    const project = get().activeProject;
    if (!project) return;
    const fresh = await api.openProject(project.id);
    const list = await api.versions(project.id);
    set({ activeProject: fresh, versions: list.versions, tags: list.tags });
    get().sceneCache.clear();
    await get().showVersion(null);
  },
}));
