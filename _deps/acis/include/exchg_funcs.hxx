/*******************************************************************/
/*    Copyright (c) 1989-2020 by Spatial Corp.                     */
/*    All rights reserved.                                         */
/*    Protected by U.S. Patents 5,257,205; 5,351,196; 6,369,815;   */
/*                              5,982,378; 6,462,738; 6,941,251    */
/*    Protected by European Patents 0503642; 69220263.3            */
/*    Protected by Hong Kong Patent 1008101A                       */
/*******************************************************************/
#if !defined( EXCHG_FUNCS_HXX )
#define EXCHG_FUNCS_HXX

#include "dcl_bool.h"
#include "logical.h"
#include "api.hxx"

class BODY;
class ENTITY;
class FACE;
class AcisOptions;
class unstitch_nm_options;
class convert_to_spline_options;

/*! \addtogroup BOOLADVSTITCH
*  @file exchg_funcs.hxx
*  @{
*/

/**
* Decomposes an input body along its non-manifold vertices and edges.
* <br><b>Technical Article:</b> <i>[Unstitching Nonmanifold Bodies]
* (http://doc.spatial.com/articles/a/d/d/Additional_Boolean_Operations_b537.html#Unstitching_Nonmanifold_Bodies)</i>
* <br><br>
* <b>Role:</b> This API function decomposes the input body into up to four bodies, each consisting
* of the input body's manifold lumps, sheets, lamina, and wires. The input body is
* destroyed.
* <br><br>
* <b>Definition of Non-manifold</b><br>
* A non-manifold edge has more than two coedges around it. A non-manifold vertex
* has elements that can be connected topologically only through that vertex, for
* example, two cones meeting at their apexes, or a vertex of a block with a
* dangling edge. Although @href api_manifold_class also reports three or more
* wire edges at a vertex as non-manifold, this API does not unstitch them.
* <br><br>
* For more information on manifold and non-manifold objects refer to the Technical Article
* <i>Manifold and Non-manifold Objects</i>.
* <br><br>
* <b>Information Returned</b><br>
* All faces and wire edges from the input body are contained somewhere in the four
* returned bodies. The four bodies returned contain the manifold lumps, the
* maximal manifold sheets, lamina (doubly covered) faces, and the wires found in
* the input body.
* <br><br>
* The first body returned by this API function (<tt>lumps</tt>) contains the manifold
* lumps from the input body, each with one peripheral shell and any void shells
* remaining.
* <br><br>
* The second body <tt>sheet</tt> contains each sheet in a separate lump, with exactly one
* shell in each lump.
* <br><br>
* The third body <tt>lamina</tt> contains one lamina face (two back-to-back faces) in
* each lump.
* <br><br>
* The last body <tt>wires</tt> has all wires from the input body in its wire pointer, and
* has no lumps or shells. Each wire is maximal in that it contains all wire edges
* that are topologically connected (through a coedge next or previous pointer) to
* the first edge referenced by the wire entity.
* <br><br>
* Any bodies that would be returned empty, such as no sheets found, are returned
* as NULL.
* <br><br>
* All non-manifold vertices and edges are unstitched so that they are manifold,
* except for shell internally-non-manifold edges as described below.
* <br><br>
* All shared geometry is duplicated.
* <br><br>
* <b>Errors:</b> <tt>input_body</tt> is a NULL pointer or does not point to a @href BODY.
* <br><br>
* <b>Limitations:</b><br>
* This API function does not unstitch shell internally-non-manifold edges.
* These are non-manifold edges whose owning shell would still be connected after
* unstitching and would result in a self-intersecting body. This function does unstitch
* shell internally-non-manifold vertices; however, in this case, the result will have
* duplicate vertices. (Use the signature of @href api_unstitch_nonmani with @href unstitch_nm_options
* to control the unstitching of internally non-manifold entities).
* <br><br>
* This API function does not unstitch non-manifold wires.
* <br><br>
* <b>Effect:</b> Changes model.
* <br><br>
* <b>Journal:</b> Available
* <br><br>
* <b>Product(s):</b> 3D ACIS Modeler, 3D ACIS Exchange, 3D Viz Exchange
* <br><br>
* @param input_body
* (in) Body to decompose into manifold parts.
* @param lumps
* (out) Returned body with each lump, a manifold lump.
* @param sheet
* (out) Returned body with each lump, a manifold sheet.
* @param lamina
* (out) Returned body with each lump, a lamina face.
* @param wires
* (out) Returned body containing wires from input body.
* @param ao
* (in) ACIS options such as versioning and journaling.
**/
DECL_BOOL outcome api_unstitch_nonmani(
	BODY *input_body,                   //    Body to be decomposed
	BODY *&lumps,                             //    Holds shells
	BODY *&sheet,                             //    Holds sheets
	BODY *&lamina,                            //    Holds lamina
	BODY *&wires,                             //    Holds wires
	AcisOptions* ao = NULL
);

