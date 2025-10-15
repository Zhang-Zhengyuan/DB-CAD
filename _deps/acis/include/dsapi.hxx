/*******************************************************************/
/*    Copyright (c) 1989-2020 by Spatial Corp.                     */
/*    All rights reserved.                                         */
/*    Protected by U.S. Patents 5,257,205; 5,351,196; 6,369,815;   */
/*                              5,982,378; 6,462,738; 6,941,251    */
/*    Protected by European Patents 0503642; 69220263.3            */
/*    Protected by Hong Kong Patent 1008101A                       */
/*******************************************************************/
// bmt 15-Apr-03 fixed DOC
// file     : dsapi.hxx
// created  : Oct 23, 1995
// author   : GWCelniker
// synopsis : The interface for the deformable modeling husk
//            consists of two parts.  The api functions which
//            manage placing and extracting deformable modeling
//            data as ACIS attribute data and the methods of the
//            ATTRIB_DM2ACIS attribute object.  Coding to the
//            deformable modeling husk is expected to use these
//            API funcions to initialize and save sculpting
//            results on ACIS faces.  Actual sculpting interfaces
//            will primarily depend on the direct interface
//            exposed through the ATTRIB_DM2ACIS object.
// contains:
//           api_initialize_deformable_modeling()
//           api_terminate_deformable_modeling()
//           api_dm_get_attrib_dm2acis()
//           api_dm_commit_attrib_dm2acis()
//           api_dm_remove_attrib_dm2acis()
//           api_dm_add_patch()
//           api_dm_rm_patch()
//           api_dm_add_multi_face()
//           api_dm_rm_multi_face()
//           api_dm_set_tolerance()
//           api_dm_journal_on()
//           api_dm_journal_play()
//           api_dm_journal_off()

#ifndef DS_DSAPI_H
#define DS_DSAPI_H

// the error number returns

#include "dcl_adm.h"
#include "adm_options.hxx"
#include "api.hxx"
/**
 * \defgroup ACISDM ACIS Deformable Modeling
 * \ingroup DEFMODAPI
 *
 */
/**
* @file dsapi.hxx
 * @CAA2Level L1
 * @CAA2Usage U1
 * \addtogroup ACISDM
 *
 * @{
 */
//#include "dshusk/dskernel/dsstdef.hxx" // DS_DEFAULT_NTGRL_DEGREE
class outcome ;
class ENTITY ;
class FACE ;
class EDGE ;
class ATTRIB_DM2ACIS ;
class DS_dmod;
class DM_dbl_array;
class ENTITY_LIST;

#include "param.hxx"  // class SPApar_pos  // class SPApar_pos
                                             // class SPApar_vec

// STI dgh begin
/**
 * Initializes the deformable modeling library.
 * <br><br>
 * <b>Effect:</b> System routine
 * <br><br>
* <b>Journal: </b> Not Available
* <br><br>
 * <b>Product(s):</b> 3D ACIS Deformable Modeling
 * <br><br>
* @param admO
 * adm options.
 * @param acisO
 * ACIS options such as version and journal.
 **/
DECL_ADM outcome
api_initialize_deformable_modeling(
  adm_options *admO=NULL, AcisOptions *acisO=NULL);
/**
 * Terminates the deformable modeling library.
 * <br><br>
 * <b>Effect:</b> System routine
 * <br><br>
* <b>Journal: </b> Not Available
* <br><br>
 * <b>Product(s):</b> 3D ACIS Deformable Modeling
 * <br><br>
* @param admO
 * adm options.
 * @param acisO
 * ACIS options such as version and journal.
 **/
DECL_ADM outcome
api_terminate_deformable_modeling(
  adm_options *admO=NULL, AcisOptions *acisO=NULL);

