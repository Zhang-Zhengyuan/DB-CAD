/*******************************************************************/
/*    Copyright (c) 1989-2020 by Spatial Corp.                     */
/*    All rights reserved.                                         */
/*    Protected by U.S. Patents 5,257,205; 5,351,196; 6,369,815;   */
/*                              5,982,378; 6,462,738; 6,941,251    */
/*    Protected by European Patents 0503642; 69220263.3            */
/*    Protected by Hong Kong Patent 1008101A                       */
/*******************************************************************/
//                  Header for offset api routines.
/*******************************************************************/
#if !defined( OFSTAPI_HXX )
#define OFSTAPI_HXX
#include "base.hxx"
class wire_offset_options;
class BODY;
class WIRE;
class TRANSFORM;
class FACE;
class EDGE;
class LOOP;
class COEDGE;
class SPAunit_vector;
class SPAvector;
class law;
class SPAposition;
class offset_face_edge_opts;
class offset_edge_on_face_opts;
class offset_edges_options;

#include "logical.h"
#include "dcl_ofst.h"
#include "api.hxx"
#include "offset_opts.hxx"
/**
 * @file ofstapi.hxx
 * @CAA2Level L1
 * @CAA2Usage U1
 * \defgroup OFSTAPI Offset Face and Wire
 *
 * \ingroup LOCALOPS
 * @{
 */
/**
 * Initializes the offsetting library.
 * <br><br>
 * <b>Effect:</b> System routine
 * <br><br>
 * <b>Journal:</b> Not Available
 * <br><br>
 * <b>Product(s):</b> 3D ACIS Modeler
 * <br><br>
**/
DECL_OFST outcome api_initialize_offsetting();

/**
 * Terminates the offsetting library.
 * <br><br>
 * <b>Effect:</b> System routine
 * <br><br>
 * <b>Journal:</b> Not Available
 * <br><br>
 * <b>Product(s):</b> 3D ACIS Modeler
 * <br><br>
**/
DECL_OFST outcome api_terminate_offsetting();
/**
 * @nodoc
 * Creates a face that is an offset of the given face.
 * <br><br>
 * <b>Role:</b> This API creates a face whose surface is offset from the given face.
 * The edges of the offset face are offset from the given face.
 * <br><br>
 * It is assumed that the user will not do offsets that result in degenerate offsets.
 * Offset distances larger than the smallest radius of curvature (principal)
 * will probably cause degenerate cases.
 * <br><br>
 * Because a face cannot have a transform, the programmer must apply the
 * transform from the body to the face after the offset, if needed, or fix the
 * transform on the body before offsetting the face.
 * <br><br>
 * <b>Errors:</b> Pointer to face is <tt>NULL</tt> or not to a <tt>FACE</tt>.
 * <br><br>
 * <b>Effect:</b> Changes model
 * <br><br>
 * <b>Journal:</b> Available
 * <br><br>
 * <b>Product(s):</b> 3D ACIS Modeler
 * <br><br>
 * @param given_face
 * given face.
 * @param offset_distance
 * distance to offset new face.
 * @param offset_face
 * returns resulting face.
 * @param ao
 * ACIS options such as versioning and journaling.
 **/
DECL_OFST outcome api_offset_face(
			                     FACE*           given_face,
			                     double          offset_distance,
			                     FACE*&          offset_face,
			                     AcisOptions*    ao);

/**
 * Creates a face that is an offset of the given face.
 * <br><br>
 * <b>Role:</b> This API creates a face whose surface is offset from the given face.
 * The edges of the offset face are offset from the given face.
 * <br><br>
 * It is assumed that the user will not do offsets that result in degenerate offsets.
 * Offset distances larger than the smallest radius of curvature (principal)
 * will probably cause degenerate cases.
 * <br><br>
 * Because a face cannot have a transform, the programmer must apply the
 * transform from the body to the face after the offset, if needed, or fix the
 * transform on the body before offsetting the face.
 * <br><br>
 * <b>Errors:</b> Pointer to face is <tt>NULL</tt> or not to a <tt>FACE</tt>.
 * <br><br>
 * <b>Effect:</b> Changes model
 * <br><br>
 * <b>Journal:</b> Available
 * <br><br>
 * <b>Product(s):</b> 3D ACIS Modeler
 * <br><br>
 * @param given_face
 * given face.
 * @param offset_distance
 * distance to offset new face.
 * @param offset_face
 * returns resulting face.
 * @param pOffOpts
 * offset options
 * @param ao
 * ACIS options such as versioning and journaling.
 **/