/**
* Decomposes an input body along its non-manifold vertices and edges.
* <br><b>Technical Article:</b> <i>[Unstitching Nonmanifold Bodies]
* (http://doc.spatial.com/articles/a/d/d/Additional_Boolean_Operations_b537.html#Unstitching_Nonmanifold_Bodies)</i>
* <br><br>
* <b>Role:</b> This API function decomposes the input body into up to four bodies, each consisting
* of the input body's manifold lumps, sheets, lamina, and wires. The input body is
* destroyed.
* <br><br>
* <b>Definition of Non-manifold</b><br>
* A non-manifold edge has more than two coedges around it. A non-manifold vertex
* has elements that can be connected topologically only through that vertex,
* for example, two cones meeting at their apexes, or a vertex of a block with a
* dangling edge. Although @href api_manifold_class also reports three or more
* wire edges at a vertex as non-manifold, this API does not unstitch them.
* <br><br>
* For more information on manifold and non-manifold objects refer to the Technical Article
* <i>[Manifold and Non-manifold Objects](http://doc.spatial.com/articles/a/c/i/Tutorial~ACIS_Tutorials_%28Topology%29_bc77.html#Manifold_and_Non-manifold_Objects)</i>
* <br><br>
* <b>Information Returned</b><br>
* All faces and wire edges from the input body are contained somewhere in the four
* returned bodies. The four bodies returned contain the manifold lumps, the
* maximal manifold sheets, lamina (doubly covered) faces, and the wires found in
* the input body.
* <br><br>
* The first body returned by this API function (<tt>lumps</tt>) contains the manifold
* lumps from the input body, each with one peripheral shell and any void shells
* remaining.
* <br><br>
* The second body <tt>sheet</tt> contains each sheet in a separate lump, with exactly
* one shell in each lump.
* <br><br>
* The third body <tt>lamina</tt> contains one lamina face (two back-to-back faces) in
* each lump.
* <br><br>
* The last body <tt>wires</tt> has all wires from the input body in its wire pointer, and
* has no lumps or shells. Each wire is maximal in that it contains all wire edges
* that are topologically connected (through a coedge next or previous pointer) to
* the first edge referenced by the wire entity.
* <br><br>
* Any bodies that would be returned empty, such as no sheets found, are returned
* as NULL.
* <br><br>
* Internally non-manifold edges and vertices are unstitched according to the value
* set in the @href unstitch_nm_options object.
* <br><br>
* All shared geometry is duplicated.
* <br><br>
* <b>Controlling the Unstitching of Internally Non-manifold Entities</b><br>
* In order to provide control over the unstitching of internally non-manifold entites,
* this version of the API requires a pointer to an options object (@href unstitch_nm_options).
* Internally non-manifold edges and vertices are those whose owning shell would still be
* connected after unstitching, and would result in a body with duplicate edges or duplicate vertices.
* <br><br>
* The options object takes four possible values (refer to @href UNSTITCH_NM_OPTION):
* <ul>
* <li> <tt>UNSTITCH_NM_ALL_BUT_INTERNAL_NM_EDGES</tt> - Unstitch all non-manifold entities,
* including internally nm-vertices but not including internally nm-edges (default).</li>
* <li> <tt>UNSTITCH_NM_ALL_NMS</tt> - Unstitch all non-manifold entities, including internally
* nm-vertices and nm-edges.</li>
* <li> <tt>UNSTITCH_NM_ALL_BUT_INTERNAL_NMS</tt> - Unstitch all non-manifold entities, except
* internally nm-edges and nm-vertices.</li>
* <li> <tt>UNSTITCH_NM_ALL_NMS_AND_REPORT_INVALIDITY</tt> - Same behavior as
* <tt>UNSTITCH_NM_ALL_NMS</tt> and throw error if lumps contain duplicate vertices.</li>
* </ul>
* <br>
* Reasons behind these options:
* <ul>
* <li>Option 0 maintains the behavior of the version of the API function without the options object. </li>
* <li>Option 1 provides consistency to the unstitching of all non-manifold entities. </li>
* <li>Option 2 leads to no duplicate vertices in the resulting lumps, as it does not unstitch
* internally non-manifold entities.</li>
* <li>Option 3 is self explanatory. (Refer to @href unstitch_nm_options
* and @href UNSTITCH_NM_OPTION.)</li>
* </ul>
* <br>
* <b>Errors:</b> <tt>input_body</tt> is a NULL pointer or does not point to a @href BODY.
* <br><br>
* <b>Limitations:</b><br>
* This API function does not unstitch non-manifold wires.
* <br><br>
* <b>Effect:</b> Changes model.
* <br><br>
* <b>Journal:</b> Available
* <br><br>
* <b>Product(s):</b> 3D ACIS Modeler, 3D ACIS Exchange, 3D Viz Exchange
* <br><br>
* @param input_body
* (in) Body to decompose into manifold parts.
* @param lumps
* (out) Returned body with each lump, a manifold lump.
* @param sheet
* (out) Returned body with each lump, a manifold sheet.
* @param lamina
* (out) Returned body with each lump, a lamina face.
* @param wires
* (out) Returned body containing wires from input body.
* @param usops
* (in) Unstitch non-mani options for the treatment of internal non-manifolds.
* @param ao
* (in) ACIS options such as versioning and journaling.
**/

