/*******************************************************************/
/*    Copyright (c) 1989-2020 by Spatial Corp.                     */
/*    All rights reserved.                                         */
/*    Protected by U.S. Patents 5,257,205; 5,351,196; 6,369,815;   */
/*                              5,982,378; 6,462,738; 6,941,251    */
/*    Protected by European Patents 0503642; 69220263.3            */
/*    Protected by Hong Kong Patent 1008101A                       */
/*******************************************************************/
//    Declare curve query procedures
/*******************************************************************/
#ifndef curveq_hxx
#define curveq_hxx
#include "dcl_kern.h"
#include "base.hxx"
/**
 * @file curveq.hxx
 * @CAA2Level L1
 * @CAA2Usage U1
 * \addtogroup ACISQUERIES
 *
 * @{
 */
class ENTITY;
class SPAposition;
class SPAvector;
class SPAunit_vector;
class pick_ray;
class bounded_curve;
class EDGE;
class curve;
class ENTITY_LIST;
/**
 * @nodoc
 * Get the ends of a curve.  Returns FALSE if the EDGE is not a curve
 */
DECL_KERN logical get_curve_ends(
    EDGE* crv,     // the curve
    SPAposition& pt1, // start SPAposition
    SPAposition& pt2  // end SPAposition
    );
/**
 * Determines if a curve is closed.
 * <br><br>
 * <b>Effect:</b> Read-only
 * <br><br>
 * @param crv
 * curve.
 **/
DECL_KERN logical is_closed_curve(EDGE* crv);
/**
 * @nodoc
 */
DECL_KERN double curve_length(EDGE* crv);
/**
 * @nodoc
 * Get the radius of a circular EDGE
 * returns FALSE if the curve is not circular or elliptical
 */
DECL_KERN logical get_curve_radius(EDGE* arc, double& radius);
/**
 * Gets the center of a circular or elliptical <tt>EDGE</tt>.
 * <br><br>
 * <b>Role:</b> Returns <tt>FALSE</tt> if the edge was not circular or elliptical.
 * <br><br>
 * <b>Effect:</b> Read-only
 * <br><br>
 * @param crv
 * curve.
 * @param center
 * returns center.
 **/
DECL_KERN logical get_curve_center(EDGE* crv, SPAposition& center);
/**
 * @nodoc
 * Get the major axis from a circular or elliptical EDGE
 */
DECL_KERN logical get_curve_major_axis(EDGE* crv, SPAvector& major_axis);
/**
 * @nodoc
 * Get the parameter range for an EDGE
 */
DECL_KERN logical get_curve_parameter_range(EDGE* crv, double& start, double& end);
/**
 * @nodoc
 * Get the normal from a planar curve. Returns FALSE if the curve is not planar
 */
DECL_KERN logical get_curve_normal(EDGE* crv, SPAunit_vector& normal);
/**
 * @nodoc
 * Get a plane normal to an array of curves
 */
DECL_KERN logical get_plane_from_curves(
    int             n_ents,
    bounded_curve*  clist[],
    SPAposition&       plane_pt,
    SPAunit_vector&    plane_normal);
/**
 * @nodoc
 * Get a plane normal to an array of curves
 */

DECL_KERN logical get_plane_from_non_planar_curves(
    int             n_ents,
    bounded_curve*  clist[],
    SPAposition&       plane_pt,
    SPAunit_vector&    plane_normal,
	logical give_centroid = FALSE // To specify whether the plane_pt should be the centroid of the clist
	);
/**
 * @nodoc
 * Get all parameters for an elliptical edge
 */
DECL_KERN logical get_ellipse_parameters(
    EDGE* ell,            // the ellipse
    SPAposition& center,     // center
    SPAunit_vector& normal,  // normal to plane of ellipse
    SPAvector& major_axis,   // major axis (length = major radius)
    double& radius_ratio, // ratio of major radius to minor radius
    double& start_angle,  // start angle in degrees
    double& end_angle     // end angle in degrees
    );
/**
 * @nodoc
 * Determine if a bounded curve is linear
 */

DECL_KERN logical is_curve_linear(
    bounded_curve*          crv,
    SPAposition&               centroid,
    SPAunit_vector&            direction,
	logical&				degenerate_point);
/**
 * @nodoc
 * Get plane from a list of subsetted curves
 */
DECL_KERN logical get_plane_from_curve_list(
	int					n_ents,
	curve*				clist[],
	logical				same_plane,
	SPAposition&           centroid,
    SPAunit_vector&        normal,
	double&				deviation,
	double&	= *(double *)NULL_REF
	);
/**
 * @nodoc
 * Get plane from an entity
 */
DECL_KERN logical get_plane_from_entity(
	ENTITY		*ent,
	logical		same_plane,
	logical		apply_transf,
	SPAposition	&centroid,
	SPAunit_vector &normal,
	double		&deviation,
	logical		&zero_area
	);
/**
 * @nodoc
 * Get plane from a list of entities
 */
DECL_KERN logical get_plane_from_list_of_entities(
	const ENTITY_LIST&  ent_list,       // list of Bodies, Wires, faces, loops, or coedges.
	logical		        same_plane,     // Get the plane only if all the points of the entity are
	                                    // on the same plane.
	logical		        apply_transf,   // Get the transformed coordinates of cetroid and normal.
	SPAposition&        centroid,       // Centroid of the plane.
	SPAunit_vector&     normal,         // Normal of the plane.
	double&             deviation,      // Maximum deviation of points on the entity from the plane.
	logical&            zero_area       // TRUE if the winding number is zero.
	);
/** @} */
#endif
