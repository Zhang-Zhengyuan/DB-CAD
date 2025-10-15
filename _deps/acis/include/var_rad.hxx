/*******************************************************************/
/*    Copyright (c) 1989-2020 by Spatial Corp.                     */
/*    All rights reserved.                                         */
/*    Protected by U.S. Patents 5,257,205; 5,351,196; 6,369,815;   */
/*                              5,982,378; 6,462,738; 6,941,251    */
/*    Protected by European Patents 0503642; 69220263.3            */
/*    Protected by Hong Kong Patent 1008101A                       */
/*******************************************************************/
// Class for taking care of the radius definition.
#ifndef VAR_RAD_DEF_HXX
#define VAR_RAD_DEF_HXX
#include "acis.hxx"
#include "dcl_kern.h"
#include "dispatch.hxx"
#include "curdef.hxx"
#include "bs2curve.hxx"
#include "blnd_info.hxx"
#define BHL_MAX_STR_SIZE 4096
class discontinuity_info;
class blend_support;
class teb_pos_evaluator;
class teb_deriv_evaluator;
/**
* @file var_rad.hxx
 * @CAA2Level L1
 * @CAA2Usage U1
 * \addtogroup ACISCURVES
 *
 * @{
 */
extern DECL_KERN int VAR_RAD_CONST;
extern DECL_KERN int VAR_RAD_TWO_ENDS;
extern DECL_KERN int VAR_RAD_FUNCTIONAL;
extern DECL_KERN int VAR_RAD_ELLIPTICAL;
extern DECL_KERN int VAR_RAD_FIXED_WIDTH;
extern DECL_KERN int VAR_RAD_HOLDLINE;
extern DECL_KERN int VAR_RAD_FIXED_DISTANCE;
extern DECL_KERN int VAR_RAD_THREE_ENTITY;
/**
 * Specifies the type of the blend radius function.
 * <br>
 * @param RADIUS_UNSET
 * Radius function is not set.
 * @param TWO_ENDS
 * Two-end radius function defined by the two blend radii at the beginning and at the end of the blend sequence.
 * @param FUNCTIONAL
 * Functional radius function--radius variation along the defining curve is determined by an (arbitrary)
 * function R(t), where t is the parameter along the defining curve of the blend.
 * @param FIXED_WIDTH
 * Implicit radius function--blend radius variation is not specified explicitely, instead, the current radius
 * is found so that the distance between the blend springs is equal to the given "blend width" value.
 * @param HOLDLINE
 * Implicit radius function--blend radius variation is not specified explicitely, instead, the currend radius
 * is found so that one of the blend spring curves coincides with the given "holdline" curve (while the blend
 * is still tangent to the surface conaining the holdline).
 */
enum radius_forms 
    { 
    RADIUS_UNSET = -1,
	TWO_ENDS = 0,
	FUNCTIONAL = 1,
	FIXED_WIDTH = 2,
	HOLDLINE = 3,
	FIXED_DISTANCE = 4,
    CONST_R = 5
    };

// Masks to check const_extensions in var_rad_functional.

GLOBAL_CONST unsigned char const_ext_start = 0x01;
GLOBAL_CONST unsigned char const_ext_end = 0x02;

// Declaration of the base class var_radius.

//
// Note: start and end parameters must always be set to something reasonable,
// even if it's not calibrated.  The SPAparameter range of an edge being
// blended is reasonable.
//

/**
 * Defines the variable radius.
 * <br>
 * <b>Role:</b> This class defines a variable radius. Start and end parameters
 * must always be set to something reasonable, even if it's not calibrated.
 * The parameter range of an edge being blended is reasonable.
 * <br><br>
 * @see blend_spl_sur, var_cross_section
 */
class DECL_KERN var_radius : public ACIS_OBJECT 
	{
private:

    // Whether the next values are valid.

	logical two_ends_calibrated;	

	double rad_start_param;
	double rad_end_param;

	// For implicit radius functions we don't know the radius when requested
	// without building a slice.  Therefore, during blend surface construction
	// we will save the min and max radius encountered for downstream code
	// that needs to know a general guess for the radius.

	double min_radius;
	double max_radius;

protected:

    // Only used by var_rad_functional at present.

    unsigned char const_extensions; 
	
public:

	// Constructors.

/**
 * 	C++ allocation constructor requests memory for this object but does not populate it.
 */
    // Allow an empty constructor.  This is necessary
	// for restore_data(), and should only be used there.

	var_radius();	

/**
 * C++ copy constructor requests memory for this object and populates it with the data from the object supplied as an argument.
 * <br><br>
 * @param old
 * old.
 */
	var_radius( var_radius const &old );

    // Destructor (nothing special needed)
	// Yes we DO need this, and we must declare it virtual. Otherwise we
	// risk creating memory leaks. DAP 27-Nov-98.

/**
 * C++ destructor, deleting a <tt>var_radius</tt>.
 */
	virtual ~var_radius();

// Identification.
/**
 * Returns the radius form.
 */
	virtual int form() const = 0;

/**
 * Returns the string "var_radius".
 */
	virtual char const *type_name() const = 0;

// Member data retrieval.
/**
 * Returns <tt>TRUE</tt> if the ends are calibrated; otherwise, it returns <tt>FALSE</tt>.
 */
	logical calibrated( void ) const { return two_ends_calibrated; }

/**
 * Returns the parameter range.
 */
	SPAinterval param_range( void )	const
        { return SPAinterval( rad_start_param, rad_end_param ); }

/**
 * Get the discontinuities of the radius function.
 */
	virtual void get_disc_info( discontinuity_info &di ) const;

	// Member data setting.
/**
 * Sets the start parameter.
 * <br><br>
 * @param new_param
 * start parameter.
 */
	void set_start_param( double new_param ) { rad_start_param = new_param; }

/**
 * Sets the end parameter.
 * <br><br>
 * @param new_param
 * end parameter.
 */
	void set_end_param( double new_param ) { rad_end_param = new_param; }

/**
 * Sets the minimum radius.
 * <br><br>
 * @param min_rad
 * minimum radius.
 */
	void set_min_radius( double min_rad ) { min_radius = min_rad; }
/**
 * Sets the maximum radius.
 * <br><br>
 * @param max_rad
 * maximum radius.
 */
	void set_max_radius( double max_rad ) { max_radius = max_rad; }

	// Copy.
/**
 * Copies a variable radius.
 */
	virtual var_radius *copy() const = 0;

	// Radius evaluation.
/**
 * Evaluates a variable radius at a given parameter value.
 * <br><br>
 * @param v_param
 * parameter value.
 * @param val
 * value.
 * @param der1
 * first derivative.
 * @param der2
 * second derivative.
 * @param der3
 * third derivative initializer.
 * @param der4
 * fourth derivative.
 */
	virtual void eval (
		            double v_param,
		            double *val = NULL,
		            evaluate_curve_side side = 
                        evaluate_curve_unknown,
		            double *der1 = NULL,
		            double *der2 = NULL,
		            double *der3 = NULL,
		            double *der4 = NULL 
                  ) const;

	// Return start or end radius and derivs.

	// These return the given end value, and work for un-calibrated radii.

	// These are used mainly while the blend is being constructed, for
	// things such as comparing with a mating blend, or checking for zero.
	// For implicit radius functions, however, we can't get the right answers
	// without taking a slice, which requires the blend surface.
	// Therefore, for implicit radii, this function will not return accurate
	// radius values.  It will be consistent, however, and it will return
	// zero if and only if the final end radius will be zero.

	// Included here are defaults -- errors -- for the 'abstract' base class.

/**
 * Returns the start radius.
 */
	virtual double start_radius() const { return -1.; }

/**
 * Returns the end radius.
 */
	virtual double end_radius() const { return -1.; }

/**
 * Returns the first derivative at the start parameter.
 */
	virtual double start_deriv1() const { return  0.; }

/**
 * Returns the first derivative at the end parameter.
 */
	virtual double end_deriv1() const { return  0.; }

/**
 * Returns the minimum radius.
 */
	double get_min_radius() const { return  min_radius; }

/**
 * Returns the maximum radius.
 */
	double get_max_radius() const { return  max_radius; }

    // More conveniences.
    // These work whether or not we are calibrated.
    // Sometimes these are difficult to determine.
    // In such cases, return FALSE, which is the usual case.
    // Therefore, these are guaranteed to return TRUE only if
    // we're sure they really are zero.

/**
 * Returns <tt>TRUE</tt> if the radius at the start or end point of the <tt>var_radius</tt> is zero (less than <tt>SPAresabs</tt>).
 * <br><br>
 * @param at_start
 * at start point if true.
 * @param tol
 * tolerance.
 */
	virtual logical zero_end_radius( logical at_start, double tol = SPAresabs ) const = 0;

/**
 * Returns <tt>TRUE</tt> if the first derivative at the start or end point of the <tt>var_radius</tt> is zero (less than <tt>SPAresabs</tt>).
 * <br><br>
 * @param at_start
 * at start point if true.
 * @param tol
 * tolerance.
 */
	virtual logical zero_end_deriv1( logical at_start, double tol = SPAresabs ) const = 0;

	// utility:
/**
 * Implements an assignment operator, which makes a copy of a <tt>var_radius</tt>.
 * <br><br>
 * @param other
 * variable radius.
 */
	virtual logical operator==( var_radius const &other ) const;

/**
 * Determines whether two variable radii are not equal.
 * <br><br>
 * @param other
 * variable radius.
 */
	virtual logical operator!=( var_radius const &other ) const
	{ return !( *this == other ); }

/**
 * Transforms the variable radius by the given transformation.
 * <br><br>
 * @param trans
 * transformation.
 */
	virtual void operator*=( SPAtransf const &trans );

/**
 * Shifts the variable radius by the given value.
 * <br><br>
 * @param delta
 * shift delta.
 */
	virtual void shift( double delta );

/**
 * Reparameterizes the variable radius with new start and end parameter values.
 * <br><br>
 * @param start
 * start parameter.
 * @param end
 * end parameter.
 */
	virtual void reparam( double start, double end );
/**
 * Trims the variable radius to the specified range.
 * <br><br>
 * @param inter
 * interval.
 */
	virtual void trim_to_range( SPAinterval const &r );
/**
 * Calibrates the variable radius between the two given parameters.
 * <br><br>
 * @param t0
 * start parameter.
 * @param t1
 * end parameter.
 */
	virtual void calibrate_radius( double t0, double t1 );
/**
 * Un-calibrates the radius function(s).
 */
	virtual void uncalibrate_radius();
/**
 * Splits a variable radius into two pieces.
 * <br><br>
 * @param param
 * parameter value.
 * @param pieces
 * the two pieces.
 */
	virtual void split( double param, var_radius *pieces[ 2 ] );
/**
 * Appends one variable radius to the specified variable radius.
 * <br><br>
 * @param varrad
 * variable radius.
 */
	virtual void append( var_radius &vrad );

// Other things local to var_radius:

//STIBUGFIX bd vrblenda, etc. begin:
// Reverse a var_radius, front to back.
/**
 * Reverses this variable radius, front to back.
 */
	virtual void negate();

// Return a bs2_curve representation of this radius function.
/**
 * Returns a bs2_curve representation of this radius function.
 */
	virtual bs2_curve make_bs2() const { return NULL; }   // Not at this level.

/**
 * Returns <tt>TRUE</tt> if the radius function is not an explicit function of <i>v</i>.
 * <br><br>
 * <b>Role:</b> The function is not an explicit function if the blend surface
 * must be evaluated in order to calculate the radius value at a given v-parameter
 */
	virtual logical implicit_var_radius() const = 0;

/**
 * Returns <tt>TRUE</tt> if the radius function is an explicit function of <i>v</i>.
 * <br><br>
 * <b>Role:</b> The function is an explicit function if it can be evaluated independently of the blend surface.
 */
	virtual logical explicit_var_radius() const
        { return !implicit_var_radius(); } 

	// Check whether a radius is zero everywhere, to a given tolerance.

/**
 * Returns <tt>TRUE</tt> if a radius function is zero everywhere, to within a given tolerance - else, it returns <tt>FALSE</tt>.
 * <br><br>
 * @param tol
 * zero tolerance.
 */
	virtual logical is_zero( double tol ) const
	{
		SPAUNUSED(tol)
		return FALSE;
	}

// If the radius is actually constant to within SPAresabs,
// return the radius value in the argument.

/**
 * Determines whether or not the radius is constant.
 * <br><br>
 * @param radius_value
 * tolerance.
 */
	virtual logical is_constant( double *rad_value = NULL ) const = 0;

// Connect two var_radius objects end to end.
//
// This function returns a new var_radius object,
// and does not delete the inputs.
//
// The parameterization of the first radius is preserved,
// and that of the second is shifted to match, but not stretched.
//
// This is not a member function, because the result may be a new derived type.
//
/*
// tbrv
*/
/**
 * @nodoc
 */
	friend DECL_KERN var_radius *var_radius_connect( var_radius *, var_radius * );

	// save / restore:
/**
 * Saves the version number and calls <tt>save_data</tt>.
 */
	virtual void save() const;
/**
 * Saves the information for the <tt>var_radius</tt> to a save file.
 */
	void save_data() const;
/*
// tbrv
*/
/**
 * @nodoc
 */
	friend DECL_KERN var_radius *restore_radius();

/**
 * Restores the data for a <tt>var_radius</tt> from a save file.
 * <br><br>
 * <b>Role:</b>
 * <table width="100%">
 * <tr>
 * <td width="30%"><tt>read_logical</tt></td>
 * <td width="70%">The two ends calibrated variable. Either "uncalibrated" or "calibrated".</td>
 * </tr>
 * <tr>
 * <td width="30%"><tt>read_real</tt></td>
 * <td width="70%">The radius starting parameter.</td>
 * </tr>
 * <tr>
 * <td width="30%"><tt>read_real</tt></td>
 * <td width="70%">The radius ending parameter</td>
 * </tr>
 * </table>
 */
	virtual void restore_data();


// debug:
/**
 * Writes debug information about the variable radius to either standard output or to the specified file.
 * <br><br>
 * @param leader
 * leader string.
 * @param fp
 * file pointer.
 */
	virtual void debug( const char *leader, FILE *fp = debug_file_ptr ) const;

/**
 * @nodoc
 */
	// enquire
	virtual blend_debug_info* enquire(FILE*fp); 
	

    };

