                   /* VglTools wide include file */ 

#ifndef VGLDEFS_DEF
#define VGLDEFS_DEF

#include "base/basedefs.h"

typedef struct vgl_Bitmap      vgl_Bitmap;
typedef struct vgl_DataBuf     vgl_DataBuf;
typedef struct vgl_DFile       vgl_DFile;
typedef struct vgl_Direct3DDev vgl_Direct3DDev;
typedef struct vgl_DispFun     vgl_DispFun;
typedef struct vgl_DList       vgl_DList;
typedef struct vgl_DMap        vgl_DMap;
typedef struct vgl_DOpt        vgl_DOpt;
typedef struct vgl_DrawFun     vgl_DrawFun;
typedef struct vgl_DrawIPC     vgl_DrawIPC;
typedef struct vgl_DTee        vgl_DTee;
typedef struct vgl_DView       vgl_DView;
typedef struct vgl_FBuffer     vgl_FBuffer;
typedef struct vgl_GDIDev      vgl_GDIDev;
typedef struct vgl_IActor      vgl_IActor;
typedef struct vgl_OpenGLDev   vgl_OpenGLDev;
typedef struct vgl_Pixelmap    vgl_Pixelmap;
typedef struct vgl_Popup       vgl_Popup;
typedef struct vgl_Quadric     vgl_Quadric;
typedef struct vgl_RasFont     vgl_RasFont;
typedef struct vgl_RendBuf     vgl_RendBuf;
typedef struct vgl_SVGDev      vgl_SVGDev;
typedef struct vgl_Texture     vgl_Texture;
typedef struct vgl_X11Dev      vgl_X11Dev;
typedef struct vgl_Xfm         vgl_Xfm;
typedef struct vgl_Xfmstack    vgl_Xfmstack;
typedef struct vgl_ZBuffer     vgl_ZBuffer;

                   /* SetFunction */
#ifdef  __cplusplus
extern "C" {
#endif

typedef void           (VDFilefunc)(vgl_DFile*,Vobject*);
typedef void           (VDListfunc)(vgl_DList*,Vobject*);
typedef void           (VIActorfunc)(vgl_IActor*,Vobject*);
typedef void           (VPopupfunc)(vgl_Popup*,Vobject*);

#ifdef  __cplusplus
}
#endif

                   /* defines for error flags */
#define VGL_ERROR_NONE        0
#define VGL_ERROR_VALUE       1
#define VGL_ERROR_ENUM        2
#define VGL_ERROR_OBJECTTYPE  3
#define VGL_ERROR_MEMORY      4
#define VGL_ERROR_NULLOBJECT  5
#define VGL_ERROR_FILE        6
#define VGL_ERROR_COMPUTE     7
#define VGL_ERROR_OPERATION   8
#define VGL_ERROR_OVERFLOW    9
#define VGL_ERROR_UNDERFLOW  10
#define VGL_ERROR_FORMAT     12

                   /* defines for object types */
#define VGL_DRAWFUN          3000
#define VGL_DRAWFUN_TEE      3001
#define VGL_DISPFUN          3010
#define VGL_RASFONT          3020
#define VGL_BITMAP           3030
#define VGL_BITMAP_LINESTYLE 3031
#define VGL_BITMAP_PATTERN   3032
#define VGL_MATERIAL         3040
#define VGL_LMODEL           3060
#define VGL_XFMSTACK         3070
#define VGL_DLIST            3080
#define VGL_IACTOR           3090
#define VGL_DVIEW            3100
#define VGL_POPUP            3120
#define VGL_POPUP_1          3121
#define VGL_POPUP_2          3122
#define VGL_POPUP_3          3123
#define VGL_QUADRIC          3130

                   /* defines for device driver mode types */ 
