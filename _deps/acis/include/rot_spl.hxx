/*******************************************************************/
/*    Copyright (c) 1989-2020 by Spatial Corp.                     */
/*    All rights reserved.                                         */
/*    Protected by U.S. Patents 5,257,205; 5,351,196; 6,369,815;   */
/*                              5,982,378; 6,462,738; 6,941,251    */
/*    Protected by European Patents 0503642; 69220263.3            */
/*    Protected by Hong Kong Patent 1008101A                       */
/*******************************************************************/
//                   ROTATION SPLINE  SURFACE
/*******************************************************************/
#if !defined( rot_spl_sur_CLASS )
#define rot_spl_sur_CLASS
/**
* @file rot_spl.hxx
 * @CAA2Level L1
 * @CAA2Usage U1
 * \addtogroup ACISSURFACES
 *
 * @{
 */
#include <stdio.h>
#include "dcl_kern.h"
#include "interval.hxx"
#include "spldef.hxx"
#include "position.hxx"
#include "unitvec.hxx"
class straight;
class SizeAccumulator;
/**
 * @nodoc
 */
DECL_KERN subtype_object *restore_rot_spl_sur();
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
/*******************************************************************/
/**
 * Represents a surface of rotation.
 * <br>
 * <b>Role:</b> This class represents a surface of rotation. The surface
 * is defined by an axis of rotation and a curve. The curve must not
 * intersect with the axis, except possibly at its ends, and must not
 * be tangential to a circle centered on the axis and perpendicular to
 * it (i.e., at no point on the curve can the tangent direction be the
 * same as that of a circle that is centered on the axis of revolution,
 * perpendicular to it, and through the point). The parameter ranges
 * defining the surface are the <i>u</i>-direction along the curve following
 * its parameterization, and the <i>v</i>-direction clockwise around the axis,
 * with the given curve as the <tt>v=0</tt> parameter line.
 * @see curve, SPAposition, SPAunit_vector
 */
class DECL_KERN rot_spl_sur: public spl_sur
{

#ifdef rot_spl_sur_FRIENDS
       rot_spl_sur_FRIENDS
#endif
#if 0
; // semicolon needed for mkman (doc tool) parsing
#endif
public:

   /**
 	* C++ initialize constructor requests memory for this object and populates it with the data supplied as arguments.
 	* <br><br>
 	* <b>Role:</b> The <i>u</i>-parameter range defaults to the full given curve, or it
 	* returns an error if the curve is unbounded. The <i>v</i>-parameter range defaults
 	* to a full circle, 0 to <tt>2pi</tt>.
 	* <br><br>
 	* @param cur
 	* given curve.
 	* @param axis_pt
 	* axis point.
 	* @param axis_dir
 	* axis direction.
 	* @param uparam
 	* u-parameter range. If NULL, defaults curve to full curve, which must not be unbounded.
 	* @param vparam
 	* v-parameter range. If NULL, defaults path to full circle (0 - 2PI).
 	*/
	rot_spl_sur(
				curve const &cur,
				SPAposition const &axis_pt,
				SPAunit_vector const &axis_dir,
				SPAinterval const &uparam = *(const class SPAinterval *)NULL_REF,
				SPAinterval const &vparam = *(const class SPAinterval *)NULL_REF
			);

   /**
 	* C++ copy constructor requests memory for this object and populates it with the data from the object supplied as an argument.
 	* <br><br>
 	* @param sur
 	* rot_spl_sur.
 	*/
	rot_spl_sur( const rot_spl_sur& sur);
   /**
 	* Axis of rotation.
 	*/
	SPAunit_vector axis_dir;
   /**
    * Base of axis of rotation.
    */
	SPAposition axis_root;
   /**
    * Profile curve.
    */
	curve* cur;
	/**
	 * Creates a copy of an item that does not share any data with the original.
	 * <br><br>
	 * <b>Role:</b> Allocates new storage for all member data and any pointers.
	 * Returns a pointer to the copied item.
	 * <br><br>
	 * @param pm
	 * list of items within the entity that are already deep copied.
	 */
	virtual spl_sur *deep_copy(pointer_map * pm = NULL) const;

