/*******************************************************************/
/*    Copyright (c) 1989-2020 by Spatial Corp.                     */
/*    All rights reserved.                                         */
/*    Protected by U.S. Patents 5,257,205; 5,351,196; 6,369,815;   */
/*                              5,982,378; 6,462,738; 6,941,251    */
/*    Protected by European Patents 0503642; 69220263.3            */
/*    Protected by Hong Kong Patent 1008101A                       */
/*******************************************************************/

#ifndef LAW_BASE_HXX
#define LAW_BASE_HXX

#include "base.hxx"
#include "dcl_law.h"
#include "logical.h"
#include "option.hxx"
#include <float.h>
/**
 * @file law_base.hxx
 * @CAA2Level L1
 * @CAA2Usage U2
 * \addtogroup ACISKERNLAWS
 *
 * @{
 */

#ifdef INTERNAL_DEBUG_CHECKS
DECL_LAW extern option_header fast_euler_false_level;
#endif // INTERNAL_DEBUG_CHECKS

class complex_number;
class SPAvector;
class SPAunit_vector;
class SPAposition;
class SPApar_pos;
class law_data;
class law_data_node;
class law_domain;
class law_polynomial;
class SPAinterval;
class SPAnvector;
class SPAbox;
class base_pointer_map;
class SPAmatrix;
class SPAtransf;
class VOID_LIST;
class SizeAccumulator;
/**
 * Law type
 */
#define LAW_TYPE_UNKNOWN      0
/**
 * Law type
 */
#define LAW_TYPE_ZERO         1
/**
 * Law type
 */
#define LAW_TYPE_INTEGER      2
/**
 * Law type
 */
#define LAW_TYPE_CONSTANT     3
/**
 * Law type
 */
#define LAW_TYPE_LINEAR       4
/**
 * Law type
 */
#define LAW_TYPE_POLYNOMIAL   5
/**
 * Law type
 */
#define LAW_TYPE_G_INFINITY   6
/**
 * Law type
 */
#define LAW_TYPE_G_5          7
/**
 * Law type
 */
#define LAW_TYPE_G_4          8
/**
 * Law type
 */
#define LAW_TYPE_G_3          9
/**
 * Law type
 */
#define LAW_TYPE_G_2          10
/**
 * Law type
 */
#define LAW_TYPE_G_1          11
/**
 * Law type
 */
#define LAW_TYPE_CONTINUIOUS  12
/**
 * Law type
 */
#define LAW_TYPE_BOUNDED      13
/**
 * Law type
 */
#define LAW_TYPE_RATIONAL     14
/**
 * @nodoc
 */
typedef enum
	{
	ORIGINAL = -1,
	DEFAULT,
	MAPLE
	}
law_symbol_type;

/**
 * @nodoc
 */
typedef enum
{
	INTEGRATE_LAW_ID,
	SHIFT_LAW_ID,
	LAW_ID,
	UNARY_LAW_ID,
	NOT_LAW_ID,
	EVEN_LAW_ID,
	ODD_LAW_ID,
	INT_LAW_ID,
	PRIME_LAW_ID,
	SET_LAW_ID,
	NEGATE_LAW_ID,
	NATURAL_LOG_LAW_ID,
	EXP_LAW_ID,
	FLOOR_LAW_ID,
	CEIL_LAW_ID,
	RAND_LAW_ID,
	SIN_LAW_ID,
	COS_LAW_ID,
	TAN_LAW_ID,
	COT_LAW_ID,
	SEC_LAW_ID,
	CSC_LAW_ID,
	ARCSIN_LAW_ID,
	ARCCOS_LAW_ID,
	ARCCOT_LAW_ID,
	ARCSEC_LAW_ID,
	ARCCSC_LAW_ID,
	SINH_LAW_ID,
	COSH_LAW_ID,
	TANH_LAW_ID,
	COTH_LAW_ID,
	SECH_LAW_ID,
	CSCH_LAW_ID,
	ARCSINH_LAW_ID,
	ARCCOSH_LAW_ID,
	ARCTANH_LAW_ID,
	ARCCOTH_LAW_ID,
	ARCSECH_LAW_ID,
	ARCCSCH_LAW_ID,
	ABS_LAW_ID,
	SQRT_LAW_ID,
	NORM_LAW_ID,
	FRENET_LAW_ID,
	SURFNORM_LAW_ID,
	SIZE_LAW_ID,
	UNARY_DATA_LAW_ID,
	GUASSIAN_CURVATURE_LAW_ID,
	MEAN_CURVATURE_LAW_ID,
	MAX_CURVATURE_LAW_ID,
	MIN_CURVATURE_LAW_ID,
	BS3_SURFACE_LAW_ID,
	WIRE_LAW_ID,
	CURVATURE_LAW_ID,
	INTERSECT_LAWI_ID,
	DISTANCE_LAWI_ID,
	CURVE_LAW_ID,
	PCURVE_LAW_ID,
	SURFACE_LAW_ID,
	PATTERNTRANS_LAW_ID,
	CONSTANT_LAW_ID,
	IDENTITY_LAW_ID,
	MULTIPLE_LAW_ID,
	BINARY_LAW_ID,
	MULTIPLE_DATA_LAW_ID,
	PI_LAW_ID,
	E_LAW_ID,
	TRUE_LAW_ID,
	FALSE_LAW_ID,
	DERIVATIVE_LAW_ID,
	MEAN_LAW_ID,
	MAX_LAW_ID,
	MIN_LAW_ID,
	VECTOR_LAW_ID,
	TERM_LAW_ID,
	DOMAIN_LAW_ID,
	PERMANENT_DOMAIN_LAW_ID,
	DOT_LAW_ID,
	ARCTAN_LAW_ID,
	CROSS_LAW_ID,
	LOG_LAW_ID,
	MOD_LAW_ID,
	TWIST_PATH_LAW_ID,
	SURFVEC_LAW_ID,
	STEP_LAW_ID,
	PIECEWISE_LAW_ID,
	MIN_ROTATION_LAW_ID,
	BEND_LAW_ID,
	DBEND_LAW_ID,
	UNBEND_LAW_ID,
	PLUS_LAW_ID,
	MINUS_LAW_ID,
	TIMES_LAW_ID,
	DIVISION_LAW_ID,
	EXPONENT_LAW_ID,
	COMPOSITE_LAW_ID,
	LESS_THAN_LAW_ID,
	GREATER_THAN_LAW_ID,
	LESS_THAN_OR_EQUAL_LAW_ID,
	GREATER_THAN_OR_EQUAL_LAW_ID,
	EQUAL_LAW_ID,
	NOT_EQUAL_LAW_ID,
	AND_LAW_ID,
	OR_LAW_ID,
	MAP_LAW_ID,
	DCURVE_LAW_ID,
	DPCURVE_LAW_ID,
	DWIRE_LAW_ID,
	DSURFACE_LAW_ID,
	TRANSFORM_LAW_ID,
	ROTATE_LAW_ID,
	SURFPERP_LAW_ID,
	CURVEPERP_LAW_ID,
	LENGTH_PARAM_LAW_ID,
	LENGTH_LAW_ID,
	CONIC_LINE_LAW_ID,
	CONIC_TROUGH_LINE_LAW_ID,
	CYCLIDE_LAW_ID,
	DCYCLIDE_LAW_ID,
	CYCLIDE_PERP_LAW_ID,
	MULTIPLE_CURVE_LAW_ID,
	MULTIPLE_CURVEPERP_LAW_ID,
	SCALETRANS_LAW_ID,
	VECTRANS_LAW_ID,
	POSTRANS_LAW_ID,
	CURVECLOSEST_LAW_ID,
	SIMPLE_HELIX_LAW_ID,
	SIMPLE_RAIL_LAW_ID,
	CURVES_DIST_LAW_ID,
	TWIST_INTERP_LAW_ID
} elaw_id;

