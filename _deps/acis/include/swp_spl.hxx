/*******************************************************************/
/*    Copyright (c) 1989-2020 by Spatial Corp.                     */
/*    All rights reserved.                                         */
/*    Protected by U.S. Patents 5,257,205; 5,351,196; 6,369,815;   */
/*                              5,982,378; 6,462,738; 6,941,251    */
/*    Protected by European Patents 0503642; 69220263.3            */
/*    Protected by Hong Kong Patent 1008101A                       */
/*******************************************************************/
#if !defined( sweep_spl_sur_CLASS )
#define sweep_spl_sur_CLASS

#include "acis.hxx"
#include "bs3curve.hxx"
#include "dcl_kern.h"
#include "debugmsc.hxx"
#include "forgn_app.hxx"
#include "law.hxx"
#include "law_spl.hxx"
#include "logical.h"
#include "position.hxx"
#include "spldef.hxx"
#include "swp_enum.hxx"
class BODY;
class COEDGE;
class intcurve;
class SizeAccumulator;
class SPAbox;
class SPApar_box;
class SPApar_pos;
class SPAposition;
class SPAtransf;
class SPAunit_vector;
class SPAvector;
class surface;
class sweep_spl_sur;
class VOID_LIST;
/*******************************************************************/
/**
* @file swp_spl.hxx
 * @CAA2Level L1
 * @CAA2Usage U1
 * \addtogroup ACISSURFACES
 *
 * @{
 */
/**
 * @nodoc
 * Internal use.
 */
DECL_KERN bs3_surface make_sweep_approx( sweep_spl_sur const &,  double &,
	SPAinterval*uint, SPAinterval*vint, bs3_curve* path_in = NULL,
	logical self_int_test = FALSE, VOID_LIST& bad_uvs = *(VOID_LIST*)NULL_REF,
	SPApar_box& exclude_region = *(SPApar_box*)NULL_REF,
	SPApar_box& extension_box = *(SPApar_box*)NULL_REF );
/**
 * @nodoc
 * Internal use
 */
DECL_KERN void get_geometric_derivatives(SPAvector *para, // Set of derivatives
										  SPAvector *geo,  // Set of geometric derivatives found
										  int size) ;       // Number of derivatives
/**
 * @nodoc
 * Internal use. Restore mechanism.
 */
DECL_KERN subtype_object *restore_sweep_spl_sur();

#if defined D3_STANDALONE || defined D3_DEBUG
/**
 * @nodoc
 */
DECL_KERN D3_istream& operator>>( D3_istream &, spl_sur *& );
/**
 * @nodoc
 */
DECL_KERN D3_ostream& operator<<( D3_ostream &, spl_sur const & );
#endif
/**
 * @nodoc
 * Helix clasification.
 */
