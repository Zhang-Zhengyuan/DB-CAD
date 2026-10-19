// 成果展示截图：真浏览器打开 /web，加载 demo 项目，截主视图与仅边模式。
// 用法：node scripts/showcase.cjs [项目名，默认 demo-scene]
const { chromium } = require("@playwright/test");
const path = require("node:path");

(async () => {
  const projectName = process.argv[2] ?? "demo-scene";
  const browser = await chromium.launch();
  const page = await browser.newPage({ viewport: { width: 1600, height: 950 } });
  await page.goto("http://127.0.0.1:8000/web/");
  await page.waitForTimeout(1500);
  const item = page.getByRole("listitem").filter({ hasText: projectName });
  await item.click();
  await page.waitForSelector(".component-list li", { timeout: 20000 });
  await page.waitForTimeout(2500); // 等 GLB 解析
  await page.getByRole("button", { name: "等轴测" }).click();
  await page.waitForTimeout(700);

  const out = (name) => path.join(__dirname, "..", "test-results", name);
  await page.screenshot({ path: out("showcase-iso.png") });
  console.log("saved showcase-iso.png");

  // 选中一个组件展示属性面板 + 高亮
  await page.locator(".component-list li").nth(2).click();
  await page.waitForTimeout(500);
  await page.screenshot({ path: out("showcase-select.png") });
  console.log("saved showcase-select.png");

  // 仅边模式
  await page.getByRole("button", { name: "仅边" }).click();
  await page.waitForTimeout(500);
  await page.screenshot({ path: out("showcase-edges.png") });
  console.log("saved showcase-edges.png");

  await browser.close();
})();