/**
 * Serves as the base class for all derived law classes.
 * <br>
 * <b>Role:</b> The <tt>law</tt> class is the base class from which all other law classes
 * are derived. Laws are functions from <i>n</i> dimensional Euclidean space to
 * <i>m</i> dimensional Euclidean space. The <tt>law</tt> class provides virtual methods used
 * by all derived law class types.
 * <br><br>
 * A law is represented internally by a tree of C++ classes that know their
 * dimensions, how to evaluate themselves, and how to take their exact
 * (symbolic) derivatives with respect to any combination of variables.
 * Laws provide the ability to solve complex, global mathematical problems.
 * <br><br>
 * Laws are parsed in the same way that equations are presented in mathematics
 * textbooks. For example, the equation <tt>f(x,y) = x&sup2+cos(x)-sin(y)</tt> becomes the
 * law function <tt>"X^2+COS(X)-SIN(Y)"</tt>, with a two-dimensional domain and a
 * one-dimensional range.
 * <br><br>
 * For efficient memory management, the law classes are use-counted.  That is,
 * a data member called <tt>use_count</tt> is maintained in order to keep track
 * of how many pointers to a particular law object are in current use.  This
 * integer is incremented and decremented by means of the <tt>add</tt> and
 * <tt>remove</tt> methods.  Accordingly, when an application is finished
 * with a law, the <tt>remove</tt> method should be called instead of
 * the class destructor (or <tt>delete</tt>). The <tt>remove</tt> method
 * itself takes care of deleting the law when <tt>use_count</tt> falls to zero.
 * <br><br>
 * For example, consider the law <i>f</i> representing <tt>"X^2"</tt>, which is actually
 * an <tt>exponent_law</tt> whose arguments are an <tt>identity_law</tt> for <i>x</i> and a <tt>constant_law</tt>
 * for <tt>2</tt>. The new <i>df</i> law <tt>("2*X")</tt> created using the <tt>derivative</tt> method
 * of <i>f</i> is a <tt>times_law</tt> whose arguments are a <tt>constant_law</tt> for <tt>2</tt> and an
 * <tt>identity_law</tt> for <i>x</i>. In constructing the <i>df</i> law,
 * instead of creating new instances of the <tt>constant_law</tt> representing <tt>2</tt> and the
 * <tt>identity_law</tt> representing <i>x</i>, the constructor calls
 * their <tt>add</tt> methods, which increments their respective use counts. In this manner,
 * memory is not taken up with duplicates of the <tt>constant_law</tt> for <tt>2</tt> or the
 * <tt>identity_law</tt> for <i>x</i>. If the <tt>remove</tt> method of either <i>f</i> or
 * <i>df</i> is called, the <tt>remove</tt> methods of their sublaws are also called.
 * The sublaws decrement their own <tt>use_count</tt> members, and only actually call
 * the <tt>law::~law</tt> destructor if <tt>use_count</tt> goes to zero.
 * <br><br>
 * For convenience, the <tt>law</tt> class has a number of evaluation methods. The naming
 * convention for these is <tt>evaluateX_Y</tt>, where <tt>X</tt> refers to the input argument type
 * and <tt>Y</tt> refers to the output.  They can be any of the following:
 * <br><br>
 * <table width= 100%>
 * <tr width = 100%><td width = 10%>R </td>  <td> double (real) </td> </tr>
 * <tr><td>M </td>  <td> double* (multiple)</td> </tr>
 * <tr><td>D </td>  <td> derivative</td> </tr>
 * <tr><td>V </td>  <td> SPAvector</td> </tr>
 * <tr><td>P </td>  <td> SPAposition</td> </tr>
 * <tr><td>PP</td>  <td> SPApar_pos</td> </tr>
 * <tr><td>NV</td>  <td> SPAnvector</td> </tr>
 * <tr><td>T </td>  <td> SPAtransf</td> </tr>
 * <tr><td>MAT</td> <td> SPAmatrix</td> </tr>
 * <tr><td>C  </td> <td> complex</td> </tr>
 * </table>
 * <br>
 * The <tt>use_count</tt> private data member stores the number of copies of this law that
 * are in use. The private data member <tt>simplified_flag</tt> is set if this law cannot
 * be simplified any more.
 * <br><br>
 * The <tt>type_flag</tt> is any one of the following values;
 * <br><br>
 * <table width= 100%>
 * <tr width = 100%><td width = 10%>0</td> <td> unknown </td></tr>
 * <tr><td>1</td> <td> zero    </td></tr>
 * <tr><td>2</td> <td> constant</td></tr>
 * <tr><td>3</td> <td> linear  </td></tr>
 * <tr><td>4</td> <td> polynomial</td></tr>
 * <tr><td>            G infinity </td></tr>
 * <tr><td>6</td> <td>    G 5 </td></tr>
 * <tr><td>7</td> <td>    G 4 </td></tr>
 * <tr><td>8</td> <td>    G 3 </td></tr>
 * <tr><td>9</td> <td>    G 2 </td></tr>
 * <tr><td>     G 1           </td></tr>
 * <tr><td>     Continuous    </td></tr>
 * <tr><td>     Bounded       </td></tr>
 * <tr><td>     Rational      </td></tr>
 * </table>
 * <br>
 * If a law is more than one type, the lower number is used. The <tt>not_type_flag</tt>
 * tells what a law is known not to be.
 * <br><br>
 * An application that needs to derive a law that is not already a part of ACIS should
 * do so from one of the big six law classes, which establish rules for parsing:
 * <tt>constant_law</tt>, <tt>unary_law</tt>, <tt>binary_law</tt>, <tt>multiple_law</tt>, <tt>unary_data_law</tt>, or
 * <tt>multiple_data_law</tt>.  All derived laws must have the following methods or inherit them:
 * <br><br>
 * <table width="70%">
 * <tr> <td>virtual law*   </td>  <td>derivative(int which=0) const;</td> </tr>
 * <tr> <td>virtual void   </td>  <td>evaluate(const double* x, double* answer) const;</td> </tr>
 * <tr> <td>virtual logical</td>  <td>isa(int t) const;</td> </tr>
 * <tr> <td>static  int    </td>  <td>id();</td> </tr>
 * <tr> <td>virtual int    </td>  <td>type() const;</td> </tr>
 * <tr> <td>virtual int    </td>  <td>return_dim() const;</td> </tr>
 * <tr> <td>virtual int    </td>  <td>take_dim() const;</td> </tr>
 * </table>
 * @see arctan_law, bend_law, binary_law, frenet_law, law_law_data, multiple_law, norm_law, size_law, surfnorm_law, twist_path_law, unary_law, unbend_law
 */
class DECL_LAW law : public ACIS_OBJECT
{
private:
	friend class law_list;

	mutable int		reference_count;		  // Number of copies
	mutable int     type_flag;				  // Cached what it is
	mutable int     not_type_flag;			  // Cached what it is not
	void            set_dlaw_cache(int) const; // Create sufficient storage for dlaw cache

protected:

/**
 * Holds the cache value of the most simplified version of this law.
 */
	mutable law* slaw;				      // Cached simplified form
/**
 * Tells what level of simplification was used to create the law held by <tt>slaw</tt>.
 * <br><br>
 * <i><b>Note:</b> See <tt>simplify</tt> for a discussion of the levels.</i>
 */
	mutable int slevel;				      // Level used in slaw
/**
 * Holds the cache derivatives with respect to each of the respective input variables.
 */
	mutable law** dlaw;					  // Cached dx,dy,dz,...
/**
 * Tells how many cache derivatives are held by <tt>dlaw</tt>.
 */
	mutable int dlaw_size;				  // Number of cached dlaws
/**
 * Holds the cache of the inverse law.
 */
	mutable law* ilaw;					  // Cached inverse law
/**
 * Specifies the cached domain dimension.
 */
	mutable int take_val;				  // Cached domain dimension
/**
 * Specifies the cached range dimension.
 */
	mutable int return_val;				  // Cached range dimension

