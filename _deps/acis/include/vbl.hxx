/* $Id: vbl.hxx,v 1.30 2002/08/20 15:20:21 jenglund Exp $ */
/*******************************************************************/
/*    Copyright (c) 1989-2020 by Spatial Corp.                     */
/*    All rights reserved.                                         */
/*    Protected by U.S. Patents 5,257,205; 5,351,196; 6,369,815;   */
/*                              5,982,378; 6,462,738; 6,941,251    */
/*    Protected by European Patents 0503642; 69220263.3            */
/*    Protected by Hong Kong Patent 1008101A                       */
/*******************************************************************/

#ifndef VBLH
#define VBLH

#include "dcl_kern.h"
#include "vbl_bdy.hxx"
#include "spldef.hxx"

/**
* @file vbl.hxx
 * @CAA2Level L1
 * @CAA2Usage U1
 * \addtogroup ACISSURFACES
 *
 * @{
 */

#if defined D3_DEBUG
class VBL_TEST_DATA;
#endif


/**
 * @nodoc
 */
#define vertex_blend_type 99

class VBL_CACHE;

// STI ROLL
class SizeAccumulator;
// STI ROLL

class VBL_OFFSURF;
class VBL_SURF;

/*
// tbrv
*/
/**
 * @nodoc
 */
void VBL_fd_diff_derivs(const VBL_SURF&, const SPApar_pos&, SPAvector**, const int, int, const SPApar_vec&, const SPApar_vec& );

/*
// tbrv
*/
/**
 * @nodoc
 */
SPApar_pos param_internal(VBL_SURF& vbl, const SPAposition& iX, logical on_surf, const SPApar_pos& uv_guess );

// enumerator to specify the type of error that is to be found by the
// VBL_SURF error functions. If the enum is set to exact error, then
// the error functions attempt to find the maximum error across the
// region of boundary or across the internal surface region; If the
// enum is set to approximate_error, then the errors over the regions
// are approximated by the errors in the centre of the region; If the
// enum is set to approx_if_too_big, the errors will be calculated
// accurately unless the error is found to be bigger than a requested
// tolerance, in which case the errors are approximated.

/**
* Specifies the type of error that is to be found by the <tt>VBL_SURF</tt> error functions.
* <br>
* @param exact_error
* attempts to find the maximum error across the region of
* boundary or across the internal surface region.
* @param approximate_error
* errors over the regions are approximated by the errors
* in the center of the region.
* @param approx_if_too_big
* errors are calculated accurately unless the error is
* found to be bigger than a requested tolerance, in which case the errors are
* approximated.
*/
enum error_type{ exact_error, approximate_error, approx_if_too_big };

// This is to do with blending the true surface onto the approximating one
// outside the polygon. We use TRUE_SUR to indicate we only need the true
// surface (so we must be in or on the polygon), APPROX_SUR when we're well
// outside and need only the approximating surface, BOTH_SURS when we're
// marginally outside and so must blend the two together.

/**
* Specifies the blending of true surfaces with the approximating one outside the polygon.
* <br>
* @param TRUE_SUR
* only need the true surface, so must be in or on the
* polygon.
* @param APPROX_SUR
* well outside and need only the approximating surface.
* @param BOTH_SURS
* marginally outside and must blend the two together.
**/
enum blend_coord_type { TRUE_SUR, APPROX_SUR, BOTH_SURS };

//
// The vertex blend surface class itself.
// It is defined entirely by the n boundaries which make it up.
//
/**
 * Defines the vertex blend surface class.
 * <br>
 * <b>Role:</b> This class defines the vertex blend surface class. It is defined entirely by the n
 * boundaries that make it up.
 */
class DECL_KERN VBL_SURF : public spl_sur {
// Allow extensions to declare themselves as friends. USE WITH EXTREME CAUTION
#ifdef VBL_SURF_FRIENDS
VBL_SURF_FRIENDS
#endif

private:

    // Cache for one-per-surface things which change from time to time.

    VBL_CACHE*		_cache;

    // Number of boundary curves of the patch.

    int			_n;

    // Pi / n and it's sine and cosine.

    double		_angle, _cos_angle, _sin_angle;

    // (st)^2 limit beyond which we're on the approx surface only.

    double		_blend_limit;

    // Grid size used for the approximating surface, if there is one.

    int			_grid;

    // Array of pointers to boundary curves, dimension _n.

    BDY_GEOM**		_boundaries;

protected:
    // This will hold a non-const pointer to this object. It is so that we can
    // make SVECs on the blend surface by first making a spline on the blend
    // surface, for which we need the non-const pointer. This is very sneaky and
    // is not provided willingly.

    // VBL_OFFSURFs also need access to this to make splines on themselves.

    VBL_SURF*		_self;

    void _deep_copy_vbl( const VBL_SURF& sf , pointer_map * pm);
    // called ONLY by deep_copy.

private:
    // Sine and cosine of 2 * i * pi / n.

    double		_sine( int i ) const
	{ return _boundaries[ i % _n ]->sine(); };
    double		_cosine( int i ) const
	{ return _boundaries[ i % _n ]->cosine(); };

    // Sine and cosine of ( 2 * i - 1 ) * pi / n.

    double		_sine_1( int i ) const
	{ return _boundaries[ i % _n ]->sine_1(); };
    double		_cosine_1( int i ) const
	{ return _boundaries[ i % _n ]->cosine_1(); };

    // Local SPAparameter s_i and t_i = 1 - s_i-1, and derivs wrt u,v.

    double		_s( int i ) const { return _boundaries[ i ]->s(); };
    double&		_su( int i ) const { return _boundaries[ i ]->su(); };
    double&		_sv( int i ) const { return _boundaries[ i ]->sv(); };
    double&		_suu( int i ) const { return _boundaries[ i ]->suu(); };
    double&		_suv( int i ) const { return _boundaries[ i ]->suv(); };
    double&		_svv( int i ) const { return _boundaries[ i ]->svv(); };

    double		_t( int i ) const
	{ return _boundaries[ i == 0 ? _n - 1 : i - 1 ]->t(); };
    double		_tu( int i ) const
	{ return _boundaries[ i == 0 ? _n - 1 : i - 1 ]->tu(); };
    double		_tv( int i ) const
	{ return _boundaries[ i == 0 ? _n - 1 : i - 1 ]->tv(); };
    double		_tuu( int i ) const
	{ return _boundaries[ i == 0 ? _n - 1 : i - 1 ]->tuu(); };
    double		_tuv( int i ) const
	{ return _boundaries[ i == 0 ? _n - 1 : i - 1 ]->tuv(); };
    double		_tvv( int i ) const
	{ return _boundaries[ i == 0 ? _n - 1 : i - 1 ]->tvv(); };

    // Settable references to the above ONLY for use if the t coord is known to
    // have been explicity set (eg. 3-sided blends).

    double&		_tu_r( int i ) const
	{ return _boundaries[ i == 0 ? _n - 1 : i - 1 ]->tu_r(); };
    double&		_tv_r( int i ) const
	{ return _boundaries[ i == 0 ? _n - 1 : i - 1 ]->tv_r(); };
    double&		_tuu_r( int i ) const
	{ return _boundaries[ i == 0 ? _n - 1 : i - 1 ]->tuu_r(); };
    double&		_tuv_r( int i ) const
	{ return _boundaries[ i == 0 ? _n - 1 : i - 1 ]->tuv_r(); };
    double&		_tvv_r( int i ) const
	{ return _boundaries[ i == 0 ? _n - 1 : i - 1 ]->tvv_r(); };


