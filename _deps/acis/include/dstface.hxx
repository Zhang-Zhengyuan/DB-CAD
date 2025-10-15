/*******************************************************************/
/*    Copyright (c) 1989-2020 by Spatial Corp.                     */
/*    All rights reserved.                                         */
/*    Protected by U.S. Patents 5,257,205; 5,351,196; 6,369,815;   */
/*                              5,982,378; 6,462,738; 6,941,251    */
/*    Protected by European Patents 0503642; 69220263.3            */
/*    Protected by Hong Kong Patent 1008101A                       */
/*******************************************************************/
// file     : dstface.hxx
// created  : June 15, 1995
// author   : GWCelniker
// synopsis : Creation of a simple spline-face for testing purposes

#ifndef DS_MFACE_H
#define DS_MFACE_H

#include "dcl_adm.h"
#include "api.hxx"
class SPAposition;
class outcome ;
class FACE ;
class EDGE ;

// declare DS_build_test_spline_face() 
//         DS_build_test_spline_edge()
// and their errors

DECL_ADM outcome                    // ret: 0=o.k. or DS_CPT_CNT_TOO_LOW
DS_build_test_spline_face  // eff: build spline-face of input size
  (int degree_u,           // in : polynomial degree in u dir
   int degree_v,           // in : polynomial degree in v dir
   int num_cpts_u,         // in : num of control points in u
   int num_cpts_v,         // in : num of control points in v
   double x0,              // in : start of surface 
   double y0,              // in : start of surface
   double x_size,          // in : 0 <= surface_x <= x_size
   double y_size,          // in : 0 <= surface_y <= y_size
   double z_size,          // in : amplitude variation in surface
   int rational,           // in : 0=no(NUB),1=yes(NURB)
   FACE *&face,            // out: The constructed face
   double *u_min=NULL,     // in : optional min domain corner [0,0]
   double *u_max=NULL) ;   // in : optional max domain corner [1,1]

DECL_ADM outcome                    // ret: 0=o.k. otherwise error code 
DS_build_test_spline_edge  // egg: build spline-edge of input size
  (int degree,             // in : curve polynomial degree(typical=3)
   int num_cpts,           // in : curve control pt count
   double x0,              // in : start of surface 
   double x_size,          // in : 0 <= surface_x <= x_size
   double z_size,          // in : amplitude variation in surface
   int rational,           // in : 0=no(NUB),1=yes(NURB)
   double u_min,           // in : optional u min value(typical=0.0)
   double u_max,           // in : optional u max value(typical=1.0)
   EDGE *&edge) ;          // out: The constructed face

DECL_ADM SPAposition *                     
DS_build_test_scatter_set  // ret: array of positions
  (int &pt_count,          // in : in=random_pt_cnt,out=total_pt_cnt
   double x_size,          // in : 0 <= surface_x <= x_size
   double y_size,          // in : 0 <= surface_y <= y_size
   double z_size) ;        // in : amplitude variation in surface

DECL_ADM void 
DS_build_spline_test_arrays_full_u
 (int num_cpts_u,  int num_cpts_v,    // in : cpt count in each dir
  int degree_u,    int degree_v,      // in : degree in each dir
  int num_knots_u, int num_knots_v,   // in : knot count excluding
                                      //      multiple end knots
                           // num_knots = num_cpts - degree + 1
  double x0,               // in : start of surface 
  double y0,               // in : start of surface
  double x_size,           // in : 0 <= surface_x <= x_size
  double y_size,           // in : 0 <= surface_y <= y_size
  double z_size,           // in : amplitude variation in surface
  double u_min[2],         // in : min domain corner
  double u_max[2],         // in : max domain corner
  SPAposition *&cpts,         // out: allocated array of cpts 
  double   *&knots_u,      // out: allocated array of u knot values
  double   *&knots_v) ;    // out: allocated array of v knot values

DECL_ADM void DS_build_1dspline_test_arrays_full_u
 (int num_cpts,            // in : cpt count in each dir
  int degree,              // in : degree in each dir(typical=3)
  int num_knots,           // in : knot count excluding
                           //      multiple end knots
                           // num_knots = num_cpts - degree + 1
  double x0,               // in : start of surface 
  double x_size,           // in : 0 <= surface_x <= x_size
  double /*z_size*/,       // in : amplitude variation in surface
  double u_min,            // in : optional u_min value (typical=0.0)
  double u_max,            // in : optional u_max value (typical=1.0)
  SPAposition *&cpts,         // out: allocated array of cpts 
  double   *&knots) ;      // out: allocated array of u knot values

DECL_ADM void 
DS_build_spline_test_arrays_short_u
 (int num_cpts_u,  int num_cpts_v,    // in : cpt count in each dir
  int degree_u,    int degree_v,      // in : degree in each dir
  int num_knots_u, int num_knots_v,   // in : knot count excluding
                                      //      multiple end knots
                           // num_knots = num_cpts - degree + 1
  double x_size,           // in : 0 <= surface_x <= x_size
  double y_size,           // in : 0 <= surface_y <= y_size
  double z_size,           // in : amplitude variation in surface
  SPAposition *&cpts,         // out: allocated array of cpts 
  double   *&knots_u,      // out: allocated array of u knot values
  double   *&knots_v) ;    // out: allocated array of v knot values


#endif // DS_MFACE_H
