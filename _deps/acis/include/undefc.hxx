/*******************************************************************/
/*    Copyright (c) 1989-2020 by Spatial Corp.                     */
/*    All rights reserved.                                         */
/*    Protected by U.S. Patents 5,257,205; 5,351,196; 6,369,815;   */
/*                              5,982,378; 6,462,738; 6,941,251    */
/*    Protected by European Patents 0503642; 69220263.3            */
/*    Protected by Hong Kong Patent 1008101A                       */
/*******************************************************************/
// A undefined curve, a UNDEFC, is defined by a undefc which
// in turn records the start and end points, directions and curvatures.
/*******************************************************************/
#ifndef UNDEFC_CLASS
#define UNDEFC_CLASS
#include "dcl_kern.h"
#include "logical.h"
#include "curve.hxx"
#include "unddef.hxx"
/**
 * @file undefc.hxx
 * @CAA2Level L1
 * @CAA2Usage U1
 * \addtogroup ACISGEOMETRICENTITIES
 *
 * @{
 */
class SPAposition;
class SPAunit_vector;
class SPAtransf;
/**
 * @nodoc
 */
ENTITY_IS_PROTOTYPE(UNDEFC , KERN)
#if 0
; // semicolon needed for mkman (doc tool) parsing
#endif


// Identifier used to find out (via identity() defined below) to what
// an entity pointer refers.

extern DECL_KERN int UNDEFC_TYPE;


// Identifier that gives number of levels of derivation of this class
// from ENTITY.

#define UNDEFC_LEVEL 2


// UNDEFC declaration proper.
/**
 * Defines a curve that is undefined except for its end points as an object in the model.
 * <br>
 * <b>Role:</b> This class defines an undefined curve that records the start and
 * end points, directions, and curvatures. The start point has a parameter value
 * of <tt>0</tt> and the end point has a parameter value of <tt>1</tt>. No other points are defined.
 * <br><br>
 */
class DECL_KERN UNDEFC: public CURVE {

	// Record a UNDEFC as a undefc.

	undefc def;


// STI ROLL begin - added virtual compare function for api_get_modified_faces
protected:
    virtual logical bulletin_no_change_vf(ENTITY const* other, logical identical_comparator) const;
// STI ROLL end

	// Include the standard member functions for all entities.

	/**
	 * @nodoc
	 */
	ENTITY_FUNCTIONS( UNDEFC , KERN)
#if 0
; // semicolon needed for mkman (doc tool) parsing)
#endif
// Because mkman isn't aware that ENTITY_FUNCTIONS changes access to "public", we do so
// explicitly here:
public:


	/**
	 * @nodoc
	 */
	FULLSIZE_FUNCTION
#if 0
; // semicolon needed for mkman (doc tool) parsing)
#endif


	// Now the functions specific to UNDEFC.

	// Make a bare UNDEFC to be filled in later.
/**
* C++ allocation constructor requests memory for this object but does not populate it.
* The allocation constructor is used primarily by restore. Applications should call this constructor only with the overloaded <tt>new</tt> operator,
* because this reserves the memory on the heap, a requirement to support roll back and history management.
 */
	UNDEFC();


	// Create a UNDEFC line with given start and end conditions.
	/**
	* C++ initialize constructor requests memory for this object and populates it with the data supplied as arguments.
	* Applications should call this constructor only with the overloaded <tt>new</tt> operator, because this reserves
	* the memory on the heap, a requirement to support roll back and history management.
	*<br><br>
	* @param SPAposition
 	* start position.
 	* @param SPAunit_vector
 	* start direction.
 	* @param SPAvector
 	* start curvature.
 	* @param SPAposition
 	* end position.
 	* @param SPAunit_vector
 	* end direction.
 	* @param SPAvector
 	* end curvature.

	*/

	UNDEFC(
			SPAposition const &,
			SPAunit_vector const &,
			SPAvector const &,
			SPAposition const &,
			SPAunit_vector const &,
			SPAvector const &
		);

	// Create a UNDEFC from a undefc.
/**
* C++ copy constructor requests memory for this object and populates it with the data from the object supplied as an argument.
* Applications should call this constructor only with the overloaded <tt>new</tt> operator, because this reserves the memory
* on the heap, a requirement to support roll back and history management.
*<br><br>
* @param undefc
* undefc

*/
	UNDEFC( undefc const & );


	// Data reading routines.
/**
* Returns the point at the start of the curve.
*/
	SPAposition const &start_point() const { return def.start_point; }
/**
* Returns the direction at the start of the curve.
*/
	SPAunit_vector const &start_dir() const { return def.start_dir; }
/**
* Returns the curvature at the start of the curve.
*/

	SPAvector const &start_curv() const { return def.start_curv; }
/**
 * Returns the point at the end of the curve.
 */

	SPAposition const &end_point() const { return def.end_point; }
/**
* Returns the direction at the end of the curve.
*/
	SPAunit_vector const &end_dir() const { return def.end_dir; }
/**
* Returns the curvature at the end of the curve.
*/
	SPAvector const &end_curv() const { return def.end_curv; }


	// Data changing routines.  Each of these routines checks
	// that the record has been posted on the bulletin-board before
	// performing the change.  If not, the routine provokes an error,
	// so that the omission (forgetting to call backup() first) can
	// be rectified in the program.  In production versions of the
	// program, these checks may be disabled, to improve efficiency.
/**
*Sets the point at the start of the curve.
*/
	void set_start_point( SPAposition const & );
/**
*Sets the direction at the start of the curve.
*/
	void set_start_dir( SPAunit_vector const & );
/**
*Sets the curvature at the start of the curve.
*/
	void set_start_curv( SPAvector const & );
/**
*Sets the point at the end of the curve.
*/
	void set_end_point( SPAposition const & );
/**
*Sets the direction at the end of the curve.
*/
	void set_end_dir( SPAunit_vector const & );
/**
*Sets the curvature at the end of the curve.
*/
	void set_end_curv( SPAvector const & );

/**
* Returns the curve equation for reading only.
*/
	curve const &equation() const;


	// Return the curve equation, checking first for backup.

/**
*Returns the curve equation, checking first for backup.
*/
	curve &equation_for_update();


	// Get a new (lower-case) curve being the undefc of the
	// UNDEFC, transformed if the given SPAtransf is non-null and
	// reversed in sense if the logical is true.

/**
* Returns a new curve, which is the undefc of the <tt>UNDEFC</tt>, transformed if the given transformation is not <tt>NULL</tt>.
* This curve is reversed in sense if logical is <tt>TRUE</tt>.
* <br><br>
* @param SPAtransf
* transformation.
* @param logical
* reverse curve.

*/
	curve *trans_curve(
					SPAtransf const & = *(SPAtransf *)NULL_REF,
					logical = FALSE
				) const;


	// Transform the stored undefc in place.
/**
* Transforms the stored undefc in place.
* <br><br>
* @param SPAtransf
* transformation.
*/
	void operator*=( SPAtransf const & );


	// The generic CURVE version of make_box is good enough
	// so the version for undefc is left in abeyance.

	// SPAbox make_box( APOINT *, APOINT *, SPAtransf const * ) const;


	// lookup is done using CURVE::lookup()
	// int lookup( logical ) const;
};
/** @} */
#endif
