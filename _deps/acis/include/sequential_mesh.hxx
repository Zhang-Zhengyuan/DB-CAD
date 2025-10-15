#ifndef sequential_mesh_hxx
#define sequential_mesh_hxx
/*******************************************************************/
/*    Copyright (c) 1989-2020 by Spatial Corp.                     */
/*    All rights reserved.                                         */
/*    Protected by U.S. Patents 5,257,205; 5,351,196; 6,369,815;   */
/*                              5,982,378; 6,462,738; 6,941,251    */
/*    Protected by European Patents 0503642; 69220263.3            */
/*    Protected by Hong Kong Patent 1008101A                       */
/*******************************************************************/
/**
 * @file sequential_mesh.hxx
 * @CAA2Level L1
 * @CAA2Usage U2
 *! \addtogroup FCTAPI
 *  \brief Declared at <sequential_mesh.hxx>
 *  @{
 */
//----------------------------------------------------------------------
#include "mmgr.hxx"
#include "dcl_fct.h"
#include "param.hxx"

class INDEXED_MESH;
class LINKED_MESH;
class AF_VU_NODE;
class seq_polygon;
class SEQUENTIAL_MESH_FRIEND;
class SPAposition;
class SPApar_pos;
class SPAunit_vector;

// SEQUENTIAL_MESH Handles
/**
 * Represents a series of vertices making a facet.
 * The vertices are represented by @href MESH_POLYNODE .
 * A <tt>MESH_POLYGON</tt> is a handle to be used just like a FILE handle
 * (created by fopen(), used by fprintf()).
 * In this way, <tt>MESH_POLYGON</tt> is created by @href INDEXED_MESH#get_first_polygon 
 * and may be passed other functions, such as @href INDEXED_MESH#get_next_polygon .
 */
class DECL_FCT MESH_POLYGON: public ACIS_OBJECT {
private:
	friend class SEQUENTIAL_MESH_FRIEND;
	friend class LINKED_MESH;
	union {
		struct {
			AF_VU_NODE *m_VUNode;
		} LM; // used in LINKED_MESH
		struct {
			int	m_PolygonIndex;	// index into m_pPolygon
		} IM; // used in INDEXED_MESH
	};
public:
	/*
	// tbrv
	*/
	/**
	 * @nodoc
	 */
	MESH_POLYGON() { LM.m_VUNode = 0; }
};

/**
 * The <tt>MESH_POLYNODE</tt> represents a point, or vertex, of one facet,
 * It is where the bulk of the information of the facets exist,
 * such as position, parametric parameters, and normal vector.
 * <tt>MESH_POLYNODE</tt>s are likely to be coincident with other <tt>MESH_POLYNODE</tt>s,
 * sharing the same position and normal.
 */
class DECL_FCT MESH_POLYNODE: public ACIS_OBJECT {
	friend class SEQUENTIAL_MESH_FRIEND;
	friend class LINKED_MESH;
private:
	union {
		struct {
			AF_VU_NODE *m_VUNode;
			char		m_Visit;
		} LM; // used in LINKED_MESH
		struct {
			int	m_PolygonIndex;	// index into m_pPolygon
			int	m_PolynodeIndex;	// index into indexed_polygon
		} IM; // used in INDEXED_MESH
	};
public:
	/*
	// tbrv
	*/
	/**
	 * @nodoc
	 */
	MESH_POLYNODE() { LM.m_VUNode = 0; LM.m_Visit = 0; }
};

/**
 * A <tt>MESH_NODE</tt> represents the point of coincident vertices of many facets (or one facet).
 * This is where one or more facets share a vertex.
 * The coincident vertices of facets that have the equal normal vectors can be represented by one <tt>MESH_NODE</tt>.
 * There are likely to be many <tt>MESH_POLYNODE</tt>s corresponding to one <tt>MESH_NODE</tt>.
 * At singularities coincident <tt>MESH_POLYNODE</tt>s will have
 * different normal vectors (such as the apex of a cone).
 * <tt>MESH_NODE</tt>s are useful when simulating <tt>INDEXED_MESH</tt> algorithms.
 */
