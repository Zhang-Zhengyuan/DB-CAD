/* ORIGINAL: acis2.1/kernutil/d3_mth/math.hxx */
/* $Id: math.hxx,v 1.11 2002/08/09 17:15:30 jeff Exp $ */
/*******************************************************************/
/*    Copyright (c) 1989-2020 by Spatial Corp.                     */
/*    All rights reserved.                                         */
/*    Protected by U.S. Patents 5,257,205; 5,351,196; 6,369,815;   */
/*                              5,982,378; 6,462,738; 6,941,251    */
/*    Protected by European Patents 0503642; 69220263.3            */
/*    Protected by Hong Kong Patent 1008101A                       */
/*******************************************************************/

#ifndef MATHH
#define MATHH

//
//    Maths utilities
//   

#include <math.h>
#include "acis.hxx"				
#include "dcl_kern.h"

#define SPA_HALF_PI 1.57079632679489661922

 

inline DECL_KERN double D3_fabs( double a ) { return a < 0.0 ? -a : a; }

inline DECL_KERN double D3_sign( double a ) { return a < 0.0 ? -1.0 : 1.0; }

inline DECL_KERN double D3_min( double a, double b ) { return a < b ? a : b; }

inline DECL_KERN double D3_min( double a, double b, double c ) 
	{ return a < b ? a < c ? a : c : b < c ? b : c; }

inline DECL_KERN double D3_min( double a, double b, double c, double d ) 
	{ return a < b ? a < c ? a < d ? a : d
			       : c < d ? c : d
		       : b < c ? b < d ? b : d
			       : c < d ? c : d; }

inline DECL_KERN double D3_min( double a, double b, double c, double d, double e ) 
	{ return a < b ? a < c ? a < d ? a < e ? a : e
				       : d < e ? d : e
			       : c < d ? c < e ? c : e
				       : d < e ? d : e
		       : b < c ? b < d ? b < e ? b : e
				       : d < e ? d : e
			       : c < d ? c < e ? c : e
				       : d < e ? d : e; }

inline DECL_KERN double D3_max( double a, double b ) { return a > b ? a : b; }

inline DECL_KERN double D3_max( double a, double b, double c ) 
	{ return a > b ? a > c ? a : c : b > c ? b : c; }

inline DECL_KERN double D3_max( double a, double b, double c, double d ) 
	{ return a > b ? a > c ? a > d ? a : d
			       : c > d ? c : d
		       : b > c ? b > d ? b : d
			       : c > d ? c : d; }

inline DECL_KERN double D3_max( double a, double b, double c, double d, double e ) 
	{ return a > b ? a > c ? a > d ? a > e ? a : e
				       : d > e ? d : e
			       : c > d ? c > e ? c : e
				       : d > e ? d : e
		       : b > c ? b > d ? b > e ? b : e
				       : d > e ? d : e
			       : c > d ? c > e ? c : e
				       : d > e ? d : e; }
/*
		This function now is avaliable as "safe_cos"
		it is defined at baseutil/vector/safe_math.hxx

inline DECL_KERN double D3_acos( double a ) 
	{ return a>1 ? 0  :  a<-1 ? M_PI  :  acos(a); }
*/
extern DECL_KERN GLOBAL_CONST double SPAnull;

extern DECL_KERN GLOBAL_CONST double d3_large_value;


// All the same (except acos!) for integer arguments and return value.

inline DECL_KERN int D3_fabs( int a ) { return a < 0 ? -a : a; }

inline DECL_KERN int D3_sign( int a ) { return a < 0 ? -1 : 1; }

inline DECL_KERN int D3_min( int a, int b ) { return a < b ? a : b; }

inline DECL_KERN int D3_min( int a, int b, int c ) 
	{ return a < b ? a < c ? a : c : b < c ? b : c; }

inline DECL_KERN int D3_min( int a, int b, int c, int d ) 
	{ return D3_min( a, D3_min( b, c, d ) ); }

inline DECL_KERN int D3_min( int a, int b, int c, int d, int e ) 
	{ return D3_min( a, D3_min( b, c, d, e ) ); }

inline DECL_KERN int D3_max( int a, int b ) { return a > b ? a : b; }

inline DECL_KERN int D3_max( int a, int b, int c ) 
	{ return a > b ? a > c ? a : c : b > c ? b : c; }

inline DECL_KERN int D3_max( int a, int b, int c, int d ) 
	{ return D3_max( a, D3_max( b, c, d ) ); }

inline DECL_KERN int D3_max( int a, int b, int c, int d, int e ) 
	{ return D3_max( a, D3_max( b, c, d, e ) ); }


// Equation solvers. The number of roots is returned, and the roots are placed
// in the array x (which must be sufficiently long, i.e. 3 for a cubic, 4 for a
// quartic. 

// Solve  x*x*x + a*x*x + b*x + c = 0. 

extern DECL_KERN 	int	D3_solve_cubic( double* x, double a, double b, double c );

// Solve  x*x*x*x + a*x*x*x + b*x*x + c*x + d = 0. 

extern DECL_KERN 	int	D3_solve_quartic( double* x, 
				  double a, double b, double c, double d, 
				  double res=SPAresabs );

#endif