// STI dgh end
/**
 * Gets an <tt>ATTRIB_DM2ACIS</tt> attribute for the input entity if one exists, otherwise creates a new one.
 * <br><br>
 * <b>Role:</b> Searches the input entity's deformable modeling hierarchy for an <tt>ATTRIB_DM2ACIS</tt> and
 * returns a pointer to it when found, after ensuring that the <tt>ATTRIB_DM2ACIS</tt>
 * internal data structures are up to date.
 * <br><br>
 * When no <tt>ATTRIB_DM2ACIS</tt> is found a new <tt>ATTRIB_DM2ACIS</tt> is created for the input entity using
 * either an existing <tt>ATTRIB_DSMODEL</tt> or default values and a pointer to the new <tt>ATTRIB_DM2ACIS</tt>
 * is returned. When the entity is not part of a deformable modeling hierarchy, a one member
 * <tt>ATTRIB_DSGROUP</tt> and an <tt>ATTRIB_DSMODEL</tt> (owned by the input entity) are created.
 * <br><br>
 * <b>Errors:</b> <tt>DS_API_NOT_FACE_OR_EDGE</tt> - The entity is not a face or an edge, and therefore is not sculptable.
 * <br><br>
 * <b>Effect:</b> Changes model
 * <br><br>
 * @param entity
 * in:target FACE or EDGE.
 * @param attrib_dm2acis
 * out: pointer to new or retrieved attrib_dm2acis for entity.
 * @param admO
 * adm options.
 * @param acisO
 * ACIS options such as version and journal.
 **/
DECL_ADM outcome
api_dm_get_attrib_dm2acis         // rtn: 0 or DS_NOT_FACE_OR_EDGE
 (ENTITY *entity,                 // in : the tgt Face or EDGE
  ATTRIB_DM2ACIS *&attrib_dm2acis,// out: ptr to new or retrieved
                                  //      attrib_dm2acis for entity.
  adm_options *admO=NULL, AcisOptions *acisO=NULL);
/**
 * Gets an <tt>ATTRIB_DM2ACIS</tt> attribute for the input ACIS entity, if one exists, or returns <tt>NULL</tt>.
 * <br><br>
 * <b>Role:</b> This function searches the entity's deformable modeling hierarchy for an
 * <tt>ATTRIB_DM2ACIS</tt> and returns a pointer to it when found. When no <tt>ATTRIB_DM2ACIS</tt> is
 * found <tt>NULL</tt> is returned.
 * <br><br>
 * <b>Effect:</b> Read-only
 * <br><br>
* <b>Journal: </b> Not Available
* <br><br>
 * <b>Product(s):</b> 3D ACIS Deformable Modeling
 * <br><br>
* @param entity
 * target.
 * @param dm2acis
 * pointer returned.
 * @param admO
 * adm options.
 * @param acisO
 * ACIS options such as version and journal.
 **/
DECL_ADM outcome
api_dm_query_attrib_dm2acis       // rtn: 0
(ENTITY *entity,                  // in : the tgt FACE or EDGE
 ATTRIB_DM2ACIS *&dm2acis,        // out: ptr to retrieved
                                  //      attrib_dm2acis for owner, or NULL
 adm_options *admO=NULL, AcisOptions *acisO=NULL);
/**
 * Gets an <tt>ATTRIB_DM2ACIS</tt> attribute for the input <tt>DS_dmod</tt>, if one exists, or returns <tt>NULL</tt>.
 * <br><br>
 * <b>Role:</b> This function searches the entity's patch hierarchy for an
 * <tt>ATTRIB_DM2ACIS</tt> and returns a pointer to it when found. When no <tt>ATTRIB_DM2ACIS</tt> is
 * found <tt>NULL</tt> is returned.
 * <br><br>
 * <b>Effect:</b> Read-only
 * <br><br>
* <b>Journal: </b> Not Available
* <br><br>
 * <b>Product(s):</b> 3D ACIS Deformable Modeling
 * <br><br>
* @param dmod
 * target.
 * @param dm2acis
 * pointer returned.
 * @param admO
 * adm options.
 * @param acisO
 * ACIS options such as version and journal.
 **/
DECL_ADM outcome
api_dm_query_attrib_dm2acis       // rtn: 0
(DS_dmod* dmod,                   // in : the tgt DS_dmod
 ATTRIB_DM2ACIS *&dm2acis,        // out: ptr to retrieved
 adm_options *admO=NULL, AcisOptions *acisO=NULL);
