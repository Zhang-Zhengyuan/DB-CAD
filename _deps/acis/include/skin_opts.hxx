/*******************************************************************/
/*    Copyright (c) 1989-2020 by Spatial Corp.                     */
/*    All rights reserved.                                         */
/*    Protected by U.S. Patents 5,257,205; 5,351,196; 6,369,815;   */
/*                              5,982,378; 6,462,738; 6,941,251    */
/*    Protected by European Patents 0503642; 69220263.3            */
/*    Protected by Hong Kong Patent 1008101A                       */
/*******************************************************************/
#if !defined( SKIN_OPTIONS_CLASS )
#define SKIN_OPTIONS_CLASS

#include "acis.hxx"
#include "dcl_skin.h"
#include "vers.hxx"

//Enum for the preferences of skin twist-minimization
//Not to be documented as the twist-prefereces are taken through intergers in
//the methods get_no_twist() and set_no_twist().
enum skin_twist_preference
{
	SKIN_TWIST_ALLOWED = 0,
	SKIN_TWIST_MIN_CG_BASED,
	SKIN_TWIST_MIN_DIST_BASED,
	SKIN_TWIST_MIN_ANGLE_BASED
};

/**
 * @file skin_opts.hxx
 * @CAA2Level L1
 * @CAA2Usage U1
 * \addtogroup SKINLOFTAPI
 * @{
 */
#define EXTENDED 0
#define ROUNDED  1
#define NATURAL  2

#if !defined(UNSET)
	#define UNSET -1
#endif

/**
 * Type of lateral gap covering.
 * @param SKIN_GAP_EXTENDED
 * Lateral surfaces are extended naturally.
 * @param SKIN_GAP_ROUNDED
 * The gap is filled with a face that is tangent to the two lateral faces.
 * @param SKIN_GAP_CHAMFERED
 * The gap is filled with a %surface that linearly connects the two lateral faces.
 */
enum skin_gap_type
{
	SKIN_GAP_EXTENDED,
	SKIN_GAP_ROUNDED,
	SKIN_GAP_CHAMFERED
};

/**
 * Enumeration type used to specify guide %curve constraint preferences.
 * @param FOLLOW_GUIDE_CONSTRAINT
 * Skin %surface follows guide constraint.
 * @param FOLLOW_TANGENT_CONSTRAINT
 * Skin %surface follows the tangent constraint.
 */
enum guide_curve_preference {
	FOLLOW_GUIDE_CONSTRAINT,
	FOLLOW_TANGENT_CONSTRAINT};

