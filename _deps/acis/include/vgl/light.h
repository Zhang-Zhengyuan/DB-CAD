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
 
#ifndef LIGHT_DEF
#define LIGHT_DEF
#include "base/basedefs.h"
#define LIGHT_AMBIENT 0
#define LIGHT_DISTANT 1
#define LIGHT_LOCAL 2
typedef struct vgl_Light { Vint ierr;int lighttype;
float position[3];float direction[3];float color[3];
float pn[3];float value;int lightback;}vgl_Light;
#ifdef __cplusplus
extern "C" { 
#endif
VKI_EXTERN vgl_Light* vgl_LightBegin(void);VKI_EXTERN
void vgl_LightEnd(vgl_Light *light);VKI_EXTERN Vint
vgl_LightError(vgl_Light *light);VKI_EXTERN void vgl_LightDef(vgl_Light
*light,Vint lighttype,Vfloat c[3],Vfloat x[3]);VKI_EXTERN
void vgl_LightInq(vgl_Light *light,Vint *lighttype,
Vfloat c[3],Vfloat x[3]);VKI_EXTERN void vgl_LightSetBackFace(vgl_Light
*light,Vint flag);VKI_EXTERN void vgl_LightGetBackFace(vgl_Light
*light,Vint *flag);VKI_EXTERN void vgl_LightSimpleAmbient(vgl_Light
*light,Vfloat *value);VKI_EXTERN void vgl_LightSimpleDiffuse(vgl_Light
*light,Vfloat v[3],Vfloat *value);VKI_EXTERN void vgl_LightNdotI(vgl_Light
*light,Vfloat v[3],Vfloat x[3],Vfloat *ni);VKI_EXTERN
void vgl_LightR(vgl_Light *light,Vfloat v[3],Vfloat
x[3],Vfloat r[3]);VKI_EXTERN void vgl_LightH(vgl_Light
*light,Vfloat vr[3],Vfloat v[3],Vfloat x[3],Vfloat h[3]);
VKI_EXTERN void vgl_LightCopy(vgl_Light *light,vgl_Light
*fromlight);VKI_EXTERN void vgl_LightPrint(vgl_Light
*light);
#ifdef __cplusplus
}
#endif
#endif

