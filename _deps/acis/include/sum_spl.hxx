/*******************************************************************/
/*    Copyright (c) 1989-2020 by Spatial Corp.                     */
/*    All rights reserved.                                         */
/*    Protected by U.S. Patents 5,257,205; 5,351,196; 6,369,815;   */
/*                              5,982,378; 6,462,738; 6,941,251    */
/*    Protected by European Patents 0503642; 69220263.3            */
/*    Protected by Hong Kong Patent 1008101A                       */
/*******************************************************************/
//                   SUM SPLINE SURFACE
/*******************************************************************/
#if !defined( sum_spl_sur_CLASS )
#define sum_spl_sur_CLASS

#include <stdio.h>
#include "dcl_kern.h"
#include "interval.hxx"
#include "spldef.hxx"
#include "position.hxx"
class SizeAccumulator;
/**
* @file sum_spl.hxx
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
DECL_KERN subtype_object *restore_sum_spl_sur();
/**
 * @nodoc
 * Internal use.
 */
class SURFACE_CURVATURE_FUNCTION;
#if defined D3_STANDALONE || defined D3_DEBUG

/**
 * @nodoc
 * Internal use.
 */
DECL_KERN D3_istream& operator>>( D3_istream &, spl_sur *& );
/**
 * @nodoc
 * Internal use.
 */
DECL_KERN D3_ostream& operator<<( D3_ostream &, spl_sur const & );
#endif

/**
 * Represents a surface built by the linear summation of two curves.
 * <br>
 * <b>Role:</b> This class represents a surface that is a linear sum 
 * of two curves. This is derived from the class <tt>spl_sur</tt>, which 
 * is used by the spline surface class to contain the surface descriptions. 
 * The surface is defined primarily by two curves that are assumed not
 * parallel, and the parameter ranges over which the surface is defined.
 * <br><br>
 * <b>Parametric Representation</b>
 * <br><br>
 * If the curves are represented as:
 * <pre>
 * x = c1(t) and y = c2(t)
 * </pre>
 * The surface is defined as:
 * <pre>
 * S(u, v) = c1(u) + c2(v) - p
 * </pre>
 * where <i>p</i> is a constant position, normally initialized to be the value of
 * <i>>c2(vo)</i>(at the start of the parameter range).
 * @see curve, SPAposition
 */
class DECL_KERN sum_spl_sur: public spl_sur 
{

#ifdef sum_spl_sur_FRIENDS
   sum_spl_sur_FRIENDS
#endif
	friend class sum_spl_sur_access;
#if 0
; // semicolon needed for mkman (doc tool) parsing
#endif
public:
	/**
	 * C++ initialize constructor requests memory for this object and populates it 
	 * with the data supplied as arguments.
	 * <br><br>
	 * <b>Role:</b> The <i>u</i>-parameter range defaults to the full first curve 
	 * and the <i>v</i>-parameter range defaults to the full second curve. If either
	 * curve is unbounded, this constructor returns an error. The datum position 
	 * is subtracted from the sum of the curves to give a position for the surface 
	 * and is normally initialized to be the start of <tt>cur2</tt>.
	 * <br><br>
	 * @param cur1
	 * 1st curve.
	 * @param cur2
	 * 2nd curve.
	 * @param range1
	 * 1st curve param range (u curve).
	 * @param range2
	 * 2nd curve param range (v curve).
	 * @param pos
	 * datum position.
	 */
	sum_spl_sur(
				curve const &cur1,
				curve const &cur2,
				SPAinterval const &range1 = *(const class SPAinterval *)NULL_REF,
				SPAinterval const &range2 = *(const class SPAinterval *)NULL_REF,
				SPAposition const &pos = *(const class SPAposition *)NULL_REF
			);

	/**
	 * C++ copy constructor requests memory for this object and populates it with 
	 * the data from the object supplied as an argument.
	 * <br><br>
	 * @param cur
	 * input curve.
	 */
	sum_spl_sur( const sum_spl_sur& cur);

