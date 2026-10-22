// 3D 查看器：GLB 组件渲染 + 显示模式 + 标准视图 + 点选高亮 + 变换 gizmo。
// GLB 布局契约（kernel/glb_writer.h）：primitives[0]=TRIANGLES(mesh)，
// [1]=LINES(LineSegments)——边线由内核 facet 产出，非前端 EdgesGeometry。
// ACIS 坐标系 z-up：Canvas 相机 up 设 [0,0,1]。
// gizmo（服务端权威）：拖拽只是本地预览，松手把增量发 POST /commands，
// 服务端烘焙几何后刷新回放——本地几何永远只读（ADR-1）。
import { useEffect, useMemo, useRef } from "react";
import { Canvas, useLoader, useThree } from "@react-three/fiber";
import { Html, OrbitControls, TransformControls } from "@react-three/drei";
import { GLTFLoader } from "three/addons/loaders/GLTFLoader.js";
import * as THREE from "three";
import { api, authHeaders, type SceneComponent } from "./api";
import { useViewer } from "./store";

// 组件组注册表：TransformControls 需要跨组件拿到主选组的 Object3D。
const componentGroups = new Map<string, THREE.Object3D>();

function ComponentModel({ component }: { component: SceneComponent }) {
  // P9：无面体/不可离散组件降级为无 GLB（mesh_error）——只入树不渲染。
  if (!component.glb) return null;
  const url = api.assetUrl(component.glb, "glb");
  const gltf = useLoader(GLTFLoader, url, (loader) => loader.setRequestHeader(authHeaders()));
  const isSelected = useViewer((s) => s.selected.includes(component.component_id));
  const intents = useViewer((s) => s.intents);
  const editingBy = useMemo(
    () => Object.entries(intents).find(([, id]) => id === component.component_id)?.[0],
    [intents, component.component_id],
  );
  const mode = useViewer((s) => s.displayMode);
  const select = useViewer((s) => s.select);
  const group = useRef<THREE.Group>(null);

  const meshes = useMemo(() => {
    const solid: THREE.Mesh[] = [];
    const edges: THREE.Line[] = [];
    gltf.scene.traverse((object) => {
      if ((object as THREE.Mesh).isMesh) solid.push(object as THREE.Mesh);
      else if ((object as THREE.Line).isLine) edges.push(object as THREE.Line);
    });
    return { solid, edges };
  }, [gltf]);

  const baseColor = useMemo(
    () => new THREE.Color(component.color[0], component.color[1], component.color[2]),
    [component.color],
  );

  const section = useViewer((s) => s.section);
  const sectionPlane = useMemo(() => {
    if (section.axis === "off") return null;
    const normal = new THREE.Vector3(
      section.axis === "x" ? -1 : 0,
      section.axis === "y" ? -1 : 0,
      section.axis === "z" ? -1 : 0,
    );
    return new THREE.Plane(normal, section.position);
  }, [section]);

  useEffect(() => {
    const showSolid = mode === "shaded" || mode === "shadedEdges" || mode === "wireframe";
    const showEdges = mode === "shadedEdges" || mode === "edges";
    const tint = editingBy ? new THREE.Color(0xd0a020) : baseColor;
    for (const mesh of meshes.solid) {
      mesh.visible = showSolid;
      mesh.material = new THREE.MeshStandardMaterial({
        color: tint,
        wireframe: mode === "wireframe",
        metalness: 0.15,
        roughness: 0.55,
        emissive: isSelected ? 0x2a5fbf : 0x000000,
        emissiveIntensity: isSelected ? 0.45 : 0,
        side: THREE.DoubleSide,
        clippingPlanes: sectionPlane ? [sectionPlane] : [],
      });
    }
    for (const edge of meshes.edges) {
      edge.visible = showEdges;
      edge.material = new THREE.LineBasicMaterial({
        color: isSelected ? 0x4d8bff : 0x333333,
        clippingPlanes: sectionPlane ? [sectionPlane] : [],
      });
    }
  }, [mode, isSelected, editingBy, meshes, baseColor, sectionPlane]);

  useEffect(() => {
    const node = group.current;
    if (!node) return;
    componentGroups.set(component.component_id, node);
    return () => {
      componentGroups.delete(component.component_id);
    };
  }, [component.component_id]);

  const measureMode = useViewer((s) => s.measureMode);
  const addMeasurePoint = useViewer((s) => s.addMeasurePoint);

  return (
    <group
      ref={group}
      onClick={(event) => {
        event.stopPropagation();
        if (measureMode) {
          addMeasurePoint([event.point.x, event.point.y, event.point.z]);
          return;
        }
        select(isSelected ? null : component.component_id);
      }}
    >
      <primitive object={gltf.scene} />
    </group>
  );
}

