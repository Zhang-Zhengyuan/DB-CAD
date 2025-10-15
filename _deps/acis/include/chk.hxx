/* ORIGINAL: acis2.1/kernint/d3_chk/chk.hxx */
/* $Id: chk.hxx,v 1.22 2002/08/09 17:18:50 jeff Exp $ */
/*******************************************************************/
/*    Copyright (c) 1989-2020 by Spatial Corp.                     */
/*    All rights reserved.                                         */
/*    Protected by U.S. Patents 5,257,205; 5,351,196; 6,369,815;   */
/*                              5,982,378; 6,462,738; 6,941,251    */
/*    Protected by European Patents 0503642; 69220263.3            */
/*    Protected by Hong Kong Patent 1008101A                       */
/*******************************************************************/

#ifndef CHKH
#define CHKH

#include "dcl_intr.h"
// ywoo 08Jan01: added the header file.
#include "base.hxx"
// ywoo: end
#include "chk_stat.hxx"

////////////////////////////////////////////////////////////////////////////////
//									      //
//			Curve and Surface Legality Checker		      //
//									      //
////////////////////////////////////////////////////////////////////////////////


// Both curve and surface checkers return similar lists of return codes when the
// curve or surface is illegal, or NULL if they are ok. Note that in both cases
// the subdivision and self-intersection tests are only performed if everything
// else worked.

// Return codes are described in chk_stat.hxx:

// The function to invoke the surface legality checker, returning NULL if the
// surface if completely ok. It is up to the caller to delete the returned list
// (if there is one).

class surface;
DECL_INTR check_status_list* 
    d3_sf_check( const surface&, 
		 const check_fix& input = *(const check_fix*) NULL_REF, 
		         // supplies a set of flags which say which fixes 
		         // are allowable (the default is to fix nothing)
		 check_fix& result = *(check_fix*) NULL_REF, 		
		         // returns a set of flags which say which fixes 
		         // were applied
		 const check_status_list* = (const check_status_list*) NULL_REF 
		         // list of checks that are to be made.  If the 
		         // list is null, then every possible check will 
		         // be made; otherwise, the function will only 
		         // check for things in the list.
		         // Note that it is possible for errors not in the
		         // input list to be returned, as certain problems
		         // preclude other checks being performed. In such
		         // a situation, the return list will contain errors
		         // which have prevented reqested checks from being made
		 );


// The corresponding function for curves. 

class curve;
DECL_INTR check_status_list* 
    d3_cu_check( const curve&,
		 const check_fix& input = *(const check_fix*) NULL_REF, 
		         // supplies a set of flags which say which fixes 
		         // are allowable (the default is to fix nothing)
		 check_fix& result = *(check_fix*) NULL_REF, 		
		         // returns a set of flags which say which fixes 
		         // were applied
		 const check_status_list* = (const check_status_list*) NULL_REF,
		         // list of checks that are to be made.  If the 
		         // list is null, then every possible check will 
		         // be made; otherwise, the function will only 
		         // check for things in the list.  The return 
		         // value for the function will then be a subset
		         // of this list. 
		 double tol = SPAresabs
		         // only used in self-intersection check: look
			 // for portions of the curve that
			 // self-intersect within this tolerance value.
		 );


// The pcurve checker function. 

class pcurve;
class COEDGE;
DECL_INTR check_status_list* d3_pcu_check( pcurve &pcu, COEDGE *coed );

// STI swa 14Aug98 -- Added curve and surface approximation checks, moved from the testbed directory
#include "logical.h"
#include "bs3surf.hxx"

class SPApar_pos;
class SPApar_box;

extern DECL_INTR int do_approx_test( surface& sf, bs3_surface bs3, double fitol, logical close,
                                     double& ret_max_error = *(double *)NULL_REF, 
                                     SPApar_pos& ret_uv_max_err = *(SPApar_pos *)NULL_REF );
extern DECL_INTR logical out_of_vb_polygon( const surface& sf,
									const SPApar_pos& uv );
extern DECL_INTR logical supported_by_pipe( const surface& sf );
extern DECL_INTR SPApar_box get_box( const surface& sf );

extern DECL_INTR logical surface_param_test( const surface& sf );
extern DECL_INTR logical surface_transform_test( const surface& sf );
extern DECL_INTR logical surface_point_perp_test( const surface& sf );
extern DECL_INTR logical surface_extend_test( const surface& sf );
extern DECL_INTR logical surface_derivs_test( const surface& sf );
// STI rr 11/02/02 begin: export this function so that it can be used in the kernel via callback.
// This function is needed to tell if the B-spline made by bs3_make_approx is self--intersecting or not.
class sf_clash_list;
DECL_INTR check_status_list* self_intersects( check_status_list* list,
											 BOUNDED_SURFACE& bsf,
											 sf_clash_list* clashes_in = NULL, 
											 SPApar_box &exclude_region = *(SPApar_box*)NULL_REF,
											 logical points_on_surface = FALSE );
// STI rr end

#endif
