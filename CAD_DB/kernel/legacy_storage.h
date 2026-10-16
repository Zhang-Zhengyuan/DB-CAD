#pragma once
#include "geometry_kernel.h"
namespace dbcad {
// Compatibility port for existing Neo4j schemas and diagnostic menus.
void initializeStorage();
void shutdownStorage();
std::int64_t legacyPartCount(const DbConfig& config, const std::string& part);
void saveLegacyPart(const DbConfig& config, const std::string& part, const ShapeList& shapes, bool incremental);
ShapeList loadLegacyPart(const DbConfig& config, const std::string& part, int generation=0);
void runLegacyAccess(const DbConfig& config);
void runLegacyIncremental(const DbConfig& config);
}
