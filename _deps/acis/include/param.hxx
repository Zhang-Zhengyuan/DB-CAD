/*******************************************************************/
/*    Copyright (c) 1989-2020 by Spatial Corp.                     */
/*    All rights reserved.                                         */
/*    Protected by U.S. Patents 5,257,205; 5,351,196; 6,369,815;   */
/*                              5,982,378; 6,462,738; 6,941,251    */
/*    Protected by European Patents 0503642; 69220263.3            */
/*    Protected by Hong Kong Patent 1008101A                       */
/*******************************************************************/
// Header for SPAparameter, SPApar_pos, SPApar_vec, SPApar_dir, 
// SPApar_box and SPApar_transf.
// These classes define a SPAparameter value along a curve, a pair of
// SPAparameter values for a SPAparameter point on a surface, a parametric
// direction on a surface, and a 2d SPAbox in SPAparameter space.
#if !defined( PARAMETER_CLASS )
#define PARAMETER_CLASS
#include "logical.h"
#include "base.hxx"
#include "dcl_base.h"
#include "interval.hxx"
#include "debugmsc.hxx"
/**
 * @file param.hxx
 * @CAA2Level L1
 * @CAA2Usage U1
 * \addtogroup ACISGEOMOPERATORS
 *
 * @{
 */

// Predeclare the later classes, for forward references

class SPApar_box;
class SPApar_pos;
class SPApar_vec;
class SPApar_dir;
class SPApar_transf;  // STI let (9/98): Added for transforming bs2_curves
class SPAparameter;

/**
 * Adds two <tt>SPAparameter</tt>s.
 * <br><br>
 * @param p1
 * SPAparameter 1.
 * @param p2
 * SPAparameter 2.
 */
DECL_BASE double operator+( SPAparameter const &p1, SPAparameter const &p2) ;

/**
 * Adds a <tt>SPAparameter</tt> and a double.
 * <br><br>
 * @param p
 * SPAparameter.
 * @param d
 * double.
 */
DECL_BASE double operator+( SPAparameter const &p, double d) ;

/**
 * Adds a double and a <tt>SPAparameter</tt>.
 * <br><br>
 * @param d
 * double.
 * @param p
 * SPAparameter.
 */
DECL_BASE double operator+( double d, SPAparameter const &p) ;

/**
 * Subtracts the second <tt>SPAparameter</tt> from the first <tt>SPAparameter</tt>.
 * <br><br>
 * @param p1
 * SPAparameter 1.
 * @param p2
 * SPAparameter 2.
 */
DECL_BASE double operator-( SPAparameter const &p1, SPAparameter const &p2) ;

/**
 * Subtracts a double from a <tt>SPAparameter</tt>.
 * <br><br>
 * @param p
 * SPAparameter.
 * @param d
 * double.
 */
DECL_BASE double operator-( SPAparameter const &p, double d) ;

/**
 * Subtracts a <tt>SPAparameter</tt> from a double.
 * <br><br>
 * @param d
 * double.
 * @param p
 * SPAparameter.
 */
DECL_BASE double operator-( double d, SPAparameter const &p) ;

/**
 * Multiplies two <tt>SPAparameter</tt>s.
 * <br><br>
 * @param p1
 * SPAparameter 1.
 * @param p2
 * SPAparameter 2.
 */
DECL_BASE double operator*( SPAparameter const &p1, SPAparameter const &p2) ;

/**
 * Multiplies a <tt>SPAparameter</tt> and a double.
 * <br><br>
 * @param p
 * SPAparameter.
 * @param d
 * double.
 */
DECL_BASE double operator*( SPAparameter const &p, double d) ;

/**
 * Multiplies a <tt>SPAparameter</tt> and a double.
 * <br><br>
 * @param d
 * double.
 * @param p
 * SPAparameter.
 */
DECL_BASE double operator*( double d, SPAparameter const &p) ;

/**
 * Multiplies a <tt>SPAparameter</tt> and an integer.
 * <br><br>
 * @param p
 * SPAparameter.
 * @param i
 * integer.
 */
DECL_BASE double operator*( SPAparameter const &p, int i) ;

/**
 * Multiplies a <tt>SPAparameter</tt> and an integer.
 * <br><br>
 * @param i
 * integer.
 * @param p
 * SPAparameter.
 */
DECL_BASE double operator*( int i, SPAparameter const &p) ;

/**
 * Divides the first <tt>SPAparameter</tt> by the second <tt>SPAparameter</tt>.
 * <br><br>
 * @param p1
 * SPAparameter 1.
 * @param p2
 * SPAparameter 2.
 */
DECL_BASE double operator/( SPAparameter const &p1, SPAparameter const &p2) ;

/**
 * Divides a <tt>SPAparameter</tt> by a double.
 * <br><br>
 * @param p
 * SPAparameter.
 * @param d
 * double.
 */
DECL_BASE double operator/( SPAparameter const &p, double d) ;

/**
 * Divides a double by a <tt>SPAparameter</tt>.
 * <br><br>
 * @param d
 * double.
 * @param p
 * SPAparameter.
 */
DECL_BASE double operator/( double d, SPAparameter const &p) ;

/**
 * Determines if the first <tt>SPAparameter</tt> is less than the second <tt>SPAparameter</tt>.
 * <br><br>
 * @param p1
 * SPAparameter 1.
 * @param p2
 * SPAparameter 2.
 */
DECL_BASE logical operator<( SPAparameter const &p1, SPAparameter const &p2) ;

/**
 * Determines if the <tt>SPAparameter</tt> is less than the double.
 * <br><br>
 * @param p
 * SPAparameter.
 * @param d
 * double.
 */
DECL_BASE logical operator<( SPAparameter const &p, double d) ;

/**
 * Determines if the double is less than the <tt>SPAparameter</tt>.
 * <br><br>
 * @param d
 * double.
 * @param p
 * SPAparameter.
 */
DECL_BASE logical operator<( double d, SPAparameter const &p) ;

/**
 * Determines if the first <tt>SPAparameter</tt> is greater than the second <tt>SPAparameter</tt>.
 * <br><br>
 * @param p1
 * SPAparameter 1.
 * @param p2
 * SPAparameter 2.
 */
DECL_BASE logical operator>( SPAparameter const &p1, SPAparameter const &p2) ;

/**
 * Determines if the <tt>SPAparameter</tt> is greater than the double.
 * <br><br>
 * @param p
 * SPAparameter.
 * @param d
 * double.
 */
DECL_BASE logical operator>( SPAparameter const &p, double d) ;

/**
 * Determines if the double is greater than the <tt>SPAparameter</tt>.
 * <br><br>
 * @param d
 * double.
 * @param p
 * SPAparameter.
 */
DECL_BASE logical operator>( double d, SPAparameter const &p) ;

/**
 * Determines if the first <tt>SPAparameter</tt> is less than or equal to the second <tt>SPAparameter</tt>.
 * <br><br>
 * @param p1
 * SPAparameter 1.
 * @param p2
 * SPAparameter 2.
 */
DECL_BASE logical operator<=( SPAparameter const &p1, SPAparameter const &p2) ;

/**
 * Determines if the <tt>SPAparameter</tt> is less than or equal to the double.
 * <br><br>
 * @param p
 * SPAparameter.
 * @param d
 * double.
 */
DECL_BASE logical operator<=( SPAparameter const &p, double d) ;

/**
 * Determines if the double is less than or equal to the <tt>SPAparameter</tt>.
 * <br><br>
 * @param d
 * double.
 * @param p
 * SPAparameter.
 */
DECL_BASE logical operator<=( double d, SPAparameter const &p) ;