	/**
	 * Internal usage.
	 */
	virtual void full_size(SizeAccumulator&, logical = TRUE) const;
/**
 * @nodoc
 */
	virtual void minimize( minimize_helper*);
	/**
	 * Returns the path used for the rotation. The caller has ownership of the returned curve and needs to cleanup to avoid memory leaks.
	 */
    virtual curve* get_path() const;
	/**
	 * Returns the profile used for the rotation.  The caller has ownership of the returned curve and needs to cleanup to avoid memory leaks.
	 * <br><br>
	 * @param param
	 * not used.
	 */
    virtual curve * get_profile(double param) const;
	/**
	 * Returns the rail used for profile orientation in the rotation operation.
	 */
    virtual law * get_rail() const;
	/**
	 * Returns the ID for the <tt>rot_spl_sur</tt> list.
	 */
	static int id();
	/**
	 * Returns the integer representing the type of this <tt>spl_sur</tt>.
	 */
	virtual int type() const;
	/**
	 * Returns the type of <tt>rotsur</tt>.
	 */
	virtual char const *type_name() const;
	/**
	 * Saves the information for a <tt>rot_spl_sur</tt> to a save file.
	 */
	virtual void save_data() const;
	/**
	 * @nodoc
	 */
	virtual logical need_save_as_approx(int save_to_version, logical check_progenitors) const;
	/**
	 * Returns the path type used used for the rotation operation.
	 */
    virtual sweep_path_type get_path_type() const;
	/**
	 * @nodoc
	 */
	virtual logical unbounded_u() const;
protected:

	/**
	 * Makes or remakes the approximating surface.
	 * <br><br>
	 * <b>Role:</b> The force flag forces the
	 * approximating surface to be made even if it is illegal.  This can be
	 * used to restore old parts that were not checked properly before being
	 * saved.  The spline argument 'spl' may be null but if it is supplied the
	 * function may be a little faster.  The function stores the approximating
     * surface and the actual fit error that was achieved in the <tt>spl_sur</tt>,
	 * overriding the declared const-ness of the function to do this.
	 */
    virtual void	make_approx(
							 double fit,
							 const spline& spl  = *(spline*) NULL_REF,
						     logical force = FALSE
							 ) const;

	// For internal use only.
/**
 * @nodoc
 */
	virtual logical validate_u_guess( const SPAparameter &u_guess,
									SPAparameter &valid_u_guess ) const;
private:
	/**
	 * Default constructor.
	 */
	 rot_spl_sur();

    /**
	 * Calculate the discontinuity information from the defining curve.
	 */
    virtual	void 	calculate_disc_info();
	/**
    * @nodoc
    */
	SPApar_pos improve_guess( SPAposition const& point, SPApar_pos const& uv_guess) const;
	SPApar_pos improve_guess_unbounded( SPAposition const& point, SPApar_pos const& uv_guess) const;

	/**
	 * Destroy the curve. The parent destructor handles the spline
	 * approximation.
	 */
	virtual ~rot_spl_sur();

	/**
	 * Duplication. Can't be done by constructor, as we want it
	 * to be virtual.
	 */
	virtual subtrans_object *copy() const;
	/**
     * Test for equality. This does not guarantee that all
	 * effectively equal surfaces are determined to be equal, but
	 * does guarantee that different surfaces are correctly
	 * identified as such.
	 */
	logical operator==( subtype_object const & ) const;
	/**
     * Transformation. The base class transforms the spline and fit
	 * tolerance: we must handle the curve and sweep vector.
	 */
	virtual void operator*=( SPAtransf const & );

