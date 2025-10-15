                   /* OpenGLDev object */ 

#ifndef DIRECT3DDEV_DEF
#define DIRECT3DDEV_DEF

#include "base/basedefs.h"
#include "vgl/vgldefs.h"
#include "vgl/drawfun.h"
#include "vgl/xfm.h"
#include "vgl/xfmstack.h"
#include "vgl/attstack.h"
#include "vgl/rasfont.h"
#include "vgl/texture.h"
#include "vgl/rmode.h"
#include "vgl/pixelmap.h"
#include "vgl/fbuffer.h"

#ifdef VKI_WIND_WIN32
#include <windows.h>
#include <tchar.h>
#ifdef VKI_3DAPI_DIRECT3D
#include <d3d9.h>
#include <d3dx9.h>
#include <d3dx9tex.h>
#endif
#endif

typedef struct {
   Vint   offset;
   Vint   size;
   Vint   deleted;
#ifdef VKI_3DAPI_DIRECT3D
   IDirect3DVertexBuffer9 *pVB;
#endif
} vgl_Direct3DDev_VBO;

struct vgl_Direct3DDev {
   Vint ierr;
   int    isxpix, isypix;             /* hardware pixels in x,y */
   float  sxcm, sycm;                 /* hardware centimeters in x,y */
   int    depth;                      /* hardware planes in frame buffer */
   int    depth_bits;                 /* hardware planes in z buffer */
   int xorig, yorig;                  /* requested position and size */
   int xsize, ysize;
   int visflag;                       /* requested size */
   float  xlencm, ylencm;             /* device centimeters in x,y */
   float  xgupcm, ygupcm;             /* resolution, pixels per centimeter */
   int    p1x, p1y, p2x, p2y;         /* device pixel limits in x,y */
   float  right, left, bottom, top;   /* current projection x,y limits */
   float  nearz, farz;                /* current projection z limits */
   int    pxmn, pxmx, pymn, pymx;     /* current viewport x,y limits */
   float  fzmn, fzmx;                 /* current depth range z limits */
   Vfloat  projsx, projsy, factsz;
   Vint projtype;
   Vint open;                         /* window opened flag */
   Vint connect;                      /* window connected flag */
   int rendermode;                    /* VGL_RENDER or defer to rmode */
   float zpfact, zpbias;              /* current polygon factor and bias */
   int pgrendmode;                    /* current polygon rendering mode */
                   /* current graphics modes */ 
   int idbfl;                         /* doublebuffer mode */
   int ifbfl;                         /* frontbuffer draw mode */
   int ifrfl;                         /* frontbuffer read mode */
   int izbfl;                         /* zbuffer mode */
   int truecolor;                     /* true color mode */
   int iltfl;                         /* lighting mode */
   int ixorm;                         /* xor mode */
   int ipdfl;                         /* polygon depth mode */
   int ibffl;                         /* back face cull mode */
   int icpfl;                         /* clip mode */
   int ieqfl;                         /* event queue mode */
   int ibtfl;                         /* blend transparency mode */
   int ilntfl;                        /* line transparency mode */
   int itsfl;                         /* two side light mode */
   int izwfl;                         /* zbuffer write mode */
   int ilbfl;                         /* leftbuffer draw mode */
   int irffl;                         /* rasfontdraw mode */
   int itafl;                         /* textanchor mode */
   int itdfl;                         /* textdirection mode */
   int issfl;                         /* select stack mode */
   int itpsfl;                        /* textpixelsize mode */
   int izbfn;                         /* zbuffer function comparison mode */

                   /* hardware capabilities */ 
   int dbufhard;                      /* hardware double buffering */
   int zbufhard;                      /* hardware z buffering */
   int overplanes;                    /* hardware number of overlay planes */
   int winsystem;                     /* hardware window system */
   int xormhard;                      /* hardware XOR operation */
   int plofhard;                      /* hardware Polygonoffset operation */
   int sterhard;                      /* hardware stereo */

