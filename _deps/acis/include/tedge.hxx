/*******************************************************************/
/*    Copyright (c) 1989-2020 by Spatial Corp.                     */
/*    All rights reserved.                                         */
/*    Protected by U.S. Patents 5,257,205; 5,351,196; 6,369,815;   */
/*                              5,982,378; 6,462,738; 6,941,251    */
/*    Protected by European Patents 0503642; 69220263.3            */
/*    Protected by Hong Kong Patent 1008101A                       */
/*******************************************************************/
// Header for the TEDGE class.
// The tolerant edge class is a derived class of edge with a tolerace value.
// Unlike an edge, whose 3D representation is provided by its underlying
// curve, the 3D representation of a tedge is defined by the 3D geometry
// of its associated tcoedges.  The curve underlying a tedge is used purely
// for graphic visualization.
// The tolerance value of a tedge indicates the maximum distance between
// any two equiparametric positions on any of its tcoedges.
// Two tedges are coincident over an SPAinterval if the maximum of the minimum distance
// between the portion of their point sets bounded by the SPAinterval is
// less than the maximum of the tedge's tolerance values. If the tolerance value
// is less than SPAresabs, then SPAresabs is used for coincidence checking.
// A tedge and an edge are coincident over an SPAinterval if the maximum
// of the minimum distance between the portion of their point sets bounded
// by the SPAinterval is less than the maximum of the tedge's tolerance value
// and SPAresabs.
// A tedge with a single tcoedge will have a zero tolerance value.
// A tedge may only be associated with tcoedges.
#if !defined( TEDGE_CLASS )
#define TEDGE_CLASS
#include "dcl_kern.h"
#include "tophdr.hxx"
#include "edge.hxx"
#include "tcoedge.hxx"
#include "vertex.hxx"
#include "tvertex.hxx"
#include "en_macro.hxx"
#include "usecount.hxx"
/**
 * @file tedge.hxx
 * @CAA2Level L1
 * @CAA2Usage U1
 * \addtogroup ACISTOPOLOGY
 *
 * @{
 */
// Identifier used to find out (via identity() defined below) to what
// an entity pointer refers.
extern DECL_KERN int TEDGE_TYPE;
/**
 * Identifier that gives number of levels of derivation of this class
 * from ENTITY
 */
#define TEDGE_LEVEL 2
/**
 * @nodoc
 */
ENTITY_IS_PROTOTYPE( TEDGE , KERN )
#if 0
; // semicolon needed for mkman (doc tool) parsing
#endif

// TEDGE declaration proper
/**
 *
 * Although this internal class is intended strictly for <tt>ACIS</tt> usage,
 * a minimal amount of information about this class is provided for the sole
 * purpose of being able to understand and trace restoration from a SAT file.
 * <br><br>
 * <b>Role</b>: The tolerant edge class is a derived class of <tt>EDGE</tt> with a tolerance value.
 * Unlike an edge, whose <tt>3D</tt> representation is provided by its underlying curve,
 * the <tt>3D</tt> representation of a <tt>TEDGE</tt> is defined by the <tt>3D</tt> geometry of its associated
 * <tt>TCOEDGEs</tt>. The curve underlying a tolerant edge is used purely for graphic visualization.
 * <br><br>
 * The tolerance value of a tolerant edge indicates the maximum distance between
 * any two equiparametric positions on any of its tolerant coedges. Two tolerant
 * edges are coincident over an interval if the maximum of the minimum distance
 * between the portion of their point sets bounded by the interval is less than
 * the maximum of the tolerant edge's tolerance values. If the tolerance value is
 * less than <tt>SPAresabs</tt>, then <tt>SPAresabs</tt> is used for coincidence checking.
 * <br><br>
 * A tolerant edge and an edge are coincident over an interval if the maximum of
 * the minimum distance between the portion of their point sets bounded by the
 * interval is less than the maximum of the tolerant edge's tolerance value and
 * <tt>SPAresabs</tt>.
 * <br><br>
 * A tolerant edge with a single tolerant coedge will have a zero tolerance value.
 * A tolerant edge may only be associated with tolerant coedges.
 * <br><br>
 * This class is used as part of the tolerant modeling technology. It is not
 * intended to be created directly. Instead, it could be created as part of model
 * translation and healing. It permits an edge to be created and supported by <tt>ACIS</tt>
 * when the original data from translation or a corrupted model did not meet the
 * tighter tolerances of <tt>ACIS</tt>.
 */
class DECL_KERN TEDGE: public EDGE {

// STI ROLL begin - added virtual compare function for api_get_modified_faces
protected:
    virtual logical bulletin_no_change_vf(ENTITY const* other, logical identical_comparator) const;
// STI ROLL end