class DECL_FCT MESH_NODE: public ACIS_OBJECT
 {
	friend class SEQUENTIAL_MESH_FRIEND;
	friend class LINKED_MESH;

private:
	union {
		struct {
			AF_VU_NODE *m_VUNode;
		} LM; // used in LINKED_MESH
		struct {
			int	m_VertexIndex;		// index into m_pVertex
		} IM; // used in INDEXED_MESH
	};
public:
	/*
	// tbrv
	*/
	/**
	 * @nodoc
	 */
	MESH_NODE() { LM.m_VUNode = 0; }
};

DECL_FCT logical operator==(MESH_POLYGON const &pn1, MESH_POLYGON const &pn2);
DECL_FCT logical operator!=(MESH_POLYGON const &pn1, MESH_POLYGON const &pn2);

DECL_FCT logical operator==(MESH_POLYNODE const &pn1, MESH_POLYNODE const &pn2);
DECL_FCT logical operator!=(MESH_POLYNODE const &pn1, MESH_POLYNODE const &pn2);

/**
 * Specifies the derived class of the base class @href SEQUENTIAL_MESH.
 * @param INDEXED_MESH_TYPE
 * @href INDEXED_MESH .
 * @param LINKED_MESH_TYPE
 * @href LINKED_MESH .
 * @param UNSPECIFIED_MESH_TYPE
 * creates a sequential mesh of unspecified type.
 */
typedef enum {
	INDEXED_MESH_TYPE,
	LINKED_MESH_TYPE,
	UNSPECIFIED_MESH_TYPE
} SEQUENTIAL_MESH_TYPES;

#include "af_serializable_mesh.hxx"
/**
 * A <tt>SEQUENTIAL_MESH</tt> is the base class for INDEXED_MESH and LINKED_MESH.
 * This provides a mechanism to write algorithms that can use either mesh.
 */
