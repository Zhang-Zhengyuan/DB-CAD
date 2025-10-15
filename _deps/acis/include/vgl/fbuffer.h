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
 
#ifndef FBUFFER_DEF
#define FBUFFER_DEF
#include "base/basedefs.h"
#include "vgl/dispfun.h"
#include "vgl/texture.h"
#ifdef VKI_WIND_WIN32
#include <windows.h>
#ifdef _MSC_VER
#include <Vfw.h>
#endif
#endif
#define FBUFFER_INTERLACE 0
#define FBUFFER_DELAYSECONDS 1
#define FBUFFER_LOOPS 2
#define FBUFFER_LOOPSFLAG 3
#define FBUFFER_GRAYSCALE 4
#define FBUFFER_AVICOMPFACT 5
#define FBUFFER_AVICOMPTYPE 6
#define FBUFFER_AVICOMP_NONE 0
#define FBUFFER_AVICOMP_INTELINDEO 1
#define FBUFFER_AVICOMP_CINEPAK 2
#define FBUFFER_AVICOMP_MSVIDEO 3
#define FBUFFER_QUALITY 7
#define FBUFFER_BLENDTRANS 8
#define FBUFFER_GIFCOMPTYPE 9
#define FBUFFER_GIFCOMP_RLE 1
#define FBUFFER_GIFCOMP_LZW 2
#define FBUFFER_GIFCMAPTYPE 10
#define FBUFFER_GIFCMAP_DITHER 0
#define FBUFFER_GIFCMAP_332RGB 1
#define FBUFFER_GIFCMAPMATCH 11
#define FBUFFER_TIFFCOMPTYPE 12
#define FBUFFER_TIFFCOMP_NONE 0
#define FBUFFER_TIFFCOMP_LZW 1
#define FBUFFER_TIFFCOMP_DEFLATE 2
#define FBUFFER_TIFFCOMP_NATIVE 3
#define FBUFFER_FRAMERATE 13
#define FBUFFER_BITRATE 14
#define FBUFFER_FFMPEG_MP4 1
#define FBUFFER_FFMPEG_MOV 2
#define FBUFFER_FFMPEG_CODEC_MPEG4 1
#define FBUFFER_FFMPEG_CODEC_H264 2
#define FBUFFER_FFMPEG_CODEC_PNG 3
#define FBUFFER_FFMPEG_CODEC_QTRLE 4
struct vgl_FBuffer { Vint ierr;Vint id;int vd[2];Vint
npix;int endian;Vuint rgba;Vfloat color[4];Vuint *fbufi;
vgl_Texture *texture;vgl_DispFun *df;Vint blendtransflag;
Vint interlaceflag;Vfloat delay;Vint loops;Vint loopsflag;
Vint grayflag;Vint quality;Vint avicomptype;Vint avicompfact;
Vint gifcomptype;Vint tiffcomptype;Vint gifcmaptype;
Vint gifcmapmatch;Vfloat framerate;void *oc;void *video_st;
Vint ffmpeginit;Vint ffmpegwidth;Vint ffmpegheight;
Vint ffmpegtype;Vint ffmpegcodec;Vint nflush;Vint bitrate;
#ifdef VKI_WIND_WIN32
#ifdef _MSC_VER
AVISTREAMINFO aviinfo;PAVISTREAM avistream;PAVISTREAM
avicompressedstream;AVICOMPRESSOPTIONS avicompopts;
LONG aviindex;BITMAPINFO aviformat;int avistatus;
#endif
#endif
#ifdef __cplusplus
public: VKI_EXTERN vgl_FBuffer(void);VKI_EXTERN void*
operator new (size_t);VKI_EXTERN ~vgl_FBuffer(void);
VKI_EXTERN void operator delete (void*);VKI_EXTERN Vint
Error();VKI_EXTERN void Def(Vint,Vint);VKI_EXTERN void
Inq(Vint*,Vint*);VKI_EXTERN void Clear(Vint,Vint,Vint,Vint);
VKI_EXTERN void Color(Vfloat[3]);VKI_EXTERN void ColorIndex(Vint);
VKI_EXTERN void Trans(Vfloat);VKI_EXTERN void Data(Vfloat);
VKI_EXTERN void SetTexture(vgl_Texture*);VKI_EXTERN
void SetObject(Vint,Vobject*);VKI_EXTERN void SetParami(Vint,Vint);
VKI_EXTERN void SetParamf(Vint,Vfloat);VKI_EXTERN void
SetPixel(Vint,Vint,Vuint);VKI_EXTERN void GetPixel(Vint,Vint,Vuint*);
VKI_EXTERN void GetFBufPtr(Vuint**);VKI_EXTERN void
SetColor(Vint,Vint,Vfloat[3]);VKI_EXTERN void GetColor(Vint,Vint,Vfloat[3]);
VKI_EXTERN void SetColorAlpha(Vint,Vint,Vfloat[4]);
VKI_EXTERN void GetColorAlpha(Vint,Vint,Vfloat[4]);
VKI_EXTERN void ReverseBW();VKI_EXTERN void GreyScale();
VKI_EXTERN void BrightContrast(Vfloat,Vfloat);VKI_EXTERN
void GammaWarp(Vfloat);VKI_EXTERN void Alpha(Vfloat);
VKI_EXTERN void Border(Vint);VKI_EXTERN void Cut(Vint,Vint,vgl_FBuffer*);
VKI_EXTERN void Blend(Vfloat,vgl_FBuffer*);VKI_EXTERN
void Paste(Vint,Vint,vgl_FBuffer*);VKI_EXTERN void Scale(Vint,vgl_FBuffer*);
VKI_EXTERN void Zoom(vgl_FBuffer*);VKI_EXTERN void Rotate(vgl_FBuffer*);
VKI_EXTERN void ReadTarga(Vchar*);VKI_EXTERN void WriteTarga(Vchar*);
VKI_EXTERN void ReadBMP(Vchar*);VKI_EXTERN void WriteBMP(Vchar*);
VKI_EXTERN void WriteSVG(Vchar*);
#ifdef VKI_WIND_WIN32
VKI_EXTERN void ReadWINDIB(BITMAPINFO*,LPVOID);
#endif
#ifdef VKI_WIND_WIN32
VKI_EXTERN void WriteWINDIB(BITMAPINFO*,LPVOID);
#endif
#if defined(VKI_WIND_WIN32)&& defined(_MSC_VER)
VKI_EXTERN void WriteWINAVI(PAVIFILE*);VKI_EXTERN void
CloseWINAVI();
#endif
VKI_EXTERN void ReadSGI(Vchar*);VKI_EXTERN void WriteSGI(Vchar*);
VKI_EXTERN void WriteJPEG(Vchar*);VKI_EXTERN void ReadJPEG(Vchar*);
VKI_EXTERN void ReadGIF(Vchar*);VKI_EXTERN void WriteGIF(Vchar*);
VKI_EXTERN void AppendGIF(Vchar*);VKI_EXTERN void WriteTIFF(Vchar*);
VKI_EXTERN void ReadTIFF(Vchar*);VKI_EXTERN void WritePict(Vchar*);
VKI_EXTERN void ReadPNG(Vchar*);VKI_EXTERN void WritePNG(Vchar*);
VKI_EXTERN void Copy(vgl_FBuffer*);VKI_EXTERN void Print();
VKI_EXTERN void Point(Vint,Vint);VKI_EXTERN void PointColor(Vint,Vint,Vfloat[4]);
VKI_EXTERN void PointData(Vint,Vint,Vint,Vfloat*);VKI_EXTERN
void Line(Vint,Vint,Vint,Vint);VKI_EXTERN void Scan(Vint,Vint,Vint);
VKI_EXTERN void ScanColor(Vint,Vfloat[4],Vint,Vfloat[4],Vint);
VKI_EXTERN void ScanData(Vint,Vint,Vfloat*,Vint,Vfloat*,Vint);
VKI_EXTERN void AlphaTest(Vint,Vint,Vint,Vfloat*,Vint*);
VKI_EXTERN void ScanDataBuf(Vint,Vfloat,Vint,Vfloat,Vint);
VKI_EXTERN void DispFun(vgl_DispFun*);VKI_EXTERN void
OpenFFMPEG(Vint,Vint,Vchar[]);VKI_EXTERN void WriteFFMPEG();
VKI_EXTERN void CloseFFMPEG();
#endif
};
#ifdef __cplusplus
extern "C" { 
#endif
VKI_EXTERN vgl_FBuffer* vgl_FBufferBegin(void);VKI_EXTERN
void vgl_FBuffer_Construct(vgl_FBuffer *p);VKI_EXTERN
void vgl_FBufferEnd(vgl_FBuffer *p);VKI_EXTERN void
vgl_FBuffer_Destruct(vgl_FBuffer *p);VKI_EXTERN Vint
vgl_FBufferError(vgl_FBuffer *p);VKI_EXTERN void vgl_FBufferDef(vgl_FBuffer
*p,Vint xsize,Vint ysize);VKI_EXTERN void vgl_FBufferInq(vgl_FBuffer
*p,Vint *xsize,Vint *ysize);VKI_EXTERN void vgl_FBufferClear(vgl_FBuffer
*p,Vint left,Vint right,Vint bot,Vint top);VKI_EXTERN
void vgl_FBufferColor(vgl_FBuffer *p,Vfloat c[3]);VKI_EXTERN
void vgl_FBufferColorIndex(vgl_FBuffer *p,Vint index);
VKI_EXTERN void vgl_FBufferTrans(vgl_FBuffer *p,Vfloat
t);VKI_EXTERN void vgl_FBufferData(vgl_FBuffer *p,Vfloat
data);VKI_EXTERN void vgl_FBufferSetTexture(vgl_FBuffer
*p,vgl_Texture *texture);VKI_EXTERN void vgl_FBufferSetObject(vgl_FBuffer
*p,Vint objecttype,Vobject *object);VKI_EXTERN void
vgl_FBufferSetParami(vgl_FBuffer *p,Vint type,Vint iparam);
VKI_EXTERN void vgl_FBufferSetParamf(vgl_FBuffer *p,
Vint type,Vfloat fparam);VKI_EXTERN void vgl_FBufferSetPixel(vgl_FBuffer
*p,Vint ix,Vint iy,Vuint pixel);VKI_EXTERN void vgl_FBufferGetPixel(vgl_FBuffer
*p,Vint ix,Vint iy,Vuint *pixel);VKI_EXTERN void vgl_FBufferGetFBufPtr(vgl_FBuffer
*p,Vuint **fbufptr);VKI_EXTERN void vgl_FBufferSetColor(vgl_FBuffer
*p,Vint dx,Vint dy,Vfloat c[3]);VKI_EXTERN void vgl_FBufferGetColor(vgl_FBuffer
*p,Vint dx,Vint dy,Vfloat c[3]);VKI_EXTERN void vgl_FBufferSetColorAlpha(vgl_FBuffer
*p,Vint dx,Vint dy,Vfloat c[4]);VKI_EXTERN void vgl_FBufferGetColorAlpha(vgl_FBuffer
*p,Vint dx,Vint dy,Vfloat c[4]);VKI_EXTERN void vgl_FBufferReverseBW(vgl_FBuffer
*p);VKI_EXTERN void vgl_FBufferGreyScale(vgl_FBuffer
*p);VKI_EXTERN void vgl_FBufferBrightContrast(vgl_FBuffer
*p,Vfloat bright,Vfloat contrast);VKI_EXTERN void vgl_FBufferGammaWarp(vgl_FBuffer
*p,Vfloat gamma);VKI_EXTERN void vgl_FBufferAlpha(vgl_FBuffer
*p,Vfloat alpha);VKI_EXTERN void vgl_FBufferBorder(vgl_FBuffer
*p,Vint borderwidth);VKI_EXTERN void vgl_FBufferCut(vgl_FBuffer
*p,Vint xorig,Vint yorig,vgl_FBuffer *cutp);VKI_EXTERN
void vgl_FBufferBlend(vgl_FBuffer *p,Vfloat fac,vgl_FBuffer
*blendp);VKI_EXTERN void vgl_FBufferPaste(vgl_FBuffer
*p,Vint xorig,Vint yorig,vgl_FBuffer *pastep);VKI_EXTERN
void vgl_FBufferScale(vgl_FBuffer *p,Vint f,vgl_FBuffer
*scalep);VKI_EXTERN void vgl_FBufferZoom(vgl_FBuffer
*p,vgl_FBuffer *zoomp);VKI_EXTERN void vgl_FBufferRotate(vgl_FBuffer
*p,vgl_FBuffer *rotatep);VKI_EXTERN void vgl_FBufferReadTarga(vgl_FBuffer
*p,Vchar *pathname);VKI_EXTERN void vgl_FBufferWriteTarga(vgl_FBuffer
*p,Vchar *pathname);VKI_EXTERN void vgl_FBufferReadBMP(vgl_FBuffer
*p,Vchar *pathname);VKI_EXTERN void vgl_FBufferWriteBMP(vgl_FBuffer
*p,Vchar *pathname);VKI_EXTERN void vgl_FBufferWriteSVG(vgl_FBuffer
*p,Vchar *pathname);
#ifdef VKI_WIND_WIN32
VKI_EXTERN void vgl_FBufferReadWINDIB(vgl_FBuffer *p,
BITMAPINFO *binfo,LPVOID bdata);
#endif
#ifdef VKI_WIND_WIN32
VKI_EXTERN void vgl_FBufferWriteWINDIB(vgl_FBuffer *p,
BITMAPINFO *binfo,LPVOID bdata);
#endif
#if defined(VKI_WIND_WIN32)&& defined(_MSC_VER)
VKI_EXTERN void vgl_FBufferWriteWINAVI(vgl_FBuffer *p,
PAVIFILE *avifile);VKI_EXTERN void vgl_FBufferCloseWINAVI(vgl_FBuffer
*p);
#endif
VKI_EXTERN void vgl_FBufferReadSGI(vgl_FBuffer *p,Vchar
*pathname);VKI_EXTERN void vgl_FBufferWriteSGI(vgl_FBuffer
*p,Vchar *pathname);VKI_EXTERN void vgl_FBufferWriteJPEG(vgl_FBuffer
*p,Vchar *pathname);VKI_EXTERN void vgl_FBufferReadJPEG(vgl_FBuffer
*p,Vchar *pathname);VKI_EXTERN void vgl_FBufferReadGIF(vgl_FBuffer
*p,Vchar *pathname);VKI_EXTERN void vgl_FBufferWriteGIF(vgl_FBuffer
*p,Vchar *pathname);VKI_EXTERN void vgl_FBufferAppendGIF(vgl_FBuffer
*p,Vchar *pathname);VKI_EXTERN void vgl_FBufferWriteTIFF(vgl_FBuffer
*p,Vchar *pathname);VKI_EXTERN void vgl_FBufferReadTIFF(vgl_FBuffer
*p,Vchar *pathname);VKI_EXTERN void vgl_FBufferWritePict(vgl_FBuffer
*p,Vchar *pathname);VKI_EXTERN void vgl_FBufferReadPNG(vgl_FBuffer
*p,Vchar *pathname);VKI_EXTERN void vgl_FBufferWritePNG(vgl_FBuffer
*p,Vchar *pathname);VKI_EXTERN void vgl_FBufferCopy(vgl_FBuffer
*p,vgl_FBuffer *fromp);VKI_EXTERN void vgl_FBufferPrint(vgl_FBuffer
*p);VKI_EXTERN void vgl_FBufferPoint(vgl_FBuffer *p,
Vint dx,Vint dy);VKI_EXTERN void vgl_FBufferPointColor(vgl_FBuffer
*p,Vint dx,Vint dy,Vfloat c[4]);VKI_EXTERN void vgl_FBufferPointData(vgl_FBuffer
*p,Vint nrws,Vint dx,Vint dy,Vfloat *d);VKI_EXTERN void
vgl_FBufferLine(vgl_FBuffer *p,Vint dx1,Vint dy1,Vint
dx2,Vint dy2);VKI_EXTERN void vgl_FBufferScan(vgl_FBuffer
*p,Vint dx1,Vint dx2,Vint dy);VKI_EXTERN void vgl_FBufferScanColor(vgl_FBuffer
*p,Vint dx1,Vfloat c1[4],Vint dx2,Vfloat c2[4],Vint
dy);VKI_EXTERN void vgl_FBufferScanData(vgl_FBuffer
*p,Vint nrws,Vint x1,Vfloat *d1,Vint x2,Vfloat *d2,
Vint y);VKI_EXTERN void vgl_FBufferAlphaTest(vgl_FBuffer
*p,Vint nrws,Vint x,Vint y,Vfloat *d,Vint *ivis);VKI_EXTERN
void vgl_FBufferScanDataBuf(vgl_FBuffer *p,Vint dx1,
Vfloat d1,Vint dx2,Vfloat d2,Vint dy);VKI_EXTERN void
vgl_FBufferDispFun(vgl_FBuffer *p,vgl_DispFun *df);
VKI_EXTERN void vgl_FBufferOpenFFMPEG(vgl_FBuffer *p,
Vint codectype,Vint filetype,Vchar filename[]);VKI_EXTERN
void vgl_FBufferWriteFFMPEG(vgl_FBuffer *p);VKI_EXTERN
void vgl_FBufferCloseFFMPEG(vgl_FBuffer *p);
#ifdef __cplusplus
}
#endif
#endif

