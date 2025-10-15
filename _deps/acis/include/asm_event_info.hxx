/*******************************************************************/
/*    Copyright (c) 1989-2020 by Spatial Corp.                     */
/*    All rights reserved.                                         */
/*    Protected by U.S. Patents 5,257,205; 5,351,196; 6,369,815;   */
/*                              5,982,378; 6,462,738; 6,941,251    */
/*    Protected by European Patents 0503642; 69220263.3            */
/*    Protected by Hong Kong Patent 1008101A                       */
/*******************************************************************/

#ifndef ASM_EVENT_INFO_HXX
#define ASM_EVENT_INFO_HXX

#include "base.hxx"
#include "dcl_asm.h"

/**
* @file asm_event_info.hxx
 * @CAA2Level L1
 * @CAA2Usage U1
 * \addtogroup ACISASSEMBLIES
 *
 * @{
 */

/**
 * @nodoc
 */
class DECL_ASM asm_event_info : public ACIS_OBJECT
{
public:

	/**
	 * C++ default constructor to construct an <tt>asm_event_info</tt> object.
	 */
    asm_event_info();

	/**
	 * Returns the ID number for the class <tt>asm_event_info</tt>.
	 */
    static int id();

	/**
	 * Returns the ID number for whichever class this object happens to be. If this object is of 
	 * the class <tt>asm_brep_transform_info</tt>, then the ID number returned by this function
	 * would be equal to the one returned by <tt>asm_brep_transform_info::id()</tt>.
	 */
    virtual int type() const;
};

/**
 * @nodoc
 */
class DECL_ASM asm_brep_transform_info : public asm_event_info
{
public:

	/**
	 * C++ default constructor to construct an <tt>asm_brep_transform_info</tt> object.
	 */
    asm_brep_transform_info();

	/**
	 * Returns the ID number for the class <tt>asm_brep_transform_info</tt>.
	 */
    static int id();

	/**
	 * Returns the ID number for whichever class this object happens to be.
	 */
    virtual int type() const;
};

// used to assign unique ID to object derived from asm_event_info objects
/**
 * @nodoc
 */
DECL_ASM void assign_asm_info_type_no( int * aitn );


 /** @} */
#endif // ASM_EVENT_INFO_HXX

