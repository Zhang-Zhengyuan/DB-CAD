/* ORIGINAL: acis2.1/kerngeom/splsur/pipe_spl.hxx */
/* $Id: pipe_spl.hxx,v 1.27 2002/08/20 15:20:22 jenglund Exp $ */
/*******************************************************************/
/*    Copyright (c) 1989-2020 by Spatial Corp.                     */
/*    All rights reserved.                                         */
/*    Protected by U.S. Patents 5,257,205; 5,351,196; 6,369,815;   */
/*                              5,982,378; 6,462,738; 6,941,251    */
/*    Protected by European Patents 0503642; 69220263.3            */
/*    Protected by Hong Kong Patent 1008101A                       */
/*******************************************************************/

// Header for a pipe, a class derived from spl_sur, that represents
// a surface which is the envelope of a fixed-radius circle centred
// on a point on a given curve, and normal to the curve at that
// point. In fact, this is derived from tube_spl_sur rather than
// direct from spl_sur, in order to ensure continued support for
// the old-style "tube", which is no longer used by blending.

// The main difference between the pipe and the tube is in its
// parametrisation. The old "tube" obtained its parametrisation from
// the spline approximation, which made evaluations a bit slow,
// prevented our constructing circles for u SPAparameter lines, and made
// the bounding of the true surface rather dubious. The new "pipe"
// uses a parametrisation based on the spine parametrisation in the
// v direction, and on angle measured from a "zero curve" in the u
// direction. Quite a few operations can be made more rigorous as well
// as speeded up by this change. Many functions are the same, though,
// so we simply inherit them from tube_spl_sur.

// All new applications should use pipe_spl_sur rather than
// tube_spl_sur, and some time in the future tubes will be eliminated.

#if !defined( pipe_spl_sur_CLASS )
#define pipe_spl_sur_CLASS

#include "logical.h"

#include "acis.hxx"
#include "dcl_kern.h"

#include "tube_spl.hxx"

#include "bs2curve.hxx"

#include "debugmsc.hxx"

class curve;
class SPApar_pos;
class SPAposition;
class SPAvector;
class SPAunit_vector;
class SPAtransf;

// STI ROLL
class SizeAccumulator;
// STI ROLL

#if defined D3_STANDALONE || defined D3_DEBUG
DECL_KERN D3_istream& operator>>( D3_istream &, spl_sur *& );
DECL_KERN D3_ostream& operator<<( D3_ostream &, spl_sur const & );
#endif

class SPAbox;
class SPApar_box;
DECL_KERN subtype_object *restore_pipe_spl_sur();

class DECL_KERN pipe_spl_sur: public tube_spl_sur {

protected:

	// Additionally to the spine and radius, we store a curve
	// which defines the zero u SPAposition at each v SPAparameter value.
	// It should be as close as sensible to the true zero SPAposition,
	// but does not need to be precise.

	curve *zero;		// curve parametrised the same as spine
						// defining the zero u SPAparameter direction.

	// Constructors.

	// Construct a generally null pipe. This is to allow flexibility
	// for constructors for derived classes to set the common data
	// members in whatever way is most convenient.

	pipe_spl_sur();

    // Calculate the discontinuity information from the spine curve. 

    virtual	void 	calculate_disc_info();

public:

	// Make a copy without sharing subdata.

	virtual spl_sur *deep_copy(pointer_map * pm = NULL) const;

	// Construct a pipe with given radius and spine curve when the
	// interpolating surface has already been found.  Makes  copies
	// of the given curves.

	pipe_spl_sur(
			double,				// radius of pipe
			curve const &,		// spine curve
			curve const &,		// u zero-direction curve
			SPAinterval const &	// range of u SPAparameter (angle)
		);

	// Construct a pipe with given radius and boundary curves,
	// possibly constructing SPAparameter-space curves for the
	// boundaries as well.

	pipe_spl_sur(
			double,				// radius of pipe
			curve const &,		// spine curve
			curve const &,		// low-u boundary curve
			curve const &,		// high-u boundary curve
			bs2_curve &,		// set to low-u boundary pcurve
			bs2_curve &			// set to high-u boundary pcurve
		);

	// Copy constructor. 

	pipe_spl_sur( const pipe_spl_sur& );


	double pipe_radius() const { return radius; }
	curve const * pipe_spine() const { return spine; }

	// STI ROLL
	virtual void full_size(SizeAccumulator&, logical = TRUE) const;
    // STI ROLL
/**
 * @nodoc
 */
	virtual void minimize( minimize_helper*);

protected:
	// Destructor needs to delete the spine curve.

	virtual ~pipe_spl_sur();

    // Function to determine if this spl_sur is a pipe or is a wrapper
	// around a pipe surface (eg offset or subset pipe).

