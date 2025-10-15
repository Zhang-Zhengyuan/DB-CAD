// $Id: law_util.hxx,v 1.9 2002/08/09 17:13:30 jeff Exp $
/*******************************************************************/
/*    Copyright (c) 1989-2020 by Spatial Corp.                     */
/*    All rights reserved.                                         */
/*    Protected by U.S. Patents 5,257,205; 5,351,196; 6,369,815;   */
/*                              5,982,378; 6,462,738; 6,941,251    */
/*    Protected by European Patents 0503642; 69220263.3            */
/*    Protected by Hong Kong Patent 1008101A                       */
/*******************************************************************/

#ifndef LAW_UTIL_HXX
#define LAW_UTIL_HXX

#include <float.h>
#include <math.h>
#include "dcl_law.h"
#include "logical.h"

#include "main_law.hxx"
#include "complex.hxx"
#include "interval.hxx"
#include "param.hxx"
#include "vector.hxx"
#include "position.hxx"

class rec2d;
//class SURFACE;
//class CURVE;
//class curve;
//class EDGE;

//////////////////////////////////////////////////////////
//
// Functions for working with laws
//
//////////////////////////////////////////////////////////

DECL_LAW void make_bound_interval (SPAinterval &in_interval, 
									double = 10000 // SPAinterval length 
									);

DECL_LAW law  *str_to_law(char const *instr,
						   law_data **laws=NULL,
						   int law_num=0);

DECL_LAW int  Nintegrate(law *intlaw,
			   double a,
			   double b,
			   double *answer,
			   double tol=1E-12,
			   int    min_level=2);

DECL_LAW int  Nintegrate_wrt(law    *inlaw,
				              double a,
			                  double b,
				              int    wrt,
				              double *along,
			                  double *answer,
							  double tol=1E-12,
			                  int    min_level=2);

DECL_LAW int  Nintegrate_wrt_and_split(law    *inlaw,
				                        double a,
			                            double b,
				                        int    wrt,
				                        double *along,
			                            double *answer,
				 					    int    split_size=0,
				   					    double *splits=NULL,
				  					    double tol=1E-12,
			                            int    min_level=2);

DECL_LAW void  Ndifferentiate(law    *inlaw,
					           double const *x,
					           int    which_dim,
						       double *answer,
							   int    times=1);	

DECL_LAW void Ndifferentiate_sided(law    *inlaw,    // what
									double const *x,  // where
									int    dim,	      // which
									int    left,      // which side 1 from the left
												      // 2 from the right
									double *answer,
									int    in_num,		// how many times
									double step_size = 1.0e-4  // previously this was hardcoded to 1.0e-4.  SPAresfit/10 is probably a better result.  Even so, the best step size depends on the scale of the problem.
									);

DECL_LAW double *Nroot(law *inlaw,
			            double minx,
			            double maxx,
			            int *size);

// This function returns the parameter value corresponding to the maximum value of the law.
// The maximum value of the law is not returned.
DECL_LAW double Nmax(law    *inlaw,
			          double minx,
			          double maxx);

// This function returns the parameter value corresponding to the minimum value of the law.
// The minimum value of the law is not returned.
DECL_LAW double Nmin(law    *inlaw,
			          double minx,
			          double maxx);

// steepest descent to find function minimum
DECL_LAW double Nmin_quick(law    *inlaw,        // input law to minimize
                            double minx,          // domain start
                            double maxx,          // domain end
                            int    num_seed_pts,  // number of evals to use to find best guess value
                            double startdx,       // inital step size
                            double min_value = - DBL_MAX); // stop if found

// steepest descent to find function maximum
DECL_LAW double Nmax_quick(law    *inlaw,        // input law to maximize
                            double minx,          // domain start
                            double maxx,          // domain end
                            int    num_seed_pts,  // number of evals to use to find best guess value
                            double startdx,       // inital step size
                            double max_value = DBL_MAX);  // stop if found

DECL_LAW double *Nsolve(law    *law1,
			             law    *law2,
			             double minx,
			             double maxx,
			             int    *size);

DECL_LAW complex_number Nmin2D(law            *inlaw,
					            law            *domain,
			                    complex_number start,
			                    double         startdx,
			                    double         maxd,
			                    double         *value,
					            double         *dxy,
			                    int            *error,
								law            *test_law=NULL);

DECL_LAW SPAnvector NminND( law            *inlaw,		           // law to minimize
					     law            *domain,	           // where to look (i.e. where domain>0)
			             SPAnvector        &start,		           // where to start looking
			             SPAnvector        &startdx,	           // how to start stepping
			             double         *value,				   // inlaw(x) where x is the local min
					     SPAnvector        *dxy,		           // final step size
						 double         min_value = - DBL_MAX, // stop if found
						 law            *stop_law = NULL);	   // law to test for stoping

