/*******************************************************************/
/*    Copyright (c) 1989-2020 by Spatial Corp.                     */
/*    All rights reserved.                                         */
/*    Protected by U.S. Patents 5,257,205; 5,351,196; 6,369,815;   */
/*                              5,982,378; 6,462,738; 6,941,251    */
/*    Protected by European Patents 0503642; 69220263.3            */
/*    Protected by Hong Kong Patent 1008101A                       */
/*******************************************************************/
// Header for SPAinterval.
// This class records an interval on the real line.  It is implemented
// as an ordered pair of reals, together with a flag that
// indicates whether each end is bounded or not. This allows the
// representation and manipulation of infinite and semi-infinite
// intervals. The boundary value at an unbounded end is irrelevant.
#if !defined( INTERVAL_CLASS )
#define INTERVAL_CLASS
#include "base.hxx"
#include "dcl_base.h"
#include "logical.h"
#include "debugmsc.hxx"
/**
 * @file interval.hxx
 * @CAA2Level L1
 * @CAA2Usage U1
 * \addtogroup ACISGEOMOPERATORS
 *
 * @{
 */
class SPAinterval;
class SPAunit_vector;
class SPAbox;
/**
 * Negates an interval.
 * @param inter
 * given interval.
 */
DECL_BASE SPAinterval operator-( SPAinterval const & inter);
DECL_BASE SPAinterval gme_operator_sub( SPAinterval const & inter);
/**
 * Adds two intervals together.
 * @param inter1
 * first interval.
 * @param inter2
 * second interval.
 */
DECL_BASE SPAinterval operator+( SPAinterval const & inter1, SPAinterval const & inter2);
DECL_BASE SPAinterval gme_operator_add( SPAinterval const & inter1, SPAinterval const & inter2);
/**
 * Adds an interval and a double.
 * @param i
 * given interval.
 * @param d
 * given double.
 */
DECL_BASE SPAinterval operator+( SPAinterval const &i, double d );
/**
 * Adds a double and an interval.
 * @param d
 * given double.
 * @param i
 * given interval.
 */
DECL_BASE SPAinterval operator+( double d, SPAinterval const &i );

/**
 * Subtracts two intervals.
 * @param i1
 * first interval.
 * @param i2
 * second interval.
 */
DECL_BASE SPAinterval operator-( SPAinterval const &i1, SPAinterval const & i2);
DECL_BASE SPAinterval gme_operator_sub( SPAinterval const &i1, SPAinterval const & i2);
/**
 * Subtracts a double from an interval.
 * @param i
 * given interval.
 * @param d
 * given double.
 */
DECL_BASE SPAinterval operator-( SPAinterval const &i, double d );

/**
 * Subtracts an interval from a double.
 * @param d
 * given double.
 * @param i
 * given interval.
 */
DECL_BASE SPAinterval operator-( double d, SPAinterval const &i );

/**
 * Multiplies an interval by a scalar.
 * @param i
 * given interval.
 * @param d
 * given scalar value.
 */
DECL_BASE SPAinterval operator*( SPAinterval const & i, double d);
DECL_BASE SPAinterval gme_operator_multiply( SPAinterval const & i, double d);
/**
 * Multiplies a scalar by an interval.
 * @param d
 * given scalar value.
 * @param i
 * given interval.
 */
DECL_BASE SPAinterval operator*( double d, SPAinterval const &i );
DECL_BASE SPAinterval gme_operator_multiply( double d, SPAinterval const &i );
/**
 * Divides an interval by a scalar.
 * @param i
 * given interval.
 * @param d
 * given scalar value.
 */
DECL_BASE SPAinterval operator/( SPAinterval const & i, double d );
DECL_BASE SPAinterval gme_operator_divide( SPAinterval const & i, double d );
/**
 * Arithmetic comparison.
 * <b>Role:</b> The meaning in each case is that every number in the first interval bears the given
 * relationship (less than) with every number in the second, where a double is treated as an interval
 * containing just that number.
 * @param i
 * given interval.
 * @param d
 * given double.
 */