/**
 * Determines if the first <tt>SPAparameter</tt> is greater than or equal to the second <tt>SPAparameter</tt>.
 * <br><br>
 * @param p1
 * SPAparameter 1.
 * @param p2
 * SPAparameter 2.
 */
DECL_BASE logical operator>=( SPAparameter const &p1, SPAparameter const &p2) ;

/**
 * Determines if the <tt>SPAparameter</tt> is greater than or equal to the double.
 * <br><br>
 * @param p
 * SPAparameter.
 * @param d
 * double.
 */
DECL_BASE logical operator>=( SPAparameter const &p, double d) ;

/**
 * Determines if the double is greater than or equal to the <tt>SPAparameter</tt>.
 * <br><br>
 * @param d
 * double.
 * @param p
 * SPAparameter.
 */
DECL_BASE logical operator>=( double d, SPAparameter const &p) ;

/**
 * Returns the sum of a parameter position with a parameter vector.
 * <br><br>
 * @param parpos
 * parameter position.
 * @param parvec
 * parameter vector.
 */
DECL_BASE SPApar_pos operator+( SPApar_pos const & parpos, SPApar_vec const & parvec);
DECL_BASE SPApar_pos gme_operator_add( SPApar_pos const & parpos, SPApar_vec const & parvec);
/**
 * Returns the sum of a parameter vector with a parameter position.
 * <br><br>
 * @param v
 * parameter vector.
 * @param p
 * parameter position.
 */
DECL_BASE SPApar_pos operator+( SPApar_vec const &v, SPApar_pos const &p );
DECL_BASE SPApar_pos gme_operator_add( SPApar_vec const &v, SPApar_pos const &p );
/**
 * Returns the subtraction of a parameter vector from a parameter position.
 * <br><br>
 * @param p
 * parameter position.
 * @param v
 * parameter vector.
 */
DECL_BASE SPApar_pos operator-( SPApar_pos const &p, SPApar_vec const &v );
DECL_BASE SPApar_pos gme_operator_sub( SPApar_pos const &p, SPApar_vec const &v );
/**
 * Returns the subtraction of the first parameter position from the second parameter position.
 * <br><br>
 * <b>Role:</b> It does so by determining the displacement vector from the first parameter position
 * to the second parameter position.
 * <br><br>
 * @param p1
 * parameter position.
 * @param p2
 * parameter position.
 */
DECL_BASE SPApar_vec operator-( SPApar_pos const &p1, SPApar_pos const & p2);
DECL_BASE SPApar_vec gme_operator_sub( SPApar_pos const &p1, SPApar_pos const & p2);
/**
 * Returns the scalar product of a parameter position with a parameter vector.
 * <br><br>
 * @param p
 * parameter position.
 * @param v
 * parameter vector.
 */
DECL_BASE double operator%( SPApar_pos const &p, SPApar_vec const &v );
DECL_BASE double gme_operator_dot( SPApar_pos const &p, SPApar_vec const &v );
/**
 * Returns the scalar product of a parameter vector with a parameter position.
 * <br><br>
 * @param v
 * parameter vector.
 * @param p
 * parameter position.
 */
DECL_BASE double operator%( SPApar_vec const &v, SPApar_pos const &p );
DECL_BASE double gme_operator_dot( SPApar_vec const &v, SPApar_pos const &p );
/**
 * Performs a unary minus operation on a parameter vector.
 * <br><br>
 * @param v
 * parameter vector.
 */
DECL_BASE SPApar_vec operator-( SPApar_vec const &v );
DECL_BASE SPApar_vec gme_operator_sub( SPApar_vec const &v );
/**
 * Adds the two parameter vectors.
 * <br><br>
 * @param v1
 * parameter vector.
 * @param v2
 * parameter vector.
 */
DECL_BASE SPApar_vec operator+( SPApar_vec const &v1, SPApar_vec const &v2 );
DECL_BASE SPApar_vec gme_operator_add( SPApar_vec const &v1, SPApar_vec const &v2 );
/**
 * Performs a binary minus operation.
 * <br><br>
 * @param v1
 * parameter vector.
 * @param v2
 * parameter vector.
 */
DECL_BASE SPApar_vec operator-( SPApar_vec const &v1, SPApar_vec const &v2 );
DECL_BASE SPApar_vec gme_operator_sub( SPApar_vec const &v1, SPApar_vec const &v2 );
/**
 * Multiplies a parameter vector by a scalar value.
 * <br><br>
 * @param v
 * parameter vector.
 * @param d
 * double.
 */
DECL_BASE SPApar_vec operator*( SPApar_vec const &v, double d);
DECL_BASE SPApar_vec gme_operator_multiply( SPApar_vec const &v, double d);
/**
 * Multiplies a parameter vector by a scalar value.
 * <br><br>
 * @param d
 * double.
 * @param v
 * parameter vector.
 */
DECL_BASE SPApar_vec operator*( double d, SPApar_vec const &v );
DECL_BASE SPApar_vec gme_operator_multiply( double d, SPApar_vec const &v );
/**
 * Divides a parameter vector by a scalar value.
 * <br><br>
 * @param v
 * parameter vector.
 * @param d
 * double.
 */
DECL_BASE SPApar_vec operator/( SPApar_vec const &v, double d);
DECL_BASE SPApar_vec gme_operator_divide( SPApar_vec const &v, double d);
/**
 * Returns the dot product of two parameter vectors.
 * <br><br>
 * @param v1
 * parameter vector.
 * @param v2
 * parameter vector.
 */
DECL_BASE double operator%( SPApar_vec const &v1, SPApar_vec const &v2 );
DECL_BASE double gme_operator_dot( SPApar_vec const &v1, SPApar_vec const &v2 );
/**
 * Returns the cross product of two parameter vectors.
 * <br><br>
 * @param v1
 * parameter vector.
 * @param v2
 * parameter vector.
 */
DECL_BASE double operator*( SPApar_vec const &v1, SPApar_vec const &v2 );
DECL_BASE double gme_operator_multiply( SPApar_vec const &v1, SPApar_vec const &v2 );
/**
 * Performs a unary minus operation.
 * <br><br>
 * @param u
 * parameter direction.
 */
DECL_BASE SPApar_dir operator-( SPApar_dir const & u);
DECL_BASE SPApar_dir gme_operator_sub( SPApar_dir const & u);
/**
 * Returns the scalar product between a parameter position and parameter direction.
 * <br><br>
 * @param p
 * parameter position.
 * @param u
 * parameter direction.
 */
DECL_BASE double operator%( SPApar_pos const &p, SPApar_dir const &u ) ;

/**
 * Returns the scalar product between a parameter direction and parameter position.
 * <br><br>
 * @param u
 * parameter direction.
 * @param p
 * parameter position.
 */
DECL_BASE double operator%( SPApar_dir const &u, SPApar_pos const &p ) ;

/**
 * Creates a parameter box that encloses the two given parameter boxes.
 * <br><br>
 * @param b1
 * first box.
 * @param b2
 * second box.
 */
DECL_BASE SPApar_box operator|( SPApar_box const &b1, SPApar_box const &b2 );
DECL_BASE SPApar_box gme_operator_or( SPApar_box const &b1, SPApar_box const &b2 );
/**
 * Creates a parameter box that is the overlap of two given parameter boxes, e.g., the intersection.
 * <br><br>
 * @param b1
 * first box.
 * @param b2
 * second box.
 */
DECL_BASE SPApar_box operator&( SPApar_box const &b1, SPApar_box const &b2 );
DECL_BASE SPApar_box gme_operator_and( SPApar_box const &b1, SPApar_box const &b2 );
/**
 * Determines whether two boxes overlap.
 * <br><br>
 * <b>Role:</b> This method returns <tt>TRUE</tt> if either box is <tt>NULL</tt> or if all the
 * intervals of one parameter box overlap the corresponding intervals of the other
 * parameter box.
 * <br><br>
 * @param b1
 * first parameter box.
 * @param b2
 * second parameter box.
 */
