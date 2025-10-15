/* $Id: esplit.hxx,v 1.11 2002/08/09 17:19:05 jeff Exp $ */
/*******************************************************************/
/*    Copyright (c) 1989-2020 by Spatial Corp.                     */
/*    All rights reserved.                                         */
/*    Protected by U.S. Patents 5,257,205; 5,351,196; 6,369,815;   */
/*                              5,982,378; 6,462,738; 6,941,251    */
/*    Protected by European Patents 0503642; 69220263.3            */
/*    Protected by Hong Kong Patent 1008101A                       */
/*******************************************************************/


#if !defined (SG_ESPLIT)
#define SG_ESPLIT

#include "dcl_intr.h"
// ywoo 08Jan01: added the header file.
#include "base.hxx"
// ywoo 08Jan01: added the classes.

/**
* @file esplit.hxx
 * @CAA2Level L1
 * @CAA2Usage U1
 */

/** \addtogroup INTRDIRECTSPLITTING
 *  \brief Declared at <esplit.hxx>, SPAintr
 *  @{
 */

class surface;
class FACE;
class SPAtransf;
class COEDGE;
// ywoo: end
class EDGE;
class VERTEX;
class ENTITY_LIST;

//STI jmg: moved this from bool to intr so we could call this in check.
//STI let: merged two sg_split_edge_at_vertex() functions
//void
//sg_split_edge_at_vertex(
//	EDGE* old_edge,
//	VERTEX* new_vertex,
//	ENTITY_LIST& coedge_list
//	);

/**
 * Splits an edge at a vertex that is known to lie within the domain of the edge.
 * <br><br>
 * <b>Role:</b> Splits an edge at a vertex that is known to lie in the interior
 * of the edge; the old edge will go from the start to the new while the new edge
 * will go from the new to the end.
 * <br><br>
 * All coedges sharing the edge are also split with the new coedge sharing the new
 * edge and the partner order of coedges around the old edge corresponding to the
 * partner order of the new coedges around the new edge.
 * <br><br>
 * If the <tt>split_geometry</tt> option is set to <tt>TRUE</tt>, the curve underneath is split if
 * the parameter lies on a discontinuity, otherwise, the new edges share the same
 * curve.
 * <br><br>
 * <b>Effect:</b> Changes model
 * <br><br>
 * @param old_edge
 * edge to be split.
 * @param new_vertex
 * vertex known to lie in interior of edge.
 * @param coedge_list
 * returned list of coedges.
 * @param split_geometry
 * hard split on geometry.
 **/
DECL_INTR void
sg_split_edge_at_vertex(
    EDGE*        old_edge,                              // edge to split
    VERTEX*      new_vertex,                            // vertex known to lie in interior of edge
    ENTITY_LIST& coedge_list = *(ENTITY_LIST*)NULL_REF, // returned list of coedges
    logical      split_geometry = FALSE                 // hard split on geometry
    );
//STI let: end
/**
 * Splits an edge at a vertex that is known to lie within the domain of the edge.
 * <br><br>
 * <b>Role:</b> Splits an edge at a vertex that is known to lie in the interior
 * of the edge; the old edge will go from the start to the new while the new edge
 * will go from the new to the end.
 * <br><br>
 * If <tt>vert_param</tt> is not provided, it will calculate this parameter from the
 * position of the new vertex with respect to the edge.
 * <br><br>
 * All coedges sharing the  edge are also split with the new coedge sharing the new
 * edge and the partner order of coedges around the old edge   corresponding to the
 * partner order of the new coedges around the new edge.
 * <br><br>
 * If the <tt>split_geometry</tt> option is set to <tt>TRUE</tt>, the curve underneath is split if
 * the parameter lies on a discontinuity, otherwise, the new edges share the same
 * curve.
 * <br><br>
 * <b>Effect:</b> Changes model
 * <br><br>
 * @param old_edge
 * edge to be split.
 * @param new_vertex
 * vertex known to lie in interior of edge.
 * @param vert_param
 * parameter at which the vertex lies.
 * @param coedge_list
 * returned list of coedges.
 * @param split_geometry
 * hard split on geometry.
 **/
DECL_INTR void
sg_split_edge_at_vertex(
    EDGE*         old_edge,                                 // edge to split
    VERTEX*       new_vertex,                               // vertex known to lie in interior of edge
    double        vert_param,                               // parameter at which the vertex lies
    ENTITY_LIST&  coedge_list    = *(ENTITY_LIST*)NULL_REF, // returned list of coedges
    logical       split_geometry = FALSE                    // hard split on geometry
   );

/*
// tbrv
*/
/**
 * @nodoc
 */

DECL_INTR void
sg_make_two_edges(
	EDGE   *old_edge,
	VERTEX *new_vertex,
	EDGE   **new_edge
	);

/*
// tbrv
*/
/**
 * @nodoc
 */


DECL_INTR logical
sg_add_poles_to_boundary(
        surface const& ref_surf,
        FACE* copied_face,
        SPAtransf const& ftrans
        );

// STI jmg  1/30/98 begin
//------------------------------------------------------------------------------
//	split edge at convexity points.
//------------------------------------------------------------------------------
/**
 * Splits an edge at convexity change points.
 * <br><br>
 * <b>Role:</b> Edges that change convexity over the course of their length cause
 * problems during operations such as blending, offsetting, and shelling. Changes 
 * in edge convexity are reported by the entity checker at level 60. Generally speaking, 
 * ACIS should not create mixed convexity edges; however, models translated from other 
 * systems may have mixed convexity edges. 
 * <br><br>
 * If <tt>split_edge</tt> is <tt>TRUE</tt>, <tt>sg_split_edge_at_convextiy</tt> 
 * searches for the points along the edge where the convexity changes and inserts 
 * vertices in any such locations. If <tt>split_edge</tt> is <tt>FALSE</tt> and
 * any such points are found, the edge is marked as being a <i>mixed convexity</i>
 * edge.
 * <br><br>
 * For additional information on edge convexity, refer to the <i>Edge Convexity</i>
 * Section of the <i>Edge</i> Technical Article and the documentation on the 
 * <i>convexity_split</i> global option.
 * <br><br>
 * <i><b>Note:</b> This function does not process tolerant edges.</i>
 * <br><br>
 * <b>Effect:</b> Changes model
 * <br><br>
 * @param edge
 * edge to be split.
 * @param split_edge
 * split the edge or flag it as a mixed convexity edge.
 * @param coedge_list
 * returned list of coedges.
 **/
DECL_INTR void
sg_split_edge_at_convexity(
EDGE *edge,
logical split_edge=TRUE,
ENTITY_LIST&  coedge_list    = *(ENTITY_LIST*)NULL_REF
);

// this function checks if there is an existing vertex before calling sg_split_edge_at_vertex
// also it splits multiple param values in a coedge

/*
// tbrv
*/
/**
 * @nodoc
 */

DECL_INTR COEDGE *split_coedge_at_params(COEDGE *, int, double *,
									     ENTITY_LIST &out_coed = *(ENTITY_LIST *) NULL_REF,
										 logical = FALSE);

/** @} */

#endif
