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
 
#ifndef QUADRIC_DEF
#define QUADRIC_DEF
#include "base/basedefs.h"
#include "vgl/drawfun.h"
#define QUADRIC_SHADE 0
#define QUADRIC_DRAWSTYLE 1
#define QUADRIC_TEXTURECOORD 2
#define QUADRIC_SLICES 3
#define QUADRIC_STACKS 4
#define QUADRIC_AZIMUTHSTART 5
#define QUADRIC_AZIMUTHSTOP 6
#define QUADRIC_LINE 7
#define QUADRIC_FILL 8
#define QUADRIC_ORIGIN 9
#define QUADRIC_POINT 10
struct vgl_Quadric { Vint ierr;Vint texcoordflag;Vint
drawstyle;Vint shade;Vint slices;Vint stacks;Vint pointflag;
Vint lineflag;Vint fillflag;Vfloat astart;Vfloat astop;
Vfloat xo[3];vgl_DrawFun *df;
#ifdef __cplusplus
public: VKI_EXTERN vgl_Quadric(void);VKI_EXTERN void*
operator new (size_t);VKI_EXTERN ~vgl_Quadric(void);
VKI_EXTERN void operator delete (void*);VKI_EXTERN Vint
Error();VKI_EXTERN void SetObject(Vint,Vobject*);VKI_EXTERN
void SetParami(Vint,Vint);VKI_EXTERN void SetParamf(Vint,Vfloat);
VKI_EXTERN void SetParamfv(Vint,Vfloat[]);VKI_EXTERN
void Cylinder(Vfloat,Vfloat,Vfloat);VKI_EXTERN void
Sphere(Vfloat);VKI_EXTERN void Copy(vgl_Quadric*);VKI_EXTERN
void Print();
#endif
};
#ifdef __cplusplus
extern "C" { 
#endif
VKI_EXTERN vgl_Quadric* vgl_QuadricBegin(void);VKI_EXTERN
void vgl_Quadric_Construct(vgl_Quadric *p);VKI_EXTERN
void vgl_QuadricEnd(vgl_Quadric *p);VKI_EXTERN void
vgl_Quadric_Destruct(vgl_Quadric *p);VKI_EXTERN Vint
vgl_QuadricError(vgl_Quadric *p);VKI_EXTERN void vgl_QuadricSetObject(vgl_Quadric
*p,Vint objecttype,Vobject *object);VKI_EXTERN void
vgl_QuadricSetParami(vgl_Quadric *p,Vint type,Vint iparam);
VKI_EXTERN void vgl_QuadricSetParamf(vgl_Quadric *p,
Vint type,Vfloat fparam);VKI_EXTERN void vgl_QuadricSetParamfv(vgl_Quadric
*p,Vint type,Vfloat fparam[]);VKI_EXTERN void vgl_QuadricCylinder(vgl_Quadric
*p,Vfloat baserad,Vfloat toprad,Vfloat height);VKI_EXTERN
void vgl_QuadricSphere(vgl_Quadric *p,Vfloat rad);VKI_EXTERN
void vgl_QuadricCopy(vgl_Quadric *p,vgl_Quadric *fromp);
VKI_EXTERN void vgl_QuadricPrint(vgl_Quadric *p);
#ifdef __cplusplus
}
#endif
#endif

