/*******************************************************************/
/*    Copyright (c) 1989-2020 by Spatial Corp.                     */
/*    All rights reserved.                                         */
/*    Protected by U.S. Patents 5,257,205; 5,351,196; 6,369,815;   */
/*                              5,982,378; 6,462,738; 6,941,251    */
/*    Protected by European Patents 0503642; 69220263.3            */
/*    Protected by Hong Kong Patent 1008101A                       */
/*******************************************************************/

#ifndef BHL_TOPCLEAN2
#define BHL_TOPCLEAN2


#include "logical.h"
#include "alltop.hxx"
#include "allsurf.hxx"
#include "pcurve.hxx"
#include "curve.hxx"
#include "point.hxx"

#include "dcl_stitch.h"


class ENTITY;
class insanity_list;

// Structure for Topclean results

struct DECL_STITCH bhl_topclean_results
{
	int no_zero_edges;			// No of zero edges
	int no_zero_faces;			// No of zero faces
	int no_dup_vert;			// No of duplicate vertices
	int no_edge_group;			// No of edge group fixed
	int no_edges_added;			// No. of additional edges filled in gaps

	// Constructor function
	bhl_topclean_results ()
	{
		no_zero_edges =0;
		no_zero_faces = 0;
		no_dup_vert = 0;
		no_edge_group = 0;
		no_edges_added = 0;
	}
};

// Group related functions which are used by other modules of the healer

DECL_STITCH void bhl_fix_edge_group( ENTITY* , int& , ENTITY_LIST* = NULL);

DECL_STITCH logical bhl_remove_edge_group(VERTEX *, EDGE* );

DECL_STITCH logical bhl_remove_edge(EDGE* ,VERTEX *vert_to_remove = NULL);

DECL_STITCH logical bhl_same_edge_group(VERTEX *,EDGE *, EDGE *);

DECL_STITCH void bhl_replace_vertex_direct(VERTEX* ,VERTEX*& ,EDGE* , logical ,logical&);

DECL_STITCH logical bhl_check_and_fix_edge_group( EDGE* );

DECL_STITCH void bhl_remove_zero_edges(ENTITY* , double , int& );

DECL_STITCH void bhl_clean_negative_area_faces(ENTITY* , int& );

DECL_STITCH logical bhl_construct_curve_pcurve(const surface& surf,
								   const SPAposition& spos,
								   const SPAposition& epos,
								   CURVE*& new_curve,
								   PCURVE*& new_pcurve);

logical bhl_check_edge_isolated(EDGE* );

// GSSL/RS (Aug 19, 2003) - Change the argument from ENTITY * to const ENTITY_LIST&
DECL_STITCH void stch_split_edges_at_convexity(const ENTITY_LIST& edges);

DECL_STITCH void stch_update_tangential_edges(BODY* , logical , double );

DECL_STITCH logical stch_check_edge_tangency(
											 EDGE* , 
											 logical , 
											 double , 
											 double& = *(double*)NULL_REF, 
											 double& = *(double*)NULL_REF,
											 logical exclude_knife = FALSE
											 );

DECL_STITCH logical stch_check_tangency_at_point(const surface& , 	// First surface
							const surface& ,	// Second surface
							const SPAposition& ,	// Test point for tangency
							double, 
							double&,
							double& );

enum STCH_EDGE_SEAM_SIMPLIFY_STATUS
{
	SEAM_LINE_SIMPLIFICATION_NOT_NEEDED = 0, // No need for simplification of the edge
	SEAM_LINE_SIMPLIFIED, // Simplification successful.
	SEAM_LINE_NOT_SIMPLIFIED // Simplification failed.
};

DECL_STITCH logical 
stch_simplify_seam_edge_on_closed_surface(EDGE *edge, // Input : Edge to be simplified.
								STCH_EDGE_SEAM_SIMPLIFY_STATUS &status // Output : status of the 
										// simplification done to the edge
								);

DECL_STITCH void 
stch_check_and_fix_tedges_in_entity(ENTITY* ent);

DECL_STITCH logical
stch_check_and_fix_tedge(TEDGE* edge, logical& already_good = *(logical *)NULL_REF);

DECL_STITCH logical
stch_chck_edge_cur_for_error(EDGE* edge, insanity_list& ilist, logical do_check_face_pcurve = TRUE);

// Moved this function from heal to stitch.
// It now returns a logical. TRUE if the body was reversed.
DECL_STITCH logical
bhl_solid_orient( ENTITY* );

DECL_STITCH void
bhl_merge_duplicate_vertices( ENTITY* acis_ent , double tol, int& no_dup_vert , logical merge_shells,
							 SPAbox* body_box = NULL);

DECL_STITCH logical
hh_is_vertex_merging_required(VERTEX* vertex, VERTEX* vert);

DECL_STITCH SPAbox get_body_box(const ENTITY_LIST& ents);


#endif // BHL_TOPCLEAN
