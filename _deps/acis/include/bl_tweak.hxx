/* ORIGINAL: acis2.1/kernbool/blending/bl_tweak.hxx */
/* $Id: bl_tweak.hxx,v 1.8 2002/08/09 21:32:42 ktatters Exp $ */
/*******************************************************************/
/*    Copyright (c) 1989-2020 by Spatial Corp.                     */
/*    All rights reserved.                                         */
/*    Protected by U.S. Patents 5,257,205; 5,351,196; 6,369,815;   */
/*                              5,982,378; 6,462,738; 6,941,251    */
/*    Protected by European Patents 0503642; 69220263.3            */
/*    Protected by Hong Kong Patent 1008101A                       */
/*******************************************************************/

// Declare structs and routines used when recomputing blend geometry for
// tweaking.

#if !defined( BL_BLEND_EDIT )
#define BL_BLEND_EDIT

class ENTITY;
class ENTITY_LIST;
class FACE;
class BLEND_EDIT;
class VERTEX;
class curve;
class SPAposition;
class surface;


#include "acis.hxx"
#include "dcl_blnd.h"

#include "position.hxx"

class DECL_BLND spring_curve_info : public ACIS_OBJECT {

public:
	// The recomputed spring curve (owned by spring_curve_info) or point.

	curve				*spring_curve;		// can be null
	SPAposition			spring_point;		// set if curve is null

	// The entity the spring curve or point is on.

	ENTITY				*on_entity;

	spring_curve_info	*next;

	spring_curve_info(		// constructor
		SPAposition const & = *(const class SPAposition *)NULL_REF,
		curve * = NULL,
		ENTITY * = NULL
	  );

	~spring_curve_info();

	void debug( FILE * = debug_file_ptr ) const;
};

class DECL_BLND blend_info : public ACIS_OBJECT {

public:
	surface				*blend_sf;	// the new blend surface

	// Now a list of arbitrary length so that all kinds of blends
	// can return the same data structure.

	spring_curve_info	*spring_curve;

	blend_info(		// constructor
		surface * = NULL,
		spring_curve_info * = NULL
	  );

	~blend_info();

	void debug( FILE * = debug_file_ptr ) const;
};

DECL_BLND blend_info **recompute_all_blends(
	int,				// number of blend faces
	FACE **,			// array of blend faces
	BLEND_EDIT *		// local op being done
);

DECL_BLND curve *find_cross_curve(
	SPAposition			// SPAposition on cross curve
);

#endif
