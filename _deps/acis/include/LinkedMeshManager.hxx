/*******************************************************************/
/*    Copyright (c) 1989-2020 by Spatial Corp.                     */
/*    All rights reserved.                                         */
/*    Protected by U.S. Patents 5,257,205; 5,351,196; 6,369,815;   */
/*                              5,982,378; 6,462,738; 6,941,251    */
/*    Protected by European Patents 0503642; 69220263.3            */
/*    Protected by Hong Kong Patent 1008101A                       */
/*******************************************************************/
//    The LINKED_MESH_MANAGER is a (non-global) indexed mesh manager.
/*******************************************************************/
#ifndef LINKED_MESH_MANAGER_HXX
#define LINKED_MESH_MANAGER_HXX
#include "dcl_fct.h"
#include "af_api.hxx"
#include "LinkedMesh.hxx"
class SPAposition;
class SPAunit_vector;
class SPAtransf;
class VERTEX_TEMPLATE;
/**
 * @file LinkedMeshManager.hxx
 * @CAA2Level L1
 * @CAA2Usage U2
 *! \addtogroup FCTAPI
 *  \brief Declared at <LinkedMeshManager.hxx>
 *  @{
 */

/**
 * This class provides an example of a mesh manager using linked data.
 * <b>Role</b>: The LINKED_MESH_MANAGER constructs a LINKED_MESH from facet data.
 * @see LINKED_MESH
 */
class DECL_FCT LINKED_MESH_MANAGER : public MESH_MANAGER {

 protected:

/**
 * @nodoc
 */
	AF_VU_SET *m_VUSet;

/**
 * Pointer to mesh.
 */
	LINKED_MESH *m_pMesh;
	
/**
 * Number of polygons in all meshes.
 */
	int m_totalPolygonCount;
	
/**
 * Number of polygon nodes in all meshes.
 */
	int m_totalPolyNodeCount;
	
/**
 * Number of nodes in all meshes.
 */
	int m_totalNodeCount;
	
/**
 * Number of polygons in current face's meshes.
 */
	int m_currFacePolygonCount;
	
/**
 * Number of polygon nodes in current face's meshes.
 */
	int m_currFacePolyNodeCount;

/**
 * Number of nodes in current face's meshes.
 */
	int m_currFaceNodeCount;
	
/**
 * Number of tri strips.
 */
	int NumStrips;

/**
 * Number of Single Triangles.
 */
	int NumSingleTris;

/**
 * Transformation to apply to the nodes of the mesh.
 */
	const SPAtransf* m_pTransform;

/**
 * Function to print an error message.
 */
	void sequence_error(char *s);

/**
 * <tt>MESH_MANAGER_SEARCH_ORDER</tt> dictates order of polygons (for strips, fans, etc)
 */
	MESH_MANAGER_SEARCH_ORDER mmso;  //SC mka 01-16-03 Added a member for storing search order. In order to be able to set it.

