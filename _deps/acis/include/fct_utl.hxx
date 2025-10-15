/*******************************************************************/
/*    Copyright (c) 1989-2020 by Spatial Corp.                     */
/*    All rights reserved.                                         */
/*    Protected by U.S. Patents 5,257,205; 5,351,196; 6,369,815;   */
/*                              5,982,378; 6,462,738; 6,941,251    */
/*    Protected by European Patents 0503642; 69220263.3            */
/*    Protected by Hong Kong Patent 1008101A                       */
/*******************************************************************/
//    Declare procedures for facetting ENTITIES
/*******************************************************************/
#ifndef fct_utl_h
#define fct_utl_h
#include <stdio.h>
class ENTITY;
class ENTITY_LIST;
class INDEXED_MESH;
class surface;
class outcome;
class facet_options;
#include "api.hxx"
#include "logical.h"
#include "dcl_fct.h"
#include "idx_mesh.hxx"
/**
 * @file fct_utl.hxx
 * @CAA2Level L1
 * @CAA2Usage U1
 *! \addtogroup FCTAPI
 *  \brief Declared at <fct_utl.hxx>
 *  @{
 */
/**
 * @nodoc
 */
DECL_FCT outcome facet_unfaceted_faces(ENTITY_LIST& elist);
/**
 * @nodoc
 */
DECL_FCT outcome initialize_facetter();
/**
 * @nodoc
 */
DECL_FCT INDEXED_MESH* GetMesh(const ENTITY*);

/**
* Facets the faces of an entity, producing a SEQUENTIAL_MESH.
* <br><br>
* <b>Role:</b> Facets the input entity using the appropriate <tt>api_facet_</tt> function.
* This function provides a quick and easy way to facet entities, using one of two common mesh managers.
* If the global option <tt>mesh_manager_opt</tt> is set to <tt>"linked"</tt>, a @href LINKED_MESH_MANAGER is used.
* Otherwise, an @href INDEXED_MESH_MANAGER is used. The default for the <tt>mesh_manager_opt</tt> option is <tt>"indexed"</tt>.
* These mesh managers attach facet data to faces using attributes.
* This facet data may be retrieved using the function @href GetSequentialMesh.
* Returns the number of facets generated.
* <br><br>
* The behavior of the Faceter is specified by the <tt>facet_options</tt> object. For more
* information on using <tt>facet_options</tt> objects, refer to the Technical Article on
* <i>Facet Options</i>.
* <br><br>
* The preferred interface is to call @href api_set_mesh_manager (specifying any valid @href MESH_MANAGER) 
* and then call @href api_facet_entity. This allows applications to set the mesh manager to any
* valid mesh manager. 
* <br><br>
* <b>Effect:</b> Attaches facet data to the faces of the input entity.
* <br><br>
* <b>Journal: </b> Not Available
* <br><br>
* @param ent
* Input entity containing the faces to facet.
* @param unfacetted_only
* Input argument. Set to <tt>TRUE</tt> if you wish to skip facetting faces that already have facet data attached.
* @param NumStrips
* Optional output variable, used internally by Spatial.
* @param NumSingleTris
* Optional output variable, used internally by Spatial.
* @param SizeOfData
* Optional output variable, used internally by Spatial.
* @param fo
* instance of a <tt>facet_options</tt> object, specifying how to facet the faces.
* @param ao
* ACIS options.
**/
DECL_FCT int FacetEntity(ENTITY* ent,
						 logical unfacetted_only,
						 int& NumStrips= *(int*)NULL_REF,
						 int& NumSingleTris = *(int*)NULL_REF,
						 unsigned& SizeOfData= *(unsigned *)NULL_REF,
						 facet_options* fo = NULL,
						 AcisOptions* ao = NULL   // acis options
						 );

/**
* Gets the @href SEQUENTIAL_MESH facet data attached to a face.
* <br><br>
* <b>Role:</b> Usually used after calling the function @href FacetEntity .
* Retrieves the facet data created by @href FacetEntity .
* <br><br>
* <b>Effect:</b> Read only
* <br><br>
* <b>Journal: </b> Not Available
* <br><br>
* @param ent
* Input face that should have @href SEQUENTIAL_MESH facet data attached to it.
* This will be one of the following derived classes: @href INDEXED_MESH or @href LINKED_MESH .
**/
DECL_FCT SEQUENTIAL_MESH* GetSequentialMesh(const ENTITY* ent);

/**
 * @nodoc
 */
DECL_FCT logical IsFacetted(ENTITY*);

/**
 * @nodoc
 */
DECL_FCT void DeleteEntityFacets(ENTITY* ent,logical apply_to_descendents);

/**
 * @nodoc
 */
DECL_FCT int CountEntityFacets(ENTITY*);

/**
 * @nodoc
 */
DECL_FCT void WriteEntityFacets(ENTITY*, FILE* );
/**
 * @nodoc
 */
DECL_FCT void WriteRAWEntityFacets(ENTITY_LIST&, FILE* );

/**
 * @nodoc
 */
DECL_FCT INDEXED_MESH* CreateMeshFromSurface( surface* surf,
											double umin,
											double umax,
											double vmin,
											double vmax,
											int nu,
											int nv );

/**
 * @nodoc
 */
DECL_FCT void              // eff: recompute the vertex locs in mesh
UpdateMeshFromSurface      //      using surf
  (surface* surf,          // in : surface defining the mesh
   INDEXED_MESH* mesh) ;   // in : Mesh made by CreateMeshFromSurface



/**
 * @nodoc
 * Class used in Progress Meter for Rendering (process facet results to vieweing system)
 * This is a specific instance for HA_Render_Model and Ha_Render_Entities functions
 * This meter will update be updated every 1%.
 */
class DECL_FCT HA_RENDER_progress_info: public SPA_progress_info
{
    // total number of entitites to render
	int ent_count;
    // current entity count
	int current_ent_count;
    // If we are in the first call or last one
	logical first_call, last_call;
    // previous percentage computed
	int last_pct;

public:
    /**
     * Constructor
     * @param ec
     * Total number of entities
     */
	HA_RENDER_progress_info( int ec );
	/**
     * Destructor
     */
	~HA_RENDER_progress_info();
    /**
     * Type of progress info
     */
	virtual SPA_progress_info_type_id get_type() const;
    /**
     * Current percentage
     */
	virtual int percentage() const;
    /**
     * Do update, will increment the counter
     */
	void update( void );
};

DECL_FCT void set_faceter_snaps_minmax( logical tf );
DECL_FCT logical faceter_snaps_minmax();

/**
* @nodoc
**/
// copies edge facets from one edge to another.  Assumes these edges have same param range, geometry, etc.  If used
// when this assumption does not hold, bad things will happen.
DECL_FCT void copy_af_points(EDGE* from_edge, EDGE* to_edge);

DECL_FCT bool get_arguments( char const * string, char const * delim, int offset, char * tok, int toklen );

/* @} */
#endif
