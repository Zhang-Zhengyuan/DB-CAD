/*******************************************************************/
/*    Copyright (c) 1989-2020 by Spatial Corp.                     */
/*    All rights reserved.                                         */
/*    Protected by U.S. Patents 5,257,205; 5,351,196; 6,369,815;   */
/*                              5,982,378; 6,462,738; 6,941,251    */
/*    Protected by European Patents 0503642; 69220263.3            */
/*    Protected by Hong Kong Patent 1008101A                       */
/*******************************************************************/
//    The INDEXED_MESH_MANAGER is a (non-global) indexed mesh manager.
/*******************************************************************/
#ifndef IDX_MM_H
#define IDX_MM_H
#include "dcl_fct.h"
#include "af_api.hxx"
#include "param.hxx"
#include "position.hxx"
#include "unitvec.hxx"
class INDEXED_MESH;
class SPAposition;
class SPAunit_vector;
class SPAtransf;
class VERTEX_TEMPLATE;

/**
 * @file idx_mm.hxx
 * @CAA2Level L1
 * @CAA2Usage U2
 *! \addtogroup FCTAPI
 *  \brief Declared at <idx_mm.hxx>
 *  @{
 */

 /**
  * This class provides an example of a mesh manager using the indexed protocol.
  * <br><br>
  * <b>Role :</b> INDEXED_MESH_MANAGER constructs <tt>INDEXED_MESH</tt>es from facet data
  * that the faceter announces through <tt>MESH_MANAGER</tt>.
  * @see INDEXED_MESH
  */
class DECL_FCT INDEXED_MESH_MANAGER : public MESH_MANAGER {

 protected:

/**
 * Pointer to the <tt>INDEXED_MESH</tt>.
 */
	INDEXED_MESH *m_pMesh;

/**
 * Number of polygons in the mesh.
 */
	int m_nNumPolygon;

/**
 * Number of polygon nodes in the mesh.
 */
	int m_nNumPolyNode;

/**
 * Number of vertices in the mesh.
 */
	int m_nNumVertex;

/**
 * Number of tri strips.
 */
	int NumStrips;
	
/**
 * Number of Single Triangles.
 */
	int NumSingleTris;
	
/**
 * @nodoc
 */
	unsigned DataSize;

/**
 * Transformation to apply to the nodes of the mesh.
 */
	const SPAtransf* m_pTransform;

/**
 * Vertex template defining data attached to node.
 */
	VERTEX_TEMPLATE* vertex_template;

/**
 * Function to print an error message.
 */
	void sequence_error(char *s);

/**
 * Storage for <tt>MESH_MANAGER_SEARCH_ORDER</tt>
 */
	MESH_MANAGER_SEARCH_ORDER mmso;  //SC mka 01-16-03 Added a member for storing search order. In order to be able to set it.

 public:

/**
 * C++ constructor, creates an <tt>INDEXED_MESH_MANAGER</tt>.
 */
 	INDEXED_MESH_MANAGER();

/**
 * C++ destructor, deletes an <tt>INDEXED_MESH_MANAGER</tt>.
 */
	virtual ~INDEXED_MESH_MANAGER();

/**
 * Returns the current mesh being constructed.
 */
	INDEXED_MESH* mesh() const { return m_pMesh; }

/**
 * Returns the total number of polygons in the mesh.
 */
	int NumPolygon() const { return m_nNumPolygon;}

/**
 * Returns the number of polygon nodes in the mesh.
 */
	int NumPolyNode() const { return m_nNumPolyNode;}

/**
 * Returns the number of vertices in the mesh.
 */
	int NumVertex() const { return m_nNumVertex;}

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
 * @nodoc
 */
	virtual unsigned GetDataSize() const { return DataSize;}

/**
 * @nodoc
 */
	virtual void AddDataSize(unsigned _DataSize) {DataSize += _DataSize;}

