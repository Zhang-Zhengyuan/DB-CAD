/*******************************************************************/
/*    Copyright (c) 1989-2020 by Spatial Corp.                     */
/*    All rights reserved.                                         */
/*    Protected by U.S. Patents 5,257,205; 5,351,196; 6,369,815;   */
/*                              5,982,378; 6,462,738; 6,941,251    */
/*    Protected by European Patents 0503642; 69220263.3            */
/*    Protected by Hong Kong Patent 1008101A                       */
/*******************************************************************/
// Header for api routines.
// For details of how to call each routine, consult the documentation
// for individual routines.
#if !defined( BLENDAPI_HXX )
#define BLENDAPI_HXX
class BODY;
class FACE;
class EDGE;
class COEDGE;
class VERTEX;
class ENTITY;
class ENTITY_LIST;
class CURVE;
class curve;
class plane;
class ATTRIB_BLEND;
class blend1_data;
class SPAposition;
class SPAunit_vector;
class SPAvector;
class detect_feature_options;

#include "dcl_blnd.h"
#include "logical.h"
#include "api.hxx"
#include "bl_enum.hxx"
#include "vbl_enum.hxx"
/**
 * \defgroup BLNDAPI Blending
 *
  */
/**
 * @file blendapi.hxx
 * @CAA2Level L1
 * @CAA2Usage U1
 * \addtogroup BLNDAPI
 *
 * @{
 */
/**
* Initializes the blending library.
* <br><br>
* <b>Effect:</b> System routine
* <br><br>
* <b>Journal: </b> Not Available
* <br><br>
* <b>Product(s):</b> 3D ACIS Modeler
 * <br><br>
**/
DECL_BLND outcome api_initialize_blending();
/**
 * Terminates the blending library.
 * <br><br>
 * <b>Effect:</b> System routine
 * <br><br>
 * <b>Journal: </b> Not Available
 * <br><br>
 * <b>Product(s):</b> 3D ACIS Modeler
 * <br><br>
**/
DECL_BLND outcome api_terminate_blending();
/**
 * Locates a graph of edges.
 * <br><br>
 * <b>Role:</b> When given an edge or vertex, this api finds a graph of edges
 * that meet smoothly; i.e., with similar blend type, convexity, and radius, and
 * with tangent continuity at unblended vertices, or edges that meet at blended
 * vertices with the same or different blend radii or convexity. It returns the
 * blended edges and vertices in an entity list.
 * <br><br>
 * <b>Errors:</b> The pointer to an entity is <tt>NULL</tt>.
 * <br><br>
 * <b>Effect:</b> Read-only
 * <br><br>
 * <b>Journal: </b> Available
 * <br><br>
 * <b>Product(s):</b> 3D ACIS Modeler
 * <br><br>
* @param this_ent
 * an edge or vertex in the graph.
 * @param ent_list
 * returned list of connected entities.
 * @param ao
 * ACIS options.
 **/
DECL_BLND outcome api_blend_graph(
			ENTITY *this_ent,			// an edge or vertex in the graph
			ENTITY_LIST &ent_list,		// list of connected edges and vertices
								// found
			AcisOptions *ao = NULL // options, version, journal
		);
/**
 * Locates a sequence of connected edges that meet smoothly.
 * <br><br>
 * <b>Role:</b> Given an edge, this API finds a sequence of connected edges that
 * meet smoothly; i.e., with tangent continuity or meet at vertices that have the
 * same implicit blend as the edges (when the vertex is also entered into the list).
 * <br><br>
 * <b>Errors:</b> The pointer to an edge is <tt>NULL</tt> or does not point to an <tt>EDGE</tt>.
 * <br><br>
 * <b>Effect:</b> Read-only
 * <br><br>
 * <b>Journal: </b> Available
 * <br><br>
 * <b>Product(s):</b> 3D ACIS Modeler
 * <br><br>
* @param this_edge
 * an edge in the sequence.
 * @param ents
 * returned new list of connected entities.
 * @param ao
 * ACIS options.
 **/
DECL_BLND outcome api_blend_seq(
			EDGE *this_edge,				// an edge in the sequence
			ENTITY_LIST &ents,		// list of entities found
			AcisOptions *ao = NULL // options, version, journal
		);
/**
 * Performs the third phase of interactive blending, completing the blend operation
 * by attaching the blend sheet to the blank body using the intersection graph.
 * <br><br>
 * <b>Role:</b> Phase one of interactive blending creates a blend sheet. Phase two of
 * interactive blending performs the the first phase of a Boolean operation to create 
 * an intersection graph. This third and final phase of interactive blending performs 
 * the second phase of a Boolean operation to attach the blend sheet to the
 * blank body using the intersection graph.
 * <br><br>
 * This API functions works in conjunction with @href api_make_blend_wire and @href api_make_blend_sheet.
 * <br><br>
 * <b>Errors:</b> The pointer to intersection graph, blend sheet, or blank body is <tt>NULL</tt> 
 * or does not point to a <tt>BODY</tt>.
 * <br><br>
 * <b>Effect:</b> Changes model
 * <br><br>
 * <b>Journal: </b> Available
 * <br><br>
 * <b>Product(s):</b> 3D ACIS Modeler
 * <br><br>
* @param int_graph
 * the intersection graph.
 * @param sheet
 * the blend sheet.
 * @param this_body
 * the body to be blended.
 * @param ao
 * ACIS options.
 **/
DECL_BLND outcome api_complete_blends(
			BODY *int_graph,					// wire body already made
			BODY *sheet,					// sheet body already made
			BODY *this_body,					// body owning blended edges
			AcisOptions *ao = NULL // options, version, journal
		);
/**
 * Finishes the making of a blend sheet by single steps.
 * <br><br>
 * <b>Role:</b> This routine performs the final step in the first phase of
 * blending. It makes a sheet body for a given list of blend attributes in single
 * steps.
 * <br><br>
 * First @href api_init_blend_ss is called to initialize the process. It is given the
 * list of blend attributes and returns a sheet body (with no faces).
 * <br><br>
 * Then calls are made repeatedly to @href api_do_one_blend_ss until the list is
 * exhausted (the list may well grow during processing so the form of for-loop
 * shown above should be used).
 * <br><br>
 * A final call to <tt>api_concl_blend_ss</tt> concludes the making of the sheet. Further
 * calls must be made to make the blend wire and complete the blend.
 * <br><br>
 * <b>Effect:</b> Changes model
 * <br><br>
 * <b>Journal: </b> Not Available
 * <br><br>
 * <b>Product(s):</b> 3D ACIS Modeler
 * <br><br>
* @param bl1_data
 * container of blend1 data.
 * @param sheet_body
 * blend sheet.
 * @param ao
 * ACIS options.
 **/
DECL_BLND outcome api_concl_blend_ss(
			blend1_data &bl1_data,	// container of blend1 data
			BODY *sheet_body,			// sheet_body
			AcisOptions *ao = NULL // options, version, journal
		);

/**
 * Deletes an implicit blend from each of a list of edges and vertices.
 * <b>Technical Articles:</b> <i>[Blending](http://doc.spatial.com/articles/b/l/e/Component~Blending_c382.html)</i>, 
 * <i>[Blend Attributes](http://doc.spatial.com/articles/b/l/e/Blend_Attributes_568e.html)</i>
 * <br><br>
 * <b>Errors:</b> An entity in list is not an <tt>BODY</tt>, <tt>FACE</tt>, <tt>EDGE</tt>, or <tt>VERTEX</tt>.
 * <br><br>
 * <b>Effect:</b> Changes model
 * <br><br>
 * <b>Journal: </b> Available
 * <br><br>
 * <b>Product(s):</b> 3D ACIS Modeler
 * <br><br>
* @param ents_list
 * entities to be unblended.
 * @param ao
 * ACIS options.
 **/
DECL_BLND outcome api_delete_blends(
			ENTITY_LIST const &ents_list,	// entities with implicit blends
									// to be removed
			AcisOptions *ao = NULL // options, version, journal
		);
/**
 * Deletes an explicit blend on each of a list of faces.
 * <br><br>
 * <b>Errors:</b> An entity in list is not a <tt>FACE</tt>.
 * <br><br>
 * <b>Effect:</b> Changes model
 * <br><br>
 * <b>Journal: </b> Available
 * <br><br>
 * <b>Product(s):</b> 3D ACIS Modeler
 * <br><br>
* @param ents_list
 * entities to be unblended.
 * @param ao
 * ACIS options.
 **/
DECL_BLND outcome api_delete_exp_blends(
			ENTITY_LIST const &ents_list,	// entities whose explicit blends
									// are to be removed
			AcisOptions *ao = NULL // options, version, journal
		);
/**
 * Executes a single step in the making of a blend sheet.
 * <br><br>
 * <b>Role:</b> This routine performs a single step in the first phase of
 * blending. It makes a sheet body for a given blend attributes in the list. The
 * application has the opportunity to give the user graphical feedback on the
 * progress of the blend.
 * <br><br>
 * First @href api_init_blend_ss is called to initialize the process. It is given the
 * initialized <tt>blend1_data</tt> object and returns an empty sheet body.
 * <br><br>
 * Then calls are made repeatedly to <tt>api_do_one_blend_ss</tt> until the list is
 * exhausted. The list may well grow during processing, so a for-loop such as the
 * following should be used:
 * <pre>
 * for ( int i = 0; ; i++ ) {
 *   result = do_one_blended_ent(
 *   bl1_data, i, blend_sheet );
 *   if ( result &lt;= 0 ) break;
 *   ...
 * }
 * </pre>
 * A final call to @href api_concl_blend_ss ends the making of the sheet. Further calls
 * must be made to make the blend wire and complete the blend.
 * <br><br>
 * <b>Effect:</b> Changes model
 * <br><br>
 * <b>Journal: </b> Available
 * <br><br>
 * <b>Product(s):</b> 3D ACIS Modeler
 * <br><br>
* @param bl1_data
 * container of blend1_data.
 * @param index
 * index in list of entities to be blended.
 * @param sheet_body
 * blend sheet (updated).
 * @param ao
 * ACIS options.
 **/
DECL_BLND outcome api_do_one_blend_ss(
			blend1_data &bl1_data,	// container of blend1 data
			int index,			// index in list of attribute to be processed
			BODY *sheet_body,			// sheet body beind made (updated)
			AcisOptions *ao = NULL // options, version, journal
		);
