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
 
#ifndef RENDBUF_DEF
#define RENDBUF_DEF
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
#include <stdio.h>
#include <math.h>
struct vgl_RendBuf { Vint ierr;int isxpix,isypix;float
sxcm,sycm;int depth;int depth_bits;int xorig,yorig;
int xsize,ysize;int visflag;float xlencm,ylencm;float
xgupcm,ygupcm;int p1x,p1y,p2x,p2y;float fzmin,fzmax;
float right,left,bottom,top;float nearz,farz;int pxmn,
pxmx,pymn,pymx;float fzmn,fzmx;Vfloat projsx,projsy,
factsz;Vint projtype;Vint open;Vint connect;int rendermode;
float zpfact,zpbias;int pgrend; int idbfl;int ifbfl;
int ifrfl;int izbfl;int truecolor;int iltfl;int idmfl;
int ipdfl;int ibffl;int icpfl;int ildfl;int ibtfl;int
ilntfl;int itsfl;int izwfl;int itafl;int itdfl;int issfl;
int itpsfl;int izbfn;int lsfact;int istfl;int palmfl;
int lalmfl; int dbufhard;int zbufhard;int tcolhard;
int overplanes;int litehard;int winsystem;int iliton;
int polyflag;int planflag;int plantemp;int ibtflsav;
int textureindsav;float path[3];float plane[3];float
pixelsize;float znear;float zfar;int itrpat;int itrans;
float transp;int pointsize;int linestyle;int linewidth;
float intensity,shininess;int rasfontind;int textureind;
int bitmapind;float forecrgba[4];float backcrgb[3];
int primalphaflag;float primalpha[3];int primtexturenum;
float primtexture[4];vgl_Bitmap *stipmask[8];vgl_Bitmap
*linemask[VGL_LINESTYLE_MAX];vgl_ZBuffer *zbuffer;vgl_FBuffer
*fbuffer;vgl_DrawFun *drawfun;vgl_Xfm *xfmtmp;vgl_Xfmstack
*xfmstack;vgl_DispFun *dispfun;vgl_LModel *lmodel;vgl_Material
*material;vsy_List *lightlist;vgl_Attstack *attstack;
vgl_RasFont *rasfontcur;Vint fonttextureindexcur;vsy_HashTable
*rasfonthash;vsy_HashTable *texturehash;vsy_HashTable
*bitmaphash;vgl_RMode *rmode;Vuint stenwritemask;Vint
stenfuncmode;Vint stenfuncref;Vuint stenfuncmask;Vint
stenopfail;Vint stenopzfail;Vint stenopzpass;Vint shadermode;
Vint transvaluemode;Vint colorvaluemode;Vfloat transvalue,
alphavalue;Vfloat colorvalue[3];vgl_DrawFun *df;Vint
mpts;Vfloat (*cp)[3];Vfloat (*xb)[3];Vchar *cb;Vchar
*vb;Vfloat *tb;Vfloat *db;
#ifdef __cplusplus
public: VKI_EXTERN vgl_RendBuf(void);VKI_EXTERN void*
operator new (size_t);VKI_EXTERN ~vgl_RendBuf(void);
VKI_EXTERN void operator delete (void*);VKI_EXTERN Vint
Error();VKI_EXTERN void SetResolution(Vfloat,Vfloat);
VKI_EXTERN void GetFBuffer(vgl_FBuffer**);VKI_EXTERN
void GetZBuffer(vgl_ZBuffer**);VKI_EXTERN void SetObject(Vint,Vobject*);
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
VKI_EXTERN void Viewport(Vint,Vint,Vint,Vint);VKI_EXTERN
void DepthRange(Vfloat,Vfloat);VKI_EXTERN void PolygonOffset(Vfloat,Vfloat);
VKI_EXTERN void PolygonMode(Vint);VKI_EXTERN void TextPlane(Vfloat[3],Vfloat[3]);
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
Vint,Vfloat*,Vint,Vfloat*);VKI_EXTERN void PolyPointBuffer(Vint,Vint,Vint,Vint,Vint,Vint,Vint);
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
VKI_EXTERN void DrawFun(vgl_DrawFun*);
#endif
};
#ifdef __cplusplus
extern "C" { 
#endif
VKI_EXTERN vgl_RendBuf* vgl_RendBufBegin(void);VKI_EXTERN
void vgl_RendBuf_Construct(vgl_RendBuf *p);VKI_EXTERN
void vgl_RendBufEnd(vgl_RendBuf *p);VKI_EXTERN void
vgl_RendBuf_Destruct(vgl_RendBuf *p);VKI_EXTERN Vint
vgl_RendBufError(vgl_RendBuf *p);VKI_EXTERN void vgl_RendBufSetResolution(vgl_RendBuf
*p,Vfloat xpixcm,Vfloat ypixcm);VKI_EXTERN void vgl_RendBufGetFBuffer(vgl_RendBuf
*p,vgl_FBuffer **fbuffer);VKI_EXTERN void vgl_RendBufGetZBuffer(vgl_RendBuf
*p,vgl_ZBuffer **zbuffer);VKI_EXTERN void vgl_RendBufSetObject(vgl_RendBuf
*p,Vint objecttype,Vobject *object);VKI_EXTERN void
vgl_RendBufPositionWindow(vgl_RendBuf *p,Vint xorig,
Vint yorig,Vint xsize,Vint ysize);VKI_EXTERN void vgl_RendBufVisualWindow(vgl_RendBuf
*p,Vint visflag);VKI_EXTERN void vgl_RendBufParentWindow(vgl_RendBuf
*p,Vword parent);VKI_EXTERN void vgl_RendBufConfigureWindow(vgl_RendBuf
*p,Vint oper,Vint *params);VKI_EXTERN void vgl_RendBufOpenWindow(vgl_RendBuf
*p,const Vchar *dummy);VKI_EXTERN void vgl_RendBufCloseWindow(vgl_RendBuf
*p);VKI_EXTERN void vgl_RendBufConnectWindow(vgl_RendBuf
*p,Vword dummy);VKI_EXTERN void vgl_RendBufDisconnectWindow(vgl_RendBuf
*p);VKI_EXTERN void vgl_RendBufSetWindow(vgl_RendBuf
*p);VKI_EXTERN void vgl_RendBufQueryWindow(vgl_RendBuf
*p,Vword *dummy);VKI_EXTERN void vgl_RendBufClear(vgl_RendBuf
*p);VKI_EXTERN void vgl_RendBufSwap(vgl_RendBuf *p);
VKI_EXTERN void vgl_RendBufFlush(vgl_RendBuf *p);VKI_EXTERN
void vgl_RendBufBell(vgl_RendBuf *p);VKI_EXTERN void
vgl_RendBufDelay(vgl_RendBuf *p,Vfloat sec);VKI_EXTERN
void vgl_RendBufResize(vgl_RendBuf *p);VKI_EXTERN void
vgl_RendBufBackColorIndex(vgl_RendBuf *p,Vint index);
VKI_EXTERN void vgl_RendBufBackColor(vgl_RendBuf *p,
Vfloat c[3]);VKI_EXTERN void vgl_RendBufSetMode(vgl_RendBuf
*p,Vint mode,Vint flag);VKI_EXTERN void vgl_RendBufGetMode(vgl_RendBuf
*p,Vint mode,Vint *flag);VKI_EXTERN void vgl_RendBufGetInteger(vgl_RendBuf
*p,Vint type,Vint *params);VKI_EXTERN void vgl_RendBufGetFloat(vgl_RendBuf
*p,Vint type,Vfloat *params);VKI_EXTERN void vgl_RendBufGetString(vgl_RendBuf
*p,Vint type,Vchar params[]);VKI_EXTERN void vgl_RendBufFBufferRead(vgl_RendBuf
*p,Vint left,Vint right,Vint bottom,Vint top,vgl_FBuffer
*fbuffer);VKI_EXTERN void vgl_RendBufFBufferWrite(vgl_RendBuf
*p,Vint left,Vint right,Vint bottom,Vint top,vgl_FBuffer
*fbuffer);VKI_EXTERN void vgl_RendBufZBufferRead(vgl_RendBuf
*p,Vint left,Vint right,Vint bottom,Vint top,vgl_ZBuffer
*zbuffer);VKI_EXTERN void vgl_RendBufZBufferWrite(vgl_RendBuf
*p,Vint left,Vint right,Vint bottom,Vint top,vgl_ZBuffer
*zbuffer);VKI_EXTERN void vgl_RendBufPixelmapCreate(vgl_RendBuf
*p,vgl_Pixelmap *pixelmap);VKI_EXTERN void vgl_RendBufPixelmapDestroy(vgl_RendBuf
*p,vgl_Pixelmap *pixelmap);VKI_EXTERN void vgl_RendBufPixelmapRead(vgl_RendBuf
*p,vgl_Pixelmap *pixelmap);VKI_EXTERN void vgl_RendBufPixelmapWrite(vgl_RendBuf
*p,vgl_Pixelmap *pixelmap);VKI_EXTERN void vgl_RendBufBitmapDefine(vgl_RendBuf
*p,Vint index,vgl_Bitmap *bitmap);VKI_EXTERN void vgl_RendBufBitmapSelect(vgl_RendBuf
*p,Vint index);VKI_EXTERN void vgl_RendBufTextureDefine(vgl_RendBuf
*p,Vint index,vgl_Texture *texture);VKI_EXTERN void
vgl_RendBufTextureSelect(vgl_RendBuf *p,Vint index);
VKI_EXTERN void vgl_RendBufRasFontDefine(vgl_RendBuf
*p,Vint index,vgl_RasFont *rasfont);VKI_EXTERN void
vgl_RendBufRasFontSelect(vgl_RendBuf *p,Vint index);
VKI_EXTERN void vgl_RendBufClipPlane(vgl_RendBuf *p,
Vint index,Vfloat eqn[4]);VKI_EXTERN void vgl_RendBufSetSwitch(vgl_RendBuf
*p,Vint type,Vint index,Vint flag);VKI_EXTERN void vgl_RendBufLight(vgl_RendBuf
*p,Vint index,Vint type,Vfloat c[3],Vfloat x[3]);VKI_EXTERN
void vgl_RendBufLightSwitch(vgl_RendBuf *p,Vint index,
Vint flag);VKI_EXTERN void vgl_RendBufLightModel(vgl_RendBuf
*p,Vint localviewer,Vfloat koffset,Vfloat krate);VKI_EXTERN
void vgl_RendBufProjOrtho(vgl_RendBuf *p,Vfloat left,
Vfloat right,Vfloat bottom,Vfloat top,Vfloat nearz,
Vfloat farz);VKI_EXTERN void vgl_RendBufProjFrustum(vgl_RendBuf
*p,Vfloat left,Vfloat right,Vfloat bottom,Vfloat top,
Vfloat nearz,Vfloat farz);VKI_EXTERN void vgl_RendBufProjPush(vgl_RendBuf
*p);VKI_EXTERN void vgl_RendBufProjPop(vgl_RendBuf *p);
VKI_EXTERN void vgl_RendBufViewport(vgl_RendBuf *p,
Vint left,Vint right,Vint bottom,Vint top);VKI_EXTERN
void vgl_RendBufDepthRange(vgl_RendBuf *p,Vfloat nearz,
Vfloat farz);VKI_EXTERN void vgl_RendBufPolygonOffset(vgl_RendBuf
*p,Vfloat factor,Vfloat bias);VKI_EXTERN void vgl_RendBufPolygonMode(vgl_RendBuf
*p,Vint rend);VKI_EXTERN void vgl_RendBufTextPlane(vgl_RendBuf
*p,Vfloat path[3],Vfloat plane[3]);VKI_EXTERN void vgl_RendBufTextPixelSize(vgl_RendBuf
*p,Vfloat pixelsize);VKI_EXTERN void vgl_RendBufPollMouse(vgl_RendBuf
*p,Vint *px,Vint *py,Vint *but1,Vint *but2,Vint *but3);
VKI_EXTERN void vgl_RendBufWarpMouse(vgl_RendBuf *p,
Vint px,Vint py);VKI_EXTERN void vgl_RendBufSetCursor(vgl_RendBuf
*p,Vint type);VKI_EXTERN void vgl_RendBufPollModifiers(vgl_RendBuf
*p,Vint *cntl,Vint *shft);VKI_EXTERN void vgl_RendBufReadQueue(vgl_RendBuf
*p,Vint *dev,Vint *val);VKI_EXTERN void vgl_RendBufTestQueue(vgl_RendBuf
*p,Vint *dev);VKI_EXTERN void vgl_RendBufResetQueue(vgl_RendBuf
*p);VKI_EXTERN void vgl_RendBufPolyPoint(vgl_RendBuf
*p,Vint npts,Vfloat x[][3]);VKI_EXTERN void vgl_RendBufPolyPointColor(vgl_RendBuf
*p,Vint npts,Vfloat x[][3],Vfloat c[][3]);VKI_EXTERN
void vgl_RendBufPolyPointDC(vgl_RendBuf *p,Vint npts,
Vfloat x[3],Vint dc[][3]);VKI_EXTERN void vgl_RendBufPolyLine(vgl_RendBuf
*p,Vint type,Vint npts,Vfloat x[][3]);VKI_EXTERN void
vgl_RendBufPolyLineColor(vgl_RendBuf *p,Vint type,Vint
npts,Vfloat x[][3],Vfloat c[][3]);VKI_EXTERN void vgl_RendBufPolyLineDC(vgl_RendBuf
*p,Vint type,Vint npts,Vfloat x[3],Vint dc[][3]);VKI_EXTERN
void vgl_RendBufColorIndex(vgl_RendBuf *p,Vint index);
VKI_EXTERN void vgl_RendBufColor(vgl_RendBuf *p,Vfloat
c[3]);VKI_EXTERN void vgl_RendBufPolygon(vgl_RendBuf
*p,Vint type,Vint npts,Vfloat x[][3],Vint vtflag,Vfloat
vt[]);VKI_EXTERN void vgl_RendBufPolygonColor(vgl_RendBuf
*p,Vint type,Vint npts,Vfloat x[][3],Vfloat c[][3],
Vint vtflag,Vfloat vt[]);VKI_EXTERN void vgl_RendBufPolygonDC(vgl_RendBuf
*p,Vint type,Vint npts,Vfloat x[3],Vint dc[][3]);VKI_EXTERN
void vgl_RendBufPolyPointArray(vgl_RendBuf *p,Vint npts,
Vfloat x[][3],Vint cflag,void *c,Vint vflag,void *v,
Vint tflag,Vfloat *t,Vint dflag,Vfloat *d);VKI_EXTERN
void vgl_RendBufPolyLineArray(vgl_RendBuf *p,Vint type,
Vint npts,Vfloat x[][3],Vint cflag,void *c,Vint vflag,
void *v,Vint tflag,Vfloat *t,Vint dflag,Vfloat *d);
VKI_EXTERN void vgl_RendBufPolygonArray(vgl_RendBuf
*p,Vint type,Vint npts,Vfloat x[][3],Vint cflag,void
*c,Vint vflag,void *v,Vint tflag,Vfloat *t,Vint dflag,
Vfloat *d);VKI_EXTERN void vgl_RendBufPolyPointBuffer(vgl_RendBuf
*p,Vint vboid,Vint offset,Vint npts,Vint cflag,Vint
vflag,Vint tflag,Vint dflag);VKI_EXTERN void vgl_RendBufPolyLineBuffer(vgl_RendBuf
*p,Vint vboid,Vint offset,Vint type,Vint npts,Vint cflag,
Vint vflag,Vint tflag,Vint dflag);VKI_EXTERN void vgl_RendBufPolygonBuffer(vgl_RendBuf
*p,Vint vboid,Vint offset,Vint type,Vint npts,Vint cflag,
Vint vflag,Vint tflag,Vint dflag);VKI_EXTERN void vgl_RendBufSetFactors(vgl_RendBuf
*p,Vint type,Vfloat factors[]);VKI_EXTERN void vgl_RendBufXfmPush(vgl_RendBuf
*p);VKI_EXTERN void vgl_RendBufXfmPop(vgl_RendBuf *p);
VKI_EXTERN void vgl_RendBufXfmMult(vgl_RendBuf *p,Vfloat
xfm[4][4]);VKI_EXTERN void vgl_RendBufXfmLoad(vgl_RendBuf
*p,Vfloat xfm[4][4]);VKI_EXTERN void vgl_RendBufAttPush(vgl_RendBuf
*p,Vint mask);VKI_EXTERN void vgl_RendBufAttPop(vgl_RendBuf
*p);VKI_EXTERN void vgl_RendBufPointSize(vgl_RendBuf
*p,Vint psize);VKI_EXTERN void vgl_RendBufLineStyle(vgl_RendBuf
*p,Vint lstyle);VKI_EXTERN void vgl_RendBufLineWidth(vgl_RendBuf
*p,Vint lwidth);VKI_EXTERN void vgl_RendBufTrans(vgl_RendBuf
*p,Vfloat transp);VKI_EXTERN void vgl_RendBufTransIndex(vgl_RendBuf
*p,Vint index);VKI_EXTERN void vgl_RendBufClip(vgl_RendBuf
*p,Vint left,Vint right,Vint bottom,Vint top);VKI_EXTERN
void vgl_RendBufText(vgl_RendBuf *p,Vfloat x[3],Vtchar
*string);VKI_EXTERN void vgl_RendBufTextDC(vgl_RendBuf
*p,Vfloat x[3],Vint dc[3],Vtchar *string);VKI_EXTERN
void vgl_RendBufSpecularity(vgl_RendBuf *p,Vfloat intensity,Vfloat
shininess);VKI_EXTERN void vgl_RendBufDataIndex(vgl_RendBuf
*p,Vint nprims,Vint nrws,Vint *index);VKI_EXTERN void
vgl_RendBufData(vgl_RendBuf *p,Vint nrws,Vfloat d[]);
VKI_EXTERN void vgl_RendBufPolyPointData(vgl_RendBuf
*p,Vint npts,Vfloat x[][3],Vint nrws,Vfloat d[]);VKI_EXTERN
void vgl_RendBufPolyLineData(vgl_RendBuf *p,Vint type,
Vint npts,Vfloat x[][3],Vint nrws,Vfloat d[]);VKI_EXTERN
void vgl_RendBufPolygonData(vgl_RendBuf *p,Vint type,
Vint npts,Vfloat x[][3],Vint nrws,Vfloat *d,Vint vtflag,
Vfloat vt[]);VKI_EXTERN void vgl_RendBufRender(vgl_RendBuf
*p,Vint type);VKI_EXTERN void vgl_RendBufSelectRegion(vgl_RendBuf
*p,Vint type,Vint *params);VKI_EXTERN void vgl_RendBufSelectBuffer(vgl_RendBuf
*p,Vint size,Vint *indexlist,Vfloat *mindepth,Vfloat
*maxdepth);VKI_EXTERN void vgl_RendBufSelectQuery(vgl_RendBuf
*p,Vint *hits);VKI_EXTERN void vgl_RendBufExtentQuery(vgl_RendBuf
*p,Vfloat *lft,Vfloat *rgt,Vfloat *btm,Vfloat *top,
Vfloat *znear,Vfloat *zfar);VKI_EXTERN void vgl_RendBufBufferSize(vgl_RendBuf
*p,Vint xsize,Vint ysize);VKI_EXTERN void vgl_RendBufBufferRow(vgl_RendBuf
*p,Vint irow);VKI_EXTERN void vgl_RendBufBufferQuery(vgl_RendBuf
*p,vgl_FBuffer *fbuffer);VKI_EXTERN void vgl_RendBufDrawFun(vgl_RendBuf
*p,vgl_DrawFun *df);
#ifdef __cplusplus
}
#endif
#define RENDBUFsetpoly(p,flag)if((flag)!= (p)->polyflag)\
{ vgl_RendBuf_setpoly ((p),(flag));}
#define RENDBUFsetplan(p,flag)if((flag)!= (p)->planflag)\
{ vgl_RendBuf_setplan ((p),(flag));}
#endif

