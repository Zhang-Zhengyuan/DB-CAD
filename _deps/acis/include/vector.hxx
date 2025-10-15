/*******************************************************************/
/*    Copyright (c) 1989-2020 by Spatial Corp.                     */
/*    All rights reserved.                                         */
/*    Protected by U.S. Patents 5,257,205; 5,351,196; 6,369,815;   */
/*                              5,982,378; 6,462,738; 6,941,251    */
/*    Protected by European Patents 0503642; 69220263.3            */
/*    Protected by Hong Kong Patent 1008101A                       */
/*******************************************************************/
// Header for SPAvector.
//
// A SPAvector represents a displacement in three-dimensional
// cartesian space.  It is subject to transformations as well as the
// usual SPAvector operations.
// We define several different types of SPAvector-like entities, and
// will only permit meaningful combinations of them.
#if !defined( VECTOR_CLASS )
#define VECTOR_CLASS
#include "base.hxx"
#include "dcl_base.h"
#include "logical.h"
#include "debugmsc.hxx"
#include "position.hxx"
#include <math.h>
/**
 * @file vector.hxx
 * @CAA2Level L1
 * @CAA2Usage U1
 * \addtogroup ACISGEOMETRICATOMS
 *
 * @{
 */
class SPAvector;
class SPAunit_vector;
class SPAmatrix;
class SPAtransf;
/**
 * Represents a displacement vector in 3D Cartesian space.
 * @see SPAtransf
 */
class DECL_BASE SPAvector 
{
	double comp[ 3 ];	// the x, y and z components of the SPAvector

	friend DECL_BASE SPAvector operator-( SPAvector const &nv );
	friend DECL_BASE SPAvector operator+( SPAvector const &v1, SPAvector const &v2 );
	friend DECL_BASE SPAvector operator-( SPAvector const &v1, SPAvector const &v2 );
	friend double operator%( SPAvector const &v1, SPAvector const &v2 );
	friend double operator%( SPAposition const &p, SPAvector const &v );
	friend double operator%( SPAvector const &v, SPAposition const &p );
	friend double operator%( SPAposition const &p, SPAunit_vector const &uv );
	friend double operator%( SPAunit_vector const &uv, SPAposition const &p );
	friend DECL_BASE SPAvector operator*( SPAvector const &v1, SPAvector const &v2 );
	friend DECL_BASE SPAvector gme_operator_multiply( SPAvector const &v1, SPAvector const &v2 );
	friend DECL_BASE SPAvector operator*( double d, SPAvector const &v );
	friend DECL_BASE SPAvector operator*( SPAvector const &v, double d );
	friend DECL_BASE SPAvector operator/( SPAvector const &v, double d );
	friend DECL_BASE SPAvector gme_operator_divide( SPAvector const &v, double d );
 	friend DECL_BASE SPAvector operator*( SPAmatrix const &m, SPAvector const &v );
	friend DECL_BASE SPAvector gme_operator_multiply( SPAmatrix const &m, SPAvector const &v );
	friend DECL_BASE SPAvector operator*( SPAvector const &v, SPAmatrix const &m );
	friend DECL_BASE SPAvector gme_operator_multiply( SPAvector const &v, SPAmatrix const &m );
	friend DECL_BASE SPAvector operator*( SPAvector const &v, SPAtransf const &t );
	friend DECL_BASE SPAvector operator*( SPAvector const &v, SPAtransf const *t );
	friend DECL_BASE SPAunit_vector normalise( SPAvector const &v );
	friend DECL_BASE SPAunit_vector gme_normalise( SPAvector const &v );
	friend DECL_BASE logical same_vector( SPAvector const&v1, SPAvector const&v2, const double res);
	friend DECL_BASE logical gme_same_vector( SPAvector const&v1, SPAvector const&v2, const double res);
	friend DECL_BASE logical parallel( SPAvector const &v1, SPAvector const &v2, const double res );
	friend DECL_BASE logical gme_parallel(SPAvector const& v1, SPAvector const& v2, const double res);
	friend DECL_BASE logical parallel( SPAunit_vector const &v1, SPAvector const &v2, const double res );
	friend DECL_BASE logical gme_parallel(SPAunit_vector const& v1, SPAvector const& v2, const double res);
	friend DECL_BASE logical parallel( SPAunit_vector const &v1, SPAunit_vector const &v2, const double res );
	friend DECL_BASE logical gme_parallel(SPAunit_vector const& v1, SPAunit_vector const& v2, const double res);
	friend DECL_BASE logical antiparallel( SPAvector const &v1, SPAvector const &v2, const double res );
	friend DECL_BASE logical gme_antiparallel(SPAvector const& v1, SPAvector const& v2, const double res);
	friend DECL_BASE logical antiparallel( SPAunit_vector const &v1, SPAvector const &v2, const double res );
	friend DECL_BASE logical gme_antiparallel(SPAunit_vector const& v1, SPAvector const& v2, const double res);
	friend DECL_BASE logical antiparallel( SPAunit_vector const &v1, SPAunit_vector const &v2, const double res );
	friend DECL_BASE logical gme_antiparallel(SPAunit_vector const& v1, SPAunit_vector const& v2, const double res);
	friend DECL_BASE logical biparallel( SPAvector const &v1, SPAvector const &v2, const double res );
	friend DECL_BASE logical gme_biparallel(SPAvector const& v1, SPAvector const& v2, const double res);
	friend DECL_BASE logical biparallel( SPAunit_vector const &v1, SPAvector const &v2, const double res );
	friend DECL_BASE logical gme_biparallel(SPAunit_vector const& v1, SPAvector const& v2, const double res);
	friend DECL_BASE logical biparallel( SPAunit_vector const &v1, SPAunit_vector const &v2, const double res );
	friend DECL_BASE logical gme_biparallel(SPAunit_vector const& v1, SPAunit_vector const& v2, const double res);
	friend DECL_BASE logical perpendicular( SPAvector const &v1, SPAvector const &v2, const double res );
	friend DECL_BASE logical gme_perpendicular(SPAvector const& v1, SPAvector const& v2, const double res);
	friend DECL_BASE logical perpendicular( SPAunit_vector const &v1, SPAvector const &v2, const double res );
	friend DECL_BASE logical gme_perpendicular(SPAunit_vector const& v1, SPAvector const& v2, const double res);
	friend DECL_BASE logical perpendicular( SPAunit_vector const &v1, SPAunit_vector const &v2, const double res );
	friend DECL_BASE logical gme_perpendicular(SPAunit_vector const& v1, SPAunit_vector const& v2, const double res);

public:
/**
 * @nodoc
 */ 
	MMGR_FREELIST_THIS
#if 0
; // semicolon needed for mkman (doc tool) parsing
#endif

