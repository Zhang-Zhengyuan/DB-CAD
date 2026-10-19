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
#include <unordered_map>

#include "vendor/nlohmann/json.hpp"

class ENTITY;

namespace dbcad::worker {

// 序列化单实体（组件）的实体图为投影 JSON：
//   {"schema":"dbcad.entity_graph.v2","serializer_build":"...",
//    "nodes":[{"id","labels","props"}...],"rels":[{"type","start","end"}...]}
// root props 额外携带 top_level/uuid/fallback/topology_node_count/
// topology_rel_count。失败抛 std::runtime_error（错误显式，不静默兜底）。
//
// prevIds（P8/Phase D 稳定实体身份）：命令前组件闭包的
// 实体指针 → 该实体在命令前投影中的 node id。序列化时同指针同类型
// 的节点携带 props.prev_id（generation 链，供网关连 DERIVED_FROM）。
nlohmann::json serializeEntityGraph(
    const std::string& componentSid,
    ENTITY& root,
    const std::unordered_map<const void*, std::string>* prevIds = nullptr);

// 拓扑闭包采集（P8.1）：BFS 枚举 root 可达的全部拓扑实体，
// 输出 指针 → node id（genNodeId 语义，与 serializeEntityGraph 一致）。
// 命令前采集、命令后随新投影比对，即得跨命令实体身份。
// 注意：children 枚举与 serializeEntityGraph 主 BFS 保持同步
// （selftest 断言 closure 数 == graph 节点数 守门）。
void collectTopologyClosure(ENTITY& root, const std::string& componentSid,
                            std::unordered_map<const void*, std::string>& out);

}  // namespace dbcad::worker
