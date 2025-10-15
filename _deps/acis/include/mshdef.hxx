/*******************************************************************/
/*    Copyright (c) 1989-2020 by Spatial Corp.                     */
/*    All rights reserved.                                         */
/*    Protected by U.S. Patents 5,257,205; 5,351,196; 6,369,815;   */
/*                              5,982,378; 6,462,738; 6,941,251    */
/*    Protected by European Patents 0503642; 69220263.3            */
/*    Protected by Hong Kong Patent 1008101A                       */
/*******************************************************************/

#if !defined( meshsurf_CLASS )
#define meshsurf_CLASS

#include "acis.hxx"
#include "dcl_kern.h"
#include "logical.h"
#include "surdef.hxx"
#include "scandef.hxx"
#include "tree.hxx"
#include "debugmsc.hxx"
#include "transf.hxx"
#include "dispatch.hxx"
#include "msh_sur.hxx"

class meshsurf;
class ENTITY;
class curve;
class SPAposition;
class SPAvector;
class SPAunit_vector;
class SPAinterval;
class SPAbox;
class SPApar_box;
class SizeAccumulator;
typedef logical REVBIT;

DECL_KERN meshsurf operator*(meshsurf const &, SPAtransf const & );
DECL_KERN surface *restore_meshsurf();


// Define an identifying type for this (lower-case) surface.
#define meshsurf_type 30

/**
 * @nodoc
 * This class records a composite mesh surface.
 * A meshsurf holds a pointer to a msh_sur, an internal class defined
 * defined so that we can use use-counts to avoid too much copying,
 * and a logical denoting reversal of the sense of the stored surface.
 * The msh_sur, which will always in fact be an object of a class
 * derived from msh_sur, contains the nodes/elements definition of the
 * surface.  All access to the msh_sur is done via virtual
 * functions, which may therefore be adapted to different mesh
 * types.
*/
class DECL_KERN meshsurf : public surface {

	msh_sur *msh;		// Mesh surface

	logical rev;		// True if this surface opposes the direction
						// of the underlying one.


	// Ensure that the reference supplied points to a singly-used 
	// record.  Take no action if it is already single, otherwise 
	// copy everything.

	void make_single_ref();

	// Transform in place without juggling references.  Only to
	// be called from entity level.

