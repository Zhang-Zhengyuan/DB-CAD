                   /* DView object */ 

#ifndef DVIEW_DEF
#define DVIEW_DEF

#include "base/basedefs.h"
#include "vgl/rendbuf.h"
#include "vgl/opengldev.h"
#include "vgl/direct3ddev.h"
#include "vgl/x11dev.h"
#include "vgl/gdidev.h"
#include "vgl/dlist.h"
#include "vgl/iactor.h"
#include "vgl/drawfun.h"

                   /* set display parameters */ 
#define DVIEW_TRANSFORMCENTER      1
#define DVIEW_TRANSFORMCENTERSIZE  2
#define DVIEW_FANCYBACKGROUND      3
#define DVIEW_DRIVER3D             4
#define DVIEW_TRACKBALL            5
#define DVIEW_OPPOSINGLIGHTS       6
#define DVIEW_STATIC               7
#define DVIEW_ANIMATE              8
#define DVIEW_ANIMATE_CYCLE        9
#define DVIEW_ANIMATE_FRAME       10

                   /* driver types */ 
#define DVIEW_DRIVER_2D           0
#define DVIEW_DRIVER_RENDBUF      1
#define DVIEW_DRIVER_OPENGL       2
#define DVIEW_DRIVER_DIRECT3D     4

                   /* set maximums */ 
#define DVIEW_MAXVIEWPORT  16

                   /* viewport types */ 
#define DVIEW_VIEWPORT_NONE       0
#define DVIEW_VIEWPORT_STATIC     1
#define DVIEW_VIEWPORT_ROTATE     2
#define DVIEW_VIEWPORT_TRANSFORM  3

                   /* viewport parameters types */ 
#define DVIEW_VIEWPORT_ANCHOR       1
#define DVIEW_VIEWPORT_EXTENT       2
#define DVIEW_VIEWPORT_PROJLIMITS   3
#define DVIEW_VIEWPORT_ANCHORALIGN  6
#define DVIEW_VIEWPORT_ANCHORUNITS  7
#define DVIEW_VIEWPORT_EXTENTUNITS  8

                   /* viewport parameter values */ 
#define DVIEW_VIEWPORT_DEVICE       0
#define DVIEW_VIEWPORT_VIRTUAL      1

struct vgl_DView {
   Vint ierr;
   Vint xorig, yorig;
   Vint xsize, ysize;
   Vint trkbalflag;
   Vint rotcenflag;
   Vint rotcensize;
   Vint fancyback;
   Vint threed;
   Vint opplits;
   Vint statflag;
   Vint animflag;
   Vint animcycle;
   Vint animframe;
   Vint animsense;

   vgl_DrawFun *df;

   vgl_RendBuf   *rendbuf;
   vgl_OpenGLDev *ogldev;
   vgl_Direct3DDev *d3d;
   vgl_X11Dev    *x11dev;
   vgl_GDIDev    *gdidev;

   Vint maxframe;
   vsy_List *ls;

#ifdef VKI_WIND_X11
   Display *display;
   int screen;
#else
   void *display;
   int screen;
#endif

   vgl_IActor *ia;
   vgl_DList *dl;
   vsy_List *list;

   Vint maxviewport;
   Vint viewtype[DVIEW_MAXVIEWPORT];
   Vfloat viewproj[DVIEW_MAXVIEWPORT][6];
   Vint anchorunits[DVIEW_MAXVIEWPORT];
   Vint extentunits[DVIEW_MAXVIEWPORT];
   Vint anchoralign[DVIEW_MAXVIEWPORT][2];
   Vint anchori[DVIEW_MAXVIEWPORT][2];
   Vint extenti[DVIEW_MAXVIEWPORT][2];
   Vfloat anchorf[DVIEW_MAXVIEWPORT][2];
   Vfloat extentf[DVIEW_MAXVIEWPORT][2];
   vgl_DList *viewdlist[DVIEW_MAXVIEWPORT];
   Vint winxsize, winysize;
};

#ifdef  __cplusplus
extern "C" {
#endif

VKI_EXTERN vgl_DView*
vgl_DViewBegin(void);
VKI_EXTERN void
vgl_DView_Construct(vgl_DView *dview);
VKI_EXTERN void
vgl_DViewEnd(vgl_DView *p);
VKI_EXTERN void
vgl_DView_Destruct(vgl_DView *dview);
VKI_EXTERN Vint
vgl_DViewError(vgl_DView *dview);
VKI_EXTERN void
vgl_DViewSetObject(vgl_DView *dview, Vint objecttype, Vobject *object);
VKI_EXTERN void
vgl_DViewSetViewport(vgl_DView *dview, Vint id, Vint type);
VKI_EXTERN void
vgl_DViewSetViewportParami(vgl_DView *dview, Vint id, Vint type, Vint iparam);
VKI_EXTERN void
vgl_DViewSetViewportParamiv(vgl_DView *dview, Vint id, Vint type, Vint iparam[]);
VKI_EXTERN void
vgl_DViewSetViewportParamfv(vgl_DView *dview, Vint id, Vint type,Vfloat param[]);
VKI_EXTERN void
vgl_DViewSetViewportObject(vgl_DView *dview,
                           Vint id, Vint objecttype, Vobject *object);
VKI_EXTERN void
vgl_DViewSetList(vgl_DView *dview, Vint id, vsy_List *list);
VKI_EXTERN void
vgl_DViewSetPosition(vgl_DView *dview,
                     Vint xorig, Vint yorig, Vint xsize, Vint ysize);
VKI_EXTERN void
vgl_DViewSetDisplayParami(vgl_DView *dview, Vint type, Vint iparam);
VKI_EXTERN void
vgl_DViewOpen(vgl_DView *dview, Vchar *title);
VKI_EXTERN void
vgl_DViewGetDrawFun(vgl_DView *dview, vgl_DrawFun **drawfun);
VKI_EXTERN void
vgl_DViewGetOpenGLDev(vgl_DView *dview, vgl_OpenGLDev **ogldev);
VKI_EXTERN void
vgl_DViewClose(vgl_DView *dview);
VKI_EXTERN void
vgl_DViewDisplayAction(vgl_DView *dview, vgl_IActor *iactor);

#ifdef  __cplusplus
}
#endif

#endif
