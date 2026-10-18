// 核心库单元测试（架构 v2，REFACTOR_PLAN M1.2/1.3/1.4/1.5/1.6 验收产物）。
//
// 无 UI、无真实网络：DocumentSession 用 FakeGeometryKernel；CollabBackend 用
// MemoryTransport（内存实现 json-delta 线协议语义，含 base_version 乐观锁 409）。
// 覆盖：变更记账、组件替换、防回环标记、级联删除、状态机、推/拉协议、
// StorageBackend（SAT 文件 + 协作后端同一接口）、CommandBus 注册表。
#include "core/collab_backend.h"
#include "core/collab_controller.h"
#include "core/collab_state.h"
#include "core/command_bus.h"
#include "core/document_session.h"
#include "core/sat_file_backend.h"

#include <QCoreApplication>
#include <QDir>
#include <QFile>
#include <QJsonArray>
#include <QJsonDocument>
#include <QJsonObject>
#include <QTemporaryDir>
#include <QUrlQuery>

#include <cstdio>
#include <map>
#include <set>
#include <stdexcept>

using namespace dbcad;

static dbcad::HandleGrid emptyGrid; // FakeKernel::create 不修改句柄网格，可复用左值
static int checks = 0;
static void expect(bool value, const char* message) {
    if (!value)
        throw std::runtime_error(message);
    ++checks;
    printf("PASS %s\n", message);
    fflush(stdout);
}

// ---------------------------------------------------------------- FakeKernel
class FakeKernel : public GeometryKernel {
public:
    struct ShapeInfo {
        int primitive = -1;
        bool draft = false;
    };
    std::map<std::uint64_t, ShapeInfo> info;
    std::set<std::uint64_t> erased;
    std::uint64_t next = 0;

    ShapeHandle make(int primitive) {
        const std::uint64_t value = ++next;
        info[value] = {primitive, false};
        return ShapeHandle{value};
    }

    ShapeHandle create(int primitive, HandleGrid& handles) override {
        if (primitive == E_BSPLINE && handles.empty())
            throw std::runtime_error("empty spline handles");
        return make(primitive);
    }
    ShapeHandle draft() override {
        const std::uint64_t value = ++next;
        info[value] = {-1, true};
        return ShapeHandle{value};
    }
    ShapeList operate(OPERATOR_TYPES, int, const ShapeList& inputs) override {
        if (inputs.empty())
            throw std::runtime_error("no inputs");
        return {make(-1)};
    }
    ShapeKind kind(ShapeHandle shape) const override {
        if (erased.count(shape.value))
            throw std::runtime_error("deleted handle rejected");
        const auto it = info.find(shape.value);
        if (it == info.end())
            throw std::runtime_error("unknown handle");
        return it->second.draft ? ShapeKind::Unknown : ShapeKind::Body;
    }
    void translate(ShapeHandle, Translation, bool) override {}
    void copyColor(ShapeHandle, ShapeHandle) override {}
    void erase(const ShapeList& shapes) override {
        for (auto shape : shapes) {
            if (!shape)
                continue;
            erased.insert(shape.value);
            info.erase(shape.value);
        }
    }
    bool mesh(ShapeHandle, GmeMesh::DisplayData&) override { return true; }
    QByteArray writeNative(const ShapeList& shapes) override {
        QByteArray data;
        for (auto shape : shapes)
            data += QByteArray::number(static_cast<qulonglong>(shape.value)) + ',';
        return data;
    }
    ShapeList readNative(const QByteArray& data) override {
        ShapeList shapes;
        for (const auto& token : data.split(',')) {
            if (token.isEmpty())
                continue;
            bool ok = false;
            const std::uint64_t value = token.toULongLong(&ok);
            if (!ok)
                throw std::runtime_error("bad native payload");
            const ShapeHandle restored = make(-1);
            info[restored.value].primitive = static_cast<int>(value);
            shapes.push_back(restored);
        }
        return shapes;
    }
    QJsonObject encodeGraph(const QHash<int, QString>& uuids, const ShapeList& shapes) override {
        QJsonArray nodes;
        for (int i = 0; i < shapes.size(); ++i) {
            if (info.count(shapes[i].value) && info[shapes[i].value].draft)
                throw std::runtime_error("unfinished draft rejected");
            QJsonObject props;
            props.insert("top_level", true);
            props.insert("sat_fragment", QString::number(static_cast<qulonglong>(shapes[i].value)));
            QJsonObject node;
            node.insert("id", uuids.value(i));
            node.insert("labels", QJsonArray{"body"});
            node.insert("props", props);
            nodes.append(node);
        }
        QJsonObject graph;
        graph.insert("nodes", nodes);
        graph.insert("rels", QJsonArray{});
        return graph;
    }
    bool decodeGraph(const QJsonObject& graph, DecodedGraph& result, QString& error) override {
        DecodedGraph decoded;
        for (const auto& value : graph.value("nodes").toArray()) {
            const QJsonObject node = value.toObject();
            const QString id = node.value("id").toString();
            const QString sat = node.value("props").toObject().value("sat_fragment").toString();
            bool ok = false;
            const std::uint64_t handle = sat.toULongLong(&ok);
            if (sat.isEmpty() || !ok) {
                error = "invalid sat fragment";
                return false;
            }
            const ShapeHandle restored = make(-1);
            info[restored.value].primitive = static_cast<int>(handle);
            decoded.shapes.insert(id, restored);
        }
        result = decoded;
        return true;
    }
    void noteState() override {}
    void dump(ShapeHandle, const std::string&, FILE*, bool) override {}
    void statistics(ShapeHandle) override {}
};