/**
 * Copies the <tt>ATTRIB_DM2ACIS</tt> surface geometry into its owning entity's surface geometry and copies the deformable model data into its owner's attributes.
 * <br><br>
 * <b>Role:</b> Replaces the owner's surface geometry with the <tt>ATTRIB_DM2ACIS</tt> surface
 * geometry. Deformable modeling operations affect only the non-persistent data of the
 * <tt>ATTRIB_DM2ACIS</tt> attribute: deformable modeling changes to the <tt>ATTRIB_DM2ACIS</tt>
 * attribute can not be undone with rollback. When a user
 * is satisfied with deformable modeling results, this function is used to commit the
 * changes to the owning entity. Changes made to the <tt>owner</tt> are backed up
 * and can be undone with rollback.
 * <br><br>
 * Additionally, this function stores the current state of the deformable model
 * within a persistent <tt>ATTRIB_DSMODEL</tt> attribute.
 * <br><br>
 * <b>Errors:</b> <tt>DS_API_NO_ATTRIB_DM2ACIS</tt> - The input <tt>ATTRIB_DM2ACIS</tt> pointer cannot be NULL.
 * <br><br>
 * <b>Effect:</b> Changes model
 * <br><br>
* <b>Journal: </b> Not Available
* <br><br>
 * @param attrib_dm2acis
 * pointer to the owner's deformable model attribute.
 * @param admO
 * adm options.
 * @param acisO
 * ACIS options such as version and journal.
 **/
DECL_ADM outcome                   // use: to save sculpting effects
api_dm_commit_attrib_dm2acis      // eff: move the attrib_dm2acis
 (ATTRIB_DM2ACIS *attrib_dm2acis, //      shape and data to the
                                  //      owner_entity's geometry and attrib_dsmodel.
  adm_options *admO=NULL, AcisOptions *acisO=NULL);

/**
 * Removes all deformable modeling attributes from an entity.
 * <br><br>
 * <b>Role:</b> It is intended to cause bodies to "forget" that their geometry was
 * created by deformable modeling.
 * <br><br>
 * <b>Errors:</b> <tt>NULL_ENTITY</tt> - The input entity pointer cannot be <tt>NULL</tt>.
 * <br><br>
 * <b>Effect:</b> Changes model
 * <br><br>
* <b>Journal: </b> Not Available
* <br><br>
 * @param entity
 * pointer to entity.
 * @param admO
 * adm options.
 * @param acisO
 * ACIS options such as version and journal.
 **/
DECL_ADM outcome                   // eff: removes all persistent DM attributes
api_dm_remove_dm_attributes       //       from an entity
 (ENTITY *entity,
  adm_options *admO=NULL, AcisOptions *acisO=NULL);

/**
 * Removes the non-persistent <tt>ATTRIB_DM2ACIS</tt> attribute from its owning entity; the owning entity is either a face or an edge.
 * <br><br>
 * <b>Role:</b> Removes the non-persistent <tt>ATTRIB_DM2ACIS</tt> attribute from its
 * owning entity without storing the deformable modeling surface geometry and deformable model
 * data back within the owning entity. This ends the deformable modeling session.
 * <br><br>
 * <b>Effect:</b> Changes model
 * <br><br>
* <b>Journal: </b> Not Available
* <br><br>
 * @param attrib_dm2acis
 * pointer to the owner's deformable model attribute.
 * @param admO
 * adm options.
 * @param acisO
 * ACIS options such as version and journal.
 **/
DECL_ADM outcome                   // eff: remove the attrib_dm2acis
api_dm_remove_attrib_dm2acis      //      data from its owner without
 (ATTRIB_DM2ACIS *attrib_dm2acis, //      saving any sculpting
                                  //      changes made since the last
                                  // api_dm_commit_attrib_dm2acis() call
  adm_options *admO=NULL, AcisOptions *acisO=NULL);
