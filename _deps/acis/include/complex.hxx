/*******************************************************************/
/*    Copyright (c) 1989-2020 by Spatial Corp.                     */
/*    All rights reserved.                                         */
/*    Protected by U.S. Patents 5,257,205; 5,351,196; 6,369,815;   */
/*                              5,982,378; 6,462,738; 6,941,251    */
/*    Protected by European Patents 0503642; 69220263.3            */
/*    Protected by Hong Kong Patent 1008101A                       */
/*******************************************************************/
#ifndef complex_number_HXX
#define complex_number_HXX
#include "base.hxx"
#include "dcl_base.h"
#include "logical.h"
/**
 * @file complex.hxx
 * @CAA2Level L1
 * @CAA2Usage U1
 * \addtogroup ACISGEOMETRICATOMS
 *
 * @{
 */

/**
 * Creates a data structure for the manipulation of complex number.
 * <br>
 * <b>Role:</b> This is a C++ class that holds two doubles, which are meant to reflect a real
 * and imaginary part. This class has contains methods for all of the traditional overload operations.
 * <br>
 * The complex number class has the overloaded C++ addition, subtraction, multiplication, and division operators.
 * <pre>
 * complex_number a(1,2);
 * complex_number b(3,4);
 * complex_number c, d;
 * c = a + b;		// c = 4 + 6i
 * d = a * b;		// d = -5 + 10i
 * </pre>
 */
