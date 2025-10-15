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
 
#ifndef MATERIAL_DEF
#define MATERIAL_DEF
#include "base/basedefs.h"
typedef struct vgl_Material { Vint ierr;Vfloat emis[3];
Vfloat amb[3];Vfloat diff[3];Vfloat spec[3];Vint exp;
}vgl_Material;
#ifdef __cplusplus
extern "C" { 
#endif
VKI_EXTERN vgl_Material* vgl_MaterialBegin(void);VKI_EXTERN
void vgl_MaterialEnd(vgl_Material *material);VKI_EXTERN
Vint vgl_MaterialError(vgl_Material *material);VKI_EXTERN
void vgl_MaterialSetEmission(vgl_Material *material,
Vfloat c[3]);VKI_EXTERN void vgl_MaterialSetAmbient(vgl_Material
*material,Vfloat c[3]);VKI_EXTERN void vgl_MaterialSetDiffuse(vgl_Material
*material,Vfloat c[3]);VKI_EXTERN void vgl_MaterialSetAmbientDiffuse(vgl_Material
*material,Vfloat c[3]);VKI_EXTERN void vgl_MaterialSetSpecular(vgl_Material
*material,Vfloat c[3],Vint exp);VKI_EXTERN void vgl_MaterialGetEmission(vgl_Material
*material,Vfloat c[3]);VKI_EXTERN void vgl_MaterialGetAmbient(vgl_Material
*material,Vfloat c[3]);VKI_EXTERN void vgl_MaterialGetDiffuse(vgl_Material
*material,Vfloat c[3]);VKI_EXTERN void vgl_MaterialGetSpecular(vgl_Material
*material,Vfloat c[3],Vint *exp);
#ifdef __cplusplus
}
#endif
#endif

