                   /* local functions header file */
#ifndef TRBALL_DEF
#define TRBALL_DEF

#include "base/basedefs.h"
#include "base/vututil.h"

typedef struct {
   float x, y, z, w;
} Quat;

enum QuatPart {
   X, Y, Z, W, QuatLen
};

typedef Quat HVect;
typedef float HMatrix[QuatLen][QuatLen];

typedef enum AxisSet {
   NoAxes,
   CameraAxes,
   BodyAxes,
   OtherAxes,
   NSets
} AxisSet;

typedef float *ConstraintSet;

typedef struct {
   HVect center;
   Vdouble radius;
   Quat qNow, qDown, qDrag;
   HVect vNow, vDown, vFrom, vTo, vrFrom, vrTo;
   HMatrix mNow, mDown, mId;
   Quat qSave;
   Vint dragging;
   ConstraintSet sets[NSets];
   int setSizes[NSets];
   float otherAxis[1][4];
   AxisSet axisSet;
   int axisIndex;
} vgl_TrBall;

#ifdef  __cplusplus
extern "C" {
#endif

VKI_EXTERN vgl_TrBall*
vgl_TrBallBegin(void);
VKI_EXTERN void
vgl_TrBallEnd(vgl_TrBall *p);
VKI_EXTERN void
vgl_TrBallPlace(vgl_TrBall *ball, HVect center, Vdouble radius);
VKI_EXTERN void
vgl_TrBallMouse(vgl_TrBall *ball, HVect vNow);
VKI_EXTERN void
vgl_TrBallUseSet(vgl_TrBall *ball, AxisSet axisSet);
VKI_EXTERN void
vgl_TrBallUpdate(vgl_TrBall *ball);
VKI_EXTERN void
vgl_TrBallSetMatrix(vgl_TrBall *ball,HMatrix mNow);
VKI_EXTERN void
vgl_TrBallGetMatrix(vgl_TrBall *ball,HMatrix mOut);
VKI_EXTERN void
vgl_TrBallValue(vgl_TrBall *ball, HMatrix mNow);
VKI_EXTERN void
vgl_TrBallBeginDrag(vgl_TrBall *ball);
VKI_EXTERN void
vgl_TrBallEndDrag(vgl_TrBall *ball);

#ifdef  __cplusplus
}
#endif

#endif