   int polyflag;                      /* polygon primitive flag */
   int dcflag;                        /* dc primitive flag */
   int planflag;                      /* text plane primitive flag */
   int plantemp;
   float path[3];
   float plane[3];
   float pixelsize;
   int iliton;                        /* lighting model status */
   float znear;                       /* Z buffer near value */
   float znears;                      /* Z buffer near value for lines */
   float zfar;                        /* Z buffer far value */
   float zfars;                       /* Z buffer far value for lines */
   int izfull;                        /* Z buffer status */
   int itrpat;                      /* current pattern index */
   int itrans;                      /* non solid pattern active flag */
   float transp;                      /* current transparency */
   int pointsize;                     /* current point size */
   int linestyle;                     /* current line style */
   int linewidth;                     /* current line width */
   float intensity, shininess;        /* current specularity */
   int rasfontind;                    /* current raster font index */
   int textureind;                    /* current texture index */
   int textureindpos;                 /* last positive texture index */
   int bitmapind;                     /* current bitmap index */
   int forecind;                      /* Foreground color index */
   float forecrgb[3];                 /* Foreground color RGB */
   int backcind;                      /* Background color index */
   float backcrgb[3];                 /* Background color RGB */
   float ctm[4][4];                   /* current transformation matrix */
   int   curblend;                    /* current blending flag */
   int   stencil;                     /* stencil flag */

                   /* pattern information */ 
   int nplines;
   float (*plines)[2];
   int maxlsfact;
   int lsfact;

   vgl_DrawFun   *drawfun;
   vgl_Attstack  *attstack;
   vsy_HashTable *rasfonthash;
   vsy_HashTable *texturehash;
   vgl_RasFont   *rasfontcur;
   Vint          fonttextureindexcur;
   vsy_HashTable *bitmaphash;
   vgl_Xfmstack  *xfmstack;
   vgl_Xfm       *xfmtmp;
   vgl_RMode     *rmode;
   Vint          lightswitch[8];

   Vint          parflag;             /* parent specified */
   Vint          strflag;             /* stereo pixelformat specified */
   Vint          scpflag;             /* swap copy pixelformat specified */
   Vint          openscene;
   Vint          newtrans;            /* new transformation flag */

   Vint          init;
   int palmfl;                        /* point array light mode */
   int lalmfl;                        /* line array light mode */

   /* default vertex buffer parameters */
   Vint vbsize;                       /* size of default vertex buffer */
   Vint vbnsize;                      /* nodal size in floats */
   Vint vbnpts;                       /* npts in default vertex buffer */
   Vint vbincnpts;                    /* incremental number of points in vb */
   Vint vbntype;                      /* node type in default vertex buffer */
   Vint vbnprim;                      /* number of primitives in def vert buf */
#ifdef VKI_3DAPI_DIRECT3D
   D3DPRIMITIVETYPE vbprim;           /* primitive for default vertex buffer */
#endif
                   /* device specific elements */ 
#ifdef VKI_WIND_WIN32
   WNDCLASS   wc;
   HWND       parent;
   HWND       win;
   HDC        hdc;
   DWORD      dwStyle;
   HMENU      childid;
   _TCHAR     AppName[33];
   UINT       maxbufpoint;
   UINT       maxbufline;
   UINT       maxbuftri;
   UINT       maxbufother;
   UINT       byteoffsetpoint;
   UINT       byteoffsetline;
   UINT       byteoffsettri;
   UINT       byteoffsetother;
   UINT       addbytespoint;
   UINT       addbytesline;
   UINT       addbytestri;
   UINT       addbytesother;
#ifdef VKI_3DAPI_DIRECT3D
   LPDIRECT3D9            g_pD3D;       /* Used to create the D3DDevice */
   LPDIRECT3DDEVICE9      g_pd3dDevice; /* Our rendering device */
   IDirect3DVertexBuffer9 *g_pVB;       /* Default vertex buffer */
   D3DXMATRIX             *g_xfm;       /* Modelview Transformation matrix */
   D3DXMATRIX             *g_xfmp;      /* Projection Transformation matrix */
   D3DXMATRIX             *g_xfmv;      /* Viewport Transformation matrix */
   D3DXMATRIX             *g_xfmt;      /* Final transformation matrix */
   LPDIRECT3DTEXTURE9     g_pTexture;   /* Current texture */
   LPDIRECT3DVERTEXDECLARATION9 vertexDecl; /* Vertex declaration */
#endif