DECL_BOOL outcome api_unstitch_nonmani(
	BODY *input_body,				//	Body to be decomposed
	BODY *&lumps,					//	Holds shells
	BODY *&sheet,					//	Holds sheets
	BODY *&lamina,					//	Holds lamina
	BODY *&wires,					//	Holds wires
	unstitch_nm_options* usops,
	AcisOptions* ao = NULL
);


/**
* Splits a face along a specified isoparameter curve.
* <br><b>Technical Article:</b> <i>[Splitting Along an Isoparameter Curve]
* (http://doc.spatial.com/articles/a/d/d/Additional_Boolean_Operations_b537.html#Splitting_Along_an_Isoparameter_Curve)</i>
* <br><br>
* <b>Role:</b> This API function splits a face along a <i>u</i> or <i>v</i> isoparameter %curve.
* It will split along <i>u</i> if the argument <tt>split_u</tt> is set to <tt>TRUE</tt>, or along <i>v</i>
* if <tt>split_u</tt> is set to <tt>FALSE</tt>. The position of the splitting %curve can be specified
* in two ways:
* <ul>
* <li>By an explicit parameter value <tt>p</tt>, which must be within the range of the %surface.
* In this case, <tt>use_percent</tt> should be set to <tt>FALSE</tt>.</li>
* <li>By a percentage value <tt>p</tt>, which must be between 0 and 1.
* In this case, <tt>use_percent</tt> should be set to <tt>TRUE</tt>.</li>
* </ul>
* <br>
* When a percentage value is specified, the value of the parameter is a
* percentage of the parameter range of the %surface unless the parameter range
* is not finite, in which case it is a percentage of the width of the parameter
* box of the face in that parameter direction. Note that the extent of a
* parameter box is not guaranteed to remain the same for different versions
* of ACIS, so the exact value of the parameter in these cases is not
* well-defined.
* <br><br>
* To split a face along isoparametric lines at G1 or G2 discontinuities
* @href api_split_face_at_disc is recommended.
* <br><br>
* <b>Errors:</b><br>
* <tt>face</tt> is a NULL pointer or does not point to a spline @href FACE.<br>
* Parameter value or percentage is incorrect.
* <br><br>
* <b>Effect:</b> Changes model.
* <br><br>
* <b>Journal:</b> Available
* <br><br>
* <b>Product(s):</b> 3D ACIS Modeler, 3D ACIS Exchange, 3D Viz Exchange
* <br><br>
* @param face
* (in) Face to be split.
* @param split_u
* (in) Flag indicating whether to split along <i>u</i> or <i>v</i>.
* @param use_percent
* (in) Flag indicating whether to treat <tt>p</tt> as an explicit parameter value or a percentage.
* @param p
* (in) Parameter value or percentage at which to split.
* @param ao
* (in) ACIS options such as versioning and journaling.
**/
DECL_BOOL outcome api_split_face(
	FACE* face,                // face to be split
	logical split_u,           // split along a constant u-parameter curve
	logical use_percent,       // use percentage vs explicit param value
	double p,                  // SPAparameter value or percentage at which to split the face
	AcisOptions* ao = NULL
);

