/* ORIGINAL: acis2.1/meshhusk/geom/compcurv.hxx */
/*******************************************************************/
/*    Copyright (c) 1989-2020 by Spatial Corp.                     */
/*    All rights reserved.                                         */
/*    Protected by U.S. Patents 5,257,205; 5,351,196; 6,369,815;   */
/*                              5,982,378; 6,462,738; 6,941,251    */
/*    Protected by European Patents 0503642; 69220263.3            */
/*    Protected by Hong Kong Patent 1008101A                       */
/*******************************************************************/

#if !defined( COMPCURV_CLASS )
#define COMPCURV_CLASS

// Header for COMPCURV.

// A COMPCURV, a class derived from CURVE, records a 
// composite curve as a (lower-case) compcurv.  The compcurv
// holds the arrays of coordinate nodes and elements (segments)
// that define a piecework curve.

#include "dcl_kern.h"
#include "curve.hxx"

#include "compdef.hxx"

class SPAtransf;
class SPAbox;
class LOOP;
class EDGE;
class neighborhood;
class MESHSURF;
typedef logical REVBIT;
/**
 * @nodoc
 */
ENTITY_IS_PROTOTYPE( COMPCURV, KERN )
#if 0
; // semicolon needed for mkman (doc tool) parsing
#endif

// Identifier used to find out (via identity() defined below) to what 
// an entity pointer refers.

extern DECL_KERN int COMPCURV_TYPE;


/**
 * Identifier that gives number of levels of derivation of this class
 * from ENTITY.
 */
#define COMPCURV_LEVEL 2


// COMPCURV declaration proper.

class DECL_KERN COMPCURV: public CURVE {

	// Record a COMPCURV as a compcurv.

	compcurv def;


// STI ROLL begin - added virtual compare function for api_get_modified_faces
protected:
    virtual logical bulletin_no_change_vf(ENTITY const* other, logical identical_comparator) const;
// STI ROLL end

	// Include the standard member functions for all entities.

	/**
	 * @nodoc
	 */
	ENTITY_FUNCTIONS( COMPCURV , KERN)
#if 0
; // semicolon needed for mkman (doc tool) parsing)
#endif
// Because mkman isn't aware that ENTITY_FUNCTIONS changes access to "public", do so
// explicitly here:
public:



	// Now the functions specific to COMPCURV.

	// Make a bare COMPCURV to be filled in later.

	COMPCURV();


	// Create a COMPCURV from a compcurv.

	COMPCURV( const compcurv & );


	// Data changing routine.  This routine checks 
	// that the record has been posted on the bulletin-board before 
	// performing the change.  If not, the routine provokes an error, 
	// so that the omission (forgetting to call backup() first) can 
	// be rectified in the program.  In production versions of the 
	// program, these checks may be disabled, to improve efficiency.

	void set_def( compcurv const & );


	// Return the curve equation for reading only.

	curve const &equation() const;


	// Return the curve equation for reading only.

	curve &equation_for_update();


	// Get a new (lower-case) curve being the compcurv of the 
	// COMPCURV, transformed if the given SPAtransf is non-null and 
	// reversed in sense if the logical is true.

	curve *trans_curve(
						SPAtransf const & = *(SPAtransf *)NULL_REF,
						logical = FALSE
					) const;


	// Transform the stored composite in place.

	void operator*=( SPAtransf const & );


	// Make a bounding SPAbox for this curve, surrounded by a set of loops
	// of edges. For the present we merely find the SPAbox containing the
	// whole underlying curve, ignoring the bounding edges. Provided
	// we keep the curve minimal, this is probably sufficient as a
	// long-term solution.

	// STI let (8/98): Added 4th arguement, to make this function signature
	// consistent with that of the virtual CURVE::makebox() method.

	SPAbox make_box( APOINT *, APOINT *, const SPAtransf *, double ) const;


	// lookup is done using CURVE::lookup()
	// int lookup( logical ) const;

	// Finish the attribute stuff.

	void add_surf_attributes( const EDGE * );

	// Do the remeshing, etc.

	void imprint_on_bodies( const EDGE * );

	// Functions for accessing and editing underlying data.

	ELEM1D *first_element();

	// Access to bounding boxes.

	par1_tree *boxes();

	// Compute the surface neighborhood of this curve at the given
	// SPAparameter value on the given surface.

	neighborhood get_neighborhood(
					double,					// Curve param
					MESHSURF *,				// Surface pointer
					REVBIT,					// Curve sense wrt face
					REVBIT,					// Surface sense wrt face
					evaluate_curve_side,	// Curve neighborhood

					const SPAtransf & = *(SPAtransf *)NULL_REF
											// Xform to get surf into curve
											// space
		) const;

	// Do a curve attribute merge.  Merge all of the curve
	// elements' attributes as well.

	void merge( COMPCURV *deleted_curve );

	// STI ROLL
	void full_size(SizeAccumulator& est, logical countSelf=TRUE) const;
    // STI ROLL
};

#endif
