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
 
#ifndef DOPT_DEF
#define DOPT_DEF
#include "base/basedefs.h"
#include "vgl/drawfun.h"
#define DOPT_MAXBUFFER 1
#define DOPT_DECOMPOSE 2
typedef struct vgl_DOpt_Set { Vint npts;Vfloat (*x)[3];
Vuchar (*c)[4];Vint vflag;Vschar (*v)[4];Vint tflag;
Vfloat *t;Vfloat (*cf)[4];Vfloat (*vf)[3];Vint npms;
Vint nrows;Vint diflag;Vint *di;Vint primclas;Vint primtype;
Vint cflag;Vfloat cs[4],transs;vgl_DrawFun *df;}vgl_DOpt_Set;
struct vgl_DOpt { Vint ierr;Vint flush;Vint cflag;Vfloat
c[4],trans;Vint curpointsize;Vint curlinewidth;Vint
curlinestyle;Vint curbitmapselect;Vint curtextureselect;
Vint currasfontselect;Vint curcolorindex;Vint curtransindex;
Vfloat curintensity,curshininess;Vint nprims;Vint nrows;
Vint curdataindex;vgl_DrawFun *df;Vint maxbuffer;Vint
decompose;vgl_DOpt_Set *polypoint;vgl_DOpt_Set *polypointcolor;
vgl_DOpt_Set *polyline;vgl_DOpt_Set *polylinecolor;
vgl_DOpt_Set *polygon;vgl_DOpt_Set *polygoncolor;vgl_DOpt_Set
*tristrip;vgl_DOpt_Set *tristripcolor;
#ifdef __cplusplus
public: VKI_EXTERN vgl_DOpt(void);VKI_EXTERN void* operator
new (size_t);VKI_EXTERN ~vgl_DOpt(void);VKI_EXTERN void
operator delete (void*);VKI_EXTERN Vint Error();VKI_EXTERN
void SetObject(Vint,Vobject*);VKI_EXTERN void SetParami(Vint,Vint);
VKI_EXTERN void Reset();VKI_EXTERN void XfmPush();VKI_EXTERN
void XfmPop();VKI_EXTERN void XfmLoad(Vfloat[4][4]);
VKI_EXTERN void XfmMult(Vfloat[4][4]);VKI_EXTERN void
ColorIndex(Vint);VKI_EXTERN void Color(Vfloat[3]);VKI_EXTERN
void DataIndex(Vint,Vint,Vint*);VKI_EXTERN void Data(Vint,Vfloat[]);
VKI_EXTERN void BackColorIndex(Vint);VKI_EXTERN void
BackColor(Vfloat[3]);VKI_EXTERN void AttPush(Vint);
VKI_EXTERN void AttPop();VKI_EXTERN void Polygon(Vint,Vint,Vfloat[][3],Vint,Vfloat[]);
VKI_EXTERN void PolygonColor(Vint,Vint,Vfloat[][3],Vfloat[][3],Vint,
Vfloat[]);VKI_EXTERN void PolygonDC(Vint,Vint,Vfloat[3],Vint[][3]);
VKI_EXTERN void PolygonData(Vint,Vint,Vfloat[][3],Vint,Vfloat[],Vint,
Vfloat[]);VKI_EXTERN void LineStyle(Vint);VKI_EXTERN
void LineWidth(Vint);VKI_EXTERN void TransIndex(Vint);
VKI_EXTERN void Trans(Vfloat);VKI_EXTERN void Specularity(Vfloat,Vfloat);
VKI_EXTERN void Clip(Vint,Vint,Vint,Vint);VKI_EXTERN
void Text(Vfloat[3],Vtchar*);VKI_EXTERN void TextDC(Vfloat[3],Vint[3],Vtchar*);
VKI_EXTERN void PolyPoint(Vint,Vfloat[][3]);VKI_EXTERN
void PolyPointColor(Vint,Vfloat[][3],Vfloat[][3]);VKI_EXTERN
void PolyPointDC(Vint,Vfloat[3],Vint[][3]);VKI_EXTERN
void PolyPointData(Vint,Vfloat[][3],Vint,Vfloat[]);
VKI_EXTERN void PolyLine(Vint,Vint,Vfloat[][3]);VKI_EXTERN
void PolyLineColor(Vint,Vint,Vfloat[][3],Vfloat[][3]);
VKI_EXTERN void PolyLineDC(Vint,Vint,Vfloat[3],Vint[][3]);
VKI_EXTERN void PolyLineData(Vint,Vint,Vfloat[][3],Vint,Vfloat[]);
VKI_EXTERN void SetMode(Vint,Vint);VKI_EXTERN void SetFactors(Vint,Vfloat[]);
VKI_EXTERN void PointSize(Vint);VKI_EXTERN void PositionWindow(Vint,Vint,Vint,Vint);
VKI_EXTERN void VisualWindow(Vint);VKI_EXTERN void ParentWindow(Vword);
VKI_EXTERN void ConfigureWindow(Vint,Vint*);VKI_EXTERN
void OpenWindow(const Vchar*);VKI_EXTERN void CloseWindow();
VKI_EXTERN void ConnectWindow(Vword);VKI_EXTERN void
DisconnectWindow();VKI_EXTERN void SetWindow();VKI_EXTERN
void QueryWindow(Vword*);VKI_EXTERN void Clear();VKI_EXTERN
void Swap();VKI_EXTERN void Flush();VKI_EXTERN void
Bell();VKI_EXTERN void Delay(Vfloat);VKI_EXTERN void
Resize();VKI_EXTERN void GetMode(Vint,Vint*);VKI_EXTERN
void GetInteger(Vint,Vint*);VKI_EXTERN void GetFloat(Vint,Vfloat*);
VKI_EXTERN void GetString(Vint,Vchar[]);VKI_EXTERN void
FBufferRead(Vint,Vint,Vint,Vint,Vobject*);VKI_EXTERN
void FBufferWrite(Vint,Vint,Vint,Vint,Vobject*);VKI_EXTERN
void ZBufferRead(Vint,Vint,Vint,Vint,Vobject*);VKI_EXTERN
void ZBufferWrite(Vint,Vint,Vint,Vint,Vobject*);VKI_EXTERN
void PixelmapCreate(Vobject*);VKI_EXTERN void PixelmapDestroy(Vobject*);
VKI_EXTERN void PixelmapRead(Vobject*);VKI_EXTERN void
PixelmapWrite(Vobject*);VKI_EXTERN void Light(Vint,Vint,Vfloat[3],Vfloat[3]);
VKI_EXTERN void LightSwitch(Vint,Vint);VKI_EXTERN void
LightModel(Vint,Vfloat,Vfloat);VKI_EXTERN void ClipPlane(Vint,Vfloat[4]);
VKI_EXTERN void SetSwitch(Vint,Vint,Vint);VKI_EXTERN
void ProjOrtho(Vfloat,Vfloat,Vfloat,Vfloat,Vfloat,Vfloat);
VKI_EXTERN void ProjFrustum(Vfloat,Vfloat,Vfloat,Vfloat,Vfloat,Vfloat);
VKI_EXTERN void ProjPush();VKI_EXTERN void ProjPop();
VKI_EXTERN void ProjLoad(Vfloat[4][4]);VKI_EXTERN void
Viewport(Vint,Vint,Vint,Vint);VKI_EXTERN void DepthRange(Vfloat,Vfloat);
VKI_EXTERN void PolygonOffset(Vfloat,Vfloat);VKI_EXTERN
void PolygonMode(Vint);VKI_EXTERN void TextPlane(Vfloat[3],Vfloat[3]);
VKI_EXTERN void TextPixelSize(Vfloat);VKI_EXTERN void
BitmapDefine(Vint,vgl_Bitmap*);VKI_EXTERN void BitmapSelect(Vint);
VKI_EXTERN void TextureDefine(Vint,vgl_Texture*);VKI_EXTERN
void TextureSelect(Vint);VKI_EXTERN void RasFontDefine(Vint,Vobject*);
VKI_EXTERN void RasFontSelect(Vint);VKI_EXTERN void
PollMouse(Vint*,Vint*,Vint*,Vint*,Vint*);VKI_EXTERN
void WarpMouse(Vint,Vint);VKI_EXTERN void SetCursor(Vint);
VKI_EXTERN void PollModifiers(Vint*,Vint*);VKI_EXTERN
void ReadQueue(Vint*,Vint*);VKI_EXTERN void TestQueue(Vint*);
VKI_EXTERN void ResetQueue();VKI_EXTERN void Render(Vint);
VKI_EXTERN void SelectRegion(Vint,Vint*);VKI_EXTERN
void SelectBuffer(Vint,Vint*,Vfloat*,Vfloat*);VKI_EXTERN
void SelectQuery(Vint*);VKI_EXTERN void ExtentQuery(Vfloat*,Vfloat*,Vfloat*,Vfloat*,Vfloat*,Vfloat*);
VKI_EXTERN void BufferSize(Vint,Vint);VKI_EXTERN void
BufferRow(Vint);VKI_EXTERN void BufferQuery(vgl_FBuffer*);
VKI_EXTERN void PolyPointArray(Vint,Vfloat[][3],Vint,void*,Vint,void*,Vint,
Vfloat*,Vint,Vfloat*);VKI_EXTERN void PolyLineArray(Vint,Vint,Vfloat[][3],Vint,void*,Vint,void*,
Vint,Vfloat*,Vint,Vfloat*);VKI_EXTERN void PolygonArray(Vint,Vint,Vfloat[][3],Vint,void*,Vint,void*,
Vint,Vfloat*,Vint,Vfloat*);VKI_EXTERN void InitBuffer(Vint,Vint*);
VKI_EXTERN void TermBuffer(Vint);VKI_EXTERN void CopyBuffer(Vint,Vint,Vfloat[][3],Vint,void*,Vint,void*,Vint,
Vfloat*,Vint,Vfloat*,Vint*);VKI_EXTERN void PolyPointBuffer(Vint,Vint,Vint,Vint,Vint,Vint,Vint);
VKI_EXTERN void PolyLineBuffer(Vint,Vint,Vint,Vint,Vint,Vint,Vint,Vint);
VKI_EXTERN void PolygonBuffer(Vint,Vint,Vint,Vint,Vint,Vint,Vint,Vint);
VKI_EXTERN void DrawFun(vgl_DrawFun*);
#endif
};
#ifdef __cplusplus
extern "C" { 
#endif
VKI_EXTERN vgl_DOpt* vgl_DOptBegin(void);VKI_EXTERN
void vgl_DOpt_Construct(vgl_DOpt *p);VKI_EXTERN void
vgl_DOptEnd(vgl_DOpt *p);VKI_EXTERN void vgl_DOpt_Destruct(vgl_DOpt
*p);VKI_EXTERN Vint vgl_DOptError(vgl_DOpt *p);VKI_EXTERN
void vgl_DOptSetObject(vgl_DOpt *p,Vint objecttype,
Vobject *object);VKI_EXTERN void vgl_DOptSetParami(vgl_DOpt
*p,Vint type,Vint iparam);VKI_EXTERN void vgl_DOptReset(vgl_DOpt
*p);VKI_EXTERN void vgl_DOptXfmPush(vgl_DOpt *p);VKI_EXTERN
void vgl_DOptXfmPop(vgl_DOpt *p);VKI_EXTERN void vgl_DOptXfmLoad(vgl_DOpt
*p,Vfloat xfm[4][4]);VKI_EXTERN void vgl_DOptXfmMult(vgl_DOpt
*p,Vfloat xfm[4][4]);VKI_EXTERN void vgl_DOptColorIndex(vgl_DOpt
*p,Vint index);VKI_EXTERN void vgl_DOptColor(vgl_DOpt
*p,Vfloat c[3]);VKI_EXTERN void vgl_DOptDataIndex(vgl_DOpt
*p,Vint nprims,Vint nrws,Vint *index);VKI_EXTERN void
vgl_DOptData(vgl_DOpt *p,Vint nrws,Vfloat d[]);VKI_EXTERN
void vgl_DOptBackColorIndex(vgl_DOpt *p,Vint index);
VKI_EXTERN void vgl_DOptBackColor(vgl_DOpt *p,Vfloat
c[3]);VKI_EXTERN void vgl_DOptAttPush(vgl_DOpt *p,Vint
mask);VKI_EXTERN void vgl_DOptAttPop(vgl_DOpt *p);VKI_EXTERN
void vgl_DOptPolygon(vgl_DOpt *p,Vint type,Vint npts,
Vfloat x[][3],Vint vtflag,Vfloat vt[]);VKI_EXTERN void
vgl_DOptPolygonColor(vgl_DOpt *p,Vint type,Vint npts,
Vfloat x[][3],Vfloat c[][3],Vint vtflag,Vfloat vt[]);
VKI_EXTERN void vgl_DOptPolygonDC(vgl_DOpt *p,Vint type,
Vint npts,Vfloat x[3],Vint dc[][3]);VKI_EXTERN void
vgl_DOptPolygonData(vgl_DOpt *p,Vint type,Vint npts,
Vfloat x[][3],Vint nrws,Vfloat d[],Vint vtflag,Vfloat
vt[]);VKI_EXTERN void vgl_DOptLineStyle(vgl_DOpt *p,
Vint lstyle);VKI_EXTERN void vgl_DOptLineWidth(vgl_DOpt
*p,Vint lwidth);VKI_EXTERN void vgl_DOptTransIndex(vgl_DOpt
*p,Vint index);VKI_EXTERN void vgl_DOptTrans(vgl_DOpt
*p,Vfloat trans);VKI_EXTERN void vgl_DOptSpecularity(vgl_DOpt
*p,Vfloat intensity,Vfloat shininess);VKI_EXTERN void
vgl_DOptClip(vgl_DOpt *p,Vint xmin,Vint xmax,Vint ymin,
Vint ymax);VKI_EXTERN void vgl_DOptText(vgl_DOpt *p,
Vfloat x[3],Vtchar *string);VKI_EXTERN void vgl_DOptTextDC(vgl_DOpt
*p,Vfloat x[3],Vint dc[3],Vtchar *string);VKI_EXTERN
void vgl_DOptPolyPoint(vgl_DOpt *p,Vint npts,Vfloat
x[][3]);VKI_EXTERN void vgl_DOptPolyPointColor(vgl_DOpt
*p,Vint npts,Vfloat x[][3],Vfloat c[][3]);VKI_EXTERN
void vgl_DOptPolyPointDC(vgl_DOpt *p,Vint npts,Vfloat
x[3],Vint dc[][3]);VKI_EXTERN void vgl_DOptPolyPointData(vgl_DOpt
*p,Vint npts,Vfloat x[][3],Vint nrws,Vfloat d[]);VKI_EXTERN
void vgl_DOptPolyLine(vgl_DOpt *p,Vint type,Vint npts,
Vfloat x[][3]);VKI_EXTERN void vgl_DOptPolyLineColor(vgl_DOpt
*p,Vint type,Vint npts,Vfloat x[][3],Vfloat c[][3]);
VKI_EXTERN void vgl_DOptPolyLineDC(vgl_DOpt *p,Vint
type,Vint npts,Vfloat x[3],Vint dc[][3]);VKI_EXTERN
void vgl_DOptPolyLineData(vgl_DOpt *p,Vint type,Vint
npts,Vfloat x[][3],Vint nrws,Vfloat d[]);VKI_EXTERN
void vgl_DOptSetMode(vgl_DOpt *p,Vint mode,Vint flag);
VKI_EXTERN void vgl_DOptSetFactors(vgl_DOpt *p,Vint
type,Vfloat factors[]);VKI_EXTERN void vgl_DOptPointSize(vgl_DOpt
*p,Vint psize);VKI_EXTERN void vgl_DOptPositionWindow(vgl_DOpt
*p,Vint xorig,Vint yorig,Vint xsize,Vint ysize);VKI_EXTERN
void vgl_DOptVisualWindow(vgl_DOpt *p,Vint visflag);
VKI_EXTERN void vgl_DOptParentWindow(vgl_DOpt *p,Vword
window);VKI_EXTERN void vgl_DOptConfigureWindow(vgl_DOpt
*p,Vint type,Vint *params);VKI_EXTERN void vgl_DOptOpenWindow(vgl_DOpt
*p,const Vchar *title);VKI_EXTERN void vgl_DOptCloseWindow(vgl_DOpt
*p);VKI_EXTERN void vgl_DOptConnectWindow(vgl_DOpt *p,
Vword window);VKI_EXTERN void vgl_DOptDisconnectWindow(vgl_DOpt
*p);VKI_EXTERN void vgl_DOptSetWindow(vgl_DOpt *p);
VKI_EXTERN void vgl_DOptQueryWindow(vgl_DOpt *p,Vword
*window);VKI_EXTERN void vgl_DOptClear(vgl_DOpt *p);
VKI_EXTERN void vgl_DOptSwap(vgl_DOpt *p);VKI_EXTERN
void vgl_DOptFlush(vgl_DOpt *p);VKI_EXTERN void vgl_DOptBell(vgl_DOpt
*p);VKI_EXTERN void vgl_DOptDelay(vgl_DOpt *p,Vfloat
sec);VKI_EXTERN void vgl_DOptResize(vgl_DOpt *p);VKI_EXTERN
void vgl_DOptGetMode(vgl_DOpt *p,Vint mode,Vint *flag);
VKI_EXTERN void vgl_DOptGetInteger(vgl_DOpt *p,Vint
type,Vint *params);VKI_EXTERN void vgl_DOptGetFloat(vgl_DOpt
*p,Vint type,Vfloat *params);VKI_EXTERN void vgl_DOptGetString(vgl_DOpt
*p,Vint type,Vchar params[]);VKI_EXTERN void vgl_DOptFBufferRead(vgl_DOpt
*p,Vint left,Vint right,Vint bottom,Vint top,Vobject
*fbuffer);VKI_EXTERN void vgl_DOptFBufferWrite(vgl_DOpt
*p,Vint left,Vint right,Vint bottom,Vint top,Vobject
*fbuffer);VKI_EXTERN void vgl_DOptZBufferRead(vgl_DOpt
*p,Vint left,Vint right,Vint bottom,Vint top,Vobject
*zbuffer);VKI_EXTERN void vgl_DOptZBufferWrite(vgl_DOpt
*p,Vint left,Vint right,Vint bottom,Vint top,Vobject
*zbuffer);VKI_EXTERN void vgl_DOptPixelmapCreate(vgl_DOpt
*p,Vobject *pixelmap);VKI_EXTERN void vgl_DOptPixelmapDestroy(vgl_DOpt
*p,Vobject *pixelmap);VKI_EXTERN void vgl_DOptPixelmapRead(vgl_DOpt
*p,Vobject *pixelmap);VKI_EXTERN void vgl_DOptPixelmapWrite(vgl_DOpt
*p,Vobject *pixelmap);VKI_EXTERN void vgl_DOptLight(vgl_DOpt
*p,Vint index,Vint type,Vfloat c[3],Vfloat x[3]);VKI_EXTERN
void vgl_DOptLightSwitch(vgl_DOpt *p,Vint index,Vint
flag);VKI_EXTERN void vgl_DOptLightModel(vgl_DOpt *p,
Vint localviewer,Vfloat koffset,Vfloat krate);VKI_EXTERN
void vgl_DOptClipPlane(vgl_DOpt *p,Vint index,Vfloat
eqn[4]);VKI_EXTERN void vgl_DOptSetSwitch(vgl_DOpt *p,
Vint type,Vint index,Vint flag);VKI_EXTERN void vgl_DOptProjOrtho(vgl_DOpt
*p,Vfloat left,Vfloat right,Vfloat bottom,Vfloat top,
Vfloat nearz,Vfloat farz);VKI_EXTERN void vgl_DOptProjFrustum(vgl_DOpt
*p,Vfloat left,Vfloat right,Vfloat bottom,Vfloat top,
Vfloat nearz,Vfloat farz);VKI_EXTERN void vgl_DOptProjPush(vgl_DOpt
*p);VKI_EXTERN void vgl_DOptProjPop(vgl_DOpt *p);VKI_EXTERN
void vgl_DOptProjLoad(vgl_DOpt *p,Vfloat xfm[4][4]);
VKI_EXTERN void vgl_DOptViewport(vgl_DOpt *p,Vint left,
Vint right,Vint bottom,Vint top);VKI_EXTERN void vgl_DOptDepthRange(vgl_DOpt
*p,Vfloat nearz,Vfloat farz);VKI_EXTERN void vgl_DOptPolygonOffset(vgl_DOpt
*p,Vfloat factor,Vfloat bias);VKI_EXTERN void vgl_DOptPolygonMode(vgl_DOpt
*p,Vint rend);VKI_EXTERN void vgl_DOptTextPlane(vgl_DOpt
*p,Vfloat path[3],Vfloat plane[3]);VKI_EXTERN void vgl_DOptTextPixelSize(vgl_DOpt
*p,Vfloat pixelsize);VKI_EXTERN void vgl_DOptBitmapDefine(vgl_DOpt
*p,Vint index,vgl_Bitmap *bitmap);VKI_EXTERN void vgl_DOptBitmapSelect(vgl_DOpt
*p,Vint index);VKI_EXTERN void vgl_DOptTextureDefine(vgl_DOpt
*p,Vint index,vgl_Texture *texture);VKI_EXTERN void
vgl_DOptTextureSelect(vgl_DOpt *p,Vint index);VKI_EXTERN
void vgl_DOptRasFontDefine(vgl_DOpt *p,Vint index,Vobject
*rasfont);VKI_EXTERN void vgl_DOptRasFontSelect(vgl_DOpt
*p,Vint index);VKI_EXTERN void vgl_DOptPollMouse(vgl_DOpt
*p,Vint *px,Vint *py,Vint *but1,Vint *but2,Vint *but3);
VKI_EXTERN void vgl_DOptWarpMouse(vgl_DOpt *p,Vint px,
Vint py);VKI_EXTERN void vgl_DOptSetCursor(vgl_DOpt
*p,Vint type);VKI_EXTERN void vgl_DOptPollModifiers(vgl_DOpt
*p,Vint *cntl,Vint *shft);VKI_EXTERN void vgl_DOptReadQueue(vgl_DOpt
*p,Vint *dev,Vint *val);VKI_EXTERN void vgl_DOptTestQueue(vgl_DOpt
*p,Vint *dev);VKI_EXTERN void vgl_DOptResetQueue(vgl_DOpt
*p);VKI_EXTERN void vgl_DOptRender(vgl_DOpt *p,Vint
type);VKI_EXTERN void vgl_DOptSelectRegion(vgl_DOpt
*p,Vint type,Vint *params);VKI_EXTERN void vgl_DOptSelectBuffer(vgl_DOpt
*p,Vint size,Vint *indexlist,Vfloat *mindepth,Vfloat
*maxdepth);VKI_EXTERN void vgl_DOptSelectQuery(vgl_DOpt
*p,Vint *hits);VKI_EXTERN void vgl_DOptExtentQuery(vgl_DOpt
*p,Vfloat *lft,Vfloat *rgt,Vfloat *btm,Vfloat *top,
Vfloat *znear,Vfloat *zfar);VKI_EXTERN void vgl_DOptBufferSize(vgl_DOpt
*p,Vint xsize,Vint ysize);VKI_EXTERN void vgl_DOptBufferRow(vgl_DOpt
*p,Vint irow);VKI_EXTERN void vgl_DOptBufferQuery(vgl_DOpt
*p,vgl_FBuffer *fbuffer);VKI_EXTERN void vgl_DOptPolyPointArray(vgl_DOpt
*p,Vint npts,Vfloat x[][3],Vint cflag,void *c,Vint vflag,
void *v,Vint tflag,Vfloat *t,Vint dflag,Vfloat *d);
VKI_EXTERN void vgl_DOptPolyLineArray(vgl_DOpt *p,Vint
type,Vint npts,Vfloat x[][3],Vint cflag,void *c,Vint
vflag,void *v,Vint tflag,Vfloat *t,Vint dflag,Vfloat
*d);VKI_EXTERN void vgl_DOptPolygonArray(vgl_DOpt *p,
Vint type,Vint npts,Vfloat x[][3],Vint cflag,void *c,
Vint vflag,void *v,Vint tflag,Vfloat *t,Vint dflag,
Vfloat *d);VKI_EXTERN void vgl_DOptInitBuffer(vgl_DOpt
*p,Vint size,Vint *vboid);VKI_EXTERN void vgl_DOptTermBuffer(vgl_DOpt
*p,Vint vboid);VKI_EXTERN void vgl_DOptCopyBuffer(vgl_DOpt
*p,Vint vboid,Vint npts,Vfloat x[][3],Vint cflag,void
*c,Vint vflag,void *v,Vint tflag,Vfloat *t,Vint dflag,
Vfloat *d,Vint *offset);VKI_EXTERN void vgl_DOptPolyPointBuffer(vgl_DOpt
*p,Vint vboid,Vint offset,Vint npts,Vint cflag,Vint
vflag,Vint tflag,Vint dflag);VKI_EXTERN void vgl_DOptPolyLineBuffer(vgl_DOpt
*p,Vint vboid,Vint offset,Vint type,Vint npts,Vint cflag,
Vint vflag,Vint tflag,Vint dflag);VKI_EXTERN void vgl_DOptPolygonBuffer(vgl_DOpt
*p,Vint vboid,Vint offset,Vint type,Vint npts,Vint cflag,
Vint vflag,Vint tflag,Vint dflag);VKI_EXTERN void vgl_DOptDrawFun(vgl_DOpt
*p,vgl_DrawFun *df);
#ifdef __cplusplus
}
#endif
#define DOptflush(dopt)if(dopt->flush){ vgl_DOpt_Flush\
((dopt));}
#endif

