/* ORIGINAL: 3dt2.1/geomhusk/surf_utl.hxx */
// $Id: surf_utl.hxx,v 1.10 2002/08/09 17:15:15 jeff Exp $
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
//    Decalre procedures for working with surfaces
//
//----------------------------------------------------------------------

#ifndef surf_utl_hxx
#define surf_utl_hxx

#include "dcl_kern.h"
// ywoo 08Jan01: added the header file.
#include "base.hxx"
// ywoo: end

//======================================================================

class surface;
class FACE;
class SPAinterval;

//----------------------------------------------------------------------

DECL_KERN surface* get_surface_from_face(FACE const*);

//======================================================================

DECL_KERN void exercise_surface_derivatives(
	FACE	 *in_face, 
	const SPAinterval &range_u = *(SPAinterval *) NULL_REF, 
	const SPAinterval &range_v = *(SPAinterval *) NULL_REF, 
	int		  nu=10, 
	int		  nv=10, 
	FILE	 *out_file_ptr=NULL);
//======================================================================
#endif
