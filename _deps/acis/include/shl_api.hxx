/*******************************************************************/
/*    Copyright (c) 1989-2020 by Spatial Corp.                     */
/*    All rights reserved.                                         */
/*    Protected by U.S. Patents 5,257,205; 5,351,196; 6,369,815;   */
/*                              5,982,378; 6,462,738; 6,941,251    */
/*    Protected by European Patents 0503642; 69220263.3            */
/*    Protected by Hong Kong Patent 1008101A                       */
/*******************************************************************/
//                Header for SPAshl API routines.
/*******************************************************************/
#if !defined( SHL_HUSK_API_ROUTINES )
#define SHL_HUSK_API_ROUTINES
#include "dcl_shl.h"
#include "api.hxx"
/**
 * @file shl_api.hxx
 * @CAA2Level L1
 * @CAA2Usage U1
 * \defgroup SHLAPI Shelling
 *
 * \ingroup LOCALOPS
 * @{
 */
class FACE;
class BODY;
class SPAposition;
class AcisOptions;
class lop_options;
class mid_sheet_body_options;

/**
 * Initializes the shelling library.
 * <br><br>
 * <b>Effect:</b> System routine
 * <br><br>
 * <b>Journal: </b> Not Available
 * <br><br>
 * <b>Product(s):</b> 3D ACIS Modeler
 * <br><br>
**/
DECL_SHL outcome api_initialize_shelling();

/**
 * Terminates the shelling library.
 * <br><br>
 * <b>Effect:</b> System routine
 * <br><br>
 * <b>Journal: </b> Not Available
 * <br><br>
 * <b>Product(s):</b> 3D ACIS Modeler
 * <br><br>
**/
DECL_SHL outcome api_terminate_shelling();

/**
 * Creates a thin walled solid shell from a solid body.
 * <b>Technical Article:</b> <i>[Hollow Body](http://doc.spatial.com/articles/s/h/e/Shelling_Operations_5d46.html#Hollow_Body)</i>
 * <br><br>
 * <b>Role:</b> Shells the supplied body by the specified thickness, omitting the specified faces,
 * thus generating openings into the internal void.
 * <br><br>
 * Body faces with radial surfaces that cannot be offset by the given thickness are removed and the
 * resulting wound healed by the surrounding face surfaces. Radial surfaces are spheres, cones,
 * tori, blended edges and blended vertices.
 * <br><br>
 * This generally leads to shells which are locally overly thick. However in the case of vertex blends
 * with large setbacks being removed, the shell may be locally too thin, or may even have a hole.
 * <br><br>
 * The box low and high arguments are used to limit any geometrical intersections and thereby improve
 * performance. Limiting the intersection improves performance. The box must contain the final faces
 * and if omitted, defaults to the size box. SPAposition(0,0,0) can be used as default value for both box-low
 * and box-high when user is unable to specify a valid box.
 * <br><br>
 * Multi-lump bodies may be hollowed, as long as at least one lump has only one shell. Lumps with
 * more than one shell are not hollowed.
 * <br><br>
 * Mergeable edges will be retained if they have a <tt>NO_MERGE_ATTRIB</tt>.
 * <br><br>
 * <b>Topology Changes:</b>
 * <br><br>
 * Refer to the topology changes listed for the function, @href api_tweak_faces.
 * <br><br>
 * <b>Geometry Changes:</b>
 * <br><br>
 * Refer to the geometry changes listed for the function, @href api_tweak_faces.
 * <br><br>
 * <b>Errors:</b> In addition to the following, refer to the Errors listed for the function, @href api_tweak_faces.
 * <br><br>
 * Valid offset (greater than minus half the body box max side), and not a zero offset
 * (magnitude greater than twice <tt>SPAresabs</tt>), or error <tt>LOP_OFF_BAD_OFFSET</tt>.
 * <br><br>
 * Valid body transformation (no shear component), or error <tt>LOP_BAD_BODY_TRANSFORM</tt>.
 * <br><br>
 * Body must have at least one lump with only one shell, or error <tt>LOP_HOL_MULTI_SHELL</tt>.
 * <br><br>
 * Some faces must remain unopen, or error <tt>LOP_HOL_ALL_OPEN</tt>.
 * <br><br>
 * The model is too large or is positioned outside the modeling space, then error <tt>LOP_OFF_BAD_MODEL_EXTENTS</tt>.
 * <br><br>
 * <b>Limitations:</b> Refer to the Limitations listed for the function, @href api_tweak_faces.
 * <br><br>
 * <b>Effect:</b> Changes model
 * <br><br>
 * <b>Journal: </b> Available
 * <br><br>
 * <b>Product(s):</b> 3D ACIS Modeler, 3D ACIS Polyhedral
 * <br><br>
 * @param body
 * body to be shelled.
 * @param nopenface
 * number of faces being omitted.
 * @param openface
 * faces being omitted.
 * @param thickness
 * distance to offset.
 * @param box_low
 * start of intersection box to be used.
 * @param box_high
 * end of intersection box to be used.
 * @param pLopts
 * local operations options. NULL argument will set the default values.
 * @param ao
 * ACIS options.
 **/
  DECL_SHL outcome api_hollow_body(
								BODY*        body,
								int const    nopenface,
								FACE*        openface[],
								double       thickness,
								const SPAposition&  box_low,
								const SPAposition&  box_high,
								lop_options* pLopts,
								AcisOptions* ao = NULL
								);
