#pragma once
/**
 * entity_graph_serializer.h
 *
 * 将 ACIS 实体层级结构（BODY → LUMP → SHELL → FACE → LOOP → COEDGE → EDGE → VERTEX
 * 以及 SURFACE / CURVE / PCURVE 等几何元素）序列化为 JSON，
 * 由 Python json_graph_store.py 按组件保存，顶级节点携带 SAT 备份。
 *
 * JSON 格式：
 * {
 *   "nodes": [
 *     {
 *       "id": "<uuid>",
 *       "labels": ["body"],
 *       "props": {
 *         "uuid": "<uuid>",
 *         "entity_type": "body",
 *         "transform": { "affine": [9 floats], "translation": [3], "scaling": 1.0, "rotate": 0, "reflect": 0, "shear": 0 }
 *       }
 *     },
 *     {
 *       "id": "<generated-id>",
 *       "labels": ["face"],
 *       "props": {
 *         "entity_type": "face",
 *         "sense": 1,
 *         "sides": 1,
 *         "cont": 0,
 *         "geometry": {
 *           "type": "plane",   // plane | sphere | torus | cone | spline | spl_sur | ...
 *           "root_point": [3],
 *           "normal": [3],
 *           "u_deriv": [3],
 *           "uv_range": [4],
 *           "reverse_v": 0
 *         }
 *       }
 *     }
 *   ],
 *   "rels": [
 *     { "type": "body_lump", "start": "<body-uuid>", "end": "<lump-id>", "props": {} }
 *   ]
 * }
 *
 * 反序列化时复用单机几何/拓扑编码重建 ACIS 实体，失败时使用同组件 SAT，
 * 然后将实体指针映射回 ETI uuid（通过 props.uuid 字段）。
 */

#include <QJsonObject>
#include <QJsonArray>
#include <QString>
#include <QHash>

// 前向声明
class ENTITY_LIST;
class ENTITY;
class Window;

/**
 * ACIS 实体图序列化器
 *
 * serializeACISEntityGraph():
 *   - 遍历 Window 中的 ACIS ENTITY_LIST
 *   - 按 ETI index → uuid 映射将顶级 body 的 uuid 注入到 props
 *   - BFS 遍历 ACIS 拓扑，生成 nodes + rels JSON
 *   - 每个顶级组件写入 sat_fragment 与 fallback 策略
 *
 * deserializeACISEntityGraph():
 *   - 解析 JSON nodes + rels
 *   - 按 UUID 分组件重建，全部成功才返回结果
 *   - 返回重建后的 ENTITY_LIST 以及 uuid → ENTITY* 映射
 */

// 序列化：Window → JSON
// entityIndexToUuid 映射 topLevelEntities 的紧凑位置 → 稳定 uuid（不是 UI 逻辑索引）
QJsonObject serializeACISEntityGraph(
    const QHash<int, QString>& entityIndexToUuid,
    const ENTITY_LIST& topLevelEntities
);

// 反序列化：JSON → ENTITY_LIST + uuid 映射
// 返回 { entities, uuidToEntity }
// uuidToEntity: key = 顶级节点 id/uuid, value = ENTITY* 指针
struct DeserializedEntityGraph {
    ENTITY_LIST* entities;
    QHash<QString, void*> uuidToEntity;
};

bool deserializeACISEntityGraph(
    const QJsonObject& graphJson,
    DeserializedEntityGraph* outResult,
    QString* errorMessage
);