    virtual logical contains_pipe() const
	    { return TRUE; }

	// Construct a duplicate in free store of this object but with
	// zero use count.

	virtual subtrans_object *copy() const;


	// Make or remake the approximating surface.  The force flag forces the 
	// approximating surface to be made even if it is illegal.  This can be 
	// used to restore old parts that were not checked properly before being 
	// saved.  The spline argument 'spl' may be null but if it is supplied the
	// function may be a little faster.  The function stores the approximating 
    // surface and the actual fit error that was achieved in the spl_sur, 
    // overriding the declared const-ness of the function to do this. 

    virtual void	make_approx( 
							 double fit, 
							 const spline& spl = *(spline*) NULL_REF, 
						     logical force = FALSE
							 ) const; 


	// Test for equality. This does not guarantee that all
	// effectively equal surfaces are determined to be equal, but
	// does guarantee that different surfaces are correctly
	// identified as such.

	logical operator==( subtype_object const & ) const;


	// Transform this pipe by the given SPAtransf.

	virtual void operator*=( SPAtransf const &	);

	// Parameter shift: adjust the spline surface to have a SPAparameter
	// range increased by the argument value (which may be negative).
	// This is only used to move portions of a periodic surface by
	// integral multiples of the period. The base class implementation
	// is fine for u, but we need to adjust the subsetting of the spine
	// when shifting in v.

//	virtual void shift_u( double );
	virtual void shift_v( double );


	// Divide a surface into two pieces at a given SPAparameter value.
	// If the split is at the end of the SPAparameter range, the spl_sur
	// is just returned as the appropriate half (in increasing
	// SPAparameter order), and the other is NULL. Otherwise a new spl_sur
	// is used for one part, and the old one is modified for the other.

	virtual void split_u(
				double,
				spl_sur *[ 2 ]
			);
	virtual void split_v(
				double,
				spl_sur *[ 2 ]
			);


	// Concatenate the contents of two surfaces into one. The surfaces
	// are guaranteed to be the same base or derived type, and to have
	// contiguous SPAparameter ranges ("this" is the beginning part of
	// the combined surface (i.e. lower SPAparameter values), the
	// argument gives the end part). The base class implementation
	// is fine for u, but we need to concatenate the spine pieces when
	// joining in v.

//	virtual void append_u( spl_sur & );
	virtual void append_v( spl_sur & );


	// Geometric evaluation.

	// Find the normal to the surface at a point on the surface 
	// nearest to the given point. The tube_spl_sur function is
	// OK.

//	virtual SPAunit_vector point_normal(
//				SPAposition const &,
//				SPApar_pos const & = *(SPApar_pos *)NULL_REF
//			) const;


	// Find the principal axes of curvature of the surface at a
	// given point, and the curvatures in those directions. The
	// implementation for tube_spl_sur is fine.

//	virtual void point_prin_curv(
//				SPAposition const &,
//				SPAunit_vector &,		// first axis direction
//				double &,			// curvature in first direction
//				SPAunit_vector &,		// second axis direction
//				double &,			// curvature in second direction
//				SPApar_pos const & = *(SPApar_pos *)NULL_REF,
//              evaluate_surface_quadrant = evaluate_surface_unknown
//			) const;


	// Find the curvature of a cross-section curve of the surface at
	// the given point.  The cross-section curve is given by the
	// intersection of the surface with a plane passing through the 
	// given point and with given normal. The default using
	// point_prin_curv is the best we can do.

//	virtual double point_cross(
//				SPAposition const &,
//				SPAunit_vector const &,
//				SPApar_pos const & = *(SPApar_pos *)NULL_REF
//			) const;


	// Find the point on the surface nearest to the given point, 
	// iterating from the given SPAparameter values (if supplied).
	// Return the found point, the normal to the surface at that 
	// point and the SPAparameter values at the found point.

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


	// Find the SPAparameter values of a point on the surface. We use
	// a slightly different parametrisation from standard, to speed
	// up evaluation: the SPAparameter value is that for the intersection
	// of the approximating surface with the true surface normal at
	// the given point.

	virtual SPApar_pos param(
				SPAposition const &,
				SPApar_pos const & = *(SPApar_pos *)NULL_REF
			) const;

	// Find the change in surface SPAparameter corresponding to a unit
	// offset in a given direction at a given uv, the direction
	// lying in the surface.

 	virtual SPApar_vec param_unitvec(
						  SPAunit_vector const &,
						  SPApar_pos const &
						  ) const;

	// Find the SPAposition and first and second  derivatives of the
	// surface at a given point. The default spl_sur version would be
	// fine, but we shall have to call it explicitly to avoid getting
	// the tube_spl_sur version instead.

