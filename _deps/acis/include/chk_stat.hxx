/*******************************************************************/
/*    Copyright (c) 1989-2020 by Spatial Corp.                     */
/*    All rights reserved.                                         */
/*    Protected by U.S. Patents 5,257,205; 5,351,196; 6,369,815;   */
/*                              5,982,378; 6,462,738; 6,941,251    */
/*    Protected by European Patents 0503642; 69220263.3            */
/*    Protected by Hong Kong Patent 1008101A                       */
/*******************************************************************/
//			Curve and Surface Legality Checker Results

#ifndef CHK_STAT_HXX
#define CHK_STAT_HXX
#include "dcl_kern.h"
#include "base.hxx"
#include "bsf.hxx"
#include "bs3surf.hxx"
#include "bs3curve.hxx"
#include "vers.hxx"        // GET_ALGORITHMIC_VERSION

class sf_clash_list;
class intcurve;
/*******************************************************************/
/**
 * \defgroup ACISCHECKING Entity Checking
 * \brief Check ENTITY for insanities
 * \ingroup MODELANALYSIS
 *
 */
/**
 * @file chk_stat.hxx
 * @CAA2Level L1
 * @CAA2Usage U1
 * \addtogroup ACISCHECKING
 *
 * @{
 */
/**
 * Curve and surface check codes.
 * <br>
 * @param check_irregular
 * Curve/surface  is  very scrunched up/twisted. (Subdivision fails.)
 * @param check_undefined
 * A vital part of the data is missing.
 * @param check_inconsistent
 * Data is conflicting.
 * @param check_self_intersects
 * Curve/surface self intersects.
 * @param check_bad_closure
 * The  curve/surface  is marked as closed and in reality is open, or
 * vice versa.
 * @param check_bs3_null
 * Curve/surface does not contain a bs3_curve/surface
 * @param check_bs3_coi_verts
 * Adjacent control vertices are coincident.
 * @param check_bs3_bad_singularity
 * Control vertices are not coincident at a singularity.
 * @param check_bad_degeneracies
 * The degenerate edges on a surface make it untreatable.
 * @param check_untreatable_singularity.
 * This code is not returned for curves. A singularity on the surface
 * is beyond the scope of the  singularity currently processing. This
 * code is not returned for curves.
 * @param check_non_G0
 * Curve/surface is not G0 continuous.
 * @param check_non_G1
 * Curve/surface is not G1 continuous. Surfaces are still G0.
 * @param check_non_G2
 * Curve/surface is not G2 continuous, though it is G0 and G1.
 * @param check_non_C1
 * Curve/surface is not C1 continous, though is G1.
 * @param check_bad_vertex_blend
 * The boundaries of a vertex blend are illegal.
 * @param check_null_curve
 * The curve underlying the COEDGE's EDGE is NULL.
 * @param check_long_enough
 * The @href pcurve is not long enough to describe the EDGE.
 * @param check_within_face_range
 * Part or all of the @href pcurve lies outside of the FACE range  on
 * the surface.
 * @param check_knots_on_curve
 * Some of the @href pcurve knots do not lie on the  true @href curve
 * to within SPAresabs.
 * @param check_tangents_at_knots
 * The  @href pcurve  tangent  vectors  at  the  knots are not within
 * SPAresnor of the curve tangents.
 * @param check_hull_angles
 * The hull turns sharply, possibly due to kinks.
 * @param check_not_on_surfs
 * Curve  does  not  lie  on  one or more of its defining surfaces to
 * within SPAresabs.
 * @param check_bad_approximation
 * The surface approximation is not within tolerance of the surface.
 * @param check_evaluation_failure
 * A curve evaluation failed.
 * @param check_subset_range
 * Subset range is inconsistent.
 * @param check_missing_disc_info
 * Some discontinuities are not marked.
 * @param check_missing_g1_disc_info
 * Some G1 discontinuities are not marked.
 * @param check_missing_g2_disc_info
 * Some G2 discontinuities are not marked.
 * @param check_bad_rb_blend
 * A rolling-ball blend is illegal.
 * @param check_overclamped
 * The approximation has knots with multiplicity greater than its degree.
 * @param check_bad_fitol
 * The fit tolerance of the curve or surface is too small.
 * @param check_surf_illegal
 * The surface is illegal and cannot be evaluated.
 * @param check_exact_sur_bad_fitol
 * The approximating surface tolerance is non-zero on exact spline surface.
 * @param check_unknown
 * Status is unknown.
 * @see check_status_list
 */