/**
 * @nodoc
 * Creates a thin walled solid shell from a solid body.
 * <br><br>
 * <b>Role:</b> Shells the supplied body by the specified thickness, omitting the specified faces,
 * thus generating openings into the internal void.
 * <br><br>
 * Body faces with radial surfaces, which cannot be offset by the thickness, are removed and the
 * resulting wound healed by the surrounding face surfaces. Radial surfaces are spheres, cones,
 * tori, blended edges and blended vertices.
 * <br><br>
 * This generally leads to shells which are locally overly thick. However in the case of vertex blends
 * with large setbacks being removed, the shell may be locally too thin, or may even have a hole.
 * <br><br>
 * The box low and high arguments are used to limit any geometrical intersections and so, improve
 * performance. Limiting the intersection improves performance. The box must contain the final faces
 * and if omitted, defaults to the size box.
 * <br><br>
 * Multi-lump bodies may be hollowed, as long as at least one lump has only one shell. Lumps with
 * more than one shell are not hollowed.
 * <br><br>
 * Mergeable edges will be retained if they have a <tt>NO_MERGE_ATTRIB</tt>.
 * <br><br>
 * <b>Topology Changes:</b>
 * <br><br>
 * Refer to the topology changes listed for the function, @href api_tweak_faces.
 * <br><br>
 * <b>Geometry Changes:</b>
 * <br><br>
 * Refer to the geometry changes listed for the function, @href api_tweak_faces.
 * <br><br>
 * <b>Errors:</b> In addition to the following, refer to the Errors listed for the function, @href api_tweak_faces.
 * <br><br>
 * Valid offset (greater than minus half the body box max side), and not a zero offset
 * (magnitude greater than twice <tt>SPAresabs</tt>), or error <tt>LOP_OFF_BAD_OFFSET</tt>.
 * <br><br>
 * Valid body transformation (no shear component), or error <tt>LOP_BAD_BODY_TRANSFORM</tt>.
 * <br><br>
 * Body must have at least one lump with only one shell, or error <tt>LOP_HOL_MULTI_SHELL</tt>.
 * <br><br>
 * Some faces must remain unopen, or error <tt>LOP_HOL_ALL_OPEN</tt>.
 * <br><br>
 * The model is too large or is positioned outside the modeling space, then error <tt>LOP_OFF_BAD_MODEL_EXTENTS</tt>.
 * <br><br>
 * <b>Limitations:</b> Refer to the Limitations listed for the function, @href api_tweak_faces.
 * <br><br>
 * <b>Effect:</b> Changes model
 * <br><br>
 * <b>Journal: </b> Not Available
 * <br><br>
 * <b>Product(s):</b> 3D ACIS Modeler, 3D ACIS Polyhedral
 * <br><br>
 * @param body
 * body to be shelled.
 * @param nopenface
 * number of faces being omitted.
 * @param openface
 * faces being omitted.
 * @param thickness
 * distance to offset.
 * @param box_low
 * start of intersection box to be used.
 * @param box_high
 * end of intersection box to be used.
 * @param ao
 * ACIS options.
 **/
 DECL_SHL outcome api_hollow_body(
								BODY*        body,
								int const    nopenface,
								FACE*        openface[],
								double       thickness,
								SPAposition  box_low,
								SPAposition  box_high,
								AcisOptions* ao = NULL
								);
