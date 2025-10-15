/*******************************************************************/
/*    Copyright (c) 1989-2020 by Spatial Corp.                     */
/*    All rights reserved.                                         */
/*    Protected by U.S. Patents 5,257,205; 5,351,196; 6,369,815;   */
/*                              5,982,378; 6,462,738; 6,941,251    */
/*    Protected by European Patents 0503642; 69220263.3            */
/*    Protected by Hong Kong Patent 1008101A                       */
/*******************************************************************/
// Header for SPAposition.
// A position vector in three-dimensional cartesian space is
// subject to transformations and certain vector operators.  This
// class is distinct from the SPAvector class, which is a displacement
// and thus origin independent.
/*******************************************************************/

#if !defined( POSITION_CLASS )
#define POSITION_CLASS
#include "base.hxx"
#include "dcl_base.h"
#include "logical.h"
#include "debugmsc.hxx"
/**
 * @file position.hxx
 * @CAA2Level L1
 * @CAA2Usage U1
 * \defgroup ACISGEOMETRICATOMS Geometric Atoms
 * \ingroup  ACISBASE
 *
 * \defgroup ACISGEOMOPERATORS  Geometric Operators
 * \ingroup ACISGEOMETRICATOMS
 *
 */
/**
 * \addtogroup ACISGEOMOPERATORS
 *
 * @{
 */
class SPAvector;
class SPAunit_vector;
class SPAmatrix;
class SPAtransf;
class SPAposition;

// STI: sak 8/02: These declarations are necessary for VA5 on aix. They
// inject the friend function scope into the namespace.

/**
 * Multiplies a position by a double.
 * <br><br>
 * @param p
 * position.
 * @param d
 * double.
 */
DECL_BASE SPAposition operator*( SPAposition const &p, double d);
DECL_BASE SPAposition gme_operator_multiply(const  SPAposition &p, double d);
/**
 * Returns the displacement (a vector) as the difference of two positions.
 * <br><br>
 * @param p1
 * first position.
 * @param p2
 * second position.
 */
DECL_BASE SPAvector operator-( SPAposition const &p1, SPAposition const &p2 );
DECL_BASE SPAvector gme_operator_sub(const  SPAposition &p1,const SPAposition  &p2 );
/**
 * Translates a position by a vector.
 * <br><br>
 * @param p
 * position.
 * @param v
 * vector.
 */
DECL_BASE SPAposition operator+( SPAposition const &p, SPAvector const &v );
DECL_BASE SPAposition gme_operator_add(const  SPAposition &p,const  SPAvector &v );
/**
 * Translates a position by a vector.
 * <br><br>
 * @param v
 * vector.
 * @param p
 * position.
 */
DECL_BASE SPAposition operator+( SPAvector const &v, SPAposition const &p );
DECL_BASE SPAposition gme_operator_add( const SPAvector &v, const SPAposition &p );
/**
 * Translates a position by a vector.
 * <br><br>
 * @param p
 * position.
 * @param v
 * vector.
 */
DECL_BASE SPAposition operator-( SPAposition const &p, SPAvector const &v );
DECL_BASE SPAposition gme_operator_sub( const SPAposition &p, const SPAvector &v );
/**
 * Returns the cross product of a position with a unit vector.
 * <br><br>
 * @param p
 * position.
 * @param uv
 * unit vector.
 */
DECL_BASE SPAposition operator*( SPAposition const &p, SPAunit_vector const &uv );
DECL_BASE SPAposition gme_operator_multiply( const SPAposition &p, const SPAunit_vector &uv );
/**
 * Returns the cross product of a unit vector with a position.
 * <br><br>
 * @param uv
 * unit vector.
 * @param p
 * position.
 */
DECL_BASE SPAposition operator*( SPAunit_vector const &uv, SPAposition const &p );
DECL_BASE SPAposition gme_operator_multiply( SPAunit_vector const &uv, SPAposition const &p );
/**
 * Transforms a position.
 * <br><br>
 * @param m
 * affine transformation matrix.
 * @param p
 * position.
 */
