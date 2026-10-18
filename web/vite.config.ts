import { defineConfig } from "vite";
import react from "@vitejs/plugin-react";

// 开发期代理到本机 FastAPI；生产构建产物由后端静态托管（/web 挂载 dist）。
export default defineConfig({
  // 后端挂载在子路径 /web（main.py StaticFiles）；开发期 vite dev 根路径
  // 不受影响，资源引用统一走 /web 前缀。
  base: "/web/",
  plugins: [react()],
  server: {
    port: 5173,
    proxy: {
      "/api": { target: "http://127.0.0.1:8000", changeOrigin: true },
      "/ws": { target: "ws://127.0.0.1:8000", ws: true },
    },
  },
  build: { outDir: "dist", sourcemap: false },
});
