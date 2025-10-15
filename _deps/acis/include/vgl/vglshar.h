                   /* vgl utility functions header file */
#ifndef VGLSHAR_DEF
#define VGLSHAR_DEF

#include "base/basedefs.h"

#ifdef  __cplusplus
extern "C" {
#endif

extern void
vgl_parsePolyLine(Vint type, Vint npts, Vint ind, Vint kx[], Vint *inc);
extern void
vgl_parsePolygon(Vint type, Vint npts, Vint ind, Vint kx[],Vint *kc, Vint *inc);
extern void
vgl_parsePolygon1(Vint type, Vint npts, Vint ind, Vint *lt,Vint *np,Vint kx[],
                  Vint *inc);
extern void
vgl_parsePolyhedron(Vint npts, Vint ind, Vint kx[], Vint *kc, Vint *inc);
extern void
vgl_reflectPolygon(Vint type, Vint npts,
                   Vint kx[], Vint *nx, Vint kc[], Vint *nc);
extern void
vgl_reflectNormal(Vint nc, Vfloat v[][3]);
extern void
vgl_normalPolygon(Vint type, Vint npts, Vfloat x[][3], Vfloat v[][3]);
extern void
vgl_normalPoly(Vint npts, Vfloat x[][3], Vfloat v[3]);
extern void
vgl_linePixels(Vint x1[2], Vint x2[2], Vint *npix, Vint pix[][2]);
extern void
vgl_triPixels(Vint x1[2], Vint x2[2], Vint x3[2], Vint *npix, Vint pix[][2]);
extern void
vgl_convertHSL2RGB(Vfloat chsl[3], Vfloat crgb[3]);
extern void
vgl_convertRGB2HSL(Vfloat crgb[3], Vfloat chsl[3]);
extern void
vgl_project(Vfloat x[3], Vfloat mv[4][4], Vfloat pr[4][4], Vint vp[4],
            Vfloat w[3]);
extern void
vgl_unproject(Vfloat w[3], Vfloat mv[4][4], Vfloat pr[4][4], Vint vp[4],
              Vfloat x[3]);
extern Vint
vgl_clipt(Vfloat den, Vfloat num, Vfloat *te, Vfloat *tl, Vint *iclp);
extern void
vgl_clipLine(Vfloat xmin, Vfloat xmax,
             Vfloat ymin, Vfloat ymax,
             Vfloat zmin, Vfloat zmax,
             Vfloat xa[3], Vfloat xb[3],
             Vfloat *te, Vfloat *tl, Vint *iclp, Vint *ivis);
extern void
vgl_clipPlane(Vfloat xmin, Vfloat xmax,
              Vint nb, Vfloat x[], Vfloat b[][2],
              Vint *nr, Vfloat r[][2], Vint *iclp, Vint *ivis);
extern void
vgl_clipTriangle(Vfloat xmin, Vfloat xmax,
                 Vfloat ymin, Vfloat ymax,
                 Vfloat zmin, Vfloat zmax,
                 Vfloat xa[3], Vfloat xb[3], Vfloat xc[3], 
                 Vint *nr, Vfloat r[][2], Vint *iclp,  Vint *ivis);
extern void
vgl_facePoly(Vint shape, Vint maxi, Vint maxj, Vint type, Vint *nix, Vint ix[]);
extern void
vgl_gvecTriangle(Vfloat x[3][2], Vfloat g[3][2]);
extern void
vgl_setfactorsnum(Vint type, Vint *num);

#ifdef  __cplusplus
}
#endif

#endif