	/**
	 * Divide a surface into two pieces at a given parameter value.
	 * If the split is at the end of the parameter range, the spl_sur
	 * is just returned as the appropriate half (in increasing
	 * parameter order), and the other is <tt>NULL</tt>. Otherwise a new spl_sur
	 * is used for one part, and the old one is modified for the other.
	 */

	virtual void split_u(
				double,
				spl_sur *[ 2 ]
			);
    virtual void split_v(
				double,
				spl_sur *[ 2 ]
			);

	/**
     * Divide a surface into separate pieces which are smooth (and therefore
     * suitable for offsetting or blending). The surface is split at its non-G1
     * discontinuities, and if it is closed after this, it is then split into
     * two. The functions return the number of pieces, and the pieces themselves
     * are a return argument.
	 */
    int split_at_kinks( spl_sur**& pieces, logical udir,double curvature = 0.0 ) const;

	// Geometric evaluation.

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

	void local_eval_norm_curv(
				SPAposition const & position,
				SPApar_pos const &uv,
				SPAposition &foot,
				SPApar_pos &uv_actual,
				SPAunit_vector &norm = *(SPAunit_vector *)NULL_REF,
				surf_princurv &curv = *(surf_princurv *)NULL_REF
		)const ;
	/**
	 * Find the parameter values of a point on the surface.
	 */
	virtual SPApar_pos param(
				SPAposition const &,
				SPApar_pos const & = *(SPApar_pos *)NULL_REF
			) const;

	virtual SPAbox bound(
			  SPApar_box const & = *(SPApar_box *)NULL_REF
			  );

	/**
	 * Return a cone bounding the normal direction of the surface.
	 */

	virtual surf_normcone normal_cone(
							  SPApar_box const & = *(SPApar_box *)NULL_REF,
							  logical = FALSE
							  );

	/**
	 * Find an outward direction from the surface at a point with
	 * given parameter values.
	 */
	virtual SPAunit_vector eval_outdir(
				SPApar_pos const &
			) const;


	/**
	 * Find the principal axes of curvature of the surface at a
	 * point with given parameter values, and the curvatures in those
	 * directions.
	 */
	virtual void eval_prin_curv(
				SPApar_pos const &,
				SPAunit_vector &,	// first axis direction
				double &,			// curvature in first direction
				SPAunit_vector &,	// second axis direction
				double &,			// curvature in second direction
                evaluate_surface_quadrant = evaluate_surface_unknown
			) const;

	/**
     * The evaluate() function calculates derivatives, of any order
	 * up to the number requested, and stores them in vectors provided
	 * by the user. It returns the number it was able to calculate;
	 * this will be equal to the number requested in all but the most
	 * exceptional circumstances. A certain number will be evaluated
	 * directly and (more or less) accurately; higher derivatives will
	 * be automatically calculated by finite differencing; the accuracy
	 * of these decreases with the order of the derivative, as the
	 * cost increases.
	 */

	virtual int evaluate(
                SPApar_pos const &,	// Parameter
                SPAposition &,			// Point on surface at given parameter
                SPAvector ** = NULL, 	// Array of pointers to arrays of
									// vectors, of size nd. Any of the
									// pointers may be null, in which
									// case the corresponding derivatives
									// will not be returned. Otherwise
									// they must point to arrays long
									// enough for all the derivatives of
									// that order - i.e. 2 for the first
									// derivatives, 3 for the second, etc.
                int = 0,       		// Number of derivatives required (nd)
				evaluate_surface_quadrant = evaluate_surface_unknown
									// the evaluation location - above,
									// below for each parameter direction,
									// or don't care.
            ) const;

