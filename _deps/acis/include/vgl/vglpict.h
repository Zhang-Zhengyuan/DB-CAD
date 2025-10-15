                   /* local functions header file */
#ifndef VGLPICT_DEF
#define VGLPICT_DEF

#include <stdio.h>

#ifdef  __cplusplus
extern "C" {
#endif

extern int
vgl_pictMemToFile (FILE *fp,
                   unsigned int *lbuffer, int xsize, int ysize, int zsize);

#ifdef  __cplusplus
}
#endif

#endif