// ------------------------------------------------------------ MemoryTransport
// json-delta 线协议的内存服务器（与 backend memory_store 语义对齐）。
class MemoryTransport : public HttpTransport {
public:
    struct Project {
        QString id;
        QString name;
        int version = 0;
        std::map<int, QJsonObject> versionGraphs; // version → delta 响应体
    };
    std::map<QString, Project> projects;
    int nextId = 0;

    HttpResponse send(const HttpRequest& request) override {
        HttpResponse response;
        const QString path = request.url.path();
        const QUrlQuery query(request.url);

        if (request.method == "GET" && path.startsWith("/projects/by-name/")) {
            const QString name = QUrl::fromPercentEncoding(path.mid(QString("/projects/by-name/").size()).toUtf8());
            const auto it = findByName(name);
            if (it == projects.end()) {
                response.statusCode = 404;
            } else {
                response.statusCode = 200;
                response.body = projectJson(it->second);
            }
            return response;
        }
        if (request.method == "POST" && path == "/projects") {
            const QJsonObject payload = QJsonDocument::fromJson(request.payload).object();
            const QString name = payload.value("name").toString();
            if (findByName(name) != projects.end()) {
                response.statusCode = 409;
                return response;
            }
            const QString id = QStringLiteral("p-%1").arg(++nextId);
            projects[id] = Project{id, name, 0, {}};
            response.statusCode = 201;
            response.body = projectJson(projects[id]);
            return response;
        }
        if (request.method == "POST" && path.endsWith("/json-delta")) {
            const QString id = path.section('/', 2, 2);
            auto it = projects.find(id);
            if (it == projects.end()) {
                response.statusCode = 404;
                response.body = errorBody("project not found");
                return response;
            }
            const QJsonObject payload = QJsonDocument::fromJson(request.payload).object();
            const QJsonValue baseValue = payload.value("base_version");
            if (!baseValue.isNull() && baseValue.toInt() != it->second.version) {
                response.statusCode = 409;
                response.body = errorBody("base_version mismatch");
                return response;
            }
            ++it->second.version;
            QJsonObject body;
            body.insert("version", it->second.version);
            it->second.versionGraphs[it->second.version] = deltaBody(it->second, payload);
            response.statusCode = 201;
            response.body = QJsonDocument(body).toJson(QJsonDocument::Compact);
            return response;
        }
        if (request.method == "GET" && path.endsWith("/json-delta")) {
            const QString id = path.section('/', 2, 2);
            const auto it = projects.find(id);
            if (it == projects.end()) {
                response.statusCode = 404;
                response.body = errorBody("project not found");
                return response;
            }
            response.statusCode = 200;
            if (it->second.version == 0) {
                QJsonObject body;
                body.insert("version", 0);
                body.insert("entity_graph", QJsonObject{{"nodes", QJsonArray{}}, {"rels", QJsonArray{}}});
                body.insert("changes", QJsonObject{{"changes", QJsonArray{}}});
                body.insert("removed_ids", QJsonArray{});
                body.insert("delta", false);
                response.body = QJsonDocument(body).toJson(QJsonDocument::Compact);
            } else {
                response.body =
                    QJsonDocument(it->second.versionGraphs.rbegin()->second).toJson(QJsonDocument::Compact);
            }
            Q_UNUSED(query);
            return response;
        }
        response.statusCode = 404;
        response.body = errorBody("no such endpoint");
        return response;
    }

