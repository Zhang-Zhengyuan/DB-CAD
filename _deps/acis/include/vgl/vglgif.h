                   /* local functions header file */
#ifndef VGLGIF_DEF
#define VGLGIF_DEF

#include <stdio.h>

#ifdef  __cplusplus
extern "C" {
#endif

#define GIFWRITER_INTERLACE   1
#define GIFWRITER_DELAYTIME   2
#define GIFWRITER_LOOPS       3
#define GIFWRITER_COMPRESS    4
#define GIFWRITER_COLORMAP    5
#define GIFWRITER_COLORMATCH  6

extern int
vgl_GIFFileToMem (FILE*, unsigned int*);
extern int
vgl_GIFMemToFile (FILE*, unsigned int*, int, int, int);
extern int
vgl_GIFSizeOfImage (FILE*, int*, int*, int *);

typedef struct vgl_GIFWriter vgl_GIFWriter;

VKI_EXTERN vgl_GIFWriter*
vgl_GIFWriterBegin(void);
VKI_EXTERN void
vgl_GIFWriterEnd(vgl_GIFWriter *gifwriter);
VKI_EXTERN void
vgl_GIFWriterSetParami(vgl_GIFWriter *gifwriter, Vint type, Vint iparam);
VKI_EXTERN void
vgl_GIFWriterInitFile(vgl_GIFWriter *gifwriter, unsigned int *buffer,
                      int xsize, int ysize, int bwflag, FILE *fp);
VKI_EXTERN void
vgl_GIFWriterAddImage(vgl_GIFWriter *gifwriter, unsigned int *buffer,
                          int xsize, int ysize, FILE *fp);
VKI_EXTERN void
vgl_GIFWriterAppendImage(vgl_GIFWriter *gifwriter, unsigned int *buffer,
                          int xsize, int ysize, FILE *fp);

#ifdef  __cplusplus
}
#endif

#endif
