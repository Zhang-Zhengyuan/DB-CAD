/* ORIGINAL: acis2.1/kerndata/geom/spline.hxx */
/* $Id: spline.hxx,v 1.12 2002/08/09 17:15:17 jeff Exp $ */
/*******************************************************************/
/*    Copyright (c) 1989-2020 by Spatial Corp.                     */
/*    All rights reserved.                                         */
/*    Protected by U.S. Patents 5,257,205; 5,351,196; 6,369,815;   */
/*                              5,982,378; 6,462,738; 6,941,251    */
/*    Protected by European Patents 0503642; 69220263.3            */
/*    Protected by Hong Kong Patent 1008101A                       */
/*******************************************************************/

// Header for SPLINE.

// A SPLINE, a class derived from SURFACE, records a parametric
// surface as a (lower-case) spline.  In turn a spline holds a
// pointer to a spl_sur and a logical denoting reversal of the
// sense of the stored surface.  A spl_sur contains a use-count
// (as does the SURFACE) and a pointer to the detailed parametric
// surface description.


#ifndef SPLINE_CLASS
#define SPLINE_CLASS

#include "dcl_kern.h"
#include "surface.hxx"

#include "spldef.hxx"

/**
* @file spline.hxx
 * @CAA2Level L1
 * @CAA2Usage U1
 * \addtogroup ACISGEOMETRICENTITIES
 *
 * @{
 */

class LOOP;
class SPAtransf;
class SPAbox;
/**
 * @nodoc
 */
ENTITY_IS_PROTOTYPE(SPLINE, KERN)
#if 0
; // semicolon needed for mkman (doc tool) parsing
#endif


// Identifier used to find out (via identity() defined below) to what
// an entity pointer refers.

extern DECL_KERN int SPLINE_TYPE;

/**
 * Identifier that gives number of levels of derivation of this class
 * from ENTITY.
 */
#define SPLINE_LEVEL 2
 /**
  * Defines a spline as an object in the model.
  * <br>
  * <b>Role:</b> <tt>SPLINE</tt> is a model geometry class that contains a pointer to a
  * (lowercase) <tt>spline</tt>, the corresponding construction geometry class. In general,
  * a model geometry class is derived from <tt>ENTITY</tt> and is used to define a permanent
  * model object. It provides model management functionality, in addition to the
  * geometry definition.
  * <br><br>
  * <tt>SPLINE</tt> is one of several classes derived from <tt>SURFACE</tt> to define a specific type of
  * surface. The spline class holds a pointer to a <tt>spl_sur</tt> and a logical denoting the
  * sense of the stored surface. A <tt>spl_sur</tt> also contains a use count and a detailed
  * parametric surface description.
  * <br><br>
  * Along with the usual <tt>SURFACE</tt> and <tt>ENTITY</tt> class methods, <tt>SPLINE</tt> has member methods
  * to provide access to specific implementations of the geometry. For example, a spline
  * can be transformed, resulting in another surface. All access to the surface data is
  * through methods for the spline class.
  * <br><br>
  * A use count allows multiple references to a <tt>SPLINE</tt>. The construction of a new SPLINE
  * initializes the use count to 0. Methods are provided to increment and decrement the
  * use count, and after the use count returns to 0, the entity is deleted.
  * @see spline
  */
class DECL_KERN SPLINE : public SURFACE {

    // Record a SPLINE as a spline.

    spline def;

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
    ENTITY_FUNCTIONS(SPLINE, KERN)
#if 0
        ; // semicolon needed for mkman (doc tool) parsing)
#endif
// Because mkman isn't aware that ENTITY_FUNCTIONS changes access to "public", we do so
// explicitly here:
public:

    // Now the functions specific to SPLINE.

    // Make a bare SPLINE to be filled in later.
/**
 * Constructs a <tt>SPLINE</tt> (default constructor).
 * <br><br>
 * <b>Role:</b> Requests memory for this object but does not populate it. The
 * allocation constructor is used primarily by restore. Applications should call
 * this constructor only with the overloaded <tt>new</tt> operator, because this
 * reserves the memory on the heap, a requirement to support roll back and history
 * management.
 */
    SPLINE();

    // Create a SPLINE from a spline.
/**
 * Constructs a <tt>SPLINE</tt> from the specified spline.
 * <br><br>
 * <b>Role:</b> Requests memory for this object and populates it with the data
 * supplied as the argument. Applications should call this constructor only with
 * the overloaded <tt>new</tt> operator, because this reserves the memory on the heap,
 * a requirement to support roll back and history management.
 * <br><br>
 * @param spl
 * spline to be wrapped by the constructed SPLINE.
 */
    SPLINE(const spline& spl);

    // These function are hidden from mkman in the ENTITY_FUNCTIONS macro; to have them documented,
    // we include them here:
#if 0
/**
 * Returns a type identifier for this object.
 * <br><br>
 * <b>Role:</b> If <tt>level</tt> is unspecified or 0, returns the type identifier <tt>SPLINE_TYPE</tt>. If
 * <tt>level</tt> is specified, returns <tt>SPLINE_TYPE</tt> for that level of derivation from <tt>ENTITY</tt>. The
 * level of this class is defined as <tt>SPLINE_LEVEL</tt>.
 * <br><br>
 * @param level
 * the derivation level at which the object is to be identified.
 */
    virtual int identity(int level = 0) const;
    /**
     * Returns the string <tt>"spline"</tt>.
     */
    virtual const char* type_name() const;
    /**
     * Posts a delete bulletin to the bulletin board indicating this <tt>SPLINE</tt> is no longer used in the active model.
     * <br><br>
     * <b>Role:</b> The <tt>lose</tt> methods for attached attributes are also called.
     */
    virtual void lose();
#endif

