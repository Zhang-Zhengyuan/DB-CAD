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
 
#ifndef RASFONT_DEF
#define RASFONT_DEF
#include "base/basedefs.h"
#include "vgl/bitmap.h"
#include "vgl/xfmstack.h"
#include "vgl/drawfun.h"
#ifdef VKI_WIND_X11
#include <X11/Xlib.h>
#endif
#ifdef VKI_WIND_WIN32
#include <windows.h>
#endif
#define RASFONT_MAXCHAR 127
#define RASFONT_NONE 0
#define RASFONT_QUALITY9X13 1
#define RASFONT_NORMALBOLD8X14 2
#define RASFONT_NORMAL7X11 3
#define RASFONT_FONTNAME 1
#define RASFONT_ITALIC 2
#define RASFONT_WEIGHT 3
#define RASFONT_WIDECHAR 4
#define RASFONT_EXTFONT 5
#define RASFONT_NUMEXTFONTINT 6
#define RASFONT_EXTFONTINT 7
#define RASFONT_TEXTPLANE 1
#define RASFONT_DEVICESIZE 2
#define RASFONT_CLAMPSIZE 3
#define RASFONT_DIRECTION 4
#define RASFONT_ANTIALIAS 5
#define RASFONT_SCREENORTHO 6
struct vgl_RasFont { Vint ierr;Vchar name[256];Vint
width;Vint height;Vint base;Vint offset;Vint space;
Vint size;Vint italic;Vint weight;Vint direction;Vint
neverantialias;Vint antialiasflag;Vint textplaneflag;
Vint devicesizeflag;Vint clampsizeflag;Vint screenorthoflag;
Vfloat px[3],py[3],pz[3];Vfloat pixelsize;Vfloat minsize,
maxsize;Vint posx,posy;Vint userdata[VGL_MAXUSERDATA];
Vint propwidth[RASFONT_MAXCHAR];vgl_Bitmap *rast[RASFONT_MAXCHAR];
vgl_DrawFun *df;vgl_Xfmstack *xfmstack;Vint unicode;
Vint extfont;Vint numextfontint;Vint extfontint[13];
#ifdef VKI_WIND_WIN32
HINSTANCE hInstance;Vint wsiz;HWND hWnd;HDC hdc;HDC
hMemoryDC;HBITMAP hBitmap;Vint sethfont;HFONT hfont;
BITMAPINFO *bmi;unsigned char *bdt;
#endif
#ifdef VKI_WIND_X11
Display *display;XFontStruct *font_info;GC gc;int screen;
int depth;
#endif
vsy_HashTable *rasth;
#ifdef __cplusplus
public: VKI_EXTERN vgl_RasFont(void);VKI_EXTERN void*
operator new (size_t);VKI_EXTERN ~vgl_RasFont(void);
VKI_EXTERN void operator delete (void*);VKI_EXTERN Vint
Error();VKI_EXTERN void Load(Vint);VKI_EXTERN void LoadPSFont(Vchar*);
#ifdef VKI_WIND_X11
VKI_EXTERN void LoadXFont(Display*,Vchar*);VKI_EXTERN
void GetXDisplay(Display**);
#endif
#ifdef VKI_WIND_WIN32
VKI_EXTERN void LoadWINFont(HFONT);
#endif
#ifdef VKI_WIND_WIN32
VKI_EXTERN void GetWINFont(HFONT*);VKI_EXTERN void SetWINFont(HFONT);
#endif
VKI_EXTERN void GetInteger(Vint,Vint[]);VKI_EXTERN void
GetString(Vint,Vchar[]);VKI_EXTERN void SetObject(Vint,Vobject*);
VKI_EXTERN void SetParami(Vint,Vint);VKI_EXTERN void
GetParami(Vint,Vint*);VKI_EXTERN void SetMetrics(Vint,Vint,Vint,Vint);
VKI_EXTERN void SetBitmap(Vint,vgl_Bitmap*);VKI_EXTERN
void SetSpacing(Vint);VKI_EXTERN void SetSize(Vint);
VKI_EXTERN void SetCharWidth(Vint,Vint);VKI_EXTERN void
SetPosition(Vint);VKI_EXTERN void GetPosition(Vint*,Vint*);
VKI_EXTERN void GetMetrics(Vint*,Vint*,Vint*,Vint*);
VKI_EXTERN void GetBitmap(Vint,vgl_Bitmap**);VKI_EXTERN
void GetSpacing(Vint*);VKI_EXTERN void GetSize(Vint*);
VKI_EXTERN void GetCharWidth(Vint,Vint*);VKI_EXTERN
void SetUserData(Vint,Vint);VKI_EXTERN void GetUserData(Vint,Vint*);
VKI_EXTERN void SetPixelSize(Vfloat);VKI_EXTERN void
SetClampSize(Vfloat,Vfloat);VKI_EXTERN void SetPlane(Vfloat[3],Vfloat[3]);
VKI_EXTERN void Length(Vtchar*,Vint*);VKI_EXTERN void
Texture(vgl_Texture*);VKI_EXTERN void DrawPlane(Vfloat[3],Vint[3],Vtchar*);
VKI_EXTERN void Draw(Vfloat[3],Vint[3],Vtchar*);VKI_EXTERN
void Copy(vgl_RasFont*);VKI_EXTERN void Print();
#endif
};
#ifdef __cplusplus
extern "C" { 
#endif
VKI_EXTERN vgl_RasFont* vgl_RasFontBegin(void);VKI_EXTERN
void vgl_RasFont_Construct(vgl_RasFont *p);VKI_EXTERN
void vgl_RasFontEnd(vgl_RasFont *p);VKI_EXTERN void
vgl_RasFont_Destruct(vgl_RasFont *p);VKI_EXTERN Vint
vgl_RasFontError(vgl_RasFont *p);VKI_EXTERN void vgl_RasFontLoad(vgl_RasFont
*p,Vint ifont);VKI_EXTERN void vgl_RasFontLoadPSFont(vgl_RasFont
*p,Vchar *fontname);
#ifdef VKI_WIND_X11
VKI_EXTERN void vgl_RasFontLoadXFont(vgl_RasFont *p,
Display *display,Vchar *fontname);VKI_EXTERN void vgl_RasFontGetXDisplay(vgl_RasFont
*p,Display **display);
#endif
#ifdef VKI_WIND_WIN32
VKI_EXTERN void vgl_RasFontLoadWINFont(vgl_RasFont *p,
HFONT hfont);
#endif
#ifdef VKI_WIND_WIN32
VKI_EXTERN void vgl_RasFontGetWINFont(vgl_RasFont *p,
HFONT *hfont);VKI_EXTERN void vgl_RasFontSetWINFont(vgl_RasFont
*p,HFONT hfont);
#endif
VKI_EXTERN void vgl_RasFontGetInteger(vgl_RasFont *p,
Vint type,Vint iparams[]);VKI_EXTERN void vgl_RasFontGetString(vgl_RasFont
*p,Vint type,Vchar params[]);VKI_EXTERN void vgl_RasFontSetObject(vgl_RasFont
*p,Vint objecttype,Vobject *object);VKI_EXTERN void
vgl_RasFontSetParami(vgl_RasFont *p,Vint type,Vint iparam);
VKI_EXTERN void vgl_RasFontGetParami(vgl_RasFont *p,
Vint type,Vint *iparam);VKI_EXTERN void vgl_RasFontSetMetrics(vgl_RasFont
*p,Vint width,Vint height,Vint offset,Vint base);VKI_EXTERN
void vgl_RasFontSetBitmap(vgl_RasFont *p,Vint ichar,
vgl_Bitmap *bitmap);VKI_EXTERN void vgl_RasFontSetSpacing(vgl_RasFont
*p,Vint spacing);VKI_EXTERN void vgl_RasFontSetSize(vgl_RasFont
*p,Vint size);VKI_EXTERN void vgl_RasFontSetCharWidth(vgl_RasFont
*p,Vint ichar,Vint width);VKI_EXTERN void vgl_RasFontSetPosition(vgl_RasFont
*p,Vint pos);VKI_EXTERN void vgl_RasFontGetPosition(vgl_RasFont
*p,Vint *posx,Vint *posy);VKI_EXTERN void vgl_RasFontGetMetrics(vgl_RasFont
*p,Vint *width,Vint *height,Vint *offset,Vint *base);
VKI_EXTERN void vgl_RasFontGetBitmap(vgl_RasFont *p,
Vint ichar,vgl_Bitmap **bitmap);VKI_EXTERN void vgl_RasFontGetSpacing(vgl_RasFont
*p,Vint *spacing);VKI_EXTERN void vgl_RasFontGetSize(vgl_RasFont
*p,Vint *size);VKI_EXTERN void vgl_RasFontGetCharWidth(vgl_RasFont
*p,Vint ichar,Vint *width);VKI_EXTERN void vgl_RasFontSetUserData(vgl_RasFont
*p,Vint index,Vint userdata);VKI_EXTERN void vgl_RasFontGetUserData(vgl_RasFont
*p,Vint index,Vint *userdata);VKI_EXTERN void vgl_RasFontSetPixelSize(vgl_RasFont
*p,Vfloat pixelsize);VKI_EXTERN void vgl_RasFontSetClampSize(vgl_RasFont
*p,Vfloat minsize,Vfloat maxsize);VKI_EXTERN void vgl_RasFontSetPlane(vgl_RasFont
*p,Vfloat path[3],Vfloat plane[3]);VKI_EXTERN void vgl_RasFontLength(vgl_RasFont
*p,Vtchar *text,Vint *dc);VKI_EXTERN void vgl_RasFontTexture(vgl_RasFont
*p,vgl_Texture *texture);VKI_EXTERN void vgl_RasFontDrawPlane(vgl_RasFont
*p,Vfloat x[3],Vint dc[3],Vtchar *text);VKI_EXTERN void
vgl_RasFontDraw(vgl_RasFont *p,Vfloat x[3],Vint dc[3],
Vtchar *text);VKI_EXTERN void vgl_RasFontCopy(vgl_RasFont
*p,vgl_RasFont *fromp);VKI_EXTERN void vgl_RasFontPrint(vgl_RasFont
*p);
#ifdef __cplusplus
}
#endif
#endif