/**
 * Creates explicit blends by making a new face to replace each implicitly blended edge and vertex, or by computing an entity-entity blend.
 * <b>Technical Articles:</b> <i>[Blending](http://doc.spatial.com/articles/b/l/e/Component~Blending_c382.html), [Blend Attributes](http://doc.spatial.com/articles/b/l/e/Blend_Attributes_568e.html)</i>
 * <br><br>
 * <b>Role:</b> Occurs when given a list of edges and vertices marked with
 * implicit blends, or a <tt>BODY</tt> is passed and there is an entity-entity blend
 * attribute on the body. The entities passed should be either edges and vertices,
 * or a body, not a mixture.
 * <br><br>
 * If the edges have similar blend types, the same convexity, and the same radius,
 * all edges that meet at an unblended vertex are blended. When edges with
 * different blend radii, convexity, and/or setback meet at a blended vertex, they
 * are blended. Edges with different blend radii, convexity, and/or setback that
 * meet smoothly (their adjoining faces meet at the edge with tangent %plane
 * continuity) at an unblended vertex, are blended.
 * <br><br>
 * If there are features lying inside the support region that is replaced 
 * by blend, then blending attempts to retain those features. This behavior 
 * can be controlled by the option <tt>bl_retain_features</tt>. However, if blending 
 * fails to retain such features, then it will continue to give a result 
 * that it is ignoring them. A <tt>sys_warning</tt> is raised when blending detects features 
 * but is unable to retain them. This functionality is available only when this fix
 * API is called after the following implicit blend APIs: @href api_set_const_chamfers, 
 * @href api_set_const_rounds, and @href api_set_var_blends.
 * <br><br>
 * Returns the blended edges and vertices of the graph in an entity list.
 * <br><br>
 * <b>Errors:</b> An entity in list is not an <tt>EDGE</tt> or <tt>VERTEX</tt> or <tt>BODY</tt>.
 * <br><br>
 * <b>Limitations:</b> Blending a single edge cannot handle blends extending beyond the
 * adjacent face boundaries or stopped chamfers/rounds. Blending at a vertex where
 * two edges meet cannot handle blending of edges meeting with a discontinuity.
 * <br><br>
 * Blending at a vertex where three edges meet if there are mixed convexities, the
 * order in which blends are fixed is important. The edge with a different
 * convexity must be fixed first. For example. if there are two convex blend edges
 * and one concave, fix the single concave edge first.
 * <br><br>
 * Blending at a vertex where more than three edges meet, only single blends
 * (blending of edges and not vertices) are supported.
 * <br><br>
 * <tt>api_fix_blends</tt> is not designed to handle edge lists that has edges from multiple
 * bodies. If edges from multiple bodies must be blended simultaneously, the
 * functions @href api_blend_edges and @href api_chamfer_edges can be used.
 * <br><br>
 * Cannot handle mixed convexities.
 * <br><br>
 * <b>Effect:</b> Changes model
 * <br><br>
 * <b>Journal: </b> Available
 * <br><br>
 * <b>Product(s):</b> 3D ACIS Modeler
 * <br><br>
* @param ents
 * list of edges and vertices, or body.
 * @param ao
 * ACIS options.
 **/
DECL_BLND outcome api_fix_blends(
			ENTITY_LIST const &ents,	// list of entities carrying implicit
			AcisOptions *ao = NULL // options, version, journal
		);

/**
 * Initializes the making of a blend sheet using single steps.
 * <br><br>
 * <b>Role:</b> This routine initializes the process for carrying out the first
 * phase of blending (make a sheet body for a given list of blend attributes) in
 * single steps. The application has the opportunity to give the user graphical
 * feedback on the progress of the blend.
 * <br><br>
 * First <tt>api_init_blend_ss</tt> is called to initialize the process. It is given the
 * list of blend attributes and returns an empty sheet body.
 * <br><br>
 * Then calls are made repeatedly to @href api_do_one_blend_ss until the list is
 * exhausted (the list may well grow during processing so the form of for-loop
 * shown above should be used).
 * <br><br>
 * A final call to @href api_concl_blend_ss ends the making of the sheet. Further calls
 * must be made to make the blend wire and complete the blend.
 * <br><br>
 * <b>Errors:</b> No blend attributes in the list.
 * <br><br>
 * <b>Effect:</b> Changes model
 * <br><br>
 * <b>Journal: </b> Not Available
 * <br><br>
 * <b>Product(s):</b> 3D ACIS Modeler
 * <br><br>
* @param bl1_data
 * container of blend1 data.
 * @param sheet_body.
 * returned new blended sheet body.
 * @param ao
 * ACIS options.
 **/
DECL_BLND outcome api_init_blend_ss(
			blend1_data &bl1_data,	// Container of blend1 data.
							// Must be constructed with the list
							// of blend attributes to be processed.
			BODY *&sheet_body,			// Initial sheet body made
			AcisOptions *ao = NULL // options, version, journal
		);
/**
 * Creates a cross %curve of an un-fixed blend at a given ''v''-parameter.
 * <br><br>
 * <b>Role:</b> Given a blend attribute, and a ''v''-parameter along the blend, this
 * routine creates a "rib" %curve, running from one base entity to the other, at a
 * location along the blend determined by the ''v''-parameter. (The ''v''-parameter runs
 * along the blend, and the ''u''-parameter runs from one base surface to the other.)
 * The ''v''-parameter of the blend is the same as that of its defining curve, which is
 * generally the curve of the edge being blended.
 * <br><br>
 * <b>Errors:</b> Sometimes the blend cannot evaluate a cross %curve at the given parameter,
 * for example, if the blend radius is too big.
 * <br><br>
 * <b>Limitations:</b> Currently this is only implemented for <tt>ATTRIB_VAR_BLENDs</tt> and
 * <tt>ATTRIB_CONST_ROUNDs</tt>.
 * <br><br>
 * <b>Effect:</b> Changes model
 * <br><br>
 * <b>Journal: </b> Available
 * <br><br>
 * <b>Product(s):</b> 3D ACIS Modeler
 * <br><br>
* @param att
 * entities to be blended.
 * @param v_param
 * parameter.
 * @param ccrv
 * new curve created.
 * @param ao
 * ACIS options.
 **/
DECL_BLND outcome api_make_blend_cross_curve (
			const ATTRIB_BLEND *att,	// blend attribute
			double v_param,				// v-SPAparameter of blend
			curve *& ccrv,				// resulting cross curve
			AcisOptions *ao = NULL // options, version, journal
		);
/**
 * Performs the first phase of interactive blending, creating a sheet body for a given list of edges, 
 * each bearing an implicit blend attribute.
 * <br><br>
 * <b>Role:</b> This first phase of interactive blending makes a sheet body for a given list
 * of edges, each bearing an implicit blend attribute.
 * <br><br>
 * The sheet made contains one or more faces for each blended entity. Attributes
 * link the sheet to the blend body (the body owning the blended entities), thereby
 * facilitating the next stage of blending when the sheet is intersected with the
 * blend body. The sheet faces lie on new blend surfaces or on surfaces of the
 * blend body where these are needed to cap blends at ends of blended edges.
 * <br><br>
 * Where two blend faces are made for blended edges meeting non smoothly in an
 * unblended vertex, the faces are trimmed to one another; i.e. are mitered, and
 * the sheet faces are joined.
 * <br><br>
 * This API functions works in conjunction with @href api_make_blend_wire and @href api_complete_blends.
 * <br><br>
 * <b>Errors:</b> No entities with implicit blend attributes in the list. Implicit blend attributes found on an
 * entity that is not an <tt>EDGE</tt> or <tt>VERTEX</tt>.
 * <br><br>
 * <b>Limitations:</b> Blending a single edge cannot handle blends extending beyond the
 * adjacent face boundaries or stopped chamfers/rounds. Blending at a vertex where
 * two edges meet cannot handle blending of edges meeting with a discontinuity.
 * <br><br>
 * When blending at a vertex where three edges meet and there are mixed convexities, the
 * order in which blends are fixed is important. The edge with a different
 * convexity must be fixed first. For example. if there are two convex blend edges
 * and one concave, fix the single concave edge first.
 * <br><br>
 * Blending at a vertex where more than three edges meet, only single blends
 * (blending of edges and not vertices) are supported.
 * <br><br>
 * Cannot handle mixed convexities.
 * <br><br>
 * <b>Effect:</b> Changes model
 * <br><br>
 * <b>Journal: </b> Available
 * <br><br>
 * <b>Product(s):</b> 3D ACIS Modeler
 * <br><br>
* @param ent_list
 * connected edges to be blended.
 * @param sheet_body
 * returned new sheet body.
 * @param ao
 * ACIS options.
 **/
DECL_BLND outcome api_make_blend_sheet(
			ENTITY_LIST const &ent_list,		// connected edges to be blended
			BODY *&sheet_body,					// sheet body made
			AcisOptions *ao = NULL // options, version, journal
		);
/**
 * Performs the second phase of interactive blending, creating a wire body that represents 
 * the intersection of the blend sheet with the body being blended.
 * <br><br>
 * <b>Role:</b> The second phase of interactive blending creates a wire body that represents
 * the <i>intersection graph</i> of the blend sheet with the body being blended. 
 * <br><br>
 * This API functions works in conjunction with @href api_make_blend_sheet and @href api_complete_blends.
 * <br><br>
 * <b>Errors:</b> Pointer to <tt>sheet_body</tt> or <tt>blank_body</tt> is <tt>NULL</tt> or not to a <tt>BODY</tt>.
 * <br><br>
 * <b>Limitations:</b> Blending a single edge cannot handle blends extending beyond the
 * adjacent face boundaries or stopped chamfers/rounds. Blending at a vertex where
 * two edges meet cannot handle blending of edges meeting with a discontinuity.
 * <br><br>
 * When blending at a vertex where three edges meet and there are mixed convexities, the
 * order in which blends are fixed is important. The edge with a different
 * convexity must be fixed first. For example, if there are two convex blend edges
 * and one concave, fix the single concave edge first.
 * <br><br>
 * Blending at a vertex where more than three edges meet, only single blends
 * (blending of edges and not vertices) are supported.
 * <br><br>
 * Cannot handle mixed convexities.
 * <br><br>
 * <b>Effect:</b> Changes model
 * <br><br>
 * <b>Journal: </b> Available
 * <br><br>
 * <b>Product(s):</b> 3D ACIS Modeler
 * <br><br>
* @param sheet_body
 * blend sheet.
 * @param blank_body
 * the body being blended.
 * @param wire_body
 * returned new wire body.
 * @param ao
 * ACIS options.
 **/
DECL_BLND outcome api_make_blend_wire(
			BODY *sheet_body,						// sheet body given
			BODY *blank_body,						// blank body given
			BODY *&wire_body,					// wire body made
			AcisOptions *ao = NULL // options, version, journal
		);