class DECL_FCT SEQUENTIAL_MESH : public af_serializable_mesh
{
public:
/**
 * C++ constructor, used by a MESH_MANAGER.
 */
	SEQUENTIAL_MESH() {}

/**
 * C++ destructor, deleting a SEQUENTIAL_MESH.
 */
	virtual ~SEQUENTIAL_MESH() {}

/**
 * Identifies this as a LINKED_MESH or a INDEXED_MESH.
 * <br><br>
 * <b>Role :</b> Used when you have a pointer to an instance of the base class
 * SEQUENTIAL_MESH. You may determine that the pointer is a LINKED_MESH or an
 * INDEXED_MESH, and cast the pointer appropriately
 * Identifies this as a LINKED_MESH, INDEXED_MESH, or anything else listed in @href SEQUENTIAL_MESH_TYPES .
 */
	virtual SEQUENTIAL_MESH_TYPES get_type_of_mesh() const = 0;

/**
 * Returns the number of polygons (facets). Count of MESH_POLYGONs. May be zero or more.
 */
	virtual int get_num_polygon() const = 0;

/**
 * Returns the number of points of a facets (count of MESH_POLYNODEs). May be zero or more.
 */
	virtual int	get_num_polynode() const = 0;

/**
 * Returns the total number of points of all facets (count of MESH_NODEs). May be zero or more.
 */
	virtual int get_num_node() const = 0;

// *****************************************************************************
// Routines for handling polygons
// *****************************************************************************

/**
 * Gets the first polygon (facet) in the mesh.
 * <br><br>
 * <b>Role :</b> Gets a valid @href MESH_POLYGON , the first polygon in the mesh.
 * Returns <tt>TRUE</tt> if there exists a mesh and a facet in the mesh.
 * Returns <tt>FALSE</tt> otherwise, and outPolygon will be invalid.
 * @param outPolygon
 * Output. A handle that will represent the first polygon of the mesh.
 */
	virtual logical	get_first_polygon(MESH_POLYGON& outPolygon) const = 0;

/**
 * Given a valid handle to a polygon, this gets the next polygon (facet) in the mesh.
 * <br><br>
 * <b>Role :</b> Given a valid @href MESH_POLYGON , get the next @href MESH_POLYGON .
 * This may be the next polygon in a strip or fan. See @href SEQUENTIAL_MESH#get_share_info .
 * This is circular. If, as input into the function, inoutPolygon is the last polygon,
 * then, as output, inoutPolygon will be the first polygon.
 * @param inoutPolygon
 * Returns <tt>TRUE</tt> if there exists a mesh and a facet in the mesh.
 * Returns <tt>FALSE</tt> otherwise.
 * On input, this is a valid handle of a polygon. 
 * On output, this will be the handle of the next polygon.
 */
	virtual logical	get_next_polygon(MESH_POLYGON& inoutPolygon) const = 0;

/**
 * Given a handle to a polynode of a polygon, this gets the next polygon (facet) in the mesh.
 * <br><br>
 * <b>Role :</b> Given a valid @href MESH_POLYNODE , get the next @href MESH_POLYGON .
 * This is circular. If the input is a polynode of the last polygon,
 * then outPolygon will be the first polygon of the mesh.
 * Returns <tt>TRUE</tt> if there exists a mesh and a facet in the mesh.
 * Returns <tt>FALSE</tt> otherwise.
 * @param inPolynode
 * Input. A valid handle of a polynode of a polygon.
 * @param outPolygon
 * Output. The handle of the next polygon.
 */
	virtual logical	get_next_polygon(MESH_POLYNODE& inPolynode, MESH_POLYGON& outPolygon) const = 0;

/**
 * Return the index of the shared edge between this polygon and the previous polygon.
 * <br><br>
 * <b>Role :</b> Return which edge of the previous polygon is shared with the first edge of this polygon.
 * When ishare is -1, it means that a new triangle (or quad) strip is created.<br>
 * In a triangle polygon, there are three facet edges: 0, 1 and 2.<br>
 * In a quad polygon, there are four facet edges: 0, 1, 2, and 3.<br>
 * An ishare value of 1 means this polygon shares facet edge 1 of the previous polygon.<br>
 * An ishare value of 2 means this polygon shares facet edge 2 of the previous polygon.<br>
 * The values of ishare for a triangle strip will be -1,1,2,1,2... until a negative number is encountered.<br>
 * The values of ishare for a quad strip will be -1,2,2,2,2... until a negative number is encountered.<br>
 * <br>
 * When ishare is -3 a new fan has started from the current triangle.<br>
 * From there, ishare values are 2,2,2 for the subsequent triangles in the fan.<br>
 * <br>
 * When ishare is -2, it means that no strips or fans is started, this polygon is not in any strip or fan.
 * @param inPolygon
 * Input. A valid handle of a polygon (facet) of a mesh.
 */
	virtual int get_share_info(MESH_POLYGON& inPolygon) const = 0;

/**
 * @nodoc.
 * Deprecated.  Do not use.
 *
 * This is intended to support older algorithms that were using an INDEXED_MESH.
 * <br><br>
 * <b>Role :</b> This returns a representaion of the polygon in the form of the call seq_polygon.
 * For an @href INDEXED_MESH , this is very quick.
 * For an @href LINKED_MESH , this is very slow.
 * Returns <tt>TRUE</tt> if successful.
 * Returns <tt>FALSE</tt> otherwise.
 * @param poly_index
 * Input. Index, from zero, of the polygon under inquiry.
 * @param polygon
 * Output. The polygon in the form of the class seq_polygon.
 @see seq_polygon, polygon_vertex
 */
	virtual logical get_seq_polygon(int poly_index, seq_polygon &polygon) = 0;

// *****************************************************************************
// Routines for handling polynodes
// *****************************************************************************

/**
 * Returns the number of vertices in a polygon (facet).
 * <br><br>
 * <b>Role :</b> Get the count of @href MESH_POLYNODE in a @href MESH_POLYGON .
 * @param inPolygon
 * Input. The polygon (facet) under inquiry.
 */
	virtual int	get_num_polynode(MESH_POLYGON& inPolygon) const = 0;

/**
 * Given a handle to a polygon, get the handle of the first polynode of that polygon.
 * <br><br>
 * <b>Role :</b> Gets the first vertex ( @href MESH_POLYNODE ) of a polygon ( @href MESH_POLYGON ).
 * Returns <tt>TRUE</tt> if inPolygon is valid.
 * Returns <tt>FALSE</tt> otherwise.
 * @param inPolygon
 * Input. A valid handle to a polygon (facet).
 * @param outPolynode
 * Output. A valid handle to a polynode (vertex of a facet).
 */
	virtual logical	get_first_polynode(MESH_POLYGON& inPolygon, MESH_POLYNODE& outPolynode) const = 0;

/**
 * Given a handle to a polynode, get the handle of the next polynode in that polygon.
 * <br><br>
 * <b>Role :</b> Gets the next vertex ( @href MESH_POLYNODE ) of a polygon ( @href MESH_POLYGON ).
 * This is circular. If the input argument is the last vertex, on output it will be the first vertex.
 * Returns <tt>TRUE</tt> if inPolynode is valid.
 * Returns <tt>FALSE</tt> otherwise.
 * @param inoutPolynode
 * Input/Output. A valid handle to a vertex of a polygon (facet).
 */
	virtual logical	get_next_polynode(MESH_POLYNODE& inoutPolynode) const = 0;

/**
 * Given a handle to a polynode, return the 3D position of the vertex.
 * <br><br>
 * <b>Role :</b> Gets the 3D position of a vertex ( @href MESH_POLYNODE ) of a polygon ( @href MESH_POLYGON ).
 * @param inPolynode
 * Input. A valid handle to a vertex of a polygon (facet).
 */
	virtual const SPAposition& get_position(MESH_POLYNODE& inPolynode) const = 0;

/**
 * Given a handle to a node, return the 3D position of the point.
 * <br><br>
 * <b>Role :</b> Gets the 3D position of a point ( @href MESH_NODE ).
 * This node can represent many MESH_POLYNODEs (of many MESH_POLYGONs).
 * @param inNode
 * Input. A valid handle to a node of a mesh.
 */
	virtual const SPAposition& get_position(MESH_NODE& inNode) const = 0;

/**
 * Given a handle to a polynode, return the normal vector of the vertex.
 * <br><br>
 * <b>Role :</b> Gets the normal vector of a vertex ( @href MESH_POLYNODE ) of a polygon ( @href MESH_POLYGON ).
 * @param inPolynode
 * Input. A valid handle to a vertex of a polygon (facet).
 */
	virtual const SPAunit_vector& get_normal(MESH_POLYNODE& inPolynode) const = 0;

/**
 * Given a handle to a node, return the normal vector of the point.
 * <br><br>
 * <b>Role :</b> Gets the normal vector of a point ( @href MESH_NODE ).
 * This node can represent many MESH_POLYNODEs (of many MESH_POLYGONs).
 * Since there may be many different normals at singularities, this will
 * return the first one it encounters.
 * @param inNode
 * Input. A valid handle to a node of a mesh.
 */
	virtual const SPAunit_vector& get_normal(MESH_NODE& inNode) const = 0;

/**
 * Given a handle to a polynode, return the 2D parametric position of the vertex.
 * <br><br>
 * <b>Role :</b> Gets the parametric position of a vertex ( @href MESH_POLYNODE ) of a polygon ( @href MESH_POLYGON ).
 * @param inPolynode
 * Input. A valid handle to a vertex of a polygon (facet).
 */
	virtual SPApar_pos get_par_pos( MESH_POLYNODE& inPolynode) const = 0;

/**
 * Given a handle to a node, return the 2D parametric position of the point.
 * <br><br>
 * <b>Role :</b> Gets the 2D parametric position of a point ( @href MESH_NODE ).
 * This node can represent many MESH_POLYNODEs (of many MESH_POLYGONs).
 * @param inNode
 * Input. A valid handle to a node of a mesh.
 */
	virtual SPApar_pos get_par_pos( MESH_NODE& inNode) const = 0;

/**
 * Get the first node node (point of coincident vertices) of a mesh.
 * <br><br>
 * <b>Role :</b> Gets the handle representing the first vertex in the mesh.
 * This is where one or more facets share a vertex.
 * This node can represent many MESH_POLYNODEs (of many MESH_POLYGONs).
 * Returns <tt>TRUE</tt> if there exists a mesh and a facet in the mesh.
 * Returns <tt>FALSE</tt> otherwise.
 * @param inoutNode
 * Output. A valid handle to a node of a mesh.
 */
	virtual logical	get_first_node(MESH_NODE& outNode) const = 0;

/**
 * Given a handle to a node, return the next node (point of coincident vertices) in the mesh.
 * <br><br>
 * <b>Role :</b> Gets the handle representing the next vertex in the mesh.
 * This is where one or more facets share a vertex.
 * This node can represent many MESH_POLYNODEs (of many MESH_POLYGONs).
 * This is circular. The last node will return the first node.
 * Returns <tt>TRUE</tt> if there exists a mesh and a facet in the mesh.
 * Returns <tt>FALSE</tt> otherwise.
 * @param inoutNode
 * On input, a valid handle to a node of a mesh.
 * On output, another node in the mesh.
 */
	virtual logical	get_next_node(MESH_NODE& inoutNode) const = 0;

/**
 * Given a handle to a polynode, return the index of the node (point of coincident vertices) in the mesh.
 * <br><br>
 * <b>Role :</b> This provides a means of supporting algorithms build for an INDEXED_MESH.
 * One may have traversed all MESH_NODEs, storing info sequentially into an array.
 * While traversing the vertices of a facet, one may quickly access the index of 
 * the array using this method.
 * There may be many MESH_POLYNODEs to one MESH_NODE.
 * @param inPolynode
 * Input. A valid handle to a vertex of a polygon (facet).
 */
	virtual int get_node_index(MESH_POLYNODE& inPolynode) const = 0;

/**
 * Given a handle to a file, and a transform, fill the file with facet information.
 * <br><br>
 * <b>Role :</b> 
 * This writes to the file all of the nodes, then all of the polygons with references
 * to the indices of the nodes.
 * The transform that you would most likely want to use would be the transform attached to the body.
 * @param inFilePtr
 * Input. A valid handle to a file opened for writing.
 * @param inTransform
 * Input. A transform that will be performed on every vertex.
 */
	virtual void write(FILE* inFilePtr, const SPAtransf* inTransform = NULL) = 0;

/**
 * @nodoc
 */
	virtual void write_raw(FILE* fp, const SPAtransf* trans = NULL) = 0;

/**
 * Change the parametric positions to be mapped from zero to one.
 * <br><br>
 * <b>Role :</b> For applications like texture mapping, it is useful to convert the
 * parametric values to a range between zero and one. This changes the values in the mesh.
 * @param onoff
 * Input. Equals TRUE if you want parametric positions mapped from zero to one.
 * Equals FALSE if you want parametric values to reflect true values found on the surface of the FACE.
 */
	virtual void set_par_pos_mapping_01(logical onoff) = 0;

/**
 * Return whether the mapping of the parametric postions have been convert to a range from zero to one..
 * <br><br>
 * <b>Role :</b> For applications like texture mapping, it is useful to convert the
 * parametric values to a range between zero and one. This informs you whether the values in the mesh were converted
 */
	virtual logical get_par_pos_mapping_01() const = 0;

/**
 * Convert the parametric positions, mapped from zero to one.
 * <br><br>
 * <b>Role :</b> For applications like texture mapping, it is useful to convert the
 * parametric values to a range between zero and one. This will get the correct value,
 * regardless of the value of get_par_pos_mapping_01(). This is expecting a parametric
 * position retrieved from the function SEQUENTIAL_MESH::get_par_pos(), or 
 * some operation on a number of positions retrieved from that function, such as
 * a midpoint or a centroid.
 * @param inUV
 * Input. A parametric position retrieved from the function get_par_pos()
 * @param outUV
 *
 */
	virtual void get_par_pos_mapped_01(const SPApar_pos& inUV, SPApar_pos& outUV) const = 0;
	
/**
 * Convert the parametric positions, not mapped from zero to one, but reflecting true values on the surface.
 * <br><br>
 * <b>Role :</b> For applications like texture mapping, it is useful to convert the
 * parametric values to a range between zero and one. This will get the correct value,
 * regardless of the value of get_par_pos_mapping_01(). This is expecting a parametric
 * position retrieved from the function SEQUENTIAL_MESH::get_par_pos(), or 
 * some operation on a number of positions retrieved from that function, such as
 * a midpoint or a centroid.
 * @param inUV
 * Input. A parametric position retrieved from the function get_par_pos()
 * @param outUV
 *
 */
	virtual void get_par_pos_surface(const SPApar_pos& inUV, SPApar_pos& outUV) const = 0;

