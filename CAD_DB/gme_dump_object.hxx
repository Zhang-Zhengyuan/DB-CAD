#pragma once
#include <ctime>
#include <fstream>

#include "acis/include/alltop.hxx"
#include "acis/include/box.hxx"
#include "acis/include/bs2c_def.hxx"
#include "acis/include/bs2curve.hxx"
#include "acis/include/bs3c_def.hxx"
#include "acis/include/bs3curve.hxx"
#include "acis/include/condef.hxx"
#include "acis/include/cone.hxx"
#include "acis/include/curdef.hxx"
#include "acis/include/curve.hxx"
#include "acis/include/elldef.hxx"
#include "acis/include/heldef.hxx"
#include "acis/include/intdef.hxx"
#include "acis/include/interval.hxx"
#include "acis/include/kernapi.hxx"
#include "acis/include/law_base.hxx"
#include "acis/include/meshsurf.hxx"
#include "acis/include/mshdef.hxx"
#include "acis/include/param.h"
#include "acis/include/pcudef.hxx"
#include "acis/include/pcurve.hxx"
#include "acis/include/pladef.hxx"
#include "acis/include/plane.hxx"
#include "acis/include/point.hxx"
#include "acis/include/sphdef.hxx"
#include "acis/include/sphere.hxx"
#include "acis/include/spldef.hxx"
#include "acis/include/spline.hxx"
#include "acis/include/strdef.hxx"
#include "acis/include/surdef.hxx"
#include "acis/include/surface.hxx"
#include "acis/include/tordef.hxx"
#include "acis/include/torus.hxx"
#include "acis/include/transfrm.hxx"

// 基础
void DECL_KERN dump_object(FILE* f, int indents, SPAposition const position);
void DECL_KERN dump_object(FILE* f, int indents, SPAvector const vector);
void DECL_KERN dump_object(FILE* f, int indents, SPAmatrix const matrix);
void DECL_KERN dump_object(FILE* f, int indents, SPAinterval const interval);
void DECL_KERN dump_object(FILE* f, int indents, SPAbox* const box);
void DECL_KERN dump_object(FILE* f, int indents, law* l);

// 几何（非持久化）
void DECL_KERN dump_object(FILE* f, int indents, bs2_curve_def* c);
void DECL_KERN dump_object(FILE* f, int indents, bs3_curve_def* c);
void DECL_KERN dump_object(FILE* f, int indents, intcurve* i);
void DECL_KERN dump_object(FILE* f, int indents, const curve* c);
void DECL_KERN dump_object(FILE* f, int indents, const pcurve* p);
void DECL_KERN dump_object(FILE* f, int indents, const surface* s);
void DECL_KERN dump_object(FILE* f, int indents, const SPAtransf* transf);

// 几何（持久化）
void DECL_KERN dump_object(FILE* f, int indents, class APOINT* const entity);
void DECL_KERN dump_object(FILE* f, int indents, class CURVE* entity);
void DECL_KERN dump_object(FILE* f, int indents, class PCURVE* entity);
void DECL_KERN dump_object(FILE* f, int indents, class SURFACE* entity);
void DECL_KERN dump_object(FILE* f, int indents, class TRANSFORM* entity);

// 拓扑
void DECL_KERN dump_object(FILE* f, int indents, class VERTEX* const entity);
void DECL_KERN dump_object(FILE* f, int indents, class EDGE* const entity);
void DECL_KERN dump_object(FILE* f, int indents, class COEDGE* const entity);
void DECL_KERN dump_object(FILE* f, int indents, class WIRE* const entity);
void DECL_KERN dump_object(FILE* f, int indents, class LOOP* const entity);
void DECL_KERN dump_object(FILE* f, int indents, class FACE* const entity);
void DECL_KERN dump_object(FILE* f, int indents, class SUBSHELL* const entity);
void DECL_KERN dump_object(FILE* f, int indents, class SHELL* const entity);
void DECL_KERN dump_object(FILE* f, int indents, class LUMP* const entity);
void DECL_KERN dump_object(FILE* f, const char* info, class BODY* const entity);

// 统一接口
void DECL_KERN dump_object(FILE* f, const char* info, int indents, class ENTITY* const entity);

// 统计信息
void DECL_KERN print_statistic(class ENTITY* e);
