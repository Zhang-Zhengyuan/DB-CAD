                   /* Direct3D utility functions header file */ 

#ifndef VGLD3D_DEF
#define VGLD3D_DEF

#ifdef  __cplusplus
extern "C" {
#endif

#ifdef VKI_3DAPI_DIRECT3D
#ifdef VKI_WIND_WIN32
VKI_EXTERN void
vgl_Direct3DDevConnectWindowD3D(vgl_Direct3DDev *p, HWND window);
VKI_EXTERN void
vgl_Direct3DDevDisconnectWindowD3D(vgl_Direct3DDev *p);
VKI_EXTERN void
vgl_Direct3DDevPolygonColorD3D(vgl_Direct3DDev *p, Vint type,
                               Vint npts, Vfloat x[][3], Vfloat c[][3],
                               Vint vtflag, Vfloat vt[]);
VKI_EXTERN void
vgl_Direct3DDevPolygonD3D(vgl_Direct3DDev *p, Vint type,
                          Vint npts, Vfloat x[][3], Vint vtflag, Vfloat vt[]);
VKI_EXTERN void
vgl_Direct3DDevPolyLineD3D(vgl_Direct3DDev *p, Vint type, Vint npts,
                           Vfloat x[][3]);
VKI_EXTERN void
vgl_Direct3DDevPolyLineColorD3D(vgl_Direct3DDev *p, Vint type, Vint npts,
                                Vfloat x[][3], Vfloat c[][3]);
VKI_EXTERN void
vgl_Direct3DDevPolyPointColorD3D(vgl_Direct3DDev *p, Vint npts, Vfloat x[][3],
                                 Vfloat c[][3], Vchar func[]);
VKI_EXTERN void
vgl_Direct3DDevFBufferWriteD3D(vgl_Direct3DDev *p, Vint left, Vint right,
                               Vint bottom, Vint top, vgl_FBuffer *fbuffer,
                               Vchar func[]);
VKI_EXTERN void
vgl_Direct3DDevClearD3D(vgl_Direct3DDev *p);
VKI_EXTERN void
vgl_Direct3DDevSwapD3D(vgl_Direct3DDev *p);
VKI_EXTERN void
vgl_Direct3DDevProjD3D(vgl_Direct3DDev *p);
VKI_EXTERN void
vgl_Direct3DDevXfmLoadD3D(vgl_Direct3DDev *p);
VKI_EXTERN void
vgl_Direct3DDevGetViewportD3D (vgl_Direct3DDev *p, Vint *left, Vint *right,
                               Vint *bottom, Vint *top, Vfloat *znear,
                               Vfloat *zfar);
VKI_EXTERN void
vgl_Direct3DDevViewportD3D(vgl_Direct3DDev *p, Vint left, Vint right,
                           Vint bottom, Vint top, Vfloat znear, Vfloat zfar);
VKI_EXTERN void
vgl_Direct3DDevGetFloatD3D(vgl_Direct3DDev *p, Vint type, Vfloat *params);
VKI_EXTERN void
vgl_Direct3DDevLightD3D(vgl_Direct3DDev *p,
                        Vint index, Vint type, Vfloat c[3], Vfloat x[3]);
VKI_EXTERN void
vgl_Direct3DDevSetSwitchD3D(vgl_Direct3DDev *p, Vint type, Vint index,
                            Vint flag);
VKI_EXTERN void
vgl_Direct3DDevSpecularityD3D(vgl_Direct3DDev *p, Vfloat intensity,
                              Vfloat shininess);
VKI_EXTERN void
vgl_Direct3DDevTextureDefineD3D(vgl_Direct3DDev *p, Vint index,
                                vgl_Texture *texture);
VKI_EXTERN void
vgl_Direct3DDevTextureSelectD3D(vgl_Direct3DDev *p, Vint index);
VKI_EXTERN void
vgl_Direct3DDevTextureEndD3D(vgl_Direct3DDev *p);
VKI_EXTERN void
vgl_Direct3DDevBitmapDefineD3D(vgl_Direct3DDev *p, Vint index,
                               vgl_Bitmap *bitmap);
VKI_EXTERN void
vgl_Direct3DDevBitmapSelectD3D(vgl_Direct3DDev *p, Vint index);
VKI_EXTERN void
vgl_Direct3DDevFBufferWriteD3D(vgl_Direct3DDev *p, Vint left, Vint right,
                               Vint bottom, Vint top, vgl_FBuffer *fbuffer,
                               Vchar func[]);
VKI_EXTERN void
vgl_Direct3DDevPixelmapReadD3D(vgl_Direct3DDev *p, Vint left, Vint right,
                               Vint bottom, Vint top, Vfloat *array);
VKI_EXTERN void
vgl_Direct3DDevPixelmapWriteD3D(vgl_Direct3DDev *p, Vint left, Vint right,
                                Vint bottom, Vint top, Vfloat *array,
                                Vchar func[]);
VKI_EXTERN void
vgl_Direct3DDevPolyPointDCD3D(vgl_Direct3DDev *p, Vint npts, Vfloat x[3],
                              Vint dc[][3]);
VKI_EXTERN void
vgl_Direct3DDevPolyLineDCD3D(vgl_Direct3DDev *p, Vint type, Vint npts,
                             Vfloat x[3], Vint dc[][3]);
VKI_EXTERN void
vgl_Direct3DDevPolygonDCD3D(vgl_Direct3DDev *p, Vint type,
                            Vint npts, Vfloat x[3], Vint dc[][3]);;
VKI_EXTERN void
vgl_Direct3DDevSetModeD3D (vgl_Direct3DDev *p, Vint mode, Vint flag);
VKI_EXTERN void
vgl_Direct3DDevPolygonModeD3D (vgl_Direct3DDev *p, Vint rend);
VKI_EXTERN void
vgl_Direct3DDevSetBlendD3D(vgl_Direct3DDev *p, Vint flag);
VKI_EXTERN void
vgl_Direct3DDevSetStencilD3D(vgl_Direct3DDev *p, Vint flag);
VKI_EXTERN void
vgl_Direct3DDevGetStringD3D(vgl_Direct3DDev *p, Vint type, Vchar string[]);
VKI_EXTERN void
vgl_Direct3DDevLightModelD3D(vgl_Direct3DDev *p, Vint localviewer);
VKI_EXTERN void
vgl_Direct3DDevGetProjD3D(vgl_Direct3DDev *p, Vfloat tm[4][4]);
VKI_EXTERN void
vgl_Direct3DDevClipPlaneD3D(vgl_Direct3DDev *p, Vint index, Vfloat eqn[4]);
VKI_EXTERN void
vgl_Direct3DDevClipD3D(vgl_Direct3DDev *p, Vint left, Vint right,
                       Vint bottom, Vint top);
VKI_EXTERN void
vgl_Direct3DDevPointSizeD3D(vgl_Direct3DDev *p, Vint psize);
VKI_EXTERN void
vgl_Direct3DDevResetDeviceD3D(vgl_Direct3DDev *p, Vchar func[]);
VKI_EXTERN void
vgl_Direct3DDevPolygonOffsetD3D(vgl_Direct3DDev *p);
VKI_EXTERN void
vgl_Direct3DDevPolyPointArrayD3D(vgl_Direct3DDev *p, Vint npts, Vfloat x[][3],
                                 Vint cflag, void *c,
                                 Vint vflag, void *v,
                                 Vint tflag, Vfloat *t,
                                 Vint dflag, Vfloat *d);
VKI_EXTERN void
vgl_Direct3DDevPolyLineArrayD3D(vgl_Direct3DDev *p, Vint type, Vint npts,
                                Vfloat x[][3],
                                Vint cflag, void *c,
                                Vint vflag, void *v,
                                Vint tflag, Vfloat *t,
                                Vint dflag, Vfloat *d);
VKI_EXTERN void
vgl_Direct3DDevPolygonArrayD3D(vgl_Direct3DDev *p, Vint type,
                               Vint npts, Vfloat x[][3],
                               Vint cflag, void *c,
                               Vint vflag, void *v,
                               Vint tflag, Vfloat *t,
                               Vint dflag, Vfloat *d);
VKI_EXTERN void
vgl_Direct3DDevInitBufferD3D(vgl_Direct3DDev *p, Vint size, Vint *vboid);
VKI_EXTERN void
vgl_Direct3DDevTermBufferD3D(vgl_Direct3DDev *p, Vint vboid);
VKI_EXTERN void
vgl_Direct3DDevCopyBufferD3D(vgl_Direct3DDev *p, Vint vboid, Vint npts,
                             Vfloat x[][3], Vint cflag, void *c,
                             Vint vflag, void *v, Vint tflag, Vfloat *t,
                             Vint dflag, Vfloat *d,
                             Vint *offset);
VKI_EXTERN void
vgl_Direct3DDevPolyPointBufferD3D(vgl_Direct3DDev *p, Vint vboid, Vint offset,
                                  Vint npts, Vint cflag,
                                  Vint vflag, Vint tflag, Vint dflag);
VKI_EXTERN void
vgl_Direct3DDevPolyLineBufferD3D(vgl_Direct3DDev *p, Vint vboid, Vint offset,
                                 Vint type, Vint npts, Vint cflag,
                                 Vint vflag, Vint tflag, Vint dflag);
VKI_EXTERN void
vgl_Direct3DDevPolygonBufferD3D(vgl_Direct3DDev *p, Vint vboid, Vint offset,
                                Vint type, Vint npts, Vint cflag,
                                Vint vflag, Vint tflag, Vint dflag);
VKI_EXTERN void
vgl_Direct3DDevFlushD3D(vgl_Direct3DDev *p);
#endif /* VKI_WIND_WIN32 */
#endif /* VKI_3DAPI_DIRECT3D */

#ifdef  __cplusplus
}
#endif

#endif /* VGLD3D_DEF */
