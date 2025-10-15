/*******************************************************************/
/*    Copyright (c) 1989-2020 by Spatial Corp.                     */
/*    All rights reserved.                                         */
/*    Protected by U.S. Patents 5,257,205; 5,351,196; 6,369,815;   */
/*                              5,982,378; 6,462,738; 6,941,251    */
/*    Protected by European Patents 0503642; 69220263.3            */
/*    Protected by Hong Kong Patent 1008101A                       */
/*******************************************************************/
//         Header for skinning and lofting api routines.
/*******************************************************************/
#if !defined( SKINAPI_HXX )
#define SKINAPI_HXX
#include "vers.hxx"
class BODY;
class FACE;
class COEDGE;
class WIRE;
class EDGE;
class ENTITY_LIST;
class SPAposition;
class VERTEX;
class AcisLoftingInterface;
class AcisSkinningInterface;
class AcisSLInterface;
class SPAinterval;
class skin_options;
struct Loft_Connected_Coedge_List;
#include "dcl_skin.h"
#include "api.hxx"
#include "logical.h"
#include "vector.hxx"
/**
 * \defgroup ADVSURF Advanced Surfacing
 * \defgroup SKINAPI Skinning
 * \defgroup LOFTAPI Lofting
 * \defgroup SLIAPI Interactive Skinning and Lofting
 * \defgroup NETAPI Net Surfacing
 */
/**
 * @file skinapi.hxx
 * @CAA2Level L1
 * @CAA2Usage U1
 * \defgroup SKINLOFTAPI Skinning and Lofting
 *
 * \ingroup ADVSURF Advanced Surfacing
 * @{
 **/
/**
 * Types of skin normal behavior at profile.
 * @param FIRST_NORMAL
 * Skin direction is normal to first profile
 * @param LAST_NORMAL
 * Skin direction is normal to last profile
 * @param ENDS_NORMAL
 * Skin direction is normal to both first and last profiles
 * @param ALL_NORMAL
 * Skin direction is normal to all profiles
 */
enum skinning_normals 
{
	FIRST_NORMAL,
	LAST_NORMAL, 
	ENDS_NORMAL, 
	ALL_NORMAL   
};

/**
 * Creates a ruled %surface between each pair of profiles.
 * @param RULED_SKINNING
 * Enable ruled surfacing
 */
enum skinning_ruled 
{
	RULED_SKINNING
};

/**
 * Initializes the skinning library.
 * <br><br>
 * <b>Effect:</b> System routine
 * <br><br>
 * <b>Journal: </b> Not Available
 * <br><br>
 * <b>Product(s):</b> 3D ACIS Modeler
 * <br><br>
**/
DECL_SKIN outcome api_initialize_skinning();

/**
 * Terminates the skinning library.
 * <br><br>
 * <b>Effect:</b> System routine
 * <br><br>
 * <b>Journal: </b> Not Available
 * <br><br>
 * <b>Product(s):</b> 3D ACIS Modeler
 * <br><br>
**/
DECL_SKIN outcome api_terminate_skinning();

// -----------------------------------------------------------------------------------------------------
//
// Single Call API's
//
// -----------------------------------------------------------------------------------------------------

/**
 * @nodoc
 */
DECL_SKIN outcome api_loft_coedges(
        int n_set,
		Loft_Connected_Coedge_List *set_ce_list,
        BODY *&sheet_body,						
		logical arc_length_option = FALSE,		
		logical twist_option = TRUE,			
		logical align_direction_option= TRUE,	
		logical perpendicular_option= FALSE,	
		logical simplify_option= TRUE,			
		logical closed_option= FALSE,			
		logical solid_option = TRUE,			
		AcisOptions *ao = NULL
);

/*! \addtogroup LOFTAPI
 *  \brief Declared at <skinapi.hxx>, SPAskin
 *  @{
 */

/**
 * Creates a sheet or solid body that fits a %surface (or set of surfaces) through a sequence of coedges, 
 * while providing start and end tangent control (via surfaces or ACIS laws).
 * <br><br>
 * <b>Role:</b> The resulting lofted surfaces are parametric surfaces (parameterized by <i>u</i> and <i>v</i>)
 * in which the user defined sequence of coedges provide the <i>u</i> parameter of the %surface and the lofting 
 * algorithm defines the <i>v</i> parameter.
 * <br><br>
 * The created sheet or solid body interpolates <i>n</i> cross sections defined by a list
 * of coedges. The <i>n</i> sections are given as an array of cross section definitions
 * (<tt>Loft_Connected_Coedge_List's</tt>). Each array element defines a list of coedges whose edges define the
 * cross section. If the coedges exist in a <tt>FACE</tt>, the %surface tangents at the boundary are associated
 * with the sections. If an ACIS law is included in the section list, the law imposes the take-off vector of the
 * resulting %surface. The default direction of tangents is outwards from the face.
 * <br><br>
 * If coedges in every given section are tangential to each other, then the skinned/lofted surfaces
 * in the resultant body will also be tangential to each other along the lateral (side) edges.
 * This is however not true in certain special situations. Refer to the Technical Article  
 * <i>Lateral Edge Tangency in Skinning and Lofting</i> for more details.
 * <br><br>
 * The function <tt>api_loft_coedges</tt> takes the data structure, <tt>Loft_Connected_Coedge_List</tt> as its
 * primary input. This structure is composed of the number of coedges in the cross section, a pointer to
 * the first coedge in a list of coedges, the cross section orientation, the take-off vector weight factor,
 * and an optional array of laws. Every cross section used in lofting must be presented with this data
 * structure and at a very minimum, two <tt>Loft_Connected_Coedge_List</tt> structures must be created.
 * <br><br>
 * Once the cross section data structures have been established, the <tt>api_loft_coedges</tt> function
 * can be called. The <tt>api_loft_coedges</tt> function also accepts the @href skin_options class.
 * <br><br>
 * In the case that a closed solid body is required, at least three distinct coedge lists must be
 * provided. If the first and last coedge lists are identical, a closed solid loft body is
 * automatically constructed. The end tangency will depend on the laws specified at the first and
 * last profiles. If duplicate copies of the internal coedge lists are sent to <tt>api_loft_coedges</tt>,
 * the copies are ignored while producing the loft.
 * <br><br>
 * If the user provides a set of closed profiles, the face normals of the loft body point outside,
 * away from the body material. When the user provides a set of open profiles, the face normals
 * of the loft face are oriented along the %surface normals, and no attempt is made to change
 * the face normal orientation.
 * <br><br>
 * <b>Limitations:</b> The loops defined by the coedge cross sections are assumed to be simple; 
 * that is, they must be well behaved and non-looping.
 * <br><br>
 * End capping (for creation of open solid bodies) is supported only for planar end profiles. If the end capping
 * operation fails then the sheet body constructed is returned.
 * <br><br>
 * <b>Effect:</b> Changes model
 * <br><br>
 * <b>Journal: </b> Available
 * <br><br>
 * <b>Product(s):</b> 3D ACIS Modeler
 * <br><br>
* @param n_set
 * number of connected coedge lists.
 * @param set_ce_list
 * array of connected coedge lists.
 * @param sheet_body
 * output sheet body.
 * @param opts
 * skinning options.
 * @param ao
 * ACIS options such as version and journal.
 **/
DECL_SKIN outcome api_loft_coedges(
        int n_set,  							// number of connected coedge list
		Loft_Connected_Coedge_List *set_ce_list,	// array of structures describing a connected list of coedges
        BODY *& sheet_body,						// output face
		skin_options *opts, 			        // skin options	arc_length, no_twist, align, perpendicular, simplify, closed, solid
		AcisOptions *ao = NULL
);

/**
 * Creates a sheet or solid body that fits a %surface (or set of surfaces) through a sequence of coedges, 
 * with start and end tangent control (via surfaces or ACIS laws) and the additional constraint of guide curves.
 * <br><br>
 * <b>Role:</b> The resulting lofted surfaces
 * are parametric surfaces (parameterized by <i>u</i> and <i>v</i>) in which the user defined sequence of coedges 
 * provide the <i>u</i> parameter of the %surface and the lofting algorithm defines the <i>v</i> parameter.
 * <br><br>
 * The created sheet or solid body interpolates <i>n</i> cross sections defined by a list
 * of coedges and given guide curves. The <i>n</i> sections are given as an array of cross section definitions
 * (<tt>Loft_Connected_Coedge_List's</tt>). Each array element defines a list of coedges whose edges define the
 * cross section. If the coedges exist in a <tt>FACE</tt>, the %surface tangents at the boundary are associated
 * with the sections. The default direction of tangents is outwards from the face. If an ACIS law is included
 * in the section list, the law imposes the take-off vector of the resulting %surface - superseding any %surface
 * constraint.
 * <br><br>
 * This overloaded version of <tt>api_loft_coedges</tt> accepts an array of guide curves in addition to the cross
 * section profiles. The guide curves "stretch" across the profiles from the first to last and add additional
 * control of the %surface in the <i>v</i> direction. If the tangent constraint and the guide %curve provide
 * conflicting directions, a <tt>skin_options</tt> is used to specify which constraint is preferred. (See @href skin_options and
 * the Technical Article on <i>Loft with Guides</i>.)
 * <br><br>
 * The function <tt>api_loft_coedges</tt> takes the data structure, <tt>Loft_Connected_Coedge_List</tt> as its
 * primary input. This structure is composed of the number of coedges in the cross section, a pointer to
 * the first coedge in a list of coedges, the cross section orientation, the take-off vector weight factor,
 * and an optional array of laws. Every cross section used in lofting must be presented with this data
 * structure and at a very minimum, two <tt>Loft_Connected_Coedge_List</tt> structures must be created.
 * <br><br>
 * The guides are accepted as <tt>EDGEs</tt> never as wire bodies and the %curve underlying the guide must be C<sup>1</sup> continuous
 * and non-looping. In addition, the guide must touch each wire profile within <tt>SPAresfit</tt> and start and stop exactly
 * on the first and last profiles. Direction is not important. The guides can intersect the section profiles on or
 * off vertices. If the guide does not lie on any vertices no %surface edge is created. If the guide intersects the
 * first profile off a vertex but intersects any other profile on a vertex, a %surface edge is made. In addition to
 * accepting guides this overloaded API supporting guide curves takes the <tt>logical</tt> flag <tt>virtualGuides</tt>. This flag toggles
 * the guides from being local %surface control to global %surface control. For example, in the case of local %surface
 * control, the guide %curve only affects the %surface created from the edges of the wire that the guide intersects. In the
 * case of <tt>virtualGuides</tt> being <tt>TRUE</tt>, the guide affects all the surfaces made from the wire body cross sections.
 * <br><br>
 * Once the cross section data structures and guides have been established, the <tt>api_loft_coedges</tt> function
 * can be called. The <tt>api_loft_coedges</tt> function also accepts the @href skin_options class.
 * <br><br>
 * In the case that a closed solid body is required, at least three distinct coedge lists must be
 * provided. If the first and last coedge lists are identical, a closed solid loft body is
 * automatically constructed. The end tangency will depend on the laws specified at the first and
 * last profiles. If duplicate copies of the internal coedge lists are sent to <tt>api_loft_coedges</tt>,
 * the copies are ignored while producing the loft.
 * <br><br>
 * If the user provides a set of closed profiles, the face normals of the loft body point outside,
 * away from the body material. When the user provides a set of open profiles, the face normals
 * of the loft face are oriented along the %surface normals, and no attempt is made to change
 * the face normal orientation.
 * <br><br>
 * <b>Limitations:</b> The loops defined by the coedge cross sections are assumed to be simple; that is, they must be well behaved
 * and non-looping.
 * <br><br>
 * End capping (for creation of open solid bodies) is supported only for planar end profiles. If the end capping
 * operation fails then the sheet body constructed is returned.
 * <br><br>
 * <b>Effect:</b> Changes model
 * <br><br>
 * <b>Journal: </b> Available
 * <br><br>
 * <b>Product(s):</b> 3D ACIS Modeler
 * <br><br>
* @param n_set
 * number of connected coedge  lists.
 * @param set_ce_list
 * array of connected coedge lists.
 * @param num_of_guides
 * number of guides.
 * @param guides
 * array of guide curves.
 * @param sheet_body
 * output sheet body.
 * @param opts
 * skinning options.
 * @param ao
 * ACIS options such as version and journal.
 **/
DECL_SKIN outcome api_loft_coedges(
		int 						n_set,		 	// number of connected coedge list
		Loft_Connected_Coedge_List *set_ce_list,	// array of connected coedge lists
        int                         num_of_guides,  // number of guides
        EDGE                        *guides[],      // guide curves
		BODY        				*&sheet_body,	// output sheet
		skin_options    			*opts,			// skin options
		AcisOptions                 *ao = NULL		// options, version, journal
		);

/**
 * @nodoc
 */
DECL_SKIN outcome api_loft_faces(
		FACE*,					// face 1
		double,					// take off factor for coedges on face 1
		FACE*,					// face 2
		double,					// take off factor for coedges on face 2
		BODY *&,				// output
		logical = FALSE,		// Arc length SPAparameter option
		logical = TRUE,			// If the co-edges are all closed minimize the surface twist.
		logical = TRUE,			// Align the directions of the co-edges so they are the same.
		logical = TRUE,			// Calculate the Loft direction perpendicular to the coedges
		logical = TRUE,          // Simplify geometry
		AcisOptions* = NULL
);

/**
 * Creates a solid body that fits a %surface (or set of surfaces) between two faces, with start and end tangent control (via neighboring surfaces).
 * <br><br>
 * <b>Role:</b> The resulting lofted surfaces are parametric surfaces (parameterized by <i>u</i> and <i>v</i>) in which
 * the user defined sequence of coedges provide the <i>u</i> parameter of the %surface and the lofting algorithm defines the
 * <i>v</i> parameter.
 * <br><br>
 * This function accepts two faces as arguments for lofting. The neighboring surfaces are determined and used
 * as tangent constraints for the resulting lofted faces. The two faces provided as arguments are removed from their respected
 * bodies. Peripheral edges from the first face are used to skin to the peripheral edges of the second face. The result is one
 * body.
 * <br><br>
 * When ACIS makes a <tt>BODY</tt> from a <tt>FACE</tt> using <tt>api_loft_faces</tt>, it makes a single-sided face body.
 * A single-sided face body is really a solid, not a sheet (infinitely thin) body. When a face body is single-sided,
 * basic solid modeling concepts (and ACIS) consider the body to be a solid which extends from the back side of the
 * face out to infinity with ill-defined boundaries extending where the edges of the original face extend backward.
 * Specific use of the single-sided face body determines whether subsequent operations, such as Boolean operations, work properly.
 * <br><br>
 * The <tt>api_loft_faces</tt> function also accepts the @href skin_options class.
 * <br><br>
 * For more information on the behavior of <tt>api_loft_faces</tt>, see the <i>Loft Between Faces</i> section of the
 * <i>Functional Variations of Lofting</i> Technical Article.
 * <br><br>
 * <b>Effect:</b> Changes model
 * <br><br>
 * <b>Journal: </b> Available
 * <br><br>
 * <b>Product(s):</b> 3D ACIS Modeler
 * <br><br>
* @param face1
 * first face.
 * @param fact1
 * take-off scale factor for coedges on face1.
 * @param face2
 * second face.
 * @param fact2
 * take-off scale factor for coedges on face2.
 * @param body
 * solid body returned.
 * @param opts
 * skinning options.
 * @param ao
 * ACIS options such as version or journal.
 **/
DECL_SKIN outcome api_loft_faces(
		FACE *face1,					// face 1
		double fact1,					// take-off factor for coedges on face 1
		FACE *face2,					// face 2
		double fact2,					// take-off factor for coedges on face 2
		BODY *&body,					// output body
		skin_options *opts, 			// skin options	arc_length, no_twist, align, perpendicular, simplify, closed, solid
		AcisOptions *ao = NULL
);

/**
 * Creates a solid body that fits a %surface (or set of surfaces) between two faces, 
 * with start and end tangent control (via neighboring surfaces) and the additional constraint of guide curves.
 * <br><br>
 * <b>Role:</b> The resulting lofted surfaces are parametric surfaces (parameterized by <i>u</i> and <i>v</i>) 
 * in which the user defined sequence of coedges provide the <i>u</i> parameter of the %surface
 * and the lofting algorithm defines the <i>v</i> parameter.
 * <br><br>
 * This overloaded version of <tt>api_loft_faces</tt> accepts an array of guide curves in addition to two faces.
 * The guide curves "stretch" across the faces from the first to last and add additional control of the %surface in the <i>v</i> direction.
 * If the tangent constraint and the guide %curve provide conflicting directions, a <tt>skin_options</tt> is used to specify which
 * constraint is preferred. (See @href skin_options and the Technical Article on <i>Loft with Guides</i>.)
 * <br><br>
 * The take-off factors control the magnitude of the tangent constraint applied to the start and end off the lofting surfaces. Values
 * of 1.0 will provide surface(s) with good curvature.
 * <br><br>
 * When a solid is lofted, the faces provided as arguments are removed from the body. The result is one body; however, this function does
 * not conduct a self-intersecting check on that body.
 * <br><br>
 * The <tt>api_loft_faces</tt> function also accepts the @href skin_options class.
 * <br><br>
 * For more information on the behavior of <tt>api_loft_faces</tt>, see the <i>Loft Between Faces</i> section of the
 * <i>Functional Variations of Lofting</i> Technical Article.
 * <br><br>
 * <b>Effect:</b> Changes model
 * <br><br>
 * <b>Journal: </b> Available
 * <br><br>
 * <b>Product(s):</b> 3D ACIS Modeler
 * <br><br>
* @param face1
 * first face.
 * @param fact1
 * take-off factor for coedges on face1.
 * @param face2
 * second face.
 * @param fact2
 * take-off factor for coedges on face2.
 * @param num_of_guides
 * number of guides.
 * @param guides
 * array of guide curves.
 * @param body
 * solid body returned.
 * @param opts
 * skinning options.
 * @param ao
 * ACIS options such as version or journal.
 **/
DECL_SKIN outcome api_loft_faces(
		FACE *face1,					// face 1
		double fact1,					// take-off factor for coedges on face 1
		FACE *face2,					// face 2
		double fact2,					// take-off factor for coedges on face 2
        int num_of_guides,              // Number of guides
        EDGE **guides,                  // Guide curve array
		BODY *&body,					// output body
		skin_options *opts, 			// skin options	arc_length, no_twist, align, perpendicular, simplify, closed, solid
		AcisOptions *ao = NULL
);

/*! @} */
/*! \addtogroup SKINAPI
 *  \brief Declared at <skinapi.hxx>, SPAskin
 *  @{
 */

/*
// tbrv
 */
/**
 * @nodoc
 */
DECL_SKIN outcome api_skin_wires(
		int,					// number of wire bodies
		BODY*[],				// array of wire bodies to skin
		BODY *&,				// output
		logical = FALSE,        // arc length option STI jmg
		logical = TRUE,			// Twist option
		logical = TRUE,			// Align direction of wires.
		logical = TRUE,	        // Simplify geometry
		logical = FALSE,        // construct a closed/open body for TRUE/FALSE
		logical = TRUE,			// construct a solid/sheet body for TRUE/FALSE
		AcisOptions* = NULL
);

/**
 * Creates a sheet or solid body that fits a %surface (or set of surfaces) through a sequence of wires contained in an array of bodies.
 * <br><br>
 * <b>Technical Articles:</b> <i>[Skinning](http://doc.spatial.com/articles/s/k/i/Component~Skinning_95b7.html)</i>, 
 * <i>[Functional Variations of Skinning](http://doc.spatial.com/articles/f/u/n/Functional_Variations_of_Skinning_c89c.html)</i>
 * <br><br>
 * <b>Role:</b> The resulting skinned surfaces are parametric surfaces (parameterized by <i>u</i> and <i>v</i>) in which the user defined
 * sequence of wires provide the <i>u</i> parameter of the %surface and the skin %surface algorithm defines the <i>v</i> parameter.
 * <br><br>
 * The given wires define the cross sections to be interpolated by the resulting sheet
 * or solid body. The resulting body will contain one or more skinned surfaces. (With the exception of the
 * <tt>simplify</tt> option being <tt>TRUE</tt>, in which the surface(s) might be converted to an applicable conical
 * type.) This overloaded version of <tt>api_skin_wires</tt> supports <i>Basic Skinning</i>. That is, the wire cross sections control
 * the %surface shape: there is no other mechanism to alter the shape. Refer to the Technical Articles on 
 * <i>Functional Variations of Skinning</i> and <i>Skinning and Lofting Options</i>
 * for a complete list of the functional variations of skinning and various methods of %surface control.
 * <br><br>
 * If edges in every given wire profiles are tangential to each other, then the skinned surfaces
 * in the resultant body will also be tangential to each other along the lateral (side) edges.
 * This is however not true in certain special situations. Refer to the Technical Article 
 * <i>Lateral Edge Tangency in Skinning and Lofting</i> for more details.
 * <br><br>
 * In all cases of skinning the cross section profiles are copies; that is, the originals remain.
 * The profiles can be either all open or all closed. In the case of closed loops the end profiles can be degenerated
 * (a point). If the user provides a set of closed profiles, the face normals of the skin body point outside, away from the
 * body material. When the user provides a set of open profiles, the face normals of the skin face are oriented along the
 * %surface normals, and no attempt is made to change the face normal orientation.
 * <br><br>
 * The <tt>api_skin_wires</tt> function also accepts the @href skin_options class.
 * <br><br>
 * <b>Errors:</b> A wire pointer in the array is <tt>NULL</tt> or not to a wire body.
 * <br><br>
 * <b>Limitations:</b> The wire bodies are assumed to be simple; that is, they must be well behaved
 * and non-looping.
 * <br><br>
 * The parameterization of the %surface in its <i>u</i> direction - the direction of the user-defined
 * wire bodies - follows the parameterization of the curves underlying the wire bodies.
 * <br><br>
 * Skinning cannot produce a %surface in which there is any one point on the %surface in which
 * the <i>u</i> and <i>v</i> directions are the same or opposite. That is, in no case can an underlying %curve
 * of the user-defined wire body and a lateral edge of the %surface generated from the skinning
 * algorithm ever meet tangentially.
 * <br><br>
 * End capping (for creation of open solid bodies) is supported only for planar end profiles.
 * If the end capping operation fails then the sheet body constructed is returned.
 * <br><br>
 * <b>Effect:</b> Changes model
 * <br><br>
 * <b>Journal: </b> Available
 * <br><br>
 * <b>Product(s):</b> 3D ACIS Modeler
 * <br><br>
* @param num_wires
 * number of wire bodies.
 * @param wires
 * array of wire bodies.
 * @param sheet_body
 * resulting sheet body or solid.
 * @param opts
 * skinning options.
 * @param ao
 * ACIS options such as version and journal.
 **/
