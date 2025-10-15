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

#ifndef SIL_SCM_OPTS_HFILE
#define SIL_SCM_OPTS_HFILE

#include "scheme.hxx"
#include "gc_obj.hxx"

class silhouette_options;

//======================================================================

class GC_Silhouette_Opt : public GC_Object
{
public:
	silhouette_options *opts;
	GC_Silhouette_Opt(
        ScmObject, 
        silhouette_options *);
	virtual ~GC_Silhouette_Opt();
};

// Determines if a Scheme object is an Silhouette_Opt
logical is_Scm_Silhouette_Opt(ScmObject);
// Creates a C++ silhouette_options object from a Scheme object
silhouette_options *get_Scm_Silhouette_Opt(ScmObject);
// Creates a scheme silhouette options object using the input data 
ScmObject make_Scm_Silhouette_Opt(silhouette_options *sil_opt);

//======================================================================
#endif // SIL_SCM_OPTS_HFILE