    int versionOf(const QString& projectId) const {
        const auto it = projects.find(projectId);
        return it == projects.end() ? -1 : it->second.version;
    }
    QString idOf(const QString& name) const {
        const auto it = findByName(name);
        return it == projects.end() ? QString() : it->second.id;
    }

private:
    static QByteArray errorBody(const QString& detail) {
        return QJsonDocument(QJsonObject{{"detail", detail}}).toJson(QJsonDocument::Compact);
    }
    static QByteArray projectJson(const Project& project) {
        return QJsonDocument(QJsonObject{{"id", project.id}, {"name", project.name}}).toJson(QJsonDocument::Compact);
    }
    static QJsonObject deltaBody(const Project& project, const QJsonObject& payload) {
        QJsonObject body;
        body.insert("version", project.version);
        body.insert("entity_graph", payload.value("entity_graph").toObject());
        body.insert("changes", payload.value("changes"));
        body.insert("removed_ids", payload.value("removed_ids"));
        body.insert("delta", payload.value("base_version").toInt() > 0);
        body.insert("graph_hash", "hash");
        return body;
    }
    std::map<QString, Project>::const_iterator findByName(const QString& name) const {
        for (auto it = projects.begin(); it != projects.end(); ++it)
            if (it->second.name == name)
                return it;
        return projects.end();
    }
};

// -------------------------------------------------------------------- helpers
static QJsonObject graphWith(FakeKernel& kernel, const std::vector<std::pair<QString, ShapeHandle>>& entries) {
    QHash<int, QString> uuids;
    ShapeList shapes;
    for (size_t i = 0; i < entries.size(); ++i) {
        uuids.insert(static_cast<int>(i), entries[i].first);
        shapes.push_back(entries[i].second);
    }
    return kernel.encodeGraph(uuids, shapes);
}

static const ENTITY_TREE_ITEM* findByUuid(const DocumentSession& session, const QString& uuid) {
    for (const auto& item : session.entities())
        if (QString::fromStdString(item.uuid) == uuid)
            return &item;
    return nullptr;
}

// ------------------------------------------------- M1.2 DocumentSession 单测
static void testChangeBookkeeping() {
    FakeKernel kernel;
    DocumentSession session(kernel);
    session.beginChangeTracking();

    auto shape = kernel.create(E_CUBE, emptyGrid);
    auto* row = session.addEntity(shape, "cube", E_CUBE);
    expect(row != nullptr, "addEntity returns row");
    expect(session.pendingChanges().size() == 1 && session.pendingChanges().first().changeType == EntityChangeType::ADD,
           "addEntity records ADD");
    expect(session.pendingChanges().first().uuid == QString::fromStdString(row->uuid), "ADD carries uuid");
    expect(session.pendingChanges().first().entityIndex == row->index, "ADD carries index");

    session.recordEntityModified(row->index);
    expect(session.pendingChanges().size() == 2 && session.pendingChanges().at(1).changeType == EntityChangeType::MODIFY,
           "recordEntityModified records MODIFY");

    const auto cascade = session.removeEntityCascade(row->index);
    expect(cascade.ok && cascade.removedIndices.size() == 1, "removeEntityCascade removes row");
    expect(session.pendingChanges().size() == 3 && session.pendingChanges().at(2).changeType == EntityChangeType::REMOVE,
           "cascade records REMOVE");

    // 追踪关闭时不记账。
    session.stopChangeTracking();
    const int before = session.pendingChanges().size();
    session.addEntity(kernel.create(E_SPHERE, emptyGrid), "sphere", E_SPHERE);
    session.recordEntityRemoved(12345);
    expect(session.pendingChanges().size() == before, "no bookkeeping after stopChangeTracking");

    // 导出格式：{changes:[...], trackingStartTime, exportTime}。
    session.beginChangeTracking();
    session.addEntity(kernel.create(E_CUBE, emptyGrid), "c2", E_CUBE);
    const QString json = session.exportEntityChangesJson(session.pendingChanges());
    const QJsonObject root = QJsonDocument::fromJson(json.toUtf8()).object();
    expect(root.contains("changes") && root.contains("trackingStartTime") && root.contains("exportTime"),
           "exportChangesJson shape");
    expect(root.value("changes").toArray().last().toObject().value("changeType").toString() == QStringLiteral("ADD"),
           "exported change type ADD");
}