	void hard_transform( const SPAtransf & );

public:
    /**
	 * Inline NULL constructor.
     */
	meshsurf() { msh = NULL; rev = FALSE; }
    /**
	 * Copy constructor.  Does a use-count (deferred) copy.
     */
	meshsurf( meshsurf const & );
    /**
	 * Construct a surface given a msh_sur.  This is simply a new
	 * use of the msh_sur.
     */
	meshsurf( msh_sur * );
    /**
	 * Remove the use of the underlying surface (decrement use-count).
     */
	virtual ~meshsurf();
    /**
	 * Need a lose function because the nodes and elements are
	 * entities.
     */
	void lose();
    /**
	 * Make a copy of this meshsurf on the heap.  Use-count (deferred)
	 * copy.
     */
	virtual surface *make_copy() const;
    /**
	 * Make a copy without any sharing of subdata.
     */
	virtual surface *deep_copy(pointer_map * pm = NULL) const;
    /**
	 * Fill a msh_sur with all the node, element, and attribute
	 * data in the msh_sur.
     */
	void scan();
    /**
	 * Fix pointers within a recently copied or restored msh_sur.
     */
	void fix_pointers( ENTITY** );
    /**
	 * Read information from the surface definition
     */
	logical reversed() const { return rev; }
    /**
	 * Read the underlying mesh surface.  Not const, because we
	 * expect fiddling if this is called.  Can only be called
	 * from the equation_for_update version of MESHSURF to mesh
	 * surf access, and therefore means that backup() is called.
     */
	msh_sur &get_msh_sur() { return *msh; }
    /**
	 * The const one.  Look but don't touch.
     */
	const msh_sur &get_msh_sur() const { return *msh; }
    /**
	 * Return a surface with a reversed sense.  Calls the copy
	 * constructor (and is therefore a use-count copy).
     */
	meshsurf operator-() const;
    /**                                                      
	 * Assignment.  Merely copy the meshsurf record, and increment the 
	 * use count of the msh_sur.  More or less the same as the copy
	 * constructor.
     */
	meshsurf &operator=( meshsurf const & );
    /**
	 * Test two surfaces for equality.  DOES NOT use a tolerance,
	 * and therefore usually only valid for testing copies.
     */
	virtual logical operator==( surface const & ) const;
    /**
	 * Return a transformed copy of the meshsurf.  Makes
	 * a deferred copy, then calls operator*=.
     */
	friend DECL_KERN meshsurf operator*( meshsurf const &, SPAtransf const & );
    /**
	 * Transform this meshsurf in place by the given SPAtransf.
	 * This makes a new ref if needed.
     */
	virtual surface &operator*=( SPAtransf const & );
    /**
	 * Negate this meshsurf in place.
     */
	virtual surface &negate();
    /**
	 * This just checks the msh pointer against NULL.
     */
	virtual logical undef() const;
    /**
	 * Change the neighborhood data.
     */
	virtual void set_neighborhood(const neighborhood &);
    /**
	 * Return a SPAbox around the surface. This may not be a tight box,
	 * depending on cost.
	 * Surface bounded in parameter space.
     */
	virtual SPAbox bound(
				SPApar_box const & = *(SPApar_box *)NULL_REF,
				SPAtransf const & = *(SPAtransf *)NULL_REF
			) const;
    /**
	 * Surface bounded in object space.
     */
	virtual SPAbox bound(
				SPAbox const &,
				SPAtransf const & = *(SPAtransf *)NULL_REF
			) const;
    /**
	 * Return a cone bounding the normal direction of the surface. The
	 * logical argument indicates whether an approximation is ok.
     */
	virtual surf_normcone normal_cone(
				SPApar_box const &,	 // parameter bounds
				logical = FALSE,
				SPAtransf const & = *(SPAtransf *)NULL_REF
			) const;
    /**
	 * Return whether the mesh is considered parametric.
     */
	virtual logical parametric() const;
    /**
	 * Find the normal to the surface at the given point.
     */
	virtual SPAunit_vector point_normal(
				SPAposition const &,
				SPApar_pos const & = *(SPApar_pos *)NULL_REF
			) const;
    /**
	 * Find an outward direction from the surface at a point on
	 * the surface nearest to the given point.
     */
	virtual SPAunit_vector point_outdir(
				SPAposition const &,
				SPApar_pos const & = *(SPApar_pos *)NULL_REF
			) const;
    /**
	 * Find the principal axes of curvature of the surface at a
	 * given point, and the curvatures in those directions.
     */
	virtual void point_prin_curv(
				SPAposition const &,
				SPAunit_vector &,		// first axis direction
				double &,			// curvature in first direction
				SPAunit_vector &,		// second axis direction
				double &,			// curvature in second direction
				SPApar_pos const & = *(SPApar_pos *)NULL_REF
			) const;
    /**
     *
     */
	surf_princurv point_prin_curv(
				SPAposition const &pos,
				SPApar_pos const &param_guess = *(SPApar_pos *)NULL_REF
			) const
	{
		return surface::point_prin_curv( pos, param_guess );
	}
    /**
	 * Find the curvature of a cross-section curve of the surface at
	 * the point on the surface closest to the given point.
     */
	virtual double point_cross(
				SPAposition const &,
				SPAunit_vector const &,
				SPApar_pos const & = *(SPApar_pos *)NULL_REF
			) const;
    /**
	 * Find the point on the surface nearest to the given point and 
	 * optionally the normal to and principal curvatures of the
	 * surface at that point.  If the surface is parametric, also
	 * return the parameter values at the found point.
     */
	virtual void point_perp(
				SPAposition const &,
				SPAposition &,
				SPAunit_vector &,
				surf_princurv &,
				SPApar_pos const & = *(SPApar_pos *)NULL_REF,
				SPApar_pos & = *(SPApar_pos *)NULL_REF,
				logical f_weak = FALSE
			) const;
    /**
	 * Find the point on the surface nearest to the given point and 
	 * optionally the normal to and principal curvatures of the
	 * surface at that point.  If the surface is parametric, also
	 * return the parameter values at the found point.
     */
	void point_perp(
				SPAposition const &pos,
				SPAposition &foot,
				SPAunit_vector &norm,
				SPApar_pos const &param_guess = *(SPApar_pos *)NULL_REF,
				SPApar_pos &param_actual = *(SPApar_pos *)NULL_REF,
				logical f_weak = FALSE
			) const
	{
		point_perp(
				pos,
				foot,
				norm,
				*(surf_princurv *)NULL_REF,
				param_guess,
				param_actual, f_weak
			);
	}
    /**
	 * Find the point on the surface nearest to the given point and 
	 * optionally the normal to and principal curvatures of the
	 * surface at that point.  If the surface is parametric, also
	 * return the parameter values at the found point.
     */
	void point_perp(
				SPAposition const &pos,
				SPAposition &foot,
				SPApar_pos const &param_guess = *(SPApar_pos *)NULL_REF,
				SPApar_pos &param_actual = *(SPApar_pos *)NULL_REF,
				logical f_weak = FALSE
			) const
	{
		point_perp(
				pos,
				foot,
				*(SPAunit_vector *)NULL_REF,
				*(surf_princurv *)NULL_REF,
				param_guess,
				param_actual, f_weak
			);
	}
    /**
	 * Find the parameter values of a point on a meshsurf surface,
	 * iterating from the given parameter values (if supplied).
     */
	virtual SPApar_pos param(
				SPAposition const &,
				SPApar_pos const & = *(SPApar_pos *)NULL_REF
			) const;
    /**
	 * Find the change in surface parameter corresponding to a unit
	 * offset in a given direction at a given position.
     */
	virtual SPApar_vec param_unitvec(
				SPAunit_vector const &,
				SPApar_pos const &
			) const;

