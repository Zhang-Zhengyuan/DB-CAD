// v1 API 客户端（V3_PLAN.md §2 契约）。同源部署走反向代理；独立部署时
// 设置 VITE_API_BASE。
const BASE = import.meta.env.VITE_API_BASE ?? "";

export class ApiError extends Error {
  constructor(
    public status: number,
    public detail: unknown,
  ) {
    super(`API ${status}: ${JSON.stringify(detail)}`);
  }
}

async function request<T>(path: string, init?: RequestInit): Promise<T> {
  const response = await fetch(`${BASE}${path}`, {
    ...init,
    headers: { ...init?.headers, ...(init?.body && !(init.body instanceof FormData) ? { "Content-Type": "application/json" } : {}) },
  });
  if (!response.ok) {
    let detail: unknown = response.statusText;
    try {
      detail = (await response.json())?.detail ?? detail;
    } catch {
      /* 非 JSON 错误体 */
    }
    throw new ApiError(response.status, detail);
  }
  return response.status === 204 ? (undefined as T) : ((await response.json()) as T);
}

export interface ProjectInfo {
  id: string;
  name: string;
  created_at: string;
  updated_at: string;
  head_version: number | null;
}

export interface SceneComponent {
  component_id: string;
  name: string;
  sat: string;
  glb: string;
  color: number[];
  bbox: { min: number[]; max: number[] };
  triangles: number;
  vertices: number;
}

export interface Scene {
  project_id: string;
  version: number;
  components: SceneComponent[];
}

export interface VersionInfo {
  version: number;
  message: string;
  author: string;
  created_at: string;
  base_version: number | null;
  component_count: number;
}

export interface VersionList {
  versions: VersionInfo[];
  tags: { name: string; version: number }[];
}

export const api = {
  projects: () => request<ProjectInfo[]>("/api/v1/projects"),
  openProject: (id: string) => request<ProjectInfo>(`/api/v1/projects/${id}`),
  versions: (id: string) => request<VersionList>(`/api/v1/projects/${id}/versions`),
  scene: (id: string, version?: number) =>
    request<Scene>(`/api/v1/projects/${id}/scene${version ? `?v=${version}` : ""}`),
  assetUrl: (hash: string, kind: "sat" | "glb") => `${BASE}/api/v1/assets/${hash}.${kind}`,
  command: (id: string, payload: Record<string, unknown>) =>
    request<Record<string, unknown>>(`/api/v1/projects/${id}/commands`, {
      method: "POST",
      body: JSON.stringify(payload),
    }),
  undo: (id: string) => request<Record<string, unknown>>(`/api/v1/projects/${id}/undo`, { method: "POST" }),
  redo: (id: string) => request<Record<string, unknown>>(`/api/v1/projects/${id}/redo`, { method: "POST" }),
};
