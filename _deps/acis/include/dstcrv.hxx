/*******************************************************************/
/*    Copyright (c) 1989-2020 by Spatial Corp.                     */
/*    All rights reserved.                                         */
/*    Protected by U.S. Patents 5,257,205; 5,351,196; 6,369,815;   */
/*                              5,982,378; 6,462,738; 6,941,251    */
/*    Protected by European Patents 0503642; 69220263.3            */
/*    Protected by Hong Kong Patent 1008101A                       */
/*******************************************************************/
// file     : dstcrv.hxx
// created  : June 15, 1995
// author   : GWCelniker
// synopsis : Creation of a simple spline-curve for testing purposes

#ifndef DS_TCURVE_H
#define DS_TCURVE_H

#include "dcl_adm.h"

class pcurve ;
class surface ;
class SPApar_pos;
class FACE ;
class SPAposition;
class DS_tprod_1d ;

DECL_ADM pcurve *                     
DS_build_str_spline_pcurve // ret: spline-curve of input size
  (surface &surf,          // in : tgt-face for pcurve
   SPApar_pos &p0,            // in : start point of pcurve line
   SPApar_pos &p1) ;          // in : stop point of pcurve line

DECL_ADM pcurve *               
DS_build_circ_spline_pcurve // ret: spline-curve of input size
  (surface &surf,          // in : tgt-face for pcurve
   SPApar_pos &center,        // in : center of circle
   double radius,          // in : radius of circle
   int npts = 8);          // in : number of pts used to approximate circle

DECL_ADM pcurve *                     
DS_build_parab_spline_pcurve // ret: spline-curve of input size
  (surface &surf,          // in : tgt-face for pcurve
   SPApar_pos &p0,            // in : start point of pcurve line
   SPApar_pos &tx,            // in : tangent intersection point
   SPApar_pos &p1) ;          // in : stop point of pcurve line

DECL_ADM pcurve *                     
DS_build_test_spline_pcurve // ret: spline-curve of input size
  (surface &surf,          // in : tgt-face for pcurve
   int degree,              // in : curve polynomial degree
   int num_cntrl_pts,       // in : num of curve control points
   SPApar_pos &p0,             // in : start pt of curve
   SPApar_pos &p1,             // in : end pt of curve
   double amp,              // in : magnitude of variation from line
   int cycles) ;            // in : number of oscillations made
                            //      while traversing from p0 to p1

DECL_ADM void DS_build_spline_pcurve_test_arrays_full
 (int num_cntrl_pts,       // in : cpt count in each dir
  int degree,              // in : degree in each dir
  int num_knots,           // in : knot count excluding
                           //      multiple end knots
                           // num_knots = num_cntrl_pts - degree + 1
  SPApar_pos &p0,             // in : start pt of curve
  SPApar_pos &p1,             // in : end pt of curve
  double amp,              // in : amplitude variation in curve
  int cycles,              // in : number of cycles in variation
  SPAposition *&cntrl_pts,    // out: allocated array of cntrl_pts 
  double   *&knots) ;      // out: allocated array of knot values

#endif // DS_TCURVE_H