DECL_BASE logical operator<( SPAinterval const &i, double d );

/**
 * Arithmetic comparison.
 * <b>Role:</b> The meaning in each case is that every number in the first interval bears the given
 * relationship (less than) with every number in the second, where a double is treated as an interval
 * containing just that number.
 * @param d
 * given double.
 * @param i
 * given interval.
 */
DECL_BASE logical operator<( double d, SPAinterval const &i );

/**
 * Arithmetic comparison.
 * <b>Role:</b> The meaning in each case is that every number in the first interval bears the given
 * relationship (less than) with every number in the second, where a double is treated as an interval
 * containing just that number.
 * @param i1
 * first interval.
 * @param i2
 * second interval.
 */
DECL_BASE logical operator<( SPAinterval const &i1, SPAinterval const &i2 );

/**
 * Arithmetic comparison.
 * <b>Role:</b> The meaning in each case is that every number in the first interval bears the given
 * relationship (greater than) with every number in the second, where a double is treated as an interval
 * containing just that number.
 * @param i
 * given interval.
 * @param d
 * given double.
 */
DECL_BASE logical operator>( SPAinterval const &i, double d );

/**
 * Arithmetic comparison.
 * <b>Role:</b> The meaning in each case is that every number in the first interval bears the given
 * relationship (greater than) with every number in the second, where a double is treated as an interval
 * containing just that number.
 * @param d
 * given double.
 * @param i
 * given interval.
 */
DECL_BASE logical operator>( double d, SPAinterval const &i );

/**
 * Arithmetic comparison.
 * <b>Role:</b> The meaning in each case is that every number in the first interval bears the given
 * relationship (greater than) with every number in the second, where a double is treated as an interval
 * containing just that number.
 * @param i1
 * first interval.
 * @param i2
 * second interval.
 */
DECL_BASE logical operator>( SPAinterval const &i1, SPAinterval const &i2 );

/**
 * Arithmetic comparison.
 * <b>Role:</b> The meaning in each case is that every number in the first interval bears the given
 * relationship (less than or equal to) with every number in the second, where a double is treated as an interval
 * containing just that number.
 * @param i
 * given interval.
 * @param d
 * given double.
 */
DECL_BASE logical operator<=( SPAinterval const &i, double d );

/**
 * Arithmetic comparison.
 * <b>Role:</b> The meaning in each case is that every number in the first interval bears the given
 * relationship (less than or equal to) with every number in the second, where a double is treated as an interval
 * containing just that number.
 * @param d
 * given double.
 * @param i
 * given interval.
 */
DECL_BASE logical operator<=( double d, SPAinterval const &i );

/**
 * Arithmetic comparison.
 * <b>Role:</b> The meaning in each case is that every number in the first interval bears the given
 * relationship (less than or equal to) with every number in the second, where a double is treated as an interval
 * containing just that number.
 * @param i1
 * first interval.
 * @param i2
 * second interval.
 */
DECL_BASE logical operator<=( SPAinterval const &i1, SPAinterval const &i2 );

/**
 * Arithmetic comparison.
 * <b>Role:</b> The meaning in each case is that every number in the first interval bears the given
 * relationship (greater than or equal to) with every number in the second, where a double is treated as an interval
 * containing just that number.
 * @param i
 * given interval.
 * @param d
 * given double.
 */
DECL_BASE logical operator>=( SPAinterval const &i, double d );

/**
 * Arithmetic comparison.
 * <b>Role:</b> The meaning in each case is that every number in the first interval bears the given
 * relationship (greater than or equal to) with every number in the second, where a double is treated as an interval
 * containing just that number.
 * @param d
 * given double.
 * @param i
 * given interval.
 */
DECL_BASE logical operator>=( double d, SPAinterval const &i );

/**
 * Arithmetic comparison.
 * <b>Role:</b> The meaning in each case is that every number in the first interval bears the given
 * relationship (greater than or equal to) with every number in the second, where a double is treated as an interval
 * containing just that number.
 * @param i1
 * first interval.
 * @param i2
 * second interval.
 */
