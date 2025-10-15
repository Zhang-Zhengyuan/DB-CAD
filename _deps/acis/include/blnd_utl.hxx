/* ORIGINAL: 3dt2.1/geomhusk/blnd_utl.hxx */
// $Id: blnd_utl.hxx,v 1.13 2002/08/09 17:28:25 jeff Exp $
/*******************************************************************/
/*    Copyright (c) 1989-2020 by Spatial Corp.                     */
/*    All rights reserved.                                         */
/*    Protected by U.S. Patents 5,257,205; 5,351,196; 6,369,815;   */
/*                              5,982,378; 6,462,738; 6,941,251    */
/*    Protected by European Patents 0503642; 69220263.3            */
/*    Protected by Hong Kong Patent 1008101A                       */
/*******************************************************************/
// --------------------------------------------------------------------
//  Purpose---
//    Header file for general blending utility procedures.
//
// --------------------------------------------------------------------

//
// Modification---
//	06-May-99 bd	Remove reduce_to_range; use the version in geom_utl.hxx.
//	30-Apr-98 bd	Decl make_blend_cross_curve() here.
//		Change its return from outcome to int.
//

#ifndef blnd_utl_hxx
#define blnd_utl_hxx

#include "dcl_blnd.h"
#include "api.hxx"
//#include "bl_att.hxx"	
#include "bl_cxty.hxx"
#include "edge.hxx"

class outcome;

class curve;
class SPAposition;
class SPApar_pos;

class ENTITY_LIST;
class ATTRIB_BLEND;

DECL_BLND int
make_blend_cross_curve(
	        ATTRIB_BLEND const *att,
	        double v_param,
	        curve * &ccrv,
            SPAposition &spine_pt = *(SPAposition *) NULL_REF,
            SPApar_pos &left_uv = *(SPApar_pos *) NULL_REF,
            SPApar_pos &right_uv = *(SPApar_pos *) NULL_REF
            );

DECL_BLND outcome
vertex_blends(
            ENTITY_LIST &vertex_list,
            double setback,
            double bulge,
	        logical autosb
            );

// Check whether there are any vertices in the blend graph where
// blend sequences have different convexity and there one or more 
// unblended edges incident on the vertex.

// Also returns, if required, 
// - the list of mixed convexity vertices which have atleast one
// unblended edge and hence vertex blending cannot handle them.
// - the list of mixed convexity vertices with no unblended edges
// and hence rollon vertex blending can handle such vertices.

DECL_BLND logical 
mixed_convexity_blends(
            ENTITY_LIST const &eds, 
            ENTITY_LIST &mix_verts
                = * (ENTITY_LIST *) NULL_REF,
            ENTITY_LIST &rollon_verts
                = * (ENTITY_LIST*) NULL_REF
            );

DECL_BLND bl_convexity 
blend_edge_convexity( EDGE *in_ed );

DECL_BLND int 
sort_edges_with_sense(
            ENTITY_LIST &eds, 
            EDGE **&order, 
            int *&senses, 
            int &closed
            );

DECL_BLND void 
mark_blend_vertices(
            ENTITY_LIST const &, 
            ENTITY_LIST &out_verts
                = *(ENTITY_LIST *) NULL_REF, 
			double bulge = 1.0, 
            double set_back = -1.0
            );

DECL_BLND int 
propogate_round_attribute(
            EDGE *base, 
            ENTITY_LIST &added, 
			ENTITY_LIST &subset 
                = * (ENTITY_LIST *) NULL_REF
            );

void 
propagate_chamfer_attribute(
			ENTITY_LIST &in_eds,
			ENTITY_LIST &subset_eds, 
			ENTITY_LIST &out_eds
			);

// Propagate the blend attributes to edges that share the same
// pair of supports as the blended edges. The two-valent vertices 
// separating these edges from the blended edges are redundant and 
// the blend won't stop at these vertices.

DECL_BLND void 
propagate_at_twovalent_ends( 
			ENTITY_LIST &graph_ents,
			logical attach_attrib
			);

DECL_BLND logical is_chamfer( ATTRIB_BLEND *bln_att );

#endif