/**
 * Creates a thin walled solid shell from a solid body, with default and specific inner and outer offsets.
 * <b>Technical Article:</b> <i>[Hollow Body](http://doc.spatial.com/articles/s/h/e/Shelling_Operations_5d46.html#Hollow_Body)</i>
 * <br><br>
 * <b>Role:</b> Shells the supplied body by the offset distances, omitting the specified open faces,
 * thus generating openings into the internal void. The default offsets are applied to all faces
 * except the open faces, unless overridden by specific offsets.
 * <br><br>
 * Body faces with radial surfaces that cannot be offset by the given thickness are removed and the
 * resulting wound healed by the surrounding face surfaces. Radial surfaces are spheres, cones,
 * tori, blended edges and blended vertices.
 * <br><br>
 * This generally leads to shells which are locally overly thick. However in the case of vertex blends
 * with large setbacks being removed, the shell may be locally too thin, or may even have a hole.
 * <br><br>
 * The box low and high arguments are used to limit any geometrical intersections and thereby improve
 * performance. Limiting the intersection improves performance. The box must contain the final faces
 * and if omitted defaults to the size box. SPAposition(0,0,0) can be used as default value for both box-low
 * and box-high when user is unable to specify a valid box.
 * <br><br>
 * Multi-lump bodies may be hollowed, as long as at least one lump has only one shell. Lumps with
 * more than one shell are not hollowed.
 * <br><br>
 * Mergeable edges will be retained if they have a <tt>NO_MERGE_ATTRIB</tt> attached.
 * <br><br>
 * <b>Topology Changes:</b>
 * <br><br>
 * Refer to the topology changes listed for the function, @href api_tweak_faces.
 * <br><br>
 * <b>Geometry Changes:</b>
 * <br><br>
 * Refer to the geometry changes listed for the function, @href api_tweak_faces.
 * <br><br>
 * <b>Errors:</b> In addition to the following, refer to the Errors listed for the function, @href api_tweak_faces.
 * <br><br>
 * The inner or outer default must be set (size greater than twice <tt>SPAresabs</tt>), and valid
 * (greater than minus half the body box max side), and the inner default offset must be less
 * than the outer offset.
 * <br><br>
 * Specific offsets must be non zero (size greater than twice <tt>SPAresabs</tt>), and valid (greater
 * than minus half the body box max side), and the outer offset for any face (default or specific)
 * must be greater than the inner offset (default or specific), or error <tt>LOP_OFF_BAD_OFFSET</tt>.
 * <br><br>
 * Valid body transformation (no shear component), or error <tt>LOP_BAD_BODY_TRANSFORM</tt>.
 * <br><br>
 * Body must have at least one lump with only one shell, or error <tt>LOP_HOL_MULTI_SHELL</tt>.
 * <br><br>
 * Some faces must remain unopen, or error <tt>LOP_HOL_ALL_OPEN</tt>.
 * <br><br>
 * Open faces must not have a specific offset, or error <tt>LOP_TWK_BAD_FACE</tt>.
 * <br><br>
 * <b>Limitations:</b> Refer to the Limitations listed for the function, @href api_tweak_faces.
 * <br><br>
 * <b>Effect:</b> Changes model
 * <br><br>
 * <b>Journal: </b> Available
 * <br><br>
 * <b>Product(s):</b> 3D ACIS Modeler
 * <br><br>
 * @param body
 * body to be shelled.
 * @param nopenface
 * number of faces being omitted.
 * @param openface
 * faces being omitted.
 * @param inner_default_offset
 * default inner offset (usually -).
 * @param outer_default_offset
 * default outer offset (usually +).
 * @param n_spec_inner
 * number of specific inner offsets.
 * @param spec_inner_face
 * faces with specific inner offsets.
 * @param spec_inner_off
 * specific inner offsets.
 * @param n_spec_outer
 * number of specific outer offsets.
 * @param spec_outer_face
 * faces with specific outer offsets.
 * @param spec_outer_off
 * specific outer offsets.
 * @param box_low
 * start of intersection box to be used.
 * @param box_high
 * end of intersection box to be used.
 * @param pLopts
 * local operations options. NULL argument will set the default values.
 * @param ao
 * ACIS options.
 **/
   DECL_SHL outcome api_hollow_body_specific(
												BODY*        body,
												int const    nopenface,
												FACE*        openface[],
												double       inner_default_offset,
												double       outer_default_offset,
												int const    n_spec_inner,
												FACE*        spec_inner_face[],
												double       spec_inner_off[],
												int const    n_spec_outer,
												FACE*        spec_outer_face[],
												double       spec_outer_off[],
												const SPAposition&  box_low,
												const SPAposition&  box_high,
												lop_options* pLopts,
												AcisOptions* ao = NULL
				 );
/**
 * @nodoc
 * Creates a thin walled solid shell from a solid body, with default and specific inner and outer offsets.
 * <br><br>
 * <b>Role:</b> Shells the supplied body by the offset distances, omitting the specified open faces,
 * thus generating openings into the internal void. The default offsets are applied to all faces
 * except the open faces, unless overridden by specific offsets.
 * <br><br>
 * Body faces with radial surfaces, which cannot be offset by the thickness, are removed and the
 * resulting wound healed by the surrounding face surfaces. Radial surfaces are spheres, cones,
 * tori, blended edges and blended vertices.
 * <br><br>
 * This generally leads to shells which are locally overly thick. However in the case of vertex blends
 * with large setbacks being removed, the shell may be locally too thin, or may even have a hole.
 * <br><br>
 * The box low and high arguments are used to limit any geometrical intersections and so, improve
 * performance. Limiting the intersection improves performance. The box must contain the final faces
 * and if omitted defaults to the size box.
 * <br><br>
 * Multi-lump bodies may be hollowed, as long as at least one lump has only one shell. Lumps with
 * more than one shell are not hollowed.
 * <br><br>
 * Mergeable edges will be retained if they have a <tt>NO_MERGE_ATTRIB</tt> attached.
 * <br><br>
 * <b>Topology Changes:</b>
 * <br><br>
 * Refer to the topology changes listed for the function, @href api_tweak_faces.
 * <br><br>
 * <b>Geometry Changes:</b>
 * <br><br>
 * Refer to the geometry changes listed for the function, @href api_tweak_faces.
 * <br><br>
 * <b>Errors:</b> In addition to the following, refer to the Errors listed for the function, @href api_tweak_faces.
 * <br><br>
 * The inner or outer default must be set (size greater than twice <tt>SPAresabs</tt>), and valid
 * (greater than minus half the body box max side), and the inner default offset must be less
 * than the outer offset.
 * <br><br>
 * Specific offsets must be non zero (size greater than twice <tt>SPAresabs</tt>), and valid (greater
 * than minus half the body box max side), and the outer offset for any face (default or specific)
 * must be greater than the inner offset (default or specific), or error <tt>LOP_OFF_BAD_OFFSET</tt>.
 * <br><br>
 * Valid body transformation (no shear component), or error <tt>LOP_BAD_BODY_TRANSFORM</tt>.
 * <br><br>
 * Body must have at least one lump with only one shell, or error <tt>LOP_HOL_MULTI_SHELL</tt>.
 * <br><br>
 * Some faces must remain unopen, or error <tt>LOP_HOL_ALL_OPEN</tt>.
 * <br><br>
 * Open faces must not have a specific offset, or error <tt>LOP_TWK_BAD_FACE</tt>.
 * <br><br>
 * <b>Limitations:</b> Refer to the Limitations listed for the function, @href api_tweak_faces.
 * <br><br>
 * <b>Effect:</b> Changes model
 * <br><br>
 * <b>Journal: </b> Not Available
 * <br><br>
 * <b>Product(s):</b> 3D ACIS Modeler
 * <br><br>
 * @param body
 * body to be shelled.
 * @param nopenface
 * number of faces being omitted.
 * @param openface
 * faces being omitted.
 * @param inner_default_offset
 * default inner offset (usually -).
 * @param outer_default_offset
 * default outer offset (usually +).
 * @param n_spec_inner
 * number of specific inner offsets.
 * @param spec_inner_face
 * faces with specific inner offsets.
 * @param spec_inner_off
 * specific inner offsets.
 * @param n_spec_outer
 * number of specific outer offsets.
 * @param spec_outer_face
 * faces with specific outer offsets.
 * @param spec_outer_off
 * specific outer offsets.
 * @param box_low
 * start of intersection box to be used.
 * @param box_high
 * end of intersection box to be used.
 * @param ao
 * ACIS options.
 **/
 DECL_SHL outcome api_hollow_body_specific(
											BODY*        body,
											int const    nopenface,
											FACE*        openface[],
											double       inner_default_offset,
											double       outer_default_offset,
											int const    n_spec_inner,
											FACE*        spec_inner_face[],
											double       spec_inner_off[],
											int const    n_spec_outer,
											FACE*        spec_outer_face[],
											double       spec_outer_off[],
											SPAposition  box_low,
											SPAposition  box_high,
											AcisOptions* ao = NULL
				 );

