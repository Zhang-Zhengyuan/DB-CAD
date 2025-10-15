/* ORIGINAL: acis2.1/kerndata/geom/straight.hxx */
/* $Id: degenerate.hxx,v 1.8 2002/08/09 17:15:17 jeff Exp $ */
/*******************************************************************/
/*    Copyright (c) 1989-2020 by Spatial Corp.                     */
/*    All rights reserved.                                         */
/*    Protected by U.S. Patents 5,257,205; 5,351,196; 6,369,815;   */
/*                              5,982,378; 6,462,738; 6,941,251    */
/*    Protected by European Patents 0503642; 69220263.3            */
/*    Protected by Hong Kong Patent 1008101A                       */
/*******************************************************************/

// Header file describing the modeller data structures

// A straight line, a DEGENERATE, is defined by a straight which
// in turn records a point on the line and a unit tangent SPAvector.
// The direction of the tangent is regarded as the inherent 
// direction of the curve.


#ifndef DEGENERATE_CLASS
#define DEGENERATE_CLASS

#include "dcl_kern.h"
#include "logical.h"

#include "curve.hxx"

#include "pntdef.hxx"

class SPAposition;
class SPAunit_vector;
class SPAtransf;
/**
 * @nodoc
 */
ENTITY_IS_PROTOTYPE(DEGENERATE , KERN)
#if 0
; // semicolon needed for mkman (doc tool) parsing
#endif


// Identifier used to find out (via identity() defined below) to what 
// an entity pointer refers.

extern DECL_KERN int DEGENERATE_TYPE;


// Identifier that gives number of levels of derivation of this class
// from ENTITY.

#define DEGENERATE_LEVEL 2


// DEGENERATE declaration proper.

class DECL_KERN DEGENERATE: public CURVE {

	// Record a DEGENERATE as a degenerate_curve.

	degenerate_curve def;


// STI ROLL begin - added virtual compare function for api_get_modified_faces
protected:
    virtual logical bulletin_no_change_vf(ENTITY const* other, logical identical_comparator) const;
// STI ROLL end

	// Include the standard member functions for all entities.

	/**
	 * @nodoc
	 */
	ENTITY_FUNCTIONS( DEGENERATE , KERN)
#if 0
; // semicolon needed for mkman (doc tool) parsing)
#endif
// Because mkman isn't aware that ENTITY_FUNCTIONS changes access to "public", do so
// explicitly here:
public:


	// Now the functions specific to DEGENERATE.

	// Make a bare DEGENERATE to be filled in later.

	DEGENERATE();


	// Create a DEGENERATE line that passes through given SPAposition 
	// and in direction of given unit SPAvector.

	DEGENERATE( SPAposition const & );


	// Create a DEGENERATE from a degenerate.

	DEGENERATE( degenerate_curve const & );


	// Data reading routines.

	SPAposition const &root_point() const { return def.root_point; }


	// Data changing routines.  Each of these routines checks 
	// that the record has been posted on the bulletin-board before 
	// performing the change.  If not, the routine provokes an error, 
	// so that the omission (forgetting to call backup() first) can 
	// be rectified in the program.  In production versions of the 
	// program, these checks may be disabled, to improve efficiency.

	void set_root_point( SPAposition const & );


	// Return the curve equation for reading only.

	curve const &equation() const;


	// Return the curve equation, checking first for backup.

	curve &equation_for_update();


	// Get a new (lower-case) curve being the degenerate_curve of the 
	// DEGENERATE, transformed if the given SPAtransf is non-null and 
	// reversed in sense if the logical is true.

	curve *trans_curve(
					SPAtransf const & = *(SPAtransf *)NULL_REF,
					logical = FALSE
				) const;


	// Transform the stored degenerate_curve in place.

	void operator*=( SPAtransf const & );

	// STI ROLL
	void full_size(SizeAccumulator& est, logical countSelf = TRUE) const;
	// STI ROLL

	// The generic CURVE version of make_box is good enough
	// so the version for degenerate_curve is left in abeyance.

	// SPAbox make_box( APOINT *, APOINT *, SPAtransf const * ) const;


	// lookup is done using CURVE::lookup()
	// int lookup( logical ) const;
};

#endif
