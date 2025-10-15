/*******************************************************************/
/*    Copyright (c) 1989-2020 by Spatial Corp.                     */
/*    All rights reserved.                                         */
/*    Protected by U.S. Patents 5,257,205; 5,351,196; 6,369,815;   */
/*                              5,982,378; 6,462,738; 6,941,251    */
/*    Protected by European Patents 0503642; 69220263.3            */
/*    Protected by Hong Kong Patent 1008101A                       */
/*******************************************************************/
// Header for SPAmatrix.
// This class stands for a 3x3 Euclidean affine transformation.
// Note that it is NOT a tensor.
#if !defined( MATRIX_CLASS )
#define MATRIX_CLASS
#include "base.hxx"
#include "dcl_base.h"
#include "logical.h"
#include "vector.hxx"
#include "debugmsc.hxx"
/**
 * @file matrix.hxx
 * @CAA2Level L1
 * @CAA2Usage U1
 * \addtogroup ACISGEOMOPERATORS
 *
 * @{
 */
class SPAtransf;
/**
 * Multiplies a matrix by a matrix.
 * <br><br>
 * @param m1
 * first matrix.
 * @param m2
 * second matrix.
 */
DECL_BASE SPAmatrix operator*( SPAmatrix const & m1, SPAmatrix const & m2 );
DECL_BASE SPAmatrix gme_operator_multiply( SPAmatrix const & m1, SPAmatrix const & m2 );
/**
 * Multiplies a matrix by a double.
 * <br><br>
 * @param d
 * value to multiply each element by.
 * @param m
 * matrix.
 */
DECL_BASE SPAmatrix operator*( double const & d, SPAmatrix const & m);
DECL_BASE SPAmatrix gme_operator_multiply( double const & d, SPAmatrix const & m);
/**
 * @nodoc
 * This is already documented in vector.hxx
 *
 * Returns the product of a matrix and a vector.
 * <br><br>
 * @param m
 * matrix.
 * @param vec
 * vector.
 */
DECL_BASE SPAvector operator*( SPAmatrix const & m, SPAvector const & vec);
DECL_BASE SPAvector gme_operator_multiply( SPAmatrix const & m, SPAvector const & vec);
/**
 * @nodoc
 * This is already documented in vector.hxx
 *
 * Returns the product of a vector and a matrix.
 * <br><br>
 * @param vec
 * vector.
 * @param m
 * matrix.
 */
DECL_BASE SPAvector operator*( SPAvector const & vec, SPAmatrix const & m);
DECL_BASE SPAvector gme_operator_multiply( SPAvector const & vec, SPAmatrix const & m);
/**
 * @nodoc
 * This is already documented in position.hxx
 *
 * Transforms a position by a matrix.
 * <br><br>
 * @param m
 * matrix.
 * @param pos
 * position.
 */
DECL_BASE SPAposition operator*( SPAmatrix const & m, SPAposition const & pos);
DECL_BASE SPAposition gme_operator_multiply( SPAmatrix const & m, SPAposition const & pos);
/**
 * @nodoc
 * This is already documented in position.hxx
 *
 * Transforms a position by a matrix.
 * <br><br>
 * @param pos
 * position.
 * @param m
 * matrix.
 */