/*! @} */

/*! \addtogroup BOOLADVOTHER
*  @file exchg_funcs.hxx
*  @{
*/

/**
* Converts the analytic geometry of an entity into a B-spline representation (precise conversion).
* <br><b>Technical Article:</b> <i>[Spline Conversion]
* (http://doc.spatial.com/articles/s/p/l/Spline_Conversion_5118.html)</i>
* <br><br>
* <b>Role:</b> This API function converts an entity with analytical surfaces (such as tori, plane, sphere,
* or cone) into B-splines through the following stages:
* <ol>
* <li>Creates a copy of the entity.</li>
* <li>Splits faces on periodic surfaces along the seams.</li>
* <li>Splits edges at poles on surfaces with point singularities.</li>
* <li>Converts the underlying geometry for the faces using @href bs3_surface_make_sur
* and other functions.</li>
* <li>Uses @href FACE#set_geometry to add this geometry to the face.</li>
* <li>Calls @href sg_add_pcurves_to_entity to create pcurves from the loop geometry.</li>
* <li>Trims the face's surface to the parameter bounds of the face.</li>
* </ol>
* <br>
* Refer to the documentation for the option <tt>new_periodic_splitting</tt>,
* which controls how periodic faces are split.
* <br><br>
* <b><i>Note:</i></b> The analytic surfaces are converted to NURBS surfaces; thus,
* no geometric gap exists between a spline-converted (NURBS) %surface and the
* corresponding original (analytic) %surface. However, a parameterization change
* occurs because a NURBS circle does not have a constant velocity parameterization.
* <br><br>
* <b><i>Note:</i></b> Planes are converted to NUBS; spheres, cones, and tori are converted to NURBS.
* <br><br>
* <b>Errors:</b> <tt>given_entity</tt> is a NULL pointer.
* <br><br>
* <b>Effect:</b> Read-only (a new model is created).
* <br><br>
* <b>Journal:</b> Available
* <br><br>
* <b>Product(s):</b> 3D ACIS Modeler, 3D ACIS Exchange, 3D Viz Exchange
* <br><br>
* @param given_entity
* (in) Entity (<tt>BODY</tt>, <tt>FACE</tt>, <tt>SHELL</tt>, or <tt>EDGE</tt>) to be
* converted to a B-spline or NURB.
* @param return_entity
* (out) Resulting B-spline or NURB entity.
* @param ao
* (in) ACIS options such as versioning and journaling.
**/
DECL_BOOL outcome api_convert_to_spline(
	ENTITY* given_entity,		// entity to convert to a spline representation
	ENTITY*& return_entity, 		// spline representation of given entity
	AcisOptions* ao = NULL
);

