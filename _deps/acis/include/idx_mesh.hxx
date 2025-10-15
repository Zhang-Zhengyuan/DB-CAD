/*******************************************************************/
/*    Copyright (c) 1989-2020 by Spatial Corp.                     */
/*    All rights reserved.                                         */
/*    Protected by U.S. Patents 5,257,205; 5,351,196; 6,369,815;   */
/*                              5,982,378; 6,462,738; 6,941,251    */
/*    Protected by European Patents 0503642; 69220263.3            */
/*    Protected by Hong Kong Patent 1008101A                       */
/*******************************************************************/
//    Declare class INDEXED_MESH
/*******************************************************************/
//  An INDEXED_MESH is a mesh format that minimizes memory by
//	placing its data in contiguous arrays.  This should also
//  give fast access to the data by minimizing the number of
//  allocations required, and by minimizing fragmentation.

//  The disadvantage of the INDEXED_MESH is that all of the memory
//  is allocated up front, so you must know how many polygons
//  and vertices will be created when you create the mesh.  You
//  also cannot go back and add new vertices and polygons.

// This class is an appropriate appropriate mesh storage format	if
//	(a) reducing memory fragmentation is a priority
//	(b) all mesh data is available for one-time construction
//	(c) no subsequent editing (other than moving vertices) is to be performed.
//
// Error handling conventions:
//
//	(a) all ipoly and inode indices are checked and adjusted to 0
//		if out of bounds.
//	(b) the SPAposition and normal of node 0 are pre-initialized to
//		(0,0,0) so that references to them can be returned even if
//		no nodes are actually defined.
//
//----------------------------------------------------------------------
#ifndef IDX_MESH_HXX
#define IDX_MESH_HXX
#include "dcl_fct.h"
#include "af_api.hxx"
#include "nodedata.hxx"
#include "position.hxx"
#include "unitvec.hxx"
#include "param.hxx"
#include "poly_vtx.hxx"
class SPAbox;
class SPAtransf;
class VERTEX_TEMPLATE;
/**
 * @file idx_mesh.hxx
 * @CAA2Level L1
 * @CAA2Usage U2
 *! \addtogroup FCTAPI
 *  \brief Declared at <idx_mesh.hxx>
 *  @{
 */
/**
 * Defines the application identification for the INDEXED_MESH facet data
 * in the ATTRIB_EYE_ATTACHED_MESH attribute attached to a FACE. The only
 * other MESH and MESH_MANAGER that can use this ID is LINKED_MESH and LINKED_MESH_MANAGER.
 */
#define IDX_MESH_APP 0x00494458

/**
 * Defines the user identification for the INDEXED_MESH facet data
 * in the ATTRIB_EYE_ATTACHED_MESH attribute attached to a FACE. Currently
 * only one INDEXED_MESH can be attached to a FACE.
 */
#define IDX_MESH_ID 0x00494458


//----------------------------------------------------------------------
// The mesh_polygon class defines an interface to a polygon in a mesh.
/**
 * Defines an interface to a polygon in a mesh.
 */
class DECL_FCT mesh_polygon : public ACIS_OBJECT
{
public:
/**
 * C++ constructor, creating a <tt>mesh_polygon</tt>.
 */
	mesh_polygon() {}

/**
 * C++ destructor, deleting a <tt>mesh_polygon</tt>.
 */
	virtual ~mesh_polygon() {}

	// Get the number of vertices of the polygon

/**
 * Get the number of vertices of the polygon.
 */
	virtual int num_vertex() const = 0;

	// Get a vertex based on its index (zero based)
/**
 * Get a vertex based on its index (zero based).
 * <br><br>
 * @param i
 * index value.
 */
	virtual polygon_vertex* get_vertex(int i) const = 0;

	// Reverse the order of the vertices (this does not reverse the
	// normal directions of the vertices).

/**
 * Reverse the order of the vertices (this does not reverse the normal directions of the vertices).
 */
	virtual void reverse_vertices() = 0;
};


//----------------------------------------------------------------------
// The indexed_polygon class is a derived class of mesh_polygon
// which stores the polygons in an INDEXED_MESH.
/**
 * @nodoc
 * Obsolete: used only in pre-1-7 Faceting.
 * <br><br>
 * <b>Role :</b> The indexed_polygon class is a derived class of <tt>mesh_polygon</tt> that
 * stores the polygons in an <tt>INDEXED_MESH</tt>.
 * @see VERTEX_TEMPLATE, polygon_vertex, INDEXED_MESH.
 */