DECL_OFST outcome api_offset_face(
			                     FACE*           given_face,
			                     double          offset_distance,
			                     FACE*&          offset_face,
                                 offset_options* pOffOpts= NULL,
			                     AcisOptions*    ao = NULL
                                  );
/**
 * Creates an EDGE by offseting <tt>given_coedge</tt> from the face it borders.
 * <br><br>
 * <b>Role:</b> 
 * This API creates an edge which is offset from <tt>given_coedge</tt>,
 * in the direction normal to the face to which <tt>given_coedge</tt> is attached.
 * <br><br>
 * The parameter interval and sense of the output edge match that of
 * <tt>given_coedge->edge()</tt>.
 * <br><br>
 * Any transformations which acted on the owner of <tt>given_coedge</tt> are
 * used to transform <tt>resulting_offset_edge</tt>.
 * <br><br>
 * <b>Effect:</b> Changes model
 * <br><br>
 * <b>Journal:</b> Available
 * <br><br>
 * <b>Product(s):</b> 3D ACIS Modeler
 * <br><br>
 * @param given_coedge
 * the given coedge.
 * @param offset_distance
 * the distance to offset the new edge.
 * @param resulting_offset_edge
 * the pointer to the result of the offset operation.
 * @param offset_face_edge_opts
 * This parameter is reserved for future use; defaults to NULL.
 * @param ao
 * ACIS options such as versioning and journaling.
 **/
DECL_OFST outcome api_offset_face_edge(
									   COEDGE	const			*given_coedge,
									   double					offset_distance,
									   EDGE						*&resulting_offset_edge,
									   offset_face_edge_opts	*opts=NULL,
							           AcisOptions				*ao=NULL
									   );

/**
 * Creates a wire body that is an offset of the wires in the given body.
 * <br><br>
 * <b>Role:</b> Each wire in the given body is offset so that no portion of the offset wire is closer
 * to the original wire than the offset distance. Trimming the offset overlap caused
 * by C<sup>0</sup> edges, filling in the gaps caused by the offsets of C<sup>0</sup> edges, and trimming
 * areas of interference between offset segments are operations that get carried out.
 * <br><br>
 * This API function has four overloaded signatures: the one using @href wire_offset_options 
 * is the newest and includes nearly all the functionality of the other three. Therefore, the others 
 * are becoming obsolete.
 * <br><br>
 * A positive offset distance is in the direction of the cross product
 * (<tt>wire_tangent x wire_plane_normal</tt>). A negative offset is in the opposite direction.
 * <br><br>
 * The function expects a body containing at least one independent wire (not
 * connected to any faces). If the body contains multiple independent wires, each
 * independent wire in the body will be offset and united with the offsets of
 * the others.  To offset a single wire of a multi-wire body, call the version of
 * @href api_offset_planar_wire that takes a @href WIRE instead of a @href BODY.
 * <br><br>
 * <b>Errors:</b>
 * <ul>
 * <li>Pointer to wire body is <tt>NULL</tt> or not to a wire body.</li>
 * <li>Zero length normal vector specified.</li>
 * </ul>
 * <br><br>
 * <b>Limitations:</b> Performing a wire body offset with open wires can encounter behavior that is
 * potentially different from what is expected. Offsetting of closed wires does
 * not have the potential for this issue. When offsetting an open wire body, all
 * of the edges are offset first. 
 * <br><br>
 * <b>Effect:</b> Changes model
 * <br><br>
 * <b>Journal:</b> Available
 * <br><br>
 * <b>Product(s):</b> 3D ACIS Modeler
 * <br><br>
 * @param given_wire
 * planar wire body to be offset.
 * @param offset_distance
 * distance to offset.
 * @param wire_normal
 * resulting normal vector.
 * @param offset_wire
 * returns offset wire body.
 * @param ao
 * ACIS options such as versioning and journaling.
 **/
