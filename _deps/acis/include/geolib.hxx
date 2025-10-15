// $Id: geolib.hxx,v 1.14 2002/08/09 17:15:32 jeff Exp $
/*******************************************************************/
/*    Copyright (c) 1989-2020 by Spatial Corp.                     */
/*    All rights reserved.                                         */
/*    Protected by U.S. Patents 5,257,205; 5,351,196; 6,369,815;   */
/*                              5,982,378; 6,462,738; 6,941,251    */
/*    Protected by European Patents 0503642; 69220263.3            */
/*    Protected by Hong Kong Patent 1008101A                       */
/*******************************************************************/

#ifndef GEOLIB_HXX
#define GEOLIB_HXX

#include "dcl_kern.h"
#include "acis3dt.h"
#include "vector.hxx"
#include "position.hxx"
#include "logical.h"

class FACE;
class LOOP;

//////////////////////////////////////////////////////////////////////////////
//
//  This function tests to see if and where a segment defined by the seg_start
//  and the seg_end hits the triangle defined by the points a, b, and c.  
//
//////////////////////////////////////////////////////////////////////////////

DECL_KERN logical segment_hit_triangle( SPAposition const &a,
						   			    SPAposition const &b,
									    SPAposition const &c,
									    SPAposition const &seg_start,
									    SPAposition const &seg_end,
									    SPAposition &hit,
									    double   &param,
                                        logical  &coincident = *(logical *)NULL_REF);

//////////////////////////////////////////////////////////////////////////////
//
//  This function test to see if and where a ray defined by the ray_root and
//  the ray_dir hits the triangle defined by the points a, b, and c.  The
//  param is such that hit=param*ray_dir+ray_root.  FALSE is returned if the
//  ray does hit the triangle.
//
//////////////////////////////////////////////////////////////////////////////

DECL_KERN logical ray_hit_triangle( SPAposition const &a,
									SPAposition const &b,
									SPAposition const &c,
									SPAposition const &ray_root,
									SPAvector   const &ray_dir,
									SPAposition &hit,
									double   &param,
                                    logical  &coincident = *(logical *)NULL_REF);

DECL_KERN logical plane_ray_intersect( SPAposition const &plane_root,
									   SPAvector   const &plane_norm,
									   SPAposition const &ray_root,
									   SPAvector   const &ray_dir,
									   SPAposition &hit,
									   double   &param);

DECL_KERN logical segment_ray_intersect( SPAposition const &p1,
                                         SPAposition const &p2,
                                         SPAposition const &ray_root,
                                         SPAvector   const &ray_dir,
                                         SPAposition &hit,
                                         double   &param);

///////////////////////////////////////////////////////////////////////////////
//
//  This function tests to see if and where two lines, defined as p0 + t0 * q0 
//  and p1 + t1 * q1, intersect. Even when the lines do not intersect, t0 and
//  t1 will indicate the closest points on the lines.
//
///////////////////////////////////////////////////////////////////////////////

DECL_KERN logical line_line_intersect( SPAposition const &p0,
                                       SPAvector   const &q0,
                                       double   &t0,
                                       SPAposition const &p1,
                                       SPAvector   const &q1,
                                       double   &t1,
                                       SPAposition &hit);

DECL_KERN logical same_side( SPAposition const &p1,
							 SPAposition const &p2,
							 SPAposition const &plane_root,
							 SPAvector   const &plane_norm);

//////////////////////////////////////////////////////////////////////////////
//
//  This function return the projected SPAposition of the point p onto the plane
//  containing the points a, b and c.  If the points a, b and c are colinear
//  then it projects them onto the line formed by the points.  If the points
//  are all the same, then the point a is returned.
//
//////////////////////////////////////////////////////////////////////////////

DECL_KERN SPAposition project_point_to_plane( SPAposition const &a,
										   SPAposition const &b,
										   SPAposition const &c,
										   SPAposition const &p);

//////////////////////////////////////////////////////////////////////////////
//
//  This function return the projected SPAposition of the point p onto the line
//  containing the points a, and b.  If the points a, b are the same, then the
//  point a is returned.
//
//////////////////////////////////////////////////////////////////////////////

DECL_KERN SPAposition project_point_to_line( SPAposition const &a,
										  SPAposition const &b,
										  SPAposition const &p);

//////////////////////////////////////////////////////////////////////////////
//
//  This function return true if the point p is on the line a b.
//
//////////////////////////////////////////////////////////////////////////////

DECL_KERN logical point_on_line( SPAposition const &a,
								 SPAposition const &b,
								 SPAposition const &p);

//////////////////////////////////////////////////////////////////////////////
//
//  This function returns TRUE if a, b, and c are colinear AND distinct.
//  It calls project_point_to_line and measures the distance.
//
//////////////////////////////////////////////////////////////////////////////

DECL_KERN logical points_colinear(	SPAposition const & a, 
									SPAposition const & b, 
									SPAposition const & c,
									double tol = SPAresnor);

//////////////////////////////////////////////////////////////////////////////
 
DECL_KERN logical check_face_normal( FACE *in_face);

DECL_KERN double  loop_winding_number( LOOP *in_loop );

#endif