	// Include the standard member functions for all entities.

	/**
	 * @nodoc
	 */
	ENTITY_FUNCTIONS( TEDGE , KERN)
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


	// Now the functions specific to TEDGE.

	// Basic constructor, used internally for bulletin board activity.
    /**
     * C++ allocation constructor requests memory for this object but does not populate it.
     * <br><br>
     * <b>Role</b>: The allocation constructor is used primarily by restore. Applications
     * should call this constructor only with the overloaded <tt>new</tt> operator, because
     * this reserves the memory on the heap, a requirement to support roll back
     * and history management.
     */
	TEDGE();
	TEDGE( const char *gme );

	// Public constructor which initialises the record and interfaces
	// with the bulletin board system.  The arguments initialise
	// start_ptr, end_ptr, geometry_ptr and sense_data respectively.
	// If necessary, the edge pointers in the two end vertices are set,
	// and the use count in the curve geometry is incremented.
    /**
     * C++ initialize constructor requests memory for this object and populates it with the data supplied as arguments.
     * <br><br>
     * <b>Role</b>: Applications should call this constructor only with the overloaded
     * <tt>new</tt> operator, because this reserves the memory on the heap, a requirement to
     * support roll back and history management.
     * <br><br>
     * @param start_ptr
     * start pointer.
     * @param end_ptr
     * end pointer.
     * @param curv
     * curve on which edge lies.
     * @param edg
     * edge sense.
     * @param edg_cvx
     * edge convexity.
     * @param tol
     * tolerance.
     */

	TEDGE( VERTEX * start_ptr, VERTEX * end_ptr, CURVE * curv, REVBIT edg , EDGE_cvty edg_cvx, double tol );
	TEDGE( const char *gme, VERTEX * start_ptr, VERTEX * end_ptr, CURVE * curv, REVBIT edg , EDGE_cvty edg_cvx, double tol );

	// Data reading routines.

	// Returns the tolerance value associated with the tedge.
	/*
	// tbrv
	*/
	/**
	 * @nodoc
	 */
	double get_tolerance() const;
	double gme_get_tolerance() const;
    /*
    // tbrv
    */
    /**
     * @nodoc
     */
	double get_tolerance();
	double gme_get_tolerance();

	// Extended tolerance routines which return the current value of the
	// tolerance that should be used.  These routines will return either the
	// tolerance of the edge or resabs/2, whichever is the greater.
    /*
    // tbrv
    */
    /**
     * @nodoc
     */
	//double get_curr_tolerance() const;
	//double get_curr_tolerance();
    
    /**
     * @nodoc
     */
    logical is_tolerant() const {return TRUE;}

	// Returns the update value.
	/**
	 * Update the tolerance on any version 5.0 part.
	 */
	logical get_update() const {return (tolerance < 0.0);}

	// Returns the 3D curve associated with a particular tcoedge.
	// Will regenerate the 3D curve if necessary
	/*
	// tbrv
	*/
	/**
	 * @nodoc
	 */

	CURVE *get_3D_curve(TCOEDGE *tce) {return tce->get_3D_curve();}

	// Returns the 3D curve associated with a particular tcoedge
	// as indicated by the specified face.
	// Will regenerate the 3D curve if necessary
	/*
	// tbrv
	*/
	/**
	 * @nodoc
	 */

	CURVE *get_3D_curve(FACE *);
	CURVE *gme_get_3D_curve(FACE *);

	// Data changing routines.  Each of these routines checks
	// that the record has been posted on the bulletin-board before
	// performing the change.  If not, the routine provokes an error,
	// so that the omission (forgetting to call <tt>backup()</tt> first) can
	// be rectified in the program.  In production versions of the
	// program, these checks may be disabled, to improve efficiency.
    /*
    // tbrv
    */
    /**
     * @nodoc
     */

	void set_tolerance( double, logical = TRUE );
	void gme_set_tolerance( double, logical = TRUE );
    /**
     * Indicates that the tolerance needs to be recalculated.
     * <br><br>
     * <b>Role</b>: Checks that the record has been posted on the bulletin board
     * before performing the change. If not, the routine provokes an error, so
     * that the omission (forgetting to call <tt>backup()</tt> first) can be rectified in
     * the program. In production versions of the program, these checks may be
     * disabled, to improve efficiency.
     * <br><br>
     * @param flag
     * update flag.
     */
	void set_update( logical flag = TRUE );
	void gme_set_update( logical flag = TRUE );
    /**
     * Calls the <tt>set_update</tt> routine.
     */
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
