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
 
#ifndef LMODEL_DEF
#define LMODEL_DEF
#include "base/basedefs.h"
#include "vgl/light.h"
#include "vgl/material.h"
typedef struct vgl_LModel { Vint ierr;Vint nlight;vsy_List
*lightlist;vgl_Material *mat;Vint twosid;Vint lvflag;
Vfloat lv[3];Vfloat koffset,krate;}vgl_LModel;
#ifdef __cplusplus
extern "C" { 
#endif
VKI_EXTERN vgl_LModel* vgl_LModelBegin(void);VKI_EXTERN
void vgl_LModelEnd(vgl_LModel *lmodel);VKI_EXTERN Vint
vgl_LModelError(vgl_LModel *lmodel);VKI_EXTERN void
vgl_LModelSetTwoSide(vgl_LModel *lmodel,Vint flag);
VKI_EXTERN void vgl_LModelSetLocalViewer(vgl_LModel
*lmodel,Vint flag);VKI_EXTERN void vgl_LModelSetAttenuation(vgl_LModel
*lmodel,Vfloat koffset,Vfloat krate);VKI_EXTERN void
vgl_LModelGetLocalViewer(vgl_LModel *lmodel,Vint *flag);
VKI_EXTERN void vgl_LModelGetAttenuation(vgl_LModel
*lmodel,Vfloat *koffset,Vfloat *krate);VKI_EXTERN void
vgl_LModelSetLight(vgl_LModel *lmodel,Vint index,vgl_Light
*light);VKI_EXTERN void vgl_LModelSetObject(vgl_LModel
*lmodel,Vint objecttype,Vobject *object);VKI_EXTERN
void vgl_LModelSimple(vgl_LModel *lmodel,Vfloat v[3],
Vfloat *ints);VKI_EXTERN void vgl_LModelFull(vgl_LModel
*lmodel,Vfloat v[3],Vfloat x[3],Vfloat c[3]);
#ifdef __cplusplus
}
#endif
#endif

