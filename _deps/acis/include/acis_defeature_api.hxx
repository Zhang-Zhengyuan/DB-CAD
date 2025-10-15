/*******************************************************************/
/*    Copyright (c) 1989-2020 by Spatial Corp.                     */
/*    All rights reserved.                                         */
/*    Protected by U.S. Patents 5,257,205; 5,351,196; 6,369,815;   */
/*                              5,982,378; 6,462,738; 6,941,251    */
/*    Protected by European Patents 0503642; 69220263.3            */
/*    Protected by Hong Kong Patent 1008101A                       */
/*******************************************************************/

#ifndef _ACIS_DEFEATURE_API_HXX
#define _ACIS_DEFEATURE_API_HXX

#include "kernapi.hxx"
#include "lists.hxx"
#include "api.err"
#include "check.hxx"
#include "dcl_defeature.h"
/**
* @file acis_defeature_api.hxx
 * @CAA2Level L1
 * @CAA2Usage U2
 * \addtogroup DFTAPI
 *
 * @{
 */
class defeature_options;

/**
* Recognizes and/or removes features (holes, chamfers, and blends) in the given body.
* <br>
* <b>Role:</b> This API function takes a body and as per the defeature_options object passed in, 
* recognizes and/or removes features.
* <br><br>
* The behavior of this API function is controlled using the defeature_options object. The defeature_options object 
* controls the actions of the API function. If a call is made
* with <tt>pause_after_recognize</tt> as <tt>FALSE</tt>, feature recognition is done but feature removal is not done. 
* The defeature_options object controls the type of features removed as well as specify parameters about the features.
* The types of features that can be removed are:
* <ul>
* <li>Holes</li>
* <li>Blends</li>
* <li>Chamfers</li>
* </ul>
* The removal of these features can be controlled using the parameters in the defeature_options object.
* For example, if only blends are to be removed, then the <tt>do_holes</tt> and <tt>do_chamfers</tt> 
* parameters are set to <tt>FALSE</tt>. 
* Further, the blends that are removed can be controlled by setting the <tt>blend_max_radius</tt> parameter
* to the appropriate value. Similarly, the behavior of the hole and chamfer removal can be controlled
* by setting the appropriate parameters on the defeature_options object. 
* <br><br>
* <b>Errors:</b> None
* <br><br>
* <b>Limitations:</b> The input body must be manifold.
* <br><br>
* <b>Library:</b> SPADefeature
* <br><br> 
* <b> Effect:</b> Changes model
* <br><br>
* <b>Journal:</b> Available
* <br><br>
* <b>Product(s):</b> 3D ACIS Defeaturing
* <br><br>
* @param input_body 
* BODY that needs to be defeatured
* @param dfopts 
* defeature_options object that specifies the behavior of the API function.
* @param aopts AcisOptions 
* General ACIS Options
**/

DECL_DEFEATURE outcome api_find_and_remove_features( BODY* input_body, 
														     defeature_options* dfopts = NULL,
														     AcisOptions* aopts = NULL );


/**
* Recognizes and/or removes features (holes, chamfers, and blends) in the input bodies.
* <br>
* <b>Role:</b> This API function takes an ENTITY_LIST of bodies and as per the defeature_options object passed in, 
* recognizes and/or removes features.
* <br><br>
* The behavior of this API function is controlled using the defeature_options object. The defeature_options
* object controls the actions of the API function. If a call is made
* with <tt>pause_after_recognize</tt> as <tt>FALSE</tt>, feature recognition is done but feature removal is not done. 
* The defeature_options object controls the type of features removed as well as specify parameters about the features.
* The types of features that can be removed are:
* <ul>
* <li>Holes</li>
* <li>Blends</li>
* <li>Chamfers</li>
* </ul>
* The removal of these features can be controlled using the parameters in the defeature_options object.
* For example, if only blends are to be removed, then the <tt>do_holes</tt> and <tt>do_chamfers</tt> 
* parameters are set to <tt>FALSE</tt>. 
* Further, the blends that are removed can be controlled by setting the <tt>blend_max_radius</tt> parameter
* to the appropriate value. Similarly, the behavior of the hole and chamfer removal can be controlled
* by setting the appropriate parameters on the defeature_options object. 
* <br><br>		
* <b>Errors:</b> None
* <br><br>
* <b>Limitations:</b> The input bodies must be manifold.
* <br><br>
* <b>Library:</b> SPADefeature
* <br><br>
* <b> Effect:</b> Changes model
* <br><br>
* <b>Journal:</b> Available
* <br><br>
* <b>Product(s):</b> 3D ACIS Defeaturing
* <br><br>
* @param input_bodies 
* ENTITY_LIST of bodies that need to be defeatured
* @param dfopts 
* defeature_options object that specifies the behavior of the API function.
* @param aopts AcisOptions 
* General ACIS Options
**/

DECL_DEFEATURE outcome api_find_and_remove_features( const ENTITY_LIST& input_bodies, 
														     defeature_options* dfopts = NULL,
														     AcisOptions* aopts = NULL );
/** @} */

#endif // _ACIS_DEFEATURE_API_H

