                   /* local functions header file */
#ifndef VGLBMP_DEF
#define VGLBMP_DEF

#include <stdio.h>

#ifdef  __cplusplus
extern "C" {
#endif

extern int
vgl_bmpFileToMem (FILE*, unsigned int*);
extern int
vgl_bmpSizeOfImage (FILE*, int*, int*);
extern void
vgl_bmpMemToFile (FILE*, unsigned int*, int, int, int);
extern void
vgl_bmpMemToMem (unsigned int*, int, int, int, unsigned char**, int*);

#ifdef  __cplusplus
}
#endif

#endif
