/*******************************************************************/
/*    Copyright (c) 1989-2020 by Spatial Corp.                     */
/*    All rights reserved.                                         */
/*    Protected by U.S. Patents 5,257,205; 5,351,196; 6,369,815;   */
/*                              5,982,378; 6,462,738; 6,941,251    */
/*    Protected by European Patents 0503642; 69220263.3            */
/*    Protected by Hong Kong Patent 1008101A                       */
/*******************************************************************/
// Functions to obtain boxes for data structure topological entity
// types. They use a SPAbox if already available, otherwise compute it
// from subsidiary ones, and save it for reuse.
/*******************************************************************/
#ifndef GETBOX_ROUTINES
#define GETBOX_ROUTINES
#include "dcl_kern.h"
#include "err_info.hxx"
/**
* @file getbox.hxx
 * @CAA2Level L1
 * @CAA2Usage U1
 * \addtogroup ACISBOXING
 *
 * @{
 */
class SPAbox;
class ENTITY;
class BODY;
class WIRE;
class LUMP;
class SHELL;
class SUBSHELL;
class FACE;
class LOOP;
class EDGE;
class SPAtransf;
class TCOEDGE;
/**
 * @nodoc
 */
DECL_KERN SPAbox get_body_box(BODY *this_body,
						   SPAtransf const *t = NULL,
						   logical tight_box = 0 );
/**
 * @nodoc
 */
DECL_KERN SPAbox get_wire_box( WIRE *,
						   SPAtransf const * = NULL,
						   logical tight_box = 0,
						   SPAbox *untransformed_box = NULL);
/**
 * @nodoc
 */
DECL_KERN SPAbox get_lump_box(LUMP *,
						   SPAtransf const * = NULL,
						   logical tight_box = 0,
						   SPAbox *untransformed_box = NULL);
/**
 * @nodoc
 */
DECL_KERN SPAbox get_shell_box( SHELL *,
							SPAtransf const * = NULL,
							logical tight_box = 0,
						    SPAbox *untransformed_box = NULL);
/**
 * @nodoc
 */
DECL_KERN SPAbox get_subshell_box( SUBSHELL *,
							   SPAtransf const * = NULL,
							   logical tight_box = 0,
							   SPAbox *untransformed_box = NULL);
/**
* Returns the bounding box for the given face.
* <br>
* <b>Role</b>: The default behavior of <tt>get_face_box</tt> is as follows:
* <br><br>
* <ul>
* <li> If a bounding box already exists, it is returned.
* <li> If no bounding box exists, then a bounding box is calculated but it is not 
*      guaranteed to be the tightest box available.
* </ul>
* <br><br>
* It is recommended that users call this function with the default values, 
* as ACIS has been optimised for this configuration.  However, if a tight box 
* is required for spherical, toroidal or spline faces, the logical argument 
* <tt>tight_box</tt> may be set to <tt>TRUE</tt>. This may cause <tt>get_face_box</tt> 
* to compute another box, even if a bounding box already exists.
* <br><br>
* The default computation of a spherical face is governed by the option 
* <tt>tight_sphere_box</tt>.  By default, this option is switched <i>off</i>.  
* This means that 
* any spherical face box that is computed will be cached, and therefore an 
* explicit request for a tight spherical face box will force a computation.  
* If <tt>tight_sphere_box</tt> is switched <i>on</i>, then only tight spherical face 
* boxes will be computed (and cached).  A box bounding the entire spherical 
* surface can be obtained through the SPHERE::make_box method.
* <br><br>
* The default computation of a toroidal face is governed by the option 
* <tt>tight_torus_box</tt>.  By default, this option is switched <i>on</i>.  This means that 
* only tight toroidal face boxes will be computed (and cached).  A box bounding 
* the entire toroidal surface can be obtained through the TORUS::make_box method.
* If <tt>tight_torus_box</tt> is switched <i>off</i>, then any toroidal face box that is 
* computed will be cached, and therefore an explicit request for a tight toroidal 
* face box will force a computation.
* <br><br>
* The default computation of a spline face is a tight box.  Only tight boxes for 
* spline faces will be computed (and cached).  This behavior is fixed for 
* optimal performance of ACIS.  A box bounding the entire spline surface can be 
* obtained through the SPLINE::make_box method.
* <br><br>
* ACIS has been configured to perform optimally with the default values for the
* options <tt>tight_sphere_box</tt> and <tt>tight_torus_box</tt>.  It is recommended 
* that users do 
* not change these values, as the options will eventually be withdrawn.  They 
* should <i>not</i> be changed mid-application.  They should be set once at the 
* beginning of the session.
* <br><br>
* If <tt>untransformed_box</tt> is supplied, it will be expanded to include the
* untransformed box of the face.
* <br><br>
* <b>Effect:</b> Changes model's derived data
* <br>
* @param this_face
* face to examine
* @param t
* transform to apply to box
* @param tight_box
* flag to calculate tight box
* @param untransformed_box
* accumulating box
**/
DECL_KERN SPAbox get_face_box( FACE *this_face,
						   SPAtransf const *t = NULL,
						   logical tight_box = FALSE,
						   SPAbox *untransformed_box = NULL );

