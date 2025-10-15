/* ORIGINAL: acis2.1/ga_husk/attrib/at_ptr.hxx */
// $Id: at_ptr.hxx,v 1.6 2001/05/22 15:36:00 jenglund Exp $
/*******************************************************************/
/*    Copyright (c) 1989-2020 by Spatial Corp.                     */
/*    All rights reserved.                                         */
/*    Protected by U.S. Patents 5,257,205; 5,351,196; 6,369,815;   */
/*                              5,982,378; 6,462,738; 6,941,251    */
/*    Protected by European Patents 0503642; 69220263.3            */
/*    Protected by Hong Kong Patent 1008101A                       */
/*******************************************************************/
// Define class ATTRIB_GEN_POINTER to provide generic attributes with
// pointers to shared entities

#ifndef ATTRIB_GEN_POINTER_CLASS
#define ATTRIB_GEN_POINTER_CLASS


// include files---

#include "dcl_ga.h"
#include "at_name.hxx"

/**
* @file at_ptr.hxx
 * @CAA2Level L1
 * @CAA2Usage U2
 * \addtogroup GAAPI
 *
 * @{
 */

/**
 * @nodoc
 */
ENTITY_IS_PROTOTYPE(ATTRIB_GEN_POINTER, GA)
#if 0
; // semicolon needed for mkman (doc tool) parsing
#endif

/**
 * @nodoc
 */
extern DECL_GA int ATTRIB_GEN_POINTER_TYPE;
/**
 * @nodoc
 */
#define ATTRIB_GEN_POINTER_LEVEL (ATTRIB_GEN_NAME_LEVEL + 1)


/**
 * Defines a generic attribute that contains a reference to an entity.
 * <br>
 * <b>Role:</b> <tt>ATTRIB_GEN_POINTER</tt> maintains a fairly weak relationship
 * with the referenced entity:
 * <ul>
 * <li>If an <tt>ATTRIB_GEN_POINTER</tt> is lost, it will not lose its referenced entity.
 * <li>If an <tt>ATTRIB_GEN_POINTER</tt> is transformed, it will not transform its referenced entity.
 * <li>If an <tt>ATTRIB_GEN_POINTER</tt> is saved, it will save its referenced entity.
 * <li>If an <tt>ATTRIB_GEN_POINTER</tt> is copied, it will not "pull" its referenced entity 
 * into the copy. However, if the referenced entity is "pulled" into the copy by some other means, 
 * then the copied <tt>ATTRIB_GEN_POINTER</tt> will properly reference the copied referenced 
 * entity. If the referenced entity is not pulled into the copy by any means, then 
 * the copied <tt>ATTRIB_GEN_POINTER</tt> will reference a <tt>NULL</tt> entity.
 * </ul>
 * @see ENTITY, ATTRIB_GEN_ENTITY
 */
class DECL_GA ATTRIB_GEN_POINTER: public ATTRIB_GEN_NAME 
{
	ENTITY *Value;

public:

	/**
	 * C++ allocation constructor requests memory for this object but does not populate it.
	 * <br><br>
	 * <b>Role:</b> The allocation constructor is used primarily by restore.
	 */
	ATTRIB_GEN_POINTER();

	/**
	 * C++ initialize constructor requests memory for this object and populates it with the data supplied as arguments.
	 * <br><br>
	 * @param owner
	 * owning entity.
	 * @param name
	 * name.
	 * @param value
	 * value.
	 * @param spl_act
	 * split action.
	 * @param mer_act
	 * merge action.
	 * @param tran_act
	 * transformation action.
	 * @param cop_act
	 * copy action.
	 */
	ATTRIB_GEN_POINTER(
			ENTITY *owner,
			char const *name,
			ENTITY *value,
			split_action spl_act = SplitKeep,
			merge_action mer_act = MergeKeepKept,
			trans_action tran_act = TransIgnore,
			copy_action cop_act = CopyCopy
		);

	/**
	 * Returns <tt>TRUE</tt> if this is pattern compatible.
	 */
    virtual logical pattern_compatible() const;

	/**
	 * Changes the entity referenced by this attribute.
	 * <br><br>
	 * @param val
	 * value.
	 */
	void set_value(ENTITY *val);

	/**
	 * Returns the attribute class identification.
	 */
	static int id();

	/**
	 * Determines if the attribute class is of the specified type.
	 * <br><br>
	 * @param t
	 * type to check.
	 */
	logical isa(int t) const;

	/**
	 * Returns the <tt>ENTITY</tt> referenced by this attribute.
	 */
	ENTITY *value() const {return Value;}

	/**
	 * @nodoc
	 */
	ATTRIB_FUNCTIONS(ATTRIB_GEN_POINTER, GA)
#if 0
; // semicolon needed for mkman (doc tool) parsing)
#endif
// Because mkman isn't aware that ATTRIB_FUNCTIONS changes access to "public", we do so
// explicitly here:
public:

};

 /** @} */
#endif