// 测量覆盖物：两个点标记 + 连线（drei Html 显示距离）。
function MeasureOverlay() {
  const points = useViewer((s) => s.measurePoints);
  if (points.length === 0) return null;
  const a = new THREE.Vector3(points[0][0], points[0][1], points[0][2]);
  const b = points.length > 1 ? new THREE.Vector3(points[1][0], points[1][1], points[1][2]) : null;
  const distance = b ? a.distanceTo(b) : null;
  const tuple = (v: THREE.Vector3): [number, number, number] => [v.x, v.y, v.z];
  return (
    <group>
      {points.map((point, index) => (
        <mesh key={index} position={[point[0], point[1], point[2]]}>
          <sphereGeometry args={[Math.max((distance ?? 1) * 0.02, 0.15), 12, 12]} />
          <meshBasicMaterial color={0x4d8bff} />
        </mesh>
      ))}
      {b && (
        <>
          <line>
            <bufferGeometry>
              <bufferAttribute
                attach="attributes-position"
                args={[new Float32Array([a.x, a.y, a.z, b.x, b.y, b.z]), 3]}
              />
            </bufferGeometry>
            <lineBasicMaterial color={0x4d8bff} />
          </line>
          <Html position={tuple(a.clone().add(b).multiplyScalar(0.5))} center>
            <span className="measure-label">{distance?.toFixed(3)}</span>
          </Html>
        </>
      )}
    </group>
  );
}

function Gizmo() {
  const gizmoMode = useViewer((s) => s.gizmoMode);
  const primary = useViewer((s) => s.selected[0]);
  const runCommand = useViewer((s) => s.runCommand);
  const target = primary ? componentGroups.get(primary) : undefined;

  if (gizmoMode === "off" || !target || !primary) return null;

  const releaseTransform = () => {
    // 计算增量并发命令；失败则回退本地预览（几何以服务端为准）。
    const position = target.position;
    const quaternion = target.quaternion;
    const scale = target.scale;
    const moved =
      Math.abs(position.x) + Math.abs(position.y) + Math.abs(position.z) > 1e-6 ||
      Math.abs(quaternion.w - 1) > 1e-6 ||
      Math.abs(scale.x - 1) + Math.abs(scale.y - 1) + Math.abs(scale.z - 1) > 1e-6;
    const reset = () => {
      target.position.set(0, 0, 0);
      target.quaternion.identity();
      target.scale.set(1, 1, 1);
    };
    if (!moved) return;
    if (gizmoMode === "translate") {
      void runCommand({
        action: "component.translate",
        params: { component: primary, offset: [position.x, position.y, position.z] },
      }).then((ok) => !ok && reset());
    } else if (gizmoMode === "rotate") {
      const angle = 2 * Math.acos(Math.min(1, Math.max(-1, quaternion.w)));
      const sinHalf = Math.sqrt(Math.max(0, 1 - quaternion.w * quaternion.w));
      const axis =
        sinHalf > 1e-9
          ? [quaternion.x / sinHalf, quaternion.y / sinHalf, quaternion.z / sinHalf]
          : [0, 0, 1];
      void runCommand({
        action: "component.rotate",
        params: { component: primary, axis, angle_degrees: (angle * 180) / Math.PI },
      }).then((ok) => !ok && reset());
    } else {
      const factor = (scale.x + scale.y + scale.z) / 3;
      void runCommand({
        action: "component.scale",
        params: { component: primary, factor },
      }).then((ok) => !ok && reset());
    }
  };

  return (
    <TransformControls object={target} mode={gizmoMode} size={0.8} onMouseUp={releaseTransform} />
  );
}

