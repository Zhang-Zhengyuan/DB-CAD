                   /* Web3D integration module */ 

#ifndef VGLWEB3D_DEF
#define VGLWEB3D_DEF

#include "base/basedefs.h"
#include "vgl/dfile.h"

typedef struct {
   Vint    type;
   Vint    colorPerVertex;
   Vint    normalType;
   Vint    texture1d;
   Vint    texture2d;
   Vint    npts;
   Vint    ncix;
   Vint    nnor;
   Vint    nix;
   Vint    ncolors;
   Vint    nc;
   Vint    maxnpt;
   Vfloat *x;
   Vfloat *c;
   Vfloat *n;
   Vfloat *t;
   Vint   *ix;
   Vint   *cix;
} vgl_Web3DBuf;

typedef struct vgl_Web3D vgl_Web3D;
struct vgl_Web3D {
   Vint ierr;
   Vint format;
   Vint indent;
   Vint linewidth;
   Vint linestyle;
   Vint nrot;
   Vfloat transp;
   Vfloat shininess;
   Vfloat intensity;
   FILE *fd;
   Vfloat  color[3];
   vgl_Web3DBuf *buffer;
   vgl_Xfm *xfm;
   vgl_Xfmstack *xfmstack;
   Vfloat trans[3];
   Vfloat scale[3];
   Vfloat rotation[4];
   Vfloat scaleOrientation[4];
   vsy_HashTable *rasfonthash;
   vsy_HashTable *texturehash;
   vgl_RasFont *rasfontcur;
   Vint textureind;
   Vfloat backcrgb[3];
   Vint backcset;
   Vint ngeom;
   Vint numat;
   vsy_IntDict *matid;
   vsy_IntVec  *mativ;
   vsy_FltVec  *tmfv;
   Vfloat tm[16];
};

#ifdef  __cplusplus
extern "C" {
#endif

VKI_EXTERN vgl_Web3D*
vgl_Web3DBegin(void);
VKI_EXTERN void
vgl_Web3DEnd(vgl_Web3D *p);
VKI_EXTERN Vint
vgl_Web3DError(vgl_Web3D *p);
VKI_EXTERN void
vgl_Web3DOpen(vgl_Web3D *p, Vint format, Vchar filename[], FILE *fd);
VKI_EXTERN void
vgl_Web3DClose(vgl_Web3D *p);
VKI_EXTERN void
vgl_Web3DColor(vgl_Web3D *p, Vfloat c[3]);
VKI_EXTERN void
vgl_Web3DPolyPoint(vgl_Web3D *p, Vint npts, Vfloat x[][3]);
VKI_EXTERN void
vgl_Web3DPolyPointColor(vgl_Web3D *p, Vint npts, Vfloat x[][3], Vfloat c[][3]);
VKI_EXTERN void
vgl_Web3DPolyLine(vgl_Web3D *p, Vint type, Vint npts, Vfloat x[][3]);
VKI_EXTERN void
vgl_Web3DPolyLineColor(vgl_Web3D *p, Vint type, Vint npts, Vfloat x[][3],
                       Vfloat c[][3]);
VKI_EXTERN void
vgl_Web3DLineStyle(vgl_Web3D *p, Vint linestyle);
VKI_EXTERN void
vgl_Web3DLineWidth(vgl_Web3D *p, Vint linewidth);
VKI_EXTERN void
vgl_Web3DPolygon(vgl_Web3D *p, Vint type, Vint npts, Vfloat x[][3],
                 Vint vtflag, Vfloat vt[]);
VKI_EXTERN void
vgl_Web3DPolygonColor(vgl_Web3D *p, Vint type, Vint npts, Vfloat x[][3],
                      Vfloat c[][3], Vint vtflag, Vfloat vt[]);
VKI_EXTERN void
vgl_Web3DTrans(vgl_Web3D *p, Vfloat transp);
VKI_EXTERN void
vgl_Web3DSpecularity(vgl_Web3D *p, Vfloat intensity, Vfloat shininess);
VKI_EXTERN void
vgl_Web3DXfmPush(vgl_Web3D *p);
VKI_EXTERN void
vgl_Web3DXfmPop(vgl_Web3D *p);
VKI_EXTERN void
vgl_Web3DXfmLoad(vgl_Web3D *p, Vfloat t[4][4]);
VKI_EXTERN void
vgl_Web3DXfmMult(vgl_Web3D *p, Vfloat t[4][4]);
VKI_EXTERN void
vgl_Web3DRasFontDefine(vgl_Web3D *p, Vint index, vgl_RasFont *rasfont);
VKI_EXTERN void
vgl_Web3DRasFontSelect(vgl_Web3D *p, Vint index);
VKI_EXTERN void
vgl_Web3DText(vgl_Web3D *p, Vfloat x[3], Vtchar *string);
VKI_EXTERN void
vgl_Web3DTextDC(vgl_Web3D *p, Vfloat x[3], Vint dc[], Vtchar *string);
VKI_EXTERN void
vgl_Web3DTextureDefine (vgl_Web3D *p, Vint index, vgl_Texture *texture);
VKI_EXTERN void
vgl_Web3DTextureSelect(vgl_Web3D *p, Vint index);
VKI_EXTERN void
vgl_Web3DBackColor(vgl_Web3D *p, Vfloat c[3]);
VKI_EXTERN void
vgl_Web3DGetInteger(vgl_Web3D *p, Vint type, Vint *params);

#ifdef  __cplusplus
}
#endif

#endif
