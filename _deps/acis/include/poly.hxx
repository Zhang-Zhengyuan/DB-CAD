/* ORIGINAL: acis2.1/kernutil/poly/poly.hxx */
/* $Id: poly.hxx,v 1.12 2002/08/09 17:15:33 jeff Exp $ */
/*******************************************************************/
/*    Copyright (c) 1989-2020 by Spatial Corp.                     */
/*    All rights reserved.                                         */
/*    Protected by U.S. Patents 5,257,205; 5,351,196; 6,369,815;   */
/*                              5,982,378; 6,462,738; 6,941,251    */
/*    Protected by European Patents 0503642; 69220263.3            */
/*    Protected by Hong Kong Patent 1008101A                       */
/*******************************************************************/

// Class for implementing polynomial and rational polynomial
// functions of a single variable.

// For a polynomial, we store a degree, an array of coefficients,
// starting with the constant term, and the maximun number of coefficients the array
// can store. The array is allocated on the heap when the degree becomes
// larger than the internal stack object array.

// A rational polynomial is just a pair of polynomials. We do not
// provide any arithmetic on rationals, as finding the LCM of the
// denominators is hard numerically, but in our application easy
// algebraically. The rational is just provided as a convenient
// holder for evaluation and zero-finding.

// A polynomial SPAvector is a triple of polynomials, representing a
// SPAposition in 3D.

// Guard against being included more than once

#if !defined( POLYNOMIAL )
#define POLYNOMIAL

#include "dcl_kern.h"
#include "logical.h"
#include "mmgr.hxx"
#include "base.hxx"

/**
* @file poly.hxx
 * @CAA2Level L1
 * @CAA2Usage U1
 *! \addtogroup KERNAPI
 *
 * @{
 */

class poly_def;
class poly_coeff;
class rat_poly;
class poly_zero;
class poly_vec;
class rat_poly_vec;

class SPAvector;
class SPAposition;

class polynomial;
/**
 * @nodoc
 */
DECL_KERN polynomial operator+(polynomial const &,polynomial const &);
/**
 * @nodoc
 */
DECL_KERN polynomial operator-(polynomial const &);
/**
 * @nodoc
 */
DECL_KERN polynomial operator-(polynomial const &,polynomial const &);
/**
 * @nodoc
 */
DECL_KERN polynomial operator*(polynomial const &,polynomial const &);
/**
 * @nodoc
 */
DECL_KERN polynomial operator/(polynomial const &,double);
/**
 * @nodoc
 */
DECL_KERN rat_poly operator/(polynomial const &,polynomial const &);
/**
 * @nodoc
 */
DECL_KERN logical operator==( double d, polynomial const &p );

// tbrv

// The max_degree that can be stored in the coeff_array,
// without allocating memory.
/**
 * @nodoc
 */
#define COEFF_ARRAY_MAX 16

/**
 * @nodoc
 */
class DECL_KERN poly_def : public ACIS_OBJECT {
public:

	int degree;				// The degree of the polynomial currently
							// stored, -1 for a constant zero
	int max_degree;			// The highest degree polynomial which can
							// be stored (array length - 1)
	double *coeffs;			// The pointer to the coefficient array.

	// Avoid heap use for typical polynomials.
	double coeff_array[COEFF_ARRAY_MAX+1];	
											
	// The argument determines max_degree.
	poly_def( int max_deg = -1 ) : degree(-1), max_degree( max_deg) { create(); }

	// The integer sets max_degree if it is	greater than the degree of the poly.
	poly_def( poly_def const &, int = -1 );
							
	// Special constructor used for return by value.
	// The data, if any, is moved into the new object.
	poly_def( logical reset_this, poly_def *def);

	~poly_def() { destroy(); }

	void create();	// Conditionally allocates the coefficient array.
	void destroy();	// Conditionally deallocates the coefficient array.
	void reset() { degree = -1; max_degree = COEFF_ARRAY_MAX; coeffs = coeff_array; }
	void init( int max_deg);	// Expands the coeff array without preserving data.
	void expand( int max_deg);	// Expands the coeff array and preserves existing data.
	// Initializes this poly, without preserving data, with that poly.
	void init( poly_def const &def ); 
};

/**
 * @nodoc
 */
class DECL_KERN polynomial : public ACIS_OBJECT {
	
