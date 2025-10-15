/*******************************************************************/
/*    Copyright (c) 1989-2020 by Spatial Corp.                     */
/*    All rights reserved.                                         */
/*    Protected by U.S. Patents 5,257,205; 5,351,196; 6,369,815;   */
/*                              5,982,378; 6,462,738; 6,941,251    */
/*    Protected by European Patents 0503642; 69220263.3            */
/*    Protected by Hong Kong Patent 1008101A                       */
/*******************************************************************/

#ifndef PATTERN_API_OLD_HXX
#define PATTERN_API_OLD_HXX

//  STL dal 21-Mar-2001: include this to supply missing def for M_PI (problems
//  compiling on non-windows machines).
#include <math.h>

#include "dcl_kern.h"
#include "acis3dt.h"
#include "api.hxx"
#include "interval.hxx"
#include "vector.hxx"

class pattern;
class law;
class SPApar_box;
class pcurve;
class SPAvector;
class SPAunit_vector;
class SPAposition;
class SPAtransf;
class curve;
class surface;
class EDGE;
class FACE;
class COEDGE;

// START DEPRECATED *****************************************************************
/**
 * @nodoc
 */
DECL_KERN outcome api_curve_pattern
(
    pattern*&           pat,
    const curve&        in_curve,
    const SPAinterval&  param_range,
    int                 num_elements,
    const SPAposition&  root,
    logical             on_endpoints    = FALSE,
    law*                rail_law        = NULL,
    const SPAvector&    rail_dir        = *(SPAvector*)NULL_REF,
    const SPAvector&    tangent_dir     = *(SPAvector*)NULL_REF,
    const SPAtransf&    in_trans        = *(SPAtransf*)NULL_REF,
	AcisOptions*        ao              = NULL);

/**
 * @nodoc
 */
DECL_KERN outcome api_cylindrical_pattern
(
    pattern*&           pat,
    const FACE*         in_face,
    int                 num_angular,
    int                 num_axial       = 1,
    double              ring_spacing    = 0.0,
    logical             alternating     = FALSE,
	AcisOptions*        ao              = NULL);

/**
 * @nodoc
 */
DECL_KERN outcome api_cylindrical_pattern
(
    pattern*&           pat,
    const SPAposition&  center,
    const SPAvector&    normal,
    int                 num_angular,
    int                 num_axial       = 1,
    double              ring_spacing    = 0.0,
    logical             alternating     = FALSE,
	AcisOptions*        ao              = NULL);

/**
 * @nodoc
 */
DECL_KERN outcome api_edge_pattern
(
    pattern*&           pat,
    EDGE*               in_edge,
    int                 num_elements,
    const SPAposition&  root,
    logical             on_endpoints    = FALSE,
    const SPAvector&    rail_dir        = *(SPAvector*)NULL_REF,
    const SPAvector&    tangent_dir     = *(SPAvector*)NULL_REF,
	AcisOptions*        ao              = NULL);

/**
 * @nodoc
 */
DECL_KERN outcome api_edge_pattern
(
    pattern*&           pat,
    EDGE*               in_edge,
    FACE*               in_face,
    int                 num_elements,
    const SPAposition&  root,
    logical             on_endpoints    = FALSE,
    const SPAvector&    normal_dir      = *(SPAvector*)NULL_REF,
    const SPAvector&    tangent_dir     = *(SPAvector*)NULL_REF,
	AcisOptions*        ao              = NULL);

/**
 * @nodoc
 */
DECL_KERN outcome api_edge_pattern
(
    pattern*&           pat,
    COEDGE*             in_coedge,
    int                 num_elements,
    const SPAposition&  root,
    logical             on_endpoints    = FALSE,
    const SPAvector&    normal_dir      = *(SPAvector*)NULL_REF,
    const SPAvector&    tangent_dir     = *(SPAvector*)NULL_REF,
	AcisOptions*        ao              = NULL);

/**
 * @nodoc
 */
DECL_KERN outcome api_elliptical_pattern
(
    pattern*&           pat,
    const SPAposition&  center,
    const SPAvector&    normal,
    int                 num_elements,
    logical             not_rotate      = FALSE,
    const SPAposition&  root            = *(SPAposition*)NULL_REF,
    double              angle           = 2.0 * M_PI,
    double              ratio           = 1.0,
    const SPAvector&    major_axis      = *(SPAvector*)NULL_REF,
	AcisOptions*        ao              = NULL);

/**
 * @nodoc
 */
DECL_KERN outcome api_hex_cylindrical_pattern
(
    pattern*&           pat,
    const FACE*         in_face,
    int                 num_angular,
    int                 num_axial       = 1,
    double              spacing         = 0.0,
	AcisOptions*        ao              = NULL);