	// If the following methods are given the result is auto cached.

/**
 * Returns a law that is a derivative of this law.
 * <br><br>
 * <b>Role:</b> This method returns a law pointer that is the derivative of the given
 * law with respect to the <tt>which</tt> variable. Variables in C++ are numbered starting at
 * zero (0). The default is to take a derivative with respect to the first variable,
 * which in a law function string is <tt>A1</tt> or <tt>X</tt>. The variables <tt>X</tt>, <tt>Y</tt>, and <tt>Z</tt> are equivalent
 * to the indices 0, 1, and 2, respectively.
 * <br><br>
 * The <tt>deriv</tt> method implements the code to
 * perform the actual derivative calculation and caches its value in memory. All classes
 * derived from law (or its children) must implement their own <tt>deriv</tt> method.
 * <br><br>
 * The <tt>deriv</tt> method should <i>not</i> be called directly by applications. Applications
 * should call the <tt>derivative</tt> method instead, which is inherited by all classes
 * derived from <tt>law</tt>. The <tt>derivative</tt> method accesses the cached derivative value
 * in memory, if one exists; otherwise it calls the <tt>deriv</tt> method.
 * <br><br>
 * @param which
 * index of the variable with respect to which the derivative is taken (default X or A1).
 */
	virtual law* deriv(int which = 0) const; // Instead of derivative()
/**
 * Returns a pointer to the sublaws that are used to make up the inverse law of this class.
 */
	virtual law* sub_inverse() const;	  // Instead of inverse()
/**
 * Returns the dimension of this law's domain (input).
 * <br><br>
 * <b>Role:</b> The default is 1. All derived law classes must have this method or inherit it.
 */
	virtual int take_size() const;		  // Instead of take_dim()
/**
 * Returns the dimension of this law's range (output).
 * <br><br>
 * <b>Role:</b> The <tt>return_size</tt> tells how many values are returned in the <tt>answer</tt>
 * argument of the <tt>evaluate</tt> method.
 * The default is 1. All derived law classes must have this method
 * or inherit it.
 */
	virtual int return_size() const;		  // Instead of return_dim()

	virtual ~law();

public:
/**
 * Specifies the number of laws in existence at any given time.
 */
	static int how_many_laws;	// Total number of laws
/**
 * Constructs a law.
 * <br><br>
 * <b>Role:</b> This sets the <tt>use_count</tt> to 1 and increments the static member
 * <tt>how_many_laws</tt>. It sets <tt>dlaw</tt>, <tt>slaw</tt>, and <tt>lawdomain</tt> to <tt>NULL</tt>.
 */
	law();
/**
 * Returns a law that is a derivative of this law.
 * <br><br>
 * <b>Role:</b> This method returns a law pointer that is the derivative of the given
 * law with respect to the <tt>which</tt> variable. Variables in C++ are numbered
 * starting at zero (0). The default
 * is to take a derivative with respect to the first variable, which in a law
 * function string is <tt>A1</tt> or <tt>X</tt>. The variables <tt>X</tt>, <tt>Y</tt>, and <tt>Z</tt> are equivalent to the
 * indices 0, 1, and 2, respectively.
 * <br><br>
 * This method calls the <tt>deriv</tt> method to perform the actual derivative calculation.
 * Once the derivative has been calculated, it is stored for later use. Whenever
 * the <tt>derivative</tt> method is called, it determines if the derivative has already
 * been calculated and, if so, retrieves this stored information rather than
 * calling <tt>deriv</tt> again.
 * <br><br>
 * The <tt>derivative</tt> method is inherited by all classes derived from <tt>law</tt>. All classes
 * derived from <tt>law</tt> (or its children) must implement their own <tt>deriv</tt> method to
 * perform the actual derivative calculation when called by <tt>derivative</tt>.
 * <br><br>
 * Applications should call <tt>derivative</tt> and <i>not</i> <tt>deriv</tt>.
 * <br><br>
 * @param which
 * index of the variable with respect to which the derivative is taken (default X or A1).
 */
	virtual law* derivative(int which = 0) const;

	// Methods for managing cached derivatives
	// In set_derivative the no_circular_reference should be true only if
	// the derivative of the law does not refer to the law it is the derivative of.
	// An example is a numeric derivative of a law that uses the law.
/**
 * Returns <tt>TRUE</tt> or <tt>FALSE</tt>, depending upon whether or not the given derivative has been cached.
 * <br><br>
 * @param deriv
 * index of the derivative to check.
 */
	logical cached_derivative(int deriv) const;             // Is the derivative cached
/**
 * Clears out the specified numbered derivative of this law, erasing the associated cached law.
 * <br><br>
 * @param deriv
 * derivative to set.
 */
	void reset_derivative(int deriv);		             // Erase cached law
/**
 * Returns whether or not this law has a specified number of derivatives.
 * <br><br>
 * @param w
 * number of derivatives.
 */
	logical has_derivative(int w = 0) const;
/**
 * Establishes a pointer to a law which represents the specified derivative of this law.
 * <br><br>
 * <b>Role:</b> This is useful if the calculation derivative is rather complex
 * and it is known at the onset what it is. If a circular reference "might" exists
 * (for example, if <tt>in_law</tt> is a numerical derivative, and references the <tt>this_law</tt>)
 * then <tt>set_derivative</tt> does a <tt>deep_copy</tt> of <tt>in_law</tt> to break such a bad cycle.
 * This takes more time, but is safer (the default). If you know that no such
 * cycles exist, pass in <tt>TRUE</tt> for the last parameter, and you will get better performance.
 * <br><br>
 * @param deriv
 * derivative to set.
 * @param in_law
 * pointer to its law.
 * @param no_circular_reference
 * no circular reference exists.
 */
	logical set_derivative(int deriv, law* in_law,
            logical no_circular_reference = FALSE) const;