DECL_SKIN outcome api_skin_wires(
		int num_wires,					// number of wire bodies
		BODY *wires[],					// array of wire bodies to skin
		BODY *&sheet_body,				// output body
		skin_options *opts, 			// skin options	arc_length, no_twist, align, perpendicular, simplify, closed, solid
		AcisOptions *ao = NULL
 );

/*
// tbrv
 */
/**
 * @nodoc
 */
DECL_SKIN outcome api_skin_wires(
		int,					// number of wire bodies
		ENTITY_LIST&,			// wires edges, or face to skin
		BODY *&,				// output
		logical = FALSE,        // arc length option STI jmg
		logical = TRUE,			// Twist option
		logical = TRUE,			// Align direction of wires.
		logical = TRUE,	        // Simplify geometry
		logical = FALSE,        // construct a closed/open body for TRUE/FALSE
		logical = TRUE,			// construct a solid/sheet body for TRUE/FALSE
		AcisOptions* = NULL
);

/**
 * Creates a sheet or solid body that fits a %surface (or set of surfaces) through a sequence of wires contained in an <tt>ENTITY_LIST</tt>.
 * <br><br>
 * <b>Technical Articles:</b> <i>[Skinning](http://doc.spatial.com/articles/s/k/i/Component~Skinning_95b7.html)</i>, 
 * <i>[Functional Variations of Skinning](http://doc.spatial.com/articles/f/u/n/Functional_Variations_of_Skinning_c89c.html)</i>
 * <br><br>
 * <b>Role:</b> The resulting skinned surfaces are parametric surfaces (parameterized by <i>u</i> and <i>v</i>) in which the user defined
 * sequence of wires provide the <i>u</i> parameter of the %surface and the skin %surface algorithm defines the <i>v</i> parameter.
 * <br><br>
 * The given wires define the cross sections to be interpolated by the resulting sheet
 * or solid body. The resulting body will contain one or more skinned surfaces. (With the exception of the
 * <tt>simplify</tt> option being <tt>TRUE</tt>, in which the surface(s) might be converted to an applicable conical
 * type.) This overloaded version of <tt>api_skin_wires</tt> supports <i>Basic Skinning</i>. That is, the wire cross sections control
 * the %surface shape, there is no other mechanism to alter the shape. Refer to the Technical Articles on 
 * <i>Functional Variations of Skinning</i> and <i>Skinning and Lofting Options</i>
 * for a complete list of the functional variations of skinning and various methods of %surface control.
 * <br><br>
 * If edges in every given wire profiles are tangential to each other, then the skinned surfaces
 * in the resultant body will also be tangential to each other along the lateral (side) edges.
 * This is however not true in certain special situations. Refer to the Technical Article 
 * <i>Lateral Edge Tangency in Skinning and Lofting</i> for more details.
 * <br><br>
 * In all cases of skinning the cross section profiles are copies; that is, the originals remain.
 * The profiles can be either all open or all closed. In the case of closed loops the end profiles can be degenerate
 * (a point). If the user provides a set of closed profiles, the face normals of the skin body point outside, away from the
 * body material. When the user provides a set of open profiles, the face normals of the skin face are oriented along the
 * %surface normals, and no attempt is made to change the face normal orientation.
 * <br><br>
 * The <tt>api_skin_wires</tt> function also accepts the @href skin_options class.
 * <br><br>
 * <b>Errors:</b> A wire pointer in the array is <tt>NULL</tt> or not to a wire body.
 * <br><br>
 * <b>Limitations:</b> The wire bodies are assumed to be simple; that is, they must be well behaved
 * and non-looping.
 * <br><br>
 * The parameterization of the %surface in its <i>u</i> direction - the direction of the user-defined
 * wire bodies - follows the parameterization of the curves underlying the wire bodies. (See
 * the skin option <tt>arc_length_u</tt> to alter this behavior.)
 * <br><br>
 * Skinning cannot produce a %surface in which there is any one point on the %surface in which
 * the <i>u</i> and <i>v</i> directions are the same or opposite. That is, in no case can an underlying %curve
 * of the user-defined wire body and a lateral edge of the %surface generated from the skinning
 * algorithm ever meet tangentially.
 * <br><br>
 * End capping (for creation of open solid bodies) is supported only for planar end profiles.
 * If the end capping operation fails then the sheet body constructed is returned.
 * <br><br>
 * <b>Effect:</b> Changes model
 * <br><br>
 * <b>Journal: </b> Not Available
 * <br><br>
 * <b>Product(s):</b> 3D ACIS Modeler
 * <br><br>
* @param num_entities
 * number of wire bodies.
 * @param in_entities
 * list of wire bodies.
 * @param sheet_body
 * resulting sheet body.
 * @param opts
 * skin options.
 * @param ao
 * ACIS options such as version and journal.
 **/
DECL_SKIN outcome api_skin_wires(
		int num_entities,				// number of wire bodies
		ENTITY_LIST &in_entities,		// wires edges, or face to skin
		BODY *&sheet_body,				// output body
		skin_options *opts, 			// skin options	arc_length, no_twist, align, perpendicular, simplify, closed, solid
		AcisOptions *ao= NULL
);

/**
 * Creates a sheet or solid body that fits a %surface (or set of surfaces) through a sequence of wire profiles 
 * with the additional constraint of take-off vectors normal to the wire profiles.
 * <br><br>
 * <b>Technical Articles:</b> <i>[Skinning](http://doc.spatial.com/articles/s/k/i/Component~Skinning_95b7.html)</i>, 
 * <i>[Functional Variations of Skinning](http://doc.spatial.com/articles/f/u/n/Functional_Variations_of_Skinning_c89c.html)</i>
 * <br><br>
 * <b>Role:</b> The resulting skinned surfaces are parametric surfaces
 * (parameterized by <i>u</i> and <i>v</i>) in which the user defined sequence of wires provide the <i>u</i> 
 * parameter of the %surface and the skin %surface algorithm defines the <i>v</i> parameter.
 * <br><br>
 * The given wires define the cross sections to be interpolated by the resulting sheet body. The resulting body will
 * contain one or more skinned surfaces. (With the exception of the <tt>simplify</tt> option being <tt>TRUE</tt>, in which 
 * the surface(s) might be converted to an applicable conical type.)
 * <br><br>
 * If edges in every given wire profiles are tangential to each other, then the skinned surfaces
 * in the resultant body will also be tangential to each other along the lateral (side) edges.
 * This is however not true in certain special situations. Refer to the technical article 
 * <i>Lateral Edge Tangency in Skinning and Lofting</i> for more details.
 * <br><br>
 * This overloadeded version of <tt>api_skin_wires</tt> supports the Skinning to the Planar Normal functional variation of skinning.
 * The API accepts the enumerated type <tt>skinning_normals</tt> as the fourth parameter. This enumerated type has four constants:
 * <tt>first_normal</tt>, <tt>last_normal</tt>, <tt>ends_normal</tt>, and <tt>all_normal</tt>. At the specified profiles 
 * (that is, the first or the last or the
 * first and the last or all) the resulting skinned %surface will take off with the direction that is given by the normal of that
 * profile. This variation of skinning does not accept degenerate profiles (points) or profiles that do not define a plane (lines).
 * In addition, it does not accept any form of magnitude control (see <i>Skinning with a Draft Angle</i>). An optimum magnitude is
 * calculated for you.
 * <br><br>
 * This <tt>api_skin_wires</tt> function also accepts the @href skin_options class.
 * <br><br>
 * <b>Errors:</b> A wire pointer in the array is <tt>NULL</tt> or not to a wire body.
 * <br><br>
 * <b>Limitations:</b> The wire bodies are assumed to be simple; that is, they must be well behaved
 * and non-looping.
 * <br><br>
 * The parameterization of the %surface in its <i>u</i> direction - the direction of the user-defined
 * wire bodies - follows the parameterization of the curves underlying the wire bodies. (See
 * the skin option <tt>arc_length_u</tt> to alter this behavior.)
 * <br><br>
 * Skinning cannot produce a %surface in which there is any one point on the %surface in which
 * the <i>u</i> and <i>v</i> directions are the same or opposite. That is, in no case can an underlying %curve
 * of the user-defined wire body and a lateral edge of the %surface generated from the skinning
 * algorithm ever meet tangentially.
 * <br><br>
 * End capping (for creation of open solid bodies) is supported only for planar end profiles.
 * If the end capping operation fails then the sheet body constructed is returned.
 * <br><br>
 * <b>Effect:</b> Changes model
 * <br><br>
 * <b>Journal: </b> Available
 * <br><br>
 * <b>Product(s):</b> 3D ACIS Modeler
 * <br><br>
* @param num_wires
 * number of wire bodies.
 * @param wires
 * array of wire bodies.
 * @param sheet_body
 * resulting sheet body or solid body.
 * @param in_normals
 * skinning normals.
 * @param opts
 * skinning options.
 * @param ao
 * ACIS options such as version and journal.
 **/
DECL_SKIN outcome api_skin_wires(
		int num_wires,					// Number of wire bodies
		BODY **wires,					// List of wire bodies
		BODY *&sheet_body,				// output body
		skinning_normals in_normals,	// skinning normal type
		skin_options *opts, 			// skin options	arc_length, no_twist, align, perpendicular, simplify, closed, solid
		AcisOptions *ao = NULL
);

/**
 * Creates a sheet or solid body that fits a %surface (or set of surfaces) through a sequence of wire profiles 
 * with the additional constraint of take-off vectors.
 * <br><br>
 * <b>Technical Articles:</b> <i>[Skinning](http://doc.spatial.com/articles/s/k/i/Component~Skinning_95b7.html)</i>, 
 * <i>[Functional Variations of Skinning](http://doc.spatial.com/articles/f/u/n/Functional_Variations_of_Skinning_c89c.html)</i>
 * <br><br>
 * <b>Role:</b> The resulting skinned surfaces are parametric surfaces (parameterized by <i>u</i> and
 * <i>v</i>) in which the user defined sequence of wires provide the <i>u</i> parameter of the %surface and the skin %surface algorithm
 * defines the <i>v</i> parameter.
 * <br><br>
 * The given wires define the cross sections to be interpolated by the resulting solid or sheet body with the
 * additional constraint of take-off vectors. The resulting body will contain one or more skinned surfaces. (With the exception
 * of the <tt>simplify</tt> option being <tt>TRUE</tt>, in which the surface(s) might be converted to an applicable conical type.)
 * <br><br>
 * If edges in every given wire profiles are tangential to each other, then the skinned surfaces
 * in the resultant body will also be tangential to each other along the lateral (side) edges.
 * This is however not true in certain special situations. Refer to the Technical Article 
 * <i>Lateral Edge Tangency in Skinning and Lofting</i> for more details.
 * <br><br>
 * This overloadeded version of <tt>api_skin_wires</tt> supports the <i>Skinning with Vectors</i> functional variation of skinning.
 * Skinning with Vectors accepts as additional constraints an array of vectors and optionally an array of magnitudes to control
 * the take-off directions with which the skinned %surface leaves the profiles. The number of supplied vectors must equal the
 * number of profiles, if the <tt>closed</tt> option is set to <tt>FALSE</tt>. If the <tt>closed</tt> option is set to <tt>TRUE</tt>, one more vector can be
 * given. It will be used on the copy that is made of the first profile to create a closed body. If no additional vector is
 * provided, the first vector will be used also on the first profile's copy. In a similar way, magnitudes can be provided.
 * A profile will be interpolated without constraint if the vector supplied for it is the the zero vector.
 * <br><br>
 * This <tt>api_skin_wires</tt> function also accepts the @href skin_options class.
 * <br><br>
 * <b>Errors:</b> A wire pointer in the array is <tt>NULL</tt> or not to a wire body.
 * <br><br>
 * <b>Limitations:</b> The wire bodies are assumed to be simple; that is, they must be well behaved
 * and non-looping.
 * <br><br>
 * The parameterization of the %surface in its <i>u</i> direction - the direction of the user-defined
 * wire bodies - follows the parameterization of the curves underlying the wire bodies. (See
 * the skin option <tt>arc_length_u</tt> to alter this behavior.)
 * <br><br>
 * Skinning cannot produce a %surface in which there is any one point on the %surface in which
 * the <i>u</i> and <i>v</i> directions are the same or opposite. That is, in no case can an underlying %curve
 * of the user-defined wire body and a lateral edge of the %surface generated from the skinning
 * algorithm ever meet tangentially.
 * <br><br>
 * End capping (for creation of open solid bodies) is supported only for planar end profiles.
 * If the end capping operation fails then the sheet body constructed is returned. 
 * <br><br>
 * <b>Effect:</b> Changes model
 * <br><br>
 * <b>Journal: </b> Available
 * <br><br>
 * <b>Product(s):</b> 3D ACIS Modeler
 * <br><br>
* @param num_wires
 * number of wire bodies.
 * @param wires
 * array of wire bodies.
 * @param sheet_body
 * resulting sheet body or solid body.
 * @param vectors
 * array of skinning vectors.
 * @param num_vectors
 * number of vectors.
 * @param magnitudes
 * array of magnitudes.
 * @param num_magnitudes
 * number of magnitudes.
 * @param opts
 * skinning options.
 * @param ao
 * ACIS options such as version and journal.
 **/
DECL_SKIN outcome api_skin_wires(
		int num_wires,						// Number of wire bodies
		BODY **wires,						// Array of wire bodies
		BODY *&sheet_body,					// output body
		SPAvector *vectors,			        // skinning vectors
		int num_vectors,			        // number of vectors
		double *magnitudes	= NULL,	        // magnitudes
		int num_magnitudes	= 0,	        // number of magnitudes
		skin_options *opts		= NULL,     // skin options
		AcisOptions *ao		= NULL	        // version, journal
);

/**
 * Creates a sheet or solid body that fits a %surface (or set of surfaces) through a sequence of wire profiles 
 * with the additional constraints of take-off vectors and guide curves.
 * <br><br>
 * <b>Technical Articles:</b> <i>[Skinning](http://doc.spatial.com/articles/s/k/i/Component~Skinning_95b7.html)</i>, 
 * <i>[Functional Variations of Skinning](http://doc.spatial.com/articles/f/u/n/Functional_Variations_of_Skinning_c89c.html)</i>
 * <br><br>
 * <b>Role:</b>  The resulting skinned surfaces are parametric surfaces
 * (parameterized by <i>u</i> and <i>v</i>) in which the user defined sequence of wires provide the <i>u</i> 
 * parameter of the %surface and the skin %surface algorithm defines the <i>v</i> parameter.
 * <br><br>
 * The given wires define the cross sections to be interpolated by the resulting sheet body with the additional
 * constraints of take-off vectors and guide curves. The resulting body will contain one or more skin or net surfaces. (With the
 * exception of the <tt>simplify</tt> option being <tt>TRUE</tt>, in which the surface(s) might be converted to an applicable conical type.)
 * <br><br>
 * This overloadeded version of <tt>api_skin_wires</tt> is a hybrid operation of the two functional variations of skinning:
 * <i>Skinning with Vectors</i> and <i>Skinning with Guide Curves</i>. That is, it accepts both vectors and guides as %surface constraints.
 * <br><br>
 * To support the vector constraints this API accepts an array of vectors to control the take-off directions with which the skinned
 * %surface leaves the profiles. The number of supplied vectors must equal the number of profiles, if the <tt>closed</tt> option is set
 * to <tt>FALSE</tt>. If the <tt>closed</tt> option is set to <tt>TRUE</tt>, one more vector can be given. It will be used on the copy that is made of
 * the first profile to create a closed body. If no additional vector is provided, the first vector will be used also on the first
 * profile's copy. A profile will be interpolated without constraint if the vector supplied for it is the the zero vector.
 * <br><br>
 * To support the guide constraints this API accepts an array of <tt>EDGEs</tt>. The %curve underlying the guide must be C<sup>1</sup> continuous
 * and non-looping. In addition, the guide must touch each wire profile within <tt>SPAresfit</tt> and START and STOP exactly
 * on the first and last profiles. Direction is not important. The guides can intersect the section profiles on or
 * off vertices. If the guide does not lie on any vertices, no %surface edge is created. If the guide intersects the
 * first profile off a vertex but intersects any other profile on a vertex, a %surface edge is made. In addition to
 * accepting guides this overloaded API supporting guide curves takes the <tt>logical</tt> flag <tt>virtualGuides</tt>. This flag toggles
 * the guides from being local %surface control to global %surface control. For example, in the case of local %surface
 * control, the guide %curve only affects the %surface created from the edges of the wire that the guide intersects. In the
 * case of <tt>virtualGuides</tt> being <tt>TRUE</tt>, the guide affects all the surfaces made from the wire body cross sections.
 * <br><br>
 * If the tangent constraint and the guide curves provide conflicting directions, a <tt>skin_options</tt> is used to specify
 * which constraint is preferred. (See @href skin_options and the technical article on <i>Loft With Guides</i>.) This API
 * also accepts the other skinning options avaliable in this class.
 * <br><br>
 * <b>Errors:</b> A wire pointer in the array is <tt>NULL</tt> or not to a wire body.
 * <br><br>
 * <b>Limitations:</b> The wire bodies are assumed to be simple; that is, they must be well behaved and non-looping.
 * <br><br>
 * The parameterization of the %surface in its <i>u</i> direction - the direction of the user-defined
 * wire bodies - follows the parameterization of the curves underlying the wire bodies. (See
 * the skin option <tt>arc_length_u</tt> to alter this behavior.)
 * <br><br>
 * Skinning cannot produce a %surface in which there is any one point on the %surface in which
 * the <i>u</i> and <i>v</i> directions are the same or opposite. That is, in no case can an underlying %curve
 * of the user-defined wire body and a lateral edge of the %surface generated from the skinning
 * algorithm ever meet tangentially.
 * <br><br>
 * End capping (for creation of open solid bodies) is supported only for planar end profiles.
 * If the end capping operation fails then the sheet body constructed is returned.
 * <br><br>
 * <b>Effect:</b> Changes model
 * <br><br>
 * <b>Journal: </b> Available
 * <br><br>
 * <b>Product(s):</b> 3D ACIS Modeler
 * <br><br>
* @param num_wires
 * number of wire bodies.
 * @param wires
 * array of wire bodies.
 * @param sheet_body
 * resulting sheet body or solid body.
 * @param vectors
 * array of skinning vectors.
 * @param num_vectors
 * number of vectors.
 * @param guides
 * array of guide curves.
 * @param number_of_guides
 * number of guides.
 * @param opts
 * skinning options.
 * @param ao
 * ACIS options such as version and journal.
 **/
DECL_SKIN outcome api_skin_wires(
		int num_wires,                           // Number of wire bodies
		BODY **wires,                            // Array of Wire Bodies
		BODY *&sheet_body,                       // Output body
		SPAvector *vectors,                      // Skinning Vectors
		int num_vectors,                         // Number of Skinning Vectors
		EDGE **guides,                           // Guide Curves
        int number_of_guides,                    // Number of Guide Curves
		skin_options *opts = NULL,               // Skin options
		AcisOptions *ao = NULL                   // acis options
 );

/*
// tbrv
 */
/**
 * @nodoc
 */
DECL_SKIN outcome api_skin_wires(
		int,					// number of wire bodies
		BODY*[],				// array of wire bodies to skin
		BODY*,                  // Wire Path
		BODY *&,				// output
		logical = FALSE,        // arc length option STI jmg
		logical = TRUE,			// Twist option
		logical = TRUE,			// Align direction of wires.
		logical = TRUE,	        // Simplify geometry
		logical = FALSE,        // construct a closed/open body for TRUE/FALSE
		logical = TRUE,			// construct a solid/sheet body for TRUE/FALSE
		AcisOptions* = NULL
);

/**
 * Creates a sheet or solid body that fits a %surface (or set of surfaces) through a sequence of wire profiles 
 * with the additional constraint of take-off vectors that are defined by a given "path".
 * <br><br>
 * <b>Technical Articles:</b> <i>[Skinning](http://doc.spatial.com/articles/s/k/i/Component~Skinning_95b7.html)</i>, 
 * <i>[Functional Variations of Skinning](http://doc.spatial.com/articles/f/u/n/Functional_Variations_of_Skinning_c89c.html)</i>
 * <br><br>
 * <b>Role:</b> The resulting skinned surfaces are parametric surfaces (parameterized by <i>u</i> and <i>v</i>) 
 * in which the user defined sequence of wires provide the <i>u</i> parameter of the %surface and the
 * skin %surface algorithm defines the <i>v</i> parameter.
 * <br><br>
 * The given wires define the cross sections to be interpolated by the resulting sheet body with the additional
 * constraint of take-off vectors defined by the <i>path</i>. That is, where the path intersects the %plane of the wire, the tangent
 * direction of the path at this point is a vector constraint for the resulting skinned %surface. The resulting body will contain
 * one or more skinned surfaces. (With the exception of the <tt>simplify</tt> option being <tt>TRUE</tt>, in which the surface(s) might be
 * converted to an applicable conical type.)
 * <br><br>
 * To support this constraint this API accepts an additional <tt>BODY</tt>* as the path. The underlying %curve of the path should be
 * non-looping and everywhere differentiable.
 * <br><br>
 * If edges in every given wire profiles are tangential to each other, then the skinned surfaces
 * in the resultant body will also be tangential to each other along the lateral (side) edges.
 * This is however not true in certain special situations. Refer to the Technical Article 
 * <i>Lateral Edge Tangency in Skinning and Lofting</i> for more details.
 * <br><br>
 * The <tt>api_skin_wires</tt> function also accepts the @href skin_options class.
 * <br><br>
 * <b>Errors:</b> A wire pointer in the array is <tt>NULL</tt> or not to a wire body.
 * <br><br>
 * <b>Limitations:</b> The wire bodies are assumed to be simple; that is, they must be well behaved and non-looping.
 * <br><br>
 * The parameterization of the %surface in its <i>u</i> direction - the direction of the user-defined
 * wire bodies - follows the parameterization of the curves underlying the wire bodies. (See
 * the skin option <tt>arc_length_u</tt> to alter this behavior.)
 * <br><br>
 * Skinning cannot produce a %surface in which there is any one point on the %surface in which
 * the <i>u</i> and <i>v</i> directions are the same or opposite. That is, in no case can an underlying %curve
 * of the user-defined wire body and a lateral edge of the %surface generated from the skinning
 * algorithm ever meet tangentially.
 * <br><br>
 * End capping (for creation of open solid bodies) is supported only for planar end profiles.
 * If the end capping operation fails then the sheet body constructed is returned.
 * <br><br>
 * <b>Effect:</b> Changes model
 * <br><br>
 * <b>Journal: </b> Available
 * <br><br>
 * <b>Product(s):</b> 3D ACIS Modeler
 * <br><br>
* @param num_entities
 * number of wire bodies.
 * @param in_entities
 * array of wire bodies.
 * @param path
 * path option.
 * @param sheet_body
 * resulting sheet body.
 * @param opts
 * skin options.
 * @param ao
 * ACIS options such as version and journal.
 **/
