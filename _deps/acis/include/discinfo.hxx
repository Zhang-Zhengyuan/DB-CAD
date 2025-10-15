/*******************************************************************/
/*    Copyright (c) 1989-2020 by Spatial Corp.                     */
/*    All rights reserved.                                         */
/*    Protected by U.S. Patents 5,257,205; 5,351,196; 6,369,815;   */
/*                              5,982,378; 6,462,738; 6,941,251    */
/*    Protected by European Patents 0503642; 69220263.3            */
/*    Protected by Hong Kong Patent 1008101A                       */
/*******************************************************************/
//               Discontinuity Information Class
// This class stores discontinuity information for a curve or surface.
// Only C1,  C2  and  C3 discontinuities  are stored since we are not
// intereseted in C4 discontinuities and above.
#if !defined discontinuity_info_CLASS
#define discontinuity_info_CLASS
#include "acis.hxx"
#include "dcl_kern.h"
#include "logical.h"
class SPAvector;
class SizeAccumulator;
class discontinuity_inserter;
/**
 * @file discinfo.hxx
 * @CAA2Level L1
 * @CAA2Usage U1
 * \addtogroup ACISGEOMETRY
 *
 * @{
 */
/**
 * Enumeration of the different relationships possible between two different <tt>discontinuity_info</tt> instances.
 * @param disc_info_equiv
 * Indicates that the two instances represent the same discontinuities.
 * @param disc_info_includes
 * Indicates that the one instance represents all of the discontinuities
 * represented by the other, and more besides (otherwise they would be
 * equivalent).
 * @param disc_info_lacking
 * Indicates that the one instance does not represent all of the
 * discontinuities represented by the other. Note that given two instances
 * a & b and a test function t that returns a discontinuity_info_rel,
 * t(a,b)==disc_info_includes implies t(b,a)==disc_info_lacking, but
 * t(a,b)==disc_info_lacking does NOT imply t(b,a)==disc_info_includes,
 * as a and b could each have represent discontinuities not represented by
 * the other.
 * @param disc_info_different
 * indicates that the two instances are not comparable. In practice, this
 * will be because they have different periodicities.
 * @see discontinuity_info
 */
enum discontinuity_info_rel {
    disc_info_equiv,
    disc_info_includes,
    disc_info_lacking,
    disc_info_different
};
/**
 * @nodoc
 */
enum disc_info_status {
    disc_info_okay,
    disc_info_supplied_lacking,
    disc_info_calculated_lacking
};
/**
 * @nodoc
 */
enum disc_info_calc_status {
    disc_calc_needed,
    disc_calc_in_progress,
    disc_calc_done
};

#if defined D3_STANDALONE || defined D3_DEBUG
#include "stream.hxx"
#endif
/**
 * Stores discontinuity information for a curve or surface.
 * <br>
 * <b>Role:</b> Used to store parameter values at which a curve or surface
 * has a discontinuity in some derivative.
 * <br><br>
 * This class stores discontinuity information for a curve or surface.
 * Only G1, G2, and G3 discontinuities are stored because we are not
 * interested in higher ordered discontinuities. Externally this class
 * can be viewed as an ordered list containing discontinuities, each of 
 * which contains a parameter value and the order of the discontinuity.
 * @see int_cur, intcurve, spl_sur, spline
 */
 class DECL_KERN discontinuity_info  : public ACIS_OBJECT
{