	virtual void eval(
				SPApar_pos const &uv,
				SPAposition &pos,
				SPAvector *dpos,	// first derivatives - array of
								// length 2 in order xu, xv
				SPAvector *ddpos	// second derivatives - array of
								// length 3 in order xuu, xuv, xvv
			) const;

	// Find the point on the surface with given SPAparameter values.
	// The default uses eval().

//	virtual SPAposition eval_position(
//				SPApar_pos const &
//			) const;


 	// Find the normal to the surface at the point with given 
	// SPAparameter values.

	virtual SPAunit_vector eval_normal(
				SPApar_pos const &
			) const;

	// Find the principal axes of curvature of the surface at a
	// point with given SPAparameter values, and the curvatures in those
	// directions. The default uses point_prin_curv.

//	virtual void eval_prin_curv(
//				SPApar_pos const &,
//				SPAunit_vector &,		// first axis direction
//				double &,			// curvature in first direction
//				SPAunit_vector &,		// second axis direction
//				double &,			// curvature in second direction
//              evaluate_surface_quadrant = evaluate_surface_unknown
//			) const;

	// Find the curvature of a cross-section curve of the surface at
	// the point on the surface with given SPAparameter values.
	// The cross-section curve is defined as the intersection of 
	// the surface with a plane passing through the point on the
	// surface and normal to the given direction, which must lie in
	// the surface. The default uses point_cross.

//	virtual double eval_cross(
//				SPApar_pos const &,
//				SPAunit_vector const &
//			) const;


	// The evaluate() function calculates derivatives, of any order
	// up to the number requested, and stores them in vectors provided
	// by the user. It returns the number it was able to calculate;
	// this will be equal to the number requested in all but the most
	// exceptional circumstances. A certain number will be evaluated
	// directly and (more or less) accurately; higher derivatives will
	// be automatically calculated by finite differencing; the accuracy
	// of these decreases with the order of the derivative, as the
	// cost increases.

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


	// The evaluate_iter() function calculates derivatives, of any order
	// up to the number requested, and stores them in vectors provided
	// by the user. It returns the number it was able to calculate;
	// this will be equal to the number requested in all but the most
	// exceptional circumstances. A certain number will be evaluated
	// directly and (more or less) accurately; higher derivatives will
	// be automatically calculated by finite differencing; the accuracy
	// of these decreases with the order of the derivative, as the
	// cost increases.

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

	// Construct a data object to retain evaluation information across
	// calls to evaluate_iter(). This is to allow subsidiary calls
	// within an iterative evaluator to start iteration much closer to the
	// required result than is possible just using the surface information
	// itself.

	virtual surface_evaldata *make_evaldata() const;


	// Return the number of derivatives which evaluate() can find
	// "accurately" (and fairly directly), rather than by finite
	// differencing, over the given portion of the surface. If there
	// is no limit to the number of accurate derivatives, returns
	// the value ALL_SURFACE_DERIVATIVES, which is large enough to be
	// more than anyone could reasonably want.

	virtual int accurate_derivs( 
				SPApar_box const & = *(SPApar_box *)NULL_REF
								 	// Defaults to the whole surface
			) const;


	// Construct a SPAparameter line on the surface. A u SPAparameter line
	// runs in the direction of increasing u SPAparameter, at constant v.
	// A v SPAparameter line runs in the direction of increasing v, at
	// constant u. The parametrisation in the non-constant direction
	// matches that of the surface, and has the range obtained by
	// use of param_range_u() or param_range_v() appropriately.

	// The new curve is constructed in free store, so it is the
	// responsibility of the caller to ensure that it is correctly
	// deleted.

	virtual curve *u_param_line(
				double,			// constant v SPAparameter
				spline const &	// owning surface
			) const;
	virtual curve *v_param_line(
				double,			// constant u SPAparameter
				spline const &	// owning surface
			) const;


	// Test whether a point lies on the surface. The default uses
	// point_perp, and tests the distance.

//	virtual logical test_point_tol(
//				SPAposition const &,
//				double,
//				SPApar_pos const & = *(SPApar_pos *)NULL_REF,
//				SPApar_pos & = *(SPApar_pos *)NULL_REF
//			) const;


	// Save and restore. Save is easy, as derived class switching goes
	// through the normal virtual function mechanism. Restore is more
	// complicated, because until it is invoked we do not have an
	// object of the right class. Instead we switch on a table defined
	// by static instances of the restore_ss_def class (see below), to
	// invoke a simple friend function which constructs an object of
	// the right (derived) type. Then it can call the appropriate
	// member function to do the actual work.

public:
	static int id();
	virtual int type() const;

	virtual char const *type_name() const;

	virtual void save_data() const;

protected:
	friend DECL_KERN subtype_object *restore_pipe_spl_sur();
	void restore_data();


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

#endif
