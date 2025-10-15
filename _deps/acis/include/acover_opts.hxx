/*******************************************************************/
/*    Copyright (c) 1989-2020 by Spatial Corp.                     */
/*    All rights reserved.                                         */
/*    Protected by U.S. Patents 5,257,205; 5,351,196; 6,369,815;   */
/*                              5,982,378; 6,462,738; 6,941,251    */
/*    Protected by European Patents 0503642; 69220263.3            */
/*    Protected by Hong Kong Patent 1008101A                       */
/*******************************************************************/
// bmt 07-Feb-03  1. Doc templates
//                2. num_spans ==> max_spans
//                3. remove default_tolerance method (never released)
//                4. add set_boundary_constraint() methods taking acovr_continuity_level
//
// bmt 10-Feb-03  1. name change: acover_options ==> acovr_options
//                2. Doc revisions
//
// bmt 24-Feb-03  1. add set_guide() & set_guides() methods which take a tol
//                2. add get_guide_tol() method
//
// bmt 24-Jun-04 add get/set methods for final check
//
#if !defined( AC_INT_OPTIONS_CLASS )
#define AC_INT_OPTIONS_CLASS

#include "logical.h"
#include "dcl_adm.h"           // DECL_COVR           // DECL_COVR

#include "acis.hxx"
#include "acovr_enum.hxx"

class acovr_edge_constraint;
class acovr_gap_report;
class adv_cover_options;
class FACE;
class EDGE;
class ENTITY;
class ENTITY_LIST;
class SPAunit_vector;

/**
* @file acover_opts.hxx
 * @CAA2Level L1
 * @CAA2Usage U1
 * \addtogroup ACISADVCOVR
 *
 * @{
 */

/**
 * Options for covering with advanced covering.
 * <br>
 * <b>Role:</b> This class is used to configure the covering problem. It allows
 * you to specify constraint continuity, contraint tolerance, maximum number of the spline
 * covering surface spans, and shape parameters for the covering algorithm. During the
 * problem configuration, the <tt>acovr_options</tt> object can be queried to determine
 * its current state. Following covering, the object can then be queried to
 * determine how well the constraints were satisfied. 
 * You must call <tt>set_default_constraint</tt> before making any changes to the 
 * <tt>acovr_options</tt> object.
 * <br><br>
 * @see acovr_edge_constraint, acovr_gap_report, ENTITY_LIST
 */
class DECL_ADM acovr_options : public ACIS_OBJECT
{
	private:
		adv_cover_options *m_ac_opts;
		// users should call clone method instead of copy constuctor or operator =
		acovr_options (acovr_options const &acovr_options);
		acovr_options &operator=  (acovr_options const &acovr_options);

	public:
		/**
		 * Constructs an <tt>acovr_options</tt> object.
		 */
		acovr_options();
		/**
		 * Destructs an <tt>acovr_options</tt> object.
		 */
		~acovr_options();

		/**
		 * Deep copy method.
		 * <br><br>
		 * <b>Role:</b> This function is provides the basis for an <tt>acovr_options</tt>
		 * "factory" which produces <tt>acovr_options</tt> objects with user-specified input settings.
		 * An example use case is covering a collection of separate circuits, all with the same
		 * continuity and tolerance specifications.
		 *
		 */
		acovr_options *clone() const;
/**
 * @nodoc
 */

		int operator== (acovr_options& in_ac_opt);
/**
 * @nodoc
 */
		int operator!= (acovr_options& in_ac_opt);
		// user options
		/**
		 * Sets the maximum number of spans in the covering surface.
		 * <br><br>
		 * <b>Role:</b> This is an integer and controls the maximum number of
		 * B-spline spans in each direction that the covering surface can have.
		 * Small values result in surfaces with few degrees of freedom that may
		 * have large gaps, while large values may result in surfaces with many
		 * control points, which adversely affects performance.
		 * <br><br>
		 * @param max_spans
		 *   Maximum number of spans.
		 */
		void set_max_spans(int max_spans);

		/**
		 * Returns the specified maximum number of spans.
		 * <br><br>
		 * <b>Role:</b> This is an integer and controls the maximum number of B-spline
		 * spans in each direction that the covering surface can have. Small
		 * values result in surfaces without many degrees of freedom that may
		 * have large gaps, while large values may result in surfaces with many
		 * control points, which adversely affects performance.
		 */
		int get_max_spans() const;