/**
 * @nodoc
 */
DECL_KERN outcome api_hex_cylindrical_pattern
(
    pattern*&           pat,
    const SPAposition&  center,
    const SPAvector&    normal,
    int                 num_angular,
    int                 num_axial       = 1,
    double              spacing         = 0.0,
	AcisOptions*        ao              = NULL);

/**
 * @nodoc
 */
DECL_KERN outcome api_hex_pattern
(
    pattern*&           pat,
    const SPAvector&    normal,
    const SPAvector&    x_vec,
    int                 num_x,
    int                 num_y,
    int                 num_z           = 1,
	AcisOptions*        ao              = NULL);

/**
 * @nodoc
 */
DECL_KERN outcome api_linear_pattern
(
    pattern*&           pat,
    const SPAvector&    x_vec,
    int                 num_x,
    const SPAvector&    y_vec           = *(SPAvector*)NULL_REF,
    int                 num_y           = 1,
    const SPAvector&    z_vec           = *(SPAvector*)NULL_REF,
    int                 num_z           = 1,
    logical             y_staggered     = FALSE,
    logical             z_staggered     = FALSE,
    logical             fit_distance    = FALSE,
	AcisOptions*        ao              = NULL);

/**
 * @nodoc
 */
DECL_KERN outcome api_polar_grid_pattern
(
    pattern*&           pat,
    const SPAposition&  center,
	const SPAvector&    normal,
	int					num_rings,
	double				distance,
	const SPAvector&    start           = *(SPAvector*)NULL_REF,
	logical				not_rotate      = FALSE,
	logical				hex_symmetry    = FALSE,
	double				start_angle     = 0.0,
	double				end_angle       = 2.0 * M_PI,
	double				ratio           = 1.0,
	AcisOptions*        ao              = NULL);

/**
 * @nodoc
 */
DECL_KERN outcome api_radial_pattern
(
    pattern*&           pat,
    const SPAposition&  center,
	const SPAvector&    normal,
	int					num_radial,
	int					num_angular,
	double				spacing,
	const SPAvector&    start           = *(SPAvector*)NULL_REF,
	logical				not_rotate      = FALSE,
	double				start_angle     = 0.0,
	double				end_angle       = 2.0 * M_PI,
	double				ratio           = 1.0,
	AcisOptions*        ao              = NULL);
/**
 * @nodoc
 */
DECL_KERN outcome api_random_pattern
(
    pattern*&           pat,
    const SPAvector&    extents,
    int                 num_elements,
    int                 dimension       = 3,
    logical             ellipsoidal     = FALSE,
    const SPAvector&    x_vec           = SPAvector(1, 0, 0),
    const SPAvector&    y_vec           = SPAvector(0, 1, 0),
	AcisOptions*        ao              = NULL);

/**
 * @nodoc
 */
DECL_KERN outcome api_spherical_pattern
(
    pattern*&           pat,
    const SPAposition&  center,
    int                 num_latitudes,
    const SPAposition&  root,
    double              spacing         = 0.0,
	AcisOptions*        ao              = NULL);

/**
 * @nodoc
 */
DECL_KERN outcome api_surface_pattern
(
    pattern*&           pat,
    FACE*               in_face,
    int                 num_u,
    int                 num_v,
    const SPAposition&  root,
    logical             on_boundary     = FALSE,
    const SPAvector&    u_dir           = *(SPAvector*)NULL_REF,
    const SPAvector&    v_dir           = *(SPAvector*)NULL_REF,
	AcisOptions*        ao              = NULL);

/**
 * @nodoc
 */
DECL_KERN outcome api_surface_pattern
(
    pattern*&           pat,
    const surface&      in_surf,
    const SPApar_box&   face_range,
    int                 num_u,
    int                 num_v,
    const SPAposition&  root,
    logical             on_boundary     = FALSE,
    const SPAvector&    u_dir           = *(SPAvector*)NULL_REF,
    const SPAvector&    v_dir           = *(SPAvector*)NULL_REF,
    const SPAtransf&    in_trans        = *(SPAtransf*)NULL_REF,
	AcisOptions*        ao              = NULL);


/**
 * @nodoc
 */
DECL_KERN outcome api_alternating_keep_pattern
(
    pattern*&           pat,
    const pattern&      in_pattern,
    logical             keep1,
    logical             keep2,
    int                 which_dim,
    logical             merge           = TRUE,
	AcisOptions*        ao              = NULL);

/**
 * @nodoc
 */