enum sweep_helix_type 
{
	SWEEP_IS_HELIX, 
	SWEEP_IS_NOT_HELIX, 
	SWEEP_HELIX_UNKNOWN 
};
/**
 * Defines the perpendicular sweep of a planar profile curve along a path curve.
 * <br>
 * <b>Role:</b> This class defines the perpendicular sweep of a planar profile
 * curve along a path curve.
 * <br><br>
 * The sweep surface equation S<i>(u,v)</i> is defined below:
 * <pre>
 *     S (u,v) = k (u) *M (v)
 * where
 *   - k(u) = w(u) * transpose ( M (start v) )
 *   - p(u) = the position of the profile at u
 *   - w(u) = p(u) - (the start of the sweep path)
 * <br><br>
 *   - M(v) = the 3 x 3 matrix
 *               with rows { mx (v), my (v), mz (v) }
 * where
 * mx (v) = rail_law (v)
 * mz (v) = normalized (sweep path tangent at v)
 * my (v) = mz (v) <i>x</i> mx (v) </pre>
 * M is a frame at v on the sweep path and
 * k is the vector in the starting frame's coordinate system that points from
 * the start of the sweep path to the profile at u.
 * <br><br>
 * When we consider scale, the equation becomes :
 * <pre>
 *      S (u,v) = k (u) *s (v) * M (v)
 * where
 *      s (v) = the 3 x 3 matrix with diagonal 
 *                    equal to { xs (v), ys (v), zs (v) }
 *      and where
 *      "_s" is the scale in the frame's "_" direction </pre>
 * <br><br>
 * When we consider draft, the equation becomes :
 * <pre>
 *      S (u,v) = k (u,v) * s (v) * M (v)
 * where
 *      k (u,v) = p (u,v) - (the start of the sweep path) and
 *      p (u,v) = the u position of the profile offset by the draft_law(v)</pre>
 * <br>
 * Non-perpendicular draft :
 * The %surface draft distance is calculated with respect to the normal 
 * at the evaluating position.
 * 
 * The equation becomes:
 * <pre>
 *      S(u,v) = So(u,v) + No(u,v)*D(v-vo);
 * where
 *      So(u,v) = Surface position without draft.
 *      No(u,v) = Normal at the position u,v without draft.
 *      D(v-vo) = Draf law ant v, provides the offset distance that will produce
 *                the requested draft angle.
 * <b>Limitations:</b>
 * <br><br>
 * - The profile must be planar if draft is used.<br>
 * - The start of the path is in the plane of the shape curve.
 * - Non-perpendicular draft case requires the progenitors (profile and path) to
 *   be at least G2 continuous curves.
 * @see curve, SPAinterval, SPAmatrix, SPAposition, SPAunit_vector , law
 */
class DECL_KERN sweep_spl_sur: public spl_sur {

	// Allow extensions to declare themselves as friends.
#ifdef swp_spl_sur_FRIENDS
       swp_spl_sur_FRIENDS
#endif
#if 0
; // semicolon needed for mkman (doc tool) parsing
#endif
	// STI rr (08/08/2003) 5862 begin: Allow special LOP routine to fix up sweeps in
	// certain conditions. The routine is in SPAlop\lop_husk_tools.m\src\rep_rb.cpp 
	// and the function is fix_swp_spl_sur_surfaces(). The problem is that sometimes
	// the path curve of a sweep surface is a subset_int_cur. These int_curs do not
	// extend properly and it is better to replace such path curves with their
	// progenitor curves as those will extend better. Therefore we need access to
	// the private data of the sweep class and hence we allow that LOP function to 
	// be a friend of the sweep_spl_sur.
#ifdef fix_swp_spl_sur_SURFACES
       fix_swp_spl_sur_SURFACES
#endif

    friend class swp_spl_sur_access;
#if 0
; // semicolon needed for mkman (doc tool) parsing
#endif
	// STI rr end.

    /**
     *  Safe non perpendicular range
     */
	 mutable SPApar_box m_safe_nonperp_range;
protected:
	/**
	 * Info about if this surface is a helix or if it should be determined
	 */
     mutable sweep_helix_type m_is_helix;
	 /**
	 * cached helix exis
	 */
	 mutable SPAvector        m_helix_axis;
	 /**
	 * cached helix root
	 */
	 mutable SPAposition      m_helix_root;
	 /**
	 * cached helix pitch
	 */
	 mutable double           m_helix_pitch;
	 /**
	 * cached helix radius
	 */
	 mutable double           m_helix_radius;
	 /**
	 * cached helix is right handed
	 */
	 mutable logical          m_helix_right_handed;
	 /**
	  * Use old evaluator.
	  * <br><br>
	  * <b>Role<b> : Use evaluator used in ACIS previous to R12. The 
	  * draft with non perpendicular path operation is handled differently.
	  */
	 mutable logical          m_use_old_nonperpdraft;

