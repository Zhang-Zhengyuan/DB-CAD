/* ORIGINAL: acis2.1/kerngeom/intcur/proj_int.hxx */
/* $Id: proj_int.hxx,v 1.20 2002/08/09 17:15:24 jeff Exp $ */
/*******************************************************************/
/*    Copyright (c) 1989-2020 by Spatial Corp.                     */
/*    All rights reserved.                                         */
/*    Protected by U.S. Patents 5,257,205; 5,351,196; 6,369,815;   */
/*                              5,982,378; 6,462,738; 6,941,251    */
/*    Protected by European Patents 0503642; 69220263.3            */
/*    Protected by Hong Kong Patent 1008101A                       */
/*******************************************************************/

// Implement an interpolated curve subtype to represent the
// perpendicular projection of a curve to a surface. This also
// includes a second surface, for the sake of its pcurve, as one
// significant use of this curve type is blending, where it is used
// to bound a (parametric) blend surface. We record which surface
// is the projection surface, and which the auxiliary one, to ensure
// consistency.

#if !defined( proj_int_cur_CLASS)
#define proj_int_cur_CLASS

#include "logical.h"

#include "acis.hxx"
#include "dcl_kern.h"

#include "interval.hxx"

#include "fit.hxx"

#include "intdef.hxx"

#include "debugmsc.hxx"

class surface;

class proj_int_interp;

class SPAposition;
class SPAvector;
class SPAunit_vector;
class SPAtransf;

class SPAparameter;
class SPApar_pos;
class SPApar_dir;

// STI ROLL
class SizeAccumulator;
// STI ROLL
DECL_KERN subtype_object *restore_proj_int_cur();
#if defined D3_STANDALONE || defined D3_DEBUG
DECL_KERN D3_ostream& operator<<( D3_ostream &, int_cur const & );
DECL_KERN D3_istream& operator>>( D3_istream &, int_cur *& );
#endif

// The class definition itself.

class DECL_KERN proj_int_cur: public int_cur {

// Allow extensions to declare themselves as friends. USE WITH EXTREME CAUTION
#ifdef proj_int_cur_FRIENDS
proj_int_cur_FRIENDS
#endif

private:
	curve *proj_cur_data;	// curve to be projected

	SPAinterval proj_range;	// SPAparameter range of the projection
							// curve corresponding to the extent
							// of this curve

	logical proj_sur1;		// set TRUE if the curve is to be projected
							// on to the first int_cur surface, FALSE
							// if it is to be the second surface.

    // Set the safe range to the full range, unless the appropriate surface is 
    // singular at either end.

    virtual	void    set_safe_range();

	// Make a null proj_int_cur.

	proj_int_cur();

public:
	// Construct a proj_int_cur when the interpolating curve has
	// already been found.

	proj_int_cur(
			curve const &,		// curve to be projected
			SPAinterval const &,	// SPAparameter range of projection curve
			bs3_curve,			// interpolating curve
			double,				// fitol
			surface const &,	// first surface on which the curve
								// lies
			surface const &,	// second surface
			logical,			// TRUE if the projection surface is
								// the first surface, FALSE if it is
								// the second
			bs2_curve = NULL,	// pcurve for first surface
			bs2_curve = NULL,	// pcurve for second surface
			const SPAinterval& = *(const class SPAinterval*) NULL_REF
		);

	// Copy constructor

	proj_int_cur( const proj_int_cur& );

	// The deep_copy method makes a copy without shared data

	virtual int_cur *deep_copy(pointer_map * pm = NULL) const;

	/**
	* @nodoc
	**/
	virtual void process(geometry_definition_processor& p) const;

	// STI ROLL
	virtual void full_size(SizeAccumulator&, logical = TRUE) const;
	// STI ROLL
/**
 * @nodoc
 */
	virtual void minimize( minimize_helper*);

    virtual	void calculate_disc_info();
private:
	// Destructor to remove our copy of the projection curve.

	virtual ~proj_int_cur();

	// Extract detailed information about the curve. The base class
	// implementations are fine.

//	virtual surface const *surf1() const;
//	virtual surface const *surf2() const;
//	virtual bs2_curve pcur1() const;
//	virtual bs2_curve pcur2() const;


	// Duplication. Can't be done by constructor, as we want it
	// to be virtual.

	virtual subtrans_object *copy() const;


	// Test for equality - not guaranteed to find all cases of curve
	// superimposition, but reliably flagging cases of inequality.

	virtual logical operator==( subtype_object const & ) const;


	// Utility function to find the point on the projection curve
	// corresponding to a given point and curve direction. This is
	// the intersection of the projection curve with the normal
	// plane.

	double proj_param(
				SPAposition const &,
				SPAunit_vector const &,
				double
			) const;

	// Parameter shift: the base class version is fine.

//	virtual void shift( double );

	// Reparametrise the curve. The default implementation is fine.

//	void reparam(
//				double,
//				double
//			);

	// Divide a proj_int_cur into two pieces at a given SPAparameter value,
	// possibly adjusting the spline approximations to an exact value
	// at the split point.

	void split(
				double,
				SPAposition const &,
				int_cur *[ 2 ]
			);

	// Spline concatenation.

	virtual void append( int_cur & );