DECL_BASE SPAposition operator*( SPAposition const & pos, SPAmatrix const & m);
DECL_BASE SPAposition gme_operator_multiply( SPAposition const & pos, SPAmatrix const & m);
/**
* Determines if two matrices are equal, within a tolerance.
* <br><br>
* <b>Effect:</b> System routine
* <br><br>
* @param m1
* first matrix.
* @param m2
* second matrix.
* @param res
* optional tolerance.
**/
DECL_BASE logical same_matrix( SPAmatrix const& m1, SPAmatrix const& m2, const double res = SPAresabs);
DECL_BASE logical gme_same_matrix( SPAmatrix const& m1, SPAmatrix const& m2, const double res = SPAresabs);
/**
* Creates the matrix for scaling.
* Scaling is equal for each coordinate.
* <br><br>
* <b>Effect:</b>  Read-only
* <br><br>
* @param factor
* scale factor.
**/
DECL_BASE SPAmatrix scaling( double factor );
DECL_BASE SPAmatrix gme_scaling( double factor );
/**
* Creates the matrix for scaling.
* Scaling is differential by coordinate.
* <br><br>
* <b>Effect:</b>  Read-only
* <br><br>
* @param xfactor
* x scale factor.
* @param yfactor
* y scale factor.
* @param zfactor
* z scale factor.
**/
DECL_BASE SPAmatrix scaling( double xfactor, double yfactor, double zfactor );
DECL_BASE SPAmatrix gme_scaling( double xfactor, double yfactor, double zfactor );
/**
* Creates the matrix for scaling.
* <br><br>
* <b>Effect:</b>  Read-only
* <br><br>
* @param factor
* xyz factors in a vector.
**/
DECL_BASE SPAmatrix scaling( const SPAvector & factor );
DECL_BASE SPAmatrix gme_scaling( const SPAvector & factor );
/**
* Creates the matrix for scaling.
* Scaling is differential by coordinate.
* <br><br>
* <b>Effect:</b>  Read-only
* <br><br>
* @param xfactor
* x scale factor.
* @param yfactor
* y scale factor.
* @param zfactor
* z scale factor.
* @param xyshear
* xy shear factor.
* @param xzshear
* xz shear factor.
* @param yzshear
* yz shear factor.
**/
DECL_BASE SPAmatrix scaling( double xfactor, double yfactor, double zfactor, double xyshear, double xzshear, double yzshear );
DECL_BASE SPAmatrix gme_scaling( double xfactor, double yfactor, double zfactor, double xyshear, double xzshear, double yzshear );
/**
 * @nodoc
 */
DECL_BASE SPAmatrix rotation( double, SPAvector const & );
DECL_BASE SPAmatrix gme_rotation( double, SPAvector const & );
/**
 * @nodoc
 */
DECL_BASE SPAmatrix reflection( SPAvector const & );
DECL_BASE SPAmatrix gme_reflection( SPAvector const & );
/**
 * Transforms a matrix by an affine transformation.
 * <br><br>
 * @param m
 * matrix.
 * @param trans
 * transform.
 */
DECL_BASE SPAmatrix operator*( SPAmatrix const & m, SPAtransf const & trans);
DECL_BASE SPAmatrix gme_operator_multiply( SPAmatrix const & m, SPAtransf const & trans);
/**
 * Transforms a matrix by an affine transformation.
 * <br><br>
 * @param m
 * matrix.
 * @param trans
 * transform.
 */
DECL_BASE SPAmatrix operator*( SPAmatrix const & m, SPAtransf const * trans);
DECL_BASE SPAmatrix gme_operator_multiply( SPAmatrix const & m, SPAtransf const * trans);
/** @} */
/**
 * \addtogroup ACISGEOMETRICATOMS
 *
 * @{
 */
/**
 * Defines a 3x3 affine transformation acting on vectors and positions.
 * <br>
 * <b>Role:</b> This class defines a 3x3 Euclidean affine transformation acting on
 * vectors and positions. It is not a tensor.
 * <br><br>
 * @see SPAtransf
 */
class DECL_BASE SPAmatrix {

	double elem[3][3];	// components of 3x3 SPAmatrix

	void zero();

public:

	/**
	 * C++ allocation constructor requests memory for this object but does not
	 * populate it.
	 */
	SPAmatrix() 
#if 0
		; // for mkman
#endif
#ifdef INTERNAL_DEBUG_CHECKS
		;
#else
		{}
#endif

	/**
	 * C++ initialize constructor requests memory for this object and populates
	 * it with the data supplied as arguments.
	 * <br><br>
	 * Constructs a matrix from three row-vectors.
	 * <br><br>
	 * @param vector1
	 * first vector.
	 * @param vector2
	 * second vector.
	 * @param vector3
	 * third vector.
	 */
	SPAmatrix( SPAvector const & vector1, SPAvector const & vector2, SPAvector const & vector3);
	SPAmatrix(const char * gme, SPAvector const & vector1, SPAvector const & vector2, SPAvector const & vector3);
	/**
	 * @nodoc
	 */
	MMGR_FREELIST_THIS
#if 0
; // semicolon needed for mkman (doc tool) parsing
#endif

	/**
	 * Extracts an element of this matrix.
	 * <br><br>
	 * @param row
	 * row value.
	 * @param col
	 * column value.
	 */
	double element( int row, int col ) const
		{ return elem[ row ][ col ]; }