DECL_BASE logical operator>=( SPAinterval const &i1, SPAinterval const &i2 );

/**
 * Equality operator for determining whether two intervals are identical.
 * <b>Role:</b> The criteria are strict, so this operator should not be used when arithmetic equality is intended.
 * @param i1
 * first interval.
 * @param i2
 * second interval.
 */
DECL_BASE logical operator==( SPAinterval const &i1, SPAinterval const & i2);
DECL_BASE logical gme_operator_equal( SPAinterval const &i1, SPAinterval const & i2);
/**
 * Inequality operator for determining whether two intervals are not identical.
 * <b>Role:</b> The criteria are strict, so this operator should not be used when arithmetic inequality is intended.
 * @param i1
 * first interval.
 * @param i2
 * second interval.
 */
DECL_BASE logical operator!=( SPAinterval const &i1, SPAinterval const &i2 );
DECL_BASE logical gme_operator_not_equal(SPAinterval const& i1, SPAinterval const& i2);

/**
 * Constructs an interval containing two intervals.
 * @param i1
 * first interval.
 * @param i2
 * second interval.
 */
DECL_BASE SPAinterval operator|( SPAinterval const &i1, SPAinterval const & i2);
DECL_BASE SPAinterval gme_operator_or( SPAinterval const &i1, SPAinterval const & i2);
/**
 * Finds the interval of overlap.
 * @param i1
 * first interval.
 * @param i2
 * second interval.
 */
DECL_BASE SPAinterval operator&( SPAinterval const &i1, SPAinterval const &i2 );
DECL_BASE SPAinterval gme_operator_and( SPAinterval const &i1, SPAinterval const &i2 );
/**
 * Determines whether two intervals overlap.
 * @param i1
 * first interval.
 * @param i2
 * second interval.
 */
DECL_BASE logical operator&&( SPAinterval const &i1, SPAinterval const & i2);
DECL_BASE logical gme_operator_and_and( SPAinterval const &i1, SPAinterval const & i2);
/**
 * Determines whether the given parameter value lies within the given interval.
 * @param r
 * given double.
 * @param i
 * given interval.
 */
DECL_BASE logical operator<<( double r, SPAinterval const &i );
DECL_BASE logical gme_operator_left_shift(double r, SPAinterval const& i);

// STI let (10/98): Added a new friend function
// Maps a parameter value into the principle range of a periodic
// function by repetitively adding (or subtracting) the period
// to (or from) the parameter value until it falls within the
// prescribed interval.
/**
 * @nodoc
 */
DECL_BASE void map_param_into_interval( SPAinterval const& i, double& d);
DECL_BASE void gme_map_param_into_interval( SPAinterval const& i, double& d);
/**
 * Returns an interval increased by <tt>delta</tt> on both ends; the midpoint is unchanged.
 * (Does not modify the given <tt>SPAinterval</tt>.)
 * @param inter
 * given interval
 * @param delta
 * delta value
 */
DECL_BASE SPAinterval enlarge_interval(SPAinterval const& inter, double delta);
DECL_BASE SPAinterval gme_enlarge_interval(SPAinterval const& inter, double delta);
/** @} */
/**
 * \addtogroup ACISGEOMETRICATOMS
 * @{
 */

/**
 * These types of SPAinterval's can be created.
 * <b>Role:</b> Used by <tt>SPAinterval</tt>.
 * @param interval_unknown
 * unknown interval type.
 * @param interval_finite
 * the interval is bounded at both the low and high parameter values.
 * @param interval_finite_below
 * the interval is bounded at the low parameter value only.
 * @param interval_finite_above
 * the interval is bounded at the high parameter value only.
 * @param interval_infinite
 * the interval is unbounded.
 * @see SPAinterval
 */
enum interval_type {
	interval_unknown,
	interval_finite,
	interval_finite_below,
	interval_finite_above,
	interval_infinite
};

