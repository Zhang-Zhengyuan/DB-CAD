/* ORIGINAL: acis2.1/kernutil/bipoly/bipoly.hxx */
/* $Id: bipoly.hxx,v 1.7 2002/08/09 17:15:28 jeff Exp $ */
/*******************************************************************/
/*    Copyright (c) 1989-2020 by Spatial Corp.                     */
/*    All rights reserved.                                         */
/*    Protected by U.S. Patents 5,257,205; 5,351,196; 6,369,815;   */
/*                              5,982,378; 6,462,738; 6,941,251    */
/*    Protected by European Patents 0503642; 69220263.3            */
/*    Protected by Hong Kong Patent 1008101A                       */
/*******************************************************************/

// Class for implementing polynomial and rational polynomial
// functions of a two variables.

// For a bipolynomial, we store the degree in the u direction and an
// array of polynomials, starting with the u constant term. The
// array is allocated on the heap, and this heap object also
// incorporates an array-size count and a reference count. The
// class itself is simply a pointer to this object.

// A rational bipolynomial is just a pair of bipolynomials. Arithmetic
// on rational bipolynomials is only moderately useful, as we do not
// attempt to find common factors in the denominators when adding or
// subtracting, or to reduce the fraction to its lowest terms when
// multiplying or dividing.

// A bipolynomial SPAvector is a triple of bipolynomials, representing a
// SPAposition in 3D. A rational bipolynomial SPAvector is a bipolynomial
// SPAvector and a bipolynomial denominator.

// We do not distinguish between positions and vectors in this code -
// "vectors" are used for both.

// Guard against being included more than once

#if !defined( BIPOLYNOMIAL )
#define BIPOLYNOMIAL

#include "dcl_kern.h"
#include "logical.h"

#include "poly.hxx"

class bipoly_def;
class rat_bipoly;
class bipoly_vec;
class rat_bipoly_vec;

class bipoly_row;
class bipoly_coeff;

class SPAvector;
class SPAposition;

class bipolynomial;
DECL_KERN bipolynomial operator+(bipolynomial const &,bipolynomial const &);
DECL_KERN bipolynomial operator-(bipolynomial const &);
DECL_KERN bipolynomial operator-(bipolynomial const &,bipolynomial const &);
DECL_KERN bipolynomial operator*(bipolynomial const &,bipolynomial const &);
DECL_KERN bipolynomial operator/(bipolynomial const &,double);
DECL_KERN rat_bipoly operator/(bipolynomial const &,bipolynomial const &);
DECL_KERN logical operator==( double d, bipolynomial const &p );

class DECL_KERN bipolynomial : public ACIS_OBJECT {
	bipoly_def *def;

	bipolynomial( bipoly_def *p );

	void make_single_ref();
public:
	bipolynomial();
	bipolynomial( bipolynomial const & );
	bipolynomial( double );			// Make a constant
	bipolynomial( int, int, double * );
									// The double * points to an
									// array of coefficients, of
									// exactly the given size.

	~bipolynomial();

	// Indexing - treat a bipolynomial as an array of polynomials.

	friend class bipoly_coeff;
	friend class bipoly_row;
	bipoly_row operator[]( int );
	polynomial operator[]( int ) const;
	int u_degree() const;
	int v_degree() const;

	// Assignment, to ensure proper handling of the subsidiary object

	bipolynomial const &operator=( bipolynomial const & );

	// Arithmetic operators. Most operations with constants are done
	// using the implicit double-to-bipolynomial conversion (though
	// this could change if it turns out to be desirable). These
	// operators are friends rather than member functions in order to
	// allow the double to be the left-hand operand.

	friend DECL_KERN bipolynomial operator+(
			bipolynomial const &,
			bipolynomial const &
		);
	friend DECL_KERN bipolynomial operator-(
			bipolynomial const &
		);
	friend DECL_KERN bipolynomial operator-(
			bipolynomial const &,
			bipolynomial const &
		);
	friend DECL_KERN bipolynomial operator*(
			bipolynomial const &,
			bipolynomial const &
		);
	friend DECL_KERN bipolynomial operator/(
			bipolynomial const &,
			double
		);
	friend DECL_KERN rat_bipoly operator/(
			bipolynomial const &,
			bipolynomial const &
		);

