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
 
#ifndef POPUP_DEF
#define POPUP_DEF
#include "base/basedefs.h"
#include "vgl/drawfun.h"
#include "vgl/pixelmap.h"
#define POPUP_ITEM 1
#define POPUP_POPUP 2
#define POPUP_TOGGLE 3
#define POPUP_RADIO 4
#define POPUP_LINE 5
#define POPUP_MAXITEM 256
#define POPUP_FUN_DISPLAY 3
#define POPUP_COLOR_BACKGROUND 0
#define POPUP_PIXELMAPSTORE 1
struct vgl_Popup { Vint ierr;Vint xpix,ypix;Vfloat xlen,
ylen;vgl_DrawFun *df;Vint pixstore;Vfloat cback[3];
Vfloat chigh[3];Vfloat clite[3];Vfloat cshad[3];Vfloat
ctog[3];Vfloat crad[3];Vint nshad;Vint xsav,ysav;Vint
xcur,ycur;Vint curdrag;Vint curpopup;Vint inflag;Vint
izmode,ilmode,ifmode;Vint itindex;Vint irindex;Vfloat
xinit[3];Vint xext;Vint yext;Vint titleflag;Vtchar title[33];
Vint numitems;Vint curitem;Vint itemtyp[POPUP_MAXITEM];
Vint itemind[POPUP_MAXITEM];Vint itemsub[POPUP_MAXITEM];
Vtchar itemstg[POPUP_MAXITEM][33];Vfunc2 *itemfun[POPUP_MAXITEM];
Vobject *itemobj[POPUP_MAXITEM];Vint itemact[POPUP_MAXITEM];
Vint itemval[POPUP_MAXITEM];Vint itemyext[POPUP_MAXITEM][2];
vgl_Popup *itempop[POPUP_MAXITEM];Vint textdc[3];Vfunc2
*fundis;Vobject *funobjdis;vgl_Pixelmap *pm;
#ifdef __cplusplus
public: VKI_EXTERN vgl_Popup(void);VKI_EXTERN void*
operator new (size_t);VKI_EXTERN ~vgl_Popup(void);VKI_EXTERN
void operator delete (void*);VKI_EXTERN Vint Error();
VKI_EXTERN void SetObject(Vint,Vobject*);VKI_EXTERN
void GetObject(Vint,Vobject**);VKI_EXTERN void SetParami(Vint,Vint);
VKI_EXTERN void SetParamfv(Vint,Vfloat*);VKI_EXTERN
void SetFunction(Vint,VPopupfunc*,Vobject*);VKI_EXTERN
void Title(Vtchar*);VKI_EXTERN void AddLine();VKI_EXTERN
void AddItem(Vint,Vtchar*,VPopupfunc*,Vobject*);VKI_EXTERN
void AddToggle(Vint,Vtchar*,VPopupfunc*,Vobject*);VKI_EXTERN
void SetToggle(Vint,Vint);VKI_EXTERN void GetToggle(Vint,Vint*);
VKI_EXTERN void AddRadio(Vint,Vint,Vtchar*,VPopupfunc*,Vobject*);
VKI_EXTERN void SetRadio(Vint,Vint);VKI_EXTERN void
GetRadio(Vint,Vint*);VKI_EXTERN void AddPopup(Vint,Vtchar*,vgl_Popup*);
VKI_EXTERN void SetActive(Vint,Vint);VKI_EXTERN void
GetActive(Vint,Vint*);VKI_EXTERN void GetIndex(Vint*);
VKI_EXTERN void Drag(Vint,Vint,Vint);
#endif
};
#ifdef __cplusplus
extern "C" { 
#endif
VKI_EXTERN vgl_Popup* vgl_PopupBegin(void);VKI_EXTERN
void vgl_Popup_Construct(vgl_Popup *p);VKI_EXTERN void
vgl_PopupEnd(vgl_Popup *p);VKI_EXTERN void vgl_Popup_Destruct(vgl_Popup
*p);VKI_EXTERN Vint vgl_PopupError(vgl_Popup *p);VKI_EXTERN
void vgl_PopupSetObject(vgl_Popup *p,Vint objecttype,
Vobject *object);VKI_EXTERN void vgl_PopupGetObject(vgl_Popup
*p,Vint objecttype,Vobject **object);VKI_EXTERN void
vgl_PopupSetParami(vgl_Popup *p,Vint type,Vint iparam);
VKI_EXTERN void vgl_PopupSetParamfv(vgl_Popup *p,Vint
type,Vfloat *values);VKI_EXTERN void vgl_PopupSetFunction(vgl_Popup
*p,Vint functype,VPopupfunc *function,Vobject *object);
VKI_EXTERN void vgl_PopupTitle(vgl_Popup *p,Vtchar *title);
VKI_EXTERN void vgl_PopupAddLine(vgl_Popup *p);VKI_EXTERN
void vgl_PopupAddItem(vgl_Popup *p,Vint index,Vtchar
*stg,VPopupfunc *function,Vobject *object);VKI_EXTERN
void vgl_PopupAddToggle(vgl_Popup *p,Vint index,Vtchar
*stg,VPopupfunc *function,Vobject *object);VKI_EXTERN
void vgl_PopupSetToggle(vgl_Popup *p,Vint index,Vint
flag);VKI_EXTERN void vgl_PopupGetToggle(vgl_Popup *p,
Vint index,Vint *flag);VKI_EXTERN void vgl_PopupAddRadio(vgl_Popup
*p,Vint index,Vint subindex,Vtchar *stg,VPopupfunc *function,
Vobject *object);VKI_EXTERN void vgl_PopupSetRadio(vgl_Popup
*p,Vint index,Vint subindex);VKI_EXTERN void vgl_PopupGetRadio(vgl_Popup
*p,Vint index,Vint *subindex);VKI_EXTERN void vgl_PopupAddPopup(vgl_Popup
*p,Vint index,Vtchar *stg,vgl_Popup *popupitem);VKI_EXTERN
void vgl_PopupSetActive(vgl_Popup *p,Vint index,Vint
flag);VKI_EXTERN void vgl_PopupGetActive(vgl_Popup *p,
Vint index,Vint *flag);VKI_EXTERN void vgl_PopupGetIndex(vgl_Popup
*p,Vint *index);VKI_EXTERN void vgl_PopupDrag(vgl_Popup
*p,Vint drag,Vint ix,Vint iy);
#ifdef __cplusplus
}
#endif
#endif

