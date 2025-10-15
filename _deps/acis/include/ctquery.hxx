/* ORIGINAL: acis2.1/ct_husk/query/ctquery.hxx */
// $Id: ctquery.hxx,v 1.13 2002/08/09 17:20:42 jeff Exp $
/*******************************************************************/
/*    Copyright (c) 1989-2020 by Spatial Corp.                     */
/*    All rights reserved.                                         */
/*    Protected by U.S. Patents 5,257,205; 5,351,196; 6,369,815;   */
/*                              5,982,378; 6,462,738; 6,941,251    */
/*    Protected by European Patents 0503642; 69220263.3            */
/*    Protected by Hong Kong Patent 1008101A                       */
/*******************************************************************/

#ifndef CTQUERY_HXX
#define CTQUERY_HXX

//--------------------------------------------------------------
//	Include file for the ct_query directory
//--------------------------------------------------------------

#include "dcl_ct.h"
//#include "ptinbody.hxx"
#include "ptcont.hxx"
#include "ptfcenum.hxx"
#include "raytest.hxx"
#include "cthdr.hxx"

class insanity_list;

/**
 * @file ctquery.hxx
 * @CAA2Level L1
 * @CAA2Usage U1
 * \addtogroup CELLTOPAPI
 *
 * @{
 */

// **********************************
// Containment Checking Functions
// **********************************

/**
 * Determines whether a given point lies inside, outside, or on the boundary of a 3D cell.
 * <br><br>
 * <b>Role:</b> This Direct Interface function determines the containment of a point (given in global
 * coordinate space) for a given cell. Returns a <tt>point_containment</tt> value of
 * <tt>point_inside</tt>, <tt>point_outside</tt>, <tt>point_boundary</tt>, or <tt>point_unknown</tt>.
 * <br><br>
 * Although containment calculations do not make substantive changes to a model, bounding
 * boxes may be calculated and cached in the model, creating a bulletin board. To make the
 * process read-only for the user, wrap the call to <tt>ct_point_in_cell</tt> in an 
 * API_NOP_BEGIN / API_NOP_END block.
 * <br><br>
 * <b>Effect:</b> Changes model
 * <br><br>
 * @param given_pnt
 * point to be tested.
 * @param this_cell
 * cell to be examined.
 **/
DECL_CT point_containment ct_point_in_cell ( SPAposition const &given_pnt, CELL3D *this_cell, ENTITY_LIST &cface_list = *(ENTITY_LIST *)NULL_REF );

/**
 * Determines whether a given point lies on or off a 2D cell.
 * <br><br>
 * <b>Role:</b> This Direct Interface function determines the containment of a point (given in global
 * coordinate space) for a given cell. 
 * <br><br>
 * Although containment calculations do not make substantive changes to a model, bounding
 * boxes may be calculated and cached in the model, creating a bulletin board. To make the
 * process read-only for the user, wrap the call to <tt>ct_point_in_cell</tt> in an 
 * API_NOP_BEGIN / API_NOP_END block.
 * <br><br>
 * <b>Effect:</b> Changes model
 * <br><br>
 * @param given_pnt
 * point to be tested.
 * @param this_cell
 * cell to be examined.
 **/
DECL_CT logical ct_point_in_cell ( SPAposition const &given_pnt, CELL2D *this_cell );


/**
 * @nodoc
 */
DECL_CT SPAposition ct_find_cshell_point ( CSHELL *, SPAposition const& = *(SPAposition *)NULL_REF);

/**
 * @nodoc
 */
DECL_CT logical ct_cshell_is_peripheral ( CSHELL *this_cshell );

/**
 * @nodoc
 */
DECL_CT point_containment do_ray_fire( SPAposition const &cshell_pnt,
											SPAposition const &test_pnt,
											CSHELL            *this_cshell);

// **********************************
// Bounding Box Functions
// **********************************

/**
* Returns the bounding box for the given 3D cell.
* <br>
* <b>Role</b>: The default behavior of <tt>ct_get_cell_box</tt> is as follows:
* <br><br>
* <ul>
* <li> If a bounding box already exists on the cell, it is returned.
* <li> If no bounding box exists, then a bounding box is calculated but it is not 
*      guaranteed to be the tightest box available.
* </ul>
* <br><br>
* <br><br>
* Effect: Changes model's derived data
* @param this_cell
* cell to examine
* @param t
* transform to apply to box
**/
DECL_CT SPAbox ct_get_cell_box( CELL3D *this_cell, SPAtransf const *t = NULL);

