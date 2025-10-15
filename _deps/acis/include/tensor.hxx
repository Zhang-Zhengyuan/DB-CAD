/*******************************************************************/
/*    Copyright (c) 1989-2020 by Spatial Corp.                     */
/*    All rights reserved.                                         */
/*    Protected by U.S. Patents 5,257,205; 5,351,196; 6,369,815;   */
/*                              5,982,378; 6,462,738; 6,941,251    */
/*    Protected by European Patents 0503642; 69220263.3            */
/*    Protected by Hong Kong Patent 1008101A                       */
/*******************************************************************/

// Header for tensor (and symmetric tensor).

// This class defines 3x3 tensor.  First there is a general one, and
// then a special case of a symmetric tensor.  This latter is used
// internally to mass property calculations, as inertia tensors are
// symmetric, and this reduces the amout of calculation involved.
#if !defined( TENSOR_CLASS )
#define TENSOR_CLASS
class SPAmatrix;
class SPAvector;
#include "acis.hxx"
#include "dcl_kern.h"
#include "debugmsc.hxx"
#include "vector.hxx"
/**
 * @file tensor.hxx
 * @CAA2Level L1
 * @CAA2Usage U1
 * \addtogroup ACISGEOMOPERATORS
 *
 * @{
 */

class symtensor;
class tensor;

/**
 * Performs a binary plus operation.
 *<br>
 * @param first_ten
 * first tensor.
 * @param sec_ten
 * second tensor.
 */
DECL_KERN tensor operator+( tensor const & first_ten, tensor const & sec_ten);
DECL_KERN tensor gme_operator_add( tensor const & first_ten, tensor const & sec_ten);

/**
 * Performs a binary minus operation.
 *<br><br>
 * @param first_ten
 * first tensor.
 * @param sec_ten
 * second tensor.
 */
DECL_KERN tensor operator-( tensor const & first_ten, tensor const & sec_ten );
DECL_KERN tensor gme_operator_sub( tensor const & first_ten, tensor const & sec_ten );
/**
 * Performs a unary minus operation.
 *<br><br>
 * @param ten
 * tensor.
 */
DECL_KERN tensor operator-( tensor const & ten);
DECL_KERN tensor gme_operator_reverse( tensor const & ten);
/**
 * Multiplies a tensor by a double.
 *<br><br>
 * @param ten
 * tensor.
 * @param val
 * double value.
 */
DECL_KERN tensor operator*(tensor const & ten, double val);
DECL_KERN tensor gme_operator_multiply(tensor const & ten, double val);
/**
 * Multiplies a tensor by a double.
 *<br><br>
 * @param val
 * double value.
 * @param ten
 * tensor.
 */
DECL_KERN tensor operator*(double val, tensor const & ten);
DECL_KERN tensor gme_operator_multiply(double val, tensor const & ten);
/**
 * Transforms a tensor by a <tt>3 x 3</tt> (orthogonal) matrix.
 *<br><br>
 * <b>Role:</b> This method pre-multiplies the tensor by the matrix, then it
 * post-multiplies the result by the transpose of the matrix.
 *<br><br>
 * @param ten
 * tensor.
 * @param mat
 * matrix.
 */
DECL_KERN tensor operator*(tensor const & ten, SPAmatrix const & mat);
DECL_KERN tensor gme_operator_multiply(tensor const & ten, SPAmatrix const & mat);
/**
 * Transforms a tensor by a general transformation.
 *<br><br>
 * <b>Role:</b> This method does a matrix multiplication for the rotation part,
 * and then multiplies it by the scaling factor twice any translation part is ignored.
 * This treats the tensor on a par with a vector (as opposed to a position or unit vector).
 *<br><br>
 * @param ten
 * tensor.
 * @param transf
 * transformation.
 */
DECL_KERN tensor operator*(tensor const & ten , SPAtransf const & transf);
DECL_KERN tensor gme_operator_multiply(tensor const & ten , SPAtransf const & transf);
/** @} */
/**
 * \addtogroup ACISGEOMETRICATOMS
 *
 * @{
 */
