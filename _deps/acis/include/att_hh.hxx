/*******************************************************************/
/*    Copyright (c) 1989-2020 by Spatial Corp.                     */
/*    All rights reserved.                                         */
/*    Protected by U.S. Patents 5,257,205; 5,351,196; 6,369,815;   */
/*                              5,982,378; 6,462,738; 6,941,251    */
/*    Protected by European Patents 0503642; 69220263.3            */
/*    Protected by Hong Kong Patent 1008101A                       */
/*******************************************************************/

//***************************************************************************
//  MASTER ACIS ATTRIBUTE CLASS FOR HH
//***************************************************************************

#if !defined( ATTRIB_HH_CLASS )
#define ATTRIB_HH_CLASS
//
// Attribute declaration for a private container class "ATTRIB_HH"
// This is used by all Husks Developed By HH.
//

/**
 * @file att_hh.hxx
 * @CAA2Level L1
 * @CAA2Usage U1
 * \addtogroup STITCHAPI
 *
 * @{
 */
#include "attrib.hxx"

#include "dcl_stitch.h"

// For internal use only
/**
 * @nodoc
 */
ENTITY_IS_PROTOTYPE(ATTRIB_HH, STITCH)
#if 0
; // semicolon needed for mkman (doc tool) parsing
#endif

// For internal use only
/**
 * @nodoc
 */
extern DECL_STITCH int ATTRIB_HH_TYPE ;
/**
 * @nodoc
 */
#define ATTRIB_HH_LEVEL (ATTRIB_LEVEL + 1)


// RS (Mar 02,2002) : Define the full class to add new functions
/**
 * @nodoc
 * <b>Obsolete:</b> This class has been deprecated. For healing, use API interfaces
 * and not Direct Interface.
 * <br><br>
 * Defines the owning organization for other HEAL attribute classes.
 * <br><br>
 * <b>Role</b>: <tt>ATTRIB_HH</tt> is the organizational class from which the other HEAL attribute classes are
 * derived. Its sole purpose is to identify its children classes as belonging to HEAL, and so adds no
 * new data or methods to those of the ACIS base attribute class, <tt>ATTRIB</tt>.
 */
class DECL_STITCH ATTRIB_HH : public ATTRIB
{
protected:
	logical m_is_restored;

public:
/**
 * C++ initialize constructor requests memory for this object and populates it with the data supplied as arguments.
 * <br><br>
 * <b>Role</b>:Applications should call this constructor only with the overloaded <tt>new</tt> operator inherited
 * from the <tt>ENTITY</tt> class (for example, <tt>x=new ATTRIB_HH(...)</tt>), because this reserves the memory on the heap,
 * a requirement to support roll back and history management.
 * <br><br>
 * @param e
 * owning entity
 */
	ATTRIB_HH ( ENTITY *e = NULL );

	// Function overrided to avoid copying.
// For internal use only
/**
 * @nodoc
 */
	virtual logical copyable () const;

	virtual logical savable () const;

// For internal use only
	/**
	 * @nodoc
	 */
	ATTRIB_FUNCTIONS (ATTRIB_HH, STITCH)
#if 0
; // semicolon needed for mkman (doc tool) parsing)
#endif
// Because mkman isn't aware that ATTRIB_FUNCTIONS changes access to "public", we do so
// explicitly here:
public:

};
// RS - end
/** @} */
#endif

