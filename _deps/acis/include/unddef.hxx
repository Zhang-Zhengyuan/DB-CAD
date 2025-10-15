/* ORIGINAL: acis2.1/kerngeom/curve/unddef.hxx */
/* $Id: unddef.hxx,v 1.16 2002/08/09 17:15:21 jeff Exp $ */
/*******************************************************************/
/*    Copyright (c) 1989-2020 by Spatial Corp.                     */
/*    All rights reserved.                                         */
/*    Protected by U.S. Patents 5,257,205; 5,351,196; 6,369,815;   */
/*                              5,982,378; 6,462,738; 6,941,251    */
/*    Protected by European Patents 0503642; 69220263.3            */
/*    Protected by Hong Kong Patent 1008101A                       */
/*******************************************************************/

// Header for "undefined curve". This is curve undefined except for its
// end points, for which we have explicit positions, directions and
// curvatures.

// It is used in blending, to allow the blend surface to spread at its
// ends. It may be useful elsewhere, as well.

// The curve is parametrised so that the start point has SPAparameter 0,
// the end point has SPAparameter 1. No other point lies on the curve,
// so the SPAparameter value is meaningless, but we in fact return 0.5.


#if !defined( undefc_CLASS )
#define undefc_CLASS

#include "dcl_kern.h"
#include "curdef.hxx"

#include "debugmsc.hxx"

#include "position.hxx"
#include "vector.hxx"
#include "unitvec.hxx"

class SPAbox;

class SPAtransf;

class SPAparameter;

// STI ROLL
class SizeAccumulator;
// STI ROLL


// Define an identifying type for this (lower-case) curve.

#define undefc_type 21

class undefc;
DECL_KERN undefc operator*(undefc const &,SPAtransf const &);
DECL_KERN curve *restore_undefc();

// undefc definition proper.

class DECL_KERN undefc: public curve {

public:
	SPAposition start_point;		// start point
	SPAunit_vector start_dir;		// start direction
	SPAvector start_curv;			// start curvature
	SPAposition end_point;			// end point
	SPAunit_vector end_dir;		// end direction
	SPAvector end_curv;			// end curvature


	// Uninitialised constructor

	undefc();


	// Construct a undefc as a copy of another.

	undefc(
				undefc const &
			);

	// The deep_copy method makes a copy without shared data

	virtual curve *deep_copy(pointer_map * pm = NULL) const;

	// Construct a undefc from two point, direction, curvature
	// triples.

	undefc(
				SPAposition const &,
				SPAunit_vector const &,
				SPAvector const &,
				SPAposition const &,
				SPAunit_vector const &,
				SPAvector const &
			);


	// Virtual destructor - can't be inlined, so we define it to
	// avoid getting multiple copies.

	~undefc();


	// Make a copy of this undefc on the heap, and return a 
	// pointer to it.

	virtual curve *make_copy() const;

	// STI ROLL
	virtual void full_size(SizeAccumulator&, logical = TRUE) const;
	// STI ROLL

	// Return a reversed undefc (i.e. with start and end interchanged,
	// and directions negated).

	undefc operator-() const;


	// Return a transformed undefc.

	friend DECL_KERN undefc operator*(
				undefc const &,
				SPAtransf const &
			);


	// Transform this undefc by the given SPAtransf.

	virtual curve &operator*=(
				SPAtransf const &
			);


	// Return TRUE if the nth SPAparameter-space curve is defined
	// (i.e. pcur() would return a non-NULL pcurve pointer),
	// FALSE otherwise. Always FALSE here, so the default works fine.

//	virtual logical pcur_present(
//				int
//			) const;


	// Return a pointer to an associated SPAparameter-space curve,
	// here always NULL.

//	virtual pcurve *pcur( int ) const;
//			curve::pcur() is sufficient for a undefc


	// Reverse this undefc.

	virtual curve &negate();


	// Indicate whether this curve is undefined. It is slightly
	// unclear whether a properly-defined undefc is "defined" or
	// not - for the moment we shall always say it is undefined,
	// because it doesn't really represent a curve. Therefore the
	// default curve implementation is fine.

//	virtual logical undef() const;


	// Return NULL - no requirement to split a undefc line. The
	// default implementation is fine.

//	virtual curve *split(
//				double,
//				SPAposition const & = *(SPAposition *)NULL_REF
//			);


	// Return a SPAbox enclosing the two given points on the undefc
	// line.

	virtual SPAbox bound(
				SPAposition const &,
				SPAposition const &,
				SPAtransf const & = *(SPAtransf *)NULL_REF
			) const;
	virtual SPAbox bound(
				SPAinterval const &,
				SPAtransf const & = *(SPAtransf *)NULL_REF
			) const;

	// Return a SPAbox surrounding that portion of the curve within the
	// given SPAbox.

