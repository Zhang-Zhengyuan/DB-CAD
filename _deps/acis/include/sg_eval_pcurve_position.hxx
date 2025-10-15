/*******************************************************************/
/*    Copyright (c) 1989-2020 by Spatial Corp.                     */
/*    All rights reserved.                                         */
/*    Protected by U.S. Patents 5,257,205; 5,351,196; 6,369,815;   */
/*                              5,982,378; 6,462,738; 6,941,251    */
/*    Protected by European Patents 0503642; 69220263.3            */
/*    Protected by Hong Kong Patent 1008101A                       */
/*******************************************************************/


#ifndef sg_eval_pcurve_position_hxx
#define sg_eval_pcurve_position_hxx

#include "add_pcu.hxx"
#include "pcurve.hxx"
class SPAposition;

/**
 * @nodoc
 */
DECL_KERN logical sg_eval_pcurve_position ( 
					PCURVE *pcurve, 
					double param, 
					SPApar_pos &uv 
				   );

#endif

