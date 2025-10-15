/*******************************************************************/
/*    Copyright (c) 1989-2020 by Spatial Corp.                     */
/*    All rights reserved.                                         */
/*    Protected by U.S. Patents 5,257,205; 5,351,196; 6,369,815;   */
/*                              5,982,378; 6,462,738; 6,941,251    */
/*    Protected by European Patents 0503642; 69220263.3            */
/*    Protected by Hong Kong Patent 1008101A                       */
/*******************************************************************/
//----------------------------------------------------------------------
// purpose---
//   Declare class to monitor the active part.
//
//----------------------------------------------------------------------

#ifndef actpart_hxx
#define actpart_hxx

#include "dcl_part.h"
#include "part.hxx"
#include "logical.h"
/**
* @file actpart.hxx
 * @CAA2Level L1
 * @CAA2Usage U1
 * \addtogroup PARTAPI
 *
 * @{
 */

/**
 * Declares a class to monitor the active part.
 * @see PART
 */
class DECL_PART active_part : public ACIS_OBJECT
{
protected:
	PART *m_activePart;		// the active part

public:
/**
 * C++ constructor, creating an <tt>active_part</tt>.
 */
	active_part();
/**
 * C++ destructor for an <tt>active_part</tt>.
 */
	~active_part() {}
/**
 * Sets the internal data structure to point to the active part that needs to be monitored.
 * <br><br>
 * @param activePart
 * part to watch.
 */
void SetActivePart( PART *activePart);
/**
 * Returns a pointer to the active part being monitored.
 */
    PART *GetActivePart();
};

/**
 * @nodoc
 */
// Constructor
inline active_part::active_part() : m_activePart( 0) {}


// Export the active part pointer
#ifdef THREAD_SAFE_ACIS
extern DECL_PART safe_object_pointer<active_part> ActivePart;
#else
extern DECL_PART active_part * ActivePart;
#endif


/** @} */
#endif