/**
 * Sets an implicit constant radius chamfer on each of a list of edges or vertices.
 * <b>Technical Articles:</b> <i>[Blending](http://doc.spatial.com/articles/b/l/e/Component~Blending_c382.html)</i>, 
 * <i>[Blend Attributes](http://doc.spatial.com/articles/b/l/e/Blend_Attributes_568e.html)</i>
 * <br><br>
 * <b>Role:</b> This API attaches implicit chamfers as attributes to each entity
 * in the list. Left and right ranges are interpreted as distances from the edge
 * along support faces adjoining the edge.
 * 
 * <br><br>
 * The sense of left and right is taken from the sequence of entities in the list.
 * Entities whose sense is reversed compared to the list have the chamfer set, but
 * left and right ranges are interchanged. If the list contains one edge or two
 * entities that form a closed sequence, the sense of left and right is taken from
 * the first edge in the list. The chamfer range(s) must be supplied in the local
 * space of the body.
 * <br><br>
 * Setbacks along the left and right spring curves can be made to differ and a
 * nonorthogonal cross %curve obtained by giving a nonzero (signed) value of (right
 * setback - left setback) and setting the appropriate logical argument <tt>TRUE</tt>. At
 * the start, the actual setbacks are found by
 * <pre>
 * start_right_setback = start_setback +
 *               (0.5 * start_sb_diff)
 * start_left_setback = start_setback -
 *               (0.5 * start_sb_diff)
 * </pre>
 * If <tt>start_sb_diff_set</tt> is given as <tt>FALSE</tt>, the setbacks are determined from the
 * intersection of the left or right spring curves with the spring curves of the
 * neighboring blended edges at the start. To ensure an orthogonal cross %curve at
 * the start, the setback difference should be set to zero and the logical
 * <tt>start_sb_diff_set</tt> given as <tt>TRUE</tt>. These are the default values. The end is
 * treated similarly.
 * <br><br>
 * If start stop angle or end stop angle values are specified, the edge blend is stopped and capped 
 * with a stop %plane at given setback distance. This stop %plane is constructed such that the angle 
 * between stop %plane and edge direction, about the line passing through end points of blend 
 * cross section, is equal to the specified stop angle value at that end. 
 * Stop angle value of 180 degrees is treated as a special case. In such a case, stop %plane is 
 * constructed such that it passes through end vertex and end points of blend cross section 
 * at given setback distance.
 * <br><br>
 * If there are features lying inside the support region that is replaced 
 * by blend, then blending attempts to retain those features. This behavior 
 * can be controlled by the option <tt>bl_retain_features</tt>. However, if blending 
 * fails to retain such features, then it will continue to give a result 
 * that it is ignoring them. A <tt>sys_warning</tt> is raised when blending detects features 
 * but is unable to retain them.
  * <br>
 * <dl>
 * <b>Errors:</b> An entity in the list is not an <tt>EDGE</tt>.
 * <dd>&nbsp;Left or right range less than -<tt>SPAresabs</tt>.</dd>
 * <dd>&nbsp;Start or end setback less than -<tt>SPAresabs</tt>.</dd>
 * </dl>
 * <br>
 * <b>Limitations:</b> Entity-entity blends cannot create a stopped blend.
 * The stopping location is based upon the edge being blended, thus entity-entity blends do not have
 * this defined location.
 * <br><br>
 * <b>Effect:</b> Changes model
 * <br><br>
 * <b>Journal: </b> Available
 * <br><br>
 * <b>Product(s):</b> 3D ACIS Modeler
 * <br><br>
* @param entities
 * connected entities to be chamfered.
 * @param left_range
 * distance along the left support entity.
 * @param right_range
 * distance along the right support entity.
 * @param start_setback
 * starting setback.
 * @param end_setback
 * ending setback.
 * @param start_setback_diff
 * start setback difference.
 * @param end_setback_diff
 * end setback difference.
 * @param start_setback_diff_set
 * <tt>TRUE</tt> if start setback difference is set.
 * @param end_setback_diff_set
 * <tt>TRUE</tt> if end setback difference is set.
 * @param start_stop_ang
 * start stop angle.
 * @param end_stop_ang
 * end stop angle.
 * @param ao
 * ACIS options.
 **/
DECL_BLND outcome api_set_const_chamfers(
			ENTITY_LIST const & entities,// edges to be chamfered
			double left_range,				// left range
			double right_range,				// right range
			double start_setback = 0,			// start setback
			double end_setback = 0,			// end setback
			double start_setback_diff = 0,			// setback difference (r - l) at start
			double end_setback_diff = 0,			// setback difference (r - l) at end
			logical start_setback_diff_set = TRUE,		// setback difference set at start
			logical end_setback_diff_set = TRUE,		// setback difference set at end
			double start_stop_ang = 0,			// start stop angle
			double end_stop_ang = 0,			// end stop angle
			AcisOptions *ao = NULL // options, version, journal
		);

/**
 * @nodoc
 */
/*
 Sets an implicit angle-distance chamfer on each of a list of edges.
 <br><br>
 <b>Role:</b> This API attaches implicit chamfers as attributes to each edge 
 corresponding to coedges in the list. i_distance is interpreted as direct 
 distance from the edge along support face on which the i_coedge lies. i_angle is
 the angle to reach the other face.
 
 <dl>
 <b>Errors:</b> An entity in the list is not a <tt>COEDGE</tt>.
 <dd>&nbsp;Distance less than -<tt>SPAresabs</tt>.</dd>
 <dd>&nbsp;Angle less than -<tt>res_near_tangent</tt>.</dd>
 </dl>
 <br>
 <b>Limitations:</b> Complex analytic support configurations.
 Spline supports.
 <br><br>
 <b>Effect:</b> Changes model
 <br><br>
 <b>Journal: </b> Available
 <br><br>
 <b>Product(s):</b> 3D ACIS Modeler
 <br><br>
 @param i_coedges
 Coedges to be chamfered.
 @param i_distance
 distance along coedge's face.
 @param angle
 distance along other face.
 @param o_edges
 List of EDGES on which chamfer attribute got attached.
 @param ao
 ACIS options.
*/
DECL_BLND outcome
api_set_angle_distance_chamfers(
			ENTITY_LIST const &i_coedges,	// Coedges to be chamfered
			double i_distance,	// Distance on face of coedge
			double i_angle,	// Angle
			ENTITY_LIST *o_edges = NULL,	// Output list of edges that can be directly given to api_fix_blends
			AcisOptions *ao = NULL // ACIS-Options, version, journal
		);

/**
 * Sets an implicit constant radius round on each entity on a list of edges or vertices.
 * <b>Technical Articles:</b> <i>[Blending](http://doc.spatial.com/articles/b/l/e/Component~Blending_c382.html)</i>, 
 * <i>[Blend Attributes](http://doc.spatial.com/articles/b/l/e/Blend_Attributes_568e.html)</i>
 * <br><br>
 * <b>Role:</b> This API attaches implicit rounds as attributes to each edge in
 * the list. Supplies the round radius and start and end setbacks in the local
 * space of the body. The start and end setbacks are zero or positive. They
 * determine the approximate distance from the blended start or end vertex where
 * the blend surface for the vertex ends and the blend surface for the edge begins.
 * Setback is ignored if the vertex is not blended.
 * <br><br>
 * Setbacks along the left and right spring curves can be made to differ and a non
 * circular cross %curve obtained by giving a nonzero (signed) value of (right
 * setback - left setback) and setting the appropriate logical argument <tt>TRUE</tt>. At
 * the start, the actual setbacks are found by
 * <pre>
 * start_right_setback = start_setback +
 *               (0.5 * start_sb_diff)
 * start_left_setback = start_setback -
 *               (0.5 * start_sb_diff) 
 * </pre>
 * If <tt>start_sb_diff_set</tt> is given as <tt>FALSE</tt>, the setbacks are determined from the
 * intersection of the left or right spring curves with the spring curves of the
 * neighboring blended edges at the start. To ensure a circular cross %curve at the
 * start, the setback difference should be set to zero and the logical
 * <tt>start_sb_diff_set</tt> given as <tt>TRUE</tt>. These are the default values. The end is
 * treated similarly.
 * <br><br>
 * If start stop angle or end stop angle values are specified, the edge blend is stopped and capped 
 * with a stop %plane at given setback distance. This stop %plane is constructed such that the angle 
 * between stop %plane and edge direction, about the line passing through end points of blend 
 * cross section, is equal to the specified stop angle value at that end. 
 * Stop angle value of 180 degrees is treated as a special case. In such a case, stop %plane is 
 * constructed such that it passes through end vertex and end points of blend cross section 
 * at given setback distance.
 * <br><br>
 * <dl>
 * <b>Errors:</b> An entity in the list is not an <tt>EDGE</tt>.
 * <dd>Radius less than -<tt>SPAresabs</tt>.</dd>
 * <dd>Start or end setback less than -<tt>SPAresabs</tt>.</dd>
 * </dl>
 * <br>
 * <b>Limitations:</b> Entity-entity blends cannot create a stopped blend.
 *  The stopping location is based upon the edge being blended, thus entity-entity blends do not have
 *  this defined location.
 * <br><br>
 * <b>Effect:</b> Changes model
 * <br><br>
 * <b>Journal: </b> Available
 * <br><br>
 * <b>Product(s):</b> 3D ACIS Modeler
 * <br><br>
* @param entities
 * entities to be rounded.
 * @param radius
 * round radius.
 * @param start_setback
 * starting setback.
 * @param end_setback
 * ending setback.
 * @param start_setback diff
 * start setback difference.
 * @param end_setback_diff
 * end setback difference.
 * @param start_setback_diff_set
 * <tt>TRUE</tt> if start setback difference is set.
 * @param end_setback_diff_set
 * <tt>TRUE</tt> if end setback difference is set.
 * @param start_stop_ang
 * start stop angle.
 * @param end_stop_ang
 * end stop angle.
 * @param eprop
 * for future use.
 * @param ao
 * ACIS options.
 **/
DECL_BLND outcome api_set_const_rounds(
			ENTITY_LIST const & entities,// edges to be rounded
			double radius,				// radius,
			double  start_setback = 0,			// start setback
			double end_setback = 0,			// end setback
			double  start_setback_diff = 0,			// setback difference (r - l) at start
			double end_setback_diff = 0,			// setback difference (r - l) at end
			logical start_setback_diff_set = TRUE,		// setback difference set at start
			logical end_setback_diff_set = TRUE,		// setback difference set at end
			double start_stop_ang  = 0,			// start stop angle
			double end_stop_ang = 0,			// end stop angle
			blend_how eprop = bl_how_default,	// pass bl_how_roll_on to "prefer edges"
			AcisOptions *ao = NULL // options, version, journal
		);
/**
 * Sets an explicit constant chamfer attribute on a face.
 * <br><br>
 * <b>Role:</b> This API attaches an explicit chamfer as an attribute of the
 * given face and places support attributes on the left and right support faces,
 * given or deduced. Local operations are then able to make the blend geometry
 * agree with the geometry given in the explicit blend attribute. It gives a means
 * for changing a blend in place (a blend edit) or for making new exact blend
 * geometry in place of existing blend geometry perhaps obtained as the result of
 * earlier modeling operations or imported from elsewhere. The face to be regarded
 * as a blend need not have been made using a blend operation. The change of blend
 * geometry must not be so great as to require a change in the topology of the
 * blend face.
 * <br><br>
 * If there are features lying inside the support region that is replaced 
 * by blend, then blending attempts to retain those features. This behavior 
 * can be controlled by the option <tt>bl_retain_features</tt>. However, if blending 
 * fails to retain such features, then it will continue to give a result 
 * that it is ignoring them. A <tt>sys_warning</tt> is raised when blending detects features 
 * but is unable to retain them.
 * <br><br>
 * All the arguments except the first are defaulted since in many cases (e.g. where
 * the blend surface is a simple analytic surface) they can be deduced from the
 * given blend face. If arguments are omitted but cannot be deduced from the given
 * blend face, the routine will return a failure outcome.
 * <br><br>
 * At present this API routine does not deal with setbacks.
 * <br><br>
 * <b>Errors:</b> Left or right range less than -<tt>SPAresabs</tt>.
 * <br><br>
 * <b>Effect:</b> Changes model
 * <br><br>
 * <b>Journal: </b> Available
 * <br><br>
 * <b>Product(s):</b> 3D ACIS Modeler
 * <br><br>
* @param bl_face
 * face to be given attribute (not defaulted).
 * @param left_face
 * left support face.
 * @param right_face
 * right support face.
 * @param left_range
 * left range.
 * @param right_range
 * right range.
 * @param convexity
 * convexity, <tt>TRUE</tt> for convex.
 * @param mid_plane
 * approximate normal plane at mid-point of spine.
 * @param ao
 * ACIS options.
 **/