	// Save a transformation for the mesh. It will be applied
	// when the data is retrieved.
/**
 * Sets the transformation to be applied to nodes.
 * <br><br>
 * @param trans
 * the transformation.
 */
	void SetTransform(const SPAtransf* trans);

/**
 * Returns the order of how the internal mesh of the faceter should be searched to provide output.
 * <br><br>
 * <b>Role :</b> The order specifies how successive polygons ought to share
 * edges and can help the application to construct the desired data format.
 * The default is to search triangle fans, triangle strips and quad strips.
 */
 virtual MESH_MANAGER_SEARCH_ORDER query_search_order();

/**
 * Sets the order of how the internal mesh of the faceter should be searched to provide output.
 * <br><br>
 * <b>Role :</b> The order specifies how successive polygons ought to share
 * edges and can help the application to construct the desired data format.
 * The default is to search triangle fans, triangle strips and quad strips.
 */
 virtual void set_search_order(MESH_MANAGER_SEARCH_ORDER mmso);  //SC mka 01-16-03 Added set_search order method.

	// This function is called when a new face is about to be faceted.
/**
 * Announces the beginning of the output of a mesh.
 * <br><br>
 * <b>Role :</b> Cleans up old mesh if necessary.
 * <br><br>
 * @param entity
 * faceted entity.
 * @param app_ref
 * applicable refinement.
 * @param format
 * output format (must be a @href VERTEX_TEMPLATE ).
 */
 virtual void begin_mesh_output(
		ENTITY *entity,				// Entity about to be faceted
		ENTITY *app_ref,			// Current REFINEMENT
		ENTITY *format				// Current output format (VERTEX_TEMPLATE)
	);

	// This function is called to announce the number of polygons,
	// nodes, and references to nodes about to be output.
/**
 * Announces the number of polygons, nodes, and node references by polygons.
 * <br><br>
 * <b>Role :</b> Creates a mesh with the proper storage.
 * <br><br>
 * @param npoly
 * polygon count.
 * @param nnode
 * node count.
 * @param nref
 * node reference count.
 */
	virtual void announce_counts(
		int npoly,					// number of polygons to be output
		int nnode,					// number of nodes to be output
		int nref					// number of references to nodes
	);

	// Let the faceter know that this is a (non-global) index mesh manager.
/**
 * If this function returns TRUE, the indexed protocol is turned on.
 * <br><br>
 * <b>Role :</b> The default returns FALSE. Used with the indexed protocol to
 * flag requested data.
 */
	virtual logical need_indexed_polygons();

/**
 * This method specifies if the user wants duplicate nodes where the surface uv's differ at the same position on the surface.
 */
  virtual logical need_duplicate_indexed_nodes_on_surface_seams();

/**
 * Make this method return TRUE if multiple nodes are desired at singularities to get multiple UVs
 */
  virtual logical need_duplicate_indexed_nodes_at_singularities();

/**
 * Provides an approximate count of polygons and nodes and number of references to nodes.
 */
 virtual logical need_approx_counts() {return FALSE;}

	// This function is called to give the mesh manager the data for another node.
/**
 * Announces an indexed node.
 * <br><br>
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
		const SPApar_pos &uv,			// the uv parameters for this node
		const SPAposition &iX,			// the coordinates for this node
		const SPAunit_vector &N		// the normal at this node
	);

	// This function is called to announce the start of a new polygon
/**
 * A polygon starts and adds it to the mesh.
 * <br><br>
 * @param ipoly
 * polygon index.
 * @param npolynode
 * number of polygon node.
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
 * <b>Role:</b> This signature has a NULL implementation in the <tt>INDEXED_MESH_MANAGER</tt>.
 * <br><br>
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
 * <br><br>
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
 * <b>Role:</b> This signature has a NULL implementation in the <tt>INDEXED_MESH_MANAGER</tt>.
 * <br><br>
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

/**
 * This function is called to announce the end of mesh output for a face
 * <br><br>
 * @param entity
 * faceted entity.
 * @param app_ref
 * applicable refinement.
 * @param format.
 * output format entity.
 */
	virtual void end_mesh_output(
		ENTITY *entity,				// entity being faceted
		ENTITY *app_ref,			// Current REFINEMENT
		ENTITY *format				// current output format (VERTEX_TEMPLATE)
	);

/**
 * This function attaches the mesh to the entity thru ATTRIB_EYE_ATTACHED_MESH attribute.
 * <br><br>
 * @param entity
 * faceted entity.
 * @param app_ref
 * applicable refinement.
 * @param format
 * output format entity.
 */
 virtual void save_mesh_output(
		ENTITY *entity,				// entity being faceted
		ENTITY *app_ref,			// Current REFINEMENT
		ENTITY *format 			    // current output format (VERTEX_TEMPLATE)
	);

};

/* @} */
#endif
