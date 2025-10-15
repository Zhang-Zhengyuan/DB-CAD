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
 
#ifndef GDIDEV_DEF
#define GDIDEV_DEF
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
#ifdef VKI_WIND_WIN32
#include <windows.h>
#include <tchar.h>
#endif
#include <stdio.h>
#include <math.h>
struct vgl_GDIDev { Vint ierr;Vfunc2 *funmon;Vobject
*funobjmon;Vint iinit,irast,ivect,nrast,nvect;Vint reflsfact;
#ifdef VKI_WIND_WIN32
int isxpix,isypix;float sxcm,sycm;int depth;int depth_bits;
int xorig,yorig;int xsize,ysize;int visflag;float xlencm,
ylencm;float xgupcm,ygupcm;int p1x,p1y,p2x,p2y;float
fzmin,fzmax;float right,left,bottom,top;float nearz,
farz;int pxmn,pxmx,pymn,pymx;int pleft,ptop,pright,
pbottom;int cxmn,cxmx,cymn,cymx;float fzmn,fzmx;Vfloat
projsx,projsy,factsz;Vint projtype;Vint open;Vint connect;
int rendermode;float zpfact,zpbias;int pgrend; int idbfl;
int ifbfl;int ifrfl;int izbfl;int truecolor;int iltfl;
int idmfl;int ixorm;int ipdfl;int ibffl;int icpfl;int
ildfl;int ieqfl;int ibtfl;int ilntfl;int itsfl;int izwfl;
int irffl;int itafl;int itdfl;int issfl;int itpsfl;
int izbfn;int lsfact;int istfl;int palmfl;int lalmfl;
 int dbufhard;int zbufhard;int tcolhard;int overplanes;
int litehard;int winsystem;int polyflag;int dcflag;
int planflag;int plantemp;int ibtflsav;int textureindsav;
float path[3];float plane[3];float pixelsize;int ibldon;
int iliton;float znear;float zfar;int itrpat;int itrans;
float transp;int pointsize;int linestyle;int linewidth;
float intensity,shininess;int rasfontind;int textureind;
int bitmapind;int numraster;int numvector;float forecrgba[4];
int primalphaflag;float primalpha[3];int primtexturenum;
float primtexture[4];float currnorm[3];COLORREF forecref;
COLORREF scancref;float backcrgb[3];COLORREF backcref;
HBRUSH backbrush;float ctm[4][4];HPEN forepen;COLORREF
crefpen;DWORD reflwidth;Vint reflstyle;DWORD refmode;
HBRUSH forebrush;COLORREF crefbrush;HFONT hfont;LOGFONT
logfont;vgl_Bitmap *stipmask[8];vgl_Bitmap *linemask[VGL_LINESTYLE_MAX];
vgl_ZBuffer *zbuffer;vgl_DrawFun *drawfun;vgl_Xfm *xfmtmp;
vgl_Xfmstack *xfmstack;vgl_DispFun *dispfun;vgl_LModel
*lmodel;vgl_Material *material;vsy_List *lightlist;
vgl_Attstack *attstack;vgl_RasFont *rasfontcur;Vint
fonttextureindexcur;vsy_HashTable *rasfonthash;vsy_HashTable
*texturehash;vsy_HashTable *bitmaphash;vgl_RMode *rmode;
Vuint stenwritemask;Vint stenfuncmode;Vint stenfuncref;
Vuint stenfuncmask;Vint stenopfail;Vint stenopzfail;
Vint stenopzpass;Vint parflag;Vint pixflag;Vint genflag;
Vint strflag;Vint scpflag;Vint mpts;Vfloat (*cp)[3];
POINT *pt;Vfloat (*x)[3]; WNDCLASS wc;HWND parent;HWND
win;HDC hdc;HPALETTE palette;DWORD dwStyle;HMENU childid;
_TCHAR AppName[33];
#endif
#ifdef __cplusplus
public: VKI_EXTERN vgl_GDIDev(void);VKI_EXTERN void*
operator new (size_t);VKI_EXTERN ~vgl_GDIDev(void);
VKI_EXTERN void operator delete (void*);VKI_EXTERN Vint
Error();VKI_EXTERN void SetFunction(Vint,Vfunc*,Vobject*);
VKI_EXTERN void PositionWindow(Vint,Vint,Vint,Vint);
VKI_EXTERN void VisualWindow(Vint);VKI_EXTERN void ParentWindow(Vword);
VKI_EXTERN void ConfigureWindow(Vint,Vint*);VKI_EXTERN
void OpenWindow(const Vchar*);VKI_EXTERN void CloseWindow();
VKI_EXTERN void ConnectWindow(Vword);VKI_EXTERN void
DisconnectWindow();VKI_EXTERN void SetWindow();VKI_EXTERN
void QueryWindow(Vword*);VKI_EXTERN void Clear();VKI_EXTERN
void Swap();VKI_EXTERN void Flush();VKI_EXTERN void
Bell();VKI_EXTERN void Delay(Vfloat);VKI_EXTERN void
Resize();VKI_EXTERN void BackColorIndex(Vint);VKI_EXTERN
void BackColor(Vfloat[3]);VKI_EXTERN void SetMode(Vint,Vint);
VKI_EXTERN void SetFactors(Vint,Vfloat[]);VKI_EXTERN
void GetMode(Vint,Vint*);VKI_EXTERN void GetInteger(Vint,Vint*);
VKI_EXTERN void GetFloat(Vint,Vfloat*);VKI_EXTERN void
GetString(Vint,Vchar[]);VKI_EXTERN void FBufferRead(Vint,Vint,Vint,Vint,vgl_FBuffer*);
VKI_EXTERN void FBufferWrite(Vint,Vint,Vint,Vint,vgl_FBuffer*);
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
void Text(Vfloat[3],Vtchar*);VKI_EXTERN void TextDC(Vfloat[3],Vint[3],Vtchar*);
VKI_EXTERN void Specularity(Vfloat,Vfloat);VKI_EXTERN
void DataIndex(Vint,Vint,Vint*);VKI_EXTERN void Data(Vint,Vfloat[]);
VKI_EXTERN void PolyPointData(Vint,Vfloat[][3],Vint,Vfloat[]);
VKI_EXTERN void PolyLineData(Vint,Vint,Vfloat[][3],Vint,Vfloat[]);
VKI_EXTERN void PolygonData(Vint,Vint,Vfloat[][3],Vint,Vfloat*,Vint,
Vfloat[]);VKI_EXTERN void Render(Vint);VKI_EXTERN void
SelectRegion(Vint,Vint*);VKI_EXTERN void SelectBuffer(Vint,Vint*,Vfloat*,Vfloat*);
VKI_EXTERN void SelectQuery(Vint*);VKI_EXTERN void ExtentQuery(Vfloat*,Vfloat*,Vfloat*,Vfloat*,Vfloat*,Vfloat*);
VKI_EXTERN void BufferSize(Vint,Vint);VKI_EXTERN void
BufferRow(Vint);VKI_EXTERN void BufferQuery(vgl_FBuffer*);
VKI_EXTERN void DrawFun(vgl_DrawFun*);VKI_EXTERN void
DispFun(vgl_DispFun*);
#endif
};
#ifdef __cplusplus
extern "C" { 
#endif
VKI_EXTERN vgl_GDIDev* vgl_GDIDevBegin(void);VKI_EXTERN
void vgl_GDIDev_Construct(vgl_GDIDev *p);VKI_EXTERN
void vgl_GDIDevEnd(vgl_GDIDev *p);VKI_EXTERN void vgl_GDIDev_Destruct(vgl_GDIDev
*p);VKI_EXTERN Vint vgl_GDIDevError(vgl_GDIDev *p);
VKI_EXTERN void vgl_GDIDevSetFunction(vgl_GDIDev *p,
Vint functype,Vfunc *function,Vobject *object);VKI_EXTERN
void vgl_GDIDevTestWIN(Vint *flag);
#ifdef VKI_WIND_WIN32
VKI_EXTERN void vgl_GDIDevConnectWIN(void);
#endif
#ifndef VKI_WIND_WIN32
VKI_EXTERN void vgl_GDIDevConnectWIN(void);
#endif
VKI_EXTERN void vgl_GDIDevDisconnect(void);VKI_EXTERN
void vgl_GDIDevPositionWindow(vgl_GDIDev *p,Vint xorig,
Vint yorig,Vint xsize,Vint ysize);VKI_EXTERN void vgl_GDIDevVisualWindow(vgl_GDIDev
*p,Vint visflag);VKI_EXTERN void vgl_GDIDevParentWindow(vgl_GDIDev
*p,Vword parent);VKI_EXTERN void vgl_GDIDevConfigureWindow(vgl_GDIDev
*p,Vint oper,Vint *params);VKI_EXTERN void vgl_GDIDevOpenWindow(vgl_GDIDev
*p,const Vchar *title);VKI_EXTERN void vgl_GDIDevCloseWindow(vgl_GDIDev
*p);VKI_EXTERN void vgl_GDIDevConnectWindow(vgl_GDIDev
*p,Vword window);VKI_EXTERN void vgl_GDIDevDisconnectWindow(vgl_GDIDev
*p);VKI_EXTERN void vgl_GDIDevSetWindow(vgl_GDIDev *p);
VKI_EXTERN void vgl_GDIDevQueryWindow(vgl_GDIDev *p,
Vword *window);VKI_EXTERN void vgl_GDIDevClear(vgl_GDIDev
*p);VKI_EXTERN void vgl_GDIDevSwap(vgl_GDIDev *p);VKI_EXTERN
void vgl_GDIDevFlush(vgl_GDIDev *p);VKI_EXTERN void
vgl_GDIDevBell(vgl_GDIDev *p);VKI_EXTERN void vgl_GDIDevDelay(vgl_GDIDev
*p,Vfloat sec);VKI_EXTERN void vgl_GDIDevResize(vgl_GDIDev
*p);VKI_EXTERN void vgl_GDIDevBackColorIndex(vgl_GDIDev
*p,Vint index);VKI_EXTERN void vgl_GDIDevBackColor(vgl_GDIDev
*p,Vfloat c[3]);VKI_EXTERN void vgl_GDIDevSetMode(vgl_GDIDev
*p,Vint mode,Vint flag);VKI_EXTERN void vgl_GDIDevSetFactors(vgl_GDIDev
*p,Vint type,Vfloat factors[]);VKI_EXTERN void vgl_GDIDevGetMode(vgl_GDIDev
*p,Vint mode,Vint *flag);VKI_EXTERN void vgl_GDIDevGetInteger(vgl_GDIDev
*p,Vint type,Vint *params);VKI_EXTERN void vgl_GDIDevGetFloat(vgl_GDIDev
*p,Vint type,Vfloat *params);VKI_EXTERN void vgl_GDIDevGetString(vgl_GDIDev
*p,Vint type,Vchar params[]);VKI_EXTERN void vgl_GDIDevFBufferRead(vgl_GDIDev
*p,Vint left,Vint right,Vint bottom,Vint top,vgl_FBuffer
*fbuffer);VKI_EXTERN void vgl_GDIDevFBufferWrite(vgl_GDIDev
*p,Vint left,Vint right,Vint bottom,Vint top,vgl_FBuffer
*fbuffer);VKI_EXTERN void vgl_GDIDevZBufferRead(vgl_GDIDev
*p,Vint left,Vint right,Vint bottom,Vint top,vgl_ZBuffer
*zbuffer);VKI_EXTERN void vgl_GDIDevZBufferWrite(vgl_GDIDev
*p,Vint left,Vint right,Vint bottom,Vint top,vgl_ZBuffer
*zbuffer);VKI_EXTERN void vgl_GDIDevPixelmapCreate(vgl_GDIDev
*p,vgl_Pixelmap *pixelmap);VKI_EXTERN void vgl_GDIDevPixelmapDestroy(vgl_GDIDev
*p,vgl_Pixelmap *pixelmap);VKI_EXTERN void vgl_GDIDevPixelmapRead(vgl_GDIDev
*p,vgl_Pixelmap *pixelmap);VKI_EXTERN void vgl_GDIDevPixelmapWrite(vgl_GDIDev
*p,vgl_Pixelmap *pixelmap);VKI_EXTERN void vgl_GDIDevBitmapDefine(vgl_GDIDev
*p,Vint index,vgl_Bitmap *bitmap);VKI_EXTERN void vgl_GDIDevBitmapSelect(vgl_GDIDev
*p,Vint index);VKI_EXTERN void vgl_GDIDevTextureDefine(vgl_GDIDev
*p,Vint index,vgl_Texture *texture);VKI_EXTERN void
vgl_GDIDevTextureSelect(vgl_GDIDev *p,Vint index);VKI_EXTERN
void vgl_GDIDevRasFontDefine(vgl_GDIDev *p,Vint index,
vgl_RasFont *rasfont);VKI_EXTERN void vgl_GDIDevRasFontSelect(vgl_GDIDev
*p,Vint index);VKI_EXTERN void vgl_GDIDevClipPlane(vgl_GDIDev
*p,Vint index,Vfloat eqn[4]);VKI_EXTERN void vgl_GDIDevSetSwitch(vgl_GDIDev
*p,Vint type,Vint index,Vint flag);VKI_EXTERN void vgl_GDIDevLight(vgl_GDIDev
*p,Vint index,Vint type,Vfloat c[3],Vfloat x[3]);VKI_EXTERN
void vgl_GDIDevLightSwitch(vgl_GDIDev *p,Vint index,
Vint flag);VKI_EXTERN void vgl_GDIDevLightModel(vgl_GDIDev
*p,Vint localviewer,Vfloat koffset,Vfloat krate);VKI_EXTERN
void vgl_GDIDevProjOrtho(vgl_GDIDev *p,Vfloat left,
Vfloat right,Vfloat bottom,Vfloat top,Vfloat nearz,
Vfloat farz);VKI_EXTERN void vgl_GDIDevProjFrustum(vgl_GDIDev
*p,Vfloat left,Vfloat right,Vfloat bottom,Vfloat top,
Vfloat nearz,Vfloat farz);VKI_EXTERN void vgl_GDIDevProjPush(vgl_GDIDev
*p);VKI_EXTERN void vgl_GDIDevProjPop(vgl_GDIDev *p);
VKI_EXTERN void vgl_GDIDevProjLoad(vgl_GDIDev *p,Vfloat
xfm[4][4]);VKI_EXTERN void vgl_GDIDevViewport(vgl_GDIDev
*p,Vint left,Vint right,Vint bottom,Vint top);VKI_EXTERN
void vgl_GDIDevDepthRange(vgl_GDIDev *p,Vfloat nearz,
Vfloat farz);VKI_EXTERN void vgl_GDIDevPolygonOffset(vgl_GDIDev
*p,Vfloat factor,Vfloat bias);VKI_EXTERN void vgl_GDIDevPolygonMode(vgl_GDIDev
*p,Vint rend);VKI_EXTERN void vgl_GDIDevTextPlane(vgl_GDIDev
*p,Vfloat path[3],Vfloat plane[3]);VKI_EXTERN void vgl_GDIDevTextPixelSize(vgl_GDIDev
*p,Vfloat pixelsize);VKI_EXTERN void vgl_GDIDevPollMouse(vgl_GDIDev
*p,Vint *px,Vint *py,Vint *but1,Vint *but2,Vint *but3);
VKI_EXTERN void vgl_GDIDevWarpMouse(vgl_GDIDev *p,Vint
px,Vint py);VKI_EXTERN void vgl_GDIDevSetCursor(vgl_GDIDev
*p,Vint type);VKI_EXTERN void vgl_GDIDevPollModifiers(vgl_GDIDev
*p,Vint *cntl,Vint *shft);VKI_EXTERN void vgl_GDIDevReadQueue(vgl_GDIDev
*p,Vint *dev,Vint *val);VKI_EXTERN void vgl_GDIDevTestQueue(vgl_GDIDev
*p,Vint *dev);VKI_EXTERN void vgl_GDIDevResetQueue(vgl_GDIDev
*p);VKI_EXTERN void vgl_GDIDevPolyPoint(vgl_GDIDev *p,
Vint npts,Vfloat x[][3]);VKI_EXTERN void vgl_GDIDevPolyPointColor(vgl_GDIDev
*p,Vint npts,Vfloat x[][3],Vfloat c[][3]);VKI_EXTERN
void vgl_GDIDevPolyPointDC(vgl_GDIDev *p,Vint npts,
Vfloat x[3],Vint dc[][3]);VKI_EXTERN void vgl_GDIDevPolyLine(vgl_GDIDev
*p,Vint type,Vint npts,Vfloat x[][3]);VKI_EXTERN void
vgl_GDIDevPolyLineColor(vgl_GDIDev *p,Vint type,Vint
npts,Vfloat x[][3],Vfloat c[][3]);VKI_EXTERN void vgl_GDIDevPolyLineDC(vgl_GDIDev
*p,Vint type,Vint npts,Vfloat x[3],Vint dc[][3]);VKI_EXTERN
void vgl_GDIDevColorIndex(vgl_GDIDev *p,Vint index);
VKI_EXTERN void vgl_GDIDevColor(vgl_GDIDev *p,Vfloat
c[3]);VKI_EXTERN void vgl_GDIDevPolygon(vgl_GDIDev *p,
Vint type,Vint npts,Vfloat x[][3],Vint vtflag,Vfloat
vt[]);VKI_EXTERN void vgl_GDIDevPolygonColor(vgl_GDIDev
*p,Vint type,Vint npts,Vfloat x[][3],Vfloat c[][3],
Vint vtflag,Vfloat vt[]);VKI_EXTERN void vgl_GDIDevPolygonDC(vgl_GDIDev
*p,Vint type,Vint npts,Vfloat x[3],Vint dc[][3]);VKI_EXTERN
void vgl_GDIDevPolyPointArray(vgl_GDIDev *p,Vint npts,
Vfloat x[][3],Vint cflag,void *c,Vint vflag,void *v,
Vint tflag,Vfloat *t,Vint dflag,Vfloat *d);VKI_EXTERN
void vgl_GDIDevPolyLineArray(vgl_GDIDev *p,Vint type,
Vint npts,Vfloat x[][3],Vint cflag,void *c,Vint vflag,
void *v,Vint tflag,Vfloat *t,Vint dflag,Vfloat *d);
VKI_EXTERN void vgl_GDIDevPolygonArray(vgl_GDIDev *p,
Vint type,Vint npts,Vfloat x[][3],Vint cflag,void *c,
Vint vflag,void *v,Vint tflag,Vfloat *t,Vint dflag,
Vfloat *d);VKI_EXTERN void vgl_GDIDevInitBuffer(vgl_GDIDev
*p,Vint size,Vint *vboid);VKI_EXTERN void vgl_GDIDevTermBuffer(vgl_GDIDev
*p,Vint vboid);VKI_EXTERN void vgl_GDIDevCopyBuffer(vgl_GDIDev
*p,Vint vboid,Vint npts,Vfloat x[][3],Vint cflag,void
*c,Vint vflag,void *v,Vint tflag,Vfloat *t,Vint dflag,
Vfloat *d,Vint *offset);VKI_EXTERN void vgl_GDIDevPolyPointBuffer(vgl_GDIDev
*p,Vint vboid,Vint offset,Vint npts,Vint cflag,Vint
vflag,Vint tflag,Vint dflag);VKI_EXTERN void vgl_GDIDevPolyLineBuffer(vgl_GDIDev
*p,Vint vboid,Vint offset,Vint type,Vint npts,Vint cflag,
Vint vflag,Vint tflag,Vint dflag);VKI_EXTERN void vgl_GDIDevPolygonBuffer(vgl_GDIDev
*p,Vint vboid,Vint offset,Vint type,Vint npts,Vint cflag,
Vint vflag,Vint tflag,Vint dflag);VKI_EXTERN void vgl_GDIDevXfmPush(vgl_GDIDev
*p);VKI_EXTERN void vgl_GDIDevXfmPop(vgl_GDIDev *p);
VKI_EXTERN void vgl_GDIDevXfmMult(vgl_GDIDev *p,Vfloat
xfm[4][4]);VKI_EXTERN void vgl_GDIDevXfmLoad(vgl_GDIDev
*p,Vfloat xfm[4][4]);VKI_EXTERN void vgl_GDIDevAttPush(vgl_GDIDev
*p,Vint mask);VKI_EXTERN void vgl_GDIDevAttPop(vgl_GDIDev
*p);VKI_EXTERN void vgl_GDIDevPointSize(vgl_GDIDev *p,
Vint psize);VKI_EXTERN void vgl_GDIDevLineStyle(vgl_GDIDev
*p,Vint lstyle);VKI_EXTERN void vgl_GDIDevLineWidth(vgl_GDIDev
*p,Vint lwidth);VKI_EXTERN void vgl_GDIDevTrans(vgl_GDIDev
*p,Vfloat transp);VKI_EXTERN void vgl_GDIDevTransIndex(vgl_GDIDev
*p,Vint index);VKI_EXTERN void vgl_GDIDevClip(vgl_GDIDev
*p,Vint left,Vint right,Vint bottom,Vint top);VKI_EXTERN
void vgl_GDIDevText(vgl_GDIDev *p,Vfloat x[3],Vtchar
*string);VKI_EXTERN void vgl_GDIDevTextDC(vgl_GDIDev
*p,Vfloat x[3],Vint dc[3],Vtchar *string);VKI_EXTERN
void vgl_GDIDevSpecularity(vgl_GDIDev *p,Vfloat intensity,
Vfloat shininess);VKI_EXTERN void vgl_GDIDevDataIndex(vgl_GDIDev
*p,Vint nprims,Vint nrws,Vint *index);VKI_EXTERN void
vgl_GDIDevData(vgl_GDIDev *p,Vint nrws,Vfloat d[]);
VKI_EXTERN void vgl_GDIDevPolyPointData(vgl_GDIDev *p,
Vint npts,Vfloat x[][3],Vint nrws,Vfloat d[]);VKI_EXTERN
void vgl_GDIDevPolyLineData(vgl_GDIDev *p,Vint type,
Vint npts,Vfloat x[][3],Vint nrws,Vfloat d[]);VKI_EXTERN
void vgl_GDIDevPolygonData(vgl_GDIDev *p,Vint type,
Vint npts,Vfloat x[][3],Vint nrws,Vfloat *d,Vint vtflag,
Vfloat vt[]);VKI_EXTERN void vgl_GDIDevRender(vgl_GDIDev
*p,Vint type);VKI_EXTERN void vgl_GDIDevSelectRegion(vgl_GDIDev
*p,Vint type,Vint *params);VKI_EXTERN void vgl_GDIDevSelectBuffer(vgl_GDIDev
*p,Vint size,Vint *indexlist,Vfloat *mindepth,Vfloat
*maxdepth);VKI_EXTERN void vgl_GDIDevSelectQuery(vgl_GDIDev
*p,Vint *hits);VKI_EXTERN void vgl_GDIDevExtentQuery(vgl_GDIDev
*p,Vfloat *lft,Vfloat *rgt,Vfloat *btm,Vfloat *top,
Vfloat *znear,Vfloat *zfar);VKI_EXTERN void vgl_GDIDevBufferSize(vgl_GDIDev
*p,Vint xsize,Vint ysize);VKI_EXTERN void vgl_GDIDevBufferRow(vgl_GDIDev
*p,Vint irow);VKI_EXTERN void vgl_GDIDevBufferQuery(vgl_GDIDev
*p,vgl_FBuffer *fbuffer);VKI_EXTERN void vgl_GDIDevDrawFun(vgl_GDIDev
*p,vgl_DrawFun *df);VKI_EXTERN void vgl_GDIDevDispFun(vgl_GDIDev
*p,vgl_DispFun *df);
#ifdef __cplusplus
}
#endif
#define GDIsetpoly(p,flag)if((flag)!= (p)->polyflag)\
{ vgl_GDIDev_setpoly ((p),(flag));}
#define GDIsetdc(p,flag)if((flag)!= (p)->dcflag){ vgl_GDIDev_setdc\
((p),(flag));}
#define GDIsetplan(p,flag)if((flag)!= (p)->planflag)\
{ vgl_GDIDev_setplan ((p),(flag));}
#endif

