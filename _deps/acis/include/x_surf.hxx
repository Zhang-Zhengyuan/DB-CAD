/* ORIGINAL: acis2.1/sg_husk/vis/x_surf.hxx */
/* $Id: x_surf.hxx,v 1.5 2002/08/09 17:19:06 jeff Exp $ */
/*******************************************************************/
/*    Copyright (c) 1989-2020 by Spatial Corp.                     */
/*    All rights reserved.                                         */
/*    Protected by U.S. Patents 5,257,205; 5,351,196; 6,369,815;   */
/*                              5,982,378; 6,462,738; 6,941,251    */
/*    Protected by European Patents 0503642; 69220263.3            */
/*    Protected by Hong Kong Patent 1008101A                       */
/*******************************************************************/


#if !defined (NEW_X_SURF)
#define NEW_X_SURF

#include "dcl_intr.h"

class surface;
class FACE;
class ENTITY_LIST;
class SPAtransf;
class surf_surf_int;
class ff_header;
// ywoo 08Jan01: added the class.
class SPAbox;
// ywoo: end


// Intersect a surface with another surface. A surface/surface
// intersection structure is returned which defines the curves
// resulting from the intersection of the two surfaces.
 
DECL_INTR void
	sg_x_surf_surf(
	surface const&  surf1,          // surface 1
	SPAtransf const&   trans1,         // transformation for surface 1
	surface const&  surf2,          // surface 2
	SPAtransf const&   trans2,         // transformation for surface 2
	SPAbox const&      region,         // intersection region of interest
	surf_surf_int*  ss_list         // surface/surface intersection
	);


// Intersect a surface with a face. A surface/surface
// intersection structure is returned which defines
// the curves resulting from the intersection of the
// surface and the face. In addition, a face/face
// intersection structure is returned which defines 
// the intersection of the curves with the boundary
// of the face.

DECL_INTR void
sg_x_surf_face(
	surface const&	,// surface (alreadyin same
					 // space as transformed face)
	FACE*			,// face,			// face
	SPAtransf const&	,// transformation to SPAposition face
	surf_surf_int*&	,// surface/surface intersection
	ff_header*&		 // face/face intersection 
	);

#endif