	/**
	 * C++ allocation constructor requests memory for this object but does not populate it.
	 */
	SPAvector() 
#if 0
		; // for mkman
#endif
#ifdef INTERNAL_DEBUG_CHECKS
		;
#else
		{}
#endif

	// Construct a SPAvector from three doubles.
	/**
	 * C++ initialize constructor requests memory for this object and populates it with the data supplied as arguments.
	 * <br><br>
	 * <b>Role:</b> Creates a <tt>SPAvector</tt> from three doubles representing the x, y, and z coordinate values.
	 * <br><br>
	 * @param x
	 * x-coordinate.
	 * @param y
	 * y-coordinate.
	 * @param z
	 * z-coordinate.
	 */
	SPAvector( double x, double y, double z ) {
		comp[ 0 ] = x;
		comp[ 1 ] = y;
		comp[ 2 ] = z;
	}

	// Construct a SPAvector from an array of three doubles.
	/**
	 * C++ initialize constructor requests memory for this object and populates it with the data supplied as arguments.
	 * <br><br>
	 * @param v
	 * array of 3 doubles.
	 */
	SPAvector( double v[ 3 ] ) {
		comp[ 0 ] = v[ 0 ];
		comp[ 1 ] = v[ 1 ];
		comp[ 2 ] = v[ 2 ];
	}

	// Copy a SPAvector.
	/**
	 * C++ copy constructor requests memory for this object and populates it with the data from the object supplied as an argument.
	 * <br><br>
	 * @param v
	 * vector.
	 */
	SPAvector( SPAvector const &v ) {
		comp[ 0 ] = v.comp[ 0 ];
		comp[ 1 ] = v.comp[ 1 ];
		comp[ 2 ] = v.comp[ 2 ];
	}

// MS IA64 Compiler bug
#if (_MSC_VER == 1300) && defined(_WIN64)
	/**
	 * @nodoc
	 */ 
	~SPAvector() {}
#endif

	// Returns the components of a SPAvector.
	/**
	 * Returns the x-component of a vector.
	 */
	inline double x() const
		{ return comp[ 0 ]; }
	/**
	 * Returns the y-component of a vector.
	 */
	inline double y() const
		{ return comp[ 1 ]; }
	/**
	 * Extracts the z-component of a vector.
	 */
	inline double z() const
		{ return comp[ 2 ]; }

	/**
	 * Returns a copy of the ith component of a vector.
	 * <br><br>
	 * @param i
	 * ith component.
	 */
	inline double component( int i ) const
		{ return comp[ i ]; }