/**
* Converts the geometry associated with an entity into a B-spline representation (precise conversion).
* <br><b>Technical Article:</b> <i>[Spline Conversion]
* (http://doc.spatial.com/articles/s/p/l/Spline_Conversion_5118.html)</i>
* <br><br>
* <b>Role:</b> This signature of this API function will convert curves and/or surfaces.
* It handles curves that are either analytic or procedural; however,
* only analytic surfaces are converted. Planes are converted to NUBS, while
* spheres, cones, and tori are converted to NURBS. By default, the conversion is
* done in a failsafe manner, that is, this function does all the conversions it can
* regardless of any failures along the way. (Refer to the Technical Article
* <i>Failsafe Behavior</i>.)
* <br><br>
* To ensure that converted curves have the same direction as the original edges (as
* opposed to the original curves), set the
* option <tt>spl_edges_forward</tt> to <tt>TRUE</tt> before calling this function.
* <br><br>
* The conversion of analytical surfaces (such as tori, planes, spheres, or cones) into
* B-spline representations involves the following stages:
* <ol>
* <li>Creates a copy of the entity (<i>if</i> in-place modification is turned off by
* means of the @href convert_to_spline_options argument).</li>
* <li>Splits faces on periodic surfaces along the seams.</li>
* <li>Splits edges at poles on surfaces with point singularities.</li>
* <li>Converts the underlying geometry for the faces using @href bs3_surface_make_sur
* and other functions.</li>
* <li>Uses @href FACE#set_geometry to add the resulting geometry to the face.</li>
* <li>Calls @href sg_add_pcurves_to_entity to create pcurves from the loop geometry.</li>
* <li>Trims the face's %surface according to the parameter bounds of the face.</li>
* </ol>
* <br>
* Refer to the documentation for the option <tt>new_periodic_splitting</tt>,
* which controls how periodic faces are split.
* <br><br>
* The argument <tt>ctso</tt> contains options that affect the way in which %spline
* conversion is done. As mentioned above, it may be used to turn off in-place
* modification. It may also be used to suppress the conversion of either
* faces or edges. Refer to the documentation of the @href convert_to_spline_options
* class for details on toggling these options.
* <br><br>
* <b><i>Note:</i></b> The analytic surfaces are converted to NURBS surfaces; thus,
* no geometric gap exists between a spline-converted (NURBS) %surface and the
* corresponding original (analytic) %surface. However, a parameterization change
* occurs because a NURBS circle does not have a constant velocity parameterization.
* <br><br>
* <b>Errors:</b> <tt>given_entity</tt> is a NULL pointer.
* <br><br>
* <b>Effect:</b> Changes model, unless the option for in-place modification is deselected
* through the parameter <tt>ctso</tt>. In the latter case, this function is read-only.
* <br><br>
* <b>Journal:</b> Not Available
* <br><br>
* <b>Product(s):</b> 3D ACIS Modeler, 3D ACIS Exchange, 3D Viz Exchange
* <br><br>
* @param given_entity
* (in) Entity (<tt>BODY</tt>, <tt>FACE</tt>, <tt>SHELL</tt>, or <tt>EDGE</tt>) whose
* geometry is to be converted.
* @param ctso
* (in) Options affecting %spline conversion.
* @param ao
* (in) ACIS options such as versioning and journaling.
**/
DECL_BOOL outcome api_convert_to_spline(
	ENTITY*						given_entity,
	convert_to_spline_options*	ctso = NULL,
	AcisOptions*				ao = NULL
);


/*! @} */


#endif // ! exchg_funcs_hxx


