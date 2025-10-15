/*******************************************************************/
/*    Copyright (c) 1989-2020 by Spatial Corp.                     */
/*    All rights reserved.                                         */
/*    Protected by U.S. Patents 5,257,205; 5,351,196; 6,369,815;   */
/*                              5,982,378; 6,462,738; 6,941,251    */
/*    Protected by European Patents 0503642; 69220263.3            */
/*    Protected by Hong Kong Patent 1008101A                       */
/*******************************************************************/
//     Local Operation Internal topology change functions
/*******************************************************************/
#ifndef LTTO
#define LTTO
#include "dcl_lopt.h"
#include "logical.h"
#include "position.hxx"
#include "errorsys.hxx"
#include "cvty.hxx"
#include "sep_shls.hxx"
#include "ptfcenum.hxx"

class ENTITY;
class BODY;
class LUMP;
class SHELL;
class FACE;
class EDGE;
class COEDGE;
class VERTEX;
class LOOP;
class surface;
class curve;
class CURVE;
class SPAbox;
class curve_surf_int;
class SPAposition;
class ATTRIB_BLEND;
class SURFACE;
class ENTITY_LIST;
class loop_class_data;

/**
 * Internal function. Do not use.
 */
DECL_LOPT logical lopt_delete_isol_vertex( LOOP *lp );
/**
 * Internal function. Do not use.
 */
DECL_LOPT FACE *lopt_unzip_edge( COEDGE *coed, FACE *use_face = NULL );
/**
 * Internal function. Do not use.
 */
DECL_LOPT void lopt_zip_up_edge( COEDGE *coed );
/**
 * Internal function. Do not use.
 */
DECL_LOPT void lopt_zip_up_edge(EDGE *edge, EDGE *mate_edge);
/**
 * Internal function. Do not use.
 */
DECL_LOPT COEDGE *lopt_split_edge( COEDGE *coed, SPAposition const &pos );
/**
 * Internal function. Do not use.
 */
DECL_LOPT logical lopt_link_out_coedge( COEDGE *coed );
/**
 * Internal function. Do not use.
 */
DECL_LOPT logical lopt_link_in_coedge( COEDGE *prev, COEDGE *coed );
/**
 * Internal function. Do not use.
 */
DECL_LOPT void lopt_split_degen_vertex( COEDGE *coed, VERTEX *split_vertex );
/**
 * Internal function. Do not use.
 */
DECL_LOPT logical lopt_link_out_loop( LOOP *loop );
/**
 * Internal function. Do not use.
 */
DECL_LOPT logical lopt_link_in_loop( LOOP *prev, LOOP *loop );
/**
 * Internal function. Do not use.
 */
DECL_LOPT logical lopt_link_out_face( FACE *fa );
/**
 * Internal function. Do not use.
 */
DECL_LOPT logical lopt_link_in_face( FACE *prev, FACE *fa );
/**
 * Internal function. Do not use.
 */
DECL_LOPT logical lopt_link_out_shell( SHELL *sh );
/**
 * Internal function. Do not use.
 */
DECL_LOPT logical lopt_link_in_shell( SHELL *prev, SHELL *sh );
/**
 * Internal function. Do not use.
 */
DECL_LOPT logical lopt_link_out_lump( LUMP *lump );
/**
 * Internal function. Do not use.
 */
DECL_LOPT logical lopt_link_in_lump( LUMP *prev, LUMP *lump );
/**
 * Internal function. Do not use.
 */
DECL_LOPT FACE *lopt_add_degen_face( COEDGE *coed, logical simple_closed, 
		      logical start_run_out, 
		      logical end_run_out );
/**
 * Internal function. Do not use.
 */
DECL_LOPT logical lopt_remove_prop( COEDGE *coed );
/**
 * Internal function. Do not use.
 */
DECL_LOPT logical lopt_add_prop( FACE *fa, SPAbox const &bx );
/**
 * Internal function. Do not use.
 */
DECL_LOPT int lopt_surface_sings( const surface &sf, SPAposition pos[ 4 ] );
/**
 * Internal function. Do not use.
 */
DECL_LOPT logical lopt_correct_sings( FACE *fa, const SPAposition *sing_vert_pos = NULL);
/**
 * Internal function. Do not use.
 */
DECL_LOPT int lopt_edge_count( VERTEX *vert );
/**
 * Internal function. Do not use.
 */
DECL_LOPT int lopt_edge_count( LOOP *loop );
/**
 * Internal function. Do not use.
 */
DECL_LOPT logical lopt_isolated_vertex( const COEDGE *coed );
/**
 * Internal function. Do not use.
 */
DECL_LOPT logical lopt_corner_vertex( COEDGE *coed );
/**
 * Internal function. Do not use.
 */
DECL_LOPT logical lopt_vortex( COEDGE *coed );
/**
 * Internal function. Do not use.
 */
