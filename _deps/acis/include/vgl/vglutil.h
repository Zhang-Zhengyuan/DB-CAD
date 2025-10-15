                   /* vgl utility functions header file */
#ifndef VGLUTIL_DEF
#define VGLUTIL_DEF

#include "base/basedefs.h"
#include "vgl/drawfun.h"

typedef void (Vpolypoint)(Vobject*,Vint,Vfloat(*)[3]);
typedef void (Vpolypointcolor)(Vobject*,Vint,Vfloat(*)[3],Vfloat(*)[3]);
typedef void (Vpolyline)(Vobject*,Vint,Vint,Vfloat(*)[3]);
typedef void (Vpolylinecolor)(Vobject*,Vint,Vint,Vfloat(*)[3],Vfloat(*)[3]);
typedef void (Vpolygon)(Vobject*,Vint,Vint,Vfloat(*)[3],Vint,Vfloat*);
typedef void (Vpolygoncolor)(Vobject*,Vint,Vint,Vfloat(*)[3],Vfloat(*)[3],Vint,
                             Vfloat*);

#ifdef  __cplusplus
extern "C" {
#endif

extern void
vgl_zoom(Vuint *fz, Vint zx, Vint zy, Vuint *fi, Vint ix, Vint iy);
extern void
vgl_arrayColor(Vfloat cf[4], Vint i, Vint cflag, void *c, Vfloat cc[4]);
extern void
vgl_arrayNormal(Vint i, Vint vflag, void *v, Vfloat vv[3]);
extern void
vgl_arrayTexture(Vint i, Vint tflag, Vfloat *t, Vint *nt, Vfloat tt[]);
extern void
vgl_extentText(vgl_DrawFun *df, Vtchar *string, Vint dc[3], Vint dct[4][3]);
extern void
vgl_extentFit(Vint npts, Vfloat x[][3], Vfloat xsl, Vfloat ysl, Vfloat zc,
              Vfloat *xrt, Vfloat *yrt, Vfloat xpr[3], Vfloat ypr[3]);
extern void
vgl_projkimitstype(Vfloat tm[4][4], Vint *projtype,
                   Vfloat *left, Vfloat *right, Vfloat *bottom, Vfloat *top,
                   Vfloat *nearz, Vfloat *farz);
extern void
vgl_convertRGB2Index(Vfloat c[3], Vint ccsize, Vint *index);
extern void
vgl_convertIndex2RGB(Vint index, Vint ccsize, Vfloat c[3]);
extern void
vgl_convertRGB2Pixel(Vfloat c[3], Vint *pixel);
extern void
vgl_convertPixel2RGB(Vint pixel, Vfloat c[3]);
extern void
vgl_vec3Matrix4(Vfloat tm[4][4], Vfloat x[3], Vfloat p[3]);
extern void
vgl_copyMatrix4(Vfloat tm[4][4], Vfloat tmfrom[4][4]);
extern void
vgl_identMatrix4(Vfloat tm[4][4]);
extern void
vgl_rotateMatrix4(Vfloat tm[4][4]);
extern void
vgl_multMatrix4(Vfloat tma[4][4], Vfloat tmb[4][4], Vfloat tmc[4][4]);
extern void
vgl_multVectorMatrix4(Vfloat tm[4][4], Vfloat va[4], Vfloat vb[4]);
extern void
vgl_decompMatrix4(Vfloat t[4][4], Vint vecflag, Vfloat trans[3],
                  Vfloat scale[3], Vfloat rotation[4],
                  Vfloat scaleOrientation[4], Vfloat r[3][3],
                  Vfloat s[3][3]);
extern void
vgl_setEuler(Vfloat txr, Vfloat tyr, Vfloat tzr, Vfloat tm[3][3]);
extern void
vgl_getEuler(Vfloat tm[3][3], Vfloat *tx, Vfloat *ty, Vfloat *tz);
extern void
vgl_xfmAxisAngle(Vfloat r[3][3], Vfloat rotation[4]);
extern void
vgl_sizePoint(Vint pointsize, Vint *npts, Vint dc[][3]);
extern void
vgl_widthLine(Vint linewidth, Vint pa[3], Vint pb[3], Vint dc[][3]);
extern void
vgl_checkDepthRange(Vfloat znear, Vfloat zfar, Vfloat *fzmn, Vfloat *fzmx);
extern void
vgl_checkViewport(Vint left, Vint right, Vint bottom, Vint top,
                  Vint xsiz, Vint ysiz,
                  Vint *pxmn, Vint *pxmx, Vint *pymn, Vint *pymx);
extern void
vgl_sleepSeconds(Vfloat sec);
extern void
vgl_querySeconds(Vfloat *sec);

#ifdef  VKI_WIND_X11

#include <X11/Xlib.h>
#include <X11/Xutil.h>
#ifndef XK_MISCELLANY
#define XK_MISCELLANY 1
#endif
#include <X11/keysymdef.h>
#include <X11/cursorfont.h>

extern void
vgl_openXDisplay(Display **display, int *screen);
extern void
vgl_initXDevices(Display *display, Window window);
extern void
vgl_termXDevices(Display *display, Window window);
extern void
vgl_closeXDisplay(Display *display);
extern void
vgl_queryXDisplay(Display *display, int screen, Vint *xspx, Vint *yspx,
                  Vfloat *xscm, Vfloat *yscm);
extern void
vgl_queryXWindow(Display *display, int screen, Window window,
                 Vint *xopx, Vint *yopx, Vint *xspx, Vint *yspx,
                 Colormap *cmap, Vint *depth, Visual **visual, Vint *truecolor);
extern void
vgl_queryXWindowSize(Display *display, Window window, Vint *xsiz, Vint *ysiz);
extern void
vgl_queryXWindowOrig(Display *display, Window window, Vint *xorg, Vint *yorg);
extern void
vgl_queryXVisual(Display *display,int screen,Visual *visual,
                 Vint *depth, Vint *truecolor);
extern void
vgl_queryXRGBMasks(Visual *visual, unsigned long rgbmasks[3]);
extern void
vgl_allocXColormap(Display *display, Colormap cmap, unsigned long pixels[],
                   Vint colall[], Vfloat cpixels[][3], Vint ccsize);
extern void
vgl_pollXMouse(Display *display, Window window, Vint p2y,
               Vint *px, Vint *py, Vint *but1, Vint *but2, Vint *but3);
extern void
vgl_warpXMouse(Display *display, Window window, Vint p2y, Vint px, Vint py);
extern void
vgl_createXCursors(Display *display, Cursor cursor[]);
extern void
vgl_freeXCursors(Display *display, Cursor cursor[]);
extern void
vgl_windowXCursors(Display *display, Window window, Cursor cursor[]);
extern void
vgl_customXCursor(Display *display, Window window,
                  Cursor *cursor, unsigned char *sour, unsigned char *mask,
                  int xsiz, int ysiz, int xhot, int yhot);
extern void
vgl_pollXModifiers(Display *display, Window window, Vint *cntl, Vint *shft);
extern void
vgl_readXQueue(Display *display, Window window, Vint *dev, Vint *val);
extern void
vgl_testXQueue(Display *display, Window window, Vint *dev);
extern void
vgl_resetXQueue(Display *display, Window window);
extern void
vgl_configureXWindow(Display *display, Window window, Window parent,  
                     Vint oper, Vint params[]);

#else

extern void
vgl_openXDisplay(void **display, int *screen);
extern void
vgl_closeXDisplay(void *display);

#endif

#ifdef  VKI_WIND_WIN32

#include <windows.h>

extern void
vgl_queryWINDisplay(Vint *xspx, Vint *yspx, Vfloat *xscm, Vfloat *yscm);
extern void
vgl_queryWINWindow(HWND window, int *xopx, int *yopx, int *xspx, int *yspx,
                   int *depth, int *truecolor);
extern void
vgl_queryWINWindowSize(HWND window, int *xsiz, int *ysiz);
extern void
vgl_queryWINWindowOrig(HWND window, Vint *xorg, Vint *yorg);
extern void
vgl_testWINQueue(HWND window, Vint *dev, Vint *val);
extern void
vgl_readWINQueue(HWND window, Vint *dev, Vint *val);
extern void
vgl_pollWINMouse(HWND window,
                 Vint *px, Vint *py, Vint *but1, Vint *but2, Vint *but3);
extern void
vgl_pollWINModifiers(HWND window, Vint *cntl, Vint *shft);
extern void
vgl_warpWINMouse(HWND window, Vint px, Vint py);
extern void
vgl_createWINCursors(HCURSOR cursor[]);
extern void
vgl_freeWINCursors(HCURSOR cursor[]);
extern void
vgl_configureWINWindow(HWND window, HWND parent, Vint oper, Vint params[]);

#endif

#ifdef  __cplusplus
}
#endif

#endif