/**
 * Defines a <tt>3 x 3</tt> tensor.
 *<br><br>
 * <b>Role:</b> This class defines a <tt>3 x 3</tt> general tensor. Examples of tensors
 * include the inertia tensor of mass-property calculations, and the second-order
 * gradient of an implicit function of the three spatial variables <i>x</i>, <i>y</i>, and <i>z</i>.
 */
class DECL_KERN tensor : public ACIS_OBJECT {

	double elem[3][3];

public:
    /**
     * C++ allocation constructor requests memory for this object but does not populate it.
     */
	tensor() {}			// allow uninitialised tensors

	/**
	 * C++ initialize constructor requests memory for this object and populates it with the data supplied as arguments.
	 *<br><br>
	 * @param sym_ten
	 * symmetric tensor.
	 */
	tensor( symtensor const & sym_tem);
        tensor(const char*, symtensor const & sym_tem);
    /**
     * Zeroes all elements of the tensor.
     */
	void zero();		// zero elements of tensor
        void gme_zero();

	// Extract an element (i,j) where row and column, i and j, are
	// in the range 0 to 2.
    /**
     * Extracts an element <i>(i,j)</i>, where row and column, <i>i</i> and <i>j</i>, range from <tt>0</tt> to <tt>2</tt>.
     *<br><br>
     * @param row
     * row.
     * @param col
     * column.
     */
	double element( int row, int col ) const
		{ return elem[ row ][ col ]; }

	// Set an element (i,j) where i and j are in the range 0 to 2.
    /**
     * Sets an element <i>(i,j)</i>, where <i>i</i> and <i>j</i> range from <tt>0</tt> to <tt>2</tt>.
     *<br><br>
     * @param i
     * ith value.
     * @param j
     * jth value.
     * @param set
     * value to be set.
     */
	void set_element( int i, int j, double set);
        void gme_set_element( int i, int j, double set);
	// Unary minus
    /*
    // tbrv
    */
    /**
     * @nodoc
     */
	friend DECL_KERN tensor operator-( tensor const & );
        friend DECL_KERN tensor gme_operator_reverse( tensor const & );

	// Binary plus

    /*
    // tbrv
    */
    /**
     * @nodoc
     */
	friend DECL_KERN tensor operator+( tensor const &, tensor const & );
        friend DECL_KERN tensor gme_operator_add( tensor const &, tensor const & );

	/**
	 * Performs the unary plus operation.
	 *<br><br>
	 * @param ten
	 * tensor.
	 */
	tensor const &operator+=( tensor const & ten);
        tensor const &gme_operator_add_assign( tensor const & ten);

	// Binary minus
    /*
    // tbrv
    */
    /**
     * @nodoc
     */
	friend DECL_KERN tensor operator-( tensor const &, tensor const & );
        friend DECL_KERN tensor gme_operator_sub( tensor const &, tensor const & );

	/**
	 * Performs the unary minus operation.
	 *<br><br>
	 * @param ten
	 * tensor.
	 */
	tensor const &operator-=( tensor const & ten);
        tensor const &gme_operator_sub_assign( tensor const & ten);

	// Multiply tensor by a double.
    /*
    // tbrv
    */
    /**
     * @nodoc
     */

	friend DECL_KERN tensor operator*( tensor const &, double );
        friend DECL_KERN tensor gme_operator_multiply( tensor const &, double );
	/*
	// tbrv
	*/
	/**
	 * @nodoc
	 */
	friend DECL_KERN tensor operator*( double, tensor const & );
        friend DECL_KERN tensor gme_operator_multiply( double, tensor const & );
	/**
	 * Multiplies a tensor by a double.
	 *<br><br>
	 * @param val
	 * double value.
	 */
	tensor const &operator*=( double val);
        tensor const &gme_operator_multiply_assign( double val);

	// Transform a tensor by a 3x3 (orthogonal) SPAmatrix.
	// Pre-multiplies tensor by SPAmatrix, then post-multiplies
	// result by transpose of SPAmatrix.
    /*
    // tbrv
    */
    /**
     * @nodoc
     */
	friend DECL_KERN tensor operator*( tensor const &, SPAmatrix const & );
        friend DECL_KERN tensor gme_operator_multiply( tensor const &, SPAmatrix const & );
	/**
	 * Transforms a tensor by a <tt>3 x 3</tt> (orthogonal) matrix.
	 *<br><br>
	 * <b>Role:</b> This method pre-multiplies tensor by the matrix, and then
	 * post-multiplies the result by the transpose of the matrix.
	 *<br><br>
	 * @param mat
	 * matrix.
	 */
	tensor const &operator*=( SPAmatrix const & mat);
        tensor const &gme_operator_multiply_assign( SPAmatrix const & mat);

