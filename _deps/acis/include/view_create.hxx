/* ORIGINAL: 3dt2.1/pmhusk/part_utl.hxx */
// $Id: view_create.hxx,v 1.4 2001/12/31 20:39:25 jmb Exp $
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
//    Define Part Management utility procedures
//----------------------------------------------------------------------

#ifndef __view_create_hxx__
#define __view_create_hxx__

#include "logical.h"
//#include "mmgr.hxx"
#include "wndo_dt.hxx"

//======================================================================

class view_creation_routine
{
public:
	virtual ~view_creation_routine(){}
	virtual window_handle create_window(int x, int y, int width, int height)=0;
};

void set_view_creation_routine(view_creation_routine*);

class SchemeView;
enum enum_view_render_type {GDI_TYPE,OPENGL_TYPE};
SchemeView* create_view(enum_view_render_type,int x=0,int y=0,int width=250, int height=250);

//======================================================================
#endif // __view_create_hxx__
