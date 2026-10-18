// P5 e2e（V3_PLAN.md）：真栈端到端——打开渲染、建模命令、测量、版本切换。
import { expect, test, type Page } from "@playwright/test";

const API = "/api/v1";

async function api(page: Page, path: string, payload?: unknown) {
  const response = await page.request[path.startsWith("/projects") ? "get" : "get"](API + path, {});
  return response.json();
}

async function command(page: Page, projectId: string, payload: Record<string, unknown>) {
  const response = await page.request.post(`${API}/projects/${projectId}/commands`, { data: payload });
  expect(response.ok(), await response.text()).toBeTruthy();
  return response.json();
}

test("真栈端到端：渲染/建模/测量/版本切换", async ({ page }) => {
  // 种子项目（真 ACIS worker）
  const created = await page.request.post(`${API}/projects`, { data: { name: `e2e-${Date.now()}` } });
  expect(created.ok(), await created.text()).toBeTruthy();
  const project = await created.json();
  await command(page, project.id, {
    action: "component.create",
    params: { kind: "cube", width: 12, height: 12, depth: 12 },
  });
  await command(page, project.id, {
    action: "component.create",
    params: { kind: "sphere", radius: 4, position: [10, 0, 0] },
  });

  // 打开查看器
  await page.goto("/web/");
  await page.getByRole("listitem").filter({ hasText: project.name }).click();
  await expect(page.locator(".header .project")).toContainText("v2", { timeout: 15_000 });
  await expect(page.locator(".component-list li")).toHaveCount(2);
  await expect(page.locator("canvas")).toBeVisible();

  // 组件树点选 → 属性面板
  await page.locator(".component-list li").first().click();
  await expect(page.locator(".properties")).toContainText("cube");
  await expect(page.locator(".properties")).toContainText("12.00 × 12.00 × 12.00");

  // GLB 资产直取（内容寻址 + 强缓存头）
  const glbHash = await page.evaluate(() => {
    const entry = performance.getEntriesByType("resource").find((e) => e.name.endsWith(".glb"));
    return entry ? entry.name.split("/").pop() : null;
  });
  expect(glbHash).toBeTruthy();
  const asset = await page.request.get(`${API}/assets/${glbHash}`);
  expect(asset.ok()).toBeTruthy();
  expect((await asset.headers())["cache-control"]).toContain("immutable");

  // 测量：先等轴测居中（初始相机几何可能偏出中心），再点两下取距离
  await page.getByRole("button", { name: "等轴测" }).click();
  await page.waitForTimeout(500);
  await page.getByRole("button", { name: "测量" }).click();
  const canvas = page.locator("canvas");
  const box = await canvas.boundingBox();
  expect(box).toBeTruthy();
  const cx = box!.x + box!.width / 2;
  const cy = box!.y + box!.height / 2;
  await page.mouse.click(cx - 40, cy);
  await page.waitForTimeout(300);
  await page.screenshot({ path: "test-results/measure-debug.png" });
  await page.mouse.click(cx + 40, cy);
  await expect(page.locator(".measure-label")).toHaveText(/\d+\.\d{3}/, { timeout: 10_000 });

  // 建模：插入圆环（schema 表单）
  await page.getByRole("button", { name: "测量" }).click(); // 关测量
  await page.locator(".insert-form select").selectOption("torus");
  await page.getByRole("button", { name: "插入" }).click();
  await expect(page.locator(".header .project")).toContainText("v3", { timeout: 20_000 });
  await expect(page.locator(".component-list li")).toHaveCount(3);

  // 版本切换（缓存增量：v1 只有立方体）
  await page.locator(".version-list li").filter({ hasText: "v1" }).first().click();
  await expect(page.locator(".component-list li")).toHaveCount(1);
  await expect(page.locator(".header .project")).toContainText("v1");

  // 显示模式切换不崩
  await page.getByRole("button", { name: "线框" }).click();
  await page.getByRole("button", { name: "仅边" }).click();
  await expect(page.locator("canvas")).toBeVisible();
});