    // Set s_i.

    void		_set_s( int i, double s ) const
	{ _boundaries[ i ]->set_s( s ); };

    void		_set_t( int i, double t ) const
	{ _boundaries[ i == 0 ? _n - 1 : i - 1 ]->set_t( t ); };

    // Value of Q_i and its derivs wrt s and t.

    SPAvector&		_Q( int i ) const { return _boundaries[ i ]->Q; };
    SPAvector&		_Qs( int i ) const { return _boundaries[ i ]->Qs; };
    SPAvector&		_Qt( int i ) const { return _boundaries[ i ]->Qt; };
    SPAvector&		_Qss( int i ) const { return _boundaries[ i ]->Qss; };
    SPAvector&		_Qst( int i ) const { return _boundaries[ i ]->Qst; };
    SPAvector&		_Qtt( int i ) const { return _boundaries[ i ]->Qtt; };

    // The blending functions (unnormalised), now fns of u and v, and derivs,
    // and their sums over i.

    double&		_b( int i ) const { return _boundaries[ i ]->b; };
    double&		_bu( int i ) const { return _boundaries[ i ]->bu; };
    double&		_bv( int i ) const { return _boundaries[ i ]->bv; };
    double&		_buu( int i ) const { return _boundaries[ i ]->buu; };
    double&		_buv( int i ) const { return _boundaries[ i ]->buv; };
    double&		_bvv( int i ) const { return _boundaries[ i ]->bvv; };

    double&		_S_b() const;
    double&		_S_bu() const;
    double&		_S_bv() const;
    double&		_S_buu() const;
    double&		_S_buv() const;
    double&		_S_bvv() const;

    // Compute end pts, derivs etc. of boundaries, and related 'fullness' values.

    void 		_ensure_bounds() const;

    // Compute local coords s for a point.

    void		_ensure_coords( const SPApar_pos&, int ) const;

    // Compute Qs and derivatives.

    void		_ensure_Qs( int nderivs ) const;

    // Compute and cache the (unnormalised) blending functions.

    void		_ensure_bs( const SPApar_pos& uv, int nderivs ) const;

    // Make boundary svecs/cvecs compute themselves.

    void		_ensure_points( int ) const;

    // Do we need to blend to the approx surface, or what?

    blend_coord_type	_decide_coord_type() const;

    // Compute the blending function and derivs to blend onto approx surface, if
    // we need it.

    void		_ensure_blend_fn( int ) const;

    // Compute the SPAparameter used in blending onto approx surface, plus derivs.

    void		_compute_blend_param( int, int, double&, double*,
					      double* ) const;

    // Set up all cached info. Return whether we're on the true or approx sur,
    // or inbetween.

    blend_coord_type	_setup( const SPApar_pos&, int ) const;

    // This is the internal function that computes the true surface
    // evaluation once _setup has been called to get everything ready
    // for it.

    void		_eval_internal( SPAposition&, SPAvector* = NULL,
					SPAvector* = NULL ) const;

    // This does the exact evaluation for the vertex blend surface.
    // VBL_OFFSURFs may call it, it evalutes the true polygonal surface
    // taking no account of any offset. It doesn't try and do any of the
    // weird "blending" into the approximate surface when you leave the
    // polygonal domain. VBL_OFFSURFs call it, hence it is protected and
    // not private. It returns whether the coordinate is on the true or
    // approx surfaces, or inbetween. If wholly on the approx surface,
    // it doesn't evaluate anything.

protected:
    blend_coord_type	_eval_no_offset( SPApar_pos const&,
					 SPAposition&,
					 SPAvector* = NULL,
					 SPAvector* = NULL ) const;
/**
 * @nodoc
 */
	virtual void full_size(SizeAccumulator&, logical = TRUE) const;
/**
 * @nodoc
 */
	virtual void minimize( minimize_helper*);

private:
    // This is the main function derived for VBL_OFFSURFs. For ordinary
    // VBL_SURFs, however, it calls straight through to _eval_no_offset.
    // Note that for VBL_OFFSURFs the second derivatives are not
    // accurate. Again, doesn't do any of the "blending" onto the
    // approximate surface outside the polygonal domain, but returns, as
    // _eval_no_offset, where the uv point lies. If wholly on the approx
    // surface, nothing is evaluated.

    virtual blend_coord_type	_eval_with_offset( SPApar_pos const&,
						   SPAposition&,
						   SPAvector* = NULL,
						   SPAvector* = NULL ) const;

    // Has the centre been cached?

    char&		_got_centre() const;

    // Return the cached central SPAposition. Note that this excludes any
    // offset (for VBL_OFFSURFs).

    SPAposition&		_centre() const;

    // uv coords of vertex i, without doing an ensure_bounds check first.

    const SPApar_pos&	_vertex( int i ) const
	{ return _boundaries[ ( i + 1 ) % _n ]->vertex(); };

    // Cached bits and pieces for computation of the blending functions.

    SPApar_dir&		_l1( int i ) const
	{ return _boundaries[ ( i + 1 ) % _n ]->l1; };
    SPApar_dir&		_l2( int i ) const
	{ return _boundaries[ ( i + 1 ) % _n ]->l2; };
    SPApar_pos&		_uvp( int i ) const
	{ return _boundaries[ ( i + 1 ) % _n ]->uvp; };

    // Telax to a SPAparameter on a boundary, the final argument containing the
    // boundary curve you think it's on. It will contain the actual boundary at
    // the end.

    logical		_bdy_relax( const SPAposition&, double&, int& ) const;

    // Some grubby memory management stuff, not for general use.
    void		_initialize();
    void		_reset() const;
    logical		_allocate( int );
    void		_deallocate();
    logical		_reallocate( int );
#if defined( osf1 )
public:
#else
protected:
#endif
    // The VBL_OFFSURF needs this one.

    void		_copy_vbl( const VBL_SURF& );
private:

#if defined D3_DEBUG
    int			_warnings() const;
#endif

    // D3 i/o supporting functions.
#if defined D3_DEBUG
protected:
    friend class VBL_TEST_DATA;

    virtual void	_print( D3_ostream& ) const;
    virtual void	_input( D3_istream& );
#endif

public:
/**
 * C++ allocation constructor requests memory for this object but does not populate it.
 */
    VBL_SURF();
/**
 * C++ initialize constructor requests memory for this object and populates it with the data supplied as arguments.
 * <br><br>
 * @param n
 * number of boundary curves.
 */
    VBL_SURF( int n );
/**
 * C++ destructor, deleting a <tt>VBL_SURF</tt>.
 */
    ~VBL_SURF();
/**
 * C++ copy constructor requests memory for this object and populates it with the data from the object supplied as an argument.
 * <br><br>
 * @param ver_blnd
 * vertex blend surface.
 */
    VBL_SURF( const VBL_SURF& ver_blnd);