	/**
	* @nodoc
	**/
	virtual int number_of_polygon_coedges() const;
	/**
	* returns the number of vertices (i.e., corners of triangles in the mesh).
	**/
	virtual int number_of_vertices() const;
	/**
	* returns the number of polygons in the mesh.
	**/
	virtual int number_of_polygons() const;

	/**
	* Writes the coordinates of the mesh vertices into an array of doubles.
	* the array to write into must be at least 3*number_of_vertices() in size.
	**/
	virtual void serialize_positions( double* out_coords ) const;
	/**
	* Writes the coordinates of the mesh vertices into an array of floats.
	* the array to write into must be at least 3*number_of_vertices() in size.
	*
	* Default implementation calls serialize_positions(double*), then iterates
	* over the array and truncates the coords.
	**/
	virtual void serialize_positions( float* out_coords ) const;

	/**
	* Writes the coordinates of the normal vectors at the mesh vertices into 
	* an array of doubles.  The array needs at least 3*number_of_vertices() doubles in it.
	**/ 
	virtual void serialize_normals(  double* out_normal_coords ) const;
	/**
	* Writes the coordinates of the normal vectors at the mesh vertices into 
	* an array of doubles.  The array needs at least 3*number_of_vertices() doubles in it.
	*
	* Default implementation calls serialize_normals(double*) then truncates each coordinate.
	**/ 
	virtual void serialize_normals(  float* out_normal_coords ) const;

	/**
	* Fills the array of integers with a series of blocks of three indices into the vert
	**/
	virtual int serialize_triangles( int* out_triangle_indices ) const;

	/**
	* Fills the array out_hoops_connectivity_array into a form suitable for the function HA_Insert_Shell.
	**/
	virtual void serialize_polygons_for_hoops( int* out_hoops_connectivity_array ) const;


	/**
	* fills <tt>out_uv_buffer</tt> with the surface par pos data for the vertices.
	* <tt>out_uv_buffer</tt> must have size at least 2*number_of_vertices().
	**/
	virtual void serialize_uv_data( float* out_uv_buffer, logical normalize_uv ) const ;

	/**
	* fills <tt>out_uv_buffer</tt> with the surface par pos data for the vertices.
	* <tt>out_uv_buffer</tt> must have size at least 2*number_of_vertices().
	**/
	virtual void serialize_uv_data( double* out_uv_buffer, logical normalize_uv ) const ;


	/**
	* @nodoc
	**/
	double m_surf_tol_target;
};


/* @} */
#endif
