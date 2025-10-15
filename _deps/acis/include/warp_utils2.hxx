/* $Id: warp_utils2.hxx,v 1.11 2002/08/09 17:30:51 jeff Exp $*/
/*******************************************************************/
/*    Copyright (c) 1989-2020 by Spatial Corp.                     */
/*    All rights reserved.                                         */
/*    Protected by U.S. Patents 5,257,205; 5,351,196; 6,369,815;   */
/*                              5,982,378; 6,462,738; 6,941,251    */
/*    Protected by European Patents 0503642; 69220263.3            */
/*    Protected by Hong Kong Patent 1008101A                       */
/*******************************************************************/

#ifndef WARP_UTILS_HXX
#define WARP_UTILS_HXX

#include "dcl_warp.h"
#include "logical.h"

class ENTITY;
class SPAposition;
class SPAunit_vector;
class BODY;
class SPAtransf;
class ENTITY_LIST;
class law;
class SPAvector;
class outcome;

// Returns the distance from the specified axis to the farthest point
// on the bounding SPAbox of the given entity.
// Defined in oper/operator/sg_husk/api/api_twist.cxx
double DECL_WARP get_range_to_axis(const ENTITY *my_entity, 
								const SPAposition & origin,
								const SPAunit_vector & axis);

// Imprints a circular disc of the give origin, axis, and radius onto the entity.
// Defined in oper/operator/sg_husk/api/api_twist.cxx
outcome DECL_WARP imprint_plane(const ENTITY *my_entity, 
								const SPAposition & origin,
								const SPAunit_vector & axis,
								double radius);

// Get the BODY transform
const DECL_WARP SPAtransf* GetTheBodyTransform(BODY* body);

// Given a list of faces (whole_face_list),
// returns a new sublist containing only the faces "inside" of the plane.
// That is, faces on the side of the plane that the normal points toward are EXcluded.
//
// It is assumed that the faces do NOT straddle the plane.
// In other words, if it straddles it should be pre-imprinted.
int DECL_WARP subset_faces_by_plane(/*const */ENTITY_LIST & whole_face_list, 
						 const SPAposition & plane_origin, 
						 const SPAunit_vector & plane_normal,
						 ENTITY_LIST & partial_face_list
						 );

// Given a list of edges (whole_edge_list),
// returns a sublist containing only the edges "inside" of the plane.
// That is, edges on the side of the plane that the normal points toward are EXcluded.
//
// It is assumed that the edges do NOT straddle the plane.
// In other words, if it straddles it should be pre-imprinted.
int DECL_WARP subset_edges_by_plane(/*const */ENTITY_LIST & whole_edge_list, 
						 const SPAposition & plane_origin, 
						 const SPAunit_vector & plane_normal,
						 ENTITY_LIST & partial_edge_list
						 );

// Creates a law that rotates about the specified axis by theta radians
DECL_WARP law *  create_rotate_law(SPAposition & pos, SPAunit_vector & normal, double theta);

// Creates a law that rotates about the X axis by theta radians
DECL_WARP law * create_rotate_law_x(double theta);

// Creates a law that translates by the given distances
DECL_WARP law * create_translate_law(const SPAvector &distances);

// Creates a law that translates along the X axis by the given distance
DECL_WARP law * create_translate_law_x(double distance);

// Sets all edges and vertices of each face to have or not have the "no-warp" attribute.
void DECL_WARP set_no_warp_attributes(ENTITY_LIST & face_list, logical on);

// Sets all edges and vertices of each face to have or not have the "transf-warp" attribute.
void DECL_WARP set_transf_warp_attributes(ENTITY_LIST & face_list, logical on);

// Creates an identity law of the specified dimension.
DECL_WARP law * create_identity_law(int dim);

// Creates a G1 continuous scaling law and its inverse.
// Values from [0,heightA] get mapped to [0,heightB].
// Also, the derivatives at 0 and heightA are 1.0, causing the smooth blend.
//
// NOTE that a simple cubic polynomial does not work well for two reasons.
// 1. does not have a well defined inverse.
// 2. is not guaranteed to be monotonically increasing, as this sin function is.
void DECL_WARP create_g1_blend_law(
					double heightA, // original height
					double heightB, // stretched height
					law * &answer,
					law * &inverse
					);

// Finds theta_max to be used in the sin() G1 blending functions.
double DECL_WARP find_theta_max(
					double heightA, // original height
					double heightB); // stretched height

// Creates a law that maps values from the range [in_low,in_high] to [out_low,out_high].
DECL_WARP law * create_linear_map_law(law * in, double in_low, double in_high, double out_low, double out_high);

// Maps values from the range [in_low,in_high] to [out_low,out_high].
// This is trusted.  Don't pass in bad values!
double DECL_WARP solve_linear_map(double in, double in_low, double in_high, double out_low, double out_high);

//
// Bending constants and functions:
//

// Returns the minimum bend width allowed.
double DECL_WARP MinimumBendWidth();

// Finds bend radius given bend angle and bend width.
double DECL_WARP CalcBendRadius(double angle, double width);

// Compute radius, angle and width properly.
// Returns FALSE if the given data is bad.
logical DECL_WARP FixValues(double & radius, double & angle, double & width, double max_width, logical do_unbend = FALSE);

// Finds the extents of the given entity beyond the specified plane, in both direction.
void DECL_WARP DistanceBeyondPlane(const ENTITY * my_entity, const SPAposition & p, const SPAunit_vector & n,
								double &min_extent, double &max_extent, logical do_unbend = FALSE);

#endif