DECL_OFST outcome api_offset_planar_wire(
			                            BODY*        given_wire,
			                            double       offset_distance,
			                            const SPAunit_vector & wire_normal,
			                            BODY*&       offset_wire,
			                            AcisOptions* ao = NULL);


/**
 * Creates a wire body that is an offset of the wires in the given body.
 * <br><br>
 * <b>Role:</b> Each wire in the given body is offset so that no portion of the offset wire is closer
 * to the original wire than the offset distance. Trimming the offset overlap
 * caused by C<sup>0</sup> edges, filling in the gaps caused by the offsets of C<sup>0</sup> edges, and
 * trimming areas of interference between offset segments are operations that get
 * carried out.
 * <br><br>
 * This API function has four overloaded signatures: the one using @href wire_offset_options 
 * is the newest and includes nearly all the functionality of the other three. Therefore, the others 
 * are becoming obsolete.
 * <br><br>
 * An example of how to use this API can be found in the technical article and the 
 * source code for the <tt>wire-body:offset</tt> Scheme extension.
 * It also shows how to create the required laws and the meaning of the gap types.
 * <br><br>
 * The law version of @href api_offset_planar_wire can always be used and can
 * obtain the same results as the non-law versions by passing it simple laws.
 * A law can be created using @href api_str_to_law, which converts a law function
 * string into the appropriate law classes and returns a pointer to the top-level
 * class which can be passed into this API.
 * <br><br>
 * In the law version of @href api_offset_planar_wire, the wire is offset by
 * the given law and trimmed for self-intersections. If a non-zero twist law is
 * given, then the resulting wire will twist around the given wire and trimming
 * will not be done.
 * <br><br>
 * The offset direction is given by the cross product of the wires tangent and
 * the planar normal. If a twist law is given, then the twist starts in the offset
 * direction and rotates around the wire by the given radians of the twist law
 * using the right-hand rule. A negative offset twists in the opposite direction.
 * <br><br>
 * The function expects a body containing at least one independent wire (not
 * connected to any faces). If the body contains multiple independent wires, each
 * independent wire in the body will be offset and united with the offsets of
 * the others. To offset a single wire of a multi-wire body, call the version of
 * @href api_offset_planar_wire that takes a @href WIRE instead of a @href BODY.
 * <br><br>
 * The <tt>gap_type</tt> is as follows:<br>
 * 0 = rounded arcs<br>
 * 1 = linear extension<br>
 * 2 = natural curve extensions
 * <br><br>
 * If the <tt>trim</tt> argument is <tt>TRUE</tt>, the function trims the self-intersections. 
 * If it is <tt>FALSE</tt>, the function does not trim the intersections. Use <tt>FALSE</tt> 
 * only if you know that the result will not self-intersect.
 * <br><br>
 * <b>Errors:</b>
 * <ul>
 * <li>Pointer to wire body is <tt>NULL</tt> or not to a wire body.</li>
 * <li>Zero length normal vector specified.</li>
 * </ul>
 * <br><br>
 * <b>Limitations:</b>
 * <ul>
 * <li>Performing a wire body offset with open wires can encounter behavior that is
 * potentially different from what is expected. Offsetting of closed wires does
 * not have the potential for this issue. When offsetting an open wire body, all
 * of the edges are offset first.</li>
 * </ul>
 * <br><br>
 * <b>Effect:</b> Changes model
 * <br><br>
 * <b>Journal:</b> Available
 * <br><br>
 * <b>Product(s):</b> 3D ACIS Modeler
 * <br><br>
 * @param given_wire
 * planar wire body to be offset.
 * @param offset_law
 * distance to offset.
 * @param twist_law
 * twist in radians.
 * @param wire_normal
 * resulting normal vector.
 * @param offset_wire
 * returns offset wire body.
 * @param gap_type
 * gap type.
 * @param trim
 * trim flag.
 * @param overlap
 * overlap flag.
 * @param  ao
 * ACIS options such as versioning and journaling.
 **/