DECL_LOPT logical lopt_vortex( VERTEX *vert );
/**
 * Internal function. Do not use.
 */
DECL_LOPT logical lopt_on_vortex( VERTEX *vert );
/**
 * Internal function. Do not use.
 */
DECL_LOPT logical lopt_on_vortex( VERTEX *vert, FACE *given_face );
/**
 * Internal function. Do not use.
 */
DECL_LOPT logical convert_non_manifold_vortex_into_manifold( VERTEX *the_vertex);
/**
 * Internal function. Do not use.
 */
DECL_LOPT logical lopt_free_edge( COEDGE *coed );
/**
 * Internal function. Do not use.
 */
DECL_LOPT logical lopt_free_vertex( VERTEX *vert );
/**
 * Internal function. Do not use.
 */
DECL_LOPT logical lopt_non_manifold_vertex( COEDGE *coed );
/**
 * Internal function. Do not use.
 */
DECL_LOPT logical lopt_non_manifold_edge( COEDGE *coed );
/**
 * Internal function. Do not use.
 */
DECL_LOPT logical lopt_non_manifold( COEDGE *coed );
/**
 * Internal function. Do not use.
 */
DECL_LOPT logical lopt_closed_edge( COEDGE *coed );
/**
 * Internal function. Do not use.
 */
DECL_LOPT logical lopt_simple_closed_edge( COEDGE *coed );
/**
 * Internal function. Do not use.
 */
DECL_LOPT logical lopt_prop_edge( COEDGE *coed );
/**
 * Internal function. Do not use.
 */
DECL_LOPT logical lopt_simple_prop( VERTEX *vert );
/**
 * Internal function. Do not use.
 */
DECL_LOPT COEDGE *lopt_find_prop( LOOP *loop );
/**
 * Internal function. Do not use.
 */
DECL_LOPT logical lopt_void_shell( SHELL *sh );
/**
 * Internal function. Do not use.
 */
DECL_LOPT logical lopt_kev( COEDGE *, logical kill_end_vertex = TRUE, 
                           logical kill_one_edge_face = FALSE, logical allow_non_mani = TRUE );
/**
 * Internal function. Do not use.
 */
DECL_LOPT COEDGE *lopt_adev( COEDGE *change_coed, COEDGE *other_coed, CURVE *CU = NULL,logical bVertexAtStartOfCoedge = FALSE);
/**
 * Internal function. Do not use.
 */
DECL_LOPT COEDGE *lopt_adev( COEDGE *change_coed, CURVE *CU = NULL ,logical bStart = TRUE);
/**
 * Internal function. Do not use.
 */
DECL_LOPT SPAunit_vector lopt_spine_tangent( SURFACE *surf, SPAposition const pos );
/**
 * Internal function. Do not use.
 */
DECL_LOPT SPAvector lopt_into_face_dir( COEDGE *coed, int loc = 1 );
/**
 * Internal function. Do not use.
 */
DECL_LOPT logical lopt_peripheral_loop( LOOP *loop, loop_class_data const &lcd );
/**
 * Internal function. Do not use.
 */
DECL_LOPT point_face_containment lopt_point_in_loop( LOOP *test_loop, 
								  const SPAposition &test_pt,
								  loop_class_data &lcd,
								  const SPApar_pos &guess = *( SPApar_pos * ) NULL_REF,
                                  logical use_tol_ver = FALSE
								  );
/**
 * Internal function. Do not use.
 */
DECL_LOPT point_face_containment lopt_point_in_face_loops( FACE *test_face, 
	    const SPAposition &test_pt, LOOP *avoid_loop = NULL );
/**
 * Internal function. Do not use.
 */
DECL_LOPT point_face_containment lopt_point_in_face( FACE *test_face, 
										   const SPAposition &test_pt );
/**
 * Internal function. Do not use.
 */
DECL_LOPT logical lopt_surrounding_faces( FACE *fa, ENTITY_LIST &faces, 
				 ENTITY_LIST &s_faces );
/**
 * Internal function. Do not use.
 */
DECL_LOPT int lopt_surrounding_faces( FACE *fa, ENTITY_LIST &faces );
/**
 * Internal function. Do not use.
 */
DECL_LOPT logical lopt_faces_touching_on_edge( FACE *f1, FACE *f2 );
/**
 * Internal function. Do not use.
 */
DECL_LOPT logical lopt_faces_touching_at_vertex( FACE *f1, FACE *f2 );
/**
 * Internal function. Do not use.
 */
DECL_LOPT logical lopt_connected_faces( ENTITY_LIST &faces, 
			     ENTITY_LIST &c_faces, logical check_across_other_loops = FALSE );
/**
 * Internal function. Do not use.
 */
DECL_LOPT logical lopt_void_shell( SHELL *sh );

/**
 * Internal function. Do not use.
 */
DECL_LOPT void remove_two_edge_vertices( FACE *face );

#endif



