/*******************************************************************/
/*    Copyright (c) 1989-2020 by Spatial Corp.                     */
/*    All rights reserved.                                         */
/*    Protected by U.S. Patents 5,257,205; 5,351,196; 6,369,815;   */
/*                              5,982,378; 6,462,738; 6,941,251    */
/*    Protected by European Patents 0503642; 69220263.3            */
/*    Protected by Hong Kong Patent 1008101A                       */
/*******************************************************************/
// Header for the TCOEDGE class.
// The tcoedge class is a derived class of coedge.
// The TCOEDGE extends the COEDGE class by adding SPAparameter bounds,
// a lazy 3D curve derived from the pcurve, and a SPAparameter SPAbox.
// Tcoedges on faces must maintain a pointer to a pcurve,
// which represents the curve underlying the edge in the parameteric
// space of the surface. Tcoedges on analytic surfaces are
// required to have pcurves.
// The edge of a tcoedge must be a tedge.
// The SPAparameter bounds of the tcoedge must be less than or equal
// to the bounds of the pcurve.
#ifndef TCOEDGE_CLASS
#define TCOEDGE_CLASS
#include "dcl_kern.h"
#include "tophdr.hxx"
#include "coedge.hxx"
#include "interval.hxx"
#include "en_macro.hxx"
#include "usecount.hxx"
/**
 * @file tcoedge.hxx
 * @CAA2Level L1
 * @CAA2Usage U1
 * \addtogroup ACISTOPOLOGY
 *
 * @{
 */
extern DECL_KERN int TCOEDGE_TYPE;
/**
 * Identifier that gives number of levels of derivation of this class
 * from ENTITY
 */
#define TCOEDGE_LEVEL 2
class SPAparameter;
class SPAposition;
class SPAbox;
class curve;
/**
 * @nodoc
 */
ENTITY_IS_PROTOTYPE( TCOEDGE , KERN )
#if 0
; // semicolon needed for mkman (doc tool) parsing
#endif

// TCOEDGE declaration proper
/**
 * Internal to <tt>ACIS</tt> and not intended for direct usage.
 * <br>
 * <b>Role:</b> Although this internal class is intended strictly for <tt>ACIS</tt> usage,
 * a minimal amount of information about this class is provided for the sole
 * purpose of being able to understand and trace restoration from a SAT file.
 * <br><br>
 * This class is used as part of the tolerant modeling technology. It is not
 * intended to be created directly. Instead, it could be created as part of model
 * translation and healing. It permits a coedge to be created and supported by
 * <tt>ACIS</tt> when the original data from translation or a corrupted model did not meet
 * the tighter tolerances of <tt>ACIS</tt>.
 * <br>
 * The tolerance coedge class, <tt>TCOEDGE</tt>, is a derived class of <tt>COEDGE</tt>. The <tt>TCOEDGE</tt>
 * extends the <tt>COEDGE</tt> class by adding parameter bounds, a lazy <tt>3D</tt> curve derived
 * from the pcurve, and a parameter box. <tt>TCOEDGEs</tt> on faces must maintain a pointer
 * to a pcurve, which represents the curve underlying the edge in the parametric
 * space of the surface. <tt>TCOEDGEs</tt> on analytic surfaces are required to have pcurves.
 * <br><br>
 * The edge of a <tt>TCOEDGE</tt> must be a <tt>TEDGE</tt>. The parameter bounds of the <tt>TCOEDGE</tt> must
 * be less than or equal to the bounds of the pcurve.
 * <br><br>
 * @see CURVE, SPAinterval.
 */
class DECL_KERN TCOEDGE: public COEDGE {

	// Tcoedge parametric bounding region (an SPAinterval).
	SPAinterval param_bound;

	// Contains the use counted box.
	entity_box_container box_container;

	// A lazy 3D representation derived from the pcurve.
	CURVE *curve_ptr;

	// This method is for internal use only.
	void set_geometry_ptr( PCURVE * );

// STI ROLL begin - added virtual compare function for api_get_modified_faces
protected:
    virtual logical bulletin_no_change_vf(ENTITY const* other, logical identical_comparator) const;

// STI ROLL end

	// Include the standard member functions for all entities.

	/**
	 * @nodoc
	 */
	ENTITY_FUNCTIONS( TCOEDGE , KERN)
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


	// Now the functions specific to TCOEDGE.

