/* ORIGINAL: 3dt2.1/main/windows/aciswin.h */
/*******************************************************************/
/*    Copyright (c) 1989-2020 by Spatial Corp.                     */
/*    All rights reserved.                                         */
/*    Protected by U.S. Patents 5,257,205; 5,351,196; 6,369,815;   */
/*                              5,982,378; 6,462,738; 6,941,251    */
/*    Protected by European Patents 0503642; 69220263.3            */
/*    Protected by Hong Kong Patent 1008101A                       */
/*******************************************************************/
/* This entire file is only used under Windows & NT */
#ifdef NT

#include <windows.h>
#include <windef.h>
#include "logical.h"
/*
// $Id: aciswin.h,v 1.18 2002/06/24 16:44:03 jeff Exp $
//----------------------------------------------------------------------
// purpose---
//      Define constant used in Windows menus
//
//----------------------------------------------------------------------
*/
/* Help menu items */
#define IDM_ABOUT			100
#define IDM_SCHEME_HELP		101
#define IDM_SCHEME_VERSION	102

/* edit menu items */
#define     IDM_UNDO     200
#define     IDM_CUT      201
#define     IDM_COPY     202
#define     IDM_PASTE    203
#define     IDM_CLEAR    204

/* File menu items */
#define     IDM_OPEN		300
#define     IDM_SAVE		301
#define     IDM_PART_CLEAR  302
#define     IDM_EXIT		303

/* View menu items */
#define     IDM_ISO					400
#define     IDM_FRONT				401
#define     IDM_TOP					402
#define     IDM_RIGHT				403
#define     IDM_ZOOMALL				404
#define     IDM_RENDER_REBUILD		405

/* Display menu items */
#define     IDM_GL						500
#define     IDM_DISPLAY_SHADED			501
#define     IDM_DISPLAY_EDGES			502
#define     IDM_DISPLAY_COEDGES			503
#define     IDM_DISPLAY_TCOEDGES		504
#define     IDM_DISPLAY_VERTICES		505
#define     IDM_DISPLAY_POLY_OFFSET     506
#define     IDM_DL						507
#define     IDM_DISPLAY_FACETS			508
#define     IDM_DISPLAY_PARAM			509
#define     IDM_DISPLAY_SURF_POLY       510
#define     IDM_DISPLAY_SILHOUETTES     511
#define     IDM_DISPLAY_AXES            512

/* Options menu items */
#define     IDM_LOAD_NOISILY			600
#define     IDM_MATCH_PARENTHESIS		601
#define     IDM_TIMING                  602

/* Debug menu items */
#define     IDM_JOURNAL				700
#define     IDM_DEBUG				701
#define     IDM_CHECK				702
#define     IDM_TOLERANT_REPORT 	703
#define     IDM_BREP_HEALTH			704
#define     IDM_BREP_HEALTH_IOP		705

/* Test menu items */
#define     IDM_SMOKE_STATUS_ALL    	800
#define     IDM_SMOKE_STATUS_SMOKE  	801
#define     IDM_SMOKE_STATUS_FAST   	802
#define     IDM_SMOKE_ACIS		    	803
#define     IDM_SMOKE_BLEND	    		804
#define     IDM_SMOKE_PATTERN			805
#define     IDM_SMOKE_SKIN          	806
#define     IDM_SMOKE_SWEEP           	807
#define     IDM_SMOKE_LOP             	808
#define     IDM_SMOKE_WARP            	809
#define     IDM_SMOKE_BOOL              810
#define     IDM_SMOKE_MT                811
#define		IDM_SMOKE_STITCH			812
#define		IDM_SMOKE_POLY				813
#define		IDM_SMOKE_INCR				814

/* Extra pull down items */
#define IDM_SCHEME_EXTRA1 2001

/* Progress dialog items */

#define IDD_DIALOG1                     101
#define IDC_PROGRESS1                   1000
#define IDC_STATIC1						1001

/* Progress dialog items - Stitch */

#define IDD_STITCH_DIALOG                     900
#define IDC_STITCH_PROGRESS                   10000
#define IDC_STITCH_STATIC						10001

/* Progress dialog items - Stitch */

#define IDD_SPA_DIALOG                     10200
#define IDC_SPA_PROGRESS                   10100
#define IDC_SPA_STATIC						10101
#define IDC_SPA_STATIC2						10102

/* Exceptions */
#define EXCEPT_CTRLC 100

int TKMain(HINSTANCE, HINSTANCE, LPSTR, int);
BOOL InitApplication(HINSTANCE, LPSTR);
BOOL InitInstance(HINSTANCE, int, LPSTR);
LRESULT WINAPI MainWndProc(HWND, UINT, WPARAM, LPARAM);
BOOL WINAPI About(HWND, UINT, UINT, LONG);
void OutOfMemory();

typedef logical (*ExtraPullDownFunction) (HWND hWnd, UINT message, WPARAM wParam, LPARAM lParam, HMENU hMenu);
extern ExtraPullDownFunction ExtraPullDown; 


#endif
