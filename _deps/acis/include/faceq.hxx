/* ORIGINAL: 3dt2.1/geomhusk/faceq.hxx */
// $Id: faceq.hxx,v 1.10 2002/08/09 17:18:43 jeff Exp $
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
//    Declare procedures for doing queries about FACES
//
//----------------------------------------------------------------------

#ifndef faceq_hxx
#define faceq_hxx

//======================================================================

#include "dcl_query.h"
#include "logical.h"
#include "faceqry.hxx"

class SPAposition;
class SPAunit_vector;
class FACE;
class SPApar_pos;

//----------------------------------------------------------------------

// Find a point in the interior of a face. For internal use only!

DECL_QUERY logical 
find_interior_point(
			FACE *,
			SPAposition &,		// set to the required point
			SPApar_pos & = *(SPApar_pos *)NULL_REF
							// set to the SPAparameter value of the point
							// on the face's surface.
		);

// Get the normal of any face at a point
DECL_QUERY logical get_face_normal(FACE*, const SPAposition&, SPAposition&, SPAunit_vector&);

// Transform face properties into the global face of the owning body
DECL_QUERY void
face_prop_glob_space(
	logical,
	FACE*,
	double& ,
	SPAposition&,
	double&,
	double&,
	SPAunit_vector&, 
	SPAunit_vector&
);

//======================================================================
#endif
