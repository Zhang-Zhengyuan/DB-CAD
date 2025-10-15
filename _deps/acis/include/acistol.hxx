/*******************************************************************/
/*    Copyright (c) 1989-2020 by Spatial Corp.                     */
/*    All rights reserved.                                         */
/*    Protected by U.S. Patents 5,257,205; 5,351,196; 6,369,815;   */
/*                              5,982,378; 6,462,738; 6,941,251    */
/*    Protected by European Patents 0503642; 69220263.3            */
/*    Protected by Hong Kong Patent 1008101A                       */
/*******************************************************************/
//    Declare procedures which do comparisons using ACIS tolerances

#ifndef acistol_hxx
#define acistol_hxx
#include "base.hxx"
#include "dcl_base.h"
#include "logical.h"
/**
 * @file acistol.hxx
 * @CAA2Level L1
 * @CAA2Usage U1
 * \addtogroup ACISGEOMOPERATORS
 *
 * @{
 */

class SPAposition;
class SPAvector;
class SPAunit_vector;

//======================================================================

// Test if two things are equal (within SPAresabs)
/**
* Determines if two doubles are equal.
* <br><br>
* <b>Role:</b> This function accepts a pair of doubles as arguments. It returns
* TRUE if the items are equal within <tt>SPAresabs</tt>.
* <br><br>
* <b>Effect:</b> System routine
* <br><br>
* @param n1
* first double.
* @param n2
* second double.
**/
DECL_BASE logical is_equal(double n1,double n2);

DECL_BASE logical gme_is_equal(double n1,double n2);
/**
* Determines if two <tt>SPApositions</tt> are equal.
* <br><br>
* <b>Role:</b> This function accepts a pair of <tt>SPApositions</tt> as arguments. It returns
* TRUE if the items are equal within <tt>SPAresabs</tt>.
* <br><br>
* <i><b>Note:</b> This function is antiquated and should be replaced in applications
* by the appropriate <tt>operator==</tt> member function.</i>
* <br><br>
* <b>Effect:</b> System routine
* <br><br>
* @param p1
* first position.
* @param p2
* second position.
**/
DECL_BASE logical is_equal(const SPAposition& p1, const SPAposition& p2);

DECL_BASE logical gme_is_equal(const SPAposition& p1, const SPAposition& p2);
/**
* Determines if two <tt>SPAvectors</tt> are equal.
* <br><br>
* <b>Role:</b> This function accepts a pair of <tt>SPAvectors</tt> as arguments.
* It returns TRUE if the items are equal within <tt>SPAresabs</tt>.
* <br><br>
* <i><b>Note:</b> This function is antiquated and should be replaced in applications
* by the appropriate <tt>operator==</tt> member function.</i>
* <br><br>
* <b>Effect:</b> System routine
* <br><br>
* @param v1
* first vector.
* @param v2
* second vector.
**/
DECL_BASE logical is_equal(const SPAvector& v1, const SPAvector& v2);
DECL_BASE logical gme_is_equal(const SPAvector& v1, const SPAvector& v2);
/**
* Determines if two <tt>SPAunit_vectors</tt> are equal.
* <br><br>
* <b>Role:</b> This function accepts a pair of <tt>SPAunit_vectors</tt> as arguments.
* It returns TRUE if the items are equal within <tt>SPAresabs</tt>.
* <br><br>
* <i><b>Note:</b> This function is antiquated and should be replaced in applications
* by the appropriate <tt>operator==</tt> member function.</i>
* <br><br>
* <b>Effect:</b> System routine
* <br><br>
* @param v1
* first unit vector.
* @param v2
* second unit vector.
**/
DECL_BASE logical is_equal(const SPAunit_vector& v1, const SPAunit_vector& v2);
DECL_BASE logical gme_is_equal(const SPAunit_vector& v1, const SPAunit_vector& v2);

// Test if two vectors are parallel
/**
* Determines if two unit vectors are parallel within <tt>SPAresnor</tt>.
* <br><br>
* <b>Role:</b> This function accepts two unit vectors as arguments. It returns
* TRUE if the unit vectors are parallel within <tt>SPAresnor</tt>.
* <br><br>
* <i><b>Note:</b> This function is antiquated and should be replaced in applications
* by the appropriate <tt>SPAunit_vector</tt> method declared in <tt>vector.hxx</tt>.</i>
* <br><br>
* <b>Effect:</b> Read-only
* <br><br>
* @param v1
* first unit vector.
* @param v2
* second unit vector.
* @param same_dir
* TRUE means same direction.
**/
DECL_BASE logical are_parallel(const SPAunit_vector& v1, // first SPAvector
                     const SPAunit_vector& v2, // second SPAvector
                     logical same_dir = FALSE     // TRUE means same direction also
                     );

