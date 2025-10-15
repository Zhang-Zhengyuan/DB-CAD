/*******************************************************************/
/*    Copyright (c) 1989-2020 by Spatial Corp.                     */
/*    All rights reserved.                                         */
/*    Protected by U.S. Patents 5,257,205; 5,351,196; 6,369,815;   */
/*                              5,982,378; 6,462,738; 6,941,251    */
/*    Protected by European Patents 0503642; 69220263.3            */
/*    Protected by Hong Kong Patent 1008101A                       */
/*******************************************************************/
// Header file describing the modeller data structures
// The CURVE class provides the framework for the range of object-space
// curve types defined at any one time in the system.  As with surfaces,
// it provides a use count, to allow multiple use of curve descriptions,
// and a range of virtual functions to access specific geometries.
#ifndef CURVE_CLASS
#define CURVE_CLASS
#include "dcl_kern.h"
#include "logical.h"
#include "en_macro.hxx"
#include "usecount.hxx"
/**
 * @file curve.hxx
 * @CAA2Level L1
 * @CAA2Usage U2
 * \addtogroup ACISGEOMETRICENTITIES
 *
 * @{
 */
class EDGE;
class APOINT;
class curve;
class SPAtransf;
class SPAbox;
class pattern_holder;
class pattern;
class VOID_LIST;
class SPAinterval;
/**
 * @nodoc
 */
ENTITY_IS_PROTOTYPE( CURVE, KERN )
#if 0
; // semicolon needed for mkman (doc tool) parsing
#endif

// STI ROLL
class SizeAccumulator;
// STI ROLL

// Generic curve record

// Unique identifier for identity().
/*
// tbrv
*/
/**
 * @nodoc
 */
extern DECL_KERN int CURVE_TYPE;
/**
 * Identifier that gives number of levels of derivation of this class
 * from ENTITY
*/
#define CURVE_LEVEL 1


// CURVE declaration proper
/**
 * Defines a generic curve as an object in the model.
 * <br>
 * <b>Role:</b> <tt>CURVE</tt> is a model geometry class that contains a pointer to a (lowercase)
 * curve, the corresponding construction geometry class. In general, a model geometry
 * class is derived from <tt>ENTITY</tt> and is used to define a permanent model object. It provides
 * model management functionality, in addition to the geometry definition.
 * <br><br>
 * The <tt>CURVE</tt> class provides the basic framework for the range of curve geometries
 * implemented at any time in the modeler. Additional classes are derived from <tt>CURVE</tt> to
 * define specific types of curves, such as <tt>COMPCURV</tt>, <tt>ELLIPSE</tt>, <tt>INTCURVE</tt>, and <tt>STRAIGHT</tt>.
 * <br><br>
 * Along with the usual <tt>ENTITY</tt> class methods, <tt>CURVE</tt> has member methods to provide
 * access to specific implementations of the geometry. For example, a curve can be
 * transformed by a given transform operator.
 * <br><br>
 * A use count allows multiple references to a <tt>CURVE</tt>. The construction of a new
 * <tt>CURVE</tt> initializes the use count to 0. Methods are provided to increment and decrement
 * the use count, and after the use count returns to 0, the entity is deleted.
 * @see ENTITY, EDGE, PCURVE, TCOEDGE
 */

class DECL_KERN CURVE: public ENTITY {

private:

	// Use count for record, allowing multiple use.  Starts at zero, and
	// if ever it reaches zero again, the record can be deleted.
	// Holds the number of edges referencing the CURVE.

// STI jmb: Handle save/restore of use counted histories
// 	unsigned int use_count_data;
// STI jmb: end

    // Track the ENTITYs owning the CURVE
private:
    ENTITY** owners_ptr;
    int num_owners;
    int num_owners_alloc;

protected:
/**
 * Returns TRUE if this entity can have a pattern_holder. For internal use only.
 */
	logical holds_pattern() const {return TRUE;}
public:
/**
 * Adds the <tt>owner</tt> argument to the list of owners.
 * <br><br>
 * @param owner
 * owner to be added.
 * @param incr
 * flag to increment use count or not.
 */
    void add_owner(ENTITY* owner, logical incr = TRUE);
/**
 * Removes the <tt>owner</tt> argument from the list of owners.
 * <br><br>
 * @param owner
 * owner to be removed.
 * @param decr
 * flag to decrement the use count.
 * @param lose
 * flag to lose if use count is zero.
 */
    void remove_owner(ENTITY *owner, logical decr = TRUE, logical lose = TRUE);
/**
 * Copies the list of owners of this <tt>CURVE</tt> to the <tt>list</tt> argument.
 * <br><br>
 * <b>Role:</b> Returns the number of owners copied.
 * <br><br>
 * @param list
 * list of owners.
 */
    int get_owners(ENTITY_LIST& list) const;
	
/**
 * Returns the first owner of this <tt>CURVE</tt>.
 * <br><br>
 * <b>Role:</b> If there are no owners, NULL is returned.
 */
	ENTITY *owner() const;


// STI ROLL begin - added virtual compare function for api_get_modified_faces
protected:
/**
 * @nodoc
 */
    virtual logical bulletin_no_change_vf(ENTITY const* other, logical identical_comparator) const;
// STI ROLL end