/**
 * Creates a solid body from a sheet.
 * <b>Technical Article:</b> <i>[Sheet Thicken](http://doc.spatial.com/articles/s/h/e/Shelling_Operations_5d46.html#Sheet_Thicken)</i>
 * <br><br>
 * <b>Role:</b> The purpose of sheet thickening is to make a realistic three-dimensional sheet from
 * an idealized two-dimensional one; therefore, the offset values supplied is usually very small.
 * <br><br>
 * This API makes a copy of the sheet to be thickened, offsets the two sheets, and fits new orthogonal 
 * side faces between the two copies. The sign of the offset determines the direction of thickening.
 * The positive offset leads to thickening in the direction of the sheet normal, whereas
 * negative offset leads to thickening in the direction opposite to the sheet normal ,and the thickness 
 * of the output body is abs(offset1) + abs(offset2).
 * If both offset values are positive, thickened body is displaced in the direction of the sheet normal,
 * whereas for both negative offsets, thickened body is displaced in the reverse direction of the sheet 
 * normal and for these scenarios, the thickness of the output body is abs(offset1 - offset2).
 * <br><br>
 * The optional intersection box limits the size of intersections between surfaces which might
 * otherwise be very long. It cannot be used to choose faces. Its main purpose is to speed up
 * complicated cases where the intersection curves might be very long, thus improving performance.
 * The box must contain the final faces and if omitted defaults to the size box. 
 * SPAposition(0,0,0) can be used as default value for both box-low
 * and box-high when user is unable to specify a valid box.
 * <br><br>
 * Refer <tt>Failsafe Offsetting</tt> section for information on obtaining a failsafe solution.
 * <br><br>
 * <b>Topology Changes:</b>
 * <br><br>
 * Refer to the topology changes listed for the function, @href api_tweak_faces.
 * <br><br>
 * Vertices may be split as a result of thickening the sheet.
 * <br><br>
 * Mergeable edges will be removed unless they have a <tt>NO_MERGE_ATTRIB</tt>.
 * <br><br>
 * <b>Geometry Changes:</b>
 * <br><br>
 * Refer to the geometry changes listed for the function, @href api_tweak_faces.
 * <br><br>
 * <b>Errors:</b> Refer to the errors listed for the function, @href api_tweak_faces.
 * <br><br>
 * <b>Limitations:</b> The sheet must be manifold. In particular that means:
 * <br><br>
 * <ul>
 * <li>There cannot be more than two sheet edges meeting at any vertex and the sheet must consist of
 * no more than one shell.</li>
 * <li>If the sheet is made up of several faces, the normals of adjacent faces must be consistent.</li>
 * <li>Laminas cannot be thickened.</li>
 * <li>Sheet body with more than one lump or more than one shell cannot be thickened. </li>
 * </ul>
 * <br>
 * Only single-sided sheets can be thickened. If a double-sided sheet is encountered it is made
 * single-sided.
 * <br><br>
 * <b>Effect:</b> Changes model
 * <br><br>
 * <b>Journal: </b> Available
 * <br><br>
 * <b>Product(s):</b> 3D ACIS Modeler
 * <br><br>
 * @param body
 * sheet to be thickened.
 * @param offset0
 * first distance to offset.
 * @param offset1
 * second distance to offset.
 * @param box_low
 * start of intersection box to be used.
 * @param box_high
 * end of intersection box to be used.
 * @param pLopts
 * local operations options. NULL argument will set the default values.
 * @param approx_ok
 * logical approximation.
 * @param ao
 * ACIS options.
 **/

 DECL_SHL outcome api_sheet_thicken(
                            BODY*        body,
                            double       offset0,
                            double       offset1,
                            const SPAposition&  box_low,
                            const SPAposition&  box_high,
                            lop_options* pLopts,
                            logical      approx_ok = TRUE,
                            AcisOptions* ao = NULL
                            );

