/*******************************************************************/
/*    Copyright (c) 1989-2020 by Spatial Corp.                     */
/*    All rights reserved.                                         */
/*    Protected by U.S. Patents 5,257,205; 5,351,196; 6,369,815;   */
/*                              5,982,378; 6,462,738; 6,941,251    */
/*    Protected by European Patents 0503642; 69220263.3            */
/*    Protected by Hong Kong Patent 1008101A                       */
/*******************************************************************/

#if !defined (BHL_STITCH_ANALYZER)
#define BHL_STITCH_ANALYZER


//-------------------------- Include Files ------------------------------------*
//******************************************************************************
// ACIS includes
#include "logical.h"

#include "dcl_stitch.h"
#include "aggrstch_stitch.hxx"
#include "bhl_stru2.hxx"

//******************************************************************************

class ENTITY_LIST;

DECL_STITCH void
bhl_analyze_stitch( ENTITY_LIST&,
				   bhl_anal_stitch_results* anal_stitch_results,
				   bhl_stitch_options* stitch_opt);

DECL_STITCH void
bhl_stitch_heuristics(bhl_anal_stitch_results* anal_stitch_results,bhl_stitch_options* stitch_opt);

#endif // end of BHL_STITCH_ANALYZER