	// Include the standard member functions for all entities.

	/**
	 * @nodoc
	 */
	ENTITY_FUNCTIONS( CURVE , KERN)
#if 0
; // semicolon needed for mkman (doc tool) parsing)
#endif
// Because mkman isn't aware that ENTITY_FUNCTIONS changes access to "public", do so
// explicitly here:
public:

	/**
	 * @nodoc
	 */
	TRANSFORM_FUNCTION
#if 0
; // semicolon needed for mkman (doc tool) parsing)
#endif

	// Indicate whether this entity is normally destroyed by lose(),
	// or whether it gets destroyed implicitly when every owner has
	// been lost. Most entities are destroyed explicitly using lose(),
	// and so the default returns TRUE.

/**
 * Indicates whether this <tt>CURVE</tt> is normally destroyed by <tt>lose</tt>.
 * <br><br>
 * <b>Role:</b> The method returns <tt>FALSE</tt>, indicating that <tt>CURVEs</tt> are, by default,
 * shared among multiple owners by means of use-count tracking, and so are destroyed
 * implicitly when every owner has been lost.  Classes derived from <tt>CURVE</tt> may of
 * course override this behavior.
 */
	virtual logical deletable() const;


	// Now the functions specific to CURVE.

	// Make a generic curve. Usually created in one of its derived
	// incarnations, but can be made stand-alone when reading in a
	// saved file containing unrecognised sub-classes of CURVE.
	// Initialises the use count to zero.
/**
 * Constructs a <tt>CURVE</tt> (default constructor).
 * <br><br>
 * <b>Role:</b> Requests memory for this object but does not populate it. The
 * allocation constructor is used primarily by restore. Applications should call
 * this constructor only with the overloaded <tt>new</tt> operator, because this
 * reserves the memory on the heap, a requirement to support roll back and history
 * management.
 */
	CURVE();

// This function is hidden from mkman in the RESTORE_DEF macro; to have it documented,
// we include it here:
#if 0
/**
 * Restores this <tt>CURVE</tt> from a SAT file.
 * <br><br>
 * <b>Role:</b> The <tt>RESTORE_DEF</tt> macro expands to the <tt>restore_common</tt> method,
 * which is used in reading information from a SAT file. This method is never called
 * directly. It is called by a higher hierarchical function if an item in the SAT file
 * is determined to be of this class type. An instance of this class will already have
 * been created through the allocation constructor. This method then populates the class
 * instance with the appropriate data from the SAT file.
 * <pre>
 *    if (restore_version_number >= PATTERN_VERSION
 *       read_ptr  APATTERN index
 *       if (apat_idx != (APATTERN*)(-1)))
 *          restore_cache();
 *    if (!get_standard_save_flag())
 *       read_int           //   use count data </pre>
 */
    void restore_common();
#endif
	// Data access

// STI jmb: Handle save/restore of use counted histories
// 	int use_count() const { return use_count_data; }
//
//
// 	// Use count manipulation. Either add or subtract one use, and if
// 	// subtraction causes the use count to fall to zero, then delete
// 	// the record.
//
// 	void add();
// 	void remove();
	/**
	 * @nodoc
	 */
	USE_COUNTED_DECL
#if 0
; // semicolon needed for mkman (doc tool) parsing)
#endif
// STI jmb: end

// These function are hidden from mkman in the ENTITY_FUNCTIONS macro; to have them documented,
// we include them here:
#if 0
/**
 * Returns a type identifier for this object.
 * <br><br>
 * <b>Role:</b> If <tt>level</tt> is unspecified or 0, returns the type identifier <tt>CURVE_TYPE</tt>. If
 * <tt>level</tt> is specified, returns <tt>CURVE_TYPE</tt> for that level of derivation from <tt>ENTITY</tt>. The
 * level of this class is defined as <tt>CURVE_LEVEL</tt>.
 * <br><br>
 * @param level
 * the derivation level at which the object is to be identified.
 */
	virtual int identity( int level = 0 ) const;
/**
 * Returns the string <tt>"curve"</tt>.
 */
	virtual const char *type_name() const;
/**
 * Posts a delete bulletin to the bulletin board indicating this <tt>CURVE</tt> is no longer used in the active model.
 * <br><br>
 * <b>Role:</b> The <tt>lose</tt> methods for attached attributes are also called.
 */
	virtual void lose();
#endif

// These functions is hidden from mkman in the USE_COUNTED_DECL macro; to have them documented,
// we include them here:
#if 0
/**
 * Increments the use count of this <tt>CURVE</tt>.
 * <br><br>
 * <b>Role:</b> Before performing the change, it checks if the data structure is
 * posted on the bulletin board. If not, the method calls <tt>backup</tt> to put
 * an entry on the bulletin board.
 */
	virtual void add( );
/**
 * Indicates if this <tt>CURVE</tt> is use-counted or not.
 * <br><br>
 * <b>Role:</b> Returns <tt>TRUE</tt> for <tt>CURVEs</tt> unless overridden in a derived class.
 */
	virtual logical is_use_counted() const;
/**
 * Decrements the use count of this <tt>CURVE</tt>.
 * <br><br>
 * <b>Role:</b> If the use count reaches 0, the <tt>CURVE</tt> is deleted. Before performing
 * the change, it checks if the data structure is posted on the bulletin board. If not,
 * the method calls <tt>backup</tt> to put an entry on the bulletin board.
 * <br><br>
 * @param lose_if_zero
 * flag to lose if use count is zero.
 */
	virtual void remove( logical lose_if_zero = TRUE );
/**
 * Sets the reference use count of this <tt>CURVE</tt>.
 * <br><br>
 * <b>Role:</b> This is used by the <tt>lose</tt> method. Refinements are not
 * destructed until the use count goes to zero.
 * <br><br>
 * @param val
 * new use count.
 */
	virtual void set_use_count(int val);
/**
 * Returns the use count of this <tt>CURVE</tt>.
 */
	virtual int use_count() const;
#endif

