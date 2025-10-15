/*******************************************************************/
/*    Copyright (c) 1989-2020 by Spatial Corp.                     */
/*    All rights reserved.                                         */
/*    Protected by U.S. Patents 5,257,205; 5,351,196; 6,369,815;   */
/*                              5,982,378; 6,462,738; 6,941,251    */
/*    Protected by European Patents 0503642; 69220263.3            */
/*    Protected by Hong Kong Patent 1008101A                       */
/*******************************************************************/
//        Procedures for getting the owner of an ENTITY
/*******************************************************************/
#ifndef getowner_hxx
#define getowner_hxx
#include "dcl_kern.h"
#include "logical.h"
class ENTITY;
class SPAtransf;
/**
 * @file getowner.hxx
 * @CAA2Level L1
 * @CAA2Usage U1
 * \addtogroup ACISQUERIES
 *
 * @{
 */
/**
 * @nodoc
 * Gets the top level owner of an entity.
 */
DECL_KERN ENTITY* get_owner( ENTITY* );
/**
 * @nodoc
 * Get a copy of the SPAtransf from the owner of an ENTITY.
 */
DECL_KERN SPAtransf get_owner_transf( const ENTITY* );
/**
 * @nodoc
 * Gets the pointer to the SPAtransf from the owner of an ENTITY.
 */
DECL_KERN SPAtransf const *get_owner_transf_ptr( const ENTITY* );
/**
* Determines if an <tt>ENTITY</tt> is top level. This means there is no owner
* for this <tt>ENTITY</tt>.
* <br><br>
* <b>Effect:</b> Read-only
* <br><br>
* @param ent
* entity.
**/
DECL_KERN logical is_toplevel(const ENTITY *ent);
/** @} */
#endif
