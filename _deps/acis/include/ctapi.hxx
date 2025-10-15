/* ORIGINAL: acis2.1/ct_husk/api/ctapi.hxx */
/*******************************************************************/
/*    Copyright (c) 1989-2020 by Spatial Corp.                     */
/*    All rights reserved.                                         */
/*    Protected by U.S. Patents 5,257,205; 5,351,196; 6,369,815;   */
/*                              5,982,378; 6,462,738; 6,941,251    */
/*    Protected by European Patents 0503642; 69220263.3            */
/*    Protected by Hong Kong Patent 1008101A                       */
/*******************************************************************/

/*******************************************************************/
// dal 31Mar03 Documentation.
// let 12Jan12 Added api_ct_merge_cells().
/*******************************************************************/

// Header for ct_husk api routines.

// This header is included in api_routines.hxx

#if !defined( CT_HUSK_API_ROUTINES )
#define CT_HUSK_API_ROUTINES

#include "base.hxx"

// Forward declarations
class BODY;
class LUMP;
class WIRE;
class SHELL;
class FACE;
class LOOP;
class COEDGE;
class EDGE;
class VERTEX;
class APOINT;
class ENTITY_LIST;
class ENTITY;
class CURVE;

class SPAGROUP;
class CELL;
class CELL3D;
class CELL2D;
class CFACE;
class CSHELL;

class outcome;

class SPAposition;
class SPAunit_vector;
class tensor;
class insanity_list;
class ct_merge_options;

#include "dcl_ct.h"
#include "api.hxx"
#include "pattern_enum.hxx"
#include "ptcont.hxx"

/**
 * \defgroup CELLTOPAPI Cellular Topology
 *
 */

/**
* @file ctapi.hxx
 * @CAA2Level L1
 * @CAA2Usage U1
 */

/*! \addtogroup CELLTOPAPI
 *  \brief Declared at <ctapi.hxx>, SPAct
 *  @{
 */

/**
 * Initializes the cellular topology library.
 * <br><br>
 * <b>Effect:</b> System routine
 * <br><br>
 * <b>Journal: </b> Not Available
 * <br><br>
 * <b>Product(s):</b> 3D ACIS Modeler
 * <br><br>
**/
DECL_CT outcome api_initialize_cellular_topology();
/**
 * Terminates the cellular topology library.
 * <br><br>
 * <b>Effect:</b> System routine
 * <br><br>
 * <b>Journal: </b> Not Available
 * <br><br>
 * <b>Product(s):</b> 3D ACIS Modeler
 * <br><br>
**/
DECL_CT outcome api_terminate_cellular_topology();
/**
 * Adds one entity to an existing group.
 * <br><br>
 * <b>Role:</b> If the given entity does not already belong to the group, this
 * API adds it to the group
 * <br><br>
 * <b>Errors:</b> The pointer to an entity is <tt>NULL</tt>.
 * The pointer to a group is <tt>NULL</tt> or does not point to a <tt>SPAGROUP</tt>.
 * <br><br>
 * <b>Effect:</b> Changes model
 * <br><br>
 * <b>Journal: </b> Not Available
 * <br><br>
 * <b>Product(s):</b> 3D ACIS Modeler
 * <br><br>
 * @param ent
 * entity to add.
 * @param group
 * group to get the entity.
 * @param ao
 * acis options.
 **/
DECL_CT outcome api_ct_add_to_group(
			ENTITY* ent,						//	entity to add
			SPAGROUP* group,					//	group to add it to
			AcisOptions *ao = NULL);
/**
 * Attaches cell data to a lump.
 * <br><br>
 * <b>Role:</b> This API computes and attaches cell data using <tt>ATTRIB_CELL</tt> to a
 * lump that does not have the data. If the lump already has the cell data
 * attribute, perform a cell update computation instead.
 * <br><br>
 * <b>Errors:</b> The pointer to a lump is <tt>NULL</tt> or does not point to a <tt>LUMP</tt>.
 * One or more of the lump's single-sided faces has a coedge without a partner; that is,
 * the given lump contains an open shell. 
 * <br><br>
 * <b>Effect:</b> Changes model
 * <br><br>
 * <b>Journal: </b> Not Available
 * <br><br>
 * <b>Product(s):</b> 3D ACIS Modeler
 * <br><br>
 * @param lump
 * lump to accept cell data.
 * @param ao
 * acis options.
 **/
