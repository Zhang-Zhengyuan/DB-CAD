/*******************************************************************/
/*    Copyright (c) 1989-2020 by Spatial Corp.                     */
/*    All rights reserved.                                         */
/*    Protected by U.S. Patents 5,257,205; 5,351,196; 6,369,815;   */
/*                              5,982,378; 6,462,738; 6,941,251    */
/*    Protected by European Patents 0503642; 69220263.3            */
/*    Protected by Hong Kong Patent 1008101A                       */
/*******************************************************************/
//    Declare procedures for getting topological entities
/*******************************************************************/

#ifndef get_top_hxx
#define get_top_hxx
#include "dcl_kern.h"
#include "logical.h"
#include "entity.hxx"
#include "kernapi.hxx"
class ENTITY;
class ENTITY_LIST;
/**
 * @file get_top.hxx
 * @CAA2Level L1
 * @CAA2Usage U1
 * \addtogroup ACISQUERIES
 *
 * @{
 */
/**
 * Get a list of <tt>LUMP</tt>s from an <tt>ENTITY</tt>.
 * For additional information regarding the arguments and behavior of this function 
 * refer to the documentation for the API function @href api_get_lumps.
 */
DECL_KERN void get_lumps( ENTITY* ent, ENTITY_LIST& lump_list, PAT_NEXT_TYPE include_pat = PAT_CAN_CREATE );
/**
 * Get a list of <tt>WIRE</tt>s from an <tt>ENTITY</tt>.
 * For additional information regarding the arguments and behavior of this function 
 * refer to the documentation for the API function @href api_get_wires.
 */
DECL_KERN void get_wires( ENTITY* ent, ENTITY_LIST& wire_list, PAT_NEXT_TYPE include_pat = PAT_CAN_CREATE );
/**
 * Get a list of <tt>SHELL</tt>s from an <tt>ENTITY</tt>.
 * For additional information regarding the arguments and behavior of this function 
 * refer to the documentation for the API function @href api_get_shells.
 */
DECL_KERN void get_shells( ENTITY* ent, ENTITY_LIST& shell_list, PAT_NEXT_TYPE include_pat = PAT_CAN_CREATE );
/**
 * Get a list of <tt>FACE</tt>s from an <tt>ENTITY</tt>.
 * For additional information regarding the arguments and behavior of this function 
 * refer to the documentation for the API function @href api_get_faces.
 */
DECL_KERN void get_faces( ENTITY* ent, ENTITY_LIST& face_list, PAT_NEXT_TYPE include_pat = PAT_CAN_CREATE );
/**
 * Get a list of <tt>LOOP</tt>s from an <tt>ENTITY</tt>.
 * For additional information regarding the arguments and behavior of this function 
 * refer to the documentation for the API function @href api_get_loops.
 */
DECL_KERN void get_loops( ENTITY* ent, ENTITY_LIST& loop_list, PAT_NEXT_TYPE include_pat = PAT_CAN_CREATE );
/**
 * Get a list of <tt>EDGE</tt>s from an <tt>ENTITY</tt>.
 * For additional information regarding the arguments and behavior of this function 
 * refer to the documentation for the API function @href api_get_edges.
*/
DECL_KERN void get_edges( ENTITY* ent, ENTITY_LIST& edge_list, PAT_NEXT_TYPE include_pat = PAT_CAN_CREATE );
/**
 * Get a list of <tt>COEDGE</tt>s from an <tt>ENTITY</tt>.
 * For additional information regarding the arguments and behavior of this function 
 * refer to the documentation for the API function @href api_get_coedges.
*/
DECL_KERN void get_coedges( ENTITY* ent, ENTITY_LIST& coed_list, PAT_NEXT_TYPE include_pat = PAT_CAN_CREATE );
/**
 * Get a list of <tt>VERTEX</tt>es from an <tt>ENTITY</tt>.
 * For additional information regarding the arguments and behavior of this function 
 * refer to the documentation for the API function @href api_get_vertices.
 */
