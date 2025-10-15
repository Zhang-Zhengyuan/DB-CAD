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
 
#ifndef RMODE_DEF
#define RMODE_DEF
#include "base/basedefs.h"
#include "vgl/xfmstack.h"
#define RMODE_POINTS 1
#define RMODE_LINES 2
#define RMODE_POLYGON 3
typedef struct vgl_RMode { Vint ierr;Vint type;Vint
ibffl;Vint issfl;Vint iprim;Vint dinprims,dinrws;Vint
dimax;Vint *dataindex;Vfloat hitmindepth,hitmaxdepth;
vgl_Xfmstack *xfmstack;Vint pointsize;Vint linewidth;
Vint hitgeom;struct { Vint lft,rgt;Vint top,btm;}rect;
Vint polysize;Vfloat (*poly)[3];Vint numhits;Vint numbbox;
Vfloat bbox[6];Vfloat left,right,bottom,top,nearz,farz;
Vfloat xsl,ysl;Vfloat xrt,yrt;Vint bfrsize;Vint bfrcurr;
Vint bfrnrws;Vint overflow;Vint *indexlist;Vfloat *mindepth;
Vfloat *maxdepth;Vint mpts;Vfloat *fx;Vfloat znear,
zfar;Vfloat zsel[2];}vgl_RMode;
#ifdef __cplusplus
extern "C" { 
#endif
VKI_EXTERN vgl_RMode* vgl_RModeBegin(void);VKI_EXTERN
void vgl_RModeEnd(vgl_RMode *p);VKI_EXTERN Vint vgl_RModeError(vgl_RMode
*p);VKI_EXTERN void vgl_RModeSetMode(vgl_RMode *p,Vint
mode,Vint flag);VKI_EXTERN void vgl_RModeSetObject(vgl_RMode
*p,Vint objecttype,Vobject *object);VKI_EXTERN void
vgl_RModeType(vgl_RMode *p,Vint type);VKI_EXTERN void
vgl_RModeExtentQuery(vgl_RMode *p,Vfloat *lft,Vfloat
*rgt,Vfloat *btm,Vfloat *top,Vfloat *nearz,Vfloat *farz);
VKI_EXTERN void vgl_RModeSelectQuery(vgl_RMode *p,Vint
*hits);VKI_EXTERN void vgl_RModeSelectBuffer(vgl_RMode
*p,Vint size,Vint *indexlist,Vfloat *mindepth,Vfloat
*maxdepth);VKI_EXTERN void vgl_RModeSelectRegion(vgl_RMode
*p,Vint type,Vint params[]);VKI_EXTERN void vgl_RModeClear(vgl_RMode
*p);VKI_EXTERN void vgl_RModeIndex(vgl_RMode *p,Vint
nprims,Vint nrws,Vint *index);VKI_EXTERN void vgl_RModePointSize(vgl_RMode
*p,Vint pointsize);VKI_EXTERN void vgl_RModeLineWidth(vgl_RMode
*p,Vint linewidth);VKI_EXTERN void vgl_RModePolyPoint(vgl_RMode
*p,Vint npts,Vfloat x[][3]);VKI_EXTERN void vgl_RModePolyLine(vgl_RMode
*p,Vint type,Vint npts,Vfloat x[][3]);VKI_EXTERN void
vgl_RModePolygon(vgl_RMode *p,Vint type,Vint npts,Vfloat
x[][3]);VKI_EXTERN void vgl_RModePolyPointDC(vgl_RMode
*p,Vint npts,Vfloat x[3],Vint dc[][3]);VKI_EXTERN void
vgl_RModePolyLineDC(vgl_RMode *p,Vint type,Vint npts,
Vfloat x[3],Vint dc[][3]);VKI_EXTERN void vgl_RModePolygonDC(vgl_RMode
*p,Vint type,Vint npts,Vfloat x[3],Vint dc[][3]);
#ifdef __cplusplus
}
#endif
#endif