	// The basic four virtual evaluate methods.
	// In evaluate_with_side -1 is the left side, 1 is the right side, and 0 is the (default)
/**
 * Evaluates this law at the given input.
 * <br><br>
 * <b>Role:</b> This method takes two pointers to memory that the caller is
 * responsible for creating and freeing.  The <i>x</i> argument tells where to evaluate
 * the law. This can be more than one dimension. The <tt>answer</tt> argument returns the
 * evaluation. This can be more than one dimension. The array <i>x</i> should be of the
 * size returned by the <tt>take_dim</tt> method, and answer should be of the size returned
 * by the <tt>return_dim</tt> method. All derived law classes must have this method or
 * inherit it.  This does no checking of the dimension of input and output arguments. It
 * is preferable to call a more specific evaluator, if possible.
 * <br><br>
 * @param x
 * array of values at which to perform the evaluation.
 * @param answer
 * array of answer values resulting from the evaluation.
 */
	virtual void evaluate(const double* x, double* answer) const;
/**
 * Evaluates this law on a specified side of the input value.
 * <br><br>
 * <b>Role:</b> If the input value is an endpoint, this can be important for
 * numerical optimization.
 * <br><br>
 * @param x
 * array of values at which to perform the evaluation.
 * @param answer
 * answer.
 * @param side
 * left (-1) or right (1) - default is zero.
 */
	virtual void evaluate_with_side(const double* x, double* answer, const int* side) const;
/**
 * Evaluates this law using a best guess as to the answer (to minimize processing).
 * <br><br>
 * @param x
 * array of values at which to perform the evaluation.
 * @param answer
 * answer.
 * @param guess
 * best guess (optional).
 */
	virtual void evaluate_with_guess(const double* x, double* answer, const double* guess) const;
/**
 * Evaluates this law at an array of real values and returns an array of real values as the result.
 * <br><br>
 * <b>Role:</b> Works on laws that accept an array of real values and return an array
 * of real results.  (For convenience, all law classes inherit this method.) It calls
 * the main <tt>evaluate</tt> member function and does some checking using <tt>take_dim</tt>
 * and <tt>return_dim</tt>.
 * <br><br>
 * @param x
 * array of values at which to perform the evaluation.
 * @param y
 * output array.
 * @param size
 * return dimension of output array.
 */
	virtual void evaluateVAR(const double* x,  double*& y, int& size) const;
/**
 * Indicates whether or not this law is of, or derived from, a specified law type.
 * <br><br>
 * <b>Role:</b> All derived law classes must have this method. The <tt>isa</tt>,
 * <tt>id</tt>, and <tt>type</tt> methods are used to identify a law's class
 * type. The methods should be the same for all law classes with the exception
 * of the <tt>isa</tt> method, which calls the <tt>isa</tt> method of its parent
 * class.  For example, the call <tt>test_law->isa(constant_law::id())</tt>
 * returns <tt>TRUE</tt> if <tt>test_law</tt> refers to a <tt>constant_law</tt> object, or to an object derived
 * from the <tt>constant_law</tt> class.
 * <br><br>
 * @param t
 * id method return.
 */
	virtual logical isa(int t) const;
/**
 * Returns a unique ID that may be used to identify a law's class type.
 * <br><br>
 * <b>Role:</b> This method should not be called directly by the application.
 * All derived law classes must have this method.
 * The <tt>isa</tt>, <tt>id</tt> and <tt>type</tt> methods are used to identify a law's class type.
 */
	static int id();
/**
 * Identifies this law's class type.
 * <br><br>
 * <b>Role:</b> All derived law classes must have this method. The <tt>isa</tt>,
 * <tt>id</tt> and <tt>type</tt> methods are used to identify a law's class type.
 * The methods should be the same for all law classes.
 */
	virtual int type() const;
/**
 * Returns a string that contains the name of this law's class.
 * <br><br>
 * <b>Role:</b> It is provided as a user-friendly interface to laws.
 */
	virtual const char* class_name() = 0;
/**
 * Returns the dimension of this law's range (output).
 * <br><br>
 * <b>Role:</b> The <tt>return_size</tt> tells how many values are returned in the <tt>answer</tt>
 * argument of the <tt>evaluate</tt> method.
 * The default is 1. All derived law classes must have this method
 * or inherit it.
 */
	virtual int return_dim() const;					// Default is 1
/**
 * Returns the dimension of this law's domain (input).
 * <br><br>
 * <b>Role:</b> The default is 1. All derived law classes must have this method
 * or inherit it.
 */
	virtual int take_dim() const;					// Default is 1

	// Convenience functions

/**
 * Returns a law that is the <i>n</i>th derivative of this law with respect to its first variable (e.g., <i>x</i>).
 * <br><br>
 * @param n
 * number of derivatives.
 */
	law* nth_derivative(int n) const;
/**
 * Returns a law that is a partial derivative in <tt>uv</tt> of this law.
 * <br><br>
 * <b>Role:</b> This is a convenience function. Instead of calling the <tt>derivative</tt>
 * method repeatedly when taking a number of partial derivatives with respect to
 * <i>u</i> and a number of partial derivatives with respect to <i>v</i>, you may
 * use this method. If a given derivative has not already been calculated, it is
 * calculated and cached. If it has been calculated, the cached value is used.
 * <br><br>
 * @param u_num
 * number of derivatives with respect to u.
 * @param v_num
 * number of derivatives with respect to v.
 */
	law* uv_partials(int u_num, int v_num) const;

