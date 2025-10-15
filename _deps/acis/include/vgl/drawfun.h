                   /* DrawFun object */ 

#ifndef DRAWFUN_DEF
#define DRAWFUN_DEF

#include "base/basedefs.h"
#include "vgl/vgldefs.h"

                   /* defines for function prototypes */
typedef void (VDrawFunColorIndex)       (Vobject*, Vint);
typedef void (VDrawFunColor)            (Vobject*, Vfloat[3]);
typedef void (VDrawFunPolygon)          (Vobject*, Vint, Vint,Vfloat[][3],Vint,Vfloat[]);
typedef void (VDrawFunTextPlane)        (Vobject*, Vfloat[3],Vfloat[3]);
typedef void (VDrawFunXfmPush)          (Vobject*);
typedef void (VDrawFunXfmPop)           (Vobject*);
typedef void (VDrawFunLineStyle)        (Vobject*, Vint);
typedef void (VDrawFunLineWidth)        (Vobject*, Vint);
typedef void (VDrawFunTrans)            (Vobject*, Vfloat);
typedef void (VDrawFunPolygonColor)     (Vobject*, Vint, Vint, Vfloat[][3], Vfloat[][3],
                                       Vint, Vfloat[]);
typedef void (VDrawFunClip)             (Vobject*, Vint, Vint, Vint, Vint);
typedef void (VDrawFunText)             (Vobject*, Vfloat[3], Vtchar*);
typedef void (VDrawFunDelay)            (Vobject*, Vfloat);
typedef void (VDrawFunSpecularity)      (Vobject*, Vfloat, Vfloat);
typedef void (VDrawFunClipPlane)        (Vobject*, Vint, Vfloat[4]);
typedef void (VDrawFunPolyPoint)        (Vobject*, Vint, Vfloat[][3]);
typedef void (VDrawFunPolyPointColor)   (Vobject*, Vint, Vfloat[][3], Vfloat[][3]);
typedef void (VDrawFunPolyLine)         (Vobject*, Vint, Vint, Vfloat[][3]);
typedef void (VDrawFunPolyLineColor)    (Vobject*, Vint, Vint, Vfloat[][3], Vfloat[][3]);
typedef void (VDrawFunSetMode)          (Vobject*, Vint, Vint);
typedef void (VDrawFunPointSize)        (Vobject*, Vint);
typedef void (VDrawFunXfmLoad)          (Vobject*, Vfloat[4][4]);
typedef void (VDrawFunXfmMult)          (Vobject*, Vfloat[4][4]);
typedef void (VDrawFunTextDC)           (Vobject*, Vfloat[3], Vint[3], Vtchar*);
typedef void (VDrawFunPolyLineDC)       (Vobject*, Vint, Vint, Vfloat[3], Vint[][3]);
typedef void (VDrawFunPolygonDC)        (Vobject*, Vint, Vint, Vfloat[3], Vint[][3]);
typedef void (VDrawFunPolyPointDC)      (Vobject*, Vint, Vfloat[3], Vint[][3]);
typedef void (VDrawFunTransIndex)       (Vobject*, Vint);
typedef void (VDrawFunPolyPointData)    (Vobject*, Vint, Vfloat[][3], Vint, Vfloat*);
typedef void (VDrawFunPolyLineData)     (Vobject*, Vint, Vint, Vfloat[][3], Vint, Vfloat*);
typedef void (VDrawFunPolygonData)      (Vobject*, Vint, Vint, Vfloat[][3], Vint, Vfloat*,
                                       Vint, Vfloat[]);
typedef void (VDrawFunPolygonMode)      (Vobject*, Vint);
typedef void (VDrawFunDataIndex)   (Vobject*, Vint, Vint, Vint*);
typedef void (VDrawFunData)             (Vobject*, Vint, Vfloat[]);
typedef void (VDrawFunAttPush)          (Vobject*, Vint);
typedef void (VDrawFunAttPop)           (Vobject*);
typedef void (VDrawFunSetSwitch)        (Vobject*, Vint, Vint, Vint);
typedef void (VDrawFunTextureDefine)    (Vobject*, Vint, Vobject*);
typedef void (VDrawFunTextureSelect)    (Vobject*, Vint);
typedef void (VDrawFunClear)            (Vobject*);
typedef void (VDrawFunSwap)             (Vobject*);
typedef void (VDrawFunFlush)            (Vobject*);
typedef void (VDrawFunBell)             (Vobject*);
typedef void (VDrawFunDepthRange)       (Vobject*, Vfloat, Vfloat);
typedef void (VDrawFunRasFontDefine)    (Vobject*, Vint, Vobject*);
typedef void (VDrawFunRasFontSelect)    (Vobject*, Vint);
typedef void (VDrawFunLightModel)       (Vobject*, Vint, Vfloat, Vfloat);
typedef void (VDrawFunGetString)        (Vobject*, Vint, Vchar*);
typedef void (VDrawFunResize)           (Vobject*);
typedef void (VDrawFunBackColorIndex)   (Vobject*, Vint);
typedef void (VDrawFunBackColor)        (Vobject*, Vfloat[3]);
typedef void (VDrawFunGetMode)          (Vobject*, Vint, Vint*);
typedef void (VDrawFunGetInteger)       (Vobject*, Vint, Vint*);
typedef void (VDrawFunGetFloat)         (Vobject*, Vint, Vfloat*);
typedef void (VDrawFunLight)            (Vobject*, Vint, Vint, Vfloat[3], Vfloat[3]);
typedef void (VDrawFunLightSwitch)      (Vobject*, Vint, Vint);
typedef void (VDrawFunPollMouse)        (Vobject*, Vint*, Vint*, Vint*, Vint*, Vint*);
typedef void (VDrawFunPollModifiers)    (Vobject*, Vint*, Vint*);
typedef void (VDrawFunWarpMouse)        (Vobject*, Vint, Vint);
typedef void (VDrawFunSetCursor)        (Vobject*, Vint);
typedef void (VDrawFunReadQueue)        (Vobject*, Vint*, Vint*);
typedef void (VDrawFunTestQueue)        (Vobject*, Vint*);
typedef void (VDrawFunResetQueue)       (Vobject*);
typedef void (VDrawFunTextPixelSize)    (Vobject*, Vfloat);
typedef void (VDrawFunFBufferRead)      (Vobject*, Vint, Vint, Vint, Vint, Vobject*);
typedef void (VDrawFunFBufferWrite)     (Vobject*, Vint, Vint, Vint, Vint, Vobject*);
typedef void (VDrawFunPixelmapCreate)   (Vobject*, Vobject*);
typedef void (VDrawFunPixelmapDestroy)  (Vobject*, Vobject*);
typedef void (VDrawFunPixelmapRead)     (Vobject*, Vobject*);
typedef void (VDrawFunPixelmapWrite)    (Vobject*, Vobject*);
typedef void (VDrawFunProjOrtho)      (Vobject*, Vfloat,Vfloat,Vfloat,Vfloat,Vfloat,Vfloat);
typedef void (VDrawFunProjFrustum)    (Vobject*, Vfloat,Vfloat,Vfloat,Vfloat,Vfloat,Vfloat);
typedef void (VDrawFunViewport)        (Vobject*, Vint, Vint, Vint, Vint);
typedef void (VDrawFunPositionWindow)  (Vobject*, Vint, Vint, Vint, Vint);
typedef void (VDrawFunVisualWindow)    (Vobject*, Vint);
typedef void (VDrawFunOpenWindow)      (Vobject*, const Vchar*);
typedef void (VDrawFunCloseWindow)     (Vobject*);
typedef void (VDrawFunConnectWindow)   (Vobject*, Vword);
typedef void (VDrawFunDisconnectWindow) (Vobject*);
typedef void (VDrawFunSetWindow)       (Vobject*);
typedef void (VDrawFunQueryWindow)     (Vobject*, Vword*);
typedef void (VDrawFunParentWindow)    (Vobject*, Vword);
typedef void (VDrawFunConfigureWindow) (Vobject*, Vint, Vint*);
typedef void (VDrawFunRender)          (Vobject*, Vint);
typedef void (VDrawFunSelectRegion)    (Vobject*, Vint, Vint*);
typedef void (VDrawFunSelectBuffer)    (Vobject*, Vint, Vint*, Vfloat*, Vfloat*); 
typedef void (VDrawFunSelectQuery)     (Vobject*, Vint*);
typedef void (VDrawFunExtentQuery)     (Vobject*, Vfloat*,Vfloat*,Vfloat*,Vfloat*,
                                      Vfloat*,Vfloat*);
typedef void (VDrawFunPolygonOffset)   (Vobject*, Vfloat, Vfloat);
typedef void (VDrawFunProjPush)        (Vobject*);
typedef void (VDrawFunProjPop)         (Vobject*);
typedef void (VDrawFunBitmapDefine)    (Vobject*, Vint, Vobject*);
typedef void (VDrawFunBitmapSelect)    (Vobject*, Vint);
typedef void (VDrawFunZBufferRead)     (Vobject*, Vint, Vint, Vint, Vint, Vobject*);
typedef void (VDrawFunZBufferWrite)    (Vobject*, Vint, Vint, Vint, Vint, Vobject*);
typedef void (VDrawFunPolyPointArray)  (Vobject*, Vint, Vfloat[][3], Vint, void*, Vint, void*, Vint, Vfloat*, Vint, Vfloat*);
typedef void (VDrawFunPolyLineArray)   (Vobject*, Vint, Vint, Vfloat[][3], Vint, void*, Vint, void*, Vint, Vfloat*, Vint, Vfloat*);
typedef void (VDrawFunPolygonArray)    (Vobject*, Vint, Vint, Vfloat[][3], Vint, void*, Vint, void*, Vint, Vfloat*, Vint, Vfloat*);
typedef void (VDrawFunInitBuffer)      (Vobject*, Vint, Vint*);
typedef void (VDrawFunTermBuffer)      (Vobject*, Vint);
typedef void (VDrawFunCopyBuffer)      (Vobject*, Vint, Vint, Vfloat[][3], Vint, void*, Vint, void*, Vint, Vfloat*, Vint, Vfloat*, Vint*);
typedef void (VDrawFunPolyPointBuffer) (Vobject*, Vint, Vint, Vint, Vint, Vint, Vint, Vint);
typedef void (VDrawFunPolyLineBuffer)  (Vobject*, Vint, Vint, Vint, Vint, Vint, Vint, Vint, Vint);
typedef void (VDrawFunPolygonBuffer)   (Vobject*, Vint, Vint, Vint, Vint, Vint, Vint, Vint, Vint);
typedef void (VDrawFunSetFactors)      (Vobject*, Vint, Vfloat[]);
typedef void (VDrawFunReadBuffer)      (Vobject*, Vint, Vint, Vint, Vfloat[][3], Vint, void*, Vint, void*, Vint, Vfloat*, Vint, Vfloat*);
typedef void (VDrawFunBufferSize)      (Vobject*,Vint,Vint);
typedef void (VDrawFunBufferRow)       (Vobject*, Vint);
typedef void (VDrawFunBufferQuery)     (Vobject*, Vobject*);
typedef void (VDrawFunProjLoad)        (Vobject*, Vfloat[4][4]);
typedef Vint (VDrawFunError)           (Vobject*);

                   /* defines for function type */
