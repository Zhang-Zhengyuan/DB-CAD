/*******************************************************************/
/*    Copyright (c) 1989-2020 by Spatial Corp.                     */
/*    All rights reserved.                                         */
/*    Protected by U.S. Patents 5,257,205; 5,351,196; 6,369,815;   */
/*                              5,982,378; 6,462,738; 6,941,251    */
/*    Protected by European Patents 0503642; 69220263.3            */
/*    Protected by Hong Kong Patent 1008101A                       */
/*******************************************************************/
#ifndef LINKEDMESH_HXX
#define LINKEDMESH_HXX
/**
 * @file LinkedMesh.hxx
 * @CAA2Level L1
 * @CAA2Usage U2
 *! \addtogroup FCTAPI
 *  \brief Declared at <LinkedMesh.hxx>
 *  @{
 */

#include "dcl_fct.h"
#include "af_api.hxx"
#include "nodedata.hxx"
#include "idx_mesh.hxx"

// ACIS includes---
#include "position.hxx"
#include "unitvec.hxx"
#include "param.hxx"

class AF_VU_NODE;
class AF_VU_SET;
class SPAbox;
class COEDGE;

/**
 * A <tt>LINKED_MESH</tt> is a series of points connected by links in a number of ways,
 * in order to facilitate fast processing while creating the facets and using the facets.
 * The entire list of unique points can be retrieved using get_first_node() and get_node().
 * Facets are connected by links and can be followed using get_first_polygon() and get_next_polygon.
 * The order is by triangular strip or fan.
 * <br><br>
 * Points of a facet are connected counter-clockwise and can be followed using get_first_polynode() 
 * and get_next_polynode().
 * <br><br>
 * Each of these member functions may have one or more handles passed to them.
 * <br><br>
 * These handles can be filled with data only by memeber functions of <tt>LINKED_MESH</tt>.
 * @see SEQUENTIAL_MESH, MESH_POLYGON, MESH_POLYNODE, MESH_NODE
 */
class DECL_FCT LINKED_MESH : public SEQUENTIAL_MESH
{
public:
/**
 * C++ constructor, creating a <tt>LINKED_MESH</tt> using the specified parameters.
 * This can only be created by the LINKED_MESH_MANAGER class.
 * Users may receive a pointer to an existing LINKED_MESH ony by certain functions of the 
 * faceter component,
 * such as GetSequentialMesh() and SEQUENTIAL_MESH::get_type_of_mesh().
 * <br><br>
 * @param vu_set
 * A set of AF_VU_NODES, a private class of the faceter.
 * @param polygonCount
 * The number of polygons (or facets) for this mesh.
 * @param polynodeCount
 * The total number of points over all polygons (facets).
 * @param nodeCount
 * The total number of unique points in the mesh (facets).
 */
	LINKED_MESH(AF_VU_SET* vu_set, int polygonCount, int polynodeCount, int nodeCount);