#ifdef THIS
#undef THIS
#endif

// Definition of var_rad_const.

/**
 * @nodoc
 */
#define THIS() var_radius
DISPATCH_DECL(KERN)
#if 0
; // semicolon needed for mkman (doc tool) parsing
#endif
#undef THIS

/**
 * Declaration of the derived class var_rad_const.
 */
class DECL_KERN var_rad_const : public var_radius 
	{
private:

	// The radius value.

	double rad_data; 

public:

// Constructors:
/**
 * C++ initialize constructor requests memory for this object and populates it with the data supplied as arguments.
 * <br><br>
 * @param const_rad
 * constant radius. 
 */
	var_rad_const( double const_rad = -1. );

/**
 * C++ copy constructor requests memory for this object and populates it with the data from the object supplied as an argument.
 * <br><br>
 * @param old
 * old.
 */
	var_rad_const( var_rad_const const &old );

// Identification.
/**
 * Returns the form <tt>VAR_RAD_CONST</tt>.
 */
    virtual int	form() const { return VAR_RAD_CONST; }

/**
 * Returns the string "const".
 */
	virtual char const *type_name() const;

/**
 * Copies a <tt>var_rad_const</tt>.
 */
    virtual var_radius *copy() const { return ACIS_NEW var_rad_const( rad_data ); }

/**
 * Returns <tt>TRUE</tt> if the radius at the start or end point of the <tt>var_rad_const</tt> is zero (less than <tt>SPAresabs</tt>).
 * <br><br>
 * @param at_start
 * at start point if true.
 * @param tol
 * tolerance.
 */
	virtual logical zero_end_radius( logical, double tol ) const { return fabs( rad_data ) < tol; }

/**
 * Returns <tt>TRUE</tt> if the first derivative at the start or end point of the <tt>var_rad_const</tt> is zero ( less than <tt>SPAresabs</tt>).
 * <br><br>
 * @param at_start
 * at start point if true.
 * @param tol
 * tolerance.
 */
    virtual logical zero_end_deriv1( logical, double ) const { return TRUE; }
    
/**
 * Determines whether the input is constant.
 * <br><br>
 * @param rad_value
 * tolerance.
 */
	virtual logical is_constant( double *rad_value = NULL ) const 
	{ 
		if ( rad_value ) *rad_value = rad_data; 	
		return TRUE; 
	}
    
// Evaluation.
/**
 * Evaluates a variable radius at a given parameter value.
 * <br><br>
 * @param v_param
 * point.
 * @param val
 * variable radius.
 * @param der1
 * first derivative.
 * @param der2
 * second derivative.
 * @param der3
 * third derivative.
 * @param der4
 * fourth derivative.
 */
	virtual void eval( 
					double, 
					double *val, 
					evaluate_curve_side, 
					double *der1, 
					double *der2, 
					double *der3, 
					double *der4 
				  ) const
    { 
		if ( val )*val = rad_data; 
		if ( der1 ) *der1 = 0.; 
		if ( der2 ) *der2 = 0.; 
		if ( der3 ) *der3 = 0.; 
		if ( der4 ) *der4 = 0.; 
	}

/**
 * Returns the radius at the start parameter.
 */
    virtual double start_radius() const { return rad_data; }

/**
 * Returns the radius at the end parameter.
 */
	virtual double end_radius() const { return rad_data; }

/**
 * Returns the first derivative at the start parameter.
 */
    virtual double start_deriv1() const { return 0.; }

/**
 * Returns the first derivative at the end parameter.
 */

	virtual double end_deriv1() const { return 0.; }

/**
 * Returns <tt>FALSE</tt> for a <tt>var_rad_const</tt>.
 */
    virtual logical implicit_var_radius() const { return FALSE; }

/**
 * Returns <tt>TRUE</tt> if a radius function is zero everywhere, to within a given tolerance - else, it returns <tt>FALSE</tt>.
 * <br><br>
 * @param eps
 * zero tolerance.
 */
	virtual logical is_zero( double eps ) const { return fabs( rad_data ) < eps; }

// utility:
/**
 * Implements an assignment operator, which tests to see if a <tt>var_rad_const</tt> is equal to a <tt>var_radius</tt>.
 * <br><br>
 * @param other
 * variable radius.
 */
    virtual logical operator==( var_radius const &other ) const;

/**
 * Transforms the <tt>var_rad_const</tt> by the given transformation.
 * <br><br>
 * @param trans
 * transformation.
 */
	virtual void operator*=( SPAtransf const &trans );

/**
 * Appends the given <tt>var_radius</tt>.
 * <br><br>
 * @param var_rad
 * variable radius.
 */
	virtual void append( var_radius &var_rad );

/**
 * Returns a <tt>bs2_curve</tt> representation of this radius function.
 */
    virtual bs2_curve make_bs2() const;

// save / restore:
/**
 * Calls <tt>var_radius::save_data</tt> method, then saves the start and end radius data.
 */
	virtual void save() const;

/**
 * Restores the data for a <tt>var_rad_const</tt> from a save file.
 * <br><br>
 * <b>Role:</b>
 * <table width="100%">
 * <tr>
 * <td width="30%"><tt>var_radius::restore_data</tt></td>
 * <td width="70%">Generic information</td>
 * </tr>
 * <tr>
 * <td width="30%"><tt>read_real</tt></td>
 * <td width="70%">radius data</td>
 * </tr>
 * </table>
 */
	virtual void restore_data();

// debug:
/**
 * Outputs a title line and information about <tt>var_rad_const</tt> to the debug file or to the specified file.
 * <br><br>
 * @param leader
 * leader string.
 * @param fp
 * file pointer.
 */
	virtual void debug( const char *leader, FILE *fp = debug_file_ptr ) const;

/**
 * @nodoc
 */
	// enquire
	virtual blend_debug_info* enquire(FILE*fp );
	
	};