#define VGL_ZBUFFERMODE       1
#define VGL_DBUFFERMODE       2
#define VGL_FRONTBUFFERDRAWMODE  3
#define VGL_XORMODE           4
#define VGL_LIGHTMODE         5
#define VGL_SSAOMODE          6
#define VGL_POLYGONDEPTHMODE  7
#define VGL_SHADERMODE        8
#define VGL_BACKFACECULLMODE  9
#define VGL_CLIPMODE         10
#define VGL_FRONTBUFFERREADMODE  11
#define VGL_EVENTQUEUEMODE   12
#define VGL_LINEDRAWMODE     13
#define VGL_BLENDTRANSMODE   14
#define VGL_TEXTANCHORMODE   15
#define VGL_TWOSIDELIGHTMODE 16
#define VGL_ZBUFFERWRITEMODE 17
#define VGL_LEFTBUFFERDRAWMODE   18
#define VGL_RASFONTDRAWMODE  19
#define VGL_SELECTSTACKMODE  20
#define VGL_ALPHATESTMODE    21
#define VGL_TEXTPIXELSIZEMODE    22
#define VGL_LINETRANSMODE    23
#define VGL_TEXTDIRECTIONMODE    24
#define VGL_ZBUFFERFUNCMODE  25
#define VGL_LINESTYLEFACTOR  26

#define VGL_STENCILMODE       27
#define VGL_STENCILWRITEMASK  28
#define VGL_STENCILFUNCMODE   29
#define VGL_STENCILFUNCREF    30
#define VGL_STENCILFUNCMASK   31
#define VGL_STENCILOPFAIL     32
#define VGL_STENCILOPZFAIL    33
#define VGL_STENCILOPZPASS    34

#define VGL_COLORWRITEMODE  35
#define VGL_SWAPMODE        36
#define VGL_POINTARRAYLIGHTMODE  37
#define VGL_LINEARRAYLIGHTMODE   38
#define VGL_UPDATEMODE       39
#define VGL_POINTICONMODE    40
#define VGL_ZBUFFEREDGEMODE  41
#define VGL_DEFORMMODE       42
#define VGL_POINTICONDIM     43
#define VGL_DEPTHPEELMODE    44
#define VGL_TRANSVALUEMODE   45
#define VGL_COLORVALUEMODE   46
#define VGL_PRIMITIVEMODE    47

#define VGL_UPDATE_FBUFFER    1
#define VGL_UPDATE_FZBUFFER   2

                   /* defines for primitive types */
#define VGL_LINESTRIP            0
#define VGL_LINELOOP             1
#define VGL_LINES                2

#define VGL_POLYGON              0
#define VGL_TRISTRIP             1
#define VGL_TRIFAN               2
#define VGL_TRIANGLES            3
#define VGL_QUADS                4
#define VGL_POLYHEDRON           5

                   /* defines for color types */
#define VGL_COLOR_4B              2
#define VGL_COLOR_3F              3
#define VGL_COLOR_4F              4

                   /* defines for normal types */
#define VGL_NORMAL_4B             1
#define VGL_NORMAL_3F             2

                   /* defines for device parameter gets */
                   /* need to be above last MODE */