	poly_def def;

public:

	polynomial() : def( -1) {}
	polynomial( polynomial const &in_poly ) : def( in_poly.def) {}
	polynomial( poly_def const &in_def ) : def( in_def) {}
	polynomial( logical reset, poly_def *in_def ) : def( reset, in_def) {} 
	polynomial( double );			// Make a constant
	polynomial( double, double, double = 0 );
									// linear and quadratic happen
									// to be useful to us
	polynomial( int, double * );

	//~polynomial() {};

	// Query, obtain an array of doubles representing the coefficients
	// The caller is responsible for deleting this array.  
	// Also (optionally) return the size of the array, which is degree+1.
	double *get_coeff_array(int &ncoeffs = *(int*)NULL_REF) const;

	// Indexing - treat a polynomial as an array of doubles, but
	// special action for non-constant ones to allow resetting.

	friend class poly_coeff;
	
	// Array indexing. This is a bit complex, as we wish to provide fully
	// automatic degree adjustment. We do this by returning a class which
	// merely records the polynomial and offset, but whose methods handle
	// assignment and conversion to double.
	poly_coeff operator[]( int );


	// Array indexing. This routine is invoked if the polynomial is
	// constant, and just returns the double value.
	double operator[]( int index) const { return index < 0 ? 0.0 
		: index > def.degree ? 0.0 : def.coeffs[ index ]; }
	
	// Return the degree of the polynomial. It is not quite clear what
	// we should do about zero constants. For consistency the degree should
	// be -1 (as stored internally), but it is more conventional to say
	// 0. We shall follow the latter course for now.
	int degree() const { return def.degree < 0 ? 0 : def.degree; }

	// Assignment, to ensure proper handling of the subsidiary object
	polynomial const &operator=( polynomial const & );

	// Arithmetic operators. Most operations with constants are done
	// using the implicit double-to-polynomial conversion (though this
	// could change if it turns out to be desirable). These operators
	// are friends rather than member functions in order to allow
	// the double to be the left-hand operand.

	friend DECL_KERN polynomial operator+(
			polynomial const &,
			polynomial const &
		);
	friend DECL_KERN polynomial operator-(
			polynomial const &
		);
	friend DECL_KERN polynomial operator-(
			polynomial const &,
			polynomial const &
		);
	friend DECL_KERN polynomial operator*(
			polynomial const &,
			polynomial const &
		);
	friend DECL_KERN polynomial operator/(
			polynomial const &,
			double
		);
	friend DECL_KERN rat_poly operator/(
			polynomial const &,
			polynomial const &
		);

	// Arithmetic assignments

	polynomial const &operator+=( polynomial const & );
	polynomial const &operator-=( polynomial const & );
	polynomial const &operator*=( polynomial const & );
	polynomial const &operator/=( double );

	// Comparison, really only used for testing for a constant zero.

	logical operator==( double ) const;
	friend DECL_KERN logical operator==( double d, polynomial const &p ) {
		return p == d;
	}

	// Specials

	polynomial deriv() const;
	polynomial integ() const;

	// Evaluation

	double eval( double ) const;

	// Solution. Find the zeros in a given SPAparameter range (excluding
	// the ends), subject to a given error for multiple roots and
	// coincidences.

	int zeros( double, double, double, poly_zero *& ) const;

	// Solve for all change-points related to zeros in the given range.
	// Points returned are:
	//		exact zeros,
	//		turning points within tolerance band,
	//		points where the polynomial enters or leaves the
	//			tolerance band, and
	//		end points of the range which are in the tolerance band.

	int all_zeros(
				double, double,	// polynomial domain (range of SPAparameter)
				double,			// coincidence/tangency tolerance
				poly_zero *&	// returned array of zeros
			) const;

	// Printout

	void print( FILE * ) const;
};

// Class to implement array indexing in a polynomial. It contains
// simply a pointer to the polynomial and an offset, but we define
// an assignment operator to set the polynomial coefficient from
// a double, and an implicit conversion to the current value for an
// Rvalue.

// tbrv
/**
 * @nodoc
 */
class DECL_KERN poly_coeff : public ACIS_OBJECT {
	polynomial *poly;
	int index;