DECL_BASE logical operator&&( SPApar_box const &b1, SPApar_box const &b2 );
DECL_BASE logical gme_operator_and_and( SPApar_box const &b1, SPApar_box const &b2 );
/**
 * Determines if a given parameter box encloses a given parameter position.
 * <br><br>
 * @param p
 * parameter position.
 * @param b
 * parameter box.
 */
DECL_BASE logical operator<<( SPApar_pos const &p, SPApar_box const &b ) ;
DECL_BASE logical gme_operator_left_shift(SPApar_pos const& p, SPApar_box const& b);


/**
 * Translates a parameter box.
 * <br><br>
 * @param b
 * parameter box.
 * @param v
 * parameter vector.
 */
DECL_BASE SPApar_box operator+( SPApar_box const &b, SPApar_vec const &v );
DECL_BASE SPApar_box gme_operator_add( SPApar_box const &b, SPApar_vec const &v );
/**
 * Translates a parameter box.
 * <br><br>
 * @param v
 * parameter vector.
 * @param b
 * parameter box.
 */
DECL_BASE SPApar_box operator+( SPApar_vec const &v, SPApar_box const &b );
DECL_BASE SPApar_box gme_operator_add( SPApar_vec const &v, SPApar_box const &b );
/**
 * Translates a parameter box.
 * <br><br>
 * @param b
 * parameter box.
 * @param v
 * parameter vector.
 */
DECL_BASE SPApar_box operator-( SPApar_box const &b, SPApar_vec const &v );
DECL_BASE SPApar_box gme_operator_sub( SPApar_box const &b, SPApar_vec const &v );
/**
 * Finds the extent of a parameter box along a given parameter direction.
 * <br><br>
 * @param d
 * parameter direction.
 * @param b
 * parameter box.
 */
DECL_BASE SPAinterval operator%( SPApar_dir const &d, SPApar_box const & b);
DECL_BASE SPAinterval gme_operator_percent( SPApar_dir const &d, SPApar_box const & b);
/**
 * Finds the extent of a parameter box along a given parameter direction.
 * <br><br>
 * @param b
 * parameter box.
 * @param d
 * parameter direction.
 */
DECL_BASE SPAinterval operator%( SPApar_box const &b, SPApar_dir const &d );
DECL_BASE SPAinterval gme_operator_percent( SPApar_box const &b, SPApar_dir const &d );
/**
 * Transforms a parameter position using a parameter transform.
 * <br><br>
 * @param p
 * parameter position.
 * @param t
 * parameter transform.
 */
DECL_BASE SPApar_pos operator*( SPApar_pos const &p, SPApar_transf const &t );
DECL_BASE SPApar_pos gme_operator_multiply( SPApar_pos const &p, SPApar_transf const &t );
/** @} */
/**
 * \addtogroup ACISGEOMETRICATOMS
 *
 * @{
 */
// The SPAparameter class defines a curve SPAparameter value.  It is just a
// floating-point number, but we declare it as a class entity largely
// for consistency. Originally it was also for transformations, but
// now SPAparameter values are invariant under transformation, so
// transformation operators have been removed.

/**
 * Defines a curve parameter value.
 * <br>
 * <b>Role:</b> This class defines a curve parameter value. It is a floating-point
 * number, but it is declared as a class entity for consistency. Parameter values
 * are invariant under transformations.
 * @see SPApar_pos, SPApar_vec
 */
class DECL_BASE SPAparameter {

	double val;

	// The ordinary arithmetic operators are essentially those for
	// doubles. We put them explicitly to avoid problems with
	// ambiguities: the (V2.1) compiler does not handle ambiguity
	// resolution with standard operators in the same way as for
	// user-defined operators or functions, and without these
	// definitions complains about "parameter + double" for example,
	// selecting "double + interval" rather than "double + double"
	// which it should do.

	friend DECL_BASE double operator+( SPAparameter const &p1, SPAparameter const &p2 );
	friend DECL_BASE double operator+( SPAparameter const &p, double d );
	friend DECL_BASE double operator+( double d, SPAparameter const &p );

	friend DECL_BASE double operator-( SPAparameter const &p1, SPAparameter const &p2 );
	friend DECL_BASE double operator-( SPAparameter const &p, double d );
	friend DECL_BASE double operator-( double d, SPAparameter const &p );

	friend DECL_BASE double operator*( SPAparameter const &p1, SPAparameter const &p2 );
	friend DECL_BASE double operator*( SPAparameter const &p, double d );
	friend DECL_BASE double operator*( double d, SPAparameter const &p );
	friend DECL_BASE double operator*( SPAparameter const &p, int i );
	friend DECL_BASE double operator*( int i, SPAparameter const &p );

	friend DECL_BASE double operator/( SPAparameter const &p1, SPAparameter const &p2 );
	friend DECL_BASE double operator/( SPAparameter const &p, double d );
	friend DECL_BASE double operator/( double d, SPAparameter const &p );

	friend DECL_BASE logical operator<( SPAparameter const &p1, SPAparameter const &p2 );
	friend DECL_BASE logical operator<( SPAparameter const &p, double d );
	friend DECL_BASE logical operator<( double d, SPAparameter const &p );

	friend DECL_BASE logical operator>( SPAparameter const &p1, SPAparameter const &p2 );
	friend DECL_BASE logical operator>( SPAparameter const &p, double d	);
	friend DECL_BASE logical operator>( double d, SPAparameter const &p );

	friend DECL_BASE logical operator<=( SPAparameter const &p1, SPAparameter const &p2 );
	friend DECL_BASE logical operator<=( SPAparameter const &p, double d );
	friend DECL_BASE logical operator<=( double d, SPAparameter const &p );

	friend DECL_BASE logical operator>=( SPAparameter const &p1, SPAparameter const &p2 );
	friend DECL_BASE logical operator>=( SPAparameter const &p, double d );
	friend DECL_BASE logical operator>=( double d, SPAparameter const &p );

public:

	// allow uninitialised parameters
	/**
	 * C++ allocation constructor requests memory for this object but does not populate it.
	 */
	SPAparameter() 
#if 0
		; // for mkman
#endif
#ifdef INTERNAL_DEBUG_CHECKS
		;
#else
		{}
#endif


	// Construct a SPAparameter from a double.
	/**
	 * C++ initialize constructor requests memory for this object and populates it with the data supplied as arguments.
	 * <br><br>
	 * @param p
	 * double.
	 */
	SPAparameter( double p )
		{ val = p; }


	/**
	 * @nodoc
	 */
	MMGR_FREELIST_THIS
#if 0
; // semicolon needed for mkman (doc tool) parsing
#endif

	/**
	 * Returns a double from a parameter.
	 */
	operator double() const
		{ return val; }

	/**
	 * Negates a parameter.
	 */
	SPAparameter operator-() const
		{ return - val; }


	// Assignment operators use conversion to double for the rhs.

	/**
	 * Converts a double for addition to the right-hand system.
	 * <br><br>
	 * @param rhs
	 * double.
	 */
	SPAparameter operator+=( double rhs )
		{ return val += rhs; }

	/**
	 * Converts a double for subtraction to the right-hand system.
	 * <br><br>
	 * @param rhs
	 * double.
	 */
	SPAparameter operator-=( double rhs )
		{ return val -= rhs; }

	/**
	 * Converts a double for multiplication to the right-hand system.
	 * <br><br>
	 * @param rhs
	 * double.
	 */
	SPAparameter operator*=( double rhs )
		{ return val *= rhs; }

