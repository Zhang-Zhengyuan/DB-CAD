/*******************************************************************/
/*    Copyright (c) 1989-2020 by Spatial Corp.                     */
/*    All rights reserved.                                         */
/*    Protected by U.S. Patents 5,257,205; 5,351,196; 6,369,815;   */
/*                              5,982,378; 6,462,738; 6,941,251    */
/*    Protected by European Patents 0503642; 69220263.3            */
/*    Protected by Hong Kong Patent 1008101A                       */
/*******************************************************************/

#ifndef _STITCH_HXX
#define _STITCH_HXX


// File Includes
#include "logical.h"
#include "version.hxx"

#include "dcl_stitch.h"
#include "bhl_opt2.hxx"
#include "tols3.hxx"
#include "stchapi.hxx"

// Forward declarations
class ENTITY_LIST;
class BODY;
class COEDGE;


// Functions for stitching
logical
bhl_stitch_list (ENTITY_LIST&,bhl_stitch_options *  ,bhl_stitch_results*, tolerant_stitch_options* );

logical
bhl_stitch_list_wrap (ENTITY_LIST&,bhl_stitch_options *  ,bhl_stitch_results* );

double
get_next_tol(bhl_stitch_results *,bhl_stitch_options *);

logical
bhl_stitch_list_at_tol(ENTITY_LIST &, double,bhl_stitch_options * ,bhl_stitch_results*, tolerant_stitch_options* );

logical
bhl_stitch_2_bodies ( BODY*, BODY*, double,bhl_stitch_options * ,bhl_stitch_results *m_stitch_results, tolerant_stitch_options*);

logical
bhl_stitch_1_entity (ENTITY* , double,bhl_stitch_results * =NULL,bhl_stitch_options * = NULL, 
					 tolerant_stitch_options* = NULL);


// GSSL/RS - (Jun 28,2002) : Single face stitching
// Added a new argument to return the stitched edge
// GSSL/RS-FSS
// GSSL/RS (Jun 12,2003) : Failsafe stitching project
// Made the argument bhl_stitch_options * to const


logical
bhl_make_partners(COEDGE*, COEDGE*,double ,logical direct, const bhl_stitch_options *, 
				  EDGE *&stitched_edge, logical=TRUE, ENTITY_LIST * = NULL, ENTITY_LIST * = NULL, 
				  tolerant_stitch_options* = NULL, logical& = *(logical *)NULL_REF,
				  logical eliminate_fix_stage=TRUE);


// GSSL/RS-FSS
// GSSL/RS (Jun 12,2003) : Failsafe stitching project
// New logical argument for skipping topology check
DECL_STITCH void
hh_analyze_stitch(BODY* inp_body, tolerant_stitch_options* tol_sopts,
				  logical skip_topo_check);

DECL_STITCH void
hh_calculate_stitch(BODY* inp_body);

DECL_STITCH void
hh_fix_stitch(BODY* inp_body);

DECL_STITCH void
hh_cleanup_stitch(BODY* inp_body);

void
hh_calculate_stitch_at_tol(BODY* body);

DECL_STITCH logical
hh_stitch(ENTITY_LIST &, bhl_stitch_results* = NULL,
			   bhl_anal_stitch_results* anal_stitch_results = NULL,
			   bhl_stitch_options* =NULL,
			   tolerant_stitch_options* = NULL);

logical
hh_stitch_incremental(ENTITY_LIST &, bhl_stitch_results*  ,bhl_stitch_options *, tolerant_stitch_options* =NULL);

logical
hh_stitch_stepped(ENTITY_LIST&, bhl_stitch_results* ,bhl_stitch_options * );

DECL_STITCH logical
hh_validate_stitching(ENTITY_LIST&, ENTITY_LIST&, ENTITY_LIST&, int&);

// GSSL/RS - (Jun 27,2002): Single face stitching
// Added a default argument to tolerize only stitched/modified edges
DECL_STITCH void
hh_tolerant_stitch(BODY*, logical only_stitched_edges = FALSE);

// RS - start (Dec 5,2001) : Function for automatic stitching
// GSSL/RS-FSS
// GSSL/RS (Jun 12,2003) : Failsafe stitching project
// New logical argument for skipping topology check
DECL_STITCH void
hh_auto_stitch(BODY *inp_body, tolerant_stitch_options* tol_sopts,
			   logical skip_topo_check);
// RS - end

// GSSL/RS-FSS
// GSSL/RS (Jun 12,2003) : Failsafe stitching project
// Adding a new argument skip_topo_check
// Adding a new argument proceeded_after_errors
DECL_STITCH void 
hh_auto_stitch_for_progress(BODY *inp_body, tolerant_stitch_options* tol_sopts,
							logical skip_topo_check, logical &proceeded_after_errors);

#endif // _STITCH_HXX