/**
 * Defines a two-ends form of a variable radius function.
 * <br>
 * <b>Role:</b> This class defines a variable radius function that varies linearly
 * between two end values.
 */
class DECL_KERN var_rad_two_ends: public var_radius 
	{
private:

	// Start and end radius values.

	double start_rad_data;		
	double end_rad_data;

public:

// Constructors:
/**
 * C++ initialize constructor requests memory for this object and populates it with the data supplied as arguments.
 * <br><br>
 * @param start_rad
 * start radius.
 * @param end_rad
 * end radius.
 */
	var_rad_two_ends( double start_rad = -1., double end_rad = -1. );

/**
 * C++ copy constructor requests memory for this object and populates it with the data from the object supplied as an argument.
 * <br><br>
 * @param old
 * old.
 */
	var_rad_two_ends( var_rad_two_ends const &old );

	// Destructor (nothing special needed)
	// Yes, it is best to define it as it is virtual. Some compilers
	// have been known to whine if we don't. DAP 27-Nov-98.
/**
 * C++ destructor, deleting a <tt>var_rad_two_ends</tt>.
 */
	virtual ~var_rad_two_ends();

// Identification.
/**
 * Returns the form <tt>TWO_ENDS</tt>.
 */
	virtual int	form() const { return VAR_RAD_TWO_ENDS; }
	
/**
 * Returns the string "two_ends".
 */
	virtual char const *type_name() const;

	// Member data retrieval:
/**
 * Returns the start radius datum.
 */
	double start_rad() const { return start_rad_data; }

/**
 * Returns the end radius datum.
 */
	double end_rad() const { return end_rad_data; }

	// Member-setting functions:
/**
 * Sets a new value for the start radius.
 * <br><br>
 * @param new_rad
 * new radius.
 */
	void set_start_rad( double new_rad ) { start_rad_data = new_rad; }

/**
 * Sets a new value for the end radius.
 * <br><br>
 * @param new_rad
 * new radius.
 */
	void set_end_rad(   double new_rad ) { end_rad_data   = new_rad; }

	// Copy.
/**
 * Copies a <tt>var_rad_two_ends</tt>.
 */
	virtual var_radius *copy() const;

// Evaluation.
/**
 * Evaluates a variable radius at a given parameter value.
 * <br><br>
 * @param v_param
 * point.
 * @param val
 * variable radius.
 * @param der1
 * first derivative.
 * @param der2
 * second derivative.
 * @param der3
 * third derivative.
 * @param der4
 * fourth derivative.
 */
	virtual void eval(
					double v_param,
					double *val  = NULL,
					evaluate_curve_side side = evaluate_curve_unknown,
					double *der1 = NULL,
					double *der2 = NULL,
					double *der3 = NULL,
					double *der4 = NULL 
				  ) const;

/**
 * Returns the radius at the start parameter.
 */
	virtual double start_radius() const;

/**
 * Returns the radius at the end parameter.
 */
	virtual double end_radius() const;

/**
 * Returns the first derivative at the start parameter.
 */
	virtual double start_deriv1() const;

/**
 * Returns the first derivative at the end parameter.
 */
	virtual double end_deriv1() const;

	// More conveniences.
/**
 * Returns <tt>TRUE</tt> if the radius at the start or end point of the <tt>var_rad_two_ends</tt> is zero (less than <tt>SPAresabs</tt>).
 * <br><br>
 * @param at_start
 * at start point if true.
 * @param tol
 * tolerance.
 */
	virtual logical zero_end_radius( 
								logical at_start, 
								double tol = SPAresabs 
							  ) const
	{ 
		return at_start ? fabs( start_rad_data ) < tol 
						: fabs( end_rad_data ) < tol; 
	}

/**
 * Returns <tt>TRUE</tt> if the first derivative at the start or end point of the <tt>var_rad_two_ends</tt> is zero ( less than <tt>SPAresabs</tt>).
 * <br><br>
 * @param at_start
 * at start point if true.
 * @param tol
 * tolerance.
 */
	virtual logical zero_end_deriv1( 
								logical at_start,
								double tol = SPAresabs 
							  ) const
	{
		SPAUNUSED( at_start )
		return fabs( start_rad_data - end_rad_data ) < tol;
	}


// utility:
/**
 * Implements an assignment operator, which tests to see if a <tt>var_rad_two_ends</tt> is equal to a <tt>var_radius</tt>.
 * <br><br>
 * @param other
 * variable radius.
 */
	virtual logical operator==( var_radius const &other ) const;

/**
 * Determines whether a <tt>var_rad_two_ends</tt> and a <tt>var_radius</tt> are not equal.
 * <br><br>
 * @param other
 * variable radius.
 */
	virtual logical operator!=( var_radius const &other ) const
		{ return !( *this == other ); }

/**
 * Transforms the <tt>var_rad_two_ends</tt> by the given transformation.
 * <br><br>
 * @param trans
 * transformation.
 */
	virtual void operator*=( SPAtransf const &trans );

/**
 * Trims the radius to the given interval.
 * <br><br>
 * @param inter
 * interval.
 */
	virtual void trim_to_range( SPAinterval const &r );

/**
 * Splits a <tt>var_rad_two_ends</tt> into two pieces at the given point.
 * <br><br>
 * @param param
 * point to split.
 * @param pieces
 * array of 2 var_radii.
 */
	virtual void split( double param, var_radius *pieces[ 2 ] );

/**
 * Appends the given <tt>var_radius</tt>.
 * <br><br>
 * @param var_rad
 * variable radius.
 */
	virtual void append( var_radius &var_rad );

/**
 * Reverses this variable radius, front to back.
 */
	virtual void negate();

/**
 * Returns a <tt>bs2_curve</tt> representation of this radius function.
 */
	virtual bs2_curve make_bs2() const;

/**
 * Returns <tt>FALSE</tt> for a <tt>var_rad_two_ends</tt>.
 */
	virtual logical implicit_var_radius() const { return FALSE; }

/**
 * Returns <tt>TRUE</tt> if a radius function is zero everywhere, to within a given tolerance - else, it returns <tt>FALSE</tt>.
 * <br><br>
 * @param eps
 * zero tolerance.
 */
	virtual logical is_zero( double eps ) const
		{ return ( fabs( start_rad_data ) < eps && fabs( end_rad_data ) < eps ); }

/**
 * Determines whether the input is constant.
 * <br><br>
 * @param const_rad_value
 * tolerance.
 */
	virtual logical is_constant( double *rad_value = NULL ) const;


// save / restore:
/**
 * Calls <tt>var_radius::save_data</tt> method, then saves the start and end radius data.
 */
	virtual void save() const;

/**
 * Restores the data for a <tt>var_rad_two_ends</tt> from a save file.
 * <br><br>
 * <b>Role:</b>
 * <table width="100%">
 * <tr>
 * <td width="30%"><tt>var_radius::restore_data</tt></td>
 * <td width="70%">Generic information</td>
 * </tr>
 * <tr>
 * <td width="30%"><tt>read_real</tt></td>
 * <td width="70%">start radius data</td>
 * </tr>
 * <tr>
 * <td width="30%"><tt>read_real</tt></td>
 * <td width="70%">end radius data</td>
 * </tr>
 * </table>
 */
	virtual void restore_data();


// debug:
/**
 * Outputs a title line and information about <tt>var_rad_two_ends</tt> to the debug file or to the specified file.
 * <br><br>
 * @param leader
 * leader string.
 * @param fp
 * file pointer.
 */
	virtual void debug( const char *leader, FILE *fp = debug_file_ptr ) const;
/**
 * @nodoc
 */
	// enquire
	virtual blend_debug_info* enquire(FILE*fp);
	

	};