	/**
	 * Scales a box.
	 * <br><br>
	 * @param rhs
	 * double.
	 */
	SPAparameter operator/=( double rhs )
		{ return val /= rhs; }

	/**
	 * Outputs the details of a <tt>SPAparameter</tt> to the debug file or to the specified file.
	 * <br><br>
	 * @param fp
	 * file name.
	 */
	void debug( FILE *fp = debug_file_ptr ) const
		{ debug_dist( val, fp ); }
};

/** @} */
/**
 * \addtogroup ACISGEOMOPERATORS
 *
 * @{
 */

// SPAparameters friend functions that we feel need to be inlined.

/**
 * @nodoc
 */
inline double operator+( SPAparameter const &p1, SPAparameter const &p2 )
	{ return p1.val + p2.val; }
/**
 * @nodoc
 */
inline double operator+( SPAparameter const &p, double d )
	{ return p.val + d; }
/**
 * @nodoc
 */
inline double operator+( double d, SPAparameter const &p )
	{ return d + p.val; }

/**
 * @nodoc
 */
inline double operator-( SPAparameter const &p1, SPAparameter const &p2 )
	{ return p1.val - p2.val; }
/**
 * @nodoc
 */
inline double operator-( SPAparameter const &p, double d )
	{ return p.val - d; }
/**
 * @nodoc
 */
inline double operator-( double d, SPAparameter const &p )
	{ return d - p.val; }

/**
 * @nodoc
 */
inline double operator*( SPAparameter const &p1, SPAparameter const &p2 )
	{ return p1.val * p2.val; }
/**
 * @nodoc
 */
inline double operator*( SPAparameter const &p, double d )
	{ return p.val * d; }
/**
 * @nodoc
 */
inline double operator*( double d, SPAparameter const &p )
	{ return d * p.val; }
/**
 * @nodoc
 */
inline double operator*( SPAparameter const &p, int i )
	{ return p.val * i; }
/**
 * @nodoc
 */
inline double operator*( int i, SPAparameter const &p )
	{ return i * p.val; }

/**
 * @nodoc
 */
inline double operator/( SPAparameter const &p1, SPAparameter const &p2 )
	{ return p1.val / p2.val; }
/**
 * @nodoc
 */
inline double operator/( SPAparameter const &p, double d )
	{ return p.val / d; }
/**
 * @nodoc
 */
inline double operator/( double d, SPAparameter const &p )
	{ return d / p.val; }

/**
 * @nodoc
 */
inline logical operator<( SPAparameter const &p1, SPAparameter const &p2 )
	{ return p1.val < p2.val; }
/**
 * @nodoc
 */
inline logical operator<( SPAparameter const &p, double d )
	{ return p.val < d; }
/**
 * @nodoc
 */
inline logical operator<( double d, SPAparameter const &p )
	{ return d < p.val; }

/**
 * @nodoc
 */
inline logical operator>( SPAparameter const &p1, SPAparameter const &p2 )
	{ return p1.val > p2.val; }
/**
 * @nodoc
 */
inline logical operator>( SPAparameter const &p, double d	)
	{ return p.val > d; }
/**
 * @nodoc
 */
inline logical operator>( double d, SPAparameter const &p )
	{ return d > p.val; }

/**
 * @nodoc
 */
inline logical operator<=( SPAparameter const &p1, SPAparameter const &p2 )
	{ return p1.val <= p2.val; }
/**
 * @nodoc
 */
inline logical operator<=( SPAparameter const &p, double d )
	{ return p.val <= d; }
/**
 * @nodoc
 */
inline logical operator<=( double d, SPAparameter const &p )
	{ return d <= p.val; }

/**
 * @nodoc
 */
inline logical operator>=( SPAparameter const &p1, SPAparameter const &p2 )
	{ return p1.val >= p2.val; }
/**
 * @nodoc
 */
inline logical operator>=( SPAparameter const &p, double d )
	{ return p.val >= d; }
/**
 * @nodoc
 */
inline logical operator>=( double d, SPAparameter const &p )
	{ return d >= p.val; }


/** @} */
/**
 * \addtogroup ACISGEOMETRICATOMS
 *
 * @{
 */

/**
 * Defines a parameter position in the parameter-space of a surface.
 * <br>
 * <b>Role:</b> This class represents a 2D parameter value that defines a (<i>u</i>, <i>v</i>)
 * parameter-space coordinate that, when evaluated on a surface, produces a 3D object space coordinate.
 * @see SPAparameter
 */
class DECL_BASE SPApar_pos {

	// Simple arithmetic
	friend DECL_BASE SPApar_pos operator+( SPApar_pos const &, SPApar_vec const & );
	friend DECL_BASE SPApar_pos operator+( SPApar_vec const &v, SPApar_pos const &p );
	friend DECL_BASE SPApar_pos operator-( SPApar_pos const &, SPApar_vec const & );
	friend DECL_BASE SPApar_vec operator-( SPApar_pos const &, SPApar_pos const & );

	// Scalar product of SPAposition with SPAvector.
	friend DECL_BASE double operator%( SPApar_pos const &, SPApar_vec const & );
	friend DECL_BASE double operator%( SPApar_vec const &v, SPApar_pos const &p );

public:

	/**
	 * The <i>u</i>-parameter.
	 */
	SPAparameter u;

	/**
	 * The <i>v</i>-parameter.
	 */
	SPAparameter v;


	// allow uninitialised SPApar_pos
	/**
	 * C++ allocation constructor requests memory for this object but does not populate it.
	 */
    SPApar_pos() {}

	// Construct a SPApar_pos from two doubles.
	/**
	 * C++ initialize constructor requests memory for this object and populates it with the data supplied as arguments.
	 * <br><br>
	 * @param uval
	 * u-parameter value.
	 * @param vval
	 * v-parameter value.
	 */
	SPApar_pos( double uval, double vval ): u( uval ), v( vval ) {}


	// Construct a SPApar_pos from an array of two doubles.
	/**
	 * C++ initialize constructor requests memory for this object and populates it with the data supplied as arguments.
	 * <br><br>
	 * @param uv
	 * array of two doubles.
	 */
	SPApar_pos( double uv[ 2 ] ): u( uv[ 0 ] ), v( uv[ 1 ] ) {}


	/**
	 * @nodoc
	 */
	MMGR_FREELIST_THIS
#if 0
; // semicolon needed for mkman (doc tool) parsing
#endif

	/**
	 * Adds a vector to a parameter position by offsetting a parameter position by a parameter vector.
	 * <br><br>
	 * @param pv
	 * parameter vector.
	 */
	SPApar_pos const &operator+=( SPApar_vec const & pv);
	SPApar_pos const &gme_operator_add_assign( SPApar_vec const & pv);
	/**
	 * Subtracts a vector from a parameter position.
	 * <br><br>
	 * @param pv
	 * parameter vector.
	 */
	SPApar_pos const &operator-=( SPApar_vec const & pv);
	SPApar_pos const &gme_operator_sub_assign( SPApar_vec const & pv);
	// Transform a SPApar_pos by a SPApar_transf
	/**
	 * Transforms a <tt>SPApar_pos</tt>.
	 * <br><br>
	 * @param t
	 * parameter transform.
	 */
	SPApar_pos const &operator*=( SPApar_transf const & t);
	SPApar_pos const &gme_operator_multiply_assign( SPApar_transf const & t);
	/**
	 * Outputs details of a parameter position to the debug file or to the specified file.
	 * <br><br>
	 * @param fp
	 * file name.
	 */
	void debug( FILE *fp = debug_file_ptr ) const;
	void gme_debug( FILE *fp = debug_file_ptr ) const;
};

/** @} */
/**
 * \addtogroup ACISGEOMOPERATORS
 *
 * @{
 */