DECL_SKIN outcome api_skin_wires(
		int num_wires,				// number of wire bodies
		BODY *wires[],				// array of wire bodies to skin
		BODY *path,                 // Wire Path
		BODY *&sheet_body,			// output body
		skin_options *opts, 		// skin options	arc_length, no_twist, align, perpendicular, simplify, closed, solid
		AcisOptions *ao = NULL
 );

/**
 * Creates a sheet or solid body that fits a %surface (or set of surfaces) through a sequence of wire profiles 
 * with the additional constraint of the resulting surfaces being ruled.
 * <br><br>
 * <b>Technical Articles:</b> <i>[Skinning](http://doc.spatial.com/articles/s/k/i/Component~Skinning_95b7.html)</i>, 
 * <i>[Functional Variations of Skinning](http://doc.spatial.com/articles/f/u/n/Functional_Variations_of_Skinning_c89c.html)</i>
 * <br><br>
 * <b>Role:</b>  The resulting skinned surfaces are parametric surfaces (parameterized by <i>u</i> and <i>v</i>) 
 * in which the user defined sequence of wires provide the <i>u</i> parameter of the %surface and the
 * skin %surface algorithm defines the <i>v</i> parameter.
 * <br><br>
 * The given wires define the cross sections to be interpolated by the resulting sheet body with the additional
 * constraint of the resulting surfaces being ruled. That is, the surfaces are linear in the <i>v</i> direction of the skin. The
 * resulting body will contain one or more ruled surfaces. (With the exception of the <tt>simplify</tt> option being <tt>TRUE</tt>, in which
 * the surface(s) might be converted to an applicable conical type.) This overloaded version of <tt>api_skin_wires</tt> accepts
 * the enumerated type <tt>skinning_ruled</tt>. This enumerated type has only one constant: <tt>RULED_SKINNING</tt>.
 * <br><br>
 * Ruled skinning accepts degenerate end points. In addition it supports the <tt>closed</tt> option; however, it does not detect 
 * self-intersections.
 * <br><br>
 * The <tt>api_skin_wires</tt> function also accepts the @href skin_options class.
 * <br><br>
 * <b>Errors:</b> A wire pointer in the array is <tt>NULL</tt> or not to a wire body.
 * <br><br>
 * <b>Limitations:</b> The wire bodies are assumed to be simple; that is, they must be well behaved and non-looping.
 * <br><br>
 * The parameterization of the %surface in its <i>u</i> direction - the direction of the user-defined
 * wire bodies - follows the parameterization of the curves underlying the wire bodies. (See
 * the skin option <tt>arc_length_u</tt> to alter this behavior.)
 * <br><br>
 * Skinning cannot produce a %surface in which there is any one point on the %surface in which
 * the <i>u</i> and <i>v</i> directions are the same or opposite. That is, in no case can an underlying %curve
 * of the user-defined wire body and a lateral edge of the %surface generated from the skinning
 * algorithm ever meet tangentially.
 * <br><br>
 * End capping (for creation of open solid bodies) is supported only for planar end profiles.
 * If the end capping operation fails then the sheet body constructed is returned.
 * <br><br>
 * <b>Effect:</b> Changes model
 * <br><br>
 * <b>Journal: </b> Available
 * <br><br>
 * <b>Product(s):</b> 3D ACIS Modeler
 * <br><br>
* @param num_wires
 * number of wire bodies.
 * @param wires
 * array of wire bodies.
 * @param sheet_body
 * resulting sheet body or solid body.
 * @param ruled
 * enumeration type to signify ruled skinning.
 * @param opts
 * skinning options.
 * @param ao
 * ACIS options such as version and journal.
 **/
DECL_SKIN outcome api_skin_wires(
		int num_wires,					// number of wire bodies
		BODY *wires[],					// array of wire bodies to skin
		BODY *&sheet_body,				// output body
		skinning_ruled ruled,			// flag for ruled skinning
		skin_options *opts, 			// skin options	arc_length, no_twist, align, perpendicular, simplify, closed, solid
		AcisOptions *ao= NULL
 );

/*
// tbrv
 */
/**
 * @nodoc
 */
DECL_SKIN outcome api_skin_wires(
		int,					// number of wire bodies
		ENTITY_LIST&,			// wires edges, or face to skin
		BODY*,                  // Wire Path
		BODY *&,				// output
		logical = FALSE,        // arc length option STI jmg
		logical = TRUE,			// Twist option
		logical = TRUE,			// Align direction of wires.
		logical = TRUE,	        // Simplify geometry
		logical = FALSE,        // construct a closed/open body for TRUE/FALSE
		logical = TRUE,			// construct a solid/sheet body for TRUE/FALSE
		AcisOptions* = NULL
);

/**
 * Creates a sheet or solid body that fits a %surface (or set of surfaces) through a sequence of wire profiles 
 * with the additional constraint that either the take-off vectors are defined from the given "path" or by 
 * sweeping along the given path.
 * <br><br>
 * <b>Technical Articles:</b> <i>[Skinning](http://doc.spatial.com/articles/s/k/i/Component~Skinning_95b7.html)</i>, 
 * <i>[Functional Variations of Skinning](http://doc.spatial.com/articles/f/u/n/Functional_Variations_of_Skinning_c89c.html)</i>
 * <br><br>
 * <b>Role:</b> The resulting skinned surfaces are parametric surfaces (parameterized by <i>u</i> and <i>v</i>) 
 * in which the user defined sequence of wires provide the <i>u</i> parameter of the %surface and the
 * skin %surface algorithm defines the <i>v</i> parameter.
 * <br><br>
 * The given wires define the cross sections to be interpolated by the resulting sheet body with an additional interpolation
 * constraint based on the <i>path</i> curve.
 * <br><br>
 * When the skinning option <i>sweep_path</i> is off : The take-off vectors are defined by the <i>path</i>. That is, where the path 
 * intersects the %plane of the wire, the tangent direction of the path at this point is a vector constraint for the resulting 
 * skinned %surface. The resulting body will contain one or more skinned surfaces. (With the exception of the <tt>simplify</tt> 
 * option being <tt>TRUE</tt>, in which the surface(s) might be converted to an applicable conical type.)
 * <br>
 * When the skinning option <i>sweep_path</i> is on : The resulting skinned %surface is swept along the path and it will interpolate
 * the profiles smoothly.
 * <br><br>
 * To support this constraint this API accepts an additional <tt>BODY</tt>* as the path. The underlying %curve of the path should be
 * non-looping and everywhere differentiable.
 * <br><br>
 * If edges in every given wire profiles are tangential to each other, then the skinned surfaces
 * in the resultant body will also be tangential to each other along the lateral (side) edges.
 * This is however not true in certain special situations. Refer to the Technical Article 
 * <i>Lateral Edge Tangency in Skinning and Lofting</i> for more details.
 * <br><br>
 * The <tt>api_skin_wires</tt> function also accepts the @href skin_options class.
 * <br><br>
 * <b>Errors:</b> A wire pointer in the array is <tt>NULL</tt> or not to a wire body.
 * <br><br>
 * <b>Limitations:</b> The wire bodies are assumed to be simple; that is, they must be well behaved and non-looping.
 * <br><br>
 * The parameterization of the %surface in its <i>u</i> direction - the direction of the user-defined
 * wire bodies - follows the parameterization of the curves underlying the wire bodies. (See
 * the skin option <tt>arc_length_u</tt> to alter this behavior.
 * <br><br>
 * Skinning cannot produce a %surface in which there is any one point on the %surface in which
 * the <i>u</i> and <i>v</i> directions are the same or opposite. That is, in no case can an underlying %curve
 * of the user-defined wire body and a lateral edge of the %surface generated from the skinning
 * algorithm ever meet tangentially.
 * <br><br>
 * End capping (for creation of open solid bodies) is supported only for planar end profiles.
 * If the end capping operation fails then the sheet body constructed is returned.
 * <br><br>
 * <b>Effect:</b> Changes model
 * <br><br>
 * <b>Journal: </b> Not Available
 * <br><br>
 * <b>Product(s):</b> 3D ACIS Modeler
 * <br><br>
* @param num_entities
 * number of wire bodies.
 * @param in_entities
 * list of wire bodies.
 * @param path
 * wire body defining the path.
 * @param sheet_body
 * resulting sheet body.
 * @param opts
 * skin options.
 * @param ao
 * ACIS options such as version and journal.
 **/
DECL_SKIN outcome api_skin_wires(
		int num_entities,				// number of wire bodies
		ENTITY_LIST &in_entities,		// wires edges, or face to skin
		BODY *path,						// Wire Path
		BODY *&sheet_body,				// output body
		skin_options *opts, 			// skin options	arc_length, no_twist, align, perpendicular, 
                                        // simplify, closed, solid, sweep_path
		AcisOptions *ao = NULL
 );

/**
 * Creates a sheet or solid body that fits a %surface (or set of surfaces) through multiple sequences of wire profiles 
 * with the additional constraint of take-off vectors being normal to the profiles.
 * <br><br>
 * <b>Technical Articles:</b> <i>[Skinning](http://doc.spatial.com/articles/s/k/i/Component~Skinning_95b7.html)</i>, 
 * <i>[Functional Variations of Skinning](http://doc.spatial.com/articles/f/u/n/Functional_Variations_of_Skinning_c89c.html)</i>
 * <br><br>
 * <b>Role:</b>  The resulting skinned surfaces are parametric surfaces (parameterized by <i>u</i> and <i>v</i>) 
 * in which the user defined sequence of wires provide the <i>u</i> parameter of the %surface and the
 * skin %surface algorithm defines the <i>v</i> parameter.
 * <br><br>
 * The given wire profiles define the cross sections to be interpolated by the resulting sheet body with the additional
 * constraint of take-off vectors being normal to the profiles. This overloadeded version of <tt>api_skin_wires</tt> supports
 * the <i>Branched Skinning</i> functional variation of skinning. The resulting body will contain one or more skinned surfaces.
 * (With the exception of the <tt>simplify</tt> option being <tt>TRUE</tt>, in which the surface(s) might be converted to an applicable
 * conical type.)
 * <br><br>
 * If edges in every given wire profiles are tangential to each other, then the skinned surfaces
 * in the resultant body will also be tangential to each other along the lateral (side) edges.
 * This is however not true in certain special situations. Refer to the Technical Article 
 * <i>Lateral Edge Tangency in Skinning and Lofting</i> for more details.
 * <br><br>
 * Branched skinning differs from all other forms of skinning in that it allows multiple lists of profiles to be skinned. The
 * first and second parameters pertain to the "trunk" of the skin. Although the order of cross sections is not important in regular
 * skinning, it is important here that the last cross section of the trunk be the section in which each of the branches will attach.
 * Parameters three, four and five pertain to the branches of the skin. Branched skinning can handle any number of branches but most
 * practical applications will only require a few. Like the trunk, the order of the cross sections in each branch is important. The
 * first cross section must be the section that attaches to the trunk.
 * <br><br>
 * Each branch and trunk must contain at least one profile.
 * <br><br>
 * In addition, this functional variant includes the enumerated type skinning_normals. It does not support degenerate end points.
 * <br><br>
 * The <tt>api_skin_wires</tt> function also accepts the @href skin_options class.
 * <br><br>
 * <b>Effect:</b> Changes model
 * <br><br>
 * <b>Journal: </b> Available
 * <br><br>
 * <b>Product(s):</b> 3D ACIS Modeler
 * <br><br>
* @param num_wires
 * number of wire bodies.
 * @param wires
 * array of wire bodies.
 * @param num_branches
 * number of branches.
 * @param count_list
 * list of number of wire bodies per branch.
 * @param branches
 * array of array of wire bodies.
 * @param sheet_body
 * resulting sheet body or solid.
 * @param in_normals
 * skinning normals.
 * @param opts
 * skinning options.
 * @param ao
 * ACIS options such as version or journal.
 **/
DECL_SKIN outcome api_skin_wires(
		int num_wires,					// Number of wires in trunk
		BODY **wires,					// Pointers to wires in trunk
		int num_branches,				// Number of branches
		int *count_list,			    // Number of wires in each branch
		BODY ***branches,				// Pointers to wires in each branch
		BODY *&sheet_body,				// Output body
		skinning_normals in_normals,	// Normal considitions
		skin_options *opts, 			// skin options	arc_length, no_twist, align, perpendicular, simplify, closed, solid
		AcisOptions *ao = NULL
 );

/*
// tbrv
 */
/**
 * @nodoc
 */
DECL_SKIN outcome api_skin_wires(
		ENTITY_LIST&,			// List of wires
		ENTITY_LIST&,			// List of guides
		BODY*&,					// output
		logical = FALSE,		// arc length option STI jmg
		logical = TRUE,			// Twist option
		logical = TRUE,			// Align direction of wires.
		logical = FALSE,		// Simplify geometry
		logical = FALSE,		// construct a closed/open body for TRUE/FALSE
		logical = TRUE,	   	    // construct a solid/sheet body for TRUE/FALSE
		logical = FALSE,		// use virtual guide curves
		AcisOptions* = NULL
);

/**
 * Creates a sheet or solid body that fits a %surface (or set of surfaces) through a sequence of wire profiles 
 * with the additional constraint of guide curves.
 * <br><br>
 * <b>Technical Articles:</b> <i>[Skinning](http://doc.spatial.com/articles/s/k/i/Component~Skinning_95b7.html)</i>, 
 * <i>[Functional Variations of Skinning](http://doc.spatial.com/articles/f/u/n/Functional_Variations_of_Skinning_c89c.html)</i>
 * <br><br>
 * <b>Role:</b> The resulting skinned surfaces are parametric surfaces (parameterized by <i>u</i> and <i>v</i>)
 * in which the user defined sequence of wires provide the <i>u</i> parameter of the %surface and the guide curves and skin %surface
 * algorithm define the <i>v</i> parameter.
 * <br><br>
 * The given wires define the cross sections to be interpolated by the resulting sheet body with the
 * additional constraint of guide curves defining the body in the <i>v</i> direction. This overloaded version of <tt>api_skin_wires</tt>
 * supports the <i>Skinning With Guide Curves</i> functional variation of skinning. The resulting body will contain one or more skinned surfaces.
 * (With the exception of the <tt>simplify</tt> option being <tt>TRUE</tt>, in which the surface(s) might be converted to an applicable conical type.)
 * <br><br>
 * If edges in every given wire profiles are tangential to each other, then the skinned surfaces
 * in the resultant body will also be tangential to each other along the lateral (side) edges.
 * This is however not true in certain special situations. Refer to the Technical Article 
 * <i>Lateral Edge Tangency in Skinning and Lofting</i> for more details.
 * <br><br>
 * In all cases of skinning the cross section profiles are copies; that is, the originals remain. The profiles can be
 * either all open or all closed. If the user provides a set of closed profiles, the face normals of the skin body point
 * outside, away from the body material. When the user provides a set of open profiles, the face normals of the skin face
 * are oriented along the %surface normals, and no attempt is made to change the face normal orientation.
 * <br><br>
 * Skinning with guide curves accepts a list of guides in addition to the cross section profiles. The guide curves
 * "stretch" across the profiles from the first to last and control the %surface in the <i>v</i> direction. The guides are
 * accepted as <tt>EDGEs</tt> as well as wire bodies. The %curve underlying the guide must be C<sup>1</sup> continuous and non-looping. 
 * Therefore, if wire bodies are supplied in the <tt>guide_list</tt>, then all edges of the each such wire body must be C<sup>1</sup> continuous 
 * and non-looping. In addition, the guide must touch each wire profile within <tt>SPAresfit</tt> and START and STOP exactly 
 * on the first and last profiles. Direction is not important. The guides can intersect the section profiles on or off vertices. 
 * If the guide does not lie on any vertices, no %surface edge is created. If the guide intersects the first profile off a vertex but
 * intersects any other profile on a vertex, a %surface edge is made. In addition to accepting guides the overloaded API
 * supporting guide curves takes the <tt>logical</tt> flag <tt>virtualGuides</tt>. This flag toggles the guides from being local %surface
 * control to global %surface control. For example, in the case of local %surface control, the guide %curve only affects the
 * %surface created from the edges of the wire that the guide intersects. In the case of <tt>virtualGuides</tt> being <tt>TRUE</tt>, the
 * guide affects all the surfaces made from the wire body cross sections. Skinning with guide curves supports degenerate wires for
 * the first and last wire only.
 * <br><br>
 * The <tt>api_skin_wires</tt> function also accepts the @href skin_options class.
 * <br><br>
 * <b>Limitations:</b> The wire bodies are assumed to be simple; that is, they must be well behaved
 * and non-looping.
 * <br><br>
 * The parameterization of the %surface in its <i>u</i> direction - the direction of the user-defined
 * wire bodies - follows the parameterization of the curves underlying the wire bodies. (See
 * the skin option <tt>arc_length_u</tt> to alter this behavior.)
 * <br><br>
 * Skinning cannot produce a %surface in which there is any one point on the %surface in which
 * the <i>u</i> and <i>v</i> directions are the same or opposite. That is, in no case can an underlying %curve
 * of the user-defined wire body and a lateral edge of the %surface generated from the skinning
 * algorithm ever meet tangentially.
 * <br><br>
 * End capping (for creation of open solid bodies) is supported only for planar end profiles.
 * If the end capping operation fails then the sheet body constructed is returned.
 * <br><br>
 * <b>Effect:</b> Changes model
 * <br><br>
 * <b>Journal: </b> Not Available
 * <br><br>
 * <b>Product(s):</b> 3D ACIS Modeler
 * <br><br>
* @param wire_list
 * list of wire bodies.
 * @param guide_list
 * list of guide curves (edges).
 * @param sheet_body
 * resulting sheet body or solid.
 * @param opts
 * skin options.
 * @param ao
 * ACIS options such as version and journal.
 **/
DECL_SKIN outcome api_skin_wires(
		ENTITY_LIST &wire_list,			// List of wires
		ENTITY_LIST &guide_list,		// List of guides
		BODY *&sheet_body,				// output body
		skin_options *opts, 			// skin options	arc_length, no_twist, align, perpendicular, simplify, closed, solid
		AcisOptions *ao = NULL
 );

/*
// tbrv
 */
/**
 * @nodoc
 */
DECL_SKIN outcome api_skin_wires(
		int,					// Number of wires
		BODY*[],				// Pointers to wires
		int,					// Number of guides
		EDGE*[],				// Pointers to guides
		BODY*&,					// output
		logical = FALSE,		// arc length option STI jmg
		logical = TRUE,			// Twist option
		logical = TRUE,			// Align direction of wires.
		logical = TRUE,			// Simplify geometry
		logical = FALSE,		// construct a closed/open body for TRUE/FALSE
		logical = TRUE, 		// construct a solid/sheet body for TRUE/FALSE
		logical = FALSE,		// use virtual guide curves
		AcisOptions* = NULL
);

/**
 * Creates a sheet or solid body that fits a %surface (or set of surfaces) through a sequence of wire profiles 
 * with the additional constraint of guide curves.
 * <br><br>
 * <b>Technical Articles:</b> <i>[Skinning](http://doc.spatial.com/articles/s/k/i/Component~Skinning_95b7.html)</i>, 
 * <i>[Functional Variations of Skinning](http://doc.spatial.com/articles/f/u/n/Functional_Variations_of_Skinning_c89c.html)</i>
 * <br><br>
 * <b>Role:</b> The resulting skinned surfaces are parametric surfaces (parameterized by <i>u</i> and <i>v</i>)
 * in which the user defined sequence of wires provide the <i>u</i> parameter of the %surface and the guide curves and skin %surface
 * algorithm define the <i>v</i> parameter.
 * <br><br>
 * The given wires define the cross sections to be interpolated by the resulting sheet body with the
 * additional constraint of guide curves defining the body in the <i>v</i> direction. This overloaded version of <tt>api_skin_wires</tt>
 * supports the <i>Skinning With Guide Curves</i> functional variation of skinning. The resulting body will contain one or more skinned surfaces.
 * (With the exception of the <tt>simplify</tt> option being <tt>TRUE</tt>, in which the surface(s) might be converted to an applicable conical type.)
 * <br><br>
 * If edges in every given wire profiles are tangential to each other, then the skinned surfaces
 * in the resultant body will also be tangential to each other along the lateral (side) edges.
 * This is however not true in certain special situations. Refer to the Technical Article 
 * <i>Lateral Edge Tangency in Skinning and Lofting</i> for more details.
 * <br><br>
 * In all cases of skinning the cross section profiles are copies; that is, the originals remain. The profiles can be
 * either all open or all closed. If the user provides a set of closed profiles, the face normals of the skin body point
 * outside, away from the body material. When the user provides a set of open profiles, the face normals of the skin face
 * are oriented along the %surface normals, and no attempt is made to change the face normal orientation.
 * <br><br>
 * Skinning with guide curves accepts an array of guides in addition to the cross section profiles. The guide curves
 * "stretch" across the profiles from the first to last and control the %surface in the <i>v</i> direction. The guides are
 * accepted as <tt>EDGEs</tt>; never as wire bodies. The %curve underlying the guide must be C<sup>1</sup> continuous and non-looping.  
 * In addition, the guide must touch each wire profile within <tt>SPAresfit</tt> and START and STOP exactly 
 * on the first and last profiles. Direction is not important. The guides can intersect the section profiles on or off vertices. 
 * If the guide does not lie on any vertices, no %surface edge is created. If the guide intersects the first profile off a vertex but
 * intersects any other profile on a vertex, a %surface edge is made. In addition to accepting guides the overloaded API
 * supporting guide curves takes the <tt>logical</tt> flag <tt>virtualGuides</tt>. This flag toggles the guides from being local %surface
 * control to global %surface control. For example, in the case of local %surface control, the guide %curve only affects the
 * %surface created from the edges of the wire that the guide intersects. In the case of <tt>virtualGuides</tt> being <tt>TRUE</tt>, the
 * guide affects all the surfaces made from the wire body cross sections. Skinning with guide curves supports degenerate wires for
 * the first and last wire only.
 * <br><br>
 * The <tt>api_skin_wires</tt> function also accepts the @href skin_options class.
 * <br><br>
 * <b>Limitations:</b> The wire bodies are assumed to be simple; that is, they must be well behaved and non-looping.
 * <br><br>
 * The parameterization of the %surface in its <i>u</i> direction - the direction of the user-defined
 * wire bodies - follows the parameterization of the curves underlying the wire bodies. (See
 * the skin option <tt>arc_length_u</tt> to alter this behavior.)
 * <br><br>
 * Skinning cannot produce a %surface in which there is any one point on the %surface in which
 * the <i>u</i> and <i>v</i> directions are the same or opposite. That is, in no case can an underlying %curve
 * of the user-defined wire body and a lateral edge of the %surface generated from the skinning
 * algorithm ever meet tangentially.
 * <br><br>
 * End capping (for creation of open solid bodies) is supported only for planar end profiles.
 * If the end capping operation fails then the sheet body constructed is returned.
 * <br><br>
 * <b>Effect:</b> Changes model
 * <br><br>
 * <b>Journal: </b> Available
 * <br><br>
 * <b>Product(s):</b> 3D ACIS Modeler
 * <br><br>
* @param num_wires
 * number of wires.
 * @param wires
 * array of wire bodies.
 * @param no_of_guides
 * number of guide curves.
 * @param guides
 * array of guide curves (edges).
 * @param sheet_body
 * resulting sheet body or solid.
 * @param opts
 * skinning options.
 * @param ao
 * ACIS options such as version or journal.
 **/