	poly_coeff( polynomial *p, int i ) {
		poly = p;
		index = i;
	}
public:
	operator double() const;		// provide the Rvalue

	double operator=( double );
							// set a new coefficient value, and
							// adjust the degree appropriately.

	friend class polynomial;
};

// Rational polynomial. This is easy, as we do not implement much, and
// we always assume that the denominator is never zero (at any
// requested SPAparameter value or in any given range).

class rat_poly;
// tbrv
/**
 * @nodoc
 */
DECL_KERN rat_poly operator+(rat_poly const &,rat_poly const &);
// tbrv
/**
 * @nodoc
 */
DECL_KERN rat_poly operator-(rat_poly const &);
// tbrv
/**
 * @nodoc
 */
DECL_KERN rat_poly operator-(rat_poly const &,rat_poly const &);
// tbrv
/**
 * @nodoc
 */
DECL_KERN rat_poly operator*(rat_poly const &,rat_poly const &);
// tbrv
/**
 * @nodoc
 */
DECL_KERN rat_poly operator/(rat_poly const &,rat_poly const &);

// tbrv
/**
 * @nodoc
 */
class DECL_KERN rat_poly : public ACIS_OBJECT {
	polynomial num;
	polynomial den;
public:
	rat_poly(): num( 0.0 ), den( 1.0 ) {}
	rat_poly( polynomial const &n ): num( n ), den( 1.0 ) {}
	rat_poly( polynomial const &, polynomial const & );
	rat_poly( rat_poly const &r ): num( r.num ), den( r.den ) {}

	// Extract the bits

	polynomial numerator() const
		{ return num; }
	polynomial denominator() const
		{ return den; }

	// Assignment, defined to ensure that the assignments of the
	// polynomials get done right.

	rat_poly const &operator=( rat_poly const &rhs ) {
		num = rhs.num;
		den = rhs.den;
		return *this;
	}

	// Arithmetic operators. Note that we make no attempt to reduce
	// the results to lowest terms, and so the general rational-
	// rational operations are not very helpful. However, it is
	// convenient to express them this way for conciseness. They are
	// almost always used with one argument actually a polynomial,
	// or occasionally we may square a rational, when we know there
	// cannot be any common factors.

	friend DECL_KERN rat_poly operator+(
			rat_poly const &,
			rat_poly const &
		);
	friend DECL_KERN rat_poly operator-(
			rat_poly const &
		);
	friend DECL_KERN rat_poly operator-(
			rat_poly const &,
			rat_poly const &
		);
	friend DECL_KERN rat_poly operator*(
			rat_poly const &,
			rat_poly const &
		);
	friend DECL_KERN rat_poly operator/(
			rat_poly const &,
			rat_poly const &
		);

	// Arithmetic assignments

	rat_poly const &operator+=( rat_poly const & );
	rat_poly const &operator-=( rat_poly const & );
	rat_poly const &operator*=( rat_poly const & );
	rat_poly const &operator/=( rat_poly const & );

	// Calculus

	rat_poly deriv() const;

	// Evaluate the function at a given SPAparameter value

	double eval( double t ) const;

	// Find the zeros of a rational polynomial in a given SPAparameter
	// SPAinterval, subject to a given tolerance on multiple roots and
	// coincidences.

	int zeros(
				double,			// low end of SPAparameter SPAinterval
				double,			// high end of SPAparameter SPAinterval
				double,			// allowable tolerance on double roots
								// i.e. function value at a zero of the
								// derivative which we consider to be
								// zero.
				poly_zero *&	// set to point to an array of
								// records describing the roots.
			) const;

	// Solve for all change-points related to zeros in the given range.
	// Points returned are:
	//		exact zeros,
	//		turning points within tolerance band,
	//		points where the polynomial enters or leaves the
	//			tolerance band, and
	//		end points of the range which are in the tolerance band.

	int all_zeros(
				double, double,	// polynomial domain (range of SPAparameter)
				double,			// coincidence/tangency tolerance
				poly_zero *&	// returned array of zeros
			) const;

	// Printout

	void print( FILE * ) const;

	// Some friends needed elsewhere, so that they can get at our
	// private members.

	friend class rat_poly_vec;

	friend DECL_KERN rat_poly_vec operator*(
			rat_poly const &,
			rat_poly_vec const &
		);
	friend DECL_KERN rat_poly_vec operator/(
			rat_poly_vec const &,
			rat_poly const &
		);
};