	// Extract the components of a SPAvector for modification
	/**
	 * Extracts the x-component of a vector for an update and allows it to be modified.
	 */
	inline double &x()
		{ return comp[ 0 ]; }
	/**
	 * Extracts the y-component of a vector for an update and allows it to be modified.
	 */
	inline double &y()
		{ return comp[ 1 ]; }
	/**
	 * Extracts the z-component of a vector for an update and allows it to be modified.
	 */
	inline double &z()
		{ return comp[ 2 ]; }

	/**
	 * Extracts the ith component of a vector and allows it to be modified.
	 * <br><br>
	 * @param i
	 * ith component.
	 */
	inline double &component( int i )
		{ return comp[ i ]; }


	// Set component values.
	/**
	 * Sets the x-coordinate of a vector.
	 * <br><br>
	 * @param new_x
	 * x-coordinate.
	 */
	inline void set_x( double new_x ) { comp[ 0 ] = new_x; }
	/**
	 * Sets the y-coordinate of a vector.
	 * <br><br>
	 * @param new_y
	 * y-coordinate.
	 */
	inline void set_y( double new_y ) { comp[ 1 ] = new_y; }
	/**
	 * Sets the z-coordinate of a vector.
	 * <br><br>
	 * @param new_z
	 * z-coordinate.
	 */
	inline void set_z( double new_z ) { comp[ 2 ] = new_z; }
	/**
	 * Sets the value of ith component of a vector.
	 * <br><br>
	 * @param i
	 * ith component.
	 * @param new_c
	 * new value.
	 */
	inline void set_component( int i, double new_c )
		{ comp[ i ] = new_c; }

	/**
	 * Adds two vectors.
	 * <br><br>
	 * @param v
	 * vector.
	 */
	inline SPAvector const &operator+=( SPAvector const &v )
	{
		 comp[ 0 ] += v.comp[ 0 ];
		 comp[ 1 ] += v.comp[ 1 ];
		 comp[ 2 ] += v.comp[ 2 ];

		 return *this;
	};


	/**
	 * Binary minus operation.
	 * <br><br>
	 * @param v
	 * vector.
	 */
	inline SPAvector const &operator-=( SPAvector const & v)
	{
		 comp[ 0 ] -= v.comp[ 0 ];
		 comp[ 1 ] -= v.comp[ 1 ];
		 comp[ 2 ] -= v.comp[ 2 ];

		 return *this;
	};

	/**
	 * Multiplication of a vector by a scalar.
	 * <br><br>
	 * @param d
	 * scalar value.
	 */
	inline SPAvector const &operator*=( double d)
	{
		 comp[ 0 ] *= d;
		 comp[ 1 ] *= d;
		 comp[ 2 ] *= d;

		 return *this;
	};

	/**
	 * Division of a vector by a scalar.
	 * <br><br>
	 * @param d
	 * scalar value.
	 */
	SPAvector const &operator/=( double d);
	SPAvector const &gme_operator_divide_assign( double d);

	// Length of a SPAvector.
	/**
	 * Returns the length squared, ( x*x + y*y + z*z ). Faster to compute 
	 * than calling the <tt>len</tt> method.
	 */
	inline double len_sq() const
	{
			double dp;
			dp = comp[0] * comp[0], dp += comp[1] * comp[1], dp += comp[2] * comp[2];
			return dp;
	}

	/**
	 * Returns the <i>length</i> of the vector.
	 */
	inline double len() const
	{
			double dp;
			dp = comp[0] * comp[0], dp += comp[1] * comp[1], dp += comp[2] * comp[2];
			return ( sqrt(	dp ) );
	}

