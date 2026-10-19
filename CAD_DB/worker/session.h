#pragma once
// v3 常驻几何会话（P2 命令引擎的 worker 侧，V3_PLAN.md ADR-1/ADR-2）。
//
// 会话 = 一个项目的权威几何状态：open 恢复组件 SAT，exec 逐条执行建模命令，
// 变更组件重新发射（SAT+GLB+元数据）到 spool 目录由网关内容寻址入库。
// 进程内单线程（ACIS modeller 进程级状态），调用方（main.cpp serve 循环）
// 天然串行——命令全序即版本全序。
//
// 协议 JSON 见 main.cpp serve 模式；本类只管几何语义。
// sid 约定：open 传入的组件 sid 沿用网关的 component_id（= SAT 内容哈希）；
// 新建组件 sid 由会话分配（n0, n1…），网关以发射的新 SAT 哈希为准重编。
#include <stdexcept>
#include <string>
#include <utility>
#include <vector>

class ENTITY;

namespace dbcad::worker {

struct ComponentRef {
    std::string sid;
    ENTITY* entity = nullptr;
    std::string kind;  // 创建时的图元名（导入组件为空）
};

class CommandError : public std::runtime_error {
public:
    explicit CommandError(const std::string& message) : std::runtime_error(message) {}
};

class GeometrySession {
public:
    explicit GeometrySession(std::string spoolDir);
    ~GeometrySession();

    // components: (sid, sat 文件名) 列表，文件位于 spool 目录。
    // 返回空串=成功，否则错误信息（会话保持空态）。
    std::string open(const std::vector<std::pair<std::string, std::string>>& components);

    // 执行一条命令，返回 JSON 结果对象：
    //   {"added":[emit...], "modified":[emit...], "removed":[sid...]}
    // emit = {"sid","kind","sat","glb","graph","graph_nodes","graph_fallback",
    //         "bbox":{"min","max"},"color","triangles","vertices"}
    //   graph = spool 内 {sid}.graph.json 实体图投影（P6/ADR-3）
    // 失败抛 CommandError（命令原子性尽力而为：失败后网关应重建会话）。
    std::string exec(const std::string& commandJson);

    bool opened() const { return opened_; }

private:
    std::string spoolDir_;
    std::vector<ComponentRef> components_;
    bool opened_ = false;
    int nextNewSid_ = 0;

    ComponentRef* find(const std::string& sid);
    std::string emitComponent(ComponentRef& component, bool added);
};

}  // namespace dbcad::worker