	/**
	 * Extracts a column from this matrix.
	 * <br><br>
	 * @param in_col
	 * column number.
	 */
	SPAvector column(int in_col) const
		{ return SPAvector(elem[0][in_col], elem[1][in_col], elem[2][in_col]); }

	/**
	 * Extracts a row from this matrix.
	 * <br><br>
	 * @param in_row
	 * row number.
	 */
	SPAvector row(int in_row) const
		{ return SPAvector(elem[in_row][0], elem[in_row][1], elem[in_row][2]); }

	/**
	 * Assigns a value to an element in the matrix.
	 * <br><br>
	 * @param row
	 * row number.
	 * @param col
	 * column number.
	 * @param new_e
	 * element value.
	 */
	void set_element( int row, int col, double new_e )
		{ elem[ row ][ col ] = new_e; }

	/**
	 * Returns a transpose of this matrix.
	 */
	SPAmatrix transpose() const;
	SPAmatrix gme_transpose() const;
	/**
	 * Returns the determinant of this matrix.
	 */
	double determinant() const;
	double gme_determinant() const;
	/**
	 * Returns the inverse of this matrix.
	 * <br><br>
	 * <b>Role:</b> The most common case is for the matrix to represent a rotation
	 * matrix for a transform.  In this case, the matrix will be orthogonal, with
	 * unit determinant.
	 */
	SPAmatrix inverse() const;
	SPAmatrix gme_inverse() const;
	/**
	 * Returns <tt>TRUE</tt> if this matrix is the identity matrix.
	 */
	logical is_identity() const;
	logical gme_is_identity() const;
	/**
	 * @nodoc
	 */
	void polar_decomposition( SPAmatrix &P, SPAmatrix &U ) const;
	void gme_polar_decomposition( SPAmatrix &P, SPAmatrix &U ) const;
	/**
	 * Multiplies two matrices.
	 * <br><br>
	 * @param mat1
	 * matrix.
	 */
	SPAmatrix const &operator*=( SPAmatrix const & mat1);
	SPAmatrix const &gme_operator_multiply_assign( SPAmatrix const & mat1);
	/**
	 * Multiplies this matrix by a double.
	 * <br><br>
	 * @param d
	 * double.
	 */
	SPAmatrix const &operator*=( double const & d);
	SPAmatrix const &gme_operator_multiply_assign( double const & d);
	/**
	 * Transforms this matrix by an affine transformation.
	 * <br><br>
	 * @param trans
	 * transformation.
	 */
	SPAmatrix const &operator*=( SPAtransf const & trans);
	SPAmatrix const &gme_operator_multiply_assign( SPAtransf const & trans);
	// Output details of this matrix.
	/**
	 * Writes output about this matrix to the debug file or to the specified file.
	 * <br><br>
	 * @param title
	 * title.
	 * @param fp
	 * output file.
	 */
	void debug( char const * title, FILE * fp= debug_file_ptr ) const;
	void gme_debug( char const * title, FILE * fp= debug_file_ptr ) const;
	/**
	 * @nodoc
	 */
	friend DECL_BASE SPAmatrix operator*( SPAmatrix const &m1, SPAmatrix const &m2 );
	friend DECL_BASE SPAmatrix gme_operator_multiply( SPAmatrix const &m1, SPAmatrix const &m2 );
	/**
	 * @nodoc
	 */
	friend DECL_BASE SPAmatrix operator*( double const &d, SPAmatrix const &m );
	friend DECL_BASE SPAmatrix gme_operator_multiply( double const &d, SPAmatrix const &m );
	/**
	 * @nodoc
	 */
    logical to_SR(SPAmatrix& s, SPAmatrix& r, int& s_dim = *(int*)NULL_REF) const;

