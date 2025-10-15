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
 
#ifndef IACTOR_DEF
#define IACTOR_DEF
#include "base/basedefs.h"
#include "vgl/xfm.h"
#include "vgl/xfmstack.h"
#include "vgl/drawfun.h"
#include "vgl/popup.h"
#include "vgl/quadric.h"
#include "vgl/vgltrball.h"
#define IACTOR_NONE 0
#define IACTOR_RETURN 1
#define IACTOR_ROTATE 2
#define IACTOR_TRANSLATE 3
#define IACTOR_SCALE 4
#define IACTOR_ROTATERATE 5
#define IACTOR_TRANSLATERATE 6
#define IACTOR_SCALERATE 7
#define IACTOR_TRANSFORMINIT 8
#define IACTOR_CENTER 9
#define IACTOR_WINDOW 10
#define IACTOR_TRACKBALL 11
#define IACTOR_FIT 12
#define IACTOR_SETROTCENTER 13
#define IACTOR_SQUIGGLE 14
#define IACTOR_POPUP 15
#define IACTOR_POPUP_1 16
#define IACTOR_POPUP_2 17
#define IACTOR_POPUP_3 18
#define IACTOR_DOLLY 19
#define IACTOR_DOLLYRATE 20
#define IACTOR_TRANSFORMSNAP 21
#define IACTOR_SCALECENTER 22
#define IACTOR_DOLLYCENTER 23
#define IACTOR_NEARPLANE 24
#define IACTOR_FARPLANE 25
#define IACTOR_FRAME 26
#define IACTOR_PAN 27
#define IACTOR_ZOOM 28
#define IACTOR_ZOOMCENTER 29
#define IACTOR_ROTATECENTER 30
#define IACTOR_CLIPPLANETRANSLATE 31
#define IACTOR_CLIPPLANETRANSLATE_DEGEN 32
#define IACTOR_CLIPPLANETRANSLATE_INPLANE 33
#define IACTOR_CLIPPLANEROTATE 34
#define IACTOR_CLIPPLANEROTATE_DEGEN 35
#define IACTOR_CLIPPLANEDETECT 36
#define IACTOR_CLIPPLANEDATA 37
#define IACTOR_SCALEBYFACTOR 38
#define IACTOR_MAXACTION 38
#define IACTOR_CURSORS 1
#define IACTOR_TRACKBALLRADIUS 2
#define IACTOR_SPINMATRIX 3
#define IACTOR_ROTCENTER 4
#define IACTOR_FITSHRINK 5
#define IACTOR_COLOR_DRAG 6
#define IACTOR_COLOR_DRAGSTRIPE 7
#define IACTOR_FITTYPE 8
#define IACTOR_FITZFACT 9
#define IACTOR_INDPROJ 10
#define IACTOR_ROTCENTERDEVICE 11
#define IACTOR_DOLLYGAIN 12
#define IACTOR_SCALEGAIN 13
#define IACTOR_NEARADJUST 14
#define IACTOR_FARADJUST 15
#define IACTOR_BOUNDBOX 16
#define IACTOR_PLANEGAIN 17
#define IACTOR_USEPROJ 18
#define IACTOR_SMOOTHMOVE 19
#define IACTOR_SMOOTHSTEPS 20
#define IACTOR_VERTDOLLY 21
#define IACTOR_VERTSCALE 22
#define IACTOR_VERTZOOM 23
#define IACTOR_LINEWIDTH_DRAG 24
#define IACTOR_FITZLENGTH 25
#define IACTOR_FITZTYPE 26
#define IACTOR_COLOR_XAXIS 27
#define IACTOR_COLOR_YAXIS 28
#define IACTOR_COLOR_ZAXIS 29
#define IACTOR_CLIPPLANECENTER 30
#define IACTOR_CLIPPLANENORMAL 31
#define IACTOR_CLIPPLANEROTVEC 32
#define IACTOR_CLIPPLANEICONRADIUS 33
#define IACTOR_COLOR_CLIPPLANETRANSLATE 34
#define IACTOR_COLOR_CLIPPLANETRANSLATE_INPLANE 35
#define IACTOR_COLOR_CLIPPLANEROTATE 36
#define IACTOR_COLOR_CLIPPLANEROTATE_HANDLE 37
#define IACTOR_CLIPPLANEICONNORMAL 38
#define IACTOR_CLIPPLANEICONRADIUSFACTOR 39
#define IACTOR_SCALEFACTOR 40
#define IACTOR_USEFRONTBUFFER 41
#define IACTOR_FITTYPE_EXTENT 0
#define IACTOR_FITTYPE_RADIUS 1
#define IACTOR_FITTYPE_BOUNDBOX 2
#define IACTOR_FITZTYPE_FACT 3
#define IACTOR_FITZTYPE_LENGTH 4
#define IACTOR_LEFTMOUSE 1
#define IACTOR_MIDDLEMOUSE 2
#define IACTOR_RIGHTMOUSE 3
#define IACTOR_SHIFT 4
#define IACTOR_CTRL 5
#define IACTOR_ALT 6
#define IACTOR_ESC 7
#define IACTOR_MAXBUTTON 7
#define IACTOR_FUN_IDLE 1
#define IACTOR_FUN_RESIZE 2
#define IACTOR_FUN_DISPLAY 3
#define IACTOR_FUN_BUTTON 4
#define IACTOR_FUN_UNBOUND 5
#define IACTOR_DRAG 1
#define IACTOR_DRAG_INIT 1
#define IACTOR_DRAG_MOVE 2
#define IACTOR_DRAG_TERM 3
struct vgl_IActor { Vint ierr;Vint onebutact[IACTOR_MAXBUTTON];
Vint twobutact[IACTOR_MAXBUTTON][IACTOR_MAXBUTTON];
Vint curact;Vint xpix,ypix;vgl_DrawFun *df;vgl_Popup
*pu,*pu1,*pu2,*pu3;Vfloat rotcen[3];Vfloat bbox[2][3];
Vfloat radius;Vfloat cplcen[3];Vfloat cplnrm[3];Vfloat
cplrot[3];Vint cplsiz;Vint cpliconnormal;Vfloat cpliconradiusfact;
Vfloat cplcenini[3],wcenini[3];Vfloat cplnrmini[3];
Vint curini[2];vgl_Quadric *quadric;Vfloat fleft,fright,
fbottom,ftop,fnearz,ffarz;Vfloat pleft,pright,pbottom,
ptop,pnearz,pfarz;Vfloat pdz;Vint pprojtype;Vfloat fnearzref;
Vfloat fitshrink;Vfloat fitzfact;Vfloat fitzlength;
Vfloat scalegain;Vfloat dollygain;Vfloat planegain;
Vint fittyp;Vint fitztyp;Vint adjnear,adjfar;Vint iuseproj;
Vint iindproj;Vint iusefrontbuffer;Vint smoothmove;
Vint smoothsteps;Vint vertdolly;Vint vertscale;Vint
vertzoom;Vint xloc,yloc;Vint xsav,ysav;Vfloat tmcorr[4][4];
Vfloat tmsave[4][4];Vint izrot;Vint dragoper;Vint dragrect;
Vfloat cdrag[3];Vfloat cdragstripe[3];Vint lwdrag;Vfloat
caxis[3][3];Vfloat cclipicon[3];Vfloat cclipnorm[3];
Vfloat cclipplan[3];Vfloat ccliprot[3];Vfloat ccliprothandle[3];
Vfloat scalefactor;Vint mainpollstatus;Vint izmode,
ixmode,ifmode;Vint itindex;Vint iold;Vfloat xr[4][3];
Vint xext[2];Vint yext[2];Vint nsquig;Vint cursorflag;
Vint curcursor;Vfunc2 *fundis;Vobject *funobjdis;Vfunc2
*funexp;Vobject *funobjexp;Vfunc2 *funidl;Vobject *funobjidl;
Vfunc2 *funbut;Vobject *funobjbut;Vfunc2 *fununb;Vobject
*funobjunb;Vint abortflag;Vfloat tmr[4][4];Vfloat wsca[3],
wtra[3],wrot[3];Vfloat tmw[4][4];Vfloat tmwi[4][4];
vgl_Xfm *xfm;vgl_Xfmstack *xfmstack;vgl_TrBall *ball;
#ifdef __cplusplus
public: VKI_EXTERN vgl_IActor(void);VKI_EXTERN void*
operator new (size_t);VKI_EXTERN ~vgl_IActor(void);
VKI_EXTERN void operator delete (void*);VKI_EXTERN Vint
Error();VKI_EXTERN void SetObject(Vint,Vobject*);VKI_EXTERN
void GetObject(Vint,Vobject**);VKI_EXTERN void SetFunction(Vint,VIActorfunc*,Vobject*);
VKI_EXTERN void Abort(Vobject*);VKI_EXTERN void CurrentAction(Vint*);
VKI_EXTERN void ButtonAction(Vint,Vint);VKI_EXTERN void
ModifierAction(Vint,Vint,Vint);VKI_EXTERN void MainLoop();
VKI_EXTERN void SetMainPollStatus(Vint);VKI_EXTERN void
GetMainPollStatus(Vint*);VKI_EXTERN void MainPoll();
VKI_EXTERN void Fit(Vobject*);VKI_EXTERN void FitNearFar(Vobject*);
VKI_EXTERN void FitRotCenter(Vobject*);VKI_EXTERN void
FitTrackBall(Vobject*);VKI_EXTERN void Drag(Vint,Vint,Vint,Vint);
VKI_EXTERN void SetParami(Vint,Vint);VKI_EXTERN void
SetParamf(Vint,Vfloat);VKI_EXTERN void SetParamfv(Vint,Vfloat[]);
VKI_EXTERN void GetFloat(Vint,Vfloat[]);VKI_EXTERN void
GetInteger(Vint,Vint[]);VKI_EXTERN void Print();VKI_EXTERN
void DViewDisplay(vgl_DView*);VKI_EXTERN void Spin(Vobject*);
VKI_EXTERN void Display(Vobject*);VKI_EXTERN void Resize(Vobject*);
VKI_EXTERN void TransformInit(Vint);VKI_EXTERN void
TransformView(Vint,Vfloat[4][4]);VKI_EXTERN void Click(Vint,Vint,Vint);
VKI_EXTERN void Unproject(Vfloat[3],Vfloat[3]);VKI_EXTERN
void Project(Vfloat[3],Vfloat[3]);VKI_EXTERN void DrawBackground(Vfloat[3],Vfloat[3]);
VKI_EXTERN void DrawTrackBall();VKI_EXTERN void DrawRotCenter(Vint);
VKI_EXTERN void DrawClipCenter(Vint,Vint,Vint,Vint,Vint*,Vfloat[3]);
VKI_EXTERN void DrawRectangle();VKI_EXTERN void DrawBoundBox();
#endif
};
#ifdef __cplusplus
extern "C" { 
#endif
VKI_EXTERN vgl_IActor* vgl_IActorBegin(void);VKI_EXTERN
void vgl_IActor_Construct(vgl_IActor *p);VKI_EXTERN
void vgl_IActorEnd(vgl_IActor *p);VKI_EXTERN void vgl_IActor_Destruct(vgl_IActor
*p);VKI_EXTERN Vint vgl_IActorError(vgl_IActor *p);
VKI_EXTERN void vgl_IActorSetObject(vgl_IActor *p,Vint
objecttype,Vobject *object);VKI_EXTERN void vgl_IActorGetObject(vgl_IActor
*p,Vint objecttype,Vobject **object);VKI_EXTERN void
vgl_IActorSetFunction(vgl_IActor *p,Vint functype,VIActorfunc
*function,Vobject *object);VKI_EXTERN void vgl_IActorAbort(vgl_IActor
*p,Vobject *dummy);VKI_EXTERN void vgl_IActorCurrentAction(vgl_IActor
*p,Vint *action);VKI_EXTERN void vgl_IActorButtonAction(vgl_IActor
*p,Vint but,Vint action);VKI_EXTERN void vgl_IActorModifierAction(vgl_IActor
*p,Vint but,Vint mod,Vint action);VKI_EXTERN void vgl_IActorMainLoop(vgl_IActor
*p);VKI_EXTERN void vgl_IActorSetMainPollStatus(vgl_IActor
*p,Vint flag);VKI_EXTERN void vgl_IActorGetMainPollStatus(vgl_IActor
*p,Vint *flag);VKI_EXTERN void vgl_IActorMainPoll(vgl_IActor
*p);VKI_EXTERN void vgl_IActorFit(vgl_IActor *p,Vobject
*dummy);VKI_EXTERN void vgl_IActorFitNearFar(vgl_IActor
*p,Vobject *dummy);VKI_EXTERN void vgl_IActorFitRotCenter(vgl_IActor
*p,Vobject *dummy);VKI_EXTERN void vgl_IActorFitTrackBall(vgl_IActor
*p,Vobject *dummy);VKI_EXTERN void vgl_IActorDrag(vgl_IActor
*p,Vint action,Vint drag,Vint ix,Vint iy);VKI_EXTERN
void vgl_IActorSetParami(vgl_IActor *p,Vint type,Vint
iparam);VKI_EXTERN void vgl_IActorSetParamf(vgl_IActor
*p,Vint type,Vfloat fparam);VKI_EXTERN void vgl_IActorSetParamfv(vgl_IActor
*p,Vint type,Vfloat fparam[]);VKI_EXTERN void vgl_IActorGetFloat(vgl_IActor
*p,Vint type,Vfloat params[]);VKI_EXTERN void vgl_IActorGetInteger(vgl_IActor
*p,Vint type,Vint params[]);VKI_EXTERN void vgl_IActorPrint(vgl_IActor
*p);VKI_EXTERN void vgl_IActorDViewDisplay(vgl_IActor
*p,vgl_DView *dview);VKI_EXTERN void vgl_IActorSpin(vgl_IActor
*p,Vobject *dummy);VKI_EXTERN void vgl_IActorDisplay(vgl_IActor
*p,Vobject *dummy);VKI_EXTERN void vgl_IActorResize(vgl_IActor
*p,Vobject *dummy);VKI_EXTERN void vgl_IActorTransformInit(vgl_IActor
*p,Vint action);VKI_EXTERN void vgl_IActorTransformView(vgl_IActor
*p,Vint action,Vfloat tm[4][4]);VKI_EXTERN void vgl_IActorClick(vgl_IActor
*p,Vint action,Vint ix,Vint iy);VKI_EXTERN void vgl_IActorUnproject(vgl_IActor
*p,Vfloat d[3],Vfloat w[3]);VKI_EXTERN void vgl_IActorProject(vgl_IActor
*p,Vfloat d[3],Vfloat w[3]);VKI_EXTERN void vgl_IActorDrawBackground(vgl_IActor
*p,Vfloat cbot[3],Vfloat ctop[3]);VKI_EXTERN void vgl_IActorDrawTrackBall(vgl_IActor
*p);VKI_EXTERN void vgl_IActorDrawRotCenter(vgl_IActor
*p,Vint censiz);VKI_EXTERN void vgl_IActorDrawClipCenter(vgl_IActor
*p,Vint action,Vint ix,Vint iy,Vint iclip,Vint *xaction,
Vfloat xrotvec[3]);VKI_EXTERN void vgl_IActorDrawRectangle(vgl_IActor
*p);VKI_EXTERN void vgl_IActorDrawBoundBox(vgl_IActor
*p);
#ifdef __cplusplus
}
#endif
#endif