// Determine if two par_poses are equal or not
/**
 * Compares two parameter positions for equality within a specifiable tolerance.
 * <br><br>
 * @param p1
 * first parameter position.
 * @param p2
 * second parameter position.
 * @param tol
 * tolerance.
 */
DECL_BASE logical same_par_pos( SPApar_pos const &p1,
								SPApar_pos const &p2,
								double tol = SPAresnor );
DECL_BASE logical gme_same_par_pos( SPApar_pos const &p1,
								SPApar_pos const &p2,
								double tol = SPAresnor );
/**
 * Compares two paramater positions for equality within a tolerance of <tt>SPAresnor</tt>.
 * <br><br>
 * @param p1
 * first parameter position.
 * @param p2
 * second parameter position.
 */
DECL_BASE inline logical operator==( SPApar_pos const &p1, SPApar_pos const &p2 )
	{ return same_par_pos(p1, p2, SPAresnor); }


/**
 * Compares two paramater positions for inequality within a tolerance of <tt>SPAresnor</tt>.
 * <br><br>
 * @param p1
 * first parameter position.
 * @param p2
 * second parameter position.
 */
DECL_BASE inline logical operator!=( SPApar_pos const &p1, SPApar_pos const &p2 )
	{ return !same_par_pos(p1, p2, SPAresnor); }

/** @} */
/**
 * \addtogroup ACISGEOMETRICATOMS
 *
 * @{
 */
/**
 * This class defines a vector (<i>du</i>, <i>dv</i>) in 2D parameter-space.
 * @see SPAparameter
 */
class DECL_BASE SPApar_vec {

	// STIPORT TAC macintosh MWCWG5 needs friend as first token in decl

	// *** Simple arithmetic
	friend DECL_BASE SPApar_vec operator-( SPApar_vec const & );
	friend DECL_BASE SPApar_vec operator+( SPApar_vec const &, SPApar_vec const & );
	friend DECL_BASE SPApar_vec operator-( SPApar_vec const &, SPApar_vec const & );
	friend DECL_BASE SPApar_vec operator*( SPApar_vec const &, double );
	// Multiplication by scalar
	friend DECL_BASE SPApar_vec operator*( double d, SPApar_vec const & v);
	// Division by scalar
	friend DECL_BASE SPApar_vec operator/( SPApar_vec const &, double );
	// Dot product
	friend DECL_BASE double operator%( SPApar_vec const &, SPApar_vec const & );
	// Cross product
	friend DECL_BASE double operator*( SPApar_vec const &, SPApar_vec const & );

public:

	/**
	 * The vector in the <i>u</i>-direction.
	 */
	SPAparameter du;

	/**
	 * The vector in the <i>v</i>-direction.
	 */
	SPAparameter dv;

	// Allow unitialised SPApar_vec.
	/**
	 * C++ allocation constructor requests memory for this object but does not populate it.
	 */
	SPApar_vec() {}


	// Construct a SPApar_vec from two doubles.
	/**
	 * C++ initialize constructor requests memory for this object and populates it with the data supplied as arguments.
	 * <br><br>
	 * @param u
	 * u value.
	 * @param v
	 * v value.
	 */
	SPApar_vec( double u, double v ): du( u ), dv( v ) {}


	// Construct a SPApar_vec from an array of two doubles.
	/**
	 * C++ initialize constructor requests memory for this object and populates it with the data supplied as arguments.
	 * <br><br>
	 * @param uv
	 * array of 2 doubles.
	 */
	SPApar_vec( double uv[ 2 ] ): du( uv[ 0 ] ), dv( uv[ 1 ] ) {}


	// Construct a SPApar_vec from a SPApar_dir. This will be defined inline,
	// but later once we have defined SPApar_dir.
	/**
	 * C++ initialize constructor requests memory for this object and populates it with the data supplied as arguments.
	 * <br><br>
	 * @param d
	 * parameter direction.
	 */
	SPApar_vec( SPApar_dir const & d);
	SPApar_vec(const char*gme, SPApar_dir const & d);
	/**
	 * @nodoc
	 */
	MMGR_FREELIST_THIS
#if 0
; // semicolon needed for mkman (doc tool) parsing
#endif

	/**
	 * Adds two parameter vectors.
	 * <br><br>
	 * @param v
	 * parameter vector.
	 */
	SPApar_vec const &operator+=( SPApar_vec const &v );
	SPApar_vec const &gme_operator_add_assign( SPApar_vec const &v );
	/**
	 * Performs a binary minus operation.
	 * <br><br>
	 * @param v
	 * parameter vector.
	 */
	SPApar_vec const &operator-=( SPApar_vec const &v );
	SPApar_vec const &gme_operator_sub_assign( SPApar_vec const &v );
	/**
	 * Multiplies a parameter by a scalar.
	 * <br><br>
	 * @param d
	 * double.
	 */
	SPApar_vec const &operator*=( double d);
	SPApar_vec const &gme_operator_multiply_assign( double d);
	/**
	 * Divides a parameter vector by a scalar value.
	 * <br><br>
	 * @param d
	 * scalar value.
	 */
	SPApar_vec const &operator/=( double d);
	SPApar_vec const &gme_operator_divide_assign( double d);
	/**
	 * Returns the length of the <tt>SPApar_vec</tt>.
	 */
	double len() const;
	double gme_len() const;
	/**
	 * Outputs details of a parameter vector to the debug file or to the specified file.
	 * <br><br>
	 * @param fp
	 * file name.
	 */
	void debug( FILE *fp = debug_file_ptr ) const;
	void gme_debug( FILE *fp = debug_file_ptr ) const;
	/**
	 * Returns (du * du + dv * dv).
	 */
	double len_sq() const;
	double gme_len_sq() const;
	/**
	 * Returns <tt>TRUE</tt> if a radius function is zero everywhere, to within a given tolerance; otherwise, it returns <tt>FALSE</tt>.
	 * <br><br>
	 * @param tol
	 * zero tolerance.
	 */
	logical is_zero( const double tol = SPAresabs ) const;
	logical gme_is_zero( const double tol = SPAresabs ) const;
};

/** @} */
/**
 * \addtogroup ACISGEOMOPERATORS
 *
 * @{
 */
/**
 * Tests two parameter vectors for equality within a tolerance of <tt>SPAresnor</tt>.
 * <br><br>
 * @param v1
 * first parameter vector.
 * @param v2
 * second parameter vector.
 */
DECL_BASE logical operator==( SPApar_vec const &v1, SPApar_vec const &v2 );
DECL_BASE logical gme_operator_equal( SPApar_vec const &v1, SPApar_vec const &v2 );
/**
 * Tests two parameter vectors for inequality within a tolerance of <tt>SPAresnor</tt>.
 * <br><br>
 * @param v1
 * first parameter vector.
 * @param v2
 * second parameter vector.
 */
DECL_BASE inline logical operator!=( SPApar_vec const &v1, SPApar_vec const &v2 )
	{ return !(v1==v2); }

/** @} */
/**
 * \addtogroup ACISGEOMETRICATOMS
 *
 * @{
 */
/**
 * This class defines a parametric direction vector on a surface in 2D parameter-space.
 */
class DECL_BASE SPApar_dir: public SPApar_vec {

	// Simple arithmetic mostly uses SPApar_vec operators, but negation
	// returns a SPApar_dir, so we redefine it here.

	// STIPORT TAC macintosh MWCWG5 needs friend as first token in decl
	friend DECL_BASE SPApar_dir operator-( SPApar_dir const & );

	// Scalar product with SPAposition. We define this explicitly to
	// avoid an ambiguity elsewhere in the system.
	friend DECL_BASE double operator%( SPApar_pos const &p, SPApar_dir const &u );
	friend DECL_BASE double operator%( SPApar_dir const &u, SPApar_pos const &p );

public:

	// Allow unitialised SPApar_dir.
	/**
	 * Constructs an un-initialized <tt>SPApar_dir</tt>.
	 */
	SPApar_dir() {}

	/**
	 * Constructs and normalizes a <tt>SPApar_dir</tt> from two doubles.
	 * <br><br>
	 * @param uval
	 * u vector.
	 * @param vval
	 * v vector.
	 */
	SPApar_dir( double uval, double vval);
	SPApar_dir(const char*gme, double uval, double vval);
	/**
	 * Constructs and normalizes a <tt>SPApar_dir</tt> from an array of two doubles.
	 * <br><br>
	 * @param uv
	 * array of 2 vectors.
	 */
	SPApar_dir( double uv[ 2 ]  );
	SPApar_dir(const char* gme, double uv[ 2 ]  );
	// Construct and normalise a SPApar_dir from a SPApar_vec. Note that
	// this permits automatic type conversion, which could be
	// dangerous, but people using SPAparameter-space arithmetic should
	// know what they are doing.
	/**
	 * Constructs and normalizes a <tt>SPApar_dir</tt> from a <tt>SPApar_vec</tt>.
	 * <br><br>
	 * @param v
	 * parameter vector direction.
	 */
	SPApar_dir( SPApar_vec const & v);
	SPApar_dir(const char* gme, SPApar_vec const & v);
};
/** @} */
/**
 * \addtogroup ACISGEOMOPERATORS
 *
 * @{
 */

// SPApar_dir friend functions that we feel should be inlined.
/**
 * @nodoc
 */
inline double operator%( SPApar_pos const &p, SPApar_dir const &u )
	{ return p % *(SPApar_vec const *)&u; }
/**
 * @nodoc
 */
inline double operator%( SPApar_dir const &u, SPApar_pos const &p )
	{ return p % *(SPApar_vec const *)&u; }

/** @} */
/**
 * \addtogroup ACISGEOMETRICATOMS
 *
 * @{
 */

// Define a bounding SPAbox in SPAparameter space.
/**
 * The SPApar_box class defines a 2D bounding box in parameter space by four values of class parameter: <tt>low_u</tt>, <tt>high_u</tt>, <tt>low_v</tt>, <tt>high_v</tt>.
 * @see SPAinterval
 */
class DECL_BASE SPApar_box {

	// extent of SPAbox in u SPAparameter direction
	SPAinterval ub;

	// extent of SPAbox in v SPAparameter direction
	SPAinterval vb;

	// Combine two boxes, i.e. a SPAbox that encloses the two given boxes.
	friend DECL_BASE SPApar_box operator|( SPApar_box const &, SPApar_box const & );

	// Find the overlap of two boxes, i.e. the intersection
	friend DECL_BASE SPApar_box operator&( SPApar_box const &, SPApar_box const & );

	// Determine whether two boxes overlap.
	// Returns true if either SPAbox is null, or if all the intervals
	// of one SPAbox overlap the corresponding intervals of the other.
	friend DECL_BASE logical operator&&( SPApar_box const &, SPApar_box const & );
	friend DECL_BASE logical gme_operator_and_and( SPApar_box const &, SPApar_box const & );
	friend DECL_BASE logical operator<<( SPApar_pos const &p, SPApar_box const &b );

	// Translate a SPApar_box.
	friend DECL_BASE SPApar_box operator+( SPApar_box const &, SPApar_vec const & );
	friend DECL_BASE SPApar_box operator+( SPApar_vec const &, SPApar_box const & );

	friend DECL_BASE SPApar_box operator-( SPApar_box const &, SPApar_vec const & );

	// Find the extent of a SPApar_box along a given direction.
	friend DECL_BASE SPAinterval operator%( SPApar_dir const &, SPApar_box const & );
	friend DECL_BASE SPAinterval gme_operator_percent( SPApar_dir const &, SPApar_box const & );
	friend DECL_BASE SPAinterval operator%( SPApar_box const &b, SPApar_dir const &d );
	friend DECL_BASE SPAinterval gme_operator_percent( SPApar_box const &b, SPApar_dir const &d );

public:
	// Construct an empty SPApar_box
	/**
	 * C++ allocation constructor requests memory for this object but does not populate it.
	 */
	SPApar_box();
	SPApar_box(const char*);

	// Simple memberwise constructor. Either SPAinterval may be empty,
	// making the SPApar_box empty.
	/**
	 * C++ initialize constructor requests memory for this object and populates it with the data supplied as arguments.
	 * <br><br>
	 * <b>Role:</b> Creates a <tt>SPApar_box</tt> from the <i>u</i> interval and the <i>v</i> interval. If
	 * either interval is empty, the <tt>SPApar_box</tt> is empty.
	 * <br><br>
	 * @param u
	 * u interval.
	 * @param v
	 * v interval.
	 */
	SPApar_box( SPAinterval const &u, SPAinterval const &v );
	SPApar_box(const char*, SPAinterval const &u, SPAinterval const &v );

	// Construct a SPApar_box at a SPApar_pos.
	/**
	 * C++ initialize constructor requests memory for this object and populates it with the data supplied as arguments.
	 * <br><br>
	 * <b>Role:</b> Creates a parameter box from a parameter position.
	 * <br><br>
	 * @param pos
	 * parameter position.
	 */
	SPApar_box( SPApar_pos const &pos);
	SPApar_box(const char*, SPApar_pos const &pos);

	// Construct a parameter box to contain two parameter position's.
	/**
	 * C++ initialize constructor requests memory for this object and populates it with the data supplied as arguments.
	 * <br><br>
	 * <b>Role:</b> Constructs a parameter box to contain two parameter positions.
	 * <br><br>
	 * @param pos1
	 * parameter position.
	 * @param pos2
	 * parameter position.
	 */
	SPApar_box( SPApar_pos const &pos1, SPApar_pos const &pos2);
	SPApar_box(const char*, SPApar_pos const &pos1, SPApar_pos const &pos2);

	// Construct a copy of another SPApar_box.
	/**
	 * C++ copy constructor requests memory for this object and populates it with the data from the object supplied as an argument.
	 * <br><br>
	 * @param b
	 * parameter box.
	 */
	SPApar_box( SPApar_box const &b );
	SPApar_box(const char*, SPApar_box const &b );
	/**
	 * @nodoc
	 */
	MMGR_FREELIST_THIS
#if 0
; // semicolon needed for mkman (doc tool) parsing
#endif

	// Extract the constituent data from the SPApar_box, as the intervals
	// in the two directions.
	/**
	 * Extracts the constituent data from the <tt>SPApar_box</tt> as an interval in the <i>u</i> direction.
	 */
	SPAinterval u_range() const { return ub; }
	/**
	 * Extracts the constituent data from the <tt>SPApar_box</tt> as an interval in the <i>v</i> direction.
	 */
	SPAinterval v_range() const { return vb; }