#define DRAWFUN_UNDEFINED         -1
#define DRAWFUN_COLORINDEX         1
#define DRAWFUN_COLOR              2
#define DRAWFUN_POLYGON            3
#define DRAWFUN_TEXTPLANE          4
#define DRAWFUN_XFMPUSH            5
#define DRAWFUN_XFMPOP             6
#define DRAWFUN_LINESTYLE          7
#define DRAWFUN_LINEWIDTH          8
#define DRAWFUN_TRANS              9
#define DRAWFUN_POLYGONCOLOR      10
#define DRAWFUN_CLIP              11
#define DRAWFUN_TEXT              12
#define DRAWFUN_DELAY             13
#define DRAWFUN_SPECULARITY       14
#define DRAWFUN_CLIPPLANE         15
#define DRAWFUN_POLYPOINT         16
#define DRAWFUN_POLYPOINTCOLOR    17
#define DRAWFUN_POLYLINE          18
#define DRAWFUN_POLYLINECOLOR     19
#define DRAWFUN_SETMODE           20
#define DRAWFUN_POINTSIZE         21
#define DRAWFUN_XFMLOAD           22
#define DRAWFUN_XFMMULT           23
#define DRAWFUN_TEXTDC            24
#define DRAWFUN_POLYLINEDC        25
#define DRAWFUN_POLYGONDC         26
#define DRAWFUN_POLYPOINTDC       27
#define DRAWFUN_TRANSINDEX        28
#define DRAWFUN_POLYPOINTDATA     29
#define DRAWFUN_POLYLINEDATA      30
#define DRAWFUN_POLYGONDATA       31
#define DRAWFUN_POLYGONMODE       32
#define DRAWFUN_DATAINDEX         33
#define DRAWFUN_DATA              34
#define DRAWFUN_ATTPUSH           35
#define DRAWFUN_ATTPOP            36
#define DRAWFUN_SETSWITCH         37
#define DRAWFUN_TEXTUREDEFINE     38
#define DRAWFUN_TEXTURESELECT     39
#define DRAWFUN_CLEAR             40
#define DRAWFUN_SWAP              41
#define DRAWFUN_FLUSH             42
#define DRAWFUN_BELL              43
#define DRAWFUN_DEPTHRANGE        44
#define DRAWFUN_RASFONTDEFINE     45
#define DRAWFUN_RASFONTSELECT     46
#define DRAWFUN_LIGHTMODEL        47
#define DRAWFUN_GETSTRING         48
#define DRAWFUN_RESIZE            49
#define DRAWFUN_BACKCOLORINDEX    50
#define DRAWFUN_BACKCOLOR         51
#define DRAWFUN_GETMODE           52
#define DRAWFUN_GETINTEGER        53
#define DRAWFUN_GETFLOAT          54
#define DRAWFUN_LIGHT             55
#define DRAWFUN_LIGHTSWITCH       56
#define DRAWFUN_POLLMOUSE         57
#define DRAWFUN_POLLMODIFIERS     58
#define DRAWFUN_WARPMOUSE         59
#define DRAWFUN_SETCURSOR         60
#define DRAWFUN_READQUEUE         61
#define DRAWFUN_TESTQUEUE         62
#define DRAWFUN_RESETQUEUE        63
#define DRAWFUN_TEXTPIXELSIZE     64
#define DRAWFUN_FBUFFERREAD       65
#define DRAWFUN_FBUFFERWRITE      66
#define DRAWFUN_PIXELMAPCREATE    67
#define DRAWFUN_PIXELMAPDESTROY   68
#define DRAWFUN_PIXELMAPREAD      69
#define DRAWFUN_PIXELMAPWRITE     70
#define DRAWFUN_PROJORTHO         71
#define DRAWFUN_PROJFRUSTUM       72
#define DRAWFUN_VIEWPORT          73
#define DRAWFUN_POSITIONWINDOW    74
#define DRAWFUN_VISUALWINDOW      75
#define DRAWFUN_OPENWINDOW        76
#define DRAWFUN_CLOSEWINDOW       77
#define DRAWFUN_CONNECTWINDOW     78
#define DRAWFUN_DISCONNECTWINDOW  79
#define DRAWFUN_SETWINDOW         80
#define DRAWFUN_QUERYWINDOW       81
#define DRAWFUN_PARENTWINDOW      82
#define DRAWFUN_CONFIGUREWINDOW   83
#define DRAWFUN_RENDER		      84
#define DRAWFUN_SELECTREGION      85
#define DRAWFUN_SELECTBUFFER      86
#define DRAWFUN_SELECTQUERY       87
#define DRAWFUN_EXTENTQUERY       88
#define DRAWFUN_POLYGONOFFSET	  89
#define DRAWFUN_PROJPUSH	      90
#define DRAWFUN_PROJPOP       	  91
#define DRAWFUN_BITMAPDEFINE      92
#define DRAWFUN_BITMAPSELECT      93
#define DRAWFUN_POLYPOINTARRAY    94
#define DRAWFUN_POLYLINEARRAY     95
#define DRAWFUN_POLYGONARRAY      96
#define DRAWFUN_ZBUFFERREAD       97
#define DRAWFUN_ZBUFFERWRITE      98
#define DRAWFUN_INITBUFFER        99
#define DRAWFUN_TERMBUFFER       100
#define DRAWFUN_COPYBUFFER       101
#define DRAWFUN_POLYPOINTBUFFER  102
#define DRAWFUN_POLYLINEBUFFER   103
#define DRAWFUN_POLYGONBUFFER    104
#define DRAWFUN_SETFACTORS       105
#define DRAWFUN_READBUFFER       106
#define DRAWFUN_BUFFERSIZE       107
#define DRAWFUN_BUFFERROW        108
#define DRAWFUN_BUFFERQUERY      109
#define DRAWFUN_PROJLOAD         110

#define DRAWFUN_ERROR       	 111

                   /* defines for function maximums */
#define DRAWFUN_MAX              112

                   /* defines for api types */
#define DRAWFUN_APIPRINT          0
#define DRAWFUN_APIRETURN         1

