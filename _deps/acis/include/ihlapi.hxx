/*******************************************************************/
/*    Copyright (c) 1989-2020 by Spatial Corp.                     */
/*    All rights reserved.                                         */
/*    Protected by U.S. Patents 5,257,205; 5,351,196; 6,369,815;   */
/*                              5,982,378; 6,462,738; 6,941,251    */
/*    Protected by European Patents 0503642; 69220263.3            */
/*    Protected by Hong Kong Patent 1008101A                       */
/*******************************************************************/
/**********************************************************************/
/* Declare all API's from the IHL Husk                                */
/**********************************************************************/
#if ! defined ( IHLAPI_INCLUDE )
/**********************************************************************/
#include "logical.h"
#include "dcl_ihl.h"
#include "face.hxx"
/**
 * \defgroup VISMODULE Model Visualization and Display
 */
/**
 * @file ihlapi.hxx
 * @CAA2Level L1
 * @CAA2Usage U1
 * \defgroup IHLAPI Interactive Hidden Line
 *
 * \ingroup VISMODULE
 * @{
 */
class ENTITY_LIST;
class TRANSFORM;
class SPAposition;
class outcome;
class POLYEDGE_MESH;
class IHL_OUTPUT_MANAGER;
class IHL_CAMERA;
class AcisOptions;
class facet_options;
class facet_options_expert;
/**********************************************************************/
/**
 * Initializes the interactive hidden line library.
 * <br>
 * <b>Effect:</b> System routine
 * <br><br>
 * <b>Journal: </b> Not Available
 * <br><br>
 * <b>Product(s):</b> 3D ACIS Modeler, 3D Viz Exchange
 * <br><br>
**/
DECL_IHL outcome api_initialize_interactive_hidden_line( void );
/**
 * Terminates the interactive hidden line library.
 * <br>
 * <b>Effect:</b> System routine
 * <br><br>
 * <b>Journal: </b> Not Available
 * <br><br>
 * <b>Product(s):</b> 3D ACIS Modeler, 3D Viz Exchange
 * <br><br>
**/
DECL_IHL outcome api_terminate_interactive_hidden_line( void );
/**
 * Retrieves interactive hidden line data from attributes attached to the entities (faces or bodies).
 * <br>
 * <b>Role:</b> This API function retrieves the data stored in attributes with a matching
 * view <tt>token</tt> and returns it as an <tt>ENTITY_LIST</tt> of segments. The attributes are
 * placed on the entity by @href api_ihl_compute when the <tt>api_ihl_compute</tt>'s token is 
 * non-zero. Attributes and hidden line data remain attached to entities.
 * <br><br>
 * This API function also returns the camera definition.
 * <br><br>
 * All data returned is a copy of the data stored in attributes on the entity. It
 * is the user's responsibility to call <tt>lose</tt> for the returned <tt>camera</tt> and all
 * entities in the list, when they are no longer needed.
 * <br><br>
 * <b>Errors:</b> The entities in the list did not have matching <tt>IHL_CAMERAs</tt>.
 * <br><br>
 * <b>Effect:</b> Read-only
 * <br><br>
 * <b>Journal: </b> Not Available
 * <br><br>
 * <b>Product(s):</b> 3D ACIS Modeler, 3D Viz Exchange
 * <br><br>
 * @param entities
 * body list.
 * @param token
 * view token.
 * @param segments
 * segment list.
 * @param camera
 * camera.
 **/
DECL_IHL outcome api_ihl_retrieve(
         const ENTITY_LIST& entities,                     // entity list
         int                token,                      // view token
         ENTITY_LIST&       segments,                   // IHL_SEGMENTs
         IHL_CAMERA*&       camera );                   // IHL_CAMERA

/**
 * Removes interactive hidden line data attributes from the model.
 * <br>
 * <b>Role:</b> This API function removes and deletes any IHL attributes with a matching
 * view token from the list of entities.
 * <br><br>
 * When entities are cleaned, attributes are removed and are not stored. These
 * attributes cannot be retrieved using the @href api_ihl_retrieve function.
 * <br><br>
 * The <tt>token</tt> argument is an integer index representing a specific view. 
 * If <tt>token</tt> is 0, no action is taken. A nonzero <tt>token</tt> value
 * causes hidden line data to be removed from attributes attached to
 * the entities for the view matching that view token; attributes for other views
 * are not affected.
 * <br><br>
 * <b>Effect:</b> Changes model
 * <br><br>
 * <b>Journal: </b> Not Available
 * <br><br>
 * <b>Product(s):</b> 3D ACIS Modeler, 3D Viz Exchange
 * <br><br>
* @param entities
 * entity list.
 * @param token
 * view token.
 **/