	// Same for a transformation: this does a SPAmatrix multiplication
	// for the rotation part, then multiplies by the scaling twice.
	// any translation part is ignored. This treats the tensor on a
	// par with a SPAvector (as opposed to a SPAposition or unit SPAvector).
    /*
    // tbrv
    */
    /**
     * @nodoc
     */
	friend DECL_KERN tensor operator*( tensor const &, SPAtransf const & );
        friend DECL_KERN tensor gme_operator_multiply( tensor const &, SPAtransf const & );

	/**
	 * Transforms a tensor by a <tt>3 x 3</tt> transformation.
	 *<br><br>
	 * <b>Role:</b> This method performs the matrix multiplication for the rotation
	 * part, then it multiplies by the scaling factor twice. Any translation part
	 * is ignored. This method treats the tensor like a vector (as opposed to a
	 * position or unit vector).
	 *<br><br>
	 * @param transf
	 * transformation.
	 */
	tensor const &operator*=( SPAtransf const & transf);
        tensor const &gme_operator_multiply_assign( SPAtransf const & transf);

	// Output details of a tensor.
    /**
     * Prints a tensor in three rows with a given leader string on each row bar first.
     *<br><br>
     * @param leader
     * leader.
     * @param filename
     * file name.
     */
	void debug( char const * leader, FILE * filename = debug_file_ptr ) const;
        void gme_debug( char const * leader, FILE * filename = debug_file_ptr ) const;
};
/** @} */
/**
 * \addtogroup ACISGEOMOPERATORS
 *
 * @{
 */
// Symmetric tensor. Obeys the same rules as for a tensor, but we store
// the off-diagonal terms only once, to reduce the calculation
// involved.
/*
// tbrv
*/
/**
 * @nodoc
 */

DECL_KERN symtensor operator-( symtensor const & );
DECL_KERN symtensor gme_operator_reverse( symtensor const & );
/*
// tbrv
*/
/**
 * @nodoc
 */

DECL_KERN symtensor operator+( symtensor const &, symtensor const & );
DECL_KERN symtensor gme_operator_add( symtensor const &, symtensor const & );
/*
// tbrv
*/
/**
 * @nodoc
 */

DECL_KERN symtensor operator-( symtensor const &, symtensor const & );
DECL_KERN symtensor gme_operator_sub( symtensor const &, symtensor const & );
/*
// tbrv
*/
/**
 * @nodoc
 */

DECL_KERN symtensor operator*( symtensor const &, double );
DECL_KERN symtensor gme_operator_multiply( symtensor const &, double );
/*
// tbrv
*/
/**
 * @nodoc
 */

DECL_KERN symtensor operator*( double, symtensor const & );
DECL_KERN symtensor gme_operator_multiply( double, symtensor const & );
/*
// tbrv
*/
/**
 * @nodoc
 */

DECL_KERN symtensor operator*( symtensor const &, SPAmatrix const & );
DECL_KERN symtensor gme_operator_multiply( symtensor const &, SPAmatrix const & );
/*
// tbrv
*/
/**
 * @nodoc
 */

DECL_KERN symtensor operator*( symtensor const &, SPAtransf const & );
DECL_KERN symtensor gme_operator_multiply( symtensor const &, SPAtransf const & );
/** @} */
/**
 * \addtogroup ACISGEOMETRICATOMS
 *
 * @{
 */
/*
// tbrv
*/
/**
 * @nodoc
 */
class DECL_KERN symtensor : public ACIS_OBJECT {

	SPAvector	diag;		// main diagonal terms (xx, yy, zz)
	SPAvector	off_diag;	// off-diag terms (yz, zx, xy)

public:

	symtensor() {}		// allow unitialised symtensors

	// Construct a symtensor from two vectors holding the diagonal
	// and off-diagonal terms.