    // Make a copy without sharing subdata.
/**
 * Creates a copy of an item that does not share any data with the original.
 * <br><br>
 * <b>Role:</b> Allocates new storage for all member data and any pointers. Returns a pointer to the
 * copied item.
 * <br><br>
 * In a deep copy, all the information about the copied item is self-contained in a new memory block.
 * By comparison, a shallow copy stores only the first instance of the item in memory, and increments
 * the reference count for each copy.
 * <br><br>
 * The pointer_map keeps a list of all pointers in the original object that have already been deep
 * copied. For example, a deep_copy of a complex model results in self contained data, but identical
 * sub-parts within the model are allowed to share a single set of data.
 * <br><br>
 * @param pm
 * list of items within the entity that are already deep copied.
 */
    virtual spl_sur *deep_copy(pointer_map * pm = NULL) const;

/**
 * Implements an assignment operator, which makes a copy of a <tt>VBL_SURF</tt>.
 * <br><br>
 * @param ver_blnd
 * vertex blend surface.
 */
    virtual VBL_SURF&	operator=( const VBL_SURF& ver_blnd);


    // Setting the boundaries with various bits of geometry. In all cases the
    // final double argument is the fullness. All but one have two final logical
    // arguments, indicating whether the smooth G1 interpolation is required
    // across the boundary (sometimes it may be possible for one corner
    // interpolant but not the other, hence 2 arguments), the defaults
    // indicating YES. The final one is for non-smooth boundaries where cross
    // boundary data is not known, so there's no choice - the answer here is NO.
    // In all cases there is another logical argument indicating whether it is
    // to be marked as a cross curve - meaning that the magic SPAvector will be
    // used for shape control. Normally it will figure the magic SPAvector out for
    // itself but occasionally it can't, eg. if the normals at the s=0 and s=1
    // are parallel, or if there is no normal information at all, and in these
    // cases the magic SPAvector should be passed if required.

    // Set boundary i geometry as pcurve.
/**
 * Sets the boundaries for the geometry as a <tt>pcurve</tt>.
 * <br><br>
 * @param no_bound
 * number of boundaries.
 * @param pcur
 * pcurve.
 * @param cross_curve
 * cross curve.
 * @param cur
 * curve.
 * @param fullness
 * fullness.
 * @param mark
 * mark as cross curve.
 * @param smooth
 * smooth at beginning.
 * @param bulge
 * bulge faceter direction.
 */
    void		set_geometry( int no_bound, const pcurve &pcur, logical cross_curve,
				      const curve &cur, double fullness = 0, logical mark = 1,
				      logical smooth = 1,
				      const SPAunit_vector& bulge = *(SPAunit_vector*)NULL_REF );

    // ...as a curve on a plane.
/**
 * Sets the boundaries for the geometry as a curve on a plane.
 * <br><br>
 * <b>Role:</b> Sets the boundaries for the geometry as a curve on a plane.
 * <br><br>
 * If logical (smooth GI interpolation) is <tt>TRUE</tt>, it indicates that smooth GI interpolation
 * is required across the boundary. Sometimes, it is possible to have one corner, but not the other.
 * <br><br>
 * If logical (nonsmooth boundaries) is <tt>TRUE</tt>, the cross-boundary data is not known.
 * <br><br>
 * If logical (mark as a cross curve) is <tt>TRUE</tt>, it means that the magic vector is used for
 * shape control. Usually, the function determines the magic vector; but occasionally, it cannot
 * (e.g., the normals at s=0 and s=1 are parallel or if there is no normal information at all); in
 * these cases, the magic vector should be passed if required.
 * <br><br>
 * @param no_bound
 * number of boundaries.
 * @param cur
 * curve.
 * @param nor_plane
 * normal to plane.
 * @param u_val
 * u-parameter value.
 * @param v_val
 * v-parameter value.
 * @param fullness
 * fullness.
 * @param mark
 * marks as cross curve.
 * @param smooth_beg
 * smooth at beginning.
 * @param smooth_end
 * smooth at end
 * @param bulge.
 * bulge faceter direction.
 */
    void		set_geometry( int no_bound, const curve& cur, const SPAunit_vector& nor_plane,
				      double u_val, double v_val, double fullness = 0, logical mark = 0,
				      logical smooth_beg = 1, logical smooth_end = 1,
				      const SPAunit_vector& bulge = *(SPAunit_vector*)NULL_REF );

    // ...as a circle on the end of a cylinder.
/**
 * Sets the boundaries for the geometry as a circle on the end of a cylinder.
 * <br><br>
 * @param no_bound
 * number of boundaries.
 * @param ellipse
 * ellipse.
 * @param start
 * start parameter.
 * @param end
 * end parameter.
 * @param fullness
 * fullness.
 * @param mark
 * marks as cross curve.
 * @param smooth_beg
 * smooth at beginning.
 * @param smooth_end
 * smooth at end.
 * @param bulge
 * bulge faceter direction.
 */
    void		set_geometry( int no_bound, const ellipse& ellipse, double start, double end,
				      double fullness = 0, logical mark = 1, logical smooth_beg = 1,
				      logical smooth_end = 1,
				      const SPAunit_vector& bulge = *(SPAunit_vector*)NULL_REF );

    // ...as a circle on the end of a torus.
/**
 * Sets the boundaries for the geometry as a circle on the end of a torus.
 * <br><br>
 * @param no_bound
 * number of boundaries.
 * @param ellipse
 * ellipse.
 * @param start
 * start parameter.
 * @param end
 * end parameter.
 * @param pt
 * point.
 * @param fullness
 * fullness.
 * @param mark
 * marks as cross curve.
 * @param smooth_beg
 * smooth at beginning.
 * @param smooth_end
 * smooth at end.
 * @param bulge
 * bulge faceter direction.
 */
    void		set_geometry( int no_bound, const ellipse& ellipse, double start, double end,
				      const SPAposition& pt, double fullness = 0, logical mark = 1,
				      logical smooth_beg = 1, logical smooth_end = 1,
				      const SPAunit_vector& bulge = *(SPAunit_vector*)NULL_REF );

    // ...as a circle on the end of a pipe.
/**
 * Sets the boundaries for the geometry as a circle on the end of a pipe.
 * <br><br>
 * @param no_bound
 * number of boundaries.
 * @param ellipse
 * ellipse.
 * @param start
 * start parameter.
 * @param end
 * end parameter.
 * @param pcur
 * pcurve.
 * @param fullness
 * fullness.
 * @param mark
 * marks as cross curve.
 * @param smooth_beg
 * smooth at beginning.
 * @param smooth_end
 * smooth at end.
 * @param bulge
 * bulge faceter direction.
 */
    void		set_geometry( int no_bound, const ellipse& ellipse, double start, double end,
				      const pcurve& pcur, double fullness = 0, logical mark = 1,
				      logical smooth_beg = 1, logical smooth_end = 1,
				      const SPAunit_vector& bulge = *(SPAunit_vector*)NULL_REF );

    // ...as a circle with given twist vectors at the ends of the boundary.
/**
 * Sets the boundaries for the geometry as a circle with given twist vectors at the ends of the boundary.
 * <br><br>
 * @param no_bound
 * number of boundaries.
 * @param ellipse
 * ellipse.
 * @param start
 * start parameter.
 * @param end
 * end parameter.
 * @param first_vec
 * first twist vector.
 * @param second_vec
 * second twist vector.
 * @param fullness
 * fullness.
 * @param mark
 * marks as cross curve.
 * @param smooth_beg
 * smooth at beginning.
 * @param smooth_end
 * smooth at end.
 * @param bulge
 * bulge faceter direction.
 */
    void		set_geometry( int no_bound, const ellipse& ellipse, double start, double end,
				      const SPAvector& first_vec, const SPAvector& second_vec,
				      double fullness = 0,
				      logical mark = 1, logical smooth_beg = 1, logical smooth_end = 1,
				      const SPAunit_vector& bulge = *(SPAunit_vector*)NULL_REF );

