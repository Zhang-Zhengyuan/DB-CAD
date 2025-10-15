/* $Id: */
/*******************************************************************/
/*    Copyright (c) 1989-2020 by Spatial Corp.                     */
/*    All rights reserved.                                         */
/*    Protected by U.S. Patents 5,257,205; 5,351,196; 6,369,815;   */
/*                              5,982,378; 6,462,738; 6,941,251    */
/*    Protected by European Patents 0503642; 69220263.3            */
/*    Protected by Hong Kong Patent 1008101A                       */
/*******************************************************************/

// A general header file to keep declarations of utility fns
// for blending.

//
// Modification---
//	06-May-99 bd	Remove reduce_to_range(): use the version in geom_utl.hxx.
//

#if !defined( IBLND2_HDR_DEF )
#define  IBLND2_HDR_DEF

#include "dcl_blnd.h"
#include "base.hxx"

class SPAunit_vector;
class ffblend_geom;
class COEDGE;
class ENTITY_LIST;
class segend;
class ATTRIB_VBLEND;
class SPAinterval;
class VERTEX;
class surface;
class SPApar_pos;

// The main entry function for improved vtx blnds

segend *
bl_vertedge_geom_new(
            ATTRIB_VBLEND *vertex_blend_att,
            ENTITY_LIST	const &ents,
            COEDGE *this_coedge
            );

// Find saved details of spring curve intersections on cc side at 
// the start of the given coedge or compute them. When we emerge 
// from this function, the relevent cc_x_info is ALWAYS marked as 
// known.

ffblend_geom *
find_cc_spring_cur_int(
	COEDGE *this_coedge,	        // coedge whose start is at blended vertex
	ENTITY_LIST	const &ents,		// list of blended entities
	ffblend_geom *this_ffblend_geom = NULL,
	ffblend_geom *cc_ffblend_geom = NULL
    );

// Function to decide if a rolling ball vertex blend solution is
// possible at the vertex.

logical 
rb_vtx_blnd_possible( 
                VERTEX *bl_vertex,
				COEDGE *this_coedge,
				ENTITY_LIST const &ents,
				logical &odd_bl_edge_at_rb_bl = 
                    *(logical *) NULL_REF
				);

// Utility to adjust for periodicity.

void 
fix_up_period( 
        surface const &sf, 
        SPApar_pos const &uv0, 
        SPApar_pos &uv1 
        );

// This routine finds the signed angle between two vectors v1, v2
// in R3. ( angle from v1 to v2 in an anticlockwise sense about 
// a given normal )

double 
find_signed_angle( 
        SPAunit_vector const &v1, 
        SPAunit_vector const &v2,
        SPAunit_vector const &nv 
        );

#endif