	// Free evaluators
	//
	//		R	double (real)
	//		M	double * (multiple)
	//		D	derivative
	//		V	SPAvector
    //      UV  unit SPAvector
	//		P	SPAposition
	//		PP	SPApar_pos
	//		NV  SPAnvector
	//		C   complex
	//      B   SPAbox
    //      MAT SPAmatrix
    //      T   SPAtransf
/**
 * Evaluates this law at a real input and returns a real result.
 * <br><br>
 * <b>Role:</b> Works on laws that take and return one-dimensional (real) values. (All law classes
 * inherit this method, for convenience.) It calls the main <tt>evaluate</tt> member
 * function, first making sure that a quantity of no more than one dimension is taken or
 * returned by calling <tt>take_dim</tt> and <tt>return_dim</tt>.
 * <br><br>
 * If the input value is an endpoint, be sure to specify which side to use for evaluation.
 * <br><br>
 * <tt>law::eval()</tt> is an ACIS direct interface function that does not involve entities. 
 * Therefore, it must be surrounded either by an <tt>API</tt> block or an <tt>EXCEPTION</tt> 
 * block (<tt>EXCEPTION</tt> block is allowed becaus it does not involve entities).
 * <br><br>
 * @param x
 * value at which to perform the evaluation.
 * @param side
 * left(-1) or right (1) - 0 is the default.
 * @param guess
 * best guess (optional).
 */
	double eval(double x, const int& side = *(int*)NULL_REF, const double& guess = *(double*)NULL_REF) const;
/**
 * Evaluates this law at an array of real inputs and returns a real result.
 * <br><br>
 * <b>Role:</b> Works on laws that have a multi-dimensional real domain and return
 * one-dimensional real values. (For convenience, all law classes
 * inherit this method.) It calls the main <tt>evaluate</tt> member
 * function, and does some checking using
 * <tt>take_dim</tt> and <tt>return_dim</tt>.
 * <br><br>
 * If the input value is an endpoint, be sure to specify which side to use for evaluation.
 * <br><br>
 * @param x
 * array of values at which to perform the evaluation.
 * @param side
 * left(-1) or right (1) - 0 is default.
 * @param guess
 * best guess (optional).
 */
	double evaluateM_R(const double* x, const int* side = NULL, const double* guess = NULL) const;
/**
 * Evaluates the <i>n</i>th derivative of this law at a real input and returns a real result.
 * <br><br>
 * <b>Role:</b> Works on laws that have both a one-dimensional real domain and a
 * one-dimensional real range. (For convenience, all law classes inherit this method.)
 * It calls the main <tt>evaluate</tt> member function, and does some checking using
 * <tt>take_dim</tt> and <tt>return_dim</tt>.
 * <br><br>
 * If the input value is an endpoint, be sure to specify which side to use
 * for evaluation.
 * <br><br>
 * @param x
 * value at which to perform the evaluation.
 * @param n
 * number of derivatives.
 * @param side
 * left(-1) or right (1) - 0 is default.
 * @param guess
 * best guess (optional).
 */
	double evaluateDR_R(double x, int n, const int& side = *(int*)NULL_REF, const double& guess = *(double*)NULL_REF) const;
/**
 * Evaluates the <i>n</i>th derivative of this law at an array of real inputs and returns a real result.
 * <br><br>
 * <b>Role:</b> Works on laws that have multi-dimensional domains and return a one-dimensional
 * result. (For convenience, all law classes inherit this method.) It calls
 * the main <tt>evaluate</tt> member function and does some checking using <tt>take_dim</tt> and
 * <tt>return_dim</tt>.
 * <br><br>
 * If the input value is an endpoint, be sure to specify which side to use for
 * evaluation.
 * <br><br>
 * @param x
 * array of values at which to perform the evaluation.
 * @param n
 * number of derivatives.
 * @param side
 * left(-1) or right (1) - 0 is default.
 * @param guess
 * best guess (optional).
 */
	double evaluateDM_R(const double* x, int n, const int* side = NULL, const double* guess = NULL) const;
/**
 * Evaluates this law at a complex-number input and returns a real result.
 * <br><br>
 * <b>Role:</b> Works on laws that have two-dimensional domains and one-dimensional
 * ranges. (For convenience, all law classes inherit this method.)
 * It calls the main <tt>evaluate</tt> member function and does some checking using
 * <tt>take_dim</tt> and <tt>return_dim</tt>.
 * <br><br>
 * If the input value is an endpoint, be sure to specify which side to use
 * for evaluation.
 * <br><br>
 * @param c
 * complex number at which to perform the evaluation.
 * @param side
 * left(-1) or right (1) - 0 is default.
 * @param guess
 * best guess (optional).
 */
	double evaluateC_R(complex_number c, const int* side = NULL, const double* guess = NULL) const;
/**
 * Evaluates this law at an input of type <tt>SPAnvector</tt> and returns a real result.
 * <br><br>
 * <b>Role:</b> Works on laws that accept an <i>n</i>-dimensional vector as a single
 * argument and return a one-dimensional real value. (For convenience, all law classes inherit
 * this method.) It calls the main <tt>evaluate</tt> member function and
 * does some checking using <tt>take_dim</tt> and <tt>return_dim</tt>.
 * <br><br>
 * If the input value is an endpoint, be sure to specify which side to use
 * for evaluation.
 * <br><br>
 * @param nv
 * n-dimensional vector at which to perform the evaluation.
 * @param side
 * left(-1) or right (1) - 0 is default.
 * @param guess
 * best guess (optional).
 */
	double evaluateNV_R(const SPAnvector& nv, const int* side = NULL, const double* guess = NULL) const;
/**
 * Evaluates this law at a real input and returns an <tt>SPAvector</tt> result.
 * <br><br>
 * <b>Role:</b> Works on laws that accept a one-dimensional real input and returns a vector
 * result. (For convenience, all law classes inherit this method.) It calls
 * the main <tt>evaluate</tt> member function and does some checking using <tt>take_dim</tt> and
 * <tt>return_dim</tt>.
 * <br><br>
 * If the input value is an endpoint, be sure to specify which side to use
 * for evaluation.
 * <br><br>
 * @param x
 * value at which to perform the evaluation.
 * @param side
 * left(-1) or right (1) - 0 is default.
 * @param guess
 * best guess (optional).
 */
	SPAvector evaluateR_V(double x, const int& side = *(int*)NULL_REF, const double& guess = *(double*)NULL_REF) const;
/**
 * Evaluates this law at a real input and returns an <tt>SPAunit_vector</tt> result.
 * <br><br>
 * <b>Role:</b> Works on laws that accept a one-dimensional real input and return a unit
 * vector result. (For convenience, all law classes inherit this method.)
 * It calls the main <tt>evaluate</tt> member function and does some checking using
 * <tt>take_dim</tt> and <tt>return_dim</tt>.
 * <br><br>
 * If the input value is an endpoint, be sure to specify which side to use
 * for evaluation.
 * <br><br>
 * @param x
 * value at which to perform the evaluation.
 * @param side
 * left(-1) or right (1) - 0 is default.
 * @param guess
 * best guess (optional).
 */
    SPAunit_vector evaluateR_UV(double x, const int& side = *(int*)NULL_REF, const double& guess = *(double*)NULL_REF) const;
/**
 * Evaluates the <i>n</i>th derivative of this law at a real input and returns an <tt>SPAvector</tt> result.
 * <br><br>
 * <b>Role:</b> Works on laws that have a one-dimensional real domain and a
 * three-dimensional vector range. (For convenience, all law classes inherit this method.)
 * It calls the main <tt>evaluate</tt> member function and does some checking using <tt>take_dim</tt>
 * and <tt>return_dim</tt>.
 * <br><br>
 * If the input value is an endpoint, be sure to specify which side to use
 * for evaluation.
 * <br><br>
 * @param x
 * value at which to perform the evaluation.
 * @param n
 * number of derivatives.
 * @param side
 * left(-1) or right (1) - 0 is default.
 * @param guess
 * best guess (optional).
 */
	SPAvector evaluateDR_V(double x, int n, const int& side = *(int*)NULL_REF, const double& guess = *(double*)NULL_REF) const;
/**
 * Evaluates this law at an array of real inputs and returns an <tt>SPAvector</tt> result.
 * <br><br>
 * <b>Role:</b> Works on laws that accept a one-dimensional real input and
 * return a three-dimensional vector.  (For convenience, all law classes inherit this method.)
 * It calls the main <tt>evaluate</tt> member function and does some checking using \
 * <tt>take_dim</tt> and <tt>return_dim</tt>.
 * <br><br>
 * If the input value is an endpoint, be sure to specify which side to use
 * for evaluation.
 * <br><br>
 * @param x
 * array of values at which to perform the evaluation.
 * @param side
 * left(-1) or right (1) - 0 is default.
 * @param guess
 * best guess (optional).
 */
	SPAvector evaluateM_V(const double* x, const int* side = NULL, const double* guess = NULL) const;
/**
 * Evaluates this law at an <tt>SPApar_pos</tt> input and returns an <tt>SPAvector</tt> result.
 * <br><br>
 * <b>Role:</b> Works on laws that accept a parameter position input and
 * return a three-dimensional vector. (For convenience, all law classes inherit this method.)
 * It calls the main <tt>evaluate</tt> member function and does some checking
 * using <tt>take_dim</tt> and <tt>return_dim</tt>.
 * <br><br>
 * If the input value is an endpoint, be sure to specify which side to use for evaluation.
 * <br><br>
 * @param pp
 * parameter position at which to perform the evaluation.
 * @param side
 * left(-1) or right (1) - 0 is default.
 * @param guess
 * best guess (optional).
 */
	SPAvector evaluatePP_V(const SPApar_pos& pp, const int* side = NULL, const double* guess = NULL) const;
/**
 * Evaluates this law at an <tt>SPAnvector</tt> input and returns an <tt>SPAnvector</tt> result.
 * <br><br>
 * <b>Role:</b> Works on laws that accept an <i>n</i>-dimensional vector as a single
 * argument and return a three-dimensional vector. (For convenience, all law classes inherit
 * this method.) It calls the main <tt>evaluate</tt> member function and
 * does some checking using <tt>take_dim</tt> and <tt>return_dim</tt>.
 * <br><br>
 * If the input value is an endpoint, be sure to specify which side to use
 * for evaluation.
 * <br><br>
 * @param nv
 * n-dimensional vector at which to perform the evaluation.
 * @param side
 * left(-1) or right (1) - 0 is default.
 * @param guess
 * best guess (optional).
 */
	SPAvector evaluateNV_V(const SPAnvector& nv, const int* side = NULL, const double* guess = NULL) const;
/**
 * Evaluates this law at an array of real inputs and returns an <tt>SPAposition</tt> result.
 * <br><br>
 * <b>Role:</b> Works on laws that have a multi-dimensional real domain and return a position.
 * (For convenience, all law classes inherit this method.) It calls the main <tt>evaluate</tt>
 * member function and does some checking using <tt>take_dim</tt> and <tt>return_dim</tt>.
 * <br><br>
 * If the input value is an endpoint, be sure to specify which side to use
 * for evaluation.
 * <br><br>
 * @param x
 * array of values at which to perform the evaluation.
 * @param side
 * left(-1) or right (1) - 0 is default.
 * @param guess
 * best guess (optional).
 */
	SPAposition evaluateM_P(const double* x, const int* side = NULL, const double* guess = NULL) const;
/**
 * Evaluates this law at an <tt>SPAposition</tt> input and returns an <tt>SPAposition</tt> result.
 * <br><br>
 * <b>Role:</b> Works on laws that accept a position input and return a position
 * result. (For convenience, all law classes inherit this method.)
 * It calls the main <tt>evaluate</tt> member function and does some checking using
 * <tt>take_dim</tt> and <tt>return_dim</tt>.
 * <br><br>
 * If the input value is an endpoint, be sure to specify which side to use
 * for evaluation.
 * <br><br>
 * @param p
 * position at which to perform the evaluation.
 * @param side
 * left(-1) or right (1) - 0 is default.
 * @param guess
 * best guess (optional).
 */
	SPAposition evaluateP_P(SPAposition p, const int* side = NULL, const double* guess = NULL) const;
/**
 * Evaluates this law at an <tt>SPAnvector</tt> input and returns an <tt>SPAposition</tt> result.
 * <br><br>
 * <b>Role:</b> Works on laws that accept an <i>n</i>-dimensional vector as a single
 * argument and return a position. (For convenience, all law classes inherit
 * this methode.) It calls the main <tt>evaluate</tt> member function and
 * does some checking using <tt>take_dim</tt> and <tt>return_dim</tt>.
 * <br><br>
 * If the input value is an endpoint, be sure to specify which side to use
 * for evaluation.
 * <br><br>
 * @param nv
 * n-dimensional vector at which to perform the evaluation.
 * @param side
 * left(-1) or right (1) - 0 is default.
 * @param guess
 * best guess (optional).
 */
	SPAposition evaluateNV_P(const SPAnvector& nv, const int* side = NULL, const double* guess = NULL) const;
/**
 * Evaluates this law at a real input and returns an <tt>SPAposition</tt> result.
 * <br><br>
 * <b>Role:</b> Works on laws that accept a one-dimensional real input and return a position
 * result. (For convenience, all law classes inherit this method.) It calls
 * the main <tt>evaluate</tt> member function and does some checking using <tt>take_dim</tt>
 * and <tt>return_dim</tt>.
 * <br><br>
 * If the input value is an endpoint, be sure to specify which side to use
 * for evaluation.
 * <br><br>
 * @param x
 * value at which to perform the evaluation.
 * @param side
 * left(-1) or right (1) - 0 is default.
 * @param guess
 * best guess (optional).
 */
	SPAposition evaluateR_P(double x, const int& side = *(int*)NULL_REF, const double& guess = *(double*)NULL_REF) const;
/**
 * Evaluates this law at an SPApar_pos input and returns an <tt>SPAposition</tt> result.
 * <br><br>
 * <b>Role:</b> Works on laws that accept a parameter position input and
 * returns a position. (For convenience, all law classes inherit this method.)
 * It calls the main <tt>evaluate</tt> member function and does some checking
 * using <tt>take_dim</tt> and <tt>return_dim</tt>.
 * <br><br>
 * If the input value is an endpoint, be sure to specify which side to use for
 * evaluation.
 * <br><br>
 * @param pp
 * parameter position at which to perform the evaluation.
 * @param side
 * left(-1) or right (1) - 0 is default.
 * @param guess
 * best guess (optional).
 */
	SPAposition evaluatePP_P(const SPApar_pos& pp, const int* side = NULL, const double* guess = NULL) const;
/**
 * Evaluates this law at an <tt>SPAposition</tt> input and returns an <tt>SPApar_pos</tt> result.
 * <br><br>
 * <b>Role:</b> Works on laws that accept a position input and return a
 * parameter position result. (For convenience, all law classes inherit this method.)
 * It calls the main <tt>evaluate</tt> member function and
 * does some checking using <tt>take_dim</tt> and <tt>return_dim</tt>.
 * <br><br>
 * If the input value is an endpoint, be sure to specify which side to use
 * for evaluation.
 * <br><br>
 * @param p
 * position at which to perform the evaluation.
 * @param side
 * left(-1) or right (1) - 0 is default.
 * @param guess
 * best guess (optional).
 */
	SPApar_pos evaluateP_PP(const SPAposition& p, const int* side = NULL, const double* guess = NULL) const;
/**
 * Evaluates this law at an array of real inputs and returns an <tt>SPApar_pos</tt> result.
 * <br><br>
 * <b>Role:</b> Works on laws that have a multi-dimensional domain and return a parameter
 * position. (For convenience, all law classes inherit this method.)
 * It calls the main <tt>evaluate</tt> member function and does some checking using <tt>take_dim</tt>
 * and <tt>return_dim</tt>.
 * <br><br>
 * If the input value is an endpoint, be sure to specify which side to use
 * for evaluation.
 * <br><br>
 * @param x
 * array of values at which to perform the evaluation.
 * @param side
 * left(-1) or right (1) - 0 is default.
 * @param guess
 * best guess (optional).
 */
	SPApar_pos evaluateM_PP(const double* x, const int* side = NULL, const double* guess = NULL) const;
/**
 * Returns the bounding box associated with this law.
 * <br><br>
 * <b>Role:</b> (For convenience, all law classes inherit this method.) It calls
 * the main <tt>evaluate</tt> member function and does some checking using <tt>take_dim</tt> and <tt>return_dim</tt>.
 */
	SPAbox evaluate_B() const;
/**
 * Evaluates the <i>n</i>th derivative of this law at an array of real inputs and returns an array of real results.
 * <br><br>
 * <b>Role:</b> Works on laws whose domains and ranges are both multi-dimensional.
 * (For convenience, all law classes inherit this method.) It calls
 * the main <tt>evaluate</tt> member function and does some checking using <tt>take_dim</tt>
 * and <tt>return_dim</tt>.
 * <br><br>
 * If the input value is an endpoint, be sure to specify which side to use
 * for evaluation.
 * <br><br>
 * @param x
 * array of values at which to perform the evaluation.
 * @param answer
 * array of answer values resulting from the evaluation.
 * @param n
 * number of derivatives.
 * @param side
 * left(-1) or right (1) - 0 is default.
 * @param guess
 * best guess (optional).
 */
	void evaluateDM(
        const double* x, double* answer, int n,
        const int* side = NULL, const double* guess = NULL) const;
/**
 * Evaluates this law at an array of real inputs and returns an <tt>SPAmatrix</tt> result.
 * <br><br>
 * <b>Role:</b> Works on laws that have a multi-dimensional domain and return a real matrix.
 * (For convenience, all law classes inherit this method.) It calls the main <tt>evaluate</tt>
 * member function and does some checking using <tt>take_dim</tt> and <tt>return_dim</tt>.
 * <br><br>
 * If the input value is an endpoint, be sure to specify which side to use
 * for evaluation.
 * <br><br>
 * @param x
 * array of values at which to perform the evaluation.
 */
    SPAmatrix evaluateM_MAT(const double* x) const;
/**
 * Evaluates this law at an array of real inputs and returns an <tt>SPAtransf</tt> result.
 * <br><br>
 * <b>Role:</b> Works on laws that accept a multi-dimensional domain and return a transform.
 * (For convenience, all law classes inherit this method.) It calls
 * the main <tt>evaluate</tt> member function and does some checking using
 * <tt>take_dim</tt> and <tt>return_dim</tt>.
 * <br><br>
 * If the input value is an endpoint, be sure to specify which side to use
 * for evaluation.
 * <br><br>
 * @param x
 * array of values at which to perform the evaluation.
 */
    SPAtransf evaluateM_T(const double* x) const;