		/**
		 * Sets the value of the flattening parameter.
		 * <br><br>
		 * <b>Role:</b> Flattening parameter is a positive number that controls the
		 * effect of G1 constraints on the shape of the covering surface. When G1
		 * constraints force a large change between the shape of the covering and
		 * initial surfaces, the flattening parameter controls the region of effect
		 * of the G1 constraints.  A large (0.7 or so) flattening parameter will
		 * cause the "rotation" (deviation of slope from that of the initial surface)
		 * of the covering surface to be localized to the vicinity of the G1
		 * constrained edge, resulting in a flattened interior which has a shape
		 * close to that of the corresponding G0 cover. A small flattening parameter
		 * will cause the effects of the G1 constraint to be spread out, resulting
		 * in a more "domed" interior. When performing a G1 cover of the top of a
		 * cylinder, for example doming, the height of the dome is controlled
		 * purely by the flattening parameter. Small values of flattening result
		 * in high, round domes, while large values result in short, flat domes.
		 * In general, small values (0.1 or so) should be used when the support
		 * surfaces for all G1 constraints are already close to being tangent to the
		 * initial surface, while large values (0.4 to 0.7) should be used when the
		 * support surfaces are close to perpendicular (as in the case of the top of a cylinder).
		 * <br><br>
		 * @param flattening
		 *   Flattening parameter to set.
		 */
		void set_flattening(double flattening);

		/**
		 * Returns the value of the flattening parameter.
		 * <br><br>
		 * @see #set_flattening
		 */
		double get_flattening() const;

		// advanced user options
		/**
		 * Chooses an initial spline surface.
		 * <br><br>
		 * <b>Role:</b> The initial surface will be obtained by copying the geometry
		 * of the face argument. The initial face can be unset by calling this
		 * routine with a face parameter of <tt>NULL</tt>. Note that the <tt>plane_normal</tt>
		 * parameter is ignored if an <tt>initial_face</tt> is set.
		 * <br><br>
		 * @param face
		 *   Face whose geometry specifies initial surface.
		 */
		void set_initial_face(const FACE* face);

		/**
		 * Returns a pointer to the <tt>initial_face</tt>.
		 * <br><br>
		 * <b>Role:</b> A return value of <tt>NULL</tt> means that the <tt>initial_face</tt> is unset,
		 * in which case the initial surface will be obtained from the projection plane.
		 * Note that the <tt>plane_normal</tt> parameter is ignored if an <tt>initial_face</tt> is set.
		 */
		const FACE* get_initial_face() const;

		// constraint specifications
		/**
		 * Specifies the default continuity and tolerance.
		 * <br><br>
		 * @param default_values
		 *   Edge constraint default values.
		 */
		void set_default_constraint(const acovr_edge_constraint& default_values );

		/**
		 * Returns a copy of the default constraint.
		 */
		acovr_edge_constraint get_default_constraint() const;

		/**
		 * Overrides the default continuity and tolerance specification for a particular boundary edge.
		 * <br><br>
		 * @param bdry_edge
		 * Boundary edge.
		 * @param constraint
		 *   Edge constraint.
		 */
		void set_boundary_constraint (       EDGE* bdry_edge,         const acovr_edge_constraint& constraint);

		/**
		 * Overrides the default continuity specification for a particular boundary edge.
		 * <br><br>
		 * @param bdry_edge
		 * Boundary edge.
		 * @param continuity
		 *   Continuity level (G0 or G1).
		 */
		void set_boundary_constraint (       EDGE* bdry_edge,         acovr_continuity_level continuity);

		/**
		 * Overrides the default continuity and tolerance specification for a list of boundary edges.
		 * <br><br>
		 * @param bdry_edges
		 * Boundary edges.
		 * @param constraint
		 *   Edge constraint.
		 */
		void set_boundary_constraints( const ENTITY_LIST& bdry_edges, const acovr_edge_constraint& constraint);

		/**
		 * Overrides the default continuity specification for a list of boundary edges.
		 * <br><br>
		 * @param bdry_edges
		 * Boundary edges.
		 * @param continuity
		 *   Continuity level (G0 or G1).
		 */
		void set_boundary_constraints( const ENTITY_LIST& bdry_edges, acovr_continuity_level continuity);