	// Arithmetic assignments

	bipolynomial const &operator+=( bipolynomial const & );
	bipolynomial const &operator-=( bipolynomial const & );
	bipolynomial const &operator*=( bipolynomial const & );
	bipolynomial const &operator/=( double );

	// Simple equality to a constant. Only likely to be used to
	// compare with zero.

	logical operator==( double ) const;
	friend DECL_KERN logical operator==( double d, bipolynomial const &p ) {
		return p == d;
	}

	// Specials

	bipolynomial u_deriv() const;
	bipolynomial v_deriv() const;
	bipolynomial u_integ() const;
	bipolynomial v_integ() const;

	// Evaluation

	double eval( double, double ) const;
	polynomial u_eval( double ) const;
	polynomial v_eval( double ) const;

	// Printout

	void print( char const *, FILE * ) const;
};

// Classes to implement array indexing in a bipolynomial. They contain
// simply a pointer to the bipolynomial and one or two offsets, but we
// define an assignment operator to set the polynomial coefficient
// from a double, and an implicit conversion to the current value for
// an Rvalue.

class DECL_KERN bipoly_row : public ACIS_OBJECT {
	bipolynomial *bipoly;
	int index;

	bipoly_row( bipolynomial *p, int i ) {
		bipoly = p;
		index = i;
	}

	bipoly_row( bipoly_row const &old ) {
		bipoly = old.bipoly;
		index = old.index;
	}
public:
	operator polynomial() const;	// provide the Rvalue

	polynomial operator=( polynomial const & ) const;
							// set a new coefficient value, and
							// adjust the degree appropriately.

	bipoly_coeff operator[]( int );

	friend class bipolynomial;
	friend class bipoly_coeff;
};

class DECL_KERN bipoly_coeff : public ACIS_OBJECT {
	bipoly_row row;
	int index;

	bipoly_coeff( bipoly_row &r, int j ): row( r ), index( j ) {}
public:
	bipoly_coeff( bipoly_coeff const &old ):
					row( old.row ), index( old.index ) {}

	operator double() const;		// provide the Rvalue

	double operator=( double ) const;
							// set a new coefficient value, and
							// adjust the degree appropriately.

	friend class bipolynomial;
	friend class bipoly_row;
};

// Rational bipolynomial. This is fairly easy, as we take advantage
// of bipolynomial to do the hard work.

DECL_KERN rat_bipoly operator+(rat_bipoly const &,rat_bipoly const &);
DECL_KERN rat_bipoly operator-(rat_bipoly const &);
DECL_KERN rat_bipoly operator-(rat_bipoly const &,rat_bipoly const &);
DECL_KERN rat_bipoly operator*(rat_bipoly const &,rat_bipoly const &);
DECL_KERN rat_bipoly operator/(rat_bipoly const &,rat_bipoly const &);
DECL_KERN rat_bipoly_vec operator*(rat_bipoly const &,rat_bipoly_vec const &);
DECL_KERN rat_bipoly_vec operator/(rat_bipoly_vec const &,rat_bipoly const &);

class DECL_KERN rat_bipoly : public ACIS_OBJECT {
	bipolynomial num;
	bipolynomial den;
public:
	rat_bipoly(): num( 0.0 ), den( 1.0 ) {}
	rat_bipoly( bipolynomial const &n ): num( n ), den( 1.0 ) {}
	rat_bipoly( bipolynomial const &, bipolynomial const & );
	rat_bipoly( rat_bipoly const &r ): num( r.num ), den( r.den ) {}

	// Extract the bits

	bipolynomial numerator() const
		{ return num; }
	bipolynomial denominator() const
		{ return den; }

	// Assignment, defined to ensure that the assignments of the
	// bipolynomials get done right.

	rat_bipoly const &operator=( rat_bipoly const &rhs ) {
		num = rhs.num;
		den = rhs.den;
		return *this;
	}

	// Arithmetic operators. Note that we make no attempt to reduce
	// the results to lowest terms, and so the general rational-
	// rational operations are not very helpful. However, it is
	// convenient to express them this way for conciseness. They are
	// almost always used with one argument actually a bipolynomial,
	// or occasionally we may square a rational, when we know there
	// cannot be any common factors.