DECL_BASE logical gme_are_parallel(const SPAunit_vector& v1, // first SPAvector
                     const SPAunit_vector& v2, // second SPAvector
                     logical same_dir = FALSE     // TRUE means same direction also
                     );
/**
* Determines if two vectors are parallel within <tt>SPAresnor</tt>.
* <br><br>
* <b>Role:</b> This function accepts two vectors as arguments. It returns
* TRUE if the vectors are parallel within <tt>SPAresnor</tt>.
* <br><br>
* <i><b>Note:</b> This function is antiquated and should be replaced in applications
* by the appropriate <tt>SPAvector</tt> method declared in <tt>vector.hxx</tt>.</i>
* <br><br>
* <b>Effect:</b> Read-only
* <br><br>
* @param v1
* first vector.
* @param v2
* second vector.
* @param same_dir
* TRUE means same direction.
**/
DECL_BASE logical are_parallel(const SPAvector& v1,  // first SPAvector
                     const SPAvector& v2,  // second SPAvector
                     logical same_dir = FALSE // TRUE means same direction also
                     );
DECL_BASE logical gme_are_parallel(const SPAvector& v1,  // first SPAvector
                     const SPAvector& v2,  // second SPAvector
                     logical same_dir = FALSE // TRUE means same direction also
                     );

// Test if two vectors are perpendicular

/**
* Determines if two unit vectors are perpendicular (within <tt>SPAresnor</tt>).
* <br><br>
* <b>Role:</b> This function accepts two unit vectors as arguments. It returns
* TRUE if the unit vectors are perpendicular within <tt>SPAresnor</tt>.
* <br><br>
* <i><b>Note:</b> This function is antiquated and should be replaced in applications
* by the appropriate <tt>SPAunit_vector</tt> method declared in <tt>vector.hxx</tt>.</i>
* <br><br>
* <b>Effect:</b> Read-only
* <br><br>
* @param v1
* first unit vector.
* @param v2
* second unit vector.
**/
DECL_BASE logical are_perpendicular(const SPAunit_vector& v1, // First SPAvector
                          const SPAunit_vector& v2  // second SPAvector
                          );

DECL_BASE logical gme_are_perpendicular(const SPAunit_vector& v1, // First SPAvector
                          const SPAunit_vector& v2  // second SPAvector
                          );
/**
* Determines if two vectors are perpendicular (within <tt>SPAresnor</tt>).
* <br><br>
* <b>Role:</b> This function accepts two vectors as arguments. It returns
* TRUE if the vectors are perpendicular within <tt>SPAresnor</tt>.
* <br><br>
* <i><b>Note:</b> This function is antiquated and should be replaced in applications
* by the appropriate <tt>SPAvector</tt> method declared in <tt>vector.hxx</tt>.</i>
* <br><br>
* <b>Effect:</b> Read-only
* <br><br>
* @param v1
* first vector.
* @param v2
* second vector.
**/

DECL_BASE logical are_perpendicular(const SPAvector& v1, // First SPAvector
                          const SPAvector& v2  // second SPAvector
                          );

DECL_BASE logical gme_are_perpendicular(const SPAvector& v1, // First SPAvector
                          const SPAvector& v2  // second SPAvector
                          );

// Test for zero numbers, positions or vectors.  The tests uses SPAresabs
/**
* Determines if the double value is zero relative to <tt>SPAresabs</tt>.
* <br><br>
* <b>Role:</b> This function accepts a double as input, and returns TRUE if the
* double is zero within <tt>SPAresabs</tt>, FALSE if it is nonzero.
* <br><br>
* <b>Effect:</b> Read-only
* <br><br>
* @param n
* double.
**/
DECL_BASE logical is_zero(double n);

