                   /* DRAWIPC Interface object */ 
#ifndef DRAWIPC_DEF
#define DRAWIPC_DEF

#include <stdio.h>

#include "base/base.h"

#define DRAWIPC_CLIENT 1
#define DRAWIPC_SERVER 2

#define DRAWIPC_NET     1
#define DRAWIPC_LOCAL   2
#define DRAWIPC_USER    3

                   /* defines for function type */
#define DRAWIPC_FUN_MONITOR  1
#define DRAWIPC_FUN_WRITE    2
#define DRAWIPC_FUN_READ     3

                   /* defines for state */
#define DRAWIPC_BEFORE  0
#define DRAWIPC_AFTER   1

                   /* defines for get */
#define DRAWIPC_DRAWFUN  0
#define DRAWIPC_STATE    1
#define DRAWIPC_FILETYPE 2
#define DRAWIPC_DBNAME   3
#define DRAWIPC_FILENAME 4

                   /* defines for function prototypes */
typedef void (VDrawIPCMonitorFunc)(vgl_DrawIPC*,Vobject*);
typedef void (VDrawIPCWriteFunc)(Vobject*,Vint,Vchar*);
typedef void (VDrawIPCReadFunc)(Vobject*,Vint,Vchar*);

struct vgl_DrawIPC {
   Vint  ierr;
   Vint  iport;
   Vint  swap;
   Vint  type;
   Vchar hostname[2048];
   Vchar filename[2048];
   Vchar dbname[2048];
   vgl_DrawFun *df;
   VDrawIPCMonitorFunc *funmon;
   Vobject *funobjmon;
   Vobject *readobj;
   Vobject *writeobj;
   Vint abortflag;
   Vint action;
   Vint state;
   Vint filetype;
   Vchar localname[2048];
   VDrawIPCWriteFunc *write;
   VDrawIPCReadFunc  *read;
   Vint  *mem[3];
   Vint  nmem[3];

#ifdef __cplusplus
public:
   VKI_EXTERN vgl_DrawIPC(void);
   VKI_EXTERN void* operator new (size_t);
   VKI_EXTERN ~vgl_DrawIPC(void);
   VKI_EXTERN void operator delete (void*);
   VKI_EXTERN Vint Error();
   VKI_EXTERN void Def(Vint);
   VKI_EXTERN void Inq(Vint*);
   VKI_EXTERN void SetSwap (Vint);
   VKI_EXTERN void SetObject(Vint,Vobject*);
   VKI_EXTERN void GetObject(Vint,Vobject**);
   VKI_EXTERN void XfmPush();
   VKI_EXTERN void XfmPop();
   VKI_EXTERN void XfmLoad(Vfloat[4][4]);
   VKI_EXTERN void ProjLoad(Vfloat[4][4]);
   VKI_EXTERN void XfmMult(Vfloat[4][4]);
   VKI_EXTERN void ColorIndex(Vint);
   VKI_EXTERN void Color(Vfloat[3]);
   VKI_EXTERN void DataIndex(Vint,Vint,Vint*);
   VKI_EXTERN void Data(Vint,Vfloat[]);
   VKI_EXTERN void BackColorIndex(Vint);
   VKI_EXTERN void BackColor(Vfloat[3]);
   VKI_EXTERN void AttPush(Vint);
   VKI_EXTERN void AttPop();
   VKI_EXTERN void Polygon(Vint,Vint,Vfloat[][3],Vint,Vfloat[]);
   VKI_EXTERN void PolygonColor(Vint,Vint,Vfloat[][3],Vfloat[][3],Vint,
                                Vfloat[]);
   VKI_EXTERN void PolygonDC(Vint,Vint,Vfloat[3],Vint[][3]);
   VKI_EXTERN void PolygonData(Vint,Vint,Vfloat[][3],Vint,Vfloat[],Vint,
                               Vfloat[]);
   VKI_EXTERN void LineStyle(Vint);
   VKI_EXTERN void TransIndex(Vint);
   VKI_EXTERN void LineWidth(Vint);
   VKI_EXTERN void Trans(Vfloat);
   VKI_EXTERN void Specularity(Vfloat,Vfloat);
   VKI_EXTERN void Clip(Vint,Vint,Vint,Vint);
   VKI_EXTERN void Text(Vfloat[3],Vtchar*);
   VKI_EXTERN void TextDC(Vfloat[3],Vint[3],Vtchar*);
   VKI_EXTERN void PolyPoint(Vint,Vfloat[][3]);
   VKI_EXTERN void PolyPointColor(Vint,Vfloat[][3],Vfloat[][3]);
   VKI_EXTERN void PolyPointDC(Vint,Vfloat[3],Vint[][3]);
   VKI_EXTERN void PolyPointData(Vint,Vfloat[][3],Vint,Vfloat[]);
   VKI_EXTERN void PolyLine(Vint,Vint,Vfloat[][3]);
   VKI_EXTERN void PolyLineColor(Vint,Vint,Vfloat[][3],Vfloat[][3]);
   VKI_EXTERN void PolyLineDC(Vint,Vint,Vfloat[3],Vint[][3]);
   VKI_EXTERN void PolyLineData(Vint,Vint,Vfloat[][3],Vint,Vfloat[]);
   VKI_EXTERN void SetMode(Vint,Vint);
   VKI_EXTERN void SetFactors(Vint,Vfloat[]);
   VKI_EXTERN void PointSize(Vint);
   VKI_EXTERN void PositionWindow(Vint,Vint,Vint,Vint);
   VKI_EXTERN void VisualWindow(Vint);
   VKI_EXTERN void ParentWindow(Vword);
   VKI_EXTERN void ConfigureWindow(Vint,Vint*);
   VKI_EXTERN void OpenWindow(const Vchar*);
   VKI_EXTERN void CloseWindow();
   VKI_EXTERN void ConnectWindow(Vword);
   VKI_EXTERN void DisconnectWindow();
   VKI_EXTERN void SetWindow();
   VKI_EXTERN void QueryWindow(Vword*);
   VKI_EXTERN void Clear();
   VKI_EXTERN void Swap();
   VKI_EXTERN void Flush();
   VKI_EXTERN void Bell();
   VKI_EXTERN void Delay(Vfloat);
   VKI_EXTERN void Resize();
   VKI_EXTERN void GetMode(Vint,Vint*);
   VKI_EXTERN void GetInteger(Vint,Vint*);
   VKI_EXTERN void GetFloat(Vint,Vfloat*);
   VKI_EXTERN void GetString(Vint,Vchar*);
   VKI_EXTERN void FBufferRead(Vint,Vint,Vint,Vint,Vobject*);
   VKI_EXTERN void FBufferWrite(Vint,Vint,Vint,Vint,Vobject*);
   VKI_EXTERN void BufferQuery(Vobject*);
   VKI_EXTERN void BufferRow(Vint);
   VKI_EXTERN void BufferSize(Vint,Vint);
   VKI_EXTERN void ZBufferRead(Vint,Vint,Vint,Vint,Vobject*);
   VKI_EXTERN void ZBufferWrite(Vint,Vint,Vint,Vint,Vobject*);
   VKI_EXTERN void PixelmapCreate(Vobject*);
   VKI_EXTERN void PixelmapDestroy(Vobject*);
   VKI_EXTERN void PixelmapRead(Vobject*);
   VKI_EXTERN void PixelmapWrite(Vobject*);
   VKI_EXTERN void ClipPlane(Vint,Vfloat[4]);
   VKI_EXTERN void Light(Vint,Vint,Vfloat[3],Vfloat[3]);
   VKI_EXTERN void LightSwitch(Vint,Vint);
   VKI_EXTERN void LightModel(Vint,Vfloat,Vfloat);
   VKI_EXTERN void SetSwitch(Vint,Vint,Vint);
   VKI_EXTERN void ProjOrtho(Vfloat,Vfloat,Vfloat,Vfloat,Vfloat,Vfloat);
   VKI_EXTERN void ProjFrustum(Vfloat,Vfloat,Vfloat,Vfloat,Vfloat,Vfloat);
   VKI_EXTERN void ProjPush();
   VKI_EXTERN void ProjPop();
   VKI_EXTERN void Viewport(Vint,Vint,Vint,Vint);
   VKI_EXTERN void DepthRange(Vfloat,Vfloat);
   VKI_EXTERN void PolygonOffset(Vfloat,Vfloat);
   VKI_EXTERN void PolygonMode(Vint);
   VKI_EXTERN void TextPlane(Vfloat[3],Vfloat[3]);
   VKI_EXTERN void TextPixelSize(Vfloat);
   VKI_EXTERN void BitmapDefine(Vint,Vobject*);
   VKI_EXTERN void BitmapSelect(Vint);
   VKI_EXTERN void TextureDefine(Vint,Vobject*);
   VKI_EXTERN void TextureSelect(Vint);
   VKI_EXTERN void RasFontDefine(Vint,Vobject*);
   VKI_EXTERN void RasFontSelect(Vint);
   VKI_EXTERN void PollMouse(Vint*,Vint*,Vint*,Vint*,Vint*);
   VKI_EXTERN void WarpMouse(Vint,Vint);
   VKI_EXTERN void SetCursor(Vint);
   VKI_EXTERN void PollModifiers(Vint*,Vint*);
   VKI_EXTERN void ReadQueue(Vint*,Vint*);
   VKI_EXTERN void TestQueue(Vint*);
   VKI_EXTERN void ResetQueue();
   VKI_EXTERN void Render(Vint);
   VKI_EXTERN void SelectRegion(Vint,Vint*);
   VKI_EXTERN void SelectBuffer(Vint,Vint*,Vfloat*,Vfloat*);
   VKI_EXTERN void SelectQuery(Vint*);
   VKI_EXTERN void ExtentQuery(Vfloat*,Vfloat*,Vfloat*,Vfloat*,Vfloat*,Vfloat*);
   VKI_EXTERN void PolyPointArray(Vint,Vfloat[][3],Vint,void*,Vint,void*,Vint,
                                  Vfloat*,Vint,Vfloat*);
   VKI_EXTERN void PolyLineArray(Vint,Vint,Vfloat[][3],Vint,void*,Vint,void*,
                                 Vint,Vfloat*,Vint,Vfloat*);
   VKI_EXTERN void PolygonArray(Vint,Vint,Vfloat[][3],Vint,void*,Vint,void*,
                                Vint,Vfloat*,Vint,Vfloat*);
   VKI_EXTERN void InitBuffer(Vint,Vint*);
   VKI_EXTERN void TermBuffer(Vint);
   VKI_EXTERN void CopyBuffer(Vint,Vint,Vfloat[][3],Vint,void*,Vint,void*,Vint,
                              Vfloat*,Vint,Vfloat*,Vint*);
   VKI_EXTERN void PolyPointBuffer(Vint,Vint,Vint,Vint,Vint,Vint,Vint);
   VKI_EXTERN void PolyLineBuffer(Vint,Vint,Vint,Vint,Vint,Vint,Vint,Vint);
   VKI_EXTERN void PolygonBuffer(Vint,Vint,Vint,Vint,Vint,Vint,Vint,Vint);
   VKI_EXTERN void StartServer();
   VKI_EXTERN void StopServer();
   VKI_EXTERN void DrawFun(vgl_DrawFun*);
   VKI_EXTERN void SetFunction(Vint,Vfunc*,Vobject*);
   VKI_EXTERN void SetMonitorFunction(VDrawIPCMonitorFunc*,Vobject*);
   VKI_EXTERN void SetReadFunction(VDrawIPCReadFunc*,Vobject*);
   VKI_EXTERN void SetWriteFunction(VDrawIPCWriteFunc*,Vobject*);
   VKI_EXTERN void Abort();
#endif
};

