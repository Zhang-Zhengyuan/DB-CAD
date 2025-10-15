                   /* vgl jt utility functions header file */
#ifndef VGLJL_DEF
#define VGLJL_DEF

#include "base/basedefs.h"
#include "vgl/dfile.h"

#ifdef  __cplusplus
extern "C" {
#endif

#ifdef VKI_LIBAPI_JT

extern void
vgl_jtRead(vgl_DFile *p);
extern void
vgl_jtClose(vgl_DFile *p);
extern void
vgl_jtInit(vgl_DFile *p);
extern void
vgl_jtTerm(vgl_DFile *p);
extern void
vgl_jtOpenJTPart(vgl_DFile *p, Vobject *part);
extern void
vgl_jtCloseJTPart(vgl_DFile *p);
extern void
vgl_jtColor(vgl_DFile *p, Vfloat c[3]);
extern void
vgl_jtPolypoint(vgl_DFile *p, Vint npts, Vfloat x[][3], Vfloat c[][3], 
                Vchar func[]);
extern void
vgl_jtPolyline(vgl_DFile *p, Vint type, Vint npts, Vfloat x[][3], Vfloat c[][3],
               Vchar func[]);
extern void
vgl_jtPolygon(vgl_DFile *p, Vint type, Vint npts, Vfloat x[][3], Vint vflag, 
              Vfloat v[], Vfloat c[][3], Vchar func[]);
#endif

#ifdef  __cplusplus
}
#endif

#endif
