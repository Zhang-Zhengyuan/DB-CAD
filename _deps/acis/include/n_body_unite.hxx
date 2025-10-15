/*******************************************************************/
/*    Copyright (c) 1989-2020 by Spatial Corp.                     */
/*    All rights reserved.                                         */
/*    Protected by U.S. Patents 5,257,205; 5,351,196; 6,369,815;   */
/*                              5,982,378; 6,462,738; 6,941,251    */
/*    Protected by European Patents 0503642; 69220263.3            */
/*    Protected by Hong Kong Patent 1008101A                       */
/*******************************************************************/
#pragma once

#include "dcl_bool.h"
#include "boolapi.hxx"

class DECL_BOOL n_body_unite_options;

/**
* @file n_body_unite.hxx
* \addtogroup BOOLAPI
* @{
*/

/**
* The API that performs the N-Body Unite opertion.
* @par Technical Article
* <i>[N-Body Unite](http://docwiki/index.php/N-Body_Unite)</i>
* @par Role
* Often a union not only between two bodies, but many or N bodies 
* needs to be performed. The trivial approach of simply taking 
* the first body and looping over the remaining N-1 bodies while 
* uniting them is suboptimal in terms of performance and robustness.
* The N-Body Unite algorithm addresses the problem of optimizing 
* the order of unions between bodies. 
* N-Body Unite supports regularized and non-regularized unions.
* @par Journal
* Available
* @par Failsafe
* This API uses @href api_boolean and @href api_combine_body 
* internally. Upon completion all bodies have only one lump, and 
* as long as all unite and combine operations succeed no body 
* should intersect with any other body. 
* If one or more operations fail, the bodies involved in those 
* failures may intersect with eachother. Those bodies can be 
* queried from the returned @href outcome.
* @par Effect
* Changes model
* @par Products
* 3D ACIS Modeler
* @param[in,out] bl List of bodies. Only single-lump bodies upon return.
* @param[in] regularized Perform regularized or non-regularized unions.
* @param[in,out] bo BoolOptions, to be passed along to @href api_boolean.
* @param[in,out] nbuo Currently not in use.
* @param[in] ao AcisOptions, used for journalling and versioning.
* @return The @href outcome object indicating success or failure.
*/
DECL_BOOL
outcome api_n_body_unite(
	ENTITY_LIST& bl,
	const bool regularized = true,
	BoolOptions* bo = nullptr,
	const n_body_unite_options* nbuo = nullptr,
	const AcisOptions* ao = nullptr);

/**
* The API <tt>api_n_body_unite_2d</tt> performs an N-body unite operation on a collection of coplanar sheet bodies.
* @par Technical Article
* <i>[N-Body Unite](http://docwiki/index.php/N-Body_Unite)</i>
* @par Role
* Perform a regularized or non-regularized union of the input coplanar sheets. The algorithm optimizes the way in which
* coplanar sheet bodies are processed. The naive approach of looping over the bodies
* while uniting them one by one is sub-optimal and makes the robustness dependant
* on the order of the input bodies. Moreover, the general n-body unite API, <tt>api_n_body_unite</tt>,
* may not be the most optimal way to process a collection of 2D sheet bodies. This issue may become
* quite apparent when uniting a large number of coplanar sheets. This API provides an optimal
* way to perform an n-body unite operation on coplanar sheet bodies. All bodies in the output
* will be single-lump and should not intersect with any other body. Bodies involved in intermediate
* failures, however, may intersect with some other bodies from the output, and may not be single-lump.
* Note that the API does not explicitly check for coplanarity of the input sheet bodies.
* <br><br>
* The API can perform the n-body unite operation in a multi-threaded manner if the option, <tt>thread_hot_2d_n_body_unite</tt>, is set to TRUE.
* <br><br>
* @par Journal
* Available
* @par Failsafe
* This API uses @href api_boolean and @href api_combine_body internally, and so, can run into some
* intermediate failures. The API processes the input in a failsafe manner, which means that it 
* attempts to process the entire input. If there are some intermediate failures, the bodies 
* involved are noted and the algorithm proceeds to work on the rest of the input. These bodies
* can be queried from the returned @href outcome.
* @par Effect
* Changes model
* @par Products
* 3D ACIS Modeler
* @param[in,out] bodies List of coplanar sheet bodies to be united. Note that the input bodies are assumed to lie in a single plane.
* Output bodies (single-lump) are populated in the same entity list.
* @param[in] regularized Flag to perform a regularized/non-regularized union operation.
* @param[in] bopts Boolean options to be passed along to @href api_boolean.
* @param[in] ao AcisOptions useful for journalling and versioning.
* @return The @href outcome object indicating success or failure. Also useful for querying
* the list of bodies that were involved in intermediate failures.
*/
DECL_BOOL outcome api_n_body_unite_2d(
	ENTITY_LIST& bodies, 
	const bool regularized = true, 
	BoolOptions* bopts = nullptr,
	const AcisOptions* ao = nullptr);