class DECL_BASE complex_number : public ACIS_OBJECT {

public:
/**
 * The real component of the complex number.
 */
	double re;
/**
 * The imaginary component of the complex number.
 */
	double im;
/**
 * C++ allocation constructor requests memory for this object but does not populate it.
 */
	complex_number();
/**
 * C++ initialize constructor requests memory for this object and populates it with the data supplied as arguments.
 * <br><br>
 * @param re
 * real component.
 */
	complex_number(double re);
/**
 * C++ initialize constructor requests memory for this object and populates it with the data supplied as arguments.
 * <br><br>
 * @param re
 * real component.
 * @param im
 * imaginary component.
 */
	complex_number(double re,double im);
/**
 * Changes the real and imaginary components in this instance of a complex number.
 * <br><br>
 * @param re
 * real component.
 * @param im
 * imaginary component.
 */
	void set(double re,double im);

/**
 * Returns true if this complex number is a pure real.
 */
	bool pure_real() const;

/**
 * Returns true if this complex number is pure imaginary.
 */
	bool pure_imaginary() const;

/**
 * This performs complex number addition.
 * <br><br>
 * @param c
 * second number.
 */
	complex_number operator+(complex_number c);
/**
 * This performs complex number subtraction.
 * <br><br>
 * @param c
 * second number.
 */
	complex_number operator-(complex_number c);
/**
 * This performs complex number multiplication.
 * <br><br>
 * @param c
 * second number.
 */
	complex_number operator*(complex_number c);
/**
 * This performs complex number division.
 * <br><br>
 * @param c
 * second number.
 */
	complex_number operator/(complex_number c);
/**
 * Returns the Euclidian distance of the complex number to the origin, also represented by the square root
 * of the summation of the real portion squared and the imaginary portion squared. See also @href complex_number#angle method.
 */
	double radius() const;
/**
 * Returns a number that represents the polar coordinate angle in radians.
 * <br><br>
 * <b>Role:</b> The angle is counter clockwise from the real axis to the ray formed by the origin and the
 * complex number. See also @href complex_number#radius method.
 */
	double angle() const;
/**
 * Returns the absolute value of the coordinate whose absolute value is maximum.
 * <br><br>
 * <b>Role:</b> This is its maximum metric distance from the origin.
 */
	double max_coord() const;
/**
 * Returns an array of complex numbers which represent its nth's roots.
 * <br><br>
 * @param n
 * which root to find.
 */
	complex_number *root(int n) const;
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

DECL_BASE complex_number *quadratic_roots(double c0,
								double c1,
								double c2);

/**
 * @nodoc
 */
DECL_BASE complex_number *cubic_roots(double c0,
							double c1,
							double c2,
							double c3);

/**
 * @nodoc
 */
DECL_BASE complex_number *quartic_roots(double c0,
							  double c1,
							  double c2,
							  double c3,
							  double c4);


/** @} */
/**
 * \addtogroup ACISGEOMETRICATOMS
 *
 * @{
 */

class SPAvector;
class SPApar_pos;
class SPAposition;

/**
 * Implements an n dimensional vector.
 */
class DECL_BASE SPAnvector : public ACIS_OBJECT
{
public:
/**
 * Array of values associated with the n dimensional vector.
 */
	double *values;
/**
 * Number of items, n, in array of values associated with the n dimensional vector.
 */
	int    size;
/**
 * C++ constructor, creating a <tt>SPAnvector</tt>.
 */
	SPAnvector();
/**
 * This should not be called directly. Use remove instead.
 */
	~SPAnvector();
/**
 * C++ copy constructor, creating a <tt>SPAnvector</tt>.
 * <br><br>
 * <b>Role:</b> The array is whatever size the input <tt>SPAnvector</tt> is.
 * <br><br>
 * @param vec
 * input vector.
 */
	SPAnvector(const SPAnvector & vec);
/**
 * C++ constructor, creating a <tt>SPAnvector</tt> - The <tt>SPAnvector</tt> array is size 3.
 * <br><br>
 * @param vec
 * input vector.
 */
	SPAnvector(SPAvector vec);
/**
 * C++ constructor, creating a <tt>SPAnvector</tt> - The <tt>SPAnvector</tt> array is size 3.
 * <br><br>
 * @param pos
 * input position.
 */
	SPAnvector(SPAposition pos);
/**
 * C++ constructor, creating a <tt>SPAnvector</tt> - The <tt>SPAnvector</tt> array is size 2.
 * <br><br>
 * @param parpos
 * input parameter position.
 */
	SPAnvector(SPApar_pos parpos);
/**
 * C++ constructor, creating a <tt>SPAnvector</tt> - The <tt>SPAnvector</tt> array is size 1.
 * <br><br>
 * @param inp
 * input double.
 */
	SPAnvector(double inp);
/**
 * C++ constructor, creating a <tt>SPAnvector</tt> - The <tt>SPAnvector</tt> array is size <tt>in_size</tt>.
 * <br><br>
 * @param in_values.
 * input array.
 * @param in_size
 * size of input array.
 */
	SPAnvector(double const*in_values,int in_size);
/**
 * Adds the given <tt>SPAnvector</tt> to this <tt>SPAnvector</tt>.
 * <br><br>
 * <b>Role:</b> It fills in the <tt>SPAnvector</tt> if they are not the same size. The respective elements are added with one another.
 * <br><br>
 * @param nv
 * given vector.
 */
	SPAnvector operator+(SPAnvector nv) const;
/**
 * Subtracts the given <tt>SPAnvector</tt> to this <tt>SPAnvector</tt>.
 * <br><br>
 * <b>Role:</b> It fills in the <tt>SPAnvector</tt> if they are not the same size. The respective elements are subtracted from one another.
 * <br><br>
 * @param nv
 * given vector.
 */
	SPAnvector operator-(SPAnvector nv) const;
/**
 * Concatenates the given <tt>SPAnvector</tt> to this <tt>SPAnvector</tt>.
 * <br><br>
 * <b>Role:</b> It fills in the <tt>SPAnvector</tt> if they are is not the right size. The respective elements are multiplied with one another.
 * <br><br>
 * @param nv
 * given vector.
 */
	double  operator*(SPAnvector nv) const;
/**
 * Assigns the given <tt>SPAnvector</tt> values to the current vector.
 * <br><br>
 * <b>Role:</b> It fills in the <tt>SPAnvector</tt> if they are not the same size.
 * <br><br>
 * @param nv
 * given vector.
 */
	SPAnvector operator=(const SPAnvector & nv);
/**
 * Calculates the length of the <tt>SPAnvector</tt> by taking the square root of the sum of the square of each element in the array.
 */
	double length() const;
/**
 * Finds the maximum value in the array.
 */
	double max_coord() const;
/**
 * Changes the size of <tt>SPAnvector</tt> to be the given size.
 * <br><br>
 * @param in_size
 * given size.
 */
	void set(int in_size);
/**
 * Returns a unit length <tt>SPAnvector</tt> that points in the same direction as this.
 */
	SPAnvector norm() const;
/**
 * Returns a <tt>SPAnvector</tt> vector which is this  scaled by the given amount. Note that
 * scale is a const method.
 * <br><br>
 * @param s
 * amount to scale.
 */
	SPAnvector scale(double s) const;
};



/** @} */
#endif