DECL_BLND outcome api_set_exp_const_chamfer(
			FACE * bl_face,										// blend face
			FACE * left_face = NULL,							// left support face
			FACE * right_face = NULL,							// right support face
			double const & left_range = *(double *)NULL_REF,		// left range
			double const & right_range = *(double *)NULL_REF,		// right range
			logical const & convexity = *(logical *)NULL_REF,		// convexity
			plane const &mid_plane = *(plane *)NULL_REF,			// mid-plane
			AcisOptions *ao = NULL // options, version, journal
		);

/**
 * Sets an explicit constant round attribute on a face and support attributes on its support faces.
 * <br><br>
 * <b>Role:</b> This API attaches an explicit round as an attribute of the given
 * face and places support attributes on the left and right support faces, given or
 * deduced. Local operations are then able to make the blend geometry agree with
 * the geometry given in the explicit blend attribute. It gives a means of changing
 * a blend in place (a blend edit) or of making new exact blend geometry in place
 * of existing blend geometry perhaps obtained as the result of earlier modeling
 * operations or imported from elsewhere. The face to be regarded as a blend need
 * not have been made using a blend operation. The change of blend geometry must
 * not be so great as to require a change in the topology of the blend face.
 * <br><br>
 * All the arguments except the first are defaulted since in many cases (e.g. where
 * the blend surface is a simple analytic surface) they can be deduced from the
 * given blend face. If arguments are omitted but cannot be deduced from the given
 * blend face, the routine returns a failure outcome. At present this API routine
 * does not deal with setbacks.
 * <br><br>
 * <b>Effect:</b> Changes model
 * <br><br>
 * <b>Journal: </b> Available
 * <br><br>
 * <b>Product(s):</b> 3D ACIS Modeler
 * <br><br>
* @param bl_face
 * face to be given attribute (not defaulted).
 * @param left_face
 * left support face.
 * @param right_face
 * right support face.
 * @param radius
 * radius.
 * @param convexity
 * convexity, <tt>TRUE</tt> for convex.
 * @param mid_plane
 * approximate normal plane at mid-point of spine.
 * @param start_cdt
 * start condition <tt>TRUE</tt> if open.
 * @param end_cdt
 * end condition <tt>TRUE</tt> if open.
 * @param ao
 * ACIS options.
 **/
DECL_BLND outcome api_set_exp_const_round(
			FACE * bl_face,										// blend face
			FACE * left_face = NULL,								// left support face
			FACE * right_face = NULL,								// right support face
			double const &radius = *(double *)NULL_REF,		// radius
			logical const &convexity = *(logical *)NULL_REF,		// convexity
			plane const &mid_plane = *(plane *)NULL_REF,			// mid-plane
			logical const &start_cdt = *(logical *)NULL_REF,		// start & end condits.
			logical const &end_cdt = *(logical *)NULL_REF,		// (true if open)
			AcisOptions *ao = NULL // options, version, journal
 		);
/**
 * Sets an explicit constant round attribute on a face (and any faces split from it) and places support attributes on the support faces.
 * <br><br>
 * <b>Role:</b> This API attaches an explicit constant round face-face blend
 * attribute to the face of the given coedges (and any other face split from it)
 * and places support attributes on the left and right support faces. If not given,
 * the blend radius is estimated. A def-plane and convexity are found for each
 * attribute made. If blend radius is not given, it is estimated from the blend
 * face curvature.
 * <br><br>
 * Member functions of the attributes made are used to re-blend geometry in healing.
 * <br><br>
 * <b>Effect:</b> Changes model
 * <br><br>
 * <b>Journal: </b> Available
 * <br><br>
 * <b>Product(s):</b> 3D ACIS Modeler
 * <br><br>
* @param start_cross_coedges
 * start cross coedges.
 * @param end_cross_coedges
 * end cross coedges.
 * @param left_spring_coedges
 * left spring coedges.
 * @param right_spring_coedges
 * right spring coedges.
 * @param bl_radius
 * blend radius.
 * @param len_tol
 * length tolerance.
 * @param bl_faces_noted
 * blend faces noted.
 * @param ao
 * ACIS options.
 **/
DECL_BLND outcome api_set_exp_co_ro_ffbl_att(		// set face-face blend att.
			ENTITY_LIST const &start_cross_coedges,					// start_cross_coedges
			ENTITY_LIST const &end_cross_coedges,					// end_cross_coedges
			ENTITY_LIST const &left_spring_coedges,					// left_spring_coedges
			ENTITY_LIST const &right_spring_coedges,					// right_spring_coedges
			double const &bl_radius = *(double *)NULL_REF,	// bl_radius
			double const &len_tol = *(double *)NULL_REF,	// len_tol
			ENTITY_LIST &bl_faces_noted = *(ENTITY_LIST *)NULL_REF, // blend face(s) noted
			AcisOptions *ao = NULL // options, version, journal
		);

/**
 * Sets an explicit constant round attribute on a face (and any faces split from it) and places support attributes on the support faces.
 * <br><br>
 * <b>Role:</b> This API attaches an explicit constant round face-face blend
 * attribute to the face of the given coedges (and any other face split from it)
 * and places support attributes on the left and right support faces. If not given,
 * the blend radius is estimated. A def-plane and convexity are found for each
 * attribute made. If blend radius is not given, it is estimated from the blend
 * face curvature.
 * <br><br>
 * Member functions of the attributes made are used to re-blend geometry in healing.
 * <br><br>
 * <b>Effect:</b> Changes model
 * <br><br>
 * <b>Journal: </b> Available
 * <br><br>
 * <b>Product(s):</b> 3D ACIS Modeler
 * <br><br>
* @param start_cross_coedges
 * start cross coedges.
 * @param end_cross_coedges
 * end cross coedges.
 * @param spring_coedges
 * spring coedges.
 * @param support_on_left
 * support on left if <tt>TRUE</tt>.
 * @param bl_radius
 * blend radius.
 * @param len_tol
 * length tolerance.
 * @param bl_faces_noted
 * blend faces noted.
 * @param ao
 * ACIS options.
 **/
DECL_BLND outcome api_set_exp_co_ro_fbl_att(		// set face blend att.
			ENTITY_LIST const & start_cross_coedges,					// start_cross_coedges
			ENTITY_LIST const & end_cross_coedges,					// end_cross_coedges
			ENTITY_LIST const & spring_coedges,					// spring_coedges
			logical const & support_on_left,						// support on left
			double const &bl_radius = *(double *)NULL_REF,	// bl_radius
			double const & len_tol = *(double *)NULL_REF,	// len_tol
			ENTITY_LIST &bl_faces_noted = *(ENTITY_LIST *)NULL_REF, // blend face(s) noted
			AcisOptions *ao = NULL // options, version, journal
		);
/**
 * Sets an implicit blend on a vertex.
 * <b>Technical Articles:</b> <i>[Blending](http://doc.spatial.com/articles/b/l/e/Component~Blending_c382.html)</i>, 
 * <i>[Blend Attributes](http://doc.spatial.com/articles/b/l/e/Blend_Attributes_568e.html)</i>
 * <br><br>
 * <b>Role:</b> An implicit blend is attached as an attribute of the given vertex.
 * <br><br>
 * The bulge may be given a value between zero and two (one is usual). A larger
 * value increases the fullness of the blend.
 * <br><br>
 * The setback, if zero or positive, is applied to the end of each blended edge
 * terminating in the vertex. If the given setback is negative, the setbacks
 * already given to blended edges at the vertex remain unchanged.
 * <br><br>
 * The details of the blend face are found from the radius and setbacks of the
 * edges that end in the vertex.
 * <br><br>
 * The <tt>bl_continuity</tt> can be set to: <tt>unset_continuity</tt>, <tt>position_continuous</tt>,
 * <tt>slope_continuous</tt> (default), or <tt>curvature_continuous</tt>.
 * <br><br>
 * The blend property can be set to:
 * <br><br>
 * <table width = 100%>
 * <tr width = 100%>
 * <td valign = "top" width = 20%><tt>bl_v_cap</tt></td>
 * <td valign = "top">means a cap or miter at a vertex joined to one or two blended
 * edges (same as default behavior with no attribute on the vertex at the end of a
 * blend sequence).</td>
 * </tr>
 * <tr>
 * <td valign = "top"><tt>bl_v_roll_on</tt></td>
 * <td valign = "top">means close off the open end or continue on to the next blended
 * edge at the vertex using edge-face or edge-edge blends.</td>
 * </tr>
 * <tr>
 * <td valign = "top"><tt>bl_v_runout</tt></td>
 * <td valign = "top">means close off the open end using a variable-radius blend.</td>
 * </tr>
 * <tr>
 * <td valign = "top"><tt>bl_v_unset</tt></td>
 * <td valign = "top">is a vertex blend.</td>
 * </tr>
 * <tr>
 * <td valign = "top"><tt>bl_v_blend</tt></td>
 * <td valign = "top">is a vertex blend.</td>
 * </tr>
 * </table>
 * <br>
 * <dl>
 * <b>Errors:</b> Pointer to vertex <tt>NULL</tt> or not to a <tt>VERTEX</tt>.
 * <dd>&nbsp;Bulge factor less than 0 or greater than 2.0.</dd>
 * </dl>
 * <br>
 * <b>Effect:</b> Changes model
 * <br><br>
 * <b>Journal: </b> Available
 * <br><br>
 * <b>Product(s):</b> 3D ACIS Modeler
 * <br><br>
* @param this_vertex
 * vertex to be blended.
 * @param bulge
 * bulge factor [0...2].
 * @param setback
 * setback from vertex.
 * @param bl_contin
 * blend continuity.
 * @param bl_v_prop
 * blend property.
 * @param ao
 * ACIS options.
 **/
DECL_BLND outcome api_set_vblend(
			VERTEX * this_vertex,	// vertex to be blended
			double bulge,				// bulge
			double  setback = 0,			// setback
			bl_continuity  bl_contin = slope_continuous,
			bl_v_property  bl_v_prop = bl_v_unset,
			AcisOptions * ao = NULL // options, version, journal
		);
