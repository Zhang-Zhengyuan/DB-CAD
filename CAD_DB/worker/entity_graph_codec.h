#pragma once
// v3 实体图投影 codec（V3_PLAN.md P6 / ARCHITECTURE.md ADR-3）。
//
// 从 v2 entity_graph_serializer（uvsafe-topology-v5-bounded）Qt-free 移植：
// BFS 遍历 ACIS 拓扑 → nodes + rels JSON，几何载荷与保真风险判定逐 case
// 保真（几何字段、区间编码、NURBS RLE、rot_spl_sur 过程载荷等全部照搬）。
//
// 与 v2 的有意差异（投影场景裁剪，非语义漂移）：
//   - 不内嵌 sat_fragment：v3 组件 SAT 以内容寻址资产独立存在（component
//     库 sat kind），投影文件只承载结构与派生标记；v2 的保真回退语义转为
//     root props 的 fallback 标记（"fine_grained" / "sat_asset"），供投影
//     消费方诚实判定细粒度覆盖率。
//   - root 节点 id/uuid = 组件 sid（v2 为客户端 ETI uuid 记账表）。
#include <string>

#include "vendor/nlohmann/json.hpp"

class ENTITY;

namespace dbcad::worker {

// 序列化单实体（组件）的实体图为投影 JSON：
//   {"schema":"dbcad.entity_graph.v2","serializer_build":"...",
//    "nodes":[{"id","labels","props"}...],"rels":[{"type","start","end"}...]}
// root props 额外携带 top_level/uuid/fallback/topology_node_count/
// topology_rel_count。失败抛 std::runtime_error（错误显式，不静默兜底）。
nlohmann::json serializeEntityGraph(const std::string& componentSid, ENTITY& root);

}  // namespace dbcad::worker
