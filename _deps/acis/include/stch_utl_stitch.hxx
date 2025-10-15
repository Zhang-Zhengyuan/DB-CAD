/*******************************************************************/
/*    Copyright (c) 1989-2020 by Spatial Corp.                     */
/*    All rights reserved.                                         */
/*    Protected by U.S. Patents 5,257,205; 5,351,196; 6,369,815;   */
/*                              5,982,378; 6,462,738; 6,941,251    */
/*    Protected by European Patents 0503642; 69220263.3            */
/*    Protected by Hong Kong Patent 1008101A                       */
/*******************************************************************/

#if !defined (BHL_STITCH_UTL)
#define BHL_STITCH_UTL

#include "api.hxx"
#include "acistype.hxx"    
#include "entity.hxx"

#include "dcl_stitch.h"
#include "aggrstch_stitch.hxx"
#include "bs2curve.hxx"

class ENTITY;
class SPAposition;
class EDGE;
class BODY;
class VERTEX;
class ENTITY_LIST;
class curve;
//class option_header;
class outcome;

// Option for Tolerant Stitching
//DECL_STITCH extern option_header hh_do_tolerant_edges;

DECL_STITCH void
bhl_remove_stitch_attribs(ENTITY* bf);

DECL_STITCH void
bhl_remove_stitch_attribs_in_list(ENTITY_LIST &);

DECL_STITCH void
bhl_copy_entity_list ( ENTITY_LIST&, ENTITY_LIST& );

DECL_STITCH void
bhl_delete_entity_list ( ENTITY_LIST& );

DECL_STITCH logical
bhl_get_unshared_edges( ENTITY* , ENTITY_LIST& );

DECL_STITCH void
bhl_make_bodies_for_stitch ( ENTITY_LIST& ent_list );

DECL_STITCH void
bhl_unhook_entities_in_list(ENTITY_LIST&);

DECL_STITCH void
bhl_get_ends_of_edge(EDGE*, SPAposition& , SPAposition& , logical = FALSE);

DECL_STITCH logical
bhl_get_params_of_edge(EDGE*, double& , double& , logical = FALSE);

DECL_STITCH logical
bhl_get_edge_params_with_curve(EDGE* , double& , double& , const curve& );


DECL_STITCH logical
are_two_edge_geom_almost_same( EDGE* ,EDGE*,double st_toler, double end_toler,
				double mid_toler,double& gap,
			        double&,double&
				);
								

// GSSL/RS-FSS
// GSSL/RS (Jun 12,2003) : Failsafe stitching project
// Made the ENTITY_LIST& argument const
DECL_STITCH logical
get_closest_vertex_from_list( VERTEX* vertex, const ENTITY_LIST&, VERTEX*&,
														double, logical);
// GSSL/RS-FSS
// GSSL/RS (Jun 12,2003) : Failsafe stitching project
// Made the ENTITY_LIST& argument const
DECL_STITCH logical
get_closest_edge_from_list(	VERTEX*, const ENTITY_LIST&, VERTEX*&, EDGE*&,
									logical&, double, const bhl_stitch_options *);

DECL_STITCH double
get_smallest_edge_near_vertex( VERTEX* vertex );


DECL_STITCH void
hh_merge_bodies (ENTITY_LIST& body_list, BODY*& new_body);

DECL_STITCH void
hh_make_body_from_face (FACE* face, BODY*& new_body);

DECL_STITCH void
hh_merge_bodies (ENTITY_LIST& body_list, BODY*& new_body);

DECL_STITCH void
hh_make_single_body (ENTITY_LIST& ent_list, BODY*& new_body);

DECL_STITCH void
hh_split_lumps_into_bodies (BODY* body, ENTITY_LIST& bodies);

DECL_STITCH logical
hh_is_edge_shared(EDGE* edge);

DECL_STITCH COEDGE*
hh_get_partner_coedge(COEDGE* this_coedge);

DECL_STITCH void
hh_get_same_vertices(VERTEX* v, ENTITY_LIST& vertex_list);

DECL_STITCH logical
hh_are_vertices_same(VERTEX* v1, VERTEX* v2);

DECL_STITCH int
hh_lookup_vertex_in_list(VERTEX* v, ENTITY_LIST& vert_list);

DECL_STITCH void
bhl_stitch_replace_vertex(VERTEX* ,VERTEX* ,EDGE *extra ,double, ENTITY_LIST * = NULL, logical unlimit_curve_if_needed = FALSE );

DECL_STITCH logical
hh_coalesce_unshared_loops(BODY * ,bhl_stitch_results *, ENTITY_LIST& edges, const bhl_stitch_options& stitch_opt );

logical
bhl_check_newcur_with_oldcur(curve* ,  EDGE* , double, double, int=10, logical=TRUE);

//RS - start (Nov 02,2001) : Function to detach all stitching attribs
DECL_STITCH logical
detach_stitching_attribs(BODY* body);
//RS - end

// Function for combining entities to a body
// RS - start (Feb 08,2002) : Its better to return a logical rather than an outcome.
DECL_STITCH logical
// RS - end
hh_combine_bodies(ENTITY_LIST &ent_list, // Input list of entities
				  BODY *&new_body,		 // Output combine body
				  int &body_count,		// Body (non-wire) count in the input list (output paramter)
				  int &face_count		// Number of the faces added to the combined body  (output paramter).
				  );


// GSSL/RS - (May 29,2002) : Function for checking whether the edge is paired. This is different from
// hh_is_edge_shared() because this function returns true for the edges which do no have
// entity stitch attributes. if attributes are present it will call hh_is_edge_shared()
logical 
stch_is_edge_not_to_be_stitched(EDGE *edge,
								const tolerant_stitch_options* tol_sopts = NULL,
								logical consider_shared_edges = FALSE);

// GSSL/RS - (Jun 27,2002) : Single face stitching
// Function to tolerize a list of entities
// Though the function returns outcome, it can still throw exceptions.
DECL_STITCH
outcome stch_tolerize_entities(
			const ENTITY_LIST &entities,
			ENTITY_LIST &bad_edges,
			ENTITY_LIST &new_edges,
			ENTITY_LIST &bad_vertices,
			ENTITY_LIST &new_vertices,
			ENTITY *& worst_edge,
			double &worst_edge_error,
			ENTITY *& worst_vertex,
			double &worst_vertex_error,
			double tolval);

// GSSL/RS - (Jul 01,2002) : Single face stitching
// Function to de-tolerize a list of EDGEs if TEDGEs.
logical
stch_replace_tedges_with_edges(const ENTITY_LIST &edges, ENTITY_LIST &new_edges);

// GSSL/RS - (Jul 03,2002) : Single face stitching
// Function to compute the stitch options - called by stitch edges
// for analyzing
logical
stch_compute_internal_stitch_opts(ENTITY_LIST &edges, BODY *body);

// sxt 1.6.2005 ray fire project
logical stch_is_shell_peripheral(SHELL* sh);

// sxt 16.9.2005
// 79782
// made global to be used in heal
DECL_STITCH
logical stch_is_pcurve_on_surface_discontinuity(COEDGE* coedge);

DECL_STITCH
logical stch_is_bs2_curve_on_surface_discontinuity(const bs2_curve bs2_c, const surface& surf);


#endif