static void testApplyRemoteReplacement() {
    FakeKernel kernel;
    DocumentSession session(kernel);
    session.beginChangeTracking();

    auto localA = kernel.create(E_CUBE, emptyGrid);
    const int indexA = session.addEntity(localA, "A", E_CUBE)->index;
    const QString uuidA = QString::fromStdString(session.itemByIndex(indexA)->uuid);
    auto localD = kernel.create(E_SPHERE, emptyGrid);
    const int indexD = session.addEntity(localD, "D", E_SPHERE, std::vector<int>{indexA}, OPERATOR_BOOLEAN)->index;
    expect(session.itemByIndex(indexD)->index_base == std::vector<int>{indexA}, "dependency recorded");
    expect(session.itemByIndex(indexA)->index_support == std::vector<int>{indexD}, "support recorded");
    session.discardPendingChanges();
    expect(session.pendingChanges().isEmpty(), "baseline cleared");

    // 远端 delta：替换 A（新几何），不含 D —— delta 模式保留本地 D。
    auto remoteShape = kernel.create(E_SPHERE, emptyGrid);
    const auto graph = graphWith(kernel, {{uuidA, remoteShape}});
    const auto apply = session.applyRemoteGraph(1, graph, true, {}, {{uuidA, "A2"}});
    expect(apply.ok, "applyRemoteGraph delta ok");
    expect(session.entities().size() == 2, "delta keeps local-only rows");
    expect(session.itemByIndex(indexA) != nullptr && session.itemByIndex(indexA)->shape != localA &&
               kernel.kind(session.itemByIndex(indexA)->shape) == ShapeKind::Body,
           "component replaced by uuid with fresh decoded shape, index preserved");
    expect(session.itemByIndex(indexA)->name == "A2", "remote display name applied");
    expect(session.itemByIndex(indexA)->operatorType == OPERATOR_IMPORT &&
               session.itemByIndex(indexA)->subOperatorType == -1 &&
               session.itemByIndex(indexA)->handles.empty(),
           "incoming geometry resets recipe to IMPORT");
    expect(session.applyingRemoteSnapshot() == false, "applying flag cleared after apply");
    expect(session.pendingChanges().isEmpty(), "anti-loop: apply records no local changes");

    // 全量快照：远端只有 A'，本地 D 被移除。
    auto remoteShape2 = kernel.create(E_CUBE, emptyGrid);
    const auto full = graphWith(kernel, {{uuidA, remoteShape2}});
    const auto apply2 = session.applyRemoteGraph(2, full, false, {}, {});
    expect(apply2.ok && session.entities().size() == 1, "full snapshot drops local-only rows");
    expect(session.itemByIndex(indexD) == nullptr, "dependent removed on full replace");
    expect(session.pendingChanges().isEmpty(), "full apply still records nothing");

    // 新组件追加：单调索引。
    const auto withNew = graphWith(kernel, {{uuidA, remoteShape2},
                                            {QStringLiteral("brand-new"), kernel.create(E_SPHERE, emptyGrid)}});
    const auto apply3 = session.applyRemoteGraph(3, withNew, true, {}, {});
    expect(apply3.ok && session.entities().size() == 2, "incoming component appended");
    const auto* appended = session.itemByIndex(session.nextIndex() - 1);
    expect(appended != nullptr && QString::fromStdString(appended->uuid) == QStringLiteral("brand-new"),
           "appended uses fresh monotonic index");

    // 坏图：解码失败不动文档。
    QJsonObject broken;
    broken.insert("nodes", QJsonArray{QJsonObject{{"id", "bad"},
                                                  {"labels", QJsonArray{"body"}},
                                                  {"props", QJsonObject{{"top_level", true}, {"sat_fragment", "junk"}}}}});
    broken.insert("rels", QJsonArray{});
    const auto sizeBefore = session.entities().size();
    const auto apply4 = session.applyRemoteGraph(4, broken, true, {}, {});
    expect(!apply4.ok && session.entities().size() == sizeBefore, "failed decode preserves document");
}

