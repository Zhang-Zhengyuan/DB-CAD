/*******************************************************************/
/*    Copyright (c) 1989-2020 by Spatial Corp.                     */
/*    All rights reserved.                                         */
/*    Protected by U.S. Patents 5,257,205; 5,351,196; 6,369,815;   */
/*                              5,982,378; 6,462,738; 6,941,251    */
/*    Protected by European Patents 0503642; 69220263.3            */
/*    Protected by Hong Kong Patent 1008101A                       */
/*******************************************************************/



#ifndef UNFOLDAPI_HXX
#define UNFOLDAPI_HXX


#include "api.hxx"
#include "dcl_hlc.h"

class ENTITY_LIST; 		
class AcisOptions;
class unfold_options;

/**
 * @file unfoldapi.hxx
 */

/*! \addtogroup WARPAPI
 * @{
 */

/**
 * This API unfolds connected list of non enclosed faces  @href BODY
 * <br><br>
 * <b>Technical Articles:</b> <i>[Unfold Sheet Body](https://doc.spatial.com/articles/u/n/f/Unfold_Sheet_Body_c677.html)</i>
 * <br><br>
 * <b>Role:</b> This API performs unfolding of input list of faces such that the distortion is minimum.
 * The default behavior is to return the flattened sheet body.
 * To get best results, input faces should have developable surfaces.
 * Giving more or less faces than needed can affect quality and
 * performance.
 * <br><br>
 * <b>Errors:</b>
 * <ul>
 * <li><tt>WRAP_DIFFICULT_GEOMETRY</tt>: Difficult geometric conditions encountered. This typically happens because of high curvature regions</li>
 * </ul>
 * <br><br>
 * <b>Limitations:</b>
 * <ul>
 * <li>A non-manifold input body is not supported. </li>
 * </ul>
 * <b>Journal:</b> Available
 * <br><br>
 * <b>Product(s):</b> 3D ACIS Modeler
 * <br><br>
 * @param inputFaces
 * List of faces to be unfolded.
 * @param flattenedBody
 * Result of unfolding. Use @href unfold_options to control the behavior.
 * @param uo
 * Used to control unfolding solution.
 * @param ao
 * ACIS options for versioning and journaling
 */
DECL_HLC outcome api_unfold(
	ENTITY_LIST inputFaces, 
	BODY *& flattenedBody, 
	unfold_options* uo = NULL, 
	AcisOptions const* ao = NULL);

/**
 * This API unfolds sheet @href BODY
 * <br><br>
 * <b>Technical Articles:</b> <i>[Unfold Sheet Body](https://doc.spatial.com/articles/u/n/f/Unfold_Sheet_Body_c677.html)</i>
 * <br><br>
 * <b>Role:</b> This API performs unfolding of input sheet body such that the distortion is minimum.
 * The default behavior is to return the flattened sheet body.
 * To get best results, sheet body with developable surfaces should be provided.
 * Giving more or less faces than needed can affect quality and
 * performance.
 * <br><br>
 * <b>Errors:</b>
 * <ul>
 * <li><tt>WRAP_DIFFICULT_GEOMETRY</tt>: Difficult geometric conditions encountered. This typically happens because of high curvature regions</li>
 * </ul>
 * <br><br>
 * <b>Limitations:</b>
 * <ul>
 * <li>A non-manifold input body is not supported. </li>
 * </ul>
 * <b>Journal:</b> Available
 * <br><br>
 * <b>Product(s):</b> 3D ACIS Modeler
 * <br><br>
 * @param inputBody
 * A sheet body to be unfolded.
 * @param flattenedBody
 * Result of unfolding. Use @href unfold_options to control the behavior.
 * @param uo
 * Used to control unfolding solution.
 * @param ao
 * ACIS options for versioning and journaling
 */
DECL_HLC outcome api_unfold(
	BODY* inputBody, 
	BODY*& flattenedBody, 
	unfold_options* uo = NULL, 
	AcisOptions const* ao = NULL);

/** @} */

#endif

