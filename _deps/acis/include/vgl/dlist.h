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
 
#ifndef DLIST_DEF
#define DLIST_DEF
#include "base/basedefs.h"
#include "vgl/vgldefs.h"
#define DLIST_INIINT 256
#define DLIST_MAXINT 8192
#define DLIST_COLORINDEX 1
#define DLIST_COLOR 2
#define DLIST_POLYGON 3
#define DLIST_TEXTPLANE 4
#define DLIST_XFMPUSH 5
#define DLIST_XFMPOP 6
#define DLIST_LINESTYLE 7
#define DLIST_LINEWIDTH 8
#define DLIST_TRANS 9
#define DLIST_POLYGONCOLOR 10
#define DLIST_CLIP 11
#define DLIST_TEXT 12
#define DLIST_DELAY 13
#define DLIST_SPECULARITY 14
#define DLIST_CLIPPLANE 15
#define DLIST_POLYPOINT 16
#define DLIST_POLYPOINTCOLOR 17
#define DLIST_POLYLINE 18
#define DLIST_POLYLINECOLOR 19
#define DLIST_SETMODE 20
#define DLIST_POINTSIZE 21
#define DLIST_XFMLOAD 22
#define DLIST_XFMMULT 23
#define DLIST_TEXTDC 24
#define DLIST_POLYLINEDC 25
#define DLIST_POLYGONDC 26
#define DLIST_POLYPOINTDC 27
#define DLIST_TRANSINDEX 28
#define DLIST_POLYPOINTDATA 29
#define DLIST_POLYLINEDATA 30
#define DLIST_POLYGONDATA 31
#define DLIST_POLYGONMODE 32
#define DLIST_DATAINDEX 33
#define DLIST_DATA 34
#define DLIST_ATTPUSH 35
#define DLIST_ATTPOP 36
#define DLIST_SETSWITCH 37
#define DLIST_TEXTUREDEFINE 38
#define DLIST_TEXTURESELECT 39
#define DLIST_CLEAR 40
#define DLIST_SWAP 41
#define DLIST_FLUSH 42
#define DLIST_BELL 43
#define DLIST_DEPTHRANGE 44
#define DLIST_RASFONTDEFINE 45
#define DLIST_RASFONTSELECT 46
#define DLIST_LIGHTMODEL 47
#define DLIST_GETSTRING 48
#define DLIST_RESIZE 49
#define DLIST_BACKCOLORINDEX 50
#define DLIST_BACKCOLOR 51
#define DLIST_GETMODE 52
#define DLIST_GETINTEGER 53
#define DLIST_GETFLOAT 54
#define DLIST_LIGHT 55
#define DLIST_LIGHTSWITCH 56
#define DLIST_POLLMOUSE 57
#define DLIST_POLLMODIFIERS 58
#define DLIST_WARPMOUSE 59
#define DLIST_SETCURSOR 60
#define DLIST_READQUEUE 61
#define DLIST_TESTQUEUE 62
#define DLIST_RESETQUEUE 63
#define DLIST_TEXTPIXELSIZE 64
#define DLIST_FBUFFERREAD 65
#define DLIST_FBUFFERWRITE 66
#define DLIST_PIXELMAPCREATE 67
#define DLIST_PIXELMAPDESTROY 68
#define DLIST_PIXELMAPREAD 69
#define DLIST_PIXELMAPWRITE 70
#define DLIST_PROJORTHO 71
#define DLIST_PROJFRUSTUM 72
#define DLIST_VIEWPORT 73
#define DLIST_POSITIONWINDOW 74
#define DLIST_VISUALWINDOW 75
#define DLIST_OPENWINDOW 76
#define DLIST_CLOSEWINDOW 77
#define DLIST_CONNECTWINDOW 78
#define DLIST_DISCONNECTWINDOW 79
#define DLIST_SETWINDOW 80
#define DLIST_QUERYWINDOW 81
#define DLIST_PARENTWINDOW 82
#define DLIST_CONFIGUREWINDOW 83
#define DLIST_POLYGONOFFSET 89
#define DLIST_PROJPUSH 90
#define DLIST_PROJPOP 91
#define DLIST_BITMAPDEFINE 92
#define DLIST_BITMAPSELECT 93
#define DLIST_POLYPOINTARRAY 94
#define DLIST_POLYLINEARRAY 95
#define DLIST_POLYGONARRAY 96
#define DLIST_ZBUFFERREAD 97
#define DLIST_ZBUFFERWRITE 98
#define DLIST_INITBUFFER 99
#define DLIST_TERMBUFFER 100
#define DLIST_COPYBUFFER 101
#define DLIST_POLYPOINTBUFFER 102
#define DLIST_POLYLINEBUFFER 103
#define DLIST_POLYGONBUFFER 104
#define DLIST_SETFACTORS 105
#define DLIST_READBUFFER 106
#define DLIST_BUFFERSIZE 107
#define DLIST_BUFFERROW 108
#define DLIST_BUFFERQUERY 109
#define DLIST_PROJLOAD 110
#define DLIST_ADDDLIST 111
#define DLIST_ADDFUNCTION 112
#define DLIST_MAX 113
#define DLIST_NEXT 114
#define DLIST_END 115
#define DLIST_INTERVAL 1
#define DLIST_TRIGGER 2
#define DLIST_SPATIALIZE 3
#define DLIST_VIEWPORT_CULL 4
#define DLIST_MAXPRIMITIVE 5
#define DLIST_PACK 6
#define DLIST_FILTER_ATT 7
#define DLIST_MOVE_CULL 8
#define DLIST_TIME_CULL 9
#define DLIST_DETAIL_CULL 10
#define DLIST_MOVE_DELTA 11
#define DLIST_MOVE_FREQ 12
#define DLIST_MOVE_BUTTON 13
#define DLIST_TIME_BUDGET 14
#define DLIST_TIME_FREQ 15
#define DLIST_BOUNDBOX 17
#define DLIST_DETAIL_MINSIZE 18
#define DLIST_DETAIL_MAXSIZE 19
#define DLIST_SORT_BACKTOFRONT 20
#define DLIST_BALANCE_ATTSTACK 21
#define DLIST_BALANCE_PROJSTACK 22
#define DLIST_BALANCE_XFMSTACK 23
#define DLIST_INIT_BLOCKSIZE 24
#define DLIST_GROW_BLOCKSIZE 25
#define DLIST_USEBUFFERS 26
typedef struct { void *pntr;Vint prim;Vint cind;Vint
mode,flag;Vint iatt;Vint sel1;Vint sel2;Vfloat x[3];
}DListsortent;typedef struct { Vint flag;vsy_Concat
*concat;Vint num;Vint cnt;Vfloat *z;Vint *i;void **p;
vgl_DrawFun *df;DListsortent ent;}DListsort;typedef
struct { Vint numprim;Vfloat bmid[3];vgl_DList *dlc[8];
}DListspat;typedef struct { Vint attstackflag;Vint projstackflag;
Vint xfmstackflag;Vint nattstack;Vint nprojstack;Vint
nxfmstack;}DListbalance;typedef struct { Vint colorindex;
Vfloat color[3];Vint transindex;Vfloat trans;Vint linestyle;
Vint linewidth;Vint pointsize;Vint rasfontindex;Vint
bitmapindex;Vint textureindex;Vfloat intensity;Vfloat
shininess;Vint dinprims;Vint dinrws;Vint dimax;Vint
*dataindex;Vint nrws;Vfloat data[16];Vint mode,flag;
}DListattr;typedef struct vgl_DList_Block vgl_DList_Block;
struct vgl_DList_Block { vgl_DList_Block *prev;vgl_DList_Block
*next;Vint datasize;Vint *data;};struct vgl_DList {
Vint ierr;Vint mallocerr;vgl_DList_Block *initbloc;
vgl_DList_Block *currbloc;Vint *initaddr;Vint *blocaddr;
Vint *curraddr;Vint blocks;Vint initblockint;Vint growblock;
Vfloat xfm[4][4];Vint maxprim;Vfloat bbox[2][3];vgl_Xfm
*xfmp;vgl_Xfmstack *xfmstackp;Vchar visflag;Vchar xfmflag;
Vchar pointrend;Vchar linerend;Vchar polyrend;Vchar
filteratt;Vchar vpcull;Vchar movecull;Vchar timecull;
Vchar detailcull;Vchar spatialize;Vchar packflag;Vchar
sortbacktofront;Vint movedelta;Vint movefreq;Vint movebutton;
Vint xpix,ypix;Vint timefreq;Vfloat timebudget;Vfloat
timezero;Vint detailminsize;Vint detailmaxsize;Vint
numarray;Vint usevbos;Vint novboids;Vint numvboids,
maxvboids;Vfloat totvbomem;Vint *vboids;DListspat *spat;
DListbalance *balance;DListsort *sort;DListattr *catt;
DListattr *satt;Vchar colorindexflag;Vchar colorflag;
Vchar transindexflag;Vchar transflag;Vchar linestyleflag;
Vchar linewidthflag;Vchar pointsizeflag;Vchar specularityflag;
Vint colorindex;Vfloat color[3];Vint transindex;Vfloat
trans;Vint linestyle;Vint linewidth;Vint pointsize;
Vfloat intensity,shininess;Vint funfrequency;Vfunc2
*fun;Vobject *funobject;Vint abortflag;vsy_HashTable
*rasfonthash;vsy_HashTable *texturehash;vsy_HashTable
*bitmaphash;vgl_DrawFun *df;
#ifdef __cplusplus
public: VKI_EXTERN vgl_DList(void);VKI_EXTERN void*
operator new (size_t);VKI_EXTERN ~vgl_DList(void);VKI_EXTERN
void operator delete (void*);VKI_EXTERN Vint Error();
VKI_EXTERN void SetRendering(Vint,Vint,Vint);VKI_EXTERN
void SetVisibility(Vint);VKI_EXTERN void GetVisibility(Vint*);
VKI_EXTERN void SetXfm(Vfloat[4][4]);VKI_EXTERN void
SetObject(Vint,Vobject*);VKI_EXTERN void GetObject(Vint,Vobject**);
VKI_EXTERN void SetParami(Vint,Vint);VKI_EXTERN void
GetParami(Vint,Vint*);VKI_EXTERN void SetParamf(Vint,Vfloat);
VKI_EXTERN void SetParamfv(Vint,Vfloat[]);VKI_EXTERN
void SetColorIndex(Vint,Vint);VKI_EXTERN void GetColorIndex(Vint*,Vint*);
VKI_EXTERN void SetColor(Vint,Vfloat[3]);VKI_EXTERN
void GetColor(Vint*,Vfloat[3]);VKI_EXTERN void SetTransIndex(Vint,Vint);
VKI_EXTERN void GetTransIndex(Vint*,Vint*);VKI_EXTERN
void SetTrans(Vint,Vfloat);VKI_EXTERN void GetTrans(Vint*,Vfloat*);
VKI_EXTERN void SetLineStyle(Vint,Vint);VKI_EXTERN void
GetLineStyle(Vint*,Vint*);VKI_EXTERN void SetLineWidth(Vint,Vint);
VKI_EXTERN void GetLineWidth(Vint*,Vint*);VKI_EXTERN
void SetPointSize(Vint,Vint);VKI_EXTERN void GetPointSize(Vint*,Vint*);
VKI_EXTERN void SetSpecularity(Vint,Vfloat,Vfloat);
VKI_EXTERN void GetSpecularity(Vint*,Vfloat*,Vfloat*);
VKI_EXTERN void SetFunction(Vint,VDListfunc*,Vobject*);
VKI_EXTERN void Abort();VKI_EXTERN void Call();VKI_EXTERN
void Summary();VKI_EXTERN void Write(Vint,Vchar*);VKI_EXTERN
void Read(Vint,Vchar*);VKI_EXTERN void Memory(Vint*,Vint*,Vint*);
VKI_EXTERN void Statistics(Vint[],Vint[]);VKI_EXTERN
void Sort();VKI_EXTERN void Compact();VKI_EXTERN void
Erase();VKI_EXTERN void Color(Vfloat[3]);VKI_EXTERN
void DataIndex(Vint,Vint,Vint*);VKI_EXTERN void Data(Vint,Vfloat[]);
VKI_EXTERN void Polygon(Vint,Vint,Vfloat[][3],Vint,Vfloat[]);
VKI_EXTERN void PolygonDC(Vint,Vint,Vfloat[3],Vint[][3]);
VKI_EXTERN void LineStyle(Vint);VKI_EXTERN void LineWidth(Vint);
VKI_EXTERN void TransIndex(Vint);VKI_EXTERN void Trans(Vfloat);
VKI_EXTERN void Specularity(Vfloat,Vfloat);VKI_EXTERN
void PolygonColor(Vint,Vint,Vfloat[][3],Vfloat[][3],Vint,
Vfloat[]);VKI_EXTERN void PolyPointArray(Vint,Vfloat[][3],Vint,void*,Vint,void*,Vint,
Vfloat*,Vint,Vfloat*);VKI_EXTERN void PolyLineArray(Vint,Vint,Vfloat[][3],Vint,void*,Vint,void*,
Vint,Vfloat*,Vint,Vfloat*);VKI_EXTERN void PolygonArray(Vint,Vint,Vfloat[][3],Vint,void*,Vint,void*,
Vint,Vfloat*,Vint,Vfloat*);VKI_EXTERN void Text(Vfloat[3],Vtchar*);
VKI_EXTERN void TextDC(Vfloat[3],Vint[3],Vtchar*);VKI_EXTERN
void PolyPoint(Vint,Vfloat[][3]);VKI_EXTERN void PolyPointDC(Vint,Vfloat[3],Vint[][3]);
VKI_EXTERN void PolyPointColor(Vint,Vfloat[][3],Vfloat[][3]);
VKI_EXTERN void PolyLine(Vint,Vint,Vfloat[][3]);VKI_EXTERN
void PolyLineDC(Vint,Vint,Vfloat[3],Vint[][3]);VKI_EXTERN
void PolyLineColor(Vint,Vint,Vfloat[][3],Vfloat[][3]);
VKI_EXTERN void PointSize(Vint);VKI_EXTERN void AddFunction(Vfunc*,Vobject*);
VKI_EXTERN void AddDList(vgl_DList*);VKI_EXTERN void
PolygonMode(Vint);VKI_EXTERN void TextPixelSize(Vfloat);
VKI_EXTERN void DrawFun(vgl_DrawFun*);VKI_EXTERN void
Copy(vgl_DList*);
#endif
};
#ifdef __cplusplus
extern "C" { 
#endif
VKI_EXTERN vgl_DList* vgl_DListBegin(void);VKI_EXTERN
void vgl_DList_Construct(vgl_DList *p);VKI_EXTERN void
vgl_DListEnd(vgl_DList *p);VKI_EXTERN void vgl_DList_Destruct(vgl_DList
*p);VKI_EXTERN Vint vgl_DListError(vgl_DList *p);VKI_EXTERN
void vgl_DListSetRendering(vgl_DList *p,Vint pointrend,
Vint linerend,Vint polyrend);VKI_EXTERN void vgl_DListSetVisibility(vgl_DList
*p,Vint flag);VKI_EXTERN void vgl_DListGetVisibility(vgl_DList
*p,Vint *flag);VKI_EXTERN void vgl_DListSetXfm(vgl_DList
*p,Vfloat xfm[4][4]);VKI_EXTERN void vgl_DListSetObject(vgl_DList
*p,Vint objecttype,Vobject *object);VKI_EXTERN void
vgl_DListGetObject(vgl_DList *p,Vint objecttype,Vobject
**object);VKI_EXTERN void vgl_DListSetParami(vgl_DList
*p,Vint type,Vint iparam);VKI_EXTERN void vgl_DListGetParami(vgl_DList
*p,Vint type,Vint *iparam);VKI_EXTERN void vgl_DListSetParamf(vgl_DList
*p,Vint type,Vfloat param);VKI_EXTERN void vgl_DListSetParamfv(vgl_DList
*p,Vint type,Vfloat param[]);VKI_EXTERN void vgl_DListSetColorIndex(vgl_DList
*p,Vint flag,Vint index);VKI_EXTERN void vgl_DListGetColorIndex(vgl_DList
*p,Vint *flag,Vint *index);VKI_EXTERN void vgl_DListSetColor(vgl_DList
*p,Vint flag,Vfloat c[3]);VKI_EXTERN void vgl_DListGetColor(vgl_DList
*p,Vint *flag,Vfloat c[3]);VKI_EXTERN void vgl_DListSetTransIndex(vgl_DList
*p,Vint flag,Vint index);VKI_EXTERN void vgl_DListGetTransIndex(vgl_DList
*p,Vint *flag,Vint *index);VKI_EXTERN void vgl_DListSetTrans(vgl_DList
*p,Vint flag,Vfloat trans);VKI_EXTERN void vgl_DListGetTrans(vgl_DList
*p,Vint *flag,Vfloat *trans);VKI_EXTERN void vgl_DListSetLineStyle(vgl_DList
*p,Vint flag,Vint style);VKI_EXTERN void vgl_DListGetLineStyle(vgl_DList
*p,Vint *flag,Vint *style);VKI_EXTERN void vgl_DListSetLineWidth(vgl_DList
*p,Vint flag,Vint width);VKI_EXTERN void vgl_DListGetLineWidth(vgl_DList
*p,Vint *flag,Vint *width);VKI_EXTERN void vgl_DListSetPointSize(vgl_DList
*p,Vint flag,Vint size);VKI_EXTERN void vgl_DListGetPointSize(vgl_DList
*p,Vint *flag,Vint *size);VKI_EXTERN void vgl_DListSetSpecularity(vgl_DList
*p,Vint flag,Vfloat intensity,Vfloat shininess);VKI_EXTERN
void vgl_DListGetSpecularity(vgl_DList *p,Vint *flag,
Vfloat *intensity,Vfloat *shininess);VKI_EXTERN void
vgl_DListSetFunction(vgl_DList *p,Vint frequency,VDListfunc
*function,Vobject *object);VKI_EXTERN void vgl_DListAbort(vgl_DList
*p);VKI_EXTERN void vgl_DListCall(vgl_DList *p);VKI_EXTERN
void vgl_DListSummary(vgl_DList *p);VKI_EXTERN void
vgl_DListWrite(vgl_DList *p,Vint type,Vchar *path);
VKI_EXTERN void vgl_DListRead(vgl_DList *p,Vint type,
Vchar *path);VKI_EXTERN void vgl_DListMemory(vgl_DList
*p,Vint *numblocks,Vint *usesize,Vint *totsize);VKI_EXTERN
void vgl_DListStatistics(vgl_DList *p,Vint primnum[],
Vint primsiz[]);VKI_EXTERN void vgl_DListSort(vgl_DList
*p);VKI_EXTERN void vgl_DListCompact(vgl_DList *p);
VKI_EXTERN void vgl_DListErase(vgl_DList *p);VKI_EXTERN
void vgl_DListColor(vgl_DList *p,Vfloat c[3]);VKI_EXTERN
void vgl_DListDataIndex(vgl_DList *p,Vint nprims,Vint
nrws,Vint *index);VKI_EXTERN void vgl_DListData(vgl_DList
*p,Vint nrws,Vfloat d[]);VKI_EXTERN void vgl_DListPolygon(vgl_DList
*p,Vint type,Vint npts,Vfloat x[][3],Vint vtflag,Vfloat
vt[]);VKI_EXTERN void vgl_DListPolygonDC(vgl_DList *p,
Vint type,Vint npts,Vfloat x[3],Vint dc[][3]);VKI_EXTERN
void vgl_DListLineStyle(vgl_DList *p,Vint lstyle);VKI_EXTERN
void vgl_DListLineWidth(vgl_DList *p,Vint lwidth);VKI_EXTERN
void vgl_DListTransIndex(vgl_DList *p,Vint index);VKI_EXTERN
void vgl_DListTrans(vgl_DList *p,Vfloat trans);VKI_EXTERN
void vgl_DListSpecularity(vgl_DList *p,Vfloat intensity,
Vfloat shininess);VKI_EXTERN void vgl_DListPolygonColor(vgl_DList
*p,Vint type,Vint npts,Vfloat x[][3],Vfloat c[][3],
Vint vtflag,Vfloat vt[]);VKI_EXTERN void vgl_DListPolyPointArray(vgl_DList
*p,Vint npts,Vfloat x[][3],Vint cflag,void *c,Vint vflag,
void *v,Vint tflag,Vfloat *t,Vint dflag,Vfloat *d);
VKI_EXTERN void vgl_DListPolyLineArray(vgl_DList *p,
Vint type,Vint npts,Vfloat x[][3],Vint cflag,void *c,
Vint vflag,void *v,Vint tflag,Vfloat *t,Vint dflag,
Vfloat *d);VKI_EXTERN void vgl_DListPolygonArray(vgl_DList
*p,Vint type,Vint npts,Vfloat x[][3],Vint cflag,void
*c,Vint vflag,void *v,Vint tflag,Vfloat *t,Vint dflag,
Vfloat *d);VKI_EXTERN void vgl_DListText(vgl_DList *p,
Vfloat x[3],Vtchar *string);VKI_EXTERN void vgl_DListTextDC(vgl_DList
*p,Vfloat x[3],Vint dc[3],Vtchar *string);VKI_EXTERN
void vgl_DListPolyPoint(vgl_DList *p,Vint npts,Vfloat
x[][3]);VKI_EXTERN void vgl_DListPolyPointDC(vgl_DList
*p,Vint npts,Vfloat x[3],Vint dc[][3]);VKI_EXTERN void
vgl_DListPolyPointColor(vgl_DList *p,Vint npts,Vfloat
x[][3],Vfloat c[][3]);VKI_EXTERN void vgl_DListPolyLine(vgl_DList
*p,Vint type,Vint npts,Vfloat x[][3]);VKI_EXTERN void
vgl_DListPolyLineDC(vgl_DList *p,Vint type,Vint npts,
Vfloat x[3],Vint dc[][3]);VKI_EXTERN void vgl_DListPolyLineColor(vgl_DList
*p,Vint type,Vint npts,Vfloat x[][3],Vfloat c[][3]);
VKI_EXTERN void vgl_DListPointSize(vgl_DList *p,Vint
psize);VKI_EXTERN void vgl_DListAddFunction(vgl_DList
*p,Vfunc *function,Vobject *object);VKI_EXTERN void
vgl_DListAddDList(vgl_DList *p,vgl_DList *addp);VKI_EXTERN
void vgl_DListPolygonMode(vgl_DList *p,Vint rend);VKI_EXTERN
void vgl_DListTextPixelSize(vgl_DList *p,Vfloat pixelsize);
VKI_EXTERN void vgl_DListDrawFun(vgl_DList *p,vgl_DrawFun
*df);VKI_EXTERN void vgl_DListCopy(vgl_DList *p,vgl_DList
*fromp);
#ifdef __cplusplus
}
#endif
#endif