DECL_KERN outcome api_alternating_scale_pattern
(
    pattern*&           pat,
    const pattern&      in_pattern,
    double              scale1,
    double              scale2,
    int                 which_dim,
    const SPAposition&  root,
    logical             merge           = TRUE,
	AcisOptions*        ao              = NULL);

/**
 * @nodoc
 */
DECL_KERN outcome api_alternating_scale_pattern
(
    pattern*&           pat,
    const pattern&      in_pattern,
    const SPAvector&    scale1,
    const SPAvector&    scale2,
    int                 which_dim,
    const SPAposition&  root,
    logical             merge           = TRUE,
	AcisOptions*        ao              = NULL);

/**
 * @nodoc
 */
DECL_KERN outcome api_linear_scale_pattern
(
    pattern*&           pat,
    const pattern&      in_pattern,
    double              first_scale,
    double              last_scale,
    int                 which_dim,
    const SPAposition&  root,
    logical             merge           = TRUE,
	AcisOptions*        ao              = NULL);

/**
 * @nodoc
 */
DECL_KERN outcome api_linear_scale_pattern
(
    pattern*&           pat,
    const pattern&      in_pattern,
    const SPAvector&    first_scale,
    const SPAvector&    last_scale,
    int                 which_dim,
    const SPAposition&  root,
    logical             merge           = TRUE,
	AcisOptions*        ao              = NULL);

/**
 * @nodoc
 */
DECL_KERN outcome api_periodic_keep_pattern
(
    pattern*&           pat,
    const pattern&      in_pattern,
    const logical*      keep,
    int                 period,
    int                 which_dim,
    logical             merge           = TRUE,
	AcisOptions*        ao              = NULL);

/**
 * @nodoc
 */
DECL_KERN outcome api_periodic_scale_pattern
(
    pattern*&           pat,
    const pattern&      in_pattern,
    const double*       scale,
    int                 period,
    int                 which_dim,
    const SPAposition&  root,
    logical             merge           = TRUE,
	AcisOptions*        ao              = NULL);

/**
 * @nodoc
 */
DECL_KERN outcome api_periodic_scale_pattern
(
    pattern*&           pat,
    const pattern&      in_pattern,
    const SPAvector*    scale,
    int                 period,
    int                 which_dim,
    const SPAposition&  root,
    logical             merge           = TRUE,
	AcisOptions*        ao              = NULL);

/**
 * @nodoc
 */
DECL_KERN outcome api_random_keep_pattern
(
    pattern*&           pat,
    const pattern&      in_pattern,
    double              keep_fraction,
    logical             merge           = TRUE,
	AcisOptions*        ao              = NULL);

/**
 * @nodoc
 */
DECL_KERN outcome api_random_offset_pattern
(
    pattern*&           pat,
    const pattern&      in_pattern,
    const SPAvector&    amplitude,
	AcisOptions*        ao              = NULL);

/**
 * @nodoc
 */
DECL_KERN outcome api_random_orient_pattern
(
    pattern*&           pat,
    const pattern&      in_pattern,
    const SPAposition&  root            = SPAposition(0, 0, 0),
    const SPAinterval&  axial_range     = SPAinterval(0.0, 2.0 * M_PI),
    const SPAvector&    axial_dir       = *(SPAvector*)NULL_REF,
    const SPAinterval&  tilt_range      = SPAinterval(0, M_PI),
    const SPAvector&    tilt_dir        = *(SPAvector*)NULL_REF,
	AcisOptions*        ao              = NULL);

/**
 * @nodoc
 */
DECL_KERN outcome api_random_scale_pattern
(
    pattern*&           pat,
    const pattern&      in_pattern,
    double              min_scale,
    double              max_scale,
    const SPAposition&  root,
    logical             merge           = TRUE,
	AcisOptions*        ao              = NULL);

/**
 * @nodoc
 */
DECL_KERN outcome api_random_scale_pattern
(
    pattern*&           pat,
    const pattern&      in_pattern,
    const SPAvector&    min_scale,
    const SPAvector&    max_scale,
    const SPAposition&  root,
    logical             merge           = TRUE,
	AcisOptions*        ao              = NULL);

/**
 * @nodoc
 */
DECL_KERN outcome api_remove_pattern(
    ENTITY*         ent,
    AcisOptions*    ao          = NULL);

/**
 * @nodoc
 */
DECL_KERN outcome api_get_pattern_transfs(
    const pattern&  pat,
    SPAtransf*&     transforms,
    int&            num,
	AcisOptions*    ao          = NULL);

// END DEPRECATED *****************************************************************

#endif //PATTERN_API_OLD_HXX
