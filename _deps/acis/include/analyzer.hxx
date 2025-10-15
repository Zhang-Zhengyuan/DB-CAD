/*******************************************************************/
/*    Copyright (c) 1989-2020 by Spatial Corp.                     */
/*    All rights reserved.                                         */
/*    Protected by U.S. Patents 5,257,205; 5,351,196; 6,369,815;   */
/*                              5,982,378; 6,462,738; 6,941,251    */
/*    Protected by European Patents 0503642; 69220263.3            */
/*    Protected by Hong Kong Patent 1008101A                       */
/*******************************************************************/

#if !defined (BHL_ANALYZER)
#define BHL_ANALYZER

#include "logical.h"

#include "analyzer2.hxx"
#include "analyzer3.hxx"
#include "bhl_stru2.hxx"

#include "aggrsimg.hxx"
#include "cegmbld.hxx"


class ENTITY_LIST;
class BODY;
class SPAbox;


DECL_HEAL void
bhl_analyze_geom_types( ENTITY_LIST& , bhl_geom_types* );

DECL_HEAL void
bhl_analyze_body_type( ENTITY_LIST&,bhl_anal_body_stats_results* =NULL );

DECL_HEAL void
bhl_fit_view( ENTITY_LIST& );

DECL_HEAL void
bhl_analyze_splitsur( ENTITY_LIST& );

DECL_HEAL void
bhl_analyze_simgeom( ENTITY_LIST&,
					bhl_anal_simgeom_results* anal_simg_results);

DECL_HEAL void
bhl_analyze_simgeom_at_tol( ENTITY_LIST& body_list, double tol,
						   bhl_anal_simgeom_results* anal_simg_results);

DECL_HEAL void
bhl_analyze_geometry( ENTITY_LIST& , logical,
					 bhl_anal_geometry_results* anal_geom_results);

// Project: HEAL_TO_TOLERANCE
DECL_HEAL void
hh_analyze_geometry_for_body(BODY* body,
			bhl_anal_geometry_results* anal_geom_results,
			logical check_discontinuity,
			logical record_untightened_entities);

DECL_HEAL void
bhl_analyze_spline_uv( ENTITY_LIST &);

DECL_HEAL void
bhl_analyze_analytic_solver( ENTITY_LIST &);

DECL_HEAL void
bhl_sim_geom_heuristics(bhl_anal_simgeom_results* anal_simg_results);

DECL_HEAL void
bhl_geom_build_heuristics (bhl_anal_geometry_results* anal_geom_results);

class HH_Anal_Geombld
{
private :
	BODY* body;
public:
	HH_Anal_Geombld(BODY* = NULL);
	~HH_Anal_Geombld();
public:
//	 int bhl_anal_splitsur_no_splits ;
	 int bhl_anal_geom_checked_surfaces;
	 int bhl_anal_geom_bad_surfaces;
	 int bhl_anal_geom_total_surfaces;
	 int bhl_anal_geom_bad_poles ;
	 int bhl_anal_geom_tangent_edges_3_4_sided ;
	 int bhl_anal_geom_tangent_uv_uv_internal_internal ;
	 int bhl_anal_geom_tangent_uv_uv_boun_internal ;
	 int bhl_anal_geom_tangent_complete_uv_uv ;
	 int bhl_anal_geom_tangent_uv_uv_boun_boun;
	 int bhl_anal_geom_tangent_edges_nonuv_nonuv;
	 int bhl_anal_geom_tangent_edges_uv_nonuv ;
	 int bhl_anal_geom_tangent_edges_uv_uv ;
	 int bhl_anal_geom_tangent_edges_analytic;
	 int bhl_anal_geom_G1_bad_analytic_tangent_edges ;
	 int bhl_anal_geom_tangent_edges;
	 int bhl_anal_geom_tranverse_edges_spline ;
	 int bhl_anal_geom_tranverse_edges_analytic ;
	 int bhl_anal_geom_checked_vertices;
	 int bhl_really_bad_vertices;
	 int bhl_anal_geom_bad_vertices;
	 int bhl_anal_geom_total_vertices;
	 int bhl_anal_geom_checked_edges;
	 int bhl_really_bad_edges;
	 int bhl_anal_geom_bad_edges;
	 int bhl_anal_geom_total_edges;
	 int bhl_anal_geom_checked_coedges;
	 int bhl_really_bad_coedges;
	 int bhl_anal_geom_bad_coedges;
	 int bhl_anal_geom_total_coedges;
	 int bhl_curr_healing_percentage ;
public:
	void init_all_members(BODY* body);
	void update_result_struct(bhl_anal_geometry_results* anal_geom_results);
	logical hh_analyze_vertex(VERTEX* v, logical& innocent);

	// HEAL_TO_TOLERANCE
	void hh_analyze_vertices(ENTITY_LIST& vertex_list, logical geombuild_check);
	
	logical hh_is_edge_tangential(EDGE* e, hh_coedge_details& c1, hh_coedge_details& c2);
	void classify_edge(EDGE* e);
	logical hh_analyze_edge(EDGE* e, logical& innocent);
	
	// HEAL_TO_TOLERANCE
	void hh_analyze_edges(ENTITY_LIST& edge_list, logical geombuild_check);
	
	logical hh_analyze_coedge(COEDGE* ce, logical& innocent);
	void hh_analyze_coedges(ENTITY_LIST& coedge_list);
	logical hh_analyze_face(FACE* f);
	void hh_analyze_faces(ENTITY_LIST& face_list);
};

#endif