struct vgl_DrawFun {
   Vint ierr;
   Vobject *obj;
   void (*fun[1])();
   void (*colorindex)       (Vobject*, Vint);
   void (*color)            (Vobject*, Vfloat[3]);
   void (*polygon)          (Vobject*, Vint, Vint,Vfloat[][3],Vint,Vfloat[]);
   void (*textplane)        (Vobject*, Vfloat[3],Vfloat[3]);
   void (*xfmpush)          (Vobject*);
   void (*xfmpop)           (Vobject*);
   void (*linestyle)        (Vobject*, Vint);
   void (*linewidth)        (Vobject*, Vint);
   void (*trans)            (Vobject*, Vfloat);
   void (*polygoncolor)     (Vobject*, Vint, Vint, Vfloat[][3], Vfloat[][3],
                                       Vint, Vfloat[]);
   void (*clip)             (Vobject*, Vint, Vint, Vint, Vint);
   void (*text)             (Vobject*, Vfloat[3], Vtchar*);
   void (*delay)            (Vobject*, Vfloat);
   void (*specularity)      (Vobject*, Vfloat, Vfloat);
   void (*clipplane)        (Vobject*, Vint, Vfloat[4]);
   void (*polypoint)        (Vobject*, Vint, Vfloat[][3]);
   void (*polypointcolor)   (Vobject*, Vint, Vfloat[][3], Vfloat[][3]);
   void (*polyline)         (Vobject*, Vint, Vint, Vfloat[][3]);
   void (*polylinecolor)    (Vobject*, Vint, Vint, Vfloat[][3], Vfloat[][3]);
   void (*setmode)          (Vobject*, Vint, Vint);
   void (*pointsize)        (Vobject*, Vint);
   void (*xfmload)          (Vobject*, Vfloat[4][4]);
   void (*xfmmult)          (Vobject*, Vfloat[4][4]);
   void (*textdc)           (Vobject*, Vfloat[3], Vint[3], Vtchar*);
   void (*polylinedc)       (Vobject*, Vint, Vint, Vfloat[3], Vint[][3]);
   void (*polygondc)        (Vobject*, Vint, Vint, Vfloat[3], Vint[][3]);
   void (*polypointdc)      (Vobject*, Vint, Vfloat[3], Vint[][3]);
   void (*transindex)       (Vobject*, Vint);
   void (*polypointdata)    (Vobject*, Vint, Vfloat[][3], Vint, Vfloat*);
   void (*polylinedata)     (Vobject*, Vint, Vint, Vfloat[][3], Vint, Vfloat*);
   void (*polygondata)      (Vobject*, Vint, Vint, Vfloat[][3], Vint, Vfloat*,
                                       Vint, Vfloat[]);
   void (*polygonmode)      (Vobject*, Vint);
   void (*dataindex)        (Vobject*, Vint,Vint,Vint*);
   void (*data)             (Vobject*, Vint, Vfloat[]);
   void (*attpush)          (Vobject*, Vint);
   void (*attpop)           (Vobject*);
   void (*setswitch)        (Vobject*, Vint, Vint, Vint);
   void (*texturedefine)    (Vobject*, Vint, Vobject*);
   void (*textureselect)    (Vobject*, Vint);
   void (*clear)            (Vobject*);
   void (*swap)             (Vobject*);
   void (*flush)            (Vobject*);
   void (*bell)             (Vobject*);
   void (*depthrange)       (Vobject*, Vfloat, Vfloat);
   void (*rasfontdefine)    (Vobject*, Vint, Vobject*);
   void (*rasfontselect)    (Vobject*, Vint);
   void (*lightmodel)       (Vobject*, Vint, Vfloat, Vfloat);
   void (*getstring)        (Vobject*, Vint, Vchar*);
   void (*resize)           (Vobject*);
   void (*backcolorindex)   (Vobject*, Vint);
   void (*backcolor)        (Vobject*, Vfloat[3]);
   void (*getmode)          (Vobject*, Vint, Vint*);
   void (*getinteger)       (Vobject*, Vint, Vint*);
   void (*getfloat)         (Vobject*, Vint, Vfloat*);
   void (*light)            (Vobject*, Vint, Vint, Vfloat[3], Vfloat[3]);
   void (*lightswitch)      (Vobject*, Vint, Vint);
   void (*pollmouse)        (Vobject*, Vint*, Vint*, Vint*, Vint*, Vint*);
   void (*pollmodifiers)    (Vobject*, Vint*, Vint*);
   void (*warpmouse)        (Vobject*, Vint, Vint);
   void (*setcursor)        (Vobject*, Vint);
   void (*readqueue)        (Vobject*, Vint*, Vint*);
   void (*testqueue)        (Vobject*, Vint*);
   void (*resetqueue)       (Vobject*);
   void (*textpixelsize)    (Vobject*, Vfloat);
   void (*fbufferread)      (Vobject*, Vint, Vint, Vint, Vint, Vobject*);
   void (*fbufferwrite)     (Vobject*, Vint, Vint, Vint, Vint, Vobject*);
   void (*pixelmapcreate)   (Vobject*, Vobject*);
   void (*pixelmapdestroy)  (Vobject*, Vobject*);
   void (*pixelmapread)     (Vobject*, Vobject*);
   void (*pixelmapwrite)    (Vobject*, Vobject*);
   void (*projortho)      (Vobject*, Vfloat,Vfloat,Vfloat,Vfloat,Vfloat,Vfloat);
   void (*projfrustum)    (Vobject*, Vfloat,Vfloat,Vfloat,Vfloat,Vfloat,Vfloat);
   void (*viewport)        (Vobject*, Vint, Vint, Vint, Vint);
   void (*positionwindow)  (Vobject*, Vint, Vint, Vint, Vint);
   void (*visualwindow)    (Vobject*, Vint);
   void (*openwindow)      (Vobject*, const Vchar*);
   void (*closewindow)     (Vobject*);
   void (*connectwindow)   (Vobject*, Vword);
   void (*disconnectwindow) (Vobject*);
   void (*setwindow)       (Vobject*);
   void (*querywindow)     (Vobject*, Vword*);
   void (*parentwindow)    (Vobject*, Vword);
   void (*configurewindow) (Vobject*, Vint, Vint*);
   void (*render)          (Vobject*, Vint);
   void (*selectregion)    (Vobject*, Vint, Vint*);
   void (*selectbuffer)    (Vobject*, Vint, Vint*, Vfloat*, Vfloat*); 
   void (*selectquery)     (Vobject*, Vint*);
   void (*extentquery)     (Vobject*, Vfloat*,Vfloat*,Vfloat*,Vfloat*,
                                      Vfloat*,Vfloat*);
   void (*polygonoffset)   (Vobject*, Vfloat, Vfloat);
   void (*projpush)        (Vobject*);
   void (*projpop)         (Vobject*);
   void (*bitmapdefine)    (Vobject*, Vint, Vobject*);
   void (*bitmapselect)    (Vobject*, Vint);
   void (*polypointarray)  (Vobject*, Vint, Vfloat[][3], Vint, void*, Vint, void*, Vint, Vfloat*, Vint, Vfloat*);
   void (*polylinearray)   (Vobject*, Vint, Vint, Vfloat[][3], Vint, void*, Vint, void*, Vint, Vfloat*, Vint, Vfloat*);
   void (*polygonarray)    (Vobject*, Vint, Vint, Vfloat[][3], Vint, void*, Vint, void*, Vint, Vfloat*, Vint, Vfloat*);
   void (*zbufferread)     (Vobject*, Vint, Vint, Vint, Vint, Vobject*);
   void (*zbufferwrite)    (Vobject*, Vint, Vint, Vint, Vint, Vobject*);
   void (*initbuffer)      (Vobject*, Vint, Vint*);
   void (*termbuffer)      (Vobject*, Vint);
   void (*copybuffer)      (Vobject*, Vint, Vint, Vfloat[][3], Vint, void*, Vint, void*, Vint, Vfloat*, Vint, Vfloat*, Vint*);
   void (*polypointbuffer) (Vobject*, Vint, Vint, Vint, Vint, Vint, Vint, Vint);
   void (*polylinebuffer)  (Vobject*, Vint, Vint, Vint, Vint, Vint, Vint, Vint, Vint);
   void (*polygonbuffer)   (Vobject*, Vint, Vint, Vint, Vint, Vint, Vint, Vint, Vint);
   void (*setfactors)      (Vobject*, Vint, Vfloat[]);
   void (*readbuffer)      (Vobject*, Vint, Vint, Vint, Vfloat[][3], Vint, void*, Vint, void*, Vint, Vfloat*, Vint, Vfloat*);
   void (*buffersize)      (Vobject*, Vint, Vint);
   void (*bufferrow)       (Vobject*, Vint);
   void (*bufferquery)     (Vobject*, Vobject*);
   void (*projload)        (Vobject*, Vfloat[4][4]);
   Vint (*error)           (Vobject*);
#ifdef  __cplusplus
public:
   VKI_EXTERN vgl_DrawFun(void);
   VKI_EXTERN void* operator new (size_t);
   VKI_EXTERN ~vgl_DrawFun(void);
   VKI_EXTERN void operator delete (void*);
   VKI_EXTERN Vint Error();
   VKI_EXTERN void Init();
   VKI_EXTERN void Set(Vint,Vfunc*);
   VKI_EXTERN void Get(Vint,Vfunc**);
   VKI_EXTERN void SetObj(Vobject*);
   VKI_EXTERN void GetObj(Vobject**);
   VKI_EXTERN void Copy(vgl_DrawFun*);
   VKI_EXTERN void API(Vint);
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
   VKI_EXTERN void BackColorIndex(Vint);
   VKI_EXTERN void BackColor(Vfloat[3]);
   VKI_EXTERN void SetMode(Vint,Vint);
   VKI_EXTERN void GetMode(Vint,Vint*);
   VKI_EXTERN void GetInteger(Vint,Vint[]);
   VKI_EXTERN void GetFloat(Vint,Vfloat[]);
   VKI_EXTERN void GetString(Vint,Vchar[]);
   VKI_EXTERN void FBufferRead(Vint,Vint,Vint,Vint,vgl_FBuffer*);
   VKI_EXTERN void FBufferWrite(Vint,Vint,Vint,Vint,vgl_FBuffer*);
   VKI_EXTERN void PixelmapCreate(vgl_Pixelmap*);
   VKI_EXTERN void PixelmapDestroy(vgl_Pixelmap*);
   VKI_EXTERN void PixelmapRead(vgl_Pixelmap*);
   VKI_EXTERN void PixelmapWrite(vgl_Pixelmap*);
   VKI_EXTERN void BitmapDefine(Vint,vgl_Bitmap*);
   VKI_EXTERN void BitmapSelect(Vint);
   VKI_EXTERN void TextureDefine(Vint,vgl_Texture*);
   VKI_EXTERN void TextureSelect(Vint);
   VKI_EXTERN void RasFontDefine(Vint,vgl_RasFont*);
   VKI_EXTERN void RasFontSelect(Vint);
   VKI_EXTERN void ClipPlane(Vint,Vfloat[4]);
   VKI_EXTERN void Light(Vint,Vint,Vfloat[3],Vfloat[3]);
   VKI_EXTERN void LightSwitch(Vint,Vint);
   VKI_EXTERN void SetSwitch(Vint,Vint,Vint);
   VKI_EXTERN void LightModel(Vint,Vfloat,Vfloat);
   VKI_EXTERN void ProjOrtho(Vfloat,Vfloat,Vfloat,Vfloat,Vfloat,Vfloat);
   VKI_EXTERN void ProjFrustum(Vfloat,Vfloat,Vfloat,Vfloat,Vfloat,Vfloat);
   VKI_EXTERN void ProjPush();
   VKI_EXTERN void ProjPop();
   VKI_EXTERN void Viewport(Vint,Vint,Vint,Vint);
   VKI_EXTERN void DepthRange(Vfloat,Vfloat);
   VKI_EXTERN void PolygonOffset(Vfloat,Vfloat);
   VKI_EXTERN void PolygonMode(Vint);
   VKI_EXTERN void TextPlane(Vfloat[3],Vfloat[3]);
   VKI_EXTERN void PollMouse(Vint*,Vint*,Vint*,Vint*,Vint*);
   VKI_EXTERN void WarpMouse(Vint,Vint);
   VKI_EXTERN void SetCursor(Vint);
   VKI_EXTERN void PollModifiers(Vint*,Vint*);
   VKI_EXTERN void ReadQueue(Vint*,Vint*);
   VKI_EXTERN void TestQueue(Vint*);
   VKI_EXTERN void ResetQueue();
   VKI_EXTERN void TextPixelSize(Vfloat);
   VKI_EXTERN void PolyPoint(Vint,Vfloat[][3]);
   VKI_EXTERN void PolyPointColor(Vint,Vfloat[][3],Vfloat[][3]);
   VKI_EXTERN void PolyPointDC(Vint,Vfloat[3],Vint[][3]);
   VKI_EXTERN void PolyLine(Vint,Vint,Vfloat[][3]);
   VKI_EXTERN void PolyLineColor(Vint,Vint,Vfloat[][3],Vfloat[][3]);
   VKI_EXTERN void PolyLineDC(Vint,Vint,Vfloat[3],Vint[][3]);
   VKI_EXTERN void ColorIndex(Vint);
   VKI_EXTERN void Color(Vfloat[3]);
   VKI_EXTERN void Polygon(Vint,Vint,Vfloat[][3],Vint,Vfloat[]);
   VKI_EXTERN void PolygonColor(Vint,Vint,Vfloat[][3],Vfloat[][3],Vint,
                                Vfloat[]);
   VKI_EXTERN void PolygonDC(Vint,Vint,Vfloat[3],Vint[][3]);
   VKI_EXTERN void XfmPush();
   VKI_EXTERN void XfmPop();
   VKI_EXTERN void XfmMult(Vfloat[4][4]);
   VKI_EXTERN void XfmLoad(Vfloat[4][4]);
   VKI_EXTERN void AttPush(Vint);
   VKI_EXTERN void AttPop();
   VKI_EXTERN void PointSize(Vint);
   VKI_EXTERN void LineStyle(Vint);
   VKI_EXTERN void LineWidth(Vint);
   VKI_EXTERN void Trans(Vfloat);
   VKI_EXTERN void TransIndex(Vint);
   VKI_EXTERN void Clip(Vint,Vint,Vint,Vint);
   VKI_EXTERN void Text(Vfloat[3],Vtchar*);
   VKI_EXTERN void TextDC(Vfloat[3],Vint[3],Vtchar*);
   VKI_EXTERN void Specularity(Vfloat,Vfloat);
   VKI_EXTERN void DataIndex(Vint,Vint,Vint*);
   VKI_EXTERN void Data(Vint,Vfloat[]);
   VKI_EXTERN void PolyPointData(Vint,Vfloat[][3],Vint,Vfloat[]);
   VKI_EXTERN void PolyLineData(Vint,Vint,Vfloat[][3],Vint,Vfloat[]);
   VKI_EXTERN void PolygonData(Vint,Vint,Vfloat[][3],Vint,Vfloat[],Vint,
                               Vfloat[]);
   VKI_EXTERN void Render(Vint);
   VKI_EXTERN void SelectRegion(Vint,Vint[]);
   VKI_EXTERN void SelectBuffer(Vint,Vint[],Vfloat[],Vfloat[]);
   VKI_EXTERN void SelectQuery(Vint*);
   VKI_EXTERN void ExtentQuery(Vfloat*,Vfloat*,Vfloat*,Vfloat*,Vfloat*,Vfloat*);
   VKI_EXTERN void ZBufferRead(Vint,Vint,Vint,Vint,vgl_ZBuffer*);
   VKI_EXTERN void ZBufferWrite(Vint,Vint,Vint,Vint,vgl_ZBuffer*);
   VKI_EXTERN void PolyPointArray(Vint,Vfloat[][3],Vint,void*,Vint,void*,Vint,
                                  Vfloat*,Vint,Vfloat*);
   VKI_EXTERN void PolyLineArray(Vint,Vint,Vfloat[][3],Vint,void*,Vint,void*,
                                 Vint,Vfloat*,Vint,Vfloat*);
   VKI_EXTERN void PolygonArray(Vint,Vint,Vfloat[][3],Vint,void*,Vint,void*,
                                Vint,Vfloat*,Vint,Vfloat*);
   VKI_EXTERN void InitBuffer(Vint,Vint*);
   VKI_EXTERN void TermBuffer(Vint);
   VKI_EXTERN void PolyPointBuffer(Vint,Vint,Vint,Vint,Vint,Vint,Vint);
   VKI_EXTERN void PolyLineBuffer(Vint,Vint,Vint,Vint,Vint,Vint,Vint,Vint);
   VKI_EXTERN void PolygonBuffer(Vint,Vint,Vint,Vint,Vint,Vint,Vint,Vint);
   VKI_EXTERN void SetFactors(Vint,Vfloat[]);
   VKI_EXTERN void SetColorIndexFun(VDrawFunColorIndex*);
   VKI_EXTERN void SetColorFun(VDrawFunColor*);
   VKI_EXTERN void SetPolygonFun(VDrawFunPolygon*);
   VKI_EXTERN void SetTextPlaneFun(VDrawFunTextPlane*);
   VKI_EXTERN void SetXfmPushFun(VDrawFunXfmPush*);
   VKI_EXTERN void SetXfmPopFun(VDrawFunXfmPop*);
   VKI_EXTERN void SetLineStyleFun(VDrawFunLineStyle*);
   VKI_EXTERN void SetLineWidthFun(VDrawFunLineWidth*);
   VKI_EXTERN void SetTransFun(VDrawFunTrans*);
   VKI_EXTERN void SetPolygonColorFun(VDrawFunPolygonColor*);
   VKI_EXTERN void SetClipFun(VDrawFunClip*);
   VKI_EXTERN void SetTextFun(VDrawFunText*);
   VKI_EXTERN void SetDelayFun(VDrawFunDelay*);
   VKI_EXTERN void SetSpecularityFun(VDrawFunSpecularity*);
   VKI_EXTERN void SetClipPlaneFun(VDrawFunClipPlane*);
   VKI_EXTERN void SetPolyPointFun(VDrawFunPolyPoint*);
   VKI_EXTERN void SetPolyPointColorFun(VDrawFunPolyPointColor*);
   VKI_EXTERN void SetPolyLineFun(VDrawFunPolyLine*);
   VKI_EXTERN void SetPolyLineColorFun(VDrawFunPolyLineColor*);
   VKI_EXTERN void SetSetModeFun(VDrawFunSetMode*);
   VKI_EXTERN void SetPointSizeFun(VDrawFunPointSize*);
   VKI_EXTERN void SetXfmLoadFun(VDrawFunXfmLoad*);
   VKI_EXTERN void SetXfmMultFun(VDrawFunXfmMult*);
   VKI_EXTERN void SetTextDCFun(VDrawFunTextDC*);
   VKI_EXTERN void SetPolyLineDCFun(VDrawFunPolyLineDC*);
   VKI_EXTERN void SetPolygonDCFun(VDrawFunPolygonDC*);
   VKI_EXTERN void SetPolyPointDCFun(VDrawFunPolyPointDC*);
   VKI_EXTERN void SetTransIndexFun(VDrawFunTransIndex*);
   VKI_EXTERN void SetPolyPointDataFun(VDrawFunPolyPointData*);
   VKI_EXTERN void SetPolyLineDataFun(VDrawFunPolyLineData*);
   VKI_EXTERN void SetPolygonDataFun(VDrawFunPolygonData*);
   VKI_EXTERN void SetPolygonModeFun(VDrawFunPolygonMode*);
   VKI_EXTERN void SetDataIndexFun(VDrawFunDataIndex*);
   VKI_EXTERN void SetDataFun(VDrawFunData*);
   VKI_EXTERN void SetAttPushFun(VDrawFunAttPush*);
   VKI_EXTERN void SetAttPopFun(VDrawFunAttPop*);
   VKI_EXTERN void SetSetSwitchFun(VDrawFunSetSwitch*);
   VKI_EXTERN void SetTextureDefineFun(VDrawFunTextureDefine*);
   VKI_EXTERN void SetTextureSelectFun(VDrawFunTextureSelect*);
   VKI_EXTERN void SetClearFun(VDrawFunClear*);
   VKI_EXTERN void SetSwapFun(VDrawFunSwap*);
   VKI_EXTERN void SetFlushFun(VDrawFunFlush*);
   VKI_EXTERN void SetBellFun(VDrawFunBell*);
   VKI_EXTERN void SetDepthRangeFun(VDrawFunDepthRange*);
   VKI_EXTERN void SetRasFontDefineFun(VDrawFunRasFontDefine*);
   VKI_EXTERN void SetRasFontSelectFun(VDrawFunRasFontSelect*);
   VKI_EXTERN void SetLightModelFun(VDrawFunLightModel*);
   VKI_EXTERN void SetGetStringFun(VDrawFunGetString*);
   VKI_EXTERN void SetResizeFun(VDrawFunResize*);
   VKI_EXTERN void SetBackColorIndexFun(VDrawFunBackColorIndex*);
   VKI_EXTERN void SetBackColorFun(VDrawFunBackColor*);
   VKI_EXTERN void SetGetModeFun(VDrawFunGetMode*);
   VKI_EXTERN void SetGetIntegerFun(VDrawFunGetInteger*);
   VKI_EXTERN void SetGetFloatFun(VDrawFunGetFloat*);
   VKI_EXTERN void SetLightFun(VDrawFunLight*);
   VKI_EXTERN void SetLightSwitchFun(VDrawFunLightSwitch*);
   VKI_EXTERN void SetPollMouseFun(VDrawFunPollMouse*);
   VKI_EXTERN void SetPollModifiersFun(VDrawFunPollModifiers*);
   VKI_EXTERN void SetWarpMouseFun(VDrawFunWarpMouse*);
   VKI_EXTERN void SetSetCursorFun(VDrawFunSetCursor*);
   VKI_EXTERN void SetReadQueueFun(VDrawFunReadQueue*);
   VKI_EXTERN void SetTestQueueFun(VDrawFunTestQueue*);
   VKI_EXTERN void SetResetQueueFun(VDrawFunResetQueue*);
   VKI_EXTERN void SetTextPixelSizeFun(VDrawFunTextPixelSize*);
   VKI_EXTERN void SetFBufferReadFun(VDrawFunFBufferRead*);
   VKI_EXTERN void SetFBufferWriteFun(VDrawFunFBufferWrite*);
   VKI_EXTERN void SetPixelmapCreateFun(VDrawFunPixelmapCreate*);
   VKI_EXTERN void SetPixelmapDestroyFun(VDrawFunPixelmapDestroy*);
   VKI_EXTERN void SetPixelmapReadFun(VDrawFunPixelmapRead*);
   VKI_EXTERN void SetPixelmapWriteFun(VDrawFunPixelmapWrite*);
   VKI_EXTERN void SetProjOrthoFun(VDrawFunProjOrtho*);
   VKI_EXTERN void SetProjFrustrumFun(VDrawFunProjFrustum*);
   VKI_EXTERN void SetViewportFun(VDrawFunViewport*);
   VKI_EXTERN void SetPositionWindowFun(VDrawFunPositionWindow*);
   VKI_EXTERN void SetVisualWindowFun(VDrawFunVisualWindow*);
   VKI_EXTERN void SetOpenWindowFun(VDrawFunOpenWindow*);
   VKI_EXTERN void SetCloseWindowFun(VDrawFunCloseWindow*);
   VKI_EXTERN void SetConnectWindowFun(VDrawFunConnectWindow*);
   VKI_EXTERN void SetDisconnectWindowFun(VDrawFunDisconnectWindow*);
   VKI_EXTERN void SetSetWindowFun(VDrawFunSetWindow*);
   VKI_EXTERN void SetQueryWindowFun(VDrawFunQueryWindow*);
   VKI_EXTERN void SetParentWindowFun(VDrawFunParentWindow*);
   VKI_EXTERN void SetConfigureWindowFun(VDrawFunConfigureWindow*);
   VKI_EXTERN void SetRenderFun(VDrawFunRender*);
   VKI_EXTERN void SetSelectRegionFun(VDrawFunSelectRegion*);
   VKI_EXTERN void SetSelectBufferFun(VDrawFunSelectBuffer*);
   VKI_EXTERN void SetSelectQueryFun(VDrawFunSelectQuery*);
   VKI_EXTERN void SetExtentQueryFun(VDrawFunExtentQuery*);
   VKI_EXTERN void SetPolygonOffsetFun(VDrawFunPolygonOffset*);
   VKI_EXTERN void SetProjPushFun(VDrawFunProjPush*);
   VKI_EXTERN void SetProjPopFun(VDrawFunProjPop*);
   VKI_EXTERN void SetBitmapDefineFun(VDrawFunBitmapDefine*);
   VKI_EXTERN void SetBitmapSelectFun(VDrawFunBitmapSelect*);
   VKI_EXTERN void SetZBufferReadFun(VDrawFunZBufferRead*);
   VKI_EXTERN void SetZBufferWriteFun(VDrawFunZBufferWrite*);
   VKI_EXTERN void SetPolyPointArrayFun(VDrawFunPolyPointArray*);
   VKI_EXTERN void SetPolyLineArrayFun(VDrawFunPolyLineArray*);
   VKI_EXTERN void SetPolygonArrayFun(VDrawFunPolygonArray*);
   VKI_EXTERN void SetPolyPointBufferFun(VDrawFunPolyPointBuffer*);
   VKI_EXTERN void SetPolyLineBufferFun(VDrawFunPolyLineBuffer*);
   VKI_EXTERN void SetPolygonBufferFun(VDrawFunPolygonBuffer*);
   VKI_EXTERN void SetSetFactorsFun(VDrawFunSetFactors*);
   VKI_EXTERN void SetReadBufferFun(VDrawFunReadBuffer*);
   VKI_EXTERN void SetBufferSizeFun(VDrawFunBufferSize*);
   VKI_EXTERN void SetBufferRowFun(VDrawFunBufferRow*);
   VKI_EXTERN void SetBufferQueryFun(VDrawFunBufferQuery*);
   VKI_EXTERN void SetProjLoadFun(VDrawFunProjLoad*);
   VKI_EXTERN void SetErrorFun(VDrawFunError*);
#endif
};

