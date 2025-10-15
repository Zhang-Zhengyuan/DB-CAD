/*******************************************************************/
/*    Copyright (c) 1989-2020 by Spatial Corp.                     */
/*    All rights reserved.                                         */
/*    Protected by U.S. Patents 5,257,205; 5,351,196; 6,369,815;   */
/*                              5,982,378; 6,462,738; 6,941,251    */
/*    Protected by European Patents 0503642; 69220263.3            */
/*    Protected by Hong Kong Patent 1008101A                       */
/*******************************************************************/
// Header for sg_husk api routines.
#if !defined( CLEAR_API_ROUTINES )
#define CLEAR_API_ROUTINES
#include "dcl_clr.h"
#include "api.hxx"
/**
 * @file clearapi.hxx
 * @CAA2Level L1
 * @CAA2Usage U1
 * \addtogroup INTRAPICLEARANCE
 *
 * @{
 */

class BODY;
class FACE;
class SPAposition;

/**
* Initializes the clearance library.
* <br><br>
* <b>Effect:</b> System routine
* <br><br>
* <b>Journal: </b> Not Available
* <br><br>
* <b>Product(s):</b> 3D ACIS Modeler
 * <br><br>
**/
DECL_CLR outcome api_initialize_clearance();

/**
* Terminates the clearance library.
* <br><br>
* <b>Effect:</b> System routine
* <br><br>
* <b>Journal: </b> Not Available
* <br><br>
* <b>Product(s):</b> 3D ACIS Modeler
 * <br><br>
**/
DECL_CLR outcome api_terminate_clearance();

// Determine the minimum distance between two bodies or if the two bodies are within the
// specified minimum clearance.
/**
* Evaluates the clearance between two solid bodies.
* <br><br>
* <b>Role:</b> There are two modes of operation, depending upon the value of the argument <tt>min_clearance</tt>:
* <br><br>
* When <tt>min_clearance</tt> is set to zero, the minimum distance between the two
* bodies is found.
* <br><br>
* When <tt>min_clearance</tt> is greater than zero, the API returns early with two points, one on
* either body, separated by
* less than this value.  The points do not necessarily represent the minimum distance in
* this case.
* <br><br>
* Facet resolution is usually the internal (nonzero) default, which is obtained by
* setting <tt>facet_resolution</tt> to zero. Optional control of this value is supplied because
* it has a large effect on performance (larger tolerance means better performance).
* <br><br>
* Because faceted representations of the 3D B-spline surface approximations are
* used, and these facets have an accuracy of <tt>SPAresfit</tt>, a returned minimum
* distance less than <tt>SPAresfit</tt> is equivalent to zero.
* <br><br>
* <b>Errors:</b> The pointer to a body is <tt>NULL</tt> or does not point to a <tt>BODY</tt>.
* The specified clearance or resolution is less than zero.
* Wire bodies are invalid inputs to this API.
* <br><br>
* <b>Effect:</b> Read-only
* <br><br>
* <b>Journal: </b> Not Available
* <br><br>
* <b>Product(s):</b> 3D ACIS Modeler
 * <br><br>
* @param user_body1
* first body used for the clearance check.
* @param user_body2
* second body used for the clearance check.
* @param min_clearance
* minimum clearance setting.
* @param facet_resolution
* maximum departure of facet from a face. A value of zero causes the non-zero default to be used.
* @param p1
* point on <tt>user_body1</tt> at the minimum distance (output).
* @param p2
* point on <tt>user_body2</tt> at the minimum distance (output).
* @param min_dist
* minimum distance between the two bodies (output).
* @param ao
* acis options.
**/
DECL_CLR outcome api_check_solid_clearance(
		BODY* user_body1,
		BODY* user_body2,
		double min_clearance, 		// required minimum distance; 0. if true minimum desired
		double facet_resolution, 	// max departure of facet from face. 0 -> use default
		SPAposition& p1, 			// OUT: pair of points at minimum distance or
		SPAposition& p2,			// OUT: at clearance violation
		double& min_dist, 			// OUT: distance between points
		AcisOptions* ao = NULL );

// Determine the minimum distance between two face or if the two faces are within the
// specified minimum clearance.
/**
* Evaluates the clearance between two faces.
* <br><br>
* <b>Role:</b> There are two modes of operation, depending upon the value of the argument <tt>min_clearance</tt>:
* <br><br>
* When <tt>min_clearance</tt> is set to zero, the minimum distance between the two
* faces is found.
* <br><br>
* When <tt>min_clearance</tt> is greater than zero, the API returns early with two points, one
* on either face, separated by
* less than this value.  The points do not necessarily represent the minimum distance in
* this case.
* <br><br>
* Facet resolution is usually the internal (nonzero) default, which is obtained by
* setting <tt>facet_resolution</tt> to zero. Optional control of this value is supplied because
* it has a large effect on performance (larger tolerance means better performance).
* <br><br>
* Because faceted representations of the 3D B-spline surface approximations are
* used, and these facets have an accuracy of <tt>SPAresfit</tt>, a returned minimum
* distance less than <tt>SPAresfit</tt> is equivalent to zero.
* <br><br>
* <b>Errors:</b> The pointer to a face is <tt>NULL</tt> or does not point to a <tt>FACE</tt>.
* The specified clearance or resolution is less than zero.
* <br><br>
* <b>Effect:</b> Read-only
* <br><br>
* <b>Journal: </b> Not Available
* <br><br>
* <b>Product(s):</b> 3D ACIS Modeler
 * <br><br>
* @param face1
* first face used for the clearance check.
* @param face2
* second face used for the clearance check.
* @param min_clearance
* minimum clearance setting.
* @param facet_resolution
* maximum departure of facet from a face. A value of zero causes the non-zero default to be used.
* @param p1
* point on <tt>face1</tt> at the minimum distance (output).
* @param p2
* point on <tt>face2</tt> at the minimum distance (output).
* @param min_dist
* minimum distance between the two faces (output).
* @param ao
* acis options.
**/
DECL_CLR outcome api_check_face_clearance(
		FACE* face1,
		FACE* face2,
		double min_clearance, 		// required minimum distance; 0. if true minimum desired
		double facet_resolution, 	// max departure of facet from face. 0 -> use default
		SPAposition& p1, 			// OUT: pair of points at minimum distance or
		SPAposition& p2,			// OUT: at clearance violation
		double& min_dist, 			// OUT: distance between points
		AcisOptions* ao = NULL );
/** @} */
#endif
