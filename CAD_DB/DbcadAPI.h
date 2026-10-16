#pragma once
#include "kernel/geometry_kernel.h"
#include "kernel/legacy_storage.h"
namespace dbcad {
bool InitializeEnvironment();
void TerminateEnvironment();
class ModelerSession {
public:
    ModelerSession(const DbConfig& config,const std::string& partName);
    ~ModelerSession();
    void SavePartFull(const ShapeList& shapes) const;
    ShapeList LoadPartFull() const;
    std::int64_t CheckPartExists() const;
    void SavePartIncremental() const;
    ShapeList LoadPartIncremental(int generation) const;
    bool GenerateMesh(ShapeHandle shape,GmeMesh::DisplayData& data);
private:
    DbConfig config;
    std::string partName;
};
}