/**
 * Creates a solid body from a sheet.
 * <b>Technical Article:</b> <i>[Sheet Thicken](http://doc.spatial.com/articles/s/h/e/Shelling_Operations_5d46.html#Sheet_Thicken)</i>
 * <br><br>
 * <b>Role:</b> The purpose of sheet thickening is to make a realistic three-dimensional sheet from
 * an idealized two-dimensional one; therefore, the thickness supplied is usually very small.
 * <br><br>
 * This API makes a copy of the sheet to be thickened, offsets it, and fits new orthogonal side faces
 * between the two copies. If <tt>both</tt> is <tt>FALSE</tt> and the thickness supplied is
 * positive, then the sheet is thickened in the direction of the normal of the sheet. Supplying a
 * negative thickness thickens it in the opposite direction. If <tt>both</tt> is <tt>TRUE</tt>, then
 * the sheet is thickened in each direction by half the thickness.
 * <br><br>
 * The optional intersection box limits the size of intersections between surfaces which might
 * otherwise be very long. It cannot be used to choose faces. Its main purpose is to speed up
 * complicated cases where the intersection curves might be very long, thus improving performance.
 * The box must contain the final faces and if omitted defaults to the size box.
 * SPAposition(0,0,0) can be used as default value for both box-low
 * and box-high when user is unable to specify a valid box.
 * <br><br>
 * Refer <tt>Failsafe Offsetting</tt> section for information on obtaining a failsafe solution.
 * <br><br>
 * <b>Topology Changes:</b>
 * <br><br>
 * Refer to the topology changes listed for the function, @href api_tweak_faces.
 * <br><br>
 * Vertices may be split as a result of thickening the sheet.
 * <br><br>
 * Mergeable edges will be removed unless they have a <tt>NO_MERGE_ATTRIB</tt>.
 * <br><br>
 * <b>Geometry Changes:</b>
 * <br><br>
 * Refer to the geometry changes listed for the function, @href api_tweak_faces.
 * <br><br>
 * <b>Errors:</b> Refer to the errors listed for the function, @href api_tweak_faces.
 * <br><br>
 * <b>Limitations:</b> The sheet must be manifold. In particular that means:
 * <br><br>
 * <ul>
 * <li>There cannot be more than two sheet edges meeting at any vertex and the sheet must consist of
 * no more than one shell.</li>
 * <li>If the sheet is made up of several faces, the normals of adjacent faces must be consistent.</li>
 * <li>Laminas cannot be thickened.</li>
 * <li>Sheet body with more than one lump or more than one shell cannot be thickened. </li>
 * </ul>
 * <br>
 * Only single-sided sheets can be thickened. If a double-sided sheet is encountered it is made
 * single-sided.
 * <br><br>
 * <b>Effect:</b> Changes model
 * <br><br>
 * <b>Journal: </b> Available
 * <br><br>
 * <b>Product(s):</b> 3D ACIS Modeler, 3D ACIS Polyhedral
 * <br><br>
 * @param body
 * sheet to be thickened.
 * @param thickness
 * distance to offset.
 * @param both
 * <tt>TRUE</tt> if thickening in both direction(s).
 * @param box_low
 * start of intersection box to be used.
 * @param box_high
 * end of intersection box to be used.
 * @param pLopts
 * local operations options. NULL argument will set the default values.
 * @param approx_ok
 * logical approximation.
 * @param ao
 * ACIS options.
 **/
  DECL_SHL outcome api_sheet_thicken(
						   BODY*        body,
						   double       thickness,
						   logical      both,
						   const SPAposition&  box_low,
						   const SPAposition&  box_high,
						   lop_options* pLopts,
						   logical      approx_ok = TRUE,
						   AcisOptions* ao = NULL
						   );