DECL_SKIN outcome api_skin_wires(
		int num_wires,				// Number of wires
		BODY *wires[],				// Pointers to wires
		int no_of_guides,			// Number of guides
		EDGE *guides[],				// Pointers to guides
		BODY *&sheet_body,			// output body
		skin_options *opts, 		// skin options	arc_length, no_twist, align, perpendicular, simplify, closed, solid
		AcisOptions *ao = NULL
 );

/*
// tbrv
 */
/**
 * @nodoc
 */
DECL_SKIN outcome api_skin_wires(
		int             num_wires,					// Number of wires
		BODY*           wires[],					// Pointers to wires
		double			draft_start,				// Draft angle of start wire
		double			draft_end,				    // Draft angle of ending wire
		double			draft_start_mag,			// Magnitude of start SPAvector
		double			draft_end_mag,			    // Magnitude of ending SPAvector
		BODY*&          sheet_body,					// Sheet body to be returned
		logical = FALSE,							// Arc length option
		logical = TRUE,								// Twist option
		logical = TRUE,								// Align direction of wires
		logical = TRUE,								// Simplify geometry
		logical = TRUE,	 							// Construct a solid/sheet body for TRUE/FALSE
		AcisOptions* = NULL
);

/*
// tbrv
 */
/**
 * @nodoc
 */
DECL_SKIN outcome api_skin_wires(
		int             num_wires,					// Number of wires
		BODY*           wires[],					// Pointers to wires
		double			draft_start,				// Draft angle of start wire
		double			draft_end,				    // Draft angle of ending wire
        int             num_of_guides,              // Number of Guides
        EDGE*           guides[],                   // Guides
		BODY*&          sheet_body,					// Sheet body to be returned
		skin_options*, 								// skin options	arc_length, no_twist, align, perpendicular, simplify, closed, solid
		AcisOptions* = NULL
);

/**
 * Creates a sheet or solid body that fits a %surface (or set of surfaces) through a sequence of wire profiles 
 * with the additional constraint of take-off vectors being defined by a draft angle.
 * <br><br>
 * <b>Technical Articles:</b> <i>[Skinning](http://doc.spatial.com/articles/s/k/i/Component~Skinning_95b7.html)</i>, 
 * <i>[Functional Variations of Skinning](http://doc.spatial.com/articles/f/u/n/Functional_Variations_of_Skinning_c89c.html)</i>
 * <br><br>
 * <b>Role:</b> The resulting skinned surfaces are parametric surfaces (parameterized by <i>u</i> and <i>v</i>) 
 * in which the user defined sequence of wires provide the <i>u</i> parameter of the %surface and the
 * skin %surface algorithm defines the <i>v</i> parameter.
 * <br><br>
 * The given wire profiles define the cross sections to be interpolated by the resulting sheet body with the additional
 * constraint of take-off vectors being defined by a draft angle. This overloadeded version of <tt>api_skin_wires</tt> supports
 * the <i>Skinning With A Draft Angle</i> functional variation of skinning. The resulting body will contain one or more skinned surfaces.
 * (With the exception of the <tt>simplify</tt> option being <tt>TRUE</tt>, in which the surface(s) might be converted to an applicable
 * conical type.)
 * <br><br>
 * If edges in every given wire profiles are tangential to each other, then the skinned surfaces
 * in the resultant body will also be tangential to each other along the lateral (side) edges.
 * This is however not true in certain special situations. Refer to the Technical Article 
 * <i>Lateral Edge Tangency in Skinning and Lofting</i> for more details.
 * <br><br>
 * The draft angle is specified in radians as an angle defined by a vector and the %plane of the wire profile. A draft angle can be
 * specified for the first and last profile only. If the start or end profiles are degenerate points or single %straight
 * lines, the wire normal used in the calculation the take-off direction will be computed for you. However, if you wish to supply your own,
 * there are two additional parameters that accept normal vectors for the first and last profiles. These vectors are only accepted in the
 * case where the end profiles are degenerate or do not have their own intrinsic normal (e.g., %straight lines).
 * <br><br>
 * Skinning with a draft angle does not support the <i>closed</i> or <i>periodic</i> options.
 * <br><br>
 * <b>Errors:</b> A wire pointer in the array is <tt>NULL</tt> or not to a wire body.
 * <br><br>
 * <b>Limitations:</b> The wire bodies are assumed to be simple; that is, they must be well behaved
 * and non-looping.
 * <br><br>
 * The parameterization of the %surface in its <i>u</i> direction - the direction of the user-defined
 * wire bodies - follows the parameterization of the curves underlying the wire bodies.
 * <br><br>
 * Skinning cannot produce a %surface in which there is any one point on the %surface in which
 * the <i>u</i> and <i>v</i> directions are the same or opposite. That is, in no case can an underlying curve
 * of the user-defined wire body and a lateral edge of the %surface generated from the skinning
 * algorithm ever meet tangentially.
 * <br><br>
 * End capping (for creation of open solid bodies) is supported only for planar end profiles.
 * If the end capping operation fails then the sheet body constructed is returned.
 * <br><br>
 * The internal Boolean operations performed during the branched skinning operation are subject to the
 * limitations of the Boolean engine.
 * <br><br>
 * <b>Effect:</b> Changes model
 * <br><br>
 * <b>Journal: </b> Not Available
 * <br><br>
 * <b>Product(s):</b> 3D ACIS Modeler
 * <br><br>
* @param num_wires
 * number of wire bodies.
 * @param wires
 * array of wire bodies.
 * @param draft_start
 * draft angle of first wire.
 * @param draft_end
 * draft angle of last wire.
 * @param draft_start_mag
 * magnitude of first draft.
 * @param draft_end_mag
 * magnitude of second draft.
 * @param sheet_body
 * resulting sheet body or solid.
 * @param opts
 * skinning options.
 * @param ao
 * ACIS options such as version and Journal.
 **/
DECL_SKIN outcome api_skin_wires(
		int             num_wires,					// Number of wires
		BODY           *wires[],					// Pointers to wires
		double			draft_start,				// Draft angle of start wire
		double			draft_end,				    // Draft angle of ending wire
		double			draft_start_mag,			// Magnitude of start SPAvector
		double			draft_end_mag,			    // Magnitude of ending SPAvector
		BODY          *&sheet_body,					// Sheet body to be returned
		skin_options   *opts, 						// skin options	arc_length, no_twist, align, perpendicular, simplify, closed, solid
		AcisOptions    *ao = NULL
 );

/*
// tbrv
 */
/**
 * @nodoc
 */
DECL_SKIN outcome api_skin_wires(
		int             num_wires,					// Number of wires
		BODY*           wires[],					// Pointers to wires
		double			draft_start,				// Draft angle of start wire
		double			draft_end,				    // Draft angle of ending wire
		double			draft_start_mag,			// Magnitude of start SPAvector
		double			draft_end_mag,			    // Magnitude of ending SPAvector
		const SPAvector &start_normal,				// Plane normal of first wire
		const SPAvector &end_normal,				    // Plane normal of last wire
		BODY*&          sheet_body,					// Sheet body to be returned
		logical = FALSE,							// Arc length option
		logical = TRUE,								// Twist option
		logical = TRUE,								// Align direction of wires
		logical = TRUE,								// Simplify geometry
		logical = TRUE,	 							// Construct a solid/sheet body for TRUE/FALSE
		AcisOptions* = NULL
);

/**
 * Creates a sheet or solid body that fits a %surface (or set of surfaces) through a sequence of wire profiles 
 * with the additional constraint of take-off vectors being defined by a draft angle.
 * <br><br>
 * <b>Technical Articles:</b> <i>[Skinning](http://doc.spatial.com/articles/s/k/i/Component~Skinning_95b7.html)</i>, 
 * <i>[Functional Variations of Skinning](http://doc.spatial.com/articles/f/u/n/Functional_Variations_of_Skinning_c89c.html)</i>
 * <br><br>
 * <b>Role:</b> The resulting skinned surfaces are parametric surfaces (parameterized by <i>u</i> and <i>v</i>) 
 * in which the user defined sequence of wires provide the <i>u</i> parameter of the %surface and the
 * skin %surface algorithm defines the <i>v</i> parameter.
 * <br><br>
 * The given wire profiles define the cross sections to be interpolated by the resulting sheet body with the additional
 * constraint of take-off vectors being defined by a draft angle. This overloadeded version of <tt>api_skin_wires</tt> supports
 * the <i>Skinning With A Draft Angle</i> functional variation of skinning. The resulting body will contain one or more skinned surfaces.
 * (With the exception of the <tt>simplify</tt> option being <tt>TRUE</tt>, in which the surface(s) might be converted to an applicable
 * conical type.)
 * <br><br>
 * If edges in every given wire profiles are tangential to each other, then the skinned surfaces
 * in the resultant body will also be tangential to each other along the lateral (side) edges.
 * This is however not true in certain special situations. Refer to the Technical Article 
 * <i>Lateral Edge Tangency in Skinning and Lofting</i> for more details.
 * <br><br>
 * The draft angle is specified in radians as an angle defined by a vector and the %plane of the wire profile. A draft angle can be
 * specified for the first and last profile only. If the start or end profiles are degenerate points or single %straight
 * lines, the wire normal used in the calculation the take-off direction will be computed for you. However, if you wish to supply your own,
 * there are two additional parameters that accept normal vectors for the first and last profiles. These vectors are only accepted in the
 * case where the end profiles are  degenerated or do not have their own intrinsic normal (e.g., %straight lines).
 * <br><br>
 * Skinning with a draft angle does not support the <i>closed</i> or <i>periodic</i> options.
 * <br><br>
 * <b>Errors:</b> A wire pointer in the array is <tt>NULL</tt> or not to a wire body.
 * <br><br>
 * <b>Limitations:</b> The wire bodies are assumed to be simple; that is, they must be well behaved
 * and non-looping.
 * <br><br>
 * The parameterization of the %surface in its <i>u</i> direction - the direction of the user-defined
 * wire bodies - follows the parameterization of the curves underlying the wire bodies.
 * <br><br>
 * Skinning cannot produce a %surface in which there is any one point on the %surface in which
 * the <i>u</i> and <i>v</i> directions are the same or opposite. That is, in no case can an underlying curve
 * of the user-defined wire body and a lateral edge of the %surface generated from the skinning
 * algorithm ever meet tangentially.
 * <br><br>
 * End capping (for creation of open solid bodies) is supported only for planar end profiles.
 * If the end capping operation fails then the sheet body constructed is returned.
 * <br><br>
 * The internal Boolean operations performed during the branched skinning operation are subject to the
 * limitations of the Boolean engine.
 * <br><br>
 * <b>Effect:</b> Changes model
 * <br><br>
 * <b>Journal: </b> Available
 * <br><br>
 * <b>Product(s):</b> 3D ACIS Modeler
 * <br><br>
* @param num_wires
 * number of wire bodies.
 * @param wires
 * array of wire bodies.
 * @param draft_start
 * draft angle of first wire.
 * @param draft_end
 * draft angle of last wire.
 * @param draft_start_mag
 * magnitude of first draft.
 * @param draft_end_mag
 * magnitude of second draft.
 * @param start_normal
 * normal plane for first wire.
 * @param end_normal
 * normal plane for last wire.
 * @param sheet_body
 * resulting sheet body or solid.
 * @param opts
 * skinning options.
 * @param ao
 * ACIS options such as  version and Journal.
 **/
DECL_SKIN outcome api_skin_wires(
		int             num_wires,					// Number of wires
		BODY           *wires[],					// Pointers to wires
		double			draft_start,				// Draft angle of start wire
		double			draft_end,				    // Draft angle of ending wire
		double			draft_start_mag,			// Magnitude of start SPAvector
		double			draft_end_mag,			    // Magnitude of ending SPAvector
		const SPAvector &start_normal,				// Plane normal of first wire
		const SPAvector &end_normal,				// Plane normal of last wire
		BODY           *&sheet_body,				// Sheet body to be returned
		skin_options   *opts, 						// skin options	arc_length, no_twist, align, perpendicular, simplify, closed, solid
		AcisOptions    *ao = NULL
 );

/*
// tbrv
 */
/**
 * @nodoc
 */
DECL_SKIN outcome api_skin_faces(
		FACE*,					// face 1
		FACE*,					// face 2
		BODY *&,				// output
		logical = FALSE,        // arc length option STI jmg
		logical = TRUE,			// Twist option
		logical = TRUE,			// Align direction of wires.
		logical = TRUE,	        // Simplify geometry
		AcisOptions* = NULL
	);

/**
 * Unites two bodies using skinning between two faces.
 * <br><br>
 * <b>Role:</b> If one the bodies is a solid, the operation is different than if it were a sheet body.
 * In the case of a solid body, the face provided as an argument is removed from the body.
 * Then, that face's peripheral edges are used to skin to the peripheral edges of the second face.
 * The result is one body. 
 * <br><br>
 * This does not check to see if the resulting body is self-intersecting.
 * Skinning does not use take-off vectors of the associated faces.
 * <br><br>
 * When ACIS makes a <tt>BODY</tt> from a <tt>FACE</tt> using <tt>api_skin_faces</tt>, it makes a single-sided face body.
 * A single-sided face body is really a solid, not a sheet (infinitely thin) body.
 * When a face body is single-sided, basic solid modeling concepts (and ACIS) consider the body to
 * be a solid which extends from the back side of the face out to infinity with ill-defined
 * boundaries extending where the edges of the original face extend backward.
 * Subsequent operations, such as Boolean operations, may not work, depending on how the single-sided
 * face body is being used.
 * <br><br>
 * <b>Effect:</b> Changes model
 * <br><br>
 * <b>Journal: </b> Available
 * <br><br>
 * <b>Product(s):</b> 3D ACIS Modeler
 * <br><br>
* @param face1
 * first face.
 * @param face2
 * second face.
 * @param body
 * solid body returned.
 * @param opts
 * skinning options.
 * @param ao
 * ACIS options such as version or journal.
 **/
DECL_SKIN outcome api_skin_faces(
		FACE *face1,						// face 1
		FACE *face2,						// face 2
		BODY *&body,						// output body
		skin_options *opts, 				// skin options	arc_length, no_twist, align, perpendicular, simplify, closed, solid
		AcisOptions *ao = NULL
 );

/*! @} */
/*! \addtogroup NETAPI
 *  \brief Declared at <skinapi.hxx>, SPAskin
 *  @{
 */
/*
// tbrv
 */
/**
 * @nodoc
 */
DECL_SKIN outcome api_net_wires(
		int,					// number of V wire bodies
		BODY*[],				// array of V wire bodies to surfaced
		int,					// number of U wire bodies
		BODY*[],				// array of U wire bodies to surfaced
		BODY *&,				// output
		logical = TRUE,			// Align direction of wires.
		logical = FALSE,		// Simplify geometry
		double  = SPAresfit,		// Net surface intersection tolerance
		AcisOptions* ao = NULL
);

/**
 * Creates a sheet body that fits a %surface through a mesh of wires contained in an array of bodies.
 * <br><br>
 * <b>Role:</b> The given bi-directional grid of wires define the cross sections to be interpolated by
 * the resulting sheet body. There must be at least two wires in each direction. As with skinning the wires
 * are copies; i.e., the originals remain. In addition the wires can be either all open or all closed.
 * <br><br>
 * If only two wires are given, one wire may be a degenerate line; that is, treated as a point. For example,
 * skinning between a degenerate line and a circular arc will give a conical spline with the "point" being the
 * apex of the conical %surface.
 * <br><br>
 * If all of the curves intersect, then the %surface passes through the curves and their intersections.
 * If any of the <i>u</i> curves of the net do not intersect all of <i>v</i> curves at some point, the intersection
 * is interpolated. The maximum distance for the interpolation is governed by the <tt>net_tol</tt> argument.
 * The default for this tolerance value is <tt>SPAresfit</tt>. If the tolerance is changed, a net %surface
 * can be created as long as the distance between <i>u</i> curves or between <i>v</i> curves (i.e., in the skin
 * direction) is larger than the tolerance intersection distance between a <i>u</i> %curve and a <i>v</i> %curve.
 * Thus, the %curve interpolating accuracy of the net %surface is controlled by the user's accuracy
 * of the intersections of the cross sections.
 * <br><br>
 * <b>Errors:</b> A wire pointer in the array is <tt>NULL</tt> or not to a wire body.
 * <br><br>
 * <b>Limitations:</b> <br><br>The wire bodies are assumed to be simple; that is, only the first wire of each body
 * is used for skinning. 
 * <br><br> Each wire must have continuous tangents (C<sup>1</sup>).
 * <br><br> Wires cannot share an end point (end points must be distinct).
 * <br><br> The %surface and lateral curves are always splines; that is, the function is not smart about using
 * analytic representations in special cases. A prop edge is inserted when the wires are closed.
 * <br><br> Net wires cannot produce a %surface in which there is any one point on the %surface in which the <i>u</i> and <i>v</i> 
 * directions are the same or opposite. That is, in no case can an underlying %curve of the user-defined wire body
 * in the <i>u</i> direction meet tangentially with a user-defined %curve of the wire body in the <i>v</i> direction. 
 * <br><br>
 * <b>Effect:</b> Changes model
 * <br><br>
 * <b>Journal: </b> Available
 * <br><br>
 * <b>Product(s):</b> 3D ACIS Modeler
 * <br><br>
* @param num_v_wires
 * number of wires in array.
 * @param v_wires
 * array of wires.
 * @param num_u_wires
 * number of wires in array.
 * @param u_wires
 * array of wires.
 * @param sheet_body
 * sheet body returned.
 * @param opts
 * skinning options.
 * @param net_tol
 * net %surface intersection tolerance.
 * @param ao
 * ACIS options such as version or journal.
 **/
DECL_SKIN outcome api_net_wires(
		int num_v_wires,				// number of V wire bodies
		BODY *v_wires[],				// array of V wire bodies to surfaced
		int num_u_wires,				// number of U wire bodies
		BODY *u_wires[],				// array of U wire bodies to surfaced
		BODY *&sheet_body,				// output body
		skin_options *opts,				// skin options
		double net_tol = SPAresfit,		// Net surface intersection tolerance
		AcisOptions *ao  = NULL
);

/**
 * Creates a sheet body that interpolates a series of sections.
 * <br><br>
 * <b>Role:</b> This class interpolates a %surface through a network of bi-directional curves.
 * The given sections define the cross-sections to be interpolated by the resulting sheet body.
 * There must be at least four sections or bodies, two in each direction.
 * The start points of the curves in the <i>v</i> direction must lie on the first %curve in the <i>u</i> direction,
 * and vice versa. The end points of the <i>v</i> curves must lie in the last %curve in the <i>u</i> direction, and
 * vice versa. All curves must intersect each other within <tt>SPAresabs</tt> in the interior of the %surface.
 * The sections/bodies are assumed to be simple, meaning only the first section of each body is used
 * for the net %surface. The sections can be open or closed. The sections are copies (the originals
 * remain). The sections should be C<sup>1</sup> continuous tangent.
 * <br><br>
 * If all of the curves (sections) intersect, then the %surface passes through the curves and their
 * intersections. If any of the <i>u</i> curves of the net do not intersect all of <i>v</i> curves at some point,
 * the intersection is interpolated. The maximum distance for the interpolation is governed by 
 * the <tt>net_tol</tt> argument. The default for this tolerance value is <tt>SPAresfit</tt>. 
 * If the tolerance is changed, a net %surface can be created as long as the distance between <i>u</i>
 * curves or between <i>v</i> curves (i.e., in the skin direction) is larger than the tolerance
 * intersection distance between a <i>u</i> %curve and a <i>v</i> %curve. Thus, the %curve interpolating accuracy of
 * the net %surface is controlled by the user's accuracy of the intersections of the cross-sections.
 * <br><br>
 * The optional argument <tt>simplify</tt> specifies whether or not to simplify the resulting %surface geometry.
 * The default is <tt>FALSE</tt>, which means not to simplify. If this option is <tt>TRUE</tt> and if all of the the
 * body-list elements are in the same %plane, the result is a plane surface bounded by the first
 * and last <i>uv</i> sections. If this option is <tt>FALSE</tt>, the result is a planar net %surface.
 * <br><br>
 * <b>Limitations:</b> <br><br> The sections are assumed to be simple; that is, only the first wire of each body
 * is used for skinning. 
 * <br><br> Each section must have continuous tangents (C<sup>1</sup>).
 * <br><br> Sections cannot share an end point (end points must be distinct).
 * <br><br> The %surface and lateral curves are always splines; that is, the function is not smart about using
 * analytic representations in special cases. A prop edge is inserted when the wires are closed.
 * <br><br> Net sections cannot produce a %surface in which there is any one point on the %surface in which the <i>u</i> and <i>v</i> 
 * directions are the same or opposite. That is, in no case can an underlying %curve of the user-defined wire body
 * in the <i>u</i> direction meet tangentially with a user-defined %curve of the wire body in the <i>v</i> direction. 
 * <br><br>
 * <b>Effect:</b> Changes model
 * <br><br>
 * <b>Journal: </b> Available
 * <br><br>
 * <b>Product(s):</b> 3D ACIS Modeler
 * <br><br>
* @param nv_sect
 * number of v sections.
 * @param v_sects
 * array of v cross sections.
 * @param nu_sect
 * number of u sections.
 * @param u_sects
 * array of u cross sections.
 * @param out_body
 * sheet body returned.
 * @param simplify
 * simplify geometry flag.
 * @param net_tol
 * net %surface intersect tolerance.
 * @param ao
 * ACIS options such as version or journal.
 **/
DECL_SKIN outcome api_net_sections(
		int	nv_sect,						 	// Number of v sections
		Loft_Connected_Coedge_List **v_sects,	// Array of coedges with laws
		int	nu_sect,		                 	// Number of u sections
		Loft_Connected_Coedge_List **u_sects,	// Array of coedges with laws
		BODY *&out_body,						// Skin body returned
		logical simplify = FALSE,				// Simplify geometry
		double net_tol = SPAresfit,				// Net surface intersection tolerance
		AcisOptions *ao = NULL
);

/*! @} */
/*! \addtogroup SLIAPI
 *  \brief Declared at <skinapi.hxx>, SPAskin
 *  @{
 */