	/**
	 * Not appropriate for general use.
	 * <br><br>
	 * <b>Role:</b> This method is more expensive than <tt>len</tt>, but (theoretically)
	 * gives the same value and is stable for very small (those for which <tt>v\%v</tt> would
	 * be lost in numerical noise) or very large norms (those for which <tt>v\%v</tt> would
	 * give overflow).
	 */
	double numerically_stable_len() const;
	double gme_numerically_stable_len() const;
	// this gets the max of the fabs of each component, and tells (via
	// "int& i") which component was the max. Note that in case of a "tie"
	// the index "i" will default to the larger index. That is for the
	// SPAvector (1,1,1) "i" will be "2". Note that the two norms "max_norm(i)"
	// and "len()" define equivalent topologies on R3 due to the inequality:
	// v.len() <= sqrt(3) * v.max_norm() <= sqrt(3) * v.len()
	/**
	 * This method returns the maximum of the fabs of each component, and tells which component was the maximum.
	 * <br><br>
	 * <b>Role:</b> In case of a "tie" the index, <tt>i</tt>, will default to the larger index.
	 * For example, given the vector (1, 1, 1), i = 2.
	 * <br><br>
	 * @param i
	 * index of max component.
	 */
	double max_norm( int& i ) const;
	double gme_max_norm( int& i ) const;
	/**
	 * Returns a unit vector that is orthogonal to the given vector.
	 * If the given vector is less than <tt>SPAresmch</tt> in length, this method returns the unit 
	 * vector (0,0,1).
	 */
	SPAunit_vector orthogonal() const;
	SPAunit_vector gme_orthogonal() const;
	/**
	 * Transforms this vector by a 3 X 3 matrix.
	 * @param m
	 * matrix
	 */
	inline SPAvector const &operator*=( SPAmatrix const & m)
	{
		// We have to make a copy of the result, and then put it back
		// anyway, so just do it the easy way.

		*this = *this * m;
		return *this;
	}
	/**
	 * Transforms this vector by an affine transformation.
	 * <br><br>
	 * @param t
	 * transform.
	 */
	SPAvector const &operator*=( SPAtransf const &t );
	SPAvector const &gme_operator_multiply_assign( SPAtransf const &t );
	/**
	 * Returns a SPAvector orthogonal to this SPAvector.
	 */
	SPAvector make_ortho();
	SPAvector gme_make_ortho();
	/**
	 * Returns TRUE if the <i>length</i> of the SPAvector is less than the given tolerance.
	 * <br><br>
	 * @param tol
	 * zero tolerance.
	 */
	logical is_zero( const double tol = SPAresabs ) const;
	logical gme_is_zero( const double tol = SPAresabs ) const;
	/**
	 * Outputs debug information to the screen or to the specified file.
	 * <br><br>
	 * @param fp
	 * file pointer.
	 */
	void debug( FILE * fp = debug_file_ptr ) const;
	void gme_debug( FILE * fp = debug_file_ptr ) const;
	/**
	 * Concatenates the debug information to the passed string.
	 * <br><br>
	 * @param str
	 * string.
	 */
	void debug_str(char *str ) const;
	void gme_debug_str(char *str ) const;
};

/** @} */
/**
 * \addtogroup ACISGEOMOPERATORS
 *
 * @{
 */
/**
 * Returns the scalar product (that is, the dot product) of two vectors.
 * <br><br>
 * @param v1
 * first vector.
 * @param v2
 * second vector.
 */
inline double operator%( SPAvector const &v1, SPAvector const &v2 )
{
	double dp;
	dp = v1.comp[0] * v2.comp[0], dp += v1.comp[1] * v2.comp[1], dp += v1.comp[2] * v2.comp[2];
	return	dp;
}

/**
 * Returns the scalar product of a position with a vector.
 * <br><br>
 * @param p
 * position.
 * @param v
 * vector.
 */
inline double operator%( SPAposition const &p, SPAvector const &v )
{
	double dp;
	dp = p.coord[0] * v.comp[0], dp += p.coord[1] * v.comp[1], dp += p.coord[2] * v.comp[2];
	return dp;
}

/**
 * Returns the scalar product of vector with a position.
 * <br><br>
 * @param v
 * vector.
 * @param p
 * position.
 */
inline double operator%( SPAvector const &v, SPAposition const &p )
{
	double dp;
	dp = p.coord[0] * v.comp[0], dp += p.coord[1] * v.comp[1], dp += p.coord[2] * v.comp[2];
	return dp;
}

/**
 * Determines if two vectors are within SPAresabs of each other.
 * <br><br>
 * @param v1
 * first vector
 * @param v2
 * second vector
 */
inline logical operator==( SPAvector const &v1, SPAvector const &v2 )
	{ return same_vector( v1, v2, SPAresabs ); }
inline logical gme_operator_equal( SPAvector const& v1, SPAvector const& v2 )
	{ return gme_same_vector( v1, v2, SPAresabs ); }


/**
 * Determines if two vectors are NOT within SPAresabs of each other.
 * <br><br>
 * @param v1
 * first vector
 * @param v2
 * second vector
 */
inline logical operator!=( SPAvector const &v1, SPAvector const &v2 )
	{ return !same_vector( v1, v2, SPAresabs ); }
inline logical gme_operator_not_equal( SPAvector const& v1, SPAvector const& v2 )
	{ return !gme_same_vector(v1, v2, SPAresabs); }

/**
 * Determines if two vectors are parallel (within some resolution).
 * The default tolerance is <tt>SPAresnor</tt>. 
 * The tolerance is the maximum allowable angle between the two vectors, or more precisely, the sine of the angle. 
 * Please refer to https://en.wikipedia.org/wiki/Small-angle_approximation for a discussion of the use of the sine of an angle for small angles.
 * <br><br>
 * <b>Effect:</b> Read-only
 * <br><br>
 * @param v1
 * first vector.
 * @param v2
 * second vector.
 * @param res
 * the tolerance (the sine of the angle).
 **/