    /**
	 * Find the SPAposition and first and second derivatives of the
	 * surface at a given point.
     */
	virtual void eval(
				SPApar_pos const &uv,
				SPAposition &pos,
				SPAvector *dpos = NULL,// first derivatives - array of
									// length 2 in order xu, xv
				SPAvector *ddpos = NULL // second derivatives - array of
									// length 3 in order xuu, xuv, xvv
			) const;
    /**
	 * Find the point on the meshsurf with given parameter values.
     */
	virtual SPAposition eval_position( SPApar_pos const & ) const;
    /**
 	 * Find the normal to the meshsurf at the point with given 
	 * parameter values.
     */
	virtual SPAunit_vector eval_normal( SPApar_pos const & ) const;
    /**
	 * Find an outward direction from the surface at a point with
	 * given parameter values.
     */
	virtual SPAunit_vector eval_outdir( SPApar_pos const & ) const;
    /**
	 * Find the principal axes of curvature of the surface at a
	 * point with given parameter values, and the curvatures in those
	 * directions.
     */
	virtual void eval_prin_curv(
				SPApar_pos const &,
				SPAunit_vector &,		// first axis direction
				double &,			// curvature in first direction
				SPAunit_vector &,		// second axis direction
				double &			// curvature in second direction
			) const;
	surf_princurv eval_prin_curv(
				SPApar_pos const &param
			) const
	{
		return surface::eval_prin_curv( param );
	}
    /**
	 * Find the curvature of a cross-section curve of the surface at
	 * the point on the surface with given parameter values.
     */
	virtual double eval_cross( SPApar_pos const &, SPAunit_vector const & ) const;
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
	 * Report whether a parametric surface is periodic in u
	 * parameter direction (i.e. it is smoothly closed, so faces can
	 * run over the seam).
     */
	virtual logical periodic_u() const;
    /**
	 * Report whether a parametric surface is periodic in v
	 * parameter direction (i.e. it is smoothly closed, so faces can
	 * run over the seam).
     */	virtual logical periodic_v() const;
    /**
	 * Report whether the surface is closed, smoothly or not, in
	 * u parameter direction.
     */
	virtual logical closed_u() const;
    /**
	 * Report whether the surface is closed, smoothly or not, in
	 * v parameter direction.
     */
    virtual logical closed_v() const;
    /**
	 * Return the period of a periodic parametric surface, zero if
	 * the surface is not periodic in the u parameter or not
	 * parametric.
     */
	virtual double param_period_u() const;
    /**
	 * Return the period of a periodic parametric surface, zero if
	 * the surface is not periodic in the v parameter or not
	 * parametric.
     */
    virtual double param_period_v() const;
    /**
	 * Return the principal parameter range of a parametric surface in
	 * a chosen parameter direction, or in both.  The range can
	 * be limited to a provided SPAbox.
     */
	virtual SPApar_box param_range( SPAbox const & = *(SPAbox *)NULL_REF ) const;
    /**
	 * Return the principal parameter range of a parametric surface in
	 * u parameter direction.  The range can
	 * be limited to a provided SPAbox.
     */
    virtual SPAinterval param_range_u( SPAbox const & = *(SPAbox *)NULL_REF ) const;
    /**
	 * Return the principal parameter range of a parametric surface in
	 * v parameter direction.  The range can
	 * be limited to a provided SPAbox.
     */
    virtual SPAinterval param_range_v( SPAbox const & = *(SPAbox *)NULL_REF ) const;
    /**
	 * Return whether the surface parametrization is singular in the u direction.
     */
	virtual logical singular_u(double) const;
    /**
	 * Return whether the surface parametrization is singular in the v direction.
     */	
    virtual logical singular_v(double) const;
    /**
	 * Indicate whether the parameter coordinate system of the surface
	 * is right- or left-handed.
     */
	virtual logical left_handed_uv() const;
    /**
	 * Construct a parameter line on the surface. A u parameter line
	 * runs in the direction of increasing u parameter, at constant v.
	 * The curve parametrization is that of the surface.
     */
	virtual curve *u_param_line( double ) const;
    /**
	 * Construct a parameter line on the surface. A u parameter line
	 * runs in the direction of increasing u parameter, at constant v.
	 * The curve parametrization is that of the surface.
     */
	virtual curve *v_param_line( double	) const;
    /**
	 * Test whether a point lies on the surface, to user-defined
	 * tolerance.
     */
	virtual logical test_point_tol(
				SPAposition const &,
				double = 0,		// defaults to SPAresabs
				SPApar_pos const & = *(SPApar_pos *)NULL_REF,
				SPApar_pos & = *(SPApar_pos *)NULL_REF
			) const;
    /**
	 * Return type of (lower-case) surface.
     */
	virtual int type() const;
    /**
	 * Return string identifier of surface.
     */
	virtual char const *type_name() const;
    /**
	 * Save and restore.
     */
	virtual void save() const;
	virtual void save(ENTITY_LIST &) const;
	void save_data() const;
	void save_data(ENTITY_LIST &) const;
	friend DECL_KERN surface *restore_meshsurf();
	void restore_data();


	virtual void full_size(SizeAccumulator&, logical = TRUE) const;

    /**
	 * Print out details of meshsurf.
     */
	virtual void debug(
				char const *,
				FILE * = debug_file_ptr
			) const;
    /**
	 * Make the entity surface class a friend to allow
	 * private access.
     */
	friend class MESHSURF;
};






#endif