DECL_CT outcome api_ct_attach_cells(
			LUMP* lump,							//	lump to attach cells to
			AcisOptions *ao = NULL);
/**
 * Computes the area of a cell.
 * <br><br>
 * <b>Role:</b> This API computes the cell's external area. The area of a <tt>CELL2D</tt> 
 * comprises both sides of all its faces. The area of a <tt>CELL3D</tt> does not include 
 * any faces used twice. 
 * <br><br>
 * <b>Errors:</b> The pointer to a cell is <tt>NULL</tt> or does not point to a <tt>CELL</tt>.
 * Negative accuracy is requested.
 * <br><br>
 * <b>Effect:</b> Read-only
 * <br><br>
 * <b>Journal: </b> Not Available
 * <br><br>
 * <b>Product(s):</b> 3D ACIS Modeler
 * <br><br>
 * @param cell
 * cell to be examined.
 * @param req_rel_accy
 * requested relative accuracy.
 * @param area
 * returned cell area computed.
 * @param est_rel_accy_achieved
 * returned estimate of relative accuracy achieved.
 * @param ao
 * acis options.
 **/
DECL_CT outcome api_ct_cell_area(
			CELL* cell,							//	cell to compute area of
			double req_rel_accy,				//  requested accuracy
			double& area,						//	computed area
			double& est_rel_accy_achieved,		//	estimated accuracy achieved
			AcisOptions *ao = NULL);

/**
 * Computes mass properties of a <tt>CELL3D</tt>.
 * <br><br>
 * <b>Role:</b> This API determines the volume, center of gravity and moments of
 * inertia of a 3D cell.
 * <br><br>
 * The major diagonal of the inertia tensor returned contains values for the inertias
 * about each axis. For the inertia about the <i>x</i>-axis <tt>(integral)((y*y + z*z)dV))</tt>; the
 * off-diagonal terms are for the products of inertia, e.g., <tt>((integral)(x*y)dV)</tt>.
 * <br><br>
 * Specify the projection plane by a point and normal (in global body space). For speed,
 * choose the plane so that as many faces as possible project on to it as lines; i.e.,
 * are edge-on to it. To improve the accuracy of the result, set the plane to pass
 * through a point within the cell; generally, the mid-point of its box.
 * <br><br>
 * The selector controls the properties computed. A value of zero calculates all
 * properties; 1 prevents the calculation of inertias; 2 prevents the calculation of inertias,
 * first moments, or center of gravity (only the volume is calculated).
 * <br><br>
 * Set the requested accuracy to the desired relative accuracy. For example, setting the
 * requested accuracy to 0.01 requests an accuracy of 1 percent.
 * <br><br>
 * Although computation of mass properties does not make substantive changes to a model,
 * boxes may be calculated and cached in the model, creating a bulletin board. To make the
 * process read-only for the user, wrap the call to <tt>api_ct_cell_mass_pr</tt> in an 
 * API_NOP_BEGIN / API_NOP_END block, for example:
 * <pre>
 * outcome out(0);
 * API_NOP_BEGIN
 * out = api_ct_cell_mass_pr(...)
 * API_NOP_END</pre>
 * <br><br>
 * See the Scheme command, <tt>cell:massprop</tt>, for an example of how to call this function.
 * <br><br>
 * <b>Errors:</b> Pointer to cell <tt>NULL</tt> or not to a <tt>CELL3D</tt>.
 * Zero length normal vector specified.
 * Negative accuracy requested.
 * Invalid value specified for selector.
 * <br><br>
 * <b>Effect:</b> Changes model
 * <br><br>
 * <b>Journal: </b> Not Available
 * <br><br>
 * <b>Product(s):</b> 3D ACIS Modeler
 * <br><br>
 * @param cell
 * cell to be examined.
 * @param root_proj_pl
 * projection plane root.
 * @param normal_proj_pl
 * projection plane normal.
 * @param selector
 * properties to compute.
 * @param req_rel_accy
 * requested relative accuracy.
 * @param volume
 * returned volume of cell.
 * @param cofg
 * returned center of gravity.
 * @param inertia
 * returned tensor (3*3) matrix.
 * @param p_moments
 * returned principal moments of inertia. An array of size 3.
 * @param p_axes
 * returned principal axes of inertia. An array of size 3.
 * @param est_rel_accy_achieved
 * returned estimate of relative accuracy achieved.
 * @param ao
 * acis options.
 **/
