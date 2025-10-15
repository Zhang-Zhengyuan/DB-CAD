/*******************************************************************/
/*    Copyright (c) 1989-2020 by Spatial Corp.                     */
/*    All rights reserved.                                         */
/*    Protected by U.S. Patents 5,257,205; 5,351,196; 6,369,815;   */
/*                              5,982,378; 6,462,738; 6,941,251    */
/*    Protected by European Patents 0503642; 69220263.3            */
/*    Protected by Hong Kong Patent 1008101A                       */
/*******************************************************************/
// File    : dsgauss.hxx
// created : Jan 12, 1995
// Author  : GW Celniker
// contains : DS_ntgrl_degree_is_in_range()
//            DS_ntgrl_degree_not_in_range()
//            DS_linear_gauss_pt_count()
//            DS_linear_ntgrl_degree()
//            DS_calc_gpt_locs()
//            extern double DS_gauss_pt[][] and
//            extern double DS_guass_wt[][]
// Summary : Numerical Integration of functions is best done by
//           Gaussian integration in which the integral is 
//           approximated by a weighted	sum of the function values 
//           at various points throughout the integral's domain.  
//           This file contains the weight values for integration 
//           over rectangular domains.
//


#ifndef DS_GAUSS_h
#define DS_GAUSS_h

//  Gaussian Integration approximates
//   -1                 N
//   Int( f(x) dx )  = Sum( f(xi) * Hi )
//   +1                i=0
//
//    Where N  equal the number of sample points and results in
//             exact integration of polynomials of degree 2N-1, and
//          xi are the sample points listed below, and
//          Hi are the sample weights also listed below, and
//        f(x) is the function to be integrated over the domain 
//             of -1 <= x <= +1
//    Integration over other domain ranges is accomplished
//    by scaling the gauss point locations so that they fill the
//    new domain, and multiplying the resultant integrand sum by
//    the jacobian  which is just dx/du
//            du = the width of the domain being integrated
//            dx = the width of the default domain -1 to 1 = 2.0
//


#define DS_GAUSS_COUNT  40 // maximum number of gauss points
#define DS_GAUSS_ERROR  1.0e-12

#define DS_MAX_NTGRL_DEGREE 79 // max ntgrl_degree for DS_GAUSS_COUNT

inline int 
DS_ntgrl_degree_is_in_range  // ret: 1=val is supported, 0=unsupported
 (int ntgrl_degree)          // in : gauss integration accuracy  
{ return ( (ntgrl_degree > 0 && ntgrl_degree <= DS_MAX_NTGRL_DEGREE)
          ? 1 : 0) ;
}
inline int 
DS_ntgrl_degree_not_in_range // ret: 1=val unsupported, 0=is supported
 (int ntgrl_degree)          // in : gauss integration accuracy  
{ return ( (ntgrl_degree > 0 && ntgrl_degree <= DS_MAX_NTGRL_DEGREE)
          ? 0 : 1) ;
}

int                        // ret: gauss-pt-count needed to exactly
 DS_linear_gauss_pt_count  //      integrate an ntgrl-degree polynomial.
 (int ntgrl_degree) ;      // in : specifies max polynomial degree
                           //      to be exactly integrated
                           // note: Gauss integration exactly 
                           // integrates polynomials of degree
                           // 2 * gauss_pt_count - 1

int                        // ret: max degree of polynomial exactly
 DS_linear_ntgrl_degree    //      integrated by gpt-count gauss pts.
 (int gpt_count) ;         // in : number of gauss pts used.


void                       // eff: calc and store in gpt_loc the
 DS_calc_gpt_locs          //      gauss pt locations for domain_box   
  (int domain_dim,         // in : dimension of domain
   int ntgrl_degree,       // in : gauss integration accuracy
   double *domain_min,     // in : lower corner of domain_box
   double *domain_max,     // in : upper corner of domain_box
   double *gpt_loc) ;      // out: [u0,u1..,v0,v1...,w0,w1...]
                           // sized:[DS_linear_gauss_pt_count
                           //        (ntgrl_degree)* domain_dim]
   
// The following are the gauss point weights for n = 1 to 10
// valid for an integration over a domain that is 2 units wide.

// note: that the zero entry into this array is not used
//       DS_gauss_wt[n] = array of n weights for n gauss pts

extern double DS_gauss_wt[DS_GAUSS_COUNT+1][DS_GAUSS_COUNT] ;

// example the gauss weights for four gauss points are found at
//    DS_gauss_wt[4][0], DS_Gauss_wt[4][1],
//    DS_gauss_wt[4][2], DS_gauss_wt[4][3]
//
// you should never call DS_gauss_wt[0][...]

// The following gauss point locations integrate over -1 <= u <= 1
// or multiplied by h               to integrate over -h <= u <= h
// remember to scale the final integrated sum by h/2.0
// to get the properly scaled integral

// note: that the zero entry into this array is not used
//       DS_gauss_pt[n] = array of n locations for n gauss pts

extern double DS_gauss_pt[DS_GAUSS_COUNT+1][DS_GAUSS_COUNT] ;


// example the gauss locations between -1.0 to 1.0 for four gauss 
// points are found at
//    DS_gauss_pt[4][0], DS_Gauss_pt[4][1],
//    DS_gauss_pt[4][2], DS_gauss_pt[4][3]
//
// you should never call DS_gauss_pt[0][...]


//  Gauss point locations to integrate over domain ranges 
//            other than -1 <= x <= +1
//         b                   +1
//  Integral(g(u) du) =  Integral(g(u(x)) du/dx dx
//         a                   -1
//
//           +1                     N
// and Integral(g(u(x)) du = du/dx Sum (g(u(xi)) * Hi
//           -1                    i=0
//
//  Example:  Integrate g(u) from 2 to 5
//
//   default x  is -1 <= x <= +1 and
//
//   u = 3/2 x + 3
//  du/dx = 3/2
//   ui = 3/2 xi + 3
//
//         5                   N
//  Integral(g(u) du) = 3/2 * Sum (g(ui) * Hi)
//         2                  i=0
//
// Conclusion:  The above Quadrature numbers can be used to 
//              integrate any domain of function h(u)
//              using the Hi values as given and generating a new 
//              set of ui values
//              with ui = g_inv(xi)
//              and multiplying the result by du/dx
//                du = the domain size of the integrated function
//                dx = 2, the domain size of the default -1 to +1
//


#endif // DS_GAUSS_H