/**
 * Records an interval on the real line.
 * <b>Role:</b> This class records an interval on the real line, i.e., a one 
 * dimensional region. It is implemented as an ordered pair of reals, together 
 * with a flag, an @href interval_type, that indicates whether each end is bounded or not. 
 * This allows the representation and manipulation of finite, infinite, semi-infinite, and 
 * empty intervals. The boundary value at an unbounded end is irrelevant.
 * <br><br>
 * Empty intervals are represented by a <tt>SPAinterval</tt> with a type of <tt>interval_finite</tt> 
 * and a low parameter of 1 and a high parameter of 0.  Infinite intervals are represented 
 * by a <tt>SPAinterval</tt> with a type of <tt>interval_infinite</tt> and a low parameter of 1 and 
 * a high parameter of 0. Semi-infinite intervals are represented by a <tt>SPAinterval</tt> 
 * with a type of either <tt>interval_finite_below</tt> or <tt>interval_finite_above</tt>;
 * the parameter value at the unbounded end is undefined, although often it will have the same 
 * value as the bounded end.
 * <br><br>
 * Intervals are typically used to record the parameter range of a variable, which is often the
 * domain of a curve. Two intervals may be combined in a <tt>SPApar_box</tt>, which may be used 
 * to record the parameter range of a surface. Three intervals may be combined in a <tt>SPAbox</tt>, 
 * which may be used to record the three-dimensional bounding box of a topological entity.
 * <br>
 * @see SPApar_box, SPAbox
 */
class DECL_BASE SPAinterval : public ACIS_OBJECT {

	double low;
	double high;

	interval_type type_data;

	// Special constructor to bypass order checking.

	SPAinterval( double l, double h, interval_type t ) {
		low = l;
		high = h;
		type_data = t;
	}

	friend DECL_BASE SPAinterval operator-( SPAinterval const & i );
	friend DECL_BASE SPAinterval gme_operator_sub( SPAinterval const & i );
	friend DECL_BASE SPAinterval operator+( SPAinterval const & i1, SPAinterval const & i2 );
	friend DECL_BASE SPAinterval gme_operator_add( SPAinterval const & i1, SPAinterval const & i2 );
	friend DECL_BASE SPAinterval operator+( SPAinterval const & i, double d );
	friend DECL_BASE SPAinterval operator+( double d, SPAinterval const &i );
	friend DECL_BASE SPAinterval operator-( SPAinterval const & i1, SPAinterval const & i2 );
	friend DECL_BASE SPAinterval gme_operator_sub( SPAinterval const & i1, SPAinterval const & i2 );
	friend DECL_BASE SPAinterval operator-( SPAinterval const & i, double d );
	friend DECL_BASE SPAinterval operator-( double d, SPAinterval const & i );
	friend DECL_BASE SPAinterval operator*( SPAinterval const & i, double d );
	friend DECL_BASE SPAinterval gme_operator_multiply( SPAinterval const & i, double d );
	friend DECL_BASE SPAinterval operator*( double d, SPAinterval const & i );
	friend DECL_BASE SPAinterval gme_operator_multiply( double d, SPAinterval const & i );
	friend DECL_BASE SPAinterval operator/( SPAinterval const & i, double d );
	friend DECL_BASE SPAinterval gme_operator_divide( SPAinterval const & i, double d );
	friend DECL_BASE logical operator<( SPAinterval const &i, double d );
	friend DECL_BASE logical operator<( double d, SPAinterval const &i );
	friend DECL_BASE logical operator<( SPAinterval const &i1, SPAinterval const &i2 );
	friend DECL_BASE logical operator>( SPAinterval const &i, double d );
	friend DECL_BASE logical operator>( double d, SPAinterval const &i );
	friend DECL_BASE logical operator>( SPAinterval const &i1, SPAinterval const &i2 );
	friend DECL_BASE logical operator<=( SPAinterval const &i, double d );
	friend DECL_BASE logical operator<=( double d, SPAinterval const &i );
	friend DECL_BASE logical operator<=( SPAinterval const &i1, SPAinterval const &i2 );
	friend DECL_BASE logical operator>=( SPAinterval const &i, double d );
	friend DECL_BASE logical operator>=( double d, SPAinterval const &i );
	friend DECL_BASE logical operator>=( SPAinterval const &i1, SPAinterval const &i2 );
	friend DECL_BASE logical operator==( SPAinterval const &i1, SPAinterval const &i2 );
	friend DECL_BASE logical gme_operator_equal( SPAinterval const &i1, SPAinterval const &i2 );
	friend DECL_BASE logical operator!=( SPAinterval const &i1, SPAinterval const &i2 );
	friend DECL_BASE logical gme_operator_not_equal(SPAinterval const& i1, SPAinterval const& i2);
	friend DECL_BASE SPAinterval operator|( SPAinterval const &i1, SPAinterval const &i2 );
	friend DECL_BASE SPAinterval gme_operator_or( SPAinterval const &i1, SPAinterval const &i2 );
	friend DECL_BASE logical operator&&( SPAinterval const &i1, SPAinterval const &i2 );
	friend DECL_BASE logical gme_operator_and_and(SPAinterval const& i1, SPAinterval const& i2);
	friend DECL_BASE logical operator<<( double r, SPAinterval const & i );
	friend DECL_BASE logical gme_operator_left_shift(double r, SPAinterval const& i);
	friend DECL_BASE SPAinterval operator&( SPAinterval const &i1, SPAinterval const &i2 );
	friend DECL_BASE SPAinterval gme_operator_and(SPAinterval const& i1, SPAinterval const& i2);
	friend DECL_BASE void map_param_into_interval( SPAinterval const&i, double&d );
	friend DECL_BASE void gme_map_param_into_interval(SPAinterval const& i, double& d);

public:

