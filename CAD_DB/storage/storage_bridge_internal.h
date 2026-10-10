#pragma once

#include "storage_bridge_service.h"

#include <QDateTime>
#include <QFile>
#include <QHostAddress>
#include <QJsonArray>
#include <QJsonDocument>
#include <QJsonObject>
#include <QTcpServer>
#include <QTcpSocket>
#include <QUrl>
#include <QUrlQuery>

#include <algorithm>
#include <cstdio>
#include <stdexcept>

#include <acis/include/alltop.hxx>

#include "access.hxx"
#include "acis.hxx"
#include "neo4j.hxx"

#include <vector>

// Private query/SAT helpers shared by Bridge endpoint implementations.
namespace dbcad::bridge {
struct QueryValue {
    mg_value_type type = MG_VALUE_TYPE_NULL;
    QString stringValue;
    int64_t intValue = 0;
};
QString nowIsoUtc();
QByteArray jsonError(const QString &detail);
QString toQString(const QueryValue &value);
int64_t toInt64(const QueryValue &value, bool *ok = nullptr);
std::vector<std::vector<QueryValue>> runQuery(const QString &host, int port, const QString &user,
                                              const QString &password, const QString &statement, mg_map *params,
                                              QString &error);
bool saveSatToPart(const QString &host, int port, const QString &user, const QString &password, const QString &partName,
                   const QString &sat, QString &error);
bool loadSatFromPart(const QString &host, int port, const QString &user, const QString &password,
                     const QString &partName, QString &sat, QString &error);
int parsePositiveInt(const QString &value, int fallback);
} // namespace dbcad::bridge