/**
 * Sets options for skinning.
 * <br>
 * <b>Role:</b> This class holds various options for the skinning operation. The following
 * lists the options:
 * <ul>
 * <li> <tt>arc_length</tt>: This option is used to choose either arc length or
 *      isoparametric parameterization of the skinning %surface. For basic skinning and lofting, with isoparametric
 *      parameterization, the %surface parameter in the <i>v</i> direction follows the cross section curves. With arc length
 *      parameterization, the %surface parameter follows lines of constant arc length. The default is isoparametric 
 *      parameterization. In the case of skinning with guide curves, with arc length parameterization - the guide %curve
 *      is arc length parameterized; however, the %surface is still isoparametric. The default is <tt>FALSE</tt>. </li>
 * 
 * <li> <tt>arc_length_u</tt>: This option reparameterizes the curves of the skinning or lofting profiles to arc length. 
 *      (The <i>u</i> parameter direction is along the profile curves; the <i>v</i> parameter direction is between the profile curves.)
 *      The default is <tt>FALSE</tt>. </li>
 * 
 * <li> <tt>no_twist</tt>: This option is used to minimize the twist of the %surface produced. Twist
 *      minimization aligns closed curves such that the start of the second %curve is aligned to the start
 *      of the first %curve. Even if a body's shape is unaffected by twisting, a %surface with a twist could
 *      produce unexpected results when faceting and rendering. The default is <tt>TRUE</tt>. </li>
 * 
 * <li> <tt>align</tt>: This option is used to align the direction of the cross section curves such that the
 *      normal of the first profile points towards the second profile. All other profiles are aligned to
 *      follow the first and second. If the cross sections are not oriented in the same direction, the <tt>align</tt>
 *      option is recommended to avoid producing a twisted, self-intersecting body. The default is <tt>TRUE</tt>. </li>
 * 
 * <li> <tt>perpendicular</tt>: The take-off vector is a tangent vector going out of the starting edge or 
 *      %surface and into the skinned or lofted %surface. The <tt>perpendicular</tt> option (for lofting only) is used 
 *      to specify the direction of the take-off vector, perpendicular to the coedge or in the loft direction.
 *      (This removes any restriction that the take-off vector for the loft has to be determined by the 
 *      cross-product of the coedge tangent vector and the %surface normal times the tangent factor.)
 *      The default is in the loft direction, because a perpendicular take-off vector can cause 
 *      self-intersections to the %surface. </li>
 * 
 * <li> <tt>simplify</tt>: This option simplifies the %surface to an analytic %surface, if applicable. If all of the
 *      cross sections lie on a analytic %surface (e.g., %plane, cylinder, %cone, %sphere, or %torus), the analytical
 *      %surface is created instead. The <tt>SPAresabs</tt> variable is used to determine whether or not the cross
 *      section lies on an analytical %surface. The default is <tt>TRUE</tt>. </li>
 * 
 * <li> <tt>closed</tt>: This option is used when the user needs to construct a body that is closed in the <i>v</i> direction.
 *      (e.g., a %torus). A solid body will be constructed only when all the wires supplied are closed.
 *      At least three profiles must be provided to create a closed body. The default is <tt>FALSE</tt>. 
 *      For historical reasons, the <tt>closed</tt> option can be set to 2, which is equivalent to setting the
 *      both the <tt>closed</tt> and <tt>periodic</tt> options to <tt>TRUE</tt>. </li>
 * 
 * <li> <tt>solid</tt>: This option is used when a solid body is desired, but not a solid body closed in the <i>v</i> direction.
 *      When a solid body that is not closed in the <i>v</i> direction is desired, the end wires are capped with planar faces. The
 *      default is <tt>TRUE</tt>. This option is forced to <tt>FALSE</tt> when skinning or lofting from faces or if the
 *      <tt>postprocess_stitch</tt> option is <tt>TRUE</tt>. </li>
 * 
 * <li> <tt>periodic</tt>: This option allows the construction of bodies that are periodic in the <i>v</i> direction;
 *      i.e., bodies that close back on themselves smoothly (continuously) at the start and end profiles. As with the 
 *      <tt>closed</tt> option, at least three profiles must be supplied to create a periodic loft body. 
 *      The default is <tt>FALSE</tt>. </li>
 * 
 * <li> <tt>virtualGuides</tt>: This option is used in order to have the user-defined guide %curves affect the body
 *      in a global nature. The default is <tt>FALSE</tt>.
 * 
 * <li> <tt>merge_wirecoedges</tt>: When this option is set to <tt>TRUE</tt>, the G<sup>1</sup> vertices of the skinning 
 *      and lofting wire profiles are removed by merging adjacent coedges/edges. This improves subsequent operations such as
 *      blending and shelling as it reduces the coedge/edge count and the number of surfaces, and
 *      eliminates near tangent edges. The default is <tt>TRUE</tt>. </li>
 * 
 * <li> <tt>estimate_loft_tanfacs</tt>: When this option is <tt>TRUE</tt>, the weight factor for the tangency conditions of
 *      the loft will be determined such that it minimizes the average radius of curvature of the lofting
 *      surfaces. The resulting bodies should support shelling to a greater thickness and also blending of
 *      their edges to larger blend radii. The default is <tt>FALSE</tt>. </li>
 * 
 * <li> <tt>match_vertices</tt>: This option suppresses the vertex-matching algorithm which ensures that all
 *      profiles consist of the same number of coedges. A heuristic approach is used to determine which
 *      vertex pairs are good matches. Profile coedges are then split where additional vertices are needed.
 *      This option is forced to <tt>TRUE</tt> if the number of coedges in the profiles are not equal. Its default
 *      is <tt>TRUE</tt>. </li>
 * 
 * <li> <tt>guide_curve_preference</tt>: This option is used to specify how an overconstrained guide is resolved. 
 *      This is an enumerated type with two possible values: <tt>FOLLOW_GUIDE_CONSTRAINT</tt> and 
 *      <tt>FOLLOW_TANGENT_CONSTRAINT</tt>. If the value <tt>FOLLOW_GUIDE_CONSTRAINT</tt> is set, then the resulting
 *      lofting %surface will always stay with the defining guide %curve. If <tt>FOLLOW_TANGENT_CONSTRAINT</tt>
 *      is specified, then the lofting %surface will always follow the tangent constraint. Default is
 *      <tt>FOLLOW_GUIDE_CONSTRAINT</tt>. </li>
 * 
 * <li> <tt>no_new_twist_vertices</tt>: The algorithm that minimizes the %surface twist may add vertices to some
 *      of the profiles if none of the existing vertices match well. This option allows the user
 *      to force the algorithm to choose matching vertices from the existing vertices. The default is
 *      <tt>FALSE</tt>. </li>
 * 
 * <li> <tt>allow_same_uv</tt>: This option allows surfaces with the same <i>u</i> and <i>v</i> direction to be created.  If the
 *      option is <tt>TRUE</tt> and a %surface with the same <i>u</i> and <i>v</i> direction is created, a warning 
 *      will be thrown. If <tt>FALSE</tt>, an error will be thrown. The default is <tt>FALSE</tt>.
 *      <br><br>
 *      <i><b>Note:</b> If this option is set <tt>TRUE</tt> and a %surface with the same <i>u</i> and <i>v</i> direction is created, 
 *      problems may appear during subsequent modeling operations.</i> </li>
 * 
 * <li> <tt>self_int_test</tt>: This option sets the behavior of the self-intersecting surface checks. Self-intersecting
 *      skin surfaces can be made based on poor tangent factor magnitudes, poor profiles, or an incorrect
 *      usage of the <tt>perpendicular</tt> option. If set to 0, the check will not be made and a body will be constructed; 
 *      however, problems may appear during subsequent modeling operations. If 2 is specified, the check will be performed, 
 *      but it will notify with a warning. The default value is 1, which means an error will be thrown and the self-intersecting 
 *      body will not be constructed. </li>
 *
 * <li> <tt>sweep_path</tt>: This option controls the behavior of the skin surface along the path curve. If this option is set,
 *		the resulting surface would be swept along the path and will join each profile smoothly. </li>
 *
 * <li> <tt>loft_interp_take_off</tt>: This option controls the take off vector fields at the input profiles. If this option is
 *      turned ON, the tangents of the edges adjacent to loft input profiles are used to generate the take off vector fields. </li>
 *
 * <li> <tt>smooth_g0_vertices</tt>: When profiles have G<sup>0</sup>, but not G<sup>1</sup>, continuity at vertices, 
 *      this option may be used to force a G<sup>1</sup> continuity at those vertices. The option specifies the maximum smooth 
 *      angle value, in degrees, between the tangent vectors from the two spline curves at the G<sup>0</sup> vertex. If the angle 
 *      is less than or equal to the specified value, the two spline curves will be modified to have a G<sup>1</sup> continuity 
 *      at the vertex. The default value is 0.0, which means no smoothing will be performed. </li>
 * 
 * <li> <tt>postprocess_stitch</tt>: This option stitches the resulting lofted body to the original bodies from which
 *      its coedge definitions came. This option only works with <tt>api_loft_coedges</tt> (i.e., lofting) and not
 *      with any skinning operation (<tt>api_skin_wires</tt>). It is identical in nature to the stitching operation
 *      performed in <tt>api_loft_faces</tt>. The default is <tt>TRUE</tt>. </li>
 * 
 * <li> <tt>gap_type</tt>: This option specifies the type of gap to be placed between
 *      the new faces. The type can be "extended" (extending the surfaces and
 *      intersecting), "rounded" (%surface tangent to both lateral faces), or
 *      "chamfered" (a ruled surface between both lateral faces). The type is specified
 *      using a @href skin_gap_type enumeration, whose value may be <tt>SKIN_GAP_EXTENDED</tt>,
 *      <tt>SKIN_GAP_ROUNDED</tt>, or <tt>SKIN_GAP_CHAMFERED</tt>. Presently, the
 *      gap types "rounded" and "chamfered" are available only when
 *      there are two profile curves specified for skinning. If more than two profile
 *      curves are specified, then the gap type (if different from
 *      "extended") is changed to "extended" and a warning is issued.
 *      When the "chamfered" or "rounded" option is used,
 *      the two neighboring surfaces of the gap must not have a partial intersection.
 *      In the case of any gap failure, the skin operation will re-try the gap
 *      algorithm in the following order: "extended", "rounded", and
 *      then "chamfered". A warning is issued if the <tt>gap_type</tt> is changed. 
 *      Default is "extended." </li>
 * 
 * <li> <tt>guide_tol</tt>: Specifies the (positional) tolerance between the wire
 *      bodies and the virtual guide curves. The default is <tt>SPAresfit</tt>. </li>
 * </ul>
 */
