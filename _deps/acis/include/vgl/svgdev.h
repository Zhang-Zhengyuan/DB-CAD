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
 
#ifndef SVGDEV_DEF
#define SVGDEV_DEF
#include "base/basedefs.h"
#include "vgl/vgldefs.h"
#include "vgl/bitmap.h"
#include "vgl/zbuffer.h"
#include "vgl/fbuffer.h"
#include "vgl/pixelmap.h"
#include "vgl/drawfun.h"
#include "vgl/xfm.h"
#include "vgl/xfmstack.h"
#include "vgl/dispfun.h"
#include "vgl/lmodel.h"
#include "vgl/material.h"
#include "vgl/light.h"
#include "vgl/attstack.h"
#include "vgl/rasfont.h"
#include "vgl/texture.h"
#include "vgl/rmode.h"
#include "vgl/dlist.h"
#ifdef VKI_WIND_WIN32
#include <windows.h>
#include <tchar.h>
#endif
#include <stdio.h>
#include <math.h>
struct vgl_SVGDev { Vint ierr;Vfunc2 *funmon;Vobject
*funobjmon;Vint iinit,irast,ivect,nrast,nvect;Vint reflsfact;
vgl_DList *dlist;vgl_DrawFun *df;vgl_DrawFun *dfdl;
Vint nfiles;Vchar filename[SYS_MAXPATHCHAR];Vchar extension[SYS_MAXPATHCHAR];
Vchar strokedasharray[80];Vchar strokewidth[80];Vchar
linestroke[80];Vchar tristroke[80];Vchar textstroke[80];
Vchar op[80];FILE *fd;Vint ndefs;int isxpix,isypix;
float sxcm,sycm;int depth;int depth_bits;int xorig,
yorig;int xsize,ysize;int visflag;float xlencm,ylencm;
float xgupcm,ygupcm;int p1x,p1y,p2x,p2y;float fzmin,
fzmax;float right,left,bottom,top;float nearz,farz;
int pxmn,pxmx,pymn,pymx;int pleft,ptop,pright,pbottom;
float fzmn,fzmx;Vfloat projsx,projsy,factsz;Vint projtype;
Vint open;Vint connect;int rendermode;float zpfact,
zpbias;int pgrend; int idbfl;int ifbfl;int ifrfl;int
izbfl;int truecolor;int iltfl;int idmfl;int ixorm;int
ipdfl;int ibffl;int icpfl;int ildfl;int ieqfl;int ibtfl;
int ilntfl;int itsfl;int izwfl;int irffl;int itafl;
int itdfl;int issfl;int itpsfl;int izbfn;int lsfact;
int istfl;int palmfl;int lalmfl; int dbufhard;int zbufhard;
int tcolhard;int overplanes;int litehard;int winsystem;
int polyflag;int dcflag;int planflag;int plantemp;int
ibtflsav;int textureindsav;float pixelsize;int ibldon;
int iliton;float znear;float zfar;int itrpat;int itrans;
float transp;int pointsize;int linestyle;int linewidth;
float intensity,shininess;int rasfontind;int textureind;
int bitmapind;int numraster;int numvector;float forecrgba[4];
int primalphaflag;float primalpha[3];int primtexturenum;
float primtexture[4];float currnorm[3];float backcrgb[4];
float ctm[4][4];Vint reflstyle;vgl_Bitmap *stipmask[8];
vgl_Bitmap *linemask[VGL_LINESTYLE_MAX];vgl_ZBuffer
*zbuffer;vgl_DrawFun *drawfun;vgl_Xfm *xfmtmp;vgl_Xfmstack
*xfmstack;vgl_DispFun *dispfun;vgl_LModel *lmodel;vgl_Material
*material;vsy_List *lightlist;vgl_Attstack *attstack;
vgl_RasFont *rasfontcur;Vint fonttextureindexcur;vsy_HashTable
*rasfonthash;vsy_HashTable *texturehash;vsy_HashTable
*bitmaphash;vgl_RMode *rmode;Vuint stenwritemask;Vint
stenfuncmode;Vint stenfuncref;Vuint stenfuncmask;Vint
stenopfail;Vint stenopzfail;Vint stenopzpass;Vint parflag;
Vint pixflag;Vint genflag;Vint strflag;Vint scpflag;
vgl_Texture *texture;vsy_ADTree *adtree;Vint clipid;
Vint cxmn;Vint cxmx;Vint cymn;Vint cymx;vgl_DrawFun
*dfbuf;Vint npts;Vfloat (*cp)[3];Vfloat (*xb)[3];Vchar
*cb;Vchar *vb;Vfloat *tb;Vfloat *db;
#ifdef __cplusplus
public: VKI_EXTERN vgl_SVGDev(void);VKI_EXTERN void*
operator new (size_t);VKI_EXTERN ~vgl_SVGDev(void);
VKI_EXTERN void operator delete (void*);VKI_EXTERN Vint
Error();VKI_EXTERN void SetFunction(Vint,Vfunc*,Vobject*);
VKI_EXTERN void PositionWindow(Vint,Vint,Vint,Vint);
VKI_EXTERN void VisualWindow(Vint);VKI_EXTERN void ParentWindow(Vword);
VKI_EXTERN void ConfigureWindow(Vint,Vint*);VKI_EXTERN
void OpenWindow(const Vchar[]);VKI_EXTERN void CloseWindow();
VKI_EXTERN void ConnectWindow(Vword);VKI_EXTERN void
DisconnectWindow();VKI_EXTERN void SetWindow();VKI_EXTERN
void QueryWindow(Vword*);VKI_EXTERN void Clear();VKI_EXTERN
void Swap();VKI_EXTERN void Flush();VKI_EXTERN void
Bell();VKI_EXTERN void Delay(Vfloat);VKI_EXTERN void
Resize();VKI_EXTERN void BackColorIndex(Vint);VKI_EXTERN
void BackColor(Vfloat[3]);VKI_EXTERN void BackTrans(Vfloat);
VKI_EXTERN void SetMode(Vint,Vint);VKI_EXTERN void SetFactors(Vint,Vfloat[]);
VKI_EXTERN void GetMode(Vint,Vint*);VKI_EXTERN void
GetInteger(Vint,Vint*);VKI_EXTERN void GetFloat(Vint,Vfloat*);
VKI_EXTERN void GetString(Vint,Vchar[]);VKI_EXTERN void
FBufferRead(Vint,Vint,Vint,Vint,vgl_FBuffer*);VKI_EXTERN
void FBufferWrite(Vint,Vint,Vint,Vint,vgl_FBuffer*);
VKI_EXTERN void ZBufferRead(Vint,Vint,Vint,Vint,vgl_ZBuffer*);
VKI_EXTERN void ZBufferWrite(Vint,Vint,Vint,Vint,vgl_ZBuffer*);
VKI_EXTERN void PixelmapCreate(vgl_Pixelmap*);VKI_EXTERN
void PixelmapDestroy(vgl_Pixelmap*);VKI_EXTERN void
PixelmapRead(vgl_Pixelmap*);VKI_EXTERN void PixelmapWrite(vgl_Pixelmap*);
VKI_EXTERN void BitmapDefine(Vint,vgl_Bitmap*);VKI_EXTERN
void BitmapSelect(Vint);VKI_EXTERN void TextureDefine(Vint,vgl_Texture*);
VKI_EXTERN void TextureSelect(Vint);VKI_EXTERN void
RasFontDefine(Vint,vgl_RasFont*);VKI_EXTERN void RasFontSelect(Vint);
VKI_EXTERN void ClipPlane(Vint,Vfloat[4]);VKI_EXTERN
void SetSwitch(Vint,Vint,Vint);VKI_EXTERN void Light(Vint,Vint,Vfloat[3],Vfloat[3]);
VKI_EXTERN void LightSwitch(Vint,Vint);VKI_EXTERN void
LightModel(Vint,Vfloat,Vfloat);VKI_EXTERN void ProjOrtho(Vfloat,Vfloat,Vfloat,Vfloat,Vfloat,Vfloat);
VKI_EXTERN void ProjFrustum(Vfloat,Vfloat,Vfloat,Vfloat,Vfloat,Vfloat);
VKI_EXTERN void ProjPush();VKI_EXTERN void ProjPop();
VKI_EXTERN void ProjLoad(Vfloat[4][4]);VKI_EXTERN void
Viewport(Vint,Vint,Vint,Vint);VKI_EXTERN void DepthRange(Vfloat,Vfloat);
VKI_EXTERN void PolygonOffset(Vfloat,Vfloat);VKI_EXTERN
void PolygonMode(Vint);VKI_EXTERN void TextPlane(Vfloat[3],Vfloat[3]);
VKI_EXTERN void TextPixelSize(Vfloat);VKI_EXTERN void
PollMouse(Vint*,Vint*,Vint*,Vint*,Vint*);VKI_EXTERN
void WarpMouse(Vint,Vint);VKI_EXTERN void SetCursor(Vint);
VKI_EXTERN void PollModifiers(Vint*,Vint*);VKI_EXTERN
void ReadQueue(Vint*,Vint*);VKI_EXTERN void TestQueue(Vint*);
VKI_EXTERN void ResetQueue();VKI_EXTERN void PolyPoint(Vint,Vfloat[][3]);
VKI_EXTERN void PolyPointColor(Vint,Vfloat[][3],Vfloat[][3]);
VKI_EXTERN void PolyPointDC(Vint,Vfloat[3],Vint[][3]);
VKI_EXTERN void PolyLine(Vint,Vint,Vfloat[][3]);VKI_EXTERN
void PolyLineColor(Vint,Vint,Vfloat[][3],Vfloat[][3]);
VKI_EXTERN void PolyLineDC(Vint,Vint,Vfloat[3],Vint[][3]);
VKI_EXTERN void ColorIndex(Vint);VKI_EXTERN void Color(Vfloat[3]);
VKI_EXTERN void Polygon(Vint,Vint,Vfloat[][3],Vint,Vfloat[]);
VKI_EXTERN void PolygonColor(Vint,Vint,Vfloat[][3],Vfloat[][3],Vint,
Vfloat[]);VKI_EXTERN void PolygonDC(Vint,Vint,Vfloat[3],Vint[][3]);
VKI_EXTERN void PolyPointArray(Vint,Vfloat[][3],Vint,void*,Vint,void*,Vint,
Vfloat*,Vint,Vfloat*);VKI_EXTERN void PolyLineArray(Vint,Vint,Vfloat[][3],Vint,void*,Vint,void*,
Vint,Vfloat*,Vint,Vfloat*);VKI_EXTERN void PolygonArray(Vint,Vint,Vfloat[][3],Vint,void*,Vint,void*,
Vint,Vfloat*,Vint,Vfloat*);VKI_EXTERN void InitBuffer(Vint,Vint*);
VKI_EXTERN void TermBuffer(Vint);VKI_EXTERN void CopyBuffer(Vint,Vint,Vfloat[][3],Vint,void*,Vint,void*,Vint,
Vfloat*,Vint,Vfloat*,Vint*);VKI_EXTERN void PolyPointBuffer(Vint,Vint,Vint,Vint,Vint,Vint,Vint);
VKI_EXTERN void PolyLineBuffer(Vint,Vint,Vint,Vint,Vint,Vint,Vint,Vint);
VKI_EXTERN void PolygonBuffer(Vint,Vint,Vint,Vint,Vint,Vint,Vint,Vint);
VKI_EXTERN void XfmPush();VKI_EXTERN void XfmPop();
VKI_EXTERN void XfmMult(Vfloat[4][4]);VKI_EXTERN void
XfmLoad(Vfloat[4][4]);VKI_EXTERN void AttPush(Vint);
VKI_EXTERN void AttPop();VKI_EXTERN void PointSize(Vint);
VKI_EXTERN void LineStyle(Vint);VKI_EXTERN void LineWidth(Vint);
VKI_EXTERN void Trans(Vfloat);VKI_EXTERN void TransIndex(Vint);
VKI_EXTERN void Clip(Vint,Vint,Vint,Vint);VKI_EXTERN
void TextDC(Vfloat[3],Vint[3],Vtchar*);VKI_EXTERN void
Text(Vfloat[3],Vtchar*);VKI_EXTERN void Specularity(Vfloat,Vfloat);
VKI_EXTERN void DataIndex(Vint,Vint,Vint*);VKI_EXTERN
void Data(Vint,Vfloat[]);VKI_EXTERN void PolyPointData(Vint,Vfloat[][3],Vint,Vfloat[]);
VKI_EXTERN void PolyLineData(Vint,Vint,Vfloat[][3],Vint,Vfloat[]);
VKI_EXTERN void PolygonData(Vint,Vint,Vfloat[][3],Vint,Vfloat*,Vint,
Vfloat[]);VKI_EXTERN void Render(Vint);VKI_EXTERN void
SelectRegion(Vint,Vint*);VKI_EXTERN void SelectBuffer(Vint,Vint*,Vfloat*,Vfloat*);
VKI_EXTERN void SelectQuery(Vint*);VKI_EXTERN void ExtentQuery(Vfloat*,Vfloat*,Vfloat*,Vfloat*,Vfloat*,Vfloat*);
VKI_EXTERN void BufferSize(Vint,Vint);VKI_EXTERN void
BufferRow(Vint);VKI_EXTERN void BufferQuery(vgl_FBuffer*);
VKI_EXTERN void DrawFun(vgl_DrawFun*);VKI_EXTERN void
SetTexture(vgl_Texture*);VKI_EXTERN void SetObject(Vint,Vobject*);
#endif
};
#ifdef __cplusplus
extern "C" { 
#endif
VKI_EXTERN vgl_SVGDev* vgl_SVGDevBegin(void);VKI_EXTERN
void vgl_SVGDev_Construct(vgl_SVGDev *p);VKI_EXTERN
void vgl_SVGDevEnd(vgl_SVGDev *p);VKI_EXTERN void vgl_SVGDev_Destruct(vgl_SVGDev
*p);VKI_EXTERN Vint vgl_SVGDevError(vgl_SVGDev *p);
VKI_EXTERN void vgl_SVGDevSetFunction(vgl_SVGDev *p,
Vint functype,Vfunc *function,Vobject *object);VKI_EXTERN
void vgl_SVGDevPositionWindow(vgl_SVGDev *p,Vint xorig,
Vint yorig,Vint xsize,Vint ysize);VKI_EXTERN void vgl_SVGDevVisualWindow(vgl_SVGDev
*p,Vint visflag);VKI_EXTERN void vgl_SVGDevParentWindow(vgl_SVGDev
*p,Vword parent);VKI_EXTERN void vgl_SVGDevConfigureWindow(vgl_SVGDev
*p,Vint oper,Vint *params);VKI_EXTERN void vgl_SVGDevOpenWindow(vgl_SVGDev
*p,const Vchar filename[]);VKI_EXTERN void vgl_SVGDevCloseWindow(vgl_SVGDev
*p);VKI_EXTERN void vgl_SVGDevConnectWindow(vgl_SVGDev
*p,Vword window);VKI_EXTERN void vgl_SVGDevDisconnectWindow(vgl_SVGDev
*p);VKI_EXTERN void vgl_SVGDevSetWindow(vgl_SVGDev *p);
VKI_EXTERN void vgl_SVGDevQueryWindow(vgl_SVGDev *p,
Vword *window);VKI_EXTERN void vgl_SVGDevClear(vgl_SVGDev
*p);VKI_EXTERN void vgl_SVGDevSwap(vgl_SVGDev *p);VKI_EXTERN
void vgl_SVGDevFlush(vgl_SVGDev *p);VKI_EXTERN void
vgl_SVGDevBell(vgl_SVGDev *p);VKI_EXTERN void vgl_SVGDevDelay(vgl_SVGDev
*p,Vfloat sec);VKI_EXTERN void vgl_SVGDevResize(vgl_SVGDev
*p);VKI_EXTERN void vgl_SVGDevBackColorIndex(vgl_SVGDev
*p,Vint index);VKI_EXTERN void vgl_SVGDevBackColor(vgl_SVGDev
*p,Vfloat c[3]);VKI_EXTERN void vgl_SVGDevBackTrans(vgl_SVGDev
*p,Vfloat transp);VKI_EXTERN void vgl_SVGDevSetMode(vgl_SVGDev
*p,Vint mode,Vint flag);VKI_EXTERN void vgl_SVGDevSetFactors(vgl_SVGDev
*p,Vint type,Vfloat factors[]);VKI_EXTERN void vgl_SVGDevGetMode(vgl_SVGDev
*p,Vint mode,Vint *flag);VKI_EXTERN void vgl_SVGDevGetInteger(vgl_SVGDev
*p,Vint type,Vint *params);VKI_EXTERN void vgl_SVGDevGetFloat(vgl_SVGDev
*p,Vint type,Vfloat *params);VKI_EXTERN void vgl_SVGDevGetString(vgl_SVGDev
*p,Vint type,Vchar params[]);VKI_EXTERN void vgl_SVGDevFBufferRead(vgl_SVGDev
*p,Vint left,Vint right,Vint bottom,Vint top,vgl_FBuffer
*fbuffer);VKI_EXTERN void vgl_SVGDevFBufferWrite(vgl_SVGDev
*p,Vint left,Vint right,Vint bottom,Vint top,vgl_FBuffer
*fbuffer);VKI_EXTERN void vgl_SVGDevZBufferRead(vgl_SVGDev
*p,Vint left,Vint right,Vint bottom,Vint top,vgl_ZBuffer
*zbuffer);VKI_EXTERN void vgl_SVGDevZBufferWrite(vgl_SVGDev
*p,Vint left,Vint right,Vint bottom,Vint top,vgl_ZBuffer
*zbuffer);VKI_EXTERN void vgl_SVGDevPixelmapCreate(vgl_SVGDev
*p,vgl_Pixelmap *pixelmap);VKI_EXTERN void vgl_SVGDevPixelmapDestroy(vgl_SVGDev
*p,vgl_Pixelmap *pixelmap);VKI_EXTERN void vgl_SVGDevPixelmapRead(vgl_SVGDev
*p,vgl_Pixelmap *pixelmap);VKI_EXTERN void vgl_SVGDevPixelmapWrite(vgl_SVGDev
*p,vgl_Pixelmap *pixelmap);VKI_EXTERN void vgl_SVGDevBitmapDefine(vgl_SVGDev
*p,Vint index,vgl_Bitmap *bitmap);VKI_EXTERN void vgl_SVGDevBitmapSelect(vgl_SVGDev
*p,Vint index);VKI_EXTERN void vgl_SVGDevTextureDefine(vgl_SVGDev
*p,Vint index,vgl_Texture *texture);VKI_EXTERN void
vgl_SVGDevTextureSelect(vgl_SVGDev *p,Vint index);VKI_EXTERN
void vgl_SVGDevRasFontDefine(vgl_SVGDev *p,Vint index,
vgl_RasFont *rasfont);VKI_EXTERN void vgl_SVGDevRasFontSelect(vgl_SVGDev
*p,Vint index);VKI_EXTERN void vgl_SVGDevClipPlane(vgl_SVGDev
*p,Vint index,Vfloat eqn[4]);VKI_EXTERN void vgl_SVGDevSetSwitch(vgl_SVGDev
*p,Vint type,Vint index,Vint flag);VKI_EXTERN void vgl_SVGDevLight(vgl_SVGDev
*p,Vint index,Vint type,Vfloat c[3],Vfloat x[3]);VKI_EXTERN
void vgl_SVGDevLightSwitch(vgl_SVGDev *p,Vint index,
Vint flag);VKI_EXTERN void vgl_SVGDevLightModel(vgl_SVGDev
*p,Vint localviewer,Vfloat koffset,Vfloat krate);VKI_EXTERN
void vgl_SVGDevProjOrtho(vgl_SVGDev *p,Vfloat left,
Vfloat right,Vfloat bottom,Vfloat top,Vfloat nearz,
Vfloat farz);VKI_EXTERN void vgl_SVGDevProjFrustum(vgl_SVGDev
*p,Vfloat left,Vfloat right,Vfloat bottom,Vfloat top,
Vfloat nearz,Vfloat farz);VKI_EXTERN void vgl_SVGDevProjPush(vgl_SVGDev
*p);VKI_EXTERN void vgl_SVGDevProjPop(vgl_SVGDev *p);
VKI_EXTERN void vgl_SVGDevProjLoad(vgl_SVGDev *p,Vfloat
xfm[4][4]);VKI_EXTERN void vgl_SVGDevViewport(vgl_SVGDev
*p,Vint left,Vint right,Vint bottom,Vint top);VKI_EXTERN
void vgl_SVGDevDepthRange(vgl_SVGDev *p,Vfloat nearz,
Vfloat farz);VKI_EXTERN void vgl_SVGDevPolygonOffset(vgl_SVGDev
*p,Vfloat factor,Vfloat bias);VKI_EXTERN void vgl_SVGDevPolygonMode(vgl_SVGDev
*p,Vint rend);VKI_EXTERN void vgl_SVGDevTextPlane(vgl_SVGDev
*p,Vfloat path[3],Vfloat plane[3]);VKI_EXTERN void vgl_SVGDevTextPixelSize(vgl_SVGDev
*p,Vfloat pixelsize);VKI_EXTERN void vgl_SVGDevPollMouse(vgl_SVGDev
*p,Vint *px,Vint *py,Vint *but1,Vint *but2,Vint *but3);
VKI_EXTERN void vgl_SVGDevWarpMouse(vgl_SVGDev *p,Vint
px,Vint py);VKI_EXTERN void vgl_SVGDevSetCursor(vgl_SVGDev
*p,Vint type);VKI_EXTERN void vgl_SVGDevPollModifiers(vgl_SVGDev
*p,Vint *cntl,Vint *shft);VKI_EXTERN void vgl_SVGDevReadQueue(vgl_SVGDev
*p,Vint *dev,Vint *val);VKI_EXTERN void vgl_SVGDevTestQueue(vgl_SVGDev
*p,Vint *dev);VKI_EXTERN void vgl_SVGDevResetQueue(vgl_SVGDev
*p);VKI_EXTERN void vgl_SVGDevPolyPoint(vgl_SVGDev *p,
Vint npts,Vfloat x[][3]);VKI_EXTERN void vgl_SVGDevPolyPointColor(vgl_SVGDev
*p,Vint npts,Vfloat x[][3],Vfloat c[][3]);VKI_EXTERN
void vgl_SVGDevPolyPointDC(vgl_SVGDev *p,Vint npts,
Vfloat x[3],Vint dc[][3]);VKI_EXTERN void vgl_SVGDevPolyLine(vgl_SVGDev
*p,Vint type,Vint npts,Vfloat x[][3]);VKI_EXTERN void
vgl_SVGDevPolyLineColor(vgl_SVGDev *p,Vint type,Vint
npts,Vfloat x[][3],Vfloat c[][3]);VKI_EXTERN void vgl_SVGDevPolyLineDC(vgl_SVGDev
*p,Vint type,Vint npts,Vfloat x[3],Vint dc[][3]);VKI_EXTERN
void vgl_SVGDevColorIndex(vgl_SVGDev *p,Vint index);
VKI_EXTERN void vgl_SVGDevColor(vgl_SVGDev *p,Vfloat
c[3]);VKI_EXTERN void vgl_SVGDevPolygon(vgl_SVGDev *p,
Vint type,Vint npts,Vfloat x[][3],Vint vtflag,Vfloat
vt[]);VKI_EXTERN void vgl_SVGDevPolygonColor(vgl_SVGDev
*p,Vint type,Vint npts,Vfloat x[][3],Vfloat c[][3],
Vint vtflag,Vfloat vt[]);VKI_EXTERN void vgl_SVGDevPolygonDC(vgl_SVGDev
*p,Vint type,Vint npts,Vfloat x[3],Vint dc[][3]);VKI_EXTERN
void vgl_SVGDevPolyPointArray(vgl_SVGDev *p,Vint npts,
Vfloat x[][3],Vint cflag,void *c,Vint vflag,void *v,
Vint tflag,Vfloat *t,Vint dflag,Vfloat *d);VKI_EXTERN
void vgl_SVGDevPolyLineArray(vgl_SVGDev *p,Vint type,
Vint npts,Vfloat x[][3],Vint cflag,void *c,Vint vflag,
void *v,Vint tflag,Vfloat *t,Vint dflag,Vfloat *d);
VKI_EXTERN void vgl_SVGDevPolygonArray(vgl_SVGDev *p,
Vint type,Vint npts,Vfloat x[][3],Vint cflag,void *c,
Vint vflag,void *v,Vint tflag,Vfloat *t,Vint dflag,
Vfloat *d);VKI_EXTERN void vgl_SVGDevInitBuffer(vgl_SVGDev
*p,Vint size,Vint *vboid);VKI_EXTERN void vgl_SVGDevTermBuffer(vgl_SVGDev
*p,Vint vboid);VKI_EXTERN void vgl_SVGDevCopyBuffer(vgl_SVGDev
*p,Vint vboid,Vint npts,Vfloat x[][3],Vint cflag,void
*c,Vint vflag,void *v,Vint tflag,Vfloat *t,Vint dflag,
Vfloat *d,Vint *offset);VKI_EXTERN void vgl_SVGDevPolyPointBuffer(vgl_SVGDev
*p,Vint vboid,Vint offset,Vint npts,Vint cflag,Vint
vflag,Vint tflag,Vint dflag);VKI_EXTERN void vgl_SVGDevPolyLineBuffer(vgl_SVGDev
*p,Vint vboid,Vint offset,Vint type,Vint npts,Vint cflag,
Vint vflag,Vint tflag,Vint dflag);VKI_EXTERN void vgl_SVGDevPolygonBuffer(vgl_SVGDev
*p,Vint vboid,Vint offset,Vint type,Vint npts,Vint cflag,
Vint vflag,Vint tflag,Vint dflag);VKI_EXTERN void vgl_SVGDevXfmPush(vgl_SVGDev
*p);VKI_EXTERN void vgl_SVGDevXfmPop(vgl_SVGDev *p);
VKI_EXTERN void vgl_SVGDevXfmMult(vgl_SVGDev *p,Vfloat
xfm[4][4]);VKI_EXTERN void vgl_SVGDevXfmLoad(vgl_SVGDev
*p,Vfloat xfm[4][4]);VKI_EXTERN void vgl_SVGDevAttPush(vgl_SVGDev
*p,Vint mask);VKI_EXTERN void vgl_SVGDevAttPop(vgl_SVGDev
*p);VKI_EXTERN void vgl_SVGDevPointSize(vgl_SVGDev *p,
Vint psize);VKI_EXTERN void vgl_SVGDevLineStyle(vgl_SVGDev
*p,Vint lstyle);VKI_EXTERN void vgl_SVGDevLineWidth(vgl_SVGDev
*p,Vint lwidth);VKI_EXTERN void vgl_SVGDevTrans(vgl_SVGDev
*p,Vfloat transp);VKI_EXTERN void vgl_SVGDevTransIndex(vgl_SVGDev
*p,Vint index);VKI_EXTERN void vgl_SVGDevClip(vgl_SVGDev
*p,Vint left,Vint right,Vint bottom,Vint top);VKI_EXTERN
void vgl_SVGDevTextDC(vgl_SVGDev *p,Vfloat x[3],Vint
dc[3],Vtchar *string);VKI_EXTERN void vgl_SVGDevText(vgl_SVGDev
*p,Vfloat x[3],Vtchar *string);VKI_EXTERN void vgl_SVGDevSpecularity(vgl_SVGDev
*p,Vfloat intensity,Vfloat shininess);VKI_EXTERN void
vgl_SVGDevDataIndex(vgl_SVGDev *p,Vint nprims,Vint nrws,
Vint *index);VKI_EXTERN void vgl_SVGDevData(vgl_SVGDev
*p,Vint nrws,Vfloat d[]);VKI_EXTERN void vgl_SVGDevPolyPointData(vgl_SVGDev
*p,Vint npts,Vfloat x[][3],Vint nrws,Vfloat d[]);VKI_EXTERN
void vgl_SVGDevPolyLineData(vgl_SVGDev *p,Vint type,
Vint npts,Vfloat x[][3],Vint nrws,Vfloat d[]);VKI_EXTERN
void vgl_SVGDevPolygonData(vgl_SVGDev *p,Vint type,
Vint npts,Vfloat x[][3],Vint nrws,Vfloat *d,Vint vtflag,
Vfloat vt[]);VKI_EXTERN void vgl_SVGDevRender(vgl_SVGDev
*p,Vint type);VKI_EXTERN void vgl_SVGDevSelectRegion(vgl_SVGDev
*p,Vint type,Vint *params);VKI_EXTERN void vgl_SVGDevSelectBuffer(vgl_SVGDev
*p,Vint size,Vint *indexlist,Vfloat *mindepth,Vfloat
*maxdepth);VKI_EXTERN void vgl_SVGDevSelectQuery(vgl_SVGDev
*p,Vint *hits);VKI_EXTERN void vgl_SVGDevExtentQuery(vgl_SVGDev
*p,Vfloat *lft,Vfloat *rgt,Vfloat *btm,Vfloat *top,
Vfloat *znear,Vfloat *zfar);VKI_EXTERN void vgl_SVGDevBufferSize(vgl_SVGDev
*p,Vint xsize,Vint ysize);VKI_EXTERN void vgl_SVGDevBufferRow(vgl_SVGDev
*p,Vint irow);VKI_EXTERN void vgl_SVGDevBufferQuery(vgl_SVGDev
*p,vgl_FBuffer *fbuffer);VKI_EXTERN void vgl_SVGDevDrawFun(vgl_SVGDev
*p,vgl_DrawFun *df);VKI_EXTERN void vgl_SVGDevSetTexture(vgl_SVGDev
*p,vgl_Texture *texture);VKI_EXTERN void vgl_SVGDevSetObject(vgl_SVGDev
*p,Vint type,Vobject *object);
#ifdef __cplusplus
}
#endif
#define SVGsetpoly(p,flag)if((flag)!= (p)->polyflag)\
{ vgl_SVGDev_setpoly ((p),(flag));}
#define SVGsetdc(p,flag)if((flag)!= (p)->dcflag){ vgl_SVGDev_setdc\
((p),(flag));}
#define SVGsetplan(p,flag)if((flag)!= (p)->planflag)\
{ vgl_SVGDev_setplan ((p),(flag));}
#endif