DECL_CT outcome api_ct_cell_mass_pr(
			CELL3D* cell,							//	3D cell to analyze
			SPAposition const& root_proj_pl,		//	proj plane root
			SPAunit_vector const& normal_proj_pl,	//	proj plane normal
			int selector,							//	selector
			double req_rel_accy,					//	requested accuracy
			double& volume,							//	volume
			SPAposition& cofg,						//	cofg
			tensor& inertia,						//	inertia tensor
			double p_moments[3],					//	principal moments
			SPAunit_vector* p_axes,					//	principal moment axes
			double&	est_rel_accy_achieved,			//  estimated accuracy achieved
            AcisOptions * ao = NULL);
/**
 * Copies a cell as a body.
 * <br><br>
 * <b>Role:</b> This API copies a cell to a single, separate body. The original
 * body is not modified. The new body is a valid body in and of itself. The cell
 * may be either 2D or 3D. Cell data is automatically attached to the copy body.
 * <br><br>
 * <b>Errors:</b> Pointer to cell is <tt>NULL</tt> or not to a <tt>CELL</tt>.
 * <br><br>
 * <b>Effect:</b> Changes model
 * <br><br>
 * <b>Journal: </b> Not Available
 * <br><br>
 * <b>Product(s):</b> 3D ACIS Modeler
 * <br><br>
 * @param cell
 * cell to be copied as a body.
 * @param body
 * resulting body.
 * @param ao
 * acis options.
 **/
DECL_CT outcome api_ct_copy_cell(
			CELL * cell,						//	cell to copy
			BODY*&	body,						//	body pointer of copy
			AcisOptions *ao = NULL);
/**
 * Transforms the cell lists of the given lump into a hierarchy of <tt>SUPERCELL</tt>s with spatial locality.
 * <br><br>
 * <b>Role:</b> This API expands a lump's cells into a hierarchy of <tt>SUPERCELL</tt>s
 * with spatial locality. A successful <tt>outcome</tt> indicates the process ran to
 * completion, even if nothing has been changed.
 * <br><br>
 * <b>Errors:</b> Pointer to lump is <tt>NULL</tt> or not to a <tt>LUMP</tt>.
 * <br><br>
 * <b>Limitations:</b> May have no effect on lumps with few cells.
 * <br><br>
 * <b>Effect:</b> Changes model
 * <br><br>
 * <b>Journal: </b> Not Available
 * <br><br>
 * <b>Product(s):</b> 3D ACIS Modeler
 * <br><br>
 * @param lump
 * lump to be subdivided.
 * @param ao
 * acis options.
 **/
DECL_CT outcome api_ct_expand_cells(
			LUMP *lump,							//	lump on which to expand cells
			AcisOptions *ao = NULL);
/**
 * Flattens the given lump's supercell structure.
 * <br><br>
 * <b>Role:</b> This API removes any supercell structure from the cells of the
 * given lump and places the cells in a single list for the lump. No action if no
 * <tt>SUPERCELL</tt>s exist. A successful <tt>outcome</tt> indicates the process 
 * ran to completion, even if nothing has been changed.
 * <br><br>
 * <b>Errors:</b> Pointer to lump is <tt>NULL</tt> or not to a <tt>LUMP</tt>.
 * <br><br>
 * <b>Effect:</b> Changes model
 * <br><br>
 * <b>Journal: </b> Not Available
 * <br><br>
 * <b>Product(s):</b> 3D ACIS Modeler
 * <br><br>
 * @param lump
 * lump to be flattened.
 * @param ao
 * acis options.
 **/
DECL_CT outcome api_ct_flatten_cells(
			LUMP *lump,							//	lump on which to flatten cells
			AcisOptions *ao = NULL);
