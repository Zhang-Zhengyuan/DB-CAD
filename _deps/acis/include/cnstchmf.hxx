/*******************************************************************/
/*    Copyright (c) 1989-2020 by Spatial Corp.                     */
/*    All rights reserved.                                         */
/*    Protected by U.S. Patents 5,257,205; 5,351,196; 6,369,815;   */
/*                              5,982,378; 6,462,738; 6,941,251    */
/*    Protected by European Patents 0503642; 69220263.3            */
/*    Protected by Hong Kong Patent 1008101A                       */
/*******************************************************************/

#if !defined( CNSTCHMF_HXX )
#define CNSTCHMF_HXX

#include "dcl_abl.h" 
#include "logical.h"

class ENTITY_LIST;
class outcome;

// Set advanced chamfering attributes. The chamfer ranges are interpreted 
// as offset values for the supports.

DECL_ABL outcome
abh_set_adv_chamfers(
			ENTITY_LIST const &edges, // edges to apply const chamfer on
			double r1,				  // left range
			double r2,				  // right range (if negative, assumed equal to r1)
			double r1_end,			  // left  range at the end ( if <0 => =r1 )
			double r2_end,			  // right range at the end ( if <0 => =r2 )
			double R1,				  // left round chamfer radius
			double R2				  // right round chamfer radius
			);

// Set fixed distance chamfers. The chamfer ranges are interpreted
// as spring distance values from the calibration curve.

DECL_ABL outcome 
abh_set_const_chamfers(
			ENTITY_LIST const & entities,           // edges to be chamfered
			double left_range,				        // left range
			double right_range,				        // right range
			double start_setback = 0,			    // start setback
			double end_setback = 0,			        // end setback
			double start_setback_diff = 0,			// setback difference (r - l) at start
			double end_setback_diff = 0,			// setback difference (r - l) at end
			logical start_setback_diff_set = TRUE,	// setback difference set at start
			logical end_setback_diff_set = TRUE,	// setback difference set at end
			double start_stop_ang = 0,			    // start stop angle
			double end_stop_ang = 0			        // end stop angle
			);

// Set advanced chamfering attributes and fix the blends. The chamfer 
// ranges are interpreted as offset values for the supports.

DECL_ABL outcome 
abh_chamfer_edges (
			ENTITY_LIST const &edges, // edges to apply const chamfer on
			double r1,				  // left range
			double r2 = -1.,		  // right range (if negative, assumed equal r1)
			double r1_end = -1.,	  // left  range at the end ( <0 => =r1 )
			double r2_end = -1.,	  // right range at the end ( <0 => =r2 )
			double R1 = 0.,			  // left round chamfer radius (if =0 => flat chamfer)
			double R2 = 0.			  // right round chamfer radius (if =0 => flat chamfer)
			);

#endif
