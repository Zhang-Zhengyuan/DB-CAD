/* ORIGINAL: acis2.1/ga_husk/attrib/at_vec.hxx */
// $Id: at_vec.hxx,v 1.9 2002/08/09 17:19:22 jeff Exp $
/*******************************************************************/
/*    Copyright (c) 1989-2020 by Spatial Corp.                     */
/*    All rights reserved.                                         */
/*    Protected by U.S. Patents 5,257,205; 5,351,196; 6,369,815;   */
/*                              5,982,378; 6,462,738; 6,941,251    */
/*    Protected by European Patents 0503642; 69220263.3            */
/*    Protected by Hong Kong Patent 1008101A                       */
/*******************************************************************/
// Define class ATTRIB_GEN_VECTOR to provide generic attributes with
// SPAvector values

#ifndef ATTRIB_GEN_VECTOR_CLASS
#define ATTRIB_GEN_VECTOR_CLASS


// include files---

#include "dcl_ga.h"
#include "at_name.hxx"

#include "vector.hxx"

/**
* @file at_vec.hxx
 * @CAA2Level L1
 * @CAA2Usage U2
 * \addtogroup GAAPI
 *
 * @{
 */

 /**
 * @nodoc
 */
ENTITY_IS_PROTOTYPE(ATTRIB_GEN_VECTOR, GA)
#if 0
; // semicolon needed for mkman (doc tool) parsing
#endif

/**
 * @nodoc
 */
extern DECL_GA int ATTRIB_GEN_VECTOR_TYPE;
/**
 * @nodoc
 */
#define ATTRIB_GEN_VECTOR_LEVEL (ATTRIB_GEN_NAME_LEVEL + 1)

/**
 * Defines a generic attribute that contains a vector.
 * @see SPAvector
 */
class DECL_GA ATTRIB_GEN_VECTOR: public ATTRIB_GEN_NAME {

	SPAvector Value;

  /**
   * Transforms the entity owned by this attribute in response to the <tt>trans_owner</tt> method.
   */
	virtual void do_transform(SPAtransf const &, ENTITY_LIST &);

public:

	/**
	 * C++ allocation constructor requests memory for this object but does not populate it.
	 * <br><br>
	 * <b>Role:</b> The allocation constructor is used primarily by restore.
	 */
	ATTRIB_GEN_VECTOR();

	/**
	 * C++ initialize constructor requests memory for this object and populates it with the
	 * data supplied as arguments.
	 * <br><br>
	 * @param owner
	 * owning entity.
	 * @param name
	 * name.
	 * @param value
	 * value.
	 * @param spt_act
	 * split action.
	 * @param mer_act
	 * merge action.
	 * @param tran_act
	 * transformation action.
	 * @param cop_act
	 * copy action.
	 */
	ATTRIB_GEN_VECTOR(
			ENTITY *owner,
			char const *name,
			SPAvector const &value,
			split_action spt_act = SplitKeep,
			merge_action mer_act = MergeKeepKept,
			trans_action tran_act = TransIgnore,
			copy_action cop_act = CopyCopy
		);

	/**
	 * Returns <tt>TRUE</tt> if this is pattern compatible.
	 */
    virtual logical pattern_compatible() const;

	/**
	 * Returns the attribute class identification.
	 */
	static int id();

	/**
	 * Determines if the attribute class is the specified type.
	 * <br><br>
	 * @param t
	 * type check.
	 */
	logical isa(int t) const;

	/**
	 * Changes the vector contained by this attribute.
	 * <br><br>
	 * @param val
	 * value.
	 */
	void set_value(SPAvector const &val);

	/**
	 * Returns the <tt>SPAvector</tt> contained by this attribute.
	 */
	SPAvector value() const {return Value;}

	/**
	 * @nodoc
	 */
	ATTRIB_FUNCTIONS(ATTRIB_GEN_VECTOR, GA)
#if 0
; // semicolon needed for mkman (doc tool) parsing)
#endif
// Because mkman isn't aware that ATTRIB_FUNCTIONS changes access to "public", we do so
// explicitly here:
public:

};

 /** @} */
#endif
