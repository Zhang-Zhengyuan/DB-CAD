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
 
#ifndef PIXELMAP_DEF
#define PIXELMAP_DEF
#ifdef VKI_WIND_X11
#include <X11/Xlib.h>
#endif
#ifdef VKI_WIND_WIN32
#include <windows.h>
#endif
#ifdef VKI_3DAPI_OPENGL
#ifdef VKI_WIND_X11
#ifdef VKI_LIBAPI_GLEW
#define GL_GLEXT_PROTOTYPES
#include <GL/glew.h>
#else
#define GL_GLEXT_PROTOTYPES
#include <GL/gl.h>
#endif
#endif
#ifdef VKI_WIND_WIN32
#ifdef VKI_LIBAPI_GLEW
#include <GL/glew.h>
#else
#include <GL/gl.h>
#endif
#endif
#endif
#include "base/basedefs.h"
#include "vgl/fbuffer.h"
#define PIXELMAP_FBUFFER 1
#define PIXELMAP_ZBUFFER 2
struct vgl_Pixelmap { Vint ierr;Vint type;Vint xorig,
yorig;Vint xsize,ysize;Vint buffer;
#ifdef VKI_WIND_X11
Pixmap pixmap;
#endif
#ifdef VKI_WIND_WIN32
HDC cdc;HBITMAP cbm;
#endif
#ifdef VKI_3DAPI_OPENGL
GLvoid *oglparray;
#endif
Vfloat *d3darray;Vdouble *zarray;vgl_FBuffer *fbuffer;
#ifdef __cplusplus
public: VKI_EXTERN vgl_Pixelmap(void);VKI_EXTERN void*
operator new (size_t);VKI_EXTERN ~vgl_Pixelmap(void);
VKI_EXTERN void operator delete (void*);VKI_EXTERN Vint
Error();VKI_EXTERN void SetBuffer(Vint);VKI_EXTERN void
GetBuffer(Vint*);VKI_EXTERN void SetSize(Vint,Vint);
VKI_EXTERN void GetSize(Vint*,Vint*);VKI_EXTERN void
SetOrigin(Vint,Vint);VKI_EXTERN void GetOrigin(Vint*,Vint*);
VKI_EXTERN void Unset();
#ifdef VKI_WIND_X11
VKI_EXTERN void SetXPixmap(Pixmap);VKI_EXTERN void GetXPixmap(Pixmap*);
#endif
#ifdef VKI_3DAPI_OPENGL
VKI_EXTERN void SetOpenGLparray(GLvoid*);VKI_EXTERN
void GetOpenGLparray(GLvoid**);
#endif
VKI_EXTERN void SetDirect3Darray(Vfloat*);VKI_EXTERN
void GetDirect3Darray(Vfloat**);
#ifdef VKI_WIND_WIN32
VKI_EXTERN void SetGDI(HDC,HBITMAP);VKI_EXTERN void
GetGDI(HDC*,HBITMAP*);
#endif
VKI_EXTERN void SetFBuffer(vgl_FBuffer*);VKI_EXTERN
void GetFBuffer(vgl_FBuffer**);VKI_EXTERN void SetZarray(Vdouble*);
VKI_EXTERN void GetZarray(Vdouble**);
#endif
};
#ifdef __cplusplus
extern "C" { 
#endif
VKI_EXTERN vgl_Pixelmap* vgl_PixelmapBegin(void);VKI_EXTERN
void vgl_Pixelmap_Construct(vgl_Pixelmap *p);VKI_EXTERN
void vgl_PixelmapEnd(vgl_Pixelmap *p);VKI_EXTERN void
vgl_Pixelmap_Destruct(vgl_Pixelmap *p);VKI_EXTERN Vint
vgl_PixelmapError(vgl_Pixelmap *p);VKI_EXTERN void vgl_PixelmapSetBuffer(vgl_Pixelmap
*p,Vint buffer);VKI_EXTERN void vgl_PixelmapGetBuffer(vgl_Pixelmap
*p,Vint *buffer);VKI_EXTERN void vgl_PixelmapSetSize(vgl_Pixelmap
*p,Vint xsize,Vint ysize);VKI_EXTERN void vgl_PixelmapGetSize(vgl_Pixelmap
*p,Vint *xsize,Vint *ysize);VKI_EXTERN void vgl_PixelmapSetOrigin(vgl_Pixelmap
*p,Vint xorig,Vint yorig);VKI_EXTERN void vgl_PixelmapGetOrigin(vgl_Pixelmap
*p,Vint *xorig,Vint *yorig);VKI_EXTERN void vgl_PixelmapUnset(vgl_Pixelmap
*p);
#ifdef VKI_WIND_X11
VKI_EXTERN void vgl_PixelmapSetXPixmap(vgl_Pixelmap
*p,Pixmap pixmap);VKI_EXTERN void vgl_PixelmapGetXPixmap(vgl_Pixelmap
*p,Pixmap *pixmap);
#endif
#ifdef VKI_3DAPI_OPENGL
VKI_EXTERN void vgl_PixelmapSetOpenGLparray(vgl_Pixelmap
*p,GLvoid *parray);VKI_EXTERN void vgl_PixelmapGetOpenGLparray(vgl_Pixelmap
*p,GLvoid **parray);
#endif
VKI_EXTERN void vgl_PixelmapSetDirect3Darray(vgl_Pixelmap
*p,Vfloat *array);VKI_EXTERN void vgl_PixelmapGetDirect3Darray(vgl_Pixelmap
*p,Vfloat **array);
#ifdef VKI_WIND_WIN32
VKI_EXTERN void vgl_PixelmapSetGDI(vgl_Pixelmap *p,
HDC cdc,HBITMAP cbm);VKI_EXTERN void vgl_PixelmapGetGDI(vgl_Pixelmap
*p,HDC *cdc,HBITMAP *cbm);
#endif
VKI_EXTERN void vgl_PixelmapSetFBuffer(vgl_Pixelmap
*p,vgl_FBuffer *fbuffer);VKI_EXTERN void vgl_PixelmapGetFBuffer(vgl_Pixelmap
*p,vgl_FBuffer **fbuffer);VKI_EXTERN void vgl_PixelmapSetZarray(vgl_Pixelmap
*p,Vdouble *zarray);VKI_EXTERN void vgl_PixelmapGetZarray(vgl_Pixelmap
*p,Vdouble **zarray);
#ifdef __cplusplus
}
#endif
#endif