DECL_LAW complex_number *Nroot2D(law            *in_law,
				                  law            *domain_law,
						          law            *tol_law,
				                  rec2d			 rec,
				                  int            *size,
								  complex_number *also=NULL,			  
								  int            asize=0);

DECL_LAW complex_number *Nroot2D(law            *in_law,	      // what to min
				                  law            *domain_law,     // where to check
				                  rec2d		     rec,		      // also where to check
				                  int            *size,		      // how many roots where found
						          complex_number *also_test,      // where to start checking
						          int            atest_size,      // how many points to check
								  law            *test_law=NULL); // when to stop

				// Returns false if an answer was unable to be found //
DECL_LAW logical find_best_branch( law    *in_law,		  // Law to fined the best branch of
								    law    *tie_breaker,  // Law that returns one value to break ties may be NULL
							        double *input,		  // The domain value to find the branch of
									double *guess_answer, // What do you want the answer close to
									double tol,			  // How good is your guess
									int    *best,		  // Returns the best branch numbers
									double *answer);	  // Returns the value at the best branch numbers

DECL_LAW logical same_law_list(law **list1,
								law **list2,
								int size);

DECL_LAW law **get_asso_list(binary_law const *in_law,
							  int        *size);

DECL_LAW law *make_list_law(law        **list,
							 int        size,
							 binary_law const *blaw);

//DECL_LAW law *numerator(law *eq);

//DECL_LAW law *denominator(law *eq);

DECL_LAW law *law_integrate(law *in_law);	

//DECL_LAW double law_multiple(law *in_law1,
//					          law *in_law2);

// GSSL hcm (08/24/01) begin: Changes for recording curvature_error_info
enum inequality_relation
{
	RELATION_LESS_THAN = 0, 
	RELATION_LESS_THAN_EQUAL, 
	RELATION_EQUAL, 
	RELATION_GREATER_THAN_EQUAL, 
	RELATION_GREATER_THAN
};

DECL_LAW law* construct_law_interval(int identity, int inequality, double value);
// GSSL hcm end.

class law_polynomial;
double poly_multiple( law_polynomial *in_p1,
		              law_polynomial *in_p2);
	   
///////////////////////////////////////////////////////
//
// Tolerances and constants for numerical algorithms
//
///////////////////////////////////////////////////////

DECL_LAW void set_root_tol_default();

class law_root_tolerances : public ACIS_OBJECT {
public:

	double ROOT_TOL_X;          // How close and distinct roots be to each other.
	double ROOT_TOL_Y;          // How close must the function be to zero to be a root.
	double ROOT_TOL_ABC;        // How small must a, b, or c be in the tangent 
								//  quadratic so that them should be considered zero.
	double ROOT_TOL_INTERVAL;   // What fraction must an SPAinterval be of the domain to 
								//  supect that there may not be a root.
	double ROOT_TOL_REACH; 		// What fraction of the way to zero must a linear
								//  worst case estimate of the function on a supected
								//  SPAinterval reach.
	double ROOT_TOL_DOUBLE; 	// What fraction must the start point be between two
								//  two roots of the tangent quadratic to count both
								//  roots.
	int    ROOT_TOL_MAX_NEWTON; // Maximum number of iterations for newton.
	int    ROOT_TOL_MAX_DAC;    // Maximum number of iterations for devide and conc
	int    ROOT_TOL_MAX_LEVEL;  // Maximum times to subdivide to find 2D roots
	int    ROOT_TOL_MAX_SD;     // Maximum number of steppest desent tries.
	double ROOT_TOL_SEP;        // Min distance between roots.

public:

	law_root_tolerances();
	void set_root_tol_default();
};

#ifdef THREAD_SAFE_ACIS
extern DECL_LAW safe_object_pointer<law_root_tolerances> root_tolerances;
#else
extern DECL_LAW law_root_tolerances* root_tolerances;
#endif

// Static list of laws

class DECL_LAW law_list : public ACIS_OBJECT
{
public:
	law          *data_law;
	law_list     *next;
	law_list(law * in_data_law);
};

// Static list of law_datas

class DECL_LAW law_data_list : public ACIS_OBJECT
{
public:
	law_data      *data;
	law_data_list *next;
	law_data_list(law_data * in_data);
};

DECL_LAW law_data *get_law_data(char const *str);

// List of rules for simplification

typedef law *(*RuleFunct)(law *);

class DECL_LAW SRule : public ACIS_OBJECT
{
public:
	RuleFunct funct;
	SRule     *next;
	char      *name;
	SRule(RuleFunct rfunct,int list_num,char *rname);
};

// open set class

class DECL_LAW Open_interval : public ACIS_OBJECT
{
public:
	double        a;
	double        b;
	Open_interval *next;
	Open_interval(double a,double b);
	double        mid_point() { return (a+b)*.5; }
};