#ifdef  __cplusplus
extern "C" {
#endif

VKI_EXTERN vgl_DrawIPC*
vgl_DrawIPCBegin(void);
VKI_EXTERN void
vgl_DrawIPC_Construct(vgl_DrawIPC *p);
VKI_EXTERN void
vgl_DrawIPCEnd(vgl_DrawIPC *p);
VKI_EXTERN void
vgl_DrawIPC_Destruct(vgl_DrawIPC *p);
VKI_EXTERN Vint
vgl_DrawIPCError(vgl_DrawIPC *p);
VKI_EXTERN void
vgl_DrawIPCDef(vgl_DrawIPC *p, Vint type);
VKI_EXTERN void
vgl_DrawIPCInq(vgl_DrawIPC *p, Vint *type);
VKI_EXTERN void
vgl_DrawIPCSetSwap (vgl_DrawIPC *p, Vint swap);
VKI_EXTERN void
vgl_DrawIPCSetObject(vgl_DrawIPC *p, Vint type, Vobject *obj);
VKI_EXTERN void
vgl_DrawIPCGetObject(vgl_DrawIPC *p, Vint type, Vobject **obj);
VKI_EXTERN void
vgl_DrawIPCXfmPush(vgl_DrawIPC *p);
VKI_EXTERN void
vgl_DrawIPCXfmPop(vgl_DrawIPC *p);
VKI_EXTERN void
vgl_DrawIPCXfmLoad(vgl_DrawIPC *p, Vfloat xfm[4][4]);
VKI_EXTERN void
vgl_DrawIPCProjLoad(vgl_DrawIPC *p, Vfloat proj[4][4]);
VKI_EXTERN void
vgl_DrawIPCXfmMult(vgl_DrawIPC *p, Vfloat xfm[4][4]);
VKI_EXTERN void
vgl_DrawIPCColorIndex(vgl_DrawIPC *p, Vint index);
VKI_EXTERN void
vgl_DrawIPCColor(vgl_DrawIPC *p, Vfloat c[3]);
VKI_EXTERN void
vgl_DrawIPCDataIndex(vgl_DrawIPC *p, Vint nprims, Vint nrws, Vint *index);
VKI_EXTERN void
vgl_DrawIPCData(vgl_DrawIPC *p, Vint nrws, Vfloat d[]);
VKI_EXTERN void
vgl_DrawIPCBackColorIndex(vgl_DrawIPC *p, Vint index);
VKI_EXTERN void
vgl_DrawIPCBackColor(vgl_DrawIPC *p, Vfloat c[3]);
VKI_EXTERN void
vgl_DrawIPCAttPush(vgl_DrawIPC *p, Vint mask);
VKI_EXTERN void
vgl_DrawIPCAttPop(vgl_DrawIPC *p);
VKI_EXTERN void
vgl_DrawIPCPolygon(vgl_DrawIPC *p, Vint type,
                   Vint npts, Vfloat x[][3], Vint vtflag, Vfloat vt[]);
VKI_EXTERN void
vgl_DrawIPCPolygonColor(vgl_DrawIPC *p, Vint type,
                        Vint npts, Vfloat x[][3], Vfloat c[][3],
                        Vint vtflag, Vfloat vt[]);
VKI_EXTERN void
vgl_DrawIPCPolygonDC(vgl_DrawIPC *p, Vint type,
                     Vint npts, Vfloat x[3], Vint dc[][3]);
VKI_EXTERN void
vgl_DrawIPCPolygonData(vgl_DrawIPC *p, Vint type,
                        Vint npts, Vfloat x[][3], Vint nrws,
                        Vfloat d[], Vint vtflag, Vfloat vt[]);
VKI_EXTERN void
vgl_DrawIPCLineStyle(vgl_DrawIPC *p, Vint lstyle);
VKI_EXTERN void
vgl_DrawIPCTransIndex(vgl_DrawIPC *p, Vint index);
VKI_EXTERN void
vgl_DrawIPCLineWidth(vgl_DrawIPC *p, Vint lwidth);
VKI_EXTERN void
vgl_DrawIPCTrans(vgl_DrawIPC *p, Vfloat trans);
VKI_EXTERN void
vgl_DrawIPCSpecularity(vgl_DrawIPC *p, Vfloat intensity, Vfloat shininess);
VKI_EXTERN void
vgl_DrawIPCClip(vgl_DrawIPC *p, Vint xmin, Vint xmax, Vint ymin, Vint ymax);
VKI_EXTERN void
vgl_DrawIPCText(vgl_DrawIPC *p, Vfloat x[3], Vtchar *string);
VKI_EXTERN void
vgl_DrawIPCTextDC(vgl_DrawIPC *p, Vfloat x[3], Vint dc[3], Vtchar *string);
VKI_EXTERN void
vgl_DrawIPCPolyPoint(vgl_DrawIPC *p, Vint npts, Vfloat x[][3]);
VKI_EXTERN void
vgl_DrawIPCPolyPointColor(vgl_DrawIPC *p,
                          Vint npts, Vfloat x[][3], Vfloat c[][3]);
VKI_EXTERN void
vgl_DrawIPCPolyPointDC(vgl_DrawIPC *p, Vint npts, Vfloat x[3], Vint dc[][3]);
VKI_EXTERN void
vgl_DrawIPCPolyPointData(vgl_DrawIPC *p,
                         Vint npts, Vfloat x[][3], Vint nrws, Vfloat d[]);
VKI_EXTERN void
vgl_DrawIPCPolyLine(vgl_DrawIPC *p, Vint type, Vint npts, Vfloat x[][3]);
VKI_EXTERN void
vgl_DrawIPCPolyLineColor(vgl_DrawIPC *p, Vint type,
                         Vint npts, Vfloat x[][3], Vfloat c[][3]);
VKI_EXTERN void
vgl_DrawIPCPolyLineDC(vgl_DrawIPC *p, Vint type,
                      Vint npts, Vfloat x[3], Vint dc[][3]);
VKI_EXTERN void
vgl_DrawIPCPolyLineData(vgl_DrawIPC *p, Vint type,
                        Vint npts, Vfloat x[][3], Vint nrws, Vfloat d[]);
VKI_EXTERN void
vgl_DrawIPCSetMode(vgl_DrawIPC *p, Vint mode, Vint flag);
VKI_EXTERN void
vgl_DrawIPCSetFactors(vgl_DrawIPC *p, Vint type, Vfloat factors[]);
VKI_EXTERN void
vgl_DrawIPCPointSize(vgl_DrawIPC *p, Vint psize);
VKI_EXTERN void
vgl_DrawIPCPositionWindow(vgl_DrawIPC *p,
                          Vint xorig, Vint yorig, Vint xsize, Vint ysize);
VKI_EXTERN void
vgl_DrawIPCVisualWindow(vgl_DrawIPC *p, Vint visflag);
VKI_EXTERN void
vgl_DrawIPCParentWindow(vgl_DrawIPC *p, Vword window);
VKI_EXTERN void
vgl_DrawIPCConfigureWindow(vgl_DrawIPC *p, Vint type, Vint *params);
VKI_EXTERN void
vgl_DrawIPCOpenWindow(vgl_DrawIPC *p, const Vchar *title);
VKI_EXTERN void
vgl_DrawIPCCloseWindow(vgl_DrawIPC *p);
VKI_EXTERN void
vgl_DrawIPCConnectWindow(vgl_DrawIPC *p, Vword window);
VKI_EXTERN void
vgl_DrawIPCDisconnectWindow(vgl_DrawIPC *p);
VKI_EXTERN void
vgl_DrawIPCSetWindow(vgl_DrawIPC *p);
VKI_EXTERN void
vgl_DrawIPCQueryWindow(vgl_DrawIPC *p, Vword *window);
VKI_EXTERN void
vgl_DrawIPCClear(vgl_DrawIPC *p);
VKI_EXTERN void
vgl_DrawIPCSwap(vgl_DrawIPC *p);
VKI_EXTERN void
vgl_DrawIPCFlush(vgl_DrawIPC *p);
VKI_EXTERN void
vgl_DrawIPCBell(vgl_DrawIPC *p);
VKI_EXTERN void
vgl_DrawIPCDelay(vgl_DrawIPC *p, Vfloat sec);
VKI_EXTERN void
vgl_DrawIPCResize(vgl_DrawIPC *p);
VKI_EXTERN void
vgl_DrawIPCGetMode(vgl_DrawIPC *p, Vint mode, Vint *flag);
VKI_EXTERN void
vgl_DrawIPCGetInteger(vgl_DrawIPC *p, Vint type, Vint *params);
VKI_EXTERN void
vgl_DrawIPCGetFloat(vgl_DrawIPC *p, Vint type, Vfloat *params);
VKI_EXTERN void
vgl_DrawIPCGetString(vgl_DrawIPC *p, Vint type, Vchar *params);
VKI_EXTERN void
vgl_DrawIPCFBufferRead(vgl_DrawIPC *p,
                       Vint left, Vint right, Vint bottom, Vint top,
                       Vobject *obj);
VKI_EXTERN void
vgl_DrawIPCFBufferWrite(vgl_DrawIPC *p,
                        Vint left, Vint right, Vint bottom, Vint top,
                        Vobject *obj);
VKI_EXTERN void
vgl_DrawIPCBufferQuery(vgl_DrawIPC *p, Vobject *obj);
VKI_EXTERN void
vgl_DrawIPCBufferRow(vgl_DrawIPC *p, Vint irow);
VKI_EXTERN void
vgl_DrawIPCBufferSize(vgl_DrawIPC *p, Vint xsize, Vint ysize);
VKI_EXTERN void
vgl_DrawIPCZBufferRead(vgl_DrawIPC *p,
                       Vint left, Vint right, Vint bottom, Vint top,
                       Vobject *obj);
VKI_EXTERN void
vgl_DrawIPCZBufferWrite(vgl_DrawIPC *p,
                        Vint left, Vint right, Vint bottom, Vint top,
                        Vobject *obj);
VKI_EXTERN void
vgl_DrawIPCPixelmapCreate(vgl_DrawIPC *p, Vobject *pixelmap);
VKI_EXTERN void
vgl_DrawIPCPixelmapDestroy(vgl_DrawIPC *p, Vobject *pixelmap);
VKI_EXTERN void
vgl_DrawIPCPixelmapRead(vgl_DrawIPC *p, Vobject *pixelmap);
VKI_EXTERN void
vgl_DrawIPCPixelmapWrite(vgl_DrawIPC *p, Vobject *pixelmap);
VKI_EXTERN void
vgl_DrawIPCClipPlane(vgl_DrawIPC *p, Vint index, Vfloat equation[4]);
VKI_EXTERN void
vgl_DrawIPCLight(vgl_DrawIPC *p, Vint index, Vint type, Vfloat c[3],
                 Vfloat x[3]);
VKI_EXTERN void
vgl_DrawIPCLightSwitch(vgl_DrawIPC *p, Vint index, Vint flag);
VKI_EXTERN void
vgl_DrawIPCLightModel(vgl_DrawIPC *p,
                      Vint localviewer, Vfloat koffset, Vfloat krate);
VKI_EXTERN void
vgl_DrawIPCSetSwitch(vgl_DrawIPC *p, Vint type, Vint index, Vint flag);
VKI_EXTERN void
vgl_DrawIPCProjOrtho(vgl_DrawIPC *p,
                     Vfloat left, Vfloat right, Vfloat bottom, Vfloat top,
                     Vfloat nearz, Vfloat farz);
VKI_EXTERN void
vgl_DrawIPCProjFrustum(vgl_DrawIPC *p,
                       Vfloat left, Vfloat right, Vfloat bottom, Vfloat top,
                       Vfloat nearz, Vfloat farz);
VKI_EXTERN void
vgl_DrawIPCProjPush(vgl_DrawIPC *p);
VKI_EXTERN void
vgl_DrawIPCProjPop(vgl_DrawIPC *p);
VKI_EXTERN void
vgl_DrawIPCViewport(vgl_DrawIPC *p, Vint left, Vint right, Vint bottom,
                    Vint top);
VKI_EXTERN void
vgl_DrawIPCDepthRange(vgl_DrawIPC *p, Vfloat nearz, Vfloat farz);
VKI_EXTERN void
vgl_DrawIPCPolygonOffset(vgl_DrawIPC *p, Vfloat factor, Vfloat bias);
VKI_EXTERN void
vgl_DrawIPCPolygonMode(vgl_DrawIPC *p, Vint rend);
VKI_EXTERN void
vgl_DrawIPCTextPlane(vgl_DrawIPC *p, Vfloat path[3], Vfloat plane[3]);
VKI_EXTERN void
vgl_DrawIPCTextPixelSize(vgl_DrawIPC *p, Vfloat pixelsize);
VKI_EXTERN void
vgl_DrawIPCBitmapDefine(vgl_DrawIPC *p, Vint index, Vobject *obj);
VKI_EXTERN void
vgl_DrawIPCBitmapSelect(vgl_DrawIPC *p, Vint index);
VKI_EXTERN void
vgl_DrawIPCTextureDefine(vgl_DrawIPC *p, Vint index, Vobject *obj);
VKI_EXTERN void
vgl_DrawIPCTextureSelect(vgl_DrawIPC *p, Vint index);
VKI_EXTERN void
vgl_DrawIPCRasFontDefine(vgl_DrawIPC *p, Vint index, Vobject *obj);
VKI_EXTERN void
vgl_DrawIPCRasFontSelect(vgl_DrawIPC *p, Vint index);
VKI_EXTERN void
vgl_DrawIPCPollMouse(vgl_DrawIPC *p,
                     Vint *px, Vint *py, Vint *but1, Vint *but2, Vint *but3);
VKI_EXTERN void
vgl_DrawIPCWarpMouse(vgl_DrawIPC *p, Vint px, Vint py);
VKI_EXTERN void
vgl_DrawIPCSetCursor(vgl_DrawIPC *p, Vint type);
VKI_EXTERN void
vgl_DrawIPCPollModifiers(vgl_DrawIPC *p, Vint *cntl, Vint *shft);
VKI_EXTERN void
vgl_DrawIPCReadQueue(vgl_DrawIPC *p, Vint *dev, Vint *val);
VKI_EXTERN void
vgl_DrawIPCTestQueue(vgl_DrawIPC *p, Vint *dev);
VKI_EXTERN void
vgl_DrawIPCResetQueue(vgl_DrawIPC *p);
VKI_EXTERN void
vgl_DrawIPCRender(vgl_DrawIPC *p, Vint type);
VKI_EXTERN void
vgl_DrawIPCSelectRegion(vgl_DrawIPC *p, Vint type, Vint *params);
VKI_EXTERN void
vgl_DrawIPCSelectBuffer(vgl_DrawIPC *p, Vint size, Vint *indexlist,
                      Vfloat *mindepth, Vfloat *maxdepth);
VKI_EXTERN void
vgl_DrawIPCSelectQuery(vgl_DrawIPC *p, Vint *hits);
VKI_EXTERN void
vgl_DrawIPCExtentQuery(vgl_DrawIPC *p,
                       Vfloat *lft, Vfloat *rgt, Vfloat *btm, Vfloat *top,
                       Vfloat *znear, Vfloat *zfar);
VKI_EXTERN void
vgl_DrawIPCPolyPointArray(vgl_DrawIPC *p, Vint npts, Vfloat x[][3],
                          Vint cflag, void *c,
                          Vint vflag, void *v,
                          Vint tflag, Vfloat *t,
                          Vint dflag, Vfloat *d);
VKI_EXTERN void
vgl_DrawIPCPolyLineArray(vgl_DrawIPC *p, Vint type, Vint npts,
                         Vfloat x[][3],
                         Vint cflag, void *c,
                         Vint vflag, void *v,
                         Vint tflag, Vfloat *t,
                         Vint dflag, Vfloat *d);
VKI_EXTERN void
vgl_DrawIPCPolygonArray(vgl_DrawIPC *p, Vint type, Vint npts, Vfloat x[][3],
                        Vint cflag, void *c,
                        Vint vflag, void *v,
                        Vint tflag, Vfloat *t,
                        Vint dflag, Vfloat *d);
VKI_EXTERN void
vgl_DrawIPCInitBuffer(vgl_DrawIPC *p, Vint size, Vint *vboid);
VKI_EXTERN void
vgl_DrawIPCTermBuffer(vgl_DrawIPC *p, Vint vboid);
VKI_EXTERN void
vgl_DrawIPCCopyBuffer(vgl_DrawIPC *p, Vint vboid, Vint npts, Vfloat x[][3],
                      Vint cflag, void *c, Vint vflag, void *v,
                      Vint tflag, Vfloat *t, Vint dflag, Vfloat *d,
                      Vint *offset);
VKI_EXTERN void
vgl_DrawIPCPolyPointBuffer(vgl_DrawIPC *p, Vint vboid, Vint offset,
                           Vint npts, Vint cflag, Vint vflag,
                           Vint tflag, Vint dflag);
VKI_EXTERN void
vgl_DrawIPCPolyLineBuffer(vgl_DrawIPC *p, Vint vboid, Vint offset,
                          Vint type, Vint npts, Vint cflag,
                          Vint vflag, Vint tflag, Vint dflag);
VKI_EXTERN void
vgl_DrawIPCPolygonBuffer(vgl_DrawIPC *p, Vint vboid, Vint offset,
                         Vint type, Vint npts, Vint cflag,
                         Vint vflag, Vint tflag, Vint dflag);
VKI_EXTERN void
vgl_DrawIPCStartServer(vgl_DrawIPC *p);
VKI_EXTERN void
vgl_DrawIPCStopServer(vgl_DrawIPC *p);
VKI_EXTERN void
vgl_DrawIPCDrawFun(vgl_DrawIPC *p, vgl_DrawFun *df);
VKI_EXTERN void
vgl_DrawIPCSetFunction(vgl_DrawIPC *p, Vint functype,
                       Vfunc *function, Vobject *object);
VKI_EXTERN void
vgl_DrawIPCSetMonitorFunction(vgl_DrawIPC *p, VDrawIPCMonitorFunc *function,
                              Vobject *object);
VKI_EXTERN void
vgl_DrawIPCSetReadFunction(vgl_DrawIPC *p, VDrawIPCReadFunc *function,
                           Vobject *object);
VKI_EXTERN void
vgl_DrawIPCSetWriteFunction(vgl_DrawIPC *p, VDrawIPCWriteFunc *function,
                            Vobject *object);
VKI_EXTERN void
vgl_DrawIPCAbort(vgl_DrawIPC *p);

#ifdef  __cplusplus
}
#endif

#endif
