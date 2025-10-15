/*******************************************************************/
/*    Copyright (c) 1989-2020 by Spatial Corp.                     */
/*    All rights reserved.                                         */
/*    Protected by U.S. Patents 5,257,205; 5,351,196; 6,369,815;   */
/*                              5,982,378; 6,462,738; 6,941,251    */
/*    Protected by European Patents 0503642; 69220263.3            */
/*    Protected by Hong Kong Patent 1008101A                       */
/*******************************************************************/

/*******************************************************************/
// let 02Feb12 Created New ATTRIB_CELL_PRECEDENCE Class.
/*******************************************************************/

#if !defined( ATTRIB_CELL_PRECEDENCE_CLASS )
#define ATTRIB_CELL_PRECEDENCE_CLASS

#include "dcl_ct.h"
#include "at_ct.hxx"

class CELL;

/**
* @file at_prcd.hxx
 * @CAA2Level L1
 * @CAA2Usage U1
 * \addtogroup CELLTOPAPI
 * @{
 */

/**
 * Determines if an <tt>ENTITY</tt> is a <tt>ATTRIB_CELL_PRECEDENCE</tt>.
 * <br><br>
 * <b>Role:</b> This procedure takes an <tt>ENTITY</tt> as an argument and returns <tt>TRUE</tt>
 * if the entity is a <tt>ATTRIB_CELL_PRECEDENCE</tt>, <tt>FALSE</tt> if it is not. 
 * This procedure also checks for a <tt>NULL</tt> pointer.
 * <br><br>
 * <b>Effect:</b> Read-only
 * <br><br>
 * @param ent
 * entity.
 **/
logical DECL_CT is_ATTRIB_CELL_PRECEDENCE( const ENTITY *ent );

/**
 * @nodoc
 */
ENTITY_IS_PROTOTYPE(ATTRIB_CELL_PRECEDENCE, CT )
#if 0
; // semicolon needed for mkman (doc tool) parsing
#endif

/**
 * @nodoc
 */
extern DECL_CT int ATTRIB_CELL_PRECEDENCE_TYPE;

/**
 * @nodoc
 */
#define ATTRIB_CELL_PRECEDENCE_LEVEL (ATTRIB_CT_LEVEL + 1)

/**
 * Specifies which cell must be retained during cell recompute.
 */
enum ct_precedence_type 
{ 
	RETAIN_UNKNOWN, ///< Unknown precedence
	RETAIN_NEITHER, ///< Neither cell needs to be retained
	RETAIN_EITHER,  ///< Either cell can be retained
	RETAIN_THIS,    ///< Retain this cell
	RETAIN_OTHER,   ///< Retain the other cell
	RETAIN_BOTH     ///< Retain both cells - Not implemented
};

/**
 * Specifies the precedence associated with a 2D or 3D cell.
 * <br>
 * <b>Role:</b> This class is intended to be a base class for cell precedence attributes placed 
 * on <tt>CELL</tt>s. Cell precedence attributes affect the propagation of attributes on cells 
 * during cell recomputation. Applications may derive attribute classes directly from this class or
 * or from a class derived from this class, such as @href ATTRIB_CELL_INT_PRECEDENCE.
 */

class DECL_CT ATTRIB_CELL_PRECEDENCE: public ATTRIB_CT {

public:

	// Construct the initial attribute 
    /**
     * C++ initialize constructor requests memory for this object and populates it with the data supplied as arguments.
     * <br><br>
     * <b>Role:</b> Applications should call this constructor only with the overloaded <tt>new</tt> operator
     * inherited from the <tt>ENTITY</tt> class (for example, <tt>x = new ATTRIB_CELL_PRECEDENCE(...)</tt>), because this reserves
     * the memory on the heap, a requirement to support roll back and history management. Generally,
	 * applications should call the constructor with using the <tt>ACIS_NEW</tt> macro, which calls the
	 * overloaded <tt>new</tt> operator.
     * <br><br>
     * @param c
     * owning cell.
     */

	ATTRIB_CELL_PRECEDENCE( CELL *c = NULL );

    /**
     * @nodoc
     */
     virtual logical pattern_compatible() const;
                                    // returns TRUE


    //	ATTRIB_CELL_PRECEDENCE propagation functions

    /**
     * Indicates if this attribute can be propagated to the specified <tt>CELL</tt>.
	 * Checks whether the given <tt>CELL</tt> entity has a similar attribute of class 
	 * <tt>ATTRIB_CELL_PRECEDENCE</tt>. If a similar <tt>ATTRIB_CELL_PRECEDENCE</tt> already 
	 * exists on the given <tt>CELL</tt>, this method returns <tt>FALSE</tt>; otherwise 
	 * it returns <tt>TRUE</tt>. This method should be overloaded in derived classes.
     * <br><br>
     * @param c
     * given <tt>CELL</tt>.
     */
 	virtual logical can_propagate( CELL *c );

    //	Attribute split and copy functions...use parent's merge

