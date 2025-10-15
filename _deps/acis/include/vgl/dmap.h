                   /* DMap object */ 

#ifndef DMAP_DEF
#define DMAP_DEF

#include "base/basedefs.h"
#include "base/bitvec.h"
#include "vgl/drawfun.h"

struct vgl_DMap {
   Vint ierr;
   vsy_BitVec *cbv;
   Vint cinmax;
   Vfloat (*cmap)[3];
   vsy_BitVec *tbv;
   Vint tinmax;
   Vfloat *tmap;
   vgl_DrawFun *df;                   /* drawing funtions */
#ifdef  __cplusplus
public:
   VKI_EXTERN vgl_DMap(void);
   VKI_EXTERN void* operator new (size_t);
   VKI_EXTERN ~vgl_DMap(void);
   VKI_EXTERN void operator delete (void*);
   VKI_EXTERN Vint Error();
   VKI_EXTERN void SetObject(Vint,Vobject*);
   VKI_EXTERN void SetColor(Vint,Vfloat[3]);
   VKI_EXTERN void SetTrans(Vint,Vfloat);
   VKI_EXTERN void DrawFun(vgl_DrawFun*);
#endif
};
 
#ifdef  __cplusplus
extern "C" {
#endif

VKI_EXTERN vgl_DMap*
vgl_DMapBegin(void);
VKI_EXTERN void
vgl_DMap_Construct(vgl_DMap *p);
VKI_EXTERN void
vgl_DMapEnd(vgl_DMap *p);
VKI_EXTERN void
vgl_DMap_Destruct(vgl_DMap *p);
VKI_EXTERN Vint
vgl_DMapError(vgl_DMap *p);
VKI_EXTERN void
vgl_DMapSetObject(vgl_DMap *p, Vint objecttype, Vobject *object);
VKI_EXTERN void
vgl_DMapSetColor(vgl_DMap *p, Vint index, Vfloat c[3]);
VKI_EXTERN void
vgl_DMapSetTrans(vgl_DMap *p, Vint index, Vfloat t);
VKI_EXTERN void
vgl_DMapDrawFun(vgl_DMap *p, vgl_DrawFun *df);

#ifdef  __cplusplus
}
#endif

#endif