		/**
		 * Specifies a list of guide points (vertices) or guide curves (edges). Uses default G0 tolerance.
		 * <br><br>
		 * <b>Role:</b> Any edge specified must be contained within an ACIS body.
		 * <br><br>
		 * @param constraining_ents
		 *   Guide entities (edges or vertices).
		 */
		void set_guides( const ENTITY_LIST& constraining_ents );

		/**
		 * Specifies a list of guide points (vertices) or guide curves (edges).
		 * <br><br>
		 * <b>Role:</b> Any edge specified must be contained within an ACIS body.
		 * <br><br>
		 * @param constraining_ents
		 *   Guide entities (edges or vertices).
		 * @param tolerance
		 *   requested G0 tolerance.
		 */
		void set_guides( const ENTITY_LIST& constraining_ents, double tolerance );

		/**
		 * Specifies a guide point (vertex) or guide curve (edge). Uses default G0 tolerance.
		 * <br><br>
		 * <b>Role:</b> Any edge specified must be contained within an ACIS body.
		 * <br><br>
		 * @param constraining_ent
		 *   vertex or edge.
		 */
		void set_guide(const ENTITY* constraining_ent); // use default tolerance

		/**
		 * Specifies a guide point (vertex) or guide curve (edge).
		 * <br><br>
		 * <b>Role:</b> Any edge specified must be contained within an ACIS body.
		 * <br><br>
		 * @param constraining_ent
		 *   vertex or edge.
		 * @param tolerance
		 *   requested G0 tolerance.
		 */
		void set_guide(const ENTITY* constraining_ent, double tolerance);

		/**
		 * Returns the G0 tolerance for the guide point (vertex) or guide curve (edge).
		 * <br><br>
		 * @param constraining_ent
		 *   vertex or edge.
		 */
		double get_guide_tol(const ENTITY* constraining_ent) const;

		/**
		 * Retrieves the list of boundary edges.
		 * <br><br>
		 * <b>Role:</b> This method overwrites the "boundaries" list with a list
		 * of all the edges which had a constraint applied to them through the
		 * <tt>set_edge_constraint</tt> or <tt>set_edge_constraints</tt> methods.
		 * <br><br>
		 * <i><b>Note:</b> Because edges are usually replaced with tolerant edges
		 * during the Advanced Covering process, this API sets "boundaries" to
		 * an empty list when called after the</i> <tt>acovr_options</tt> <i>object has been
		 * used in a call to</i> <tt>api_advanced_cover</tt>.
		 * <br><br>
		 * @param oboundaries
		 *   Constrained edges.
		 */
		void get_constrained_boundaries(ENTITY_LIST& oboundaries) const;  // Get boundary edges

		/**
		 * Returns the constraints of a boundary edge.
		 * <br><br>
		 * @param bdy_ent
		 *   Given edge.
		 */
		acovr_edge_constraint get_boundary_constraint( const EDGE *bdy_ent) const;

		/**
		 * Retrieves the edges defining the guide curves.
		 * <br><br>
		 * @param guide_edges
		 *   Guide edges.
		 */
		void get_guide_curves(ENTITY_LIST& guide_edges) const;	// Get edges defining guide curves

		/**
		 * Retrieves the vertices defining the guide points.
		 * <br><br>
		 * @param guide_vertices
		 *   Guide vertices.
		 */
		void get_guide_points(ENTITY_LIST& guide_vertices) const;	// Get vertices defining guide points

		/**
		 * Retrieves the maximum gap in position (G0) and angle (G1).
		 * <br><br>
		 * <b>Role:</b> This method examines each boundary edge's constraint and
		 * reports the maximum gap in position (G0) and angle (G1).
		 * Guide curve gaps are not considered. Note that for performance reasons,
		 * the gap reported is the gap between points that Advanced Covering was
		 * trying to make coincide.
		 * <br><br>
		 * @param gr
		 *   Contains maximum gap values.
		 */
		void report_max_gap(acovr_gap_report& gr) const;  // Report max gap of all boundary constraints