	virtual SPAbox bound(
				SPAbox const &,
				SPAtransf const & = *(SPAtransf *)NULL_REF
			) const;


	// The following is retained for historical reasons, but will be
	// withdrawn.

	SPAbox bound(
				double start,
				double end,
				SPAtransf const &t = *(SPAtransf *)NULL_REF
			) const
	{
		return bound( SPAinterval( start, end ), t );
	}


	// Return a cone bounding the tangent direction of a curve. The
	// cone is deemed to have its apex at the origin, and has a given
	// axis direction and (positive) half-angle. If the logical
	// argument is TRUE, then a quick approximation may be found. The
	// approximate result may lie wholly within or wholly outside the
	// guaranteed bound (obtained with a FALSE argument), but may not
	// cross from inside to outside. Flags in the returned object
	// indicate whether the cone is in fact the best available, and
	// if not whether this result is inside or outside the best cone.

	virtual curve_tancone tangent_cone(
				SPAinterval const &,
				logical = FALSE,
				SPAtransf const & = *(SPAtransf *)NULL_REF
			) const;


	// A undefc can only be evaluated at its end points - other
	// positions or SPAparameter values give an error.
	// Returns direction of undefc.

	virtual SPAunit_vector point_direction(
				SPAposition const &,
				SPAparameter const & = *(SPAparameter *)NULL_REF
			) const;


	// Returns curvature.

	virtual SPAvector point_curvature(
				SPAposition const &,
				SPAparameter const & = *(SPAparameter *)NULL_REF
			) const;


	// Find the foot of the perpendicular from the given point to
	// the curve, and tangent to the curve at that point, and its
	// SPAparameter value. If an input SPAparameter value is supplied (as
	// argument 4), the perpendicular found is the one nearest to
	// the supplied SPAparameter SPAposition, otherwise it is the one
	// at which the curve is nearest to the given point. Any of the
	// return value arguments may be a NULL reference, in which case
	// it is simply ignored.

	virtual void point_perp(
				SPAposition const &,
				SPAposition &,
				SPAunit_vector &,
				SPAvector &,
				SPAparameter const & = *(SPAparameter *)NULL_REF,
				SPAparameter & = *(SPAparameter *)NULL_REF,
				logical f_weak = FALSE
			) const;
	void point_perp(
				SPAposition const &pos,
				SPAposition &foot,
				SPAunit_vector &foot_dt,
				SPAparameter const &guess = *(SPAparameter *)NULL_REF,
				SPAparameter &actual = *(SPAparameter *)NULL_REF,
				logical f_weak = FALSE
			) const
	{
		point_perp(
					pos,
					foot,
					foot_dt,
					*(SPAvector *)NULL_REF,
					guess,
					actual, f_weak
				);
	}
	void point_perp(
				SPAposition const &pos,
				SPAposition &foot,
				SPAparameter const &guess = *(SPAparameter *)NULL_REF,
				SPAparameter &actual = *(SPAparameter *)NULL_REF,
				logical f_weak = FALSE
			) const
	{
		point_perp(
					pos,
					foot,
					*(SPAunit_vector *)NULL_REF,
					*(SPAvector *)NULL_REF,
					guess,
					actual, f_weak
				);
	}


	// Return the SPAparameter value for a given point. Fails if the
	// point is neither the start nor the end.
	// The second argument is ignored.

	virtual double param(
				SPAposition const &,
				SPAparameter const & = *(SPAparameter *)NULL_REF
			) const;


	// Evaluate SPAposition and two derivatives at given SPAparameter value.

	virtual void eval(
				double,
				SPAposition &,
				SPAvector & = *(SPAvector *)NULL_REF,	// first derivative
				SPAvector & = *(SPAvector *)NULL_REF,	// second derivative
				logical = FALSE,
				logical = FALSE
			) const;


	// Find point on line at given SPAparameter value. The default uses
	// eval(), and this is fine.

//	virtual SPAposition eval_position(
//				double,
//				logical = FALSE,
//				logical = FALSE
//			) const;


	// Return parametric derivative of undefc line. Only defined
	// at the ends. The default uses eval(), and this is fine.

//	virtual SPAvector eval_deriv(
//				double,
//				logical = FALSE,
//				logical = FALSE
//			) const;


	// Find the magnitude of the derivative at the given SPAparameter
	// value on the curve.

	virtual double eval_deriv_len(
				double,
				logical = FALSE,
				logical = FALSE
			) const;


	// Returns curvature at the given SPAparameter value.

	virtual SPAvector eval_curvature(
				double,
				logical = FALSE,
				logical = FALSE
			) const;


	// Find regions of high curvature of the curve.  Given a curvature value k, 
	// an array of intervals is returned over which the curvature exceeds k. 
	// The function value returns the number of intervals. 