    // ...as a degenerate zero-radius 'cross curve', or just a point.
/**
 * Sets the boundaries for the geometry as a degenerate zero-radius cross curve or as a point.
 * <br><br>
 * @param no_bound
 * number of boundaries.
 * @param pos
 * position.
 * @param nor_start
 * normal at start.
 * @param nor_end
 * normal at end.
 * @param fullness
 * fullness.
 * @param mark
 * marks as cross curve.
 * @param smooth_beg
 * smooth at beginning.
 * @param smooth_end
 * smooth at end.
 * @param bulge
 * bulge faceter direction.
 */
    void		set_geometry( int no_bound, const SPAposition& pos, const SPAunit_vector& nor_start,
				      const SPAunit_vector& nor_end, double fullness = 0,
				      logical mark = 1, logical smooth_beg = 1, logical smooth_end = 1,
				      const SPAunit_vector& bulge = *(SPAunit_vector*)NULL_REF );

    // ...as a curve across which the blend is not smooth.
/**
 * Sets the boundaries for the geometry as a curve across which the blend is not smooth.
 * <br><br>
 * @param no_bound
 * number of boundaries.
 * @param cur
 * curve.
 * @param start
 * start parameter.
 * @param end
 * end parameter.
 * @param fullness
 * fullness.
 * @param mark
 * marks as cross curve.
 * @param bulge
 * bulge faceter direction.
 */
    void		set_geometry( int no_bound, const curve& cur,
				      double start, double end, double fullness = 0, logical mark = 0,
				      const SPAunit_vector& bulge = *(SPAunit_vector*)NULL_REF );


    // Check for any data errors in the surface, and correct the errors if
    // possible.  The various arguments provide control over which checks
    // are made, which fixes can be applied and which fixes were actually
    // applied.  The function returns a list of errors that remain in the
    // surface on exit.
    // See chk_stat.hxx for information on the argument types used here.
/**
 * Checks for any data errors in the surface, and corrects the errors if possible.
 * <br><br>
 * <b>Role:</b> The various arguments provide control over which checks are made, which fixes can
 * be applied and which fixes were actually applied. The function returns a list of errors that
 * remain in the surface on exit.
 * <br><br>
 * The default for the set of flags which say which fixes are allowable is none (nothing is fixed).
 * If the list of checks to be made is null, then every possible check will be made. Otherwise,
 * the function will only check for things in the list. The return value for the function will then
 * be a subset of this list.
 * <br><br>
 * @param input
 * flags for allowed fixes.
 * @param result
 * fixes applied.
 * @param status
 * checks to be made,default is none.
 */
    virtual	check_status_list*
	check( const check_fix& input = *(const check_fix*) NULL_REF,
	           // supplies a set of flags which say which fixes
	           // are allowable (the default is to fix nothing)
	       check_fix& result = *(check_fix*) NULL_REF,
	           // returns a set of flags which say which fixes
	           // were applied
	       const check_status_list* status = (const check_status_list*) NULL_REF
	           // list of checks that are to be made.  If the
	           // list is null, then every possible check will
	           // be made; otherwise, the function will only
	           // check for things in the list.  The return
	           // value for the function will then be a subset
	           // of this list.
	       );


    // Do a check of the surface, returning FALSE if we think it
    // is illegal. This may be called as soon as all the boundaries are
    // set.
/**
 * Performs a quick check of the surface, returning <tt>FALSE</tt> if we think it is illegal.
 * <br><br>
 * <b>Role:</b> This may be called as soon as all the boundaries are set. At the moment all we do
 * is a rough check that the normal does not flip obviously anywhere round the boundary. This is
 * sufficient to catch cases of unreasonable boundary data being given.
 */
    check_status_list* 		check( ) const;

    // Number of boundaries.
/**
 * Returns the number of boundaries in a <tt>VBL_SURF</tt>.
 */
    int n() const	{ return _n; };

    // Cosine of pi / n.
/**
 * Returns the cosine angle, which is <i>pi</i> divided by the number of boundaries.
 */
    double	cos_angle() const { return _cos_angle; };

    // Return grid size used for the approximating surface, if there is one.
/**
 * Returns the grid size used for the approximating surface, if there is one.
 */
    int	grid() const { return _grid; };

    // The ith boundary.
/**
 * Returns the ith boundary in a <tt>VBL_SURF</tt>.
 * <br><br>
 * @param i
 * ith boundary.
 */
    BDY_GEOM& boundary( int i ) const	{ return *_boundaries[ i ]; };

    // Guess uv SPAparameter for SPAposition, and optionally whether it reckons it's
    // guessed bang-on.

/**
 * Guesses the <i>uv</i>-parameter for position, and optionally whether it thinks that the guess is correct.
 * <br><br>
 * @param pos
 * guess position.
 * @param pos_cur
 * position correct.
 */
    SPApar_pos	param_guess( const SPAposition& pos, logical& pos_cur = *(logical*)NULL_REF ) const;

    // Relax to a SPAparameter on a boundary, the first argument being a
    // guessed boundary SPAparameter, the final argument containing the
    // boundary curve you think it's on. Both arguments should be set
    // correctly at the end. Returns TRUE if it thinks it found a
    // perpendicular to the boundary.
/**
 * Returns the relaxed body.
 * <br><br>
 * @param pos
 * position.
 * @param dval
 * double.
 * @param ival
 * integer.
 */
    logical	bdy_relax( const SPAposition& pos, double& dval, int& ival ) const;

    // Is this uv SPAparameter on the surface?
/**
 * Returns <tt>TRUE</tt> if the <i>uv</i>-parameter is on the surface; otherwise, it returns <tt>FALSE</tt>.
 * <br><br>
 * @param uv_param
 * uv-parameter.
 */
    logical	on_surface( const SPApar_pos& uv_param) const;

    // Return valid range for v for the given value of u.

/**
 * Returns the valid range for <i>v</i> for the given value of <i>u</i>.
 * <br><br>
 * @param u_val
 * u value.
 */
    SPAinterval		v_range_for_u( double u_val) const;

    // as above, mut. mut.
/**
 * Returns the valid range for <i>u</i> for the given value of <i>v</i>.
 * <br><br>
 * @param v_val
 * v value.
 */
    SPAinterval	 u_range_for_v( double v_val) const;

    // Return the value of v for the given u, on the ith boudary. Return TRUE if
    // there is a value v, else FALSE. If requested return the value of lambda,
    // ie. how far along the edge we are.
/**
 * Returns the value of <i>v</i> for the given <i>u</i>, on the ith boundary.
 * <br><br>
 * <b>Role:</b> This method returns <tt>TRUE</tt> if there is a value <i>u</i>; otherwise, it returns <tt>FALSE</tt>. If
 * requested, this method returns the value of lambda; i.e., how far along the edge.
 * <br><br>
 * @param i_bound
 * ith boundary.
 * @param u_val
 * u-parameter value.
 * @param ulen
 * length along edge in u.
 * @param vlen
 * length along edge in v.
 */
    logical	v_for_u_on_i( int i_bound, double u_val, double& ulen,
				      double& vlen = *(double*)NULL_REF ) const;

