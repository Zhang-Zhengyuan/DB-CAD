/* ORIGINAL: acis2.1/ga_husk/attrib/at_int.hxx */
// $Id: at_int.hxx,v 1.6 2001/05/22 15:36:00 jenglund Exp $
/*******************************************************************/
/*    Copyright (c) 1989-2020 by Spatial Corp.                     */
/*    All rights reserved.                                         */
/*    Protected by U.S. Patents 5,257,205; 5,351,196; 6,369,815;   */
/*                              5,982,378; 6,462,738; 6,941,251    */
/*    Protected by European Patents 0503642; 69220263.3            */
/*    Protected by Hong Kong Patent 1008101A                       */
/*******************************************************************/
// Define class ATTRIB_GEN_INTEGER to provide generic attributes with
// integer values

#ifndef ATTRIB_GEN_INTEGER_CLASS
#define ATTRIB_GEN_INTEGER_CLASS


// include files---

#include "dcl_ga.h"
#include "at_name.hxx"

/**
* @file at_int.hxx
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
ENTITY_IS_PROTOTYPE(ATTRIB_GEN_INTEGER, GA)
#if 0
; // semicolon needed for mkman (doc tool) parsing
#endif

/**
 * @nodoc
 */
extern DECL_GA int ATTRIB_GEN_INTEGER_TYPE;
/**
 * @nodoc
 */
#define ATTRIB_GEN_INTEGER_LEVEL (ATTRIB_GEN_NAME_LEVEL + 1)


/**
 * Defines a generic attribute that contains an integer value.
 */
class DECL_GA ATTRIB_GEN_INTEGER: public ATTRIB_GEN_NAME {

	int Value;

public:

	// Default constructor - used in backup and restore
	/**
	 * C++ allocation constructor requests memory for this object but does not populate it.
	 * <br><br>
	 * <b>Role:</b> The allocation constructor is used primarily by restore.
	 */
	ATTRIB_GEN_INTEGER();

	/**
	 * C++ initialize constructor requests memory for this object and populates it with the data supplied as arguments.
	 * <br><br>
	 * <b>Role:</b> Applications should call this constructor only with the overloaded <tt>new</tt> operator
	 * inherited from the <tt>ENTITY</tt> class (for example, <tt>x=new ATTRIB_GEN_INTEGER(...)</tt>), because this
	 * reserves the memory on the heap, a requirement to support roll back and history management.
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
	ATTRIB_GEN_INTEGER(
			ENTITY *owner,
			char const *name,
			int value,
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
	 * Changes the integer value contained by this attribute.
	 * <br><br>
	 * @param val
	 * integer value.
	 */
	void set_value(int val);

	/**
	 * Returns the attribute class identification.
	 */
	static int id();

	/**
	 * Determines if the attribute class is the specified type.
	 * <br><br>
	 * @param t
	 * type to check.
	 */
	logical isa(int t) const;

	/**
	 * Returns the integer value contained by this attribute.
	 */
	int value() const {return Value;}

	/**
	 * @nodoc
	 */
	ATTRIB_FUNCTIONS(ATTRIB_GEN_INTEGER, GA)
#if 0
; // semicolon needed for mkman (doc tool) parsing)
#endif
// Because mkman isn't aware that ATTRIB_FUNCTIONS changes access to "public", we do so
// explicitly here:
public:

};

 /** @} */
#endif
