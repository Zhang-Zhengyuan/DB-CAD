                   /* FFMPEG utility functions header file */ 

#ifndef VGLFFMPEG_DEF
#define VGLFFMPEG_DEF

#ifdef  __cplusplus
extern "C" {
#endif

VKI_EXTERN void
vgl_ffmpegInit(void);
VKI_EXTERN Vint
vgl_ffmpegOpen(vgl_FBuffer *p, Vint codectype, Vint filetype, Vchar filename[]);
VKI_EXTERN void
vgl_ffmpegWrite(vgl_FBuffer *p, Vuchar yuv[]);
VKI_EXTERN void
vgl_ffmpegClose(vgl_FBuffer *p);

#ifdef  __cplusplus
}
#endif

#endif /* VGLFFMPEG_DEF */