static void testCascadeAndRebuild() {
    FakeKernel kernel;
    DocumentSession session(kernel);
    session.beginChangeTracking();
    QStringList messages;
    session.setMessageSink([&messages](const QString& message) { messages << message; });

    const int baseIndex = session.addEntity(kernel.create(E_CUBE, emptyGrid), "base", E_CUBE)->index;
    const auto baseShape = session.itemByIndex(baseIndex)->shape;
    const int midIndex =
        session.addEntity(kernel.create(E_SPHERE, emptyGrid), "mid", E_SPHERE, {baseIndex}, OPERATOR_BOOLEAN)->index;
    const auto midShape = session.itemByIndex(midIndex)->shape;
    const int leafIndex =
        session.addEntity(kernel.create(E_TORUS, emptyGrid), "leaf", E_TORUS, {midIndex}, OPERATOR_BOOLEAN)->index;
    const auto leafShape = session.itemByIndex(leafIndex)->shape;
    session.discardPendingChanges();

    auto cascade = session.removeEntityCascade(baseIndex);
    expect(cascade.ok && cascade.removedIndices.size() == 3, "cascade removes transitive dependents");
    expect(session.entities().empty(), "tree empty after cascade");
    bool removeRecords = true;
    for (int index : cascade.removedIndices) {
        bool found = false;
        for (const auto& change : session.pendingChanges())
            if (change.changeType == EntityChangeType::REMOVE && change.entityIndex == index)
                found = true;
        removeRecords = removeRecords && found;
    }
    expect(removeRecords, "cascade records REMOVE per dependent");
    expect(kernel.erased.count(baseShape.value) && kernel.erased.count(midShape.value) &&
               kernel.erased.count(leafShape.value),
           "kernel shapes erased by cascade");

    // 重建：编辑 base 后下游重建、形状更新并记 MODIFY。
    const int parentIndex = session.addEntity(kernel.create(E_CUBE, emptyGrid), "parent", E_CUBE)->index;
    const int derivedIndex = session.addEntity(kernel.create(E_SPHERE, emptyGrid), "derived", DEFEATURE_UNREPEAT,
                                                {parentIndex}, OPERATOR_DEFEATURE)
                                  ->index;
    session.discardPendingChanges();
    const auto oldDerivedShape = session.itemByIndex(derivedIndex)->shape;
    std::vector<int> downstream{parentIndex};
    std::vector<GmeMesh::DisplayData*> retiredByRebuild;
    expect(session.rebuildDownstream(downstream, retiredByRebuild), "rebuildDownstream succeeds");
    expect(session.itemByIndex(derivedIndex) != nullptr &&
               session.itemByIndex(derivedIndex)->shape != oldDerivedShape,
           "dependent rebuilt with fresh shape");
    bool modifyRecorded = false;
    for (const auto& change : session.pendingChanges())
        if (change.changeType == EntityChangeType::MODIFY && change.entityIndex == derivedIndex)
            modifyRecorded = true;
    expect(modifyRecorded, "rebuild records MODIFY");

    // 循环依赖：A↔B。
    const int indexA = session.addEntity(kernel.create(E_CUBE, emptyGrid), "a", E_CUBE)->index;
    const int indexB = session.addEntity(kernel.create(E_CUBE, emptyGrid), "b", E_CUBE)->index;
    session.entitiesMutable().back().index_base.push_back(indexA);
    session.itemByIndex(indexA)->index_base.push_back(indexB);
    session.itemByIndex(indexA)->index_support.push_back(indexB);
    session.itemByIndex(indexB)->index_support.push_back(indexA);
    messages.clear();
    std::vector<int> seeds{indexA};
    std::vector<GmeMesh::DisplayData*> retiredByCycle;
    expect(!session.rebuildDownstream(seeds, retiredByCycle), "cycle detected");
    expect(messages.contains(QString::fromUtf8("检测到循环依赖，已停止重建")), "cycle message surfaced");
}