/**
 * Sets an implicit blend on a vertex, computing and assigning setbacks automatically.
 * <br><br>
 * <b>Role:</b> An implicit blend is attached as an attribute of the given
 * vertex. The bulge may be given a value between zero and two (one is usual). A
 * larger value increases the fullness of the blend.
 * <br><br>
 * Setbacks are computed and applied to each edge of the vertex. Setback values
 * depend on the geometry of the body local to the blended vertex and on sizes of
 * blends already assigned to edges ending in the vertex. The caller should ensure
 * that blend attributes are set on the edges of the vertex before this API is
 * called. Setbacks on edges blended with blends of zero size are left unchanged.
 * <br><br>
 * The details of the blend face are found from the radius and setbacks of the
 * edges that end in the vertex.
 * <br><br>
 * The <tt>bl_continuity</tt> can be set to: <tt>unset_continuity</tt>, <tt>position_continuous</tt>,
 * <tt>slope_continuous</tt> (default), or <tt>curvature_continuous</tt>.
 * <br><br>
 * The blend property can be set to:
 * <br><br>
 * <table width = 100%>
 * <tr width = 100%>
 * <td valign = "top" width = 20%><tt>bl_v_cap</tt></td>
 * <td valign = "top">means a cap or miter at a vertex joined to one or two blended
 * edges (same as default behavior with no attribute on the vertex at the end of a
 * blend sequence).</td>
 * </tr>
 * <tr>
 * <td valign = "top"><tt>bl_v_roll_on</tt></td>
 * <td valign = "top">means close off the open end or continue on to the next blended
 * edge at the vertex using edge-face or edge-edge blends.</td>
 * </tr>
 * <tr>
 * <td valign = "top"><tt>bl_v_runout</tt></td>
 * <td valign = "top">means close off the open end using a variable-radius blend.</td>
 * </tr>
 * <tr>
 * <td valign = "top"><tt>bl_v_unset</tt></td>
 * <td valign = "top">is a vertex blend.</td>
 * </tr>
 * <tr>
 * <td valign = "top"><tt>bl_v_blend</tt></td>
 * <td valign = "top">is a vertex blend.</td>
 * </tr>
 * </table>
 * <br>
 * <dl>
 * <b>Errors:</b> Pointer to vertex <tt>NULL</tt> or not to a <tt>VERTEX</tt>.
 * <dd>&nbsp;Bulge factor less than 0 or greater than 2.0.</dd>
 * </dl>
 * <br>
 * <b>Effect:</b> Changes model
 * <br><br>
 * <b>Journal: </b> Available
 * <br><br>
 * <b>Product(s):</b> 3D ACIS Modeler
 * <br><br>
* @param this_vertex
 * vertex to be blended.
 * @param bulge
 * bulge factor [0...2].
 * @param bl_contin
 * blend continuity.
 * @param bl_v_prop
 * blend property.
 * @param ao
 * ACIS options.
 **/

DECL_BLND outcome api_set_vblend_auto(
			VERTEX * this_vertex,			// vertex to be blended
			double bulge,				// bulge
			bl_continuity bl_contin = slope_continuous,
			bl_v_property  bl_v_prop = bl_v_unset,
			AcisOptions *ao = NULL // options, version, journal
		);
/**
 * Sets an implicit blend on a vertex.
 * <br><br>
 * <b>Role:</b> This function is similar to @href api_set_vblend_auto, but creates
 * vertex blends that have been marked as autosetback blends using a rolling ball
 * vertex blend whenever possible.
 * <br><br>
 * An implicit blend is attached as an attribute of the given vertex. The bulge may
 * be given a value between zero and two (one is usual). A larger value increases
 * the fullness of the blend.
 * <br><br>
 * Setbacks are computed and applied to each edge of the vertex. Setback values
 * depend on the geometry of the body local to the blended vertex and on sizes of
 * blends already assigned to edges ending in the vertex. The caller should ensure
 * that blend attributes are set on the edges of the vertex before this API is
 * called. Setbacks on edges blended with blends of zero size are left unchanged.
 * <br><br>
 * The details of the blend face are found from the radius and setbacks of the
 * edges that end in the vertex.
 * <br><br>
 * The <tt>bl_continuity</tt> is always set to <tt>slope_continuous</tt>.
 * <br><br>
 * The blend property can be set to:
 * <br><br>
 * <table width = 100%>
 * <tr width = 100%>
 * <td valign = "top" width = 20%><tt>bl_v_cap</tt></td>
 * <td valign = "top">means a cap or miter at a vertex joined to one or two blended
 * edges (same as default behavior with no attribute on the vertex at the end of a
 * blend sequence).</td>
 * </tr>
 * <tr>
 * <td valign = "top"><tt>bl_v_roll_on</tt></td>
 * <td valign = "top">means close off the open end or continue on to the next blended
 * edge at the vertex using edge-face or edge-edge blends.</td>
 * </tr>
 * <tr>
 * <td valign = "top"><tt>bl_v_runout</tt></td>
 * <td valign = "top">means close off the open end using a variable-radius blend.</td>
 * </tr>
 * <tr>
 * <td valign = "top"><tt>bl_v_unset</tt></td>
 * <td valign = "top">is a vertex blend.</td>
 * </tr>
 * <tr>
 * <td valign = "top"><tt>bl_v_blend</tt></td>
 * <td valign = "top">is a vertex blend.</td>
 * </tr>
 * </table>
 * <br>
 * <dl>
 * <b>Errors:</b> Pointer to vertex <tt>NULL</tt> or not to a <tt>VERTEX</tt>.
 * <dd>&nbsp;Bulge factor less than 0 or greater than 2.0.</dd>
 * </dl>
 * <br>
 * <b>Effect:</b> Changes model
 * <br><br>
 * <b>Journal: </b> Available
 * <br><br>
 * <b>Product(s):</b> 3D ACIS Modeler
 * <br><br>
* @param this_vertex
 * entities.
 * @param bulge
 * bulge factor [0...2].
 * @param bl_contin
 * reserved for future enhancement.
 * @param bl_v_prop
 * blend property.
 * @param ao
 * ACIS options.
 **/
DECL_BLND outcome api_set_vblend_autoblend(
			VERTEX *this_vertex,			// vertex to be blended
			double bulge,				// bulge
			bl_continuity bl_contin = slope_continuous,
			bl_v_property bl_v_prop = bl_v_unset,
			AcisOptions *ao = NULL // options, version, journal
		);
/**
 * Gets a sequence of connected edges for the given edge.
 * <b>Technical Articles:</b> <i>[Blending](http://doc.spatial.com/articles/b/l/e/Component~Blending_c382.html)</i>, 
 * <i>[Blend Attributes](http://doc.spatial.com/articles/b/l/e/Blend_Attributes_568e.html)</i>
 * <br><br>
 * <b>Role:</b> Gets a sequence of connected edges for the given edge that meet smoothly;
 * i.e., with tangent continuity.
 * <br><br>
 * <b>Errors:</b> Pointer to edge is <tt>NULL</tt> or not to an <tt>EDGE</tt>.
 * <br><br>
 * <b>Limitations:</b> It is assumed that the input edge(s) define natural or trimming
 * edges participating in loops of faces in a solid or sheet model. The code
 * explicitly assumes that the coedges have non-<tt>NULL</tt> partner and owner pointers. A
 * segmentation error occurs if one passes in edges that only participate in a pure
 * wire body.
 * <br><br>
 * <b>Effect:</b> Read-only
 * <br><br>
 * <b>Journal: </b> Available
 * <br><br>
 * <b>Product(s):</b> 3D ACIS Modeler
 * <br><br>
* @param this_edge
 * an edge in the sequence.
 * @param edge_list
 * returned the list of connected edges found.
 * @param ao
 * ACIS options.
 **/
DECL_BLND outcome api_smooth_edge_seq(
			EDGE *this_edge,				// an edge in the sequence
			ENTITY_LIST &edge_list,		// list of connected edges found
			AcisOptions *ao = NULL // options, version, journal
		);
/**
 * Generates a calibration %curve fit to a list of edges.
 * <br><br>
 * <b>Role:</b> This API makes a single curve that approximates the given list
 * of smoothly-connected edges. The list need not be sorted upon input. The curve
 * and pointers to the edges corresponding to the start and end of the curve are
 * returned. The curve is returned as a new <tt>CURVE</tt> with a use count of 1, and must
 * be disposed of properly. <tt>first_edge</tt> and <tt>last_edge</tt> are pointers directly into the
 * body. This API function is used in blending, for example to find a defining curve for a
 * variable-radius blend sequence, which parameterizes the blend surface and
 * calibrates the radius function. Typical usage would be:
 * <pre>
 * CURVE *calibration_curve;
 * EDGE *first_edge;
 * EDGE *last_edge;
 * api_smooth_edges_to_curve(ents, calibration_curve, first_edge, last_edge); <br>
 * result = api_set_var_blends(ents, start_radius, end_radius, start_setback, end_setback,
 *                             calibration_curve, first_edge, last_edge, 0, 0, TRUE, TRUE, start_stop_ang, end_stop_ang); <br>
 * calibration_CURVE->remove(); 
 * </pre>
 * <b>Effect:</b> Read-only
 * <br><br>
 * <b>Journal: </b> Available
 * <br><br>
 * <b>Product(s):</b> 3D ACIS Modeler
 * <br><br>
* @param ents
 * entities.
 * @param calibration_curve
 * returned calibration curve.
 * @param first_edge
 * returned the first edge on the curve.
 * @param last_edge
 * returned the last edge on the curve.
 * @param ao
 * ACIS options.
 **/
DECL_BLND outcome api_smooth_edges_to_curve(
	ENTITY_LIST const &ents,
	CURVE *& calibration_curve,
	EDGE *& first_edge,
	EDGE *& last_edge,
	AcisOptions *ao = NULL // options, version, journal
	);

// Blend EDGES of a solid using round blends
/**
 * Blends edges of a solid using round blends.
 * <br><br>
 * <b>Role:</b> This API creates round blends along one or more edges based on
 * the given blend radius. The blend radius is specified in world coordinates.
 * If all edges that meet at a specific vertex are in the entity list, the
 * vertex is also blended. If a vertex is marked as rounded, the details of the
 * blend face are found from the radius of the edges that end in the vertex.
 * <br><br>
 * If there are features lying inside the support region that is replaced 
 * by blend, then blending attempts to retain those features. This behavior 
 * can be controlled by the option <tt>bl_retain_features</tt>. However, if blending 
 * fails to retain such features, then it will continue to give a result 
 * that it is ignoring them. A <tt>sys_warning</tt> is raised when blending detects features 
 * but is unable to retain them.
 * <br><br>
 * <b>Effect:</b> Changes model
 * <br><br>
 * <b>Journal: </b> Available
 * <br><br>
 * <b>Product(s):</b> 3D ACIS Modeler
 * <br><br>
* @param edges
 * list of edges to blend.
 * @param radius
 * blend radius.
 * @param ao
 * ACIS options.
 **/
DECL_BLND outcome api_blend_edges(
						ENTITY_LIST const &edges, // list of EDGES to blend
						double			radius,	  // blend radius
						AcisOptions *ao = NULL    // options, version, journal
						);

// Chamfer EDGES of a solid
/**
 * Chamfers the edges of a solid.
 * <br><br>
 * <b>Role:</b> This API creates chamfers with linear cross section on the given edges with
 * the given left and right ranges, where left and right are with respect to the
 * edge direction. Ranges are always positive; if the given right range is negative,
 * a range equal to the left range is used. For a chamfer with equal left and
 * right ranges, the points where the chamfer surfaces meet the faces of the
 * chamfered edge are the same as those for a round with radius equal to the
 * chamfer range. If the ranges for left and right differ, the meeting points on
 * the left face are the same as those for a round with radius equal to the left
 * range, and the meeting points on the right face are the same as those for a
 * round with radius equal to the right range.
 * <br><br>
 * If there are features lying inside the support region that is replaced 
 * by blend, then blending attempts to retain those features. This behavior 
 * can be controlled by the option <tt>bl_retain_features</tt>. However, if blending 
 * fails to retain such features, then it will continue to give a result 
 * that it is ignoring them. A <tt>sys_warning</tt> is raised when blending detects features 
 * but is unable to retain them.
 * <br><br>
 * <b>Effect:</b> Changes model
 * <br><br>
 * <b>Journal: </b> Available
 * <br><br>
 * <b>Product(s):</b> 3D ACIS Modeler
 * <br><br>
* @param edges
 * edges to chamfer.
 * @param left_range
 * left range of chamfer.
 * @param right_range
 * if negative, it's equal to left_range.
 * @param ao
 * ACIS options.
 **/
