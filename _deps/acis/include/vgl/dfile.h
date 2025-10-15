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
 
#ifndef DFILE_DEF
#define DFILE_DEF
#include <stdio.h>
#include <math.h>
#include "base/basedefs.h"
#include "vgl/vgldefs.h"
#include "vgl/bitmap.h"
#include "vgl/zbuffer.h"
#include "vgl/fbuffer.h"
#include "vgl/drawfun.h"
#include "vgl/xfm.h"
#include "vgl/xfmstack.h"
#include "vgl/attstack.h"
#include "vgl/rasfont.h"
#include "vgl/dispfun.h"
#include "vgl/texture.h"
#include "vgl/vglweb3d.h"
#define DFILE_UNITPRECISION 0
#define DFILE_UNITPRECISION_DEFAULT 0
#define DFILE_UNITPRECISION_SINGLE9 1
#define DFILE_UNITPRECISION_SINGLE7 2
#define DFILE_NATIVEVERSION 1
#define DFILE_CLOSE -2
#define DFILE_UNKNOWN -1
#define DFILE_ASCII 0
#define DFILE_BINARY 1
#define DFILE_VRML 4
#define DFILE_APIPRINT 5
#define DFILE_STL_ASCII 6
#define DFILE_JTPART 7
#define DFILE_JT 8
#define DFILE_OBJ 10
#define DFILE_X3D 11
#define DFILE_DAE 12
struct vgl_DFile { Vint ierr;Vint filetype;Vint filevers;
Vint uservers;Vchar filename[256];Vchar assemblyname[256];
FILE *fd;Vint byteswap;vgl_DrawFun *df;Vint funfrequency;
Vfunc2 *fun;Vobject *funobject;long setpos;long curpos;
Vint unitprecision;Vint wxpix,wypix;void* jt;vsy_HashTable
*rasfonthash;vsy_HashTable *texturehash;vsy_HashTable
*bitmaphash;Vint abortflag;Vint mpts;Vfloat (*xx)[3];
void *cx;void *vx;Vfloat *tx;Vfloat *dx;void *px;Vint
fpts;Vfloat (*c3f)[3];Vfloat *vtf;Vint mprims;Vint *di;
vgl_DrawFun *drawfun;vgl_Attstack *attstack;vgl_Web3D
*web3d;float transp;int pointsize;int linestyle;int
linewidth;float intensity,shininess;float forecrgb[4];
int pxmn,pxmx,pymn,pymx;float fzmn,fzmx;
#ifdef __cplusplus
public: VKI_EXTERN vgl_DFile(void);VKI_EXTERN void*
operator new (size_t);VKI_EXTERN ~vgl_DFile(void);VKI_EXTERN
void operator delete (void*);VKI_EXTERN Vint Error();
VKI_EXTERN void SetObject(Vint,Vobject*);VKI_EXTERN
void SetFunction(Vint,VDFilefunc*,Vobject*);VKI_EXTERN
void Abort();VKI_EXTERN void SetParami(Vint,Vint);VKI_EXTERN
void SetPosition(long);VKI_EXTERN void CurPosition(long*);
VKI_EXTERN void SetWindowSize(Vint,Vint);VKI_EXTERN
void GetWindowSize(Vint*,Vint*);VKI_EXTERN void Open();
VKI_EXTERN void Close();VKI_EXTERN void Read();VKI_EXTERN
void ReadFileType(Vint*);VKI_EXTERN void SetFileType(Vint);
VKI_EXTERN void SetFileName(Vchar*);VKI_EXTERN void
SetJTAssemblyName(Vchar*);VKI_EXTERN void GetFileType(Vint*);
VKI_EXTERN void GetFileName(Vchar[]);VKI_EXTERN void
OpenJTPart(Vobject*);VKI_EXTERN void CloseJTPart();
VKI_EXTERN void PolyPointBuffer(Vint,Vint,Vint,Vint,Vint,Vint,Vint);
VKI_EXTERN void PolyLineBuffer(Vint,Vint,Vint,Vint,Vint,Vint,Vint,Vint);
VKI_EXTERN void PolygonBuffer(Vint,Vint,Vint,Vint,Vint,Vint,Vint,Vint);
VKI_EXTERN void DrawFun(vgl_DrawFun*);
#endif
};
#ifdef __cplusplus
extern "C" { 
#endif
VKI_EXTERN vgl_DFile* vgl_DFileBegin(void);VKI_EXTERN
void vgl_DFile_Construct(vgl_DFile *p);VKI_EXTERN void
vgl_DFileEnd(vgl_DFile *p);VKI_EXTERN void vgl_DFile_Destruct(vgl_DFile
*p);VKI_EXTERN Vint vgl_DFileError(vgl_DFile *p);VKI_EXTERN
void vgl_DFileSetObject(vgl_DFile *p,Vint objecttype,
Vobject *object);VKI_EXTERN void vgl_DFileSetFunction(vgl_DFile
*p,Vint frequency,VDFilefunc *function,Vobject *object);
VKI_EXTERN void vgl_DFileAbort(vgl_DFile *p);VKI_EXTERN
void vgl_DFileSetParami(vgl_DFile *p,Vint type,Vint
iparam);VKI_EXTERN void vgl_DFileSetPosition(vgl_DFile
*p,long pos);VKI_EXTERN void vgl_DFileCurPosition(vgl_DFile
*p,long *pos);VKI_EXTERN void vgl_DFileSetWindowSize(vgl_DFile
*p,Vint wxpix,Vint wypix);VKI_EXTERN void vgl_DFileGetWindowSize(vgl_DFile
*p,Vint *wxpix,Vint *wypix);VKI_EXTERN void vgl_DFileOpen(vgl_DFile
*p);VKI_EXTERN void vgl_DFileClose(vgl_DFile *p);VKI_EXTERN
void vgl_DFileRead(vgl_DFile *p);VKI_EXTERN void vgl_DFileReadFileType(vgl_DFile
*p,Vint *filetype);VKI_EXTERN void vgl_DFileSetFileType(vgl_DFile
*p,Vint filetype);VKI_EXTERN void vgl_DFileSetFileName(vgl_DFile
*p,Vchar *filename);VKI_EXTERN void vgl_DFileSetJTAssemblyName(vgl_DFile
*p,Vchar *name);VKI_EXTERN void vgl_DFileGetFileType(vgl_DFile
*p,Vint *filetype);VKI_EXTERN void vgl_DFileGetFileName(vgl_DFile
*p,Vchar filename[]);VKI_EXTERN void vgl_DFileOpenJTPart(vgl_DFile
*p,Vobject *part);VKI_EXTERN void vgl_DFileCloseJTPart(vgl_DFile
*p);VKI_EXTERN void vgl_DFilePolyPointBuffer(vgl_DFile
*p,Vint vboid,Vint offset,Vint npts,Vint cflag,Vint
vflag,Vint tflag,Vint dflag);VKI_EXTERN void vgl_DFilePolyLineBuffer(vgl_DFile
*p,Vint vboid,Vint offset,Vint type,Vint npts,Vint cflag,
Vint vflag,Vint tflag,Vint dflag);VKI_EXTERN void vgl_DFilePolygonBuffer(vgl_DFile
*p,Vint vboid,Vint offset,Vint type,Vint npts,Vint cflag,
Vint vflag,Vint tflag,Vint dflag);VKI_EXTERN void vgl_DFileDrawFun(vgl_DFile
*p,vgl_DrawFun *df);
#ifdef __cplusplus
}
#endif
#endif

