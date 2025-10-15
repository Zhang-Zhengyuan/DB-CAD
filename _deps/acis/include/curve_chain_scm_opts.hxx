/*******************************************************************/
/*    Copyright (c) 1989-2020 by Spatial Corp.                     */
/*    All rights reserved.                                         */
/*    Protected by U.S. Patents 5,257,205; 5,351,196; 6,369,815;   */
/*                              5,982,378; 6,462,738; 6,941,251    */
/*    Protected by European Patents 0503642; 69220263.3            */
/*    Protected by Hong Kong Patent 1008101A                       */
/*******************************************************************/
/*******************************************************************/
//  Modification history
//  DM  05Dec07   Created file
/*******************************************************************/
//----------------------------------------------------------------------
// purpose---
//    declare procedures for working with class 
//    silhouette_options in scheme
//
//----------------------------------------------------------------------

#ifndef CURVE_CHAIN_SCM_OPTS_HFILE
#define CURVE_CHAIN_SCM_OPTS_HFILE

#include "scheme.hxx"
#include "gc_obj.hxx"

class chain_curve_options;

//======================================================================

class GC_Curve_Chain_Opt : public GC_Object
{
public:
	chain_curve_options *opts;
	GC_Curve_Chain_Opt(ScmObject, chain_curve_options *);
	virtual ~GC_Curve_Chain_Opt();
};

// Determines if a Scheme object is an Curve_Chain_Opt
logical is_Scm_Curve_Chain_Opt(ScmObject);
// Creates a C++ chain_curve_options object from a Scheme object
chain_curve_options *get_Scm_Curve_Chain_Opt(ScmObject);
// Creates a scheme chain_curve_options object using the input data 
ScmObject make_Scm_Curve_Chain_Opt(chain_curve_options *curve_chain_opt);

//======================================================================
#endif // CURVE_CHAIN_SCM_OPTS_HFILE
