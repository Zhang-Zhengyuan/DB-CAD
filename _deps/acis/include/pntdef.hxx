/*******************************************************************/
/*    Copyright (c) 1989-2020 by Spatial Corp.                     */
/*    All rights reserved.                                         */
/*    Protected by U.S. Patents 5,257,205; 5,351,196; 6,369,815;   */
/*                              5,982,378; 6,462,738; 6,941,251    */
/*    Protected by European Patents 0503642; 69220263.3            */
/*    Protected by Hong Kong Patent 1008101A                       */
/*******************************************************************/

// Header for degenerate_curve, which represents a degenerate curve.

// A degenerate_curve is represented by a point.

// The parametric equation of the point is
//		point = root_point
// where t can be any SPAparameter.

// The degenerate curve class was built for the skin/loft ... surfaces.
// Its intended scope is to provide a way to build skin/loft
// surfaces that come to a point at either end. 

#if !defined( degenerate_curve_CLASS )
#define degenerate_curve_CLASS

#include "dcl_kern.h"
#include "curdef.hxx"

#include "debugmsc.hxx"

#include "position.hxx"
#include "vector.hxx"
#include "unitvec.hxx"

class SPAbox;
class SPAtransf;
class SPAparameter;
class law;

// STI ROLL
class SizeAccumulator;
// STI ROLL

// Define an identifying type for this (lower-case) curve.

#define degenerate_curve_type 3

class degenerate_curve;
DECL_KERN degenerate_curve operator*(degenerate_curve const &,SPAtransf const &);
DECL_KERN curve *restore_degenerate_curve();


// degenerate_curve definition proper.

class DECL_KERN degenerate_curve: public curve {

public:

	// The defining degenerate_curve.

	SPAposition root_point;


	degenerate_curve();


	// Construct a degenerate_curve from another.

	degenerate_curve(
				degenerate_curve const &
			);


	// Construct a degenerate_curve from a SPAposition.

	degenerate_curve(
				SPAposition const &
			);

	// The deep_copy method makes a copy without shared data

	virtual curve *deep_copy(pointer_map * pm = NULL) const;

	// STI ROLL
	virtual void full_size(SizeAccumulator&, logical = TRUE) const;
	// STI ROLL

	// Virtual destructor - can't be inlined, so we define it to
	// avoid getting multiple copies.

	~degenerate_curve();


	// Make a copy of this degenerate_curve on the heap, and return a 
	// pointer to it.

	virtual curve *make_copy() const;

	// Return the law form of a degenerate_curve

	law *law_form();

	// Return a degenerate_curve with the opposite sense from "this" one.

	degenerate_curve operator-() const;


	// Test two curves for equality. This, like testing floating-point
	// numbers for equality, is not guaranteed to say "equal" for
	// effectively-equal curves, but is guaranteed to say "not equal"
	// if they are indeed not equal. The result can be used for
	// optimisation, but not where it really matters.

	virtual logical operator==( curve const & ) const;


	// Return "this" degenerate_curve transformed by the given SPAtransf.

	friend DECL_KERN degenerate_curve operator*(
				degenerate_curve const &,
				SPAtransf const &
			);


	// Transform this degenerate_curve by the given SPAtransf.

	virtual curve &operator*=(
				SPAtransf const &
			);


	// Return TRUE if the nth SPAparameter-space curve is defined
	// (i.e. pcur() would return a non-NULL pcurve pointer),
	// FALSE otherwise. Always FALSE here, so the default works fine.

//	virtual logical pcur_present(
//				int
//			) const;


	// Return a pointer to an associated SPAparameter-space curve -
	// always NULL for a point, so the default is fine.

//	virtual pcurve *pcur(
//				int
//			) const;


	// Negate this point (i.e. negate the direction).

	virtual curve &negate();


	// Indicate whether this curve is undefined.

	logical undef() const;


	// Split the curve if it is splittable; the default just does
	// nothing just returning NULL, and this is fine for us.

//	virtual curve *split(
//				double,
//				SPAposition const & = *(SPAposition *)NULL_REF
//			);


