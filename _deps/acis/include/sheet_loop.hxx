/*******************************************************************/
/*    Copyright (c) 1989-2020 by Spatial Corp.                     */
/*    All rights reserved.                                         */
/*    Protected by U.S. Patents 5,257,205; 5,351,196; 6,369,815;   */
/*                              5,982,378; 6,462,738; 6,941,251    */
/*    Protected by European Patents 0503642; 69220263.3            */
/*    Protected by Hong Kong Patent 1008101A                       */
/*******************************************************************/
#ifndef SPA_SHEET_LOOP_HXX
#define SPA_SHEET_LOOP_HXX

#include "dcl_covr.h"
class outcome;
class EDGE;
class ENTITY_LIST;
class AcisOptions;
/**
 * @file sheet_loop.hxx
 * @CAA2Level L1
 * @CAA2Usage U1
 * \addtogroup ACISQUERIES
 *
 * @{
 */

/**
 * Find the edges comprising a connected branch of the boundary of a sheet body or open solid which includes the input seed edge. This is analogous to a loop of a face.
 * <br><br>
 * <b>Role:</b> This API finds all of the connected edges comprising a connected portion of a sheet body or open solid. 
 * The input edge is part of the returned boundary.
 * <br><br>
 * <b>Limitations</b>: The input edge must lie on a sheet boundary, that is, must have exactly one coedge and exactly one face.
 * <br><br>
 * <b>Effect:</b> Read-only
 * <br><br>
 * <b>Journal: </b> Available
 * <br><br>
 * <b>Product(s):</b> 3D ACIS Modeler
 * <br><br>
 * @param edge
 * edge on the sheet body boundary
 * @param loop
 * returned edges comprising the boundary
 * @param ao
 * ACIS options such as journal, version
 **/
DECL_COVR outcome api_sheet_loop(
						EDGE* ed, 
						ENTITY_LIST& loop, 
						AcisOptions* ao=NULL);
/** @} */

#endif