DECL_IHL outcome api_ihl_clean(
         const ENTITY_LIST& entities,                     // entity list
         int                token );                    // view token

// Facets the given list of ENTITYs
/**
 * Facets each of the given entities and attaches the resulting <tt>POLYEDGE_MESH</tt> to each entity using an attribute.
 * <br>
 * <b>Effect:</b> Changes model
 * <br><br>
 * <b>Journal: </b> Not Available
 * <br><br>
 * <b>Product(s):</b> 3D ACIS Modeler, 3D Viz Exchange
 * <br><br>
* @param entities
 * entities to facet.
 * @param unfacetted
 * if <tt>TRUE</tt>, do not refacet an entity if it is already faceted.
 **/
DECL_IHL outcome api_ihl_facet(
         const ENTITY_LIST&	entities,                  // entity list
		 logical			unfacetted,               // True if you do not want to refacet the entity
		 facet_options *	fo = (facet_options*) NULL_REF );
/**
 * Gets a <tt>POLYEDGE_MESH</tt> for an entity, faceting the entity if necessary first.
 * <br>
 * If an entity does not have an attached <tt>POLYEDGE_MESH</tt>, this API function will
 * generate the mesh and attach it to the entity, before returning a pointer to the attached mesh.
 * <br><br>
 * <b>Effect:</b> Changes model
 * <br><br>
 * <b>Journal: </b> Not Available
 * <br><br>
 * <b>Product(s):</b> 3D ACIS Modeler, 3D Viz Exchange
 * <br><br>
* @param entity
 * entity.
 * @param mesh
 * returned mesh.
 **/
DECL_IHL outcome api_ihl_get_mesh(
         ENTITY		*entity,                   // entity list
		 POLYEDGE_MESH      *&mesh);                   // mesh
/**
 * Computes interactive hidden line data and optionally stores it on the model using attributes.
 * <br>
 * <b>Role:</b> This API function computes the hidden line data for the given viewing
 * parameters and list of entities. The list of entities is run through the faceter
 * to generate a corresponding list of meshes. The meshes are then used to generate
 * hidden line data. The hidden line data consists of 2D line segments (<tt>IHL_SEGMENTs</tt>) returned in
 * the <tt>ENTITY_LIST</tt> <tt>segments</tt>.
 * <br>
 * If the view <tt>token</tt> is nonzero, the data is stored on the model using 
 * attributes and identified by the view <tt>token</tt>. Existing attributes identified by the same
 * <tt>token</tt> are removed. 
 * <br>
 * Five <i>flags</i> are used to control the behavior of this API function.
 * <ul>
 * <li> If the perspective projection flag, <tt>fPersp</tt>, is <tt>TRUE</tt>, data is calculated for 
 *      display in a perspective projection; otherwise, calculations are done for a parallel projection.</li>
 * <li> If the interior segments flag, <tt>fIntrSeg</tt>, is <tt>TRUE</tt>,
 *      segments are generated for edges interior to a face; otherwise they are not.</li>
 * <li> If the hidden segments flag, <tt>fHidSeg</tt>, is <tt>TRUE</tt>, 
 *      segments for hidden lines are generated; otherwise they are not.</li>
 * <li> If the unfacetted entities flag, <tt>unfacetted</tt>, is <tt>TRUE</tt>, 
 *      only unfacetted entities are refaceted.</li>
 * <li> If the no hidden line calculations flag, <tt>fNoHidCalc</tt>, is <tt>TRUE</tt>, 
 *      hidden line calculations are not performed: only silhouette lines are generated.</li>
 * </ul>
 * <br>
 * All data returned is allocated with <tt>new</tt>. It is the user's responsibility to call
 * <tt>lose</tt> for all entities in the list when they are no longer needed.
 * <br><br>
 * <b>Errors:</b> The distance between <tt>eyepos</tt> and <tt>target</tt> is less than <tt>SPAresabs</tt>.
 * <br><br>
 * <b>Effect:</b> Changes model
 * <br><br>
 * <b>Journal: </b> Not Available
 * <br><br>
 * <b>Product(s):</b> 3D ACIS Modeler, 3D Viz Exchange
 * <br><br>
* @param entities
 * entity list.
 * @param token
 * view token.
 * @param eyepos
 * view eye.
 * @param target
 * view target.
 * @param fPersp
 * <tt>TRUE</tt> if perspective projection.
 * @param fIntrSeg
 * <tt>TRUE</tt> if interior segments are needed.
 * @param fHidSeg
 * <tt>TRUE</tt> if hidden segments are needed.
 * @param unfacetted
 * <tt>TRUE</tt> to avoid refaceting entities.
 * @param fNoHidCalc
 * <tt>TRUE</tt> if you do not want to calculate hidden line but only silhouette lines.
 * @param segments
 * segments returned.
 * @param ao
 * AcisOptions
 **/