// Classify the types of zeros of polynomial functions.
// tbrv
/**
 * @nodoc
 */
enum poly_zero_type {
	poly_exact_zero,
	poly_turning_point,
	poly_entering_tol_band,
	poly_leaving_tol_band,
	poly_end_of_range,
	poly_type_unknown
};

// Classify the spans adjacent to a zero. Each side
// may be positive or negative, possibly tangent, or coincident.

// tbrv
/**
 * @nodoc
 */
enum poly_zero_rel {
	poly_negative = -2,
	poly_neg_tan,
	poly_coincident,
	poly_pos_tan,
	poly_positive,
	poly_off_end,
	poly_unknown
};

// Class to hold information on a zero of a polynomial.

class poly_zero;
/**
 * @nodoc
 */
DECL_KERN poly_vec operator+( poly_vec const &, poly_vec const & );
/**
 * @nodoc
 */
DECL_KERN poly_vec operator-( poly_vec const & );
/**
 * @nodoc
 */
DECL_KERN poly_vec operator-( poly_vec const &, poly_vec const & );
/**
 * @nodoc
 */
DECL_KERN poly_vec operator*( poly_vec const &, polynomial const & );
/**
 * @nodoc
 */
DECL_KERN poly_vec operator*( polynomial const &, poly_vec const & );
/**
 * @nodoc
 */
DECL_KERN poly_vec operator/( poly_vec const &, double );
/**
 * @nodoc
 */
DECL_KERN poly_vec operator*( poly_vec const &, poly_vec const & );
/**
 * @nodoc
 */
DECL_KERN polynomial operator%( poly_vec const &, poly_vec const & );


// tbrv
/**
 * @nodoc
 */
class DECL_KERN poly_zero : public ACIS_OBJECT {
public:
	double param;

	poly_zero_rel low_rel;
	poly_zero_rel high_rel;

	poly_zero_type type;

	void print( FILE * ) const;
};

// A SPAvector of polynomials (or a polynomial of vectors). For the moment
// we do not distinguish between SPAposition and displacement vectors.

// tbrv
/**
 * @nodoc
 */
class DECL_KERN poly_vec : public ACIS_OBJECT {
	polynomial x;
	polynomial y;
	polynomial z;
public:
	poly_vec(); // {}
	poly_vec( SPAposition const & );
	poly_vec( SPAvector const & );
	poly_vec(
			polynomial const &,
			polynomial const &,
			polynomial const &
		);
	poly_vec( poly_vec const & );

	// Query
	polynomial const &get_x() const { return x; }
	polynomial const &get_y() const { return y; }
	polynomial const &get_z() const { return z; }

	// Assignment

	poly_vec const &operator=( poly_vec const & );

	// Arithmetic

	friend DECL_KERN poly_vec operator+( poly_vec const &, poly_vec const & );
	friend DECL_KERN poly_vec operator-( poly_vec const & );
	friend DECL_KERN poly_vec operator-( poly_vec const &, poly_vec const & );
	friend DECL_KERN poly_vec operator*( poly_vec const &, polynomial const & );
	friend DECL_KERN poly_vec operator*( polynomial const &, poly_vec const & );
	friend DECL_KERN poly_vec operator/( poly_vec const &, double );
	friend DECL_KERN poly_vec operator*( poly_vec const &, poly_vec const & );
	friend DECL_KERN polynomial operator%( poly_vec const &, poly_vec const & );

	// Arithmetic assignment

	poly_vec const &operator+=( poly_vec const & );
	poly_vec const &operator-=( poly_vec const & );
	poly_vec const &operator*=( polynomial const & );
	poly_vec const &operator/=( double );

	// Calculus

	poly_vec deriv() const;
	poly_vec integ() const;

	// Evaluation

	SPAvector eval( double ) const;

	// Printout

	void print( FILE * ) const;
};

// A SPAvector of rational polynomials (or more accurately a rational
// polynomial of vectors, as the denominator is the same for all
// coordinates). For the moment we do not distinguish between SPAposition
// and displacement vectors.

class rat_poly_vec;
/**
 * @nodoc
 */
