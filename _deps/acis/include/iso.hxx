/* ORIGINAL: acis2.1/sg_husk/vis/iso.hxx */
/* $Id: iso.hxx,v 1.9 2002/08/09 17:19:06 jeff Exp $ */
/*******************************************************************/
/*    Copyright (c) 1989-2020 by Spatial Corp.                     */
/*    All rights reserved.                                         */
/*    Protected by U.S. Patents 5,257,205; 5,351,196; 6,369,815;   */
/*                              5,982,378; 6,462,738; 6,941,251    */
/*    Protected by European Patents 0503642; 69220263.3            */
/*    Protected by Hong Kong Patent 1008101A                       */
/*******************************************************************/


#if !defined (NEW_ISO)

#define NEW_ISO

#include "dcl_intr.h"
#include "box.hxx"

class FACE;
class ENTITY_LIST;
class SPAtransf;
class surface;
class surf_surf_int;


//
// Calculate a v SPAparameter line for a fixed u SPAparameter value on a surface. 
// A region of interest must be specified to delimit the SPAparameter line.
// A surface/surface intersection structure is initalized which contains
// the requested SPAparameter line (as its curve) along with start/end
// terminators representing the extent of the SPAparameter line within the
// region of interest. An auxhillary surface containing the SPAparameter line
// may also be returned.

DECL_INTR void
sg_surface_v_param_line(
	const surface&,	// surface on which to get v SPAparameter line
	double,			// u SPAparameter at which to calculate v SPAparameter line
	const SPAbox&,	// region of interest to delimit SPAparameter line
	surf_surf_int*&,// surface/surface intersection structure; should be NULL
	surface*&		// auxhillary surface containing SPAparameter line 
	);


// Calculate a u SPAparameter line for a fixed v SPAparameter value on a surface. 
// A region of interest must be specified to delimit the SPAparameter line.
// A surface/surface intersection structure is initalized which contains
// the requested SPAparameter line (as its curve) along with start/end
// terminators representing the extent of the SPAparameter line within the
// region of interest. An auxhillary surface containing the SPAparameter line
// may also be returned.

DECL_INTR void
sg_surface_u_param_line(
	const surface&,	// surface on which to get u SPAparameter line
	double,			// v SPAparameter at which to calculate u SPAparameter line
	const SPAbox&,	// region of interest to delimit SPAparameter line
	surf_surf_int*&,// surface/surface intersection structure; should be NULL
	surface*&		// auxhillary surface containing SPAparameter line 
	);


// Calculate nu (nv) equally spaced U (V) iso-parametric curves on a face.
// Trim the iso-curves to the boundary of the face and return a list of
// edges representing these trimmed curves.

DECL_INTR void
sg_face_nu_nv_isolines(
	int,			// nu,
	int,			// nv,
	FACE*,			// face,
	const SPAtransf&,	// ftrans,
	ENTITY_LIST*	// edge_list
	);



// Calculate EDGEs representing a constant u iso-parametric curve bound to a
// face. Trim the iso-curves to the boundary of the face and return a list of
// edges representing these trimmed curves.

DECL_INTR void
sg_face_u_iso(
	double,			// v SPAparameter
	FACE*,			// face,
	const SPAtransf&,	// ftrans,
	ENTITY_LIST*	// edge_list
	);



// Calculate EDGEs representing a constant v iso-parametric curve bound to a
// face. Trim the iso-curves to the boundary of the face and return a list of
// edges representing these trimmed curves.

DECL_INTR void
sg_face_v_iso(
	double,			// u SPAparameter
	FACE*,			// face,
	const SPAtransf&,	// ftrans,
	ENTITY_LIST*	// edge_list
	);

#endif