#define VGL_WINDOWSIZE            101
#define VGL_PLANEDEPTH            102
#define VGL_MAXWINDOWSIZE         103
#define VGL_MAXWINDOWCENTIMETERS  104
#define VGL_DOUBLEBUFFER          105
#define VGL_DEPTHBUFFER           106
#define VGL_TRUECOLOR             107
#define VGL_WINDOWCENTIMETERS     108
#define VGL_WINDOWSYSTEM          109
#define VGL_MODELVIEWMATRIX       110
#define VGL_PROJECTIONMATRIX      111
#define VGL_VIEWPORT              112
#define VGL_DEPTHRANGE            113
#define VGL_MAXDEPTHRANGE         114
#define VGL_POINTSIZE             115
#define VGL_LINEWIDTH             116
#define VGL_LINESTYLE             117
#define VGL_TRANSINDEX            118
#define VGL_COLORINDEX            120
#define VGL_BACKCOLORINDEX        121
#define VGL_COLOR                 122
#define VGL_BACKCOLOR             123
#define VGL_TRANS                 124
#define VGL_SPECULARITY           125
#define VGL_RASFONTINDEX          126
#define VGL_RASFONT_METRICS       127
#define VGL_RASFONT_SPACING       128
#define VGL_RASFONT_SIZE          129
#define VGL_WINDOWORIGIN          130
#define VGL_WINDOWORIGINSIZE      131
#define VGL_WINDOWRAISE           132
#define VGL_WINDOWLOWER           133
#define VGL_RENDERMODE            134
#define VGL_BITMAPINDEX           135
#define VGL_TEXTUREINDEX          136
#define VGL_VENDOR                137
#define VGL_RENDERER              138
#define VGL_PARENTSIZE            139
#define VGL_PARENTFLAG            140
#define VGL_VERSION               141
#define VGL_PROJECTIONLIMITS      142
#define VGL_PROJECTIONTYPE        143
#define VGL_POLYGONOFFSET         144
#define VGL_SECONDS               145
#define VGL_POLYGONMODE           146
#define VGL_WINDOWSTATEINIT       147
#define VGL_WINDOWSTATETERM       148
#define VGL_WINDOWUSECONTEXT      149
#define VGL_RASFONTNUMBER         150
#define VGL_RASFONTINDICES        151
#define VGL_BITMAPNUMBER          152
#define VGL_BITMAPINDICES         153
#define VGL_TEXTURENUMBER         154
#define VGL_TEXTUREINDICES        155
#define VGL_NUMRASTERPRIM         156
#define VGL_NUMVECTORPRIM         157
#define VGL_DEVICESIZE            158
#define VGL_ZBUFFERDEPTH          159
#define VGL_SHADER_VERSION        160
#define VGL_SHADER_SUPPORTED      161

                   /* defines for window systems gets */
#define VGL_WINDOW_NONE           0
#define VGL_WINDOW_X              1
#define VGL_WINDOW_NT             2

                   /* defines for cursor types */
#define VGL_CURSOR_DEFAULT       0
#define VGL_CURSOR_WATCH         1
#define VGL_CURSOR_HAND          2
#define VGL_CURSOR_CROSSHAIR     3
#define VGL_CURSOR_TRANSLATION   4
#define VGL_CURSOR_ROTATION      5
#define VGL_CURSOR_ZOOM          6
#define VGL_CURSOR_ARROW         7
#define VGL_CURSOR_QUESTION      8
#define VGL_CURSOR_PENCIL        9
#define VGL_CURSOR_MAGNIFYGLASS 10
#define VGL_CURSOR_PIRATE       11
#define VGL_CURSOR_LLANGLE      12
#define VGL_CURSOR_LRANGLE      13
#define VGL_CURSOR_ULANGLE      14
#define VGL_CURSOR_URANGLE      15
#define VGL_CURSOR_DOLLY        16
#define VGL_CURSOR_NEAR         17
#define VGL_CURSOR_FAR          18
#define VGL_CURSORMAX           18

                   /* defines for render mode */
                   /* VGL_RENDER should be 0, all others non-zero */
#define VGL_RENDER              0
#define VGL_SELECT              1
#define VGL_EXTENT              2
#define VGL_EXTENT_DEVICE       2
#define VGL_EXTENT_EYE          4
#define VGL_EXTENT_WORLD        5
#define VGL_EXTENT_FIT          6
#define VGL_BUFFER_DATAINDEX    7
#define VGL_BUFFER_COLOR        8

                   /* defines for hit geometry */
#define VGL_REGION_POLYGON        1
#define VGL_REGION_RECTANGLE      2
#define VGL_REGION_CLIPPLANE      3

                   /* defines for max number of data rows */
#define VGL_MAXDATAROWS  16

                   /* defines for maximum lights */
#define VGL_MAXLIGHT            8

                   /* define for suggested number of points in primitive */
#define VGL_NUMPOINTS          64

#define VGL_MAXPOINTSIZE        8
#define VGL_MAXLINEWIDTH       64
                   /* maximum size of index stack */ 
#define VGL_MAXINDEXSTACK      64
                   /* maximum size of user data */ 
#define VGL_MAXUSERDATA         8
                   /* maximum size of user data */ 
