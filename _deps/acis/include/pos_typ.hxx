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
//    Define scheme SPAposition data type
//
//----------------------------------------------------------------------
#ifndef pos_typ_hxx
#define pos_typ_hxx
#include "scheme.hxx"
#include "position.hxx"
#include "vector.hxx"
#include "unitvec.hxx"
#include "transf.hxx"

class SPAposition_vector;
/**
 * @file pos_typ.hxx
 * @CAA2Level L1
 * @CAA2Usage U1
 * \addtogroup SCMAPI
 * \Declared at <pos_typ.hxx>
 * @{ 
 */
/**
 * @nodoc
 */
logical is_Scm_Position(ScmObject);
// tbrv
/**
 * @nodoc
 */
logical is_Scm_Position_List(ScmObject);
/**
* Creates a C++ <tt>SPAposition</tt> from a Scheme position object.
* <br><br>
* <b> Effect:</b> Read-only
* <br><br>
* @param sp
* Scheme object.
**/
SPAposition get_Scm_Position(ScmObject sp);
/**
 * @nodoc
 */
int get_Scm_Position_List(ScmObject l, SPAposition*& pts);
/**
 * @nodoc
 */
int get_Scm_Position_List(ScmObject l,SPAposition_vector &pts);
/**
 * @nodoc
 */
ScmObject make_Scm_Position(double x, double y, double z);
/**
 * @nodoc
 */
ScmObject make_Scm_Position(const SPAposition&);
/**
 * @nodoc
 */
ScmObject make_Scm_Position_List(int npts, const SPAposition *pts);
/**
 * @nodoc
 * Set the coordinates of a scheme SPAposition
 */
void set_Scm_Position(ScmObject, const SPAposition&);

/** @} */
#endif