	// Simple tests.
	/**
	 * Tests if the box is empty.
	 */
	logical empty() const;
	logical gme_empty() const;
	/**
	 * Returns <tt>TRUE</tt> if any of the coordinate ranges is infinite or <tt>FALSE</tt> otherwise.
	 */
	logical infinite() const;
	logical gme_infinite() const;
	/**
	 * Returns <tt>TRUE</tt> if the coordinate ranges are finite below or <tt>FALSE</tt> otherwise.
	 */
	logical finite_below() const;
	logical gme_finite_below() const;
	/**
	 * Returns <tt>TRUE</tt> if the coordinate ranges are finite above or <tt>FALSE</tt> otherwise.
	 */
	logical finite_above() const;
	logical gme_finite_above() const;
	/**
	 * Returns <tt>TRUE</tt> if the coordinate ranges are finite or <tt>FALSE</tt> otherwise.
	 */
    logical finite() const { return !infinite(); }
	/**
	 * Returns <tt>TRUE</tt> if the coordinate ranges are finite or <tt>FALSE</tt> otherwise.
	 */
	logical bounded() const { return finite(); }
	/**
	 * Returns <tt>TRUE</tt> if the coordinate ranges are either finite or finite below or <tt>FALSE</tt> otherwise.
	 */
	logical bounded_below() const { return finite() || finite_below(); }
	/**
	 * Returns <tt>TRUE</tt> if the coordinate ranges are either finite or finite above or <tt>FALSE</tt> otherwise.
	 */
	logical bounded_above() const { return finite() || finite_above(); }
	/**
	 * Returns <tt>TRUE</tt> if the coordinate ranges are infinite or <tt>FALSE</tt> otherwise.
	 */
	logical unbounded() const { return !bounded(); }
	/**
	 * Returns <tt>TRUE</tt> if the coordinate ranges are either infinite or infinite below or <tt>FALSE</tt> otherwise.
	 */
	logical unbounded_below() const { return infinite() || finite_above(); }
	/**
	 * Returns <tt>TRUE</tt> if the coordinate ranges are either infinite or infinite above or <tt>FALSE</tt> otherwise.
	 */
	logical unbounded_above() const { return infinite() || finite_below(); }

    // Extract the constituent data from the SPApar_box, as the ends
	// and middle of the leading diagonal.
	/**
	 * Extracts the low end of the leading diagonal from the parameter box.
	 */
	SPApar_pos low() const;
	SPApar_pos gme_low() const;
	/**
	 * Extracts the high end of the leading diagonal from the parameter box.
	 */
	SPApar_pos high() const;
	SPApar_pos gme_high() const;
	/**
	 * Extracts the middle of the leading diagonal from the parameter box.
	 */
	SPApar_pos mid() const;
	SPApar_pos gme_mid() const;

	/**
	 * Compounds one parameter box into another.
	 * <br><br>
	 * <b>Role:</b> This method extends this box until it also
	 * encloses the given parameter box.
	 * <br><br>
	 * @param b
	 * given box.
	 */
	SPApar_box &operator|=( SPApar_box const &b );
	SPApar_box &gme_operator_or_assign( SPApar_box const &b );

	/**
	 * Limits one parameter box by another.
	 * <br><br>
	 * <b>Role:</b> Forms the intersection of this parameter box with the given
	 * parameter box, which results in this parameter box.
	 * <br><br>
	 * @param b
	 * given box.
	 */
	SPApar_box &operator&=( SPApar_box const &b );
	SPApar_box &gme_operator_and_assign( SPApar_box const &b );

	// Determine parametric point containment, i.e. returns true if
	// point is contained within this SPAbox (or if this SPAbox is null).
	// The point counts as within if it is strictly within the SPAbox or
	// within the SPAbox enlarged by SPAresabs in all four directions
	// (+u, -u, +v, -v). Note that the use of SPAresabs is rather
	// questionable, but normally will be over-large, which is safe.
	/**
	 * Determines the parametric point containment.
	 * <br><br>
	 * <b>Role:</b> This method returns <tt>TRUE</tt> if the point is contained within
	 * this parameter box or if this parameter box is <tt>NULL</tt>. The point counts as within if it is strictly
	 * within the parameter box or within the box enlarged by <tt>SPAresabs</tt> in all four directions
	 * (+<i>u</i>, -<i>u</i>, +<i>v</i>, -<i>v</i>).
	 * <br><br>
	 * @param pos
	 * parameter position.
	 */
	logical operator>>( SPApar_pos const &pos ) const;
	logical gme_operator_right_shift( SPApar_pos const &pos ) const;

	// Determine if this SPAbox entirely encloses given SPAbox.
	// Returns true if this SPAbox is null, false if given SPAbox is null,
	// and true if given SPAbox is strictly within this SPAbox or
	// within this SPAbox enlarged by SPAresabs in all four directions
	// (+u, -u, +v, -v).
	/**
	 * Determines if this box entirely encloses the given box.
	 * <br><br>
	 * <b>Role:</b> This method returns <tt>TRUE</tt> if this box is <tt>NULL</tt> or if the given
	 * box is strictly within this box or within this box enlarged by <tt>SPAresabs</tt>
	 * in all six directions (+<i>x</i>, -<i>x</i>, +<i>y</i>, -<i>y</i>, +<i>z</i>, -<i>z</i>). Otherwise, or if the given
	 * box is <tt>NULL</tt>, this method returns <tt>FALSE</tt>.
	 * <br><br>
	 * @param b
	 * given box.
	 */
	logical operator>>( SPApar_box const &b ) const;
	logical gme_operator_right_shift( SPApar_box const &b ) const;
	/**
	 * Determines if the given parameter box encloses this parameter box.
	 * <br><br>
	 * @param b
	 * given parameter box.
	 */
	logical operator<<( SPApar_box const &b ) const
		{ return b >> *this; }

	/**
	 * Translates a parameter box by a parameter vector.
	 * <br><br>
	 * @param v
	 * parameter vector.
	 */
	SPApar_box &operator+=( SPApar_vec const &v );
	SPApar_box &gme_operator_add_assign( SPApar_vec const &v );
	/**
	 * Translates a parameter box by a parameter vector.
	 * @param v
	 * parameter vector
	 */
	SPApar_box &operator-=( SPApar_vec const & v);
	SPApar_box &gme_operator_sub_assign( SPApar_vec const & v);
	/**
	 * Prints a titles and debug information about this parameter box to the
	 * debug file or to the specified file.
	 * @param title
	 * title
	 * @param fp
	 * file pointer
	 */

	 /**
	 * Equality operator for determining whether this box is identical to given box.
	 * @param b
	 * given box
	 */
	logical operator==(SPApar_box const & b);
	logical gme_same_par_box(SPApar_box const & b);
	/**
	* Inequality operator for determining whether this box isn't identical to given box.
	* @param b
	* given box
	*/
	logical operator!=(SPApar_box const & b);
	logical gme_operator_no_equal(SPApar_box const & b);
	void debug( char const *title, FILE *fp = debug_file_ptr ) const;
	void gme_debug( char const *title, FILE *fp = debug_file_ptr ) const;
};

/** @} */
/**
 * \addtogroup ACISGEOMOPERATORS
 *
 * @{
 */
// SPApar_box friend functions that we feel should be inlined.
/**
 * @nodoc
 */
inline logical operator<<( SPApar_pos const &p, SPApar_box const &b )
	{ return b >> p; }


// Define SPApar_vec constructor, deferred from the class definition
// because of the forward reference to SPApar_dir.

/**
 * @nodoc
 * C++ initialize constructor requests memory for this object and populates it with the data supplied as arguments.
 * <br><br>
 * @param pdir
 * parameter direction.
 */
inline DECL_BASE SPApar_vec::SPApar_vec( SPApar_dir const &pdir )
	{ du = pdir.du; dv = pdir.dv; }


// Compose the transform.

DECL_BASE SPApar_transf operator*(SPApar_transf const &t1,SPApar_transf const &t2);
DECL_BASE SPApar_transf gme_operator_multiply(SPApar_transf const &t1,SPApar_transf const &t2);

/** @} */
/**
 * \addtogroup ACISGEOMETRICATOMS
 *
 * @{
 */