DECL_BLND outcome api_chamfer_edges(
						ENTITY_LIST const &edges,	// list of EDGES to chamfer
						double left_range,			// left range of the chamfer
						double right_range = -1,	// if negative, it's equal to the left range
						AcisOptions *ao = NULL		// options, version, journal
						);

// Chamfer VERTEXs of a solid; each vertex must have exactly 3 adjacent edges
/**
 * Creates planar chamfers on vertices of a solid body.
 * <br><br>
 * <b>Role:</b> This API creates flat (planar) chamfers on the given vertices.
 * The first argument, vertices, specifies a list of solid <tt>VERTEX</tt> entities to be
 * chamfered. The second argument, offset, specifies the chamfer offset, equal for
 * all edges (if any) of all vertices.
 * <br><br>
 * If any edge touches a vertex, the offset specifies either the distance along the
 * %straight line between the vertex and the point on the edge (if <tt>distanceStraight</tt>
 * is <tt>TRUE</tt>, which is its default value) or the distance along the edge (if
 * <tt>distanceStraight</tt> is <tt>FALSE</tt>). The chamfer plane is then placed so that it passes
 * through all these points. If the edges are straight, <tt>distanceStraight</tt> does not
 * matter.
 * <br><br>
 * See @href api_chamfer_vertex for further details. The following code snippet
 * illustrates a way to use this API.
 * <pre>
 * const double H = 20.0; // block size
 * const double C =  H  ; // primary chamfers
 * const double c =  H/2; // secondary chamfers <br>
 * BODY* block = NULL; <br>
 * // Create a cube
 * check_outcome( result = api_make_cuboid( 2*H,2*H,2*H, block ) ); <br>
 * // Get all original vertices
 * ENTITY_LIST    vertices;
 * check_outcome( result = api_get_vertices( block, vertices ) ); <br>
 * // Chamfer the original vertices
 * check_outcome( result = api_chamfer_vertices( vertices, C ) ); <br>
 * // Get all the secondary vertices
 * vertices.clear();
 * check_outcome( result = api_get_vertices( block, vertices ) ); <br>
 * // Chamfer the secondary vertices
 * check_outcome( result = api_chamfer_vertices( vertices, c ) ); 
 * </pre>
 * <b>Limitations:</b> The offset cannot exceed the length of any edge adjacent to a
 * vertex, and vertices with three edges of mixed convexity may not always chamfer
 * successfully.
 * <br><br>
 * When the offset is positive, it may fail to detect concave vertices with edges
 * of mixed convexity. In this case, use a negative offset value to explicitly
 * specify concave vertices.
 * <br><br>
 * When the offset is so large the chamfer %plane should intersect the faces not
 * containing the vertex, chamfering is not performed and an error message is
 * returned.
 * <br><br>
 * <b>Effect:</b> Changes model
 * <br><br>
 * <b>Journal: </b> Available
 * <br><br>
 * <b>Product(s):</b> 3D ACIS Modeler
 * <br><br>
* @param vertices
 * list of vertices.
 * @param offset
 * chamfer offset.
 * @param distanceStraight
 * along the %straight line or the non-straight edge.
 * @param ao
 * ACIS options.
 **/
DECL_BLND outcome api_chamfer_vertices(
            ENTITY_LIST& vertices,					// list of VERTEXs to chamfer
            double       offset  ,					// offset (equal for all edges of all vertices)
            logical      distanceStraight = TRUE,	// measure offset along the straight line
			AcisOptions *ao = NULL // options, version, journal
        );


// Chamfer one VERTEX of a solid, which has exactly 3 adjacent edges
/**
 * Creates a planar chamfer on a vertex of a solid body.
 * <br><br>
 * <b>Role:</b> This API creates a planar chamfer on a vertex of a solid body.
 * <br><br>
 * There are two distinct forms of the API. One is when offsets are the distances
 * along the edges, and additional edge and offset pairs are specifically defined
 * (and normal is not defined). In the second form, the chamfer %plane is
 * constructed to be perpendicular to the normal vector and the position of the
 * plane is determined by offsetting it from the vertex along the normal. In this
 * case, normal must be specified and no edges or additional offsets are defined.
 * <br><br>
 * In both forms, <tt>offset1</tt> can be defined as a negative value to create a concave
 * chamfer (when material is added to form a chamfer face). Conversely, a positive
 * <tt>offset1</tt> value provides no information about the chamfering type. For example, a
 * positive <tt>offset1</tt> can be used for removing material from convex vertices and
 * adding material to concave ones. Therefore, ACIS must detect the chamfering type.
 * When the vertex does not have adjacent edges, which is only possible with cones
 * and vortex tori, ACIS analyzes the face sense. For vertices with adjacent edges,
 * convexity of the edges is used to predict the chamfering type. If such a
 * prediction is impossible (e.g., a vertex with mixed-convexity edges) and <tt>offset1</tt>
 * is positive, a convex chamfer is assumed.
 * <br><br>
 * In the first form of this API, if the vertex is the apex of a cone, only the
 * first two arguments are used: a pointer to the vertex and the offset value. In
 * this case, the <tt>offset1</tt> specifies the radius of the sphere with the center at the
 * vertex, which intersects the cone to obtain the intersection %curve. This curve
 * is then used to find the location of the chamfer %plane (the chamfer %plane passes through
 * the intersection %curve).
 * <br><br>
 * If any edge touches the vertex, corresponding offsets specify either the
 * distance along the %straight line between the vertex and the point on the edge
 * (if <tt>distanceStraight</tt> is <tt>TRUE</tt>, which is its default value) or the distance along
 * the edge (if <tt>distanceStraight</tt> is <tt>FALSE</tt>). The chamfer %plane is then placed so
 * that it passes through all these points. If the edges are straight,
 * <tt>distanceStraight</tt> does not matter.
 * <br><br>
 * The minimum number of required arguments is two: only the vertex and offset have
 * always to be specified. If the vertex is touched by several edges and only one
 * offset is specified, equal offsets are assumed for all edges.
 * <br><br>
 * If there are more than three edges touching the vertex and not all of the edge
 * pointers (<tt>edge1</tt>, <tt>edge2</tt>, and <tt>edge3</tt>) are specified in the argument list, all
 * offsets are assumed equal to <tt>offset1</tt> even if <tt>offset2</tt> and offset3 were specified,
 * and the three "best" edges are selected according to the following algorithm:
 * first, points are placed on all edges at offset1 from the vertex, and then three
 * such edges are selected so that they all have the same convexity (at the points
 * location) and whose points make the triangle with the largest perimeter.
 * <br><br>
 * In the form of this API with the normal specified, <tt>offset1</tt> means the offset of
 * the chamfer %plane from the vertex along the normal. This form allows for tilted
 * chamfers on vertices with no or one adjacent edge. If the normal supplied is a
 * zero vector, the face normal at the vertex is used.
 * <br><br>
 * The following code snippet illustrates a way to use this API.
 * <pre>
 * const double R = 30.0;   // sphere radius
 * const double r = R/1000; // ray radius <br>
 * BODY* block = NULL;
 * double* ray_params = NULL; <br>
 * // Create a quarter-sphere
 * check_outcome( result = api_make_sphere( R, block ) );
 * BODY* tool;
 * check_outcome( result = api_make_cuboid ( R, R, R, tool ) );
 * check_outcome( result = api_apply_transf( tool, translate_transf( vector( R/2,R/2,R/2 ))) );
 * check_outcome( result = api_intersect( tool, block) ); <br>
 * // Pick a vertex to chamfer
 * ENTITY_LIST vertices;
 * check_outcome( result = api_get_ents( SPAposition(R,0,0),SPAunit_vector(1,0,0),r, VERTEX_TYPE,block, vertices, ray_params ) );
 * ACIS_DELETE [] ray_params; <br>
 * // Pick edges to pass to api_chamfer_vertex
 * ENTITY_LIST edges;
 * check_outcome( result = api_get_ents( SPAposition(R/2,0,0),SPAunit_vector(0,0,-1),r, EDGE_TYPE,block, edges, ray_params ) );
 * ACIS_DELETE [] ray_params;
 * check_outcome( result = api_get_ents( SPAposition(R/2,0,R/4),SPAunit_vector(1,0,0),r, EDGE_TYPE,block, edges, ray_params ) );
 * ACIS_DELETE [] ray_params;
 * check_outcome( result = api_get_ents( SPAposition(R/2,R/2,0),SPAunit_vector(1,0,0),r, EDGE_TYPE,block, edges, ray_params ) );
 * ACIS_DELETE [] ray_params; <br>
 * // Chamfer the vertex
 * if( vertices.count()==1  &&  edges.count()==3 )
 * check_outcome( result = api_chamfer_vertex((VERTEX*)vertices[0], R/2, (EDGE*)edges[0], R*M_PI/4, (EDGE*)edges[1], R*M_PI/4, (EDGE*)edges[2], FALSE ) ); 
 * </pre>
 * <b>Limitations:</b> If normal is not defined, the offset along any edge cannot
 * exceed the length of that edge, so a vertex with three edges of mixed convexity
 * may not always chamfer successfully. When normal is specified and <tt>offset1</tt> is
 * positive, it may fail to detect a concave vertex with edges of mixed convexity.
 * In this case, use a negative <tt>offset1</tt> value to explicitly specify a concave
 * vertex. When any offset is so large the chamfer %plane should intersect the faces
 * not containing the vertex, chamfering is not performed and the "chamfer case not
 * implemented" error message is returned.
 * <br><br>
 * <b>Effect:</b> Changes model
 * <br><br>
 * <b>Journal: </b> Available
 * <br><br>
 * <b>Product(s):</b> 3D ACIS Modeler
 * <br><br>
* @param vertex
 * vertex to chamfer.
 * @param offset1
 * offset along first edge.
 * @param edge1
 * first edge.
 * @param offset2
 * offset along second edge.
 * @param edge2
 * second edge.
 * @param offset3
 * offset along third edge.
 * @param edge3
 * third edge.
 * @param distanceStraight
 * measure offset along the %straight line.
 * @param ao
 * ACIS options.
 **/
DECL_BLND outcome api_chamfer_vertex(
            VERTEX*  vertex,				  // the VERTEX to chamfer
            double   offset1,				  // offset along the first edge
            EDGE  *  edge1 = NULL,			  // first edge
            double   offset2 = -1,			  // offset along the second edge
            EDGE  *  edge2 = NULL,			  // second edge
            double   offset3 = -1,			  // offset along the third edge
            EDGE  *  edge3 = NULL,			  // third edge
            logical  distanceStraight = TRUE, // measure offset along the straight line
			AcisOptions *ao = NULL // options, version, journal
        );