DECL_KERN void get_vertices( ENTITY* ent, ENTITY_LIST& vert_list, PAT_NEXT_TYPE include_pat = PAT_CAN_CREATE );
/**
 * Get a list of <tt>TEDGE</tt>s from an <tt>ENTITY</tt>.
 * For additional information regarding the arguments and behavior of this function 
 * refer to the documentation for the API function @href api_get_tedges.
 */
DECL_KERN void get_tedges( ENTITY* ent, ENTITY_LIST& tedge_list, PAT_NEXT_TYPE include_pat = PAT_CAN_CREATE );
/**
 * Get a list of <tt>TCOEDGE</tt>s from an <tt>ENTITY</tt>.
 * For additional information regarding the arguments and behavior of this function 
 * refer to the documentation for the API function @href api_get_tcoedges.
 */
DECL_KERN void get_tcoedges( ENTITY* ent, ENTITY_LIST& tcoed_list, PAT_NEXT_TYPE include_pat = PAT_CAN_CREATE );
/**
 * Get a list of <tt>TVERTEX</tt>es from an <tt>ENTITY</tt>.
 * For additional information regarding the arguments and behavior of this function 
 * refer to the documentation for the API function @href api_get_tvertices.
 */
DECL_KERN void get_tvertices( ENTITY* ent, ENTITY_LIST& tvert_list, PAT_NEXT_TYPE include_pat = PAT_CAN_CREATE );
/**
 * Gets all specified entities related to an entity.
 * For additional information regarding the arguments and behavior of this function 
 * refer to the documentation for the API function @href api_get_entities.
 * @param entity
 * given <tt>ENTITY</tt>
 * @param list
 * Resulting list of <tt>ENTITY</tt>s
 * @param topo_id
 * Type of topology to search for (may be composed)
 * @param geom_id
 * Type of geometry to search for (may be composed)
 * @param include_pat
 * include patterns
 */
DECL_KERN logical get_entities( ENTITY * entity, ENTITY_LIST& list, ENTITY_ID topo_id, ENTITY_ID geom_id, PAT_NEXT_TYPE include_pat = PAT_CAN_CREATE);
/**
 * Get a list of laminar <tt>EDGE</tt>s from an <tt>ENTITY</tt>.
 * A laminar edge, or free edge, is represented by an EDGE with only one COEDGE.
 */
DECL_KERN void get_laminar_edges(ENTITY * ent, ENTITY_LIST & edge_list, PAT_NEXT_TYPE include_pat = PAT_CAN_CREATE); 
/**
 * Get all connected <tt>ENTITY</tt>s.
 */
DECL_KERN void get_connected_entities(ENTITY * ent, ENTITY_LIST & list);
/**
 * Get the face <tt>LOOP</tt> determined by the two input <tt>ENTITY</tt>s.
 * If the two <tt>ENTITY</tt>s do not define a unique <tt>LOOP</tt>, then the first
 * <tt>LOOP</tt> that they have in common is returned.  If they do not share
 * a common <tt>LOOP</tt>, then <tt>NULL</tt> is returned.
 */
DECL_KERN ENTITY* find_loop( ENTITY* ent1, ENTITY* ent2);

/**
 * Get the approximate number of faces from the entities in the list.
 * This function behaves similar to <tt>api_get_faces</tt>, in how it traverses
 * the topological hierarchy of all input entities to find connected faces.
 * However, the results may not be 100% accurate because the algorithm
 * is meant to be fast as opposed to thorough. Use this function when
 * a rough estimate is sufficient.
 */
DECL_KERN unsigned get_approx_face_count( const ENTITY_LIST& list);
/**
 * Get the approximate number of edges from the entities in the list.
 * This function behaves similar to <tt>api_get_edges</tt>, in how it traverses
 * the topological hierarchy of all input entities to find connected edges.
 * However, the result may not be 100% accurate because the algorithm
 * is meant to be fast as opposed to thorough. Use this function when
 * a rough estimate is sufficient.
 */
DECL_KERN unsigned get_approx_edge_count( const ENTITY_LIST& list);

/** @} */
#endif