// Define a SPAparameter space transformation containing
// rotation, scaling and translation components.  The SPApar_transf
// allows the manipulation of a SPApar_pos as follows:
//
//    par_pos_new = par_pos_old * SPApar_transf
//
// where:
//
//    u_new = u_old * _r00 + v_old * _r01 + du
//    v_new = u_old * _r10 + v_old * _r11 + dv
//	
//	here _r00, _r01, _r10, _r11 is the 2D matrix which contain scaling and roation component both.
//
/**
 * Defines a parameter space transformation containing rotation, scaling and translation components.
 * <br>
 * <b>Role:</b> This class defines a parameter space transformation containing
 * rotation, scaling and translation components. The <tt>SPAparameter</tt>, <tt>SPApar_pos</tt>, <tt>SPApar_vec</tt>,
 * <tt>SPApar_dir</tt>, <tt>SPApar_box</tt> and <tt>SPApar_transf</tt> classes define a parameter value along
 * a curve, a pair of parameter values for a parameter point on a surface, a
 * parametric direction on a surface, and a 2d box in parameter space.
 * <br><br>
 * The <tt>SPApar_transf</tt> allows the manipulation of a <tt>SPApar_pos</tt> as follows:
 * <br><br>
 * <tt>SPApar_pos_new = SPApar_pos_old * SPApar_transf</tt>
 * <br>
 * where:
 * <br>
 * &nbsp;&nbsp;&nbsp;&nbsp;<tt>u_new = u_old * _r00 + v_old * _r01 + du</tt>
 * <br>
 * &nbsp;&nbsp;&nbsp;&nbsp;<tt>v_new = u_old * _r10 + v_old * _r11 + dv</tt>
 * <br><br>
 * here _r00, _r01, _r10, _r11 are the component of 2x2 matrix which contain rotation and scaling components.
 * du and dv are the translation components.
 * <br><br>
 * &nbsp;&nbsp;&nbsp;&nbsp;<tt>_r00 = u_scale * cos (rot_angle)</tt>
 * <br> 
 * &nbsp;&nbsp;&nbsp;&nbsp;<tt>_r01 = u_scale * sin (rot_angle)</tt>
 * <br>
 * &nbsp;&nbsp;&nbsp;&nbsp;<tt>_r10 = - v_scale * sin (rot_angle)</tt>
 * <br>
 * &nbsp;&nbsp;&nbsp;&nbsp;<tt>_r11 = v_scale * cos (rot_angle)</tt>
 *
 */
class DECL_BASE SPApar_transf {

	logical identity_flag;
	double _r00, _r01, _r10, _r11;
	double du;
	double dv;

	// Transform a SPApar_pos.
	friend DECL_BASE SPApar_pos operator*( SPApar_pos const &, SPApar_transf const & );
	friend SPApar_pos const &SPApar_pos::operator*=( SPApar_transf const & );
	friend SPApar_pos const &SPApar_pos::gme_operator_multiply_assign( SPApar_transf const & );
	// Compose the transform.
	friend DECL_BASE SPApar_transf operator*(SPApar_transf const &t1,SPApar_transf const &t2);

public:

	/**
	 * Creates an identity parameter transform.
	 * <br><br>
	 * <b>Role:</b> <i>u</i> and <i>v</i> scaling are exactly 1.0; <i>u</i> and <i>v</i>
	 * translation are exactly 0.0; rotation is exactly 0.0 degree.
	 */
	SPApar_transf() : identity_flag(1), _r00(1.0), _r01(0.0), _r10(0.0), _r11(1.0), du(0.0), dv(0.0) {}

	/**
	 * C++ initialize constructor requests memory for this object and populates it with the data supplied as arguments.
	 * <br><br>
	 * @param u_scale
	 * u scaling.
	 * @param v_scale
	 * v scaling.
	 * @param u
	 * u translation.
	 * @param v
	 * v translation.
	 */
	SPApar_transf(double u_scale, double v_scale, double u, double v) : _r00(u_scale), _r01(0.0), _r10(0.0), _r11(v_scale), du(u), dv(v)
		{ identity_flag = (u_scale==1.0 && v_scale==1.0 && u==0.0 && v==0) ? 1 : 0; }

	/**
	 * C++ initialize constructor requests memory for this object and populates it with the data supplied as arguments.
	 * <br><br>
	 * @param rot_angle
	 * anti-clockwise rotation angle in radians.
	 * @param u
	 * u translation.
	 * @param v
	 * v translation.
	 */
	explicit SPApar_transf( double rot_angle, double u = 0., double v = 0. );
	explicit SPApar_transf(const char*, double rot_angle, double u = 0., double v = 0. );
	/**
	 *	Performs a unary multiplication operation on a <tt>SPApar_transf</tt>.
	 * <br><br>
	 * @param transf
	 * parameter SPApar_transf.
	 */
	
	SPApar_transf &  operator*=( SPApar_transf const &t1 );
	SPApar_transf &  gme_operator_multiply_assign( SPApar_transf const &t1 );
	/**
	 * @nodoc
	 */
	MMGR_FREELIST_THIS
#if 0
; // semicolon needed for mkman (doc tool) parsing
#endif

	// Member Setting Methods: not intended for general use
	/**
	 * Changes the <i>u</i> scaling. This method is deprecated.
	 * <br><br>
	 * @param us
	 * u scale.
	 */
	void set_u_scale( double us )
		{ _r00 = us; _r01 = 0.; }

	/**
	 * Changes the <i>v</i> scaling. This method is deprecated.
	 * <br><br>
	 * @param vs
	 * v scale.
	 */
	void set_v_scale( double vs )
		{ _r11 = vs; _r10 = 0.; }

	/**
	 * Change the <i>u</i> translation.
	 * <br><br>
	 * @param delta_u
	 * value to set.
	 */
	void set_delta_u( double delta_u )
		{ du = delta_u; }

	/**
	 * Change the <i>v</i> translation.
	 * <br><br>
	 * @param delta_v
	 * value to set.
	 */
	void set_delta_v( double delta_v )
		{ dv = delta_v; }

	// If an application set the values of class members, then
	// the application is responsible for setting the identity flag too!
	/**
	 * Specifies whether or not to turn transform into identity transform.
	 * <br><br>
	 * @param i
	 * use identity.
	 */
	void set_identity( logical i )
		{ identity_flag = i; }

	// Query Methods
	/**
	 * Returns whether or not the given transform is the identity transform.
	 */
	logical identity() const
		{ return identity_flag; }

	/**
	 * @deprecated R2017 1.0.
	 * Returns the value of the <i>u</i> scaling.
	 */
	double u_scale() const;
	double gme_u_scale() const;
	/**
	 * @deprecated R2017 1.0.
	 * Returns the value of the <i>v</i> scaling.
	 */

	double v_scale() const;
	double gme_v_scale() const;
	/**
	 * Returns the translation change in <i>u</i>.
	 */
	double delta_u() const
		{ return du; }
	/**
	 * Returns the translation change in <i>v</i>.
	 */
	double delta_v() const
		{ return dv; }
	/**
	 * Returns the ( 0, 0 ) element of the 2D affine transform.
	 */
	double r00() const
		{ return _r00; }
	/**
	 * Returns the ( 0, 1 ) element of the 2D affine transform.
	 */
	double  r01() const 
		{ return _r01; }
	/**
	 * Returns the ( 1, 0 ) element of the 2D affine transform.
	 */
	double r10() const 
		{ return _r10; }
	/**
	 * Returns the ( 1, 1 ) element of the 2D affine transform.
	 */
	double r11() const 
		{ return _r11; }

	/**
	 * Outputs to the specified debug file information about this class.
	 * <br><br>
	 * @param fp
	 * file pointer.
	 */
	void debug( FILE *fp = debug_file_ptr ) const;
	void gme_debug( FILE *fp = debug_file_ptr ) const;
};

// STI let: end



/** @} */
#endif