	/**
	 * Creates a copy of an item that does not share any data with the original.
	 * <br><br>
	 * <b>Role:</b> Allocates new storage for all member data and any pointers.
	 * Returns a pointer to the copied item.
	 * <br><br>
	 * @param pm
	 * list of items within the entity that are already deep copied.
	 */
	virtual spl_sur* deep_copy(pointer_map * pm = NULL) const;

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
	 * Returns the sweep path curve. The caller has ownership of the returned curve and needs to cleanup to avoid memory leaks.
	 */
	virtual curve * get_path() const;
	/**
	 * Returns the sweep path type.
	 */
	virtual sweep_path_type get_path_type() const;
	/**
	 * Returns the sweep profile on the <tt>sum_spl_sur</tt>, which is the <i>u</i>-curve. The caller has ownership of the returned curve and needs to cleanup to avoid memory leaks.
	 * <br><br>
	 * @param param
	 * param.
	 */
	virtual curve* get_profile(double param/*param*/) const;
	/**
	 * Returns the rail law for the <tt>sum_spl_sur</tt>.
	 * <br><br>
	 * @see sweep_options#set_rail_law
	 */
	virtual law* get_rail() const;
	/**
	 * Returns the ID for the <tt>sum_spl_sur</tt> list.
	 */
	static int id();
	/**
	 * Returns whether or not the <tt>sum_spl_sur</tt> can be extended to the point 
	 * where it can include the given <tt>sum_spl_sur</tt>. <br><br>
	 * <b>Role:</b> Returns whether or not the <tt>sum_spl_sur</tt> can be extended 
	 * to the point where it can include the given <tt>sum_spl_sur</tt>. It also 
	 * returns what the new domain of the "this" surface should be to include the
	 * other surface. This method returns false in any case that is not covered.
	 * <br><br>
	 * @param other
	 * other surface.
	 * @param new_domain
	 * new domain.
	 */
	virtual logical is_extendable(sum_spl_sur const &other,
		                          SPApar_box &new_domain) const;
	/**
	 * Save the information for the <tt>sum_spl_sur</tt> to a save file.
	 */
	virtual void save_data() const;
	/**
	 * @nodoc
	 */
	virtual logical need_save_as_approx(int save_to_version, logical check_progenitors) const;
	/**
	 * Returns the type of <tt>sum_spl_sur</tt>.
	 */
	virtual int type() const;
	/**
	 * Returns the string <tt>"sumsur"</tt>.
	 */
	virtual char const *type_name() const;

	/**
	* @nodoc
	**/
	virtual void process( geometry_definition_processor& p ) const;
protected:
	/**
	* Makes or remakes the approximating surface.
	* <br><br>
	* <b>Role:</b> The force flag forces the approximating surface to be made even if it is illegal.  This can be
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


	/**
	* Returns the object-space bounding box for a subset of the surface.
	* <br><br>
	* <b>Role:</b> The default version uses the object-space spline, expanding
	* its box by the fit tolerance. Note that this cannot be const,
	* as it uses <tt>subset()</tt>, which is itself not const.
	*/
	virtual SPAbox bound(
				SPApar_box const & = *(SPApar_box *)NULL_REF
			);

	// For internal use only.
/**
 * @nodoc
 */
	virtual logical validate_u_guess( const SPAparameter &u_guess, 
									SPAparameter &valid_u_guess ) const;

	// For internal use only.
/**
 * @nodoc
 */
	virtual logical validate_v_guess( const SPAparameter &v_guess,
									SPAparameter &valid_v_guess ) const;

private:
	/**
	 * curves to be summed. <i>u</i> direction
	 */
	curve* cur1;
   /**
	 * curves to be summed. <i>v</i> direction
	 */
	curve* cur2;
   /**
     * Datum SPAposition subtracted from the sum of the curves to give a SPAposition
     * for the surface. Normally initialised
     * to be the start of <tt>cur2</tt>.
	 */
	SPAposition datum;