/**
 * @nodoc
 * Creates a solid body from a sheet.
 * <br><br>
 * <b>Role:</b> The purpose of sheet thickening is to make a realistic three-dimensional sheet from
 * an ideal two-dimensional one. So usually the thickness supplied will be very small.
 * <br><br>
 * This API makes a copy of the sheet to be thickened, offsets it, and fits new orthogonal side faces
 * between the two copies. If the keyword <tt>both</tt> is not supplied and the thickness supplied is
 * positive, then the sheet is thickened in the direction of the normal of the sheet. Supplying a
 * negative thickness thickens it in the opposite direction. If the keyword <tt>both</tt> is supplied, then
 * the sheet is thickened in each direction by half the thickness.
 * <br><br>
 * The optional intersection box limits the size of intersections between surfaces which might
 * otherwise be very long. It can not be used to choose faces. Its main purpose is to speed up
 * complicated cases where the intersection curves might be very long, thus improving performance.
 * The box must contain the final faces and if omitted defaults to the size box.
 * <br><br>
 * <b>Topology Changes:</b>
 * <br><br>
 * Refer to the topology changes listed for the function, @href api_tweak_faces.
 * <br><br>
 * Vertices may be split as a result of thickening the sheet.
 * <br><br>
 * Mergeable edges will be removed unless they have a <tt>NO_MERGE_ATTRIB</tt>.
 * <br><br>
 * <b>Geometry Changes:</b>
 * <br><br>
 * Refer to the geometry changes listed for the function, @href api_tweak_faces.
 * <br><br>
 * <b>Errors:</b> In addition to the following, refer to the errors listed for the function, @href api_tweak_faces.
 * <br><br>
 * The two offset values must be different, or error LOP_OFF_BAD_OFFSET
 * <br><br>
 * <b>Limitations:</b> The sheet must be manifold. In particular that means:
 * <br><br>
 * <ul>
 * <li>There cannot be more than two sheet edges meeting at any vertex and the sheet must consist of
 * no more than one shell.</li>
 * <li>If the sheet is made up of several faces, the normals of adjacent faces must be consistent.</li>
 * <li>Laminas cannot be thickened.</li>
 * <li>Sheet body with more than one lump or more than one shell cannot be thickened. </li>
 * </ul>
 * <br>
 * Only single-sided sheets can be thickened. If a double-sided sheet is encountered it is made
 * single sided.
 * <br><br>
 * <b>Effect:</b> Changes model
 * <br><br>
 * <b>Journal: </b> Not Available
 * <br><br>
 * <b>Product(s):</b> 3D ACIS Modeler, 3D ACIS Polyhedral
 * <br><br>
 * @param body
 * sheet to be thickened.
 * @param thickness
 * distance to offset.
 * @param both
 * true if thickening in both direction(s).
 * @param box_low
 * start of intersection box to be used.
 * @param box_high
 * end of intersection box to be used.
 * @param approx_ok
 * logical approximation.
 * @param ao
 * ACIS options.
 **/
 DECL_SHL outcome api_sheet_thicken(
						   BODY*        body,
						   double       thickness,
						   logical      both,
						   SPAposition  box_low,
						   SPAposition  box_high,
						   logical      approx_ok = TRUE,
						   AcisOptions* ao        = NULL
						   );
/**
 * Offsets an array of faces and makes a new sheet out of the offset 
 * faces while the original body is untouched.
 * <br><br>
 * <b>Role:</b> Offsets surfaces of the supplied faces by an offset distance specific for each face.
 * Creates a new sheet body out of those faces. The sheet body is converted to two-dimensional sheet.
 * <br><br>
 * Radial faces with surfaces which cannot be so offset are removed and the resulting wound healed
 * by the surrounding face surfaces. Optionally repair body can be used during the offset.
 * <br><br>
 * <b>Topology Changes:</b>
 * <br><br>
 * Refer to the topology changes listed for the Scheme extension, <tt>lop:offset-specific</tt>.
 * <br><br>
 * <b>Geometry Changes:</b>
 * <br><br>
 * Refer to the geometry changes listed for the Scheme extension, <tt>lop:offset-specific</tt>.
 * <br><br>
 * <b>Errors:</b> Refer to the errors listed for @href api_offset_faces_specific
 * <br><br>
 * <b>Limitations:</b> Refer to the Limitations listed for @href api_offset_faces_specific
 * <br><br>
 * <b>Effect:</b> Changes model
 * <br><br>
 * <b>Journal: </b> Available
 * <br><br>
 * <b>Product(s):</b> 3D ACIS Modeler
 * <br><br>
 * @param n_def_face
 * number of faces offset.
 * @param def_face
 * faces being offset.
 * @param def_offset
 * default distance to offset.
 * @param n_spec_face
 * number of faces with specific offsets.
 * @param spec_face
 * faces with specific offsets.
 * @param spec_offset
 * specific offset values.
 * @param sheet_body
 * resultant output sheet body.
 * @param box_low
 * start of intersection box to be used.
 * @param box_high
 * end of intersection box to be used.
 * @param pLopts
 * local operations options. NULL argument will set the default values.
 * @param ao
 * acis options.
 **/
 DECL_SHL outcome api_offset_faces_make_sheet(
							int const&   n_def_face,
							FACE*        def_face[],
							double       def_offset,
							int const&   n_spec_face,
							FACE*        spec_face[],
							double       spec_offset[],
							BODY*&       sheet_body,
							const SPAposition&  box_low,
							const SPAposition&  box_high,
							lop_options* pLopts,
							AcisOptions* ao = NULL
							);
/**
 * @nodoc
 * Offsets an array of faces, (each face as required) and makes a new sheet out of the offset 
 * faces while the original body is untouched.
 * <br><br>
 * <b>Role:</b> Offsets surfaces of the supplied faces by an offset distance specific for each face.
 * Creates a new sheet body out of those faces. The sheet body is converted to two-dimensional sheet.
 * <br><br>
 * Radial faces with surfaces which cannot be so offset are removed and the resulting wound healed
 * by the surrounding face surfaces. Optionally repair bodycan be used during the offset.
 * <br><br>
 * <b>Topology Changes:</b>
 * <br><br>
 * Refer to the topology changes listed for the Scheme extension, <tt>lop:offset-specific</tt>.
 * <br><br>
 * <b>Geometry Changes:</b>
 * <br><br>
 * Refer to the geometry changes listed for the Scheme extension, <tt>lop:offset-specific</tt>.
 * <br><br>
 * <b>Errors:</b> Refer to the errors listed for @href api_offset_faces_specific
 * <br><br>
 * <b>Limitations:</b> Refer to the Limitations listed for @href api_offset_faces_specific
 * <br><br>
 * <b>Effect:</b> Changes model
 * <br><br>
 * <b>Journal: </b> Not Available
 * <br><br>
 * <b>Product(s):</b> 3D ACIS Modeler
 * <br><br>
 * @param n_def_face
 * number of faces offset.
 * @param def_face
 * faces being offset.
 * @param def_offset
 * default distance to offset.
 * @param n_spec_face
 * number of faces with specific offsets.
 * @param spec_face
 * faces with specific offsets.
 * @param spec_offset
 * specific offset values.
 * @param sheet_body
 * resultant output sheet body.
 * @param box_low
 * start of intersection box to be used.
 * @param box_high
 * end of intersection box to be used.
 * @param ao
 * acis options.
 **/
 DECL_SHL outcome api_offset_faces_make_sheet(
									int const&   n_def_face,
									FACE*        def_face[],
									double       def_offset,
									int const&   n_spec_face,
									FACE*        spec_face[],
									double       spec_offset[],
									BODY*&       sheet_body,
									SPAposition  box_low,
									SPAposition  box_high,
									AcisOptions* ao = NULL
									);