   /**
	* The evaluate_iter() function calculates derivatives, of any order
	* up to the number requested, and stores them in vectors provided
	* by the user. It returns the number it was able to calculate;
	* this will be equal to the number requested in all but the most
	* exceptional circumstances. A certain number will be evaluated
	* directly and (more or less) accurately; higher derivatives will
	* be automatically calculated by finite differencing; the accuracy
	* of these decreases with the order of the derivative, as the
	* cost increases.
    */
	virtual int evaluate_iter(
                SPApar_pos const &,	// Parameter
				surface_evaldata *,	// Initialisation data for iterations.
                SPAposition &,			// Point on surface at given parameter
                SPAvector ** = NULL, 	// Array of pointers to arrays of
									// vectors, of size nd. Any of the
									// pointers may be null, in which
									// case the corresponding derivatives
									// will not be returned. Otherwise
									// they must point to arrays long
									// enough for all the derivatives of
									// that order - i.e. 2 for the first
									// derivatives, 3 for the second, etc.
                int = 0,       		// Number of derivatives required (nd)
				evaluate_surface_quadrant = evaluate_surface_unknown
									// the evaluation location - above,
									// below for each parameter direction,
									// or don't care.
            ) const;

   /**
	* Construct a data object to retain evaluation information across
	* calls to evaluate_iter(). This is to allow subsidiary calls
	* within an iterative evaluator to start iteration much closer to the
	* required result than is possible just using the surface information
	* itself.
	*/
	virtual surface_evaldata *make_evaldata() const;

   /**
	* Return the number of derivatives which evaluate() can find
	* "accurately" (and fairly directly), rather than by finite
	* differencing, over the given portion of the surface. If there
	* is no limit to the number of accurate derivatives, returns
	* the value ALL_SURFACE_DERIVATIVES, which is large enough to be
	* more than anyone could reasonably want.
	*/

	virtual int accurate_derivs(
				SPApar_box const & = *(SPApar_box *)NULL_REF
								 	// Defaults to the whole surface
			) const;

   /**
    * Get the surface closure properties from the curve and ranges, and
    * work out, by evaluating the curve, whether the surface is singular.
    * This function sets the closure and singularity flags.  Note that there
    * is an interaction between singularity, namely that we do not allow
    * surfaces to be both periodic and singular in u, which is why we sort
    * everything out in a single function.
	*/
    void	determine_closure_and_singularity();


	// Construct a parameter line on the surface. A u parameter line
	// runs in the direction of increasing u parameter, at constant v.
	// A v parameter line runs in the direction of increasing v, at
	// constant u. The parametrisation in the non-constant direction
	// matches that of the surface, and has the range obtained by
	// use of param_range_u() or param_range_v() appropriately.

	// The new curve is constructed in free store, so it is the
	// responsibility of the caller to ensure that it is correctly
	// deleted.

	virtual curve *u_param_line(
				double,			// constant v parameter
				spline const &	// owning surface
			) const;
	virtual curve *v_param_line(
				double,			// constant u parameter
				spline const &	// owning surface
			) const;


	friend DECL_KERN subtype_object *restore_rot_spl_sur();
	void restore_data();

	/**
	* @nodoc
	**/
	virtual void process( geometry_definition_processor& p ) const;



	// Check for any data errors in the surface, and correct the errors if
	// possible.  The various arguments provide control over which checks
	// are made, which fixes can be applied and which fixes were actually
	// applied.  The function returns a list of errors that remain in the
	// curve on exit.
	// See chk_stat.hxx for information on the argument types used here.
	// At the moment we only fix non-G1 discontinuities by fixing
	// them on the defining curve.

	virtual	check_status_list*	check(
  		        const check_fix& input = *(const check_fix*) NULL_REF,
						 // supplies a set of flags which say which fixes
						 // are allowable (the default is to fix nothing)
				check_fix& result = *(check_fix*) NULL_REF,
						 // returns a set of flags which say which fixes
						 // were applied
				const check_status_list* = (const check_status_list*) NULL_REF
						 // list of checks that are to be made.  If the
						 // list is null, then every possible check will
						 // be made; otherwise, the function will only
						 // check for things in the list.  The return
						 // value for the function will then be a subset
						 // of this list.
				);

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
	mutable bool inside_pointperp ;
};

/** @} */
#endif
