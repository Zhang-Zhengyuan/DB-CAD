/*******************************************************************/
/*    Copyright (c) 1989-2020 by Spatial Corp.                     */
/*    All rights reserved.                                         */
/*    Protected by U.S. Patents 5,257,205; 5,351,196; 6,369,815;   */
/*                              5,982,378; 6,462,738; 6,941,251    */
/*    Protected by European Patents 0503642; 69220263.3            */
/*    Protected by Hong Kong Patent 1008101A                       */
/*******************************************************************/

#ifndef _BHL_ANACOMN_
#define _BHL_ANACOMN_

#include "box.hxx"

#include "bhl_stru2.hxx"
#include "analyzer2.hxx"
#include "bhl_stru3.hxx"
#include "all_chk.hxx"


enum VIEW_TYPE{
	RESET,
	TOP,
	BOTTOM,
	FRONTV,
	BACKV,
	LEFT,
	RIGHT,
	ISO,
	PAR,
	PER,
	SCALE_UP,
	SCALE_DOWN
};


DECL_STITCH void
bhl_get_body_types_and_box( ENTITY_LIST& , SPAbox&,bhl_anal_body_stats_results * = NULL );


#endif
