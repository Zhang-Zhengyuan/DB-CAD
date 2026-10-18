#include "core/sat_file_backend.h"

#include <QDir>
#include <QFile>
#include <QFileInfo>

namespace dbcad {

SatFileBackend::SatFileBackend(GeometryKernel& geometryKernel) : kernel_(geometryKernel) {}

std::optional<StorageDocument> SatFileBackend::open(const QString& location, QString* error) {
    QFile file(location);
    if (!file.open(QIODevice::ReadOnly)) {
        if (error != nullptr)
            *error = QString::fromUtf8("无法读取文件");
        return std::nullopt;
    }
    try {
        StorageDocument document;
        document.location = location;
        document.shapes = kernel_.readNative(file.readAll());
        return document;
    } catch (const std::exception& e) {
        if (error != nullptr)
            *error = QString::fromUtf8(e.what());
        return std::nullopt;
    }
}

std::optional<int> SatFileBackend::save(const QString& location, const SaveRequest& request, QString* error) {
    QFile file(location);
    if (!file.open(QIODevice::WriteOnly) || file.write(request.satPayload) != request.satPayload.size()) {
        if (error != nullptr)
            *error = QString::fromUtf8("无法写入文件：%1").arg(location);
        return std::nullopt;
    }
    return 0;
}

QStringList SatFileBackend::list(const QString& scope, QString* error) {
    QDir directory(scope.isEmpty() ? QStringLiteral(".") : scope);
    if (!directory.exists()) {
        if (error != nullptr)
            *error = QString::fromUtf8("目录不存在：%1").arg(scope);
        return {};
    }
    return directory.entryList({QStringLiteral("*.sat")}, QDir::Files, QDir::Name);
}

bool SatFileBackend::remove(const QString& location, QString* error) {
    if (!QFile::remove(location)) {
        if (error != nullptr)
            *error = QString::fromUtf8("无法删除文件：%1").arg(location);
        return false;
    }
    return true;
}

} // namespace dbcad