/**
 * Creates a planar chamfer on a vertex of a solid body.
 * <br><br>
 * <b>Role:</b> This API creates a planar chamfer on a vertex of a solid body.
 * <br><br>
 * There are two distinct forms of the API. One is when offsets are the distances
 * along the edges, and additional edge and offset pairs are specifically defined
 * (and normal is not defined). In the second form, the chamfer %plane is
 * constructed to be perpendicular to the normal vector and the position of the
 * plane is determined by offsetting it from the vertex along the normal. In this
 * case, normal must be specified and no edges or additional offsets are defined.
 * <br><br>
 * In both forms, <tt>offset1</tt> can be defined as a negative value to create a concave
 * chamfer (when material is added to form a chamfer face). Conversely, a positive
 * <tt>offset1</tt> value provides no information about the chamfering type. For example, a
 * positive <tt>offset1</tt> can be used for removing material from convex vertices and
 * adding material to concave ones. Therefore, ACIS must detect the chamfering type.
 * When the vertex does not have adjacent edges, which is only possible with cones
 * and vortex tori, ACIS analyzes the face sense. For vertices with adjacent edges,
 * convexity of the edges is used to predict the chamfering type. If such a
 * prediction is impossible (e.g., a vertex with mixed-convexity edges) and <tt>offset1</tt>
 * is positive, a convex chamfer is assumed.
 * <br><br>
 * In the first form of this API, if the vertex is the apex of a cone, only the
 * first two arguments are used: a pointer to the vertex and the offset value. In
 * this case, the <tt>offset1</tt> specifies the radius of the sphere with the center at the
 * vertex, which intersects the cone to obtain the intersection %curve. This curve
 * is then used to find the location of the chamfer %plane (the chamfer %plane passes through
 * the intersection %curve).
 * <br><br>
 * If any edge touches the vertex, corresponding offsets specify either the
 * distance along the %straight line between the vertex and the point on the edge
 * (if <tt>distanceStraight</tt> is <tt>TRUE</tt>, which is its default value) or the distance along
 * the edge (if <tt>distanceStraight</tt> is <tt>FALSE</tt>). The chamfer %plane is then placed so
 * that it passes through all these points. If the edges are straight,
 * <tt>distanceStraight</tt> does not matter.
 * <br><br>
 * The minimum number of required arguments is two: only the vertex and offset have
 * always to be specified. If the vertex is touched by several edges and only one
 * offset is specified, equal offsets are assumed for all edges.
 * <br><br>
 * If there are more than three edges touching the vertex and not all of the edge
 * pointers (<tt>edge1</tt>, <tt>edge2</tt>, and <tt>edge3</tt>) are specified in the argument list, all
 * offsets are assumed equal to <tt>offset1</tt> even if <tt>offset2</tt> and offset3 were specified,
 * and the three "best" edges are selected according to the following algorithm:
 * first, points are placed on all edges at offset1 from the vertex, and then three
 * such edges are selected so that they all have the same convexity (at the points
 * location) and whose points make the triangle with the largest perimeter.
 * <br><br>
 * In the form of this API with the normal specified, <tt>offset1</tt> means the offset of
 * the chamfer %plane from the vertex along the normal. This form allows for tilted
 * chamfers on vertices with no or one adjacent edge. If the normal supplied is a
 * zero vector, the face normal at the vertex is used.
 * <br><br>
 * The following code snippet illustrates a way to use this API.
 * <pre>
 * const double R = 30.0;   // sphere radius
  * const double r = R/1000; // ray radius <br>
  * BODY* block = NULL;
  * double* ray_params = NULL; <br>
  * // Create a quarter-sphere
  * check_outcome( result = api_make_sphere( R, block ) );
  * BODY* tool;
  * check_outcome( result = api_make_cuboid ( R, R, R, tool ) );
  * check_outcome( result = api_apply_transf( tool, translate_transf( vector( R/2,R/2,R/2 ))) );
  * check_outcome( result = api_intersect( tool, block) ); <br>
  * // Pick a vertex to chamfer
  * ENTITY_LIST vertices;
  * check_outcome( result = api_get_ents( SPAposition(R,0,0),SPAunit_vector(1,0,0),r, VERTEX_TYPE,block, vertices, ray_params ) );
  * ACIS_DELETE [] ray_params; <br>
  * // Pick edges to pass to api_chamfer_vertex
  * ENTITY_LIST edges;
  * check_outcome( result = api_get_ents( SPAposition(R/2,0,0),SPAunit_vector(0,0,-1),r, EDGE_TYPE,block, edges, ray_params ) );
  * ACIS_DELETE [] ray_params;
  * check_outcome( result = api_get_ents( SPAposition(R/2,0,R/4),SPAunit_vector(1,0,0),r, EDGE_TYPE,block, edges, ray_params ) );
  * ACIS_DELETE [] ray_params;
  * check_outcome( result = api_get_ents( SPAposition(R/2,R/2,0),SPAunit_vector(1,0,0),r, EDGE_TYPE,block, edges, ray_params ) );
  * ACIS_DELETE [] ray_params; <br>
  * // Chamfer the vertex
  * if( vertices.count()==1  &&  edges.count()==3 )
 * check_outcome( result = api_chamfer_vertex((VERTEX*)vertices[0], R/2, (EDGE*)edges[0], R*M_PI/4, (EDGE*)edges[1], R*M_PI/4, (EDGE*)edges[2], FALSE ) ); 
 * </pre>
 * <b>Limitations:</b> If normal is not defined, the offset along any edge cannot
 * exceed the length of that edge, so a vertex with three edges of mixed convexity
 * may not always chamfer successfully. When normal is specified and <tt>offset1</tt> is
 * positive, it may fail to detect a concave vertex with edges of mixed convexity.
 * In this case, use a negative <tt>offset1</tt> value to explicitly specify a concave
 * vertex. When any offset is so large the chamfer %plane should intersect the faces
 * not containing the vertex, chamfering is not performed and the "chamfer case not
 * implemented" error message is returned.
 * <br><br>
 * <b>Effect:</b> Changes model
 * <br><br>
 * <b>Journal: </b> Available
 * <br><br>
 * <b>Product(s):</b> 3D ACIS Modeler
 * <br><br>
* @param vertex
 * vertex to chamfer.
 * @param offset
 * offset along the normal.
 * @param normal
 * normal of the chamfer %plane.
 * @param ao
 * ACIS options.
 **/
DECL_BLND outcome api_chamfer_vertex(
            VERTEX*			vertex,     // the VERTEX to chamfer
            double			offset,     // offset along the normal
            SPAvector			normal,     // normal of the future "chamfer" plane (face normal if zero SPAvector)
			AcisOptions* ao = NULL // options, version, journal
        );
/**
 * Creates planar chamfers on vertices of a solid body.
 * <br><br>
 * <b>Role:</b> This API creates a flat on the given faces of a solid body. The
 * first argument, <tt>ptPosition</tt>, specifies a position in space which may lie on the
 * faces to be modified. It must lie on the faces if faces are not given, or may
 * not belong to any face. The offset, which may be positive or negative,
 * determines the offset of a chamfer %plane along its normal. The normal may be
 * given as the third argument. If the normal of the chamfer %plane is not specified,
 * it is found as an outward normal of the faces being chamfered at the position
 * nearest to <tt>ptPosition</tt>.
 * <br><br>
 * If the normal is not specified, it is deduced from the other supplied parameters:
 * either faces or owner must be specified. If a body, owner, is specified, then
 * <tt>ptPosition</tt> must lie on one or more faces of the body. A point lying on multiple
 * faces belongs to the intersection of these faces. A list of faces to be
 * chamfered is then constructed. The normal is computed by averaging the
 * corresponding outward normals of each face in the faces array.
 * <br><br>
 * The sign of the second argument, <tt>offset</tt>, indicates whether the flat is to be
 * made by deleting the material (negative offset) from a convex face or by adding
 * the material to the concave face (positive offset). Note that this sign
 * convention is different from the one used in @href api_chamfer_vertex.
 * <br><br>
 * If the intersection of the chamfer %plane with the faces produces multiple
 * intersection curves, the curve closest to the projection of the <tt>ptPosition</tt> on
 * the chamfer %plane is used.
 * <br><br>
 * The following code snippet demonstrates a way to use this API.
 * <pre>
 * const double h = 3.0;   // base height
 * BODY* b = NULL; <br>
 * // Create a plate and apply waves on top of it
 * check_outcome(result = api_make_cuboid(4*M_PI,4*M_PI,2*h, b));
 * check_outcome(result = api_apply_transf( b, translate_transf( vector( 1.5*M_PI, 1.5*M_PI, 0 ))) );
 * law* sins = NULL;
 * check_outcome( result = api_str_to_law( "vec(x,y,z+sin(x)+sin(y))", &sins) );
 * check_outcome( result = api_space_warp( b, sins ) );
 * if( sins ) sins->remove(); <br>
 * BODY* t;
 * check_outcome( result = api_make_cuboid(4*M_PI,4*M_PI,3*h, t));
 * check_outcome( result = api_apply_transf( t, translate_transf( vector( 1.5*M_PI, 1.5*M_PI, -1.5*h ))) );
 * check_outcome( result = api_subtract( t, b) ); <br>
 * // Put flats on the mountains (negative offset => convex)
 * check_outcome( result = api_make_flat_on_faces( SPAposition(2.5*M_PI, 2.5*M_PI, h+2), -1.95, (SPAunit_vector*)NULL_REF, (FACE**)NULL_REF, 0, b ) );
 * SPAunit_vector uv1(-.5,-.5,1);
 * check_outcome( result = api_make_flat_on_faces( SPAposition(0.5*M_PI, 0.5*M_PI, h+2), -0.25, &uv1, (FACE**)NULL_REF, 0, b ) ); <br>
 * double* ray_params = NULL; ENTITY_LIST faces;
 * check_outcome( result = api_get_ents( SPAposition(0,0,(h-2)/2),SPAunit_vector(0,0,1),0.01, FACE_TYPE, b, faces, ray_params ) );
 * ACIS_DELETE [] ray_params;
 * FACE* top_face = (FACE*) faces[0];
 * SPAunit_vector uv2(0.2, 0.3, 1);
 * check_outcome( result = api_make_flat_on_faces( SPAposition(1.5*M_PI, 1.5*M_PI, 1), 1, &uv2, &top_face, 1 ) ); <br>
 * // This last call creates a pseudo-chamfer at one of the side
 * // waves. There are no vertices--we can't use api_chamfer_vertex.
 * SPAunit_vector uv3(0.3,-2, 1);
 * check_outcome( result = api_make_flat_on_faces( SPAposition(M_PI/2, -M_PI/2, h), -0.3, &uv3, (FACE**)NULL_REF, 1, b ) );
 * </pre>
 * <b>Errors:</b> When a slant flat is placed at the (concave) center of a vortex torus,
 * the erroneous situation of too large an offset and/or tilt is not caught and a
 * bad body is created.
 * <br><br>
 * <b>Limitations:</b> Making the maximum possible flat at the the center of a vortex
 * %torus (when the chamfer %plane just touches the %torus), i.e., completely filling
 * the toroid indentation, is not possible. Instead, use @href api_chamfer_vertex with
 * the central vertices of the vortex %torus.
 * <br><br>
 * <b>Effect:</b> Changes model
 * <br><br>
 * <b>Journal: </b> Available
 * <br><br>
 * <b>Product(s):</b> 3D ACIS Modeler
 * <br><br>
* @param ptPosition
 * position.
 * @param offset
 * offset along normal.
 * @param normal
 * normal of the future chamfer %plane.
 * @param faces
 * faces to chamfer.
 * @param n_faces
 * number of faces in the array.
 * @param owner
 * body owning the faces.
 * @param ao
 * ACIS options.
 **/
