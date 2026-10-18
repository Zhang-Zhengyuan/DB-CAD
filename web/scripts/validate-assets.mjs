// 组件库 GLB 全量校验：用 three 的 GLTFLoader（前端真实消费路径）解析
// 内容寻址库里的每个 .glb。GLB 写入器（kernel/glb_writer）的回归在此把关
// （曾漏掉 accessor 越界引用，浏览器才暴露——见 mesh_tests 交叉引用校验）。
//
//   node scripts/validate-assets.mjs [组件库根目录，默认 ../../CAD_DB/backend/data/components]
import { readdir, readFile } from "node:fs/promises";
import { join } from "node:path";
import { createRequire } from "node:module";

const require = createRequire(import.meta.url);
const THREE = require("three");
const { GLTFLoader } = require("three/examples/jsm/loaders/GLTFLoader.js");

const root = process.argv[2] ?? join(import.meta.dirname, "..", "..", "CAD_DB", "backend", "data", "components");

async function walk(dir) {
  const entries = await readdir(dir, { withFileTypes: true });
  const nested = await Promise.all(
    entries.map((entry) =>
      entry.isDirectory() ? walk(join(dir, entry.name)) : Promise.resolve([join(dir, entry.name)]),
    ),
  );
  return nested.flat();
}

const files = (await walk(root)).filter((name) => name.endsWith(".glb"));
if (files.length === 0) {
  console.log(`no .glb assets under ${root}`);
  process.exit(0);
}

let failures = 0;
for (const file of files) {
  const buffer = await readFile(file);
  const arrayBuffer = buffer.buffer.slice(buffer.byteOffset, buffer.byteOffset + buffer.byteLength);
  await new Promise((resolve) => {
    new GLTFLoader().parse(
      arrayBuffer,
      "",
      (gltf) => {
        let meshes = 0;
        let lines = 0;
        gltf.scene.traverse((object) => {
          if (object.isMesh) meshes++;
          if (object.isLine) lines++;
        });
        console.log(`OK   ${file.split(/[\\/]/).pop()} meshes=${meshes} lines=${lines}`);
        resolve();
      },
      (error) => {
        failures++;
        console.log(`FAIL ${file}: ${error.message}`);
        resolve();
      },
    );
  });
}
console.log(`${files.length - failures}/${files.length} assets valid`);
process.exit(failures ? 1 : 0);