/**
 * Removes the cell data from a lump.
 * <br><br>
 * <b>Role:</b> This API removes all cell data from a lump. There must be an
 * <tt>ATTRIB_CELL</tt> attached to the lump.
 * <br><br>
 * <b>Errors:</b> The pointer to lump is <tt>NULL</tt> or does not point to a <tt>LUMP</tt>.
 * <br><br>
 * <b>Effect:</b> Changes model
 * <br><br>
 * <b>Journal: </b> Not Available
 * <br><br>
 * <b>Product(s):</b> 3D ACIS Modeler
 * <br><br>
 * @param lump
 * lump to be cleaned of cells.
 * @param ao
 * acis options.
 **/
DECL_CT outcome api_ct_lose_cells(
			LUMP* lump,							//	lump to remove cells from
			AcisOptions *ao = NULL);
/**
 * Un-group a group of entities and delete the group.
 * <br><br>
 * <b>Role:</b> This API removes all entities from the group and then delete the group entity itself.
 * <br><br>
 * <b>Errors:</b> The pointer to a group is <tt>NULL</tt> or does not point to a <tt>SPAGROUP</tt>.
 * <br><br>
 * <b>Effect:</b> Changes model
 * <br><br>
 * <b>Journal: </b> Not Available
 * <br><br>
 * <b>Product(s):</b> 3D ACIS Modeler
 * <br><br>
 * @param group
 * group to be removed.
 * @param ao
 * acis options.
 **/
DECL_CT outcome api_ct_lose_group(
			SPAGROUP* group,							//	group to remove
			AcisOptions *ao = NULL);
/**
 * Creates a group from a list of entities.
 * <br><br>
 * <b>Role:</b> This API combines a set of entities by creating a group entity
 * that points to each entity in the group. 
 * <br><br>
 * <b>Effect:</b> Changes model
 * <br><br>
 * <b>Journal: </b> Not Available
 * <br><br>
 * <b>Product(s):</b> 3D ACIS Modeler
 * <br><br>
 * @param list
 * list of entities to group.
 * @param group
 * returned resulting group.
 * @param ao
 * acis options.
 **/
DECL_CT outcome api_ct_make_group(
			ENTITY_LIST& list,					//	list of entities to put in group
			SPAGROUP*& group,					//	new group pointer
			AcisOptions *ao = NULL);
/**
 * Determines whether a given point lies inside, outside, or on the boundary of a 3D cell.
 * <br><br>
 * <b>Role:</b> This API determines the location of a point (given in global
 * coordinate space) for a given cell. Returns a <tt>point_containment</tt> value of
 * <tt>point_inside</tt>, <tt>point_outside</tt>, <tt>point_boundary</tt>, or <tt>point_unknown</tt>.
 * <br><br>
 * A call to this API may cause boxes to be computed and hence the model will
 * change and a bulletin board will be made.
 * <br><br>
 * Although containment calculations do not make substantive changes to a model, bounding
 * boxes may be calculated and cached in the model, creating a bulletin board. To make the
 * process read-only for the user, wrap the call to <tt>api_ct_point_in_cell</tt> in an 
 * API_NOP_BEGIN / API_NOP_END block, for example:
 * <pre>
 * outcome out(0);
 * API_NOP_BEGIN
 * out = api_ct_point_in_cell(...)
 * API_NOP_END</pre>
 * <br><br>
 * <b>Errors:</b> Pointer to cell is <tt>NULL</tt> or not to a <tt>CELL3D</tt>.
 * <br><br>
 * <b>Effect:</b> Changes model
 * <br><br>
 * <b>Journal: </b> Available
 * <br><br>
 * <b>Product(s):</b> 3D ACIS Modeler
 * <br><br>
 * @param test_point
 * point to be tested.
 * @param target_cell
 * cell to be examined.
 * @param pc
 * returned inside, outside, boundary, unknown.
 * @param ao
 * acis options.
 **/
DECL_CT outcome api_ct_point_in_cell(
			SPAposition const& test_point,			//	test point
			CELL3D* target_cell,					//	target cell
			point_containment& pc,					//	containment result
			AcisOptions *ao = NULL);