// Declaration of the derived class var_rad_functional.

//
// Note on behavior:
// When a bs2_curve is passed into a var_rad_functional constructor,
// the constructors will not make a copy of the curve.  The
// bs2_curve becomes the exclusive property of the var_rad_functional
// object, and will be deleted by var_rad_functional destructors.
// If for some reason the caller wants to keep a bs2 radius
// function after calling our constructor, they must make a copy.
// (E.g., rad = var_rad_functional( FUNCTIONAL, bs2_curve_copy( fcn ) ).)
//
//
// Implementation note: We're going to use a bs2_curve for the radius function.
// We could use polynomials, but it is more convenient to work with control
// points than monomial coefficients, and, more importantly, the polynomial
// class lacks save/restore and other utilities such as operator==, append, etc.
// We could use bs1_curve, but that hasn't been invented yet.
//

// An identifying string for type_name().
// STI jmb: Moved to varadius.cxx so static string is in cxx file
//static char functional_id[] = "functional";

/**
 * Defines a functional form of a variable radius function.
 * <br>
 * <b>Role:</b> This class defines a variable radius function that is specified
 * by a given function, which is represented by a <tt>bs2_curve</tt>.
 */
class DECL_KERN var_rad_functional : public var_radius 
	{
private:

	bs2_curve rad_fcn;

public:

// Constructors:
/**
 * C++ constructor, creating a <tt>var_rad_functional</tt> using the specified parameters.
 * <br><br>
 * <b>Role:</b> When a <tt>bs2_curve</tt> passes into a <tt>var_rad_functional</tt> constructor,
 * the constructors do not make a copy of the curve. The <tt>bs2_curve</tt> becomes the
 * exclusive property of the <tt>var_rad_functional</tt> object, and it is deleted by the
 * <tt>var_rad_functional</tt> destructors. If the caller wants to keep a bs2 radius
 * function after calling the constructor, the caller must make a copy; e.g. using:
 * <tt>rad =	var_rad_functional(FUNCTIONAL, bs2_curve_copy(fcn)))</tt>
 * <br><br>
 * @param radius_fcn
 * bs2 curve for radius.
 */
	var_rad_functional( bs2_curve radius_fcn = NULL );

/**
 * C++ constructor, creating a <tt>var_rad_functional</tt> by copying one.
 * <br><br>
 * @param old
 * old.
 */
	var_rad_functional( var_rad_functional const &old );

// Destructor
/**
 * C++ destructor, deleting a <tt>var_rad_functional</tt>.
 */
	virtual ~var_rad_functional();

	// Identification.
/**
 * Returns the form <tt>FUNCTIONAL</tt>.
 */
	virtual int form() const { return VAR_RAD_FUNCTIONAL; }
	
/**
 * Returns the string "functional".
 */
	virtual char const *type_name() const;

/**
 * Get the discontinuities of the radius function.
 */
	virtual void get_disc_info( discontinuity_info &) const;

	// Member data retrieval:
/**
 * Returns the <tt>bs2_curve</tt> defining the radius function.
 */
	bs2_curve rad_function() const { return rad_fcn; }

/*
// tbrv
*/
/**
 * @nodoc
 */
    int get_const_extensions() const { return const_extensions; }

	// Member-setting functions:
/*
// tbrv
*/
/**
 * @nodoc
 */
	int set_const_extensions( int flags );

	// Copy.
/**
 * Copies a <tt>var_rad_functional</tt>.
 */
	virtual var_radius *copy() const;

	//  evaluation:
/**
 * Evaluates a variable radius at a given parameter value.
 * <br><br>
 * @param v_param
 * point.
 * @param val
 * variable radius.
 * @param der1
 * first derivative.
 * @param der2
 * second derivative.
 * @param der3
 * third derivative.
 * @param der4
 * fourth derivative.
 */
	virtual void eval(
					double v_param,
					double *val  = NULL,
					evaluate_curve_side side = evaluate_curve_unknown,
					double *der1 = NULL,
					double *der2 = NULL,
					double *der3 = NULL,
					double *der4 = NULL 
				  ) const;

/**
 * Returns the radius at the start parameter.
 */
	virtual double start_radius() const;

/**
 * Returns the radius at the end parameter.
 */
	virtual double end_radius() const;

/**
 * Returns the first derivative at the start parameter.
 */
	virtual double start_deriv1() const;

/**
 * Returns the first derivative at the end parameter.
 */
	virtual double end_deriv1() const;

	// More conveniences.
/**
 * Returns <tt>TRUE</tt> if the radius at the start or end point of the <tt>var_rad_functional</tt> is zero (less than <tt>SPAresabs</tt>).
 * <br><br>
 * @param at_start
 * at start point if true.
 * @param tol
 * tolerance.
 */
	virtual logical zero_end_radius( 
								logical at_start, 
								double tol = SPAresabs 
							  ) const
		{ return at_start ? fabs( this->start_radius() ) < tol
						  : fabs( this->end_radius() ) < tol; }

/**
 * Returns <tt>TRUE</tt> if the first derivative at the start or end point of the <tt>var_rad_functional</tt> is zero (less than <tt>SPAresabs</tt>).
 * <br><br>
 * @param at_start
 * at start point if true.
 * @param tol
 * tolerance.
 */
	virtual logical zero_end_deriv1( 
								logical at_start, 
								double tol = SPAresabs 
							  ) const
		{ return at_start ? fabs( this->start_deriv1() ) < tol
						  : fabs( this->end_deriv1()   ) < tol; }

	// utility:
/**
 * Implements an assignment operator, which tests to see if a <tt>var_rad_functional</tt> is equal to a <tt>var_radius</tt>.
 * <br><br>
 * @param other
 * variable radius.
 */
	virtual logical operator==( var_radius const &other ) const;

/**
 * Determines whether a <tt>var_rad_functional</tt> and a <tt>var_radius</tt> are not equal.
 * <br><br>
 * @param other
 * variable radius.
 */
	virtual logical operator!=( var_radius const &other ) const
		{ return !( *this == other ); }

/**
 * Transforms the <tt>var_rad_functional</tt> by the given transformation.
 * <br><br>
 * @param trans
 * transformation.
 */
	virtual void operator*=( SPAtransf const & trans );

/**
 * Reparameterizes the <tt>var_rad_functional</tt> by the given parameter shift.
 * <br><br>
 * @param delta
 * shift delta.
 */
	virtual void shift( double delta );

/**
 * Reparameterizes a <tt>var_rad_functional</tt>.
 * <br><br>
 * @param start
 * start parameter.
 * @param end
 * end parameter.
 */
	virtual void reparam( double start, double end );

//
// The following no longer applies, with the new defining curve changes:
// *
// * We don't do anything special for calibrate_radius().
// * In particular, this should not reparameterize rad_fcn,
// * because its range does not necessarily coincide with our
// * calibration range; for example, when the radius function
// * is extended into a mating blend ( ATTRIB_VAR_BLEND::extend_def() ).
// * The parameterization of rad_fcn must be dealt with explicitly.
// *
// The radius function extends the entire length of a subsequence
// of blends, i.e., a smooth sequence of var-rad blends with no
// constant blends.  Every blend in the subsequence gets the same
// radius function, and extend_def() and extend_rad() are no longer
// called at all.  The given start and end parameters are the
// extreme values at the ends of the subsequence.
// We don't reparam the radius function, however, because we don't
// want to shift all of the interior knot values.  We just want to
// adjust the start and end values.

/**
 * The radius function extends the entire length of a subsequence of blends.
 * <br><br>
 * <b>Role:</b> A smooth sequence of variable radius blends with no constant blends.
 * Every blend in the subsequence gets the same radius function, and <tt>extend_def</tt>
 * and <tt>extend_rad</tt> are no longer called at all. The given start and end parameters
 * are the extreme values at the ends of the subsequence. We don't reparam the
 * radius function, however, because we don't want to shift all of the interior
 * knot values. We just want to adjust the start and end values.
 * <br><br>
 * @param t0
 * first calibration.
 * @param t1
 * second calibration.
 */
	virtual void calibrate_radius( double t0, double t1 );

/**
 * Splits a <tt>var_rad_functional</tt> into two pieces at the given point.
 * <br><br>
 * @param param
 * point of split.
 * @param pieces
 * array of 2 var radii.
 */
	virtual void split( double param, var_radius *pieces[ 2 ] );

/**
 * Appends the given <tt>var_radius</tt>.
 * <br><br>
 * @param var_rad
 * variable radius.
 */
	virtual void append( var_radius &var_rad );

/**
 * Reverses a functional variable radius, front to back.
 */
	virtual void negate();

/**
 * Returns a <tt>bs2_curve</tt> representation of this radius function.
 */
	virtual bs2_curve make_bs2() const;

/**
 * Returns <tt>FALSE</tt> for a <tt>var_rad_functional</tt>.
 */
	virtual logical implicit_var_radius() const { return FALSE; }

/**
 * Returns <tt>TRUE</tt> if a radius function is zero everywhere, to within a given tolerance - else, it returns <tt>FALSE</tt>.
 * <br><br>
 * @param eps
 * zero tolerance.
 */
	virtual logical is_zero( double eps ) const;

/**
 * Determines whether the input is constant.
 * <br><br>
 * @param const_rad_value
 * tolerance.
 */
	virtual logical is_constant( double *rad_value = NULL ) const;

	// Static version of the above member function.
/**
 * @nodoc
 */
	static logical is_constant( 
							bs2_curve bs2, 
							double tol = SPAresabs, 
							double *rad_value = NULL 
							);

	// save / restore:
/**
 * Saves the form or id, calls <tt>save_data</tt>, then saves the underlying curve.
 */
	virtual void save() const;

/**
 * Restores the data for a <tt>var_rad_functional</tt> from a save file.
 * <br><br>
 * <b>Role:</b>
 * <table width="100%">
 * <tr>
 * <td width="30%"><tt>var_radius::restore_data</tt></td>
 * <td width="70%">Generic radius information</td>
 * </tr>
 * <tr>
 * <td width="30%"><tt>bs2_curve_restore</tt></td>
 * <td width="70%">Underlying B-spline curve</td>
 * </tr>
 * </table>
 */
	virtual void restore_data();

// debug:
/**
 * Outputs a title line and information about <tt>var_rad_functional</tt> to the debug file or to the specified file.
 * <br><br>
 * @param leader
 * leader string.
 * @param fp
 * pointer.
 */
	virtual void debug( const char *leader, FILE *fp = debug_file_ptr ) const;
	};