    // as above, mut. mut.
/**
 * Returns the value of <i>u</i> for the given <i>v</i>, on the ith boundary.
 * <br><br>
 * <b>Role:</b> This method returns <tt>TRUE</tt> if there is a value v; otherwise, it returns <tt>FALSE</tt>. If
 * requested, this method returns the value of lambda; i.e., how far along the edge.
 * <br><br>
 * @param i_bound
 * ith boundary.
 * @param v_val
 * u-parameter value.
 * @param ulen
 * length along edge in u.
 * @param vlen
 * length along edge in v.
 */
    logical	u_for_v_on_i( int i_bound, double v_val, double& ulen,
				      double& vlen = *(double*)NULL_REF ) const;

    // uv coords of vertex i.


	SPApar_pos center_uv() const;
/**
 * Returns the <i>uv</i> coordinates of ith vertex.
 * <br><br>
 * @param i
 * ith vertex.
 */
    const SPApar_pos& vertex( int i ) const;

    // Make the approximating bs3_surface to specified fitol, return NULL if we
    // fail - must be called after setting all the boundaries. Pass the
    // reference int only if you want to find out the resulting grid size used,
    // if you pass it set to a value >1 it gets used as the initial attempt at
    // the grid size. The next arg is for testing code only and may get removed
    // at any time. The final one is for the input member function: it passes
    // the correct fitol so if the desired grid size is also passed there'll be
    // no need to check the achieved fit. The default -1 means 'the fit must be
    // checked'. This works even for VBL_OFFSURFs because it calls eval
    // thereby getting offset data to interpolate.
/**
 * Makes the approximating <tt>bs3_surface</tt> to the specified fit tolerance.
 * <br><br>
 * <b>Role:</b> This method returns <tt>NULL</tt> if a <tt>bs3_surface</tt> cannot be constructed. Call this method
 * after setting all the boundaries. Pass the first int only to find out the resulting grid size
 * used; if the value is larger than 1, the value is used as the initial attempt at grid size. The
 * next int is for testing code only and may be removed at any time. The final double is the input
 * member function; it passes the correct fit tolerance so if the desired grid size is also passed,
 * there is no need to check the achieved fit. The default of -1 means that the fit tolerance must
 * be checked.
 * <br><br>
 * @param tol
 * fit tolerance.
 * @param grid
 * grid size.
 * @param test
 * test code.
 * @param check_tol
 * check fit tolerance.
 */
    bs3_surface	   make_sur( double tol, int& grid = *(int*)NULL_REF, int test = 0,
				 double check_tol = -1 );

    // Function to make up several four-sided approximating bs3_surface's
    // to cover the vbl surface. The arguments supplied are an integer,
    // which will be used to return the number of surfaces made, and the
    // requested fit tolerances, for the interior and the boundary of the
    // patches. These references will return the fit tolerances actually
    // obtained. The function returns an array of _n bs3_surface's.
/**
 * Makes up several four-sided approximating <tt>bs3_surface</tt>'s to cover the vbl surface.
 * <br><br>
 * <b>Role:</b> The arguments supplied are an integer, which will be used to return the number of
 * surfaces made, and the requested fit tolerances, for the interior and the boundary of the patches.
 * These references will return the fit tolerances actually obtained. The function returns an array
 * of <i>n_sfs</i> <tt>bs3_surface</tt>'s.
 * <br><br>
 * @param n_sfs
 * number of surfaces.
 * @param interior_fit
 * interior tolerance.
 * @param boundary_fit
 * boundary tolerance.
 * @param approx_error
 * approximate errors.
 */
    bs3_surface *make_output_surs( int &n_sfs, double &interior_fit,
				   double &boundary_fit,
				   logical approx_error = FALSE ) const;

    // Function to make up a bs3_surface approximating the i'th patch
    // of the supplied VBL_SURF, using the supplied number of points
    // across the s and t SPAparameter ranges. If the reference values
    // int_fit, fit_s and fit_t are supplied, the approximation error
    // inside the patch and on the boundaries is calculated. If the logical
    // argument approx_s_fit is set to TRUE and any of the errors along the
    // s SPAparameter boundary are found to be greater than the requested
    // tolerance, after this point the errors will be calculated
    // approximately. Similarly for the logical approx_t_fit. If either of
    // the logical arguments are set to TRUE, the references int_fit, fit_s,
    // and fit_t should be used to pass the requested fit tolerances into
    // the function.
/**
 * Calculates the <tt>bs3_surface</tt> inside the patch and on the boundaries.
 * <br><br>
 * <b>Role:</b> The <tt>bs3_surface</tt> inside the patch and on the boundaries is calculated. If the logical
 * argument <i>approx_s_fit</i> is set to <tt>TRUE</tt> and any of the errors along the s parameter boundary are
 * found to be greater than the requested tolerance, the errors are calculated approximately.
 * Similarly for the logical <i>approx_t_fit</i>. If either of the logical arguments are set to <tt>TRUE</tt>, the
 * references <i>int_fit</i>, <i>fit_s</i>, and <i>fit_t</i> should be used to pass the requested fit tolerances into the
 * function.
 * <br><br>
 * @param i
 * number of int_fit.
 * @param ns
 * number of s tol.
 * @param nt
 * number of t tol.
 * @param int_fit
 * tolerances for int.
 * @param fit_s
 * tolerances for s.
 * @param fit_t
 * tolerances for t.
 * @param approx_s_fit
 * flag for approximating s.
 * @param approx_t_fit
 * flag for approximating t.
 */
    bs3_surface
	make_approximating_patch( int i, int ns, int nt,
				  double &int_fit = *( double *) NULL_REF,
				  double &fit_s = *( double *) NULL_REF,
				  double &fit_t = *( double *) NULL_REF,
				  int approx_s_fit = 0,
				  int approx_t_fit = 0 ) const;

    // Mapping function which, given an st SPApar_pos on the four-sided kite-
    // shaped patch i of the VBL_SURF, calculates and returns the u and v
    // SPAparameter values, and if required, the SPAposition, the first partial
    // derivatives with respect to s and t, and the second partial
    // derivative with respect to s and t. The function returns a logical,
    // which denotes the success of the function.
/**
 * Returns the <i>u</i> and <i>v</i> values for a given <tt>SPApar_pos</tt>.
 * <br><br>
 * <b>Role:</b> This mapping function, when given a <tt>SPApar_pos</tt> st on the four-sided kite-shaped patch
 * i of the <tt>VBL_SURF</tt>, calculates and returns the <i>u</i> and <i>v</i> parameter values, and if
 * required, the position, the first partial derivatives with respect to s and t, and the second partial
 * derivative with respect to s and t.
 * <br><br>
 * The function returns a logical, which denotes the success of the function.
 * <br><br>
 * @param i
 * which patch.
 * @param st
 * where to evaluate.
 * @param uv
 * calculated uv.
 * @param pos
 * calculated pos.
 * @param derivs
 * array of derivatives.
 * @param n_derivs
 * number of derivatives.
 * @param quadrant
 * quadrant for mapping.
 */
    int st_evaluate( int i, SPApar_pos const &st, SPApar_pos &uv,
		     SPAposition &pos, SPAvector **derivs, int n_derivs,
		     evaluate_surface_quadrant quadrant
		     = evaluate_surface_unknown ) const;