// --------------------------------------------------- M1.3 CollabState 单测
static void testStateMachine() {
    CollabState state;
    expect(state.state() == CollabState::State::Disconnected, "initial Disconnected");
    expect(state.tryBeginSubmit("x").kind == CollabState::SubmitDecision::RejectNoProject,
           "submit rejected without project");

    state.setProjectId("p1");
    state.onReconnect(true);
    expect(state.state() == CollabState::State::Connected_Idle, "reconnect → Connected_Idle");
    expect(state.isConnected(), "isConnected after reconnect");

    state.onUserEdit();
    expect(state.state() == CollabState::State::Connected_LocalDirty, "edit → LocalDirty");
    expect(state.isLocalDirty(), "isLocalDirty accessor");

    const auto allow = state.tryBeginSubmit("t");
    expect(allow.kind == CollabState::SubmitDecision::Allow, "submit allowed when dirty");
    expect(state.isSubmitInFlight(), "submit in flight");

    state.onUserEdit(); // in-flight 期间继续编辑
    expect(state.state() == CollabState::State::Connected_SubmitInFlight_Dirty, "edit during flight → dirty");
    expect(state.tryBeginSubmit("again").kind == CollabState::SubmitDecision::RejectAlreadyInFlight,
           "second submit rejected while in flight");

    state.onSubmitAccepted(1);
    expect(!state.isSubmitInFlight() && state.modelVersion() == 1, "accepted advances version");
    expect(state.state() == CollabState::State::Connected_LocalDirty, "dirty-during-submit survives accept");

    state.onRemotePending(2);
    expect(state.pendingRemoteVersion() == 2, "remote pending recorded");
    expect(state.state() == CollabState::State::Connected_RemotePending_Dirty, "remote pending state");
    expect(state.tryBeginSubmit("t").kind == CollabState::SubmitDecision::RejectRemotePending,
           "submit rejected while remote pending");

    state.onRemoteApplied(2);
    expect(state.modelVersion() == 2 && state.pendingRemoteVersion() == 0, "applied clears pending");
    expect(state.state() == CollabState::State::Connected_Idle, "back to idle after apply");

    // 回滚（无 in-flight 编辑）：回到 Idle。
    state.onUserEdit();
    state.tryBeginSubmit("t2");
    state.rollbackSubmit();
    expect(!state.isSubmitInFlight() && state.state() == CollabState::State::Connected_Idle,
           "rollback returns to Idle");

    // apply remote 期间的编辑被忽略（防回环）。
    state.onUserEdit();
    state.setApplyingRemoteSnapshot(true);
    state.onUserEdit();
    expect(state.state() == CollabState::State::Connected_LocalDirty, "edits during remote apply ignored");
    state.setApplyingRemoteSnapshot(false);

    state.onDisconnected();
    expect(state.state() == CollabState::State::Disconnected && !state.isSubmitInFlight(),
           "disconnect resets in-flight flags");

    // 切项目：整体归零。
    state.onReconnect(true);
    state.setModelVersion(7);
    state.setProjectId("p2");
    expect(state.modelVersion() == 0, "project switch resets snapshot");
}

