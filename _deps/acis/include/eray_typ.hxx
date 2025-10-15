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
//    declare procedures for working with class entity_with_ray in scheme
//
//----------------------------------------------------------------------
#ifndef eray_typ_hxx
#define eray_typ_hxx
#include "scheme.hxx"
#include "logical.h"
/**
 * @file eray_typ.hxx
 * @CAA2Level L1
 * @CAA2Usage U1
 * \addtogroup SCMAPI
 * \Declared at <eray_typ.hxx>
 * @{
 */

class entity_with_ray;

//======================================================================

/**
* Determines if a Scheme object is an entray.
* <br><br>
* <b>Effect:</b> Read-only
* <br><br>
* @param r
* Scheme object.
**/

logical is_Scm_Entray(ScmObject r);

/*
// tbrv
*/
/**
 * @nodoc
 */

entity_with_ray get_Scm_Entray(ScmObject);

/*
// tbrv
*/
/**
 * @nodoc
 */

ScmObject make_Scm_Entray(const entity_with_ray&);

/*
// tbrv
*/
/**
 * @nodoc
 */

int get_Scm_Entray_List( ScmObject l, entity_with_ray*& eray_list );

/** @} */
#endif
