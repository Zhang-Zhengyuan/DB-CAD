/***********************************************************************
*                                                                      *
*               Copyright (C) 2016, Visual Kinematics, Inc.            *
*                                                                      *
*  These coded instructions, statements and computer programs contain  *
*  unpublished proprietary information of Visual Kinematics, Inc.,     *
*  and are protected by Federal copyright law.  They may not be        *
*  disclosed to third parties or copied or duplicated in any form,     *
*  in whole or in part, without the prior written consent of           *
*  Visual Kinematics, Inc.                                             *
*                                                                      *
***********************************************************************/
/***********************************************************************
*                                                                      *
*               DevTools4.4.1 Thu Jun 23 14:48:16 PDT 2016              *
*                                                                      *
***********************************************************************/
 
#ifndef DISPFUN_DEF
#define DISPFUN_DEF
#include "base/basedefs.h"
#include "vgl/vgldefs.h"
#define DISPFUN_POINT 1
#define DISPFUN_LINE 2
#define DISPFUN_SCAN 3
#define DISPFUN_POINTCOLOR 4
#define DISPFUN_SCANCOLOR 5
#define DISPFUN_TRIANGLE 6
#define DISPFUN_POINTDATA 7
#define DISPFUN_SCANDATA 8
#define DISPFUN_TEXT 9
#define DISPFUN_ALPHATEST 10
#define DISPFUN_POLYLINE 11
#define DISPFUN_MAX 12
#define DISPFUN_APIPRINT 0
struct vgl_DispFun { Vint ierr;Vobject *obj;void (*fun[1])();
void (*point)(Vobject*,Vint,Vint);void (*line)(Vobject*,
Vint,Vint,Vint,Vint);void (*scan)(Vobject*,Vint,Vint,
Vint);void (*pointcolor)(Vobject*,Vint,Vint,Vfloat[3]);
void (*scancolor)(Vobject*,Vint,Vfloat[3],Vint,Vfloat[3],
Vint);void (*triangle)(Vobject*,Vint,Vint,Vint,Vint,
Vint,Vint);void (*pointdata)(Vobject*,Vint,Vint,Vint,
Vfloat*);void (*scandata)(Vobject*,Vint,Vint,Vfloat*,
Vint,Vfloat*,Vint);void (*text)(Vobject*,Vint,Vint,
Vtchar*);void (*alphatest)(Vobject*,Vint,Vint,Vint,
Vfloat*,Vint*);void (*polyline)(Vobject*,Vint,Vint[][2]);
#ifdef __cplusplus
public: VKI_EXTERN vgl_DispFun(void);VKI_EXTERN void*
operator new (size_t);VKI_EXTERN ~vgl_DispFun(void);
VKI_EXTERN void operator delete (void*);VKI_EXTERN Vint
Error();VKI_EXTERN void Init();VKI_EXTERN void Set(Vint,Vfunc*);
VKI_EXTERN void Get(Vint,Vfunc**);VKI_EXTERN void SetObj(Vobject*);
VKI_EXTERN void GetObj(Vobject**);VKI_EXTERN void Copy(vgl_DispFun*);
VKI_EXTERN void API(Vint);VKI_EXTERN void Point(Vint,Vint);
VKI_EXTERN void PointColor(Vint,Vint,Vfloat[3]);VKI_EXTERN
void Line(Vint,Vint,Vint,Vint);VKI_EXTERN void Scan(Vint,Vint,Vint);
VKI_EXTERN void ScanColor(Vint,Vfloat[3],Vint,Vfloat[3],Vint);
VKI_EXTERN void Triangle(Vint,Vint,Vint,Vint,Vint,Vint);
VKI_EXTERN void PointData(Vint,Vint,Vint,Vfloat*);VKI_EXTERN
void ScanData(Vint,Vint,Vfloat*,Vint,Vfloat*,Vint);
VKI_EXTERN void Text(Vint,Vint,Vtchar*);VKI_EXTERN void
AlphaTest(Vint,Vint,Vint,Vfloat*,Vint*);VKI_EXTERN void
PolyLine(Vint,Vint[][2]);
#endif
};
#ifdef __cplusplus
extern "C" { 
#endif
VKI_EXTERN vgl_DispFun* vgl_DispFunBegin(void);VKI_EXTERN
void vgl_DispFun_Construct(vgl_DispFun *p);VKI_EXTERN
void vgl_DispFunEnd(vgl_DispFun *p);VKI_EXTERN void
vgl_DispFun_Destruct(vgl_DispFun *p);VKI_EXTERN Vint
vgl_DispFunError(vgl_DispFun *p);VKI_EXTERN void vgl_DispFunInit(vgl_DispFun
*p);VKI_EXTERN void vgl_DispFunSet(vgl_DispFun *p,Vint
type,Vfunc *function);VKI_EXTERN void vgl_DispFunGet(vgl_DispFun
*p,Vint type,Vfunc **function);VKI_EXTERN void vgl_DispFunSetObj(vgl_DispFun
*p,Vobject *obj);VKI_EXTERN void vgl_DispFunGetObj(vgl_DispFun
*p,Vobject **obj);VKI_EXTERN void vgl_DispFunCopy(vgl_DispFun
*p,vgl_DispFun *fromp);VKI_EXTERN void vgl_DispFunAPI(vgl_DispFun
*p,Vint api);
#ifdef __cplusplus
}
#endif
#define vgl_DispFunObj(x)(x)->obj
#define vgl_DispFunPoint(x,a,b) (x)->point((x)->obj,(a),(b))
#define vgl_DispFunPointColor(x,a,b,c) (x)->pointcolor((x)->obj,(a),(b),(c))
#define vgl_DispFunLine(x,a,b,c,d) (x)->line((x)->obj,(a),(b),(c),(d))
#define vgl_DispFunScan(x,a,b,c) (x)->scan((x)->obj,(a),(b),(c))
#define vgl_DispFunScanColor(x,a,b,c,d,e) (x)->scancolor((x)->obj,(a),(b),(c),(d),(e))
#define vgl_DispFunTriangle(x,a,b,c,d,e,f) (x)->triangle((x)->obj,(a),(b),(c),(d),(e),(f))
#define vgl_DispFunPointData(x,a,b,c,d) (x)->pointdata((x)->obj,(a),(b),(c),(d))
#define vgl_DispFunScanData(x,a,b,c,d,e,f) (x)->scandata((x)->obj,(a),(b),(c),(d),(e),(f))
#define vgl_DispFunText(x,a,b,c) (x)->text((x)->obj,(a),(b),(c))
#define vgl_DispFunAlphaTest(x,a,b,c,d,e) (x)->alphatest((x)->obj,(a),(b),(c),(d),(e))
#define vgl_DispFunPolyLine(x,a,b) (x)->polyline((x)->obj,(a),(b))
#endif