// -----------------------------------------------------------------------------------------------------
//
// Interactive API's
//
// -----------------------------------------------------------------------------------------------------
/*
// tbrv
 */
/**
 * @nodoc
 */
DECL_SKIN outcome api_create_li(
		int,							// number of lofting coedges
		Loft_Connected_Coedge_List*,	// lofting coedges
		AcisLoftingInterface*&,			// AcisloftingInterface
		BODY**&,						// The wires that will be aligned and broken
		logical = FALSE,				// arc length parameterization option
		logical = TRUE,					// twist option
		logical = TRUE,					// align option
		logical = TRUE,					// simplify option
		logical = FALSE,				// perpendicular option
		logical = TRUE,					// solid option
		logical = FALSE,				// closed option
		AcisOptions* = NULL
	);

/**
 * Creates an <tt>AcisLoftingInterface</tt> object.
 * <br><br>
 * <b>Technical Articles:</b> <i>[Lofting](http://doc.spatial.com/articles/l/o/f/Lofting.html)</i>, 
 * <i>[Skinning and Lofting Interactive Interface](http://doc.spatial.com/articles/s/k/i/Skinning_and_Lofting_Interactive_Interface_1561.html)</i>
 * <br><br>
 * <b>Role:</b> This object is used as the input to additional skinning and lofting breakup APIs.
 * It contains all the given input and residual data created via the lofting process.
 * <br><br>
 * Commonly, the <tt>api_create_li</tt> takes a data structure called <tt>Loft_Connected_Coedge_List</tt>.
 * This is composed of the number of coedges in the cross section, a pointer to the first coedge in
 * a list of coedges, the cross section orientation, the take-off vector, weight factor, and an array
 * of laws. Every cross section used in lofting has to have this data structure.
 * At the very minimum, two <tt>Loft_Connected_Coedge_List</tt> structures need to be created.
 * <br><br>
 * The <tt>api_create_li</tt> function accepts the following special options depending on the specific API:
 * <br><br>
 * <ul>
 * <li> The <i>arc_length</i> option is used to choose arc length or isoparametric parameterization of the
 * skinning surfaces. For isoparametric parameterization, the %surface parameter in the <i>v</i> direction
 * follows the cross section curves. For arc length parameterization, the %surface parameter follows
 * lines of constant length. The default is isoparametric parameterization. Surfaces resulting from
 * skinning with guide curves support isoparametric parameterization only.
 * The default is <tt>FALSE</tt>.</li>
 *
 * <li> The <i>no_twist</i> option may be used to minimize the twist of the %surface produced.
 * Twist minimization aligns closed curves such that the start of the second %curve is aligned to the
 * start of the first %curve. Even if a body's shape is unaffected by twisting, a %surface with a twist
 * could produce unexpected results when faceting and rendering.The default is <tt>TRUE</tt>.</li>
 *
 * <li> The <i>align</i> option is used to align the direction of the cross section curves such that the normal
 * of the first profile points towards the second profile. All other profiles are aligned to follow
 * the first and second. If the sections are not oriented in the same direction, the <i>align</i> option
 * should be used to avoid producing a twisted, self-intersecting body. The default is <tt>TRUE</tt>.</li>
 *
 * <li> The <i>perpendicular</i> option specifies the direction of the take-off vector, either perpendicular
 * to the coedge or in the loft direction. The default is <tt>FALSE</tt> (that is, in the loft direction).</li>
 *
 * <li> The <i>simplify</i> option simplifies the %surface to a conical %surface, if applicable.
 * If all of the cross sections lie on a conical %surface (plane, cylinder, cone, sphere, or torus),
 * the conical %surface is created instead.The <tt>SPAresabs</tt> variable is used to determine whether or not
 * the cross section lies on a conical %surface. The default is <tt>TRUE</tt>.</li>
 *
 * <li> The <i>closed</i> option may be used when the user needs to construct a solid body closed in <i>v</i>
 * (e.g., a %torus).A solid body will be constructed only when all the wires supplied are closed.
 * At least three profiles must be provided to create a closed body. The default is <tt>FALSE</tt>.</li>
 *
 * <li> The <i>solid</i> option may be used when a solid body is desired, but not a solid body closed in the <i>v</i> direction. 
 * When a solid body that is not closed in the <i>v</i> direction is desired, the end wires are capped with planar faces.
 * The default is <tt>TRUE</tt>.</li>
 *
 * <li> The <i>periodic</i> option allows to construct loft bodies that are periodic in <i>v</i>; that is, bodies
 * that close back on themselves smoothly (continuously) at the start and end profiles.
 * As with the <i>closed</i> option, at least three profiles must be supplied to create a periodic loft body. 
 * The default is <tt>FALSE</tt>.</li>
 *
 * <li> The <i>virtualGuide</i> option my be used in order to have the user defined guides effect
 * the body in a global nature. The default is <tt>FALSE</tt>.</li>
 *
 * <li> When the <i>merge_wirecoedges</i> option is set to true, the G<sup>1</sup> vertices of the skinning and lofting
 * wire profiles are removed by merging adjacent coedges/edges. This improves operations such as
 * blending and shelling because it reduces the coedge/edge count and the number of surfaces and
 * eliminates near tangent edges. The default is <tt>TRUE</tt>.</li>
 *
 * <li> When the <i>estimate_loft_tanfacs</i> option is on, the weight factor for the tangency conditions
 * of the loft will be determined such that it minimizes the average radius of curvature of the
 * lofting surfaces. The resulting bodies should support shelling to greater thickness and also
 * blending of their edges to larger blend radii. The default is <tt>FALSE</tt>.</li>
 *
 * <li> The <i>match_vertices</i> option allows to suppress the vertex-matching-algorithm which ensures that
 * all profiles consist of the same number of coedges. A heuristic approach is used to determine
 * which vertex pairs are good matches.Profile coedges are then split where additional vertices
 * are needed. This option is forced to <tt>TRUE</tt> if the coedge numbers of the profiles are not equal.
 * The default is <tt>TRUE</tt>.</li>
 * </ul>
 * <br>
 * The algorithm that minimizes the %surface twist (see <i>no_twist</i> option) may add vertices to some
 * of the profiles if none of the existing vertices match well enough. The <i>no_new_twist_vertices</i>
 * option forces the algorithm to choose matching vertices from the existing vertices.
 * The default is <tt>FALSE</tt>.
 * <br><br>
 * <b>Limitations:</b> The coedge lists are assumed to be simple; that is, they must be well behaved and
 * non-looping. The parameterization of the %surface in its <i>u</i> direction - the direction of the coedge
 * list - follows the parameterization of the curves underlying the coedges.
 * <br><br>
 * Lofting cannot produce a %surface in which there is any one point on the %surface in which the
 * <i>u</i> and <i>v</i> directions are the same or opposite. That is, in no case can an underlying %curve of the
 * user-defined wire body and a lateral edge of the %surface generated from the lofting algorithm
 * ever meet tangentially. (Lofting will toggle the <i>perpendicular</i> option for you to avoid this
 * condition.) End capping (for creation of open solid bodies) is supported only for planar end
 * profiles. If the end capping operation fails then the sheet body constructed is returned.
 * <br><br>
 * <b>Effect:</b> Read-only
 * <br><br>
 * <b>Journal: </b> Available
 * <br><br>
 * <b>Product(s):</b> 3D ACIS Modeler
 * <br><br>
* @param number_coedges
 * number of coedge lists.
 * @param edgeList
 * array of coedge lists.
 * @param obj
 * interface object created.
 * @param wireBodies
 * array of temporary wire bodies.
 * @param opts
 * skin options.
 * @param ao
 * ACIS options such as version and journal.
 **/
DECL_SKIN outcome api_create_li(
		int number_coedges,						// number of lofting coedges
		Loft_Connected_Coedge_List *edgeList,	// lofting coedges
		AcisLoftingInterface *&obj,				// AcisloftingInterface returned
		BODY **&wireBodies,						// The wires that will be aligned and broken
		skin_options *opts, 			        // skin options	arc_length, no_twist, align, perpendicular, simplify, closed, solid
		AcisOptions *ao = NULL
	);

/*
// tbrv
 */
/**
 * @nodoc
 */
DECL_SKIN outcome api_create_li(
		FACE *face1,					// face 1
		double factor1,					// take off factor for coedges on face 1
		FACE *face2,					// face 2
		double factor2,					// take off factor for coedges on face 2
		AcisLoftingInterface*&,			// AcisLoftingInterface
		BODY**&,						// The wire bodies
		logical = FALSE,				// Arc length parameterization option
		logical = TRUE,					// If the co-edges are all closed minimize the surface twist.
		logical = TRUE,					// Align the directions of the co-edges so they are the same.
		logical = TRUE,					// Calculate the Loft direction perpendicular to the coedges
		logical = TRUE,					// Simplify geometry
		AcisOptions* = NULL
	);

/**
 * Creates an <tt>AcisLoftingInterface</tt> object for <i>Lofting Between Faces</i>.
 * <br><br>
 * <b>Technical Articles:</b> <i>[Lofting](http://doc.spatial.com/articles/l/o/f/Lofting.html)</i>, 
 * <i>[Skinning and Lofting Interactive Interface](http://doc.spatial.com/articles/s/k/i/Skinning_and_Lofting_Interactive_Interface_1561.html)</i>
 * <br><br>
 * <b>Role:</b>  This object is used as the input to additional skinning and lofting breakup APIs.
 * It contains all the given input and residual data created via the lofting process.
 * <br><br>
 * This signature of <tt>api_create_li</tt> is used for <i>Lofting Between Faces</i>. See the Technical
 * Article on <i>Functional Variations of Lofting</i> for more information on this variation of lofting.
 * This variation unites two bodies by creating a connection between one face of each body.
 * It accepts as input a face and a take-off vector scaling factor from the first body,
 * and a face and a take-off vector scaling factor from the second body.
 * <br><br>
 * The <tt>api_create_li</tt> function accepts the following special options depending on the specific API:
 * <br><br>
 * <ul>
 * <li> The <i>arc_length</i> option is used to choose arc length or isoparametric parameterization of the
 * skinning surfaces. For isoparametric parameterization, the %surface parameter in the <i>v</i> direction
 * follows the cross section curves. For arc length parameterization, the %surface parameter follows
 * lines of constant length. The default is isoparametric parameterization. Surfaces resulting from
 * skinning with guide curves support isoparametric parameterization only.
 * The default is <tt>FALSE</tt>.</li>
 *
 * <li> The <i>no_twist</i> option may be used to minimize the twist of the %surface produced.
 * Twist minimization aligns closed curves such that the start of the second %curve is aligned to the
 * start of the first %curve. Even if a body's shape is unaffected by twisting, a %surface with a twist
 * could produce unexpected results when faceting and rendering. The default is <tt>TRUE</tt>.</li>
 *
 * <li> The <i>align</i> option is used to align the direction of the cross section curves such that the normal
 * of the first profile points towards the second profile. All other profiles are aligned to follow
 * the first and second. If the sections are not oriented in the same direction, the <i>align</i> option
 * should be used to avoid producing a twisted, self-intersecting body. The default is <tt>TRUE</tt>.</li>
 *
 * <li> The <i>perpendicular</i> option specifies the direction of the take-off vector, either perpendicular
 * to the coedge or in the loft direction. The default is <tt>FALSE</tt> (that is, in the loft direction).</li>
 *
 * <li> The <i>simplify</i> option simplifies the %surface to a conical %surface, if applicable.
 * If all of the cross sections lie on a conical %surface (plane, cylinder, cone, sphere, or torus),
 * the conical %surface is created instead.The <tt>SPAresabs</tt> variable is used to determine whether or not
 * the cross section lies on a conical %surface. The default is <tt>TRUE</tt>.</li>
 *
 * <li> The <i>solid</i> option may be used when a solid body is desired, but not a solid body closed in the <i>v</i> direction. 
 * When a solid body that is not closed in the <i>v</i> direction is desired, the end wires are capped with planar faces.
 * The default is <tt>TRUE</tt>.</li>
 *
 * <li> The <i>virtualGuide</i> option my be used in order to have the user defined guides effect
 * the body in a global nature. The default is <tt>FALSE</tt>.</li>
 *
 * <li> When the <i>merge_wirecoedges</i> option is set to true, the G<sup>1</sup> vertices of the skinning and lofting
 * wire profiles are removed by merging adjacent coedges/edges. This improves operations such as
 * blending and shelling because it reduces the coedge/edge count and the number of surfaces and
 * eliminates near tangent edges. The default is <tt>TRUE</tt>.</li>
 *
 * <li> When the <i>estimate_loft_tanfacs</i> option is on, the weight factor for the tangency conditions
 * of the loft will be determined such that it minimizes the average radius of curvature of the
 * lofting surfaces. The resulting bodies should support shelling to greater thickness and also
 * blending of their edges to larger blend radii. The default is <tt>FALSE</tt>.</li>
 *
 * <li> The <i>match_vertices</i> option allows to suppress the vertex-matching-algorithm which ensures that
 * all profiles consist of the same number of coedges. A heuristic approach is used to determine
 * which vertex pairs are good matches. Profile coedges are then split where additional vertices
 * are needed. This option is forced to <tt>TRUE</tt> if the coedge numbers of the profiles are not equal.
 * The default is <tt>TRUE</tt>.</li>
 * </ul>
 * <br>
 * The algorithm that minimizes the %surface twist (see <i>no_twist</i> option) may add vertices to some
 * of the profiles if none of the existing vertices match well enough. The <i>no_new_twist_vertices</i>
 * option forces the algorithm to choose matching vertices from the existing vertices.
 * The default is <tt>FALSE</tt>.
 * <br><br>
 * <b>Limitations:</b> The coedge lists are assumed to be simple; that is, they must be well behaved and
 * non-looping. The parameterization of the %surface in its <i>u</i> direction - the direction of the coedge
 * list - follows the parameterization of the curves underlying the coedges.
 * <br><br>
 * Lofting cannot produce a %surface in which there is any one point on the %surface in which the
 * <i>u</i> and <i>v</i> directions are the same or opposite. That is, in no case can an underlying %curve of the
 * user-defined wire body and a lateral edge of the %surface generated from the lofting algorithm
 * ever meet tangentially. (Lofting will toggle the <i>perpendicular</i> option for you to avoid this
 * condition.) End capping (for creation of open solid bodies) is supported only for planar end
 * profiles. If the end capping operation fails then the sheet body constructed is returned.
 * <br><br>
 * <b>Effect:</b> Read-only
 * <br><br>
 * <b>Journal: </b> Available
 * <br><br>
 * <b>Product(s):</b> 3D ACIS Modeler
 * <br><br>
* @param face1
 * face 1.
 * @param factor1
 * take-off factor for coedges on face 1.
 * @param face2
 * face 2.
 * @param factor2
 * take-off factor for coedges on face 2.
 * @param obj
 * interface object created.
 * @param wireBodies
 * array of temporary wire bodies.
 * @param opts
 * skin options.
 * @param ao
 * ACIS options such as version and journal.
 **/
DECL_SKIN outcome api_create_li(
		FACE *face1,					// face 1
		double factor1,					// take-off factor for coedges on face 1
		FACE *face2,					// face 2
		double factor2,					// take-off factor for coedges on face 2
		AcisLoftingInterface *&obj,		// AcisLoftingInterface returned
		BODY **&wireBodies,				// The wire bodies
		skin_options *opts, 			// skin options arc_length, no_twist, align, perpendicular, simplify, closed, solid
		AcisOptions *ao = NULL
 );

/*
 // tbrv
 */
/**
 * @nodoc
 */
DECL_SKIN outcome api_create_si(
		ENTITY_LIST&,					// List of wires
		AcisSkinningInterface*&,		// SkinningInterface object
		BODY**&,						// Array of created wire bodies
		BODY* = NULL,					// Optional Skinning path
		logical = FALSE,				// arc length option
		logical = TRUE,					// twist option
		logical = TRUE,					// align option
		logical = TRUE, 				// simplify option
		logical = TRUE,					// construct a solid/sheet body for TRUE/FALSE
		logical = FALSE,				// construct a closed/open body for TRUE/FALSE
		logical = FALSE,				// virtual guide flag
		AcisOptions* = NULL
	);

/**
 * Creates an <tt>AcisSkinningInterface</tt> object for either <i>Basic Skinning</i> or <i>Skinning with a Path</i>.
 * <br><br>
 * <b>Role:</b> This object is used as the input to additional skinning and lofting breakup APIs.
 * It contains all the given input and residual data created via the skinning process.
 * There are eight functional variations of skinning each differing by methods of %surface control;
 * however, only seven are supported through this API. Branched skinning is only supported through
 * <tt>api_skin_wires</tt>.
 * <br><br>
 * For a complete overview refer to the <i>Skinning</i> Technical Article.
 * This API has the following variations: <br>
 * <ul>
 * <li>Basic Skinning</li>
 * <li>Skinning with Vectors</li>
 * <li>Skinning with a Path</li>
 * <li>Skinning with Draft Angles</li>
 * <li>Skinning to the Planar Normal</li>
 * <li>Skinning with Guide Curves (or Virtual Guide Curves, if <tt>virtualGuides</tt> is set to <tt>TRUE</tt>)</li>
 * <li>Skinning with Ruled Surfaces</li>
 * </ul>
 * <br>
 * In all cases of skinning the cross section profiles are copies; that is, the originals remain.
 * The profiles can be either all open or all closed.
 * If the user provides a set of closed profiles, the face normals of the skin body point outside,
 * away from the body material. When the user provides a set of open profiles, the face normals of
 * the skin face are oriented along the %surface normals, and no attempt is made to change
 * the face normal orientation.
 * <br><br>
 * This overloaded version of <tt>api_create_si</tt> supports either <b>Basic Skinning</b> or <b>Skinning with a Path</b>.
 * <br><br>
 * <b>Basic Skinning</b>
 * <br><br>
 * Basic skinning accepts wire bodies as cross section profiles over which skinned surfaces are defined.
 * Basic skinning allows no control over the resulting surfaces other than the cross section profiles
 * themselves. (The <tt>path</tt> argument should be <tt>NULL</tt> for basic skinning.)
 * <br><br>
 * The profiles can either be all closed or all open loops. In the case of closed loops the end
 * profiles can be degenerate (a point). For example, basic skinning between a circle and a point
 * will give a conical spline with the "point" being the apex. The cross sections can be in the form
 * of wire bodies or edges; however, wire bodies are preferred.
 * <br><br>
 * <b>Skinning with a Path</b>
 * <br><br>
 * Skinning with a path accepts an additional wire body to control the take-off directions of
 * the %surface. The path is accepted as a wire body. It does not have to intersect the cross section
 * profiles. Skinning with a path accepts degenerate wires as end points; however, it is unlikely
 * that a desirable result will be produced.
 * <br><br>
 * <b>Special Options</b>
 * <br><br>
 * The <tt>api_create_si</tt> function accepts the following special options depending on the specific API:
 * <br><br>
 * The <i>arc_length</i> option is used to choose arc length or isoparametric parameterization of the
 * skinning surfaces. For isoparametric parameterization, the %surface parameter in the <i>v</i> direction
 * follows the cross section curves. For arc length parameterization, the %surface parameter follows
 * lines of constant length. The default is isoparametric parameterization. Surfaces resulting from
 * skinning with guide curves support isoparametric parameterization only. The default is <tt>FALSE</tt>.
 * <br><br>
 * The <i>no_twist</i> option may be used to minimize the twist of the %surface produced.
 * Twist minimization aligns closed curves such that the start of the second %curve is aligned to
 * the start of the first %curve. Even if a body's shape is unaffected by twisting, a %surface with
 * a twist could produce unexpected results when faceting and rendering. The default is <tt>TRUE</tt>.
 * <br><br>
 * The <i>align</i> option is used to align the direction of the cross section curves such that the normal
 * of the first profile points towards the second profile. All other profiles are aligned to follow
 * the first and second. If the sections are not oriented in the same direction, the <i>align</i> option
 * should be used to avoid producing a twisted, self-intersecting body. The default is <tt>TRUE</tt>.
 * <br><br>
 * The <i>simplify</i> option simplifies the %surface to a conical %surface, if applicable. If all of the
 * cross sections lie on a conical %surface (plane, cylinder, cone, sphere, or torus), the conical
 * %surface is created instead. The <tt>SPAresabs</tt> variable is used to determine whether or not the cross
 * section lies on a conical %surface. The default is <tt>TRUE</tt>.
 * <br><br>
 * The <i>closed</i> option may be used when the user needs to construct a solid body closed in <i>v</i>.
 * (e.g., a %torus). A solid body will be constructed only when all the wires supplied are closed.
 * At least three profiles must be provided to create a closed body. The default is <tt>FALSE</tt>.
 * <br><br>
 * <li> The <i>solid</i> option may be used when a solid body is desired, but not a solid body closed in the <i>v</i> direction. 
 * When a solid body that is not closed in the <i>v</i> direction is desired, the end wires are capped with planar faces.
 * The default is <tt>TRUE</tt>.</li>
 * <br><br>
 * The <i>periodic</i> option allows the construction of loft bodies that are periodic in <i>v</i>; that is,
 * bodies that close back on themselves smoothly (continuously) at the start and end profiles.
 * As with the <i>closed</i> option, at least three profiles must be supplied to create a periodic loft body.
 * The default is <tt>FALSE</tt>.
 * <br><br>
 * The <i>virtualGuides</i> option may be used in order to have the user defined guides affect the body in a
 * global nature. The default is <tt>FALSE</tt>.
 * <br><br>
 * When the <i>merge_wirecoedges</i> option is set to <tt>TRUE</tt>, the G<sup>1</sup> vertices of the skinning and lofting
 * wire profiles are removed by merging adjacent coedges/edges. This improves operations such as
 * blending and shelling because it reduces the coedge/edge count and the number of surfaces, and
 * eliminates near tangent edges. The default is <tt>TRUE</tt>.
 * <br><br>
 * When the <i>estimate_loft_tanfacs</i> option is on, the weight factor for the tangency conditions of
 * the loft will be determined such that it minimizes the average radius of curvature of the lofting
 * surfaces. The resulting bodies should support shelling to greater thickness and also blending of
 * their edges to larger blend radii. The default is <tt>FALSE</tt>.
 * <br><br>
 * The <i>match_vertices</i> option suppresses the vertex-matching-algorithm which ensures that all profiles
 * consist of the same number of coedges. A heuristic approach is used to determine which vertex pairs
 * are good matches. Profile coedges are then split where additional vertices are needed.
 * This option is forced to <tt>TRUE</tt> if the coedge numbers of the profiles are not equal.
 * The default is <tt>TRUE</tt>.
 * <br><br>
 * The algorithm that minimizes the %surface twist (see <i>no_twist</i> option) may add vertices to some
 * of the profiles if none of the existing vertices match well enough. The <i>no_new_twist_vertices</i>
 * option allows to force the algorithm to choose matching vertices from the existing vertices.
 * The default is <tt>FALSE</tt>.
 * <br><br>
 * <b>Limitations:</b> The cross section profiles are assumed to be simple; that is, they must be well
 * behaved and non-looping.
 * <br><br>
 * The parameterization of the %surface in its <i>u</i> direction - the direction of the user-defined wire
 * bodies - follows the parameterization of the curves underlying the wire bodies.
 * <br><br>
 * Skinning cannot produce a %surface in which there is any one point on the %surface in which the
 * <i>u</i> and <i>v</i> directions are the same or opposite. That is, in no case can an underlying %curve of the
 * user-defined wire body and a lateral edge of the %surface generated from the skinning algorithm
 * ever meet tangentially.
 * <br><br>
 * End capping (for creation of open solid bodies) is supported only for planar end profiles.
 * If the end capping operation fails then the sheet body constructed is returned.
 * <br><br>
 * <b>Effect:</b> Read-only
 * <br><br>
 * <b>Journal: </b> Available
 * <br><br>
 * <b>Product(s):</b> 3D ACIS Modeler
 * <br><br>
* @param li_wires
 * list of wires.
 * @param obj
 * interface object created.
 * @param wireBodies
 * array of temporary wire bodies.
 * @param path
 * skinning path (can be <tt>NULL</tt>).
 * @param opts
 * skinning options.
 * @param ao
 * ACIS options such as version and journal.
 **/