#ifdef  __cplusplus
extern "C" {
#endif

VKI_EXTERN vgl_DrawFun*
vgl_DrawFunBegin(void);
VKI_EXTERN void
vgl_DrawFun_Construct(vgl_DrawFun *p);
VKI_EXTERN void
vgl_DrawFunEnd(vgl_DrawFun *p);
VKI_EXTERN void
vgl_DrawFun_Destruct(vgl_DrawFun *p);
VKI_EXTERN Vint
vgl_DrawFunError(vgl_DrawFun *p);
VKI_EXTERN void
vgl_DrawFunInit(vgl_DrawFun *p);
VKI_EXTERN void
vgl_DrawFunSet(vgl_DrawFun *p, Vint type, Vfunc *function);
VKI_EXTERN void
vgl_DrawFunGet(vgl_DrawFun *p, Vint type, Vfunc **function);
VKI_EXTERN void
vgl_DrawFunSetObj(vgl_DrawFun *p, Vobject *obj);
VKI_EXTERN void
vgl_DrawFunGetObj(vgl_DrawFun *p, Vobject **obj);
VKI_EXTERN void
vgl_DrawFunCopy(vgl_DrawFun *p, vgl_DrawFun *fromp);
VKI_EXTERN void
vgl_DrawFunAPI(vgl_DrawFun *p, Vint api);
VKI_EXTERN void
vgl_DrawFunPositionWindow(vgl_DrawFun *p,
                          Vint xorig, Vint yorig, Vint xsize, Vint ysize);
VKI_EXTERN void
vgl_DrawFunVisualWindow(vgl_DrawFun *p, Vint visflag);
VKI_EXTERN void
vgl_DrawFunParentWindow(vgl_DrawFun *p, Vword parent);
VKI_EXTERN void
vgl_DrawFunConfigureWindow(vgl_DrawFun *p, Vint oper, Vint *params);
VKI_EXTERN void
vgl_DrawFunOpenWindow(vgl_DrawFun *p, const Vchar *title);
VKI_EXTERN void
vgl_DrawFunCloseWindow(vgl_DrawFun *p);
VKI_EXTERN void
vgl_DrawFunConnectWindow(vgl_DrawFun *p, Vword window);
VKI_EXTERN void
vgl_DrawFunDisconnectWindow(vgl_DrawFun *p);
VKI_EXTERN void
vgl_DrawFunSetWindow(vgl_DrawFun *p);
VKI_EXTERN void
vgl_DrawFunQueryWindow(vgl_DrawFun *p, Vword *window);
VKI_EXTERN void
vgl_DrawFunClear(vgl_DrawFun *p);
VKI_EXTERN void
vgl_DrawFunSwap(vgl_DrawFun *p);
VKI_EXTERN void
vgl_DrawFunFlush(vgl_DrawFun *p);
VKI_EXTERN void
vgl_DrawFunBell(vgl_DrawFun *p);
VKI_EXTERN void
vgl_DrawFunDelay(vgl_DrawFun *p, Vfloat sec);
VKI_EXTERN void
vgl_DrawFunResize(vgl_DrawFun *p);
VKI_EXTERN void
vgl_DrawFunBackColorIndex(vgl_DrawFun *p, Vint index);
VKI_EXTERN void
vgl_DrawFunBackColor(vgl_DrawFun *p, Vfloat c[3]);
VKI_EXTERN void
vgl_DrawFunSetMode(vgl_DrawFun *p, Vint mode, Vint flag);
VKI_EXTERN void
vgl_DrawFunGetMode(vgl_DrawFun *p, Vint mode, Vint *flag);
VKI_EXTERN void
vgl_DrawFunGetInteger(vgl_DrawFun *p, Vint type, Vint params[]);
VKI_EXTERN void
vgl_DrawFunGetFloat(vgl_DrawFun *p, Vint type, Vfloat params[]);
VKI_EXTERN void
vgl_DrawFunGetString(vgl_DrawFun *p, Vint type, Vchar params[]);
VKI_EXTERN void
vgl_DrawFunFBufferRead(vgl_DrawFun *p,
                       Vint left, Vint right, Vint bottom, Vint top,
                       vgl_FBuffer *fbuffer);
VKI_EXTERN void
vgl_DrawFunFBufferWrite(vgl_DrawFun *p,
                        Vint left, Vint right, Vint bottom, Vint top,
                        vgl_FBuffer *fbuffer);
VKI_EXTERN void
vgl_DrawFunPixelmapCreate(vgl_DrawFun *p, vgl_Pixelmap *pixelmap);
VKI_EXTERN void
vgl_DrawFunPixelmapDestroy(vgl_DrawFun *p, vgl_Pixelmap *pixelmap);
VKI_EXTERN void
vgl_DrawFunPixelmapRead(vgl_DrawFun *p, vgl_Pixelmap *pixelmap);
VKI_EXTERN void
vgl_DrawFunPixelmapWrite(vgl_DrawFun *p, vgl_Pixelmap *pixelmap);
VKI_EXTERN void
vgl_DrawFunBitmapDefine(vgl_DrawFun *p, Vint index, vgl_Bitmap *bitmap);
VKI_EXTERN void
vgl_DrawFunBitmapSelect(vgl_DrawFun *p, Vint index);
VKI_EXTERN void
vgl_DrawFunTextureDefine(vgl_DrawFun *p, Vint index, vgl_Texture *texture);
VKI_EXTERN void
vgl_DrawFunTextureSelect(vgl_DrawFun *p, Vint index);
VKI_EXTERN void
vgl_DrawFunRasFontDefine(vgl_DrawFun *p,Vint index,vgl_RasFont *rasfont);
VKI_EXTERN void
vgl_DrawFunRasFontSelect(vgl_DrawFun *p, Vint index);
VKI_EXTERN void
vgl_DrawFunClipPlane(vgl_DrawFun *p, Vint index, Vfloat equation[4]);
VKI_EXTERN void
vgl_DrawFunLight(vgl_DrawFun *p,
                 Vint index, Vint type, Vfloat c[3], Vfloat x[3]);
VKI_EXTERN void
vgl_DrawFunLightSwitch(vgl_DrawFun *p, Vint index, Vint flag);
VKI_EXTERN void
vgl_DrawFunSetSwitch(vgl_DrawFun *p, Vint type, Vint index, Vint flag);
VKI_EXTERN void
vgl_DrawFunLightModel(vgl_DrawFun *p,
                     Vint localviewer, Vfloat koffset, Vfloat krate);
VKI_EXTERN void
vgl_DrawFunProjOrtho(vgl_DrawFun *p,
                     Vfloat left, Vfloat right, Vfloat bottom, Vfloat top,
                     Vfloat nearz, Vfloat farz);
VKI_EXTERN void
vgl_DrawFunProjFrustum(vgl_DrawFun *p,
                       Vfloat left, Vfloat right, Vfloat bottom, Vfloat top,
                       Vfloat nearz, Vfloat farz);
VKI_EXTERN void
vgl_DrawFunProjPush(vgl_DrawFun *p);
VKI_EXTERN void
vgl_DrawFunProjPop(vgl_DrawFun *p);
VKI_EXTERN void
vgl_DrawFunViewport(vgl_DrawFun *p,
                    Vint left, Vint right, Vint bottom, Vint top);
VKI_EXTERN void
vgl_DrawFunDepthRange(vgl_DrawFun *p, Vfloat nearz, Vfloat farz);
VKI_EXTERN void
vgl_DrawFunPolygonOffset(vgl_DrawFun *p, Vfloat factor, Vfloat bias);
VKI_EXTERN void
vgl_DrawFunPolygonMode(vgl_DrawFun *p, Vint rend);
VKI_EXTERN void
vgl_DrawFunTextPlane(vgl_DrawFun *p, Vfloat path[3], Vfloat plane[3]);
VKI_EXTERN void
vgl_DrawFunPollMouse(vgl_DrawFun *p,
                     Vint *px, Vint *py, Vint *but1, Vint *but2, Vint *but3);
VKI_EXTERN void
vgl_DrawFunWarpMouse(vgl_DrawFun *p, Vint px, Vint py);
VKI_EXTERN void
vgl_DrawFunSetCursor(vgl_DrawFun *p, Vint type);
VKI_EXTERN void
vgl_DrawFunPollModifiers(vgl_DrawFun *p, Vint *cntl, Vint *shft);
VKI_EXTERN void
vgl_DrawFunReadQueue(vgl_DrawFun *p, Vint *dev, Vint *val);
VKI_EXTERN void
vgl_DrawFunTestQueue(vgl_DrawFun *p, Vint *dev);
VKI_EXTERN void
vgl_DrawFunResetQueue(vgl_DrawFun *p);
VKI_EXTERN void
vgl_DrawFunTextPixelSize(vgl_DrawFun *p, Vfloat pixelsize);
VKI_EXTERN void
vgl_DrawFunPolyPoint(vgl_DrawFun *p, Vint npts, Vfloat x[][3]);
VKI_EXTERN void
vgl_DrawFunPolyPointColor(vgl_DrawFun *p,
                         Vint npts, Vfloat x[][3], Vfloat c[][3]);
VKI_EXTERN void
vgl_DrawFunPolyPointDC(vgl_DrawFun *p, Vint npts, Vfloat x[3], Vint dc[][3]);
VKI_EXTERN void
vgl_DrawFunPolyLine(vgl_DrawFun *p, Vint type, Vint npts, Vfloat x[][3]);
VKI_EXTERN void
vgl_DrawFunPolyLineColor(vgl_DrawFun *p, Vint type,
                         Vint npts, Vfloat x[][3], Vfloat c[][3]);
VKI_EXTERN void
vgl_DrawFunPolyLineDC(vgl_DrawFun *p, Vint type,
                      Vint npts, Vfloat x[3], Vint dc[][3]);
VKI_EXTERN void
vgl_DrawFunColorIndex(vgl_DrawFun *p, Vint index);
VKI_EXTERN void
vgl_DrawFunColor(vgl_DrawFun *p, Vfloat c[3]);
VKI_EXTERN void
vgl_DrawFunPolygon(vgl_DrawFun *p, Vint type,
                   Vint npts, Vfloat x[][3], Vint vtflag, Vfloat vt[]);
VKI_EXTERN void
vgl_DrawFunPolygonColor(vgl_DrawFun *p, Vint type,
                        Vint npts, Vfloat x[][3], Vfloat c[][3],
                        Vint vtflag, Vfloat vt[]);
VKI_EXTERN void
vgl_DrawFunPolygonDC(vgl_DrawFun *p, Vint type,
                     Vint npts, Vfloat x[3], Vint dc[][3]);
VKI_EXTERN void
vgl_DrawFunXfmPush(vgl_DrawFun *p);
VKI_EXTERN void
vgl_DrawFunXfmPop(vgl_DrawFun *p);
VKI_EXTERN void
vgl_DrawFunXfmMult(vgl_DrawFun *p, Vfloat xfm[4][4]);
VKI_EXTERN void
vgl_DrawFunXfmLoad(vgl_DrawFun *p, Vfloat xfm[4][4]);
VKI_EXTERN void
vgl_DrawFunAttPush(vgl_DrawFun *p, Vint mask);
VKI_EXTERN void
vgl_DrawFunAttPop(vgl_DrawFun *p);
VKI_EXTERN void
vgl_DrawFunPointSize(vgl_DrawFun *p, Vint psize);
VKI_EXTERN void
vgl_DrawFunLineStyle(vgl_DrawFun *p, Vint lstyle);
VKI_EXTERN void
vgl_DrawFunLineWidth(vgl_DrawFun *p, Vint lwidth);
VKI_EXTERN void
vgl_DrawFunTrans(vgl_DrawFun *p, Vfloat transp);
VKI_EXTERN void
vgl_DrawFunTransIndex(vgl_DrawFun *p, Vint index);
VKI_EXTERN void
vgl_DrawFunClip(vgl_DrawFun *p, Vint left, Vint right, Vint bottom, Vint top);
VKI_EXTERN void
vgl_DrawFunText(vgl_DrawFun *p, Vfloat x[3], Vtchar *string);
VKI_EXTERN void
vgl_DrawFunTextDC(vgl_DrawFun *p, Vfloat x[3], Vint dc[3], Vtchar *string);
VKI_EXTERN void
vgl_DrawFunSpecularity(vgl_DrawFun *p, Vfloat intensity, Vfloat shininess);
VKI_EXTERN void
vgl_DrawFunDataIndex(vgl_DrawFun *p, Vint nprims, Vint nrws, Vint *index);
VKI_EXTERN void
vgl_DrawFunData(vgl_DrawFun *p, Vint nrws, Vfloat d[]);
VKI_EXTERN void
vgl_DrawFunPolyPointData(vgl_DrawFun *p,
                         Vint npts, Vfloat x[][3], Vint nrws, Vfloat d[]);
VKI_EXTERN void
vgl_DrawFunPolyLineData(vgl_DrawFun *p,
                        Vint type, Vint npts,
                        Vfloat x[][3], Vint nrws, Vfloat d[]);
VKI_EXTERN void
vgl_DrawFunPolygonData(vgl_DrawFun *p,
                       Vint type, Vint npts, Vfloat x[][3],
                       Vint nrws,  Vfloat d[],
                       Vint vtflag, Vfloat vt[]);
VKI_EXTERN void
vgl_DrawFunRender(vgl_DrawFun *p, Vint type);
VKI_EXTERN void
vgl_DrawFunSelectRegion(vgl_DrawFun *p, Vint type, Vint params[]);
VKI_EXTERN void
vgl_DrawFunSelectBuffer(vgl_DrawFun *p, Vint size, Vint indexlist[],
                        Vfloat mindepth[], Vfloat maxdepth[]);
VKI_EXTERN void
vgl_DrawFunSelectQuery(vgl_DrawFun *p, Vint *hits);
VKI_EXTERN void
vgl_DrawFunExtentQuery(vgl_DrawFun *p, 
                       Vfloat *lft, Vfloat *rgt, Vfloat *btm, Vfloat *top, 
                       Vfloat *znear, Vfloat *zfar);
VKI_EXTERN void
vgl_DrawFunZBufferRead(vgl_DrawFun *p,
                       Vint left, Vint right, Vint bottom, Vint top,
                       vgl_ZBuffer *zbuffer);
VKI_EXTERN void
vgl_DrawFunZBufferWrite(vgl_DrawFun *p,
                        Vint left, Vint right, Vint bottom, Vint top,
                        vgl_ZBuffer *zbuffer);
VKI_EXTERN void
vgl_DrawFunPolyPointArray(vgl_DrawFun *p, Vint npts, Vfloat x[][3],
                          Vint cflag, void *c,
                          Vint vflag, void *v,
                          Vint tflag, Vfloat *t,
                          Vint dflag, Vfloat *d);
VKI_EXTERN void
vgl_DrawFunPolyLineArray(vgl_DrawFun *p, Vint type, Vint npts, Vfloat x[][3],
                         Vint cflag, void *c,
                         Vint vflag, void *v,
                         Vint tflag, Vfloat *t,
                         Vint dflag, Vfloat *d);
VKI_EXTERN void
vgl_DrawFunPolygonArray(vgl_DrawFun *p, Vint type, Vint npts, Vfloat x[][3],
                        Vint cflag, void *c,
                        Vint vflag, void *v,
                        Vint tflag, Vfloat *t,
                        Vint dflag, Vfloat *d);
VKI_EXTERN void
vgl_DrawFunInitBuffer(vgl_DrawFun *p, Vint size, Vint *vboid);
VKI_EXTERN void
vgl_DrawFunTermBuffer(vgl_DrawFun *p, Vint vboid);
VKI_EXTERN void
vgl_DrawFunPolyPointBuffer(vgl_DrawFun *p, Vint vboid, Vint offset,
                           Vint npts, Vint cflag, Vint vflag, Vint tflag,
                           Vint dflag);
VKI_EXTERN void
vgl_DrawFunPolyLineBuffer(vgl_DrawFun *p, Vint vboid, Vint offset,
                          Vint type, Vint npts,
                          Vint cflag, Vint vflag, Vint tflag, Vint dflag);
VKI_EXTERN void
vgl_DrawFunPolygonBuffer(vgl_DrawFun *p, Vint vboid, Vint offset,
                         Vint type, Vint npts,
                         Vint cflag, Vint vflag, Vint tflag, Vint dflag);
VKI_EXTERN void
vgl_DrawFunSetFactors(vgl_DrawFun *p, Vint type, Vfloat factors[]);
VKI_EXTERN void
vgl_DrawFunSetColorIndexFun(vgl_DrawFun *p, VDrawFunColorIndex *func);
VKI_EXTERN void
vgl_DrawFunSetColorFun(vgl_DrawFun *p, VDrawFunColor *func);
VKI_EXTERN void
vgl_DrawFunSetPolygonFun(vgl_DrawFun *p, VDrawFunPolygon *func);
VKI_EXTERN void
vgl_DrawFunSetTextPlaneFun(vgl_DrawFun *p, VDrawFunTextPlane *func);
VKI_EXTERN void
vgl_DrawFunSetXfmPushFun(vgl_DrawFun *p, VDrawFunXfmPush *func);
VKI_EXTERN void
vgl_DrawFunSetXfmPopFun(vgl_DrawFun *p, VDrawFunXfmPop *func);
VKI_EXTERN void
vgl_DrawFunSetLineStyleFun(vgl_DrawFun *p, VDrawFunLineStyle *func);
VKI_EXTERN void
vgl_DrawFunSetLineWidthFun(vgl_DrawFun *p, VDrawFunLineWidth *func);
VKI_EXTERN void
vgl_DrawFunSetTransFun(vgl_DrawFun *p, VDrawFunTrans *func);
VKI_EXTERN void
vgl_DrawFunSetPolygonColorFun(vgl_DrawFun *p, VDrawFunPolygonColor *func);
VKI_EXTERN void
vgl_DrawFunSetClipFun(vgl_DrawFun *p, VDrawFunClip *func);
VKI_EXTERN void
vgl_DrawFunSetTextFun(vgl_DrawFun *p, VDrawFunText *func);
VKI_EXTERN void
vgl_DrawFunSetDelayFun(vgl_DrawFun *p, VDrawFunDelay *func);
VKI_EXTERN void
vgl_DrawFunSetSpecularityFun(vgl_DrawFun *p, VDrawFunSpecularity *func);
VKI_EXTERN void
vgl_DrawFunSetClipPlaneFun(vgl_DrawFun *p, VDrawFunClipPlane *func);
VKI_EXTERN void
vgl_DrawFunSetPolyPointFun(vgl_DrawFun *p, VDrawFunPolyPoint *func);
VKI_EXTERN void
vgl_DrawFunSetPolyPointColorFun(vgl_DrawFun *p, VDrawFunPolyPointColor *func);
VKI_EXTERN void
vgl_DrawFunSetPolyLineFun(vgl_DrawFun *p, VDrawFunPolyLine *func);
VKI_EXTERN void
vgl_DrawFunSetPolyLineColorFun(vgl_DrawFun *p, VDrawFunPolyLineColor *func);
VKI_EXTERN void
vgl_DrawFunSetSetModeFun(vgl_DrawFun *p, VDrawFunSetMode *func);
VKI_EXTERN void
vgl_DrawFunSetPointSizeFun(vgl_DrawFun *p, VDrawFunPointSize *func);
VKI_EXTERN void
vgl_DrawFunSetXfmLoadFun(vgl_DrawFun *p, VDrawFunXfmLoad *func);
VKI_EXTERN void
vgl_DrawFunSetXfmMultFun(vgl_DrawFun *p, VDrawFunXfmMult *func);
VKI_EXTERN void
vgl_DrawFunSetTextDCFun(vgl_DrawFun *p, VDrawFunTextDC *func);
VKI_EXTERN void
vgl_DrawFunSetPolyLineDCFun(vgl_DrawFun *p, VDrawFunPolyLineDC *func);
VKI_EXTERN void
vgl_DrawFunSetPolygonDCFun(vgl_DrawFun *p, VDrawFunPolygonDC *func);
VKI_EXTERN void
vgl_DrawFunSetPolyPointDCFun(vgl_DrawFun *p, VDrawFunPolyPointDC *func);
VKI_EXTERN void
vgl_DrawFunSetTransIndexFun(vgl_DrawFun *p, VDrawFunTransIndex *func);
VKI_EXTERN void
vgl_DrawFunSetPolyPointDataFun(vgl_DrawFun *p, VDrawFunPolyPointData *func);
VKI_EXTERN void
vgl_DrawFunSetPolyLineDataFun(vgl_DrawFun *p, VDrawFunPolyLineData *func);
VKI_EXTERN void
vgl_DrawFunSetPolygonDataFun(vgl_DrawFun *p, VDrawFunPolygonData *func);
VKI_EXTERN void
vgl_DrawFunSetPolygonModeFun(vgl_DrawFun *p, VDrawFunPolygonMode *func);
VKI_EXTERN void
vgl_DrawFunSetDataIndexFun(vgl_DrawFun *p, VDrawFunDataIndex *func);
VKI_EXTERN void
vgl_DrawFunSetDataFun(vgl_DrawFun *p, VDrawFunData *func);
VKI_EXTERN void
vgl_DrawFunSetAttPushFun(vgl_DrawFun *p, VDrawFunAttPush *func);
VKI_EXTERN void
vgl_DrawFunSetAttPopFun(vgl_DrawFun *p, VDrawFunAttPop *func);
VKI_EXTERN void
vgl_DrawFunSetSetSwitchFun(vgl_DrawFun *p, VDrawFunSetSwitch *func);
VKI_EXTERN void
vgl_DrawFunSetTextureDefineFun(vgl_DrawFun *p, VDrawFunTextureDefine *func);
VKI_EXTERN void
vgl_DrawFunSetTextureSelectFun(vgl_DrawFun *p, VDrawFunTextureSelect *func);
VKI_EXTERN void
vgl_DrawFunSetClearFun(vgl_DrawFun *p, VDrawFunClear *func);
VKI_EXTERN void
vgl_DrawFunSetSwapFun(vgl_DrawFun *p, VDrawFunSwap *func);
VKI_EXTERN void
vgl_DrawFunSetFlushFun(vgl_DrawFun *p, VDrawFunFlush *func);
VKI_EXTERN void
vgl_DrawFunSetBellFun(vgl_DrawFun *p, VDrawFunBell *func);
VKI_EXTERN void
vgl_DrawFunSetDepthRangeFun(vgl_DrawFun *p, VDrawFunDepthRange *func);
VKI_EXTERN void
vgl_DrawFunSetRasFontDefineFun(vgl_DrawFun *p, VDrawFunRasFontDefine *func);
VKI_EXTERN void
vgl_DrawFunSetRasFontSelectFun(vgl_DrawFun *p, VDrawFunRasFontSelect *func);
VKI_EXTERN void
vgl_DrawFunSetLightModelFun(vgl_DrawFun *p, VDrawFunLightModel *func);
VKI_EXTERN void
vgl_DrawFunSetGetStringFun(vgl_DrawFun *p, VDrawFunGetString *func);
VKI_EXTERN void
vgl_DrawFunSetResizeFun(vgl_DrawFun *p, VDrawFunResize *func);
VKI_EXTERN void
vgl_DrawFunSetBackColorIndexFun(vgl_DrawFun *p, VDrawFunBackColorIndex *func);
VKI_EXTERN void
vgl_DrawFunSetBackColorFun(vgl_DrawFun *p, VDrawFunBackColor *func);
VKI_EXTERN void
vgl_DrawFunSetGetModeFun(vgl_DrawFun *p, VDrawFunGetMode *func);
VKI_EXTERN void
vgl_DrawFunSetGetIntegerFun(vgl_DrawFun *p, VDrawFunGetInteger *func);
VKI_EXTERN void
vgl_DrawFunSetGetFloatFun(vgl_DrawFun *p, VDrawFunGetFloat *func);
VKI_EXTERN void
vgl_DrawFunSetLightFun(vgl_DrawFun *p, VDrawFunLight *func);
VKI_EXTERN void
vgl_DrawFunSetLightSwitchFun(vgl_DrawFun *p, VDrawFunLightSwitch *func);
VKI_EXTERN void
vgl_DrawFunSetPollMouseFun(vgl_DrawFun *p, VDrawFunPollMouse *func);
VKI_EXTERN void
vgl_DrawFunSetPollModifiersFun(vgl_DrawFun *p, VDrawFunPollModifiers *func);
VKI_EXTERN void
vgl_DrawFunSetWarpMouseFun(vgl_DrawFun *p, VDrawFunWarpMouse *func);
VKI_EXTERN void
vgl_DrawFunSetSetCursorFun(vgl_DrawFun *p, VDrawFunSetCursor *func);
VKI_EXTERN void
vgl_DrawFunSetReadQueueFun(vgl_DrawFun *p, VDrawFunReadQueue *func);
VKI_EXTERN void
vgl_DrawFunSetTestQueueFun(vgl_DrawFun *p, VDrawFunTestQueue *func);
VKI_EXTERN void
vgl_DrawFunSetResetQueueFun(vgl_DrawFun *p, VDrawFunResetQueue *func);
VKI_EXTERN void
vgl_DrawFunSetTextPixelSizeFun(vgl_DrawFun *p, VDrawFunTextPixelSize *func);
VKI_EXTERN void
vgl_DrawFunSetFBufferReadFun(vgl_DrawFun *p, VDrawFunFBufferRead *func);
VKI_EXTERN void
vgl_DrawFunSetFBufferWriteFun(vgl_DrawFun *p, VDrawFunFBufferWrite *func);
VKI_EXTERN void
vgl_DrawFunSetPixelmapCreateFun(vgl_DrawFun *p, VDrawFunPixelmapCreate *func);
VKI_EXTERN void
vgl_DrawFunSetPixelmapDestroyFun(vgl_DrawFun *p, VDrawFunPixelmapDestroy *func);
VKI_EXTERN void
vgl_DrawFunSetPixelmapReadFun(vgl_DrawFun *p, VDrawFunPixelmapRead *func);
VKI_EXTERN void
vgl_DrawFunSetPixelmapWriteFun(vgl_DrawFun *p, VDrawFunPixelmapWrite *func);
VKI_EXTERN void
vgl_DrawFunSetProjOrthoFun(vgl_DrawFun *p, VDrawFunProjOrtho *func);
VKI_EXTERN void
vgl_DrawFunSetProjFrustrumFun(vgl_DrawFun *p, VDrawFunProjFrustum *func);
VKI_EXTERN void
vgl_DrawFunSetViewportFun(vgl_DrawFun *p, VDrawFunViewport *func);
VKI_EXTERN void
vgl_DrawFunSetPositionWindowFun(vgl_DrawFun *p, VDrawFunPositionWindow *func);
VKI_EXTERN void
vgl_DrawFunSetVisualWindowFun(vgl_DrawFun *p, VDrawFunVisualWindow *func);
VKI_EXTERN void
vgl_DrawFunSetOpenWindowFun(vgl_DrawFun *p, VDrawFunOpenWindow *func);
VKI_EXTERN void
vgl_DrawFunSetCloseWindowFun(vgl_DrawFun *p, VDrawFunCloseWindow *func);
VKI_EXTERN void
vgl_DrawFunSetConnectWindowFun(vgl_DrawFun *p, VDrawFunConnectWindow *func);
VKI_EXTERN void
vgl_DrawFunSetDisconnectWindowFun(vgl_DrawFun *p, VDrawFunDisconnectWindow *func);
VKI_EXTERN void
vgl_DrawFunSetSetWindowFun(vgl_DrawFun *p, VDrawFunSetWindow *func);
VKI_EXTERN void
vgl_DrawFunSetQueryWindowFun(vgl_DrawFun *p, VDrawFunQueryWindow *func);
VKI_EXTERN void
vgl_DrawFunSetParentWindowFun(vgl_DrawFun *p, VDrawFunParentWindow *func);
VKI_EXTERN void
vgl_DrawFunSetConfigureWindowFun(vgl_DrawFun *p,
                                 VDrawFunConfigureWindow *func);
VKI_EXTERN void
vgl_DrawFunSetRenderFun(vgl_DrawFun *p, VDrawFunRender *func);
VKI_EXTERN void
vgl_DrawFunSetSelectRegionFun(vgl_DrawFun *p, VDrawFunSelectRegion *func);
VKI_EXTERN void
vgl_DrawFunSetSelectBufferFun(vgl_DrawFun *p, VDrawFunSelectBuffer *func);
VKI_EXTERN void
vgl_DrawFunSetSelectQueryFun(vgl_DrawFun *p, VDrawFunSelectQuery *func);
VKI_EXTERN void
vgl_DrawFunSetExtentQueryFun(vgl_DrawFun *p, VDrawFunExtentQuery *func);
VKI_EXTERN void
vgl_DrawFunSetPolygonOffsetFun(vgl_DrawFun *p, VDrawFunPolygonOffset *func);
VKI_EXTERN void
vgl_DrawFunSetProjPushFun(vgl_DrawFun *p, VDrawFunProjPush *func);
VKI_EXTERN void
vgl_DrawFunSetProjPopFun(vgl_DrawFun *p, VDrawFunProjPop *func);
VKI_EXTERN void
vgl_DrawFunSetBitmapDefineFun(vgl_DrawFun *p, VDrawFunBitmapDefine *func);
VKI_EXTERN void
vgl_DrawFunSetBitmapSelectFun(vgl_DrawFun *p, VDrawFunBitmapSelect *func);
VKI_EXTERN void
vgl_DrawFunSetZBufferReadFun(vgl_DrawFun *p, VDrawFunZBufferRead *func);
VKI_EXTERN void
vgl_DrawFunSetZBufferWriteFun(vgl_DrawFun *p, VDrawFunZBufferWrite *func);
VKI_EXTERN void
vgl_DrawFunSetPolyPointArrayFun(vgl_DrawFun *p, VDrawFunPolyPointArray *func);
VKI_EXTERN void
vgl_DrawFunSetPolyLineArrayFun(vgl_DrawFun *p, VDrawFunPolyLineArray *func);
VKI_EXTERN void
vgl_DrawFunSetPolygonArrayFun(vgl_DrawFun *p, VDrawFunPolygonArray *func);
VKI_EXTERN void
vgl_DrawFunSetPolyPointBufferFun(vgl_DrawFun *p, VDrawFunPolyPointBuffer *func);
VKI_EXTERN void
vgl_DrawFunSetPolyLineBufferFun(vgl_DrawFun *p, VDrawFunPolyLineBuffer *func);
VKI_EXTERN void
vgl_DrawFunSetPolygonBufferFun(vgl_DrawFun *p, VDrawFunPolygonBuffer *func);
VKI_EXTERN void
vgl_DrawFunSetSetFactorsFun(vgl_DrawFun *p, VDrawFunSetFactors *func);
VKI_EXTERN void
vgl_DrawFunSetReadBufferFun(vgl_DrawFun *p, VDrawFunReadBuffer *func);
VKI_EXTERN void
vgl_DrawFunSetBufferSizeFun(vgl_DrawFun *p, VDrawFunBufferSize *func);
VKI_EXTERN void
vgl_DrawFunSetBufferRowFun(vgl_DrawFun *p, VDrawFunBufferRow *func);
VKI_EXTERN void
vgl_DrawFunSetBufferQueryFun(vgl_DrawFun *p, VDrawFunBufferQuery *func);
VKI_EXTERN void
vgl_DrawFunSetProjLoadFun(vgl_DrawFun *p, VDrawFunProjLoad *func);
VKI_EXTERN void
vgl_DrawFunSetErrorFun(vgl_DrawFun *p, VDrawFunError *func);

#ifdef  __cplusplus
}
#endif

                   /* defines for inline methods vgl_DrawFunAbcXyz style */