class DECL_FCT indexed_polygon : public mesh_polygon
{
protected:
	int m_nNumVertex;					// number of vertices
	polygon_vertex** m_pVertexPtrs;		// pointer into an array which
										// contains pointers to the
										// vertices for this polygon.
										// The vertices themselves are
										// stored in a separate array
	VERTEX_TEMPLATE* vertex_template;	// vertex template defining data
										// stored at each node of the polygon
	int ishare;							// Which edge from prev is shared with
										//this polygon's first edge for proper
										// opengl tristrip and other order

public:
	// Create a polygon.  These will all be allocated at once
	// in an array, so there is a separate procedure to set the
	// data later on.
/**
 * C++ constructor, creating an <tt>indexed_polygon</tt>.
 * <br><br>
 * <b>Role :</b> These are allocated all at once in an array. There is a separate
 * procedure to initialize the data later on.
 */
	indexed_polygon();

	// Nothing to do in the destructor, because the mesh is assumed
	// to own the vertex data
//STI mka
	//virtual ~indexed_polygon() {vertex_template->remove();}

/**
 * C++ destructor, deleting an <tt>indexed_polygon</tt>.
 */
	virtual ~indexed_polygon() {}

	// Set the data for the polygon

/**
 * Set the data.
 * <br><br>
 * @param num_vertex
 * number of vertices.
 * @param verts
 * verts.
 * @param ishare_in
 * info about which edge of previous polygon is shared with this one. Used for traingle strips and 
 * and triangle fans.
 */
	virtual void set_data(int num_vertex, polygon_vertex** verts, int ishare_in=-2);

	// Get the number of vertices of the polygon

/**
 * Get the number of vertices of the polygon.
 */
	virtual int num_vertex() const { return m_nNumVertex; }

	// Get a vertex based on its index (zero based)

/**
 * Get a vertex based on its index (zero based).
 * <br><br>
 * @param i
 * integer.
 */
	virtual polygon_vertex* get_vertex(int i) const;

	// Reverse the order of the vertices (this does not reverse the
	// normal directions of the vertices).

/**
 * Reverse the vertices.
 */
	virtual void reverse_vertices();

	// Get array of vertex pointers

/**
 * Get vertices.
 */
	polygon_vertex** get_vertices() const { return m_pVertexPtrs; };

	// Set a vertex pointer
/**
 * Set a vertex pointer.
 * <br><br>
 * @param vertex_num
 * vertex number.
 * @param vertex
 * vertex.
 */
	int set_vertex(int vertex_num, polygon_vertex* vertex);

	// Set vertex template
/**
 * Get the vertex template for the polygon.
 * <br><br>
 * @param _vertex_template
 * pointer to class template to point to.
 */
 void set_vertex_template(VERTEX_TEMPLATE* _vertex_template){
       if (vertex_template)
			vertex_template->remove();
       if (_vertex_template){
	   		_vertex_template->share();
	   		vertex_template = _vertex_template;
       }
       else
            vertex_template = NULL;
    };


/**
 * @nodoc
 */
 void add(){
   	  if (vertex_template)
		 vertex_template->add();};


/**
 * @nodoc
 */
   void remove_ref(logical lose_if_zero){ if (vertex_template)
                      vertex_template->remove(lose_if_zero);};

	// Get vertex template
/**
 * Get the vertex template for the polygon.
 */
	VERTEX_TEMPLATE* get_vertex_template() const { return vertex_template; };

	//Get share info
/**
 * Gets the share information from the indexed polygon.
 */
	int get_share_info() const {return ishare;};
	//Set share info
/**
 * Specifies the shared information.
 * <br><br>
 * @param i
 * index number.
 */
	void set_share_info(int i) {ishare = i;};
};

//----------------------------------------------------------------------
class AF_VU_NODE;


// Represents one facet in a form used by the INDEXED_MESH.
/*
// tbrv
*/
/**
 * @nodoc
 */
class DECL_FCT seq_polygon : public indexed_polygon
{
public:
	seq_polygon();
	virtual ~seq_polygon();
	AF_VU_NODE* m_Polynode0;
	polygon_vertex* m_PolygonVertices;
	polygon_vertex**m_PolygonVertPtrs;
	int				m_PolygonVertCount;
};