	// Basic constructor, used internally for bulletin board activity.
    /**
     * C++ allocation constructor requests memory for this object but does not populate it.
     * <br><br>
     * <b>Role:</b> The allocation constructor is used primarily by restore.
     * Applications should call this constructor only with the overloaded new
     * operator, because this reserves the memory on the heap, a requirement to
     * support roll back and history management.
     */
	TCOEDGE();
	TCOEDGE(const char* gme);


	// Public constructor, which initialises the record, and interfaces
	// with the bulletin board system.  The arguments initialise
	// edge_ptr (and indirectly partner_ptr), sense_data, next_ptr and
	// previous_ptr respectively.  Coedge back-pointers are also set in
	// the two argument coedges, but presently only validly if all the
	// coedges are part of a conventional simple loop.
    /**
     * C++ initialize constructor requests memory for this object and populates it with the data supplied as arguments.
     * <br><br>
     * <b>Role:</b> Applications should call this constructor only with the overloaded
     * <tt>new</tt> operator, because this reserves the memory on the heap, a requirement
     * to support roll back and history management.
     * <br><br>
     * @param edg
     * edge pointer.
     * @param sense
     * sense.
     * @param ne_coedge
     * next coedge.
     * @param prev_coedge
     * previous coedge.
     * @param region
     * bounding region.
     */
	TCOEDGE( EDGE * edg, REVBIT sense, COEDGE * ne_coedge, COEDGE * prev_coedge, SPAinterval region);
	TCOEDGE( const char* gme, EDGE * edg, REVBIT sense, COEDGE * ne_coedge, COEDGE * prev_coedge, SPAinterval region);

	// Data reading routines.

	// Routines for finding the positions of the start and end of
	// the edge. Used a lot, so convenient to have.
    /*
    // tbrv
    */
    /**
     * @nodoc
     */

	SPAposition start_pos() const;
	SPAposition gme_start_pos() const;
	/*
	// tbrv
	*/
	/**
	 * @nodoc
	 */

	SPAposition end_pos() const;
	SPAposition gme_end_pos() const;

	// These two functions return the lazy 3D curve of the tcoedge.
	/*
	// tbrv
	*/
	/**
	 * @nodoc
	 */

	CURVE *get_3D_curve();
	CURVE *gme_get_3D_curve();
	/*
	// tbrv
	*/
	/**
	 * @nodoc
	 */

	CURVE *get_3D_curve() const;
	CURVE *gme_get_3D_curve() const;
	/*
	// tbrv
	*/
	/**
	 * @nodoc
	 */

	SPAbox *bound() const { return box_container.get_box(); }

	CURVE *get_curve() const {return curve_ptr;}

	// Routines for finding the SPAparameter bounds of the coedge on its
	// curve. The first two are for backwards compatibility, the third
	// is to be preferred in cases where both parameters are needed
	// (which means almost always). They all save the evaluated range
	// in case it is wanted again, so cannot be declared const.
    /*
    // tbrv
    */
    /**
     * @nodoc
     */

	SPAparameter start_param() const;
	SPAparameter gme_start_param() const;
	/*
	// tbrv
	*/
	/**
	 * @nodoc
	 */

	SPAparameter end_param() const;
	SPAparameter gme_end_param() const;
	/*
	// tbrv
	*/
	/**
	 * @nodoc
	 */

	SPAinterval param_range() const;
	SPAinterval gme_param_range() const;

	// Data changing routines.  Each of these routines checks
	// that the record has been posted on the bulletin-board before
	// performing the change.  If not, the routine provokes an error,
	// so that the omission (forgetting to call backup() first) can
	// be rectified in the program.  In production versions of the
	// program, these checks may be disabled, to improve efficiency.

	// This function checks the given range with the start
	// and end vertexes if they do not agree it returns
	// FALSE and does not set the range.  If the range could
	// be set it returns TRUE
    /*
    // tbrv
    */
    /**
     * @nodoc
     */

	void set_param_range( SPAinterval const & );
	void gme_set_param_range( SPAinterval const & );
    /*
    // tbrv
    */
    /**
     * @nodoc
     */

	void set_bound( SPAbox *in_box ) { box_container.set_box( this, in_box); }

    // (We need the second SPAparameter only in order for agreement with
    // the COEDGE virtual function of the same name)
    /*
    // tbrv
    */
    /**
     * @nodoc
     */
	void set_geometry( PCURVE * , logical reset_pattern = TRUE);
	//void gme_set_geometry( PCURVE * , logical reset_pattern = TRUE);
    /*
    // tbrv
    */
    /**
     * @nodoc
     */
	void set_3D_curve( CURVE * );
	void gme_set_3D_curve( CURVE * );

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
