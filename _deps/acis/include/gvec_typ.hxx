/*******************************************************************/
/*    Copyright (c) 1989-2020 by Spatial Corp.                     */
/*    All rights reserved.                                         */
/*    Protected by U.S. Patents 5,257,205; 5,351,196; 6,369,815;   */
/*                              5,982,378; 6,462,738; 6,941,251    */
/*    Protected by European Patents 0503642; 69220263.3            */
/*    Protected by Hong Kong Patent 1008101A                       */
/*******************************************************************/
//    define scheme gvector data type

#ifndef gvec_typ_hxx
#define gvec_typ_hxx
#include "scheme.hxx"
#include "position.hxx"
#include "vector.hxx"
#include "unitvec.hxx"
#include "transf.hxx"
/**
 * @file gvec_typ.hxx
 * @CAA2Level L1
 * @CAA2Usage U1
 * \addtogroup SCMAPI
 * \Declared at <gvec_typ.hxx>
 * @{ 
 */
/**
 * Is Scheme object a gvector
 */
logical is_Scm_Gvector(ScmObject);

/**
* Creates a C++ <tt>SPAvector</tt> from a Scheme <tt>gvector</tt> object.
* <br><br>
* <b>Effect:</b> Read-only
* <br><br>
* @param sp
* Scheme object.
**/
SPAvector get_Scm_Gvector(ScmObject sp);
/**
 * Make a scheme gvector from three doubles
 */
ScmObject make_Scm_Gvector(double x, double y, double z);
/**
 * Make a scheme gvector from SPAvector
 */
ScmObject make_Scm_Gvector(const SPAvector&);
/**
 * Is scheme object a list of gvectors
 */
logical is_Scm_Gvector_List(ScmObject list);
/**
 * Make a list of scheme gvectors from array of SPAvector
 */
ScmObject make_Scm_Gvector_List(int nvecs, const SPAvector* vecs);
/**
 * Create array of SPAvector from scheme object gvector list
 */
int get_Scm_Gvector_List(ScmObject list, SPAvector*& vecs);
/**
 * Change the coordintates of a scheme gvector using the coordintates of input SPAvector
 */
void set_Scm_Gvector(ScmObject, const SPAvector&);
/**
 * @nodoc
 */
int get_Scm_Unit_Gvector_List(ScmObject list, SPAunit_vector *&vecs);

/** @} */
#endif