	friend DECL_KERN rat_bipoly operator+(
			rat_bipoly const &,
			rat_bipoly const &
		);
	friend DECL_KERN rat_bipoly operator-(
			rat_bipoly const &
		);
	friend DECL_KERN rat_bipoly operator-(
			rat_bipoly const &,
			rat_bipoly const &
		);
	friend DECL_KERN rat_bipoly operator*(
			rat_bipoly const &,
			rat_bipoly const &
		);
	friend DECL_KERN rat_bipoly operator/(
			rat_bipoly const &,
			rat_bipoly const &
		);

	// Arithmetic assignments

	rat_bipoly const &operator+=( rat_bipoly const & );
	rat_bipoly const &operator-=( rat_bipoly const & );
	rat_bipoly const &operator*=( rat_bipoly const & );
	rat_bipoly const &operator/=( rat_bipoly const & );

	// Calculus

	rat_bipoly u_deriv() const;
	rat_bipoly v_deriv() const;

	// Evaluate the function at a given SPAparameter value

	double eval( double, double ) const;
	rat_poly u_eval( double ) const;
	rat_poly v_eval( double ) const;

	// Printout

	void print( char const *, FILE * ) const;

	// Some friends needed elsewhere, so that they can get at our
	// private members.

	friend class rat_bipoly_vec;

	friend DECL_KERN rat_bipoly_vec operator*(
			rat_bipoly const &,
			rat_bipoly_vec const &
		);
	friend DECL_KERN rat_bipoly_vec operator/(
			rat_bipoly_vec const &,
			rat_bipoly const &
		);
};

// A SPAvector of bipolynomials (or a bipolynomial of vectors). We do not
// distinguish between SPAposition and displacement vectors.

DECL_KERN bipoly_vec operator+( bipoly_vec const &, bipoly_vec const & );
DECL_KERN bipoly_vec operator-( bipoly_vec const & );
DECL_KERN bipoly_vec operator-( bipoly_vec const &, bipoly_vec const & );
DECL_KERN bipoly_vec operator*( bipoly_vec const &, bipolynomial const & );
DECL_KERN bipoly_vec operator*( bipolynomial const &, bipoly_vec const & );
DECL_KERN bipoly_vec operator/( bipoly_vec const &, double );
DECL_KERN bipoly_vec operator*( bipoly_vec const &, bipoly_vec const & );
DECL_KERN bipolynomial operator%( bipoly_vec const &, bipoly_vec const & );

class DECL_KERN bipoly_vec : public ACIS_OBJECT {
	bipolynomial x;
	bipolynomial y;
	bipolynomial z;
public:
	bipoly_vec();
	bipoly_vec( SPAposition const & );
	bipoly_vec( SPAvector const & );
	bipoly_vec(
			bipolynomial const &,
			bipolynomial const &,
			bipolynomial const &
		);
	bipoly_vec( bipoly_vec const & );

	// Assignment

	bipoly_vec const &operator=( bipoly_vec const & );

	// Arithmetic

	friend DECL_KERN bipoly_vec operator+( bipoly_vec const &, bipoly_vec const & );
	friend DECL_KERN bipoly_vec operator-( bipoly_vec const & );
	friend DECL_KERN bipoly_vec operator-( bipoly_vec const &, bipoly_vec const & );
	friend DECL_KERN bipoly_vec operator*( bipoly_vec const &, bipolynomial const & );
	friend DECL_KERN bipoly_vec operator*( bipolynomial const &, bipoly_vec const & );
	friend DECL_KERN bipoly_vec operator/( bipoly_vec const &, double );
	friend DECL_KERN bipoly_vec operator*( bipoly_vec const &, bipoly_vec const & );
	friend DECL_KERN bipolynomial operator%( bipoly_vec const &, bipoly_vec const & );

	// Arithmetic assignment

	bipoly_vec const &operator+=( bipoly_vec const & );
	bipoly_vec const &operator-=( bipoly_vec const & );
	bipoly_vec const &operator*=( bipolynomial const & );
	bipoly_vec const &operator/=( double );

	// Calculus

	bipoly_vec u_deriv() const;
	bipoly_vec v_deriv() const;
	bipoly_vec u_integ() const;
	bipoly_vec v_integ() const;

	// Evaluation