function CameraRig({ center, radius }: { center: [number, number, number]; radius: number }) {
  const { camera, controls } = useThree();
  const request = useViewer((s) => s.viewRequest);
  const applyView = (view: string) => {
    const d = Math.max(radius * 2.4, 4);
    const dirs: Record<string, [number, number, number]> = {
      front: [0, -d, 0],
      back: [0, d, 0],
      left: [-d, 0, 0],
      right: [d, 0, 0],
      top: [0, -0.001 * d, d],
      bottom: [0, -0.001 * d, -d],
      iso: [d * 0.8, -d * 0.7, d * 0.7],
      fit: [d * 0.8, -d * 0.7, d * 0.7],
    };
    const dir = dirs[view] ?? dirs.iso;
    camera.position.set(center[0] + dir[0], center[1] + dir[1], center[2] + dir[2]);
    camera.up.set(0, 0, 1);
    camera.lookAt(...center);
    const orbit = controls as unknown as { target?: THREE.Vector3; update?: () => void } | null;
    if (orbit?.target) {
      orbit.target.set(...center);
      orbit.update?.();
    }
  };
  // 首帧自动等轴测对中：初始相机默认朝 -Z 平视，场景中心会落在视锥外。
  useEffect(() => {
    applyView("iso");
    // eslint-disable-next-line react-hooks/exhaustive-deps
  }, []);
  useEffect(() => {
    if (request) applyView(request.view);
  }, [request, camera, controls, center, radius]);
  return null;
}

export function Viewer() {
  const scene = useViewer((s) => s.scene);
  const select = useViewer((s) => s.select);

  const bounds = useMemo(() => {
    if (!scene || scene.components.length === 0)
      return { center: [0, 0, 0] as [number, number, number], radius: 10 };
    const box = new THREE.Box3();
    for (const component of scene.components) {
      box.union(
        new THREE.Box3(
          new THREE.Vector3(...component.bbox.min),
          new THREE.Vector3(...component.bbox.max),
        ),
      );
    }
    const size = box.getSize(new THREE.Vector3());
    const center = box.getCenter(new THREE.Vector3());
    return {
      center: [center.x, center.y, center.z] as [number, number, number],
      radius: Math.max(size.length() / 2, 1),
    };
  }, [scene]);

  return (
    <Canvas
      className="canvas-host"
      gl={{ localClippingEnabled: true }}
      camera={{
        position: [bounds.radius * 2, -bounds.radius * 2, bounds.radius * 1.7],
        fov: 45,
        near: 0.01,
        far: 100000,
        up: [0, 0, 1],
      }}
      onPointerMissed={() => select(null)}
    >
      <color attach="background" args={["#1c2126"]} />
      <hemisphereLight intensity={0.55} groundColor="#2a2f35" />
      <directionalLight position={[50, -80, 120]} intensity={1.1} />
      <directionalLight position={[-60, 40, -60]} intensity={0.35} />
      {scene?.components.map((component) => (
        <ComponentModel key={component.component_id} component={component} />
      ))}
      <Gizmo />
      <MeasureOverlay />
      <CameraRig center={bounds.center} radius={bounds.radius} />
      <OrbitControls makeDefault />
      <gridHelper args={[200, 40, 0x3a4148, 0x262b30]} position={[0, 0, 0]} />
    </Canvas>
  );
}
