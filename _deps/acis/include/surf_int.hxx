/*******************************************************************/
/*    Copyright (c) 1989-2020 by Spatial Corp.                     */
/*    All rights reserved.                                         */
/*    Protected by U.S. Patents 5,257,205; 5,351,196; 6,369,815;   */
/*                              5,982,378; 6,462,738; 6,941,251    */
/*    Protected by European Patents 0503642; 69220263.3            */
/*    Protected by Hong Kong Patent 1008101A                       */
/*******************************************************************/

// Define an interpolated curve subtype which is the projection
// of a spline on a surface (where the spline is within fit tolerance
// of the surface).

#if !defined( surf_int_cur_CLASS)
#define surf_int_cur_CLASS

#include "logical.h"

#include "acis.hxx"
#include "dcl_kern.h"

#include "intdef.hxx"

#include "debugmsc.hxx"

class surface;

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
/*
// tbrv
*/
/**
 * @nodoc
 */
DECL_KERN subtype_object *restore_surf_int_cur();
#if defined D3_STANDALONE || defined D3_DEBUG
/*
// tbrv
*/
/**
 * @nodoc
 */
DECL_KERN D3_ostream& operator<<( D3_ostream &, int_cur const & );
/*
// tbrv
*/
/**
 * @nodoc
 */
DECL_KERN D3_istream& operator>>( D3_istream &, int_cur *& );
#endif

// The class definition itself.
/**
 * @nodoc
 * <b>Obsolete:</b> use @href par_int_cur instead.
 * Represents spline curves on a surface within the given fit tolerance.
 */
class DECL_KERN surf_int_cur: public int_cur {
#ifdef surf_int_cur_FRIENDS
surf_int_cur_FRIENDS
#endif

private:
	// The only additional data required is to select which surface
	// and pcurve to use for projection.
	logical use_sur1;		// TRUE if the curve is defined to lie
							// on sur1, FALSE if it is sur2.
    // Parameters used by surf_int_cur extensions
    logical is_extended;
    SPAinterval range_before_extension;
    SPApar_pos uv_start_before_extension;
    SPApar_pos uv_end_before_extension;
    SPApar_vec duv_start_before_extension;
    SPApar_vec duv_end_before_extension;

    // Sets the extension parameters.  Assumes that the current param
    // range is the unextended range unless the input argument is given.
    logical set_extension_data(
        const SPAinterval& unextended_range = *(SPAinterval*)NULL_REF);

    // Extends the bs3 curve.
    logical extend_cur_data(
        const SPAinterval& new_range);


	//surf_int_cur();

    // Set the safe range to the full range, unless the appropriate surface is
    // singular at either end.

    virtual	void    set_safe_range();

//VMB:  
//Moved from private to protected because sss_int_cur is derived from this class
//Inventor 11 changes - Reverse Engineered
protected:
	surf_int_cur(); 
	// We do not need a specific destructor, as we do not add any
	// subsidiary structure, but it is virtual, so we define it
	// anyway.

	~surf_int_cur();

public:
	// Construct a surface curve.
/**
 *C++ initialize constructor requests memory for this object and populates it with the data supplied as arguments.
 *<br><br>
 * @param splcur
 * spline curve.
 * @param tol
 * fit tolerance.
 * @param surf
 * surface where curve lies.
 * @param cur
 * curve in param space.
 * @param trcur
 * surface for true curve.
 * @param range
 * safe range for curve.
 */
	surf_int_cur(
			bs3_curve splcur,			// spline curve
			double tol,				// fit tolerance
			surface const &surf = *(surface *)NULL_REF,
								// surface on which curve lies
			bs2_curve cur= NULL,	// curve in SPAparameter space of the
								// surface
			logical trcur= TRUE,		// TRUE to make surf1
								// define the true curve, FALSE for
								// surf2
			const SPAinterval& range = *(const class SPAinterval*) NULL_REF
				                // safe_range for the curve
		);

	// Construct a general surface curve, with an additional surface
	// and SPAparameter curve.
/**
 * C++ initialize constructor requests memory for this object and populates it with the data supplied as arguments.
 *<br><br>
 * @param splcur
 * spline curve.
 * @param tol
 * fit tolerance.
 * @param surf1
 * 1st surface for curve.
 * @param surf2
 * 2nd surface for curve.
 * @param cur1
 * 1st curve in param space.
 * @param cur2
 * 2nd curve in param space.
 * @param trcur
 * surface for true curve.
 * @param range
 * safe range for curve.
 */
	surf_int_cur(
			bs3_curve splcur,			// spline curve
			double tol,				// fit tolerance
			surface const &surf1,	// first surface on which curve lies
			surface const &surf2,	// second surface on which curve lies
			bs2_curve cur1,			// curve in SPAparameter space of the
								// first surface
			bs2_curve cur2,			// curve in SPAparameter space of the
								// second surface
			logical trcur= TRUE,		// TRUE to make surf1
								// define the true curve, FALSE for
								// surf2
			const SPAinterval& range= *(const class SPAinterval*) NULL_REF
				                // safe_range for the curve
		);

