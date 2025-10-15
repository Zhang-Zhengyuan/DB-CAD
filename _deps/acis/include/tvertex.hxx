/*******************************************************************/
/*    Copyright (c) 1989-2020 by Spatial Corp.                     */
/*    All rights reserved.                                         */
/*    Protected by U.S. Patents 5,257,205; 5,351,196; 6,369,815;   */
/*                              5,982,378; 6,462,738; 6,941,251    */
/*    Protected by European Patents 0503642; 69220263.3            */
/*    Protected by Hong Kong Patent 1008101A                       */
/*******************************************************************/
// Header for the TVERTEX class.
// The tvertex class is a derived class of vertex with a tolerance value.
// A tvertex is a 0-dimensional, topological entity that is used
// to bound an edge or a tedge. Each tvertex is represented by a point
// in the geometric model. The tolerance value indicates the maximum
// distance from the SPAposition of the tvertex to the end of each coedge.
// Two tvertices are coincident if the distance between their points is
// less than the maximum of their tolerance values. If the tolerance value
// is less than SPAresabs, then SPAresabs is used for coincidence checking.
// A tvertex and a vertex are coincident if the distance between their points is
// less than the maximum of the tvertex tolerance value and SPAresabs.
#if !defined( TVERTEX_CLASS )
#define TVERTEX_CLASS
#include "dcl_kern.h"
#include "usecount.hxx"
#include "tophdr.hxx"
#include "vertex.hxx"
#include "en_macro.hxx"
/**
 * @file tvertex.hxx
 * @CAA2Level L1
 * @CAA2Usage U1
 * \addtogroup ACISTOPOLOGY
 *
 * @{
 */

/**
 * @nodoc
 */
ENTITY_IS_PROTOTYPE( TVERTEX , KERN )
#if 0
; // semicolon needed for mkman (doc tool) parsing
#endif
/**
 * @nodoc
 * Identifier used to find out (via identity() defined below) to what
 * an entity pointer refers.
 */
extern DECL_KERN int TVERTEX_TYPE;
/**
 * Identifier that gives number of levels of derivation of this class
 * from ENTITY
 */
#define TVERTEX_LEVEL 2
/**
 * Internal to <tt>ACIS</tt> and not intended for direct usage.
 * <br>
 * <b>Role:</b> Although this internal class is intended strictly for <tt>ACIS</tt> usage,
 * a minimal amount of information about this class is provided for the sole purpose
 * of being able to understand and trace restoration from a <tt>SAT</tt> file.
 * <br><br>
 * The tolerant vertex class, <tt>TVERTEX</tt>, is a derived class of <tt>VERTEX</tt> with a tolerance
 * value. A tolerant vertex is a zero-dimensional, topological entity that is used
 * to bound an edge or a tedge. Each tolerant vertex is represented by a point
 * in the geometric model. The tolerance value indicates the maximum distance
 * from the position of the tvertex to the end of each coedge.
 * <br><br>
 * Two tolerant vertices are coincident if the distance between their points
 * is less than the maximum of their tolerance values. If the tolerance value is
 * less than resabs, then resabs is used for coincidence checking.
 * <br><br>
 * A tolerant vertex and a vertex are coincident if the distance between their
 * points is less than the maximum of the tolerant vertex tolerance value and resabs.
 * <br><br>
 * This class is used as part of the tolerant modeling technology. It is not intended
 * to be created directly. Instead, it could be created as part of model translation
 * and healing. It permits a vertex to be created and supported by <tt>ACIS</tt> when the
 * original data from translation or a corrupted model did not meet the tighter
 * tolerances of <tt>ACIS</tt>.
 */
class DECL_KERN TVERTEX: public VERTEX {

protected:

// STI ROLL begin - added virtual compare function for api_get_modified_faces
protected:
    virtual logical bulletin_no_change_vf(ENTITY const* other, logical identical_comparator) const;
// STI ROLL end

	// Include the standard member functions for all entities.

	/**
	 * @nodoc
	 */
	ENTITY_FUNCTIONS( TVERTEX , KERN)
#if 0
; // semicolon needed for mkman (doc tool) parsing)
#endif
// Because mkman isn't aware that ENTITY_FUNCTIONS changes access to "public", we do so
// explicitly here:
public:

	/**
	 * @nodoc
	 */
	TRANSFORM_FUNCTION
#if 0
; // semicolon needed for mkman (doc tool) parsing)
#endif


	// Search a private list for this object, used for debugging.

	/**
	 * @nodoc
	 */
	LOOKUP_FUNCTION
#if 0
; // semicolon needed for mkman (doc tool) parsing)
#endif


	// Now the functions specific to TVERTEX.

	// Basic constructor, used internally for bulletin board activity.

	TVERTEX();
	TVERTEX(const char *gme );


	// Public constructor, which initialises the record and interfaces
	// with the bulletin board.  Increments the point's use count to
	// reflect this new use.

	TVERTEX( APOINT *, double );
	TVERTEX( const char *gme, APOINT *, double );

	// Data reading routines.
	double get_tolerance() const;
	double gme_get_tolerance() const;

	double get_tolerance();
	double gme_get_tolerance();

    /**
     * @nodoc
     */
    logical is_tolerant() const {return TRUE;}

	//double get_curr_tolerance() const;
	//double get_curr_tolerance();

	// Returns the update value.
	logical get_update() const {return (tolerance < 0.0);}

	// Data changing routines.  Each of these routines checks
	// that the record has been posted on the bulletin-board before
	// performing the change.  If not, the routine provokes an error,
	// so that the omission (forgetting to call backup() first) can
	// be rectified in the program.  In production versions of the
	// program, these checks may be disabled, to improve efficiency.

	void set_tolerance( double, logical = TRUE );
	void gme_set_tolerance( double, logical = TRUE );

	void set_update( logical flag = TRUE );
	void gme_set_update( logical flag = TRUE );
	void mark_to_update() {set_update(TRUE);}

// STI jmb: Handle save/restore of use counted histories
	/**
	 * @nodoc
	 */
	USE_COUNT_REFERENCE_DECL
#if 0
; // semicolon needed for mkman (doc tool) parsing)
#endif
// STI jmb: end

};

/** @} */
#endif