    // This function is hidden from mkman in the RESTORE_DEF macro; to have it documented,
    // we include it here:
#if 0
/**
 * Restores this <tt>SPLINE</tt> from a SAT file.
 * <br><br>
 * <b>Role:</b> The <tt>RESTORE_DEF</tt> macro expands to the <tt>restore_common</tt> method,
 * which is used in reading information from a SAT file. This method is never called
 * directly. It is called by a higher hierarchical function if an item in the SAT file
 * is determined to be of this class type. An instance of this class will already have
 * been created through the allocation constructor. This method then populates the class
 * instance with the appropriate data from the SAT file.
 * <br><br>
 * <tt>if (restore_version_number &lt; SURFACE_VERSION)</tt><br>
 * <tt>// Old style - the reverse bit was read explicitly, and</tt><br>
 * <tt>// forgotten when reading a lower-case spline normally.</tt><br>
 * <tt>read_int  Reverse bit</tt><br>
 * <tt>spline::restore_data</tt>&nbsp;&nbsp;&nbsp;&nbsp;low-level geometry definition of the spline<br>
 * <tt>if (reverse bit)</tt><br>
 * <tt>spline::negate</tt>&nbsp;&nbsp;&nbsp;&nbsp;Change definition of underlying spline.<br>
 */
    void restore_common();
#endif

    // Data changing routine.  This routine checks
    // that the record has been posted on the bulletin-board before
    // performing the change.  If not, the routine provokes an error,
    // so that the omission (forgetting to call backup() first) can
    // be rectified in the program.  In production versions of the
    // program, these checks may be disabled, to improve efficiency.
/**
 * Sets this <tt>SPLINE</tt>'s definition spline to the given spline.
 * <br><br>
 * <b>Role:</b> Before performing the change, it checks if the data structure
 * is posted on the bulletin board. If not, the method calls <tt>backup</tt>
 * to put an entry on the bulletin board.
 * <br><br>
 * @param spl
 * the new spline.
 */
    void set_def(const spline& spl);

    // Return the surface equation for reading only.
/**
 * Returns the <tt>surface</tt> equation of this <tt>SPLINE</tt>, for reading only.
 */
    const surface& equation() const;

    // Return the surface equation for reading only.
/**
 * Returns the <tt>surface</tt> equation for update operations.
 * <br><br>
 * <b>Role:</b> Before performing the change, it checks if the data structure
 * is posted on the bulletin board. If not, the method calls <tt>backup</tt>
 * to put an entry on the bulletin board.
 */
    surface& equation_for_update();

    // Get a new (lower-case) surface being the spline of the
    // SPLINE, transformed if the given SPAtransf is non-null and
    // reversed in sense if the logical is true.
/**
 * Returns the transformed spline equation of this <tt>SPLINE</tt>.
 * <br><br>
 * <b>Role:</b> If the transform <i>t</i> is non-<tt>NULL</tt>, the surface is transformed. If
 * the logical <tt>negate</tt> is <tt>TRUE</tt>, the surface is reversed in <tt>sense</tt>.
 * <br><br>
 * @param t
 * transform to apply.
 * @param negate
 * flag to reverse the surface.
 */
    surface* trans_surface(
        const SPAtransf& t = *(SPAtransf*)NULL_REF,
        logical negate = FALSE
    ) const;

    // Transform the stored spline in place.
/**
 * Transforms this <tt>SPLINE</tt> in place.
 * <br><br>
 * <b>Role:</b> Before performing the change, it checks if the data structure
 * is posted on the bulletin board. If not, the method calls <tt>backup</tt>
 * to put an entry on the bulletin board.
 * <br><br>
 * @param t
 * transform to apply.
 */
    void operator*=(const SPAtransf& t);

    // Make a bounding SPAbox for this surface, surrounded by a set of loops
    // of edges. For the present we merely find the SPAbox containing the
    // whole underlying surface, ignoring the bounding edges. Provided
    // we keep the surface minimal, this is probably sufficient as a
    // long-term solution.

    // wseibold (Feb`02) added arguments to compute tighter bounding boxes
/**
 * Returns a bounding box for this <tt>SPLINE</tt>.
 * <br><br>
 * <b>Role:</b> Returns a <tt>SPAbox</tt> bounding the complete <tt>surface</tt>
 * and the supplied <tt>LOOP</tt> list. If tight_box is TRUE, then a tighter box
 * bounding only the supplied <tt>LOOP</tt> list is computed. If untransformed_box
 * is supplied, it will be expanded to include the untransformed box just computed.
 * <br><br>
 * @param loop
 * list of <tt>LOOP</tt>s on the surface
 * @param t
 * transform to apply to the box
 * @param tight_box
 * flag to compute a tighter box
 * @param untransformed_box
 * accumulating box
 */
    SPAbox make_box(LOOP* loop = NULL, // STL amt 26Jun03: default value added
        const SPAtransf* t = NULL,
        logical tight_box = FALSE,
        SPAbox* untransformed_box = NULL) const;

    // lookup is done using SURFACE::lookup()
    // int lookup( logical ) const;

    // STI ROLL
/**
 * @nodoc
 */
    void full_size(SizeAccumulator& est, logical countSelf = TRUE) const;
    // STI ROLL

public:
    // get and set functions for access.
    spline gme_get_def();
    void gme_set_spl(spl_sur* spl);
    void** gme_get_spl_ptr();
};
/** @} */
#endif