DECL_SKIN outcome api_create_si(
		ENTITY_LIST &li_wires,				// List of wires
		AcisSkinningInterface *&obj,		// SkinningInterface object
		BODY **&wireBodies,					// Array of created wire bodies
		BODY *path,			    		    // Skinning path; can be NULL
		skin_options *opts, 			    // skin options	arc_length, no_twist, align, perpendicular, simplify, closed, solid
		AcisOptions *ao = NULL
	);

/**
 * Creates an <tt>AcisSkinningInterface</tt> object for <i>Skinning with Ruled Surfaces</i>.
 * <br><br>
 * <b>Role:</b> This object is used as the input to additional skinning and lofting breakup APIs.
 * It contains all the given input and residual data created via the skinning process.
 * There are eight functional variations of skinning each differing by methods of %surface control;
 * however, only seven are supported through this API. Branched skinning is only supported through
 * <tt>api_skin_wires</tt>.
 * <br><br>
 * For a complete overview refer to the <i>Skinning</i> Technical Article.
 * This API has the following variations: <br>
 * <ul>
 * <li>Basic Skinning</li>
 * <li>Skinning with Vectors</li>
 * <li>Skinning with a Path</li>
 * <li>Skinning with Draft Angles</li>
 * <li>Skinning to the Planar Normal</li>
 * <li>Skinning with Guide Curves (or Virtual Guide Curves, if <tt>virtualGuides</tt> is set to <tt>TRUE</tt>)</li>
 * <li>Skinning with Ruled Surfaces</li>
 * </ul>
 * <br>
 * In all cases of skinning the cross section profiles are copies; that is, the originals remain.
 * The profiles can be either all open or all closed.
 * If the user provides a set of closed profiles, the face normals of the skin body point outside,
 * away from the body material. When the user provides a set of open profiles, the face normals of
 * the skin face are oriented along the %surface normals, and no attempt is made to change
 * the face normal orientation.
 * <br><br>
 * This overloaded version of <tt>api_create_si</tt> supports <b>Skinning with Ruled Surfaces</b>.
 * <br><br>
 * Ruled skinning accepts the enumerated type <tt>skinning_ruled</tt>. This enumerated type has only one constant: 
 * <tt>RULED_SKINNING</tt>. This variation accepts degenerate end points. In addition, it supports the <i>closed</i> option.
 * It does not detect self-intersections.
 * <br><br>
 * <b>Special Options</b>
 * <br><br>
 * The <tt>api_create_si</tt> function accepts the following special options depending on the specific API:
 * <br><br>
 * The <i>arc_length</i> option is used to choose arc length or isoparametric parameterization of the
 * skinning surfaces. For isoparametric parameterization, the %surface parameter in the <i>v</i> direction
 * follows the cross section curves. For arc length parameterization, the %surface parameter follows
 * lines of constant length. The default is isoparametric parameterization. Surfaces resulting from
 * skinning with guide curves support isoparametric parameterization only. The default is <tt>FALSE</tt>.
 * <br><br>
 * The <i>no_twist</i> option may be used to minimize the twist of the %surface produced.
 * Twist minimization aligns closed curves such that the start of the second %curve is aligned to
 * the start of the first %curve. Even if a body's shape is unaffected by twisting, a %surface with
 * a twist could produce unexpected results when faceting and rendering. The default is <tt>TRUE</tt>.
 * <br><br>
 * The <i>align</i> option is used to align the direction of the cross section curves such that the normal
 * of the first profile points towards the second profile. All other profiles are aligned to follow
 * the first and second. If the sections are not oriented in the same direction, the <i>align</i> option
 * should be used to avoid producing a twisted, self-intersecting body. The default is <tt>TRUE</tt>.
 * <br><br>
 * The <i>simplify</i> option simplifies the %surface to a conical %surface, if applicable. If all of the
 * cross sections lie on a conical %surface (plane, cylinder, cone, sphere, or torus), the conical
 * %surface is created instead. The <tt>SPAresabs</tt> variable is used to determine whether or not the cross
 * section lies on a conical %surface. The default is <tt>TRUE</tt>.
 * <br><br>
 * The <i>closed</i> option may be used when the user needs to construct a solid body closed in <i>v</i>.
 * (e.g., a %torus). A solid body will be constructed only when all the wires supplied are closed.
 * At least three profiles must be provided to create a closed body. The default is <tt>FALSE</tt>.
 * <br><br>
 * <li> The <i>solid</i> option may be used when a solid body is desired, but not a solid body closed in the <i>v</i> direction. 
 * When a solid body that is not closed in the <i>v</i> direction is desired, the end wires are capped with planar faces.
 * The default is <tt>TRUE</tt>.</li>
 * <br><br>
 * The <i>periodic</i> option allows the construction of loft bodies that are periodic in <i>v</i>; that is,
 * bodies that close back on themselves smoothly (continuously) at the start and end profiles.
 * As for the <i>closed</i> option, at least three profiles must be supplied to create a periodic loft body.
 * The default is <tt>FALSE</tt>.
 * <br><br>
 * The <i>virtualGuide</i> option may be used in order to have the user defined guides affect the body in a
 * global nature. The default is <tt>FALSE</tt>.
 * <br><br>
 * When the <i>merge_wirecoedges</i> option is set to <tt>TRUE</tt>, the G<sup>1</sup> vertices of the skinning and lofting
 * wire profiles are removed by merging adjacent coedges/edges.This improves operations such as
 * blending and shelling because it reduces the coedge/edge count and the number of surfaces, and
 * eliminates near tangent edges. The default is <tt>TRUE</tt>.
 * <br><br>
 * When the <i>estimate_loft_tanfacs</i> option is on, the weight factor for the tangency conditions of
 * the loft will be determined such that it minimizes the average radius of curvature of the lofting
 * surfaces. The resulting bodies should support shelling to greater thickness and also blending of
 * their edges to larger blend radii. The default is <tt>FALSE</tt>.
 * <br><br>
 * The <i>match_vertices</i> option suppresses the vertex-matching-algorithm which ensures that all profiles
 * consist of the same number of coedges. A heuristic approach is used to determine which vertex pairs
 * are good matches. Profile coedges are then split where additional vertices are needed.
 * This option is forced to <tt>TRUE</tt> if the coedge numbers of the profiles are not equal.
 * The default is <tt>TRUE</tt>.
 * <br><br>
 * The algorithm that minimizes the %surface twist (see <i>no_twist</i> option) may add vertices to some
 * of the profiles if none of the existing vertices match well enough. The <i>no_new_twist_vertices</i>
 * option allows to force the algorithm to choose matching vertices from the existing vertices.
 * The default is <tt>FALSE</tt>.
 * <br><br>
 * <b>Limitations:</b> The cross section profiles are assumed to be simple; that is, they must be well
 * behaved and non-looping.
 * <br><br>
 * The parameterization of the %surface in its <i>u</i> direction - the direction of the user-defined wire
 * bodies - follows the parameterization of the curves underlying the wire bodies.
 * <br><br>
 * Skinning cannot produce a %surface in which there is any one point on the %surface in which the
 * <i>u</i> and <i>v</i> directions are the same or opposite. That is, in no case can an underlying %curve of the
 * user-defined wire body and a lateral edge of the %surface generated from the skinning algorithm
 * ever meet tangentially.
 * <br><br>
 * End capping (for creation of open solid bodies) is supported only for planar end profiles.
 * If the end capping operation fails then the sheet body constructed is returned.
 * <br><br>
 * <b>Effect:</b> Read-only
 * <br><br>
 * <b>Journal: </b> Available
 * <br><br>
 * <b>Product(s):</b> 3D ACIS Modeler
 * <br><br>
* @param wireList
 * list of wires.
 * @param obj
 * interface object created.
 * @param wireBodies
 * array of temporary wire bodies.
 * @param ruled
 * ruled skinning.
 * @param opts
 * skinning options.
 * @param ao
 * ACIS options such as version and Journal.
 **/
DECL_SKIN outcome api_create_si(
		ENTITY_LIST &wireList,				// List of wires
		AcisSkinningInterface *&obj,		// SkinningInterface object
		BODY **&wireBodies,					// Array of created wire bodies
		skinning_ruled ruled,				// flag for ruled skinning
		skin_options *opts, 			    // skin options arc_length, no_twist, align, perpendicular, simplify, closed, solid
		AcisOptions * = NULL
	);
/*
// tbrv
 */
/**
 * @nodoc
 */
DECL_SKIN outcome api_create_si(
		ENTITY_LIST& wireList,						// List of wires
		AcisSLInterface*& obj,						// Interface object
		BODY**& wireBodies,							// Array of created wire bodies
		double draft_start,							// Draft Angle for first wire
		double draft_end,							// Draft Angle for second wire
		double draft_start_mag = 0.0,				// Magnitude of take-off vectors on first wire
		double draft_end_mag = 0.0,					// Magnitude of take-off vectors on second wire
		const SPAvector &start_normal = SPAvector(0.0,0.0,0.0),	// Plane normal of first wire
		const SPAvector &end_normal = SPAvector(0.0,0.0,0.0),	// Plane normal of last wire
		logical = FALSE,							// arc length option
		logical = TRUE,								// twist option
		logical = TRUE, 							// align option
		logical = TRUE,								// simplify option
		logical = TRUE, 							// construct a solid/sheet body for TRUE/FALSE
		AcisOptions* = NULL
	);

/**
 * Creates an <tt>AcisSkinningInterface</tt> object for <i>Skinning with Draft Angles</i>.
 * <br><br>
 * <b>Role:</b> This object is used as the input to additional skinning and lofting breakup APIs.
 * It contains all the given input and residual data created via the skinning process.
 * There are eight functional variations of skinning each differing by methods of %surface control;
 * however, only seven are supported through this API. Branched skinning is only supported through
 * <tt>api_skin_wires</tt>.
 * <br><br>
 * For a complete overview refer to the <i>Skinning</i> Technical Article.
 * This API has the following variations: <br>
 * <ul>
 * <li>Basic Skinning</li>
 * <li>Skinning with Vectors</li>
 * <li>Skinning with a Path</li>
 * <li>Skinning with Draft Angles</li>
 * <li>Skinning to the Planar Normal</li>
 * <li>Skinning with Guide Curves (or Virtual Guide Curves, if <tt>virtualGuides</tt> is set to <tt>TRUE</tt>)</li>
 * <li>Skinning with Ruled Surfaces</li>
 * </ul>
 * <br>
 * In all cases of skinning the cross section profiles are copies; that is, the originals remain.
 * The profiles can be either all open or all closed.
 * If the user provides a set of closed profiles, the face normals of the skin body point outside,
 * away from the body material. When the user provides a set of open profiles, the face normals of
 * the skin face are oriented along the %surface normals, and no attempt is made to change
 * the face normal orientation.
 * <br><br>
 * This overloaded version of <tt>api_create_si</tt> supports <b>Skinning with Draft Angles</b>.
 * <br><br>
 * Skinning with draft angles accepts two angles to control the take-off direction of the %surface
 * at the first and last profiles. The cross section wire list may contain any number of wires
 * (greater than two) however, the draft angles are only supplied to the first and last profiles.
 * The draft angles are accepted in the form of radians. In addition to the angles themselves all
 * overloaded forms of this variation accept two more doubles to control the magnitude of the
 * draft angles. These values may be zero; in this case we calculate the optimum magnitude for you.
 * <br><br>
 * Skinning with draft angles allows the end profiles to be degenerate or single %straight lines.
 * In this case, the wire normal used in the calculation of the take-off direction will be computed
 * for you. However, if you wish to supply your own, there are two additional parameters that accept
 * normal vectors for the first and last profiles. These vectors are only accepted in the case where
 * the end profiles are degenerate or do not have their own intrinsic normal (i.e., %straight lines).
 * <br><br>
 * Skinning with a draft does not support the <i>closed</i> or <i>periodic</i> options.
 * <br><br>
 * <b>Special Options</b>
 * <br><br>
 * The <tt>api_create_si</tt> function accepts the following special options depending on the specific API:
 * <br><br>
 * The <i>arc_length</i> option is used to choose arc length or isoparametric parameterization of the
 * skinning surfaces. For isoparametric parameterization, the %surface parameter in the <i>v</i> direction
 * follows the cross section curves. For arc length parameterization, the %surface parameter follows
 * lines of constant length. The default is isoparametric parameterization. Surfaces resulting from
 * skinning with guide curves support isoparametric parameterization only. The default is <tt>FALSE</tt>.
 * <br><br>
 * The <i>no_twist</i> option may be used to minimize the twist of the %surface produced.
 * Twist minimization aligns closed curves such that the start of the second %curve is aligned to
 * the start of the first %curve. Even if a body's shape is unaffected by twisting, a %surface with
 * a twist could produce unexpected results when faceting and rendering. The default is <tt>TRUE</tt>.
 * <br><br>
 * The <i>align</i> option is used to align the direction of the cross section curves such that the normal
 * of the first profile points towards the second profile. All other profiles are aligned to follow
 * the first and second. If the sections are not oriented in the same direction, the <i>align</i> option
 * should be used to avoid producing a twisted, self-intersecting body. The default is <tt>TRUE</tt>.
 * <br><br>
 * The <i>simplify</i> option simplifies the %surface to a conical %surface, if applicable. If all of the
 * cross sections lie on a conical %surface (plane, cylinder, cone, sphere, or torus), the conical
 * %surface is created instead. The <tt>SPAresabs</tt> variable is used to determine whether or not the cross
 * section lies on a conical %surface. The default is <tt>TRUE</tt>.
 * <br><br>
 * <li> The <i>solid</i> option may be used when a solid body is desired, but not a solid body closed in the <i>v</i> direction. 
 * When a solid body that is not closed in the <i>v</i> direction is desired, the end wires are capped with planar faces.
 * The default is <tt>TRUE</tt>.</li>
 * <br><br>
 * The <i>virtualGuide</i> option may be used in order to have the user defined guides affect the body in a
 * global nature. The default is <tt>FALSE</tt>.
 * <br><br>
 * When the <i>merge_wirecoedges</i> option is set to <tt>TRUE</tt>, the G<sup>1</sup> vertices of the skinning and lofting
 * wire profiles are removed by merging adjacent coedges/edges.This improves operations such as
 * blending and shelling because it reduces the coedge/edge count and the number of surfaces, and
 * eliminates near tangent edges. The default is <tt>TRUE</tt>.
 * <br><br>
 * When the <i>estimate_loft_tanfacs</i> option is on, the weight factor for the tangency conditions of
 * the loft will be determined such that it minimizes the average radius of curvature of the lofting
 * surfaces. The resulting bodies should support shelling to greater thickness and also blending of
 * their edges to larger blend radii. The default is <tt>FALSE</tt>.
 * <br><br>
 * The <i>match_vertices</i> option suppresses the vertex-matching-algorithm which ensures that all profiles
 * consist of the same number of coedges. A heuristic approach is used to determine which vertex pairs
 * are good matches. Profile coedges are then split where additional vertices are needed.
 * This option is forced to <tt>TRUE</tt> if the coedge numbers of the profiles are not equal.
 * The default is <tt>TRUE</tt>.
 * <br><br>
 * The algorithm that minimizes the %surface twist (see <i>no_twist</i> option) may add vertices to some
 * of the profiles if none of the existing vertices match well enough. The <i>no_new_twist_vertices</i>
 * option allows to force the algorithm to choose matching vertices from the existing vertices.
 * The default is <tt>FALSE</tt>.
 * <br><br>
 * <b>Limitations:</b> The cross section profiles are assumed to be simple; that is, they must be well
 * behaved and non-looping.
 * <br><br>
 * The parameterization of the %surface in its <i>u</i> direction - the direction of the user-defined wire
 * bodies - follows the parameterization of the curves underlying the wire bodies.
 * <br><br>
 * Skinning cannot produce a %surface in which there is any one point on the %surface in which the
 * <i>u</i> and <i>v</i> directions are the same or opposite. That is, in no case can an underlying %curve of the
 * user-defined wire body and a lateral edge of the %surface generated from the skinning algorithm
 * ever meet tangentially.
 * <br><br>
 * End capping (for creation of open solid bodies) is supported only for planar end profiles.
 * If the end capping operation fails then the sheet body constructed is returned.
 * <br><br>
 * <b>Effect:</b> Read-only
 * <br><br>
 * <b>Journal: </b> Available
 * <br><br>
 * <b>Product(s):</b> 3D ACIS Modeler
 * <br><br>
* @param wireList
 * list of wires.
 * @param obj
 * interface object created.
 * @param wireBodies
 * array of temporary wire bodies.
 * @param opts
 * skin options.
 * @param draft_start
 * start of draft angle.
 * @param draft_end
 * end of draft angle.
 * @param draft_start_mag
 * start draft  magnitude.
 * @param draft_end_mag
 * end draft  magnitude.
 * @param start_normal
 * start draft  normal.
 * @param end_normal
 * end draft  normal.
 * @param ao
 * ACIS options such as version and journal.
 **/
DECL_SKIN outcome api_create_si(
		ENTITY_LIST &wireList,						// List of wires
		AcisSLInterface *&obj,						// Interface object
		BODY **&wireBodies,							// Array of created wire bodies
		skin_options *opts,							// skin options
		double draft_start,							// Draft Angle for first wire
		double draft_end,							// Draft Angle for second wire
		double draft_start_mag = 0.0,				// Magnitude of take-off vectors on first wire
		double draft_end_mag = 0.0,					// Magnitude of take-off vectors on second wire
		const SPAvector &start_normal = SPAvector(0.0,0.0,0.0),	// Plane normal of first wire
		const SPAvector &end_normal = SPAvector(0.0,0.0,0.0),   // Plane normal of last wire
		AcisOptions *ao = NULL
	);

