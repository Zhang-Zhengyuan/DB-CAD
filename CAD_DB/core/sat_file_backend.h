#pragma once

// SatFileBackend —— SAT 本地文件后端（架构 v2 核心库，REFACTOR_PLAN M1.5）。
//
// 包裹 kernel().writeNative / readNative：本地持久化与交换格式的唯一入口。
// SAT 片段作为测试正确性基准（docs/ARCHITECTURE.md §4）。
// 本地快 IO：回调在调用内同步完成（接口契约允许）。

#include "core/storage_backend.h"

namespace dbcad {

class SatFileBackend : public StorageBackend {
public:
    explicit SatFileBackend(GeometryKernel& geometryKernel);

    QString backendName() const override { return QStringLiteral("sat-file"); }
    bool isConfigured() const override { return true; }
    bool isCollaborative() const override { return false; }

    void open(const QString& location, std::function<void(const StorageOpenResult&)> callback) override;
    void save(const QString& location, const SaveRequest& request,
              std::function<void(const StorageSaveResult&)> callback) override;
    void list(const QString& scope, std::function<void(const StorageListResult&)> callback) override;
    void remove(const QString& location, std::function<void(const StorageRemoveResult&)> callback) override;

private:
    GeometryKernel& kernel_;
};

} // namespace dbcad
