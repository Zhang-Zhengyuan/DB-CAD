/*******************************************************************/
/*    Copyright (c) 1989-2020 by Spatial Corp.                     */
/*    All rights reserved.                                         */
/*    Protected by U.S. Patents 5,257,205; 5,351,196; 6,369,815;   */
/*                              5,982,378; 6,462,738; 6,941,251    */
/*    Protected by European Patents 0503642; 69220263.3            */
/*    Protected by Hong Kong Patent 1008101A                       */
/*******************************************************************/

#if !defined (BHL_NET_HEAL)
#define BHL_NET_HEAL

#include "api.hxx"
#include "logical.h"
#include "bhl_stru.hxx"
#include "dcl_heal.h"

class FACE;
class ENTITY_LIST;
/*
DECL_HEAL extern int bhl_tgt_spl_unsolvable;
DECL_HEAL extern int bhl_tgt_spl_no_nets_4sided;
DECL_HEAL extern int bhl_tgt_spl_no_nets_3sided;
DECL_HEAL extern int bhl_tgt_spl_no_bad_nets;
*/
// Function to analyze nets in body

DECL_HEAL void bhl_analyze_nets_in_body( ENTITY* );

// Function to calculate nets in body

DECL_HEAL void bhl_calculate_nets_in_body( ENTITY*, bhl_advanced_spline_solver_results* res = NULL);


#endif

// ---------------------------------------------------------------------------
// ---------------------------------------------------------------------------