    // Function to find the maximum error between one side of a four-sided
    // patch of the VBL_SURF and its approximating bs3_surface (parameterised
    // in s and t), along the given side only (0 => t=0, 1 => t=1, 2 => s=0,
    // 3 => s=1). If the enum err_type is of type exact_error, then the
    // function VBL_SURF_ERROR_FUNCTION::find_maximum will be used to find
    // the maximum errors over each of the spans of the supplied side; if
    // err_type is of type approximate_error, then the error over the side
    // will simply be approximated to be the maximum of the errors at the
    // mid-points of the spans; if err_type is of type approx_if_too_big,
    // then the function will start to calculate the errors using the
    // find_maximum function, but if the error exceeds the requested
    // tolerance, err_type will be changed to approximate_error, and the
    // errors will be approximated from this point onwards.
/*
// tbrv
*/
/**
 * @nodoc
 */
    friend class VBL_SURF_ERROR_FUNCTION;
/**
 * Finds the maximum error between one side of a four-sided patch of the <tt>VBL_SURF</tt> and its approximating <tt>bs3_surface</tt> (parameterized in s and t), along the given side only (0 => t=0, 1 => t=1, 2 => s=0, 3 => s=1).
 * <br><br>
 * <b>Role:</b> If the enum <i>err_type</i> is of type <tt>exact_error</tt>, then the function
 * <tt>VBL_SURF_ERROR_FUNCTION::find_maximum</tt> will be used to find the maximum errors over each of the
 * spans of the supplied side.
 * <br><br>
 * If <i>err_type</i> is of type <tt>approximate_error</tt>, then the error over the side will simply be approximated
 * to be the maximum of the errors at the mid-points of the spans.
 * <br><br>
 * If <i>err_type</i> is of type <tt>approx_if_too_big</tt>, then the function will start to calculate the errors
 * using the find_maximum function. If the error exceeds the requested tolerance, <i>err_type</i> will be
 * changed to <tt>approximate_error</tt>, and the errors will be approximated from this point onwards.
 * <br><br>
 * @param bs3_sf
 * approaching surface.
 * @param i
 * number of sides.
 * @param side
 * which side.
 * @param err_type
 * how to handle errors.
 * @param requested_fit
 * tolerance.
 */
    double st_measure_side_fit( bs3_surface bs3_sf, int i, int side,
				error_type &err_type =
				*( error_type * ) NULL_REF,
				double requested_fit = 0.0 ) const;

    // Function to find the maximum error on the interior of a four-sided
    // patch of the VBL_SURF and its approximating bs3_surface
    // (parameterised in s and t). If the enum err_type is of type
    // exact_error, then the function VBL_SURF_ERROR_FUNC_2V::turning_point
    // will be used to find the errors over each of the spans in the interior
    // of the surface; if err_type is of type approximate_error, then the
    // error over the interior will simply be approximated to be the maximum
    // of the errors at the mid-points of the spans; if err_type is of type
    // approx_if_too_big, then the function will start to calculate the errors
    // using the turning_point function, but if the error exceeds the requested
    // tolerance, err_type will be changed to approximate_errors, and the
    // errors will be approximated from this point onwards.
/*
// tbrv
*/
/**
 * @nodoc
 */
    friend class VBL_SURF_ERROR_FUNC_2V;
/**
 * Finds the maximum error on the interior of a four-sided patch of the <tt>VBL_SURF</tt> and its approximating <tt>bs3_surface</tt> (parameterized in s and t).
 * <br><br>
 * <b>Role:</b> If <i>err_type</i> is of type <tt>approximate_error</tt>, then the error over the
 * interior is simply approximated to be the maximum of the errors at the mid-points of the spans.
 * <br><br>
 * If <i>err_type</i> is of type <tt>approx_if_too_big</tt>, then the function starts to calculate
 * the errors using the <i>turning_point</i> function. But if the error exceeds the requested tolerance,
 * <i>err_type</i> is changed to <tt>approximate_errors</tt>, and the errors are approximated from this point onwards.
 * <br><br>
 * @param bs3_sf
 * surface.
 * @param i
 * number.
 * @param err_type
 * enumeration for what to do.
 * @param requested_fit
 * tolerance.
 */
    double st_measure_interior_fit( bs3_surface bs3_sf, int i,
				    error_type &err_type =
				    *( error_type * ) NULL_REF,
				    double requested_fit = 0.0 ) const;

    // Return the ith boundary as a 3-space curve on the heap, and return its
    // range on the blend surface (which may not be the actual range of the
    // curve) and its uv range on the blend surface.
/**
 * Returns the ith boundary as a three-space curve on the heap, its range on the blend surface (which may not be the actual range of the curve), and its <i>uv</i> range on the blend surface.
 * <br><br>
 * @param i
 * ith boundary.
 * @param start_par
 * start parameter.
 * @param end_par
 * end parameter.
 * @param start_uv
 * start uv-parameter.
 * @param end_uv
 * end uv-parameter.
 */
    virtual curve*	boundary_curve( int i, double& start_par,
					double& end_par,
					SPApar_pos& start_uv = *(SPApar_pos*)NULL_REF,
					SPApar_pos& end_uv =
					*(SPApar_pos*)NULL_REF ) const;

    // Whether a boundary is degenerate.
/**
 * Returns whether the ith boundary is degenerate.
 * <br><br>
 * @param i
 * ith boundary.
 */
    virtual logical	boundary_degenerate( int i ) const;

    // spl_sur member functions:

    // For VBL_OFFSURFs it returns offset evaluations, obviously enough.
    // For all kinds of vertex blend this function applies the
    // "blending" into the approx sf for uv values outside the proper
    // polygon of definition. It calls _eval_with_offset to find out
    // whether it needs the true surface evaluations, and what they are.
/**
 * Evaluates the vertex blend surface up to two derivatives.
 * <br><br>
 * @param param_val
 * parameter value.
 * @param pt
 * point on surface.
 * @param first_der
 * first derivatives.
 * @param second_der
 * second derivatives.
 */
    virtual void	eval( SPApar_pos const& param_val, SPAposition& pt, SPAvector* first_der = NULL,
			      SPAvector* second_der = NULL ) const;

    // This will do finite differencing to get extra derivatives (above
    // the normal 2). It is redefined for VBL_OFFSURFs merely to stop it
    // doing this as they're pretty hopeless - even the 2nd derivs are
    // not very precise.
/**
 * Calculates derivatives, of any order up to the number requested, and stores them in vectors provided by the user.
 * <br><br>
 * <b>Role:</b> This method returns the number it was able to calculate; this equals to the number
 * requested in all but the most exceptional circumstances. A certain number are evaluated directly
 * and accurately; higher derivatives are automatically calculated by finite differencing; the
 * accuracy of these decreases with the order of the derivative, as the cost increases. Any of the
 * pointers may be <tt>NULL</tt>, in which case, the corresponding derivatives are not returned. Otherwise
 * they must point to arrays long enough for all the derivatives of that order; i.e., 2 for the
 * first derivatives, 3 for the second, etc.
 * <br><br>
 * @param par
 * parameter.
 * @param pt
 * pt on surface.
 * @param deriv
 * array of ptrs to arrays of vectors.
 * @param nd
 * number of derivatives.
 * @param loc
 * eval. location.
 */
    virtual int evaluate( const SPApar_pos& par,
			  SPAposition & pt,
			  SPAvector **deriv = NULL,  // Array of pointers to vectors
			  int nd = 0,       // of size (nd*(nd+3))/2
			  evaluate_surface_quadrant loc = evaluate_surface_unknown
			  ) const;