// Declaration of the derived class var_rad_rot_ellipse.

// This one is awkward, in that the left and right radii are not separate.
// Execution efficiency is important here, because this is called on
// every surface evaluation.
// We'll go on the assumption that the surface using this will be using
// this same object for left and right.  (Actually, it's hard to imagine
// any other case, but we won't require it here, we'll just assume that
// it's probably true.)

// An identifying string for type_name().
// STI jmb: Moved to varadius.cxx so static string is in cxx file
// static char elliptical_id[] = "elliptical";

/**
 * Defines a rotational radius ellipse for a variable radius function.
 */
class DECL_KERN var_rad_rot_ellipse : public var_radius 
	{
private:

	// Regular data.

	double maj0, maj1;
	double min0, min1;
	double rot0, rot1;
	logical is_left;	// which side is the reference face

	// Cache.

	int num_derivs_saved;
	double saved_param;
	double offset1[ 4 ];	// offset1 will be the reference face.
	double offset2[ 4 ];
	double weight[ 4 ];

public:

	// Constructors:
/**
 * C++ allocation constructor requests memory for this object but does not populate it.
 */
	var_rad_rot_ellipse();

/**
 * C++ initialize constructor requests memory for this object and populates it with the data supplied as arguments.
 * <br><br>
 * @param start_maj
 * start major.
 * @param end_maj
 * end major.
 * @param start_min
 * start minor.
 * @param end_min
 * end minor.
 * @param start_rot
 * start rotation.
 * @param end_rot
 * end rotation.
 * @param left_face_is_ref
 * left face.
 */
	var_rad_rot_ellipse(		 		// Not calibrated.
					double start_maj, 
					double end_maj,
					double start_min, 
					double end_min,
					double start_rot, 
					double end_rot,
					logical left_face_is_ref
					);

/**
 * C++ copy constructor requests memory for this object and populates it with the data from the object supplied as an argument.
 * <br><br>
 * @param old
 * old.
 */
	var_rad_rot_ellipse( var_rad_rot_ellipse const &old );

// Destructor (nothing special needed)
	// Yes, it is best to define it as it is virtual. Some compilers
	// have been known to whine if we don't. DAP 27-Nov-98.

/**
 * C++ destructor, deleting a <tt>var_rad_rot_ellipse</tt>.
 */
	virtual ~var_rad_rot_ellipse();

	// Identification.
/**
 * Returns a <tt>var_rad_rot_ellipse</tt> form.
 */
	virtual int form() const { return VAR_RAD_ELLIPTICAL; }
	
/**
 * Returns the string "elliptical".
 */
	virtual char const *type_name() const;// { return elliptical_id; }

// Member data retrieval:
/**
 * Returns the start major.
 */
	double	start_maj() const { return maj0; }

/**
 * Returns the end major.
 */
	double	end_maj() const { return maj1; }

/**
 * Returns the start minor.
 */
	double	start_min() const { return min0; }

/**
 * Returns the end minor.
 */
	double	end_min() const { return min1; }

/**
 * Returns the start rotation.
 */
	double	start_rot() const { return rot0; }

/**
 * Returns the end rotation.
 */
	double	end_rot() const { return rot1; }

/**
 * Returns <tt>TRUE</tt> if the given reference is from the left - else, it returns <tt>FALSE</tt>.
 */
	logical ref_is_left() const	{ return is_left; }

// Member-setting functions:
/**
 * Sets the start major.
 * <br><br>
 * @param new_maj0
 * new major.
 */
	void set_start_maj( double new_maj0 ) { maj0 = new_maj0; }

/**
 * Sets end major.
 * <br><br>
 * @param new_maj1
 * new major.
 */
	void set_end_maj( double new_maj1 ) { maj1 = new_maj1; }

/**
 * Sets the start minor.
 * <br><br>
 * @param new_min0
 * new minor.
 */
	void set_start_min( double new_min0 ) { min0 = new_min0; }

/**
 * Sets end minor.
 * <br><br>
 * @param new_min1
 * new minor.
 */
	void set_end_min( double new_min1 ) { min1 = new_min1; }

/**
 * Sets the start rotation.
 * <br><br>
 * @param new_rot0
 * new rotation.
 */
	void set_start_rot( double new_rot0 ) { rot0 = new_rot0; }

/**
 * Sets end rotation.
 * <br><br>
 * @param new_rot1
 * new rotation.
 */
	void set_end_rot(   double new_rot1 ) { rot1 = new_rot1; }

/**
 * Sets the referenced face.
 * <br><br>
 * @param new_left
 * new left.
 */
	void set_ref_face( logical new_left ) { is_left = new_left; }

	// Copy.
/**
 * Copies the <tt>var_rad_rot_ellipse</tt>.
 */
	virtual var_radius *copy() const;

//  evaluation:

// ***	Note: this one shouldn't be called at this level,
// ***	because we need to know whether we're right or left.
// ***	This mess will be straightened out soon...

/**
 * Finds the position of the 1st, 2nd, and 3rd derivatives.
 * <br><br>
 * @param v_param
 * v-parameter.
 * @param val
 * value.
 * @param der1
 * 1st derivative.
 * @param der2
 * 2nd derivative.
 * @param der3
 * 3rd derivative.
 * @param der4
 * 4th derivative.
 */
	virtual void eval(
					double v_param,
					double *val = NULL,
					evaluate_curve_side side = evaluate_curve_unknown,
					double *der1 = NULL,
					double *der2 = NULL,
					double *der3 = NULL,
					double *der4 = NULL 
				  ) const;

// The next two are new to this derived class:
/**
 * Finds the position of the 1st, 2nd, and 3rd derivatives.
 * <br><br>
 * @param v_param
 * v-parameter.
 * @param is_left
 * is left.
 * @param val
 * value.
 * @param der1
 * 1st derivative.
 * @param der2
 * 2nd derivative.
 * @param der3
 * 3rd derivative.
 * @param der4
 * 4th derivative.
 */
	virtual void eval(
					double v_param, 
					logical is_left,	// left/right flag
					double *val = NULL,
					double *der1 = NULL,
					double *der2 = NULL,
					double *der3 = NULL,
					double *der4 = NULL 
				  ) const;

/**
 * Returns the position of all the evaluators.
 * <br><br>
 * @param param
 * parameter.
 * @param n_derivs
 * n derivatives.
 * @param left_rad
 * left radius.
 * @param right_rad
 * right radius.
 * @param left_tbw
 * left thumbweight.
 * @param right_tbw
 * right thumbweight.
 */
	void eval_all(
				double param, 
				int n_derivs,
				double left_rad[ 5 ], 
				double right_rad[ 5 ],
				double left_tbw[ 5 ], 
				double right_tbw[ 5 ] 
			  ) const;

/**
 * Returns the start radius.
 */
	virtual double start_radius() const;

/**
 * Returns the end radius.
 */
	virtual double end_radius() const;

/**
 * Returns the start derivative.
 */
	virtual double start_deriv1() const;

/**
 * Returns the end derivative.
 */
	virtual double end_deriv1() const;

// More conveniences.
/**
 * Returns <tt>TRUE</tt> if the radius at the start or end point of the <tt>var_rad_rot_ellipse</tt> is zero (less than <tt>SPAresabs</tt>).
 * <br><br>
 * @param at_start
 * at start point if true.
 * @param tol
 * tolerance.
 */
	virtual logical zero_end_radius( 
								logical at_start, 
								double tol = SPAresabs 
							  ) const
		{ return at_start ? fabs( maj0 ) < tol : fabs( maj1 ) < tol; }

/**
 * Returns <tt>TRUE</tt> if the first derivative at the start or end point of the <tt>var_rad_rot_ellipse</tt> is zero (less than <tt>SPAresabs</tt>).
 * <br><br>
 * @param at_start
 * at start point if true.
 * @param tol
 * tolerance.
 */
	virtual logical zero_end_deriv1( 
								logical at_start,
								double tol = SPAresabs 
							  ) const
	{
		SPAUNUSED( at_start )
		return ( fabs( maj1 - maj0 ) < tol && fabs( min1 - min0 ) < tol );
	}


// utility:
/**
 * Implements an assignment operator, which tests to see if a <tt>var_rad_rot_ellipse</tt> is equal to a <tt>var_radius</tt>.
 * <br><br>
 * @param other
 * other var_radius.
 */
	virtual logical operator==( var_radius const &other ) const;

/**
 * Determines whether a <tt>var_rad_rot_ellipse</tt> is equal.
 * <br><br>
 * @param other
 * other var_radius.
 */
	virtual logical operator!=( var_radius const &other ) const
		{ return !( *this == other ); }

/**
 * Transforms the variable radius by the specified transform.
 * <br><br>
 * @param trans
 * transformation.
 */
	virtual void operator*=( SPAtransf const &trans );

/**
 * Trims to range.
 * <br><br>
 * @param inter
 * interval.
 */
	virtual void trim_to_range( SPAinterval const &r );

/**
 * Returns the split.
 * <br><br>
 * @param param
 * parameter.
 * @param pieces
 * pieces.
 */
	virtual void split( double param, var_radius *pieces[ 2 ] );
/**
 * Appends the <tt>var_rad_rot_ellipse</tt>.
 * <br><br>
 * @param var_rad
 * variance radius.
 */
	virtual void append( var_radius &var_rad );

/**
 * Reverses this variable radius, front to back.
 */
	virtual void negate();

/**
 * Returns a bs2_curve representation of this radius function.
 */
	virtual bs2_curve make_bs2() const;

/**
 * Determines if implicit.
 */
	virtual logical implicit_var_radius() const { return FALSE; }

/**
 * Returns <tt>TRUE</tt> the two <tt>var_rad_rot_ellipse</tt> are zero - else, it returns <tt>FALSE</tt>.
 * <br><br>
 * @param eps
 * zero tolerance.
 */
	virtual logical is_zero( double eps ) const
		{ return ( fabs( maj0 ) < eps && fabs( maj1 ) < eps
				&& fabs( min0 ) < eps && fabs( min1 ) < eps ); }

/**
 * Determines whether the input is constant.
 * <br><br>
 * @param const_rad_value
 * tolerance.
 */
	virtual logical is_constant( double *rad_value = NULL ) const;

	// save / restore:
/**
 * Saves the form or id, calls <tt>save_data</tt>, saves the major radius, minor radius, rotation, and face left or right.
 */
	virtual void save() const;

/**
 * Restores the data for a <tt>var_rad_rot_ellipse</tt> from a save file.
 * <br><br>
 * <b>Role:</b>
 * <table width="100%">
 * <tr>
 * <td width="30%"><tt>read_real</tt></td>
 * <td width="70%">Start major radius</td>
 * </tr>
 * <tr>
 * <td width="30%"><tt>read_real</tt></td>
 * <td width="70%">End major radius</td>
 * </tr>
 * <tr>
 * <td width="30%"><tt>read_real</tt></td>
 * <td width="70%">Start minor radius</td>
 * </tr>
 * <tr>
 * <td width="30%"><tt>read_real</tt></td>
 * <td width="70%">End minor radius</td>
 * </tr>
 * <tr>
 * <td width="30%"><tt>read_real</tt></td>
 * <td width="70%">Start rotation angle</td>
 * </tr>
 * <tr>
 * <td width="30%"><tt>read_real</tt></td>
 * <td width="70%">End rotation angle</td>
 * </tr>
 * <tr>
 * <td width="30%"><tt>read_logical</tt></td>
 * <td width="70%">Face for reference. Either "right" or "left"</td>
 * </tr>
 * </table>
 */
	virtual void restore_data();


	// debug:
/**
 * Prints out a class-specific identifying line to standard output or to the specified file.
 * <br><br>
 * @param leader
 * leader.
 * @param fp
 * file.
 */
	virtual void debug( const char *leader, FILE *fp = debug_file_ptr ) const;
	};


