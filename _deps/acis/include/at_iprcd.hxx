/*******************************************************************/
/*    Copyright (c) 1989-2020 by Spatial Corp.                     */
/*    All rights reserved.                                         */
/*    Protected by U.S. Patents 5,257,205; 5,351,196; 6,369,815;   */
/*                              5,982,378; 6,462,738; 6,941,251    */
/*    Protected by European Patents 0503642; 69220263.3            */
/*    Protected by Hong Kong Patent 1008101A                       */
/*******************************************************************/

/*******************************************************************/
// let 02Feb12 Created New ATTRIB_EDA Class.
// let 21Feb12 Renamed ATTRIB_ADA to ATTRIB_CELL_INT_PRECEDENCE
/*******************************************************************/

#if !defined( ATTRIB_CELL_INT_PRECEDENCE_CLASS )
#define ATTRIB_CELL_INT_PRECEDENCE_CLASS

#include "dcl_ct.h"
#include "at_prcd.hxx"

/**
* @file at_iprcd.hxx
 * @CAA2Level L1
 * @CAA2Usage U1
 * \addtogroup CELLTOPAPI
 * @{
 */

/**
 * Determines if an <tt>ENTITY</tt> is a <tt>ATTRIB_CELL_INT_PRECEDENCE</tt>.
 * <br><br>
 * <b>Role:</b> This procedure takes an <tt>ENTITY</tt> as an argument and returns <tt>TRUE</tt>
 * if the entity is a <tt>ATTRIB_CELL_INT_PRECEDENCE</tt>, <tt>FALSE</tt> if it is not. 
 * This procedure also checks for a <tt>NULL</tt> pointer.
 * <br><br>
 * <b>Effect:</b> Read-only
 * <br><br>
 * @param ent
 * entity.
 **/
logical DECL_CT is_ATTRIB_CELL_INT_PRECEDENCE( const ENTITY *ent );

/**
 * @nodoc
 */
ENTITY_IS_PROTOTYPE(ATTRIB_CELL_INT_PRECEDENCE, CT )
#if 0
; // semicolon needed for mkman (doc tool) parsing
#endif

/**
 * @nodoc
 */
extern DECL_CT int ATTRIB_CELL_INT_PRECEDENCE_TYPE;

/**
 * @nodoc
 */
#define ATTRIB_CELL_INT_PRECEDENCE_LEVEL (ATTRIB_CELL_PRECEDENCE_LEVEL + 1)


/**
 * Specifies the precedence associated with a 2D or 3D cell.
 * <br>
 * <b>Role:</b> This class is an implementation of a cell precedence attribute to be attached to a cell. 
 * Cell precedence attributes affect the propagation of attributes on cells during cell recomputation.
 * In this class the precedence is specified by an integer value. The higher the integer value the higher
 * the precedence of the cell owning the attribute. When two cells are combined (for example, during 
 * a nonregularized unite or cell merge operation), the resulting cell will have the higher precedence value. 
 * This attribute also contains a label, which can be used to identify the specific cell or the material 
 * of the cell.
 */

class DECL_CT ATTRIB_CELL_INT_PRECEDENCE: public ATTRIB_CELL_PRECEDENCE {

private:

	//	Data members that specify the precedence value and a label. 

	int my_precedence;
	char * my_label;

public:

	// Construct the initial attribute 
    /**
     * C++ initialize constructor requests memory for this object and populates it with the data supplied as arguments.
     * <br><br>
     * <b>Role:</b> Applications should call this constructor only with the overloaded 
	 * <tt>new</tt> operator inherited from the <tt>ENTITY</tt> class (for example, 
	 * <tt>x = new ATTRIB_CELL_INT_PRECEDENCE(...)</tt>), because this reserves
     * the memory on the heap, a requirement to support roll back and history management. 
	 * Generally, applications should call the constructor with using the <tt>ACIS_NEW</tt> macro, 
	 * which calls the overloaded <tt>new</tt> operator.
     * <br><br>
     * @param c
     * owning cell.
     */

	ATTRIB_CELL_INT_PRECEDENCE( CELL* c = NULL, int prcd = 0, const char* str = NULL);