    // Find an outward direction from the surface at a point with
    // given SPAparameter values.
/**
 * Finds an outward direction from the surface at a point with given parameter values.
 * <br><br>
 * @param pos
 * position.
 */
    virtual SPAunit_vector eval_outdir(
				    SPApar_pos const &pos
				    ) const;

    // Return a direction which points outward from the surface. This
    // should be the outward normal if the point is not singular,
    // otherwise a fairly arbitrary outward direction.
/**
 * Returns a direction that points outward from the surface.
 * <br><br>
 * <b>Role:</b> This should be the outward normal if the point is not singular; otherwise, it is an
 * arbitrary outward direction.
 * <br><br>
 * @param pos
 * position on surface.
 * @param param_val
 * parameter value.
 */
    virtual SPAunit_vector point_outdir(
				     SPAposition const &pos,
				     SPApar_pos const &param_val = *(SPApar_pos *)NULL_REF
				     ) const;

    // Special versions of some spl_sur member functions which can be called
    // whether or not there is an approximating surface.
/**
 * Returns the bound of the variable blend, whether or not there is an approximating surface.
 */
    virtual SPAbox		vbl_bound() const;
/**
 * Returns the variable blend <i>u</i>-parameter range, whether or not there is an approximating surface.
 */
    SPAinterval		vbl_param_range_u() const;
/**
 * Returns the variable blend <i>v</i>-parameter range, whether or not there is an approximating surface.
 */
    SPAinterval		vbl_param_range_v() const;
/**
 * Returns the vertex blend parameter range, whether or not there is an approximating surface.
 */
    SPApar_box		vbl_param_range() const;

    // Return the continuity of the surface.
/**
 * Returns the continuity at the surface boundaries.
 */
    int			get_continuity() const;

    // Param works ok, though it's not too quick. It is known to have
    // difficulties if you give it a point near the centre of curvature of a
    // portion of the surface. However, the worst you should get back in these
    // cases is the closest boundary point. If you're very very unlucky you
    // might get the closest corner of the patch.

/**
 * Returns the parameter values of a point on the vertex blend surface.
 * <br><br>
 * <b>Role:</b> The parameter is that for the intersection of the approximating surface with the true
 * surface normal at the given point.
 * <br><br>
 * @param pt
 * point.
 * @param par
 * parameter guess.
 */
    virtual SPApar_pos	param( const SPAposition& pt,
			       const SPApar_pos& par = *(SPApar_pos*)NULL_REF ) const;

    // Wouldn't need to provide our own param_unitvec were it not for
    // degenerate blend singularities.
/**
 * Finds the change in the surface parameter corresponding to a unit offset in a given direction at a given position on the surface.
 * <br><br>
 * @param nor
 * normal to surface.
 * @param param_val
 * parameter value.
 */
    virtual SPApar_vec 	param_unitvec( const SPAunit_vector& nor,
				       const SPApar_pos& param_val) const;

/**
 * Finds the point on the surface nearest to the given point, iterating from the given parameter values (if supplied). This function returns the found point, the normal to the surface at that point, and the parameter values at the found point.
 * <br><br>
 * @param pos1
 * given point.
 * @param pos2
 * point on surface.
 * @param vec
 * normal to surface.
 * @param curv
 * principal curvature.
 * @param par_pos1
 * guess parameter value.
 * @param par_pos2
 * guess parameter value.
 * @param l
 * weak flag.
 * @param box
 * bounding box.
 */
    virtual void	point_perp( const SPAposition& pos1, SPAposition& pos2,
				    SPAunit_vector& vec, surf_princurv& curv,
				    const SPApar_pos& par_pos1, SPApar_pos& par_pos2, logical l=FALSE,
                    const SPApar_box & box = *(SPApar_box *)NULL_REF ) const;
/**
 * Tests whether a given point lies on the surface to within a given fit tolerance.
 * <br><br>
 * <b>Role:</b> The parameter value of the closest point on the surface to the given point is returned.
 * <br><br>
 * @param pt
 * given point.
 * @param tol
 * fit tolerance.
 * @param guess_val
 * guess parameter.
 * @param actual_val
 * actual parameter.
 */
    virtual logical	test_point_tol(	const SPAposition& pt, double tol, const SPApar_pos& guess_val,
					SPApar_pos& actual_val ) const;
/**
 * Transform this blend by the given transformation.
 * <br><br>
 * @param trans
 * transformation
 */
    virtual void	operator*=( const SPAtransf& trans);

/**
 * Divides a surface into two pieces at the given <i>u</i>-parameter value.
 * <br><br>
 * <b>Role:</b> If the split is at the end of the parameter range, the <tt>spl_sur</tt> returns as the
 * appropriate half (in increasing parameter order), and the other is <tt>NULL</tt>. Otherwise, a new <tt>spl_sur</tt>
 * is used for one part, and the old one is modified for the other.
 * <br><br>
 * @param uparam
 * u-parameter value.
 * @param piece
 * two pieces.
 */
    virtual void	split_u( double uparam, spl_sur* piece [ 2 ] );
/**
 * Divides a surface into two pieces at the given <i>v</i>-parameter value.
 * <br><br>
 * <b>Role:</b> If the split is at the end of the parameter range, the <tt>spl_sur</tt> returns as the
 * appropriate half (in increasing parameter order), and the other is <tt>NULL</tt>. Otherwise, a new <tt>spl_sur</tt>
 * is used for one part, and the old one is modified for the other.
 * <br><br>
 * @param vparam
 * v-parameter value.
 * @param piece
 * two pieces.
 */
    virtual void	split_v( double vparam, spl_sur* piece [ 2 ] );

/**
 * Returns the <i>u</i>-parameter line, given a <i>v</i> value.
 * <br><br>
 * @param vval
 * v value.
 * @param sur
 * owning surface.
 */
    virtual curve*	u_param_line( double vval, const spline& sur) const;
/**
 * Returns the <i>v</i>-parameter line, given a <i>u</i> value.
 * <br><br>
 * @param uval
 * u value.
 * @param sur
 * owning surface.
 */
    virtual curve*	v_param_line( double uval, const spline& sur) const;

/**
 * Constructs a duplicate of this object in free storage.
 */
    subtrans_object*	copy() const;

    // The following do nothing:

/**
 * Appends the given surface to the end of this surface in the <i>u</i>-parameter direction.
 * <br><br>
 * <b>Role:</b> This function is intended to be called after <tt>split_u</tt> has been called, so that the
 * surfaces are guaranteed to be compatible.
 * <br><br>
 * @param spline_sur
 * spline surface.
 */
    virtual void	append_u( spl_sur& spline_sur);
/**
 * Appends the given surface to the end of this surface in the <i>v</i>-parameter direction.
 * <br><br>
 * <b>Role:</b> This function is intended to be called after <tt>split_v</tt> has been called, so that the
 * surfaces are guaranteed to be compatible.
 * <br><br>
 * @param spline_sur
 * spline surface.
 */
    virtual void	append_v( spl_sur& spline_sur);