#define vgl_DrawFunObj(x)                (x)->obj
#if !defined(VKI_FUN_NODEFINE) || defined(VKI_FUN_DEFINE)
#define vgl_DrawFunColorIndex(x,a)       \
              (x)->colorindex((x)->obj,(a))
#define vgl_DrawFunColor(x,a)            \
              (x)->color((x)->obj,(a))
#define vgl_DrawFunPolygon(x,a,b,c,d,e)  \
              (x)->polygon((x)->obj,(a),(b),(c),(d),(e))
#define vgl_DrawFunTextPlane(x,a,b)  \
              (x)->textplane((x)->obj,(a),(b))
#define vgl_DrawFunXfmPush(x)            \
              (x)->xfmpush((x)->obj)
#define vgl_DrawFunXfmPop(x)             \
              (x)->xfmpop((x)->obj)
#define vgl_DrawFunLineStyle(x,a)        \
              (x)->linestyle((x)->obj,(a))
#define vgl_DrawFunLineWidth(x,a)        \
              (x)->linewidth((x)->obj,(a))
#define vgl_DrawFunTrans(x,a)            \
              (x)->trans((x)->obj,(a))
#define vgl_DrawFunPolygonColor(x,a,b,c,d,e,f)  \
              (x)->polygoncolor((x)->obj,(a),(b),(c),(d),(e),(f))