/**
 * Propagates the cface volume attributes (<tt>ATTRIB_CFACE_VOL</tt>) on a lump.
 * <br><br>
 * <b>Role:</b> This API propagates any attributes derived from
 * <tt>ATTRIB_CFACE_VOL</tt> on the cell data on the lump. Such attributes are intended to
 * represent cell volume data, such as material type, and must be propagated after
 * any change to the model (such as a Boolean operation) to reflect volume containment and
 * merging.
 * <br><br>
 * The algorithm follows:
 * <br><br>
 * First, copy each attribute descended from <tt>ATTRIB_CFACE_VOL</tt> found on a <tt>cface</tt> to
 * all other <tt>cface</tt>s on the <tt>CELL3D</tt>, unless other <tt>cface</tt>s already have that attribute.
 * <br><br>
 * Then, if the <tt>cface</tt> has an attribute copied to it, it is also copied to the
 * partner (opposing) <tt>cface</tt>, if any.
 * <br><br>
 * After propagation, the cell data can be scanned for attribute conflicts, such as
 * two different material types on one cell.
 * <br><br>
 * <b>Errors:</b> Pointer to lump is <tt>NULL</tt> or not to a <tt>LUMP</tt>.
 * No cell data attached to lump.
 * <br><br>
 * <b>Effect:</b> Changes model
 * <br><br>
 * <b>Journal: </b> Available
 * <br><br>
 * <b>Product(s):</b> 3D ACIS Modeler
 * <br><br>
 * @param lump
 * lump of interest.
 * @param ao
 * acis options.
 **/
DECL_CT outcome api_ct_propagate_cface_attribs(
			LUMP*lump,							//	lump on which to propagate atts
			AcisOptions *ao = NULL);
/**
 * Removes an entity from a group.
 * <br><br>
 * <b>Role:</b> This API removes the entity from the group's list of entities,
 * If the entity is not in the group, it does nothing.
 * <br><br>
 * <b>Errors:</b> Pointer to entity is <tt>NULL</tt>.
 * Pointer to group is <tt>NULL</tt> or not to a <tt>SPAGROUP</tt>.
 * <br><br>
 * <b>Effect:</b> Changes model
 * <br><br>
 * <b>Journal: </b> Not Available
 * <br><br>
 * <b>Product(s):</b> 3D ACIS Modeler
 * <br><br>
 * @param ent
 * entity to be removed.
 * @param group
 * group containing entity.
 * @param ao
 * acis options.
 **/
DECL_CT outcome api_ct_remove_from_group(
			ENTITY* ent,						//	entity to remove from group
			SPAGROUP* group,					//	group from which to remove
			AcisOptions *ao = NULL);
/**
 * Gets a list of all entities in a group.
 * <br><br>
 * <b>Errors:</b> Pointer to group is <tt>NULL</tt> or not to a <tt>SPAGROUP</tt>.
 * <br><br>
 * <b>Effect:</b> Read-only
 * <br><br>
 * <b>Journal: </b> Not Available
 * <br><br>
 * <b>Product(s):</b> 3D ACIS Modeler
 * <br><br>
 * @param group
 * group to be examined.
 * @param list
 * returned entities in the group.
 * @param ao
 * acis options.
 **/
DECL_CT outcome api_ct_return_ents(
			SPAGROUP * group,						//	group from which to return
			ENTITY_LIST& list,						//	list from group
			AcisOptions *ao = NULL);
/**
 * Gets a list of all groups in which an entity participates.
 * <br><br>
 * <b>Role:</b> This API retrieves a list of all groups that an entity is in.
 * <br><br>
 * <b>Errors:</b> <tt>NULL</tt> entity.
 * <br><br>
 * <b>Effect:</b> Read-only
 * <br><br>
 * <b>Journal: </b> Not Available
 * <br><br>
 * <b>Product(s):</b> 3D ACIS Modeler
 * <br><br>
 * @param ent
 * entity of interest.
 * @param list
 * returned groups containing the entity.
 * @param ao
 * acis options.
 **/
DECL_CT outcome api_ct_return_groups(
			ENTITY *ent,						//	entity from which to return
			ENTITY_LIST& list,					//	list of groups
			AcisOptions *ao = NULL);