/**
 * Creates an <tt>AcisSkinningInterface</tt> object for <i>Skinning to the Planar Normal</i>.
 * <br><br>
 * <b>Role:</b> This object is used as the input to additional skinning and lofting breakup APIs.
 * It contains all the given input and residual data created via the skinning process.
 * There are eight functional variations of skinning each differing by methods of %surface control;
 * however, only seven are supported through this API. Branched skinning is only supported through
 * <tt>api_skin_wires</tt>.
 * <br><br>
 * For a complete overview refer to the <i>Skinning</i> Technical Article.
 * This API has the following variations: <br>
 * <ul>
 * <li>Basic Skinning</li>
 * <li>Skinning with Vectors</li>
 * <li>Skinning with a Path</li>
 * <li>Skinning with Draft Angles</li>
 * <li>Skinning to the Planar Normal</li>
 * <li>Skinning with Guide Curves (or Virtual Guide Curves, if <tt>virtualGuides</tt> is set to <tt>TRUE</tt>)</li>
 * <li>Skinning with Ruled Surfaces</li>
 * </ul>
 * <br>
 * In all cases of skinning the cross section profiles are copies; that is, the originals remain.
 * The profiles can be either all open or all closed.
 * If the user provides a set of closed profiles, the face normals of the skin body point outside,
 * away from the body material. When the user provides a set of open profiles, the face normals of
 * the skin face are oriented along the %surface normals, and no attempt is made to change
 * the face normal orientation.
 * <br><br>
 * This overloaded version of <tt>api_create_si</tt> supports <b>Skinning to the Planar Normal</b>.
 * <br><br>
 * Skinning to the planar normal accepts the enumerated type <tt>skinning_normals</tt> as the fourth parameter.
 * This enumerated type has four constants: <tt>first_normal</tt>, <tt>last_normal</tt>, <tt>ends_normal</tt> and
 * <tt>all_normal</tt>. This variation of skinning does not accept degenerate profiles (points) or profiles
 * that do not define a plane (lines). In addition, it does not accept any form of magnitude control
 * (see skinning with a draft angle). An optimum magnitude is calculated for you.
 * <br><br>
 * <b>Special Options</b>
 * <br><br>
 * The <tt>api_create_si</tt> function accepts the following special options depending on the specific API:
 * <br><br>
 * The <i>arc_length</i> option is used to choose arc length or isoparametric parameterization of the
 * skinning surfaces. For isoparametric parameterization, the %surface parameter in the <i>v</i> direction
 * follows the cross section curves. For arc length parameterization, the %surface parameter follows
 * lines of constant length. The default is isoparametric parameterization. Surfaces resulting from
 * skinning with guide curves support isoparametric parameterization only. The default is <tt>FALSE</tt>.
 * <br><br>
 * The <i>no_twist</i> option may be used to minimize the twist of the %surface produced.
 * Twist minimization aligns closed curves such that the start of the second %curve is aligned to
 * the start of the first %curve. Even if a body's shape is unaffected by twisting, a %surface with
 * a twist could produce unexpected results when faceting and rendering. The default is <tt>TRUE</tt>.
 * <br><br>
 * The <i>align</i> option is used to align the direction of the cross section curves such that the normal
 * of the first profile points towards the second profile. All other profiles are aligned to follow
 * the first and second. If the sections are not oriented in the same direction, the <i>align</i> option
 * should be used to avoid producing a twisted, self-intersecting body. The default is <tt>TRUE</tt>.
 * <br><br>
 * The <i>simplify</i> option simplifies the %surface to a conical %surface, if applicable. If all of the
 * cross sections lie on a conical %surface (plane, cylinder, cone, sphere, or torus), the conical
 * %surface is created instead. The <tt>SPAresabs</tt> variable is used to determine whether or not the cross
 * section lies on a conical %surface. The default is <tt>TRUE</tt>.
 * <br><br>
 * The <i>closed</i> option may be used when the user needs to construct a solid body closed in <i>v</i>.
 * (e.g., a %torus). A solid body will be constructed only when all the wires supplied are closed.
 * At least three profiles must be provided to create a closed body. The default is <tt>FALSE</tt>.
 * <br><br>
 * <li> The <i>solid</i> option may be used when a solid body is desired, but not a solid body closed in the <i>v</i> direction. 
 * When a solid body that is not closed in the <i>v</i> direction is desired, the end wires are capped with planar faces.
 * The default is <tt>TRUE</tt>.</li>
 * <br><br>
 * The <i>periodic</i> option allows the construction of loft bodies that are periodic in <i>v</i>; that is,
 * bodies that close back on themselves smoothly (continuously) at the start and end profiles.
 * As for the <i>closed</i> option, at least three profiles must be supplied to create a periodic loft body.
 * The default is <tt>FALSE</tt>.
 * <br><br>
 * The <i>virtualGuide</i> option may be used in order to have the user defined guides affect the body in a
 * global nature. The default is <tt>FALSE</tt>.
 * <br><br>
 * When the <i>merge_wirecoedges</i> option is set to <tt>TRUE</tt>, the G<sup>1</sup> vertices of the skinning and lofting
 * wire profiles are removed by merging adjacent coedges/edges.This improves operations such as
 * blending and shelling because it reduces the coedge/edge count and the number of surfaces, and
 * eliminates near tangent edges. The default is <tt>TRUE</tt>.
 * <br><br>
 * When the <i>estimate_loft_tanfacs</i> option is on, the weight factor for the tangency conditions of
 * the loft will be determined such that it minimizes the average radius of curvature of the lofting
 * surfaces. The resulting bodies should support shelling to greater thickness and also blending of
 * their edges to larger blend radii. The default is <tt>FALSE</tt>.
 * <br><br>
 * The <i>match_vertices</i> option suppresses the vertex-matching-algorithm which ensures that all profiles
 * consist of the same number of coedges. A heuristic approach is used to determine which vertex pairs
 * are good matches. Profile coedges are then split where additional vertices are needed.
 * This option is forced to <tt>TRUE</tt> if the coedge numbers of the profiles are not equal.
 * The default is <tt>TRUE</tt>.
 * <br><br>
 * The algorithm that minimizes the %surface twist (see <i>no_twist</i> option) may add vertices to some
 * of the profiles if none of the existing vertices match well enough. The <i>no_new_twist_vertices</i>
 * option allows to force the algorithm to choose matching vertices from the existing vertices.
 * The default is <tt>FALSE</tt>.
 * <br><br>
 * <b>Limitations:</b> The cross section profiles are assumed to be simple; that is, they must be well
 * behaved and non-looping.
 * <br><br>
 * The parameterization of the %surface in its <i>u</i> direction - the direction of the user-defined wire
 * bodies - follows the parameterization of the curves underlying the wire bodies.
 * <br><br>
 * Skinning cannot produce a %surface in which there is any one point on the %surface in which the
 * <i>u</i> and <i>v</i> directions are the same or opposite. That is, in no case can an underlying %curve of the
 * user-defined wire body and a lateral edge of the %surface generated from the skinning algorithm
 * ever meet tangentially.
 * <br><br>
 * End capping (for creation of open solid bodies) is supported only for planar end profiles.
 * If the end capping operation fails then the sheet body constructed is returned.
 * <br><br>
 * <b>Effect:</b> Read-only
 * <br><br>
 * <b>Journal: </b> Available
 * <br><br>
 * <b>Product(s):</b> 3D ACIS Modeler
 * <br><br>
* @param wireList
 * list of wire bodies.
 * @param obj
 * interface object created.
 * @param wireBodies
 * array of temporary wire bodies.
 * @param normals
 * skinning normals.
 * @param opts
 * skin options.
 * @param ao
 * ACIS options such as version and journal.
 **/
DECL_SKIN outcome api_create_si(
 		ENTITY_LIST &wireList,			// List of wires
		AcisSLInterface *&obj,			// Interface object
		BODY **&wireBodies,				// Array of created wire bodies
		skinning_normals normals,		// Normal type
		skin_options *opts, 			// skin options arc_length, no_twist, align, perpendicular, simplify, closed, solid
		AcisOptions *ao = NULL
	);

/**
 * Creates an <tt>AcisSkinningInterface</tt> object for <i>Skinning with Vectors</i>.
 * <br><br>
 * <b>Role:</b> This object is used as the input to additional skinning and lofting breakup APIs.
 * It contains all the given input and residual data created via the skinning process.
 * There are eight functional variations of skinning each differing by methods of %surface control;
 * however, only seven are supported through this API. Branched skinning is only %supported through
 * <tt>api_skin_wires</tt>.
 * <br><br>
 * For a complete overview refer to the <i>Skinning</i> Technical Article.
 * This API has the following variations: <br>
 * <ul>
 * <li>Basic Skinning</li>
 * <li>Skinning with Vectors</li>
 * <li>Skinning with a Path</li>
 * <li>Skinning with Draft Angles</li>
 * <li>Skinning to the Planar Normal</li>
 * <li>Skinning with Guide Curves (or Virtual Guide Curves, if <tt>virtualGuides</tt> is set to <tt>TRUE</tt>)</li>
 * <li>Skinning with Ruled Surfaces</li>
 * </ul>
 * <br>
 * In all cases of skinning the cross section profiles are copies; that is, the originals remain.
 * The profiles can be either all open or all closed.
 * If the user provides a set of closed profiles, the face normals of the skin body point outside,
 * away from the body material. When the user provides a set of open profiles, the face normals of
 * the skin face are oriented along the %surface normals, and no attempt is made to change
 * the face normal orientation.
 * <br><br>
 * This overloaded version of <tt>api_create_si</tt> supports <b>Skinning with Vectors</b>.
 * <br><br>
 * <b>Basic Skinning</b>
 * <br><br>
 * Basic skinning accepts wire bodies as cross section profiles over which skinned surfaces are defined.
 * Basic skinning allows no control over the resulting surfaces other than the cross section profiles
 * themselves. (The <tt>path</tt> argument should be <tt>NULL</tt> for basic skinning.)
 * <br><br>
 * The profiles can either be all closed or all open loops.In the case of closed loops the end
 * profiles can be degenerate (a point). For example, basic skinning between a circle and a point
 * will give a conical spline with the "point" being the apex. The cross sections can be in the form
 * of wire bodies or edges; however, wire bodies are preferred.
 * <br><br>
 * <b>Skinning with Vectors</b>
 * <br><br>
 * Skinning with vectors accepts as additional constraints an array of vectors and, optionally, 
 * an array of magnitudes to control the take-off vectors with which the skinned %surface leaves the profiles. 
 * The number of supplied vectors must equal the number of profiles, if the <tt>closed</tt> option 
 * is set to <tt>FALSE</tt>. If the <tt>closed</tt> option is set to <tt>TRUE</tt>, one additional vector 
 * can be specified. It will be used on the copy that is made of the first profile to create a closed body. 
 * If no additional vector is provided, the first vector will be reused on the first profiles copy. 
 * In a similarly way, magnitudes can be provided. A profile will be interpolated without constraint 
 * if the vector supplied for it is the zero vector. 
 * <br><br>
 * <b>Skinning with a Path</b>
 * <br><br>
 * Skinning with a path accepts an additional wire body to control the take-off directions of
 * the %surface. The path is accepted as a wire body. It does not have to intersect the cross section
 * profiles. Skinning with a path accepts degenerate wires as end points; however, it is unlikely
 * that a desirable result will be produced.
 * <br><br>
 * <b>Skinning with Guide Curves (Virtual Guides)</b>
 * <br><br>
 * Skinning with guide curves accepts a list of guides in addition to the cross section profiles.
 * In the case of this API the guides are accepted via the function: <tt>api_add_guide_curve_si</tt>.
 * The guide curves "stretch" across the profiles from the first to last and control the
 * %surface in the <i>v</i> direction.
 * <br><br>
 * <b>Skinning To the Planar Normal</b>
 * <br><br>
 * Skinning to the planar normal accepts the enumerated type <tt>skinning_normals</tt> as the fourth parameter.
 * This enumerated type has four constants: <tt>first_normal</tt>, <tt>last_normal</tt>, <tt>ends_normal</tt> and
 * <tt>all_normal</tt>. This variation of skinning does not accept degenerate profiles (points) or profiles
 * that do not define a plane (lines). In addition, it does not accept any form of magnitude control
 * (see <i>Skinning with Draft Angles</i>). An optimum magnitude is calculated for you.
 * <br><br>
 * <b>Skinning with Draft Angles</b>
 * <br><br>
 * Skinning with draft angles accepts two angles to control the take-off direction of the %surface
 * at the first and last profiles. The cross section wire list may contain any number of wires
 * (greater than two) however, the draft angles are only supplied to the first and last profiles.
 * The draft angles are accepted in the form of radians. In addition to the angles themselves all
 * overloaded forms of this variation accept two more doubles to control the magnitude of the
 * draft angles. These values may be zero; in this case we calculate the optimum magnitude for you.
 * <br><br>
 * Skinning with draft angles allows the end profiles to be degenerate or single %straight lines.
 * In this case, the wire normal used in the calculation of the take-off direction will be computed
 * for you. However, if you wish to supply your own, there are two additional parameters that accept
 * normal vectors for the first and last profiles. These vectors are only accepted in the case where
 * the end profiles are degenerate or do not have their own intrinsic normal (i.e., %straight lines).
 * <br><br>
 * Skinning with a draft does not support the <i>closed</i> or <i>periodic</i> options.
 * <br><br>
 * <b>Ruled Skinning</b>
 * <br><br>
 * Ruled skinning accepts the enumerated type <tt>ruled_skinning</tt>.
 * This enumerated type has only one constant: <tt>ruled</tt>. 
 * This variation accepts degenerate end points.
 * In addition, it supports the <i>closed</i> option.
 * It does not detect self-intersections.
 * <br><br>
 * <b>Special Options</b>
 * <br><br>
 * The <tt>api_create_si</tt> function accepts the following special options depending on the specific API:
 * <br><br>
 * The <i>arc_length</i> option is used to choose arc length or isoparametric parameterization of the
 * skinning surfaces. For isoparametric parameterization, the %surface parameter in the <i>v</i> direction
 * follows the cross section curves. For arc length parameterization, the %surface parameter follows
 * lines of constant length. The default is isoparametric parameterization. Surfaces resulting from
 * skinning with guide curves support isoparametric parameterization only. The default is <tt>FALSE</tt>.
 * <br><br>
 * The <i>no_twist</i> option may be used to minimize the twist of the %surface produced.
 * Twist minimization aligns closed curves such that the start of the second %curve is aligned to
 * the start of the first %curve. Even if a body's shape is unaffected by twisting, a %surface with
 * a twist could produce unexpected results when faceting and rendering. The default is <tt>TRUE</tt>.
 * <br><br>
 * The <i>align</i> option is used to align the direction of the cross section curves such that the normal
 * of the first profile points towards the second profile. All other profiles are aligned to follow
 * the first and second. If the sections are not oriented in the same direction, the <i>align</i> option
 * should be used to avoid producing a twisted, self-intersecting body. The default is <tt>TRUE</tt>.
 * <br><br>
 * The <i>simplify</i> option simplifies the %surface to a conical %surface, if applicable. If all of the
 * cross sections lie on a conical %surface (plane, cylinder, cone, sphere, or torus), the conical
 * %surface is created instead. The <tt>SPAresabs</tt> variable is used to determine whether or not the cross
 * section lies on a conical %surface. The default is <tt>TRUE</tt>.
 * <br><br>
 * The <i>closed</i> option may be used when the user needs to construct a solid body closed in <i>v</i>.
 * (e.g., a %torus). A solid body will be constructed only when all the wires supplied are closed.
 * At least three profiles must be provided to create a closed body. The default is <tt>FALSE</tt>.
 * <br><br>
 * <li> The <i>solid</i> option may be used when a solid body is desired, but not a solid body closed in the <i>v</i> direction. 
 * When a solid body that is not closed in the <i>v</i> direction is desired, the end wires are capped with planar faces.
 * The default is <tt>TRUE</tt>.</li>
 * <br><br>
 * The <i>periodic</i> option allows the construction of loft bodies that are periodic in <i>v</i>; that is,
 * bodies that close back on themselves smoothly (continuously) at the start and end profiles.
 * As for the <i>closed</i> option, at least three profiles must be supplied to create a periodic loft body.
 * The default is <tt>FALSE</tt>.
 * <br><br>
 * The <i>virtualGuide</i> option may be used in order to have the user defined guides affect the body in a
 * global nature. The default is <tt>FALSE</tt>.
 * <br><br>
 * When the <i>merge_wirecoedges</i> option is set to <tt>TRUE</tt>, the G<sup>1</sup> vertices of the skinning and lofting
 * wire profiles are removed by merging adjacent coedges/edges.This improves operations such as
 * blending and shelling because it reduces the coedge/edge count and the number of surfaces, and
 * eliminates near tangent edges. The default is <tt>TRUE</tt>.
 * <br><br>
 * When the <i>estimate_loft_tanfacs</i> option is on, the weight factor for the tangency conditions of
 * the loft will be determined such that it minimizes the average radius of curvature of the lofting
 * surfaces. The resulting bodies should support shelling to greater thickness and also blending of
 * their edges to larger blend radii. The default is <tt>FALSE</tt>.
 * <br><br>
 * The <i>match_vertices</i> option suppresses the vertex-matching-algorithm which ensures that all profiles
 * consist of the same number of coedges. A heuristic approach is used to determine which vertex pairs
 * are good matches. Profile coedges are then split where additional vertices are needed.
 * This option is forced to <tt>TRUE</tt> if the coedge numbers of the profiles are not equal.
 * The default is <tt>TRUE</tt>.
 * <br><br>
 * The algorithm that minimizes the %surface twist (see <i>no_twist</i> option) may add vertices to some
 * of the profiles if none of the existing vertices match well enough. The <i>no_new_twist_vertices</i>
 * option allows to force the algorithm to choose matching vertices from the existing vertices.
 * The default is <tt>FALSE</tt>.
 * <br><br>
 * <b>Limitations:</b> The cross section profiles are assumed to be simple; that is, they must be well
 * behaved and non-looping.
 * <br><br>
 * The parameterization of the %surface in its <i>u</i> direction - the direction of the user-defined wire
 * bodies - follows the parameterization of the curves underlying the wire bodies.
 * <br><br>
 * Skinning cannot produce a %surface in which there is any one point on the %surface in which the
 * <i>u</i> and <i>v</i> directions are the same or opposite. That is, in no case can an underlying %curve of the
 * user-defined wire body and a lateral edge of the %surface generated from the skinning algorithm
 * ever meet tangentially.
 * <br><br>
 * End capping (for creation of open solid bodies) is supported only for planar end profiles.
 * If the end capping operation fails then the sheet body constructed is returned.
 * <br><br>
 * <b>Effect:</b> Read-only
 * <br><br>
 * <b>Journal: </b> Available
 * <br><br>
 * <b>Product(s):</b> 3D ACIS Modeler
 * <br><br>
* @param wireList
 * list of wires.
 * @param obj
 * interface object created.
 * @param wireBodies
 * array of temporary wire bodies.
 * @param vectors
 * array of skinning vectors.
 * @param num_vectors
 * number of vectors.
 * @param magnitudes
 * array of magnitudes.
 * @param num_magnitudes
 * number of magnitudes.
 * @param opts
 * skinning options.
 * @param ao
 * Acis options such as version and journal.
 **/
DECL_SKIN outcome api_create_si(
		ENTITY_LIST &wireList,			// List of wires
		AcisSLInterface *&obj,			// Interface object
		BODY **&wireBodies,				// Array of created wire bodies
		SPAvector *vectors,		        // Skinning vectors
		int num_vectors,                // number of vectors
		double *magnitudes	= NULL,     // Magnitudes
		int num_magnitudes	= 0,        // number of magnitudes
		skin_options *opts		= NULL, // skin options	arc_length, no_twist, align, perpendicular, simplify, closed, solid
		AcisOptions *ao		= NULL		// version, journal
	);

/*
// tbrv
 */
/**
 * @nodoc
 */
DECL_SKIN outcome api_set_options_li(
		AcisLoftingInterface* obj,		// AcisloftingInterface object
		logical = FALSE,				// arc length option
		logical = TRUE,					// twist option
		logical = TRUE,					// align option
		logical = TRUE,					// simplify option
		logical = FALSE,				// perpendicular option
		logical = TRUE,					// construct a solid/sheet body for TRUE/FALSE
		logical = FALSE,				// construct a closed/open body for TRUE/FALSE
		AcisOptions* = NULL
	);

/**
 * Sets the options in the interactive lofting interface object.
 * <br><br>
 * <b>Effect:</b> Read-only
 * <br><br>
 * <b>Journal: </b> Available
 * <br><br>
 * <b>Product(s):</b> 3D ACIS Modeler
 * <br><br>
* @param obj
 * lofting interface object.
 * @param opts
 * skinning options.
 * @param ao
 * ACIS options such as version or journal.
 **/
DECL_SKIN outcome api_set_options_li(
		AcisLoftingInterface *obj,		// AcisloftingInterface object
		skin_options *opts, 			// skin options arc_length, no_twist, align, perpendicular, simplify, closed, solid
		AcisOptions *ao = NULL
	);

/*
// tbrv
 */
/**
 * @nodoc
 */
DECL_SKIN outcome api_set_options_si(
		AcisSkinningInterface* obj,		// AcisSkinningInterface object
		logical = FALSE,				// arc length option
		logical = TRUE, 				// twist option
		logical = TRUE,					// align option
		logical = TRUE,					// simplify option
		logical = TRUE,					// construct a solid/sheet body for TRUE/FALSE
		logical = FALSE,				// construct a closed/open body for TRUE/FALSE
		logical = FALSE,				// virtual guide flag
		AcisOptions* = NULL
	);

/**
 * Sets the options in the interactive skinning interface object.
 * <br><br>
 * <b>Effect:</b> Read-only
 * <br><br>
 * <b>Journal: </b> Available
 * <br><br>
 * <b>Product(s):</b> 3D ACIS Modeler
 * <br><br>
* @param obj
 * skinning interface object.
 * @param opts
 * skinning options.
 * @param ao
 * ACIS options such as version or journal.
 **/
DECL_SKIN outcome api_set_options_si(
		AcisSkinningInterface *obj,		// AcisSkinningInterface object
		skin_options *opts, 			// skin options arc_length, no_twist, align, perpendicular, simplify, closed, solid
		AcisOptions *ao = NULL
	);

/**
 * Sets the coedge list and remakes the lofting wires.
 * <br><br>
 * <b>Role:</b> Deletes the laws, sections, fixed wire list,
 * internal coedge list, then reassigns the coedges and remakes the wires.
 * <br><br>
 * <b>Journal: </b> Available
 * <br><br>
 * <b>Product(s):</b> 3D ACIS Modeler
 * <br><br>
* @param number_coedges
 * number of coedge lists.
 * @param edgeList
 * array of coedge lists.
 * @param obj
 * AcisLoftingInterface object.
 * @param wireBodies
 * wire bodies made.
 * @param opts
 * ACIS options such as version or journal.
 */
DECL_SKIN outcome api_reenter_coedges_li(
		int number_coedges,
		Loft_Connected_Coedge_List* edgeList,
		AcisLoftingInterface* obj,
		BODY**& wireBodies,
		AcisOptions* opts = NULL
	); 
/**
 * Creates a set of broken up wires used for skinning or lofting.
 * <br><br>
 * <b>Role:</b> Creates a set of wires that are broken-up according to the
 * skinning and lofting breakup algorithm. (The original wires or coedges that were sent in for input
 * still exist.) It is this list of broken up wires that will be used in the skinning or
 * lofting algorithm.
 * <br><br>
 * <b>Effect:</b> Changes model
 * <br><br>
 * <b>Journal: </b> Available
 * <br><br>
 * <b>Product(s):</b> 3D ACIS Modeler
 * <br><br>
* @param obj
 * AcisSLInterface object.
 * @param wire_list
 * list of broken-up wires.
 * @param opts
 * ACIS options such as version or journal.
 **/
DECL_SKIN outcome api_make_wires_sli(
		AcisSLInterface *obj,				// AcisLoftingInterface
		ENTITY_LIST &wire_list,				// Entity list containing the created wires
		AcisOptions* opts = NULL
	);

/**
 * Builds a list of edges that represent the extents of the surfaces if the wires or coedges were to be lofted or skinned.
 * <br><br>
 * <b>Role:</b> This function builds a list of edges that represent the extents of the surfaces if
 * the wires were to be skinned at the present configuration. This function is intended to be used as
 * a preview of the potential lofting body that will be created. The application developer may delete
 * this list of edges at any time, allow the end user to move the vertices of a lofting or skinning
 * wire and recreate the edges.
 * <br><br>
 * <b>Effect:</b> Changes model
 * <br><br>
 * <b>Journal: </b> Available
 * <br><br>
 * <b>Product(s):</b> 3D ACIS Modeler
 * <br><br>
* @param obj
 * AcisSLInterface object.
 * @param edgeList
 * list of created edges.
 * @param opts
 * ACIS options such as version or journal.
 **/
DECL_SKIN outcome api_build_edges_sli(
		AcisSLInterface *obj,				// AcisLoftingInterface
		ENTITY_LIST &edgeList,					// Entity list containing the created edges
		AcisOptions *opts = NULL
	);

/**
 * Modifies the position of a vertex on a coedge of a wire.
 * <br><br>
 * <b>Role:</b> This function is used to alter the coedges of a wire of the skinning process so
 * the end-user may control the body generated.
 * <br><br>
 * <b>Effect:</b> Changes model
 * <br><br>
 * <b>Journal: </b> Available
 * <br><br>
 * <b>Product(s):</b> 3D ACIS Modeler
 * <br><br>
* @param obj
 * AcisSLInterface object.
 * @param coedge1
 * coedge to left of position.
 * @param coedge2
 * coedge to right of position.
 * @param adjusted_point
 * position to adjust vertex to.
 * @param wire
 * wire this belongs to.
 * @param opts
 * ACIS options such as version or journal.
 **/
DECL_SKIN outcome api_modify_wire_sli(
		AcisSLInterface *obj,				// AcisLoftingInterface
		COEDGE *coedge1,					// Coedge 1
		COEDGE *coedge2,					// Coedge 2
		const SPAposition &adjusted_point,	// Position to move the vertex to
		WIRE *wire,							// Wire number
		AcisOptions* opts = NULL
	);