/**
 * Adds a deformable patch to an entity for local deformations.
 * <br><br>
 * <b>Role:</b> Adds a patch to a parent target deformable model in preparation
 * for local deformations. The <tt>shape_flag</tt> argument specifies the shape of the new
 * patch. When <tt>shape_flag</tt> equals 1, a rectangular patch is added whose lower corner
 * is domain_pt1 and whose upper corner is <tt>domain_pt2</tt>. When <tt>shape_flag</tt> equals 2, an
 * elliptical patch is added whose center point is <tt>domain_pt1</tt>, the first axis is
 * the vector from <tt>domain_pt1</tt> to <tt>domain_pt2</tt>, and the second axis is the vector from
 * <tt>domain_pt1</tt> to <tt>domain_pt3</tt>. When <tt>shape_flag</tt> equals 3, a rectangular patch is added
 * whose corners are given arc fillets of radius equal to <tt>domain_pt3</tt>.
 * <br><br>
 * When <tt>shape_flag</tt> equals 1, <tt>domain_pt3</tt> is not used. The domain point arguments are
 * all <tt>SPApar_pos</tt> objects. They specify locations in the domain space of the target
 * parent deformable model whose domain space always maps to the unit square.
 * <br><br>
 * The refinement argument specifies the density of the patch control points as
 * compared to the parents. A refinement value of 1 specifies that the new patch
 * control point density is the same as that of the target deformable model. A
 * refinement value of 2 specifies that the new patch has twice the density of
 * control points as the parent. The refinement value must be an integer greater
 * than 0.
 * <br><br>
 * The <tt>tag_flag</tt> argument specifies which deformable model within the <tt>attrib_dm2acis</tt>
 * patch hierarchy is to be the parent of the new patch. When <tt>tag_flag</tt> equals 2 the root
 * deformable model receives the patch. Any other value specifies the deformable
 * model whose tag value is equal to the <tt>tag_flag</tt>.
 * <br><br>
 * <b>Errors:</b> <tt>DS_API_NO_ATTRIB_DM2ACIS</tt> - The input <tt>ATTRIB_DM2ACIS </tt>pointer cannot be <tt>NULL</tt>.
 * <br><br>
 * <tt>DS_API_NOT_FACE_OR_EDGE</tt> -
 * The entity is not a face or an edge, and therefore is not sculptable.
 * <br><br>
 * <tt>DS_API_NOT_IN_HIERARCHY</tt> - The value of the target deformable model is not valid.
 * <br><br>
 * <tt>DS_API_BAD_PATCH_DOMAIN</tt> - The patch domain is not contained by a parent or it overlaps a sibling patch.
 * <br><br>
 * <b>Limitations:</b> Face patches must be completely contained within the interior of their parents;
 * the boundary of the child may not touch or overlap the boundary of its parent.
 * <br><br>
 * <b>Effect:</b> Changes model
 * <br><br>
* <b>Journal: </b> Not Available
* <br><br>
 * @param attrib_dm2acis
 * sculpting data attribute.
 * @param shape_flag
 * 1 = add rectangle, 2 = add ellipse, 3 = add fillet_square.
 * @param pt1
 * rect = min-corner, ellipse = center-pt.
 * @param pt2
 * rect = max-corner, ellipse = 1st-axis- end-pt.
 * @param pt3
 * rect = not-used, ellipse = 2nd-axis- end-pt.
 * @param refinement
 * parent_knot_spacing/ child_knot_spacing.
 * @param tgt_dmod
 * specify parent tgt_dmod to get new patch.
 * @param patch_tag
 * specify parent tgt_dmod to get new patch.
 * @param admO
 * adm options.
 * @param acisO
 * ACIS options such as version and journal.
 **/
DECL_ADM outcome                   // rtn: outcome 0 or error value
api_dm_add_patch                  // eff: add a patch to a parent
  (ATTRIB_DM2ACIS *attrib_dm2acis,// in : current sculpting data
   int shape_flag,                // in : patch_shape:1=rectangle,2=ellipse
                                  //      3=fillet_square
   SPApar_pos pt1,                   // in : rect=min_corner,ellipse=center_pt
   SPApar_pos pt2,                   // in : rect=max_corner,ellipse=1st_axis_end_pt
   SPApar_pos pt3,                   // in : rect=not_used,  ellipse=2nd_axis_end_pt
                                  //      filler_rad=pt3.u
   int refinement,                // in : parent_knot_spacing/child_knot_spacing
   int tgt_dmod,                  // in : specify which dmod recieves the patch
                                  //      1=active_dmod,2=root_dmod
                                  //      else=dmod with tag==tgt_dmod
   int &patch_tag,                // out: tag of newly added patch
   adm_options *admO=NULL, AcisOptions *acisO=NULL);