		/**
		 * Returns <tt>TRUE</tt>, if the <tt>plane_normal</tt> parameter has been set or <tt>FALSE</tt> otherwise.
		 */
		logical input_plane_specified() const;

/**
 * Defines the initial projection plane.
 * <br><br>
 * <b>Role:</b> Since the boundary circuit will be projected into this plane, only
 * a normal vector is needed; the absolute position of the plane is irrelevant.
 * Advanced Covering automatically calculates a default initial plane from the
 * shape of the boundary circuit; the <tt>plane_normal</tt> parameter should be set if
 * this default plane is unsatisfactory. Usually this will occur because the
 * default algorithm picks a plane that would result in a double-valued covering
 * surface (not allowed) or because the model containing the boundary circuit has
 * a symmetry that is not obeyed by the boundary circuit (G1 covers only).
 * For example, consider performing a G1 cover of a cylinder that has been sliced
 * by a plane whose normal was not perpendicular to the axis of the cylinder.
 * Advanced Covering tries to satisfy G1 constraints by pushing the covering
 * surface up in the normal direction of the initial surface. The default plane
 * for the sliced cylinder will have the same normal as the slicing plane; the G1
 * constraints in this case will cause the covering surface to bulge away from
 * the axis of the cylinder. A much more natural shape is found when the normal
 * vector is set to coincide with the axis; in this case the bulge is along the
 * axis, with a very smooth transition between surfaces. Note that the <tt>plane_normal</tt>
 * parameter will be ignored if an initial spline surface has been specified with
 * <tt>set_initial_face</tt>.
 * <br><br>
 * @param dir
 * normal vector for initial plane.
 */
		void set_plane_normal(const SPAunit_vector& dir);

/**
 * Returns the current value of the plane_normal parameter.
 * <br><br>
 * <b>Role:</b> This method should only be called if a <tt>plane_normal</tt> parameter has
 * been set, i.e. if <tt>input_plane_specified</tt>() returns <tt>TRUE</tt>.
 * <br><br>
 * @see #set_plane_normal
 */
		SPAunit_vector get_plane_normal() const;

/**
 * Returns the number of constraint conflicts found.
 * <br><br>
 * <b>Role:</b> The boundary constraint geometry may inadverntantly conflict with user-input
 * tangency or curvature data. This function returns the number of conflicts found.
 * <br><br>
 * @see #get_conflict
 */
		int get_num_conflicts() const;

/**
 * Returns a description of the n-th constraint conflict.
 * <br><br>
 * <b>Role:</b> The boundary constraint geometry may inadverntantly conflict with user-input
 * tangency or curvature data. This function returns a description of the n-th such conflict.
 * <br><br>
 * @param n
 *   Which conflict to query.
 * @param ed1
 *   First EDGE; conflict occurs at VERTEX joining the two edges.
 * @param ed2
 *   Second EDGE; conflict occurs at VERTEX joining the two edges.
 * @param type
 *   Type of conflict.
 * <br><br>
 * @see #get_num_conflicts
 */
		void get_conflict( int n, EDGE*& ed1, EDGE*& ed2, acovr_conflict_type& type) const;
	
/**
 * Returns whether or not advanced covering checks the correctness of candidate covering surfaces.
 * <br><br>
 * <b>Role:</b> By default, candidate covering surfaces are checked and rejected if bad. If the final_surf_check 
 * flag is false, this checking will not occur. WARNING - if this flag is false, advanced covering can produce bad
 * surfaces, i.e., surfaces which may cause problems during downstream ACIS operations.
 * <br><br>
 * @see #set_final_surf_check
 */
		logical get_final_surf_check() const;
/**
 * Sets whether or not advanced covering checks the correctness of candidate covering surfaces.
 * <br><br>
 * <b>Role:</b> By default, candidate covering surfaces are checked and rejected if bad. If the final_surf_check 
 * flag is false, this checking will not occur. WARNING - after setting this flag is false, advanced covering can produce bad
 * surfaces, i.e., surfaces which may cause problems during downstream ACIS operations. The option is provided mainly as a diagnostic 
 * for cases in which covering fails; users may then loosen tolerances, check input constraints (tangent vs position) for conflicts, or both.
 * <br><br>
 * @param new_final_check
 *   New setting for final check (TRUE or FALSE).
 * <br><br>
 * @see #get_final_surf_check
 */
		void set_final_surf_check(logical new_final_check);

		// for internal use only
/**
 * @nodoc
 */
		adv_cover_options *get_adv_cover_options() const;

/**
 * @nodoc
 */
		void set_use_R10_algorithm(logical use);

/**
 * @nodoc
 */
		logical get_use_R10_algorithm() const;

};

 /** @} */
#endif // AC_INT_OPTIONS_CLASS
