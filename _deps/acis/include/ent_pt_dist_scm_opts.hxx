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
// AJR  01Jul09   Replace logical with enum for diatance mode to allow
//                for unsigned point entity distance calculations.
//  DM  10Jul07   Remove references to help data. Allow signed distances for point
//                entity distance calculations
//  DM  21May07   Created file
/*******************************************************************/
//----------------------------------------------------------------------
// purpose---
//    declare procedures for working with class 
//    SPAentity_point_distance_options in scheme
//
//----------------------------------------------------------------------

#ifndef ENT_PT_DIST_SCM_OPTS_HFILE
#define ENT_PT_DIST_SCM_OPTS_HFILE

#include "scheme.hxx"
#include "gc_obj.hxx"
#include "ent_pt_dist_opts.hxx"

class SPAentity_point_distance_options;

//======================================================================

class GC_Entity_Point_Distance_Opt : public GC_Object
{
public:
	SPAentity_point_distance_options *opts;
    SPAepd_distance_mode dist_mode;
	GC_Entity_Point_Distance_Opt(
        ScmObject, 
        SPAentity_point_distance_options *, 
        SPAepd_distance_mode);
	virtual ~GC_Entity_Point_Distance_Opt();
};

// Determines if a Scheme object is an Entity_Point_Distance_Opt
logical is_Scm_Entity_Point_Distance_Opt(ScmObject);
// Creates a C++ SPAentity_point_distance_options object from a Scheme object
SPAentity_point_distance_options *get_Scm_Entity_Point_Distance_Opt(ScmObject);
// Creates a scheme scheme entity point distance options object using the input data 
ScmObject make_Scm_Entity_Point_Distance_Opt(SPAentity_point_distance_options *epd_opt, 
                                             SPAepd_distance_mode dist_mode);

//======================================================================
#endif // ENT_PT_DIST_SCM_OPTS_HFILE