	//
	// DEBUG FUNCTIONS:
	//

	// method to write scheme commands to file for visualization
	// [in] pFp   - the file to write
	// [in] Color - color of the entity drawn
	//
/**
 * @nodoc
 */
	virtual void debug_scheme( FILE *pFp, int Color ) const;

	// method to write scheme commands to file for visualization
	// [in] pFp   - the file to write
	// [in] Color - color of the entity drawn
	// [in] Range - the relevant range of the curve to be plotted
	//
/**
 * @nodoc
 */
	virtual void debug_scheme_range( FILE *pFp, int Color, SPAinterval Range ) const;



	// Return the curve equation for reading only.
	// Will be NULL for a generic CURVE.
/**
 * Returns the equation of this <tt>CURVE</tt>, for reading only, or <tt>NULL</tt> for a generic <tt>CURVE</tt>.
 */
	virtual const curve &equation() const;


	// Return the curve equation.
/**
 * Returns the equation of this <tt>CURVE</tt> for update operations.
 * <br><br>
 * <b>Role:</b> Before a change
 * is performed, the bulletin board is checked to see if the data structure
 * has been posted. If not, <tt>backup</tt> is called to put an entry on the bulletin board.
 */
	virtual curve &equation_for_update();


	// Transform a curve (action taken only by derived classes).
/**
 * Returns the transformed curve.
 * <br><br>
 * <b>Role:</b> If the logical argument is <tt>TRUE</tt>, the returned curve is reversed.
 * <br><br>
 * @param t
 * transform to apply.
 * @param reverse
 * flag to reverse the curve.
 */
	virtual curve* trans_curve(
		const SPAtransf &t = *(SPAtransf*)NULL_REF, logical reverse = FALSE) const;
/**
 * Transforms the equation of this <tt>CURVE</tt>.
 * <br><br>
 * <b>Role:</b> Before performing the change, it checks if the data structure
 * is posted on the bulletin board. If not, the method calls <tt>backup</tt>
 * to put an entry on the bulletin board.
 * <br><br>
 * @param t
 * transform to apply.
 */
	virtual void operator*=( const SPAtransf &t );


	// Determine a bounding SPAbox for the portion of the curve through two
	// points.  (The curve definition must be such as to be able to
	// determine uniquely the portion lying between any two points lying
	// on it).
/**
 * Determines a bounding box for the portion of this <tt>CURVE</tt> delimited by two specified points.
 * <br><br>
 * <b>Role:</b> The curve definition must be such as to be able to determine uniquely
 * the portion lying between any two points lying on it.
 * <br><br>
 * @param start
 * first point on the curve.
 * @param end
 * second point on the curve.
 * @param t
 * transform.
 * @param tol
 * tolerance.
 */
	virtual SPAbox make_box(
        APOINT *start, APOINT *end, const SPAtransf *t , double tol = 0.0) const;

    // STI swa 19Jul02 - New method for detection of clash with SPAbox.
    //                   Note, the default methods will assume the
    //                   SPAbox and surface interfere when no test is
    //                   available.
/*
// tbrv
*/
/**
 * @nodoc
 */
    virtual logical box_clash(const SPAbox &test_box,
                              const SPAtransf &surf_transf = *(SPAtransf*)NULL_REF,
                              double tol = SPAresabs) const;

	// Routines used for system debugging and instrumentation.
/**
 * @nodoc
 */
	virtual int lookup( logical ) const;

	// STI ROLL
	protected:
/**
 * @nodoc
 */
	void full_size(SizeAccumulator& est, logical = TRUE) const;
	// STI ROLL
};
/** @} */
#endif