	 /**
	  * Whether or not the path is a straight line
	  */
     logical m_is_linear_path;
	// Declare the function make_sweep_approx as a friend of the sweep_spl_sur
	// class, as this will enable this function to use the members of
	// this class freely.  If self_int_test is TRUE, self intersection check is
    // done on the surface and NULL is returned if singularities are found.
    // Par_pos of singularities (which must be deleted by the caller)
    // are returned in the bad_uvs list if provided.  An exclude region is an area
    // that is not tested against itself (e.g. in the case of extensions where
    // a region is assumed to be good).  If the exclude_region and extension_box
    // are both supplied, the extension SPAbox contains the exclude_region and no
    // singularities.
	friend DECL_KERN bs3_surface make_sweep_approx(
								sweep_spl_sur const &,
								double &,
								SPAinterval *uint,
								SPAinterval *vint,
								bs3_curve* path_in,
                                logical self_int_test,
                                VOID_LIST& bad_uvs,
                                SPApar_box& exclude_region,
                                SPApar_box& extension_box
							);

/**
 * Switch on the path type.
 */
	logical is_path_normal;
/**
 * Copy of the path curve.
 */
	curve *path_curve;
/**
 * Original path domain.
 */
    SPAinterval  draft_domain;
/**
 * Copy of the profile curve.
 */
	curve *profile_curve;
/**
 * Rail law: vector field for 3D paths or any path.
 */
	law *rail_law;

/**
 * Start of frame: Defined by the path and the rail law.
 * <br><br>
 * <b>Role:</b> x axis is the rail, z axis is the path direction at start,
 * y axis is z cross x
 */
	SPAmatrix start_frame;
/**
 * Transpose of start frame.
 */
	SPAmatrix start_frame_t;

/**
 * Path start position.
 */
	SPAposition path_start;
/*
 * Whether or not the profile is the plane formed by <tt>local_x</tt> 
 * and <tt>local_y</tt>.
 */
	logical sweep_nor;
/**
 * Profile normal.
 */
	SPAunit_vector profile_nor;

/**
 * Rigid sweep. (Curve tangent is constant, defined by m_rigid_tangent_dir.)
 */
	logical        m_rigid;
   SPAunit_vector m_rigid_tangent_dir;

/**
 * Axis sweep. (sweeping while keep planar profile normal perpendicular to the axis)
 */
	logical        m_axis_sweep;
   SPAunit_vector m_axis;

/**
 * Draft distance is defined by a law, and is a function of the length of the path.
 */
	law* draft_law;

/**
 * Function from R to R3 for scale in x,y, and z.
 * <br><br>
 * <b>Role:</b> x is the rail, z is the sweep path and y is z X x.
 */
	law *scale_law;

/**
 * C default constructor.
 * <br><br>
 * <b>Role:</b> Private for internal use only Default constructor, used in to 
 * create a empty class, so that
 * the copy function can use it.
 */
	sweep_spl_sur();
/**
 * Destructor needs to delete the spine curve.
 */
	virtual ~sweep_spl_sur();
/**
 * Construct a duplicate in free store of this object but with zero use count.
 */

	virtual subtrans_object *copy() const;
/**
 * Test for equality.
 * <br><br>
 * <b>Role:</b> This does not guarantee that all
 * effectively equal surfaces are determined to be equal, but
 * does guarantee that different surfaces are correctly
 * identified as such.
 */
	logical operator==( subtype_object const & ) const;
/**
 * Transform this sweep by the given SPAtransf.
 */

	virtual void operator*=( SPAtransf const &	);

	virtual void append_u(spl_sur & back);

	virtual void append_v(spl_sur & back);
/**
 * Parameter shift.
 * <br><br>
 * <b>Role:</b> Adjust the spline surface to have a parameter range increased 
 * by the argument value (which may be negative).
 * This is only used to move portions of a periodic surface by
 * integral multiples of the period, so <tt>"shift_v"</tt> will never be
 * used, and so we can leave it undefined here.
 */

	virtual void shift_u( double );

/**
 * Parameter shift.
 * <br><br>
 * <b>Role:</b> Adjust the spline surface to have a parameter
 * range increased by the argument value (which may be negative).
 * This is only used to move portions of a periodic surface by
 * integral multiples of the period, so <tt>"shift_v"</tt> will never be
 * used, and so we can leave it undefined here.
 */
	virtual void shift_v( double );

/**
 * Divide a surface into two pieces at a given u parameter value.
 * <br><br>
 * <b>Role:</b> If the split is at the end of the parameter range, the <tt>spl_sur</tt>
 * is just returned as the appropriate half (in increasing
 * parameter order), and the other is <tt>NULL</tt>. Otherwise a new <tt>spl_sur</tt>
 * is used for one part, and the old one is modified for the other.
 */

