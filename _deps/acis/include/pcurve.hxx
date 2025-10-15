/*******************************************************************/
/*    Copyright (c) 1989-2020 by Spatial Corp.                     */
/*    All rights reserved.                                         */
/*    Protected by U.S. Patents 5,257,205; 5,351,196; 6,369,815;   */
/*                              5,982,378; 6,462,738; 6,941,251    */
/*    Protected by European Patents 0503642; 69220263.3            */
/*    Protected by Hong Kong Patent 1008101A                       */
/*******************************************************************/
// Header for PCURVE.
// Generic parametric curve record
// A PCURVE represents a SPAparameter-space approximation to a curve
// lying on a parametrised surface. Because there is only one such
// representation, this class does not need to have derived classes
// for specific geometries.
// Note that we retain a use count in this record, even though the
// (lower_case) pcurve also has a use count.  This is largely for
// consistency within the geometry entities.
#ifndef PCURVE_CLASS
#define PCURVE_CLASS
/**
 * @file pcurve.hxx
 * @CAA2Level L1
 * @CAA2Usage U2
 * \addtogroup ACISGEOMETRICENTITIES
 *
 * @{
 */
#include "dcl_kern.h"
#include "logical.h"
#include "en_macro.hxx"
#include "pcudef.hxx"
#include "param.hxx"
#include "usecount.hxx"
class CURVE;
class COEDGE;
class pattern_holder;
class pattern;
class VOID_LIST;
/**
 * @nodoc
 */
ENTITY_IS_PROTOTYPE(PCURVE, KERN)
#if 0
; // semicolon needed for mkman (doc tool) parsing
#endif

// Identifier used to find out (via identity() defined below) to what
// an entity pointer refers.

extern DECL_KERN int PCURVE_TYPE;
/**
 * Identifier that gives number of levels of derivation of this class
 * from ENTITY
*/
#define PCURVE_LEVEL 1


// PCURVE declaration proper.

/**
 * Defines a 2D parameter-space approximation to a curve as an object in the model.
 * <br>
 * <b>Role:</b> The purpose of a <tt>PCURVE</tt> is to provide a persistent, logable, savable object to
 * manage pcurve information associated with a <tt>COEDGE</tt>.
 * <br><br>
 * <tt>PCURVE</tt> is a model geometry class that provides a (lowercase) pcurve, the corresponding
 * construction geometry class. In general, a model geometry class is derived from <tt>ENTITY</tt> and is
 * used to define a permanent model object. It provides model management functionality, in
 * addition to the geometry definition.
 * <br><br>
 * A <tt>PCURVE</tt> provides a procedural 2D parameter-space representation of a <tt>CURVE</tt> lying on a
 * parameterized <tt>SURFACE</tt>. The representation may be a private copy (i.e., contained within a
 * lowercase pcurve associated with the <tt>PCURVE</tt>), or it may refer to information contained within
 * an intcurve. In either case, it may be negated from the underlying parameter space
 * representation. Because there is only one such representation, this class does not need to
 * have derived classes for specific geometries.
 * <br><br>
 * Along with the usual <tt>ENTITY</tt> class methods, <tt>PCURVE</tt> has member methods to provide access to
 * specific implementations of the geometry. For example, a pcurve can be transformed by a given
 * transform operator.
 * <br><br>
 * A use count allows multiple references to a <tt>PCURVE</tt>. The construction of a new <tt>PCURVE</tt>
 * initializes the use count to 0. Methods are provided to increment and decrement the use count,
 * and after the use count returns to 0, the entity is deleted.
 * @see CURVE, ENTITY, pcurve, COEDGE, SPApar_vec
 */
class DECL_KERN PCURVE : public ENTITY {

private:

    // STI jmb: Handle save/restore of use counted histories
    // 	unsigned int use_count_data;
    // STI jmb: end


        // Details of the PCURVE are found in the pcurve def OR
        // via the CURVE *cur (see below), according to the value of
        // int def_type.
        // A def_type of zero indicates a private definition,
        // supplied by def (below).
        // A positive value n indicates the nth pcurve associated with
        // the CURVE cur, shifted in SPAparameter space by off.
        // A negative value -n indicates the nth pcurve associated with
        // cur, negated. shifted in SPAparameter space by off.

    int def_type;

    // Definition of an explicit pcurve.

    pcurve def;			// ignored (null) if def_type is non-zero.

    // Definition of an implicit pcurve.