	// Return a SPAbox enclosing the degenerate_curve.

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
				logical,
				SPAtransf const & = *(SPAtransf *)NULL_REF
			) const;


	// Returns the direction of the curve at a point on it. This is,
	// of course, a constant for a point, so it is immaterial
	// in this case whether the given point is on or off the curve.

	virtual SPAunit_vector point_direction(
				SPAposition const &,
				SPAparameter const & = *(SPAparameter *)NULL_REF
			) const;


	// Returns the curvature - zero for a point. Again it
	// is immaterial whether the point is on or off the curve.
	// The SPAparameter argument is ignored.

	virtual SPAvector point_curvature(
				SPAposition const &,
				SPAparameter const & = *(SPAparameter *)NULL_REF
			) const;


	// Find the foot of the perpendicular from the given point to
	// the curve, and tangent to the curve at that point, and its
	// SPAparameter value. If an input SPAparameter value is supplied (as
	// argument 5), the perpendicular found is the one nearest to
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


	// Find the SPAparameter value at the given point on the curve.
	// If the point is not on the curve, a plane is constructed
	// perpendicular to the line, and the SPAparameter value for its
	// intersection with the line returned.

	virtual double param(
				SPAposition const &,
				SPAparameter const & = *(SPAparameter *)NULL_REF
			) const;


	// Evaluate the curve at a given SPAparameter value, giving SPAposition,
	// and first and second derivatives (all optionally).

	virtual void eval(
				double,
				SPAposition &,
				SPAvector & = *(SPAvector *)NULL_REF,	// first derivative
				SPAvector & = *(SPAvector *)NULL_REF,	// second derivative
				logical = FALSE,
				logical = FALSE
			) const;


	// Find point on line at given SPAparameter value. The default
	// implementation just uses eval(), which is fine.

//	virtual SPAposition eval_position(
//				double,
//				logical = FALSE,
//				logical = FALSE
//			) const;


	// Return the derivative with respect to SPAparameter at a given
	// SPAparameter value. The default implementation just uses eval(),
	// which is fine.

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


	// Return the curvature at a given SPAparameter value. This is
	// always zero, so all the arguments are ignored.

	virtual SPAvector eval_curvature(
				double,
				logical = FALSE,
				logical = FALSE
			) const;


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


	// Return a distance value, which represents the greatest
	// discrepancy between positions calculated by calls to eval() or
	// eval_position() with the "approx_OK" logical set by turns to
	// TRUE and FALSE. Here uses the default, returning 0.

//	virtual double approx_error() const;


	// Indicate whether the curve is periodic, that is joins itself
	// smoothly at the ends of its principal SPAparameter range, so that
	// edges may span the "seam".

	virtual logical periodic() const;


	// Indicate whether a curve is closed, that is joins itself
	// (smoothly or not) at the ends of its principal SPAparameter
	// range. Note that this function should always return TRUE
	// if periodic() does.

	virtual logical closed() const;


	// Return the SPAparameter period, zero in this case because a
	// degenerate_curve is not periodic.

	virtual double param_period() const;

	// Return the principal SPAparameter range, or what is inside the
	// given SPAbox, if one is indeed given. If there is no SPAbox, the
	// range is unbounded, and we return the empty SPAinterval.

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
	// is held constant. The parameter approx_ok is not used for degenerate_curves.

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
	// range. The parameter approx_ok is not used for degenerate_curves.

	virtual double length_param(
				double,			// datum SPAparameter
				double,			// arc length
				logical approx_ok=TRUE
			) const;


	// Find the extrema of the curve in the given direction.
	// A point has no extrema, so here we return NULL.

	virtual curve_extremum *find_extrema(
				SPAunit_vector const &
			) const;


	// Test point-on-curve, returning the SPAparameter value if
	// required.

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
	friend DECL_KERN curve *restore_degenerate_curve();
	void restore_data();



#if defined D3_STANDALONE || defined D3_DEBUG

	virtual void print(
				D3_ostream &
			) const;

	virtual void input(
				D3_istream &
			);

#endif

	// Output details of the curve.

	virtual void debug(
				char const *,
				FILE * = debug_file_ptr
			) const;
};

#endif
