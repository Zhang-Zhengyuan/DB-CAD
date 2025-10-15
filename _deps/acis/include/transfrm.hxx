/*******************************************************************/
/*    Copyright (c) 1989-2020 by Spatial Corp.                     */
/*    All rights reserved.                                         */
/*    Protected by U.S. Patents 5,257,205; 5,351,196; 6,369,815;   */
/*                              5,982,378; 6,462,738; 6,941,251    */
/*    Protected by European Patents 0503642; 69220263.3            */
/*    Protected by Hong Kong Patent 1008101A                       */
/*******************************************************************/

// Header for TRANSFORM.

// A TRANSFORM, a derived class of ENTITY, records a transformation
// as a SPAtransf which in turn holds a 3x3 SPAmatrix for the normalised
// rotation transformation, a translation SPAvector, a real
// scale factor, and three logicals true if there is respectively
// rotation, reflection or shear (at present shear is not usually
// allowed).  Every body contains a TRANSFORM to record the
// transformation from body local coordinate space to global
// coordinate space.


#ifndef TRANSFORM_CLASS
#define TRANSFORM_CLASS

#include "dcl_kern.h"
#include "en_macro.hxx"
#include "transf.hxx"
/**
 * \defgroup ACISTRANSFORM Transformations
 * \ingroup KERNAPI
 * Model geometric transformations
 */
/**
* @file transfrm.hxx
 * @CAA2Level L1
 * @CAA2Usage U2
 * \addtogroup ACISTRANSFORM
 *
 * @{
 */

/**
 * @nodoc
 */
ENTITY_IS_PROTOTYPE( TRANSFORM, KERN )
#if 0
; // semicolon needed for mkman (doc tool) parsing
#endif

// Identifier used to find out (via identity() defined below) to what
// an entity pointer refers.

extern DECL_KERN int TRANSFORM_TYPE;

/// Identifier that gives number of levels of derivation of this class
/// from ENTITY.

#define TRANSFORM_LEVEL 1

// TRANSFORM declaration proper.

/**
 * Represents an overall transformation applied to a <tt>BODY</tt>, and is saved as part of the model.
 * <br>
 * <b>Role:</b> The <tt>TRANSFORM</tt> class represents an overall transformation applied
 * to a <tt>BODY</tt>. <tt>TRANSFORM</tt> allows object-space transformations to be applied without
 * the need to recompute the <tt>BODY</tt> geometry (until two <tt>BODY</tt>s are to be combined
 * and their internal coordinate systems have to be brought into agreement).
 * <br><br>
 * It allows a general affine transformation, but records the separate elements
 * of the transformation (scaling, rotation, translation, etc.) to simplify the
 * task of geometry transformation in the common case of solid-body transformations.
 * <br><br>
 * In particular, differential scaling, or shear, can change certain surface and
 * curve types, so this is an important consideration. Also a reflection changes
 * the <tt>sense</tt> of the relationship between edge tangents and surface normals and
 * the topology has to be reversed to maintain the conventional edge direction.
 * @see BODY, SPAtransf
 */
class DECL_KERN TRANSFORM: public ENTITY {

	// Record a TRANSFORM by a SPAtransf.

	SPAtransf transform_data;

	// Pointer to the owning entity

	ENTITY *owner_ptr;

	// Include the standard member functions for all entities.

	/**
	 * @nodoc
	 */
	ENTITY_FUNCTIONS( TRANSFORM , KERN)
#if 0
; // semicolon needed for mkman (doc tool) parsing)
#endif
// Because mkman isn't aware that ENTITY_FUNCTIONS changes access to "public", we do so
// explicitly here:
public:

	// Indicate whether this entity is normally destroyed by lose(),
	// or whether it gets destroyed implicitly when every owner has
	// been lost. Most entities are destroyed explicitly using lose(),
	// and so the default returns TRUE.
/**
 * Indicates whether this <tt>TRANSFORM</tt> is normally destroyed by <tt>lose</tt>.
 * <br><br>
 * <b>Role:</b> The method returns <tt>FALSE</tt>, indicating that <tt>TRANSFORM</tt>s are, by default,
 * shared among multiple owners by means of use-count tracking, and so are destroyed
 * implicitly when every owner has been lost.  Classes derived from <tt>TRANSFORM</tt> may of
 * course override this behavior
 */
	virtual logical deletable() const;

	// Now the functions specific to TRANSFORM.

	// Construct an identity transformation.
/**
  * Constructs a <tt>TRANSFORM</tt> (default constructor).
  * <br><br>
  * <b>Role:</b> The allocation constructor is used primarily by restore. Applications should
  * call this constructor only with the overloaded <tt>new</tt> operator inherited from the
  * <tt>ENTITY</tt> class (for example, <tt>x = new TRANSFORM</tt>, because this reserves the memory
  * on the heap, a requirement to support roll back and history management.
  */
	TRANSFORM();