	virtual void split_u(
				double,
				spl_sur *[ 2 ]
			);
/**
 * Divide a surface into two pieces at a given v parameter value.
 * <br><br>
 * <b>Role:</b> If the split is at the end of the parameter range, the <tt>spl_sur</tt>
 * is just returned as the appropriate half (in increasing
 * parameter order), and the other is <tt>NULL</tt>. Otherwise a new <tt>spl_sur</tt>
 * is used for one part, and the old one is modified for the other.
 */
	virtual void split_v(
				double,
				spl_sur *[ 2 ]
			);


	logical generate_guess( SPAposition const& point , SPApar_pos &uv_guess ) const;
	void local_eval_norm_curv(
				SPAposition const & position,
				SPApar_pos const &uv,
				SPAposition &foot,
				SPApar_pos &uv_actual,
				SPAunit_vector &norm = *(SPAunit_vector *)NULL_REF,
				surf_princurv &curv = *(surf_princurv *)NULL_REF
		)const ;

	/**
	 * Find the point on the surface nearest to the given point,
	 * iterating from the given parameter values (if supplied).
	 * Return the found point, the normal to the surface at that
	 * point and the parameter values at the found point.
	 */

	virtual void point_perp(
				SPAposition const &,
				SPAposition &,
				SPAunit_vector &,
				surf_princurv &,
				SPApar_pos const & = *(SPApar_pos *)NULL_REF,
				SPApar_pos & = *(SPApar_pos *)NULL_REF,
				logical f_weak = FALSE,
			    SPApar_box const & = *(SPApar_box *)NULL_REF
			) const;

/**
 * Finds the normal to the surface at a point on the surface nearest to the given point.
 */

	virtual SPAunit_vector point_normal(
				SPAposition const &,
				SPApar_pos const & = *(SPApar_pos *)NULL_REF
			) const;

/**
 * Finds the principal axes of curvature of the surface at a given point, 
 * and the curvatures in those directions.
 */

	virtual void point_prin_curv(
				SPAposition const &,
				SPAunit_vector &,		// first axis direction
				double &,			// curvature in first direction
				SPAunit_vector &,		// second axis direction
				double &,			// curvature in second direction
				SPApar_pos const & = *(SPApar_pos *)NULL_REF,
                evaluate_surface_quadrant = evaluate_surface_unknown
			) const;

/**
 * Finds the parameter values of a point on the surface.
 */

	virtual SPApar_pos param(
				SPAposition const &,
				SPApar_pos const & = *(SPApar_pos *)NULL_REF
			) const;


/**
 * Finds the change in surface parameter corresponding to a unit offset
 * in a given direction at a given uv, the direction lying in the surface.
 */

	virtual SPApar_vec param_unitvec(
				SPAunit_vector const &,
				SPApar_pos const &
			) const;

/**
 * Finds the position and first and second derivatives of the surface 
 * at a given parameter.
 */
	virtual int evaluate(
				SPApar_pos const			&uv,
				SPAposition					&xyz,
				SPAvector					**derivs,
				int							number_of_derivs,
				evaluate_surface_quadrant	quadrant
			) const;

/**
 * Finds the normal to the surface at the point with given parameter values.
 */

	virtual SPAunit_vector eval_normal(
				SPApar_pos const &
			) const;

/**
 * Finds the principal axes of curvature of the surface at a point with given 
 * parameter values, and the curvatures in those directions.
 */