DECL_BASE SPAposition operator*( SPAmatrix const &m, SPAposition const &p );
DECL_BASE SPAposition gme_operator_multiply( SPAmatrix const &m, SPAposition const &p );
/**
 * Transforms a position.
 * <br><br>
 * @param p
 * position.
 * @param m
 * affine transformation matrix.
 */
DECL_BASE SPAposition operator*( SPAposition const &p, SPAmatrix const &m );
DECL_BASE SPAposition gme_operator_multiply( SPAposition const &p, SPAmatrix const &m );
/**
 * Transforms a position.
 * <br><br>
 * @param p
 * position.
 * @param t
 * transformation.
 */
DECL_BASE SPAposition operator*( SPAposition const &p, SPAtransf const &t );
DECL_BASE SPAposition gme_operator_multiply( SPAposition const &p, SPAtransf const &t );
/**
 * Transforms a position.
 * <br><br>
 * @param p
 * position.
 * @param t
 * transformation.
 */
DECL_BASE SPAposition operator*( SPAposition const &p, SPAtransf const *t );
DECL_BASE SPAposition gme_operator_multiply( SPAposition const &p, SPAtransf const *t );
/**
 * Interpolates between two positions.
 * <br><br>
 * <b>Role:</b> The parameter gives the portion of the segment between the first
 * position and the second position. This function returns:
 * <br><br>
 * <tt>((1 - parameter)*p1 + (parameter*p2))</tt>
 * <br><br>
 * for the given parameter and two positions. If the parameter is less than 0 or
 * greater than 1, the function extrapolates.
 * <br><br>
 * <b>Effect:</b> Read-only
 * <br><br>
 * @param param
 * parameter for interpolation.
 * @param p1
 * first position (p1).
 * @param p2
 * second position (p2).
 **/
DECL_BASE SPAposition interpolate( double param, SPAposition const &p1, SPAposition const &p2 );
DECL_BASE SPAposition gme_interpolate( double param, SPAposition const &p1, SPAposition const &p2 );
/**
 * Returns <tt>TRUE</tt> if the two positions are the same (i.e., they lie within the specified resolution or <tt>SPAresabs</tt>);
 * otherwise returns <tt>FALSE</tt>.
 * <br><br>
 * @param p1
 * first position.
 * @param p2
 * second position.
 * @param res
 * optional positional tolerance.
 */
DECL_BASE logical same_point( SPAposition const&p1, SPAposition const&p2, const double res = SPAresabs);


/** @} */
/**
 * \addtogroup ACISGEOMETRICATOMS
 *
 * @{
 */
/**
 * Represents position vectors (points) in 3D Cartesian space that are subject to certain vector and transformation operations.
 * <br>
 * <b>Role:</b> This class represents position vectors (points) in 3D Cartesian
 * space that are subject to certain vector and transformation operations. This
 * class is distinct from the @href SPAvector class, which is a displacement and is origin independent.
 */
class DECL_BASE SPAposition {

	double coord[ 3 ];	// x, y and z coordinates of the SPAposition


	// Scale the object space.
	// This requires care, and so the operator is private.
/**
 * @nodoc
 */
 	friend DECL_BASE SPAposition operator*( SPAposition const &p, double d);
	friend DECL_BASE SPAposition gme_operator_multiply(const SPAposition &p, double d);
public:

	// Force creation of all positions to be by constructor.
	/**
	 * C++ allocation constructor requests memory for this object but does not populate it.
	 */
	SPAposition() 
#if 0
		; // for mkman
#endif
#ifdef INTERNAL_DEBUG_CHECKS
		;
#else
		{}
#endif