    // For internal use
/**
 * @nodoc
 */
    SPAmatrix evaluateM_MAT_LT(const double* x) const;

    // Used for parsing
/**
 * Returns the string that represents this law class's symbol.
 * <br><br>
 * <b>Role:</b> The symbol is used for parsing the law and for saving and restoring
 * law-based geometry. For a law to be saved and restored, it must have or inherit
 * this method.
 * <br><br>
 * This is the top-level class. The default law symbol for this class is an
 * error message. Derived classes need to define their own symbol methods to
 * override this error message.
 * <br><br>
 * @param type
 * type of law symbol - standard ACIS type.
 */
	virtual const char* symbol(law_symbol_type type = DEFAULT) const;
/**
 * Returns one of five values that indicates the precedence of this law during evaluation.
 * <br><br>
 * <b>Role:</b> For example, minus has the <tt>PRECEDENCE_PLUS</tt>. The default for the
 * law class is <tt>PRECEDENCE_FUNCTION</tt>.
 * <br><br>
 * The valid precedence values are <tt>PRECEDENCE_PLUS</tt> (1), <tt>PRECEDENCE_TIMES</tt> (2),
 * <tt>PRECEDENCE_POWER</tt> (3), <tt>PRECEDENCE_FUNCTION</tt>(4), and <tt>PRECEDENCE_CONSTANT</tt> (5).
 * <br><br>
 * This is used for simplification and parsing. For a law to be saved and
 * restored, it must have or inherit this method.
 */
	virtual int precedence() const;
/**
 * Determines whether or not this law is equivalent to the input law.
 * <br><br>
 * <b>Role:</b> This is inherited by all laws and calls the <tt>same</tt> method, which
 * the major law subclasses override. This is used for simplification.
 * <br><br>
 * @param inlaw
 * law to compare this to.
 */
	logical operator==(law& inlaw) const;
/**
 * Determines whether or not this law is not equivalent to the input law.
 * <br><br>
 * <b>Role:</b> This is inherited by all laws and calls the <tt>same</tt> method, which
 * the major law subclasses override. This is used for simplification.
 * <br><br>
 * @param inlaw
 * law to compare this to.
 */
	logical operator!=(law& inlaw) const;
/**
 * Determines whether or not two specified laws are the same.
 * <br><br>
 * <b>Role:</b> This method should not be called directly by the application.
 * It is used for simplification.  For a law to be saved and restored, it must
 * have or inherit this method. It is called by the == method, to see if two laws
 * are the same.
 * <br><br>
 * @param law1
 * 1st law to test.
 * @param law2
 * 2nd law to test.
 */
	virtual int same(const law* law1, const law* law2) const;

