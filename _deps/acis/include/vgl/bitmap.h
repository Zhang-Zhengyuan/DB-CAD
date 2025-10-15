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
 
#ifndef BITMAP_DEF
#define BITMAP_DEF
#include "base/basedefs.h"
#include "vgl/vgldefs.h"
#define BITMAP_SCREEN1 0
#define BITMAP_SCREEN2 1
#define BITMAP_SCREEN3 2
#define BITMAP_SCREEN4 3
#define BITMAP_SCREEN5 4
#define BITMAP_SCREEN6 5
#define BITMAP_SCREEN7 6
#define BITMAP_SCREEN8 7
#define BITMAP_DOT 10
#define BITMAP_DASH 11
#define BITMAP_DOTDASH 12
#define BITMAP_LDASH 13
#define BITMAP_DOTLDASH 14
#define BITMAP_DOTDOT 15
#define BITMAP_DOTDOTLDASH 16
#define BITMAP_LLDASH 17
#define BITMAP_XES5 20
#define BITMAP_XES7 21
#define BITMAP_CIRCLES5 22
#define BITMAP_CIRCLES7 23
#define BITMAP_DIAMONDS5 24
#define BITMAP_DIAMONDS7 25
#define BITMAP_BOXES5 26
#define BITMAP_BOXES7 27
#define BITMAP_SQUARE4 28
#define BITMAP_SQUARE8 29
#define BITMAP_SQUARE8B 30
#define BITMAP_SQUARE16B 31
#define BITMAP_CROSS8 32
#define BITMAP_CROSS8B 33
#define BITMAP_CROSS16 34
#define BITMAP_CROSS16B 35
#define BITMAP_BDIAGONAL8 36
#define BITMAP_BDIAGONAL8B 37
#define BITMAP_BDIAGONAL16 38
#define BITMAP_BDIAGONAL16B 39
#define BITMAP_FDIAGONAL8 40
#define BITMAP_FDIAGONAL8B 41
#define BITMAP_FDIAGONAL16 42
#define BITMAP_FDIAGONAL16B 43
#define BITMAP_HORIZONTAL4 44
#define BITMAP_HORIZONTAL8 45
#define BITMAP_HORIZONTAL8B 46
#define BITMAP_HORIZONTAL16B 47
#define BITMAP_VERTICAL4 48
#define BITMAP_VERTICAL8 49
#define BITMAP_VERTICAL8B 50
#define BITMAP_VERTICAL16B 51
struct vgl_Bitmap { Vint ierr;Vint width,height;Vint
propwidth;Vint bitwidth;Vint sizeofrast;Vuchar *rast;
Vint sizeofalph;Vuchar *alph;
#ifdef __cplusplus
public: VKI_EXTERN vgl_Bitmap(void);VKI_EXTERN void*
operator new (size_t);VKI_EXTERN ~vgl_Bitmap(void);
VKI_EXTERN void operator delete (void*);VKI_EXTERN Vint
Error();VKI_EXTERN void Load(Vint);VKI_EXTERN void Def(Vint,Vint);
VKI_EXTERN void Inq(Vint*,Vint*);VKI_EXTERN void Clear();
VKI_EXTERN void SetRast(Vuchar*);VKI_EXTERN void GetRast(Vuchar*);
VKI_EXTERN void GetRastSize(Vint*);VKI_EXTERN void GetByte(Vint,Vint,Vuchar*);
VKI_EXTERN void RepByte(Vint,Vint,Vuchar*);VKI_EXTERN
void GetRastPtr(Vuchar**);VKI_EXTERN void GetAlphPtr(Vuchar**);
VKI_EXTERN void SetBit(Vint,Vint,Vint);VKI_EXTERN void
GetBit(Vint,Vint,Vint*);VKI_EXTERN void SetAlpha(Vint,Vint,Vint);
VKI_EXTERN void GetAlpha(Vint,Vint,Vint*);VKI_EXTERN
void GetTrans(Vint,Vint,Vfloat*);VKI_EXTERN void SetPropWidth(Vint);
VKI_EXTERN void GetPropWidth(Vint*);VKI_EXTERN void
SetBitWidth(Vint);VKI_EXTERN void GetBitWidth(Vint*);
VKI_EXTERN void Copy(vgl_Bitmap*);VKI_EXTERN void Print();
VKI_EXTERN void WriteBMP(Vchar*);
#endif
};
#ifdef __cplusplus
extern "C" { 
#endif
VKI_EXTERN vgl_Bitmap* vgl_BitmapBegin(void);VKI_EXTERN
void vgl_Bitmap_Construct(vgl_Bitmap *p);VKI_EXTERN
void vgl_BitmapEnd(vgl_Bitmap *p);VKI_EXTERN void vgl_Bitmap_Destruct(vgl_Bitmap
*p);VKI_EXTERN Vint vgl_BitmapError(vgl_Bitmap *p);
VKI_EXTERN void vgl_BitmapLoad(vgl_Bitmap *p,Vint irast);
VKI_EXTERN void vgl_BitmapDef(vgl_Bitmap *p,Vint width,
Vint height);VKI_EXTERN void vgl_BitmapInq(vgl_Bitmap
*p,Vint *width,Vint *height);VKI_EXTERN void vgl_BitmapClear(vgl_Bitmap
*p);VKI_EXTERN void vgl_BitmapSetRast(vgl_Bitmap *p,
Vuchar *rast);VKI_EXTERN void vgl_BitmapGetRast(vgl_Bitmap
*p,Vuchar *rast);VKI_EXTERN void vgl_BitmapGetRastSize(vgl_Bitmap
*p,Vint *size);VKI_EXTERN void vgl_BitmapGetByte(vgl_Bitmap
*p,Vint bw,Vint bh,Vuchar *rast);VKI_EXTERN void vgl_BitmapRepByte(vgl_Bitmap
*p,Vint bw,Vint bh,Vuchar *rast);VKI_EXTERN void vgl_BitmapGetRastPtr(vgl_Bitmap
*p,Vuchar **rastptr);VKI_EXTERN void vgl_BitmapGetAlphPtr(vgl_Bitmap
*p,Vuchar **alphptr);VKI_EXTERN void vgl_BitmapSetBit(vgl_Bitmap
*p,Vint ix,Vint iy,Vint bit);VKI_EXTERN void vgl_BitmapGetBit(vgl_Bitmap
*p,Vint ix,Vint iy,Vint *bit);VKI_EXTERN void vgl_BitmapSetAlpha(vgl_Bitmap
*p,Vint ix,Vint iy,Vint alpha);VKI_EXTERN void vgl_BitmapGetAlpha(vgl_Bitmap
*p,Vint ix,Vint iy,Vint *alpha);VKI_EXTERN void vgl_BitmapGetTrans(vgl_Bitmap
*p,Vint ix,Vint iy,Vfloat *trans);VKI_EXTERN void vgl_BitmapSetPropWidth(vgl_Bitmap
*p,Vint propwidth);VKI_EXTERN void vgl_BitmapGetPropWidth(vgl_Bitmap
*p,Vint *propwidth);VKI_EXTERN void vgl_BitmapSetBitWidth(vgl_Bitmap
*p,Vint bitwidth);VKI_EXTERN void vgl_BitmapGetBitWidth(vgl_Bitmap
*p,Vint *bitwidth);VKI_EXTERN void vgl_BitmapCopy(vgl_Bitmap
*p,vgl_Bitmap *fromp);VKI_EXTERN void vgl_BitmapPrint(vgl_Bitmap
*p);VKI_EXTERN void vgl_BitmapWriteBMP(vgl_Bitmap *p,
Vchar *pathname);
#ifdef __cplusplus
}
#endif
#endif