	// Copy constructor
/**
 * C++ copy constructor requests memory for this object and populates it with the data from the object supplied as an argument.
 *<br><br>
 * @param input
 * input curve.
 */
	surf_int_cur( const surf_int_cur& input);

	// The deep_copy method makes a copy without shared data
/**
 * Creates a copy of an item that does not share any data with the original.
 *<br><br>
 *<b>Role:</b> Allocates new storage for all member data and any pointers.
 * Returns a pointer to the copied item.
 *<br><br>
 * @param pm
 * list of items within the entity that are already deep copied.
 */
	virtual int_cur *deep_copy(pointer_map * pm = NULL) const;

	// STI ROLL
/*
// tbrv
*/
/**
 * @nodoc
 */
	virtual void full_size(SizeAccumulator&, logical = TRUE) const;
	// STI ROLL

private:

	// The default action is to return any surface stored, so this
	// is fine.

//	virtual surface const *surf1() const;
//	virtual surface const *surf2() const;


	// The default action is to return any pcurve stored.

//	virtual bs2_curve pcur1() const;
//	virtual bs2_curve pcur2() const;


	// Construct a duplicate in free store of this object but with
	// zero use count.

	virtual subtrans_object *copy() const;


	// Test for equality - not guaranteed to find all cases of curve
	// superimposition, but reliably flagging cases of inequality.
	// The base class int_cur version is sufficient.

	virtual logical operator==( subtype_object const & ) const;


	// Parameter shift: the base class version is fine.

//	virtual void shift( double );

	// Reparametrise the curve. The default inplementation is fine.

//	void reparam(
//				double,
//				double
//			);

	// Divide an intersection curve into two pieces at a given
	// SPAparameter value, possibly adjusting the spline approximations
	// to an exact value at the split point.

	virtual void split(
				double,
				SPAposition const &,
				int_cur *[ 2 ]
			);

	// Spline concatenation: the base class version is fine.

//	virtual void append( int_cur & );

	// Transformation. The base class version is fine.

//	virtual void operator*=( SPAtransf const & );


	// Geometric evaluation

	// Tangent direction to curve at given point.

	virtual SPAunit_vector point_direction(
				SPAposition const &,
				SPAparameter const &
			) const;

	// Curvature at point on curve.

	virtual SPAvector point_curvature(
				SPAposition const &,
				SPAparameter const &
			) const;

	// Parameter value for given point on curve. The default
	// implementation is fine.

//	virtual double param(
//				SPAposition const &,
//				SPAparameter const &
//			) const;

	// Find SPAposition, first and second derivative on curve at given
	// SPAparameter value.

	virtual void eval(
				double,
				SPAposition &,
				SPAvector &,
				SPAvector &,
				logical
			) const;

	// Find SPAposition on curve at given SPAparameter value. The default
	// version using eval() is fine.

//	virtual SPAposition eval_position(
//				double,
//				logical
//			) const;

	// Find parametric derivative (magnitude and direction). The
	// default version using eval() is fine.

//	virtual SPAvector eval_deriv(
//				double,
//				logical
//			) const;

	// Find curvature. The default using eval() is OK.

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
/**
 * Returns the <tt>ID</tt> for the <tt>surf_int_cur</tt> list.
 */
	static int id();
/**
 * Returns the type of <tt>surf_int_cur</tt>.
 */
	virtual int type() const;
/**
 * Returns the string <tt>"surfcur"</tt>.
 */
	virtual char const *type_name() const;
/**
 * @nodoc
 */
	virtual void save() const;
/**
 * Save the information for the <tt>surf_int_cur</tt> to a save file.
 */
	virtual void save_data() const;

	/**
	*@nodoc
	**/
	virtual void process(geometry_definition_processor& p) const;
protected:
	virtual logical need_save_approx_as_full() const;

private:
	friend DECL_KERN subtype_object *restore_surf_int_cur();
	void restore_data();

	// Check for any data errors in the curve, and correct the errors if
	// possible.  The various arguments provide control over which checks
	// are made, which fixes can be applied and which fixes were actually
	// applied.  The function returns a list of errors that remain in the
	// curve on exit.
	// See chk_stat.hxx for information on the argument types used here.

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
};

#endif