DECL_IHL outcome api_ihl_compute(
         const ENTITY_LIST& entities,         // entity list
         int                token,          // view token
         const SPAposition&    eyepos,         // eye SPAposition
         const SPAposition&    target,         // target SPAposition
         logical            fPersp,         // TRUE if perspective projection
         logical            fIntrSeg,       // TRUE if interior segments needed
         logical            fHidSeg,        // TRUE if hidden segments needed
		 logical			unfacetted,		// STI mka True if you do not want to refacet the entity
		 logical			fNoHidCalc,	    // STI mka TRUE if you do not want to calc hidden line but only silhouette
         ENTITY_LIST&       segments,     // IHL_SEGMENTs
		 AcisOptions* ao = NULL);
DECL_IHL outcome api_ihl_compute(
         const ENTITY_LIST &	entities,		// entity list
         int					token,          // view token
         const SPAposition &	eyepos,         // eye SPAposition
         const SPAposition &	target,         // target SPAposition
         logical		        fPersp,         // TRUE if perspective projection
         logical		        fIntrSeg,       // TRUE if interior segments needed
         logical		        fHidSeg,        // TRUE if hidden segments needed
		 logical				unfacetted,		// STI mka True if you do not want to refacet the entity
		 logical				fNoHidCalc,	    // STI mka TRUE if you do not want to calc hidden line but only silhouette
         ENTITY_LIST &			segments,		// IHL_SEGMENTs
		 facet_options *		fo,
		 AcisOptions *			ao = NULL);

/**
 * Computes silhouette segments from interactive hidden line data.
 * <br>
 * <b>Role:</b> This API function computes the faceted silhouette segment data for the
 * given viewing parameters and list of entities. The list of entities is processed
 * and then is used to generate faceted silhouette segment data. The hidden line
 * data consists of 3D line segments (<tt>IHL_SEGMENTs</tt>) stored in the <tt>ENTITY_LIST</tt> 
 * <tt>segments</tt>. 
 * <br>
 * Four <i>flags</i> are used to control the behavior of this API function.
 * <ul>
 * <li> If the perspective projection flag, <tt>fPersp</tt>, is <tt>TRUE</tt>, data is calculated for 
 *      display in a perspective projection; otherwise, calculations are done for a parallel projection.</li>
 * <li> If the hidden segments flag, <tt>fHidSeg</tt>, is <tt>TRUE</tt>, 
 *      segments for hidden lines are generated; otherwise they are not.</li>
 * <li> If the unfacetted entities flag, <tt>unfacetted</tt>, is <tt>TRUE</tt>, 
 *      only unfacetted entities are refaceted.</li>
 * <li> If the no hidden line calculations flag, <tt>fNoHidCalc</tt>, is <tt>TRUE</tt>, 
 *      hidden line calculations are not performed.</li>
 * </ul>
 * <br>
 * All data returned is allocated with <tt>new</tt>. It is the user's
 * responsibility to <tt>lose</tt> them when they are no longer needed.
 * <br><br>
 * <b>Errors:</b> An error results if the distance between the eye position and the target
 * is less than <tt>SPAresabs</tt>.
 * <br><br>
 * <b>Effect:</b> Changes model
 * <br><br>
 * <b>Journal: </b> Not Available
 * <br><br>
 * <b>Product(s):</b> 3D ACIS Modeler, 3D Viz Exchange
 * <br><br>
* @param entities
 * list of entities to compute from.
 * @param eyepos
 * eye position.
 * @param target
 * target position.
 * @param fPersp
 * <tt>TRUE</tt> if perspective projection.
 * @param fHidSeg
 * <tt>TRUE</tt> if hidden segments are needed.
 * @param unfacetted
 * <tt>TRUE</tt> to avoid refaceting entities.
 * @param fNoHidCalc
 * <tt>TRUE</tt> to avoid hidden line calculations.
 * @param segments
 * segments returned.
 **/
DECL_IHL outcome api_ihl_compute_entity_silhouette(
         const ENTITY_LIST& entities,         // entity list
         const SPAposition&    eyepos,         // eye SPAposition
         const SPAposition&    target,         // target SPAposition
         logical            fPersp,         // TRUE if perspective projection
         logical            fHidSeg,        // TRUE if hidden segments needed
		 logical			unfacetted,		// STI mka True if you do not want to refacet the entity
		 logical			fNoHidCalc,	    // STI mka TRUE if you do not want to calc hidden line but only silhouette
         ENTITY_LIST&       segments );     // IHL_SEGMENTs