enum check_status
{
    check_irregular,
    check_undefined,
    check_inconsistent,
    check_self_intersects,
    check_bad_closure,
    check_bs3_null,
    check_bs3_coi_verts,
	check_bs3_bad_singularity,
    check_bad_degeneracies,
    check_untreatable_singularity,
    check_non_G0,
    check_non_G1,
    check_non_G2,
    check_non_C1,
    check_bad_vertex_blend,
    check_null_curve,
    check_long_enough,
    check_within_face_range,
    check_knots_on_curve,
    check_tangents_at_knots,
    check_hull_angles,
    check_not_on_surfs,
    check_bad_approximation,
    check_evaluation_failure,
    check_subset_range,
    check_missing_disc_info,
	check_missing_g1_disc_info,
	check_missing_g2_disc_info,
	check_bad_rb_blend,
	check_overclamped,
	check_bad_fitol,
	check_surf_illegal,
	check_exact_sur_bad_fitol,
    check_unknown // This one should remain last:
    };

/**
 * Enumeration of the different check details types.
 * <br>
 * @param high_curvature_details
 * Details of a point or region of high curvature.
 * @param bad_fitol_details
 * Details of a fit tolerance found to be smaller than the
 * maximum gap between an object and its approximation.
 * @param self_intersection_details
 * Details of a point of self intersection.
 */
enum check_details_type
{
	high_curvature_details,
	bad_fitol_details,
	self_intersection_details,
	cur_self_int_details
};

/** 
 * A class for returning details of a check status error.
 * <br>
 * <b>Role:</b> An instance of this class can optionally be
 * added to a check_status_list instance when it is created.
 * The check_status_details gives more detailed information about
 * the circumstances of an error e.g. a particular parameter
 * position where a surface has been found to be illegal.
 * <br>
 * A type is assigned to each instance to act as an identifier
 * for derived classes, so that receiving classes can cast to
 * the correct derived type.
 * <br>
 * There is no guarantee that a particular check_status_list object
 * will have any details. The details, if any, can be retrieved using
 * the details() method of check_status_list, but the caller must check
 * that the returned pointer is non-null before trying to dereference it.
 */
class DECL_KERN check_status_details : public ACIS_OBJECT
{
private:
	check_details_type _type;

protected:
	check_status_details(check_details_type type) : _type(type) {}

public:
	check_details_type type() const { return _type; }

	virtual check_status_details* clone() const = 0;
	virtual ~check_status_details();
};

/**
 *.
 */
class DECL_KERN check_high_curvature_details : public check_status_details
{
private:
	double _u0;
	double _v0;
	double _u1;
	double _v1;
	double _rad;
public:
    /**
     *.
     */
	check_high_curvature_details(double u0,double v0,double u1,double v1,
						 double rad_curvature)
		: check_status_details(high_curvature_details),
	      _u0(u0),_v0(v0),_u1(u1),_v1(v1),_rad(rad_curvature) {}
    /**
     *.
     */
	virtual check_status_details* clone() const;
    /**
     *.
     */
	double u0() const { return _u0; }
    /**
     *.
     */
    double v0() const { return _v0; }
    /**
     *.
     */
	double u1() const { return _u1; }
    /**
     *.
     */
	double v1() const { return _v1; }
    /**
     *.
     */
	double rad_curvature() const { return _rad; }
};

/** 
 * A class for returning details of a check status error.
 * <br>
 * <b>Role:</b> Instances of this class may be returned as details
 * for a "check_bad_fitol" status. The type() method must be used
 * to check the details type before downcasting.
 */