inline logical parallel( SPAvector const &v1, SPAunit_vector const &v2,
		const double res = SPAresnor )
	{ return parallel( v2, v1, res ); }
inline logical gme_parallel( SPAvector const& v1, SPAunit_vector const& v2,
		const double res = SPAresnor )
	{ return gme_parallel( v2, v1, res ); }

/**
 * Determines if two vectors are antiparallel (within some resolution).
 * "Antiparallel" indicates that the vectors are parallel, but in opposite directions.
 * The default tolerance is <tt>SPAresnor</tt>.
 * The tolerance is the maximum allowable angle between the two vectors, or more precisely, the sine of the angle. 
 * Please refer to https://en.wikipedia.org/wiki/Small-angle_approximation for a discussion of the use of the sine of an angle for small angles.
 * <br><br>
 * <b>Effect:</b> Read-only
 * @param v1
 * first vector.
 * @param v2
 * second vector.
 * @param res
 * the tolerance (the sine of the angle).
 **/
inline logical antiparallel( SPAvector const &v1, SPAunit_vector const &v2,
		const double res = SPAresnor )
	{ return antiparallel( v2, v1, res ); }
inline logical gme_antiparallel( SPAvector const& v1, SPAunit_vector const& v2,
		const double res = SPAresnor )
	{ return gme_antiparallel( v2, v1, res ); }


/**
 * Determines if two vectors are bi-parallel (within some resolution).
 * "Bi-parallel" indicates that the vectors are parallel and may be in either the same or opposite directions.
 * The default tolerance is <tt>SPAresnor</tt>.
 * The tolerance is the maximum allowable angle between the two vectors, or more precisely, the sine of the angle. 
 * Please refer to https://en.wikipedia.org/wiki/Small-angle_approximation for a discussion of the use of the sine of an angle for small angles.
 * <br><br>
 * <b>Effect:</b> Read-only
 * <br><br>
 * @param v1
 * first vector.
 * @param v2
 * second vector.
 * @param res
 * the tolerance (the sine of the angle).
 **/
inline logical biparallel( SPAvector const &v1, SPAunit_vector const &v2,
		const double res = SPAresnor )
	{ return biparallel( v2, v1, res ); }
inline logical gme_biparallel( SPAvector const& v1, SPAunit_vector const& v2,
		const double res = SPAresnor )
	{ return gme_biparallel( v2, v1, res ); }


/**
 * Determines if two vectors are perpendicular (within some resolution).
 * The default tolerance is <tt>SPAresnor</tt>.
 * Two vectors are deemed perpendicular if the cosine of the angle between them is less than the specified angular tolerance.
 * <br><br>
 * <b>Effect:</b> Read-only
 * <br><br>
 * @param v1
 * first vector.
 * @param v2
 * second vector.
 * @param res
 * the tolerance (the cosine of the angle).
 **/
inline logical perpendicular( SPAvector const &v1, SPAunit_vector const &v2,
		const double res = SPAresnor )
	{ return perpendicular( v2, v1, res ); }
inline logical gme_perpendicular( SPAvector const& v1, SPAunit_vector const& v2,
		const double res = SPAresnor )
	{ return gme_perpendicular( v2, v1, res ); }


/**
 * @nodoc
 * Useful constant SPAvector
 */
extern GLOBAL_CONST DECL_BASE SPAvector null_vector;
/**
 * Unary minus operator.
 * <br><br>
 * <b>Role:</b> Returns the negation of the given vector.
 * <br><br>
 * @param nv
 * given vector.
 */
DECL_BASE inline SPAvector operator-( SPAvector const &nv )
{
		 return SPAvector( -nv.comp[ 0 ], -nv.comp[ 1 ], -nv.comp[ 2 ] );
};

/**
 * Addition of two vectors.
 * <br><br>
 * @param v1
 * first vector.
 * @param v2
 * second vector.
 */
DECL_BASE inline SPAvector operator+( SPAvector const &v1, SPAvector const & v2)
{
		 return SPAvector( v1.comp[ 0 ] + v2.comp[ 0 ],
		 		 		 		    v1.comp[ 1 ] + v2.comp[ 1 ],
		 		 		 		    v1.comp[ 2 ] + v2.comp[ 2 ]
		 		 		 		 );
};

/**
 * Binary minus operation.
 * <br><br>
 * @param v1
 * first vector.
 * @param v2
 * second vector.
 */
