// $Id: acovrapi.hxx,v 1.5 2002/07/29 15:44:52 btomas Exp $
/*******************************************************************/
/*    Copyright (c) 1989-2020 by Spatial Corp.                     */
/*    All rights reserved.                                         */
/*    Protected by U.S. Patents 5,257,205; 5,351,196; 6,369,815;   */
/*                              5,982,378; 6,462,738; 6,941,251    */
/*    Protected by European Patents 0503642; 69220263.3            */
/*    Protected by Hong Kong Patent 1008101A                       */
/*******************************************************************/
// bmt 11-Feb-03 - Doc revision + name change: acover_options ==> acovr_options
// bmt 31-Oct-03 - added api_advanced_cover_no_stitch()
//
#ifndef ADV_COVER_API
#define ADV_COVER_API
/**
 * \defgroup ACISADVCOVR Advanced Covering
 * \ingroup DEFMODAPI
 *
 */
/**
* @file acovrapi.hxx
 * @CAA2Level L1
 * @CAA2Usage U1
 * \addtogroup ACISADVCOVR
 *
 * @{
 */

// For user convenience, we #include all required header files here
#include "acover_opts.hxx"
#include "acovr_enum.hxx"
#include "acovr_gap_report.hxx"

#include "dcl_adm.h"

class outcome;
class EDGE;
class FACE;
class AcisOptions;

#ifndef NULL
#define NULL 0
#endif

/**
 * Calculates a new surface covering a circuit of edges and adds it to the body.
 * <br><br>
 * <b>Role:</b> This signature of the API function creates a face by covering the specified circuit
 * of edges. The circuit surrounds a hole in the given face. ACIS uses the input edge to find 
 * a circuit of unshared (i.e., free or wire) edges that include that edge. 
 * <br><br>
 * After finding the circuit of edges ACIS calculates a covering surface that contains the
 * edges in the circuit. ACIS then creates a new face using this surface and hooks
 * it into the model (see <tt>api_cover_circuits</tt>). This face is returned in the
 * <tt>result_face</tt> argument. Note that <tt>result_face</tt> must be set to <tt>NULL</tt> before 
 * passing it to <tt>api_advanced_cover</tt>; this protects you from accidentally overwriting
 * an active face pointer.
 * <br><br>
 * After the new face has been added to the model, edges in the boundary circuit
 * are made tolerant as necessary to avoid gap errors.
 * <br><br>
 * The advance covering options provide additional information to be used
 * in creating the surface, such as continuity constraints on the boundary edges,
 * shape parameters, required tolerances, or maximum number of B-spline spans
 * to be used in the covering surface.
 * <br><br>
 * <b>Limitations:</b>
 * <ul>
 * <li>Covering circuits must be projectable onto the initial surface.</li>
 * <li>Each edge in the circuit must have a single coedge.</li>
 * <li>A circuit is not allowed to branch.</li>
 * <li>A circuit is not allowed to be self-intersecting.</li>
 * <li>"Double-valued" covering surfaces cannot be created, that is, no two 
 * points on the covered surface can project to the same point on the initial surface.</li>
 * </ul>
 * Refer to the <i>Advanced Covering Limitations</i> Technical Article 
 * for an in-depth discussion of the limitations.
 * <br><br>
 * <b>Effect:</b> Changes model.
 * <br><br>
 * <b>Journal: </b> Available
 * <br><br>
 * <b>Product(s):</b> 3D ACIS Advanced Covering, 3D ACIS Deformable Modeling
 * <br><br>
 * @param result_face
 * face created.
 * @param edge_in_circuit
 * an edge in the circuit to be covered.
 * @param aco
 * Advanced Covering options.
 * @param opts
 * general ACIS options.
 **/
DECL_ADM outcome api_advanced_cover( FACE *&result_face, EDGE *edge_in_circuit, acovr_options *aco=NULL, AcisOptions *opts=NULL);