// Declaration of the derived class var_rad_fixed_width.

// An identifying string for type_name().
// STI jmb: Moved to varadius.cxx so static string is in cxx file
// static char fixed_width_id[] = "fixed_width";

/**
 * Defines a constant cross section radius for a variable radius function.
 * <br>
 * <b>Role:</b> This radius is an implicit function of the parameter <i>v</i>. It cannot
 * be evaluated explicitly as <i>r(v)</i> without partially evaluating the blend surface
 * itself.
 */
class DECL_KERN var_rad_fixed_width : public var_radius 
	{
private:

	double	width_data;

public:

	// Constructors:
/**
 * C++ initialize constructor requests memory for this object and populates it with the data supplied as arguments.
 * <br><br>
 * @param width
 * given width.
 */
	var_rad_fixed_width( double	width = -1. );

/**
 * C++ copy constructor requests memory for this object and populates it with the data from the object supplied as an argument.
 * <br><br>
 * @param old
 * given width.
 */
	var_rad_fixed_width( var_rad_fixed_width const &old );

	// Destructor (nothing special needed)
	// Yes, it is best to define it as it is virtual. Some compilers
	// have been known to whine if we don't. DAP 27-Nov-98.
/**
 * C++ destructor, deleting a <tt>var_fixed_width</tt>.
 */
	virtual ~var_rad_fixed_width();


	// Identification.
/**
 * Returns the form <tt>FIXED_WIDTH</tt>.
 */
	virtual int form() const { return VAR_RAD_FIXED_WIDTH; }
	
/**
 * Returns the string "fixed_width".
 */
	virtual char const *type_name() const;// { return fixed_width_id; }

	// Member data retrieval:
/**
 * Gets the width of a <tt>var_rad_fixed_width</tt>.
 */
	double	width() const { return width_data; }

	// Member-setting functions:
/**
 * Sets the width of the radius.
 * <br><br>
 * @param width
 * width value.
 */
	void set_width( double width ) { width_data = width; }

	// Copy.
/**
 * Copies a <tt>var_rad_fixed_width</tt>.
 */
	virtual var_radius *copy( ) const;

//
//  evaluation: These are all errors.  The base class implementation does that.
//
//	virtual void eval (double param,
//			double * = NULL,
//			double * = NULL,
//			double * = NULL,
//			double * = NULL) const;
//
// Note: these don't give accurate offset values.  See comments in base class.
//
/**
 * Returns the start of the radius.
 */
	virtual double start_radius() const { return width_data; }

/**
 * Returns the end of the radius.
 */
	virtual double end_radius() const { return width_data; }

/**
 * Returns the start of the first derivative.
 */
	virtual double start_deriv1() const { return 0.; }

/**
 * Returns the end of the first derivative.
 */
	virtual double end_deriv1() const { return 0.; }

// More conveniences.
/**
 * Returns <tt>TRUE</tt> if the radius at the start or end point of the <tt>var_rad_fixed_width</tt> is zero (less than <tt>SPAresabs</tt>).
 * <br><br>
 * @param at_start
 * at start point if true.
 * @param tol
 * tolerance.
 */
	virtual logical zero_end_radius( 
								logical at_start,
								double tol = SPAresabs 
							  ) const
	{
		SPAUNUSED( at_start )
		return width_data < tol;
	}

/**
 * Returns <tt>TRUE</tt> if the first derivative at the start or end point of the <tt>var_rad_fixed_width</tt> is zero (less than <tt>SPAresabs</tt>).
 * <br><br>
 * @param at_start
 * at start point if true.
 * @param tol
 * tolerance.
 */
	virtual logical zero_end_deriv1( 
								logical /*at_start*/,
								double /*tol*/ = SPAresabs 
							  ) const
	{
		// Can't know for sure without evaluating the blend.

		return FALSE;
	}


// utility:
/**
 * Implements an assignment operator, which tests to see if a <tt>var_rad_fixed_width</tt> is equal to a <tt>var_radius</tt>.
 * <br><br>
 * @param other
 * variable radius.
 */
	virtual logical operator==( var_radius const &other ) const;

/**
 * Tests the two <tt>var_rad_fixed_widths</tt> to see if they are not equal.
 * <br><br>
 * @param other
 * variable radius.
 */
	virtual logical operator!=( var_radius const &other ) const
		{ return !( *this == other ); }

/**
 * Transforms the <tt>var_rad_fixed_width</tt>.
 * <br><br>
 * <b>Role:</b> Only scaling is significant here.
 * <br><br>
 * @param trans
 * transformation.
 */
	virtual void operator*=( SPAtransf const &trans );

/**
 * Returns a <tt>bs2_curve</tt> representation of this radius function.
 */
	virtual bs2_curve make_bs2() const;

/**
 * Returns <tt>TRUE</tt> for a <tt>var_rad_fixed_width</tt>.
 */
	virtual logical implicit_var_radius() const { return TRUE; }

/**
 * Returns <tt>TRUE</tt> if a radius function is zero everywhere, to within a given tolerance - else, it returns <tt>FALSE</tt>.
 * <br><br>
 * @param eps
 * zero tolerance.
 */
	virtual logical is_zero( double eps ) const
		{ return ( fabs( width_data ) < eps ); }

/**
 * Determines whether the input is constant.
 * <br><br>
 * @param tol
 * tolerance.
 */
	virtual logical is_constant( double *rad_value = NULL ) const
	{
		SPAUNUSED( rad_value )
		return FALSE;
	}

	// save / restore:
/**
 * Saves the form or id, calls a <tt>save_data</tt>, then saves width data.
 */
	virtual void save() const;

/**
 * Restores the data for a <tt>var_rad_fixed_width</tt> from a save file.
 * <br><br>
 * <b>Role:</b>
 * <table width="100%">
 * <tr>
 * <td width="30%"><tt>var_radius::restore_data</tt></td>
 * <td width="70%">Save the underlying data</td>
 * </tr>
 * <tr>
 * <td width="30%"><tt>read_real</tt></td>
 * <td width="70%">radius width</td>
 * </tr>
 * </table>
 */
	virtual void restore_data();

// debug:
/**
 * Outputs a leader line and details about the <tt>var_rad_fixed_width</tt> to the debug file or to the specified file.
 * <br><br>
 * @param leader
 * leader string.
 * @param fp
 * file pointer.
 */
	virtual void debug( const char *leader, FILE *fp = debug_file_ptr ) const;
	};