// ------------------------------------- M1.4 协议测试（CollabBackend + 内存服务器）
static void testPushPullProtocol() {
    FakeKernel kernel;
    DocumentSession session(kernel);
    MemoryTransport* memory = new MemoryTransport;
    memory->projects["p-x"] = {"p-x", "proj", 0, {}};
    CollabBackend backend;
    backend.setTransport(memory);
    backend.configure("http://memory", "tester", "");
    CollabController controller(session, backend);

    controller.bindProject("p-x", "proj");
    controller.onChannelConnected();
    expect(controller.isConnected(), "controller connected");

    session.beginChangeTracking();

    // 无待提交 → nothing to do。
    auto nothing = controller.push("t");
    expect(nothing.ok && nothing.nothingToDo, "push with no pending changes is a no-op");

    // 首推 v1。
    const int cubeIndex = session.addEntity(kernel.create(E_CUBE, emptyGrid), "cube", E_CUBE)->index;
    const QString cubeUuid = QString::fromStdString(session.itemByIndex(cubeIndex)->uuid);
    auto pushed = controller.push("t");
    expect(pushed.ok && pushed.version == 1, "first push creates v1");
    expect(controller.modelVersion() == 1, "model version advanced");
    expect(session.pendingChanges().isEmpty(), "pending cleared after push");
    expect(memory->versionOf("p-x") == 1, "memory server stored v1");

    // 再推（同一实体修改）→ v2。
    session.recordEntityModified(cubeIndex);
    auto pushed2 = controller.push("t");
    expect(pushed2.ok && pushed2.version == 2, "second push advances v2");

    // 对端（peer）以合法基线推到 v3。
    CollabBackend peer;
    peer.setTransport(memory);
    peer.configure("http://memory", "peer", "");
    const auto peerGraph = QString::fromUtf8(
        QJsonDocument(graphWith(kernel, {{QStringLiteral("peer-body"), kernel.create(E_SPHERE, emptyGrid)}}))
            .toJson(QJsonDocument::Compact));
    const auto peerPush = peer.pushJsonDelta("p-x", "peer", peerGraph, "[]", {}, 2);
    expect(peerPush.status == CollabBackend::PushResult::Status::Created && peerPush.version == 3,
           "peer push accepted at v3");

    // 本地仍在 v2：推 → 409 → Conflict + 回滚。
    session.recordEntityModified(cubeIndex);
    auto stale = controller.push("t");
    expect(!stale.ok && stale.conflict, "stale push surfaces Conflict");
    expect(!controller.state().isSubmitInFlight(), "conflict rolls back submit");

    // 拉取：v3 合入（delta：peer-body 追加，本地 cube 保留）。
    auto pulled = controller.pull();
    expect(pulled.ok && pulled.version == 3, "pull applies v3");
    expect(controller.modelVersion() == 3, "pull advances model version");
    expect(findByUuid(session, QStringLiteral("peer-body")) != nullptr, "peer component imported");
    expect(findByUuid(session, cubeUuid) != nullptr, "delta pull keeps local rows");

    // 冲突：本地未提交修改命中远端变更组件 → 诚实拒绝。
    const auto* peerRow = findByUuid(session, QStringLiteral("peer-body"));
    expect(peerRow != nullptr, "peer row present for conflict setup");
    const ShapeHandle oldPeerShape = peerRow->shape;
    session.recordEntityModified(peerRow->index);
    const auto peerPush2 = peer.pushJsonDelta("p-x", "peer", peerGraph, "[]", {}, controller.modelVersion());
    expect(peerPush2.status == CollabBackend::PushResult::Status::Created && peerPush2.version == 4,
           "peer advances same component to v4");
    auto conflictPull = controller.pull();
    expect(!conflictPull.ok && conflictPull.conflict, "pull conflicts with unpushed local edit");
    expect(findByUuid(session, QStringLiteral("peer-body"))->shape == oldPeerShape,
           "conflicting pull does not touch document");

    // 放弃本地未提交 → pull 应用 v4。
    session.discardPendingChanges();
    auto applied = controller.pull();
    expect(applied.ok && applied.version == 4, "pull applies after conflict resolved");
    expect(findByUuid(session, QStringLiteral("peer-body"))->shape != oldPeerShape, "peer body replaced by v4");

    // 已是最新。
    auto upToDate = controller.pull();
    expect(upToDate.ok && upToDate.nothingToDo, "pull up-to-date is a no-op");

    // 打开项目：不存在 → 创建空项目（版本 0）。
    FakeKernel openKernel;
    DocumentSession openSession(openKernel);
    CollabController openController(openSession, backend);
    openSession.beginChangeTracking();
    openSession.addEntity(openKernel.create(E_CUBE, emptyGrid), "pre", E_CUBE);
    auto opened = openController.openProject(QStringLiteral("fresh-project"));
    expect(opened.ok && openController.modelVersion() == 0 && openSession.entities().empty(),
           "openProject creates empty project and clears canvas");
    expect(openController.projectName() == QStringLiteral("fresh-project"), "project name bound");
    // 再次打开（服务端有版本 0）→ 仍成功，画布为空。
    auto reopened = openController.openProject(QStringLiteral("fresh-project"));
    expect(reopened.ok, "reopen empty project succeeds");
}

// ---------------------------------------------- M1.5 StorageBackend 同接口测试
static void testStorageBackends() {
    FakeKernel kernel;

    // SAT 文件后端。
    SatFileBackend sat(kernel);
    QTemporaryDir dir;
    expect(dir.isValid(), "temp dir for SAT backend");
    const QString path = dir.path() + "/model.sat";
    dbcad::SaveRequest request;
    auto shapeA = kernel.create(E_CUBE, emptyGrid);
    auto shapeB = kernel.create(E_SPHERE, emptyGrid);
    request.satPayload = kernel.writeNative({shapeA, shapeB});
    QString error;
    expect(sat.save(path, request, &error).has_value() && QFile::exists(path), "SatFileBackend save writes file");
    const auto opened = sat.open(path, &error);
    expect(opened.has_value() && opened->shapes.size() == 2, "SatFileBackend open restores shapes");
    const auto listed = sat.list(dir.path(), &error);
    expect(listed.contains("model.sat"), "SatFileBackend list finds file");
    expect(sat.remove(path, &error) && !QFile::exists(path), "SatFileBackend remove deletes file");

    // 协作后端实现同一接口。
    MemoryTransport* memory = new MemoryTransport;
    CollabBackend collab;
    collab.setTransport(memory);
    collab.configure("http://memory", "tester", "");
    expect(collab.backendName() == QStringLiteral("fastapi-collab") && collab.isCollaborative(),
           "collab backend identity");
    expect(sat.backendName() == QStringLiteral("sat-file") && !sat.isCollaborative(), "sat backend identity");

    QString openError;
    const auto missing = collab.open(QStringLiteral("no-such-project"), &openError);
    expect(!missing.has_value() && openError.contains(QString::fromUtf8("不存在")),
           "collab open of missing project reports not-found");

    dbcad::SaveRequest collabSave;
    collabSave.deltaGraph = graphWith(kernel, {{QStringLiteral("uuid-1"), kernel.create(E_CUBE, emptyGrid)}});
    collabSave.changesJson = "[]";
    QString saveError;
    const auto version = collab.save(QStringLiteral("brand-new"), collabSave, &saveError);
    expect(version.has_value() && *version == 1, "collab save creates project and pushes v1");
    const auto reopenedDoc = collab.open(QStringLiteral("brand-new"), &openError);
    expect(reopenedDoc.has_value() && reopenedDoc->version == 1 && !reopenedDoc->projectId.isEmpty(),
           "collab open returns latest version");
    QString listError;
    expect(collab.list({}, &listError).isEmpty() && !listError.isEmpty(), "collab list unsupported by server");
    QString removeError;
    expect(!collab.remove(QStringLiteral("brand-new"), &removeError), "collab remove unsupported by server");
}