class DECL_KERN check_bad_fitol_details : public check_status_details
{
private:
	double _required_fitol;
	double _current_fitol;
public:
	/**
	 * Constructor takes the calculated fit tolerance, and the
     * current fit tolerance.
	 * @param required
     * The fit tolerance calculated to be required by the
     * check.
	 * @param current
     * The fit tolerance currently claimed for the object being
     * checked.
	 */
	check_bad_fitol_details(double required,double current)
		: check_status_details(bad_fitol_details),
	      _required_fitol(required),_current_fitol(current) {}

	virtual check_status_details* clone() const;

	/**
	 * Access the value of the required fit tolerance found
	 * by this check.
     */
	double required_fitol() const { return _required_fitol; }

	/**
	 * Access the value of the current fit tolerance claimed
	 * by the object being checked.
     */
	double current_fitol() const { return _current_fitol; }
};

/** 
 * A class for returning details of a check status error.
 * <br>
 * <b>Role:</b> Instances of this class may be returned as details
 * for a self intersection status. The type() method must be used
 * to check the details type before downcasting.
 */
class DECL_KERN check_self_intersection_details : public check_status_details
{
private:
	SPApar_pos _uv1;
	SPApar_pos _uv2;
public:
	check_self_intersection_details(SPApar_pos const& uv1, SPApar_pos const& uv2);

	virtual check_status_details* clone() const;

	SPApar_pos uv(logical first=TRUE) const;
};


/**
 * Implements the list of return codes for status checking.
 * <br>
 * <b>Role:</b> Both curve  and  surface checkers return similar lists
 * of  return codes when a curve or surface is illegal, or <tt>NULL</tt> if it
 * is  OK.  Note  that  in  both  cases  the  subdivision  and  self-
 * intersection tests are only performed if everything else worked.
 * @see check_status
 */
class DECL_KERN check_status_list : public ACIS_OBJECT
    {
private:
/**
 * Special case.
 */
	logical             _other_data;
/**
 * Status code.
 */
    check_status        _status;
/**
 * Next element on list.
 */
    check_status_list*  _next;

	check_status_details* _details;

/**
 * Constructor
 */
/*	check_status_list( check_status       status,
		               check_status_list* next,
			   check_status_details* details)
		:_status(status), _next(next),_details(details)
		{ _other_data = FALSE; };
		*/
	check_status_list(check_status       status,
		check_status_list* next,
		check_status_details* details);

public:
	/**
 * Destructor, destroying the list from here on.
 */
    ~check_status_list();
/**
 * Adds an error to the front of the list.
 * <br><br>
 * <b>Role:</b> Returns a new <tt>check_status_list</tt> with the new error incorporated.
 * <br><br>
 * @param status
 * error to add to list
 * @param details
 * optional details of the error
 */
    check_status_list* add_error( check_status status,
						  check_status_details* details = 0);
/**
 * Adds a list of errors to the front of the list, Returns the new start.
 * <br><br>
 * @param list
 * error list.
 */
     check_status_list* add_list( check_status_list* list );
/**
 * Checks for a particular status.
 * <br><br>
 * @param wanted
 * status to check for.
 */
    logical	contains( check_status wanted )	const;
/**
 * Returns the next element of the error <tt>check_status_list</tt>, or <tt>NULL</tt> if there is none.
 */
    check_status_list*	next() { return _next; }

/**
 * Returns the next element of the error <tt>check_status_list</tt>, or <tt>NULL</tt> if there is none.
 */
    const check_status_list* next() const { return _next; }

/** 
 * Return the details for this error, if any (could be a null pointer)
 */
	const check_status_details* details() const { return _details; }


/**
 * Returns a new list of all of the errors available (excluding '<tt>unknown</tt>').
 * <br><br>
 * <b>Role:</b> The caller is responsible for deleting the returned list.
 */
	static check_status_list* all_errors();
/**
 * Removes  all  occurrences of an  error from the list, returning the new start of the list.
 * <br><br>
 * <b>Role:</b> Note that the returned pointer could be nullif all instances in have the specified status.
 * <br><br>
 * @param status
 * error to remove from list.
 */
	check_status_list* remove_error(check_status status);
/**
 * @nodoc
 * Get special case.
 */
	logical other_data(void) const { return _other_data; }
/**
 * @nodoc
 * Set special case.
 */
	void other_data(logical val) { _other_data = val; }
/**
 * Returns the status code of this element of the list.
 */
     check_status	status() const { return _status; };
/**
 * Returns a copy of the current check_list
 */
	 check_status_list* copy_list() const;
 };