/**
 * Modifies a 3D cell from filled to void.
 * <br><br>
 * <b>Role:</b> This API modifies the <i>sidedness</i> and <i>containment</i> of all the
 * cell's faces so that the cell is now void. Does not change the cell data, except
 * to invalidate affected cells. The cells are updated during the cell recompute at
 * the end of the API.
 * <br><br>
 * <b>Errors:</b> Pointer to cell is <tt>NULL</tt> or not to a <tt>CELL3D</tt>.
 * <br><br>
 * <b>Effect:</b> Changes model
 * <br><br>
 * <b>Journal: </b> Not Available
 * <br><br>
 * <b>Product(s):</b> 3D ACIS Modeler
 * <br><br>
 * @param cell
 * 3D cell to be vacated.
 * @param ao
 * acis options.
 **/
DECL_CT outcome api_ct_vacate_cell(
			CELL3D* cell,							//	3D cell to make into void
			AcisOptions *ao = NULL);

// --------------------------------------------------------------------
//  Purpose---
//    Gets all CELLs attached to each LUMP within the given list of
//    BODYs and adds them to an entity list.  This procedure assumes
//    that the elements of the given ENTITY_LIST are of type BODY.
// --------------------------------------------------------------------
/**
 * Gets all cells attached to each <tt>LUMP</tt> within the given list of <tt>BODY</tt>s and adds them to an entity list.
 * <br><br>
 * <b>Role:</b> This extension gets all <tt>CELL</tt>s attached to each <tt>LUMP</tt> within the
 * given list of <tt>BODY</tt>s and adds them to an entity list. This procedure assumes that
 * the elements of the given <tt>ENTITY_LIST</tt> are of type <tt>BODY</tt>.
 * <br><br>
 * <b>Effect:</b> Changes model
 * <br><br>
 * <b>Journal: </b> Not Available
 * <br><br>
 * <b>Product(s):</b> 3D ACIS Modeler
 * <br><br>
 * @param body_list
 * body list.
 * @param cell_list
 * cell list.
 * @param ao
 * acis options.
 **/
DECL_CT outcome api_ct_get_all_cells(
	ENTITY_LIST const &body_list,
	ENTITY_LIST  &cell_list,
	AcisOptions * ao = NULL);
/**
 * Gets all the faces that a given entity contains and lists them in an <tt>ENTITY_LIST</tt>.
 * <br><br>
 * <b>Role:</b> Gets all the faces of a given entity. The given entity must be either a cell
 * or a standard topological entity. By default, patterned objects are included in the list of
 * entities. In general, however, the parameter <tt>include_pat</tt> determines how this
 * function deals with such objects. The following are valid values for this
 * argument:
 * <br><br>
 * <dl>
 * <dt><tt>PAT_CAN_CREATE</tt></dt><dd>patterned objects are created if they do not already exist, and
 * are included in the list.</dd>
 * <dt><tt>PAT_NO_CREATE</tt></dt><dd>only those patterned objects that have already been
 * created are included in the list.</dd>
 * <dt><tt>PAT_IGNORE</tt></dt><dd>no patterned objects besides seed pattern
 * objects are included in the list.</dd>
 * </dl><br>
 * <b>Effect:</b> Read-only
 * <br><br>
 * <b>Journal: </b> Available
 * <br><br>
 * <b>Product(s):</b> 3D ACIS Modeler
 * <br><br>
 * @param ent
 * entity to query.
 * @param face_list
 * list of faces.
 * @param include_pat
 * how to treat patterned faces.
 * @param ao
 * acis options.
 **/
DECL_CT outcome api_get_faces_from_all_entities(
    ENTITY* ent,
    ENTITY_LIST& face_list,
    PAT_NEXT_TYPE include_pat = PAT_CAN_CREATE,
	AcisOptions *ao = NULL);