 public:

/**
 * A callback that tells the faceter we need a count of polygons, polynodes, and nodes.
 * <br><br>
 * <b>Role:</b> If this function returns <tt>TRUE</tt>, the number of polygons, number
 * of nodes, and number of polygon references of node are calculated and
 * announced. If these are not needed, it should return <tt>FALSE</tt>. 
 */
	 logical need_counts() { return FALSE; }

/**
 * C++ constructor, creates an LINKED_MESH_MANAGER.
 */
 	LINKED_MESH_MANAGER();

/**
 * C++ destructor, deletes an LINKED_MESH_MANAGER.
 */
	virtual ~LINKED_MESH_MANAGER();

/**
 * Returns the current mesh be constructed.
 */
	LINKED_MESH* mesh() const { return m_pMesh; }

/**
 * Returns the total number of polygons in all meshes.
 */
	int NumPolygon() const { return m_totalPolygonCount;}

/**
 * Returns the total number of polynodes in all meshes.
 * Each vertex may be shared by <i>n</i> polygons, so there would be <i>n</i> polynodes
 * for that vertex, one for each polygon.
 */
	int NumPolyNode() const { return m_totalPolyNodeCount;}

/**
 * Returns the total number of polygons in all meshes.
 */
	int NumVertex() const { return m_totalNodeCount;}

/**
 * Returns the total number of strips in all meshes (includes single triangle strips).
 */
	virtual int GetNumStrips() const { return NumStrips;}

/**
 * Returns the number of strips containing only one triangle in all meshes.
 */
	virtual int GetNumSingleTris() const { return NumSingleTris;}

/**
 * Called by the faceter, informing the manager about the number of strips
 * added for each mesh encountered.
 */
	virtual void AddNumStrips(int _NumStrips) {NumStrips += _NumStrips;}

/**
 * Called by the faceter, informing the manager about the number of strips with single triangles
 * added for each mesh encountered.
 */
	virtual void AddNumSingleTris(int _NumSingleTris) {NumSingleTris += _NumSingleTris;}

/**
 * Sets the transformation to be applied to nodes.
 * @param trans
 * transformation
 */
	void SetTransform(const SPAtransf* trans);

/**
 * Returns the order of how the internal mesh of the faceter should be searched to provide output.
 * <b>Role</b>: The order specifies how successive polygons ought to share
 * edges and can help the application to construct the desired data format.
 * The default is to search triangle fans, triangle strips and quad strips.
 */
 virtual MESH_MANAGER_SEARCH_ORDER query_search_order();

/**
 * Sets the order of how the internal mesh of the faceter should be searched to provide output.
 * <b>Role</b>: The order specifies how successive polygons ought to share
 * edges and can help the application to construct the desired data format.
 * The default is to search triangle fans, triangle strips and quad strips.
 */
 virtual void set_search_order(MESH_MANAGER_SEARCH_ORDER mmso);//SC mka 01-16-03 Added set_searchj order method.

// This function is called when a new face is about to be faceted.
/**
 * Announces the beginning of the output of a mesh.
 * <b>Role</b>: Cleans up old mesh if necessary.
 * @param entity
 * faceted entity
 * @param app_ref
 * applicable refinement
 * @param format
 * output format entity
 */
 virtual void begin_mesh_output(
		ENTITY *entity,				// Entity about to be faceted
		ENTITY *app_ref,			// Current REFINEMENT
		ENTITY *format				// Current output format (VERTEX_TEMPLATE)
	);

/**
 * If this function returns <tt>TRUE</tt>, the indexed protocol is turned on.
 * <b>Role</b>: The default returns <tt>FALSE</tt>. Used with the indexed protocol to
 * flag requested data.
 */
	virtual logical need_indexed_polygons() { return TRUE; }

    //following is called if the user wants duplicate nodes where the surface uv's differ
    //at the same SPAposition on the surface.
/**
 * This method is called if the user wants duplicate nodes where the surface uv's differ
 * at the same position on the surface.
 */
	virtual logical need_duplicate_indexed_nodes_on_surface_seams() { return FALSE; }

/**
 * This method is called if the user wants duplicate nodes where singularities have different 
 * normals for different facets, such as at the apex of a cone.
 */
	virtual logical need_duplicate_indexed_nodes_at_singularities() { return TRUE; }

/**
 * Provides an approximate count of polygons and nodes and number of references to nodes.
 */
 virtual logical need_approx_counts(){return FALSE;}

/**
 * Announces an indexed node with its %surface parameters, position, and normal.
 * @param inode
 * 0-based node index.
 * @param uv
 * parametric coordinates.
 * @param iX
 * cartesian coordinates.
 * @param N
 * %surface normal.
 */
 virtual void *announce_indexed_node(
		int inode,					// the number of this node
		const SPApar_pos &uv,		// the uv parameters for this node
		const SPAposition &iX,		// the coordinates for this node
		const SPAunit_vector &N		// the normal at this node
	);

/**
 * Announces the start of a polygon and adds it to the mesh.
 * @param ipoly
 * polygon index
 * @param npolynode
 * number of polygon node
 * @param ishare
 * info about which edge of previous polygon is shared with this one; <tt>-2</tt> means not used.
 */
	virtual void start_indexed_polygon(
		int ipoly,					// the number of this polygon
		int npolynode,				// the number of node references in this polygon
		int ishare = -2				//the edge which is common from previous poly
	);

