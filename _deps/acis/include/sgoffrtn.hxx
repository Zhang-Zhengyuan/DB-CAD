/* ORIGINAL: acis2.1/sg_husk/offset/sgoffrtn.hxx */
/* $Id: sgoffrtn.hxx,v 1.5 2002/08/09 17:27:14 jeff Exp $ */
/*******************************************************************/
/*    Copyright (c) 1989-2020 by Spatial Corp.                     */
/*    All rights reserved.                                         */
/*    Protected by U.S. Patents 5,257,205; 5,351,196; 6,369,815;   */
/*                              5,982,378; 6,462,738; 6,941,251    */
/*    Protected by European Patents 0503642; 69220263.3            */
/*    Protected by Hong Kong Patent 1008101A                       */
/*******************************************************************/

#if !defined( SG_OFFSET_ROUTINES )
#define SG_OFFSET_ROUTINES

#include "dcl_ofst.h"
// ywoo 08Jan01: added the header file.
#include "logical.h"
// ywoo: end
#include "offset_opts.hxx"

// forward declarations
class SPAbox;
class curve;
class pcurve;
class SPAposition;
class surface;
class FACE;

// Offset routines

DECL_OFST FACE*
sg_offset_face( 							// Return the offset FACE
	const FACE *,							// FACE to offset
	double,									// offset distance
	logical &,                               // Return if we failed to make a full face
    offset_options* pOffopts =NULL );		// Offset options	

DECL_OFST surface*
sg_offset_surface(							// Return offset surface
	const surface *,						// Surface to offset
	const SPAbox &,							// Region of interest --- only for elliptical cylinder
	double );								// offset distance

DECL_OFST SPAposition 
sg_offset_pos(								// Return SPAposition of offset point
	const SPAposition &,						// Point to offset
	const surface *,						// Surface to offset from
	double );								// offset distance

DECL_OFST FACE *
sg_extract_face(							// Return a copy of 
	FACE*	this_face 						// this face with all loops etc. but detached from shells
	);

DECL_OFST logical
sg_at_apex(									// Return TRUE
	const SPAposition&	test_pos,				// if test_pos is at the apex of
	const surface*	surf					// this surface
	);

#endif
