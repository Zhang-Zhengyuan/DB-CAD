/*******************************************************************/
/*    Copyright (c) 1989-2020 by Spatial Corp.                     */
/*    All rights reserved.                                         */
/*    Protected by U.S. Patents 5,257,205; 5,351,196; 6,369,815;   */
/*                              5,982,378; 6,462,738; 6,941,251    */
/*    Protected by European Patents 0503642; 69220263.3            */
/*    Protected by Hong Kong Patent 1008101A                       */
/*******************************************************************/
#if !defined( PID_ROUTINES )
#define PID_ROUTINES

class ENTITY;
class pid_base;
class outcome;

#include "acis.hxx"
#include "dcl_pid.h"
#include "api.hxx"
#include "logical.h"
/**
 * @nodoc
 * \defgroup PERSISTIDAPI Persistent ID
 * \ingroup KERNAPI
 */
/**
 * from session to session. The identifier is designed to be unique over 
 * all sessions of ACIS. The user has great flexibility in controlling 
 * the migration of the identifier through ACIS processing. 
 * The identifier is attached to an entity by ACIS ATTRIBUTE objects.
 */
/**
 * @nodoc
 * @file pidapi.hxx
 * @CAA2Level L1
 * @CAA2Usage U1
 * \addtogroup PERSISTIDAPI
 *
 * @{
 */

// Make routines callable from non-C++ applications.

/**
* @nodoc
* Initializes the persistent identification library.
* <br><br>
* <b>Effect:</b> System routine.
* <br><br>
* <b>Journal: </b> Not Available
* <br><br>
* <b>Product(s):</b> 3D ACIS Modeler
 * <br><br>
**/
DECL_PID outcome api_initialize_persistent_id();


/**
* @nodoc
* Terminates the persistent identification library.
* <br><br>
* <b>Effect:</b> System routine.
* <br><br>
* <b>Journal: </b> Not Available
* <br><br>
* <b>Product(s):</b> 3D ACIS Modeler
 * <br><br>
**/
DECL_PID outcome api_terminate_persistent_id();

/**
* @nodoc
* Sets the <tt>PID</tt> of the entity.
* <br><br>
* <b>Role:</b> This <tt>API</tt> sets the <tt>PID</tt> of the entity to the next current one.
* <br><br>
* <b>Errors:</b> Pointer to entity is <tt>NULL</tt>.
* <br><br>
* <b>Effect:</b> Changes model.
* <br><br>
* <b>Journal: </b> Not Available
* <br><br>
* <b>Product(s):</b> 3D ACIS Modeler
 * <br><br>
* @param given_entity
* entity to have PID added.
* @param ao
* acis options.
**/
DECL_PID outcome api_pidset(
	ENTITY*		given_entity
	,AcisOptions * ao = NULL);

/**
* @nodoc
* Gets the persistent <tt>ID</tt> (<tt>PID</tt>) of the entity.
* <br><br>
* <b>Errors:</b> Pointer to entity is <tt>NULL</tt>.
* <br><br>
* <b>Effect:</b> Read-only.
* <br><br>
* <b>Journal: </b> Not Available
* <br><br>
* <b>Product(s):</b> 3D ACIS Modeler
 * <br><br>
* @param given_entity
* entity to get PID from.
* @param pid
* returned PID of the entity.
* @param ao
* acis options.
**/
DECL_PID outcome api_pidget(
	ENTITY*		given_entity,
	const pid_base*& pid
	,AcisOptions * ao= NULL);

/**
* @nodoc
* Removes the <tt>PID</tt> from the entity.
* <br><br>
* <b>Errors:</b> Pointer to entity is <tt>NULL</tt>.
* <br><br>
* <b>Effect:</b> Changes model.
* <br><br>
* <b>Journal: </b> Not Available
* <br><br>
* <b>Product(s):</b> 3D ACIS Modeler
 * <br><br>
* @param given_entity
* entity to have PID removed.
* @param ao
* acis options.
**/
DECL_PID outcome api_pidrem(
	ENTITY*		given_entity
	,AcisOptions * ao = NULL);

/** @} */
#endif