	// Construct a SPAposition from three doubles.
/**
 * C++ initialize constructor requests memory for this object and populates it with the data supplied as arguments.
 * <br><br>
 * <b>Role:</b> Creates a <tt>SPAposition</tt> using the <i>x, y, z</i> coordinates.
 * <br><br>
 * @param xi
 * x-coordinate value.
 * @param yi
 * y-coordinate value.
 * @param zi
 * z-coordinate value.
 */
	inline SPAposition( double xi, double yi, double zi ) {
		coord[ 0 ] = xi;
		coord[ 1 ] = yi;
		coord[ 2 ] = zi;
	}

	// Construct a SPAposition from an array of three doubles.
/**
 * C++ initialize constructor requests memory for this object and populates it with the data supplied as arguments.
 * <br><br>
 * <b>Role:</b> Creates a <tt>SPAposition</tt> using the specified array of three doubles, specifying the <i>x, y, z</i> coordinates.
 * <br><br>
 * @param p
 * array of 3 doubles.
 */
	inline SPAposition( double p[ 3 ] ) {
		coord[ 0 ] = p[ 0 ];
		coord[ 1 ] = p[ 1 ];
		coord[ 2 ] = p[ 2 ];
	}

	// Copy a SPAposition
/**
 * C++ copy constructor requests memory for this object and populates it with the data from the object supplied as an argument.
 * <br><br>
 * @param p
 * given position.
 */
	inline SPAposition( SPAposition const &p ) {
		coord[ 0 ] = p.coord[ 0 ];
		coord[ 1 ] = p.coord[ 1 ];
		coord[ 2 ] = p.coord[ 2 ];
	}
	// MS IA64 Compiler bug
#if (_MSC_VER == 1300) && defined(_WIN64)

/**
 * @nodoc
 */
	~SPAposition() {}

#endif

/**
 * @nodoc
 */
	MMGR_FREELIST_THIS
#if 0
; // semicolon needed for mkman (doc tool) parsing
#endif

	// Extract a coordinate value.
/**
 * Returns the x-coordinate value.
 */
	inline double x() const
		{ return coord[ 0 ]; }

/**
 * Returns the y-coordinate value.
 */
	inline double y() const
		{ return coord[ 1 ]; }
/**
 * Returns the z-coordinate value.
 */
	inline double z() const
		{ return coord[ 2 ]; }
/**
 * Returns the ith component value.
 * <br><br>
 * The <i>x</i> coordinate has an index value of 0, 
 * the <i>y</i> coordinate has an index value of 1, and
 * the <i>z</i> coordinate has an index value of 2.
 * <br><br>
 * @param i
 * ith component.
 */
	inline double coordinate( int i ) const
		{ return coord[ i ]; }
	// Extract a coordinate value for update.
/**
 * Extracts the x-coordinate value.
 */
	inline double &x()
		{ return coord[ 0 ]; }

/**
 * Extracts the y-coordinate value.
 */
	inline double &y()
		{ return coord[ 1 ]; }

/**
 * Extracts the z-coordinate value.
 */
	inline double &z()
		{ return coord[ 2 ]; }

/**
 * Extracts the ith component value.
 * <br><br>
 * The <i>x</i> coordinate has an index value of 0, 
 * the <i>y</i> coordinate has an index value of 1, and
 * the <i>z</i> coordinate has an index value of 2.
 * <br><br>
 * @param i
 * ith component.
 */
	inline double &coordinate( int i )
		{ return coord[ i ]; }

 	// Set coordinate values.
/**
 * Sets the x-coordinate value.
 * <br><br>
 * @param new_x
 * new x-coordinate value.
 */
 	inline void set_x( double new_x ) { coord[ 0 ] = new_x; }
/**
 * Sets the y-coordinate value.
 * <br><br>
 * @param new_y
 * new y-coordinate value.
 */
 	inline void set_y( double new_y ) { coord[ 1 ] = new_y; }

/**
 * Sets the z-coordinate value.
 * <br><br>
 * @param new_z
 * new z-coordinate value.
 */
 	inline void set_z( double new_z ) { coord[ 2 ] = new_z; }
/**
 * Sets the ith component value.
 * <br><br>
 * The <i>x</i> coordinate has an index value of 0, 
 * the <i>y</i> coordinate has an index value of 1, and
 * the <i>z</i> coordinate has an index value of 2.
 * <br><br>
 * @param i
 * ith component.
 * @param new_c
 * new coordinate value.
 */
 	inline void set_coordinate( int i, double new_c )
		{ coord[ i ] = new_c; }

