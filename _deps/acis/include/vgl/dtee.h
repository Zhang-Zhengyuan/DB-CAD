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
 
#ifndef DTEE_DEF
#define DTEE_DEF
#include "base/basedefs.h"
#include "vgl/drawfun.h"
struct vgl_DTee { Vint ierr;vgl_DrawFun *df;vgl_DrawFun
*dt;
#ifdef __cplusplus
public: VKI_EXTERN vgl_DTee(void);VKI_EXTERN void* operator
new (size_t);VKI_EXTERN ~vgl_DTee(void);VKI_EXTERN void
operator delete (void*);VKI_EXTERN Vint Error();VKI_EXTERN
void SetObject(Vint,Vobject*);VKI_EXTERN void DrawFun(vgl_DrawFun*);
#endif
};
#ifdef __cplusplus
extern "C" { 
#endif
VKI_EXTERN vgl_DTee* vgl_DTeeBegin(void);VKI_EXTERN
void vgl_DTee_Construct(vgl_DTee *p);VKI_EXTERN void
vgl_DTeeEnd(vgl_DTee *p);VKI_EXTERN void vgl_DTee_Destruct(vgl_DTee
*p);VKI_EXTERN Vint vgl_DTeeError(vgl_DTee *p);VKI_EXTERN
void vgl_DTeeSetObject(vgl_DTee *p,Vint objecttype,
Vobject *object);VKI_EXTERN void vgl_DTeeDrawFun(vgl_DTee
*p,vgl_DrawFun *df);
#ifdef __cplusplus
}
#endif
#endif

