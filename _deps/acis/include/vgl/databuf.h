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
 
#ifndef DATABUF_DEF
#define DATABUF_DEF
#include "base/basedefs.h"
#include "vgl/vgldefs.h"
#include "vgl/bitmap.h"
#include "vgl/zbuffer.h"
#include "vgl/fbuffer.h"
#include "vgl/drawfun.h"
#include "vgl/xfm.h"
#include "vgl/xfmstack.h"
#include "vgl/attstack.h"
#include "vgl/rasfont.h"
#include "vgl/texture.h"
#include "vgl/dispfun.h"
struct vgl_DataBuf { Vint ierr;int isxpix,isypix;float
sxcm,sycm;int depth;int depth_bits;int xorig,yorig;
int xsize,ysize;int visflag;int ccsize;int ccsize2,
ccsize3;float xlencm,ylencm;float xgupcm,ygupcm;int
p1x,p1y,p2x,p2y;float fzmin,fzmax;float right,left,
bottom,top;float nearz,farz;int pxmn,pxmx,pymn,pymx;
float fzmn,fzmx;Vfloat projsx,projsy,factsz;Vint projtype;
Vint open;Vint connect;int rendermode;float zpfact,
zpbias;int pgrend; int idbfl;int ifbfl;int ifrfl;int
izbfl;int truecolor;int iltfl;int idmfl;int ipdfl;int
ibffl;int icpfl;int ildfl;int itsfl;int izwfl;int itafl;
int itdfl;int issfl;int itpsfl;int izbfn;int lsfact;
int istfl;int shadermode; int dbufhard;int zbufhard;
int tcolhard;int overplanes;int litehard;int winsystem;
 Vuint stenwritemask;Vint stenfuncmode;Vint stenfuncref;
Vuint stenfuncmask;Vint stenopfail;Vint stenopzfail;
Vint stenopzpass;int polyflag;int planflag;int plantemp;
float path[3];float plane[3];float pixelsize;float znear;
float zfar;int itrpat;int itrans;float transp;int pointsize;
int linestyle;int linewidth;float intensity,shininess;
int rasfontind;int textureind;int bitmapind;Vint nprims;
Vint nrows;Vuint nibs;Vint maxdataindices;Vuint *dataindices;
Vfloat data[VGL_MAXDATAROWS];Vint setdataindex;Vint
ndbs;Vint pntflag;Vint linflag;Vint polflag;vgl_Bitmap
*stipmask[8];vgl_Bitmap *linemask[VGL_LINESTYLE_MAX];
vgl_ZBuffer *zbuffer;vgl_FBuffer *fbufferi[VGL_MAXDATAROWS];
vgl_FBuffer *fbufferf[VGL_MAXDATAROWS];vgl_DrawFun *drawfun;
vgl_Xfm *xfmtmp;vgl_Xfmstack *xfmstack;vgl_Attstack
*attstack;vgl_RasFont *rasfontcur;Vint fonttextureindexcur;
vsy_HashTable *rasfonthash;vsy_HashTable *texturehash;
vsy_HashTable *bitmaphash;vgl_DispFun *dispfun;vgl_DrawFun
*df;Vint mpts;Vfloat (*xb)[3];Vchar *cb;Vchar *vb;Vfloat
*tb;Vfloat *db;
#ifdef __cplusplus
public: VKI_EXTERN vgl_DataBuf(void);VKI_EXTERN void*
operator new (size_t);VKI_EXTERN ~vgl_DataBuf(void);
VKI_EXTERN void operator delete (void*);VKI_EXTERN Vint
Error();VKI_EXTERN void SetResolution(Vfloat,Vfloat);
VKI_EXTERN void SetBuffers(Vint,Vint);VKI_EXTERN void
SetRender(Vint,Vint,Vint);VKI_EXTERN void SetObject(Vint,Vobject*);
VKI_EXTERN void PositionWindow(Vint,Vint,Vint,Vint);
VKI_EXTERN void VisualWindow(Vint);VKI_EXTERN void ParentWindow(Vword);
VKI_EXTERN void ConfigureWindow(Vint,Vint*);VKI_EXTERN
void OpenWindow(const Vchar*);VKI_EXTERN void CloseWindow();
VKI_EXTERN void ConnectWindow(Vint);VKI_EXTERN void
DisconnectWindow();VKI_EXTERN void SetWindow();VKI_EXTERN
void QueryWindow(Vint*);VKI_EXTERN void LinePixels(Vint[2],Vint[2],Vint*,Vint[][2]);
VKI_EXTERN void TriPixels(Vint[2],Vint[2],Vint[2],Vint*,Vint[][2]);
VKI_EXTERN void GetDataIndex(Vint,Vint,Vint*);VKI_EXTERN
void GetData(Vint,Vint,Vint*,Vfloat*);VKI_EXTERN void
Clear();VKI_EXTERN void Swap();VKI_EXTERN void Flush();
VKI_EXTERN void Bell();VKI_EXTERN void Delay(Vfloat);
VKI_EXTERN void Resize();VKI_EXTERN void BackColorIndex(Vint);
VKI_EXTERN void BackColor(Vfloat[3]);VKI_EXTERN void
SetMode(Vint,Vint);VKI_EXTERN void GetMode(Vint,Vint*);
VKI_EXTERN void GetInteger(Vint,Vint*);VKI_EXTERN void
GetFloat(Vint,Vfloat*);VKI_EXTERN void GetString(Vint,Vchar[]);
VKI_EXTERN void FBufferRead(Vint,Vint,Vint,Vint,vgl_FBuffer*);
VKI_EXTERN void FBufferWrite(Vint,Vint,Vint,Vint,vgl_FBuffer*);
VKI_EXTERN void ZBufferRead(Vint,Vint,Vint,Vint,vgl_ZBuffer*);
VKI_EXTERN void ZBufferWrite(Vint,Vint,Vint,Vint,vgl_ZBuffer*);
VKI_EXTERN void PixelmapCreate(Vobject*);VKI_EXTERN
void PixelmapDestroy(Vobject*);VKI_EXTERN void PixelmapRead(Vobject*);
VKI_EXTERN void PixelmapWrite(Vobject*);VKI_EXTERN void
BitmapDefine(Vint,vgl_Bitmap*);VKI_EXTERN void BitmapSelect(Vint);
VKI_EXTERN void TextureDefine(Vint,vgl_Texture*);VKI_EXTERN
void TextureSelect(Vint);VKI_EXTERN void RasFontDefine(Vint,vgl_RasFont*);
VKI_EXTERN void RasFontSelect(Vint);VKI_EXTERN void
ClipPlane(Vint,Vfloat[4]);VKI_EXTERN void SetSwitch(Vint,Vint,Vint);
VKI_EXTERN void Light(Vint,Vint,Vfloat[3],Vfloat[3]);
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
VKI_EXTERN void PolyPointData(Vint,Vfloat[][3],Vint,Vfloat*);
VKI_EXTERN void pixpoint(Vint,Vint);VKI_EXTERN void
pixpointcolor(Vint,Vint,Vfloat[3]);VKI_EXTERN void pixpointdata(Vint,Vint,Vint,Vfloat*);
VKI_EXTERN void PolyLine(Vint,Vint,Vfloat[][3]);VKI_EXTERN
void PolyLineColor(Vint,Vint,Vfloat[][3],Vfloat[][3]);
VKI_EXTERN void PolyLineDC(Vint,Vint,Vfloat[3],Vint[][3]);
VKI_EXTERN void PolyLineData(Vint,Vint,Vfloat[][3],Vint,Vfloat[]);
VKI_EXTERN void pixline(Vint,Vint,Vint,Vint);VKI_EXTERN
void pixscan(Vint,Vint,Vint);VKI_EXTERN void pixscancolor(Vint,Vfloat*,Vint,Vfloat*,Vint);
VKI_EXTERN void pixscandata(Vint,Vint,Vfloat*,Vint,Vfloat*,Vint);
VKI_EXTERN void ColorIndex(Vint);VKI_EXTERN void Color(Vfloat[3]);
VKI_EXTERN void Polygon(Vint,Vint,Vfloat[][3],Vint,Vfloat[]);
VKI_EXTERN void PolygonColor(Vint,Vint,Vfloat[][3],Vfloat[][3],Vint,
Vfloat[]);VKI_EXTERN void PolygonDC(Vint,Vint,Vfloat[3],Vint[][3]);
VKI_EXTERN void PolygonData(Vint,Vint,Vfloat[][3],Vint,Vfloat*,Vint,
Vfloat[]);VKI_EXTERN void PolyPointArray(Vint,Vfloat[][3],Vint,void*,Vint,void*,Vint,
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
void Data(Vint,Vfloat[]);VKI_EXTERN void Render(Vint);
VKI_EXTERN void SelectRegion(Vint,Vint*);VKI_EXTERN
void SelectBuffer(Vint,Vint*,Vfloat*,Vfloat*);VKI_EXTERN
void SelectQuery(Vint*);VKI_EXTERN void ExtentQuery(Vfloat*,Vfloat*,Vfloat*,Vfloat*,Vfloat*,Vfloat*);
VKI_EXTERN void BufferSize(Vint,Vint);VKI_EXTERN void
BufferRow(Vint);VKI_EXTERN void BufferQuery(vgl_FBuffer*);
VKI_EXTERN void DrawFun(vgl_DrawFun*);VKI_EXTERN void
DispFun(vgl_DispFun*);
#endif
};
#ifdef __cplusplus
extern "C" { 
#endif
VKI_EXTERN vgl_DataBuf* vgl_DataBufBegin(void);VKI_EXTERN
void vgl_DataBuf_Construct(vgl_DataBuf *p);VKI_EXTERN
void vgl_DataBufEnd(vgl_DataBuf *p);VKI_EXTERN void
vgl_DataBuf_Destruct(vgl_DataBuf *p);VKI_EXTERN Vint
vgl_DataBufError(vgl_DataBuf *p);VKI_EXTERN void vgl_DataBufSetResolution(vgl_DataBuf
*p,Vfloat xpixcm,Vfloat ypixcm);VKI_EXTERN void vgl_DataBufSetBuffers(vgl_DataBuf
*p,Vint indexflag,Vint dataflag);VKI_EXTERN void vgl_DataBufSetRender(vgl_DataBuf
*p,Vint pointflag,Vint lineflag,Vint polyflag);VKI_EXTERN
void vgl_DataBufSetObject(vgl_DataBuf *p,Vint objecttype,
Vobject *object);VKI_EXTERN void vgl_DataBufPositionWindow(vgl_DataBuf
*p,Vint xorig,Vint yorig,Vint xsize,Vint ysize);VKI_EXTERN
void vgl_DataBufVisualWindow(vgl_DataBuf *p,Vint visflag);
VKI_EXTERN void vgl_DataBufParentWindow(vgl_DataBuf
*p,Vword parent);VKI_EXTERN void vgl_DataBufConfigureWindow(vgl_DataBuf
*p,Vint oper,Vint *params);VKI_EXTERN void vgl_DataBufOpenWindow(vgl_DataBuf
*p,const Vchar *dummy);VKI_EXTERN void vgl_DataBufCloseWindow(vgl_DataBuf
*p);VKI_EXTERN void vgl_DataBufConnectWindow(vgl_DataBuf
*p,Vint dummy);VKI_EXTERN void vgl_DataBufDisconnectWindow(vgl_DataBuf
*p);VKI_EXTERN void vgl_DataBufSetWindow(vgl_DataBuf
*p);VKI_EXTERN void vgl_DataBufQueryWindow(vgl_DataBuf
*p,Vint *dummy);VKI_EXTERN void vgl_DataBufLinePixels(vgl_DataBuf
*p,Vint ixy1[2],Vint ixy2[2],Vint *npix,Vint pix[][2]);
VKI_EXTERN void vgl_DataBufTriPixels(vgl_DataBuf *p,
Vint ixy1[2],Vint ixy2[2],Vint ixy3[2],Vint *npix,Vint
pix[][2]);VKI_EXTERN void vgl_DataBufGetDataIndex(vgl_DataBuf
*p,Vint ix,Vint iy,Vint *index);VKI_EXTERN void vgl_DataBufGetData(vgl_DataBuf
*p,Vint ix,Vint iy,Vint *nrws,Vfloat *data);VKI_EXTERN
void vgl_DataBufClear(vgl_DataBuf *p);VKI_EXTERN void
vgl_DataBufSwap(vgl_DataBuf *p);VKI_EXTERN void vgl_DataBufFlush(vgl_DataBuf
*p);VKI_EXTERN void vgl_DataBufBell(vgl_DataBuf *p);
VKI_EXTERN void vgl_DataBufDelay(vgl_DataBuf *p,Vfloat
sec);VKI_EXTERN void vgl_DataBufResize(vgl_DataBuf *p);
VKI_EXTERN void vgl_DataBufBackColorIndex(vgl_DataBuf
*p,Vint index);VKI_EXTERN void vgl_DataBufBackColor(vgl_DataBuf
*p,Vfloat c[3]);VKI_EXTERN void vgl_DataBufSetMode(vgl_DataBuf
*p,Vint mode,Vint flag);VKI_EXTERN void vgl_DataBufGetMode(vgl_DataBuf
*p,Vint mode,Vint *flag);VKI_EXTERN void vgl_DataBufGetInteger(vgl_DataBuf
*p,Vint type,Vint *params);VKI_EXTERN void vgl_DataBufGetFloat(vgl_DataBuf
*p,Vint type,Vfloat *params);VKI_EXTERN void vgl_DataBufGetString(vgl_DataBuf
*p,Vint type,Vchar params[]);VKI_EXTERN void vgl_DataBufFBufferRead(vgl_DataBuf
*p,Vint left,Vint right,Vint bottom,Vint top,vgl_FBuffer
*fbuffer);VKI_EXTERN void vgl_DataBufFBufferWrite(vgl_DataBuf
*p,Vint left,Vint right,Vint bottom,Vint top,vgl_FBuffer
*fbuffer);VKI_EXTERN void vgl_DataBufZBufferRead(vgl_DataBuf
*p,Vint left,Vint right,Vint bottom,Vint top,vgl_ZBuffer
*zbuffer);VKI_EXTERN void vgl_DataBufZBufferWrite(vgl_DataBuf
*p,Vint left,Vint right,Vint bottom,Vint top,vgl_ZBuffer
*zbuffer);VKI_EXTERN void vgl_DataBufPixelmapCreate(vgl_DataBuf
*p,Vobject *pixelmap);VKI_EXTERN void vgl_DataBufPixelmapDestroy(vgl_DataBuf
*p,Vobject *pixelmap);VKI_EXTERN void vgl_DataBufPixelmapRead(vgl_DataBuf
*p,Vobject *pixelmap);VKI_EXTERN void vgl_DataBufPixelmapWrite(vgl_DataBuf
*p,Vobject *pixelmap);VKI_EXTERN void vgl_DataBufBitmapDefine(vgl_DataBuf
*p,Vint index,vgl_Bitmap *bitmap);VKI_EXTERN void vgl_DataBufBitmapSelect(vgl_DataBuf
*p,Vint index);VKI_EXTERN void vgl_DataBufTextureDefine(vgl_DataBuf
*p,Vint index,vgl_Texture *texture);VKI_EXTERN void
vgl_DataBufTextureSelect(vgl_DataBuf *p,Vint index);
VKI_EXTERN void vgl_DataBufRasFontDefine(vgl_DataBuf
*p,Vint index,vgl_RasFont *rasfont);VKI_EXTERN void
vgl_DataBufRasFontSelect(vgl_DataBuf *p,Vint index);
VKI_EXTERN void vgl_DataBufClipPlane(vgl_DataBuf *p,
Vint index,Vfloat eqn[4]);VKI_EXTERN void vgl_DataBufSetSwitch(vgl_DataBuf
*p,Vint type,Vint index,Vint flag);VKI_EXTERN void vgl_DataBufLight(vgl_DataBuf
*p,Vint index,Vint type,Vfloat c[3],Vfloat x[3]);VKI_EXTERN
void vgl_DataBufLightSwitch(vgl_DataBuf *p,Vint index,
Vint flag);VKI_EXTERN void vgl_DataBufLightModel(vgl_DataBuf
*p,Vint localviewer,Vfloat koffset,Vfloat krate);VKI_EXTERN
void vgl_DataBufProjOrtho(vgl_DataBuf *p,Vfloat left,
Vfloat right,Vfloat bottom,Vfloat top,Vfloat nearz,
Vfloat farz);VKI_EXTERN void vgl_DataBufProjFrustum(vgl_DataBuf
*p,Vfloat left,Vfloat right,Vfloat bottom,Vfloat top,
Vfloat nearz,Vfloat farz);VKI_EXTERN void vgl_DataBufProjPush(vgl_DataBuf
*p);VKI_EXTERN void vgl_DataBufProjPop(vgl_DataBuf *p);
VKI_EXTERN void vgl_DataBufViewport(vgl_DataBuf *p,
Vint left,Vint right,Vint bottom,Vint top);VKI_EXTERN
void vgl_DataBufDepthRange(vgl_DataBuf *p,Vfloat nearz,
Vfloat farz);VKI_EXTERN void vgl_DataBufPolygonOffset(vgl_DataBuf
*p,Vfloat factor,Vfloat bias);VKI_EXTERN void vgl_DataBufPolygonMode(vgl_DataBuf
*p,Vint rend);VKI_EXTERN void vgl_DataBufTextPlane(vgl_DataBuf
*p,Vfloat path[3],Vfloat plane[3]);VKI_EXTERN void vgl_DataBufTextPixelSize(vgl_DataBuf
*p,Vfloat pixelsize);VKI_EXTERN void vgl_DataBufPollMouse(vgl_DataBuf
*p,Vint *px,Vint *py,Vint *but1,Vint *but2,Vint *but3);
VKI_EXTERN void vgl_DataBufWarpMouse(vgl_DataBuf *p,
Vint px,Vint py);VKI_EXTERN void vgl_DataBufSetCursor(vgl_DataBuf
*p,Vint type);VKI_EXTERN void vgl_DataBufPollModifiers(vgl_DataBuf
*p,Vint *cntl,Vint *shft);VKI_EXTERN void vgl_DataBufReadQueue(vgl_DataBuf
*p,Vint *dev,Vint *val);VKI_EXTERN void vgl_DataBufTestQueue(vgl_DataBuf
*p,Vint *dev);VKI_EXTERN void vgl_DataBufResetQueue(vgl_DataBuf
*p);VKI_EXTERN void vgl_DataBufPolyPoint(vgl_DataBuf
*p,Vint npts,Vfloat x[][3]);VKI_EXTERN void vgl_DataBufPolyPointColor(vgl_DataBuf
*p,Vint npts,Vfloat x[][3],Vfloat c[][3]);VKI_EXTERN
void vgl_DataBufPolyPointDC(vgl_DataBuf *p,Vint npts,
Vfloat x[3],Vint dc[][3]);VKI_EXTERN void vgl_DataBufPolyPointData(vgl_DataBuf
*p,Vint npts,Vfloat x[][3],Vint nrws,Vfloat *d);VKI_EXTERN
void vgl_DataBufpixpoint(vgl_DataBuf *p,Vint x,Vint
y);VKI_EXTERN void vgl_DataBufpixpointcolor(vgl_DataBuf
*p,Vint x,Vint y,Vfloat c[3]);VKI_EXTERN void vgl_DataBufpixpointdata(vgl_DataBuf
*p,Vint nrws,Vint x,Vint y,Vfloat *data);VKI_EXTERN
void vgl_DataBufPolyLine(vgl_DataBuf *p,Vint type,Vint
npts,Vfloat x[][3]);VKI_EXTERN void vgl_DataBufPolyLineColor(vgl_DataBuf
*p,Vint type,Vint npts,Vfloat x[][3],Vfloat c[][3]);
VKI_EXTERN void vgl_DataBufPolyLineDC(vgl_DataBuf *p,
Vint type,Vint npts,Vfloat x[3],Vint dc[][3]);VKI_EXTERN
void vgl_DataBufPolyLineData(vgl_DataBuf *p,Vint type,
Vint npts,Vfloat x[][3],Vint nrws,Vfloat d[]);VKI_EXTERN
void vgl_DataBufpixline(vgl_DataBuf *p,Vint x1,Vint
y1,Vint x2,Vint y2);VKI_EXTERN void vgl_DataBufpixscan(vgl_DataBuf
*p,Vint x1,Vint x2,Vint y);VKI_EXTERN void vgl_DataBufpixscancolor(vgl_DataBuf
*p,Vint x1,Vfloat *d1,Vint x2,Vfloat *d2,Vint y);VKI_EXTERN
void vgl_DataBufpixscandata(vgl_DataBuf *p,Vint nrws,Vint
x1,Vfloat *d1,Vint x2,Vfloat *d2,Vint y);VKI_EXTERN
void vgl_DataBufColorIndex(vgl_DataBuf *p,Vint index);
VKI_EXTERN void vgl_DataBufColor(vgl_DataBuf *p,Vfloat
c[3]);VKI_EXTERN void vgl_DataBufPolygon(vgl_DataBuf
*p,Vint type,Vint npts,Vfloat x[][3],Vint vtflag,Vfloat
vt[]);VKI_EXTERN void vgl_DataBufPolygonColor(vgl_DataBuf
*p,Vint type,Vint npts,Vfloat x[][3],Vfloat c[][3],
Vint vtflag,Vfloat vt[]);VKI_EXTERN void vgl_DataBufPolygonDC(vgl_DataBuf
*p,Vint type,Vint npts,Vfloat x[3],Vint dc[][3]);VKI_EXTERN
void vgl_DataBufPolygonData(vgl_DataBuf *p,Vint type,
Vint npts,Vfloat x[][3],Vint nrws,Vfloat *d,Vint vtflag,
Vfloat vt[]);VKI_EXTERN void vgl_DataBufPolyPointArray(vgl_DataBuf
*p,Vint npts,Vfloat x[][3],Vint cflag,void *c,Vint vflag,
void *v,Vint tflag,Vfloat *t,Vint dflag,Vfloat *d);
VKI_EXTERN void vgl_DataBufPolyLineArray(vgl_DataBuf
*p,Vint type,Vint npts,Vfloat x[][3],Vint cflag,void
*c,Vint vflag,void *v,Vint tflag,Vfloat *t,Vint dflag,
Vfloat *d);VKI_EXTERN void vgl_DataBufPolygonArray(vgl_DataBuf
*p,Vint type,Vint npts,Vfloat x[][3],Vint cflag,void
*c,Vint vflag,void *v,Vint tflag,Vfloat *t,Vint dflag,
Vfloat *d);VKI_EXTERN void vgl_DataBufPolyPointBuffer(vgl_DataBuf
*p,Vint vboid,Vint offset,Vint npts,Vint cflag,Vint
vflag,Vint tflag,Vint dflag);VKI_EXTERN void vgl_DataBufPolyLineBuffer(vgl_DataBuf
*p,Vint vboid,Vint offset,Vint type,Vint npts,Vint cflag,
Vint vflag,Vint tflag,Vint dflag);VKI_EXTERN void vgl_DataBufPolygonBuffer(vgl_DataBuf
*p,Vint vboid,Vint offset,Vint type,Vint npts,Vint cflag,
Vint vflag,Vint tflag,Vint dflag);VKI_EXTERN void vgl_DataBufSetFactors(vgl_DataBuf
*p,Vint type,Vfloat factors[]);VKI_EXTERN void vgl_DataBufXfmPush(vgl_DataBuf
*p);VKI_EXTERN void vgl_DataBufXfmPop(vgl_DataBuf *p);
VKI_EXTERN void vgl_DataBufXfmMult(vgl_DataBuf *p,Vfloat
xfm[4][4]);VKI_EXTERN void vgl_DataBufXfmLoad(vgl_DataBuf
*p,Vfloat xfm[4][4]);VKI_EXTERN void vgl_DataBufAttPush(vgl_DataBuf
*p,Vint mask);VKI_EXTERN void vgl_DataBufAttPop(vgl_DataBuf
*p);VKI_EXTERN void vgl_DataBufPointSize(vgl_DataBuf
*p,Vint psize);VKI_EXTERN void vgl_DataBufLineStyle(vgl_DataBuf
*p,Vint lstyle);VKI_EXTERN void vgl_DataBufLineWidth(vgl_DataBuf
*p,Vint lwidth);VKI_EXTERN void vgl_DataBufTrans(vgl_DataBuf
*p,Vfloat transp);VKI_EXTERN void vgl_DataBufTransIndex(vgl_DataBuf
*p,Vint index);VKI_EXTERN void vgl_DataBufClip(vgl_DataBuf
*p,Vint left,Vint right,Vint bottom,Vint top);VKI_EXTERN
void vgl_DataBufText(vgl_DataBuf *p,Vfloat x[3],Vtchar
*string);VKI_EXTERN void vgl_DataBufTextDC(vgl_DataBuf
*p,Vfloat x[3],Vint dc[3],Vtchar *string);VKI_EXTERN
void vgl_DataBufSpecularity(vgl_DataBuf *p,Vfloat intensity,Vfloat
shininess);VKI_EXTERN void vgl_DataBufDataIndex(vgl_DataBuf
*p,Vint nprims,Vint nrws,Vint *index);VKI_EXTERN void
vgl_DataBufData(vgl_DataBuf *p,Vint nrws,Vfloat d[]);
VKI_EXTERN void vgl_DataBufRender(vgl_DataBuf *p,Vint
type);VKI_EXTERN void vgl_DataBufSelectRegion(vgl_DataBuf
*p,Vint type,Vint *params);VKI_EXTERN void vgl_DataBufSelectBuffer(vgl_DataBuf
*p,Vint size,Vint *indexlist,Vfloat *mindepth,Vfloat
*maxdepth);VKI_EXTERN void vgl_DataBufSelectQuery(vgl_DataBuf
*p,Vint *hits);VKI_EXTERN void vgl_DataBufExtentQuery(vgl_DataBuf
*p,Vfloat *lft,Vfloat *rgt,Vfloat *btm,Vfloat *top,
Vfloat *znear,Vfloat *zfar);VKI_EXTERN void vgl_DataBufBufferSize(vgl_DataBuf
*p,Vint xsize,Vint ysize);VKI_EXTERN void vgl_DataBufBufferRow(vgl_DataBuf
*p,Vint irow);VKI_EXTERN void vgl_DataBufBufferQuery(vgl_DataBuf
*p,vgl_FBuffer *fbuffer);VKI_EXTERN void vgl_DataBufDrawFun(vgl_DataBuf
*p,vgl_DrawFun *df);VKI_EXTERN void vgl_DataBufDispFun(vgl_DataBuf
*p,vgl_DispFun *df);
#ifdef __cplusplus
}
#endif
#define DATABUFsetpoly(p,flag)if((flag)!= (p)->polyflag)\
{ vgl_DataBuf_setpoly ((p),(flag));}
#endif