	/**
	 * C++ allocation constructor requests memory for this object and constructs an empty <tt>SPAinterval</tt>.
	 */
	SPAinterval() {
		// Construct an empty interval.
		low = 1;
		high = 0;
		type_data = interval_finite;
	}

	// Construct a zero-length SPAinterval from one real value.
	/**
	 * C++ initialize constructor requests memory for this object and populates it with the data supplied as arguments.
	 * <b>Role:</b> Constructs a zero-length interval from one parameter value.
	 * @param d
	 * given parameter value.
	 */
	SPAinterval( double d ) {
		low = d;
		high = d;
		type_data = interval_finite;
	}

	// Construct an SPAinterval from two real values.
	/**
	 * C++ initialize constructor requests memory for this object and populates it with the data supplied as arguments.
	 * <b>Role:</b> Constructs a finite interval from two values. The arguments do not need to be in ascending sequence. 
	 * The constructor checks and adjusts the argument sequence.
	 * @param d1
	 * first parameter value.
	 * @param d2
	 * second parameter value.
	 */
	SPAinterval( double d1, double d2);
	SPAinterval( const char* gme,double d1, double d2);
	// Construct a bounded or unbounded SPAinterval from a descriptor
	// and up to two values. If the SPAinterval type is "unknown", then the
	// type is taken from the existence or otherwise of the double arguments:
	// absence means infinite at the appropriate end. Otherwise the type
	// is as given, and any required bound is taken from the arguments -
	// if there are two present then they are low/high, if there is only one
	// it is used for both ends, and if there is none, then the SPAinterval is
	// set up empty (1,0).
	/**
	 * C++ initialize constructor requests memory for this object and populates it with the data supplied as arguments.
	 * <b>Role:</b> Constructs a bounded or unbounded interval from a descripter and up to two values. 
	 * If the given @href interval_type is <tt>interval_unknown</tt>, the type is derived from the 
	 * presence or absence of the real arguments ("absence" means unbounded at the appropriate end). 
	 * Otherwise, the type is as given. Any required bound is taken from the arguments. If there are two 
	 * real arguments, they are low/high. If there is only one real argument, it is used for both ends.
	 * @param type
	 * given interval type.
	 * @param d1
	 * lower parameter value.
	 * @param d2
	 * higher parameter value.
	 */
	SPAinterval(
			interval_type type,
			double const & d1 = *(double *)NULL_REF,
			double const & d2 = *(double *)NULL_REF
		);
		SPAinterval(const char* gme,
			interval_type type,
			double const & d1 = *(double *)NULL_REF,
			double const & d2 = *(double *)NULL_REF
		);

