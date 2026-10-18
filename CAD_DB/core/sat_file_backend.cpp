#include "core/sat_file_backend.h"

#include <QDir>
#include <QFile>

namespace dbcad {

SatFileBackend::SatFileBackend(GeometryKernel& geometryKernel) : kernel_(geometryKernel) {}

void SatFileBackend::open(const QString& location, std::function<void(const StorageOpenResult&)> callback) {
    StorageOpenResult result;
    QFile file(location);
    if (!file.open(QIODevice::ReadOnly)) {
        result.error = QString::fromUtf8("无法读取文件");
        return callback(result);
    }
    try {
        result.document.location = location;
        result.document.shapes = kernel_.readNative(file.readAll());
        result.ok = true;
    } catch (const std::exception& e) {
        result = StorageOpenResult{};
        result.error = QString::fromUtf8(e.what());
    }
    callback(result);
}

void SatFileBackend::save(const QString& location, const SaveRequest& request,
                          std::function<void(const StorageSaveResult&)> callback) {
    StorageSaveResult result;
    QFile file(location);
    if (!file.open(QIODevice::WriteOnly) || file.write(request.satPayload) != request.satPayload.size()) {
        result.error = QString::fromUtf8("无法写入文件：%1").arg(location);
        return callback(result);
    }
    result.ok = true;
    callback(result);
}

void SatFileBackend::list(const QString& scope, std::function<void(const StorageListResult&)> callback) {
    StorageListResult result;
    QDir directory(scope.isEmpty() ? QStringLiteral(".") : scope);
    if (!directory.exists()) {
        result.error = QString::fromUtf8("目录不存在：%1").arg(scope);
        return callback(result);
    }
    result.ok = true;
    result.entries = directory.entryList({QStringLiteral("*.sat")}, QDir::Files, QDir::Name);
    callback(result);
}

void SatFileBackend::remove(const QString& location, std::function<void(const StorageRemoveResult&)> callback) {
    StorageRemoveResult result;
    if (!QFile::remove(location)) {
        result.error = QString::fromUtf8("无法删除文件：%1").arg(location);
        return callback(result);
    }
    result.ok = true;
    callback(result);
}

} // namespace dbcad