DECL_OFST outcome api_offset_planar_wire(
			                            BODY*        given_wire,
			                            law*         offset_law,
			                            law*         twist_law, 
			                            const SPAunit_vector &wire_normal,
			                            BODY*&       offset_wire,
			                            int          gap_type = 2,    
			                            logical      trim     = TRUE,	
			                            logical      overlap  = FALSE,
			                            AcisOptions* ao       = NULL);

/**
 * Creates a wire body that is an offset of the wires in the given body.
 * <br><br>
 * <b>Role:</b> Each wire in the given body is offset so that no portion of the offset wire is closer
 * to the original wire than the offset distance. Trimming the offset overlap
 * caused by C<sup>0</sup> edges, filling in the gaps caused by the offsets of C<sup>0</sup> edges, and
 * trimming areas of interference between offset segments are operations that get
 * carried out.
 * <br><br>
 * This API function has four overloaded signatures. This one, using @href wire_offset_options, 
 * is the newest and includes nearly all the functionality of the other three. Therefore, the others 
 * are becoming obsolete.
 * Refer to the documentation for the @href wire_offset_options for detailed information 
 * on the meaning of all the available options, or to the technical article for the 
 * <tt>wire-offset:options</tt> Scheme extension.
 * An example of how to use this API can be found in the source code for the
 * <tt>wire-body:offset</tt> Scheme extension.
 * It also shows how to create the required laws and the meaning of the gap types.
 * <br><br>
 * The function expects a body containing at least one independent wire (not
 * connected to any faces). If the body contains multiple independent wires, each
 * independent wire in the body will be offset and united with the offsets of
 * the others. To offset a single wire of a multi-wire body, call the version of
 * @href api_offset_planar_wire that takes a @href WIRE instead of a @href BODY.
 * <br><br>
 * <b>Known Case:</b> Performing a wire body offset with open wires can encounter behavior 
 * that is potentially different from what is expected. Offsetting of closed wires does
 * not have the potential for this issue. When offsetting an open wire body, all
 * of the edges are offset first.
 * <br><br>
 * <b>Errors:</b>
 * <ul>
 * <li>Pointer to wire body is <tt>NULL</tt> or not to a wire body.</li>
 * <li>Zero length normal vector specified.</li>
 * </ul>
 * <br><br>
 * <b>Limitations:</b> Performing a wire body offset with open wires can encounter
 * behavior that is potentially different from what is expected (refer to the
 * <i>Known Case</i> section above).
 * <br><br>
 * <b>Effect:</b> Changes model
 * <br><br>
 * <b>Journal:</b> Available
 * <br><br>
 * <b>Product(s):</b> 3D ACIS Modeler
 * <br><br>
 * @param iGivenWire
 * planar wire body to be offset.
 * @param iOptons
 * offset options.
 * @param oOffsetWire
 * returns offset wire body.
 * @param  ao
 * ACIS options such as versioning and journaling.
 **/
DECL_OFST outcome api_offset_planar_wire(
			                                BODY                 *iGivenWire,
                                            wire_offset_options  *iOptons,
			                                BODY                 *&oOffsetWire,
			                                AcisOptions          *ao = NULL);