/**
 * Removes a patch from a deformable model's patch hierarchy.
 * <br><br>
 * <b>Role:</b> Removes and deletes a deformable model patch and its offspring
 * from the target's parent.
 * <br><br>
 * The target argument specifies which deformable model to use in a patch hierarchy.
 * <br><br>
 * <b>Errors:</b> <tt>DS_API_NOT_FACE_OR_EDGE</tt> - The entity is not a face or an edge, and therefore is not sculptable.
 * <br><br>
 * <tt>DS_API_NOT_IN_HIERARCHY</tt> - The value of the target deformable model is not valid.
 * <br><br>
 * <tt>DS_API_BAD_PATCH_DOMAIN</tt> - The patch domain is not contained by a parent or it overlaps a sibling patch.
 * <br><br>
 * <b>Limitations:</b> This function cannot be used to remove a root deformable model
 * shape. It works only on children within the patch hierarchy.
 * <br><br>
 * <b>Effect:</b> Changes model
 * <br><br>
* <b>Journal: </b> Not Available
* <br><br>
 * @param attrib_dm2acis
 * sculpting data attribute.
 * @param tgt_dmod
 * tag of deformable model to remove.
 * @param admO
 * adm options.
 * @param acisO
 * ACIS options such as version and journal.
 **/

DECL_ADM outcome                   // rtn: outcome 0 or error value
api_dm_rm_patch                   // eff: add a patch to a parent
  (ATTRIB_DM2ACIS *attrib_dm2acis,// in : current sculpting data
   int tgt_dmod,                  // in : specify which dmod recieves the patch
                                  //     else=dmod with tag==tgt_dmod
   adm_options *admO=NULL, AcisOptions *acisO=NULL);

/**
 * Adds a face to the deformable modeling multi-surface structure.
 * <br><br>
 * <b>Role:</b> Attaches attributes to the input face that includes it in the
 * deformable modeling set described by the <tt>ATTRIB_DM2ACIS</tt>.
 * <br><br>
 * This function will allow a set of faces to be connected together into a deformable
 * modeling hierarchy. Link loads between faces in the hierarchy can constrain position,
 * tangency, or curvature.
 * <br><br>
 * If the input face is an existing deformable model, all of its tag numbers will be changed,
 * ensuring uniqueness in the multi-surface deformable modeling hierarchy.
 * <br><br>
 * <b>Errors:</b> If the input face is an existing deformable model, all of its tag numbers, for
 * deformable models, loads, and constraints, will be changed to ensure
 * they are unique in the entire multi-surface deformable modeling hierarchy.
 * <br><br>
 * <tt>DS_API_NO_ATTRIB_DM2ACIS</tt> - The input <tt>ATTRIB_DM2ACIS</tt> pointer cannot be <tt>NULL</tt>.
 * <br><br>
 * <tt>DS_API_NOT_FACE_OR_EDGE</tt> - The entity is not a face or an edge, and therefore is not sculptable.
 * <br><br>
 * <tt>DS_NULL_INPUT_PTR</tt> - The face cannot be <tt>NULL</tt> on input.
 * <br><br>
 * <b>Limitations:</b> Does not support a multi-edge network.
 * <br><br>
 * <b>Effect:</b> Changes model
 * <br><br>
* <b>Journal: </b> Not Available
* <br><br>
 * @param attrib_dm2acis
 * sculpting data attribute.
 * @param entity
 * FACE or EDGE to connect.
 * @param entity_tag
 * tag number for dmod.
 * @param tag_shift
 * amount face's tag numbers have shifted new_tag_id = old_tag_id + tag_shift.
 * @param admO
 * adm options.
 * @param acisO
 * ACIS options such as version and journal.
 **/
DECL_ADM outcome                   // rtn: outcome 0 or error value
api_dm_add_multi_face             // eff: connect face to multisurf
  (ATTRIB_DM2ACIS *attrib_dm2acis,// in : the sculpting attribute
   ENTITY *entity,                // in : FACE or EDGE to connect
   int &entity_tag,               // out: tag number for the entity's dmod
   int &tag_shift,                // out: amount face's tag numbers have shifted - new_tag_id = old_tag_id + tag_shift
   adm_options *admO=NULL, AcisOptions *acisO=NULL);
