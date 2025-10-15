                   /* local functions header file */
#ifndef VGLTARGA_DEF
#define VGLTARGA_DEF

#include <stdio.h>

#ifdef  __cplusplus
extern "C" {
#endif

extern int
vgl_targaFileToMem(FILE *,unsigned int *);
extern int
vgl_targaSizeOfImage(FILE *,int *,int *);
extern void
vgl_targaMemToFile(FILE *,unsigned int *, int, int);

#ifdef  __cplusplus
}
#endif

#endif