    CURVE* cur;			// ignored (NULL) if def_type is zero.
    SPApar_vec off;		// ignored (zero) if def_type is zero.

    // Track the ENTITYs owning the PCURVE
private:
    ENTITY** owners_ptr;
    int num_owners;
    int num_owners_alloc;

protected:
    /**
     * Returns TRUE if this entity can have a pattern_holder. For internal use only.
     */
    logical holds_pattern() const { return TRUE; }
public:
    /**
     * Adds the owner argument to the list of owners.
     * <br><br>
     * @param owner
     * owner to be added.
     * @param increment_use_count
     * flag to increment use count or not.
     */
    void add_owner(ENTITY* owner, logical increment_use_count = TRUE);
    /**
     * Removes the owner argument from the list of owners.
     * <br><br>
     * @param owner
     * owner to be removed.
     * @param decrement_use_count
     * flag to decrement the use count.
     * @param zero_flag
     * flag to lose if use count is zero.
     */
    void remove_owner(ENTITY* owner, logical decrement_use_count = TRUE, logical zero_flag = TRUE);
    /**
     * Copies the list of owners of <tt>this</tt> <tt>CURVE</tt> to the <tt>list</tt> argument.
     * <br><br>
     * <b>Role:</b> Returns the number of owners copied.
     * <br><br>
     * @param list
     * list of owners.
     */
    int get_owners(ENTITY_LIST& list) const;

    /**
     * Returns the first owner of this <tt>PCURVE</tt>.
     * <br><br>
     * <b>Role:</b> If there are no owners, NULL is returned.
     */
    ENTITY* owner() const;


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
    ENTITY_FUNCTIONS(PCURVE, KERN)
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

    // Indicate whether this entity is normally destroyed by lose(),
    // or whether it gets destroyed implicitly when every owner has
    // been lost. Most entities are destroyed explicitly using lose(),
    // and so the default returns TRUE.
/**
 * Indicates whether <tt>this</tt> <tt>PCURVE</tt> is normally destroyed by <tt>lose</tt>.
 * <br><br>
 * <b>Role:</b> The method returns <tt>FALSE</tt>, indicating that <tt>PCURVE</tt>s are, by default,
 * shared among multiple owners by means of use-count tracking, and so are destroyed
 * implicitly when every owner has been lost.  Classes derived from <tt>PCURVE</tt> may of
 * course override this behavior
 */
    virtual logical deletable() const;


    // Now the functions specific to PCURVE.

    // Make a bare pcurve.
/**
 * Constructs a <tt>PCURVE</tt> (default constructor).
 * <br><br>
 * <b>Role:</b> Requests memory for this object but does not populate it. The
 * allocation constructor is used primarily by restore. Applications should call
 * this constructor only with the overloaded <tt>new</tt> operator, because this
 * reserves the memory on the heap, a requirement to support roll back and history
 * management.
 */
    PCURVE();

    // Make a PCURVE from a pcurve.
/**
 * Constructs a <tt>PCURVE</tt> containing a specified pcurve.
 * <br><br>
 * <b>Role:</b> Requests memory for this object and populates it with the data
 * supplied as the argument. Applications should call this constructor only with
 * the overloaded <tt>new</tt> operator, because this reserves the memory on the heap,
 * a requirement to support roll back and history management.
 * <br><br>
 * @param pcur
 * pcurve to be wrapped by the constructed PCURVE.
 */
    PCURVE(const pcurve& pcur);

    // Make a PCURVE to point to an existing pcurve (via a CURVE).
    // The given integer is positive; a logical value of true means
    // the pcurve referenced via the CURVE, is considered negated.
/**
 * Constructs a <tt>PCURVE</tt> that points to an existing <tt>PCURVE</tt>, via a <tt>CURVE</tt>.
 * <br><br>
 * <b>Role:</b> Requests memory for this object and populates it with the data
 * supplied as the argument. Applications should call this constructor only with
 * the overloaded <tt>new</tt> operator, because this reserves the memory on the heap,
 * a requirement to support roll back and history management.
 * <br><br>
 * Makes a <tt>PCURVE</tt> to that points to an existing <tt>PCURVE</tt> via the <tt>crv</tt> argument.
 * The index <tt>def</tt> is positive 1 or 2, representing the two surfaces in order.
 * Setting <tt>negate</tt> to <tt>TRUE</tt> means the <tt>PCURVE</tt> referenced via the <tt>CURVE</tt> is
 * to be considered negated, and <tt>par_vec</tt> offsets the spline surface in parametric
 * space.
 * <br><br>
 * @param crv
 * CURVE relating the constructed PCURVE to an existing one.
 * @param def
 * definition type index.
 * @param negate
 * flag to consider the constructed PCURVE as negated.
 * @param pv
 * offset to the spline surface in parametric space.
 */
    PCURVE(
        CURVE* crv,
        int def,
        logical negate = FALSE,
        const SPApar_vec& pv = *(const class SPApar_vec*)NULL_REF
    );

