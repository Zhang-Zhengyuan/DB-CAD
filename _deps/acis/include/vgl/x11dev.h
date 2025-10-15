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
 
#ifndef X11DEV_DEF
#define X11DEV_DEF
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
#ifdef VKI_WIND_X11
#include <X11/Xlib.h>
#include <X11/Xutil.h>
#ifndef XK_MISCELLANY
#define XK_MISCELLANY 1
#endif
#include <X11/keysymdef.h>
#endif
#include <stdio.h>
#include <math.h>
struct vgl_X11Dev { Vint ierr;Vfunc2 *funmon;Vobject
*funobjmon;Vint iinit,irast,ivect,nrast,nvect;
#ifdef VKI_WIND_X11
int isxpix,isypix;float sxcm,sycm;int depth;int depth_bits;
int xorig,yorig;int xsize,ysize;int visflag;float xlencm,
ylencm;float xgupcm,ygupcm;int p1x,p1y,p2x,p2y;float
fzmin,fzmax;float right,left,bottom,top;float nearz,
farz;int pxmn,pxmx,pymn,pymx;int cxmn,cxmx,cymn,cymx;
float fzmn,fzmx;Vfloat projsx,projsy,factsz;Vint projtype;
Vint open;Vint connect;int rendermode;float zpfact,
zpbias;int pgrend; int idbfl;int ifbfl;int ifrfl;int
izbfl;int truecolor;int iltfl;int idmfl;int ixorm;int
ipdfl;int ibffl;int icpfl;int ildfl;int ieqfl;int ibtfl;
int ilntfl;int itsfl;int izwfl;int irffl;int itafl;
int itdfl;int issfl;int itpsfl;int izbfn;int lsfact;
int istfl;int palmfl;int lalmfl;int iupfl; int dbufhard;
int zbufhard;int tcolhard;int overplanes;int litehard;
int winsystem;int polyflag;int dcflag;int planflag;
int plantemp;int textureindsav;float path[3];float plane[3];
float pixelsize;int ibldon;int iliton;float znear;float
zfar;int itrpat;int itrans;float transp;int pointsize;
int linestyle;int linewidth;float intensity,shininess;
int rasfontind;int textureind;int bitmapind;float forecrgba[4];
float backcrgb[3];float ctm[4][4];int primalphaflag;
float primalpha[3];int primtexturenum;float primtexture[4];
float currnorm[3];vgl_Bitmap *stipmask[8];vgl_Bitmap
*linemask[VGL_LINESTYLE_MAX];vgl_ZBuffer *zbuffer;vgl_DrawFun
*drawfun;vgl_Xfm *xfmtmp;vgl_Xfmstack *xfmstack;vgl_DispFun
*dispfun;vgl_LModel *lmodel;vgl_Material *material;
vsy_List *lightlist;vgl_Attstack *attstack;vgl_RasFont
*rasfontcur;Vchar rasfontname[256];XFontStruct *font_info;
Vint fonttextureindexcur;vsy_HashTable *rasfonthash;
vsy_HashTable *texturehash;vsy_HashTable *bitmaphash;
vgl_RMode *rmode;vgl_Pixelmap *fpixelmap;vgl_Pixelmap
*zpixelmap;Vuint stenwritemask;Vint stenfuncmode;Vint
stenfuncref;Vuint stenfuncmask;Vint stenopfail;Vint
stenopzfail;Vint stenopzpass;Vint mpts;Vfloat (*cp)[3];
Display *display;int screen;Window parent;Vint parflag;
Window win;Pixmap pix;Drawable drw;GC gc;Visual *visual;
Colormap cmap;unsigned long rgbmasks[3];Vfloat rgbrecip[3];
int clflag;XRectangle clrect;int clorigx,clorigy;
#endif
#ifdef __cplusplus
public: VKI_EXTERN vgl_X11Dev(void);VKI_EXTERN void*
operator new (size_t);VKI_EXTERN ~vgl_X11Dev(void);
VKI_EXTERN void operator delete (void*);VKI_EXTERN Vint
Error();VKI_EXTERN void SetFunction(Vint,Vfunc*,Vobject*);
#ifdef VKI_WIND_X11
VKI_EXTERN void BestVisualX(Visual**);
#endif
#ifndef VKI_WIND_X11
VKI_EXTERN void BestVisualX(void**);
#endif
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
VKI_EXTERN void SetFactors(Vint,Vfloat[]);VKI_EXTERN
void XfmPush();VKI_EXTERN void XfmPop();VKI_EXTERN void
XfmMult(Vfloat[4][4]);VKI_EXTERN void XfmLoad(Vfloat[4][4]);
VKI_EXTERN void AttPush(Vint);VKI_EXTERN void AttPop();
VKI_EXTERN void PointSize(Vint);VKI_EXTERN void LineStyle(Vint);
VKI_EXTERN void LineWidth(Vint);VKI_EXTERN void Trans(Vfloat);
VKI_EXTERN void TransIndex(Vint);VKI_EXTERN void Clip(Vint,Vint,Vint,Vint);
VKI_EXTERN void Text(Vfloat[3],Vtchar*);VKI_EXTERN void
TextDC(Vfloat[3],Vint[3],Vtchar*);VKI_EXTERN void Specularity(Vfloat,Vfloat);
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
DispFun(vgl_DispFun*);
#endif
};
#ifdef __cplusplus
extern "C" { 
#endif
VKI_EXTERN vgl_X11Dev* vgl_X11DevBegin(void);VKI_EXTERN
void vgl_X11Dev_Construct(vgl_X11Dev *p);VKI_EXTERN
void vgl_X11DevEnd(vgl_X11Dev *p);VKI_EXTERN void vgl_X11Dev_Destruct(vgl_X11Dev
*p);VKI_EXTERN Vint vgl_X11DevError(vgl_X11Dev *p);
VKI_EXTERN void vgl_X11DevSetFunction(vgl_X11Dev *p,
Vint functype,Vfunc *function,Vobject *object);VKI_EXTERN
void vgl_X11DevTestX(Vint *flag);
#ifdef VKI_WIND_X11
VKI_EXTERN void vgl_X11DevConnectX(Display *display,
int screen);
#endif
#ifndef VKI_WIND_X11
VKI_EXTERN void vgl_X11DevConnectX(void *display,int
screen);
#endif
VKI_EXTERN void vgl_X11DevDisconnect(void);
#ifdef VKI_WIND_X11
VKI_EXTERN void vgl_X11DevBestVisualX(vgl_X11Dev *p,
Visual **bestvisual);
#endif
#ifndef VKI_WIND_X11
VKI_EXTERN void vgl_X11DevBestVisualX(vgl_X11Dev *p,
void **bestvisual);
#endif
VKI_EXTERN void vgl_X11DevPositionWindow(vgl_X11Dev
*p,Vint xorig,Vint yorig,Vint xsize,Vint ysize);VKI_EXTERN
void vgl_X11DevVisualWindow(vgl_X11Dev *p,Vint visflag);
VKI_EXTERN void vgl_X11DevParentWindow(vgl_X11Dev *p,
Vword parent);VKI_EXTERN void vgl_X11DevConfigureWindow(vgl_X11Dev
*p,Vint oper,Vint *params);VKI_EXTERN void vgl_X11DevOpenWindow(vgl_X11Dev
*p,const Vchar *title);VKI_EXTERN void vgl_X11DevCloseWindow(vgl_X11Dev
*p);VKI_EXTERN void vgl_X11DevConnectWindow(vgl_X11Dev
*p,Vword window);VKI_EXTERN void vgl_X11DevDisconnectWindow(vgl_X11Dev
*p);VKI_EXTERN void vgl_X11DevSetWindow(vgl_X11Dev *p);
VKI_EXTERN void vgl_X11DevQueryWindow(vgl_X11Dev *p,
Vword *window);VKI_EXTERN void vgl_X11DevClear(vgl_X11Dev
*p);VKI_EXTERN void vgl_X11DevSwap(vgl_X11Dev *p);VKI_EXTERN
void vgl_X11DevFlush(vgl_X11Dev *p);VKI_EXTERN void
vgl_X11DevBell(vgl_X11Dev *p);VKI_EXTERN void vgl_X11DevDelay(vgl_X11Dev
*p,Vfloat sec);VKI_EXTERN void vgl_X11DevResize(vgl_X11Dev
*p);VKI_EXTERN void vgl_X11DevBackColorIndex(vgl_X11Dev
*p,Vint index);VKI_EXTERN void vgl_X11DevBackColor(vgl_X11Dev
*p,Vfloat c[3]);VKI_EXTERN void vgl_X11DevSetMode(vgl_X11Dev
*p,Vint mode,Vint flag);VKI_EXTERN void vgl_X11DevGetMode(vgl_X11Dev
*p,Vint mode,Vint *flag);VKI_EXTERN void vgl_X11DevGetInteger(vgl_X11Dev
*p,Vint type,Vint *params);VKI_EXTERN void vgl_X11DevGetFloat(vgl_X11Dev
*p,Vint type,Vfloat *params);VKI_EXTERN void vgl_X11DevGetString(vgl_X11Dev
*p,Vint type,Vchar params[]);VKI_EXTERN void vgl_X11DevFBufferRead(vgl_X11Dev
*p,Vint left,Vint right,Vint bottom,Vint top,vgl_FBuffer
*fbuffer);VKI_EXTERN void vgl_X11DevFBufferWrite(vgl_X11Dev
*p,Vint left,Vint right,Vint bottom,Vint top,vgl_FBuffer
*fbuffer);VKI_EXTERN void vgl_X11DevZBufferRead(vgl_X11Dev
*p,Vint left,Vint right,Vint bottom,Vint top,vgl_ZBuffer
*zbuffer);VKI_EXTERN void vgl_X11DevZBufferWrite(vgl_X11Dev
*p,Vint left,Vint right,Vint bottom,Vint top,vgl_ZBuffer
*zbuffer);VKI_EXTERN void vgl_X11DevPixelmapCreate(vgl_X11Dev
*p,vgl_Pixelmap *pixelmap);VKI_EXTERN void vgl_X11DevPixelmapDestroy(vgl_X11Dev
*p,vgl_Pixelmap *pixelmap);VKI_EXTERN void vgl_X11DevPixelmapRead(vgl_X11Dev
*p,vgl_Pixelmap *pixelmap);VKI_EXTERN void vgl_X11DevPixelmapWrite(vgl_X11Dev
*p,vgl_Pixelmap *pixelmap);VKI_EXTERN void vgl_X11DevBitmapDefine(vgl_X11Dev
*p,Vint index,vgl_Bitmap *bitmap);VKI_EXTERN void vgl_X11DevBitmapSelect(vgl_X11Dev
*p,Vint index);VKI_EXTERN void vgl_X11DevTextureDefine(vgl_X11Dev
*p,Vint index,vgl_Texture *texture);VKI_EXTERN void
vgl_X11DevTextureSelect(vgl_X11Dev *p,Vint index);VKI_EXTERN
void vgl_X11DevRasFontDefine(vgl_X11Dev *p,Vint index,
vgl_RasFont *rasfont);VKI_EXTERN void vgl_X11DevRasFontSelect(vgl_X11Dev
*p,Vint index);VKI_EXTERN void vgl_X11DevClipPlane(vgl_X11Dev
*p,Vint index,Vfloat eqn[4]);VKI_EXTERN void vgl_X11DevSetSwitch(vgl_X11Dev
*p,Vint type,Vint index,Vint flag);VKI_EXTERN void vgl_X11DevLight(vgl_X11Dev
*p,Vint index,Vint type,Vfloat c[3],Vfloat x[3]);VKI_EXTERN
void vgl_X11DevLightSwitch(vgl_X11Dev *p,Vint index,
Vint flag);VKI_EXTERN void vgl_X11DevLightModel(vgl_X11Dev
*p,Vint localviewer,Vfloat koffset,Vfloat krate);VKI_EXTERN
void vgl_X11DevProjOrtho(vgl_X11Dev *p,Vfloat left,
Vfloat right,Vfloat bottom,Vfloat top,Vfloat nearz,
Vfloat farz);VKI_EXTERN void vgl_X11DevProjFrustum(vgl_X11Dev
*p,Vfloat left,Vfloat right,Vfloat bottom,Vfloat top,
Vfloat nearz,Vfloat farz);VKI_EXTERN void vgl_X11DevProjPush(vgl_X11Dev
*p);VKI_EXTERN void vgl_X11DevProjPop(vgl_X11Dev *p);
VKI_EXTERN void vgl_X11DevProjLoad(vgl_X11Dev *p,Vfloat
xfm[4][4]);VKI_EXTERN void vgl_X11DevViewport(vgl_X11Dev
*p,Vint left,Vint right,Vint bottom,Vint top);VKI_EXTERN
void vgl_X11DevDepthRange(vgl_X11Dev *p,Vfloat nearz,
Vfloat farz);VKI_EXTERN void vgl_X11DevPolygonOffset(vgl_X11Dev
*p,Vfloat factor,Vfloat bias);VKI_EXTERN void vgl_X11DevPolygonMode(vgl_X11Dev
*p,Vint rend);VKI_EXTERN void vgl_X11DevTextPlane(vgl_X11Dev
*p,Vfloat path[3],Vfloat plane[3]);VKI_EXTERN void vgl_X11DevTextPixelSize(vgl_X11Dev
*p,Vfloat pixelsize);VKI_EXTERN void vgl_X11DevPollMouse(vgl_X11Dev
*p,Vint *px,Vint *py,Vint *but1,Vint *but2,Vint *but3);
VKI_EXTERN void vgl_X11DevWarpMouse(vgl_X11Dev *p,Vint
px,Vint py);VKI_EXTERN void vgl_X11DevSetCursor(vgl_X11Dev
*p,Vint type);VKI_EXTERN void vgl_X11DevPollModifiers(vgl_X11Dev
*p,Vint *cntl,Vint *shft);VKI_EXTERN void vgl_X11DevReadQueue(vgl_X11Dev
*p,Vint *dev,Vint *val);VKI_EXTERN void vgl_X11DevTestQueue(vgl_X11Dev
*p,Vint *dev);VKI_EXTERN void vgl_X11DevResetQueue(vgl_X11Dev
*p);VKI_EXTERN void vgl_X11DevPolyPoint(vgl_X11Dev *p,
Vint npts,Vfloat x[][3]);VKI_EXTERN void vgl_X11DevPolyPointColor(vgl_X11Dev
*p,Vint npts,Vfloat x[][3],Vfloat c[][3]);VKI_EXTERN
void vgl_X11DevPolyPointDC(vgl_X11Dev *p,Vint npts,
Vfloat x[3],Vint dc[][3]);VKI_EXTERN void vgl_X11DevPolyLine(vgl_X11Dev
*p,Vint type,Vint npts,Vfloat x[][3]);VKI_EXTERN void
vgl_X11DevPolyLineColor(vgl_X11Dev *p,Vint type,Vint
npts,Vfloat x[][3],Vfloat c[][3]);VKI_EXTERN void vgl_X11DevPolyLineDC(vgl_X11Dev
*p,Vint type,Vint npts,Vfloat x[3],Vint dc[][3]);VKI_EXTERN
void vgl_X11DevColorIndex(vgl_X11Dev *p,Vint index);
VKI_EXTERN void vgl_X11DevColor(vgl_X11Dev *p,Vfloat
c[3]);VKI_EXTERN void vgl_X11DevPolygon(vgl_X11Dev *p,
Vint type,Vint npts,Vfloat x[][3],Vint vtflag,Vfloat
vt[]);VKI_EXTERN void vgl_X11DevPolygonColor(vgl_X11Dev
*p,Vint type,Vint npts,Vfloat x[][3],Vfloat c[][3],
Vint vtflag,Vfloat vt[]);VKI_EXTERN void vgl_X11DevPolygonDC(vgl_X11Dev
*p,Vint type,Vint npts,Vfloat x[3],Vint dc[][3]);VKI_EXTERN
void vgl_X11DevPolyPointArray(vgl_X11Dev *p,Vint npts,
Vfloat x[][3],Vint cflag,void *c,Vint vflag,void *v,
Vint tflag,Vfloat *t,Vint dflag,Vfloat *d);VKI_EXTERN
void vgl_X11DevPolyLineArray(vgl_X11Dev *p,Vint type,
Vint npts,Vfloat x[][3],Vint cflag,void *c,Vint vflag,
void *v,Vint tflag,Vfloat *t,Vint dflag,Vfloat *d);
VKI_EXTERN void vgl_X11DevPolygonArray(vgl_X11Dev *p,
Vint type,Vint npts,Vfloat x[][3],Vint cflag,void *c,
Vint vflag,void *v,Vint tflag,Vfloat *t,Vint dflag,
Vfloat *d);VKI_EXTERN void vgl_X11DevInitBuffer(vgl_X11Dev
*p,Vint size,Vint *vboid);VKI_EXTERN void vgl_X11DevTermBuffer(vgl_X11Dev
*p,Vint vboid);VKI_EXTERN void vgl_X11DevCopyBuffer(vgl_X11Dev
*p,Vint vboid,Vint npts,Vfloat x[][3],Vint cflag,void
*c,Vint vflag,void *v,Vint tflag,Vfloat *t,Vint dflag,
Vfloat *d,Vint *offset);VKI_EXTERN void vgl_X11DevPolyPointBuffer(vgl_X11Dev
*p,Vint vboid,Vint offset,Vint npts,Vint cflag,Vint
vflag,Vint tflag,Vint dflag);VKI_EXTERN void vgl_X11DevPolyLineBuffer(vgl_X11Dev
*p,Vint vboid,Vint offset,Vint type,Vint npts,Vint cflag,
Vint vflag,Vint tflag,Vint dflag);VKI_EXTERN void vgl_X11DevPolygonBuffer(vgl_X11Dev
*p,Vint vboid,Vint offset,Vint type,Vint npts,Vint cflag,
Vint vflag,Vint tflag,Vint dflag);VKI_EXTERN void vgl_X11DevSetFactors(vgl_X11Dev
*p,Vint type,Vfloat factors[]);VKI_EXTERN void vgl_X11DevXfmPush(vgl_X11Dev
*p);VKI_EXTERN void vgl_X11DevXfmPop(vgl_X11Dev *p);
VKI_EXTERN void vgl_X11DevXfmMult(vgl_X11Dev *p,Vfloat
xfm[4][4]);VKI_EXTERN void vgl_X11DevXfmLoad(vgl_X11Dev
*p,Vfloat xfm[4][4]);VKI_EXTERN void vgl_X11DevAttPush(vgl_X11Dev
*p,Vint mask);VKI_EXTERN void vgl_X11DevAttPop(vgl_X11Dev
*p);VKI_EXTERN void vgl_X11DevPointSize(vgl_X11Dev *p,
Vint psize);VKI_EXTERN void vgl_X11DevLineStyle(vgl_X11Dev
*p,Vint lstyle);VKI_EXTERN void vgl_X11DevLineWidth(vgl_X11Dev
*p,Vint lwidth);VKI_EXTERN void vgl_X11DevTrans(vgl_X11Dev
*p,Vfloat transp);VKI_EXTERN void vgl_X11DevTransIndex(vgl_X11Dev
*p,Vint index);VKI_EXTERN void vgl_X11DevClip(vgl_X11Dev
*p,Vint left,Vint right,Vint bottom,Vint top);VKI_EXTERN
void vgl_X11DevText(vgl_X11Dev *p,Vfloat x[3],Vtchar
*string);VKI_EXTERN void vgl_X11DevTextDC(vgl_X11Dev
*p,Vfloat x[3],Vint dc[3],Vtchar *string);VKI_EXTERN
void vgl_X11DevSpecularity(vgl_X11Dev *p,Vfloat intensity,
Vfloat shininess);VKI_EXTERN void vgl_X11DevDataIndex(vgl_X11Dev
*p,Vint nprims,Vint nrws,Vint *index);VKI_EXTERN void
vgl_X11DevData(vgl_X11Dev *p,Vint nrws,Vfloat d[]);
VKI_EXTERN void vgl_X11DevPolyPointData(vgl_X11Dev *p,
Vint npts,Vfloat x[][3],Vint nrws,Vfloat d[]);VKI_EXTERN
void vgl_X11DevPolyLineData(vgl_X11Dev *p,Vint type,
Vint npts,Vfloat x[][3],Vint nrws,Vfloat d[]);VKI_EXTERN
void vgl_X11DevPolygonData(vgl_X11Dev *p,Vint type,
Vint npts,Vfloat x[][3],Vint nrws,Vfloat *d,Vint vtflag,
Vfloat vt[]);VKI_EXTERN void vgl_X11DevRender(vgl_X11Dev
*p,Vint type);VKI_EXTERN void vgl_X11DevSelectRegion(vgl_X11Dev
*p,Vint type,Vint *params);VKI_EXTERN void vgl_X11DevSelectBuffer(vgl_X11Dev
*p,Vint size,Vint *indexlist,Vfloat *mindepth,Vfloat
*maxdepth);VKI_EXTERN void vgl_X11DevSelectQuery(vgl_X11Dev
*p,Vint *hits);VKI_EXTERN void vgl_X11DevExtentQuery(vgl_X11Dev
*p,Vfloat *lft,Vfloat *rgt,Vfloat *btm,Vfloat *top,
Vfloat *znear,Vfloat *zfar);VKI_EXTERN void vgl_X11DevBufferSize(vgl_X11Dev
*p,Vint xsize,Vint ysize);VKI_EXTERN void vgl_X11DevBufferRow(vgl_X11Dev
*p,Vint irow);VKI_EXTERN void vgl_X11DevBufferQuery(vgl_X11Dev
*p,vgl_FBuffer *fbuffer);VKI_EXTERN void vgl_X11DevDrawFun(vgl_X11Dev
*p,vgl_DrawFun *df);VKI_EXTERN void vgl_X11DevDispFun(vgl_X11Dev
*p,vgl_DispFun *df);
#ifdef __cplusplus
}
#endif
#define X11setpoly(p,flag)if((flag)!= (p)->polyflag)\
{ vgl_X11Dev_setpoly ((p),(flag));}
#define X11setdc(p,flag)if((flag)!= (p)->dcflag){ vgl_X11Dev_setdc\
((p),(flag));}
#define X11setplan(p,flag)if((flag)!= (p)->planflag)\
{ vgl_X11Dev_setplan ((p),(flag));}
#endif