   /**
	 * Default C constructor. Generally null object, preparatory to filling it in.
	 */
	sum_spl_sur();

   /**
    * Calculates the discontinuity information from the defining curves.
	*/
    virtual	void 	calculate_disc_info();

   /**
     * C Destructor. Destroys the curves. The parent destructor handles the spline approximation.
	 */
	virtual ~sum_spl_sur();

	/**
	 * Duplication. Can't be done by constructor, as we want it to be virtual.
	 */
	virtual subtrans_object* copy() const;


	/**
	 * Test for equality. This does not guarantee that all effectively equal surfaces are determined to be equal, but does guarantee that different surfaces are correctly identified as such.
	 */
	logical operator==( subtype_object const & ) const;

	/**
	 * Is this surface contained.
	 */
	logical operator>>( subtype_object const & ) const;

	/**
	 * Transformation. The base class transforms the spline and fit tolerance: we must handle the curve and sweep <tt>SPAvector</tt>.
     */
	virtual void operator*=( SPAtransf const & );

	/**
	 * Divide a surface into two pieces at a given u parameter.
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
	 * Divide a surface into two pieces at a given <i>v</i> parameter.
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

   /**
    * Divide a surface into separate pieces which are smooth (and therefore suitable for offsetting or blending).
    * <br><br>
    * <b>Role:</b> The surface is split at its non-G1 discontinuities, and if it is closed after this, it is then split into
    * two. The functions return the number of pieces, and the pieces themselves
    * are a return argument.
	*/
    int split_at_kinks( spl_sur**& pieces, logical udir,
				double curvature = 0.0 ) const;
	/**
	 * Finds the parameter values of a point on the surface.
	 */
	virtual SPApar_pos param(
				SPAposition const &,
				SPApar_pos const & = *(SPApar_pos *)NULL_REF
			) const;

	/**
	 * Finds the position and first and second  derivatives of the surface at a given point.
	 */
	virtual void eval(
				SPApar_pos const &uv,
				SPAposition &pos,
				SPAvector *dpos,	// first derivatives - array of
								// length 2 in order xu, xv
				SPAvector *ddpos	// second derivatives - array of
								// length 3 in order xuu, xuv, xvv
			) const;

	/**
	 * Find the principal axes of curvature of the surface at a point with given <tt>SPAparameter</tt> values, and the curvatures in those directions.
	 */
	virtual void eval_prin_curv(
				SPApar_pos const &,
				SPAunit_vector &,		// first axis direction
				double &,			// curvature in first direction
				SPAunit_vector &,		// second axis direction
				double &,			// curvature in second direction
                evaluate_surface_quadrant = evaluate_surface_unknown
			) const;

	/**
	* Calculates derivatives, of any order up to the number requested, and stores them in vectors provided by the user.
	* <br><br>
	* <b>Role:</b> It returns the number it was able to calculate;
	* this will be equal to the number requested in all but the most
	* exceptional circumstances. A certain number will be evaluated
	* directly and (more or less) accurately; higher derivatives will
	* be automatically calculated by finite differencing; the accuracy
	* of these decreases with the order of the derivative, as the
	* cost increases.
	*/

	virtual int evaluate(
                SPApar_pos const &,	// Parameter
                SPAposition &,			// Point on surface at given SPAparameter
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
									// below for each SPAparameter direction,
									// or don't care.
            ) const;


	/**
	* Calculates derivatives, of any order up to the number requested, and stores them in vectors provided by the user.
	* <br><br>
	* <b>Role:</b> It returns the number it was able to calculate;
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
                SPAposition &,			// Point on surface at given SPAparameter
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
									// below for each SPAparameter direction,
									// or don't care.
            ) const;

	/**
	 * Construct a data object to retain evaluation information across calls to <tt>evaluate_iter()</tt>.
	 * <br><br>
	 * <b>Role:</b> This is to allow subsidiary calls
	 * within an iterative evaluator to start iteration much closer to the
	 * required result than is possible just using the surface information
	 * itself.
	 */
	virtual surface_evaldata *make_evaldata() const;