/**
 * Separates a face from the deformable modeling multi-surface structure and marks it as a stand-alone deformable model. The face is not deleted.
 * <br><br>
 * <b>Role:</b> Removes the attributes from the target deformable model's <tt>FACE</tt> or <tt>EDGE</tt>
 * that mark it as a member of the multi-surface deformable modeling hierarchy
 * The target deformable model's <tt>FACE</tt> or <tt>EDGE</tt> is left with
 * attributes that mark it as its own standalone deformable modeling hierarchy
 * preserving all of its current tag information.
 * <br><br>
 * <b>Errors:</b> <tt>DS_API_NO_ATTRIB_DM2ACIS</tt> - The input <tt>ATTRIB_DM2ACIS</tt> pointer cannot be <tt>NULL</tt>
 * <br><br>
 * <tt>DS_API_NOT_IN_HIERARCHY</tt> - The input tag does not identify a deformable model within the <tt>attrib_dm2acis</tt>
 * hierarchy.
 * <br><br>
 * <tt>DS_API_NOT_A_ROOT_SIBLING</tt> - The input tag must be a sibling of the root deformable model
 * <br><br>
 * <b>Limitations:</b> This function requires (as a precondition) that only queries
 * have been called on the deformable modeling hierarchy since the last call to
 * <tt>api_dm_commit_attrib_dm2acis</tt>.
 * <br><br>
 * <b>Effect:</b> Changes model
 * <br><br>
 * @param attrib_dm2acis
 * sculpting data attribute.
 * @param tag
 * face to add to deformable set.
 * @param admO
 * adm options.
 * @param acisO
 * ACIS options such as version and journal.
 **/
DECL_ADM outcome                   // rtn: outcome 0 or error value
api_dm_rm_multi_face              // eff: disconnect face from multisurf
  (ATTRIB_DM2ACIS *attrib_dm2acis,// in : the sculpting attribute
   int tag,                       // in : TAG id for deformable
                                  //      model to disconnect from multi-surf sibling set.
   adm_options *admO=NULL, AcisOptions *acisO=NULL);
/**
 * @nodoc
 */
DECL_ADM outcome                   // rtn: outcome 0 or error value
api_dm_add_link                    // eff: disconnect face from multisurf
  ( ATTRIB_DM2ACIS *attrib_dm2acis,// in : the sculpting attribute
    const ENTITY *ent,             // in : edge for link
    int behavior,                  // in : requested behavior
    double gain,                   // in : requested gain
    int& tag,                      // out: tag of new link
    adm_options *admO=NULL, AcisOptions *acisO=NULL);

/**
 * Sets the deformable surface tolerance value to be that of the current ACIS tolerance value stored in <tt>SPAresabs</tt>.
 * <br><br>
 * <b>Role:</b> Sets the deformable surface tolerance values to be the same as
 * the current ACIS tolerance values. SDM uses two tolerance values: <tt>DS_tolerance</tt>
 * to determine when a distance is equivalent to zero and <tt>DS_angle_tol</tt> to determine
 * when an angle is equal to zero. This function sets:
 * <br><br>
 * <tt>DS_tolerance = SPAresabs<br>
 * DS_angle_tol = SPAresnor</tt>
 * <br><br>
 * <b>Effect:</b> Changes model
 * <br><br>
* <b>Journal: </b> Not Available
* <br><br>
 * @param admO
 * adm options.
 * @param acisO
 * ACIS options such as version and journal.
 **/
DECL_ADM outcome                   // rtn: outcome always API_SUCCESS
api_dm_set_tolerance              // eff: set DS_tolerance = SPAresabs
  (adm_options *admO=NULL, AcisOptions *acisO=NULL);
/**
 * Overrides the use of link loads in favor of link constraints in the current deformable modeling session.
 * <br><br>
 * <b>Role:</b> Sets the deformable modeling session to override the use of link
 * loads in favor of link constraints. This will return the user to pre-release 6.2
 * behavior. This function should only be called at the start of the deformable
 * modeling session. This API is for backward compatibility during the transition
 * from link constraints to link loads.
 * <br><br>
 * <b>Effect:</b> Changes model
 * <br><br>
* <b>Journal: </b> Not Available
* <br><br>
 * @param admO
 * adm options.
 * @param acisO
 * ACIS options such as version and journal.
 **/