/**
 * Adds a vertex to each wire in a list of wires.
 * <br><br>
 * <b>Role:</b> This function adds a vertex to the middle of the specified coedge and to
 * each of the corresponding coedges in the wire list.
 * <br><br>
 * <b>Effect:</b> Changes model
 * <br><br>
 * <b>Journal: </b> Available
 * <br><br>
 * <b>Product(s):</b> 3D ACIS Modeler
 * <br><br>
* @param obj
 * AcisSLInterface object.
 * @param wire
 * wire to add vertex on.
 * @param coedge
 * coedge to add vertex to middle of.
 * @param opts
 * ACIS options such as version or journal.
 **/
DECL_SKIN outcome api_add_vertex_sli(
		AcisSLInterface *obj,				// AcisLoftinginterface
		WIRE *wire,							// Wire	that contains the given coedge (see next param)
		COEDGE *coedge,						// Coedge to add the vertex to the middle of
		AcisOptions *opts = NULL
	);

/**
 * Modifies which vertex in a loop of coedges forming a wire is the starting vertex for traversing the loop.
 * <br><br>
 * <b>Journal: </b> Available
 * <br><br>
 * <b>Product(s):</b> 3D ACIS Modeler
 * <br><br>
* @param obj
 * AcisSLInterface object.
 * @param wire
 * wire that contains the vertex.
 * @param vertex
 * becomes new starting vertex.
 * @param opts
 * ACIS options such as version or journal.
 */
DECL_SKIN outcome api_start_vertex_sli(
		AcisSLInterface* obj,			// AcisLoftingInterface
		WIRE *wire,
		VERTEX *vertex,
		AcisOptions *opts = NULL
	);

/**
 * Modifies which vertex in a loop of coedges forming a wire is the starting vertex for traversing the loop.
 * <br><br>
 * <b>Journal: </b> Available
 * <br><br>
 * <b>Product(s):</b> 3D ACIS Modeler
 * <br><br>
* @param obj
 * AcisSLInterface object.
 * @param index
 * index of wire which contains the vertex in lofting interface object.
 * @param vertex
 * becomes new starting vertex.
 * @param opts
 * ACIS options such as version or journal.
 */
DECL_SKIN outcome api_start_vertex_sli(
		AcisSLInterface* obj,			// AcisLoftingInterface
		int index,						// Index of wire
		VERTEX* vertex,					// Position of the vertex on the wire to remove
		AcisOptions* opts = NULL
	);

/**
 * Removes a vertex from each wire in a list of wires.
 * <br><br>
 * <b>Role:</b> This function removes the specified vertex from the its coedge and a vertex from
 * each of the corresponding coedges in the wire list.
 * <br><br>
 * <b>Effect:</b> Changes model
 * <br><br>
 * <b>Journal: </b> Available
 * <br><br>
 * <b>Product(s):</b> 3D ACIS Modeler
 * <br><br>
* @param obj
 * AcisSLInterface object.
 * @param wire
 * wire to remove vertex from.
 * @param pos
 * vertex to be removed.
 * @param opts
 * ACIS options such as version or journal.
 **/
DECL_SKIN outcome api_remove_vertex_sli(
		AcisSLInterface *obj,				// Interface object
		WIRE *wire,							// Wire that has a vertex at the given SPAposition (see next param)
		const SPAposition &pos,				// Position of the vertex on the wire to remove
		AcisOptions *opts = NULL
	);

/**
 * Builds the sheet body from the data in the interactive skinning or lofting interface object.
 * <br><br>
 * <b>Role:</b> When lofting or skinning via the interactive skinning and lofting interface, the journaling process 
 * must begin at the creation of the interactive skinning and lofting interface object: either with @href api_create_li or 
 * @href api_create_si in C++, or with <tt>slinterface:lofting</tt> in Scheme AIDE. 
 * Journaling must remain enabled for each interactive skinning or lofting function call until the interactive 
 * skinning and lofting interface object is deleted with either @href api_delete_sli or <tt>slinterface:delete-interface</tt>.
 * <br><br>
 * <b>Effect:</b> Changes model
 * <br><br>
 * <b>Journal: </b> Available
 * <br><br>
 * <b>Product(s):</b> 3D ACIS Modeler
 * <br><br>
* @param obj
 * AcisSLInterface object.
 * @param body_out
 * created body.
 * @param opts
 * ACIS options such as versioning and journaling.
 **/
DECL_SKIN outcome api_build_body_sli(
		AcisSLInterface *obj,				// AcisLoftingInterface
		BODY *&body_out,							// The body that is made
		AcisOptions *opts = NULL
	);

/**
 * Deletes a degenerate coedge in each wire of a list of wires.
 * <br><br>
 * <b>Role:</b> This function will remove a degenerate coedge in each wire of a
 * list of wires. In order for the coedges to be removed, each wire in the list
 * must  contain a degenerate coedge at the same position. That is, if the fifth
 * coedge of wire one is degenerate, it and the other coedges will only be removed
 * if the fifth coedge of every other wire is degenerate as well.
 * <br><br>
 * <b>Effect:</b> Changes model
 * <br><br>
 * <b>Journal: </b> Available
 * <br><br>
 * <b>Product(s):</b> 3D ACIS Modeler
 * <br><br>
* @param obj
 * AcisSLInterface.
 * @param wire_list
 * list of modified wires.
 * @param opts
 * ACIS options such as version or journal.
 **/
DECL_SKIN outcome api_collapse_wires_sli(
		AcisSLInterface *obj,				// AcisLoftingInterface
		ENTITY_LIST &wire_list,			// wire list
		AcisOptions *opts = NULL
	);

/**
 * Deletes an <tt>AcisSLInterface</tt> object.
 * <br><br>
 * <b>Limitations:</b> The coedge lists are assumed to be simple; that is, they must be well
 * behaved and non-looping.
 * <br><br>
 * The parameterization of the %surface in its <i>u</i> direction (the direction of the coedge list)
 * follows the parameterization of the curves underlying the coedges.
 * <br><br>
 * Skinning cannot produce a %surface in which there is any one point on the %surface in which
 * the <i>u</i> and <i>v</i> directions are the same or opposite. That is, in no case can an underlying %curve
 * of the user-defined wire body and a lateral edge of the %surface generated from the skinning
 * algorithm ever meet tangentially.
 * <br><br>
 * End capping (for creation of open solid bodies) is supported only for planar end profiles.
 * If the end capping operation fails then the sheet body constructed is returned.
 * <br><br>
 * <b>Effect:</b> Read-only
 * <br><br>
 * <b>Journal: </b> Available
 * <br><br>
 * <b>Product(s):</b> 3D ACIS Modeler
 * <br><br>
* @param obj
 * object to delete.
 * @param opts
 * ACIS options such as version or journal.
 **/
DECL_SKIN outcome api_delete_sli(
		AcisSLInterface *obj,		        // AcisLoftingInterface
		AcisOptions *opts = NULL
	);

/**
 * Aligns the directions of the wires in the skinning or lofting profiles.
 * <br><br>
 * <b>Effect:</b> Changes model
 * <br><br>
 * <b>Journal: </b> Available
 * <br><br>
 * <b>Product(s):</b> 3D ACIS Modeler
 * <br><br>
* @param obj
 * AcisSLInterface object.
 * @param start
 * index of starting wire.
 * @param end
 * index of ending wire.
 * @param opts
 * ACIS options such as version or journal.
 **/
DECL_SKIN outcome api_align_wires_sli(
		AcisSLInterface *obj,		    // AcisLoftingInterface
		int start = 0,					// index of starting wire
		int end	  = 0,		            // index of ending wire
		AcisOptions *opts = NULL
	);

/**
 * Gets a list of valid starting vertices for skinning or lofting.
 * <br><br>
 * <b>Effect:</b> Read-only
 * <br><br>
 * <b>Journal: </b> Available
 * <br><br>
 * <b>Product(s):</b> 3D ACIS Modeler
 * <br><br>
* @param obj
 * AcisSLInterface object.
 * @param wire
 * wire to examine.
 * @param vertex_list
 * list of valid vertices.
 * @param opts
 * ACIS options such as version or journal.
 **/
DECL_SKIN outcome api_valid_start_vertices_sli(
		AcisSLInterface *obj,			// AcisLoftingInterface
		WIRE *wire,						// Wire to be looked at
		ENTITY_LIST &vertex_list,       // List of valid starting vertices
		AcisOptions *opts = NULL
	);

/**
 * Creates an equal number of coedges in each wire of the skinning or lofting profiles which is required to create the surfaces.
 * <br><br>
 * <b>Effect:</b> Changes model
 * <br><br>
 * <b>Journal: </b> Available
 * <br><br>
 * <b>Product(s):</b> 3D ACIS Modeler
 * <br><br>
* @param obj
 * AcisSLInterface object.
 * @param start
 * index of starting wire.
 * @param end
 * index of ending wire.
 * @param opts
 * ACIS options such as version or journal.
 **/
DECL_SKIN outcome api_breakup_wires_sli(
		AcisSLInterface *obj,			// AcisLoftingInterface
		int start = 0,					// index of starting wire
		int end   = 0,					// index of ending wire
		AcisOptions *opts = NULL
	);

/**
 * Aligns the start vertices of the wires in the skinning/lofting profiles.
 * <br><br>
 * <b>Role:</b> Performs an alignment of the start vertices of the wires in the
 * skinning/lofting profiles. This allows a %surface of minimum twist to be built.
 * <br><br>
 * <b>Effect:</b> Changes model
 * <br><br>
 * <b>Journal: </b> Available
 * <br><br>
 * <b>Product(s):</b> 3D ACIS Modeler
 * <br><br>
* @param obj
 * AcisSLInterface object.
 * @param start
 * index of starting  profile to be aligned.
 * @param end
 * index of ending  profile to be aligned.
 * @param opts
 * ACIS options such as version or journal.
 **/
DECL_SKIN outcome api_minimize_twist_wires_sli(
		AcisSLInterface *obj,
		int start = 0,					// index of starting wire
		int end	= 0,					// index of ending wire
		AcisOptions *opts = NULL
	);

/**
 * Reduces G<sup>1</sup> vertices in a wire body.
 * <br><br>
 * <b>Role:</b> The function removes the G<sup>1</sup> vertices of the intermediate wire profiles by
 * merging the adjacent coedges and edges. This reduces the coedge/edge count, which reduces the
 * number of surfaces as well as eliminating near tangent edges, which improves blending and shelling.
 * <br><br>
 * <b>Effect:</b> Changes model
 * <br><br>
 * <b>Journal: </b> Available
 * <br><br>
 * <b>Product(s):</b> 3D ACIS Modeler
 * <br><br>
* @param obj
 * interface object.
 * @param opts
 * ACIS options such as version or journal.
 **/
DECL_SKIN outcome api_simplify_wires_sli(
		AcisSLInterface *obj,
		AcisOptions *opts = NULL
	);

/**
 * Builds a list of skinning or lofting faces.
 * <br><br>
 * <b>Role:</b> The returned faces are not stitched together and no sheet body or solid is made.
 * <br><br>
 * <b>Effect:</b> Changes model
 * <br><br>
 * <b>Journal: </b> Available
 * <br><br>
 * <b>Product(s):</b> 3D ACIS Modeler
 * <br><br>
* @param obj
 * AcisSLInterface object.
 * @param face_list
 * returned face list.
 * @param opts
 * ACIS options such as version or journal.
 **/
DECL_SKIN outcome api_build_faces_sli(
		AcisSLInterface *obj,
		ENTITY_LIST &face_list,
		AcisOptions *opts = NULL
	);

/**
 * Removes the %surface conditions from the wires in the lofting profiles.
 * <br><br>
 * <b>Role:</b> This will effectively reduce the lofting operation to a skinning operation.
 * <br><br>
 * <b>Effect:</b> Read-only
 * <br><br>
 * <b>Journal: </b> Available
 * <br><br>
 * <b>Product(s):</b> 3D ACIS Modeler
 * <br><br>
* @param obj
 * AcisSLInterface object.
 * @param opts
 * ACIS options such as version or journal.
 **/
DECL_SKIN outcome api_lose_surface_conditions_li(
		AcisLoftingInterface *obj,
		AcisOptions *opts = NULL
	);

/**
 * Estimates the optimal magnitude to scale the take-off vectors on the loft profile cross section.
 * <br><br>
 * <b>Role:</b> This API estimates the factor by which all of the take-off
 * vectors should be scaled to obtain a "lofted" body with as large a minimum radius
 * of curvature as possible. It is important to note that this API provides a
 * single scale factor that should be applied simultaneously to all the take-off
 * vectors. Doing so will, for instance, produce a loft that may be shelled to a
 * greater thickness or allow its edges to be blended with larger blend radii. If
 * the user is only interested in a range of optimum values to apply then the last
 * two arguments to the API should not be supplied. This results in faster
 * execution times. Because the API estimates the magnitudes of the take-off vectors,
 * it will produce a scale factor equal to 1.0 (by default) if used with <i>skinned</i>
 * bodies.
 * <br><br>
 * <b>Effect:</b> Read-only
 * <br><br>
 * <b>Journal: </b> Available
 * <br><br>
 * <b>Product(s):</b> 3D ACIS Modeler
 * <br><br>
* @param obj
 * AcisSLInterface object.
 * @param range
 * range of magnitudes for the take-off vectors.
 * @param optimum
 * Optimal magnitude for the take-off vectors.
 * @param min_radius
 * minimum radius of curvature of the loft corresponding scale factor =  optimum.
 * @param opts
 * ACIS options such as version or journal.
 **/
DECL_SKIN outcome api_estimate_tangent_factor_scale_li(
		AcisLoftingInterface *obj,
		SPAinterval &range,
		double &optimum = *(double *)NULL_REF,
		double &min_radius = *(double *)NULL_REF,
		AcisOptions *opts = NULL
	);

/**
 * Sets the scale factors of the take-off vectors for the lofting operation.
 * <br><br>
 * <b>Role:</b> The function allows the user to determine these values optimally and reset the
 * values in the <tt>AcisLoftingInterface</tt> object.
 * <br><br>
 * <b>Effect:</b> Read-only
 * <br><br>
 * <b>Journal: </b> Available
 * <br><br>
 * <b>Product(s):</b> 3D ACIS Modeler
 * <br><br>
* @param obj
 * AcisLoftingInterface object.
 * @param tan_factors
 * array of tangent factors.
 * @param opts
 * ACIS options such as version or journal.
 **/
DECL_SKIN outcome api_set_tangent_factors_li(
		AcisLoftingInterface *obj,
		double *tan_factors,
		AcisOptions *opts = NULL
	);

/**
 * Gets the current set of tangent factors on the loft profiles.
 * <br><br>
 * <b>Effect:</b> Read-only
 * <br><br>
 * <b>Journal: </b> Available
 * <br><br>
 * <b>Product(s):</b> 3D ACIS Modeler
 * <br><br>
* @param obj
 * AcisLoftingInterface object.
 * @param tan_factors
 * returned array of tangent factors.
 * @param opts
 * ACIS options such as version or journal.
 **/
DECL_SKIN outcome api_get_tangent_factors_li(
		AcisLoftingInterface *obj,
		double *&tan_factors,
		AcisOptions *opts = NULL
	);

/**
 * Estimates the magnitude of the tangent vector field to build surfaces with a minimum radius of curvature.
 * <br><br>
 * <b>Role:</b> The estimation should help the user to estimate the maximum thickness
 * that the skinned %surface can possibly be shelled and the maximum blend radii that can possibly
 * be applied to the edges of the skin body. This API works on both skin and loft bodies.
 * <br><br>
 * <b>Effect:</b> Read-only
 * <br><br>
 * <b>Journal: </b> Available
 * <br><br>
 * <b>Product(s):</b> 3D ACIS Modeler
 * <br><br>
* @param obj
 * AcisSLInterface object.
 * @param min_rad
 * minimum radius of curvature that can be used.
 * @param opts
 * ACIS options such as version or journal.
 **/
DECL_SKIN outcome api_estimate_min_rad_curvature_skin(
		AcisSLInterface *obj,
		double &min_rad,
		AcisOptions *opts = NULL
	);

/**
 * Adds a guide %curve to a set of skinning or lofting profiles.
 * <br><br>
 * <b>Role:</b> This function adds a guide %curve to the <tt>AcisSLInterface</tt>.
 * <br><br>
 * For more information on the use of guide curves, see the <i>Skinning with Guide Curves</i> or
 * <i>Skinning with Virtual Guide Curves</i> section of the <i>Functional Variations of Skinning</i> 
 * Technical Article, or the <i>Loft with Guides</i> section of the <i>Functional Variations of Lofting</i>
 * Technical Article. 
 * <br><br>
 * <b>Effect:</b> Read-only
 * <br><br>
 * <b>Journal: </b> Available
 * <br><br>
 * <b>Product(s):</b> 3D ACIS Modeler
 * <br><br>
* @param obj
 * interactive skinning or lofting interface object.
 * @param guide
 * guide %curve to add.
 * @param opts
 * ACIS options such as version or journal.
 **/
DECL_SKIN outcome api_add_guide_curve_si(
		AcisSLInterface *obj,
		EDGE *guide,
		AcisOptions *opts = NULL
	);

/**
 * Adds a mapping %curve to a set of skinning or lofting profiles.
 * <br><br>
 * <b>Role:</b> This function adds a mapping %curve to the <tt>AcisSLInterface</tt>.
 * A mapping %curve controls the breakup of the profiles such that the position in the mapping
 * %curve are guaranteed to be aligned.
 * <br><br>
 * For more information on the use of mapping curves see the <i>Mapping Curves with Skinning and Lofting</i>
 * section of the <i>Skinning and Lofting Interactive Interface</i> Technical Article.
 * <br><br>
 * <b>Limitations:</b> Mapping curves are only allowed on closed profiles.
 * <br><br>
 * <b>Effect:</b> Read-only
 * <br><br>
 * <b>Journal: </b> Available
 * <br><br>
 * <b>Product(s):</b> 3D ACIS Modeler
 * <br><br>
* @param obj
 * AcisSLInterface object.
 * @param num
 * number of positions in mapping %curve.
 * @param pts
 * array of positions.
 * @param opts
 * ACIS options such as version or journal.
 **/
DECL_SKIN outcome api_add_mapping_curve_sli(
		AcisSLInterface *obj,
		int num,
		SPAposition *pts,
		AcisOptions *opts = NULL
	);

/**
 * Removes a mapping %curve from the <tt>AcisSLInterface</tt>.
 * <br><br>
 * <b>Role:</b> The mapping %curve removed is given by the second parameter, <tt>index</tt>.
 * <br><br>
 * <b>Effect:</b> Read-only
 * <br><br>
 * <b>Journal: </b> Available
 * <br><br>
 * <b>Product(s):</b> 3D ACIS Modeler
 * <br><br>
* @param obj
 * AcisSLInterface object.
 * @param index
 * index of mapping %curve.
 * @param opts
 * ACIS options such as version or journal.
 **/
DECL_SKIN outcome api_remove_mapping_curve_sli(
		AcisSLInterface *obj,
		int index,
		AcisOptions *opts = NULL
	);

/**
 * Removes all the mapping curves from the <tt>AcisSLInterface</tt>.
 * <br><br>
 * <b>Effect:</b> Read-only
 * <br><br>
 * <b>Journal: </b> Available
 * <br><br>
 * <b>Product(s):</b> 3D ACIS Modeler
 * <br><br>
* @param obj
 * AcisSLInterface object.
 * @param opts
 * ACIS options such as version or journal.
 **/
DECL_SKIN outcome api_clear_mapping_curves_sli(
 		AcisSLInterface *obj,
		AcisOptions *opts = NULL
	);

/**
 * Gets a list of the mapping curves that currently exist in the <tt>AcisSLInterface</tt>.
 * <br><br>
 * <b>Role:</b> This function returns a list of edges that represent the mapping curves in the
 * <tt>AcisSLInterface</tt>.
 * <br><br>
 * <b>Errors:</b> The mapping %curve is bad; possibly intersecting another mapping %curve.
 * <br><br>
 * <b>Effect:</b> Read-only
 * <br><br>
 * <b>Journal: </b> Available
 * <br><br>
 * <b>Product(s):</b> 3D ACIS Modeler
 * <br><br>
* @param obj
 * AcisSLInterface object.
 * @param edgeList
 * List of mapping curves (edges).
 * @param opts
 * ACIS options such as version or journal.
 **/
DECL_SKIN outcome api_make_mapping_curves_sli(
		AcisSLInterface *obj,
		ENTITY_LIST &edgeList,
		AcisOptions *opts = NULL
	);

/**
 * Clears the guide curves in the <tt>AcisSLInterface</tt>.
 * <br><br>
 * <b>Effect:</b> Read-only
 * <br><br>
 * <b>Journal: </b> Available
 * <br><br>
 * <b>Product(s):</b> 3D ACIS Modeler
 * <br><br>
* @param obj
 * AcisSLInterface object.
 * @param opts
 * ACIS options such as version or journal.
 **/
DECL_SKIN outcome api_clear_guide_curves_sli(
		AcisSLInterface *obj,
		AcisOptions     *opts = NULL
	);

/**
 * Gets a list of the virtual guide curves that currently exist in the <tt>AcisSkinningInterface</tt>.
 * <br><br>
 * <b>Role:</b> This function creates and returns a list of edges that represent the virtual 
 * guide curves in the <tt>AcisSkinningInterface</tt>.
 * <br><br>
 * <b>Effect:</b> Read-only
 * <br><br>
 * <b>Journal: </b> Available
 * <br><br>
 * <b>Product(s):</b> 3D ACIS Modeler
 * <br><br>
* @param obj
 * AcisSkinningInterface object.
 * @param edgeList
 * virtual guides curves (edges).
 * @param opts
 * ACIS options such as version or journal.
 **/
DECL_SKIN outcome api_show_guides_si(
		AcisSkinningInterface *obj,
		ENTITY_LIST &edgeList,
		AcisOptions *opts = NULL
	);

/**
 * Modifies the position of a vertex on an intermediate skinning or lofting wire.
 * <br><br>
 * <b>Role:</b> It can only be used in this context. Additionally, only non-corner vertices can be moved.
 * <br><br>
 * <b>Errors:</b> A non-movable vertex is specified.
 * <br><br>
 * <b>Effect:</b> Changes model
 * <br><br>
 * <b>Journal: </b> Available
 * <br><br>
 * <b>Product(s):</b> 3D ACIS Modeler
 * <br><br>
* @param obj
 * AcisSLInterface object.
 * @param coedge1
 * first coedge.
 * @param coedge2
 * second coedge.
 * @param pos
 * position of vertex to be moved.
 * @param wire
 * Specific skinning or lofting intermediate wire containing the coedges.
 * @param opts
 * ACIS options such as version or journal.
 **/
DECL_SKIN outcome api_move_vertex_sli(
		AcisSLInterface *obj,
		COEDGE *coedge1,
		COEDGE *coedge2,
		const SPAposition &pos,
		WIRE *wire,
		AcisOptions *opts = NULL
	);

/*! @} */
/** @} */
#endif
