/*******************************************************************/
/*    Copyright (c) 1989-2020 by Spatial Corp.                     */
/*    All rights reserved.                                         */
/*    Protected by U.S. Patents 5,257,205; 5,351,196; 6,369,815;   */
/*                              5,982,378; 6,462,738; 6,941,251    */
/*    Protected by European Patents 0503642; 69220263.3            */
/*    Protected by Hong Kong Patent 1008101A                       */
/*******************************************************************/

#ifndef HEAL_STITCH_STITCH_HXX
#define HEAL_STITCH_STITCH_HXX

#include "stitch_stitch.hxx"
#include "dcl_heal.h"

/*
DECL_HEAL void
hh_analyze_stitch(BODY* inp_body);

DECL_HEAL void
hh_calculate_stitch(BODY* inp_body);

DECL_HEAL void
hh_fix_stitch(BODY* inp_body);

DECL_HEAL void
hh_cleanup_stitch(BODY* inp_body);

DECL_HEAL logical
hh_stitch(ENTITY_LIST &, bhl_join_results* = NULL,
			   bhl_anal_join_results* anal_stitch_results = NULL,
			   bhl_join_options* =NULL );

DECL_HEAL void
hh_tolerant_stitch(BODY*);

// RS - start (Dec 24,2001) : Function for automatic stitching
DECL_HEAL void
hh_auto_stitch(BODY *inp_body);
// RS - end
*/

#endif // _STITCH_HXX