/**
 * Creates a wire body that is an offset of the given wire.
 * <br><br>
 * <b>Role:</b> The wire is offset so that no portion of the offset wire is closer
 * to the original wire than the offset distance. Trimming the offset overlap caused
 * by C<sup>0</sup> edges, filling in the gaps caused by the offsets of C<sup>0</sup> edges, and trimming
 * areas of interference between offset segments are operations that get carried out.
 * <br><br>
 * This API function has four overloaded signatures: the one using @href wire_offset_options 
 * is the newest and includes nearly all the functionality of the other three. Therefore, the others 
 * are becoming obsolete.
 * <br><br>
 * A positive offset distance is in the direction of the cross product
 * (<tt>wire_tangent x wire_plane_normal</tt>). A negative offset is in the opposite direction.
 * <br><br>
 * <b>Errors:</b>
 * <ul>
 * <li>Pointer to wire is <tt>NULL</tt>.</li>
 * <li>Zero length normal vector specified.</li>
 * </ul>
 * <br><br>
 * <b>Limitations:</b>
 * <ul>
 * <li> Performing a wire body offset with open wires can encounter behavior that is
 * potentially different from what is expected. Offsetting of closed wires does
 * not have the potential for this issue. When offsetting an open wire body, all
 * of the edges are offset first.</li>
 * </ul>
 * <br><br>
 * <b>Effect:</b> Changes model
 * <br><br>
 * <b>Journal:</b> Available
 * <br><br>
 * <b>Product(s):</b> 3D ACIS Modeler
 * <br><br>
 * @param given_wire
 * planar wire to be offset.
 * @param trans
 * transformation.
 * @param offset_distance
 * given distance.
 * @param wire_normal
 * resulting normal vector.
 * @param offset_wire
 * returns offset wire body.
 * @param ao
 * ACIS options such as versioning and journaling.
 **/
DECL_OFST outcome api_offset_planar_wire(
		                                WIRE*            given_wire,
		                                TRANSFORM const* trans,
		                                double           offset_distance,
		                                const SPAunit_vector & wire_normal,
		                                BODY*&           offset_wire, 			
		                                AcisOptions*     ao = NULL);

/**
 * Offsets a face's loops to form a new body. The face geometry need not be planar.
 * <br><br>
 * <b>Role:</b> This API creates a new face, in the outbody. This face contains the loops which are the 
 * offset from the input face. The face geometry need not be planar.
 * <br><br>
 * <b>Limitations:</b> Refer to the <tt>face:offset-loops</tt> technical article for known limitations. 
 * <br><br>
 * <b>Effect:</b> Changes model
 * <br><br>
 * <b>Journal:</b> Available
 * <br><br>
 * <b>Product(s):</b> 3D ACIS Modeler
 * <br><br>
 * @param in_face
 * face which has the loop(s) to be offset.
 * @param offset_dist
 * distance to offset.
 * @param outbody
 * offset body.
 * @param ao
 * ACIS options such as versioning and journaling.
 **/
DECL_OFST outcome api_offset_face_loops
   (
      FACE*        iFace,
      double       offset_dist,
      BODY         *&outbody,
      AcisOptions  *ao = NULL
   );

/**
 * Offsets a face's loops to form a new body. The face geometry need not be planar.
 * <br><br>
 * <b>Role:</b> This API creates a new face, in the outbody. This face contains the loops which are the
 * offset from the input face. The face geometry need not be planar.
 * <br><br>
 * <b>Limitations:</b> Refer to the <tt>face:offset-loops</tt> technical article for known limitations. 
 * <br><br>
 * <b>Effect:</b> Changes model
 * <br><br>
 * <b>Journal:</b> Available
 * <br><br>
 * <b>Product(s):</b> 3D ACIS Modeler
 * <br><br>
 * @param in_face
 * face which has the loop(s) to be offset.
 * @param iEdges
 * list of edges to specify the loops, which contain at least one edge in the list.
 * @param offset_dist
 * distance to offset.
 * @param outbody
 * offset body.
 * @param ao
 * ACIS options such as versioning and journaling.
 **/
DECL_OFST outcome api_offset_face_loops
   (
      FACE*          iFace,
      ENTITY_LIST    &iEdges,
      double         iOffsetDist,
      BODY           *&oResultBody,
      AcisOptions    *ao = NULL
   );