    // Acis i/o of various sorts.
/**
 * Returns the <tt>ID</tt> for the <tt>VBL_SURF</tt> list.
 */
    LOCAL_PROC int		id();
/**
 * Returns the type of <tt>VBL_SURF</tt>.
 */
    int				type() const;
/**
 * Returns the string "vertexblendsur".
 */
    const char*		type_name() const;
/**
 * Writes debug information to the specified output file.
 * <br><br>
 * @param str
 * leader string.
 * @param output
 * brief output OK.
 * @param fp
 * file pointer.
 */
    virtual void	debug( char const* str, logical output, FILE* fp) const;
/**
 * Saves the information for the <tt>VBL_SURF</tt> to a save file.
 * <br><br>
 * <b>Role:</b> Stores the information from this class to the save file. This method is never called
 * directly. It is called by a higher hierarchical function if an item in the SAT file is determined
 * to be of this class type.
 */
    virtual void	save_data() const;

/**
 * @nodoc
 */
	virtual logical need_save_as_approx(int save_to_version, logical check_progenitors) const;

/**
 * Restores the data for a <tt>VBL_SURF</tt> from a save file.
 * <br><br>
 * <b>Role:</b> This method is never called directly. It is called by a higher hierarchical function
 * if an item in the SAT file is determined to be of this class type. An instance of this class will
 * already have been created through the allocation constructor. This method then populates the class
 * instance with the appropriate data from the SAT file.
 */
    void		restore_data();


    // Return how many curve derivatives are available over the given SPAinterval
    // (which defaults to the whole range); if present, the argument "nacc"
    // returns the number which are accurate.

/**
 * Returns the number of curve derivatives over the given interval (which defaults to the whole range).
 * <br><br>
 * @param box
 * bounding box.
 */
    virtual int accurate_derivs( const SPApar_box& box = *(SPApar_box*)NULL_REF ) const;

    // Obviously this must include the effect of any offset (for
    // Make an offset vertex blend on the heap. May also be called on an
    // already offset vertex blend. These two are the only VBL_SURF
    // functions in vbl_off.cxx.

/**
 * Makes an offset vertex blend on the heap.
 * <br><br>
 * <b>Role:</b> May also be called on an already offset vertex blend. These two are the only <tt>VBL_SURF</tt>
 * functions in <i>vbl_off.cxx</i>.
 * <br><br>
 * @param offset_dist
 * offset distance.
 */
    virtual VBL_OFFSURF* offset( double offset_dist ) const;

    // Suggest a range of legitimate offset distances. Based on a crude
    // sampling method, but nonetheless better than nothing.

/**
 * Suggests a range of legitimate offset distances.
 */
    virtual SPAinterval	suggest_offset() const;


    // This function is used in vbl_bound.
/*
// tbrv
*/
/**
 * @nodoc
 */
    friend class VBL_FUNCTION;
/*
// tbrv
*/
/**
 * @nodoc
 */
    friend void VBL_fd_diff_derivs( const VBL_SURF&, const SPApar_pos&,
				    SPAvector**, const int, int,
				    const SPApar_vec&, const SPApar_vec& );

	/**
	* @nodoc
	**/
	void process( geometry_definition_processor& p ) const;

#if defined D3_DEBUG
/*
// tbrv
*/
/**
 * @nodoc
 */
    void	print( D3_ostream& os ) const;
/*
// tbrv
*/
/**
 * @nodoc
 */
    void	input( D3_istream& is );
/*
// tbrv
*/
/**
 * @nodoc
 */
    void	plot_Q( FILE*, int ) const;
#endif

    // STI let (r3995): Made this function a friend
    // It should not be documented.
    // It should not be called by customers.
/*
// tbrv
*/
/**
 * @nodoc
 */
    friend SPApar_pos param_internal( VBL_SURF& vbl,
                                   const SPAposition& iX,
                                   logical on_surf,
                                   const SPApar_pos& uv_guess );
    // STI let: end

    };


// This class stores a list of degenerate vertex blend boundaries. The
// VBL_BISPAN needs to know these so that it can tell whether there are any
// degenerate edges to split off.
/*
// tbrv
*/
/**
 * @nodoc
 */
class DECL_KERN VBL_SING_LIST : public ACIS_OBJECT
    {
private:
    // The number of the degenerate boundary, as recognised by the VBL_SURF.

    int			_boundary;

    // The next degenerate boundary, or NULL.

    VBL_SING_LIST*	_next;

    // Set to TRUE when we trim back from this singularity.

    int			_trimmed;

    // The starting uv value of the new boundary created by trimming back.

    SPApar_pos		_uv0;

    // The ending uv value of the new boundary created by trimming back.

    SPApar_pos		_uv1;

    // The amount of the following boundary (_boundary+1) which we 'accidentally'
    // snipped off in trimming back from this singularity.

    double		_trim_next;

    // The amout of the previous boundary (_boundary-1) which we 'accidentally'
    // snipped off in trimming back from this singularity.

    double		_trim_prev;

    // The new curve (actually a par_int_cur intcurve) which gives the new
    // boundary introduced by trimming back.

    curve*		_cur;

public:
    VBL_SING_LIST( int, VBL_SING_LIST* = NULL );
    VBL_SING_LIST( const VBL_SING_LIST& );
    ~VBL_SING_LIST();

    // Delete the entire list from this one onwards.

    void		kill();

    // Copy the entire list from this one onwards.

    VBL_SING_LIST*	copy() const;

    int 		boundary() const { return _boundary; };
    VBL_SING_LIST*	next() const { return _next; };
    int			trimmed() const { return _trimmed; };
    curve*		cur() const { return _cur; };
    const SPApar_pos&	uv0() const { return _uv0; };
    const SPApar_pos&	uv1() const { return _uv1; };
    double		trim_next() const { return _trim_next; };
    double		trim_prev() const { return _trim_prev; };

    // Return this one if it's untrimmed, else zip along to the next untrimmed
    // one (or NULL if there isn't).

    VBL_SING_LIST*	check_untrimmed();

    // How many there are from this one onwards.

    int			n() const;

    // How many untrimmed ones there are from here onwards.

    int 		n_untrimmed() const;

    // Mark this degenerate edge as trimmed with a new boundary curve from uv0
    // to uv1. trim_next is how much of the next boundary (as a fraction from 0
    // to 1) we're also removing, so it's really related to uv1. trim_prev
    // likewise for the previous boundary, so it's related to uv0. In both cases
    // a value of 0 would mean 'we haven't touched this bit of boundary' so in
    // practice these values should be >0, and <1/3, say, as we mustn't snip too
    // much off.

    void		trim( const SPApar_pos& uv0, const SPApar_pos& uv1,
			     double trim_next, double trim_prev );

    // Store the given curve as the new boundary curve.

    void		set_cur( curve* cur ) { _cur = cur; };
    };


#if defined D3_DEBUG

/*
// tbrv
*/
/**
 * @nodoc
 */

DECL_KERN D3_ostream& operator<<( D3_ostream& os, const VBL_SURF& sf );

/*
// tbrv
*/
/**
 * @nodoc
 */

DECL_KERN D3_istream& operator>>( D3_istream& is, VBL_SURF& sf );

/*
// tbrv
*/
/**
 * @nodoc
 */

DECL_KERN D3_ostream& operator<<( D3_ostream& os, blend_coord_type coord_type );
#endif

#if defined D3_DEBUG
/**
 * @nodoc
 */
#define VBL_warning( level, stream )		  		\
    if ( _warnings() >= level )                  		\
	{ ERR_warning( stream ); }
#else
/**
 * @nodoc
 */
#define VBL_warning( level, stream )
#endif


/** @} */
#endif