class DECL_SKIN skin_options : public ACIS_OBJECT
{
private:
	// -1 unset; 0 false; 1 true
	int arc_length;
	int	no_twist;
	int	align;
	int	perpendicular;
	int	simplify;
	int	closed;
	int	solid;
	int	periodic;
	int	virtualGuides;
	int merge_wirecoedges;
	int estimate_loft_tanfacs;
	int match_vertices;
	int no_new_twist_vertices; // net: not all the way passed down but set to FALSE in sg_make_wires_compatible
	int allow_same_uv;
    int postprocess_stitch;
	int arc_length_u_option;
    guide_curve_preference guide_pref;
    int self_int_test;
	int sweep_path;
  	int loft_interp_take_off;
	skin_gap_type m_gap_type;
	double m_guide_tol;
    double m_smooth_g0_vertices;

    void init_variables();

public:
/**
 * Default constructor.
 */
	skin_options();

/**
 * Default destructor.
 */
	~skin_options();


// Set functions.

/**
 * Sets the value of the <tt>arc_length</tt> option.
 * <b>Role:</b> Specifies whether the surface will be arc length parameterized in the <i>v</i> direction. 
 * The default is <tt>FALSE</tt>.
 * @param set
 * new value. 
 */
	void set_arc_length(int set);

/**
 * Sets the value of twist minimization preference.
 * <b>Role:</b> Minimize the twist of the %surface produced.  The values are: 0 (allow twist of %surface),
 1 (CG based twist minimization), 2 (Distance based twist minimization), 3 (Angle based twist minimization). 
 The default is 1.
 * @param set
 * new value (0, 1, 2 or 3).
 */
	void set_no_twist(int set);

/**
 * Sets the value of the <tt>align</tt> option.
 * <b>Role:</b> Align the direction of the cross section curves. The default is <tt>TRUE</tt>.
 * @param set
 * new value.
 */
	void set_align(int set);

/**
 * Sets the value of the <tt>perpendicular</tt> option.
 * <b>Role:</b> Specifies the direction of the take-off vector, perpendicular to the coedge or in the loft direction.
 * Applies to lofting only. The default is <tt>FALSE</tt>.
 * @param set
 * new value.
 */
	void set_perpendicular(int set);

/**
 * Sets the value of the <tt>simplify</tt> option.
 * <b>Role:</b> Specifies an attempt should be made to simplify the new %surface to an analytic %surface. 
 * The default is <tt>TRUE</tt>.
 * @param set
 * new value.
 */
	void set_simplify(int set);

/**
 * Sets the value of the <tt>closed</tt> option.
 * <b>Role:</b> Construct a body that is closed in the <i>v</i> direction. The default is <tt>FALSE</tt>.
 * @param set
 * new value.
 */
	void set_closed(int set);

/**
 * Sets the value of the <tt>solid</tt> option.
 * <b>Role:</b> A solid body is desired, but not a solid body closed in the <i>v</i> direction. The default is <tt>TRUE</tt>.
 * @param set
 * new value.
 */
	void set_solid(int set);

/**
 * Sets the value of the <tt>periodic</tt> option.
 * <b>Role:</b> Construct a body that is periodic in the <i>v</i> direction. The default is <tt>FALSE</tt>.
 * @param set
 * new value.
 */
	void set_periodic(int set);

/**
 * Sets the value of the <tt>virtualGuides</tt> option.
 * <b>Role:</b> Have the user-defined guide %curves affect the body in a global nature. The default is <tt>FALSE</tt>.
 * @param set
 * new value.
 */
	void set_virtualGuides(int set);

/**
 * Sets the value of the <tt>merge_wirecoedges</tt> option.
 * <b>Role:</b> Specifies whether G<sup>1</sup> vertices of the skinning and lofting wire profiles are removed 
 * by merging adjacent edges. The default is <tt>TRUE</tt>.
 * @param set
 * new value.
 */
	void set_merge_wirecoedges(int set);

/**
 * Sets the value of the <tt>estimate_loft_tanfacs</tt> option.
 * <b>Role:</b> Determine the weight factor for the tangency conditions of the loft such that it minimizes the 
 * average radius of curvature of the lofting surfaces. The default is <tt>FALSE</tt>.
 * @param set
 * new value.
 */
	void set_estimate_loft_tanfacs(int set);

/**
 * Sets the value of the <tt>match_vertices</tt> option.
 * <b>Role:</b> Suppresses the vertex-matching algorithm which ensures that all profiles consist of the same number of coedges.
 * The default is <tt>TRUE</tt>.
 * @param set
 * new value.
 */
	void set_match_vertices(int set);

/**
 * Sets the value of the <tt>no_new_twist_vertices</tt> option.
 * <b>Role:</b> The algorithm that minimizes the %surface twist may add vertices to some
 * of the profiles if none of the existing vertices match well. This option allows the user
 * to force the algorithm to choose matching vertices from the existing vertices. The default is
 * <tt>FALSE</tt>. 
 * @param set
 * new value.
 */
	void set_no_new_twist_vertices(int set);

/**
 * Sets the value of the <tt>allow_same_uv</tt> option.
 * <b>Role:</b> Allows surfaces with the same <i>u</i> and <i>v</i> direction to be created. The default is <tt>FALSE</tt>.
 * @param set
 * new value.
 */
	void set_allow_same_uv(int set);

/**
 * Sets the value of the <tt>arc_length_u</tt> option.
 * <b>Role:</b> Specifies whether the surface will be arc length parameterized in the <i>u</i> direction.
 * The default is <tt>FALSE</tt>.
 * @param set
 * new value.
 */
	void set_arc_length_u(int set);

/**
 * Sets the value of the <tt>postprocess_stitch</tt> option.
 * <b>Role:</b> Stitches the resulting lofted body to the original bodies from which
 * its coedge definitions came. This applies to lofting only. The default is <tt>TRUE</tt>.
 * @param set
 * new value.
 */
	void set_postprocess_stitch(int set);

/**
 * Sets the value of the <tt>guide_curve_preference</tt> option.
 * <b>Role:</b> Specifies how an overconstrained guide is resolved. Default is
 * <tt>FOLLOW_GUIDE_CONSTRAINT</tt>.
 * @param set
 * <tt>guide_curve_preference</tt>, either <tt>FOLLOW_GUIDE_CONSTRAINT</tt> or <tt>FOLLOW_TANGENT_CONSTRAINT</tt>.
 */
    void set_guide_constraint_preference(guide_curve_preference set);