	virtual void eval_prin_curv(
				SPApar_pos const &,
				SPAunit_vector &,		// first axis direction
				double &,				// curvature in first direction
				SPAunit_vector &,		// second axis direction
				double &,				// curvature in second direction
                evaluate_surface_quadrant = evaluate_surface_unknown
			) const;

/**
 * Finds the curvature of a cross-section curve of the surface at the point on 
 * the surface with given parameter values.
 * <br><br>
 * <b>Role:</b> The cross-section curve is defined as the intersection of
 * the surface with a plane passing through the point on the
 * surface and normal to the given direction, which must lie in
 * the surface.
 */
	virtual double eval_cross(
				SPApar_pos const &,
				SPAunit_vector const &
			) const;

/**
 * Tests whether a point lies on the surface.
 */
	virtual logical test_point_tol(
				SPAposition const &,
				double,
				SPApar_pos const & = *(SPApar_pos *)NULL_REF,
				SPApar_pos & = *(SPApar_pos *)NULL_REF
			) const;

/**
 * Makes or remakes the approximating surface.
 * <br><br>
 * <b>Role:</b> This function, which is a virtual function of <tt>spl_sur</tt>,
 * is implemented using <tt>make_sweep_approx</tt>. The function stores the approximating
 * surface and the actual fit error that was achieved in the <tt>spl_sur</tt>,
 * overriding the declared const-ness of the function to do this.
 */
    virtual void	make_approx(
							 double fit,
							 const spline& spl = *(spline*) NULL_REF,
						     logical force = FALSE
							 ) const;

//GSSL VPL Incremental approx project.
	virtual logical incremental_make_approx( double fit);

/**
 * Calculates and adds the dicontinuities from the generating curves to the sweep surface.
 */
	virtual void calculate_disc_info();

	friend DECL_KERN subtype_object *restore_sweep_spl_sur();
	friend subtype_object *restore_helix_spl_sur();

	/**
	* @nodoc
	**/
	void process( geometry_definition_processor& p ) const;
/**
 * Restores <tt>spl_sur</tt> data from file.
 */
	void restore_data();
	void restore_helix_spl_sur_data();

#if defined D3_STANDALONE || defined D3_DEBUG

	friend DECL_KERN D3_istream& operator>>(
				D3_istream &,
				spl_sur *&
			);

	friend DECL_KERN D3_ostream& operator<<(
				D3_ostream &,
				spl_sur const &
			);

	virtual void input(
				D3_istream &
			);

	virtual void print(
				D3_ostream &
			) const;
#endif

	// Debug printout. As for save and restore we split the operation
	// into two parts: the virtual function "debug" prints a class-
	// specific identifying line, then calls the ordinary function
	// "debug_data" to put out the details. It is done this way so that
	// a derived class' debug_data can call its parent's version first,
	// to put out the common data. Indeed, if the derived class has no
	// additional data it need not define its own version of debug_data
	// and use its parent's instead. A string argument provides the
	// introduction to each displayed line after the first, and a
	// logical sets "brief" output (normally removing detailed
	// subsidiary curve and surface definitions).