// ------------------------------------------------ M1.6 CommandBus 骨架测试
static void testCommandBus() {
    CommandBus bus;
    expect(bus.registerGroup("insert", {"g1", QStringLiteral("体"), false}), "group registered");
    expect(!bus.registerGroup("insert", {"g1", QStringLiteral("重复"), false}), "duplicate group rejected");

    CommandBus::Command command;
    command.id = "insert.cube";
    command.category = "insert";
    command.groupId = "g1";
    command.label = "立方体";
    command.operatorType = OPERATOR_CONSTRUCTOR;
    command.subOperatorType = E_CUBE;
    int executed = 0;
    command.execute = [&executed](const QJsonObject& params) {
        executed += params.value("factor").toInt(1);
        return true;
    };
    command.paramSchema =
        CommandBus::toJsonSchema({{QStringLiteral("factor"), QStringLiteral("integer"), false, QJsonValue(2)}});
    expect(bus.registerCommand(std::move(command)), "command registered");

    CommandBus::Command dup;
    dup.id = "insert.cube";
    dup.execute = [](const QJsonObject&) { return true; };
    expect(!bus.registerCommand(std::move(dup)), "duplicate command id rejected");

    expect(bus.commands("insert").size() == 1, "commands by category");
    expect(bus.command("insert.cube") != nullptr, "lookup by id");

    QString error;
    expect(bus.dispatch("insert.cube", {{"factor", 5}}, &error) && executed == 5, "dispatch executes with params");
    expect(bus.dispatch("insert.cube", {}, &error) && executed == 6, "optional params default in executor");
    expect(!bus.dispatch("no.such", {}, &error) && error.contains(QString::fromUtf8("未知命令")),
           "unknown command rejected");

    CommandBus::Command required;
    required.id = "operate.boolean";
    required.category = "operate";
    required.execute = [](const QJsonObject&) { return true; };
    required.paramSchema =
        CommandBus::toJsonSchema({{QStringLiteral("ids"), QStringLiteral("array"), true, QJsonValue()}});
    bus.registerCommand(std::move(required));
    expect(!bus.dispatch("operate.boolean", {}, &error) && error.contains(QString::fromUtf8("必填参数")),
           "missing required param rejected");
    expect(bus.dispatch("operate.boolean", {{"ids", QJsonArray{}}}, &error), "required param satisfied");

    const QJsonArray schema = bus.command("insert.cube")->paramSchema;
    expect(schema.first().toObject().value("name").toString() == QStringLiteral("factor") &&
               schema.first().toObject().value("type").toString() == QStringLiteral("integer"),
           "param schema serialized");
}

int main(int argc, char** argv) {
    QCoreApplication app(argc, argv);
    int code = 0;
    try {
        testChangeBookkeeping();
        testApplyRemoteReplacement();
        testCascadeAndRebuild();
        testStateMachine();
        testPushPullProtocol();
        testStorageBackends();
        testCommandBus();
        printf("PASS total=%d\n", checks);
    } catch (const std::exception& e) {
        fprintf(stderr, "FAIL after %d checks: %s\n", checks, e.what());
        code = 1;
    }
    return code;
}