#include "sequential_mesh.hxx"

//----------------------------------------------------------------------

// <tt>INEXED_MESH</tt> class.  Internal structures as defined above.

/**
 * Shows an example of a mesh that stores indices to a vertex array.
 * <br><br>
 * <b>Role :</b> An <tt>INDEXED_MESH</tt> is a mesh format that minimizes memory places
 * its data in contiguous arrays, presumably minimizing memory fragmentation.
 * The indexed mesh is stored as an array of nodes and a set of polygons defined
 * by (numerical) indices into the node set. This is a "packed" representation in
 * which both polygons and nodes are referenced via integer that index into arrays.
 * <br><br>
 * The disadvantage of the <tt>INDEXED_MESH</tt> is that all of the memory is allocated
 * up front, so you must know how many polygons and vertices will be created when
 * you create the mesh. You also cannot go back and add new vertices and polygons.
 * <br><br>
 * This class represents an alternate version of <tt>SIMPLE_INDEXED_MESH</tt>.
 * @see indexed_polygon, polygon_vertex, INDEXED_MESH_MANAGER
 */
class DECL_FCT INDEXED_MESH : public SEQUENTIAL_MESH
{
protected:

	// These counts are upper-limits passed at constructor time.  These
	// are used to allocate the arrays.  The actual numbers may actually
	// be less than this (although hopefully not a lot less)
	logical m_lScaled; // True  = uv scaled into 0 to
	                   // FALSE = uv left as entered
	int m_nMaxVertex;	// maximum number of vertices
	int m_nMaxPolygon;	// maximum number of polygons
	int m_nMaxPolynode;	// maximum number of vertex references in all polygons

	// Actual counts.
	int m_nVertex;		// number of vertices
	int m_nPolygon;		// number of polygons
	int m_nPolynode;	// number of polygon vertex references

	// Array of all of the vertices
	polygon_vertex* m_pVertex;

	// Array of pointers into the vertex array.  The polygons will
	// contain pointers into this array.  This is allocated all
	// at once (as opposed to having each polygon allocate its
	// own array) to minimize memory fragmentation
	polygon_vertex** m_pVertexPtrs;

	// Array of polygons
	indexed_polygon* m_pPolygon;

	// Keep track of the SPAparameter ranges of UV values so that we can
	// map them into the range [0, 1]
	double m_UMin;
	double m_UMax;
	double m_VMin;
	double m_VMax;
    logical remove_vertex_template;
	unsigned DataSize;
	logical reverse_flag;

public:

/**
 * C++ constructor, creating an <tt>INDEXED_MESH</tt> using the specified parameters.
 * <br><br>
 * @param max_vertex
 * maximum vertex.
 * @param max_poly
 * maximum polygon.
 * @param max_polynode
 * maximum polynode.
 */
	INDEXED_MESH(int max_vertex, int max_poly, int max_polynode);
/**
 * C++ constructor, creating an <tt>INDEXED_MESH</tt> by copying one.
 * <br><br>
 * @param imesh
 * indexed mesh
 */
	INDEXED_MESH(const INDEXED_MESH& imesh);

/**
 * C++ destructor, deleting an INDEXED_MESH.
 */
	~INDEXED_MESH();


/**
 * @nodoc
 */
    void remove_ref(logical lose_if_zero);


/**
 * @nodoc
 */
   	void add();


/**
 * @nodoc
 */
    void set_remove_vertex_template(logical);

/**
 * @nodoc
 */
	virtual unsigned GetDataSize() const {return DataSize;}


	//Copy the contents of an <tt>INDEXED_MESH</tt> into
	//this - has to be an empty mesh, with sufficient space
/**
 * Copy the contents of an <tt>INDEXED_MESH</tt> into
 * this - has to be an empty mesh, with sufficient space
 * <br><br>
 * @param imesh
 * indexed mesh.
 */
	INDEXED_MESH& operator|=(const INDEXED_MESH& imesh);

	// Get the total number of vertices
/**
 * Returns the number of vertices on mesh.
 */
	int get_num_vertex() const { return m_nVertex; }