    /**
     * @nodoc
     */
     virtual logical pattern_compatible() const;
                                    // returns TRUE


    //	ATTRIB_CELL_INT_PRECEDENCE propagation functions

    /**
     * Indicates if this attribute can be propagated to the specified <tt>CELL</tt>.
	 * Checks whether the given <tt>CELL</tt> entity has an attribute of class 
	 * <tt>ATTRIB_CELL_INT_PRECEDENCE</tt>. If an <tt>ATTRIB_CELL_INT_PRECEDENCE</tt> 
	 * already exists on the given <tt>CELL</tt>, this method returns <tt>FALSE</tt>; 
	 * otherwise it returns <tt>TRUE</tt>. This method should be overloaded in derived classes.
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
	 * For an <tt>ATTRIB_CELL_INT_PRECEDENCE</tt> the default behavior is to attach a new 
	 * <tt>ATTRIB_CELL_INT_PRECEDENCE</tt> with this attribute's precedence value to the new 
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
	 * be deleted. 
	 * <br><br>
	 * For an <tt>ATTRIB_CELL_INT_PRECEDENCE</tt> the default behavior is to select the cell 
	 * possessing the <tt>ATTRIB_CELL_INT_PRECEDENCE</tt> with the highest precedence value, 
	 * when this cell is being deleted. (<tt>merge_owner</tt> is called multiple times when entities
	 * are merged, so this overloaded method operates only when the owner is being deleted.)
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
	 * For an <tt>ATTRIB_CELL_INT_PRECEDENCE</tt> the default behavior is to attach a new 
	 * <tt>ATTRIB_CELL_INT_PRECEDENCE</tt> with this attribute's precedence value to the new 
	 * <tt>CELL</tt> arising from the copy. 
	 * <br><br>
	 * @param copy_entity
	 * the new <tt>CELL</tt> that is a copy of this attribute's owner.
	 */
	virtual void copy_owner( ENTITY *copy_entity );

	// *******************************************
	//  Application specific member functions
	// *******************************************

	/**
	* Determines if two attributes are equivalent. 
	* This method should be overloaded in all derived classes.
	* <br><br>
	* @param other_att
	* the other ATTRIB_CELL_INT_PRECEDENCE with which to compare this attribute.
	*/
	virtual logical same_as( const ATTRIB_CELL_PRECEDENCE *other_att ) const;

	/**
	* Gets the precedence value of this attribute.
	*/
	int get_precedence() const { return my_precedence; }

	/**
	* Gets the label of this attribute.
	*/
	const char* get_label() const{ return my_label; }

	/**
	* Sets the precedence value of this attribute.
	* <br><br>
	* @param new_prcd
	* the new precedence value for this attribute.
	*/
	void set_precedence( int new_prcd );

	/**
	* Sets the label of this attribute.
	* <br><br>
	* @param str
	* the new label for this attribute.
	*/
	void set_label( char const *str );

	/**
	* Gets the precedence of this attribute with respect to another attribute. 
	* This method should be overloaded in all derived classes.
	* <br><br>
	* When two cells are being merged into one cell, this method determines
	* which of the two cells should be retained and which should be deleted.
	* If cells are split and merged as a result of the operation, the original
	* higher priority cell may not persist, but its attribute (or a copy of 
	* it attribute) will. 
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
	ATTRIB_FUNCTIONS( ATTRIB_CELL_INT_PRECEDENCE, CT )
#if 0
; // semicolon needed for mkman (doc tool) parsing)
#endif
// Because mkman isn't aware that ATTRIB_FUNCTIONS changes access to "public", we do so
// explicitly here:
public:

	/**
	 * @nodoc
	 */
	FULLSIZE_FUNCTION
#if 0
; // semicolon needed for mkman (doc tool) parsing)
#endif

};

/**
 * Finds either the first or the next <tt>ATTRIB_CELL_INT_PRECEDENCE</tt> on an entity.
 * For the next attribute, pass the current attribute into the function.
 * <br><br>
 * @param ent
 * the owner of the attribute for the first call; the previous attribute for subsequent calls.
 */
DECL_CT ATTRIB_CELL_INT_PRECEDENCE* find_cell_int_precedence_attribute( const ENTITY* ent );

/** @} */
#endif

