/*******************************************************************/
/*    Copyright (c) 1989-2020 by Spatial Corp.                     */
/*    All rights reserved.                                         */
/*    Protected by U.S. Patents 5,257,205; 5,351,196; 6,369,815;   */
/*                              5,982,378; 6,462,738; 6,941,251    */
/*    Protected by European Patents 0503642; 69220263.3            */
/*    Protected by Hong Kong Patent 1008101A                       */
/*******************************************************************/

#if !defined (BHL_STRUCTS2_HXX)
#define BHL_STRUCTS2_HXX

#include "dcl_stitch.h"
#include "bhl_stru3.hxx"
#include "hh_stru2.hxx"

enum HealStates
{
	INVALID_STATE = 0,

	PREPROCESS = 1,

	SIMPLIFY_ANALYZE = 21, SIMPLIFY_CALCULATE = 22, SIMPLIFY_FIX = 23,

	STITCH_ANALYZE = 31, STITCH_CALCULATE = 32, STITCH_FIX = 33,

	GEOMBUILD_ANALYZE = 41, GEOMBUILD_CALCULATE = 42, GEOMBUILD_FIX = 43,

	ANALYTIC_ANALYZE = 411, ANALYTIC_CALCULATE = 412, /*ANALYTIC_FIX = 413,*/
	ISOSPLINE_ANALYZE = 421, ISOSPLINE_CALCULATE = 422, /*ISOSPLINE_FIX = 423,*/
	REBLEND_ANALYZE = 461, REBLEND_CALCULATE = 462, /*REBLEND_FIX = 463,*/
	SHARP_EDGE_ANALYZE = 431, SHARP_EDGE_CALCULATE = 432, /*SHARP_EDGE_FIX = 433,*/
	GEN_SPLINE_ANALYZE = 441, GEN_SPLINE_CALCULATE = 442, /*GEN_SPLINE_FIX = 443,*/
	WRAPUP_ANALYZE = 451, WRAPUP_CALCULATE = 452, /*WRAPUP_FIX = 453,*/

	POSTPROCESS = 5
};

#define BHL_MAX_STR_SIZE 4096

// Structure and enumeration for Autoheal results

enum AUTOHEAL_RESULT
{
	FULLY_HEALED,		// Denotes completely healed successfully
	PARTIAL_HEALED, 	// Denotes partially healed
	ONLY_STITCHED,		// Denotes that the body is stitched but not healed
	FAILED				// Denotes failed
};


//  PVA Added : 7/2/98 Start
struct DECL_STITCH bhl_file_details
{
	long ip_file_size;
	long op_file_size;

	bhl_file_details()
	{
		ip_file_size = 0;
		op_file_size = 0;
	}
};


#endif
