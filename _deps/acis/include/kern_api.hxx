/*******************************************************************/
/*    Copyright (c) 1989-2020 by Spatial Corp.                     */
/*    All rights reserved.                                         */
/*    Protected by U.S. Patents 5,257,205; 5,351,196; 6,369,815;   */
/*                              5,982,378; 6,462,738; 6,941,251    */
/*    Protected by European Patents 0503642; 69220263.3            */
/*    Protected by Hong Kong Patent 1008101A                       */
/*******************************************************************/
/**
* ACIS Compound
*/
#if !defined(KERN_API_HXX)
#define KERN_API_HXX
#include "dcl_compound.h"
#include "api.hxx"
#include "entity_handle_list.hxx"
#include "entity_handle.hxx"
#include "vers.hxx"
/**
 * @file kern_api.hxx
 * \addtogroup ACISCOMPOUND
 * @{
 */
/**
* <summary>Gets all faces related to an entity.</summary>
* <param name="entity">Input entity handle.</param>
* <param name="list">List of faces, in the form of entity_handle_list.</param>
* <param name="ao">ACIS Options.</param>
*/
DECL_COMPOUND outcome api_get_faces(entity_handle* entity, entity_handle_list& list, AcisOptions* ao = NULL);
/**
* <summary>Gets all edges related to an entity.</summary>
* <param name="entity">Input entity handle.</param>
* <param name="list">List of edges, in the form of entity_handle_list.</param>
* <param name="ao">ACIS Options.</param>
*/
DECL_COMPOUND outcome api_get_edges(entity_handle* entity, entity_handle_list& list,AcisOptions* ao = NULL);
/**
* <summary>Gets the owner Transform.</summary>
* <param name="entity">Input entity handle.</param>
* <param name="transf">Resulting transform, if non found, returns identity transform.</param>
* <param name="ao">ACIS Options.</param>
*/
DECL_COMPOUND outcome api_get_owner_transf(entity_handle* entity, SPAtransf& transf,AcisOptions* ao = NULL);
/**
* <summary>Get this ACIS version.</summary>
* <param name="version">Version.</param>
*/
DECL_COMPOUND outcome api_get_acis_version(AcisVersion& version);
/** @} */
#endif