DECL_ADM outcome                    // rtn: outcome always API_SUCCESS
api_dm_use_link_cstrns             // eff: override use of link loads; use link constraints
  (adm_options *admO=NULL, AcisOptions *acisO=NULL);
/**
 * Sets whether curves or surfaces with degree less than 3 are automatically elevated to degree 3 when brought into deformable modeling.
 * <br><br>
 * <b>Role:</b> By default, curves or surfaces with degree less than 3 are
 * automatically elevated to degree 3 when brought into deformable modeling. This
 * API allows a user to toggle this behavior. Calling <tt>api_dm_auto_elevate</tt> will turn
 * off auto-elevate for all curves and surfaces subsequently brought into
 * deformable modeling. Calling with a non-zero argument will turn on auto-elevate
 * for all curves or surfaces subsequently brought into deformable modeling.
 * <br><br>
 * <b>Effect:</b> Changes model
 * <br><br>
* <b>Journal: </b> Not Available
* <br><br>
 * @param y_n
 * autoelevate key.
 * @param admO
 * adm options.
 * @param acisO
 * ACIS options such as version and journal.
 **/
DECL_ADM outcome                   // rtn: outcome always API_SUCCESS
api_dm_auto_elevate                // eff: do or do NOT autoelevate
  (int y_n ,                       //  in: 0==> no, non-zero==> yes
   adm_options *admO=NULL, AcisOptions *acisO=NULL);
/**
 * Sizes the <tt>DM_dbl_array</tt> and initializes the memory.
 * <br><br>
 * <b>Role:</b> The <tt>DM_dbl_array</tt> is a double array container class returned from
 * deformable modeling queries. The <tt>DM_dbl_array</tt> can
 * also serve as a user container class by sizing with the <tt>api_dm_set_array_size</tt>
 * method.
 * <br><br>
 * <b>Effect:</b> System routine
 * <br><br>
* <b>Journal: </b> Not Available
* <br><br>
 * @param arr
 * array.
 * @param new_size
 * array size.
 * @param init_val
 * value stored in returned memory.
 * @param admO
 * adm options.
 * @param acisO
 * ACIS options such as version and journal.
 **/
DECL_ADM outcome                  // rtn: outcome 0 or error value
api_dm_set_array_size             // eff: set a DM_dbl_array size & initialize memory
  (DM_dbl_array& arr,             // in : the array
   int new_size,                  // in : array size
   double init_val=0.0,           // in : val stored in returned memory
   adm_options *admO=NULL, AcisOptions *acisO=NULL);
/**
 * Enables deformable modeling API level journaling and opens a journal file for write.
 * <br><br>
 * <b>Role:</b> A call to this function begins a deformable modeling API journal session. The
 * call opens for write the file indicated by the input filename and begins journaling all future
 * deformable modeling API calls. Deformable modeling API call journaling is a
 * debugging tool for deformable modeling customers who do not run ACIS or Scheme
 * AIDE. When <tt>DM_journal</tt> is set to 1, every call to a deformable modeling API
 * routine generates an input report listing all the input variable values when the
 * routine was entered. The deformable modeling API routines also generate an
 * output report listing all the output variable values just before the routine
 * exits.
 * <br><br>
 * The cascade argument controls how much information gets written to the journal
 * file. When the value of cascade is:
 * <br><br>
 * 0 = all entry level deformable modeling calls are journaled.
 * <br>
 * 1 = all entry level and cascaded deformable modeling calls are journaled.
 * <br>
 * 2 = all entry level and intersection callback functions are journaled.
 * <br>
 * 3 = all entry level deformable modeling calls, cascaded deformable
 * modeling calls, and intersection callback functions are journaled.
 * <br><br>
 * When this function is called with journaling enabled, it closes the current
 * journal file and opens a new one. Opening a journal file with the same name as
 * the current journal file will cause an overwrite of the old journal file data.
 * <br><br>
 * <b>Errors:</b> <tt>DM_NULL_INPUT_PTR</tt> - The filename is <tt>NULL</tt> on input
 * <br><br>
 * <tt>DM_BAD_CASCADE_VALUE</tt> - The value of cascade must be 0 to 3
 * <br><br>
 * <tt>DM_FAILED_FILE_OPEN_WRITE</tt> - The designated file is not opened for write
 * <br><br>
 * <b>Effect:</b> System routine
 * <br><br>
* <b>Journal: </b> Not Available
* <br><br>
 * @param filename
 * file to open for write.
 * @param cascade
 * 1=log nested API calls 0=log only entry API calls.
 * @param admO
 * adm options.
 * @param acisO
 * ACIS options such as version and journal.
 **/

