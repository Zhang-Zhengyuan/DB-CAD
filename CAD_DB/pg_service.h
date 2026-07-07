#pragma once

#include <QObject>
#include <QString>
#include <QVector>
#include <memory>

namespace dbcad::pg_demo {
    struct PgConfig;
    struct StoredPart;
    struct PgStore;
}

class PgService : public QObject {
    Q_OBJECT

public:
    // 'configPath' may be empty — in that case loadPgEnvFromConfig() is used.
    explicit PgService(const QString& configPath = QString(), QObject* parent = nullptr);
    ~PgService();

    bool isInitialized() const { return m_initialized; }

public slots:
    void saveSat(const QString& name, const QString& satText);
    void loadSat(const QString& name);
    void listParts(int limit = 200);
    void countParts();
    void deleteByName(const QString& name);

signals:
    void saved(const QString& name, qint64 sizeBytes, qint64 id);
    void saveFailed(const QString& error);
    void loaded(const QString& satText);
    void loadFailed(const QString& error);
    void listed(const QVector<QString>& names,
                const QVector<qint64>& ids,
                const QVector<qint64>& sizes,
                const QVector<QString>& updatedAts);
    void listFailed(const QString& error);
    void counted(qint64 count);
    void countFailed(const QString& error);
    void deleted(const QString& name);
    void deleteFailed(const QString& error);
    void initialized();
    void initFailed(const QString& error);

private:
    void loadConfig();

    std::unique_ptr<dbcad::pg_demo::PgStore> m_store;
    QString m_configPath;
    bool m_initialized = false;
};