  /**
	* Returns the number of derivatives.
	* <br><br>
	* <b>Role:</b> Returns the number of derivatives which <tt>evaluate()</tt> can find
	* "accurately" (and fairly directly), rather than by finite
	* differencing, over the given portion of the surface. If there
	* is no limit to the number of accurate derivatives, returns
	* the value <tt>ALL_SURFACE_DERIVATIVES</tt>, which is large enough to be
	* more than anyone could reasonably want.
	*/
	virtual int accurate_derivs(
				SPApar_box const & = *(SPApar_box *)NULL_REF
								 	// Defaults to the whole surface
			) const;

    /**
	* Constructs a parameter line on the surface. A u parameter line
	* runs in the direction of increasing u parameter, at constant <i>v</i>.
	* <br><br>
    * <b>Role:</b> The parametrisation in the non-constant direction
	* matches that of the surface, and has the range obtained by
	* use of <tt>param_range_u()</tt> or <tt>param_range_v()</tt> appropriately.
	* The new curve is constructed in free store, so it is the
	* responsibility of the caller to ensure that it is correctly
	* deleted.
	*/
	virtual curve *u_param_line(
				double,			// constant v SPAparameter
				spline const &	// owning surface
			) const;
    /**
	* Constructs a parameter line on the surface.
	* <br><br>
	* <b>Role:</b> A <i>v</i> parameter line runs in the direction of increasing <i>v</i> parameter, at constant <i>u</i>.
    * The parametrisation in the non-constant direction matches that of the surface, and has the range obtained by
	* use of <tt>param_range_u()</tt> or <tt>param_range_v()</tt> appropriately.
	* The new curve is constructed in free store, so it is the
	* responsibility of the caller to ensure that it is correctly deleted.
	*/
	virtual curve *v_param_line(
				double,			// constant u SPAparameter
				spline const &	// owning surface
			) const;


    /**
	* @nodoc
	* Common routine used by u_param_line and v_param_line.
	*/
	curve *iso_param_line( 
		bool,				//u_param_line if true, v_param_line if false
		double				//SPAparameter
		) const;

	// Test whether a point lies on the surface, within a user-defined
	// tolerance. The base class version is probably the best we can
	// do.

	//	virtual logical test_point_tol(
	//				SPAposition const &,
	//				double,
	//				SPApar_pos const & = *(SPApar_pos *)NULL_REF,
	//				SPApar_pos & = *(SPApar_pos *)NULL_REF
	//			) const;

	/**
	* @nodoc
	* Internal use.
	*/
	friend class SURFACE_CURVATURE_FUNCTION;

	/**
	* @nodoc
	* Internal use.
	*/
	int surface_curvature( double k, SPAinterval*& spans,  logical udir ) const;

	friend DECL_KERN subtype_object *restore_sum_spl_sur();
	friend subtype_object *restore_cyl_spl_sur();

	// Save and restore. Save is easy, as derived class switching goes
	// through the normal virtual function mechanism. Restore is more
	// complicated, because until it is invoked we do not have an
	// object of the right class. Instead we switch on a table defined
	// by static instances of the restore_ss_def class (see below), to
	// invoke a simple friend function which constructs an object of
	// the right (derived) type. Then it can call the appropriate
	// member function to do the actual work.

	void restore_data();
	void restore_cyl_sur_data();

   /**
	* Check for any data errors in the surface, and correct the errors if possible.
	* <br><br>
	* <b>Role:</b> The various arguments provide control over which checks
	* are made, which fixes can be applied and which fixes were actually
	* applied.  The function returns a list of errors that remain in the
	* surface on exit.
	* See <tt>chk_stat.hxx</tt> for information on the argument types used here.
	* At the moment we only fix non-G1 discontinuities by fixing
	* them on the defining curves.
	*/
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


};

/** @} */
#endif