/**
 * Gets all the edges that a given entity contains and lists them in an ENTITY_LIST.
 * <br><br>
 * <b>Role:</b> Gets all the edges of a given entity. The given entity must be either a cell
 * or a standard topological entity. By default, patterned objects are included in the list of
 * entities. In general, however, the parameter <tt>include_pat</tt> determines how this
 * function deals with such objects. The following are valid values for this
 * argument:
 * <br><br>
 * <dl>
 * <dt><tt>PAT_CAN_CREATE</tt></dt><dd>patterned objects are created if they do not already exist, and
 * are included in the list.</dd>
 * <dt><tt>PAT_NO_CREATE</tt></dt><dd>only those patterned objects that have already been
 * created are included in the list.</dd>
 * <dt><tt>PAT_IGNORE</tt></dt><dd>no patterned objects besides seed pattern
 * objects are included in the list.</dd>
 * </dl><br>
 * <b>Effect:</b> Read-only
 * <br><br>
 * <b>Journal: </b> Available
 * <br><br>
 * <b>Product(s):</b> 3D ACIS Modeler
 * <br><br>
 * @param ent
 * entity to query.
 * @param edge_list
 * list of edges.
 * @param include_pat
 * how to treat patterned edges.
 * @param ao
 * acis options.
 **/
DECL_CT outcome api_get_edges_from_all_entities(
    ENTITY* ent,
    ENTITY_LIST& edge_list,
    PAT_NEXT_TYPE include_pat = PAT_CAN_CREATE,
	AcisOptions *ao = NULL);
/**
 * Gets all the vertices that a given entity contains and lists them in an <tt>ENTITY_LIST</tt>.
 * <br><br>
 * <b>Role:</b> Gets all the vertices of a given entity. The given entity must be either a cell
 * or a standard topological entity. By default, patterned objects are included in the list of
 * entities. In general, however, the parameter <tt>include_pat</tt> determines how this
 * function deals with such objects. The following are valid values for this
 * argument:
 * <br><br>
 * <dl>
 * <dt><tt>PAT_CAN_CREATE</tt></dt><dd>patterned objects are created if they do not already exist, and
 * are included in the list.</dd>
 * <dt><tt>PAT_NO_CREATE</tt></dt><dd>only those patterned objects that have already been
 * created are included in the list.</dd>
 * <dt><tt>PAT_IGNORE</tt></dt><dd>no patterned objects besides seed pattern
 * objects are included in the list.</dd>
 * </dl><br>
 * <b>Effect:</b> Read-only
 * <br><br>
 * <b>Journal: </b> Available
 * <br><br>
 * <b>Product(s):</b> 3D ACIS Modeler
 * <br><br>
 * @param ent
 * entity to query.
 * @param vertex_list
 * list of vertices.
 * @param include_pat
 * how to treat patterned vertices.
 * @param ao
 * acis options.
 **/
DECL_CT outcome api_get_vertices_from_all_entities(
    ENTITY* ent,
    ENTITY_LIST& vertex_list,
    PAT_NEXT_TYPE include_pat = PAT_CAN_CREATE,
	AcisOptions *ao = NULL);
/**
 * Attaches cellular topology to each lump within each body in the input list.
 * <br><br>
 * <b>Role:</b> This API attaches cellular topology to each <tt>LUMP</tt> within each
 * <tt>BODY</tt> in the input list.
 * <br><br>
 * <b>Effect:</b> Changes model
 * <br><br>
 * <b>Journal: </b> Available
 * <br><br>
 * <b>Product(s):</b> 3D ACIS Modeler
 * <br><br>
 * @param body_list
 * list of bodies.
 * @param ao
 * acis options.
 **/
DECL_CT outcome  api_ct_attach( ENTITY_LIST  &body_list,
							   AcisOptions *ao = NULL);
/**
 * Expands the cellular topology by grouping cells within each body in the given list into supercells.
 * <br><br>
 * <b>Role:</b> If cellular topology is attached to a body in the input  <tt>body_list</tt>,
 * this API expands the cellular topology by grouping the <tt>CELL</tt>s into
 * <tt>SUPERCELL</tt>s. This may increase the speed and efficiency
 * of other cellular topology operations.
 * <br><br>
 * This API does not try to expand the cell list into supercells unless 50 or more
 * cells exist.
 * <br><br>
 * <b>Effect:</b> Changes model
 * <br><br>
 * <b>Journal: </b> Not Available
 * <br><br>
 * <b>Product(s):</b> 3D ACIS Modeler
 * <br><br>
 * @param body_list
 * list of bodies.
 * @param ao
 * acis options.
 **/
DECL_CT outcome  api_ct_expand( ENTITY_LIST  &body_list,
							   AcisOptions *ao = NULL);