DECL_KERN rat_poly_vec operator+(rat_poly_vec const &,rat_poly_vec const &);
/**
 * @nodoc
 */
DECL_KERN rat_poly_vec operator-(rat_poly_vec const &);
/**
 * @nodoc
 */
DECL_KERN rat_poly_vec operator-(rat_poly_vec const &,rat_poly_vec const &);
/**
 * @nodoc
 */
DECL_KERN rat_poly_vec operator*(rat_poly const &,rat_poly_vec const &);
/**
 * @nodoc
 */
DECL_KERN rat_poly_vec operator*(rat_poly_vec const &lhs,rat_poly const &rhs);
/**
 * @nodoc
 */
DECL_KERN rat_poly_vec operator/(rat_poly_vec const &,rat_poly const &);
/**
 * @nodoc
 */
DECL_KERN rat_poly_vec operator*(rat_poly_vec const &,	rat_poly_vec const &);
/**
 * @nodoc
 */
DECL_KERN rat_poly operator%(rat_poly_vec const &,rat_poly_vec const &);


/**
 * A SPAvector of rational polynomials (or more accurately a rational
 * polynomial of vectors, as the denominator is the same for all
 * coordinates). For the moment we do not distinguish between SPAposition
 * and displacement vectors.
 */
class DECL_KERN rat_poly_vec : public ACIS_OBJECT {
	poly_vec num;
	polynomial den;
public:
	rat_poly_vec();
	rat_poly_vec( poly_vec const &n );
	rat_poly_vec( poly_vec const &n, polynomial const &d );
	rat_poly_vec( rat_poly_vec const &old );

	~rat_poly_vec();

	// Extract the bits

	poly_vec numerator() const
		{ return num; }
	polynomial denominator() const
		{ return den; }

	// Assignment

	rat_poly_vec const &operator=( rat_poly_vec const & );

	// Arithmetic operators. Note that we make no attempt to reduce
	// the results to lowest terms, and so the general rational-
	// rational operations are not very helpful. However, it is
	// convenient to express them this way for conciseness. They are
	// almost always used with one argument actually a poly_vec,
	// or occasionally we may square a rational, when we know there
	// cannot be any common factors.

	friend DECL_KERN rat_poly_vec operator+(
			rat_poly_vec const &,
			rat_poly_vec const &
		);
	friend DECL_KERN rat_poly_vec operator-(
			rat_poly_vec const &
		);
	friend DECL_KERN rat_poly_vec operator-(
			rat_poly_vec const &,
			rat_poly_vec const &
		);
	friend DECL_KERN rat_poly_vec operator*(
			rat_poly const &,
			rat_poly_vec const &
		);
	friend DECL_KERN rat_poly_vec operator*(
			rat_poly_vec const &lhs,
			rat_poly const &rhs
		) {
		return rhs * lhs;
	}
	friend DECL_KERN rat_poly_vec operator/(
			rat_poly_vec const &,
			rat_poly const &
		);
	friend DECL_KERN rat_poly_vec operator*(
			rat_poly_vec const &,
			rat_poly_vec const &
		);
	friend DECL_KERN rat_poly operator%(
			rat_poly_vec const &,
			rat_poly_vec const &
		);

	// Arithmetic assignment

	rat_poly_vec const &operator+=( rat_poly_vec const & );
	rat_poly_vec const &operator-=( rat_poly_vec const & );
	rat_poly_vec const &operator*=( rat_poly const & );
	rat_poly_vec const &operator/=( rat_poly const & );

	// Calculus

	rat_poly_vec deriv() const;

	// Evaluation

	SPAvector eval( double ) const;

	// Printout

	void print( FILE * ) const;
};

// Constructors of rat_poly_vec
/**
 * @nodoc
 */
inline rat_poly_vec::rat_poly_vec(): num(), den( 1 ) {}
/**
 * @nodoc
 */
inline rat_poly_vec::rat_poly_vec( poly_vec const &n ): num( n ), den( 1 ) {}
/**
 * @nodoc
 */
inline rat_poly_vec::rat_poly_vec( poly_vec const &n, polynomial const &d ):
					num( n ), den( d ) {}
/**
 * @nodoc
 */
inline rat_poly_vec::rat_poly_vec( rat_poly_vec const &old ):
					num( old.num ), den( old.den ) {}


/*! @} */
#endif