#define vgl_DrawFunClip(x,a,b,c,d)     \
              (x)->clip((x)->obj,(a),(b),(c),(d))
#define vgl_DrawFunText(x,a,b)           \
              (x)->text((x)->obj,(a),(b))
#define vgl_DrawFunDelay(x,a)    \
              (x)->delay((x)->obj,(a))
#define vgl_DrawFunSpecularity(x,a,b)    \
              (x)->specularity((x)->obj,(a),(b))
#define vgl_DrawFunClipPlane(x,a,b)    \
              (x)->clipplane((x)->obj,(a),(b))
#define vgl_DrawFunPolyPoint(x,a,b)      \
              (x)->polypoint((x)->obj,(a),(b))
#define vgl_DrawFunPolyPointColor(x,a,b,c) \
              (x)->polypointcolor((x)->obj,(a),(b),(c))
#define vgl_DrawFunPolyLine(x,a,b,c)     \
              (x)->polyline((x)->obj,(a),(b),(c))
#define vgl_DrawFunPolyLineColor(x,a,b,c,d)  \
              (x)->polylinecolor((x)->obj,(a),(b),(c),(d))
#define vgl_DrawFunSetMode(x,a,b)        \
              (x)->setmode((x)->obj,(a),(b))
#define vgl_DrawFunPointSize(x,a)        \
              (x)->pointsize((x)->obj,(a))