class DECL_LAW Open_set : public ACIS_OBJECT
{
private:
	logical closed;
	double  max;
	double  min;
	Open_interval *op_int;
public:
	Open_set(double a,double b,logical in_closed=FALSE);
	~Open_set();
	void add(double a,double b);
    void remove(double a,double b, bool remove_ends = false);
	int is_full();	
	Open_interval *in(double a);
	Open_interval *before(double a);
	Open_interval *after(double a);
	void print();
	int parts();
	double *mid_points();
	double hole();
	double hole_size();
	double largest_hole(double& length);
};

// Root_2D_set

class DECL_LAW Root_2D : public ACIS_OBJECT
{
public:
	Root_2D(complex_number inxy,
			double         v,
			double         dxy,
			Root_2D        *n);

	complex_number xy;
	double         value;
	double         dx;
	Root_2D        *next;
};

class DECL_LAW Root_2D_set : public ACIS_OBJECT
{
public:
	Root_2D_set();
	~Root_2D_set();
	Root_2D *root;
	int add(law            *inlaw,
			complex_number xy,
			double         v,
			double         dx);
	int size();
};

// Rectangle

class DECL_LAW rec2d : public ACIS_OBJECT
{
public:
	complex_number Down_left;
	complex_number Up_right;
	rec2d(double minx,
		     double maxx,
			 double miny,
			 double maxy);
	rec2d();
	rec2d(complex_number dl,
		      complex_number ur);
	complex_number center();
	complex_number down_left();
	complex_number down_right();
	complex_number up_left();
	complex_number up_right();
	double diagonal();
	int inside(complex_number c);
};

// Quad_tree

class DECL_LAW Quad_tree : public ACIS_OBJECT
{
public:
	rec2d          rec;
	complex_number con ;		// where the center converges to
	double         dx;          // tolerance in con
	int            error;		// If it dose not converge.
	double         value;		// The function value at where it converges.
	Quad_tree      *sub[4];		// Holds four sub Quad_trees
	int            rootnum;		// Which vaule in the root list
	
	Quad_tree();
	~Quad_tree();
	void fill(law *in_law,
		      law *domain_law);
	void mins(law *in_law,
		      Root_2D_set *roots);
	int  should_divide(law *in_law,
		               law *domain_law,
				       law *tol_law);
	void divide(law *in_law,
		        law *domain_law,
				law *tol_law,
				Root_2D_set *roots);
};

// funtions to create laws

law DECL_LAW *no_more_than_one(double *data,int size);

law DECL_LAW *make_polynomial_law(double *coeff,int degree);

// Finds polynomial coefficients such that:
// F(a) = fa.
// F(b) = fb.
DECL_LAW void make_linear_polynomial(double a, double b,
								double fa, double fb,
								double coeff[2]);

// Finds polynomial coefficients such that:
// F(a) = fa.
// F(b) = fb.
// F'(a) = ffa.
// F'(b) = ffb.
DECL_LAW void make_cubic_polynomial(double a, double b,
								double fa, double fb,
								double ffa, double ffb,
								double coeff[4]);

// Finds polynomial coefficients such that:
// F(a) = fa.
// F(b) = fb.
// F'(a) = ffa.
// F'(b) = ffb.
// F''(a) = fffa.
// F''(b) = fffb.
DECL_LAW void make_quintic_polynomial(double a, double b,
								double fa, double fb,
								double ffa, double ffb,
								double fffa, double fffb,
								double coeff[6]);
// Evaluates a polynomial at x
DECL_LAW double eval_linear_polynomial( double x, const double c[2]);

// Evaluates a polynomial at x
DECL_LAW double eval_quadratic_polynomial( double x, const double c[3]);

// Evaluates a polynomial at x
DECL_LAW double eval_cubic_polynomial( double x, const double c[4]);

// Evaluates a polynomial at x
DECL_LAW double eval_quartic_polynomial( double x, const double c[5]);

// Evaluates a polynomial at x
DECL_LAW double eval_quintic_polynomial( double x, const double c[6]);

#if 0
// The law_domain class

enum interval_topo_type 
{
	unknown,
	open,
    closed,
    closed_below,
	closed_above
};

class DECL_LAW law_domain : public ACIS_OBJECT
{
	SPAinterval            *domain_bound;
	interval_topo_type	*topo_type;
	int                 dim;

	double              *points;
	int                 *point_types;
	int                 point_size;

protected:
	~law_domain();

public:
	law_domain(); // creates an unknown domain
	law_domain(SPAinterval           in_domain_bound,
		       interval_topo_type in_topo_type=closed);
	law_domain(SPAinterval           *in_domain_bound,
			   int                in_dim,
			   interval_topo_type *in_topo_type=NULL);
	void set_dim(int in_dim) {dim=in_dim;}
    void set_points(int    in_size,
		            double *in_points,
			        int    *point_types);
	double greatest_lower_bound(int which_dim=0);
	double least_upper_bound(int which_dim=0);
	logical bounded_below();
	logical bounded_above();
	logical finite() { return domain_bound[0].finite();}
	char *string();