	virtual ~LINKED_MESH();

/**
 * @nodoc
 */
	LINKED_MESH(AF_VU_SET* vu_set, int polygonCount, int polynodeCount, int nodeCount,
						 double surf_tol_target);
/**
 * @nodoc
 */
	LINKED_MESH(const LINKED_MESH& from);

/**
 * @nodoc
 */
	void init(AF_VU_SET* vu_set, int polygonCount, int polynodeCount, int nodeCount);

/**
 * Identifies this as a LINKED_MESH.
 * <br><br>
 * <b>Role:</b> See @href SEQUENTIAL_MESH and its member function get_type_of_mesh().
 */
	SEQUENTIAL_MESH_TYPES get_type_of_mesh() const { return LINKED_MESH_TYPE; }

/**
 * @see SEQUENTIAL_MESH#get_num_polygon
 */
	int		get_num_polygon() const { return m_PolygonCount; }

/**
 * @see SEQUENTIAL_MESH#get_num_polynode
 */
	int		get_num_polynode() const;

/**
 * @see SEQUENTIAL_MESH#get_num_node.
 */
	int		get_num_node() const;

// ****************************************************************************
// Routines for handling polygons.
// ****************************************************************************

/**
 * @see SEQUENTIAL_MESH#get_first_polygon
 */
	logical	get_first_polygon(
					MESH_POLYGON& outPolygon) const;
/**
 * @see SEQUENTIAL_MESH#get_next_polygon
 */
	logical	get_next_polygon(
					MESH_POLYGON& inoutPolygon) const;
/**
 * @see SEQUENTIAL_MESH#get_next_polygon
 */
	logical	get_next_polygon(
					MESH_POLYNODE& inPolynode,
					MESH_POLYGON& outPolygon) const;
/**
 * @see SEQUENTIAL_MESH#get_share_info
 */
	int		get_share_info(MESH_POLYGON& inPolygon) const;

// ****************************************************************************
// Routines for handling polynodes.
// ****************************************************************************

/**
 * @see SEQUENTIAL_MESH#get_num_polynode
 */
	int		get_num_polynode(
					MESH_POLYGON& inPolygon) const;
/**
 * @see SEQUENTIAL_MESH#get_first_polynode
 */
	logical	get_first_polynode(
					MESH_POLYGON& inPolygon,
					MESH_POLYNODE& outPolynode) const;
/**
 * @see SEQUENTIAL_MESH#get_next_polynode
 */
	logical	get_next_polynode(	// within a facet in a counter-clockwise direction
					MESH_POLYNODE& inoutPolynode) const;
/**
 * @see SEQUENTIAL_MESH#get_polygon
 */
	logical get_polygon(
					MESH_POLYNODE& inPolynode,
					MESH_POLYGON& outPolygon);
/**
 * @see SEQUENTIAL_MESH#get_position
 */
	const SPAposition& get_position(MESH_POLYNODE& inPolynode) const;
/**
 * @see SEQUENTIAL_MESH#get_position
 */
	const SPAposition& get_position(MESH_NODE& inNode) const;
/**
 * @see SEQUENTIAL_MESH#get_normal
 */
	const SPAunit_vector& get_normal(MESH_POLYNODE& inPolynode) const;
/**
 * @see SEQUENTIAL_MESH#get_normal
 */
	const SPAunit_vector& get_normal(MESH_NODE& inNode) const;
/**
 * @see SEQUENTIAL_MESH#get_par_pos
 */
	SPApar_pos get_par_pos(MESH_POLYNODE& inPolynode) const;
/**
 * @see SEQUENTIAL_MESH#get_par_pos
 */
	SPApar_pos get_par_pos(MESH_NODE& inNode) const;
/**
 * @nodoc
 */
	// min edge length is the shortest allowed length of a facet edge.
	double get_min_edge_length() const;
/**
 * @nodoc
 */
// for internal use only.  only has an effect if called during faceting.
	void set_min_edge_length( double new_min_edge_length );
/*
// tbrv
*/
/**
 * @nodoc
 */
	logical	get_prev_polynode(
					MESH_POLYNODE& inoutPolynode) const;

/*
// tbrv
*/
/**
 * @nodoc
 */
	logical get_next_polynode_CCW_around_node(
					MESH_POLYNODE&	inPolynode,
					MESH_POLYNODE&	outPolynode,
					FACE**			outFace1 = 0,
					LINKED_MESH**	outMesh1 = 0,
					MESH_POLYNODE*	outMeshPolynode1 = 0,
					COEDGE**		outCoedge1 = 0,
					FACE**			outFace2 = 0,
					LINKED_MESH**	outMesh2 = 0,
					MESH_POLYNODE*	outMeshPolynode2 = 0,
					COEDGE**		outCoedge2 = 0);
	
/*
// tbrv
*/
/**
 * @nodoc
 */
	void	clear_marks(void);
	
/*
// tbrv
*/
/**
 * @nodoc
 */
	void	set_mark(
					MESH_POLYNODE& inPolynode);
	
/*
// tbrv
*/
/**
 * @nodoc
 */
	void	clear_mark(
					MESH_POLYNODE& inPolynode);
	
/*
// tbrv
*/
/**
 * @nodoc
 */
	logical get_mark(
					MESH_POLYNODE& inPolynode);

// ****************************************************************************
// Routines for handling unique nodes. One node may coincide with many polynodes
// ****************************************************************************
/**
 * @see SEQUENTIAL_MESH#get_first_node
 */
	logical	get_first_node(MESH_NODE& outNode) const;
/**
 * @see SEQUENTIAL_MESH#get_next_node
 */
	logical	get_next_node(MESH_NODE& inoutNode) const;
/*
// tbrv
*/
/**
 * @nodoc
 */
	logical get_a_polynode(MESH_NODE& inNode, MESH_POLYNODE& outPolynode);
			// par_pos at a node can be ambiguous at singularities.

// ****************************************************************************
// Routines involving the whole mesh...
// ****************************************************************************
/**
 * @nodoc
 */
	SPAbox get_box();
/**
 * @nodoc
 */
	void write(FILE* fp, const SPAtransf* trans = NULL);
/**
 * @nodoc
 */
	void write_raw(FILE* fp, const SPAtransf* trans = NULL);

// ****************************************************************************
// Routines that support algorithms written for INDEXED_MESH
// ****************************************************************************
/**
 * @nodoc
 */
	logical get_seq_polygon(int inPolygonIndex, seq_polygon& outIndexedPolygon);
/**
 * @nodoc
 */
	int		get_node_index(MESH_POLYNODE& inPolynode) const;

// ****************************************************************************
// Routines dealing with mapping UV values from 0 to 1...
// ****************************************************************************

/**
 * @nodoc
 */
	void set_par_pos_mapping_01(logical onoff);
/**
 * @nodoc
 */
	logical get_par_pos_mapping_01() const;
/**
 * @nodoc
 */
	void get_par_pos_mapped_01(const SPApar_pos& inUV, SPApar_pos& outUV) const;
/**
 * @nodoc
 */
	void get_par_pos_surface(const SPApar_pos& inUV, SPApar_pos& outUV) const;

/**
 * @nodoc
 */
	logical transform(SPAtransf const& inTransform);

/**
 * @nodoc
 */
	COEDGE * get_coedge(MESH_POLYNODE& inNode) const;

private: // data
	AF_VU_SET*			m_VUSet;
	int					m_PolygonCount;
	int					m_TotalPolynodeCount;
	int					m_NodeCount;
	double				m_UMin;
	double				m_UMax;
	double				m_VMin;
	double				m_VMax;
	logical				m_lScaled;
	double				m_min_edge_length;

private: // functions
	LINKED_MESH(); // Undefined! Don't let anyone use a null constructor
	AF_VU_NODE* get_first_AF_VU_NODE() const;
	AF_VU_NODE* get_next_AF_VU_NODE_in_facet(
						AF_VU_NODE* inVUNode) const;
	AF_VU_NODE* get_prev_AF_VU_NODE_in_facet(
						AF_VU_NODE* inVUNode) const;
	AF_VU_NODE* get_next_AF_VU_NODE_in_set(
						AF_VU_NODE* inVUNode,
						logical* newPolygon = 0) const;
	AF_VU_NODE* get_next_AF_VU_NODE(
						AF_VU_NODE* inVUNode,
						char* inoutVisit,
						logical* newPolygon = 0) const;
	AF_VU_NODE* get_prev_AF_VU_NODE(
						AF_VU_NODE* inVUNode,
						char* inoutVisit) const;
	void		make_polynode(
						AF_VU_NODE* inVUNode,
						MESH_POLYNODE& outPolynode);
	void		get_border_info(
						AF_VU_NODE*		inExtVUNode,
						FACE**			outFace1,
						LINKED_MESH**	outMesh1,
						AF_VU_NODE**	outExtVUNode1,
						COEDGE**		outCoedge1,
						FACE**			outFace2,
						LINKED_MESH**	outMesh2,
						AF_VU_NODE**	outExtVUNode2,
						COEDGE**		outCoedge2);
	void		get_mate(
						COEDGE*			inCoedge,
						const SPAposition & inPos,
						FACE**			outAdjFace,
						LINKED_MESH**	outAdjMesh,
						AF_VU_NODE**	outAdjExtVUNode,
						logical			inUseFirstVUNode = TRUE);
	AF_VU_NODE* find_next_VUNode_around_vertex(
						AF_VU_NODE* inVUNode,
						AF_VU_NODE** inExtVUNode = 0);
	/**
	* @nodoc
	**/
	// internal use only
	friend class DECL_FCT linked_mesh_private_interface;
};

/* @} */
#endif