/**
 * Creates a wire body whose edges are the offset of edges on faces for all edges of a face.
 * <br><br>
 * <b>Role:</b> This API creates a wire body by offseting edges where the offset
 * edges lie on faces which are topologically connected to the input. The offset
 * distance is measured along the surfaces of the intervening faces.
 * <br><br>
 * The enumeration <tt>offset_on_faces</tt> determines the side on which to calculate
 * the offset. The value <tt>SPA_OFST_ON_LEFT_FACE</tt> uses the face to the left of
 * the edge's coedge, which typically is the face associated with the input.
 * The value <tt>SPA_OFST_ON_RIGHT_FACE</tt> uses the face to the right of the edge's
 * coedge, which typically is the face adjacent to that associated with the input.
 * <br><br>
 * <b>Errors:</b> NULL input.
 * <br><br>
 * <b>Limitations:</b>
 * <ul>
 * <li>Offsets beyond a sheet boundary are not handled.</li>
 * <li>Negative offset distances are currently not allowed.</li>
 * <li>No automatic trimming of loops in the resulting wire body.</li>
 * <li>Offsets with large shape differences as compared to the input may not have the desired 
 * accuracy.</li>
 * </ul>
 * <br><br>
 * <b>Effect:</b> Changes model
 * <br><br>
 * <b>Journal:</b> Available
 * <br><br>
 * <b>Product(s):</b> 3D ACIS Modeler
 * <br><br>
 * @param inp_face
 * face whose edges are to be offset.
 * @param which_faces
 * enumeration specifying the side.
 * @param offset_dist
 * distance to offset the edges on the faces.
 * @param out_wire_body
 * offset wire body.
 * @param offset_edges_opts
 * not currently used.
 * @param ao
 * ACIS options such as versioning and journaling.
 */
DECL_OFST outcome api_offset_edges_on_faces (
      FACE                    *inp_face,
      offset_on_faces         which_faces,
      double                  offset_dist,
      BODY                    *&out_wire_body,
      offset_edges_options    *offset_edges_opts,
      AcisOptions             *ao
   );

/**
 * Creates a wire body whose edges are the offset of edges on faces for all edges of a loop.
 * <br><br>
 * <b>Role:</b> This API creates a wire body by offseting edges where the offset
 * edges lie on faces which are topologically connected to the input. The offset
 * distance is measured along the surfaces of the intervening faces.
 * <br><br>
 * The enumeration <tt>offset_on_faces</tt> determines the side on which to calculate
 * the offset. The value <tt>SPA_OFST_ON_LEFT_FACE</tt> uses the face to the left of
 * the edge's coedge, which typically is the face associated with the input.
 * The value <tt>SPA_OFST_ON_RIGHT_FACE</tt> uses the face to the right of the edge's
 * coedge, which typically is the face adjacent to that associated with the input.
 * <br><br>
 * <b>Errors:</b> NULL input.
 * <br><br>
 * <b>Limitations:</b>
 * <ul>
 * <li>Offsets beyond a sheet boundary are not handled.</li>
 * <li>Negative offset distances are currently not allowed.</li>
 * <li>No automatic trimming of loops in the resulting wire body.</li>
 * <li>Offsets with large shape differences as compared to the input may not have the desired 
 * accuracy.</li>
 * </ul>
 * <br><br>
 * <b>Effect:</b> Changes model
 * <br><br>
 * <b>Journal:</b> Available
 * <br><br>
 * <b>Product(s):</b> 3D ACIS Modeler
 * <br><br>
 * @param inp_loop
 * loop whose edges are to be offset.
 * @param which_faces
 * enumeration specifying the side.
 * @param offset_dist
 * distance to offset the edges on the faces.
 * @param out_wire_body
 * offset wire body.
 * @param offset_edges_opts
 * not currently used.
 * @param ao
 * ACIS options such as versioning and journaling.
 */
DECL_OFST outcome api_offset_edges_on_faces
   (
      LOOP                    *inp_loop,
      offset_on_faces         which_faces,
      double                  offset_dist,
      BODY                    *&out_wire_body,
      offset_edges_options    *offset_edges_opts,
      AcisOptions             *ao
   );

