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
 
#ifndef ZBUFFER_DEF
#define ZBUFFER_DEF
#include "base/basedefs.h"
#include "vgl/vgldefs.h"
#include "vgl/bitmap.h"
#include "vgl/rasfont.h"
#include "vgl/dispfun.h"
#define ZBUFFER_LINESTYLE 1
#define ZBUFFER_PATTERN 2
#define ZBUFFER_LINESTYLEFACTOR 3
struct vgl_ZBuffer { Vint ierr;Vint xsize,ysize;Vint
left,right;Vint bottom,top;Vfloat farz,nearz;Vint vd[2];
Vdouble vz;Vint cxmin,cxmax,cymin,cymax;Vdouble zfact,
zbias;Vint pointsize;Vint linewidth;Vint linestyleflag;
Vint linestylefact;vgl_Bitmap *linestylerast;Vshort
*linestyle;int linestylebit;int linestylefnc;Vint patternflag;
vgl_Bitmap *patternrast;Vuint pattern[32];vgl_RasFont
*deffont;vgl_RasFont *usrfont;vgl_RasFont *rasfont;
Vint zbuffermode;Vint backfacemode;Vint clipmode;Vint
linedrawmode;Vint polygondepthmode;Vint writemode;Vint
alphatestmode;Vint textanchormode;Vint textdirectionmode;
Vint sbuffermode; Vint maxeqn;Vint eqnflag[VGL_MAXCLIPPLANE];
Vfloat eqn[VGL_MAXCLIPPLANE][4];Vfloat ptmi[4][4];Vint
funcmode;double *zbufd;int *xl,*xr;double *zl,*zr;float
*cl,*cr;Vint (*xy)[2];Vint mpts;unsigned char *sbufi;
Vuint stenwritemask;Vint stenfuncmode;Vint stenfuncref;
Vuint stenfuncmask;Vint stenopfail;Vint stenopzfail;
Vint stenopzpass;vgl_DispFun *df;
#ifdef __cplusplus
public: VKI_EXTERN vgl_ZBuffer(void);VKI_EXTERN void*
operator new (size_t);VKI_EXTERN ~vgl_ZBuffer(void);
VKI_EXTERN void operator delete (void*);VKI_EXTERN Vint
Error();VKI_EXTERN void Def(Vint,Vint);VKI_EXTERN void
Inq(Vint*,Vint*);VKI_EXTERN void Viewport(Vint,Vint,Vint,Vint);
VKI_EXTERN void DepthRange(Vfloat,Vfloat);VKI_EXTERN
void PolygonOffset(Vfloat,Vfloat);VKI_EXTERN void Clear();
VKI_EXTERN void SetMode(Vint,Vint);VKI_EXTERN void SetClip(Vint,Vint,Vint,Vint);
VKI_EXTERN void ClipPlane(Vint,Vfloat[4]);VKI_EXTERN
void ClipPlaneSwitch(Vint,Vint);VKI_EXTERN void Projection(Vfloat[4][4]);
VKI_EXTERN void SetPointSize(Vint);VKI_EXTERN void SetLineWidth(Vint);
VKI_EXTERN void SetParami(Vint,Vint);VKI_EXTERN void
SetObject(Vint,Vobject*);VKI_EXTERN void InitLine();
VKI_EXTERN void Point(Vfloat[3]);VKI_EXTERN void PointColor(Vfloat[3],Vfloat[4]);
VKI_EXTERN void PointData(Vfloat[3],Vint,Vfloat*);VKI_EXTERN
void Line(Vfloat[3],Vfloat[3]);VKI_EXTERN void LineColor(Vfloat[3],Vfloat[3],Vfloat[4],Vfloat[4]);
VKI_EXTERN void LineData(Vfloat[3],Vfloat[3],Vint,Vfloat*,Vfloat*);
VKI_EXTERN void PolyLine(Vint,Vfloat[][3]);VKI_EXTERN
void Polygon(Vint,Vfloat[][3]);VKI_EXTERN void PolygonColor(Vint,Vfloat[][3],Vfloat[][4]);
VKI_EXTERN void PolygonData(Vint,Vfloat[][3],Vint,Vfloat*);
VKI_EXTERN void Text(Vfloat[3],Vtchar*);VKI_EXTERN void
SetDepth(Vint,Vint,Vfloat);VKI_EXTERN void GetDepth(Vint,Vint,Vfloat*);
VKI_EXTERN void LoadDepth(Vdouble[]);VKI_EXTERN void
SaveDepth(Vdouble[]);VKI_EXTERN void Print();
#endif
};
#ifdef __cplusplus
extern "C" { 
#endif
VKI_EXTERN vgl_ZBuffer* vgl_ZBufferBegin(void);VKI_EXTERN
void vgl_ZBuffer_Construct(vgl_ZBuffer *p);VKI_EXTERN
void vgl_ZBufferEnd(vgl_ZBuffer *p);VKI_EXTERN void
vgl_ZBuffer_Destruct(vgl_ZBuffer *p);VKI_EXTERN Vint
vgl_ZBufferError(vgl_ZBuffer *p);VKI_EXTERN void vgl_ZBufferDef(vgl_ZBuffer
*p,Vint xsize,Vint ysize);VKI_EXTERN void vgl_ZBufferInq(vgl_ZBuffer
*p,Vint *xsize,Vint *ysize);VKI_EXTERN void vgl_ZBufferViewport(vgl_ZBuffer
*p,Vint left,Vint right,Vint bot,Vint top);VKI_EXTERN
void vgl_ZBufferDepthRange(vgl_ZBuffer *p,Vfloat nearz,
Vfloat farz);VKI_EXTERN void vgl_ZBufferPolygonOffset(vgl_ZBuffer
*p,Vfloat zfact,Vfloat zbias);VKI_EXTERN void vgl_ZBufferClear(vgl_ZBuffer
*p);VKI_EXTERN void vgl_ZBufferSetMode(vgl_ZBuffer *p,
Vint mode,Vint flag);VKI_EXTERN void vgl_ZBufferSetClip(vgl_ZBuffer
*p,Vint left,Vint right,Vint bot,Vint top);VKI_EXTERN
void vgl_ZBufferClipPlane(vgl_ZBuffer *p,Vint index,
Vfloat eqn[4]);VKI_EXTERN void vgl_ZBufferClipPlaneSwitch(vgl_ZBuffer
*p,Vint index,Vint flag);VKI_EXTERN void vgl_ZBufferProjection(vgl_ZBuffer
*p,Vfloat tm[4][4]);VKI_EXTERN void vgl_ZBufferSetPointSize(vgl_ZBuffer
*p,Vint pointsize);VKI_EXTERN void vgl_ZBufferSetLineWidth(vgl_ZBuffer
*p,Vint linewidth);VKI_EXTERN void vgl_ZBufferSetParami(vgl_ZBuffer
*p,Vint type,Vint iparam);VKI_EXTERN void vgl_ZBufferSetObject(vgl_ZBuffer
*p,Vint objecttype,Vobject *object);VKI_EXTERN void
vgl_ZBufferInitLine(vgl_ZBuffer *p);VKI_EXTERN void
vgl_ZBufferPoint(vgl_ZBuffer *p,Vfloat x[3]);VKI_EXTERN
void vgl_ZBufferPointColor(vgl_ZBuffer *p,Vfloat x[3],
Vfloat c[4]);VKI_EXTERN void vgl_ZBufferPointData(vgl_ZBuffer
*p,Vfloat x[3],Vint nrws,Vfloat *d);VKI_EXTERN void
vgl_ZBufferLine(vgl_ZBuffer *p,Vfloat xa[3],Vfloat xb[3]);
VKI_EXTERN void vgl_ZBufferLineColor(vgl_ZBuffer *p,
Vfloat xa[3],Vfloat xb[3],Vfloat ca[4],Vfloat cb[4]);
VKI_EXTERN void vgl_ZBufferLineData(vgl_ZBuffer *p,
Vfloat xa[3],Vfloat xb[3],Vint nrws,Vfloat *da,Vfloat
*db);VKI_EXTERN void vgl_ZBufferPolyLine(vgl_ZBuffer
*p,Vint npts,Vfloat x[][3]);VKI_EXTERN void vgl_ZBufferPolygon(vgl_ZBuffer
*p,Vint n,Vfloat x[][3]);VKI_EXTERN void vgl_ZBufferPolygonColor(vgl_ZBuffer
*p,Vint n,Vfloat x[][3],Vfloat c[][4]);VKI_EXTERN void
vgl_ZBufferPolygonData(vgl_ZBuffer *p,Vint n,Vfloat
x[][3],Vint nrws,Vfloat *d);VKI_EXTERN void vgl_ZBufferText(vgl_ZBuffer
*p,Vfloat x[3],Vtchar *text);VKI_EXTERN void vgl_ZBufferSetDepth(vgl_ZBuffer
*p,Vint dx,Vint dy,Vfloat z);VKI_EXTERN void vgl_ZBufferGetDepth(vgl_ZBuffer
*p,Vint dx,Vint dy,Vfloat *z);VKI_EXTERN void vgl_ZBufferLoadDepth(vgl_ZBuffer
*p,Vdouble z[]);VKI_EXTERN void vgl_ZBufferSaveDepth(vgl_ZBuffer
*p,Vdouble z[]);VKI_EXTERN void vgl_ZBufferPrint(vgl_ZBuffer
*p);
#ifdef __cplusplus
}
#endif
#endif

