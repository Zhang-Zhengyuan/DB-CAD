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
//    Define scheme transform data type
//
//----------------------------------------------------------------------
#ifndef tran_typ_hxx
#define tran_typ_hxx
#include "scheme.hxx"
/**
 * @file tran_typ.hxx
 * @CAA2Level L1
 * @CAA2Usage U1
 * \addtogroup SCMAPI
 * \Declared at <tran_typ.hxx>
 * @{
 */
class SPAtransf;
/**
 * @nodoc
 */
logical is_Scm_Transform(ScmObject);
/**
* Creates a C++ <tt>SPAtransf</tt> from a Scheme transform object.
* <br><br>
* <b>Effect:</b> Read-only
* <br><br>
* @param t
* Scheme object.
**/
SPAtransf get_Scm_Transform(ScmObject t);
/**
 * @nodoc
 */
ScmObject make_Scm_Transform(const SPAtransf&);
/**
 * @nodoc
 */
logical is_Scm_Transform_List(ScmObject);
/**
 * @nodoc
 */
int get_Scm_Transform_List(ScmObject l, SPAtransf*& pts);
/**
 * @nodoc
 */
ScmObject make_Scm_Transform_List(int npts, const SPAtransf* pts);
/** @} */

#endif