	friend class discontinuity_inserter;

private:
/**
 * @nodoc
 * Number of C1 discontinuties.
 */
    int		_n_C1_discontinuities;
/**
 * @nodoc
 * Number of C2 discontinuties.
 */
    int		_n_C2_discontinuities;
/**
 * @nodoc
 * Number of C3 discontinuties.
 */
    int		_n_C3_discontinuities;
/**
 * @nodoc
 * Array of reals that contain all discontinuities.
 */
    double*	_discontinuities;
/**
 * @nodoc
 * Array of reals that contain only the C2 discontinuities.
 */
    double* 	_all_C2_discontinuities;
/**
 * @nodoc
 * Array of reals that contain only the C3 discontinuities.
 */
    double* 	_all_C3_discontinuities;
/**
 * @nodoc
 * Period.
 */
    double 	_period;
/**
 * @nodoc
 * Brings a parameter value into the base range of the discontinuity list.
 */
    double	base_param( double t ) const;
/**
 * @nodoc
 * Lowest value in list.
 */
    double	tmin()	const;
/**
 * @nodoc
 * Highest value in list.
 */
    double	tmax()	const;

public:
/**
 * C++ allocation constructor requests memory for this object and initilizes data members.
 * <br><br>
 * <b>Role:</b> The allocation constructor is used primarily by the restore mechanism. 
 */
     discontinuity_info();
/**
 * C++ copy constructor requests memory for this object.
 * <br><br>
 * <b>Role:</b> Allocates memory for the object and populates it with the data from 
 * the object supplied as an argument.
 * <br><br>
 * @param old
 * instance to copy.
 */
     discontinuity_info( const discontinuity_info& old );
/**
 * C++ destructor for <tt>discontinuity_info</tt> which deallocates memory.
 */
     virtual ~discontinuity_info(); // Ensure any derived class destructor gets
						            // a say when we destroy a surface.
/**
 * Adds a discontinuity to the list.
 * <br><br>
 * <b>Role:</b> Adds a discontinuity of the given order at the given parameter value.
 * In periodic cases, it's up to the calling function to ensure that the parameter 
 * values are in the same parameter period.
 * <br><br>
 * @param value
 * parameter value of discontinuity.
 * @param order
 * order of discontinuity.
 */

