/* ORIGINAL: acis2.1/faceter/acisintf/handlers.hxx */
// $Id: handlers.hxx,v 1.12 2002/08/20 16:05:07 gradclif Exp $
/*******************************************************************/
/*    Copyright (c) 1989-2020 by Spatial Corp.                     */
/*    All rights reserved.                                         */
/*    Protected by U.S. Patents 5,257,205; 5,351,196; 6,369,815;   */
/*                              5,982,378; 6,462,738; 6,941,251    */
/*    Protected by European Patents 0503642; 69220263.3            */
/*    Protected by Hong Kong Patent 1008101A                       */
/*******************************************************************/
#ifndef _HANDLERS
#define _HANDLERS


#include "dcl_fct.h"


// General headers

#include "logical.h"


// ACIS classes

class ENTITY ;
class ENTITY_LIST ;
class BODY ;
class LUMP ;
class SHELL ;
class FACE ;

class SPAbox;

class SPAposition;
class SPAvector;
class SPAtransf;

// Facetter classes

class REFINEMENT ;
class POLYGON_POINT_MESH ;
class POLYGON ;
class facet_options;
// STIPORT TAC from 1.5.2
#if defined( osf1 ) || defined( __alpha )
typedef long parameter_token;
#else
typedef int parameter_token ;
#endif
class VERTEX_TEMPLATE;
class REFINEMENT;
class REFINEMENT_ARRAY;
class AF_ENVIRONMENT;

#include "af_env.hxx"

DECL_FCT AF_ENVIRONMENT *af_modal_environment();

DECL_FCT logical initialise_faceter(MESH_MANAGER *mm);

DECL_FCT logical initialize_faceter();

DECL_FCT logical terminate_faceter();

DECL_FCT logical create_refinement ( REFINEMENT *&ref);

DECL_FCT logical modify_refinement
        (
           REFINEMENT *R,
           double     flatness = 0.5,
           double     aspect_ratio = 4.0,
           double     surface_deviation = 0.0,
           double     normal_deviation = 0.0,
           double     silhouette_deviation = 0.0,
           double     pixel_area = 0.0,
           int        minimum_level = 0,
           int        maximum_level = 0,
           int        mode = 0
        );

// Attach a refinement to an entity; if 'apply_to_descendants' is TRUE the
// refinement will be attached to all entities below the given entity (eg.
// if the entity is a LUMP the refinement will be attached to all its
// SHELL's and to each SHELL's FACEs.  Setting the refinement
// for a particular entity to NULL has the effect of causing the default
// refinement, to be used when facetting is subsequently performed.


DECL_FCT logical set_entity_refinement(
	ENTITY     *E,
	REFINEMENT *R = (REFINEMENT *)NULL,
	logical    apply_to_descendants = FALSE
        );

DECL_FCT logical get_entity_refinement(ENTITY *E,REFINEMENT *&R,
		AF_SURF_MODE mode =AF_SURF_ALL);


DECL_FCT logical set_entity_vertex_template(
							  ENTITY     *E,
							  VERTEX_TEMPLATE *T,
							  logical    apply_to_descendants
							  );

DECL_FCT logical get_entity_vertex_template(ENTITY *E, VERTEX_TEMPLATE *&T);


DECL_FCT logical set_default_refinement(REFINEMENT *R);

DECL_FCT logical get_default_refinement(REFINEMENT *&R,
		AF_SURF_MODE mode =AF_SURF_ALL);

DECL_FCT logical create_vertex_template( 
           int             n_tokens,
           parameter_token tokens[],
           VERTEX_TEMPLATE *&T
        );

DECL_FCT logical modify_vertex_template
        ( 
           int             n_tokens,
           parameter_token tokens[],
           VERTEX_TEMPLATE *T
        );


DECL_FCT logical set_default_vertex_template ( VERTEX_TEMPLATE *T);

DECL_FCT logical get_default_vertex_template( VERTEX_TEMPLATE *&T);