// Declaration of the derived class var_rad_holdline

// There are only two abstract class functions in var_radius:
//    virtual logical zero_end_radius(...) and zero_end_deriv1(...).
//
// There are also several "convenience" functions (start_radius, end_radius,
// start_deriv1, and end_deriv1), which are hard to implement for implicit radius
// like this.


/*
// tbrv
*/
/**
 * @nodoc
 */
class DECL_KERN var_rad_holdline : public var_radius 
	{
private:

	// Instance of var_rad_holdline owns this curve!

	curve *crv_holdline; 

public:

/**
 * 	C++ allocation constructor requests memory for this object but does not populate it.
 */
    // Allow an empty constructor.  This is necessary
	// for restore_data(), and should only be used there.

	var_rad_holdline( curve *cu = NULL );

/**
 * C++ copy constructor requests memory for this object and populates it with the data from the object supplied as an argument.
 * <br><br>
 * @param old
 * old.
 */
	var_rad_holdline( var_rad_holdline const &old );

/**
 * C++ destructor, deleting a <tt>var_fixed_width</tt>.
 */
	virtual ~var_rad_holdline();

	// Identification.
/**
 * Returns the radius form.
 */
	virtual int form() const { return VAR_RAD_HOLDLINE; }

/**
 * Returns the string "holdline".
 */
	virtual char const *type_name() const;

	// Copy.
/**
 * Copies a variable radius.
 */
	virtual var_radius *copy() const;

    // Member data retrieval.

	curve *holdline() const { return crv_holdline; }

    // Note: these don't give accurate offset values.  See comments in base class.
	// I'm not sure if it's better to return something
	// acceptable (like 1.0) or to give back a negative
	// number indicating the problem.

/**
 * Returns the start radius.
 */
	virtual double start_radius() const { return -1.; }

/**
 * Returns the end radius.
 */
	virtual double end_radius() const { return -1.; }

/**
 * Returns the first derivative at the start parameter.
 */
	virtual double start_deriv1() const { return  0.; }

/**
 * Returns the first derivative at the end parameter.
 */
	virtual double end_deriv1() const { return  0.; }

/**
 * Returns <tt>TRUE</tt> if the radius at the start or end point of the <tt>var_radius</tt> is zero (less than <tt>SPAresabs</tt>).
 * <br><br>
 * @param at_start
 * at start point if true.
 * @param tol
 * tolerance.
 */
	virtual logical zero_end_radius( logical /*at_start*/, double /*tol*/ = SPAresabs ) const
		{ return FALSE; } // Can't know for sure without evaluating the blend.

/**
 * Returns <tt>TRUE</tt> if the first derivative at the start or end point of the <tt>var_radius</tt> is zero (less than <tt>SPAresabs</tt>).
 * <br><br>
 * @param at_start
 * at start point if true.
 * @param tol
 * tolerance.
 */
	virtual logical zero_end_deriv1( logical /*at_start*/, double /*tol*/ = SPAresabs ) const
		{ return FALSE; } // Can't know for sure without evaluating the blend.

/**
 * Transforms the variable radius by the given transformation.
 * <br><br>
 * @param trans
 * transformation.
 */
	virtual void operator*=( SPAtransf const &xfm ) { *crv_holdline *= xfm; }

	// utility:
/**
 * Implements an assignment operator, which makes a copy of a <tt>var_radius</tt>.
 * <br><br>
 * @param other
 * variable radius.
 */
	virtual logical operator==( var_radius const &other ) const;

/**
 * Determines whether two variable radii are not equal.
 * <br><br>
 * @param other
 * variable radius.
 */
	virtual logical operator!=( var_radius const &other ) const
		{ return !( *this == other ); }

/**
 * Returns <tt>TRUE</tt> if the radius function is not an explicit function of <i>v</i>.
 * <br><br>
 * <b>Role:</b> The function is not an explicit function if the blend surface
 * must be evaluated in order to calculate the radius value at a given v-parameter
 */
	virtual logical implicit_var_radius() const { return TRUE; }

	// Check whether a radius is zero everywhere, to a given tolerance.
/**
 * Returns <tt>TRUE</tt> if a radius function is zero everywhere, to within a given tolerance - else, it returns <tt>FALSE</tt>.
 * <br><br>
 * @param tol
 * zero tolerance.
 */
	virtual logical is_zero( double /* eps */ ) const
		{ return FALSE; }  // Can't know for sure without evaluating the blend.

/**
 * Determines whether or not the radius is constant.
 * <br><br>
 * @param radius_value
 * tolerance.
 */
	virtual logical is_constant( double *rad_value = NULL ) const 
	{ 
		SPAUNUSED( rad_value )
		return FALSE; 
	}

	// save / restore:
/**
 * Calls <tt>var_radius::save_data</tt> method, then saves the start and end radius data.
 */
	virtual void save() const;

/**
 * Restores the data for a <tt>var_rad_holdline</tt> from a save file.
 * <br><br>
 * <b>Role:</b>
 * <table width="100%">
 * <tr>
 * <td width="30%"><tt>var_radius::restore_data</tt></td>
 * <td width="70%">Save the underlying data</td>
 * </tr>
 * <tr>
 * <td width="30%"><tt>restore_curve</tt></td>
 * <td width="70%">holdline curve</td>
 * </tr>
 * </table>
 */
	virtual void restore_data();

// debug:
/**
 * Writes debug information about the variable radius to either standard output or to the specified file.
 * <br><br>
 * @param leader
 * leader string.
 * @param fp
 * file pointer.
 */
	virtual void debug( const char *leader, FILE *fp = debug_file_ptr ) const;
	};


// Declaration of the derived class var_rad_fixed_distance

// There are only two abstract class functions in var_radius:
//    virtual logical zero_end_radius(...) and zero_end_deriv1(...).
//
// There are also several "convenience" functions (start_radius, end_radius,
// start_deriv1, and end_deriv1), which are hard to implement for implicit radius
// like this.

/*
// tbrv
*/
/**
 * @nodoc
 */