	// Get SPAposition or normal given a vertex number
/**
 * Gets the pointer to a polygon.
 * <br><br>
 * @param inode
 * index to node.
 */
	const SPAposition &get_position(int inode) const;
/**
 * Gets the normal at a node.
 * <br><br>
 * @param inode
 * index to node.
 */
	const SPAunit_vector &get_normal(int inode) const;

/**
 * Gets the vertex at the node.
 * <br><br>
 * @param inode
 * index to node.
 */
	polygon_vertex& get_vertex(int inode) const;

	// Given a pointer to a vertex, get its index in the vertex array.

/**
 * Gets the index of a polygon vertex.
 * <br><br>
 * @param polver
 * polygon vertex.
 */
	int get_vertex_index(const polygon_vertex* polver) const;

	// Get the number of polygons
/**
 * Returns the number of polygon on mesh.
 */
	int get_num_polygon() const { return m_nPolygon; }

	// Get a polygon
/**
 * Gets the pointer to a polygon.
 * <br><br>
 * @param poly_index
 * index to polygon.
 */
	indexed_polygon* get_polygon(int poly_index) const;

	// Add a new vertex
/**
 * Adds a vertex to the current polygon.
 * <br><br>
 * @param pos
 * position of vertex.
 * @param uv
 * normal at vertex.
 * @param parpos
 * parameter position.
 */
	polygon_vertex* add_vertex( const SPAposition& pos,
								const SPAunit_vector& uv,
								const SPApar_pos& parpos);

/**
 * Adds a vertex to the current polygon.
 * <br><br>
 * @param polver
 * vertex to add.
 */
	polygon_vertex* add_vertex( const polygon_vertex& polver);


	// Add a new polygon - you must set the vertices separately
/**
 * Adds a polygon.
 * <br><br>
 * @param ipoly
 * polygon index.
 * @param num_vertex
 * number vertex.
 * @param vertex_template
 * pointer to class vertex template.
 * @param ishare
 * info about which edge of previous polygon is shared with this one.
 */
	int add_polygon(int ipoly, int num_vertex,
					VERTEX_TEMPLATE* vertex_template = NULL,int ishare=-2);

/**
 * Sets the data of the polygon vertex.
 * <br><br>
 * @param ipoly
 * polygon index.
 * @param vertex_number
 * index of the vertex.
 * @param polver
 * data of the vertex.
 */
	int set_poly_vertex(int ipoly, int vertex_number, polygon_vertex* polver);

	// Map the UV values of all vertices into the range [0, 1]
/**
 * Maps uv-parameter into a range between 0 and 1.
 */
	void map_uv_into_01();

	// Reverse the MESH
/**
 * Reverses the indexed mesh.
 */
	void reverse();

/**
 * @nodoc
 */
  logical reverse_mesh();

/**
 * @nodoc
 */
	void set_reverse_flag(logical _reverse_flag){reverse_flag=_reverse_flag;}

/**
 * @nodoc
 */
	logical get_reverse_flag(){return reverse_flag;}

	// Write the MESH to a file

/**
 * Writes the indexed mesh to a file.
 * <br><br>
 * @param fp
 * output file stream.
 * @param trans
 * transformation.
 */
	virtual void write(FILE* fp, const SPAtransf* trans = NULL);

		// virtual methods of MESH


/**
 * @nodoc
 */
	virtual SPAbox get_transformed_box(const SPAtransf* = NULL);

	// Write the RAW MESH to a file
/**
 * Writes the mesh to a file.
 * <br><br>
 * @param fp
 * output file stream.
 * @param trans
 * transformation.
 */
 virtual void write_raw(FILE* fp, const SPAtransf* trans = NULL);

	// virtual methods of MESH
/**
 * Returns the bounding box of the mesh.
 */
	virtual SPAbox get_box();

	// Get an editable version of the vertex.  This allows
	// one to iterate through the array of vertices and make
	// modifications without having to use the polygon methods
	// and visit the vertices multiple times.
/**
 * Gets an editable version of the vertex.
 * <br><br>
 * <b>Role :</b> This allows one to iterate through the array of vertices and
 * make modifications without having to use the polygon methods and visit the
 * vertices multiple times.
 * @param inode
 * node.
 */
 polygon_vertex &get_vertex_for_edit( int inode);


	// Get scaled or as-entered uv given a vertex number
/**
 * Given a vertex number, get the "as entered" uv value.
 * <br><br>
 * @param inode
 * index to node.
 */
	SPApar_pos get_uv_as_entered(int inode) ;
/**
 * Given a vertex number, get a copy of the "as scaled" (0 to 1) uv value.
 * <br><br>
 * @param inode
 * index to node.
 */
	SPApar_pos get_uv_as_scaled(int inode) ;

