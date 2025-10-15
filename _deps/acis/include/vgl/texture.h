/***********************************************************************
*                                                                      *
*               Copyright (C) 2016, Visual Kinematics, Inc.            *
*                                                                      *
*  These coded instructions, statements and computer programs contain  *
*  unpublished proprietary information of Visual Kinematics, Inc.,     *
*  and are protected by Federal copyright law.  They may not be        *
*  disclosed to third parties or copied or duplicated in any form,     *
*  in whole or in part, without the prior written consent of           *
*  Visual Kinematics, Inc.                                             *
*                                                                      *
***********************************************************************/
/***********************************************************************
*                                                                      *
*               DevTools4.4.1 Thu Jun 23 14:48:16 PDT 2016              *
*                                                                      *
***********************************************************************/
 
#ifndef TEXTURE_DEF
#define TEXTURE_DEF
#include "base/basedefs.h"
#include "vgl/fbuffer.h"
#define TEXTURE_CLAMP 0
#define TEXTURE_REPEAT 1
#define TEXTURE_NEAREST 0
#define TEXTURE_LINEAR 1
#define TEXTURE_DECAL 0
#define TEXTURE_MODULATE 1
#define TEXTURE_BLEND 2
#define TEXTURE_HUE_BANDED 1
#define TEXTURE_HUE_BANDED_GRAY 2
struct vgl_Texture { Vint ierr;Vint vd[2];Vint components;
Vint minfilter,magfilter;Vint env;Vfloat cenv[4];Vint
swrap,twrap;Vuint *texti;
#ifdef __cplusplus
public: VKI_EXTERN vgl_Texture(void);VKI_EXTERN void*
operator new (size_t);VKI_EXTERN ~vgl_Texture(void);
VKI_EXTERN void operator delete (void*);VKI_EXTERN Vint
Error();VKI_EXTERN void Load(Vint);VKI_EXTERN void Def(Vint,Vint,Vint);
VKI_EXTERN void Inq(Vint*,Vint*,Vint*);VKI_EXTERN void
SetFilter(Vint,Vint);VKI_EXTERN void GetFilter(Vint*,Vint*);
VKI_EXTERN void SetEnv(Vint);VKI_EXTERN void GetEnv(Vint*);
VKI_EXTERN void SetEnvColor(Vfloat[]);VKI_EXTERN void
GetEnvColor(Vfloat[]);VKI_EXTERN void SetWrap(Vint,Vint);
VKI_EXTERN void GetWrap(Vint*,Vint*);VKI_EXTERN void
SetTexels(Vuint[]);VKI_EXTERN void GetTexels(Vuint[]);
VKI_EXTERN void SetColor(Vint,Vint,Vfloat[]);VKI_EXTERN
void GetColor(Vint,Vint,Vfloat[]);VKI_EXTERN void SetColors(Vfloat[]);
VKI_EXTERN void GetColors(Vfloat[]);VKI_EXTERN void
Point(Vfloat,Vfloat,Vfloat[],Vfloat[4],Vfloat[4]);VKI_EXTERN
void Copy(vgl_Texture*);VKI_EXTERN void FBuffer(vgl_FBuffer*);
VKI_EXTERN void Print();
#endif
};
#ifdef __cplusplus
extern "C" { 
#endif
VKI_EXTERN vgl_Texture* vgl_TextureBegin(void);VKI_EXTERN
void vgl_Texture_Construct(vgl_Texture *p);VKI_EXTERN
void vgl_TextureEnd(vgl_Texture *p);VKI_EXTERN void
vgl_Texture_Destruct(vgl_Texture *p);VKI_EXTERN Vint
vgl_TextureError(vgl_Texture *p);VKI_EXTERN void vgl_TextureLoad(vgl_Texture
*p,Vint itexture);VKI_EXTERN void vgl_TextureDef(vgl_Texture
*p,Vint components,Vint xsize,Vint ysize);VKI_EXTERN
void vgl_TextureInq(vgl_Texture *p,Vint *components,
Vint *xsize,Vint *ysize);VKI_EXTERN void vgl_TextureSetFilter(vgl_Texture
*p,Vint minfilter,Vint magfilter);VKI_EXTERN void vgl_TextureGetFilter(vgl_Texture
*p,Vint *minfilter,Vint *magfilter);VKI_EXTERN void
vgl_TextureSetEnv(vgl_Texture *p,Vint env);VKI_EXTERN
void vgl_TextureGetEnv(vgl_Texture *p,Vint *env);VKI_EXTERN
void vgl_TextureSetEnvColor(vgl_Texture *p,Vfloat c[]);
VKI_EXTERN void vgl_TextureGetEnvColor(vgl_Texture *p,
Vfloat c[]);VKI_EXTERN void vgl_TextureSetWrap(vgl_Texture
*p,Vint swrap,Vint twrap);VKI_EXTERN void vgl_TextureGetWrap(vgl_Texture
*p,Vint *swrap,Vint *twrap);VKI_EXTERN void vgl_TextureSetTexels(vgl_Texture
*p,Vuint texels[]);VKI_EXTERN void vgl_TextureGetTexels(vgl_Texture
*p,Vuint texels[]);VKI_EXTERN void vgl_TextureSetColor(vgl_Texture
*p,Vint ix,Vint iy,Vfloat c[]);VKI_EXTERN void vgl_TextureGetColor(vgl_Texture
*p,Vint ix,Vint iy,Vfloat c[]);VKI_EXTERN void vgl_TextureSetColors(vgl_Texture
*p,Vfloat c[]);VKI_EXTERN void vgl_TextureGetColors(vgl_Texture
*p,Vfloat c[]);VKI_EXTERN void vgl_TexturePoint(vgl_Texture
*p,Vfloat rx,Vfloat ry,Vfloat t[],Vfloat cf[4],Vfloat
c[4]);VKI_EXTERN void vgl_TextureCopy(vgl_Texture *p,
vgl_Texture *fromp);VKI_EXTERN void vgl_TextureFBuffer(vgl_Texture
*p,vgl_FBuffer *fbuffer);VKI_EXTERN void vgl_TexturePrint(vgl_Texture
*p);
#ifdef __cplusplus
}
#endif
#endif