#define VGL_MAXCLIPPLANE        6

                   /* defines for visuals */
#define VGL_VISUAL_USECONTEXT    1 << 0
#define VGL_VISUAL_SINGLEBUFFER  1 << 1
#define VGL_VISUAL_OVERLAY       1 << 2

#define VGL_VISUAL_PIXMAP        1 << 4

#define VGL_VISUAL_STEREO        1 << 6
#define VGL_VISUAL_SWAPCOPY      1 << 7
#define VGL_VISUAL_DEFAULT       1 << 8
#define VGL_VISUAL_STENCIL       1 << 9

                   /* defines for attribute stack */
#define VGL_POINTATT    1 << 0
#define VGL_LINEATT     1 << 1
#define VGL_POLYGONATT  1 << 2
#define VGL_COLORATT    1 << 3
#define VGL_VIEWPORTATT 1 << 4
#define VGL_ALLATT      31

                   /* defines for primitive type */ 
#define VGL_PRIM_POINT    1
#define VGL_PRIM_LINE     2
#define VGL_PRIM_POLYGON  3

                   /* defines for function comparisons, do not change */ 
#define VGL_NEVER    0
#define VGL_LESS     1
#define VGL_LEQUAL   2
#define VGL_GREATER  3
#define VGL_GEQUAL   4
#define VGL_EQUAL    5
#define VGL_NOTEQUAL 6
#define VGL_ALWAYS   7
                   /* defines for stencil operations, do not change */ 
#define VGL_OP_ZERO    0
#define VGL_OP_KEEP    1
#define VGL_OP_REPLACE 2
#define VGL_OP_INCR    3
#define VGL_OP_DECR    4
#define VGL_OP_INVERT  5

                   /* defines for linestyle */ 
#define VGL_LINESTYLE_SOLID     0
#define VGL_LINESTYLE_DOT       1
#define VGL_LINESTYLE_DASH      2
#define VGL_LINESTYLE_DOTDASH   3
#define VGL_LINESTYLE_LDASH     4
#define VGL_LINESTYLE_DOTLDASH  5
#define VGL_LINESTYLE_DOTDOT       6
#define VGL_LINESTYLE_DOTDOTLDASH  7
#define VGL_LINESTYLE_LLDASH       8
#define VGL_LINESTYLE_MAX          8

                   /* defines for pointstyle */ 
#define VGL_POINTMODE_DOT     0
#define VGL_POINTMODE_CUBE    1
#define VGL_POINTMODE_SPHERE  2
#define VGL_POINTMODE_MAX     3

                   /* defines for depthpeelmode */ 
#define VGL_DEPTHPEELLAST    -1

                   /* defines for projection type */ 
#define VGL_PROJORTHO    0
#define VGL_PROJFRUSTUM  1

                   /* polygon vertex shading flags */ 
#define VGL_NOSHADE       0
#define VGL_FLATSHADE     1
#define VGL_VERTEXSHADE   1 << 1
#define VGL_ANYSHADE      (VGL_FLATSHADE | VGL_VERTEXSHADE)

                   /* polygon vertex texture coordinate flags */ 
#define VGL_NOTEXTURE     0
#define VGL_1DTEXTURE     1 << 2
#define VGL_2DTEXTURE     1 << 3
#define VGL_ANYTEXTURE    (VGL_1DTEXTURE | VGL_2DTEXTURE)

                   /* number of normals in primitive */ 
#define POLYNUMBNORM(nt,np) (((nt) == VGL_VERTEXSHADE) * (np) + \
                            ((nt) == VGL_FLATSHADE))

#define NUMBNORM(nt,pt,np) (((nt) == VGL_VERTEXSHADE) * (np) + \
                 ((nt) == VGL_FLATSHADE) * (((pt) == VGL_POLYGON) + \
                                    (((np) - 2) * ((pt) == VGL_TRISTRIP)) + \
                                    (((pt) == VGL_POLYHEDRON)) + \
                                    ((np/3) * ((pt) == VGL_TRIANGLES)) + \
                                    ((np/4) * ((pt) == VGL_QUADS))))

