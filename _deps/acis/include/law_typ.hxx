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
//    declare procedures for working with class law in scheme
//
//----------------------------------------------------------------------

// paul-2.2.1

#ifndef law_typ_hxx
#define law_typ_hxx

#include "scheme.hxx"
#include "logical.h"

/**
 * @file law_typ.hxx
 * @CAA2Level L1
 * @CAA2Usage U1
 * \addtogroup SCMAPI
 * \Declared at <law_typ.hxx>
 * @{ 
 */

class law;
/**
 * Is scjeme object of type law
 */
logical is_Scm_Law(ScmObject);
/**
* Creates a C++ <tt>law</tt> from a Scheme <tt>law</tt> object.
* <br><br>
* <b>Limitations:</b> The law created with this function should be removed when no
* longer needed, to free the memory space.
* <br><br>
* <b>Effect:</b> Read-only
* <br><br>
* @param r
* Scheme object.
**/

law *get_Scm_Law(ScmObject r);
/**
 * Make ACIS law from scheme object
 */

ScmObject make_Scm_Law(law *eq);
/**
 * @nodoc
 */

int get_Scm_Law_List(ScmObject  list,law **&laws);
/**
 * @nodoc
 */

logical is_Scm_Law_List( ScmObject list );

/** @} */
#endif