	// Position operators

	// Get displacement, i.e. a SPAvector, as difference of two positions.
/**
 * @nodoc
 */
	friend DECL_BASE SPAvector operator-( SPAposition const &p1, SPAposition const &p2 );
	friend DECL_BASE SPAvector gme_operator_sub( SPAposition const &p1, SPAposition const &p2 );

	// Translate a SPAposition by a SPAvector.
/**
 * @nodoc
 */
	friend DECL_BASE SPAposition operator+( SPAposition const &p, SPAvector const &v );
	friend DECL_BASE SPAposition gme_operator_add( SPAposition const &p, SPAvector const &v );
/**
 * @nodoc
 */
	friend DECL_BASE SPAposition operator+( SPAvector const &v, SPAposition const &p );
	friend DECL_BASE SPAposition gme_operator_add( SPAvector const &v, SPAposition const &p );
/**
 * Translates a position by a vector.
 * <br><br>
 * @param v
 * vector.
 */
	SPAposition const &operator+=( SPAvector const &v);
	SPAposition const &gme_operator_add_assign( const SPAvector &v);
/**
 * @nodoc
 */
	friend DECL_BASE SPAposition operator-( SPAposition const &p, SPAvector const &v );
	friend DECL_BASE SPAposition gme_operator_sub( SPAposition const &p, SPAvector const &v );
/**
 * Translates a position by a vector.
 * <br><br>
 * @param v
 * vector.
 */
	SPAposition const &operator-=( SPAvector const &v );
	SPAposition const &gme_operator_sub_assign( const SPAvector &v );

	// Scalar product of SPAposition with SPAvector.
/**
 * @nodoc
 */
	friend double operator%( SPAposition const &p, SPAvector const &v );
/**
 * @nodoc
 */
	friend double operator%( SPAvector const &v, SPAposition const &p );
/**
 * @nodoc
 */
	friend double operator%( SPAposition const &p, SPAunit_vector const &uv );
/**
 * @nodoc
 */
	friend double operator%( SPAunit_vector const &uv, SPAposition const &p );



	// Cross product of a SPAposition with a unit SPAvector.
/**
 * @nodoc
 */
	friend DECL_BASE SPAposition operator*( SPAposition const &p, SPAunit_vector const &uv );
	friend DECL_BASE SPAposition gme_operator_multiply( SPAposition const &p, SPAunit_vector const &uv );
/**
 * @nodoc
 */
	friend DECL_BASE SPAposition operator*( SPAunit_vector const &uv, SPAposition const &p );
	friend DECL_BASE SPAposition gme_operator_multiply( SPAunit_vector const &uv, SPAposition const &p );

	// Transform a SPAposition.

/**
 * @nodoc
 */
	friend DECL_BASE SPAposition operator*( SPAmatrix const &m, SPAposition const &p );
	friend DECL_BASE SPAposition gme_operator_multiply( SPAmatrix const &m, SPAposition const &p );
/**
 * @nodoc
 */
	friend DECL_BASE SPAposition operator*( SPAposition const &p, SPAmatrix const &m );
	friend DECL_BASE SPAposition gme_operator_multiply( SPAposition const &p, SPAmatrix const &m );
/**
 * Transforms a vector by an affine transformation matrix.
 * <br><br>
 * @param m
 * affine transformation matrix.
 */
	SPAposition const &operator*=( SPAmatrix const & m);
	SPAposition const &gme_operator_multiply_assign( SPAmatrix const & m);
/**
 * @nodoc
 */
	friend DECL_BASE SPAposition operator*( SPAposition const &p, SPAtransf const &t );
	friend DECL_BASE SPAposition gme_operator_multiply( const SPAposition &p, const SPAtransf  &t );
/**
 * @nodoc
 */
	friend DECL_BASE SPAposition operator*( SPAposition const &p, SPAtransf const *t );
	friend DECL_BASE SPAposition gme_operator_multiply( const SPAposition &p, const SPAtransf *t );
/**
 * Transforms a position.
 * <br><br>
 * @param t
 * transformation.
 */
	SPAposition const &operator*=( SPAtransf const &t );
	SPAposition const &gme_operator_multiply_assign( SPAtransf const &t );