class DECL_KERN var_rad_fixed_distance : public var_radius 
	{
private:

	// The distance between the spring curve and the defining curve.

	 double   distance_data;

public:

/**
 * 	C++ allocation constructor requests memory for this object but does not populate it.
 */
    // Allow an empty constructor.  This is necessary
	// for restore_data(), and should only be used there.

	var_rad_fixed_distance( double distance = 0. );

/**
 * C++ copy constructor requests memory for this object and populates it with the data from the object supplied as an argument.
 * <br><br>
 * @param old
 * old.
 */
	var_rad_fixed_distance( var_rad_fixed_distance const &old );

	// Get the fixed distance.
/**
 * Returns the fixed distance between the spring and the defining curve.
 */
	double distance() const { return distance_data; }

	// Identification.
/**
 * Returns the radius form.
 */
	virtual int form() const { return VAR_RAD_FIXED_DISTANCE; }

/**
 * Returns the string "fixed_distance".
 */
	virtual char const *type_name() const;

	// Copy.
/**
 * Copies a variable radius.
 */
	virtual var_radius *copy() const;	

    // Note: these don't give accurate offset values.  See comments in base class.
	// I'm not sure if it's better to return something
	// acceptable (like 1.0) or to give back a negative
	// number indicating the problem.

/**
 * Returns the start radius.
 */
	virtual double start_radius() const { return -1.; }
/**
 * Returns the end radius.
 */
	virtual double end_radius() const { return -1.; }
/**
 * Returns the first derivative at the start parameter.
 */
	virtual double start_deriv1() const { return  0.; }
/**
 * Returns the first derivative at the end parameter.
 */
	virtual double end_deriv1() const { return  0.; }

/**
 * Returns <tt>TRUE</tt> if the radius at the start or end point of the <tt>var_radius</tt> is zero (less than <tt>SPAresabs</tt>).
 * <br><br>
 * @param at_start
 * at start point if true.
 * @param tol
 * tolerance.
 */
	virtual logical zero_end_radius( logical /*at_start*/, double /*tol*/ = SPAresabs ) const
    { return FALSE; } // Can't know for sure without evaluating the blend.

/**
 * Returns <tt>TRUE</tt> if the first derivative at the start or end point of the <tt>var_radius</tt> is zero (less than <tt>SPAresabs</tt>).
 * <br><br>
 * @param at_start
 * at start point if true.
 * @param tol
 * tolerance.
 */
	virtual logical zero_end_deriv1( logical /*at_start*/, double /*tol*/ = SPAresabs ) const
    { return FALSE; } // Can't know for sure without evaluating the blend.

/**
 * Transforms the variable radius by the given transformation.
 * <br><br>
 * @param trans
 * transformation.
 */
	virtual void operator*=( SPAtransf const &xfm );

	// utility:
/**
 * Implements an assignment operator, which makes a copy of a <tt>var_radius</tt>.
 * <br><br>
 * @param other
 * variable radius.
 */
	virtual logical operator==( var_radius const &other ) const;

/**
 * Determines whether two variable radii are not equal.
 * <br><br>
 * @param other
 * variable radius.
 */
	virtual logical operator!=( var_radius const &other ) const
		{ return !( *this == other ); }

/**
 * Returns <tt>TRUE</tt> if the radius function is not an explicit function of <i>v</i>.
 * <br><br>
 * <b>Role:</b> The function is not an explicit function if the blend surface
 * must be evaluated in order to calculate the radius value at a given v-parameter
 */
	virtual logical implicit_var_radius() const { return TRUE; }

	// Check whether a radius is zero everywhere, to a given tolerance.

/**
 * Returns <tt>TRUE</tt> if a radius function is zero everywhere, to within a given tolerance - else, it returns <tt>FALSE</tt>.
 * <br><br>
 * @param tol
 * zero tolerance.
 */
	virtual logical is_zero( double /* eps */ ) const
    { return FALSE; }  // Can't know for sure without evaluating the blend.

/**
 * Determines whether or not the radius is constant.
 * <br><br>
 * @param radius_value
 * tolerance.
 */
	virtual logical is_constant( double *rad_value = NULL ) const 
	{
		SPAUNUSED( rad_value )
		return FALSE; 
	}

	// save / restore:
/**
 * Calls <tt>var_radius::save_data</tt> method, then saves the start and end radius data.
 */
	virtual void save() const;

/**
 * Restores the data for a <tt>var_rad_fixed_distance</tt> from a save file.
 * <br><br>
 * <b>Role:</b>
 * <table width="100%">
 * <tr>
 * <td width="30%"><tt>var_radius::restore_data</tt></td>
 * <td width="70%">Save the underlying data</td>
 * </tr>
 * <tr>
 * <td width="30%"><tt>read_real</tt></td>
 * <td width="70%">fixed distance</td>
 * </tr>
 * </table>
 */
	virtual void restore_data();

// debug:
/**
 * Writes debug information about the variable radius to either standard output or to the specified file.
 * <br><br>
 * @param leader
 * leader string.
 * @param fp
 * file pointer.
 */
	virtual void debug( const char *leader, FILE *fp = debug_file_ptr ) const;
	};

// Declaration of the derived class var_rad_three_entity. Do not export this class
// as this code is temporary.

/*
// tbrv
*/
/**
 * @nodoc
 */
class DECL_KERN var_rad_three_entity : public var_radius 
	{
public:

/**
 * 	C++ allocation constructor requests memory for this object but does not populate it.
 */
    // Allow an empty constructor.  This is necessary
	// for restore_data(), and should only be used there.

	var_rad_three_entity( bs2_curve rad_bs1 = NULL );

/**
 * C++ copy constructor requests memory for this object and populates it with the data from the object supplied as an argument.
 * <br><br>
 * @param orig
 * orig.
 */
	var_rad_three_entity( var_rad_three_entity const &orig );

    // The assignment operator, not trivial.

    var_rad_three_entity &operator=( var_rad_three_entity const &rhs );

/**
 * C++ destructor, deleting a <tt>var_rad_three_entity</tt>.
 */
    ~var_rad_three_entity();

    // Get the approximate radius variation of the three entity
    // blend.
/**
 * Returns the approximate radius variation of the three entity
 * blend.
 */
	bs2_curve radius_bs1() const { return _radius_bs1; }

	// Set the approximate radius variation.
/**
 * Set the approximate radius variation of the three entity blend.
 */
	void set_radius_bs1( bs2_curve rad_bs1 );

	// Access to the position evaluator of three entity blends.

	teb_pos_evaluator *get_position_evaluator();

	// Access to the derivative evaluator of three entity blends.

	teb_deriv_evaluator *get_deriv_evaluator();

	// Identification.
/**
 * Returns the radius form.
 */
	virtual int form() const { return VAR_RAD_THREE_ENTITY; }

/**
 * Returns the string "three_entity".
 */
	virtual char const *type_name() const;

	// Copy.
/**
 * Copies a variable radius.
 */
	virtual var_radius *copy() const;	

    // Note: these don't give accurate offset values.  See comments in base class.
	// I'm not sure if it's better to return something
	// acceptable (like 1.0) or to give back a negative
	// number indicating the problem.

/**
 * Returns the start radius.
 */
	virtual double start_radius() const { return -1.; }
/**
 * Returns the end radius.
 */
	virtual double end_radius() const { return -1.; }
/**
 * Returns the first derivative at the start parameter.
 */
	virtual double start_deriv1() const { return  0.; }
/**
 * Returns the first derivative at the end parameter.
 */
	virtual double end_deriv1() const { return  0.; }

/**
 * Returns <tt>TRUE</tt> if the radius at the start or end point of the <tt>var_radius</tt> is zero (less than <tt>SPAresabs</tt>).
 * <br><br>
 * @param at_start
 * at start point if true.
 * @param tol
 * tolerance.
 */
	virtual logical zero_end_radius( logical /*at_start*/, double /*tol*/ = SPAresabs ) const
    { return FALSE; } // Can't know for sure without evaluating the blend.

/**
 * Returns <tt>TRUE</tt> if the first derivative at the start or end point of the <tt>var_radius</tt> is zero (less than <tt>SPAresabs</tt>).
 * <br><br>
 * @param at_start
 * at start point if true.
 * @param tol
 * tolerance.
 */
	virtual logical zero_end_deriv1( logical /*at_start*/, double /*tol*/ = SPAresabs ) const
    { return FALSE; } // Can't know for sure without evaluating the blend.

/**
 * Transforms the variable radius by the given transformation.
 * <br><br>
 * @param trans
 * transformation.
 */
	virtual void operator*=( SPAtransf const &xfm );

	// utility:
/**
 * Implements an assignment operator, which makes a copy of a <tt>var_radius</tt>.
 * <br><br>
 * @param other
 * variable radius.
 */
	virtual logical operator==( var_radius const &other ) const;

/**
 * Determines whether two variable radii are not equal.
 * <br><br>
 * @param other
 * variable radius.
 */
	virtual logical operator!=( var_radius const &other ) const
		{ return !( *this == other ); }

/**
 * Returns <tt>TRUE</tt> if the radius function is not an explicit function of <i>v</i>.
 * <br><br>
 * <b>Role:</b> The function is not an explicit function if the blend surface
 * must be evaluated in order to calculate the radius value at a given v-parameter
 */
	virtual logical implicit_var_radius() const { return TRUE; }

	// Check whether a radius is zero everywhere, to a given tolerance.

/**
 * Returns <tt>TRUE</tt> if a radius function is zero everywhere, to within a given tolerance - else, it returns <tt>FALSE</tt>.
 * <br><br>
 * @param tol
 * zero tolerance.
 */
	virtual logical is_zero( double /* eps */ ) const
    { return FALSE; }  // Can't know for sure without evaluating the blend.

/**
 * Determines whether or not the radius is constant.
 * <br><br>
 * @param radius_value
 * tolerance.
 */
	virtual logical is_constant( double *rad_value = NULL ) const 
	{
		SPAUNUSED( rad_value )
		return FALSE; 
	}

	// save / restore:
/**
 * Calls <tt>var_radius::save_data</tt> method, then saves the start and end radius data.
 */
	virtual void save() const;

/**
 * Restores the data for a <tt>var_rad_fixed_distance</tt> from a save file.
 * <br><br>
 * <b>Role:</b>
 * <table width="100%">
 * <tr>
 * <td width="30%"><tt>var_radius::restore_data</tt></td>
 * <td width="70%">Save the underlying data</td>
 * </tr>
 * <tr>
 * <td width="30%"><tt>read_real</tt></td>
 * <td width="70%">fixed distance</td>
 * </tr>
 * </table>
 */
	virtual void restore_data();

// debug:
/**
 * Writes debug information about the variable radius to either standard output or to the specified file.
 * <br><br>
 * @param leader
 * leader string.
 * @param fp
 * file pointer.
 */
	virtual void debug( char const *leader, FILE *fp = debug_file_ptr ) const;

private:

    // The approximate radius variation of the three entity blend.

    bs2_curve _radius_bs1;

	// The position evaluator for three entity blends.

	teb_pos_evaluator *_rlx;

	// The derivative evaluator for three entity blends.

	teb_deriv_evaluator *_d_calc;
	};

// Helper functions
/*
// tbrv
*/
/**
 * @nodoc
 */
DECL_KERN logical bs2_radius_is_zero( bs2_curve bs2, double eps );

/*
// tbrv
*/
/**
 * @nodoc
 */
DECL_KERN var_radius *restore_radius();

/*
// tbrv
*/
/**
 * @nodoc
 */
DECL_KERN var_radius *var_radius_connect( var_radius *, var_radius * );

/** @} */
#endif
