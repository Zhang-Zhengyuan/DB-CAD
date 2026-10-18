#pragma once

// SatFileBackend —— SAT 本地文件后端（架构 v2 核心库，REFACTOR_PLAN M1.5）。
//
// 包裹 kernel().writeNative / readNative：本地持久化与交换格式的唯一入口。
// SAT 片段作为测试正确性基准（docs/ARCHITECTURE.md §4）。

#include "core/storage_backend.h"

namespace dbcad {

class SatFileBackend : public StorageBackend {
public:
    explicit SatFileBackend(GeometryKernel& geometryKernel);

    QString backendName() const override { return QStringLiteral("sat-file"); }
    bool isConfigured() const override { return true; }
    bool isCollaborative() const override { return false; }

    std::optional<StorageDocument> open(const QString& location, QString* error) override;
    std::optional<int> save(const QString& location, const SaveRequest& request, QString* error) override;
    QStringList list(const QString& scope, QString* error) override;
    bool remove(const QString& location, QString* error) override;

private:
    GeometryKernel& kernel_;
};

} // namespace dbcad
