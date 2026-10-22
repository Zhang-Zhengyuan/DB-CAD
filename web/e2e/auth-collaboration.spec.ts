import { expect, test, type Page } from "@playwright/test";
import { readFile } from "node:fs/promises";

const password = "dbcad-e2e-access";

async function login(page: Page) {
  await page.goto("/web/");
  await page.getByLabel("访问密码").fill(password);
  await page.getByRole("button", { name: "连接", exact: true }).click();
  await expect(page.locator(".header")).toBeVisible();
}

test("密码登录、模型资产、导入导出与双会话同步", async ({ page: a, browser, baseURL }) => {
  expect((await a.request.get("/api/v1/projects")).status()).toBe(401);
  await a.goto("/web/");
  await a.getByLabel("访问密码").fill("incorrect");
  await a.getByRole("button", { name: "连接", exact: true }).click();
  await expect(a.getByRole("alert")).toContainText("请输入正确的访问密码");
  const context = await browser.newContext({ baseURL });
  try {
    const b = await context.newPage();
    await Promise.all([login(a), login(b)]);
    const name = `collaboration-${Date.now()}`;
    await a.getByLabel("新项目名称").fill(name);
    await a.getByRole("button", { name: "创建项目", exact: true }).click();
    await expect(a.locator(".header .project")).toContainText(name);
    await b.getByRole("button", { name: "刷新列表", exact: true }).click();
    await b.locator(".project-list li").filter({ hasText: name }).click();
    await expect(a.locator(".presence .user-chip")).toHaveCount(2);
    await expect(b.locator(".presence .user-chip")).toHaveCount(2);
    const asset = a.waitForResponse((response) => response.url().endsWith(".glb") && response.ok());
    await a.getByRole("button", { name: "插入", exact: true }).click();
    await expect(b.locator(".header .project")).toContainText("v1");
    expect((await asset).request().headers()["x-api-password"]).toBe(password);
    await b.locator(".insert-form select").selectOption("sphere");
    await b.getByRole("button", { name: "插入", exact: true }).click();
    await expect(a.locator(".header .project")).toContainText("v2");
    await expect(a.locator(".component-list li")).toHaveCount(2);
    const downloaded = a.waitForEvent("download");
    await a.getByRole("button", { name: "⬇ SAT", exact: true }).click();
    const sat = await downloaded;
    expect(sat.suggestedFilename()).toMatch(/\.sat$/);
    expect(await sat.failure()).toBeNull();
    const buffer = await readFile(await sat.path());
    expect(buffer.length).toBeGreaterThan(0);
    await a.locator("input[type=file]").setInputFiles({ name: "roundtrip.sat", mimeType: "application/octet-stream", buffer });
    await expect(a.locator(".header .project")).toContainText("v3");
    await expect(b.locator(".header .project")).toContainText("v3");
    await a.reload();
    await expect(a.locator(".header")).toBeVisible();
  } finally {
    await context.close();
  }
});
