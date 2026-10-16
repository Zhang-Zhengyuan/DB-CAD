#pragma once
#include "model_types.h"
#include "../gme_mesh.hxx"
#include <QByteArray>
#include <QHash>
#include <QJsonObject>
#include <QString>
#include <memory>
#include <cstdio>

namespace dbcad {
struct DecodedGraph { QHash<QString, ShapeHandle> shapes; };
// This boundary owns native geometry. Clients keep values and session handles only.
// Throws std::runtime_error for invalid handles, unsupported operations and kernel errors.
class GeometryKernel {
public:
    virtual ~GeometryKernel() = default;
    virtual ShapeHandle create(int primitive, HandleGrid& handles) = 0;
    virtual ShapeHandle draft() = 0;
    virtual ShapeList operate(OPERATOR_TYPES operation, int subtype, const ShapeList& inputs) = 0;
    virtual ShapeKind kind(ShapeHandle shape) const = 0;
    virtual void translate(ShapeHandle shape, Translation transform, bool bake=false) = 0;
    virtual void copyColor(ShapeHandle source, ShapeHandle destination) = 0;
    virtual void erase(const ShapeList& shapes) = 0;
    virtual bool mesh(ShapeHandle shape, GmeMesh::DisplayData& data) = 0;
    virtual QByteArray writeNative(const ShapeList& shapes) = 0;
    virtual ShapeList readNative(const QByteArray& data) = 0;
    virtual QJsonObject encodeGraph(const QHash<int, QString>& uuids, const ShapeList& shapes) = 0;
    virtual bool decodeGraph(const QJsonObject& graph, DecodedGraph& result, QString& error) = 0;
    virtual void noteState() = 0;
    virtual void dump(ShapeHandle shape, const std::string& name, FILE* file, bool debug) = 0;
    virtual void statistics(ShapeHandle shape) = 0;
};
std::unique_ptr<GeometryKernel> createGeometryKernel();
GeometryKernel& kernel();
// Explicit shutdown occurs after the document has released its geometry.
void shutdownKernel();
}
