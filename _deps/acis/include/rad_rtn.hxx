/* ORIGINAL: blnd/blend/sg_husk/vrbln/rad_rtn.hxx */
/*******************************************************************/
/*    Copyright (c) 1989-2020 by Spatial Corp.                     */
/*    All rights reserved.                                         */
/*    Protected by U.S. Patents 5,257,205; 5,351,196; 6,369,815;   */
/*                              5,982,378; 6,462,738; 6,941,251    */
/*    Protected by European Patents 0503642; 69220263.3            */
/*    Protected by Hong Kong Patent 1008101A                       */
/*******************************************************************/
/* ORIGINAL: abl2.1/abl_husk/msc/ablmsrtn.hxx */

// old:
/* $Id: rad_rtn.hxx,v 1.11 2002/08/09 17:28:31 jeff Exp $ */

// Declarations of routines that create radius functions.


//
// Modification---
//	12-Feb-03	bwc	Added parameters to rad_fcn_from_positions so that known
//		params can be passed in.
//  02-feb-02 tgg   Added the handle_closed_like_periodic SPAparameter to handle variable radius miters.
//	25-May-99 bd	Comments (only) about transforms in rad_fcn_from_positions()
//	19-Aug-98 bd	Moved into blnd module from ABL husk and renamed.
//  5 Nov  97 nvp	rad_fcn_from_bs3_curve also takes logical flag
// 	24 Sep 97 nvp	rad_fcn_from_bs3_curve now takes calibration curve
//	11 Mar 96 nvp	rad_fcn_from_positions now takes a calibration curve
//	08-Mar-96 bd	Add acis.hxx include.
//

#ifndef RAD_RTN_H
#define RAD_RTN_H

class ENTITY_LIST;
class SPAposition;
class CURVE;
class SPAinterval;

#include "dcl_blnd.h"
#include "acis.hxx"

#include "bs2curve.hxx"
#include "bs3curve.hxx"
#include "curdef.hxx"



// Create a radius function (bs2_curve) given a list of
// < SPAposition, radius value > pairs, and the edges to which the positions refer.
//
// Note: the calibration curve and the positions must be represented in the
// same coordinate system.  For blending, the coordinate system should be
// that of the body being blended, not model space.  If the calibration
// curve is created by api_smooth_edges_to_curve(), then it will be in the
// coordinate system of the body (i.e., that of the edges passed to it.)
// Therefore, if the positions are given in model space, then they have to
// be transformed by the inverse of the blended body's transform.
//

// STIPORT aw changed SPAposition positions[] to SPAposition *positions for ultrix,osf1

DECL_BLND bs2_curve rad_fcn_from_positions( CURVE * calibration_curve,
                                    int n_fixes,
                                    SPAposition* positions,
                                    double    rad_values[],
                                    double  * start_slope = NULL,
                                    double  * end_slope   = NULL,
                                    SPAinterval* edge_range  = NULL,
                                    int     * new_n_fixes = NULL,
                                    double  * fix_params  = NULL,
                                    double  * sort_rads   = NULL,
									logical handle_closed_like_periodic = FALSE,
									int num_given_params = 0,
									double * given_params = NULL
                                );
// STI/Autodesk rsb (4/98)
// Overloaded function to allow specification of slopes at ends.
DECL_BLND bs2_curve rad_fcn_from_params ( int n_fixes,
									double params[],
									double rad_values[],
									double * start_slope,
									double * end_slope
								);

// STI bas 6-Apr-2001
// Overloaded function for the case when slopes are known (e.g., radious function
// is constructed from piece-wise analytical functions).
DECL_BLND bs2_curve rad_fcn_from_params ( int n_fixes,
									double params    [],
									double rad_values[],
									double slopes    []
								);

DECL_BLND bs2_curve rad_fcn_from_params ( int n_fixes,
									double params[],
									double rad_values[]
								);


// Create a radius function (bs2_curve) from the y-coordiantes of a bs3_curve.

DECL_BLND bs2_curve rad_fcn_from_bs3_curve ( bs3_curve bs3, CURVE * calibration_curve = NULL,
						logical edge_periodic = FALSE );


// Returns whether the input curve is periodic
// within its SPAparameter domain (for instance, a segment
// from an ellipse is not periodic).  Returns true if so,
// false if not.

DECL_BLND logical is_parameter_periodic( curve const& crv , SPAinterval* edge_range , 
						logical chk_finite );


#endif