DECL_ADM outcome                   // rtn: outcome 0 or error value
api_dm_journal_on                 // eff: turn on DM API journaling
  (char *filename,                // in : file to open for write
   int cascade=0,                 // in : 1=log nested DM API calls
                                  //      0=log only entry DM API calls
   adm_options *admO=NULL, AcisOptions *acisO=NULL);

/**
 * @nodoc
 */
DECL_ADM outcome                   // rtn: outcome 0 or error value
api_dm_journal_play               // eff: turn on DM API journaling
  (char *filename,                // in : file to open for read
   FACE *&face,                   // out: A face to contain the first
                                  //      deformable surface made by the journal file.
   adm_options *admO=NULL, AcisOptions *acisO=NULL);
/**
 * @nodoc
 */
DECL_ADM outcome                   // rtn: outcome 0 or error value
api_dm_journal_play               // eff: turn on DM API journaling
  (char *filename,                // in : file to open for read
   ENTITY_LIST& list,             // out: A list of faces containing the deformable surface made by
                                  //      the journal file.
   adm_options *admO=NULL, AcisOptions *acisO=NULL);
/**
 * Turns off deformable modeling API level journaling and closes any journal file opened for write by <tt>api_dm_journal_on</tt>.
 * <br><br>
 * <b>Role:</b> This API closes any file opened by a <tt>api_dm_journal_on</tt> call and
 * turns off deformable modeling API level journaling. Deformable modeling API
 * journaling is not a modeling tool; it is a debugging tool developed for
 * customers running the deformable modeling API without ACIS or Scheme AIDE.
 * <br><br>
 * <b>Effect:</b> System routine
 * <br><br>
* <b>Journal: </b> Not Available
* <br><br>
 * @param admO
 * adm options.
 * @param acisO
 * ACIS options such as version and journal.
 **/
DECL_ADM outcome                   // rtn: outcome always API_SUCCESS
api_dm_journal_off                // eff: turn off DM API journaling
  (adm_options *admO=NULL, AcisOptions *acisO=NULL);

/**
 * This query returns the <tt>ENTITY</tt>s associated with all the <tt>DS_dmod</tt>'s in the deformable modeling hierarchy.
 * <br><br>
 * <b>Role:</b> In ACIS deformable modeling, every deformable model corresponds
 * to one or more ACIS <tt>ENTITIES</tt>, either <tt>FACE</tt>s or <tt>EDGE</tt>s. This query returns the
 * <tt>ENTITY</tt>s associated with all the <tt>DS_dmod</tt>'s in the deformable modeling hierarchy.
 * <br><br>
 * <b>Effect:</b> System routine
 * <br><br>
* <b>Journal: </b> Not Available
* <br><br>
 * @param attrib_dm2acis
 * in: ADM  attribute.
 * @param elist
 * out: entity list.
 * @param admO
 * adm options.
 * @param acisO
 * ACIS options such as version and journal.
 **/
DECL_ADM outcome                 // rtn: outcome 0 or error value
api_dm_get_hierarchy_entities    // eff: get ACIS entities associated with DS_dmods in the DM hierarchy
  (ATTRIB_DM2ACIS* attrib_dm2acis,              //  in: ADM attribute
    ENTITY_LIST& elist,          // out: ENTITY_LIST
    adm_options *admO=NULL, AcisOptions *acisO=NULL);


/**
 * @nodoc
 */
DECL_ADM outcome                        // rtn: outcome 0 or error value
adm_disable_legacy_domain_scaling    // eff: disable legacy domain scaling behavior
  (adm_options *admO=NULL, AcisOptions *acisO=NULL);

 /** @} */
#endif // DS_DSAPI_H