	// Used for memory management

/**
 * Increments the use count of this law.
 * <br><br>
 * <b>Role:</b> An application calls this method in order to preserve a copy of this
 * law.  It is not usually called by an application directly, but by the law constructor(s)
 * for the law being constructed, which also call <tt>add</tt> for all of its sublaws. It
 * increments the reference count.
 */
	void add() const;
/**
 * Decrements the use-count of this law and destructs the law when the use-count drops to zero.
 * <br><br>
 * <b>Role:</b> For efficient memory management, the law classes are use-counted. Applications
 * should accordingly call this method rather than the tilde (~) destructor to
 * get rid of a law. It decrements <tt>use_count</tt> and calls the law destructor when
 * this number reaches zero.  Destructors of derived laws should call this method on all of
 * their sublaw members.
 */
	void remove();
/**
 * Returns the use count of this law.
 */
	int ref_count() {return reference_count;};
/**
 * Returns a string that represents the type of this law.
 * <br><br>
 * <b>Role:</b> The law function is composed of its symbol, associated parentheses,
 * and the strings associated with its sublaws. It is provided as a user-friendly
 * interface to laws. A derived class must override this function to be able to
 * save a law.
 * <br><br>
 * It the caller's responsibility to delete the resulting string when finished
 * with it. Use the array <tt>delete</tt> operator, e.g.,
 * <pre>
 * char *str == someLaw->string();
 * delete [] str;</pre>
 * @param type
 * type of law symbol - standard ACIS type.
 * @param count
 * count.
 * @param ldn
 * law data node.
 */
	virtual char* string(
            law_symbol_type type    = DEFAULT,
            int&            count   = *(int*) NULL_REF,
            law_data_node*& ldn     = *(law_data_node**) NULL_REF) const;
/**
 * Returns a string representing this law and its data.
 * <br><br>
 * <b>Role:</b> This method calls the <tt>string</tt> to obtain a string that represents the current
 * law function. This is placed together with the <tt>law_data</tt> of a law for the saving
 * of a law. It is provided as a user-friendly interface to laws. A derived class
 * must have or inherit this method to be able to save a law.
 * <br><br>
 * @param ld
 * array of law data.
 * @param size
 * size of array.
 * @param type
 * type of law symbol - standard ACIS type.
 */
	char* string_and_data(
            law_data***     ld,
            int*            size,
            law_symbol_type type    = DEFAULT) const;
/**
 * Returns the version of ACIS in which this law class first appeared.
 * <br><br>
 * <b>Role:</b> If a law is part of a model that is to be saved at a previous
 * ACIS release level, this is used to indicate whether the law can be saved
 * or not.
 */
	virtual int date() const;
/**
 * Creates a copy of an item that does not share any data with the original.
 * <br><br>
 * <b>Role:</b> Allocates new storage for all member data and any pointers.
 * Returns a pointer to the copied item.
 * <br><br>
 * In a <i>deep</i> copy, all the information about the copied item is self-contained
 * in a new memory block. By comparison, a <i>shallow</i> copy stores only the first
 * instance of the item in memory, and increments the reference count for each copy.
 * <br><br>
 * The <tt>pointer_map</tt> keeps a list of all pointers in the original object that
 * have already been deep-copied. For example, a <tt>deep_copy</tt> of a complex model
 * results in self contained data, but identical sub-parts within the model are allowed
 * to share a single set of data.
 * <br><br>
 * @param pm
 * list of items within the entity that are already deep copied.
 */
	virtual law* deep_copy(base_pointer_map* pm = NULL) const;

	// Algebra type functions
/**
 * Determines whether this law is zero.
 * <br><br>
 * <b>Role:</b> The law function <tt>vec(0, 0, 0)</tt> is considered zero by this method.
 * <br><br>
 * @param tol
 * tolerance to use in determination.
 */
	logical zero(double tol = SPAresabs) const;
/**
 * Returns <tt>TRUE</tt> if this law is an integer law.
 */
	logical integer() const;
/**
 * Determines whether or not this law is constant.
 * <br><br>
 * <b>Role:</b> This is not the same as whether or not the law is of the
 * <tt>constant_law</tt> class. The expression <tt>if (f->isa(constant_law::id()))</tt> is a much
 * different test than is <tt>if (f->constant())</tt>. The former tells whether or not
 * <i>f</i> is a <tt>constant_law</tt>. For example, "5" is a <tt>constant_law</tt>. The latter
 * tells whether or not <i>f</i> is constant in the sense that it returns the same
 * value for all inputs. The law "5*4" always returns the same value, but is a <tt>times_law</tt>
 * rather than a <tt>constant_law</tt>.
 */
	logical constant() const;
/**
 * Determines whether or not this law is linear.
 * <br><br>
 * <b>Role:</b> Sets internal flags accordingly.
 */
	logical linear() const;
/**
 * Returns the <tt>law_polynomial</tt> associated with the specified law.
 * <br><br>
 * <b>Role:</b> This is used to help determine the degree of the polynomial from the top-level law.
 * <br><br>
 * @param in
 * input law.
 */
	virtual law_polynomial* polynomial(law* in) const;
/**
 * Returns <tt>TRUE</tt> if this law is the identity within a specified interval.
 * <br><br>
 * @param in_bounds
 * pointer to interval within which to check.
 */
	virtual logical identity(SPAinterval* in_bounds = NULL) const;	  // Is it the identity on the given bounds?
/**
 * Returns <tt>TRUE</tt> if this law is the identity within a specified box.
 * <br><br>
 * @param in_box
 * box within which to check.
 */
	virtual logical identity(SPAbox& in_box) const;					  // Is it the identity in the given box?