	/**
	 * Notifies this <tt>ATTRIB</tt> that its owner is about to be split into two parts.
	 * <br><br>
	 * <b>Role:</b> The application has the chance to duplicate or otherwise modify the
	 * attribute. This function is supplied by the application whenever it derives a new attribute, 
	 * and is called when a split occurs.
	 * <br><br>
	 * For an <tt>ATTRIB_CELL_PRECEDENCE</tt> the default behavior is to attach a new 
	 * <tt>ATTRIB_CELL_PRECEDENCE</tt> with this attribute's precedence value to the new 
	 * <tt>CELL</tt> arising from the split. 
	 * <br><br>
	 * @param new_entity
	 * the new <tt>CELL</tt> arising from the split.
	 */
	virtual void split_owner( ENTITY *new_entity);

	/**
	 * Notifies this <tt>ATTRIB</tt> that its owning <tt>ENTITY</tt> is about to be merged 
	 * with a given <tt>ENTITY</tt>.
	 * <br><br>
	 * <b>Role:</b>  The application has the chance to delete or otherwise modify the attribute.
	 * After the merge, the owner of this attribute will be deleted if the logical argument
	 * <tt>del_owner</tt> is <tt>TRUE</tt>; otherwise, it will be retained and the other owner will
	 * be deleted. The default action is to do nothing. This function is supplied by the
	 * application whenever it defines a new, derived attribute, and is called when a merge occurs.
	 * <br><br>
	 * For an <tt>ATTRIB_CELL_PRECEDENCE</tt> the default behavior is to move the 
	 * <tt>ATTRIB_CELL_PRECEDENCE</tt> to the other <tt>CELL</tt>, if this <tt>CELL</tt> 
	 * is being deleted and <tt>can_propagate</tt> returns <tt>TRUE</tt> for the other 
	 * <tt>CELL</tt> involved in the merge. This method is occasionally overloaded in derived classes.
	 * <br><br>
	 * @param entity
	 * the <tt>CELL</tt> with which this attribute's owner is being merged.
	 * @param del_owner
	 * flag that the owner of this attribute will be deleted after the merge.
	 */
	virtual void merge_owner( ENTITY * entity, logical del_owner );

	/**
 	 * Notifies this <tt>ATTRIB</tt> that its owning <tt>ENTITY</tt> has been copied.
	 * <br><br>
	 * <b>Role:</b> This method is used to notify this attribute that it's owner has been copied.
	 * <br><br>
	 * The predominant fashion by which <tt>copy_owner</tt> is called is during the course of ACIS 
	 * operations that are constructing, deconstructing, or modifying either topology and/or 
	 * geometry. This method is also called at the end of the @href api_down_copy_entity algorithm.
	 * As a general rule, the attribute implementing this method cannot assume the validity
	 * of the model "above" or "below" its owner, only that its owner is valid and that the
	 * <tt>copy_ent</tt> is valid.
	 * <br><br>
	 * For an <tt>ATTRIB_CELL_PRECEDENCE</tt> the default behavior is to attach a new 
	 * <tt>ATTRIB_CELL_PRECEDENCE</tt> with this attribute's precedence value to the new 
	 * <tt>CELL</tt> arising from the copy. 
	 * <br><br>
	 * @param copy_entity
	 * the new <tt>CELL</tt> that is a copy of this attribute's owner.
	 */
	virtual void copy_owner( ENTITY *copy_entity );

	/**
	* Determines if two attributes are equivalent. 
	* This method should be overloaded in all derived classes.
	* <br><br>
	* @param other_att
	* the other attribute with which to compare this attribute.
	*/
	virtual logical same_as( const ATTRIB_CELL_PRECEDENCE *other_att ) const;

	/**
	* Gets the relative precedence of this cell during cell recompuation. 
	* This method should be overloaded in all derived classes.
	* <br><br>
	* @param other_att
	* the other attribute with which to compare this attribute.
	* @param result
	* relative precedence result.
	*/
	virtual void determine_precedence ( const ATTRIB_CELL_PRECEDENCE *other_att, ct_precedence_type& result ) const;

	// Standard housekeeping functions
	/**
	 * @nodoc
	 */
	ATTRIB_FUNCTIONS( ATTRIB_CELL_PRECEDENCE, CT )
#if 0
; // semicolon needed for mkman (doc tool) parsing)
#endif
// Because mkman isn't aware that ATTRIB_FUNCTIONS changes access to "public", we do so
// explicitly here:
public:

};

/**
 * Finds either the first or the next <tt>ATTRIB_CELL_PRECEDENCE</tt> on an entity.
 * For the next attribute, pass the current attribute into the function.
 * <br><br>
 * @param ent
 * the owner of the attribute for the first call; the previous attribute for subsequent calls.
 */
DECL_CT ATTRIB_CELL_PRECEDENCE* find_cell_precedence_attribute( const ENTITY* ent );

// Global function to determine which attribute or cell to retain.
// This function is not needed outside the CT component.
/**
* @nodoc
*/
void ct_decide_which ( 
				const ATTRIB_CELL_PRECEDENCE *first_att, 
				const ATTRIB_CELL_PRECEDENCE *second_att, 
				ct_precedence_type& result );

/** @} */
#endif
