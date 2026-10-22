import { defineConfig } from "@playwright/test";

// P5 e2e 门禁（V3_PLAN.md）：真浏览器 + 真后端 + 真 ACIS worker。
// 后端独立端口 8765 避免与开发实例冲突；组件库指向临时目录保证隔离。
export default defineConfig({
  testDir: "./e2e",
  timeout: 60_000,
  retries: 0,
  workers: 1,
  use: {
    baseURL: "http://127.0.0.1:8765",
    viewport: { width: 1440, height: 900 },
    trace: "off",
  },
  webServer: {
    env: { CAD_DB_API_PASSWORD: "dbcad-e2e-access", CAD_DB_NEO4J_URI: "" },
    command: "..\\..\\web\\scripts\\e2e-server.cmd",
    cwd: "../CAD_DB/backend",
    url: "http://127.0.0.1:8765/health",
    reuseExistingServer: false,
    timeout: 30_000,
  },
});