/**
 * @nodoc
 * Available checks.
 */
enum fix_option
    {
	bs2_curve_fix_range_e        = 1,
	bs2_curve_fix_closure_e      = 2,
	bs2_curve_fix_extra_knots_e  = 4,
	bs2_curve_fix_nonG1_disc_e   = 8,
	bs3_curve_fix_closure_e      = 16,
	bs3_curve_fix_extra_knots_e  = 32,
	bs3_curve_fix_coi_verts_e    = 64,
	bs3_curve_fix_nonG1_disc_e   = 128,
	bs3_surface_fix_degeneracy_e = 256,
    fix_unknown  // This one should remain last:
    };
/**
 * @nodoc
 * Check fix
 * <br><br>
 * <b>Role :</b>
 * Curves and surfaces can sometimes be fixed when they are found to be
 * faulty.  The check_fix class is used to describe the fixes that are
 * available.  The check functions use the check_fix class both to specify
 * which operations are allowable in fixing the faults, and which fixes have
 * actually been applied.
 */
class 	DECL_KERN  check_fix  : public ACIS_OBJECT
{
/**
 * Make any overspecified properties, such as curve range or
 * closure type, consistent with the master definition.
 */
    logical	_make_consistent;
/**
 * Check for split faults in B-spline curve or surface.
 */
    logical	_check_split;
/**
 * Fix faults by moving interior B-spline vertices of a curve
 * or surface by up to this amount.
 */
    double	_interior_vertices;
/**
 * Fix faults by moving boundary B-spline vertices of a curve
 * or surface by up to this amount.
 */
    double	_boundary_vertices;
/**
 * Fix faults by moving parameter space B-spline vertices of a
 * bs2_curve by up to this amount.
 */
    double	_pspace_vertices;
/**
 * Fix faults by moving interior B-spline vertices of a curve or
 * surface is the angle of the tangents on either side of a
 * discontinuity does not exceed this amount.
 */
    double	_tangent_angle;
/**
 * Some fixing functions require more than one argument to be
 * supplied and the fix will then be made in such a way that
 * both conditions are fulfilled. If the user is only interested
 * in on condition, then the other arguments should be set to -1.0
 */
	int _fix_mask;
/**
 * Sets the tolerance for detecting and fixing coincident control points.
 */
	double _coin_tolerance;
public:
/**
 * Constructor setting all data to initial values
 * (either on or off, depending on argument).
 */
    check_fix( logical on = FALSE )
	:   _make_consistent( on ),
			_check_split( FALSE ),
			_fix_mask( 0 ),
            _interior_vertices( on ? (double)SPAresabs : 0.0 ),
            _boundary_vertices( on ? (double)SPAresabs : 0.0 ),
            _pspace_vertices( on ? (double)SPAresabs : 0.0 ),
            _tangent_angle( on ? 1.0 : 0.0 ),
			_coin_tolerance( on ? (double)SPAresabs : 0.0 )
	{}

    logical	make_consistent()	const 	{ return _make_consistent; }

    logical	check_split()	const 	{ return _check_split; }

    double	interior_vertices()	const 	{ return _interior_vertices; }

    double	boundary_vertices()	const 	{ return _boundary_vertices; }

    double	pspace_vertices()	const 	{ return _pspace_vertices; }

    double	tangent_angle()		const 	{ return _tangent_angle; }

	double  coin_tolerance() const { return _coin_tolerance; }

	// STI let 3 December 2021 : Fixed a typo
    logical fix_mask( fix_option fix_opt ) const  
		{ return (GET_ALGORITHMIC_VERSION() > AcisVersion(32, 0, 0)) ? 
			_fix_mask & fix_opt : _fix_mask && fix_opt; }

    void    set_make_consistent( logical on ) 	{ _make_consistent = on; }

    void    set_check_split( logical split ) 	{ _check_split = split; }