    // Duplicate a PCURVE, usually used to unshare a shared PCURVE
    // before transforming in some way.
/**
 * Copy-constructs a <tt>PCURVE</tt>.
 * <br><br>
 * <b>Role:</b> Requests memory for this object and populates it with the data
 * supplied as the argument. Applications should call this constructor only with
 * the overloaded <tt>new</tt> operator, because this reserves the memory on the heap,
 * a requirement to support roll back and history management.
 * <br><br>
 * @param pcrv
 * PCURVE whose data is to be copied.
 */
    PCURVE(PCURVE* pcrv);

    // This function is hidden from mkman in the RESTORE_DEF macro; to have it documented,
    // we include it here:
#if 0
/**
 * Restores <tt>this</tt> <tt>PCURVE</tt> from a SAT file.
 * <br><br>
 * <b>Role:</b> The <tt>RESTORE_DEF</tt> macro expands to the <tt>restore_common</tt> method,
 * which is used in reading information from a SAT file. This method is never called
 * directly. It is called by a higher hierarchical function if an item in the SAT file
 * is determined to be of this class type. An instance of this class will already have
 * been created through the allocation constructor. This method then populates the class
 * instance with the appropriate data from the SAT file.
 * <br><br><tt>
 * &nbsp;&nbsp;&nbsp;if (restore_version_number >= PATTERN_VERSION<br>
 * &nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;read_ptr  </tt>APATTERN index<br><tt>
 * &nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;if (apat_idx != (APATTERN*)(-1)))<br>
 * &nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;restore_cache();<br>
 * &nbsp;&nbsp;&nbsp;read_int  </tt>Type of pcurve.<br><tt>
 * &nbsp;&nbsp;&nbsp;if (def_type == 0)<br>
 * &nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;pcurve::restore_data  </tt>Save the data from the
 * underlying low-level geometry definition.<br><tt>
 * &nbsp;&nbsp;&nbsp;else<br>
 * &nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;read_ptr  </tt>Pointer to the CURVE definition.<br><tt>
 * &nbsp;&nbsp;&nbsp;if (restore_version_number &lt; PCURVE_VERSION)<br>
 * &nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;// Set off = SPApar_vec(0, 0)<br>
 * &nbsp;&nbsp;&nbsp;else<br>
 * &nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;read_real  </tt>du<br><tt>
 * &nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;read_real  </tt>dv<br><tt>
 * &nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;// Set off = SPApar_vec(du, dv)<br>
 * &nbsp;&nbsp;&nbsp;if ( !std_acis_save_flag )<br>
 * &nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;read_int   </tt>use count data
 */
    void restore_common();
#endif

    // STI jmb: Handle save/restore of use counted histories
    // 	// Use count manipulation, incrementing or decrementing.  If the
    // 	// decrement leaves the use count at zero, the record is deleted
    // 	// using lose().
    //
    // 	void add();
    // 	void remove();
    //
    //
    // 	// Data reading routines.
    //
    // 	int use_count() const { return use_count_data; }
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
 * Returns a type identifier for <tt>this</tt> object.
 * <br><br>
 * <b>Role:</b> If <tt>level</tt> is unspecified or 0, returns the type identifier <tt>PCURVE_TYPE</tt>. If
 * <tt>level</tt> is specified, returns <tt>PCURVE_TYPE</tt> for that level of derivation from <tt>ENTITY</tt>. The
 * level of this class is defined as <tt>PCURVE_LEVEL</tt>.
 * <br><br>
 * @param level
 * the derivation level at which the object is to be identified.
 */
    virtual int identity(int level = 0) const;
    /**
     * Returns the string <tt>"pcurve"</tt>.
     */
    virtual const char* type_name() const;
    /**
     * Posts a delete bulletin to the bulletin board indicating <tt>this</tt> <tt>PCURVE</tt> is no
     * longer used in the active model.
     * <br><br>
     * <b>Role:</b> The <tt>lose</tt> methods for attached attributes are also called.
     */
    virtual void lose();
#endif