/**
 * Specifies the option value of how to grow the open faces.
 * <br>
 * <b>Role:</b> Used by <tt>api_make_solid_from_faces</tt> to determine growth type.
 * @param from_parent
 * Grow faces to repair hole from parent body (Default).
 * @param from_child
 * Grow faces to repair hole from child body.
 * @param from_cover
 * Use cover to repair hole.
 **/
enum growth_type 
{
	from_parent,
	from_child,
	from_cover
};
/**
 * Creates solid bodies from a list of faces by growing faces to cover openings.
 * <br><br>
 * <b>Role:</b> The purpose of this function is to create a solid from each manifold 
 * connected set of faces in the input face list.
 * The first step of the function is to copy the faces and separate them into unique 
 * manifold, connected face sets. Each connected set of faces
 * is then repaired by growing faces to cover openings, and solids are created from these faces.
 * These solids are then returned by the function, along with
 * a flag for each solid indicating if the solid is filling a hole (<tt>holes[i] == TRUE</tt>)
 * or replacing a boss (<tt>holes[i] == FALSE</tt>).
 * This function can be used as the base functionality to "copy and paste" a portion of a solid, 
 * for solid decomposition, or for the local modification of
 * a portion of a solid body.
 * <br><br>
 * The <tt>face_list</tt> argument identifies the faces of a body from which to create the new solids.
 * <br><br>
 * The argument <tt>growth_type</tt> specifies how to grow the faces to close the openings. 
 * Valid values are:
 * <br>
 * <tt>from_parent</tt> - grow faces to repair holes from parent body. (default)<br>
 * <tt>from_child</tt> - grow faces to repair holes from child body.<br>
 * <tt>from_cover</tt> - use cover to repair holes.
 * <br><br>
 * The <tt>remove_faces</tt> flag controls whether to change the orginal body by removing the faces in
 * <tt>face_list</tt> from the body.
 * <br><br>
 * <b>Limitations:</b> Faces in <tt>face_list</tt> must be in manifold, connected face sets that can 
 * be removed from the body by <tt>api_remove_faces</tt>.
 * <br><br>
 * <b>Effect:</b> Changes model
 * <br><br>
 * <b>Journal: </b> Available
 * <br><br>
 * <b>Product(s):</b> 3D ACIS Modeler
 * <br><br>
 * @param face_list
 * List of faces from which to make a solid.
 * @param gtype
 * Option to indicate the type of growth desired.
 * @param remove_faces
 * Option to remove faces from the original body.
 * @param count
 * Number of solids returned.
 * @param holes
 * Type of solids returned.
 * @param solids
 * Solids made from the list of faces.
 * @param pLopts
 * Local Operations options. Default values are used when this argument is NULL.
 * @param ao
 * ACIS options.
 **/
DECL_SHL outcome api_make_solid_from_faces(
									ENTITY_LIST  face_list,
									growth_type  gtype,
									logical      remove_faces,
									int&         count,
									logical*&    holes,
									BODY**&      solids,
									lop_options* pLopts,
									AcisOptions* ao = NULL
									);
/**
 * @nodoc
 * Creates solid bodies from a list of faces by growing faces to cover openings.
 * <br><br>
 * <b>Role:</b> The purpose of this function is to create a solid from each manifold 
 * connected set of faces in the input face list.
 * The first step of the function is to copy the faces and separate them into unique 
 * manifold, connected face sets. Each connected set of faces
 * is then repaired by growing faces to cover openings, and solids are created from these faces.
 * These solids are then returned by the function, along with
 * a flag for each solid indicating if the solid is filling a hole (<tt>holes[i] == TRUE</tt>)
 * or replacing a boss (<tt>holes[i] == FALSE</tt>).
 * This function can be used as the base functionality to "copy and paste" a portion of a solid, 
 * for solid decomposition, or for the local modification of
 * a portion of a solid body.
 * <br><br>
 * The <tt>face_list</tt> argument identifies the faces of a body from which to create the new solids.
 * <br><br>
 * The argument <tt>growth_type</tt> specifies how to grow the faces to close the openings. 
 * Valid values are:
 * <br>
 * from_parent - grow faces to repair holes from parent body. (default)<br>
 * from_child - grow faces to repair holes from child body.<br>
 * from_cover - use cover to repair holes.
 * <br><br>
 * The <tt>remove_faces</tt> flag controls whether to change the orginal body by removing the faces in
 * <tt>face_list</tt> from the body.
 * <br><br>
 * <b>Limitations:</b> Faces in <tt>face_list</tt> must be in manifold, connected face sets that can 
 * be removed from the body by <tt>api_remove_faces</tt>.
 * <br><br>
 * <b>Effect:</b> Changes model
 * <br><br>
 * <b>Journal: </b> Not Available
 * <br><br>
 * <b>Product(s):</b> 3D ACIS Modeler
 * <br><br>
 * @param face_list
 * List of faces from which to make a solid.
 * @param gtype
 * Option to indicate the type of growth desired.
 * @param remove_faces
 * Option to remove faces from the original body.
 * @param count
 * Number of solids returned.
 * @param holes
 * Type of solids returned.
 * @param solids
 * Solids made from the list of faces.
 * @param ao
 * ACIS options.
 **/