/**
 * Calculates a new surface covering a circuit of edges and adds it to the body.
 * <br><br>
 * <b>Role:</b> This signature of the API function "re-covers" an existing face 
 * by calculating a new surface to cover the outer loop of edges of the face.
 * After finding the outer loop of edges ACIS calculates a covering surface that contains the
 * edges and replaces the geometry of the existing face argument, <tt>target_face</tt>. 
 * After the new surface has been added to the model, edges in the boundary circuit
 * are made tolerant as necessary to avoid gap errors.
 * <br><br>
 * The advance covering options provide additional information to be used
 * in creating the surface, such as continuity constraints on the boundary edges,
 * shape parameters, required tolerances, or maximum number of B-spline spans
 * to be used in the covering surface.
 * <br><br>
 * <b>Limitations:</b>
 * <ul>
 * <li>Covering circuits must be projectable onto the initial surface.</li>
 * <li>Each edge in the circuit must have a single coedge on the given face.</li>
 * <li>A circuit is not allowed to branch.</li>
 * <li>A circuit is not allowed to be self-intersecting.</li>
 * <li>"Double-valued" covering surfaces cannot be created, that is, no two 
 * points on the covered surface can project to the same point on the initial surface.</li>
 * </ul>
 * Refer to the <i>Advanced Covering Limitations</i> Technical Article 
 * for an in-depth discussion of the limitations.
 * <br><br>
 * <b>Effect:</b> Changes model.
 * <br><br>
 * <b>Journal: </b> Available
 * <br><br>
 * <b>Product(s):</b> 3D ACIS Advanced Covering, 3D ACIS Deformable Modeling
 * <br><br>
 * @param target_face
 * face to be re-covered.
 * @param aco
 * Advanced Covering options.
 * @param opts
 * general ACIS options.
 **/
DECL_ADM outcome api_advanced_cover( FACE *target_face, acovr_options *aco=NULL, AcisOptions *opts=NULL);


/**
 * Calculates a new surface covering a circuit of edges, and a corresponding face, bounded by the circuit. The new face is independent
 * of any existing topology.
 * <br><br>
 * <b>Role:</b> This API function creates a face by covering the specified circuit of
 * edges. The edges must form a closed circuit. Two vertices are considered coincident for the purpose of determining 
 * the circuit's closure if they are separated by a distance less than the looser of the two position constraint 
 * tolerances of the corresponding edges.
 * The input edges can come from arbitrary owners; the resulting face will not be stitched to any existing topology:  
 * all input edges will be copied. 
 * <br><br>
 * <b>Limitations:</b>
 * <ul><li>Covering circuits must be projectable onto the initial surface.</li>
 * <li>Each edge in the circuit must have a single coedge.</li>
 * <li>A circuit is not allowed to branch.</li>
 * <li>A circuit is not allowed to be self-intersecting.</li>
 * <li>"Double-valued" covering surfaces cannot be created, that is, no two 
 * points on the covered surface can project to the same point on the initial surface.</li></ul>
 * <br> 
 * Refer to the <i>Advanced Covering Limitations</i> Technical Article 
 * for an in-depth discussion of the limitations.
 * <br><br>
 * <b>Effect:</b> Changes model.
 * <br><br>
 * <b>Journal: </b> Available
 * <br><br>
 * <b>Product(s):</b> 3D ACIS Advanced Covering, 3D ACIS Deformable Modeling
 * <br><br>
 * @param result_face
 * FACE created.
 * @param edges
 * list of edges forming the circuit, in any order
 * @param aco
 * Advanced Covering options.
 * @param opts
 * general ACIS options.
 **/
DECL_ADM outcome
api_advanced_cover_no_stitch(FACE*& result_face, ENTITY_LIST const& edges,  acovr_options* aco=NULL, AcisOptions* opts=NULL);

// global function to report all gaps on an edge
/*
// tbrv.
*/
/**
 * @nodoc
 */
DECL_ADM void acover_report_gaps(const EDGE* ed, acovr_gap_report& gr);  // Report the gaps on this edge

 /** @} */
#endif