	// Get the number of polygon nodes
/**
 * Returns the number of polynodes on mesh.
 */
 int get_num_polynode() const { return m_nPolynode; }

    //Transform the vertices

/**
 * Transform the nodes of index mesh.
 * <br><br>
 * @param t
 * transformation type SPAtransf.
 */
	logical transform(SPAtransf const& t);


/**
 * @see SEQUENTIAL_MESH#get_type_of_mesh
 */
	SEQUENTIAL_MESH_TYPES get_type_of_mesh() const { return INDEXED_MESH_TYPE; }
/**
 * @see SEQUENTIAL_MESH#get_num_node
 */
	int get_num_node() const { return get_num_vertex(); }
/**
 * @see SEQUENTIAL_MESH#get_first_polygon
 */
	logical	get_first_polygon(MESH_POLYGON& outPolygon) const;
/**
 * @see SEQUENTIAL_MESH#get_next_polygon
 */
	logical	get_next_polygon(MESH_POLYGON& inoutPolygon) const;
/**
 * @see SEQUENTIAL_MESH#get_next_polygon
 */
	logical	get_next_polygon(MESH_POLYNODE& inPolynode, MESH_POLYGON& outPolygon) const;
/**
 * @see SEQUENTIAL_MESH#get_num_polynode
 */
	int		get_num_polynode(MESH_POLYGON& inPolygon) const;
/**
 * @see SEQUENTIAL_MESH#get_first_polynode
 */
	logical	get_first_polynode(MESH_POLYGON& inPolygon, MESH_POLYNODE& outPolynode) const;
/**
 * @see SEQUENTIAL_MESH#get_next_polynode
 */
	logical	get_next_polynode(MESH_POLYNODE& inoutPolynode) const;
/**
 * @see SEQUENTIAL_MESH#get_position
 */
	const SPAposition&	  get_position(MESH_POLYNODE& inPolynode) const;
/**
 * @see SEQUENTIAL_MESH#get_normal
 */
	const SPAunit_vector& get_normal(  MESH_POLYNODE& inPolynode) const;
/**
 * @see SEQUENTIAL_MESH#get_par_pos
 */
	SPApar_pos	  get_par_pos( MESH_POLYNODE& inPolynode) const;
/**
 * @see SEQUENTIAL_MESH#get_first_node
 */
	logical	get_first_node(MESH_NODE& outNode) const;
/**
 * @see SEQUENTIAL_MESH#get_next_node
 */
	logical	get_next_node(MESH_NODE& inoutNode) const;
/**
 * @see SEQUENTIAL_MESH#get_node_index
 */
	int		get_node_index(MESH_POLYNODE& inPolynode) const;
/**
 * @see SEQUENTIAL_MESH#get_position
 */
	const SPAposition&	  get_position(MESH_NODE& inNode) const;
/**
 * @see SEQUENTIAL_MESH#get_normal
 */
	const SPAunit_vector& get_normal(  MESH_NODE& inNode) const;
/**
 * @see SEQUENTIAL_MESH#get_par_pos
 */
	SPApar_pos get_par_pos( MESH_NODE& inNode) const;
/**
 * @see SEQUENTIAL_MESH#get_share_info
 */
	int get_share_info(MESH_POLYGON& inPolygon) const;
/**
 * @see SEQUENTIAL_MESH#get_seq_polygon
 */
	logical get_seq_polygon(int inPolygonIndex, seq_polygon& outIndexedPolygon);
/**
 * @see SEQUENTIAL_MESH#set_par_pos_mapping_01
 */
	void set_par_pos_mapping_01(logical onoff);
/**
 * @see SEQUENTIAL_MESH#get_par_pos_mapping_01
 */
	logical get_par_pos_mapping_01() const;
/**
 * @see SEQUENTIAL_MESH#get_par_pos_mapped_01
 */
	void get_par_pos_mapped_01(const SPApar_pos& inUV, SPApar_pos& outUV) const;
/**
 * @see SEQUENTIAL_MESH#get_par_pos_surface
 */
	void get_par_pos_surface(const SPApar_pos& inUV, SPApar_pos& outUV) const;
};

/* @} */
#endif