DECL_BASE inline SPAvector operator-( SPAvector const &v1, SPAvector const &v2 )
{
		 return SPAvector( v1.comp[ 0 ] - v2.comp[ 0 ],
		 		 		 		    v1.comp[ 1 ] - v2.comp[ 1 ],
		 		 		 		    v1.comp[ 2 ] - v2.comp[ 2 ]
		 		 		 		 );
};

/**
 * Cross product of two vectors - Also applies to unit vectors.
 * <br><br>
 * @param v1
 * first vector.
 * @param v2
 * second vector.
 */
DECL_BASE SPAvector operator*( SPAvector const &v1, SPAvector const &v2 );
DECL_BASE SPAvector gme_operator_multiply( SPAvector const &v1, SPAvector const &v2 );
/**
 * Multiplies a vector by a scalar value.
 * <br><br>
 * @param d
 * scalar value.
 * @param v
 * vector.
 */
DECL_BASE inline SPAvector operator*( double d, SPAvector const &v )
{
		 return SPAvector(
		 		 		 		 d * v.comp[ 0 ],
		 		 		 		 d * v.comp[ 1 ],
		 		 		 		 d * v.comp[ 2 ]
		 		 		 );
};

/**
 * Multiplies a vector by a scalar value.
 * <br><br>
 * @param v
 * vector.
 * @param d
 * scalar value.
 */
DECL_BASE inline SPAvector operator*( SPAvector const &v, double d)
{
		 return SPAvector(
		 		 		 		 d * v.comp[ 0 ],
		 		 		 		 d * v.comp[ 1 ],
		 		 		 		 d * v.comp[ 2 ]
		 		 		 );
};

/**
 * Division of a vector by a scalar.
 * <br><br>
 * @param v
 * vector.
 * @param d
 * scalar value.
 */
DECL_BASE SPAvector operator/( SPAvector const &v, double d);
DECL_BASE SPAvector gme_operator_divide( SPAvector const &v, double d);
/**
 * Transforms a vector by a 3 X 3 matrix.
 * <br><br>
 * @param m
 * matrix.
 * @param v
 * vector.
 */
DECL_BASE SPAvector operator*( SPAmatrix const &m, SPAvector const &v );
DECL_BASE SPAvector gme_operator_multiply( SPAmatrix const &m, SPAvector const &v );
/**
 * Transforms a vector by a 3 X 3 matrix.
 * <br><br>
 * @param v
 * vector.
 * @param m
 * matrix.
 */
DECL_BASE SPAvector operator*( SPAvector const &v, SPAmatrix const &m );
DECL_BASE SPAvector gme_operator_multiply( SPAvector const &v, SPAmatrix const &m );
/**
 * Transforms a vector by an affine transformation.
 * <br><br>
 * @param v
 * vector.
 * @param t
 * transformation.
 */
DECL_BASE SPAvector operator*( SPAvector const &v, SPAtransf const &t );
DECL_BASE SPAvector gme_operator_multiply( SPAvector const &v, SPAtransf const &t );
/**
 * Transforms a vector by an affine transformation.
 * <br><br>
 * @param v
 * vector.
 * @param t
 * transformation.
 */
DECL_BASE SPAvector operator*( SPAvector const &v, SPAtransf const *t );
DECL_BASE SPAvector gme_operator_multiply( SPAvector const &v, SPAtransf const *t );
/**
 * Converts a vector into a unit vector.
 * <br><br>
 * <b>Effect:</b> Read-only
 * <br><br>
 * @param v
 * vector.
 **/
DECL_BASE SPAunit_vector normalise( SPAvector const &v );
DECL_BASE SPAunit_vector gme_normalise( SPAvector const &v );
/**
* Determines whether or not two vectors are the same.
* The default tolerance is <tt>SPAresabs</tt>.
* <br><br>
* <b>Effect:</b> System routine
* <br><br>
* @param v1
* first vector.
* @param v2
* second vector.
* @param res
* resolution for comparison.
**/
DECL_BASE logical same_vector( SPAvector const&v1, SPAvector const&v2, const double res = SPAresabs);
DECL_BASE logical gme_same_vector( SPAvector const&v1, SPAvector const&v2, const double res = SPAresabs);
/**
* Determines if two vectors are parallel (within some resolution).
* The default tolerance is <tt>SPAresnor</tt>.
* The tolerance is the maximum allowable angle between the two vectors, or more precisely, the sine of the angle. 
* Please refer to https://en.wikipedia.org/wiki/Small-angle_approximation for a discussion of the use of the sine of an angle for small angles.
* <br><br>
* <b>Effect:</b> Read-only
* <br><br>
* @param v1
* first vector.
* @param v2
* second vector.
* @param res
* the tolerance (the sine of the angle).
**/
DECL_BASE logical parallel( SPAvector const &v1, SPAvector const &v2, const double res = SPAresnor);
DECL_BASE logical gme_parallel( SPAvector const &v1, SPAvector const &v2, const double res = SPAresnor);
/**
* Determines if two vectors are parallel (within some resolution).
* The default tolerance is <tt>SPAresnor</tt>.
* The tolerance is the maximum allowable angle between the two vectors, or more precisely, the sine of the angle. 
* Please refer to https://en.wikipedia.org/wiki/Small-angle_approximation for a discussion of the use of the sine of an angle for small angles.
* <br><br>
* <b>Effect:</b> Read-only
* <br><br>
* @param v1
* first vector.
* @param v2
* second vector.
* @param res
* the tolerance (the sine of the angle).
**/
DECL_BASE logical parallel( SPAunit_vector const &v1, SPAvector const &v2, const double res = SPAresnor);
DECL_BASE logical gme_parallel( SPAunit_vector const &v1, SPAvector const &v2, const double res = SPAresnor);