   /* buffer data */
   Vint curvboid;
   Vint nvbos;
   Vfloat *curbfr;
   Vint curntype;
   Vint curvboids;
   vgl_Direct3DDev_VBO *vbo;
#endif
#ifdef  __cplusplus
public:
   VKI_EXTERN vgl_Direct3DDev(void);
   VKI_EXTERN void* operator new (size_t);
   VKI_EXTERN ~vgl_Direct3DDev(void);
   VKI_EXTERN void operator delete (void*);
   VKI_EXTERN Vint Error();
   VKI_EXTERN void TestX(Vint*);
   VKI_EXTERN void TestWIN(Vint*);
   VKI_EXTERN void ConnectWIN(void);
   VKI_EXTERN void Disconnect(void);
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
   VKI_EXTERN void GetInteger(Vint,Vint*);
   VKI_EXTERN void GetFloat(Vint,Vfloat*);
   VKI_EXTERN void GetString(Vint,Vchar[]);
   VKI_EXTERN void FBufferRead(Vint,Vint,Vint,Vint,vgl_FBuffer*);
   VKI_EXTERN void FBufferWrite(Vint,Vint,Vint,Vint,vgl_FBuffer*);
   VKI_EXTERN void ZBufferRead(Vint,Vint,Vint,Vint,vgl_ZBuffer*);
   VKI_EXTERN void ZBufferWrite(Vint,Vint,Vint,Vint,vgl_ZBuffer*);
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
   VKI_EXTERN void SetSwitch(Vint,Vint,Vint);
   VKI_EXTERN void Light(Vint,Vint,Vfloat[3],Vfloat[3]);
   VKI_EXTERN void LightSwitch(Vint,Vint);
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
   VKI_EXTERN void TextPixelSize(Vfloat);
   VKI_EXTERN void PollMouse(Vint*,Vint*,Vint*,Vint*,Vint*);
   VKI_EXTERN void WarpMouse(Vint,Vint);
   VKI_EXTERN void SetCursor(Vint);
   VKI_EXTERN void PollModifiers(Vint*,Vint*);
   VKI_EXTERN void ReadQueue(Vint*,Vint*);
   VKI_EXTERN void TestQueue(Vint*);
   VKI_EXTERN void ResetQueue();
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
   VKI_EXTERN void PolyPointArray(Vint,Vfloat[][3],
                                  Vint,void*,Vint,void*,Vint,Vfloat*,
                                  Vint,Vfloat*);
   VKI_EXTERN void PolyLineArray(Vint,Vint,Vfloat[][3],
                                 Vint,void*,Vint,void*,Vint,Vfloat*,
                                 Vint,Vfloat*);
   VKI_EXTERN void PolygonArray(Vint,Vint,Vfloat[][3],
                                Vint,void*,Vint,void*,Vint,Vfloat*,
                                Vint,Vfloat*);
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
   VKI_EXTERN void PolygonData(Vint,Vint,Vfloat[][3],Vint,Vfloat*,Vint,
                               Vfloat[]);
   VKI_EXTERN void Render(Vint);
   VKI_EXTERN void SelectRegion(Vint,Vint*);
   VKI_EXTERN void SelectBuffer(Vint,Vint*,Vfloat*,Vfloat*);
   VKI_EXTERN void SelectQuery(Vint*);
   VKI_EXTERN void ExtentQuery(Vfloat*,Vfloat*,Vfloat*,Vfloat*,Vfloat*,Vfloat*);
   VKI_EXTERN void Pointer(Vint,Vint,void*);
   VKI_EXTERN void InitBuffer(Vint,Vint*);
   VKI_EXTERN void TermBuffer(Vint);
   VKI_EXTERN void CopyBuffer(Vint,Vint,Vfloat[][3],Vint,void*,
                              Vint,void*,Vint,Vfloat*,Vint,Vfloat*,Vint*);
   VKI_EXTERN void PolyPointBuffer(Vint,Vint,Vint,Vint,Vint,Vint,Vint);
   VKI_EXTERN void PolyLineBuffer(Vint,Vint,Vint,Vint,Vint,Vint,Vint,Vint);
   VKI_EXTERN void PolygonBuffer(Vint,Vint,Vint,Vint,Vint,Vint,Vint,Vint);
   VKI_EXTERN void SetFactor(Vint,Vfloat);
   VKI_EXTERN void DrawFun(vgl_DrawFun*);
#endif
};