#define NUMBTEXT(nt,np)    (((nt) == VGL_1DTEXTURE) * (np) + \
                            ((nt) == VGL_2DTEXTURE) * 2*(np))

                   /* project DC primitives */ 
#define VGL_projDC(obj,p,dc,pd)  if((obj)->projtype == 0) {\
        (pd)[0] = (p)[0] + (dc)[0] * (obj)->projsx;\
        (pd)[1] = (p)[1] + (dc)[1] * (obj)->projsy;\
                                 } else {\
        (pd)[0] = (p)[0] + (dc)[0] * (obj)->projsx*(p)[2]*(obj)->factsz;\
        (pd)[1] = (p)[1] + (dc)[1] * (obj)->projsy*(p)[2]*(obj)->factsz;\
                                 }\
                                 (pd)[2] = (p)[2]
                   /* entity types */ 
#define VGL_LIGHT      1
#define VGL_CLIPPLANE  2

                   /* light types */ 
#define VGL_LIGHT_NONE    -1
#define VGL_LIGHT_AMBIENT  0
#define VGL_LIGHT_DISTANT  1
#define VGL_LIGHT_LOCAL    2

                   /* event types */ 
#define VGL_EVENT_UNRECOGNIZED -1
#define VGL_EVENT_NONE          0
#define VGL_EVENT_LEFTMOUSE     1
#define VGL_EVENT_EXPOSE        2
#define VGL_EVENT_MIDDLEMOUSE   3
#define VGL_EVENT_RIGHTMOUSE    4
#define VGL_EVENT_KEYPRESS      5
#define VGL_EVENT_KEYRELEASE    6
#define VGL_EVENT_F1           11
#define VGL_EVENT_F2           12
#define VGL_EVENT_F3           13
#define VGL_EVENT_F4           14
#define VGL_EVENT_F5           15
#define VGL_EVENT_F6           16
#define VGL_EVENT_F7           17
#define VGL_EVENT_F8           18
#define VGL_EVENT_F9           19
#define VGL_EVENT_F10          20
#define VGL_EVENT_F11          21
#define VGL_EVENT_F12          22

#define VGL_EVENT_SHIFT        27
#define VGL_EVENT_CTRL         28
#define VGL_EVENT_ALT          29
#define VGL_EVENT_ESC          30

                   /* mouse buttons and modifier keys */ 
#define VGL_LEFTMOUSE      1 << 0
#define VGL_MIDDLEMOUSE    1 << 1
#define VGL_RIGHTMOUSE     1 << 2
#define VGL_SHIFT          1 << 3
#define VGL_CTRL           1 << 4
#define VGL_ALT            1 << 5
#define VGL_ESC            1 << 6

                   /* defines for on - off flag */
#define VGL_OFF 0
#define VGL_ON  1

                   /* defines for position, identical to VisTools */
#define VGL_RIGHT       0
#define VGL_LEFT        1
#define VGL_BOTTOM      2
#define VGL_TOP         3
#define VGL_CENTER      4
#define VGL_BOTTOMRIGHT 5
#define VGL_TOPRIGHT    6
#define VGL_BOTTOMLEFT  7
#define VGL_TOPLEFT     8
#define VGL_BACK        9
#define VGL_FRONT      10

                   /* defines for dragger types */
#define VGL_DRAG_INIT  1
#define VGL_DRAG_MOVE  2
#define VGL_DRAG_TERM  3

                   /* defines for buffer types */
#define VGL_COLOR_BUFFER  0
#define VGL_DEPTH_BUFFER  1

                   /* defines for array data types */
#define VGL_DATA_NONE     0
#define VGL_DATA_DEFORM   1

                   /* defines for factors */
#define VGL_ZBUFFEREDGESCALE  1
#define VGL_DEFORMSCALE       2
#define VGL_POINTICONSCALE    3
#define VGL_POINTICONSCALEXYZ 4
#define VGL_TRANSVALUE        5
#define VGL_COLORVALUE        6
#define VGL_SSAODIST          7

#endif