    // These functions is hidden from mkman in the USE_COUNTED_DECL macro; to have them documented,
    // we include them here:
#if 0
/**
 * Increments the use count of <tt>this</tt> <tt>PCURVE</tt>.
 * <br><br>
 * <b>Role:</b> Before performing the change, it checks if the data structure is
 * posted on the bulletin board. If not, the method calls <tt>backup</tt> to put
 * an entry on the bulletin board.
 */
    virtual void add();
    /**
     * Indicates if this <tt>PCURVE</tt> is use-counted or not.
     * <br><br>
     * <b>Role:</b> Returns <tt>TRUE</tt> for <tt>PCURVE</tt>s unless overridden in a derived class.
     */
    virtual logical is_use_counted() const;
    /**
     * Decrements the use count of this <tt>PCURVE</tt>.
     * <br><br>
     * <b>Role:</b> If the use count reaches 0, the <tt>PCURVE</tt> is deleted. Before performing
     * the change, it checks if the data structure is posted on the bulletin board. If not,
     * the method calls <tt>backup</tt> to put an entry on the bulletin board.
     * <br><br>
     * @param lose_if_zero
     * flag to lose if use count is zero.
     */
    virtual void remove(logical lose_if_zero = TRUE);
    /**
     * Sets the reference use count of this <tt>PCURVE</tt>.
     * <br><br>
     * <b>Role:</b> This is used by the <tt>lose</tt> method. Refinements are not
     * destructed until the use count goes to zero.
     * <br><br>
     * @param val
     * new use count.
     */
    virtual void set_use_count(int val);
    /**
     * Returns the use count of <tt>this</tt> <tt>PCURVE</tt>.
     */
    virtual int use_count() const;
#endif

    /**
     * Returns the definition type of <tt>this</tt> <tt>PCURVE</tt>.
     * <br><br>
     * <b>Role:</b> A 0 value indicates a private pcurve. A positive 1 or 2 represents
     * the first or second pcurve in an intcurve definition, while a negative 1 or 2
     * represents the reverse of the corresponding pcurve.
     */
    int index() const { return def_type; }
    /**
     * Returns the definition pcurve, or <tt>NULL</tt> if the pcurve is not private.
     */
    const pcurve& def_pcur() const { return def; }
    /**
     * Returns the reference <tt>CURVE</tt>.
     */
    CURVE* ref_curve() const { return cur; }
    /**
     * Returns the <tt>SPApar_vec</tt> parameter space vector offset.
     * <br><br>
     * <b>Role:</b> The offset is the displacement in parameter space between the "fit"
     * definition and <tt>this</tt> <tt>PCURVE</tt>. This allows the <tt>PCURVE</tt> to be positioned in the
     * infinite parameter space of a periodic surface, so that continuous curve sequences in
     * object space are continuous in parameter space. The components of this vector should
     * always be integer multiples of the corresponding surface parameter period, zero if
     * it is not periodic in that direction.
     */
    SPApar_vec offset() const { return off; }


    // Checks this PCURVE has been backed up, then zeros def_type,
    // removes any CURVE referred to by cur (which is set to NULL),
    // sets def_type to zero, and puts given pcurve in def.
/**
 * Puts the specified pcurve in <tt>def</tt>.
 * <br><br>
 * <b>Role:</b> Checks that <tt>this</tt> <tt>PCURVE</tt> has been backed up, then removes any
 * curve referred to by <tt>cur</tt> (which is set to <tt>NULL</tt>), sets <tt>def_type</tt> to
 * zero, and puts the given pcurve in <tt>def</tt>. Before performing the change, it checks
 * if the data structure is posted on the bulletin board. If not, the method calls
 * <tt>backup</tt> to put an entry on the bulletin board.
 * <br><br>
 * @param pcrv
 * pcurve to which <tt>def</tt> is to be set.
 */
    void set_def(const pcurve& pcrv);