	virtual void debug(
				char const *,
				logical,
				FILE *
			) const;
	void debug_data(
				char const *,
				logical,
				FILE *
			) const;

/**
 * @nodoc
 * Internal use only.
 */
	virtual logical validate_u_guess( const SPAparameter &u_guess,
									SPAparameter &valid_u_guess ) const;

/**
 * @nodoc
 * Internal use only.
 */
	virtual logical validate_v_guess( const SPAparameter &v_guess,
									SPAparameter &valid_v_guess ) const;

public:
/**
 * C++ initialize constructor requests memory for this object and populates it 
 * with the data supplied as arguments.
 * <br><br>
 * <b>Role:</b> This sweep surface is formed by sweeping the shape along the
 * path curve, with an optional rail curve to specify the twist of the shape
 * curve when it is swept along the path. If the path curve is normal, the
 * path normal must not be NULL. If the path is not planar, a rail curve must
 * be specified.
 * <br><br>
 * @param shape
 * shape curve.
 * @param path
 * path curve.
 * @param rail
 * rail law.
 * @param draft
 * draft law.
 * @param scale
 * scale law.
 * @param u_range
 * shape range.
 * @param v_range
 * path range.
 * @param shape_perp
 * shape profile is perpendicular to path.
 * @param help_profile_bs
 * help profile.
 * @param help_path_bs
 * help path.
 * @param in_profile_nor
 * unit vector normal in shape.
 * @param use_old_nonperpdraft
 * use the evaluation method for non perpendicular draft
 * cases implemented in versions prior to R12
 * @param in_rigid_tangent
 * the curve tangent direction for a rigid sweep where the
 * curve tangent is a constant
 */
	sweep_spl_sur(
			curve const&          shape,
			curve const&          path,
			law*                  rail,
			law*                  draft,
			law*                  scale,
			SPAinterval const&    u_range,
			SPAinterval const&    v_range,
			logical               shape_perp,
			bs3_curve help_profile_bs  = (bs3_curve)NULL,
			bs3_curve help_path_bs  = (bs3_curve)NULL,
			SPAunit_vector const &in_profile_nor = *(const class SPAunit_vector *)NULL_REF,
			logical               use_old_nonperpdraft = FALSE,
         SPAunit_vector const &in_rigid_tangent = *(const class SPAunit_vector *)NULL_REF,
         SPAunit_vector const &in_axis = *(const class SPAunit_vector *)NULL_REF
		);
/**
 * Calculates the derivatives for the surface.
 * <br><br>
 * @param range
 * range for derivs.
 */
	int accurate_derivs(
               SPApar_box const & range = *(SPApar_box *)NULL_REF
           ) const;
/**
 * This function should be used after creating a <tt>sweep_spl_sur</tt>,
 * to ensure the surface will be valid for downstream operations. Check if
 * the approximation is available, if not, constructs one.
 */
	void check_for_approx() const;
/**
 * Creates a copy of an item that does not share any data with the original.
 * <br><br>
 * <b>Role:</b>  Allocates new storage for all member data and any pointers.
 * Returns a pointer to the copied item.
 * <br><br>
 * @param pm
 * list of items within the entity that are already deep copied.
 */
	virtual spl_sur *deep_copy(pointer_map * pm = NULL) const;
/**
 * @nodoc
 * Internal use.
 */ 
	virtual void full_size(SizeAccumulator&, logical = TRUE) const;
/**
 * @nodoc
 */
	virtual void minimize( minimize_helper*);
/**
 * Returns the draft law for the sweep.
 */
    virtual law * get_draft() const;
/**
 * Returns the sweep path curve. The caller has ownership of the returned curve and needs to cleanup to avoid memory leaks.
 */
    virtual curve * get_path() const;
/**
 * Returns the sweep path type.
 */
    virtual sweep_path_type get_path_type() const;
/**
 * Evaluates the curve at the given parameter location. The caller has ownership of the returned curve and needs to cleanup to avoid memory leaks.
 * <br><br>
 * @param param
 * parameter location.
 */
    virtual  curve * get_profile(double param) const;
/**
 * Returns a pointer to the sweep rail law.
 */
    virtual law * get_rail()  const;
/**
 * Returns a pointer to the sweep scale law.
 */
    virtual law * get_scale() const;
/**
 * Returns if the sweep is a helical surface.
 * <br><br>
 * @param owner
 * starting surface.
 * @param axis
 * axis start direction.
 * @param root
 * axis start position.
 * @param pitch
 * thread pitch.
 * @param radius
 * radius of helix.
 * @param right_handed
 * TRUE is right handed.
 */
	logical helix( spline const &owner,
				   SPAvector &axis = *(SPAvector *)NULL_REF,
				   SPAposition &root = *(SPAposition *)NULL_REF,
				   double &pitch = *(double *)NULL_REF,
				   double &radius = *(double *)NULL_REF,
				   logical &right_handed = *(logical *)NULL_REF) const;
/**
 * Returns the ID for the <tt>sweep_spl_sur</tt> list.
 */
	static int id();
/**
 * Checks whether the sweep profile is planar.
 */
	virtual logical planar_profile() const;
/**
 * Checks whether the sweep profile is planar and returns a vector normal to the profile.
 * <br><br>
 * @param norm
 * normal to profile.
 */
	virtual logical planar_profile(SPAunit_vector &norm) const;
/**
 * Returns the sweep profile curve.
 */
	curve const &profile() const;

