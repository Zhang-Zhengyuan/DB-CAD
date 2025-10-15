// $Id: bullsmal.hxx,v 1.4 2001/01/11 18:08:28 ywoo Exp $
/*******************************************************************/
/*    Copyright (c) 1989-2020 by Spatial Corp.                     */
/*    All rights reserved.                                         */
/*    Protected by U.S. Patents 5,257,205; 5,351,196; 6,369,815;   */
/*                              5,982,378; 6,462,738; 6,941,251    */
/*    Protected by European Patents 0503642; 69220263.3            */
/*    Protected by Hong Kong Patent 1008101A                       */
/*******************************************************************/
//-----------------------------------------------------------------------
// Purpose---
//	Factor out a few things needed in some big headers, such as entity.hxx
//  and api.hxx so other changes to bulletin.hxx don't cause so many compiles.
//-----------------------------------------------------------------------

#ifndef __BULLSMAL_HXX__
#define __BULLSMAL_HXX__

// ywoo 08Jan01: added the header file.
#include "dcl_kern.h"
#include "logical.h"
#include "base.hxx"

class HISTORY_STREAM;

/**
* @file bullsmal.hxx
 * @CAA2Level L1
 * @CAA2Usage U1
 * \addtogroup ACISHISTORY
 *
 * @{
 */

// ywoo: end

// BULLETIN_TYPE is used by entity.hxx

// A bulletin has a type signifying creation, change or deletion of
// a model entity. This is not stored, but deduced from the presence
// or absence of new and old entity pointers.


/**
* Specifies creation, change, or deletion of a model entity.
* @param NO_BULLETIN
* no event.
* @param CREATE_BULLETIN
* entity creation.
* @param CHANGE_BULLETIN
* entity change.
* @param DELETE_BULLETIN
* entity deletion.
**/

enum BULLETIN_TYPE {
	NO_BULLETIN,
	CREATE_BULLETIN,
	CHANGE_BULLETIN,
	DELETE_BULLETIN
};

/**
 * Values that specify the application logging setting of a history stream.
 * @param stream_logging_unspecified
 * A value that indicates that the stream specific application logging setting has not been specified.
 * @param stream_logging_disabled
 * A value that indicates that application logging is disabled for a stream.
 * @param stream_logging_enabled
 * A value that indicates that application logging is enabled for a stream.

 */
enum stream_logging {
	stream_logging_unspecified = -1,
	stream_logging_disabled,
	stream_logging_enabled
};

// update_from_bb is used by api.hxx.

// Function called after each successful API function, which calls the
// processing function if this is at the topmost level.


/*
// tbrv
*/
/**
 * @nodoc
 */

DECL_KERN void update_from_bb();


// The flags are not used in so many places, but don't change much so ...

// Flags to control rollback.

// Flag (and option) to control whether bulletin-boards and delta
// states are to be visible at the application level. If set off,
// each bulletin board is deleted as soon as the next is opened, and
// application functions to get bulletin boards or delta states always
// return NULL.

/*
// tbrv
*/
/**
 * @nodoc
 */

DECL_KERN void set_application_logging( logical, HISTORY_STREAM* = NULL );

/*
// tbrv
*/
/**
 * @nodoc
 */

DECL_KERN logical application_logging_on( HISTORY_STREAM* = NULL);

// Flag to set logging for rollback on or off. It should only be
// used temporarily, and in exceptional circumstances.
/**
 * @nodoc
 */
#define logging (logging_opt_on())

/**
* Flag to control whether rollback records are produced or not.
* <br><br>
* <b>Role:</b> This function is used in special circumstances to make an entity
* permanent notwithstanding rollback, and during rollback logging, to avoid
* infinite recursion.
* Specifically, the <tt>set_logging</tt> function specifies whether BULLETINs are created in an application. 
* This is different from <tt>api_logging</tt>, which keeps BULLETINs and BULLETIN_BOARDs hidden 
* from the application level, but still creates them to support error recovery. 
* When <tt>set_logging(FALSE)</tt> is used, no history objects are created (see exception below). 
* Hence, rolling to the beginning of an operation in the event of a failure does not occur. 
* This may leave a model in a corrupt state. This setting should be used with caution, and only 
* when a custom recovery process has been implemented.
* <br><br>
* The exception to the BULLETIN prohibition when <tt>set_logging</tt> is set to FALSE is within 
* the bounds of API_TRIAL_BEGIN and API_TRIAL_END or API_NOP_BEGIN and API_NOP_END blocks. 
* To support to trial or read-only functionality of these macros, BULLETINs are created regardless 
* of the <tt>set_logging</tt> value to support returning the model to its initial state when an 
* error or the *_END block is reached.
* <br><br>
* <b>Effect:</b> Changes model
* <br><br>
* @param onoff
* set or not.
**/

DECL_KERN void set_logging( logical onoff );

/*
// tbrv
*/
/**
 * @nodoc
 */

DECL_KERN logical logging_opt_on();

/** @} */
#endif //__BULLTYPE_HXX__