	friend class law;
};
#endif

// The law_polynomal class

class DECL_LAW law_polynomial : public ACIS_OBJECT
{
	int    this_degree;
	int    size;
	double *coefficents;
	int    *terms;		 
	law    *in_law;
public:
	~law_polynomial();
	law_polynomial();
	law_polynomial(const law_polynomial &inlp);
	law_polynomial operator+(law_polynomial p);
	law_polynomial operator-(law_polynomial p);
	law_polynomial operator-();
	law_polynomial operator*(law_polynomial p);
	law_polynomial operator/(double d);
	law_polynomial operator^(int n);
	law_polynomial operator=(const law_polynomial &inlp);

	void   print();
	int    degree();
	double coefficent(int i);
	
	law    *in();
	law    *factor();
	law    *expand();

	friend class law;
	friend class plus_law;
	friend class minus_law;
	friend class negate_law;
	friend class constant_law;
	friend class times_law;
	friend class division_law;
	friend class exponent_law;
	friend double poly_multiple( law_polynomial *in_p1,
		                         law_polynomial *in_p2);
};	 


class DECL_LAW fact_list : public ACIS_OBJECT
{
public:
	int factor;
	int power;
	fact_list *next;
	fact_list(int in_factor,
		      int in_power,
			  fact_list *in_next);
};

int DECL_LAW factor_int(int input, 
			             int **factors,
			             int **powers);

int DECL_LAW GCD_int(int *list,
					  int size);

int DECL_LAW pow_int(int base,
					  int power);

////////////////////////////////////////////////////
// The solve_linear function takes in a SPAmatrix
// mat[row][column] with size row's and size+1 
// columns.
//
// For example if size=3
//
// mat[0][0]*a0+mat[0][1]*a1+mat[0][2]*a2=mat[0][3]
// mat[1][0]*a0+mat[1][1]*a1+mat[1][2]*a2=mat[1][3]
// mat[2][0]*a0+mat[2][1]*a1+mat[2][2]*a2=mat[2][3]
// 
// The function will set mat[0-2][3] equal to 
// a0, a1, a2
////////////////////////////////////////////////////

logical DECL_LAW solve_linear(double **mat,
							int    size);

void DECL_LAW sphere_cylinder_intersect( SPAposition sphere_center,
							    SPAposition cylinder_center,
								double   sphere_radius,
								double   cylinder_radius,
								SPAvector   cylinder_axis,
								int      *number_of_laws,
								law      ***laws,
								SPAinterval **intervals);

// functions for timing

void DECL_LAW law_start_time();

void DECL_LAW law_end_time();

double DECL_LAW law_get_time();


DECL_LAW double check_dlaw(
	law    *in_law, 
	int     w, 
	double  &problem_at= *(double *) NULL_REF   // where the law has problem
	);

DECL_LAW double check_ilaw(
	law     *in_law,
	double *&problem_at = (double *&) NULL_REF   // where the law has problem
	);

// functions for the law complier

class DECL_LAW codeline : public ACIS_OBJECT
{
	char *str;
	char *removestr;
public:
	codeline(law *inlaw,char *name,char *subname);
	~codeline();
	void print();
	void remove_print();
	codeline *next;
	codeline *last;
	void add(law *inlaw,char *name,char *subname);
};

// Same as strlen, but doesn't crash if str is NULL
DECL_LAW int strlen2(char const *str);

// Makes a string representation without all the garbage of sprintf.
// For example, "1.0000000000" goes to "1".
DECL_LAW char *number_string(double x, logical allow_scientific_notation = TRUE);


// returns true if number is within tolerance of zero
inline logical near_zero(double number, double tol = SPAresmch)
{
	return fabs( number ) <= tol;
}

// returns true if number is less than zero, tolerance area excluded
inline logical near_less_zero(double number, double tol = SPAresmch)
{
	return number < -tol;
}

// returns true if number is less than or equal to zero, tolerance area
// included
inline logical near_less_equal_zero(double number, double tol = SPAresmch)
{
	return number <= tol;
}

// returns true if number is greater than zero, tolerance area excluded
inline logical near_greater_zero(double number, double tol = SPAresmch)
{
	return number > tol;
}

// returns true if number is greater than or equal to zero, tolerance
// area included
inline logical near_greater_equal_zero(double number, double tol = SPAresmch)
{
	return number >= -tol;
}

// Returns a positive number (1) if A is greater than B.
// Return a negative integer (-1) if A is less than B.
// Returns 0 if they are equal.
DECL_LAW int position_cmp(const SPAposition &A, const SPAposition &B, double tol = SPAresabs);

#endif