	// Save and restore. Save is easy, as derived class switching goes
	// through the normal virtual function mechanism. Restore is more
	// complicated, because until it is invoked we do not have an
	// object of the right class. Instead we switch on a table defined
	// by static instances of the restore_ss_def class (see below), to
	// invoke a simple friend function which constructs an object of
	// the right (derived) type. Then it can call the appropriate
	// member function to do the actual work.
	/**
	 * Saves an approximation of the surface, or calls the subtype object's save method.
	 */
	virtual void save() const;
/**
 * Save the information for the <tt>sweep_spl_sur</tt> to a save file.
 */
	virtual void save_data() const;
/**
 * @nodoc
  */
	virtual logical need_save_as_approx(int save_to_version, logical check_progenitors) const;
/**
 * Returns the type <tt>sweep_spl_sur</tt>.
 */
	virtual int type() const;
/**
 * Returns a string <tt>"sweepsur"</tt>.
 */
	virtual char const *type_name() const;

/**
* @nodoc
*/
	virtual logical unbounded_u() const;
/**
* @nodoc
*/
	virtual logical unbounded_v() const;

/**
* Returns the principal parameter range of a parametric surface in both <i>u</i> and <i>v</i>-parameter directions.
* <br><br>
* <b>Role:</b>
* A periodic surface is defined for all parameter values in the periodic direction,
* by reducing the given parameter modulo the period into this principal range.
* For a surface open or nonperiodic in the chosen direction the surface evaluation
* functions are defined only for parameter values in the returned range.
* <br><br>
* If a box is specified, the parameter range returned may be restricted to a portion
* of the surface which is guaranteed to contain all portions of the surface that
* lie within the region of interest. If none is specified and the parameter range
* in some direction is unbounded, then an interval unbounded in that direction is returned.
* <br><br>
* @param box
* object space box.
*/
	SPApar_box param_range(SPAbox const &box = *(SPAbox *)NULL_REF) const;
/**
* Returns the principal parameter range of a parametric surface in the <i>u</i>-parameter direction.
* <br><br>
* <b>Role:</b> A periodic surface is defined for all parameter values in the
* periodic direction, by reducing the given parameter modulo the period into this
* principal range. For a surface open or nonperiodic in the chosen direction the
* surface evaluation functions are defined only for parameter values in the
* returned range.
* <br><br>
* @param box
* object space box.
*/
	SPAinterval param_range_u(SPAbox const & = *(SPAbox *)NULL_REF) const;
/**
* Returns the principal parameter range of a parametric surface in the <i>v</i>-parameter direction.
* <br><br>
* <b>Role:</b> A periodic surface is defined for all parameter values in the
* periodic direction, by reducing the given parameter modulo the period into this
* principal range. For a surface open or nonperiodic in the chosen direction the
* surface evaluation functions are defined only for parameter values in the returned
* range.
* <br><br>
* @param box
* object space box.
*/
	SPAinterval param_range_v(SPAbox const &box = *(SPAbox *)NULL_REF)const;
	//changes end
};

/**
 * @nodoc
 * Internal use.
 */
DECL_KERN int
is_path_planar(
		curve 	    *path,
		SPAunit_vector &normal
	);
/**
 * @nodoc
 * Internal use.
 */
DECL_KERN int
is_path_planar(
		       COEDGE	   *path,
		       SPAunit_vector &normal
	          );
/**
 * @nodoc
 * Internal use.
 * Returns transformation SPAmatrix from a coordinate system to another.
 */

DECL_KERN SPAtransf transf_from_to(SPAposition const&, 
								   SPAvector const&, 
								   SPAvector const&,
								   SPAposition const&, 
								   SPAvector const&, 
								   SPAvector const&);
/**
 * @nodoc
 * Internal use.
 */

DECL_KERN SPAtransf transf_from_to(SPAposition const&, 
								   SPAunit_vector const&, 
								   SPAunit_vector const&,
								   SPAposition const&, 
								   SPAunit_vector const&, 
								   SPAunit_vector const&);


/** @} */
#endif