/**
 * @nodoc
 */
DECL_KERN SPAbox get_loop_box( LOOP *,
						   SPAtransf const * = NULL,
						   logical tight_box = 0,
						   SPAbox *untransformed_box = NULL);
/**
 * @nodoc
 */
DECL_KERN SPAbox get_sensible_loop_box( LOOP *,
						   SPAtransf const * = NULL,
						   logical tight_box = 0,
						   SPAbox *untransformed_box = NULL);


/**
* Returns the bounding box for the given edge.
* <br>
* <b>Role</b>: If a bounding box already exists, it is returned. If there is no
* bounding box, it is calculated and returned. After a box is computed it is saved
* for later reuse. If <tt>tight_box</tt> flag is <tt>TRUE</tt> then a tighter box is 
* calculated. 
* <br><br>
* If <tt>untransformed_box</tt> is supplied, it will be expanded to include the
* untransformed box of the edge.
* <br><br>
* <b>Effect:</b> Changes model's derived data
* <br>
* @param this_edge
* edge to examine
* @param t
* transform to apply to box
* @param tight_box
* flag to calculate tight box
* @param untransformed_box
* accumulating box
**/
DECL_KERN SPAbox get_edge_box( EDGE *this_edge,
						   SPAtransf const *t = NULL,
						   logical tight_box = 0,
						   SPAbox *untransformed_box = NULL);
/**
 * @nodoc
 */
DECL_KERN SPAbox get_sensible_edge_box( EDGE *this_edge,
						   SPAtransf const *t = NULL,
						   logical tight_box = 0,
						   SPAbox *untransformed_box = NULL);
/**
 * @nodoc
 */
DECL_KERN SPAbox get_tcoedge_box( TCOEDGE *,
							  SPAtransf const * = NULL,
						      logical tight_box = 0,
						      SPAbox *untransformed_box = NULL );

/**
* Resets the box of the given entity and then resets the boxes of all owning entities.
* <br>
* <b>Role</b>: Resets the bounding box of the topological entity (i.e., sets it
* to NULL), then does the same for its owners. Its argument must be a BODY, LUMP,
* SHELL, SUBSHELL, FACE, LOOP, EDGE, WIRE, or VERTEX.
* <br>
* <b>Effect:</b> Changes model's derived data
* <br>
* @param this_entity
* topological entity
**/
DECL_KERN void reset_boxes( ENTITY *this_entity );

/**
* Resets the box of the given entity and then resets the boxes of all constituent entities.
* <br>
* <b>Role</b>: Resets the box of the given entity and then resets the boxes of
* all constituent boxes. In other words, it sets the box pointer to NULL for the
* portion of the topological tree below this entity.
* <br>
* <b>Effect:</b> Changes model's derived data
* <br>
* @param ent
* topological entity
**/
DECL_KERN void reset_boxes_downward( ENTITY* ent );

/**
* Resets the boxes of the given entities and then resets the boxes of all owning entities.
* <br>
* <b>Role</b>: Resets the bounding box of each given topological entity (i.e., sets it
* to NULL), then does the same for its owners. <tt>given_ents</tt> can 
* contain vertices, edges, loops, faces, and shells. If <tt>do_upper_topo</tt> is <tt>FALSE</tt>, 
* it invalidates boxes up to the face level only. If <tt>do_upper_topo</tt> is <tt>TRUE</tt>, 
* it also invalidates the bounding boxes on the topology above the face level.
* The purpose of this function is to invalidate boxes only once. It is more
* efficient than calling @href reset_boxes for each entity in the list if they share
* upper topology. 
* <br>
* <b>Effect:</b> Changes model's derived data
* <br>
* @param given_ents
* entities whose boxes need to be reset
* @param do_upper_topo
* invalidate the boxes on the owning body, lump, shell, and any subshells
**/
DECL_KERN void invalidate_boxes( ENTITY_LIST& given_ents, logical do_upper_topo );

/** @} */
#endif
