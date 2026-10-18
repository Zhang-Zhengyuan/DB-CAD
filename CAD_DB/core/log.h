#pragma once

// 统一日志入口（架构 v2 核心库，REFACTOR_PLAN M2.3）。
//
// 约定：
//   * 常规可诊断信息 —— dbcad::logInfo() / qInfo
//   * 异常但可恢复（SAT 回退、解码失败原因）—— dbcad::logWarn() / qWarning
//   * 错误 —— dbcad::logCritical() / qCritical
//   * 纯调试轨迹（printf 风格）—— DBCAD_TRACE(...)：默认编译关闭，
//     设 DBCAD_TRACE_ENABLED（或 /DDBCAD_TRACE_ENABLED）打开。
// 不再直接 fprintf(stderr) 散落业务代码。

#include <QDebug>
#include <cstdio>

namespace dbcad {
inline QDebug logInfo() { return qInfo().noquote(); }
inline QDebug logWarn() { return qWarning().noquote(); }
inline QDebug logCritical() { return qCritical().noquote(); }
} // namespace dbcad

#ifdef DBCAD_TRACE_ENABLED
#define DBCAD_TRACE(...) std::fprintf(stderr, __VA_ARGS__)
#else
#define DBCAD_TRACE(...) ((void)0)
#endif