    // Checks this PCURVE has been backed up,
    // Set set_def to n-th pcurve of an existing CURVE.  n is given
    // as a positive integer, and the logical is given as true if
    // the reversed pcurve is wanted.  Removes any previous
    // reference in cur to a CURVE, and increments use-count for
    // given CURVE.
    // Makes a null pcurve and sets it in def.
/**
 * Sets <tt>def</tt> to the <i>n</i>th pcurve of an existing <tt>CURVE</tt>, where <i>n</i> as a
 * positive integer.
 * <br><br>
 * <b>Role:</b> The logical <tt>negate</tt> is <tt>TRUE</tt> for a reversed pcurve. Removes any
 * previous reference in <tt>cur</tt> to a <tt>CURVE</tt>, and increments the use count for the
 * given <tt>CURVE</tt>. Makes a <tt>NULL</tt> pcurve and puts it in <tt>def</tt>. Before performing the
 * change, it checks if the data structure is posted on the bulletin board. If not, the
 * method calls <tt>backup</tt> to put an entry on the bulletin board.
 * <br><br>
 * @param crv
 * curve to which <tt>cur</tt> is to be set.
 * @param index
 * pcurve index.
 * @param negate
 * flags a reversed pcurve.
 * @param offset
 * parameter space vector.
 */
    void set_def(
        CURVE* crv,
        int index,
        logical negate = FALSE,
        const SPApar_vec& offset = *(SPApar_vec*)NULL_REF
    );


    // Return the curve equation, for reading only
    // Logical flag indicating if the pcurve is temporary
/**
 * Returns the equation of <tt>this</tt> <tt>PCURVE</tt>, for reading only.
 * <br><br>
 * @param temporary
 * internal use only.
 */
    pcurve equation(logical temporary = FALSE) const;

    // Shift the PCURVE in SPAparameter space, required to move it
    // by integral multiples of the period on a periodic surface.
/**
 * Shifts <tt>this</tt> <tt>PCURVE</tt> in parameter space by integral multiples of the period
 * on a periodic surface.
 * <br><br>
 * <b>Role:</b> Before performing the change, it checks if the data structure is
 * posted on the bulletin board. If not, the method calls <tt>backup</tt> to put an
 * entry on the bulletin board.
 * <br><br>
 * @param shift_vec
 * parameter-space vector specifying the shift.
 */
    void shift(const SPApar_vec& shift_vec);


    // Negate the PCURVE, either by reversing the pcurve
    // or by reversing the value of a non_zero def_type.
/**
 * Negates the pcurve, either by reversing the pcurve or by reversing the value of a
 * nonzero <tt>def_type</tt>.
 * <br><br>
 * <b>Role:</b> Before performing the change, it checks if the data structure is
 * posted on the bulletin board. If not, the method calls <tt>backup</tt> to put
 * an entry on the bulletin board.
 */
    void negate();

    // Construct a transformed pcurve.
/**
 * Constructs a transformed pcurve.
 * <br><br>
 * <b>Role:</b> The logical <tt>negate</tt> is <tt>TRUE</tt> if the pcurve is considered to be
 * reversed. Before performing the change, it checks if the data structure is posted
 * on the bulletin board. If not, the method calls <tt>backup</tt> to put an entry on
 * the bulletin board.
 * <br><br>
 * @param t
 * transform to apply.
 * @param negate
 * flag to consider the pcurve as reversed.
 */
    pcurve* trans_pcurve(
        const SPAtransf& t = *(SPAtransf*)NULL_REF,
        logical negate = FALSE
    ) const;


    // Transform the PCURVE. If the definition is a CURVE reference,
    // assume that the curve will be transformed as well, so do nothing.
/**
 * Transforms <tt>this</tt> <tt>PCURVE</tt>.
 * <br><br>
 * <b>Role:</b> If the definition is a <tt>CURVE</tt> reference, this method assumes that the curve
 * will be transformed as well, so it does nothing. Before performing the change, it checks
 * if the data structure is posted on the bulletin board. If not, the method calls
 * <tt>backup</tt> to put an entry on the bulletin board.
 * <br><br>
 * @param t
 * transform to apply.
 */
    void operator*=(const SPAtransf& t);

    // Routine used for system debugging
/**
 * @nodoc
 */
    virtual int lookup(logical) const;

    // STI ROLL
/**
 * @nodoc
 */
    void full_size(SizeAccumulator& est, logical countSelf = TRUE) const;
    // STI ROLL
public:
    // for save
    int gme_get_def_type();
    pcurve gme_get_def();
    // for restore
    pcurve* gme_get_def_ptr();
    void** gme_get_ref_curve_ptr();
    void set_fit(par_cur* fit);
    void** gme_get_fit_ptr();
};

/** @} */
#endif