DECL_FCT ENTITY *af_body_to_entity(BODY *body);

DECL_FCT ENTITY *af_shell_to_entity(SHELL *shell);

DECL_FCT ENTITY *af_lump_to_entity(LUMP *lump);

DECL_FCT ENTITY *af_face_to_entity(FACE *face);

DECL_FCT logical af_facet_entity(ENTITY *entity,logical griddable, facet_options* fo);

// API's to handle incremental faceting, i.e. faceting unmarked faces only.
DECL_FCT logical af_facet_unfaceted_entity(ENTITY *entity, logical griddable, facet_options* fo);

DECL_FCT logical af_facet_entities(ENTITY *owner,ENTITY_LIST &EL,logical griddable, facet_options* fo);

DECL_FCT logical af_facet_unfaceted_entities(ENTITY *owner,ENTITY_LIST &EL,logical griddable, facet_options* fo);
 
// Test whether a face has been facetted.
// This was mispelled in the API, preserve it here via define:
#define facetted_face faceted_face
DECL_FCT logical facetted_face
        (
           FACE *face, logical &facetted
        );

// Get the Read-only version of the facets that are attached to an entity.  If
// no facets exist a NULL value is returned.  If it is required to edit the
// POLYGON_POINT_MESH it must be converted to a polygon_point_mesh (which can,
// if necessary, be converted back to a POLYGON_POINT_MESH).
DECL_FCT logical get_body_facets
        (
           BODY               *body,
           POLYGON_POINT_MESH *&pmesh,
           logical            share_edge_vertices
        );

DECL_FCT logical get_lump_facets
        (
           LUMP               *lump,
           POLYGON_POINT_MESH *&pmesh,
           logical            share_edge_vertices
        );

DECL_FCT logical get_shell_facets
        (
           SHELL              *shell,
           POLYGON_POINT_MESH *&pmesh,
           logical            share_edge_vertices
        );

DECL_FCT logical get_face_facets
        (
           FACE               *face,
           POLYGON_POINT_MESH *&pmesh,
           logical            share_edge_vertices
        );

DECL_FCT logical delete_entity_facets(ENTITY *E, logical apply_to_descendents, logical delete_mark = TRUE);

DECL_FCT FACE *af_get_indexed_face(ENTITY *E,int id);

DECL_FCT logical af_transform_edge_points( EDGE *E, SPAtransf &T );

DECL_FCT logical mark_faceted_faces( logical mark );

typedef logical (* ALT_FACETER)(ENTITY *);

DECL_FCT void set_alternate_faceter(ALT_FACETER);

DECL_FCT  ALT_FACETER get_alternate_faceter(void);

DECL_FCT void UpdateRefVt(ENTITY * ent, REFINEMENT *& ref, VERTEX_TEMPLATE *&vt, int& obtained_from);

DECL_FCT logical af_facet_area(ENTITY* entity, double& area);

class DECL_FCT degen_tri_info {
	public:
	double		m_area;
	SPAposition	m_pos[3];
	int			m_face_tag;
	int			m_face_index;
	int			m_polygon_index;
	virtual void process(double area, int face_tag, int face_index, int polygon_index, SPAposition pos0, SPAposition pos1, SPAposition pos2)
	{
		m_area = area;
		m_face_tag = face_tag;
		m_face_index = face_index;
		m_polygon_index = polygon_index;
		m_pos[0] = pos0;
		m_pos[1] = pos1;
		m_pos[2] = pos2;
	};
};

DECL_FCT logical facet_area(ENTITY* entity, double& area, int* degen_tri_count = nullptr, degen_tri_info * degen_tri = nullptr, int* degen_tri_at_singularity_count = nullptr );

DECL_FCT logical af_verify_facet_normals(ENTITY* entity, int* bad_vertex_normal_count, int* bad_polygon_normal_count, double* out_worst_angle);

#endif