#ifdef  __cplusplus
extern "C" {
#endif

VKI_EXTERN vgl_Direct3DDev*
vgl_Direct3DDevBegin(void);
VKI_EXTERN void
vgl_Direct3DDev_Construct(vgl_Direct3DDev *p);
VKI_EXTERN void
vgl_Direct3DDevEnd(vgl_Direct3DDev *p);
VKI_EXTERN void
vgl_Direct3DDev_Destruct(vgl_Direct3DDev *p);
VKI_EXTERN Vint
vgl_Direct3DDevError(vgl_Direct3DDev *p);
VKI_EXTERN void
vgl_Direct3DDevTestX(Vint *flag);
VKI_EXTERN void
vgl_Direct3DDevTestWIN(Vint *flag);
VKI_EXTERN void
vgl_Direct3DDevConnectWIN(void);
VKI_EXTERN void
vgl_Direct3DDevDisconnect(void);
VKI_EXTERN void
vgl_Direct3DDevPositionWindow(vgl_Direct3DDev *p,
                            Vint xorig, Vint yorig, Vint xsize, Vint ysize);
VKI_EXTERN void
vgl_Direct3DDevVisualWindow(vgl_Direct3DDev *p, Vint visflag);
VKI_EXTERN void
vgl_Direct3DDevParentWindow(vgl_Direct3DDev *p, Vword parent);
VKI_EXTERN void
vgl_Direct3DDevConfigureWindow(vgl_Direct3DDev *p, Vint oper, Vint *params);
VKI_EXTERN void
vgl_Direct3DDevOpenWindow(vgl_Direct3DDev *p, const Vchar *title);
VKI_EXTERN void
vgl_Direct3DDevCloseWindow(vgl_Direct3DDev *p);
VKI_EXTERN void
vgl_Direct3DDevConnectWindow(vgl_Direct3DDev *p, Vword window);
VKI_EXTERN void
vgl_Direct3DDevDisconnectWindow(vgl_Direct3DDev *p);
VKI_EXTERN void
vgl_Direct3DDevSetWindow(vgl_Direct3DDev *p);
VKI_EXTERN void
vgl_Direct3DDevQueryWindow(vgl_Direct3DDev *p, Vword *window);
VKI_EXTERN void
vgl_Direct3DDevClear(vgl_Direct3DDev *p);
VKI_EXTERN void
vgl_Direct3DDevSwap(vgl_Direct3DDev *p);
VKI_EXTERN void
vgl_Direct3DDevFlush(vgl_Direct3DDev *p);
VKI_EXTERN void
vgl_Direct3DDevBell(vgl_Direct3DDev *p);
VKI_EXTERN void
vgl_Direct3DDevDelay(vgl_Direct3DDev *p, Vfloat sec);
VKI_EXTERN void
vgl_Direct3DDevResize(vgl_Direct3DDev *p);
VKI_EXTERN void
vgl_Direct3DDevBackColorIndex(vgl_Direct3DDev *p, Vint index);
VKI_EXTERN void
vgl_Direct3DDevBackColor(vgl_Direct3DDev *p, Vfloat c[3]);
VKI_EXTERN void
vgl_Direct3DDevSetMode(vgl_Direct3DDev *p, Vint mode, Vint flag);
VKI_EXTERN void
vgl_Direct3DDevGetMode(vgl_Direct3DDev *p, Vint mode, Vint *flag);
VKI_EXTERN void
vgl_Direct3DDevGetInteger(vgl_Direct3DDev *p, Vint type, Vint *params);
VKI_EXTERN void
vgl_Direct3DDevGetFloat(vgl_Direct3DDev *p, Vint type, Vfloat *params);
VKI_EXTERN void
vgl_Direct3DDevGetString(vgl_Direct3DDev *p, Vint type, Vchar params[]);
VKI_EXTERN void
vgl_Direct3DDevFBufferRead(vgl_Direct3DDev *p,
                           Vint left, Vint right, Vint bottom, Vint top,
                           vgl_FBuffer *fbuffer);
VKI_EXTERN void
vgl_Direct3DDevFBufferWrite(vgl_Direct3DDev *p,
                            Vint left, Vint right, Vint bottom, Vint top,
                            vgl_FBuffer *fbuffer);
VKI_EXTERN void
vgl_Direct3DDevZBufferRead(vgl_Direct3DDev *p,
                         Vint left, Vint right, Vint bottom, Vint top,
                         vgl_ZBuffer *zbuffer);
VKI_EXTERN void
vgl_Direct3DDevZBufferWrite(vgl_Direct3DDev *p,
                          Vint left, Vint right, Vint bottom, Vint top,
                          vgl_ZBuffer *zbuffer);
VKI_EXTERN void
vgl_Direct3DDevPixelmapCreate(vgl_Direct3DDev *p, vgl_Pixelmap *pixelmap);
VKI_EXTERN void
vgl_Direct3DDevPixelmapDestroy(vgl_Direct3DDev *p, vgl_Pixelmap *pixelmap);
VKI_EXTERN void
vgl_Direct3DDevPixelmapRead(vgl_Direct3DDev *p, vgl_Pixelmap *pixelmap);
VKI_EXTERN void
vgl_Direct3DDevPixelmapWrite(vgl_Direct3DDev *p, vgl_Pixelmap *pixelmap);
VKI_EXTERN void
vgl_Direct3DDevBitmapDefine(vgl_Direct3DDev *p, Vint index, vgl_Bitmap *bitmap);
VKI_EXTERN void
vgl_Direct3DDevBitmapSelect(vgl_Direct3DDev *p, Vint index);
VKI_EXTERN void
vgl_Direct3DDevTextureDefine(vgl_Direct3DDev *p, Vint index,
                             vgl_Texture *texture);
VKI_EXTERN void
vgl_Direct3DDevTextureSelect(vgl_Direct3DDev *p, Vint index);
VKI_EXTERN void
vgl_Direct3DDevRasFontDefine(vgl_Direct3DDev *p, Vint index,
                             vgl_RasFont *rasfont);
VKI_EXTERN void
vgl_Direct3DDevRasFontSelect(vgl_Direct3DDev *p, Vint index);
VKI_EXTERN void
vgl_Direct3DDevClipPlane(vgl_Direct3DDev *p, Vint index, Vfloat eqn[4]);
VKI_EXTERN void
vgl_Direct3DDevSetSwitch(vgl_Direct3DDev *p, Vint type, Vint index, Vint flag);
VKI_EXTERN void
vgl_Direct3DDevLight(vgl_Direct3DDev *p,
                     Vint index, Vint type, Vfloat c[3], Vfloat x[3]);
VKI_EXTERN void
vgl_Direct3DDevLightSwitch(vgl_Direct3DDev *p, Vint index, Vint flag);
VKI_EXTERN void
vgl_Direct3DDevLightModel(vgl_Direct3DDev *p,
                        Vint localviewer, Vfloat koffset, Vfloat krate);
VKI_EXTERN void
vgl_Direct3DDevProjOrtho(vgl_Direct3DDev *p,
                         Vfloat left, Vfloat right, Vfloat bottom, Vfloat top,
                         Vfloat nearz, Vfloat farz);
VKI_EXTERN void
vgl_Direct3DDevProjFrustum(vgl_Direct3DDev *p,
                         Vfloat left, Vfloat right, Vfloat bottom, Vfloat top,
                         Vfloat nearz, Vfloat farz);
VKI_EXTERN void
vgl_Direct3DDevProjPush(vgl_Direct3DDev *p);
VKI_EXTERN void
vgl_Direct3DDevProjPop(vgl_Direct3DDev *p);
VKI_EXTERN void
vgl_Direct3DDevViewport(vgl_Direct3DDev *p,
                      Vint left, Vint right, Vint bottom, Vint top);
VKI_EXTERN void
vgl_Direct3DDevDepthRange(vgl_Direct3DDev *p, Vfloat nearz, Vfloat farz);
VKI_EXTERN void
vgl_Direct3DDevPolygonOffset(vgl_Direct3DDev *p, Vfloat factor, Vfloat bias);
VKI_EXTERN void
vgl_Direct3DDevPolygonMode(vgl_Direct3DDev *p, Vint rend);
VKI_EXTERN void
vgl_Direct3DDevTextPlane(vgl_Direct3DDev *p, Vfloat path[3], Vfloat plane[3]);
VKI_EXTERN void
vgl_Direct3DDevTextPixelSize(vgl_Direct3DDev *p, Vfloat pixelsize);
VKI_EXTERN void
vgl_Direct3DDevPollMouse(vgl_Direct3DDev *p,
                       Vint *px, Vint *py, Vint *but1, Vint *but2, Vint *but3);
VKI_EXTERN void
vgl_Direct3DDevWarpMouse(vgl_Direct3DDev *p, Vint px, Vint py);
VKI_EXTERN void
vgl_Direct3DDevSetCursor(vgl_Direct3DDev *p, Vint type);
VKI_EXTERN void
vgl_Direct3DDevPollModifiers(vgl_Direct3DDev *p, Vint *cntl, Vint *shft);
VKI_EXTERN void
vgl_Direct3DDevReadQueue(vgl_Direct3DDev *p, Vint *dev, Vint *val);
VKI_EXTERN void
vgl_Direct3DDevTestQueue(vgl_Direct3DDev *p, Vint *dev);
VKI_EXTERN void
vgl_Direct3DDevResetQueue(vgl_Direct3DDev *p);
VKI_EXTERN void
vgl_Direct3DDevPolyPoint(vgl_Direct3DDev *p, Vint npts, Vfloat x[][3]);
VKI_EXTERN void
vgl_Direct3DDevPolyPointColor(vgl_Direct3DDev *p,
                            Vint npts, Vfloat x[][3], Vfloat c[][3]);
VKI_EXTERN void
vgl_Direct3DDevPolyPointDC(vgl_Direct3DDev *p, Vint npts, Vfloat x[3],
                           Vint dc[][3]);
VKI_EXTERN void
vgl_Direct3DDevPolyLine(vgl_Direct3DDev *p, Vint type, Vint npts, Vfloat x[][3]);
VKI_EXTERN void
vgl_Direct3DDevPolyLineColor(vgl_Direct3DDev *p, Vint type,
                           Vint npts, Vfloat x[][3], Vfloat c[][3]);
VKI_EXTERN void
vgl_Direct3DDevPolyLineDC(vgl_Direct3DDev *p, Vint type,
                          Vint npts, Vfloat x[3], Vint dc[][3]);
VKI_EXTERN void
vgl_Direct3DDevColorIndex(vgl_Direct3DDev *p, Vint index);
VKI_EXTERN void
vgl_Direct3DDevColor(vgl_Direct3DDev *p, Vfloat c[3]);
VKI_EXTERN void
vgl_Direct3DDevPolygon(vgl_Direct3DDev *p, Vint type,
                     Vint npts, Vfloat x[][3], Vint vtflag, Vfloat vt[]);
VKI_EXTERN void
vgl_Direct3DDevPolygonColor(vgl_Direct3DDev *p, Vint type,
                          Vint npts, Vfloat x[][3], Vfloat c[][3],
                          Vint vtflag, Vfloat vt[]);
VKI_EXTERN void
vgl_Direct3DDevPolygonDC(vgl_Direct3DDev *p, Vint type,
                         Vint npts, Vfloat x[3], Vint dc[][3]);
VKI_EXTERN void
vgl_Direct3DDevPolyPointArray(vgl_Direct3DDev *p, Vint npts, Vfloat x[][3],
                              Vint cflag, void *c,
                              Vint vflag, void *v,
                              Vint tflag, Vfloat *t,
                              Vint dflag, Vfloat *d);
VKI_EXTERN void
vgl_Direct3DDevPolyLineArray(vgl_Direct3DDev *p, Vint type, Vint npts,
                             Vfloat x[][3],
                             Vint cflag, void *c,
                             Vint vflag, void *v,
                             Vint tflag, Vfloat *t,
                             Vint dflag, Vfloat *d);
VKI_EXTERN void
vgl_Direct3DDevPolygonArray(vgl_Direct3DDev *p, Vint type,
                            Vint npts, Vfloat x[][3],
                            Vint cflag, void *c,
                            Vint vflag, void *v,
                            Vint tflag, Vfloat *t,
                            Vint dflag, Vfloat *d);
VKI_EXTERN void
vgl_Direct3DDevXfmPush(vgl_Direct3DDev *p);
VKI_EXTERN void
vgl_Direct3DDevXfmPop(vgl_Direct3DDev *p);
VKI_EXTERN void
vgl_Direct3DDevXfmMult(vgl_Direct3DDev *p, Vfloat xfm[4][4]);
VKI_EXTERN void
vgl_Direct3DDevXfmLoad(vgl_Direct3DDev *p, Vfloat xfm[4][4]);
VKI_EXTERN void
vgl_Direct3DDevAttPush(vgl_Direct3DDev *p, Vint mask);
VKI_EXTERN void
vgl_Direct3DDevAttPop(vgl_Direct3DDev *p);
VKI_EXTERN void
vgl_Direct3DDevPointSize(vgl_Direct3DDev *p, Vint psize);
VKI_EXTERN void
vgl_Direct3DDevLineStyle(vgl_Direct3DDev *p, Vint lstyle);
VKI_EXTERN void
vgl_Direct3DDevLineWidth(vgl_Direct3DDev *p, Vint lwidth);
VKI_EXTERN void
vgl_Direct3DDevTrans(vgl_Direct3DDev *p, Vfloat transp);
VKI_EXTERN void
vgl_Direct3DDevTransIndex(vgl_Direct3DDev *p, Vint index);
VKI_EXTERN void
vgl_Direct3DDevClip(vgl_Direct3DDev *p,
                  Vint left, Vint right, Vint bottom, Vint top);
VKI_EXTERN void
vgl_Direct3DDevText(vgl_Direct3DDev *p, Vfloat x[3], Vtchar *string);
VKI_EXTERN void
vgl_Direct3DDevTextDC(vgl_Direct3DDev *p, Vfloat x[3], Vint dc[3],
                      Vtchar *string);
VKI_EXTERN void
vgl_Direct3DDevSpecularity(vgl_Direct3DDev *p, Vfloat intensity,
                           Vfloat shininess);
VKI_EXTERN void
vgl_Direct3DDevDataIndex(vgl_Direct3DDev *p,
                         Vint nprims, Vint nrws, Vint *index);
VKI_EXTERN void
vgl_Direct3DDevData(vgl_Direct3DDev *p, Vint nrws, Vfloat d[]);
VKI_EXTERN void
vgl_Direct3DDevPolyPointData(vgl_Direct3DDev *p,
                           Vint npts, Vfloat x[][3], Vint nrws, Vfloat d[]);
VKI_EXTERN void
vgl_Direct3DDevPolyLineData(vgl_Direct3DDev *p, Vint type,
                          Vint npts, Vfloat x[][3], Vint nrws, Vfloat d[]);
VKI_EXTERN void
vgl_Direct3DDevPolygonData(vgl_Direct3DDev *p,
                         Vint type, Vint npts, Vfloat x[][3],
                         Vint nrws, Vfloat *d, Vint vtflag, Vfloat vt[]);
VKI_EXTERN void
vgl_Direct3DDevRender(vgl_Direct3DDev *p, Vint type);
VKI_EXTERN void
vgl_Direct3DDevSelectRegion(vgl_Direct3DDev *p, Vint type, Vint *params);
VKI_EXTERN void
vgl_Direct3DDevSelectBuffer(vgl_Direct3DDev *p, Vint size, Vint *indexlist,
                          Vfloat *mindepth, Vfloat *maxdepth);
VKI_EXTERN void
vgl_Direct3DDevSelectQuery(vgl_Direct3DDev *p, Vint *hits);
VKI_EXTERN void
vgl_Direct3DDevExtentQuery(vgl_Direct3DDev *p,
                         Vfloat *lft, Vfloat *rgt, Vfloat *btm, Vfloat *top, 
                         Vfloat *znear, Vfloat *zfar);
VKI_EXTERN void
vgl_Direct3DDevPointer(vgl_Direct3DDev *p, Vint datatype, Vint ptrtype,
                       void *ptr);
VKI_EXTERN void
vgl_Direct3DDevInitBuffer(vgl_Direct3DDev *p, Vint size, Vint *vboid);
VKI_EXTERN void
vgl_Direct3DDevTermBuffer(vgl_Direct3DDev *p, Vint vboid);
VKI_EXTERN void
vgl_Direct3DDevCopyBuffer(vgl_Direct3DDev *p, Vint vboid, Vint npts,
                          Vfloat x[][3], Vint cflag, void *c,
                          Vint vflag, void *v, Vint tflag, Vfloat *t,
                          Vint dflag, Vfloat *d, Vint *offset);
VKI_EXTERN void
vgl_Direct3DDevPolyPointBuffer(vgl_Direct3DDev *p, Vint vboid, Vint offset,
                               Vint npts, Vint cflag, Vint vflag,
                               Vint tflag, Vint dflag);
VKI_EXTERN void
vgl_Direct3DDevPolyLineBuffer(vgl_Direct3DDev *p, Vint vboid, Vint offset,
                              Vint type, Vint npts, Vint cflag,
                              Vint vflag, Vint tflag, Vint dflag);
VKI_EXTERN void
vgl_Direct3DDevPolygonBuffer(vgl_Direct3DDev *p, Vint vboid, Vint offset,
                             Vint type, Vint npts, Vint cflag,
                             Vint vflag, Vint tflag, Vint dflag);
VKI_EXTERN void
vgl_Direct3DDevSetFactor(vgl_Direct3DDev *p, Vint type, Vfloat factor);
VKI_EXTERN void
vgl_Direct3DDevDrawFun(vgl_Direct3DDev *p, vgl_DrawFun *df);

#ifdef  __cplusplus
}
#endif

                   /* define for fast polygon switching */ 
#define Direct3Dsetpoly(p,flag)  if((flag) != (p)->polyflag) {\
                                  vgl_Direct3DDev_setpoly ((p),(flag));\
                               }
                   /* define for fast text plane switching */
#define Direct3Dsetplan(p,flag)  if((flag) != (p)->planflag) {\
                                  vgl_Direct3DDev_setplan ((p),(flag));\
                               }
                   /* define for fast light on/off switching */
#define Direct3Dsetshade(p,flag)  if((flag) != (p)->iliton) {\
                                  vgl_Direct3DDev_setshade ((p),(flag));\
                               }

#endif