DECL_SHL outcome api_make_solid_from_faces(
										ENTITY_LIST  face_list,
										growth_type  gtype,
										logical      remove_faces,
										int&         count,
										logical*&    holes,
										BODY**&      solids,
										AcisOptions* ao = NULL);

/**
 * Creates a wire body that can be used to create miter faces after a sheet-thicken operation.
 * <br><br>
 * <b>Role:</b> The purpose of this function is to create a wire body of edges
 * on the offset side of a thickened body given a list of edges
 * on the original side of the thickened body.
 * The input edges should be a list of edges on the thickened body that
 * lie in the original sheet (the one the body was created from using sheet-thicken).
 * These edges also have to be connected, non-branching and open (i.e. not a closed loop).
 * The output is a wire body of edges that correspond to the input edges
 * and lie on the offset faces of the thickened body. The correspondence
 * between the input and output edges is not necessarily one-to-one, e.g.
 * some extra edges that were created during vertex-split might be added to the output wire.
 * The output wire together with the input edges can be used to create the miter faces
 * with the help of skin-wire. Before the call to skin-wire, the input edges must be converted
 * to a wire-body, too.
 * <br>
 * This function uses annotations, therefore it only works if "annotation" was turned on and
 * "unhook_annotations" was turned off during sheet-thicken.
 * <br>
 * The function only works if sheet-thicken was one-directional.
 * <br><br>
 * <b>Limitations:</b> The sheet-thicken operation that was used to create the thickened body
 * must have been a thickening in one direction only, i.e. the original sheet must be a sheet
 * of the thickened body. The input edges must be connected, non-branching and open.
 * <br><br>
 * <b>Effect:</b> Changes model
 * <br><br>
 * <b>Journal: </b> Available
 * <br><br>
 * <b>Product(s):</b> 3D ACIS Modeler
 * <br><br>
 * @param input_edges
 * List of input edges on the original side of the thickened body
 * @param outp_wire
 * The new wire-body containing edges on the offset side of the thickened body.
 * @param ao
 * ACIS options.
 **/

DECL_SHL
outcome api_create_wire_for_miter_faces(
									ENTITY_LIST &input_edges,
									BODY *&outp_wire,
									AcisOptions *ao = NULL);


/**
 * Specifies the structure for storing left and right face of face-pair.
 * <br>
 * <b>Role:</b> Used by <tt>api_make_mid_sheet_body</tt> for input argument
 * @param _left_face
 * Left face from face-pair.
 * @param _right_face
 * Right face from face-pair.
 **/

struct DECL_SHL mid_sheet_face_pair
{
	FACE* _left_face = nullptr;
	FACE* _right_face = nullptr;
};

/**
 * This API create the mid sheet @href BODY
 * <br><br>
 * <b>Technical Articles:</b> <i>[Mid Sheet Body](https://doc.spatial.com/articles/m/i/d/Mid_Sheet_Generation_90eb.html)</i>
 * <br><br>
 * <b>Role:</b> The mid-surface sheet body is created between suitable selected face-pairs on solid bodies.
 * It finds the minimum distance between the left and right faces and consider this to be the thickness between the faces.
 * It will then offset the 'left_faces' relative to this thickness.
 * This is opposite of sheet thicken. 
 * The mid-surface sheets are generally used to perform finite element analysis of thin-walled parts.
 * <br><br>
 * <b>Errors:</b> Refer to the errors listed for @href api_offset_faces_specific
 * <br><br>
 * <b>Limitations:</b>
 * <ul>
 * <li>Faces in mid_sheet_face_pair must be in manifold. </li>
 * <li>All left faces in pair should be connected. Multi-lump body is created for dis-connected left faces </li>
 * </ul>
 * <b>Journal:</b> Available
 * <br><br>
 * <b>Product(s):</b> 3D ACIS Modeler
 * <br><br>
 * @param num_pairs
 * Number of face-pair.
 * @param mid_sheet_face_pairs
 * face-pair array having left and right faces.
 * @param mid_sheet_body
 * mid-sheet body after the operation. Use @href mid_sheet_body_options to control the behavior.
 * @param ao
 * ACIS options for versioning and journaling
 */
DECL_SHL
outcome api_make_mid_sheet_body(int num_pairs,
	mid_sheet_face_pair mid_sheet_face_pairs[],
	BODY*& mid_sheet_body,
	mid_sheet_body_options* mid_sheet_body_opts = nullptr,
	AcisOptions* ao = nullptr);

/** @} */
#endif
