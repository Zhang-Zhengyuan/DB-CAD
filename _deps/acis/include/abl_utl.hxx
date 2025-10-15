/* ORIGINAL: abl2.1/scmext/scmext/abl_utl.hxx */
/*******************************************************************/
/*    Copyright (c) 1989-2020 by Spatial Corp.                     */
/*    All rights reserved.                                         */
/*    Protected by U.S. Patents 5,257,205; 5,351,196; 6,369,815;   */
/*                              5,982,378; 6,462,738; 6,941,251    */
/*    Protected by European Patents 0503642; 69220263.3            */
/*    Protected by Hong Kong Patent 1008101A                       */
/*******************************************************************/
#include "acis.hxx"

#include "logical.h"

// ACIS includes

#include "api.hxx"

#include "lists.hxx"
#include "transfrm.hxx"
#include "intcurve.hxx"
#include "alltop.hxx"

#include "var_rad.hxx"
#include "v_bl_spl.hxx"
#include "v_bl_att.hxx"
#include "blnd_sec.hxx"

#include "ablapi.hxx"

logical smooth_edge (EDGE *ed);

outcome
set_edge_blends(
	var_radius *rad_fcn1,  
	var_radius *rad_fcn2,
	EDGE* edge,
	EDGE* edge0,
	EDGE* edge1,
	logical single,
	logical is_tangent0,
	logical is_tangent1,
	double st_setback,
	double end_setback,
	logical rnd_ch_case,
	double round0,
	double round1,
	logical section_given,
	double left_thumbweight,
	double right_thumbweight,
	EDGE* loft_edge,
	double start_sbdiff,
	double end_sbdiff,
	double start_stop_ang,
	double end_stop_ang,
	AcisOptions *ao,
	ENTITY_LIST &ents	  // gets set by this routine.
	);









