                   /* vgljpeg header */ 
#include <stdio.h>

#ifndef VGLJPEG_DEF
#define VGLJPEG_DEF

extern void
vgl_jpegWrite(FILE *fileptr, unsigned int *fbufi, int vd[2], int quality,
              int grayflag);
extern void
vgl_jpegRead1(FILE *fileptr, void **pcinfo, void **pjerr,
              int *width, int *height);
extern void
vgl_jpegRead2(FILE *fileptr, unsigned int *fbufi, int vd[2], void *pcinfo,
              void *pjerr);

#endif
