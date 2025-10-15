/* ORIGINAL: acis2.1/ga_husk/attrib/at_ent.hxx */
// $Id: at_ent.hxx,v 1.8 2002/08/09 17:19:21 jeff Exp $
/*******************************************************************/
/*    Copyright (c) 1989-2020 by Spatial Corp.                     */
/*    All rights reserved.                                         */
/*    Protected by U.S. Patents 5,257,205; 5,351,196; 6,369,815;   */
/*                              5,982,378; 6,462,738; 6,941,251    */
/*    Protected by European Patents 0503642; 69220263.3            */
/*    Protected by Hong Kong Patent 1008101A                       */
/*******************************************************************/
// Define class ATTRIB_GEN_ENTITY to provide generic attributes with
// pointers to owned entities

#ifndef ATTRIB_GEN_ENTITY_CLASS
#define ATTRIB_GEN_ENTITY_CLASS


// include files---

#include "dcl_ga.h"
#include "at_name.hxx"

/**
* @file at_ent.hxx
 * @CAA2Level L1
 * @CAA2Usage U2
 * \addtogroup GAAPI
 *
 * @{
 */
//======================================================================
/**
 * @nodoc
 */
ENTITY_IS_PROTOTYPE(ATTRIB_GEN_ENTITY, GA)
#if 0
; // semicolon needed for mkman (doc tool) parsing
#endif

/**
 * @nodoc
 */
extern DECL_GA int ATTRIB_GEN_ENTITY_TYPE;

/**
 * @nodoc
 */
#define ATTRIB_GEN_ENTITY_LEVEL (ATTRIB_GEN_NAME_LEVEL + 1)

/**
 * Defines a generic attribute that "owns" an entity.
 * <br>
 * <b>Role:</b> <tt>ATTRIB_GEN_ENTITY</tt> maintains a strong relationship
 * with the entity it "owns":
 * <ul>
 * <li>If an <tt>ATTRIB_GEN_ENTITY</tt> is lost, it will lose the entity it "owns".
 * <li>If an <tt>ATTRIB_GEN_ENTITY</tt> is transformed, it will transform the entity it "owns".
 * <li>If an <tt>ATTRIB_GEN_ENTITY</tt> is saved, it will save the entity it "owns".
 * <li>If an <tt>ATTRIB_GEN_ENTITY</tt> is copied, it will copy the entity it "owns".
 * </ul>
 * @see ENTITY, ATTRIB_GEN_POINTER
 */
class DECL_GA ATTRIB_GEN_ENTITY: public ATTRIB_GEN_NAME 
{
	ENTITY *Value;

protected:
 /**
  * Transforms the entity owned by this attribute in response to the <tt>trans_owner</tt> method.
  */
	virtual void do_transform(SPAtransf const &, ENTITY_LIST &);

public:

	// Default constructor - used in backup and restore
	/**
	 * C++ allocation constructor requests memory for this object but does not populate it.
	 * <br><br>
	 * <b>Role:</b> The allocation constructor is used primarily by restore.
	 */
	ATTRIB_GEN_ENTITY();

	// Create a generic pointer attribute
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
	ATTRIB_GEN_ENTITY(
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
	 * Changes the entity owned by this attribute.
	 * <br><br>
	 * @param val
	 * entity value.
	 */
	void set_value(ENTITY *val);

	/**
	 * Returns the attribute class identification.
	 */
	static int id();

	/**
	 * Determines if the attribute class is the specified type.
	 * <br><br>
	 * @param t
	 * class type to check.
	 */
	logical isa(int t) const;

	/**
	 * Return the entity owned by this attribute.
	 */
	ENTITY *value() const {return Value;}

	/**
	 * @nodoc
	 */
	ATTRIB_FUNCTIONS(ATTRIB_GEN_ENTITY, GA)
#if 0
; // semicolon needed for mkman (doc tool) parsing)
#endif
// Because mkman isn't aware that ATTRIB_FUNCTIONS changes access to "public", we do so
// explicitly here:
public:

};

 /** @} */
#endif