     void	add_discontinuity( double value, int order );

/**
 * @nodoc
 */
	 discontinuity_inserter* get_disc_inserter () const;

/**
 * Returns a read-only array containing discontinuities up to and including the given order.
 * <br><br>
 * @param n_discont
 * number of discontinuities in the returned array.
 * @param order
 * order of discontinuity.
 */
     const double*	all_discontinuities( int& n_discont, int order );
/**
 * Compares this list with another list.
 * <br><br>
 * <b>Role:</b> This function returns the relationship between this
 * discontinuity_info and a given discontinuity_info, considering discontinuities 
 * up to and including the given order present in this discontinuity_info.
 * <br><br>
 * @param other
 * Instance to test against.
 * @param to_order
 * Maximum order of discontinuities to test. Allowed values are {1,2,3},
 * default is 3.
 */
    discontinuity_info_rel compare_with(const discontinuity_info& other, int to_order = 3) const;
/**
 * Outputs a title line and the details of this discontinuity_info to the specified file.
 * <br><br>
 * @param tline
 * title line.
 * @param fp
 * file pointer.
 */
 	void	debug( char const *tline, FILE *fp) const;
/**
 * Returns a read-only array containing discontinuities of a given order.
 * <br><br>
 * @param n_discont
 * number of discontinuities in the returned array.
 * @param order
 * order of disccontinuity.
 */
     const double*	discontinuities( int& n_discont, int order ) const;
/**
 * Returns whether a discontinuity exists at the given parameter value.
 * <br><br>
 * <b>Role:</b> If there is a discontinuity at the given parameter value,
 * returns the order of the discontinuity. Otherwise, returns 0.
 * <br><br>
 * @param t
 * parameter value at which to test.
 */
     int		discontinuous_at( double t ) const;
/**
 * @nodoc
 * Internal use.Roll
 */
	virtual void full_size(SizeAccumulator&, logical = TRUE) const;
/**
 * Merges two <tt>discontinuity_info</tt> lists, keeping the supplied one unchanged.
 * <br><br>
 * <b>Role:</b> The entries from the given list are added into this list.
 * <br><br>
 * @param old
 * instance to merge.
 */
     void	merge( discontinuity_info const& old );
/**
 * Negates the data for the <tt>discontinuity_info</tt>.
 * <br><br>
 * <b>Role:</b> The entries in this list are reordered, and their parameter values are negated.
 */
     void	negate();
/**
 * Transfers data from the given list to this list (assignment operator).
 * <br><br>
 * @param old
 * the given list.
 */
     discontinuity_info&	operator=( const discontinuity_info& old );
/**
 * Returns the period of the parameter values in the list.
 */
     double	period() const { return _period; }
/**
 * Removes a discontinuity from the list.
 * <br><br>
 * @param value
 * parameter value at which to remove the discontinuity.
 */
     void	remove_discontinuity( double value );
/**
 * Reparameterizes the discontinuities within this list.
 * <br><br>
 * <b>Role:</b> Makes a linear change of parameter to all the discontinuity values <tt>(new value = a * old_value + b)</tt>.
 * <br><br>
 * @param a
 * scale factor.
 * @param b
 * offset.
 */
     void	reparam( double a, double b );
/**
 * Deletes discontinuity information and reinitializes the data fields to zero.
 */
     void	reset();
/**
 * Returns TRUE if no discontinuity information is contained within the list.
 */
	 logical is_empty() const;
/**
 * Restores <tt>discontinuity_info</tt> data from a SAT file.
 * <br><br>
 * <b>Role:</b> This method is never called directly. It is called by a higher
 * hierarchical function if an item in the SAT file is determined to be of this
 * class type. An instance of this class will already have been created through
 * the allocation constructor. This method then populates the class instance
 * with the appropriate data from the SAT file.
 */
     void	restore();
/**
 * Saves <tt>discontinuity_info</tt> data to a SAT file.
 * <br><br>
 * <b>Role:</b> This method is never called directly. It is called by a higher 
 * hierarchical function if an item in the SAT file is determined to be of this 
 * class type while storing information to a save file.
 */
     void	save()	const;
/**
 * Sets the period of the parameter values in the list.
 * <br><br>
 * @param per
 * specified period.
 */
     void	set_periodic( double per ) { _period = per > SPAresnor ? per : 0.0; }
/**
 * Shifts the parameter value of each discontinuity by a given amount.
 * <br><br>
 * @param incr
 * amount to shift.
 */
     void	shift( double incr );

/**
 * Splits the list into two list at a given parameter value.
 * <br><br>
 * <b>Role:</b> Like <tt>curve::split</tt>, the return value contains the initial values
 * (before the split parameter), and the original <tt>discontinuity_info</tt> contains the others
 * (after the split parameter). If the split parameter is itself a discontinuity, it is
 * removed from the list.
 * <br><br>
 * @param param
 * parameter value at which to split the list.
 */
     discontinuity_info	split( double param );


#if defined D3_STANDALONE || defined D3_DEBUG
/**
 * @nodoc
 */
    friend DECL_KERN
	D3_istream &operator>>( D3_istream &, discontinuity_info& );
/**
 * @nodoc
 */
    friend DECL_KERN
	D3_ostream &operator<<( D3_ostream &, const discontinuity_info& );

#endif

};

#if defined D3_STANDALONE || defined D3_DEBUG

class discontinuity_info;
/**
 * @nodoc
 */
DECL_KERN  D3_istream &operator>>(D3_istream &, discontinuity_info& );
/**
 * @nodoc
 */
DECL_KERN  D3_ostream &operator<<(D3_ostream &, const discontinuity_info& );

#endif
/**
 * @nodoc
* Test whether two lists of vectors represent a discontinuity, and add the
* discontinuity if they do.  Typically, this function will be supplied with
* lists of curve derivatives evaluated above and below a suspected
* discontinuity. This function is provided so that consistent tests for
* continuity can be made but it should not be regarded as the authorative test
* for whether a curve is continuous. The order of discontinuity is returned, or
* 0 to indicate no discontinuity.
*
* The tolerance specifies the value within which discontinuity should be
* checked. The value 0.0 indicates using default tolerance for checking
* continuity. 
 */
int DECL_KERN test_discontinuity( SPAvector* D0, SPAvector* D1, int length, double tol = 0.0 );
/** @} */
#endif