	// Transformation

	virtual void operator*=( SPAtransf const & );


	// Geometric evaluation

	// Tangent direction to intcurve at given point. The default
	// uses param() and eval_deriv().

//	virtual SPAunit_vector point_direction(
//				SPAposition const &,
//				SPAparameter const &
//			) const;
 
	// Curvature at point on intcurve. The default uses param() and
	// eval_curvature().

//	virtual SPAvector point_curvature(
//				SPAposition const &,
//				SPAparameter const &
//			) const;
	
	// Parameter value for given point on curve. The default
	// int_cur implementation is fine.

//	virtual double param(
//				SPAposition const &,
//				SPAparameter const &
//			) const;

	// Evaluate the curve, giving SPAposition and first and second
	// derivatives. The default version using evaluate() is fine.

//	virtual void eval(
//				double,
//				SPAposition &,
//				SPAvector &,
//				SPAvector &,
//				logical
//			) const;

	// Find SPAposition on curve at given SPAparameter value. The default
	// version using eval() is fine.

//	virtual SPAposition eval_position(
//				double,
//				logical
//			) const;

	// Find parametric derivative, magnitude and direction. The default
	// version using eval() is fine.

//	virtual SPAvector eval_deriv(
//				double,
//				logical
//			) const;

	// Find curvature. The default version using eval() is OK.

//	virtual SPAvector eval_curvature(
//				double,
//				logical
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
                double,				// Parameter
				curve_evaldata *,	// Initialisation data for iterations.
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

	// Construct a data object to retain evaluation information across
	// calls to evaluate_iter(). This is to allow subsidiary calls
	// within an iterative evaluator to start iteration much closer to the
	// required result than is possible just using the curve information
	// itself.

	virtual curve_evaldata *make_evaldata() const;


	// Return the number of derivatives which evaluate() can find
	// "accurately" (and fairly directly), rather than by finite
	// differencing, over the given portion of the curve. If there
	// is no limit to the number of accurate derivatives, returns
	// the value ALL_CURVE_DERIVATIVES, which is large enough to be
	// more than anyone could reasonably want.

	virtual int accurate_derivs( 
				SPAinterval const & = *(SPAinterval*)NULL_REF
								 	// Defaults to the whole curve
			) const;


	// Save and restore. Save is easy, as derived class switching goes
	// through the normal virtual function mechanism. Restore is more
	// complicated, because until it is invoked we do not have an
	// object of the right class. Instead we switch on a table defined
	// by static instances of the restore_ts_def class (see below), to
	// invoke a simple friend function which constructs an object of
	// the right (derived) type. Then it can call the appropriate
	// member function to do the actual work.

public:
	static int id();
	virtual int type() const;

	virtual char const *type_name() const;

	virtual void save_data() const;

	virtual logical need_save_as_approx(int save_to_version, logical check_progenitors) const;

protected:
	virtual logical need_save_approx_as_full() const;

private:
	friend DECL_KERN subtype_object *restore_proj_int_cur();
	void restore_data();

#if defined D3_STANDALONE || defined D3_DEBUG

	friend DECL_KERN D3_ostream& operator<<(
				D3_ostream &,
				int_cur const &
			);

	friend DECL_KERN D3_istream& operator>>(
				D3_istream &,
				int_cur *&
			);

	virtual void input(
				D3_istream &
			);

	virtual void print(
				D3_ostream &
			) const;

#endif

	// Debug printout

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

public:
	// This is a bit of a fix, to allow blending to set the blend
	// surface and its SPAparameter-space curve into the object after
	// it has been constructed. It just assumes that the surface
	// and curve have been constructed properly.

	void set_other_surface( surface const &, bs2_curve );
};

// Now a class to fit a curve to the projection of a curve to a
// surface given a preliminary list of points, directions etc.

class DECL_KERN proj_int_interp: public curve_interp {

private:
	curve const *proj_cur_data;	// curve to be projected

	SPAinterval proj_range;	// SPAparameter range of the projection
							// curve corresponding to the extent
							// of this curve

	logical proj_sur1;		// set TRUE if the curve is to be projected
							// on to the first int_cur surface, FALSE
							// if it is to be the second surface.

public:
	// Constructor providing all the information required.

	proj_int_interp(
			curve const &,		// curve to be projected
			SPAinterval const &,	// curve SPAparameter range
			surface const &,	// first surface
			surface const &,	// second surface
			logical,			// true to project to first surface,
								// false for second
			int,				// number of points
			SPAposition const *,	// array of positions
			SPAvector const *,		// array of tangents
			SPApar_pos const *,	// SPApar_pos on first surf if parametric
			SPApar_pos const *,	// SPApar_pos on second surf if parametric
			double				// tolerance for fit
		);

	// Virtual destructor

	~proj_int_interp();

	// Find the true-point for projection.

	void true_point(
			double,			// tolerance
			point_data &	// approximate SPAposition, direction,
							// SPAparameter values on input, exact values
							// on output
		) const;

	// Construct the appropriate int_cur subclass object (in this
	// case a proj_int_cur) from the data in this object after curve
	// interpolation.

	int_cur *make_int_cur();

    curve const *proj_cur() const
       {return proj_cur_data;}
};

#endif
