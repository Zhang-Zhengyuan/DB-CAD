/*******************************************************************/
/*    Copyright (c) 1989-2020 by Spatial Corp.                     */
/*    All rights reserved.                                         */
/*    Protected by U.S. Patents 5,257,205; 5,351,196; 6,369,815;   */
/*                              5,982,378; 6,462,738; 6,941,251    */
/*    Protected by European Patents 0503642; 69220263.3            */
/*    Protected by Hong Kong Patent 1008101A                       */
/*******************************************************************/

/*******************************************************************/
// dal 03Apr03 Remove docs for inherited methods such as copy_owner
// dal 31Mar03 Documentation.
/*******************************************************************/

#if !defined( ATTRIB_FACECFACE_CLASS )
#define ATTRIB_FACECFACE_CLASS

#include "dcl_ct.h"
#include "at_ct.hxx"

/**
* @file at_fcf.hxx
 * @CAA2Level L1
 * @CAA2Usage U1
 * \addtogroup CELLTOPAPI
 * @{
 */

class CFACE;
class FACE;

/**
 * Determines if an <tt>ENTITY</tt> is a <tt>ATTRIB_FACECFACE</tt>.
 * <br><br>
 * <b>Role:</b> This procedure takes an <tt>ENTITY</tt> as an argument and returns <tt>TRUE</tt>
 * if the entity is a <tt>ATTRIB_FACECFACE</tt>, <tt>FALSE</tt> if it is not. This procedure also checks for a
 * <tt>NULL</tt> pointer.
 * <br><br>
 * <b>Effect:</b> Read-only
 * <br><br>
 * @param ent
 * entity.
 **/
logical DECL_CT is_ATTRIB_FACECFACE( const ENTITY *ent );

/**
 * @nodoc
 */
ENTITY_IS_PROTOTYPE(ATTRIB_FACECFACE, CT )
#if 0
; // semicolon needed for mkman (doc tool) parsing
#endif

/**
 * @nodoc
 */
extern DECL_CT int ATTRIB_FACECFACE_TYPE;

/**
 * @nodoc
 */
#define ATTRIB_FACECFACE_LEVEL (ATTRIB_CT_LEVEL + 1)

/**
 * Defines an attribute that records one or two <tt>CFACE</tt> entities that refer to a given face.
 * <br>
 * <b>Role:</b> One of two attribute classes (the other being @href ATTRIB_CELL) that connect the
 * Cellular Topology Data Structure to the standard ACIS Topology Structure. This attribute
 * is attached to a face and records one or two <tt>CFACE</tt> entities that refer to the front (outside)
 * and back (inside) of a <tt>FACE</tt>. An <tt>ATTRIB_FACECFACE</tt> effectively provides 
 * a back pointer from a face to the each of the cells that it bounds. A single-sided face can be part 
 * of the boundary of at most one cell, a <tt>CELL3D</tt>. A double-sided face can be part of the boundary 
 * of one or two cells, which may be either <tt>CELL2D</tt>s or <tt>CELL3D</tt>s. A single-sided face 
 * will have one <tt>CFACE</tt>, whereas a double-sided face will have two <tt>CFACE</tt>s, one for each
 * side of the face.
 * <br><br>
 * For additional information, refer to the Technical Article on <i>Cellular Topology Data Structure</i>.
 * @see CFACE
 */

class DECL_CT ATTRIB_FACECFACE: public ATTRIB_CT {

//	Pointer to front CFACE, which is the CFACE that
//	uses the face in the REVERSED direction.

	CFACE *front_cface_ptr;   // first cface

//	Pointer to back CFACE, which is the CFACE that
//	uses the face in the FORWARD direction.

	CFACE *back_cface_ptr;    // second cface

public:
	// Construct the initial attribute - add up to two cfaces
    /**
     * C++ initialize constructor requests memory for this object and populates it with the data supplied as arguments.
     * <br><br>
     * <b>Role:</b> Applications should call this constructor only with the overloaded <tt>new</tt> operator
     * inherited from the <tt>ENTITY</tt> class (for example, <tt>x = new ATTRIB_FACECFACE(...)</tt>), because this reserves
     * the memory on the heap, a requirement to support roll back and history management.
     * <br><br>
     * @param owner
     * owner.
     * @param f_face
     * front cface.
     * @param b_face
     * back cface.
     */

	ATTRIB_FACECFACE( FACE * owner = NULL, CFACE * f_face= NULL, CFACE * b_face= NULL);

    /**
     * Returns a pointer to front <tt>CFACE</tt>, which is the <tt>CFACE</tt> that uses the face in the <tt>REVERSED</tt> direction.
     * <br><br>
     * <b>Role:</b> This method returns a pointer to the <tt>CFACE</tt> that is <i>outside</i> the face owning of this attribute.
	 * This pointer always will be NULL for single-sided faces and non-NULL for double-sided faces that belong to lumps with 
	 * Cellular Topology.
	 */
	CFACE *front_cface() const {return front_cface_ptr;}

    /**
     * Returns a pointer to back <tt>CFACE</tt>, which is the <tt>CFACE</tt> that uses the face in the <tt>FORWARD</tt> direction.
     * <br><br>
     * <b>Role:</b> This method returns a pointer to the <tt>CFACE</tt> that is <i>inside</i> the face owning of this attribute.
	 * This pointer always will be non-NULL for both single-sided faces and double-sided faces that belong to lumps with 
	 * Cellular Topology.
     */
	CFACE *back_cface() const {return back_cface_ptr;}

    /**
     * Sets the front <tt>CFACE</tt>.
     * <br><br>
     * @param cface
     * cface.
     */
	void set_front_cface(CFACE *cface);

    /**
     * Sets the back <tt>CFACE</tt>.
     * <br><br>
     * @param cface
     * cface.
     */
	void set_back_cface(CFACE * cface);

	// Split, merge and copy functions
    /**
     * @nodoc
     */
	virtual void split_owner( ENTITY * entity);

    /**
     * @nodoc
     */
	virtual void merge_owner( ENTITY *entity, logical del_owner);

    /**
     * @nodoc
     */
	virtual void copy_owner( ENTITY * copy_entity);

    /**
     * @nodoc
     */
	virtual void reverse_owner();

	// Standard housekeeping functions

	/**
	 * @nodoc
	 */
	ATTRIB_FUNCTIONS( ATTRIB_FACECFACE, CT )
#if 0
; // semicolon needed for mkman (doc tool) parsing)
#endif
// Because mkman isn't aware that ATTRIB_FUNCTIONS changes access to "public", we do so
// explicitly here:
public:

};

/** @} */
#define ATTRIB_FACECFACE_CLASS
#endif

