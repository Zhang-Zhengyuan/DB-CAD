                   /* local functions header file */
#ifndef VGLSGIRGB_DEF
#define VGLSGIRGB_DEF

#include <stdio.h>

#ifdef  __cplusplus
extern "C" {
#endif

extern int
vgl_SGIRGBFileToMem(FILE*, unsigned int*);
extern int
vgl_SGIRGBMemToFile(FILE*, unsigned int*, int, int ,int);
extern int
vgl_SGIRGBSizeOfImage(FILE*, int*, int*, int*);

#ifdef  __cplusplus
}
#endif

#endif