	symtensor( SPAvector const &u, SPAvector const &v )
		{ diag = u; off_diag = v; }

	// Zero the terms of a symtensor.

	void zero();
        void gme_zero();

	// Return the major diagonal terms in a SPAvector.

	SPAvector diagonal() const
		{ return diag; }

	// Return the off-diagonal terms in a SPAvector (yz, zx, xy).

	SPAvector off_diagonal() const
		{ return off_diag; }

	// Set the diagonal terms of a symtensor.

	void set_diag( SPAvector const & );
        void gme_set_diag( SPAvector const & );


	// Set the off-diagonal terms in a symtensor.

	void set_off_diag( SPAvector const & );
        void gme_set_off_diag( SPAvector const & );

	// Unary minus.

	friend DECL_KERN symtensor operator-( symtensor const & );
        friend DECL_KERN symtensor gme_operator_reverse( symtensor const & );

	// Binary plus.

	friend DECL_KERN symtensor operator+( symtensor const &, symtensor const & );
        friend DECL_KERN symtensor gme_operator_add( symtensor const &, symtensor const & );
	symtensor const &operator+=( symtensor const & );
        symtensor const &gme_operator_add_assign( symtensor const & );

	// Binary minus.

	friend DECL_KERN symtensor operator-( symtensor const &, symtensor const & );
        friend DECL_KERN symtensor gme_operator_sub( symtensor const &, symtensor const & );
	symtensor const &operator-=( symtensor const & );
        symtensor const &gme_operator_sub_assign( symtensor const & );

	// Multiply a symtensor by a double.

	friend DECL_KERN symtensor operator*( symtensor const &, double );
        friend DECL_KERN symtensor gme_operator_multiply( symtensor const &, double );
	friend DECL_KERN symtensor operator*( double, symtensor const & );
        friend DECL_KERN symtensor gme_operator_multiply( double, symtensor const & );
	symtensor const &operator*=( double );
        symtensor const &gme_operator_multiply_assign( double );

	// Transform a symtensor by a 3x3 (orthogonal) SPAmatrix.
	// Pre-multiplies symtensor by SPAmatrix, then post-multiplies
	// result by transpose of SPAmatrix.

	friend DECL_KERN symtensor operator*( symtensor const &, SPAmatrix const & );
        friend DECL_KERN symtensor gme_operator_multiply( symtensor const &, SPAmatrix const & );
	symtensor const &operator*=( SPAmatrix const & );
        symtensor const &gme_operator_multiply_assign( SPAmatrix const & );

	// Same for a transformation: this does a SPAmatrix multiplication
	// for the rotation part, then multiplies by the scaling twice.
	// any translation part is ignored. This treats the tensor on a
	// par with a SPAvector (as opposed to a SPAposition or unit SPAvector).

	friend DECL_KERN symtensor operator*( symtensor const &, SPAtransf const & );
        friend DECL_KERN symtensor gme_operator_multiply( symtensor const &, SPAtransf const & );
	symtensor const &operator*=( SPAtransf const & );
        symtensor const &gme_operator_multiply_assign( SPAtransf const & );

	// Output details of a symtensor.

	void debug( char const *, FILE * = debug_file_ptr ) const;
        void gme_debug( char const *, FILE * = debug_file_ptr ) const;
};
/** @} */
/**
 * \addtogroup ACISGEOMOPERATORS
 *
 * @{
 */
// Form a tensor as the outer product of two vectors.
/*
// tbrv
*/
/**
 * @nodoc
 */

DECL_KERN tensor outer( SPAvector const&, SPAvector const& );
DECL_KERN tensor gme_outer( SPAvector const&, SPAvector const& );

// Form the outer product of a SPAvector with itself.
/*
// tbrv
*/
/**
 * @nodoc
 */

DECL_KERN symtensor outer( SPAvector const& );
DECL_KERN symtensor gme_outer( SPAvector const& );

// Form a symtensor from vectors a and b as
// 0.5*( outer(a,b) + outer(b,a) ).
/*
// tbrv
*/
/**
 * @nodoc
 */

DECL_KERN symtensor sym_outer( SPAvector const&, SPAvector const& );
DECL_KERN symtensor gme_sym_outer( SPAvector const&, SPAvector const& );

/** @} */
#endif