/**
* Determines if two vectors are parallel (within some resolution).
* The default tolerance is <tt>SPAresnor</tt>.
* The tolerance is the maximum allowable angle between the two vectors, or more precisely, the sine of the angle. 
* Please refer to https://en.wikipedia.org/wiki/Small-angle_approximation for a discussion of the use of the sine of an angle for small angles.
* <br><br>
* <b>Effect:</b> Read-only
* <br><br>
* @param v1
* first vector.
* @param v2
* second vector.
* @param res
* the tolerance (the sine of the angle).
**/
DECL_BASE logical parallel( SPAunit_vector const &v1, SPAunit_vector const &v2, const double res = SPAresnor);
DECL_BASE logical gme_parallel( SPAunit_vector const &v1, SPAunit_vector const &v2, const double res = SPAresnor);
/**
* Determines if two vectors are antiparallel (within some resolution).
* "Antiparallel" indicates that the vectors are parallel, but in opposite directions.
* The default tolerance is <tt>SPAresnor</tt>.
* The tolerance is the maximum allowable angle between the two vectors, or more precisely, the sine of the angle. 
* Please refer to https://en.wikipedia.org/wiki/Small-angle_approximation for a discussion of the use of the sine of an angle for small angles.
* <br><br>
* <b>Effect:</b> Read-only
* <br><br>
* @param v1
* first vector.
* @param v2
* second vector.
* @param res
* the tolerance (the sine of the angle).
**/
DECL_BASE logical antiparallel( SPAvector const &v1, SPAvector const &v2, const double res = SPAresnor);
DECL_BASE logical gme_antiparallel( SPAvector const &v1, SPAvector const &v2, const double res = SPAresnor);
/**
* Determines if two vectors are antiparallel (within some resolution).
* "Antiparallel" indicates that the vectors are parallel, but in opposite directions.
* The default tolerance is <tt>SPAresnor</tt>.
* The tolerance is the maximum allowable angle between the two vectors, or more precisely, the sine of the angle. 
* Please refer to https://en.wikipedia.org/wiki/Small-angle_approximation for a discussion of the use of the sine of an angle for small angles.
* <br><br>
* <b>Effect:</b> Read-only
* <br><br>
* @param v1
* first vector.
* @param v2
* second vector.
* @param res
* the tolerance (the sine of the angle).
**/
DECL_BASE logical antiparallel( SPAunit_vector const &v1, SPAvector const &v2, const double res = SPAresnor);
DECL_BASE logical gme_antiparallel( SPAunit_vector const &v1, SPAvector const &v2, const double res = SPAresnor);
/**
* Determines if two vectors are antiparallel (within some resolution).
* "Antiparallel" indicates that the vectors are parallel, but in opposite directions.
* The default tolerance is <tt>SPAresnor</tt>.
* The tolerance is the maximum allowable angle between the two vectors, or more precisely, the sine of the angle. 
* Please refer to https://en.wikipedia.org/wiki/Small-angle_approximation for a discussion of the use of the sine of an angle for small angles.
* <br><br>
* <b>Effect:</b> Read-only
* <br><br>
* @param v1
* first vector.
* @param v2
* second vector.
* @param res
* the tolerance (the sine of the angle).
**/
DECL_BASE logical antiparallel( SPAunit_vector const &v1, SPAunit_vector const &v2, const double res = SPAresnor);
DECL_BASE logical gme_antiparallel( SPAunit_vector const &v1, SPAunit_vector const &v2, const double res = SPAresnor);
/**
* Determines if two vectors are bi-parallel (within some resolution).
* "Bi-parallel" indicates that the vectors are parallel and may be in either the same or opposite directions.
* The default tolerance is <tt>SPAresnor</tt>.
* The tolerance is the maximum allowable angle between the two vectors, or more precisely, the sine of the angle. 
* Please refer to https://en.wikipedia.org/wiki/Small-angle_approximation for a discussion of the use of the sine of an angle for small angles.
* <br><br>
* <b>Effect:</b> Read-only
* <br><br>
* @param v1
* first vector.
* @param v2
* second vector.
* @param res
* the tolerance (the sine of the angle).
**/
DECL_BASE logical biparallel( SPAvector const &v1, SPAvector const &v2, const double res = SPAresnor);
DECL_BASE logical gme_biparallel( SPAvector const &v1, SPAvector const &v2, const double res = SPAresnor);
/**
* Determines if two vectors are bi-parallel (within some resolution).
* "Bi-parallel" indicates that the vectors are parallel and may be in either the same or opposite directions.
* The default tolerance is <tt>SPAresnor</tt>.
* The tolerance is the maximum allowable angle between the two vectors, or more precisely, the sine of the angle. 
* Please refer to https://en.wikipedia.org/wiki/Small-angle_approximation for a discussion of the use of the sine of an angle for small angles.
* <br><br>
* <b>Effect:</b> Read-only
* <br><br>
* @param v1
* first vector.
* @param v2
* second vector.
* @param res
* the tolerance (the sine of the angle).
**/
DECL_BASE logical biparallel( SPAunit_vector const &v1, SPAvector const &v2, const double res = SPAresnor);
DECL_BASE logical gme_biparallel( SPAunit_vector const &v1, SPAvector const &v2, const double res = SPAresnor);
/**
* Determines if two vectors are bi-parallel (within some resolution).
* "Bi-parallel" indicates that the vectors are parallel and may be in either the same or opposite directions.
* The default tolerance is <tt>SPAresnor</tt>.
* The tolerance is the maximum allowable angle between the two vectors, or more precisely, the sine of the angle. 
* Please refer to https://en.wikipedia.org/wiki/Small-angle_approximation for a discussion of the use of the sine of an angle for small angles.
* <br><br>
* <b>Effect:</b> Read-only
* <br><br>
* @param v1
* first vector.
* @param v2
* second vector.
* @param res
* the tolerance (the sine of the angle).
**/
DECL_BASE logical biparallel( SPAunit_vector const &v1, SPAunit_vector const &v2, const double res = SPAresnor);
DECL_BASE logical gme_biparallel( SPAunit_vector const &v1, SPAunit_vector const &v2, const double res = SPAresnor);
/**
* Determines if two vectors are perpendicular (within some resolution).
* The default tolerance is <tt>SPAresnor</tt>.
* Two vectors are deemed perpendicular if the cosine of the angle between them is less than the specified angular tolerance.
* <br><br>
* <b>Effect:</b> Read-only
* <br><br>
* @param v1
* first vector.
* @param v2
* second vector.
* @param res
* the tolerance (the cosine of the angle).
**/
DECL_BASE logical perpendicular( SPAvector const &v1, SPAvector const &v2, const double res = SPAresnor);
DECL_BASE logical gme_perpendicular( SPAvector const &v1, SPAvector const &v2, const double res = SPAresnor);
/**
* Determines if two vectors are perpendicular (within some resolution).
* The default tolerance is <tt>SPAresnor</tt>.
* Two vectors are deemed perpendicular if the cosine of the angle between them is less than the specified angular tolerance.
* <br><br>
* <b>Effect:</b> Read-only
* <br><br>
* @param v1
* first vector.
* @param v2
* second vector.
* @param res
* the tolerance (the cosine of the angle).
**/
DECL_BASE logical perpendicular( SPAunit_vector const &v1, SPAvector const &v2, const double res = SPAresnor);
DECL_BASE logical gme_perpendicular( SPAunit_vector const &v1, SPAvector const &v2, const double res = SPAresnor);
/**
* Determines if two vectors are perpendicular (within some resolution).
* The default tolerance is <tt>SPAresnor</tt>.
* Two vectors are deemed perpendicular if the cosine of the angle between them is less than the specified angular tolerance.
* <br><br>
* <b>Effect:</b> Read-only
* <br><br>
* @param v1
* first vector.
* @param v2
* second vector.
* @param res
* the tolerance (the cosine of the angle).
**/
DECL_BASE logical perpendicular( SPAunit_vector const &v1, SPAunit_vector const &v2, const double res = SPAresnor);
DECL_BASE logical gme_perpendicular( SPAunit_vector const &v1, SPAunit_vector const &v2, const double res = SPAresnor);
/** @} */
#endif

