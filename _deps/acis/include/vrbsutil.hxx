// $Id: vrbsutil.hxx,v 1.13 2002/08/09 17:15:39 jeff Exp $
/*******************************************************************/
/*    Copyright (c) 1989-2020 by Spatial Corp.                     */
/*    All rights reserved.                                         */
/*    Protected by U.S. Patents 5,257,205; 5,351,196; 6,369,815;   */
/*                              5,982,378; 6,462,738; 6,941,251    */
/*    Protected by European Patents 0503642; 69220263.3            */
/*    Protected by Hong Kong Patent 1008101A                       */
/*******************************************************************/

// Miscellaneous routines that are needed for blending
// in both the kern and blnd modules.

//
// Modification---
//	28-Apr-99 bd	Move adjust_for_envelope() here.
//	01-Apr-99 bd	remove blend_slice_int_3_planes.
//	13-Nov-98 bd	Add blend_slice_int_3_planes, blend_slice_check_solution,
//		blend_slice_check_progress, blend_slice_fix_tangent,
//		and relax_svec_extended.  Also add some comments.
//

#ifndef VRBSUTIL_HXX
#define VRBSUTIL_HXX

#include "dcl_kern.h"
#include "position.hxx"
#include "vector.hxx"
#include "bl_cntct.hxx"

class ratBez_section;
class var_blend_spl_sur;
class ATTRIB_VAR_BLEND;

// Update the bs2_curves representing the approximate parameter space
// curves map of the spring curves on the supports.

DECL_KERN void 
calculate_support_pcurves(
            var_blend_spl_sur *var_spl,
            list_of_contacts const &contacts,
            int join_side = 0
            );

// Update the min / max values of the radii of the variable radius blend. 
// Calculates approximate extrema based on the slices found from marching
// which works out well ( for now ) for the purposes for which this data 
// is needed.

DECL_KERN void
calculate_radii_extrema(
			var_blend_spl_sur *var_spl,
			list_of_contacts const &contacts
			);

// Check whether a given a trial configuration (during an evaluation
// iteration) is a solution.

DECL_KERN logical
blend_slice_check_solution(
			v_bl_contacts *slice,
			double left_rad,
			double right_rad 
            );

// See how an evaluation iteration is progressing.

DECL_KERN logical
blend_slice_check_progress(
		    int i, 
            v_bl_contacts *slice,
		    SPAposition const &off_int_pt, 
            SPAposition const &crv_pt,
		    double *new_param 
            );

// Attempt to deal with a trial configuration in which
// the surface normals are parallel or anti-parallel.

DECL_KERN logical
blend_slice_fix_tangent( 
            v_bl_contacts *slice,
            SPAposition const &left_off_pt, 
            SPAposition const &right_off_pt,
            SPAposition *new_off_int_pt 
            );

// Relax an svec to a point, checking whether the svec's uv is set or not.
// It will relax outside the basic SPAparameter domain of the surface
// if necessary, without explicitly extending the surface.

DECL_KERN logical
relax_svec_extended( SVEC *S, SPAposition const &int_pt );

// Function to determine which blend support surface the holdline
// curve lies in.  The input is the var rad blend surface and a
// parameter on the holdline curve at which to test the distance
// from the two support surfaces.

DECL_KERN void
set_has_holdline_flag( var_blend_spl_sur* vbss, double param );

// Generate the data for a B-spline surface that interpolates
// the data in the given list of cross sections.

DECL_KERN void
cubic_bspline_srf(
		int				n_sections,				// input
		ratBez_section	*sections[],
		double			knots[],

		int			*n_spl_u_pts,				// output
		int			*n_spl_v_pts,
		SPAposition 	*spl_pt_array[],
		double	 	*spl_wt_array[],
		int			*n_spl_u_knots,
		int			*n_spl_v_knots,
		double	 	*spl_u_knots[],
		double	 	*spl_v_knots[]
	    );

// Generate the data for a B-spline curve that interpolates
// the data in the given list of points and tangents.

DECL_KERN void
cubic_bspline_crv(
		int		 npts,			// input
		SPAposition pts[],
		SPAvector	 tans[],
		double	 knots[],

		int		 *n_spl_pts,	// output
		SPAposition *spl_pts[],
		int		 *n_spl_knots,
		double	 *spl_knots[]
	    );

// Check for degenerate edges of a B-spline surface given its points.

DECL_KERN void 
set_poles( 
        SPAposition pts[], 
        int npts_u, 
        int npts_v, 
        double tol,
		int *pole_u, 
        int *pole_v 
        );

// Adjust a slice from the original offset-iteration configuration,
// which represents a rolling-ball snapshot surface, so that it
// represents the rolling-ball envelope surface.
//
// This process requires one derivative more than what the final result
// will have.  It is up to the caller to take care of this.
// If insufficient derivatives are present in the slice, those derivatives
// will be called zero.  This might actually happen at the highest
// level of derivatives.

DECL_KERN void
adjust_for_envelope( v_bl_contacts *slice, int n_derivs );

enum DECL_KERN bl_march_status 
{
	BL_MARCH_SUCCESS,
	BL_MARCH_PARTIAL_SUCCESS,
	BL_MARCH_FAILURE
};

DECL_KERN bl_march_status 
march(
        var_blend_spl_sur *blnd_srf, 
        SPAinterval const &range,
        logical fwd,
        double tol,                  
        list_of_contacts &slice_list,
        logical restoring_data = FALSE,
        ATTRIB_VAR_BLEND *this_att = NULL
        );

#endif