	// Domain and range functions.  They are not fully implemented.
/**
 * Establishes the domain of a given term in this law.
 * <br><br>
 * @param term
 * term to bound.
 * @param domain
 * bounds for term.
 */
	virtual logical term_domain(int term, SPAinterval& domain) const;
/**
 * Checks to see if a given input is within this law's domain.
 * <br><br>
 * @param where
 * where to test domain.
 */
	virtual logical in_domain(double* where) const;
/**
 * Specifies where in this law there might be discontinuities.
 * <br><br>
 * <b>Role:</b> The array <tt>where</tt> notes where the discontinuity occurs. The <tt>type</tt>
 * indicates 0 if there is a discontinuity, 1 if the discontinuity in the 1st
 * derivative, and any integer <i>n</i> if the discontinuity is in the <i>n</i>th derivative.
 * The value -1 means that the type is not defined.
 * <br><br>
 * @param where
 * where discontinuities exist.
 * @param type
 * discontinuity types.
 * @param start
 * start.
 * @param end
 * end.
 * @param period
 * period.
 */
	virtual int singularities(
                double**    where,
                int**       type,
                double      start   = -DBL_MAX,
                double      end     = DBL_MAX,
                double**    period  = NULL) const;
/**
 * Sets the domain of this law.
 * <br><br>
 * @param low_range
 * low range.
 * @param high_range
 * high range.
 * @param low_domain
 * low domain.
 * @param high_domain
 * high domain.
 * @param requested_tol
 * tolerance.
 * @param min_gaps
 * minimum gaps.
 */
	double bounds(
            double* low_range,
            double* high_range,
            double* low_domain      = NULL,
            double* high_domain     = NULL,
            double  requested_tol   = SPAresfit * 0.5,
            double* min_gaps        = NULL) const;
/**
 * Establishes the domain of this law.
 * <br><br>
 * <b>Role:</b> Permits the law to be altered for its input array size.
 * <br><br>
 * @param new_domain
 * new input domain.
 * @param set
 * adds domain if TRUE, substitutes if FALSE.
 */
	virtual law* set_domain(SPAinterval* new_domain, logical set = FALSE);
/**
 * Sets a new range for this law.
 * <br><br>
 * @param new_range
 * new range.
 * @param set
 * set or not.
 */
	virtual law* set_range(SPAinterval* new_range, logical set = FALSE);

	// Simplify and inverse functions
/**
 * Returns a law that is a simplification of this law.
 * <br><br>
 * <b>Role:</b> This is a member function that may be overloaded by derived classes
 * to provide assistance to the simplifier.
 * It helps the simplifier in dealing with this particular law.
 * This method is called by the simplifier but generally not called directly
 * by the application.
 * <br><br>
 * For example, a law class such as <tt>plus_law</tt> might use an equation <tt>X + X</tt>.
 * The <tt>sub_simplify</tt> method could return this equation as <tt>2*X</tt>. The <tt>sub_simplify</tt>
 * method can access the private members of the law that the simplifier does not
 * have access to. Most laws simply inherit a function that returns <tt>NULL</tt>.
 * <br><br>
 * @param level
 * level of simplification.
 * @param what
 * text string that describes the simplified law.
 */
	virtual law* sub_simplify(int level = 0, const char*& what = *(const char**)NULL_REF) const;
/**
 * Returns a law that is a mathematical simplification of this law.
 * <br><br>
 * <b>Role:</b> This returns another law or a copy of the given law if it cannot
 * be simplified any further.
 * <br><br>
 * When the <tt>show_work</tt> argument is set to a positive integer, debugging information
 * is provided. Because laws cache their simplified version, simplifying a law
 * twice does not result in any debugging information on the second call. To trick
 * the simplifier into not using its cached value, the level argument should be
 * set to "666".
 * <br><br>
 * @param level
 * level of simplification.
 * @param show_work
 * for debugging, shows simplification.
 */
	law* simplify(int level = 1, int show_work = 0) const;
/**
 * Returns a pointer to a law that is the inverse of this law.
 */
	law* inverse() const;
/**
 * Sets the inverse of the given law.
 * <br><br>
 * @param inv
 * law to invert.
 */
	void set_inverse(law* inv) const;
/**
 * Returns <tt>TRUE</tt> if this law is simplified to the specified level.
 * <br><br>
 * @param level
 * simplify level.
 */
	logical is_simplified(int level) const;
/**
 * Sets the simplification level of this law.
 * <br><br>
 * @param in_law
 * law to simplify.
 * @param level
 * level of simplification.
 */
	void set_simplify(law* in_law, int level) const;
/**
 * Resets the simplification level of this law.
 * <br><br>
 * @param in_law
 * law to simplify.
 * @param level
 * level of simplification.
 */
	void reset_simplify(law* in_law, int level) const; // same as set_simplify, but not dependent on level.
/**
 * Returns a copy of this law with branching enabled.
 * <br><br>
 * <b>Role:</b> Returns <tt>NULL</tt> if the operation fails, or if the necessary functionality
 * has not been provided by a derived class.
 */
	virtual law* enable_branching() const;
/**
 * Returns a non-negative integer that tells the dimension of the branches of this law.
 * <br><br>
 * <b>Role:</b> For example, <tt>sin(0)</tt> equals 0, but so does <tt>sin(pi)</tt>,
 * <tt>sin(2*pi)</tt>, <tt>sin(n*pi)</tt>, and so on. Hence, the <tt>arcsin</tt> of zero is
 * dependent
 * on one extra value <tt>n</tt>. The function <tt>arcsin(x)*arccos(x)</tt> requires two extra
 * values: one to tell which branch the <tt>arcsin</tt> is on, and one to tell which branch
 * the <tt>arccos</tt> is on. Hence, the <tt>branches</tt> method of the law <tt>arcsin(x)</tt>
 * will return 1, while that of the function <tt>arccos(x)*arcsin(x)</tt> will return 2.
 * For functions that do not have branches, like <tt>sin(x)</tt>, it will return 0.
 */
	virtual int branches() const;
/**
 * @nodoc
 */
	virtual void full_size(SizeAccumulator&, logical = TRUE) const;
/**
 * @nodoc
 * Find if type of law used for a law.  If a multiple law or
 * has a law as a member, searches through to find the type
 * similar to the isa function.  Does not include deriv laws
 * or inverse_sub laws if applicable.
 */
	virtual void hasa(int type, VOID_LIST& out_laws);
};

// Other law functions
/**
 * Creates a unique id number for the given law, which is used for type identification.
 * <br><br>
 * <b>Role:</b> The id number this function creates is unique for the currently
 * defined laws in the system. It is not persistent and is not saved to SAT.
 * It is used mostly for law type identification. This function is never called
 * directly by an application.It is used internally by the law system. This is
 * called by law member functions. It returns a unique number which is used by
 * the id method of the respective law class.
 * <br><br>
 * <b>Effect:</b> System routine
 **/
DECL_LAW int new_law_id(); 

/**
 * @nodoc
 */
DECL_LAW law* law_simplify(
            const law*   in_law,
            int          level      = 0,
            int          show_work  = 0);

/**
 * Law type
 */
#define NOT_ASSOCIATIVE 0
/**
 * Law type
 */
#define ASSOCIATIVE     1
/**
 * Law type
 */
#define NOT_COMMUTATIVE 0
/**
 * Law type
 */
#define COMMUTATIVE     1
/**
  * Law precedence level
 */
#define PRECEDEDCE_OR       1

/**
 * Law precedence level
 */
#define PRECEDEDCE_AND      2

/**
 * Law precedence level
 */
#define PRECEDEDCE_NOT      3
/**
 * Law precedence level
 */
#define PRECEDENCE_EQUAL    4
/**
  * Law precedence level
 */
#define PRECEDENCE_PLUS     5
/**
 * Law precedence level
 */
#define PRECEDENCE_TIMES    6
/**
  * Law precedence level
 */
#define PRECEDENCE_POWER    7
/**
 * Law precedence level
 */
#define PRECEDENCE_FUNCTION 8
/**
 * Law precedence level
 */
#define PRECEDENCE_CONSTANT 9
/** @} */
#endif