	// Simple inquiries.
	/**
	 * Returns the type of this interval.
	 */
	interval_type type() const { return type_data ; }

	/**
	 * Determines if this interval is finite; that is, bounded on both ends.
	 */
	logical finite() const { return type_data == interval_finite; }
	/**
	 * Determines if this interval is finite below; that is, bounded at the lower parameter only.
	 */
	logical finite_below() const { return type_data == interval_finite_below; }
	/**
	 * Determines if this interval is finite above; that is, bounded at the higher parameter only.
	 */
	logical finite_above() const { return type_data == interval_finite_above; }
	/**
	 * Determines if this interval is infinite; that is, unbounded on both ends.
	 */
	logical infinite() const { return type_data == interval_infinite; }
	/**
	 * Determines if this interval is bounded above and below.
	 */
	logical bounded() const { return finite(); }
	/**
	 * Determines if this interval is bounded below, regardless of whether it is bounded above or not.
	 */
	logical bounded_below() const { return finite() || finite_below(); }
	/**
	 * Determines if this interval is bounded above, regardless of whether it is bounded below or not.
	 */
	logical bounded_above() const { return finite() || finite_above(); }
	/**
	 * Determines if this interval is unbounded; that is, it is either infinite or semi-infinite.
	 */
	logical unbounded() const { return !bounded(); }
	/**
	 * Determines if this interval is unbounded below, regardless of whether it is bounded above or not.
	 */
	logical unbounded_below() const { return infinite() || finite_above(); }
	/**
	 * Determines if this interval is unbounded above, regardless of whether it is bounded below or not.
	 */
	logical unbounded_above() const { return infinite() || finite_below(); }
	/**
	 * Determines if this interval is empty.
	 */
	logical empty() const { return bounded() && low > high; }
	/**
	 * Determines if this interval is a scalar; that is, if it consists of a single point.
	 */
	logical scalar() const { return bounded() && low == high; }

	// Return useful information. Only meaningful if the relevant
	// ends are bounded, but no error if not.
	/**
	 * Returns the start point of this interval.
	 * <b>Role:</b> This method is meaningful only if the lower end is bounded. 
	 * If the lower end is not bounded, there is no error.
	 */
	double start_pt() const
		{ return low; }

	/**
	 * Returns the end point of this interval.
	 * <b>Role:</b> This method is meaningful only if the upper end is bounded. 
	 * If the upper end is not bounded, there is no error.
	 */
	double end_pt() const
		{ return high; }

	/**
	 * Returns the middle point of this interval.
	 * <b>Role:</b> This method is only meaningful if both ends are bounded.
	 * If the relevant ends are not bounded, there is no error.
	 */
	double mid_pt() const;
	double gme_mid_pt() const;
	// Returns (1 - param) * low-end + param * high-end for given
	// param. Meaningful only if the relevant ends are bounded, but
	// no error if not.
	/**
	 * Interpolates within this interval.
	 * <b>Role:</b> This method returns:
	 * <br><br> 
	 * <tt>(1 - parameter) * low-end + parameter * high-end</tt>
	 * <br><br>
	 * for the given parameter. This method is meaningful only if both ends are bounded.
	 * If the ends are not bounded, there is no error.
	 * @param d
	 * given parameter value.
	 */
	double interpolate( double d ) const;
	double gme_interpolate( double d ) const;
	// Return the difference between the high and low ends. By
	// historical convention, both "empty" and "infinite" have
	// returned negative values (since formerly an infinite SPAparameter
	// range was returned as "empty"), so this is retained. For
	// convenient distinction, "empty" returns exactly -1.0, and
	// "infinite" (or semi-infinite) returns exactly -2.0.
	/**
	 * Returns the difference between the high and low ends of the interval.
	 * <b>Role:</b> By historical convention, both empty and infinite intervals return negative values 
	 * (since formerly an infinite parameter range returned an empty interval), so this is retained. 
	 * For convenient distinction, an empty interval returns exactly -1.0, and an infinite 
	 * (or semi-infinite) interval returns exactly -2.0.
	 */
	double length() const;
	double gme_length() const;
	/**
	 * Negates this interval in place.
	 */
	SPAinterval &negate();
	SPAinterval &gme_negate();
	/**
	 * Adds an interval to this interval.
	 * @param inter
	 * other interval.
	 */
	SPAinterval &operator+=( SPAinterval const & inter);
	SPAinterval &gme_operator_add_assgin( SPAinterval const & inter);
	/**
	 * Adds a scalar value to both ends of this interval, shifting this interval by the value.
	 * @param d
	 * shift value.
	 */
	SPAinterval &operator+=( double d ) {
		low += d;
		high += d;
		return *this;
	}

