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
 
#ifndef ATTSTACK_DEF
#define ATTSTACK_DEF
#include "base/basedefs.h"
#include "vgl/drawfun.h"
#define ATTSTACK_MAX 32
typedef struct vgl_Attstack { Vint ierr;Vint numatt;
Vint mask;Vint pointflag[ATTSTACK_MAX];Vint pointsize[ATTSTACK_MAX];
Vint lineflag[ATTSTACK_MAX];Vint linestyle[ATTSTACK_MAX];
Vint linewidth[ATTSTACK_MAX];Vint polygonflag[ATTSTACK_MAX];
Vfloat intensity[ATTSTACK_MAX];Vfloat shininess[ATTSTACK_MAX];
Vint colorflag[ATTSTACK_MAX];Vfloat c[ATTSTACK_MAX][3];
Vfloat t[ATTSTACK_MAX];Vint viewportflag[ATTSTACK_MAX];
Vint left[ATTSTACK_MAX];Vint right[ATTSTACK_MAX];Vint
bottom[ATTSTACK_MAX];Vint top[ATTSTACK_MAX];Vfloat nearz[ATTSTACK_MAX];
Vfloat farz[ATTSTACK_MAX];vgl_DrawFun *df;}vgl_Attstack;
#ifdef __cplusplus
extern "C" { 
#endif
VKI_EXTERN vgl_Attstack* vgl_AttstackBegin(void);VKI_EXTERN
void vgl_AttstackEnd(vgl_Attstack *p);VKI_EXTERN Vint
vgl_AttstackError(vgl_Attstack *p);VKI_EXTERN void vgl_AttstackSetObject(vgl_Attstack
*p,Vint objecttype,Vobject *object);VKI_EXTERN void
vgl_AttstackSetMask(vgl_Attstack *p,Vint mask);VKI_EXTERN
void vgl_AttstackPush(vgl_Attstack *p,Vint psize,Vint
lstyle,Vint lwidth,Vfloat t,Vfloat sint,Vfloat sexp,
Vfloat c[3],Vint left,Vint right,Vint bottom,Vint top,
Vfloat nearz,Vfloat farz);VKI_EXTERN void vgl_AttstackPop(vgl_Attstack
*p);
#ifdef __cplusplus
}
#endif
#endif