/**
 * Creates a wire body whose edges are the offset of edges on faces for all edges of a closed circuit.
 * <br><br>
 * <b>Role:</b> This API creates a wire body by offseting edges where the offset
 * edges lie on faces which are topologically connected to the input. The offset
 * distance is measured along the surfaces of the intervening faces.
 * <br><br>
 * The input edge representing the closed circuit must lie on the sheet boundary.
 * <br><br>
 * <b>Errors:</b>
 * <ul>
 * <li>NULL input.</li>
 * <li>Input edge is not on the sheet boundary.</li>
 * </ul>
 * <br><br>
 * <b>Limitations:</b>
 * <ul>
 * <li>Offsets beyond a sheet boundary are not handled.</li>
 * <li>Negative offset distances are currently not allowed.</li>
 * <li>No automatic trimming of loops in the resulting wire body.</li>
 * <li>Offsets with large shape differences as compared to the input may not have the desired 
 * accuracy.</li>
 * </ul>
 * <br><br>
 * <b>Effect:</b> Changes model
 * <br><br>
 * <b>Journal:</b> Available
 * <br><br>
 * <b>Product(s):</b> 3D ACIS Modeler
 * <br><br>
 * @param inp_edge_of_circuit
 * edge representing a closed circuit whose edges are to be offset.
 * @param offset_dist
 * distance to offset the edges on the faces.
 * @param out_wire_body
 * offset wire body.
 * @param offset_edges_opts
 * not currently used.
 * @param ao
 * ACIS options such as versioning and journaling.
 */
DECL_OFST outcome api_offset_edges_on_faces
   (
      EDGE                    *inp_edge_of_circuit,
      double                  offset_dist,
      BODY                    *&out_wire_body,
      offset_edges_options    *offset_edges_opts,
      AcisOptions             *ao
   );

/**
 * Creates a wire body whose edges are the offset of edges on faces for all edges of a coedge chain.
 * <br><br>
 * <b>Role:</b> This API creates a wire body by offseting edges where the offset
 * edges lie on faces which are topologically connected to the input. The offset
 * distance is measured along the surfaces of the intervening faces.
 * <br><br>
 * The input coedge chain must contain a list of coedges which are connected tip-to-tail,
 * but proper ordering is not required.
 * <br><br>
 * The enumeration <tt>offset_on_faces</tt> determines the side on which to calculate
 * the offset. The value <tt>SPA_OFST_ON_LEFT_FACE</tt> uses the face to the left of
 * the edge's coedge, which typically is the face associated with the input.
 * The value <tt>SPA_OFST_ON_RIGHT_FACE</tt> uses the face to the right of the edge's
 * coedge, which typically is the face adjacent to that associated with the input.
 * <br><br>
 * <b>Errors:</b> Coedge list does not form a properly connected chain.
 * <br><br>
 * <b>Limitations:</b>
 * <ul>
 * <li>Offsets beyond a sheet boundary are not handled.</li>
 * <li>Negative offset distances are currently not allowed.</li>
 * <li>No automatic trimming of loops in the resulting wire body.</li>
 * <li>Offsets with large shape differences as compared to the input may not have the desired 
 * accuracy.</li>
 * </ul>
 * <br><br>
 * <b>Effect:</b> Changes model
 * <br><br>
 * <b>Journal:</b> Available
 * <br><br>
 * <b>Product(s):</b> 3D ACIS Modeler
 * <br><br>
 * @param coedge_chain
 * coedge chain whose edges are to be offset.
 * @param which_faces
 * enumeration specifying the side.
 * @param offset_dist
 * distance to offset the edges on the faces.
 * @param out_wire_body
 * offset wire body.
 * @param offset_edges_opts
 * not currently used.
 * @param ao
 * ACIS options such as versioning and journaling.
 */
DECL_OFST outcome api_offset_edges_on_faces
   (
      ENTITY_LIST             &coedge_chain,
      offset_on_faces         which_faces,
      double                  offset_dist,
      BODY                    *&out_wire_body,
      offset_edges_options    *offset_edges_opts,
      AcisOptions             *ao
   );

/** @} */
#endif