	/**
	 * Subtracts an interval from this interval.
	 * @param inter
	 * other interval.
	 */
	SPAinterval &operator-=( SPAinterval const & inter);
	SPAinterval &gme_operator_sub_assgin( SPAinterval const & inter);
	/**
	 * Subtracts a scalar value from both ends of this interval, shifting this interval by the value.
	 * @param d
	 * shift value.
	 */
	SPAinterval &operator-=( double d ) {
		low -= d;
		high -= d;
		return *this;
	}

	/**
	 * Multiplies this interval by a scalar.
	 * @param d
	 * the given scalar value.
	 */
	SPAinterval &operator*=( double d );
	SPAinterval &gme_operator_multiply_assgin( double d );
	/**
	 * Divides this interval by a scalar.
	 * @param d
	 * the given scalar value.
	 */
	SPAinterval &operator/=( double d);
	SPAinterval &gme_operator_divide_assgin( double d);
	/**
	 * Updates this interval to include the given interval.
	 * @param i
	 * given interval
	 */
	SPAinterval &operator|=( SPAinterval const & i);
	SPAinterval &gme_operator_or_assgin( SPAinterval const & i);
	/**
	 * Updates this interval to be the overlap of itself and the given interval.
	 * @param i
	 * given interval
	 */
	SPAinterval &operator&=( SPAinterval const & i);
	SPAinterval &gme_operator_and_assgin( SPAinterval const & i);
	/**
	 * Determines whether this interval is entirely enclosed within the given interval.
	 * <b>Role:</b> The given method returns <tt>FALSE</tt> if this interval is <tt>NULL</tt>, <tt>TRUE</tt> 
	 * if the given interval is <tt>NULL</tt>; otherwise, it returns <tt>TRUE</tt> if low end of this 
	 * interval exceeds low end of the given interval (less <tt>SPAresabs</tt>) and high end of this 
	 * interval is less than high end of the given interval (plus <tt>SPAresabs</tt>).
	 * @param i
	 * given interval.
	 */
	logical operator<<( SPAinterval const &i ) const
		{ return i >> *this; }
	logical gme_operator_left_shift(SPAinterval const& i) const;

	// Determine whether a point lies within an SPAinterval.
	// Returns true if this interval is null or if given value
	// lies within this interval (expanded by SPAresabs at each end).
	/**
	 * Determines whether a parameter value lies within this interval.
	 * <b>Role:</b> This method returns <tt>TRUE</tt> if this interval is <tt>NULL</tt> or if the given 
	 * parameter value lies within this interval (expanded by <tt>SPAresabs</tt>
	 * at each end).
	 * @param d
	 * given parameter value.
	 */
	logical operator>>( double d) const;
	logical gme_operator_right_shift( double d) const;