 /**
 * Sets the value of the <tt>sweep_path</tt> option.
 * <b>Role:</b> Specifies the type of skinning surface, the profiles would be swept along the given path.
 * @param sweep_path
 */
    void set_sweep_path( int sweep_path );

/**
 * Sets the value of the <tt>loft_interp_take_off</tt> option.
 * <b>Role:</b> Specifies whether lofting is based on interpolation of take_off vectors from ends of adjacent edges.
 * @param loft_interp_take_off
 */
    void set_loft_interp_take_off( int loft_interp_take_off );

 /**
 * Sets the value of the <tt>self_int_test</tt> option.
 * <b>Role:</b> Sets the type of self-intersection testing. This test is performed at the moment of
 * the creation of surfaces. The values are: 0 (No checking), 1 (check and throw error), 2 (check and throw warning, resulting body will contain
 * self-intersecting surfaces). Default is 1.
 * @param set
 * type of check (0, 1, or 2).
 */
    void set_self_int_test(int set);

/**
 * Sets the value of the <tt>gap_type</tt> option.
 * @param gap_type
 * gap can be "extended", "rounded", or "chamfered". Default is "extended."
 */
	void set_gap_type(skin_gap_type gap_type);

/**
 * Sets the value of the <tt>guide_tol</tt> option.
 * @param tol
 * new virtual guide %curve tolerance value. Default is <tt>SPAresfit</tt>.
 */
	void set_guide_tol(double tol);

/**
 * Sets the value of the <tt>smooth_g0_vertices</tt> option.
 * @param iAngle
 * maximum smoothing angle beween two tangent vectors from two connected %spline %curves at a G<sup>0</sup> vertex.
 */
	void set_smooth_g0_vertices (const double iAngle);

// Get functions.

/**
 * Returns the current value of the <tt>arc_length</tt> option.
 */
	int get_arc_length() const ;

/**
 * Returns the current value of the <tt>no_twist</tt> option.
 */
	int get_no_twist() const ;

/**
 * Returns the current value of the <tt>align</tt> option.
 */
	int get_align() const ;

/**
 * Returns the current value of the <tt>perpendicular</tt> option.
 */
	int get_perpendicular() const ;

/**
 * Returns the current value of the <tt>simplify</tt> option.
 */
	int get_simplify() const ;

/**
 * Returns the current value of the <tt>closed</tt> option.
 */
	int get_closed() const ;

/**
 * Returns the current value of the <tt>solid</tt> option.
 */
	int get_solid() const ;

/**
 * Returns the current value of the <tt>periodic</tt> option.
 */
	int get_periodic() const ;

/**
 * Returns the current value of the <tt>virtualGuides</tt> option.
 */
	int get_virtualGuides() const ;

/**
 * Returns the current value of the <tt>merge_wirecoedges</tt> option.
 */
	int get_merge_wirecoedges() const ;

/**
 * Returns the current value of the <tt>estimate_loft_tanfacs</tt> option.
 */
	int get_estimate_loft_tanfacs() const ;

/**
 * Returns the current value of the <tt>match_vertices</tt> option.
 */
	int get_match_vertices() const ;

/**
 * Returns the current value of the <tt>no_new_twist_vertices</tt> option.
 */
	int get_no_new_twist_vertices() const ;

/**
 * Returns the current value of the <tt>guide_curve_preference</tt> option.
 */
    guide_curve_preference get_guide_constraint_preference() const ;

/**
 * Returns the current value of the <tt>sweep_path</tt> option.
 */
    int get_sweep_path() const ;

/**
 * Returns the current value of the <tt>loft_interp_take_off</tt> option.
 */
    int get_loft_interp_take_off() const ;

/**
 * Returns the current value of the <tt>allow_same_uv</tt> option.
 */
    int get_allow_same_uv() const ;

/**
 * Returns the current value of the <tt>arc_length_u</tt> option.
 */
	int get_arc_length_u() const ;

/**
 * Returns the current value of the <tt>postprocess_stitch</tt> option.
 */
	int get_postprocess_stitch() const ;

/**
 * Returns the current value of the <tt>self_int_test</tt> option.
 */
	int get_self_int_test() const ;

/**
 * Returns the current value of the <tt>gap_type</tt> option.
 */
	skin_gap_type get_gap_type() const;

/**
 * Determines if two <tt>skin_options</tt> are equal.
 * @param in_skin_opt
 * other <tt>skin_options</tt>.
 */
	logical operator== (skin_options const& in_skin_opt) const;

/**
 * Tests two <tt>skin_options</tt> for inequality.
 * @param in_skin_opt
 * other <tt>skin_options</tt>.
 */
	logical operator!= (skin_options const& in_skin_opt) const;

/**
 * Returns the current value of the <tt>guide_tol</tt> option.
 */
	double get_guide_tol() const;

/**
 * Returns the current value of the <tt>smooth_g0_vertices</tt> option.
 */
	double get_smooth_g0_vertices() const;

