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
 
#ifndef XFMSTACK_DEF
#define XFMSTACK_DEF
#include "base/basedefs.h"
#include "vgl/xfm.h"
#define XFMSTACK_VIEWING 0
#define XFMSTACK_PROJECTION 1
#define XFMSTACK_MAX 32
struct vgl_Xfmstack { Vint ierr;Vint numxfm;vgl_Xfm
*xfm;vgl_Xfm *mfm[XFMSTACK_MAX];Vint perpfm;Vint numpfm;
vgl_Xfm *pfm[XFMSTACK_MAX];Vfloat plimits[XFMSTACK_MAX][6];
Vint ptype[XFMSTACK_MAX];Vfloat ti[3][3];Vint txiflag;
Vfloat txi[4][4];Vint left,right;Vint bottom,top;Vfloat
pxi,pyi;Vint maxeqn;Vint eqnflag[VGL_MAXCLIPPLANE];
Vfloat eqn[VGL_MAXCLIPPLANE][4];vgl_Xfm *xft;
#ifdef __cplusplus
public: VKI_EXTERN vgl_Xfmstack(void);VKI_EXTERN void*
operator new (size_t);VKI_EXTERN ~vgl_Xfmstack(void);
VKI_EXTERN void operator delete (void*);VKI_EXTERN Vint
Error();VKI_EXTERN void Push();VKI_EXTERN void Pop();
VKI_EXTERN void Load(vgl_Xfm*);VKI_EXTERN void Mult(vgl_Xfm*);
VKI_EXTERN void Proj(vgl_Xfm*);VKI_EXTERN void SetProjLimits(Vfloat,Vfloat,Vfloat,Vfloat,Vfloat,Vfloat);
VKI_EXTERN void GetProjLimits(Vfloat*,Vfloat*,Vfloat*,Vfloat*,Vfloat*,
Vfloat*);VKI_EXTERN void SetProjType(Vint);VKI_EXTERN
void GetProjType(Vint*);VKI_EXTERN void ProjPop();VKI_EXTERN
void ProjPush();VKI_EXTERN void GetNum(Vint*);VKI_EXTERN
void SetViewport(Vint,Vint,Vint,Vint);VKI_EXTERN void
GetViewport(Vint*,Vint*,Vint*,Vint*);VKI_EXTERN void
SetClipPlane(Vint,Vfloat[4]);VKI_EXTERN void GetClipPlaneEye(Vint,Vfloat[4]);
VKI_EXTERN void SetSwitch(Vint,Vint,Vint);VKI_EXTERN
void ClipPlanePoint(Vfloat[3],Vint*);VKI_EXTERN void
GetMatrix(Vfloat[4][4]);VKI_EXTERN void GetMatrixModelView(Vfloat[4][4]);
VKI_EXTERN void GetMatrixProjection(Vfloat[4][4]);VKI_EXTERN
void GetMatrixMVPInverse(Vfloat[4][4]);VKI_EXTERN void
Vertex(Vfloat[3],Vfloat[3]);VKI_EXTERN void VertexW(Vfloat[3],Vfloat[3],Vfloat*);
VKI_EXTERN void UnVertex(Vfloat[3],Vfloat[3]);VKI_EXTERN
void VertexModelView(Vfloat[3],Vfloat[3]);VKI_EXTERN
void NormalModelView(Vfloat[3],Vfloat[3]);VKI_EXTERN
void NearClipPoint(Vint,Vfloat[3],Vfloat[3],Vint*);
VKI_EXTERN void NearClipLine(Vint,Vfloat[2][3],Vfloat[2][3],Vint,Vfloat*,
Vfloat*,Vint*);VKI_EXTERN void NearClipTriangle(Vint,Vfloat[3][3],Vint*,Vfloat[][3],Vint,
Vfloat*,Vfloat*,Vint*);VKI_EXTERN void Project(Vfloat,Vfloat,Vfloat,Vfloat*,Vfloat*,Vfloat*);
VKI_EXTERN void Unproject(Vfloat,Vfloat,Vfloat,Vfloat*,Vfloat*,Vfloat*);
VKI_EXTERN void InitDrawFun(vgl_DrawFun*);VKI_EXTERN
void Print();
#endif
};
#ifdef __cplusplus
extern "C" { 
#endif
VKI_EXTERN vgl_Xfmstack* vgl_XfmstackBegin(void);VKI_EXTERN
void vgl_Xfmstack_Construct(vgl_Xfmstack *p);VKI_EXTERN
void vgl_XfmstackEnd(vgl_Xfmstack *p);VKI_EXTERN void
vgl_Xfmstack_Destruct(vgl_Xfmstack *p);VKI_EXTERN Vint
vgl_XfmstackError(vgl_Xfmstack *p);VKI_EXTERN void vgl_XfmstackPush(vgl_Xfmstack
*p);VKI_EXTERN void vgl_XfmstackPop(vgl_Xfmstack *p);
VKI_EXTERN void vgl_XfmstackLoad(vgl_Xfmstack *p,vgl_Xfm
*xfm);VKI_EXTERN void vgl_XfmstackMult(vgl_Xfmstack
*p,vgl_Xfm *xfm);VKI_EXTERN void vgl_XfmstackProj(vgl_Xfmstack
*p,vgl_Xfm *xfm);VKI_EXTERN void vgl_XfmstackSetProjLimits(vgl_Xfmstack
*p,Vfloat left,Vfloat right,Vfloat bottom,Vfloat top,
Vfloat nearz,Vfloat farz);VKI_EXTERN void vgl_XfmstackGetProjLimits(vgl_Xfmstack
*p,Vfloat *left,Vfloat *right,Vfloat *bottom,Vfloat
*top,Vfloat *nearz,Vfloat *farz);VKI_EXTERN void vgl_XfmstackSetProjType(vgl_Xfmstack
*p,Vint ptype);VKI_EXTERN void vgl_XfmstackGetProjType(vgl_Xfmstack
*p,Vint *ptype);VKI_EXTERN void vgl_XfmstackProjPop(vgl_Xfmstack
*p);VKI_EXTERN void vgl_XfmstackProjPush(vgl_Xfmstack
*p);VKI_EXTERN void vgl_XfmstackGetNum(vgl_Xfmstack
*p,Vint *numxfm);VKI_EXTERN void vgl_XfmstackSetViewport(vgl_Xfmstack
*p,Vint left,Vint right,Vint bottom,Vint top);VKI_EXTERN
void vgl_XfmstackGetViewport(vgl_Xfmstack *p,Vint *left,
Vint *right,Vint *bottom,Vint *top);VKI_EXTERN void
vgl_XfmstackSetClipPlane(vgl_Xfmstack *p,Vint index,
Vfloat eqn[4]);VKI_EXTERN void vgl_XfmstackGetClipPlaneEye(vgl_Xfmstack
*p,Vint index,Vfloat eqn[4]);VKI_EXTERN void vgl_XfmstackSetSwitch(vgl_Xfmstack
*p,Vint type,Vint index,Vint flag);VKI_EXTERN void vgl_XfmstackClipPlanePoint(vgl_Xfmstack
*p,Vfloat x[3],Vint *ivis);VKI_EXTERN void vgl_XfmstackGetMatrix(vgl_Xfmstack
*p,Vfloat matrix4x4[4][4]);VKI_EXTERN void vgl_XfmstackGetMatrixModelView(vgl_Xfmstack
*p,Vfloat matrix4x4[4][4]);VKI_EXTERN void vgl_XfmstackGetMatrixProjection(vgl_Xfmstack
*p,Vfloat matrix4x4[4][4]);VKI_EXTERN void vgl_XfmstackGetMatrixMVPInverse(vgl_Xfmstack
*p,Vfloat matrix4x4[4][4]);VKI_EXTERN void vgl_XfmstackVertex(vgl_Xfmstack
*p,Vfloat x[3],Vfloat xp[3]);VKI_EXTERN void vgl_XfmstackVertexW(vgl_Xfmstack
*p,Vfloat x[3],Vfloat xp[3],Vfloat *w);VKI_EXTERN void
vgl_XfmstackUnVertex(vgl_Xfmstack *p,Vfloat xp[3],Vfloat
x[3]);VKI_EXTERN void vgl_XfmstackVertexModelView(vgl_Xfmstack
*p,Vfloat x[3],Vfloat xm[3]);VKI_EXTERN void vgl_XfmstackNormalModelView(vgl_Xfmstack
*p,Vfloat v[3],Vfloat vp[3]);VKI_EXTERN void vgl_XfmstackNearClipPoint(vgl_Xfmstack
*p,Vint idev,Vfloat x[3],Vfloat xp[3],Vint *ivis);VKI_EXTERN
void vgl_XfmstackNearClipLine(vgl_Xfmstack *p,Vint idev,
Vfloat x[2][3],Vfloat xp[2][3],Vint ns,Vfloat *s,Vfloat
*sp,Vint *ivis);VKI_EXTERN void vgl_XfmstackNearClipTriangle(vgl_Xfmstack
*p,Vint idev,Vfloat x[3][3],Vint *nr,Vfloat xp[][3],
Vint ns,Vfloat *s,Vfloat *sp,Vint *ivis);VKI_EXTERN
void vgl_XfmstackProject(vgl_Xfmstack *p,Vfloat wx,
Vfloat wy,Vfloat wz,Vfloat *dx,Vfloat *dy,Vfloat *dz);
VKI_EXTERN void vgl_XfmstackUnproject(vgl_Xfmstack *p,
Vfloat dx,Vfloat dy,Vfloat dz,Vfloat *wx,Vfloat *wy,
Vfloat *wz);VKI_EXTERN void vgl_XfmstackInitDrawFun(vgl_Xfmstack
*p,vgl_DrawFun *df);VKI_EXTERN void vgl_XfmstackPrint(vgl_Xfmstack
*p);
#ifdef __cplusplus
}
#endif
#endif

