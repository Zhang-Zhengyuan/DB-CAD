#include "DbcadAPI.h"
#include <stdexcept>
namespace dbcad {
bool InitializeEnvironment() { try { kernel(); initializeStorage(); return true; } catch(...) { return false; } }
void TerminateEnvironment() { shutdownStorage(); shutdownKernel(); }
ModelerSession::ModelerSession(const DbConfig& c,const std::string& part):config(c),partName(part) {}
ModelerSession::~ModelerSession()=default;
void ModelerSession::SavePartFull(const ShapeList& shapes) const { saveLegacyPart(config,partName,shapes,false); }
ShapeList ModelerSession::LoadPartFull() const { if(CheckPartExists()!=1) throw std::runtime_error("零件不存在或不唯一"); return loadLegacyPart(config,partName); }
std::int64_t ModelerSession::CheckPartExists() const { return legacyPartCount(config,partName); }
void ModelerSession::SavePartIncremental() const { saveLegacyPart(config,partName,{},true); }
ShapeList ModelerSession::LoadPartIncremental(int generation) const { return loadLegacyPart(config,partName,generation); }
bool ModelerSession::GenerateMesh(ShapeHandle shape,GmeMesh::DisplayData& data) { return kernel().mesh(shape,data); }
}