	/**
	 * @nodoc
	 */
	friend DECL_BASE SPAvector operator*( SPAmatrix const &m, SPAvector const &vec );
	friend DECL_BASE SPAvector gme_operator_multiply( SPAmatrix const &m, SPAvector const &vec );
	/**
	 * @nodoc
	 */
	friend DECL_BASE SPAvector operator*( SPAvector const &vec, SPAmatrix const &m );
	friend DECL_BASE SPAvector gme_operator_multiply( SPAvector const &vec, SPAmatrix const &m );
	/**
	 * @nodoc
	 */
	friend DECL_BASE SPAposition operator*( SPAmatrix const &m, SPAposition const &pos );
	friend DECL_BASE SPAposition gme_operator_multiply( SPAmatrix const &m, SPAposition const &pos );
	/**
	 * @nodoc
	 */
	friend DECL_BASE SPAposition operator*( SPAposition const &pos, SPAmatrix const &m );
	friend DECL_BASE SPAposition gme_operator_multiply( SPAposition const &pos, SPAmatrix const &m );
	/**
	 * @nodoc
	 */
	friend DECL_BASE logical same_matrix( SPAmatrix const& m1, SPAmatrix const& m2, const double res);
	friend DECL_BASE logical gme_same_matrix( SPAmatrix const& m1, SPAmatrix const& m2, const double res);
	// "Constructors" for particular types of transformation.
	// These are not actually constructors because there is
	// insufficient information in their argument types to
	// identify the different cases.

	// Form SPAmatrix for equal scaling.
	/**
	 * @nodoc
	 */
	friend DECL_BASE SPAmatrix scaling( double factor );
	friend DECL_BASE SPAmatrix gme_scaling( double factor );
	// Form SPAmatrix for differential scaling.
	/**
	 * @nodoc
	 */
	friend DECL_BASE SPAmatrix scaling( double xfactor, double yfactor, double zfactor );
	friend DECL_BASE SPAmatrix gme_scaling( double xfactor, double yfactor, double zfactor );
	// Form SPAmatrix for differential scaling.
	/**
	 * @nodoc
	 */
	friend DECL_BASE SPAmatrix scaling( const SPAvector &factor );
	friend DECL_BASE SPAmatrix gme_scaling( const SPAvector &factor );
	// Form SPAmatrix for differential scaling.
	/**
	 * @nodoc
	 */
	friend DECL_BASE SPAmatrix scaling( double xfactor, double yfactor, double zfactor, 
		double xyshear, double xzshear, double yzshear);
		friend DECL_BASE SPAmatrix gme_scaling( double xfactor, double yfactor, double zfactor, 
		double xyshear, double xzshear, double yzshear);
	// Form SPAmatrix for rotation by angle about SPAvector.
	/**
	 * @nodoc
	 */
	friend DECL_BASE SPAmatrix rotation( double, SPAvector const & );
		friend DECL_BASE SPAmatrix gme_rotation( double, SPAvector const & );
	// Form SPAmatrix for reflection about a plane through the origin
	// with the given SPAvector as normal.
	/**
	 * @nodoc
	 */
	friend DECL_BASE SPAmatrix reflection( SPAvector const & );
	friend DECL_BASE SPAmatrix gme_reflection( SPAvector const & );
	// Transform a SPAmatrix i.e. by an affine transformation.
	/**
	 * @nodoc
	 */
	friend DECL_BASE SPAmatrix operator*( SPAmatrix const &m, SPAtransf const &trans );

	/**
	 * @nodoc
	 */
	friend DECL_BASE SPAmatrix operator*( SPAmatrix const &m, SPAtransf const *trans );
};

/** @} */
/**
 * \addtogroup ACISGEOMOPERATORS
 *
 * @{
 */

/**
* Determines if two matrices are equal with respect to resnor.
* <br><br>
* @param m1
* first matrix.
* @param m2
* second matrix.
**/
inline logical operator==( SPAmatrix const &m1, SPAmatrix const &m2 )
	{ return same_matrix( m1, m2, SPAresnor ); }
inline logical gme_operator_equal( SPAmatrix const& m1, SPAmatrix const& m2 )
	{ return gme_same_matrix( m1, m2, SPAresnor ); }

/**
* Determines if two matrices are not equal with respect to resnor.
* <br><br>
* @param m1
* first matrix.
* @param m2
* second matrix.
**/
inline logical operator!=( SPAmatrix const &m1, SPAmatrix const &m2 )
	{ return !same_matrix( m1, m2, SPAresnor ); }
inline logical gme_operator_not_equal( SPAmatrix const& m1, SPAmatrix const& m2 )
	{ return !gme_same_matrix( m1, m2, SPAresnor ); }


/** @} */
#endif