	// Interpolate between two positions. The parameter gives the
	// proportion of the segment between p1 and p2. In fact, the
	// parameter can be < 0 or > 1, in which case the function
	// extrapolates.

/**
 * @nodoc
 */
	friend DECL_BASE SPAposition interpolate( double param,
						SPAposition const &p1, SPAposition const &p2 );
	friend DECL_BASE SPAposition gme_interpolate( double param,
						SPAposition const &p1, SPAposition const &p2 );
	// STI let: add some general functions

	// Determine if 2 positions are equal, given some resolution

/**
 * @nodoc
 */
	friend DECL_BASE logical same_point( SPAposition const &p1, SPAposition const &p2,
		const double res );

	// Output details of a SPAposition.
/**
 * Writes information about the position to the debug file or the specified file.
 * <br><br>
 * @param fp
 * output file.
 */
	void debug( FILE * fp = debug_file_ptr ) const;
	void gme_debug( FILE * fp = debug_file_ptr ) const;
/**
 * Concatenates the information about the position to the passed string.
 * <br><br>
 * @param strP
 * given string.
 */
	void debug_str(char * strP) const;
	void gme_debug_str(char * strP) const;
};
/** @} */
/**
 * \addtogroup ACISGEOMOPERATORS
 *
 * @{
 */

/**
 * @nodoc
 */
inline logical same_point( SPAposition const &p1, SPAposition const &p2, const double res ) {
	double res2=res*res;
	double Len_SQ=0.0;
	for (int i=0; i<3; i++)
	{
		double dv = (p1.coord[i] - p2.coord[i])*(p1.coord[i] - p2.coord[i]);
		if( dv > res2 ) return FALSE;
		Len_SQ+=dv;
	}
	return  Len_SQ < res2 ;
};

// STI let (7/96): global inline operators
// Determine if 2 positions are within SPAresabs of each other
/**
 * Determines if two positions are within <tt>SPAresabs</tt> of each other.
 * @param p1
 * first position
 * @param p2
 * second position
 */
inline logical operator==( SPAposition const &p1, SPAposition const &p2 )
	{ return same_point( p1, p2, SPAresabs ); }

/**
 * Determines if two positions are <i>not</i> within <tt>SPAresabs</tt> of each other.
 * @param p1
 * first position
 * @param p2
 * second position
 */
inline logical operator!=( SPAposition const &p1, SPAposition const &p2 )
	{ return !same_point( p1, p2, SPAresabs ); }
// STI let: end

// writes a point into a file as a scheme command for visualization
// [in] pFp   - the file to write
// [in] Color - color of the entity drawn
// [in] Pos   - the point to be visualized
//
/**
 * @nodoc
 */
void DECL_BASE debug_scheme_pos( FILE *pFp, int Color, SPAposition const &Pos );
void DECL_BASE gme_debug_scheme_pos( FILE *pFp, int Color, SPAposition const &Pos );
// writes a segment represented by two endpoints into a file as a scheme command for visualization
// [in] pFp   - the file to write
// [in] Color - color of the entity drawn
// [in] P0    - the first point of the segment
// [in] P1    - the other end point of the segment
//
/**
 * @nodoc
 */
void DECL_BASE debug_scheme_segment( FILE *pFp, int Color, SPAposition const &P0, SPAposition const &P1 );
void DECL_BASE gme_debug_scheme_segment( FILE *pFp, int Color, SPAposition const &P0, SPAposition const &P1 );
/** @} */
#endif