	// Make a TRANSFORM from a SPAtransf.
/**
 * Constructs a <tt>TRANSFORM</tt> containing a specified <tt>SPAtransf</tt>.
 * <br><br>
 * <b>Role:</b> Requests memory for this object and populates it with the data
 * supplied as the argument. Applications should call this constructor only with
 * the overloaded <tt>new</tt> operator, because this reserves the memory on the heap,
 * a requirement to support roll back and history management.
 * <br><br>
 * @param transf
 * SPAtransf to be wrapped by the constructed TRANSFORM.
 */
	TRANSFORM( const SPAtransf & transf);

// These function are hidden from mkman in the ENTITY_FUNCTIONS macro; to have them documented,
// we include them here:
#if 0
/**
 * Returns a type identifier for this object.
 * <br><br>
 * <b>Role:</b> If <tt>level</tt> is unspecified or 0, returns the type identifier <tt>TRANSFORM_TYPE</tt>. If
 * <tt>level</tt> is specified, returns <tt>TRANSFORM_TYPE</tt> for that level of derivation from <tt>ENTITY</tt>. The
 * level of this class is defined as <tt>TRANSFORM_LEVEL</tt>.
 * <br><br>
 * @param level
 * the derivation level at which the object is to be identified.
 */
	virtual int identity( int level = 0 ) const;
/**
 * Returns the string <tt>"transform"</tt>.
 */
	virtual const char *type_name() const;
/**
 * Posts a delete bulletin to the bulletin board indicating this <tt>TRANSFORM</tt> is no longer used in the active model.
 * <br><br>
 * <b>Role:</b> The <tt>lose</tt> methods for attached attributes are also called.
 */
	virtual void lose();
#endif

// This function is hidden from mkman in the RESTORE_DEF macro; to have it documented,
// we include it here:
#if 0
/**
 * Restores this <tt>TRANSFORM</tt> from a SAT file.
 * <br><br>
 * <b>Role:</b> The <tt>RESTORE_DEF</tt> macro expands to the <tt>restore_common</tt> method,
 * which is used in reading information from a SAT file. This method is never called
 * directly. It is called by a higher hierarchical function if an item in the SAT file
 * is determined to be of this class type. An instance of this class will already have
 * been created through the allocation constructor. This method then populates the class
 * instance with the appropriate data from the SAT file.
 * <pre>
 *    read_transf      Transformation matrix</pre>
 */
    void restore_common();
#endif

    // Data reading routine.
/**
 * Returns the <tt>SPAtransf</tt> object for this <tt>TRANSFORM</tt>.
 */
	const SPAtransf &transform() const { return transform_data; }


	// Data changing routine.
/**
 * Sets this <tt>TRANSFORM</tt> to the given <tt>SPAtransf</tt> object.
 * <br><br>
 * <b>Role:</b> Before performing the change, it checks if the data structure
 * is posted on the bulletin board. If not, the method calls <tt>backup</tt>
 * to put an entry on the bulletin board.
 * <br><br>
 * @param transf
 * the new SPAtransf.
 */
	void set_transform( const SPAtransf & transf);

/**
 * Returns a pointer to the owning <tt>BODY</tt>.
 */
	ENTITY *owner() const;

	// Data changing routine.

/**
 * Sets the owner of this <tt>TRANSFORM</tt>.
 * <br><br>
 * <b>Role:</b> Before performing a change, it checks whether the data structure is posted on
 * the bulletin board.If not, the routine calls backup to put an entry on the bulletin board.
 * <br><br>
 * @param owner
 * entity.
 */
	void set_owner( ENTITY * owner);

	// Operator to combine two transformations.
/**
 * Composes an existing transformation with a new one.
 * <br><br>
 * <b>Role:</b> Let <tt>T1()</tt> represent the existing transformation and let <tt>T2()</tt>
 * represent the new one.  Then this method replaces the former by the composition
 * <tt>T2(T1())</tt>.
 * <br><br>
 * Before performing the change, this method checks if the data structure
 * is posted on the bulletin board. If not, it calls <tt>backup</tt>
 * to put an entry on the bulletin board.
 * <br><br>
 * @param transf
 * the new SPAtransf.
 */
	TRANSFORM& operator*=( const SPAtransf & transf);


	// Find a TRANSFORM in its list, returning the sequence number.
	// If it is not there, optionally insert it, creating the lists
	// if necessary.
/**
 * @nodoc
 */
	int lookup( logical ) const;
};
/** @} */
#endif