DECL_BLND outcome api_make_flat_on_faces(
            SPAposition     ptPosition,    // must lie on the face(s) if faces==NULL
            double       offset,        // along the normal
            SPAunit_vector* normal = NULL, // normal of the future "chamfer" plane (face normal if NULL)
            FACE      ** faces  = NULL, // faces to "chamfer" (if NULL, search for the faces containing ptPosition)
            int        n_faces  = 1,    // number of faces in the array
            BODY*		 owner  = NULL,  // body owning the faces (only necessary if faces==NULL)
			AcisOptions * ao = NULL // options, version, journal
        );

// STIBUGFIX billd enhancement: variable radius blends: begin:
/**
 * Sets an implicit variable radius blend on each of a list of edges or vertices.
 * <b>Technical Articles:</b> <i>[Blending](http://doc.spatial.com/articles/b/l/e/Component~Blending_c382.html)</i>, 
 * <i>[Blend Attributes](http://doc.spatial.com/articles/b/l/e/Blend_Attributes_568e.html)</i>
 * <br><br>
 * <b>Role:</b> Implicit variable-radius blends are attached as attributes to
 * each of the edges or vertices in the list.
 * <br><br>
 * Setback at an end of an edge Determines where the blend is to be stopped short
 * of the vertex at the edge end. It is only significant when the vertex is blended.
 * <br><br>
 * The blend radii and left and right setbacks must be supplied in the local space
 * of the body.
 * <br><br>
 * If start stop angle or end stop angle values are specified, the edge blend is stopped and capped 
 * with a stop %plane at given setback distance. This stop %plane is constructed such that the angle 
 * between stop %plane and edge direction, about the line passing through end points of blend 
 * cross section, is equal to the specified stop angle value at that end. 
 * Stop angle value of 180 degrees is treated as a special case. In such a case, stop %plane is 
 * constructed such that it passes through end vertex and end points of blend cross section 
 * at given setback distance.
 * <br><br>
 * If there are features lying inside the support region that is replaced 
 * by blend, then blending attempts to retain those features. This behavior 
 * can be controlled by the option <tt>bl_retain_features</tt>. However, if blending 
 * fails to retain such features, then it will continue to give a result 
 * that it is ignoring them. A <tt>sys_warning</tt> is raised when blending detects features 
 * but is unable to retain them.
 * <br><br>
 * <b>Limitations:</b> Entity-entity blends cannot create a stopped blend.
 * The stopping location is based upon the edge being blended, thus entity-entity blends do not have
 * this defined location.
 * <br><br>
 * <b>Effect:</b> Changes model
 * <br><br>
 * <b>Journal: </b> Available
 * <br><br>
 * <b>Product(s):</b> 3D ACIS Modeler
 * <br><br>
* @param entities
 * entities to be blended.
 * @param start_radius
 * start blend radius.
 * @param end_radius
 * end blend radius.
 * @param start_setback
 * start setback.
 * @param end_setback
 * end setback.
 * @param calibration_curve
 * calibration curve.
 * @param first_edge
 * first edge in smooth sequence.
 * @param last_edge
 * last edge in smooth sequence.
 * @param start_sbdiff
 * setback difference (r - l) at start.
 * @param end_sbdiff
 * setback difference (r - l) at end.
 * @param start_sbdiff_set
 * setback difference set at start.
 * @param end_sbdiff_set
 * setback difference set at end.
 * @param start_stop_ang
 * start stop angle.
 * @param end_stop_ang
 * end stop angle.
 * @param ao
 * ACIS options.
 **/
DECL_BLND outcome api_set_var_blends(
			ENTITY_LIST const & entities,	// edges to be rounded
			double start_radius,             	// start radius,
			double end_radius,             	// end radius,
			double start_setback,             	// start setback
			double end_setback,              	// end setback
			CURVE * calibration_curve = NULL,		// calibration curve
			EDGE * first_edge = NULL,		// first edge in smooth sequence
			EDGE *last_edge = NULL,		// last edge in smooth sequence
			double start_sbdiff = 0,			// setback difference (r - l) at start
			double end_sbdiff  = 0,			// setback difference (r - l) at end
			logical start_sbdiff_set  = TRUE,		// setback difference set at start
			logical end_sbdiff_set = TRUE,		// setback difference set at end
			double start_stop_ang = 0,			// start stop angle
			double end_stop_ang = 0,			// end stop angle
			AcisOptions *ao = NULL // options, version, journal
	);
// STIBUGFIX billd enhancement: variable radius blends: end.

// Create a rather basic blend faces as "previews" for the given blend
// attributes. The sheet body is created if NULL, otherwise the new
// FACEs are added to it. The final entity list contains attributes that
// should have been previewable but couldn't be for some reason.
// Currently works only for rounds, constant and variable. Also respects
// setbacks at the ends of edges. This function is extremely fast
// compared to *really* doing the blend, but gives a fairly rough and
// ready approximation.

// An exact spline surface is constructed for each blend. In all cases
// the v parametrisation is the along-the-blend one, and the u
// parametrisation runs from 0 on one face to 1 on the other one.

/**
 * Creates a "preview body" containing faces which allow a rough preview of one or more blends.
 * <br><br>
 * <b>Role:</b> A single 4-sided face is made up for every edge blend attribute.
 * Vertex blends, entity-entity blends etc. are ignored. However, any setbacks on
 * an edge are respected, so the effect of setbacks on the adjacent edges can be
 * inspected.
 * <br><br>
 * The body returned (or faces inserted into it) do NOT form a true valid ACIS body.
 * Each face has a single loop of 4 coedges, with a surface which fits exactly
 * into the face. Every surface is an exact B-spline and the ''v'' parameterization is
 * ALONG the blend, matching the edge parameterization, and the ''u'' parameter runs
 * across from 0.0 on the left face to 1.0 on the right. There is NO topological
 * connectivity between faces, indeed they only meet "very approximately". The
 * surfaces may in actual fact contain self-intersections and the like, and should
 * NEVER be handed on to intersectors, silhouette calculations, Booleans and the
 * like. Any algorithms that process these faces MUST be able to cope with any such
 * problems. Note that the ''v'' parameter lines are likely to be problematic for
 * certain algorithms that expect strictly "legal" geometry, although the ''u''
 * parameter lines, taken one at a time, so to speak, should be reasonable.
 * <br><br>
 * ***<tt>THE FACES RETURNED ARE VISUALIZATION AIDS ONLY</tt>***
 * <br><br>
 * The faces only lie "very approximately" on the faces being blended together.
 * Just because a preview face can be constructed DOES NOT MEAN the blend itself
 * will succeed. Actually committing valid topology and geometry to the model is
 * several orders of magnitude harder.
 * <br><br>
 * The algorithms are designed for maximum speed, constructing the preview sheet
 * should be orders of magnitude quicker than calculating the final blend. The
 * preview faces are always dumped into the first shell of the first lump of the
 * body, regardless.
 * <br><br>
 * <b>Errors:</b> Should not produce errors, though you may get no preview faces.
 * <br><br>
 * <b>Limitations:</b> Currently only works for round (constant and variable) blends.
 * Any blend attributes not supported are ignored quietly.
 * <br><br>
 * <b>Effect:</b> Read-only
 * <br><br>
 * <b>Journal: </b> Available
 * <br><br>
 * <b>Product(s):</b> 3D ACIS Modeler
 * <br><br>
* @param blank_body
 * existing body which is being blended.
 * @param sheet_body
 * existing sheet body to dump the preview faces into, or if <tt>NULL</tt>, a new body is made up and returned.
 * @param blend_atts
 * list of blend attributes to be previewed, or alternatively, the entities (<tt>EDGEs</tt> etc.) being blended.
 * @param cannot_preview
 * any blends which the algorithm thought it should have been able to preview, but failed, are put in here.
 * @param ao
 * ACIS options.
 **/

DECL_BLND outcome api_preview_blends(
							 BODY *blank_body,
							 BODY *& sheet_body,
							 ENTITY_LIST const &blend_atts,
							 ENTITY_LIST & cannot_preview,
							 AcisOptions *ao = NULL // options, version, journal
							 );

/** @} */


/** \addtogroup DETECTFEATURE
 *  \brief Declared at <blendapi.hxx>
 *  @{
 */

// This enumerated type encodes the type of blend elements.[being sought in api_detect_blends]
/**
 * Specifies blend element type.
 * @param blend_sequence
 * Collection of contiguous set of blend faces created in a single blending operation
 * @param blend_network
 * Collection of blend sequences connected to each other.
 */
enum blend_element_type 
{
	BLEND_SEQUENCE,		
	BLEND_NETWORK
};

 /**
 * This API detects blend elements on the model based on the seed face. The recognized elements
 * of given blend type are returned as an ENTITY_LIST. The supplied seed face should be a blend.
 * <br><br>
 * <b>Limitations:</b>  
 * Variable radius blends are not detected. 
 * Chamfers are not detected.
 * <br><br>
 * <b>Journal: </b> Available
 * <br><br> 
 * <b>Product(s):</b> 3D ACIS Modeler
 * <br><br>
 * @param seed_blend
 * seed face on the model
 * @param blend_faces
 * list of detected blend faces
 * @param bl_type
 * type of blend collection being sought
 * @param df_opts
 * detect feature options
 */
DECL_BLND outcome api_detect_blends(FACE const * seed_blend, 
								    ENTITY_LIST& blend_faces, 
							        blend_element_type bl_type = BLEND_SEQUENCE,
								    detect_feature_options const * df_opts = NULL
									);


// This enumerated type encodes the type of chamfer elements.[being sought in api_detect_chamfers]
/**
* Specifies chamfer element type.
* @param chamfer_sequence
* Collection of contiguous set of chamfer faces created in a single chamfering operation
* @param chamfer_network
* Collection of chamfer sequences connected to each other.
*/
enum class chamfer_element_type
{
	CHAMFER_SEQUENCE,
	CHAMFER_NETWORK
}; 


/**
* This API detects chamfer elements on the model based on the seed face. The recognized elements
* of given chamfer type are returned as an ENTITY_LIST. The supplied seed face should be a chamfer.
* <br><br>
* <b>Limitations:</b>
* Variable width chamfer are not detected.
* <br><br>
* <b>Journal: </b> Available
* <br><br>
* <b>Product(s):</b> 3D ACIS Modeler
* <br><br>
* @param seed_chamfer
* seed face on the model
* @param chamfer_faces
* list of detected chamfer faces
* @param chamfer_type
* type of chamfer collection being sought
* @param df_opts
* detect feature options
*/
DECL_BLND outcome api_detect_chamfers(FACE const * seed_chamfer,
	ENTITY_LIST& chamfer_faces,
	chamfer_element_type chamfer_type = chamfer_element_type::CHAMFER_SEQUENCE,
	detect_feature_options const * df_opts = NULL
);


/** @} */



#endif