/**
 * Removes any <tt>SUPERCELL</tt>s within each body in the given list.
 * <br><br>
 * <b>Role:</b> This API removes any <tt>SUPERCELL</tt>s within each body in the given
 * list. If cellular topology is attached to a body in the input <tt>body_list</tt>, this
 * API checks for and removes any <tt>SUPERCELL</tt>s.
 * <br><br>
 * <b>Effect:</b> Changes model
 * <br><br>
 * <b>Journal: </b> Not Available
 * <br><br>
 * <b>Product(s):</b> 3D ACIS Modeler
 * <br><br>
 * @param body_list
 * list of bodies.
 * @param ao
 * acis options.
 **/
DECL_CT outcome  api_ct_flatten( ENTITY_LIST  &body_list,
								AcisOptions *ao = NULL);
/**
 * Removes cellular topology from each lump within each body in the input list.
 * <br><br>
 * <b>Role:</b> This API removes the cellular topology from each <tt>LUMP</tt>
 * within each <tt>BODY</tt> in the <tt>body_list</tt>. If the given <tt>BODY</tt> 
 * has no cellular topology, this API has no effect.
 * <br><br>
 * <b>Effect:</b> Changes model
 * <br><br>
 * <b>Journal: </b> Not Available
 * <br><br>
 * <b>Product(s):</b> 3D ACIS Modeler
 * <br><br>
 * @param body_list
 * list of bodies.
 * @param ao
 * acis options.
 **/
DECL_CT outcome  api_ct_remove( ENTITY_LIST  &body_list,
							   AcisOptions *ao = NULL);

/**
 * Checks the sanity of cellular topology.
 * <br><br>
 * <b>Role:</b> This API traverses and checks the cellular topology attached to the given
 * entity. If any problems are found, they are added to the <tt>insanity_list</tt>.
 * <br><br>
 * <b>Errors:</b> The pointer to an <tt>ENTITY</tt> is <tt>NULL</tt>.
 * <br><br>
 * <b>Effect:</b> System routine
 * <br><br>
 * <b>Journal: </b> Not Available
 * <br><br>
 * <b>Product(s):</b> 3D ACIS Modeler
 * <br><br>
 * @param ent
 * entity to check.
 * @param list
 * list of insane entities and info.
 * @param ao
 * acis options.
 **/
DECL_CT outcome  api_check_ct(const ENTITY* ent, insanity_list *&list,
							  AcisOptions *ao = NULL);

/**
 * Merges 3D cells.
 * <br><br>
 * <b>Role:</b> This API function traverses the cellular topology and removes the faces between
 * adjacent 3D cells in the input list. For greatest efficiency this function should be called
 * with one set of adjacent cells. For convenience it can also be called with a list that 
 * contains multiple sets of adjacent cells. (For example, if cells 1, 2, and 3 are to be merged,
 * and cells 4, 5, and 6 are to be merged, the function can be called with a list containing cells 
 * 1, 2, 3, 4, 5, and 6; however, it is more efficient to call the function twice, once with a list
 * containing cells 1, 2, and 3 and once with a list containing cells 4, 5, and 6.) 
 * <br><br>
 * In addition, this function performs a localized recompute of the cellular topology. Therefore, 
 * this function can control which cell(s) persist after the merge. Specifically, the function
 * retains the cell(s) that occur earlier in the input list. If cells have 
 * <tt>ATTRIB_CELL_PRECEDENCE</tt> attributes attached to them, the precedence of cells will
 * be determined using the cell precedence attributes rather than the input list order. 
 * <br><br>
 * <b>Errors:</b> Pointer to cell is <tt>NULL</tt> or not to a <tt>CELL3D</tt>.
 * <br><br>
 * <b>Effect:</b> Changes model
 * <br><br>
 * <b>Journal: </b> Available
 * <br><br>
 * <b>Product(s):</b> 3D ACIS Modeler
 * <br><br>
 * @param cell_list
 * 3D cells to merge.
 * @param ctmo
 * merge options object - For Future Use.
 * @param ao
 * acis options.
 **/
DECL_CT outcome  api_ct_merge_cells( ENTITY_LIST& cell_list, ct_merge_options* ctmo = NULL,
							  AcisOptions *ao = NULL );

/** @} */

#endif