/**
* Returns the bounding box for the given 2D cell.
* <br>
* <b>Role</b>: The default behavior of <tt>ct_get_cell_box</tt> is as follows:
* <br><br>
* <ul>
* <li> If a bounding box already exists on the cell, it is returned.
* <li> If no bounding box exists, then a bounding box is calculated but it is not 
*      guaranteed to be the tightest box available.
* </ul>
* <br><br>
* <br><br>
* Effect: Changes model's derived data
* @param this_cell
* cell to examine
* @param t
* transform to apply to box
**/
DECL_CT SPAbox ct_get_cell_box( CELL2D *this_cell, SPAtransf const *t = NULL);

/**
* Returns the bounding box for the given cell.
* <br>
* <b>Role</b>: The default behavior of <tt>ct_get_cell_box</tt> is as follows:
* <br><br>
* <ul>
* <li> If a bounding box already exists on the cell, it is returned.
* <li> If no bounding box exists, then a bounding box is calculated but it is not 
*      guaranteed to be the tightest box available.
* </ul>
* <br><br>
* <br><br>
* Effect: Changes model's derived data
* @param this_cell
* cell to examine
* @param t
* transform to apply to box
**/
DECL_CT SPAbox ct_get_cell_box( CELL *this_cell,   SPAtransf const *t = NULL);

/**
* Returns the bounding box for the given supercell.
* <br>
* <b>Role</b>: The default behavior of <tt>ct_get_supercell_box</tt> is as follows:
* <br><br>
* <ul>
* <li> If a bounding box already exists on the supercell, it is returned.
* <li> If no bounding box exists, then a bounding box is calculated but it is not 
*      guaranteed to be the tightest box available.
* </ul>
* <br><br>
* <br><br>
* Effect: Changes model's derived data
* @param this_supercell
* supercell to examine
* @param t
* transform to apply to box
**/
DECL_CT SPAbox ct_get_supercell_box( SUPERCELL *this_supercell, SPAtransf const *t = NULL);

/**
* Returns the bounding box for the given cshell.
* <br>
* <b>Role</b>: The default behavior of <tt>ct_get_cshell_box</tt> is as follows:
* <br><br>
* <ul>
* <li> If a bounding box already exists on the cshell, it is returned.
* <li> If no bounding box exists, then a bounding box is calculated but it is not 
*      guaranteed to be the tightest box available.
* </ul>
* <br><br>
* <br><br>
* Effect: Changes model's derived data
* @param this_cshell
* cshell to examine
* @param t
* transform to apply to box
**/
DECL_CT SPAbox ct_get_cshell_box( CSHELL *this_cshell, SPAtransf const *t = NULL);

// **********************************
// Raytest Functions
// **********************************

/**
 * Tests a ray against the given 3D cell.
 * <br><br>
 * <b>Role:</b> The Direct Interface function raytest fires a ray at a 3D cell and returns on the
 * heap a list of hits or NULL if no hits are found. The heap contains the entity
 * hit, the type of hit, and the parameter value at the hit. Only entities in the forward
 * direction along the ray can be hit.
 * <br><br>
 * If the ray hits the interior of a face, the face is returned; if it hits an edge
 * of a face (i.e., two adjacent faces), the edge is returned; and if it hits all
 * faces adjacent at a vertex, the vertex is returned.
 * <br><br>
 * A call to this routine may cause boxes to be computed and thus the model may change.
 * <br><br>
 * <b>Effect:</b> Changes model's derived data
 * <br><br>
 * @param this_ray
 * given ray.
 * @param this_cell
 * target cell.
 **/
DECL_CT hit* ct_raytest_cell ( ray& this_ray, CELL3D* this_cell );

/**
 * @nodoc
 */
DECL_CT hit* ct_raytest_cshell ( ray& this_ray, CSHELL* this_cshell );

/** @} */

#endif // CTQUERY_HXX