	SPAvector eval( double, double ) const;
	poly_vec u_eval( double ) const;
	poly_vec v_eval( double ) const;

	// Printout

	void print( char const *, FILE * ) const;
};

// A SPAvector of rational bipolynomials (or more accurately a rational
// bipolynomial of vectors, as the denominator is the same for all
// coordinates). We do not distinguish between SPAposition and
// displacement vectors.
class rat_bipoly_vec;
DECL_KERN rat_bipoly_vec operator+(rat_bipoly_vec const &,rat_bipoly_vec const &);
DECL_KERN rat_bipoly_vec operator-(rat_bipoly_vec const &);
DECL_KERN rat_bipoly_vec operator-(rat_bipoly_vec const &,rat_bipoly_vec const &);
DECL_KERN rat_bipoly_vec operator*(rat_bipoly const &,rat_bipoly_vec const &);
DECL_KERN rat_bipoly_vec operator*(rat_bipoly_vec const &lhs,rat_bipoly const &rhs);
DECL_KERN rat_bipoly_vec operator/(rat_bipoly_vec const &,rat_bipoly const &);
DECL_KERN rat_bipoly_vec operator*(rat_bipoly_vec const &,rat_bipoly_vec const &);
DECL_KERN rat_bipoly operator%(rat_bipoly_vec const &,rat_bipoly_vec const &);

class DECL_KERN rat_bipoly_vec : public ACIS_OBJECT {
	bipoly_vec num;
	bipolynomial den;
public:
	rat_bipoly_vec(): num(), den( 1 ) {}
	rat_bipoly_vec( bipoly_vec const &n ): num( n ), den( 1 ) {}
	rat_bipoly_vec( bipoly_vec const &n, bipolynomial const &d ):
					num( n ), den( d ) {}
	rat_bipoly_vec( rat_bipoly_vec const &old ):
					num( old.num ), den( old.den ) {}

	~rat_bipoly_vec();

	// Extract the bits

	bipoly_vec numerator() const
		{ return num; }
	bipolynomial denominator() const
		{ return den; }

	// Assignment

	rat_bipoly_vec const &operator=( rat_bipoly_vec const & );

	// Arithmetic operators. Note that we make no attempt to reduce
	// the results to lowest terms, and so the general rational-
	// rational operations are not very helpful. However, it is
	// convenient to express them this way for conciseness. They are
	// almost always used with one argument actually a bipoly_vec,
	// or occasionally we may square a rational, when we know there
	// cannot be any common factors.

	friend DECL_KERN rat_bipoly_vec operator+(
			rat_bipoly_vec const &,
			rat_bipoly_vec const &
		);
	friend DECL_KERN rat_bipoly_vec operator-(
			rat_bipoly_vec const &
		);
	friend DECL_KERN rat_bipoly_vec operator-(
			rat_bipoly_vec const &,
			rat_bipoly_vec const &
		);
	friend DECL_KERN rat_bipoly_vec operator*(
			rat_bipoly const &,
			rat_bipoly_vec const &
		);
	friend DECL_KERN rat_bipoly_vec operator*(
			rat_bipoly_vec const &lhs,
			rat_bipoly const &rhs
		) {
		return rhs * lhs;
	}
	friend DECL_KERN rat_bipoly_vec operator/(
			rat_bipoly_vec const &,
			rat_bipoly const &
		);
	friend DECL_KERN rat_bipoly_vec operator*(
			rat_bipoly_vec const &,
			rat_bipoly_vec const &
		);
	friend DECL_KERN rat_bipoly operator%(
			rat_bipoly_vec const &,
			rat_bipoly_vec const &
		);

	// Arithmetic assignment

	rat_bipoly_vec const &operator+=( rat_bipoly_vec const & );
	rat_bipoly_vec const &operator-=( rat_bipoly_vec const & );
	rat_bipoly_vec const &operator*=( rat_bipoly const & );
	rat_bipoly_vec const &operator/=( rat_bipoly const & );

	// Calculus

	rat_bipoly_vec u_deriv() const;
	rat_bipoly_vec v_deriv() const;

	// Evaluation

	SPAvector eval( double, double ) const;
	rat_poly_vec u_eval( double ) const;
	rat_poly_vec v_eval( double ) const;

	// Printout

	void print( char const *, FILE * ) const;
};

#endif