#define vgl_DrawFunXfmLoad(x,a)          \
              (x)->xfmload((x)->obj,(a))
#define vgl_DrawFunXfmMult(x,a)          \
              (x)->xfmmult((x)->obj,(a))
#define vgl_DrawFunTextDC(x,a,b,c)       \
              (x)->textdc((x)->obj,(a),(b),(c))
#define vgl_DrawFunPolyLineDC(x,a,b,c,d) \
              (x)->polylinedc((x)->obj,(a),(b),(c),(d))
#define vgl_DrawFunPolygonDC(x,a,b,c,d)  \
              (x)->polygondc((x)->obj,(a),(b),(c),(d))
#define vgl_DrawFunPolyPointDC(x,a,b,c)  \
              (x)->polypointdc((x)->obj,(a),(b),(c))
#define vgl_DrawFunTransIndex(x,a)       \
              (x)->transindex((x)->obj,(a))
#define vgl_DrawFunPolyPointData(x,a,b,c,d) \
              (x)->polypointdata((x)->obj,(a),(b),(c),(d))
#define vgl_DrawFunPolyLineData(x,a,b,c,d,e)  \
              (x)->polylinedata((x)->obj,(a),(b),(c),(d),(e))
#define vgl_DrawFunPolygonData(x,a,b,c,d,e,f,g) \
              (x)->polygondata((x)->obj,(a),(b),(c),(d),(e),(f),(g))