	// Determine whether an interval is entirely enclosed within
	// another.  Returns true if this interval is null, false if
	// given interval is null, otherwise true if low end of given
	// interval exceeds low end of this interval (less SPAresabs) and
	// high end of given interval is less than high end of this
	// interval (plus SPAresabs).
	/**
	 * Determines whether the given interval is entirely enclosed within this interval.
	 * <b>Role:</b> This method returns <tt>TRUE</tt> if this interval is </tt>NULL</tt>, 
	 * <tt>FALSE</tt> if the given interval is <tt>NULL</tt>; otherwise, it returns <tt>TRUE</tt> 
	 * if low end of given interval exceeds low end of this interval (less <tt>SPAresabs</tt>) 
	 * and high end of the given interval is less than high end of this interval (plus <tt>SPAresabs</tt>).
	 * @param inter
	 * given interval.
	 */
	logical operator>>( SPAinterval const & inter) const;
	logical gme_operator_right_shift( SPAinterval const & inter) const;
	/**
	 * Outputs the details of an interval to the specified file.
	 * @param fp
	 * file pointer.
	 */
	void debug( FILE * fp = debug_file_ptr ) const;
	void gme_debug( FILE * fp = debug_file_ptr ) const;
	/**
	 * Concatenates the details of this interval to the given string.
	 * @param str1
	 * given string.
	 */
	void debug_str(char * str1) const;
	void gme_debug_str(char * str1) const;
};

/** @} */

/**
 * \addtogroup ACISGEOMOPERATORS
 * @{
 */

/**
 * @nodoc
 */
inline SPAinterval operator+( SPAinterval const &i, double d )
	{ return SPAinterval( i.low + d, i.high + d, i.type_data ); }
/**
 * @nodoc
 */
inline SPAinterval operator+( double d, SPAinterval const &i )
	{ return SPAinterval( d + i.low, d + i.high, i.type_data ); }
/**
 * @nodoc
 */
inline SPAinterval operator-( SPAinterval const &i, double d )
	{ return SPAinterval( i.low - d, i.high - d, i.type_data ); }
/**
 * @nodoc
 */
inline SPAinterval operator-( double d, SPAinterval const &i )
	{ return SPAinterval( d - i.high, d - i.low, i.type_data ); }
/**
 * @nodoc
 */
inline logical operator<( SPAinterval const &i, double d )
	{ return i.bounded_above() && i.high < d; }
/**
 * @nodoc
 */
inline logical operator<( double d, SPAinterval const &i )
	{ return i.bounded_below() && d < i.low; }
/**
 * @nodoc
 */
inline logical operator<( SPAinterval const &i1, SPAinterval const &i2 )
	{ return i1.bounded_above() && i2.bounded_below() && i1.high < i2.low; }
/**
 * @nodoc
 */
inline logical operator>( SPAinterval const &i, double d )
	{ return i.bounded_below() && i.low > d; }
/**
 * @nodoc
 */
inline logical operator>( double d, SPAinterval const &i )
	{ return i.bounded_above() && d > i.high; }
/**
 * @nodoc
 */
inline logical operator>( SPAinterval const &i1, SPAinterval const &i2 )
	{ return i1.bounded_below() && i2.bounded_above() && i1.low > i2.high; }
/**
 * @nodoc
 */
inline logical operator<=( SPAinterval const &i, double d )
	{ return i.bounded_above() && i.high <= d; }
/**
 * @nodoc
 */
inline logical operator<=( double d, SPAinterval const &i )
	{ return i.bounded_below() && d <= i.low; }
/**
 * @nodoc
 */
inline logical operator<=( SPAinterval const &i1, SPAinterval const &i2 )
	{ return i1.bounded_above() && i2.bounded_below() && i1.high <= i2.low; }
/**
 * @nodoc
 */
inline logical operator>=( SPAinterval const &i, double d )
	{ return i.bounded_below() && i.low >= d; }
/**
 * @nodoc
 */
inline logical operator>=( double d, SPAinterval const &i )
	{ return i.bounded_above() && d >= i.high; }
/**
 * @nodoc
 */
inline logical operator>=( SPAinterval const &i1, SPAinterval const &i2 )
	{ return i1.bounded_below() && i2.bounded_above() && i1.low >= i2.high; }
/**
 * @nodoc
 */
inline logical operator!=( SPAinterval const &i1, SPAinterval const &i2 )
	{ return !(i1 == i2); }
/**
 * @nodoc
 */
inline logical operator<<( double r, SPAinterval const &i )
	{ return i >> r; }


/** @} */
#endif

