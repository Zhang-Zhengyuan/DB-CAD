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
 
#ifndef XFM_DEF
#define XFM_DEF
#include "base/basedefs.h"
#include "vgl/vgldefs.h"
#define XFM_XAXIS 1
#define XFM_YAXIS 2
#define XFM_ZAXIS 3
#define XFM_ROTATE 1
#define XFM_TRANSLATE 2
#define XFM_SCALE 3
struct vgl_Xfm { Vint ierr;float tg[4][4];
#ifdef __cplusplus
public: VKI_EXTERN vgl_Xfm(void);VKI_EXTERN void* operator
new (size_t);VKI_EXTERN ~vgl_Xfm(void);VKI_EXTERN void
operator delete (void*);VKI_EXTERN Vint Error();VKI_EXTERN
void Identity();VKI_EXTERN void Invert();VKI_EXTERN
void Purge(Vint);VKI_EXTERN void Small(Vfloat);VKI_EXTERN
void Rotate(Vfloat,Vint);VKI_EXTERN void RotateVector(Vfloat,Vfloat,Vfloat,Vfloat);
VKI_EXTERN void ComputeRotateVector(Vfloat*,Vfloat*,Vfloat*,Vfloat*);
VKI_EXTERN void RotateEuler(Vfloat,Vfloat,Vfloat);VKI_EXTERN
void ComputeRotateEuler(Vfloat*,Vfloat*,Vfloat*);VKI_EXTERN
void Translate(Vfloat,Vfloat,Vfloat);VKI_EXTERN void
Scale(Vfloat,Vfloat,Vfloat);VKI_EXTERN void LookAt(Vfloat,Vfloat,Vfloat,Vfloat,Vfloat,Vfloat,Vfloat,
Vfloat,Vfloat);VKI_EXTERN void Ortho(Vfloat,Vfloat,Vfloat,Vfloat,Vfloat,Vfloat);
VKI_EXTERN void Frustum(Vfloat,Vfloat,Vfloat,Vfloat,Vfloat,Vfloat);
VKI_EXTERN void TestPerspective(Vint*);VKI_EXTERN void
SetMatrix(Vfloat[4][4]);VKI_EXTERN void GetMatrix(Vfloat[4][4]);
VKI_EXTERN void GetMatrix3(Vfloat[3][3]);VKI_EXTERN
void Mult(vgl_Xfm*);VKI_EXTERN void Copy(vgl_Xfm*);
VKI_EXTERN void Print();
#endif
};
#ifdef __cplusplus
extern "C" { 
#endif
VKI_EXTERN vgl_Xfm* vgl_XfmBegin(void);VKI_EXTERN void
vgl_Xfm_Construct(vgl_Xfm *p);VKI_EXTERN void vgl_XfmEnd(vgl_Xfm
*p);VKI_EXTERN void vgl_Xfm_Destruct(vgl_Xfm *p);VKI_EXTERN
Vint vgl_XfmError(vgl_Xfm *p);VKI_EXTERN void vgl_XfmIdentity(vgl_Xfm
*p);VKI_EXTERN void vgl_XfmInvert(vgl_Xfm *p);VKI_EXTERN
void vgl_XfmPurge(vgl_Xfm *p,Vint type);VKI_EXTERN void
vgl_XfmSmall(vgl_Xfm *p,Vfloat eps);VKI_EXTERN void
vgl_XfmRotate(vgl_Xfm *p,Vfloat angle,Vint axis);VKI_EXTERN
void vgl_XfmRotateVector(vgl_Xfm *p,Vfloat angle,Vfloat
vectx,Vfloat vecty,Vfloat vectz);VKI_EXTERN void vgl_XfmComputeRotateVector(vgl_Xfm
*p,Vfloat *angle,Vfloat *vectx,Vfloat *vecty,Vfloat
*vectz);VKI_EXTERN void vgl_XfmRotateEuler(vgl_Xfm *p,
Vfloat tx,Vfloat ty,Vfloat tz);VKI_EXTERN void vgl_XfmComputeRotateEuler(vgl_Xfm
*p,Vfloat *tx,Vfloat *ty,Vfloat *tz);VKI_EXTERN void
vgl_XfmTranslate(vgl_Xfm *p,Vfloat tx,Vfloat ty,Vfloat
tz);VKI_EXTERN void vgl_XfmScale(vgl_Xfm *p,Vfloat sx,
Vfloat sy,Vfloat sz);VKI_EXTERN void vgl_XfmLookAt(vgl_Xfm
*p,Vfloat eyex,Vfloat eyey,Vfloat eyez,Vfloat centerx,
Vfloat centery,Vfloat centerz,Vfloat upx,Vfloat upy,
Vfloat upz);VKI_EXTERN void vgl_XfmOrtho(vgl_Xfm *p,
Vfloat left,Vfloat right,Vfloat bottom,Vfloat top,Vfloat
nearz,Vfloat farz);VKI_EXTERN void vgl_XfmFrustum(vgl_Xfm
*p,Vfloat left,Vfloat right,Vfloat bottom,Vfloat top,
Vfloat nearz,Vfloat farz);VKI_EXTERN void vgl_XfmTestPerspective(vgl_Xfm
*p,Vint *iperflag);VKI_EXTERN void vgl_XfmSetMatrix(vgl_Xfm
*p,Vfloat matrix4x4[4][4]);VKI_EXTERN void vgl_XfmGetMatrix(vgl_Xfm
*p,Vfloat matrix4x4[4][4]);VKI_EXTERN void vgl_XfmGetMatrix3(vgl_Xfm
*p,Vfloat matrix3x3[3][3]);VKI_EXTERN void vgl_XfmMult(vgl_Xfm
*p,vgl_Xfm *multp);VKI_EXTERN void vgl_XfmCopy(vgl_Xfm
*p,vgl_Xfm *fromp);VKI_EXTERN void vgl_XfmPrint(vgl_Xfm
*p);
#ifdef __cplusplus
}
#endif
#endif

