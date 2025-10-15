/*******************************************************************/
/*    Copyright (c) 1989-2020 by Spatial Corp.                     */
/*    All rights reserved.                                         */
/*    Protected by U.S. Patents 5,257,205; 5,351,196; 6,369,815;   */
/*                              5,982,378; 6,462,738; 6,941,251    */
/*    Protected by European Patents 0503642; 69220263.3            */
/*    Protected by Hong Kong Patent 1008101A                       */
/*******************************************************************/

// Define an interpolated curve subtype which is the 3D extension of
// the SPAparameter curve representing a curve on a surface. This is used
// internally by Acis during point-in-face testing on a parametric
// surface, and certain member functions which are not required by
// Acis are disabled, to simplify the implementation. It should not be
// used by an application.

#if !defined( pcur_int_cur_CLASS)
#define pcur_int_cur_CLASS

/**
* @file pcur_int.hxx
 * @CAA2Level L1
 * @CAA2Usage U1
 * \addtogroup ACISCURVES
 *
 * @{
 */
#include "logical.h"

#include "acis.hxx"
#include "dcl_kern.h"

#include "intdef.hxx"

#include "debugmsc.hxx"

#include "transf.hxx"

class surface;

class SPAposition;
class SPAvector;
class SPAunit_vector;

class SPAparameter;
class SPApar_pos;
class SPApar_dir;

// STI ROLL
class SizeAccumulator;
// STI ROLL
#if defined D3_STANDALONE || defined D3_DEBUG
/*
// tbrv
*/
/**
 * @nodoc
 */
DECL_KERN D3_istream& operator>>( D3_istream &, int_cur *& );
/*
// tbrv
*/
/**
 * @nodoc
 */
DECL_KERN D3_ostream& operator<<( D3_ostream &, int_cur const & );
#endif

/**
 * @nodoc
 * Defines an interpolated curve subtype that is the 3D extension of the parameter curve representing a curve on a surface.
 * <br>
 * <b>Role</b>: This class defines an interpolated curve subtype that is the <tt>3D</tt> extension of the
 * parameter curve representing a curve on a surface. This is used internally by <tt>ACIS</tt> during
 * point-in-face testing on a parametric surface, and certain member functions that are not
 * required by <tt>ACIS</tt> are disabled, to simplify the implementation. It should not be used by an
 * application.
 * <br><br>
 * @see curve, pcurve
 */
class DECL_KERN pcur_int_cur: public int_cur {

private:
	// Store the curve and pcurve which define the curve.

	curve *def_cur_data;
	pcurve *def_pcur_data;

	// If we were to allow transformations of these curevs, we should
	// have to store the resultant transformation here, and apply it
	// on any evaluation or inversion. Acis does not require this, and
	// so transformations are disabled (along with several other
	// awkward operations).

	pcur_int_cur();

public:
/**
 * C++ copy constructor requests memory for this object and populates it with the data from the object supplied as an argument.
 * <br><br>
 * @param curve2
 * 2D curve.
 * @param curve3
 * 3D pcurve.
 */
	pcur_int_cur(
			curve const &curve2,
			pcurve const &curve3
		);

	// Copy constructor

/**
 * C++ copy constructor requests memory for this object and populates it with the data from the object supplied as an argument.
 * <br><br>
 * @param int_curve
 * pcur_int_curve.
 */
	pcur_int_cur( const pcur_int_cur &int_curve );

	// The deep_copy method makes a copy without shared data

/**
 * Creates a copy of an item that does not share any data with the original.
 * <br><br>
 * <b>Role</b>: Allocates new storage for all member data and any pointers. Returns a pointer to
 * the copied item.
 * <br><br>
 * @param pm
 * list of items within the entity that are already deep copied.
 */
	virtual int_cur *deep_copy(pointer_map * pm = NULL) const;

	/**
	* @nodoc
	**/
	virtual void process(geometry_definition_processor& p ) const;

	// STI ROLL
/*
// tbrv
*/
/**
 * @nodoc
 */
	virtual void full_size(SizeAccumulator&, logical = TRUE) const;
	// STI ROLL
/**
 * @nodoc
 */
	virtual void minimize( minimize_helper*);

private:
	~pcur_int_cur();

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

	virtual logical operator==( subtype_object const & ) const;


	// Parameter shift: this is disabled for non-periodic underlying
	// curve and pcurve, to avoid the inconvenience of maintaining
	// explicit SPAparameter ranges. Acis only needs it for the periodic
	// case anyway.

	virtual void shift( double );

	// Reparametrise the curve: this is disabled for this subtype.

	void reparam(
				double,
				double
			);

	// Divide a 2D/3D spline into two pieces at a given SPAparameter
	// value, possibly adjusting the spline approximations to an
	// exact value at the split point.

	virtual void split(
				double,
				SPAposition const &,
				int_cur *[ 2 ]
			);

	// Spline concatenation. The default is OK.

//	virtual void append( int_cur & );

	// Transformation. Disabled to avoid having to keep and use an
	// explicit transformation.

	virtual void operator*=( SPAtransf const & );


	// Geometric evaluation

	// Tangent direction to curve at given point. The base class
	// just uses eval_direction(), which is fine.

//	virtual SPAunit_vector point_direction(
//				SPAposition const &,
//				SPAparameter const &
//			) const;

	// Curvature at point on curve. The base class just uses
	// eval_curvature(), which is fine.

//	virtual SPAvector point_curvature(
//				SPAposition const &,
//				SPAparameter const &
//			) const;

	// Parameter value for given point on curve.

	virtual double param(
				SPAposition const &,
				SPAparameter const &
			) const;

	// Find SPAposition, first and second derivative on curve at given
	// SPAparameter value. Second derivative is only found approximately,
	// sufficient for Acis internal use.

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

	// Find parametric derivative (magnitude and direction). The
	// default version using eval() is fine.

//	virtual SPAvector eval_deriv(
//				double,
//				logical
//			) const;

	// Find curvature. The default version using eval() is fine.

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
 * Returns the <tt>ID</tt> for the pcur_int_cur <tt>list</tt>.
 */
	static int id();
/**
 * Returns the type of <tt>pcur_int_cur</tt>.
 */
	virtual int type() const;
/**
 * Returns the string <tt>"pcurcur"</tt>.
 */
	virtual char const *type_name() const;

	// This intcur subtype is only for internal Acis purposes, and
	// so does not need to be saved. All the following
	// functions are disabled.
/**
 * Saves the information for a <tt>pcur_int_cur</tt> to the save file.
 */
	virtual void save_data() const;

private:
//	friend subtype_object <tt>*restore_pcur_int_cur()</tt>;
//	void restore_data();

#if defined D3_STANDALONE || defined D3_DEBUG

	friend DECL_KERN D3_istream& operator>>(
				D3_istream &,
				int_cur *&
			);

	friend DECL_KERN D3_ostream& operator<<(
				D3_ostream &,
				int_cur const &
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

/** @} */
#endif
