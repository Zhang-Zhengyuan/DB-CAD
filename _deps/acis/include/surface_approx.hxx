// $Id: surface_approx.hxx,v 1.10 2002/08/09 17:15:32 jeff Exp $
/*******************************************************************/
/*    Copyright (c) 1989-2020 by Spatial Corp.                     */
/*    All rights reserved.                                         */
/*    Protected by U.S. Patents 5,257,205; 5,351,196; 6,369,815;   */
/*                              5,982,378; 6,462,738; 6,941,251    */
/*    Protected by European Patents 0503642; 69220263.3            */
/*    Protected by Hong Kong Patent 1008101A                       */
/*******************************************************************/

#ifndef SURFACE_APPROX_HXX
#define SURFACE_APPROX_HXX

#include "dcl_kern.h"
#include "option.hxx"
#include "bs3surf.hxx" // defines bs3_surf_form and bs3_surface // defines bs3_surf_form and bs3_surface
#include "sw_common.hxx"

class sw_surface;
class law;
class spl_sur;
class SPAinterval;
class SPAposition;
class SPAvector;
class spline;
class patch_breakpoint_list;
class SPApar_box;
class VOID_LIST;
class sf_clash_list;

DECL_KERN bs3_surface law_to_bs3_surface(
			law * in_law, 
			const SPAinterval &range_s, 
			const SPAinterval & range_t,
			double fit_tol = SPAresfit, 
			int initial_num_knots_u=0,
			int initial_num_knots_v=0,
			const double * initial_knots_u=NULL,
			const double * initial_knots_v=NULL,
			knot_removal_strategy knot_strategy=knot_removal_smart,
				// knot_strategy = knot_removal_smart (default), knot_removal_never, 
				//    or knot_removal_always (not recommended)
			logical approx_ok=FALSE
			);

DECL_KERN bs3_surface splsur_to_bs3_surface(
			const spl_sur & this_spl_sur, 
			SPAinterval            *uint=NULL,// uses the SPAinterval on the spl_sur, unless one is specified here.
			SPAinterval            *vint=NULL,
			double fit_tol = SPAresfit, 
			int initial_num_knots_u=0,
			int initial_num_knots_v=0,
			const double * initial_knots_u=NULL,
			const double * initial_knots_v=NULL,
			knot_removal_strategy knot_strategy=knot_removal_smart,
				// knot_strategy = knot_removal_smart (default), knot_removal_never, 
				//    or knot_removal_always (not recommended)
			logical approx_ok=FALSE //GSSL VPL Incremental approx project.
			);

DECL_KERN bs3_surface spline_to_bs3_surface(
			const spline & this_spline, 
			SPAinterval            *uint=NULL,// uses the SPAinterval on the spl_sur, unless one is specified here.
			SPAinterval            *vint=NULL,
			double fit_tol = SPAresfit, 
			int initial_num_knots_u=0,
			int initial_num_knots_v=0,
			const double * initial_knots_u=NULL,
			const double * initial_knots_v=NULL,
			knot_removal_strategy knot_strategy=knot_removal_smart
				// knot_strategy = knot_removal_smart (default), knot_removal_never, 
				//    or knot_removal_always (not recommended)
			);

// This is called by the routines above, but you may want to call it directly.
// knot_strategy = knot_removal_smart (default), knot_removal_never, 
//    or knot_removal_always (not recommended)
DECL_KERN bs3_surface patch_breakpoints_to_bs3_surface(patch_breakpoint_list &pbl, logical approx_ok=FALSE);


DECL_KERN bs3_surface sw_surface_to_bs3_surface(sw_surface * sws);
DECL_KERN sw_surface * bs3_surface_to_sw_surface(bs3_surface bs);

// Tests several points and returns the maximum distance between the original
// and its bs3_surface approximation.
DECL_KERN double bs_accuracy(const law * in_law,  
			bs3_surface bs, 
			const SPAinterval &u_int, 
			const SPAinterval &v_int);
DECL_KERN double bs_accuracy(const spline &sp,  
			bs3_surface bs, 			
			const SPAinterval *uint_temp = NULL,
			const SPAinterval *vint_temp = NULL);
DECL_KERN double bs_accuracy(const spl_sur &ss,  
			bs3_surface bs, 			
			const SPAinterval *uint_temp = NULL,
			const SPAinterval *vint_temp = NULL);
// Tests for self intersections on the given bs3_surface and returns various results.
// Returns TRUE if everything is OK.
DECL_KERN logical bs3_self_int_test(bs3_surface res, 
						  SPApar_box &exclude_region = *(SPApar_box*)NULL_REF,
						  SPApar_box &extension_box = *(SPApar_box*)NULL_REF,
						  VOID_LIST &bad_uvs = *(VOID_LIST*)NULL_REF,
						  sf_clash_list*& clashes = *(sf_clash_list**)NULL_REF );
#endif

