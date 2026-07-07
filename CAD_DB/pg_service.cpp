#include "pg_service.h"
#include "pg_store.hxx"

#include <QApplication>
#include <QDir>
#include <QFile>
#include <QTextStream>
#include <QStringConverter>

PgService::PgService(const QString& configPath, QObject* parent)
    : QObject(parent), m_configPath(configPath) {
    loadConfig();
}

PgService::~PgService() = default;

void PgService::loadConfig() {
    const QString appDir = QApplication::applicationDirPath();

    QStringList candidates;
    if (!m_configPath.isEmpty()) {
        candidates << m_configPath;
    }

    const QByteArray envOverride = qgetenv("DBCAD_PG_CONNECT");
    if (!envOverride.isEmpty()) {
        candidates << QString::fromLocal8Bit(envOverride);
    }
    candidates << QDir(appDir).filePath("pg_connect_info.conf")
               << QDir(appDir).filePath("../pg_connect_info.conf")
               << QDir(appDir).filePath("../../pg_connect_info.conf");

    QString confPath;
    for (const QString& c : candidates) {
        if (QFile::exists(c)) { confPath = c; break; }
    }

    if (confPath.isEmpty()) {
        emit initFailed(QString::fromUtf8("pg_connect_info.conf not found in any candidate path"));
        return;
    }

    QFile conf(confPath);
    if (!conf.open(QIODevice::ReadOnly | QIODevice::Text)) {
        emit initFailed(QString::fromUtf8("Cannot open %1").arg(confPath));
        return;
    }

    QStringList lines;
    QTextStream in(&conf);
    in.setEncoding(QStringConverter::Utf8);
    while (!in.atEnd() && lines.size() < 5) {
        QString line = in.readLine().trimmed();
        if (line.isEmpty() || line.startsWith(u'#')) continue;
        lines << line;
    }
    conf.close();

    if (lines.size() < 5) {
        emit initFailed(QString::fromUtf8("pg_connect_info.conf: need 5 lines, got %1").arg(lines.size()));
        return;
    }

    dbcad::pg_demo::PgConfig cfg;
    cfg.host     = lines[0].toStdString();
    cfg.port     = lines[1].toInt();
    cfg.user     = lines[2].toStdString();
    cfg.password = lines[3].toStdString();
    cfg.dbname   = lines[4].toStdString();

    m_store = std::make_unique<dbcad::pg_demo::PgStore>(cfg);

    std::string err;
    if (!m_store->ensureSchema(err)) {
        emit initFailed(QString::fromUtf8("ensureSchema failed: %1").arg(QString::fromStdString(err)));
        return;
    }

    m_initialized = true;
    emit initialized();
}

void PgService::saveSat(const QString& name, const QString& satText) {
    if (!m_store) {
        emit saveFailed(QString::fromUtf8("PgService not initialized"));
        return;
    }
    std::string err;
    auto sp = m_store->saveSat(name.toStdString(), satText.toStdString(), err);
    if (!sp) {
        emit saveFailed(QString::fromUtf8("saveSat failed: %1").arg(QString::fromStdString(err)));
        return;
    }
    emit saved(name, sp->size_bytes, sp->id);
}

void PgService::loadSat(const QString& name) {
    if (!m_store) {
        emit loadFailed(QString::fromUtf8("PgService not initialized"));
        return;
    }
    std::string err;
    auto txt = m_store->loadSat(name.toStdString(), err);
    if (!txt) {
        emit loadFailed(QString::fromUtf8("loadSat(%1) failed: %2")
                        .arg(name).arg(QString::fromStdString(err)));
        return;
    }
    emit loaded(QString::fromStdString(*txt));
}

void PgService::listParts(int limit) {
    if (!m_store) {
        emit listFailed(QString::fromUtf8("PgService not initialized"));
        return;
    }
    std::string err;
    auto parts = m_store->listParts(limit, err);
    if (!parts) {
        emit listFailed(QString::fromUtf8("listParts failed: %1").arg(QString::fromStdString(err)));
        return;
    }

    QVector<QString> names, updatedAts;
    QVector<qint64> ids, sizes;
    names.reserve(parts->size());
    ids.reserve(parts->size());
    sizes.reserve(parts->size());
    updatedAts.reserve(parts->size());

    for (const auto& p : *parts) {
        names.push_back(QString::fromStdString(p.name));
        ids.push_back(p.id);
        sizes.push_back(p.size_bytes);
        updatedAts.push_back(QString::fromStdString(p.updated_at));
    }
    emit listed(names, ids, sizes, updatedAts);
}

void PgService::countParts() {
    if (!m_store) {
        emit countFailed(QString::fromUtf8("PgService not initialized"));
        return;
    }
    std::string err;
    long long cnt = m_store->countParts(err);
    if (cnt < 0) {
        emit countFailed(QString::fromUtf8("countParts failed: %1").arg(QString::fromStdString(err)));
        return;
    }
    emit counted(cnt);
}

void PgService::deleteByName(const QString& name) {
    if (!m_store) {
        emit deleteFailed(QString::fromUtf8("PgService not initialized"));
        return;
    }
    std::string err;
    auto del = m_store->deleteByName(name.toStdString(), err);
    if (!del) {
        emit deleteFailed(QString::fromUtf8("deleteByName(%1) failed: %2")
                          .arg(name).arg(QString::fromStdString(err)));
        return;
    }
    emit deleted(name);
}
