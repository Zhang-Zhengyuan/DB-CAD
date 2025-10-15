/* ORIGINAL: ACIS 5.0 kern\kernel\geomhusk\lawent.hx  */
/*******************************************************************/
/*    Copyright (c) 1989-2020 by Spatial Corp.                     */
/*    All rights reserved.                                         */
/*    Protected by U.S. Patents 5,257,205; 5,351,196; 6,369,815;   */
/*                              5,982,378; 6,462,738; 6,941,251    */
/*    Protected by European Patents 0503642; 69220263.3            */
/*    Protected by Hong Kong Patent 1008101A                       */
/*******************************************************************/
// $Id $
//----------------------------------------------------------------------
// purpose---
//    define a LAW class
//----------------------------------------------------------------------

#ifndef LAWENT_HXX
#define LAWENT_HXX

//======================================================================

#include "dcl_kern.h"
#include "entity.hxx"
/**
 * \defgroup ACISKERNLAWS Laws
 * \ingroup KERNAPI
 *
 */
/**
 * @file lawent.hxx
 * @CAA2Level L1
 * @CAA2Usage U1
 * \addtogroup ACISKERNLAWS
 *
 * @{
 */
/**
 * @nodoc
 */
ENTITY_IS_PROTOTYPE(LAW, KERN )
#if 0
; // semicolon needed for mkman (doc tool) parsing
#endif

// STI ROLL
class SizeAccumulator;
// STI ROLL

class law;


//----------------------------------------------------------------------

//======================================================================

// Identifier used to find out (via identity() defined below) to what
// an entity pointer refers.

/**
 * @nodoc
 */
extern DECL_KERN int LAW_TYPE;

// Identifier that gives number of levels of derivation of this class
// from ENTITY.

#define LAW_LEVEL 1

// Routine to restore a TEXT_ENT entity from disc (declared here to make it
// a friend and static).

/**
 * @nodoc
 */
LOCAL_PROC ENTITY* LAW_restore_data();

// TEXT_ENT declaration proper.
/**
 * Stores a law mathematics function as an entity for saving to and restoring from a SAT file.
 * <br>
 * <b>Role:</b> This class permits a law mathematic function to be written to a SAT file without
 * the requirement of it being attached to some model topology. This is used in conjunction with
 * <tt>api_law_to_entity</tt>. As such, equations used in some model analysis have some permanence from
 * session to session.
 * <br><br>
 * @see law
 */
class DECL_KERN LAW : public ENTITY
{
private:
	// the law definition

	law* law_def;

	// Include the standard member functions for all entities.

	/**
	 * @nodoc
	 */
	ENTITY_FUNCTIONS( LAW, KERN )
#if 0
; // semicolon needed for mkman (doc tool) parsing)
#endif
// Because mkman isn't aware that ENTITY_FUNCTIONS changes access to "public", we do so
// explicitly here:
public:


	/**
	 * @nodoc
	 */
	TRANSFORM_FUNCTION
#if 0
; // semicolon needed for mkman (doc tool) parsing)
#endif

	/**
	 * @nodoc
	 */
	LOOKUP_FUNCTION
#if 0
; // semicolon needed for mkman (doc tool) parsing)
#endif

public:

    //----------------------------------------
    // Constructors
/**
  * Constructs a <tt>LAW</tt> (default constructor).
  * <br><br>
  * <b>Role:</b> The allocation constructor is used primarily by restore. Applications should
  * call this constructor only with the overloaded <tt>new</tt> operator, as doing so
  * reserves the memory on the heap, a requirement to support roll back and history management.
  */
	LAW();
/**
  * Constructs a <tt>LAW</tt> from the specified <tt>LAW</tt> object.
  * <br><br>
  * <b>Role:</b> Applications should call this constructor only with the overloaded <tt>new</tt>
  * operator, as doing so reserves the memory on the heap, a requirement to support roll back
  * and history management.
  * <br><br>
  * @param law_copy
  * law mathematic function.
  */
	LAW(law* law_copy);

/**
  * Constructs a <tt>LAW</tt> from the specified <tt>LAW</tt> object (copy constructor).
  * <br><br>
  * <b>Role:</b> Applications should call this constructor only with the overloaded <tt>new</tt>
  * operator, as doing so reserves the memory on the heap, a requirement to support roll back
  * and history management.
  * <br><br>
  * @param law_math_func
  * law entity to copy.
  */
 	LAW(LAW* law_math_func);

    //----------------------------------------
    // Destructor
    //  The destructor of LAW is private.  To delete a LAW entity, use the
	//	lose method.

// This function is hidden from mkman in the RESTORE_DEF macro; to have it documented,
// we include it here:
#if 0
/**
 * Restores <tt>this</tt> LAW from a SAT file.
 * <br><br>
 * <b>Role:</b> The RESTORE_DEF macro expands to the <tt>restore_common</tt> method, 
 * which is used in reading information from a SAT file. This method is never called 
 * directly. It is called by a higher hierarchical function if an item in the SAT file 
 * is determined to be of this class type. An instance of this class will already have 
 * been created through the allocation constructor. This method then populates the class 
 * instance with the appropriate data from the SAT file.  
 * <pre>
 *    restore_law     Restore the underlying law mathematic function  
 * </pre>
 */
    void restore_common(); 
#endif

// These function are hidden from mkman in the ENTITY_FUNCTIONS macro; to have them documented,
// we include them here:
#if 0
/**
 * Returns a type identifier for <tt>this</tt> object.
 * <br><br>
 * <b>Role:</b> If <tt>level</tt> is unspecified or 0, returns the type identifier LAW_TYPE. If 
 * <tt>level</tt> is specified, returns LAW_TYPE for that level of derivation from ENTITY. The 
 * level of this class is defined as LAW_LEVEL.
 * <br><br>
 * @param level
 * the derivation level at which the object is to be identified.
 */
	virtual int identity( int level = 0 ) const;
/**
 * Returns the string "LAW".
 */
	virtual const char *type_name() const;
/**
 * Posts a delete bulletin to the bulletin board indicating <tt>this</tt> LAW is no 
 * longer used in the active model.
 * <br><br>
 * <b>Role:</b> The <tt>lose</tt> methods for attached attributes are also called.
 */
	virtual void lose();
#endif

/**
  * Returns the law mathematic function enclosed within the <tt>LAW</tt> entity.
  */
	law* def() const { return law_def; }
/**
  * Specifies a new law function to store in the <tt>LAW</tt> entity.
  * <br><br>
  * @param in_law
  * new law.
  */

	void set_def(law* in_law);
/**
  * Transforms a <tt>LAW</tt>.
  * <br><br>
  * <b>Role:</b> Before performing the change, this method checks whether or not the data
  * structure is posted on the bulletin board. If not, the routine calls <tt>backup</tt> to
  * put an entry on the bulletin board.
  * <br><br>
  * @param out_trans
  * output transform.
  */
	void operator*=(const SPAtransf& out_trans);

	// STI ROLL
protected:
	void full_size(SizeAccumulator& est, logical countSelf = TRUE) const;
    // STI ROLL
};
/** @} */
/**
 * \addtogroup ACISQUERIES
 * @{
 */
//----------------------------------------------------------------------
/**
* Determines if a specified <tt>ENTITY</tt> is a <tt>LAW</tt>.
* <br><br>
* <b>Effect:</b> Read-only
* <br><br>
* @param ent
* entity to test.
**/
DECL_KERN logical is_LAW (const ENTITY* ent);

//======================================================================
/** @} */
#endif