	// This null implementation is just to get rid of a warning more likely due
	// to compiler's own confusion about function being hidden.
/**
 * Announces an indexed polynode and stores the data into the polygon.
 * <br><br>
 * <b>Role:</b> This signature has a NULL implementation in the <tt>LINKED_MESH_MANAGER</tt>.
 * @param E
 * coedge along the edge following the polynode.
 * @param ipoly
 * 0-based polygon index.
 * @param i
 * 0-based node index within this polygon.
 * @param id
 * node identifier as previously received from <tt>announce_indexed_node</tt>.
 */
	virtual void announce_indexed_polynode(ENTITY *E, int ipoly, int i, void* id) 
	{
		SPAUNUSED(E)
		SPAUNUSED(ipoly)
		SPAUNUSED(i)
		SPAUNUSED(id)
	}

/**
 * Announces an indexed polynode and stores the data into the polygon.
 * @param ipoly
 * 0-based polygon index.
 * @param i
 * 0-based node index within this polygon.
 * @param id
 * node identifier as previously received from <tt>announce_indexed_node</tt>.
 */
	virtual void announce_indexed_polynode(
		int ipoly,					// 0-based polygon index. This is the
									//  same as the immediately preceding call to
									//  start_indexed_polygon().
		int i,						// 0-based counter within the polygon.  This
									//  increments sequentially on successive calls.
		void *id					// Node identifer as previously received from
									// announce_indexed_node
	);

/**
 * Announces an indexed polynode and stores the data into the polygon.
 * <br><br>
 * <b>Role:</b> This signature has a NULL implementation in the <tt>LINKED_MESH_MANAGER</tt>.
 * @param E
 * coedge along the edge following the polynode.
 * @param ipoly
 * 0-based polygon index.
 * @param i
 * 0-based node index within this polygon.
 * @param id
 * node identifier as previously received from <tt>announce_indexed_node</tt>.
 * @param tpar
 * if the node lies on the interior of a model edge, then returns the curve parameter value.
 * @param uv
 * parametric coordinates.
 * @param iX
 * cartesian coordinates.
 * @param N
 * %surface normal.
 */
 virtual void announce_indexed_polynode(
	ENTITY* E,				// Entity (actually, COEDGE or null) along the
								// edge following the polynode.
	int ipoly,				// 0-based polygon index. This is the
								//	same as the immediately preceding call to
								//  start_indexed_polygon().
	int i,					// 0-based counter within the polygon.  This
								//	increments sequentially on successive calls.
	void* id,				// Node identifer as previously received from
								// announce_indexed_node
    const double& tpar,		//If the node lies on an edge,
								// then returns tpar, otherwise returns garbage.
	const SPApar_pos& uv,		// parametric coordinates
	const SPAposition& iX,		// cartesian coordinates
	const SPAunit_vector& N		// surface normal
	)
 {
	SPAUNUSED(E)
	SPAUNUSED(ipoly)
	SPAUNUSED(i)
	SPAUNUSED(id)
	SPAUNUSED(tpar)
	SPAUNUSED(uv)
	SPAUNUSED(iX)
	SPAUNUSED(N)
 }

	// This function is called to announce the end of mesh output for a face.
/**
 * The mesh is complete and updates polygon counts and fixes up parameters.
 * @param entity
 * faceted entity
 * @param app_ref
 * applicable refinement
 * @param format
 * output format entity
 */
	virtual void end_mesh_output(
		ENTITY *entity,				// entity being faceted
		ENTITY *app_ref,			// Current REFINEMENT
		ENTITY *format				// current output format (VERTEX_TEMPLATE)
	);

/**
 * This function is called to announce the end of mesh output for a face.
 * @param entity
 * faceted entity
 * @param app_ref
 * applicable refinement
 * @param format
 * output format entity
 */
 virtual void save_mesh_output(
		ENTITY *entity,				// entity being faceted
		ENTITY *app_ref,			// Current REFINEMENT
		ENTITY *format 			    // current output format (VERTEX_TEMPLATE)
	);

private:
 virtual logical need_internal_data(void * data);
};

/* @} */
#endif