 /**
 * Sets the default value of the options, if they have not been previously set.
 * @param arc_length
 * value for the <tt>arc_length</tt> option.
 * @param no_twist
 * value for the <tt>no_twist</tt> option.
 * @param align
 * value for the <tt>align</tt> option.
 * @param perpendicular
 * value for the <tt>perpendicular</tt> option.
 * @param simplify
 * value for the <tt>simplify</tt> option.
 * @param closed
 * value for the <tt>closed</tt> option.
 * @param solid
 * value for the <tt>solid</tt> option.
 * @param periodic
 * value for the <tt>periodic</tt> option.
 * @param virtual_guide_curves
 * value for the <tt>virtualGuides</tt> option.
 * @param merge
 * value for the <tt>merge_wirecoedges</tt> option.
 * @param tanfacs
 * value for the <tt>estimate_loft_tanfacs</tt> option.
 * @param breakup
 * value for the <tt>match_vertices</tt> option.
 * @param no_twist_verts
 * value for the <tt>no_new_twist_vertices</tt> option.
 * @param g_pref
 * value for the <tt>guide_curve_preference</tt> option.
 * @param same_uv
 * value for the <tt>allow_same_uv</tt> option.
 * @param arc_length_u
 * value for the <tt>arc_length_u</tt> option.
 * @param gap_type
 * value for the <tt>gap_type</tt> option.
 * @param guide_tol
 * value for the <tt>guide_tol</tt> option.
 */
    void set_defaults(int arc_length = FALSE,
                      int no_twist = TRUE,
                      int align = TRUE,
                      int perpendicular = FALSE,
                      int simplify = TRUE,
                      int closed = FALSE,
                      int solid = TRUE,
                      int periodic = FALSE,
                      int virtual_guide_curves = FALSE,
					  int merge = TRUE,
                      int tanfacs = FALSE,
                      int breakup = TRUE,
                      int no_twist_verts = FALSE,
					  guide_curve_preference g_pref = FOLLOW_GUIDE_CONSTRAINT,
                      int same_uv = FALSE,
                      int arc_length_u = FALSE,
                      int postprocess_stitch = FALSE,
                      int set_int_test = TRUE,
					  skin_gap_type gap_type = SKIN_GAP_EXTENDED,
					  double guide_tol = SPAresfit,
					  int sweep_path = FALSE,
					  int loft_interp_take_off = FALSE
					  );

};
/** @} */
#endif
