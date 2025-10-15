/* ORIGINAL: acis2.1/ga_husk/attrib/at_real.hxx */
// $Id: at_real.hxx,v 1.6 2001/05/22 15:36:01 jenglund Exp $
/*******************************************************************/
/*    Copyright (c) 1989-2020 by Spatial Corp.                     */
/*    All rights reserved.                                         */
/*    Protected by U.S. Patents 5,257,205; 5,351,196; 6,369,815;   */
/*                              5,982,378; 6,462,738; 6,941,251    */
/*    Protected by European Patents 0503642; 69220263.3            */
/*    Protected by Hong Kong Patent 1008101A                       */
/*******************************************************************/
// Define class ATTRIB_GEN_REAL to provide generic attributes with
// real values

#ifndef ATTRIB_GEN_REAL_CLASS
#define ATTRIB_GEN_REAL_CLASS


// include files---

#include "dcl_ga.h"
#include "at_name.hxx"

/**
* @file at_real.hxx
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
ENTITY_IS_PROTOTYPE(ATTRIB_GEN_REAL, GA)
#if 0
; // semicolon needed for mkman (doc tool) parsing
#endif

/**
 * @nodoc
 */
extern DECL_GA int ATTRIB_GEN_REAL_TYPE;
/**
 * @nodoc
 */
#define ATTRIB_GEN_REAL_LEVEL (ATTRIB_GEN_NAME_LEVEL + 1)


/**
 * Defines a generic attribute that contains a real (double) value.
 */
class DECL_GA ATTRIB_GEN_REAL: public ATTRIB_GEN_NAME {

	double Value;

public:

	/**
	 * C++ allocation constructor requests memory for this object but does not populate it.
	 */
	ATTRIB_GEN_REAL();

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
	ATTRIB_GEN_REAL(
			ENTITY *owner,
			char const *name,
			double value,
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
	 * Changes the real value contained by this attribute.
	 * <br><br>
	 * @param val
	 * value.
	 */
	void set_value(double val);

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
	 * Returns the real (double) value contained by the attribute.
	 */
	double value() const {return Value;}

	/**
	 * @nodoc
	 */
	ATTRIB_FUNCTIONS(ATTRIB_GEN_REAL, GA)
#if 0
; // semicolon needed for mkman (doc tool) parsing)
#endif
// Because mkman isn't aware that ATTRIB_FUNCTIONS changes access to "public", we do so
// explicitly here:
public:

};

 /** @} */
#endif