DECL_BASE logical gme_is_zero(double n);
/**
* Determines if a position is zero relative to <tt>SPAresabs</tt>.
* <br><br>
* <b>Role:</b> This function accepts a <tt>SPAposition</tt> as input, and returns TRUE if the
* position is zero within <tt>SPAresabs</tt>, FALSE if it is nonzero.
* <br><br>
* <b>Effect:</b> Read-only
* <br><br>
* @param p
* position.
**/
DECL_BASE logical is_zero(const SPAposition& p);
DECL_BASE logical gme_is_zero(const SPAposition& p);
/**
* Determines if a vector is zero relative to <tt>SPAresabs</tt>.
* <br><br>
* <b>Role:</b> This function accepts a <tt>SPAvector</tt> as input, and returns TRUE if the
* vector is zero within <tt>SPAresabs</tt>, FALSE if it is nonzero.
* <br><br>
* <b>Effect:</b> Read-only
* <br><br>
* @param v
* vector.
**/
DECL_BASE logical is_zero(const SPAvector& v);
DECL_BASE logical gme_is_zero(const SPAvector& v);
/**
* Determines if an unit vector is zero relative to <tt>SPAresabs</tt>.
* <br><br>
* <b>Role:</b> This function accepts a <tt>SPAunit_vector</tt> as input, and returns TRUE if the
* unit vector is zero within <tt>SPAresabs</tt>, FALSE if it is nonzero.
* <br><br>
* <b>Effect:</b> Read-only
* <br><br>
* @param p
* unit vector.
**/
DECL_BASE logical is_zero(const SPAunit_vector& p);
DECL_BASE logical gme_is_zero(const SPAunit_vector& p);
/**
* Test for zero numbers relative to <tt>SPAresabs</tt> squared.
* <br><br>
* <b>Role:</b> This function accepts a double as input, and returns TRUE if it
* is less than <tt>SPAresabs</tt> squared, FALSE if it is greater.
* <br><br>
* It is assumed that the given number is non-negative.
* This method is part of an optimzation scheme.
* Instead of calling distance_to_point in your algorithm,
* you may be able to call distance_to_point_squared.
* You can then test for zero using is_zero_squared, instead of is_zero.
* If so, you saved doing any square root operations!
* <br><br>
* <b>Effect:</b> Read-only
* <br><br>
* @param n
* double.
**/
DECL_BASE logical is_zero_squared(double n);
DECL_BASE logical gme_is_zero_squared(double n);
/**
* Test if a value is zero relative to <tt>SPAresmch</tt>.
* <br><br>
* <b>Role:</b> This function accepts a double as input, and returns TRUE if it
* is less than <tt>SPAresmch</tt>, FALSE if it is greater.
* <br><br>
* <b>Effect:</b> Read-only
* <br><br>
* @param n
* double.
**/
DECL_BASE logical is_zero_mch(double n);

DECL_BASE logical gme_is_zero_mch(double n);
/**
* Test if a value is zero relative to <tt>SPAresnor</tt>.
* <br><br>
* <b>Role:</b> This function accepts a double as input, and returns TRUE if it
* is less than <tt>SPAresnor</tt>, FALSE if it is greater.
* <br><br>
* <b>Effect:</b> Read-only
* <br><br>
* @param n
* double.
**/
DECL_BASE logical is_zero_nor(double n);
DECL_BASE logical gme_is_zero_nor(double n);
/**
* Determines if the double value is positive.
* <br><br>
* <b>Effect:</b> System routine
* <br><br>
* @param n
* double.
**/
DECL_BASE logical is_positive(double n);

DECL_BASE logical gme_is_positive(double n);
/**
* Determines if the double value is negative.
* <br><br>
* <b>Effect:</b> System routine
* <br><br>
* @param n
* double.
**/
DECL_BASE logical is_negative(double n);
DECL_BASE logical gme_is_negative(double n);
/**
* Determines if the first double value is less than the second double value.
* <br><br>
* <b>Effect:</b> System routine
* <br><br>
* @param n1
* first double.
* @param n2
* second double.
**/
DECL_BASE logical is_less_than(double n1, double n2);
DECL_BASE logical gme_is_less_than(double n1, double n2);
/**
* Determines if the first double value is greater than the second double value.
* <br><br>
* <b>Effect:</b> System routine
* <br><br>
* @param n1
* first double.
* @param n2
* second double.
**/
DECL_BASE logical is_greater_than(double n1, double n2);
DECL_BASE logical gme_is_greater_than(double n1, double n2);
/**
* Gets the <tt>SPAresabs</tt> resolution.
* <br><br>
* <b>Effect:</b> Read-only
**/
DECL_BASE double get_resabs();
DECL_BASE double gme_get_resabs();
/**
* Gets the <tt>SPAresnor</tt> resolution.
* <br><br>
* <b>Effect:</b> Read-only
**/
DECL_BASE double get_resnor();
DECL_BASE double gme_get_resnor();
/**
* Gets the <tt>SPAresfit</tt> resolution.
* <br><br>
* <b>Effect:</b> Read-only
**/
DECL_BASE double get_resfit();
DECL_BASE double gme_get_resfit();

/**
* Gets the <tt>resmch</tt> resolution.
* <br><br>
* <b>Effect:</b> Read-only
**/
DECL_BASE double get_resmch();

DECL_BASE double gme_get_resmch();
/** @} */
#endif