	virtual	int	high_curvature( double k, SPAinterval*& spans ) const;


	// Return a distance value, which represents the greatest
	// discrepancy between positions calculated by calls to eval() or
	// eval_position() with the "approx_OK" logical set by turns to
	// TRUE and FALSE. Uses the default which returns 0.

//	virtual double approx_error() const;


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
                double,				// Parameter
                SPAposition &,			// Point on curve at given SPAparameter
                SPAvector ** = NULL, 	// Array of pointers to vectors, of
									// size nd. Any of the pointers may
									// be null, in which case the
									// corresponding derivative will not
									// be returned. 
                int = 0,       		// Number of derivatives required (nd) 
				evaluate_curve_side = evaluate_curve_unknown
									// the evaluation location - above,
									// below or don't care.
            ) const;


	// Return the number of derivatives which evaluate() can find
	// "accurately" (and fairly directly), rather than by finite
	// differencing, over the given portion of the curve. If there
	// is no limit to the number of accurate derivatives, returns
	// the value ALL_CURVE_DERIVATIVES, which is large enough to be
	// more than anyone could reasonably want.

#define ALL_CURVE_DERIVATIVES 9999

	virtual int accurate_derivs( 
				SPAinterval const & = *(SPAinterval*)NULL_REF
								 	// Defaults to the whole curve
			) const;


	// Indicate whether a curve is periodic, that is joins itself
	// smoothly at the ends of its principal SPAparameter range, so that
	// edges may span the "seam".

	virtual logical periodic() const;


	// Indicate whether a curve is closed, that is joins itself
	// (smoothly or not) at the ends of its principal SPAparameter
	// range. Note that this function should always return TRUE
	// if periodic() does.

	virtual logical closed() const;


	// Return the period of a periodic curve, zero if the curve
	// is not periodic.

	virtual double param_period() const;


	// Return the range of SPAparameter values.

	virtual SPAinterval param_range(
				SPAbox const & = *(SPAbox *)NULL_REF
			) const;


	// Arc-length. Return the algebraic distance along the curve
	// between the given parameters, the sign being positive if the
	// SPAparameter values are given in increasing order, and negative
	// if they are in decreasing order. The result is undefined if
	// either SPAparameter value is outside the SPAparameter range of a
	// bounded curve. For a periodic curve the parameters are not
	// reduced to the principal range, and so the portion of the
	// curve evaluated may include several complete circuits. This
	// function is therefore always a monotonically increasing
	// function of its second argument if the first is held constant,
	// and a decreasing function of its first argument if the second
	// is held constant. The parameter approx_ok is not used for undefc's

	virtual double length(
				double,
				double,
				logical approx_ok=TRUE
			) const;


	// The inverse of the length function. Return the SPAparameter value
	// of the point on the curve at the given algebraic arc length
	// from that defined by the datum SPAparameter. The result is not
	// defined for a bounded non-periodic curve if the datum SPAparameter
	// is outside the SPAparameter range, or if the length is outside
	// the range bounded by the values for the ends of the SPAparameter
	// range. The parameter approx_ok is not used for undefc's

	virtual double length_param(
				double,			// datum SPAparameter
				double,			// arc length
				logical approx_ok=TRUE
			) const;


	// An undefc has no extrema - returns NULL.

	virtual curve_extremum *find_extrema(
				SPAunit_vector const &
			) const;


	// Test point-on-curve to given tolerance. Only true points
	// near to the end points.

	virtual logical test_point_tol(
				SPAposition const &,
				double = 0,
				SPAparameter const & = *(SPAparameter *)NULL_REF,
				SPAparameter & = *(SPAparameter *)NULL_REF
			) const;


	// Return a cylinder which encloses the portion of the curve bounded by the
	// SPAinterval. 

	virtual BOUNDING_CYLINDER enclosing_cylinder( const SPAinterval& = 
												    *(SPAinterval*)NULL_REF ) const;


	// Return an identifier uniquely specifying the curve type

	virtual int type() const;


	// Return a string identifying the curve type

	virtual char const *type_name() const;


	// Save and restore. Save is easy, as derived class switching goes
	// through the normal virtual function mechanism. Restore is more
	// complicated, because until it is invoked we do not have an
	// object of the right class. Instead we switch on a table defined
	// by static instances of the restore_cu_def class (see below), to
	// invoke a simple friend function which constructs an object of
	// the right (derived) type. Then it can call the appropriate
	// member function to do the actual work. The save_data and
	// restore_data function for each class can be called in
	// circumstances when we know what type of curve we are expecting
	// and have one in our hand to be filled in.

	virtual void save() const;
	void save_data() const;
	friend DECL_KERN curve *restore_undefc();
	void restore_data();

	// Output details of an undefc.

	virtual void debug(
				char const *,
				FILE * = debug_file_ptr
			) const;
};

#endif