#define vgl_DrawFunPolygonMode(x,a)        \
              (x)->polygonmode((x)->obj,(a))
#define vgl_DrawFunDataIndex(x,a,b,c)    \
              (x)->dataindex((x)->obj,(a),(b),(c))
#define vgl_DrawFunData(x,a,b)           \
              (x)->data((x)->obj,(a),(b))
#define vgl_DrawFunAttPush(x,a)          \
              (x)->attpush((x)->obj,(a))
#define vgl_DrawFunAttPop(x)             \
              (x)->attpop((x)->obj)
#define vgl_DrawFunSetSwitch(x,a,b,c) \
              (x)->setswitch((x)->obj,(a),(b),(c))
#define vgl_DrawFunClear(x)              \
              (x)->clear((x)->obj)
#define vgl_DrawFunSwap(x)               \
              (x)->swap((x)->obj)
#define vgl_DrawFunFlush(x)              \
              (x)->flush((x)->obj)
#define vgl_DrawFunBell(x)               \
              (x)->bell((x)->obj)
#define vgl_DrawFunDepthRange(x,a,b)     \
              (x)->depthrange((x)->obj,(a),(b))
#define vgl_DrawFunTextureDefine(x,a,b)  \
              (x)->texturedefine((x)->obj,(a),(b))
#define vgl_DrawFunTextureSelect(x,a)    \
              (x)->textureselect((x)->obj,(a))
#define vgl_DrawFunRasFontDefine(x,a,b)  \
              (x)->rasfontdefine((x)->obj,(a),(b))
#define vgl_DrawFunRasFontSelect(x,a)    \
              (x)->rasfontselect((x)->obj,(a))
#define vgl_DrawFunResize(x)         \
              (x)->resize((x)->obj)
#define vgl_DrawFunBackColorIndex(x,a)   \
              (x)->backcolorindex((x)->obj,(a))
#define vgl_DrawFunBackColor(x,a)        \
              (x)->backcolor((x)->obj,(a))
#define vgl_DrawFunGetMode(x,a,b)        \
              (x)->getmode((x)->obj,(a),(b))
#define vgl_DrawFunGetInteger(x,a,b)     \
              (x)->getinteger((x)->obj,(a),(b))
#define vgl_DrawFunGetFloat(x,a,b)       \
              (x)->getfloat((x)->obj,(a),(b))
#define vgl_DrawFunGetString(x,a,b)       \
              (x)->getstring((x)->obj,(a),(b))
#define vgl_DrawFunLight(x,a,b,c,d)      \
              (x)->light((x)->obj,(a),(b),(c),(d))
#define vgl_DrawFunLightSwitch(x,a,b)    \
              (x)->lightswitch((x)->obj,(a),(b))
#define vgl_DrawFunLightModel(x,a,b,c)   \
              (x)->lightmodel((x)->obj,(a),(b),(c))
#define vgl_DrawFunPollMouse(x,a,b,c,d,e)  \
              (x)->pollmouse((x)->obj,(a),(b),(c),(d),(e))
#define vgl_DrawFunPollModifiers(x,a,b)  \
              (x)->pollmodifiers((x)->obj,(a),(b))
#define vgl_DrawFunWarpMouse(x,a,b)      \
              (x)->warpmouse((x)->obj,(a),(b))
#define vgl_DrawFunSetCursor(x,a)        \
              (x)->setcursor((x)->obj,(a))
#define vgl_DrawFunReadQueue(x,a,b)      \
              (x)->readqueue((x)->obj,(a),(b))
#define vgl_DrawFunTestQueue(x,a)        \
              (x)->testqueue((x)->obj,(a))
#define vgl_DrawFunResetQueue(x)         \
              (x)->resetqueue((x)->obj)
#define vgl_DrawFunTextPixelSize(x,a)        \
              (x)->textpixelsize((x)->obj,(a))
#define vgl_DrawFunFBufferRead(x,a,b,c,d,e)  \
              (x)->fbufferread((x)->obj,(a),(b),(c),(d),(e))
#define vgl_DrawFunFBufferWrite(x,a,b,c,d,e) \
              (x)->fbufferwrite((x)->obj,(a),(b),(c),(d),(e))
#define vgl_DrawFunPixelmapCreate(x,a)   \
              (x)->pixelmapcreate((x)->obj,(a))
#define vgl_DrawFunPixelmapDestroy(x,a)  \
              (x)->pixelmapdestroy((x)->obj,(a))
#define vgl_DrawFunPixelmapRead(x,a)     \
              (x)->pixelmapread((x)->obj,(a))
#define vgl_DrawFunPixelmapWrite(x,a)    \
              (x)->pixelmapwrite((x)->obj,(a))
#define vgl_DrawFunProjOrtho(x,a,b,c,d,e,f) \
              (x)->projortho((x)->obj,(a),(b),(c),(d),(e),(f))
#define vgl_DrawFunProjFrustum(x,a,b,c,d,e,f) \
              (x)->projfrustum((x)->obj,(a),(b),(c),(d),(e),(f))
#define vgl_DrawFunViewport(x,a,b,c,d) \
              (x)->viewport((x)->obj,(a),(b),(c),(d))
#define vgl_DrawFunPositionWindow(x,a,b,c,d) \
              (x)->positionwindow((x)->obj,(a),(b),(c),(d))
#define vgl_DrawFunVisualWindow(x,a)     \
              (x)->visualwindow((x)->obj,(a))
#define vgl_DrawFunOpenWindow(x,a)       \
              (x)->openwindow((x)->obj,(a))
#define vgl_DrawFunCloseWindow(x)        \
              (x)->closewindow((x)->obj)
#define vgl_DrawFunConnectWindow(x,a)    \
              (x)->connectwindow((x)->obj,(a))
#define vgl_DrawFunDisconnectWindow(x)   \
              (x)->disconnectwindow((x)->obj)
#define vgl_DrawFunSetWindow(x)          \
              (x)->setwindow((x)->obj)
#define vgl_DrawFunQueryWindow(x,a)      \
              (x)->querywindow((x)->obj,(a))
#define vgl_DrawFunParentWindow(x,a)     \
              (x)->parentwindow((x)->obj,(a))
#define vgl_DrawFunConfigureWindow(x,a,b)     \
              (x)->configurewindow((x)->obj,(a),(b))
#define vgl_DrawFunConfigureWindow(x,a,b)     \
              (x)->configurewindow((x)->obj,(a),(b))
#define vgl_DrawFunRender(x,a)     \
              (x)->render((x)->obj,(a))
#define vgl_DrawFunSelectRegion(x,a,b)     \
              (x)->selectregion((x)->obj,(a),(b))
#define vgl_DrawFunSelectBuffer(x,a,b,c,d)     \
              (x)->selectbuffer((x)->obj,(a),(b),(c),(d))
#define vgl_DrawFunSelectQuery(x,a)    \
              (x)->selectquery((x)->obj,(a))
#define vgl_DrawFunExtentQuery(x,a,b,c,d,e,f)     \
              (x)->extentquery((x)->obj,(a),(b),(c),(d),(e),(f))
#define vgl_DrawFunPolygonOffset(x,a,b)     \
              (x)->polygonoffset((x)->obj,(a),(b))
#define vgl_DrawFunProjPush(x)            \
              (x)->projpush((x)->obj)
#define vgl_DrawFunProjPop(x)             \
              (x)->projpop((x)->obj)
#define vgl_DrawFunBitmapDefine(x,a,b)  \
              (x)->bitmapdefine((x)->obj,(a),(b))
#define vgl_DrawFunBitmapSelect(x,a)    \
              (x)->bitmapselect((x)->obj,(a))
#define vgl_DrawFunZBufferRead(x,a,b,c,d,e)  \
              (x)->zbufferread((x)->obj,(a),(b),(c),(d),(e))
#define vgl_DrawFunZBufferWrite(x,a,b,c,d,e) \
              (x)->zbufferwrite((x)->obj,(a),(b),(c),(d),(e))
#define vgl_DrawFunPolyPointArray(x,a,b,c,d,e,f,g,h,i,j) \
       (x)->polypointarray((x)->obj,(a),(b),(c),(d),(e),(f),(g),(h),(i),(j))
#define vgl_DrawFunPolyLineArray(x,a,b,c,d,e,f,g,h,i,j,k) \
       (x)->polylinearray((x)->obj,(a),(b),(c),(d),(e),(f),(g),(h),(i),(j),(k))
#define vgl_DrawFunPolygonArray(x,a,b,c,d,e,f,g,h,i,j,k) \
       (x)->polygonarray((x)->obj,(a),(b),(c),(d),(e),(f),(g),(h),(i),(j),(k))
#define vgl_DrawFunInitBuffer(x,a,b) \
              (x)->initbuffer((x)->obj,(a),(b))
#define vgl_DrawFunTermBuffer(x,a) \
              (x)->termbuffer((x)->obj,(a))
#define vgl_DrawFunCopyBuffer(x,a,b,c,d,e,f,g,h,i,j,k,l) \
  (x)->copybuffer((x)->obj,(a),(b),(c),(d),(e),(f),(g),(h),(i),(j),(k),(l))
#define vgl_DrawFunPolyPointBuffer(x,a,b,c,d,e,f,g) \
              (x)->polypointbuffer((x)->obj,(a),(b),(c),(d),(e),(f),(g))
#define vgl_DrawFunPolyLineBuffer(x,a,b,c,d,e,f,g,h) \
              (x)->polylinebuffer((x)->obj,(a),(b),(c),(d),(e),(f),(g),(h))
#define vgl_DrawFunPolygonBuffer(x,a,b,c,d,e,f,g,h) \
              (x)->polygonbuffer((x)->obj,(a),(b),(c),(d),(e),(f),(g),(h))
#define vgl_DrawFunSetFactors(x,a,b) \
              (x)->setfactors((x)->obj,(a),(b))
#define vgl_DrawFunReadBuffer(x,a,b,c,d,e,f,g,h,i,j,k,l) \
  (x)->readbuffer((x)->obj,(a),(b),(c),(d),(e),(f),(g),(h),(i),(j),(k),(l))
#define vgl_DrawFunBufferSize(x,a,b) \
              (x)->buffersize((x)->obj,(a),(b))
#define vgl_DrawFunBufferRow(x,a) \
              (x)->bufferrow((x)->obj,(a))
#define vgl_DrawFunBufferQuery(x,a) \
              (x)->bufferquery((x)->obj,(a))
#define vgl_DrawFunProjLoad(x,a)          \
              (x)->projload((x)->obj,(a))
#endif

#endif
