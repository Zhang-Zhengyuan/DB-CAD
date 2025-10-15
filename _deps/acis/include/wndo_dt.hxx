/*******************************************************************/
/*    Copyright (c) 1989-2020 by Spatial Corp.                     */
/*    All rights reserved.                                         */
/*    Protected by U.S. Patents 5,257,205; 5,351,196; 6,369,815;   */
/*                              5,982,378; 6,462,738; 6,941,251    */
/*    Protected by European Patents 0503642; 69220263.3            */
/*    Protected by Hong Kong Patent 1008101A                       */
/*******************************************************************/
//----------------------------------------------------------------------
// purpose---
//    Graphics system dependent data types
//
//----------------------------------------------------------------------

#ifndef wndo_dt_hxx
#define wndo_dt_hxx

//======================================================================

#if defined( NT ) || defined( _WINDOWS )

#ifdef BOOLEAN
#undef BOOLEAN
#define REDEFINE_BOOLEAN
#endif
#include "windows.h"
#ifdef REDEFINE_BOOLEAN
#define BOOLEAN BOOLEAN
#endif

// windows.h (actually some OLE files included by it) defines THIS and
// small which conflict with some ACIS stuff, so we undefine them here
#ifdef small
#undef small
#endif

#ifdef THIS
#undef THIS
#endif

typedef HWND window_handle;
typedef unsigned int display_handle;
typedef HDC device_context;

#endif

#if defined( mac ) && !defined( macos_a64 ) && !defined( macos_b64 )
#include <math.h>
#include "carbonheader.h"

// window clase (windowkind) for the mac windows
enum { macKind = 1000, qd3dKind };

// resource IDs
enum { macWindowID = 129 };

typedef WindowPtr window_handle;
typedef WindowPtr device_context;
typedef unsigned int display_handle;

#endif // mac

#ifdef X_WINDOWS
#include <X11/Xlib.h>
#include <X11/Intrinsic.h>
typedef Widget window_handle;
typedef Display* display_handle;
typedef unsigned long device_context;

#endif


#if defined( OS_Darwin )// ( defined( macos_a64) || defined( macos_b64) )
typedef long window_handle;
typedef long device_context;
typedef unsigned int display_handle;
#endif

//======================================================================
#endif