    void    set_interior_vertices( double tol ) { _interior_vertices = tol; }

    void    set_boundary_vertices( double tol ) { _boundary_vertices = tol; }

    void    set_pspace_vertices( double tol ) 	{ _pspace_vertices = tol; }

    void    set_tangent_angle( double tol )     { _tangent_angle = tol; }

    void    set_fix_mask_on( fix_option fix_opt )     { _fix_mask |= fix_opt; }

    void    set_fix_mask_off( fix_option fix_opt )     { _fix_mask &= ~fix_opt; }

    void    set_fix_mask_on()     { _fix_mask = 0xffff; }

	void    set_coin_tolerance( double tol )     { _coin_tolerance = tol; }

};
/**
 * @nodoc
 * Returns the enum in a check_status as a readable string.
 */
DECL_KERN char const *get_check_status_name( check_status );
/**
 * @nodoc
 * Self intersecting test function type.
 */
typedef check_status_list * (*self_intersect_test_fn_fn)( check_status_list*, BOUNDED_SURFACE&,
																sf_clash_list* clashes_in ,
															    SPApar_box &exclude_region ,
																logical points_on_surface );
 
#ifdef THREAD_SAFE_ACIS
/**
 * @nodoc
 */
extern DECL_KERN safe_function_type< self_intersect_test_fn_fn > self_intersect_test_fn;
#else
/**
 * @nodoc
 */
extern DECL_KERN self_intersect_test_fn_fn self_intersect_test_fn;

#endif
/**
 * @nodoc
 * Check surface self intersections
 * <br><br>
 * <b>Role :</b> Checks for true self-intersections in the surface. This function will
 * return TRUE if no self intsersections are found and will return FALSE otherwise.
 * Only true self-intersections (where different uv values of the surface have the same
 * 3D position (to resabs) cause this function to return FALSE. Thus
 * this function is more thorough than B-spline hull checks and likewise.
 * @param sf
 * Surface to check.
 * @param exclude_region
 * Region on surface to avoid checks, if any.
 * @param range
 * Surface domain (possibly subsetted) on which to do the checks
 * @param points_on_surface
 * Use the B-spline Hull (default) to do the checks or use the
 * points from the surface to do the checks.
 */
DECL_KERN logical sg_check_surface_self_intersections(surface*   sf,
									SPApar_box &exclude_region	 = *(SPApar_box*)NULL_REF,
									SPApar_box &range            = *(SPApar_box*)NULL_REF,
									logical    points_on_surface = FALSE,
									SPApar_pos &uv				 = *(SPApar_pos*)NULL_REF);

/**
 * @nodoc
 * Surface self intersection check.
 * @param sf
 * Surface to check.
 * @param range
 * Surface domain (possibly subsetted) on which to do the checks
 * @param exclude_region
 * Region on surface to avoid checks, if any.
 * @param points_on_surface
 * Use the B-spline Hull (default) to do the checks or use the
 * points from the surface to do the checks.
 */
DECL_KERN logical
sg_check_surface_self_intersections(bs3_surface       sf,
									SPApar_box const& range,
									SPApar_box&       exclude_region    = *(SPApar_box*)NULL_REF,
									logical           points_on_surface = FALSE,
									SPApar_pos &uv				 = *(SPApar_pos*)NULL_REF);


#if defined D3_DEBUG

class D3_ostream;
class D3_istream;
/**
 * @nodoc
 */
	DECL_KERN D3_ostream& operator<<( D3_ostream&, check_status );
/**
 * @nodoc
 */
	DECL_KERN D3_istream& operator>>( D3_istream&, check_status& );
/**
 * @nodoc
 */
	DECL_KERN D3_ostream& operator<<( D3_ostream&, check_status_list* );
/**
 * @nodoc
 */
	DECL_KERN D3_istream& operator>>( D3_istream&, check_status_list*& );
/**
 * @nodoc
 */
	DECL_KERN D3_ostream& operator<<( D3_ostream&, const check_fix& );
/**
 * @nodoc
 */
	DECL_KERN D3_istream& operator>>( D3_istream&, check_fix& );

#endif
/** @} */
#endif