// Set the output manager used by the IHL
/**
 * Sets the current output manager.
 * <br>
 * <b>Role:</b> Needs to be called only prior to calling
 * @href api_ihl_compute_from_meshes. The output manager gives an application full
 * control over how hidden line data is output.
 * <br><br>
 * Generally an application is expected to derive an application-specific class
 * from the <tt>IHL_OUTPUT_MANAGER</tt> and then pass this output manager to
 * <tt>api_ihl_set_output_manager</tt>.
 * <br><br>
 * <b>Effect:</b> System routine
 * <br><br>
 * <b>Journal: </b> Not Available
 * <br><br>
 * <b>Product(s):</b> 3D ACIS Modeler, 3D Viz Exchange
 * <br><br>
 * @param ihlOM
 * new current output manager.
 **/
DECL_IHL outcome api_ihl_set_output_manager(
         IHL_OUTPUT_MANAGER *ihlOM );
/**
 * Gets the current output manager.
 * <br>
 * <b>Role:</b> The output manager returned is used to output hidden line data.
 * <br><br>
 * <b>Effect:</b> Read-only
 * <br><br>
 * <b>Journal: </b> Not Available
 * <br><br>
 * <b>Product(s):</b> 3D ACIS Modeler, 3D Viz Exchange
 * <br><br>
* @param ihlOM
 * current output manager.
 **/
DECL_IHL outcome api_ihl_get_output_manager(
         IHL_OUTPUT_MANAGER *&ihlOM );

/**
 * Creates interactive hidden line output from a set of pre-existing meshes.
 * <br>
 * <b>Role:</b> Uses the current output manager to generate hidden line data. An
 * output manager of the <tt>IHL_OUTPUT_MANAGER</tt> class must be specified using the
 * @href api_ihl_set_output_manager function before this function is called.
 * (@href api_ihl_compute always uses the standard output manager.)
 * <br><br>
 * Normally hidden line data is created using @href api_ihl_compute, which accepts a list
 * of bodies, generates a list of corresponding meshes using the faceter, and
 * calculates hidden line data from these meshes. If the meshes already exists 
 * (i.e., the faceter does not need to be run again), <tt>api_ihl_compute_from_meshes</tt> 
 * can be called to generate hidden line data directly. 
 * <br><br>
 * If the perspective projection flag <tt>fPersp</tt> is <tt>TRUE</tt>, data is calculated for
 * display in a perspective projection; otherwise, calculations are done for a
 * parallel projection.
 * <br><br>
 * All data returned is allocated with <tt>new</tt>. It is the user's responsibility to call
 * <tt>lose</tt> for all entities in the list when they are no longer needed.
 * <br><br>
 * <b>Effect:</b> Changes model
 * <br><br>
 * <b>Journal: </b> Not Available
 * <br><br>
 * <b>Product(s):</b> 3D ACIS Modeler, 3D Viz Exchange
 * <br><br>
* @param meshs
 * array of meshes.
 * @param transfs
 * array of transforms.
 * @param num_meshs
 * number of meshes and transforms in the arrays.
 * @param eyepos
 * eye position.
 * @param target
 * target position.
 * @param fPersp
 * <tt>TRUE</tt> if perspective projection.
 **/
DECL_IHL outcome api_ihl_compute_from_meshes(
         POLYEDGE_MESH   *meshs[],           // list of meshes
         TRANSFORM       *transfs[],         // list of transforms
         int             num_meshs,          // Number of meshes and transforms in the list
         const SPAposition& eyepos,             // eye SPAposition
         const SPAposition& target,             // target SPAposition
         logical         fPersp );           // TRUE if perspective projection

/**
 * Removes overlapping invisible segments from the IHL segment list.
 * <br>
 * <b>Role:</b> This API function eliminates overlapping hidden segments. A tolerance
 * value is used to determine overlapping conditions. Visible segments are not affected.
 * <br><br>
 * <b>Effect:</b> Changes model
 * <br><br>
 * <b>Journal: </b> Not Available
 * <br><br>
 * <b>Product(s):</b> 3D ACIS Modeler, 3D Viz Exchange
 * <br><br>
 * @param ihl_segs
 * ihl segments.
 * @param tol
 * tolerance value default is <tt>SPAresfit</tt>.
 **/
DECL_IHL outcome api_ihl_occlusion(
		ENTITY_LIST &ihl_segs,	// ihl segments after ihl compute
		double tol=SPAresfit);		// tolerance to be used for occlusion

/**********************************************************************/
#define  IHLAPI_INCLUDE
/** @} */
#endif
/**********************************************************************/
