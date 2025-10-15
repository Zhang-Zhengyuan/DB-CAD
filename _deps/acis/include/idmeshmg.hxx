/*******************************************************************/
/*    Copyright (c) 1989-2020 by Spatial Corp.                     */
/*    All rights reserved.                                         */
/*    Protected by U.S. Patents 5,257,205; 5,351,196; 6,369,815;   */
/*                              5,982,378; 6,462,738; 6,941,251    */
/*    Protected by European Patents 0503642; 69220263.3            */
/*    Protected by Hong Kong Patent 1008101A                       */
/*******************************************************************/
#ifndef _IDMESHMG
#define _IDMESHMG
#include "dcl_fct.h"
#include "af_api.hxx"
class SIMPLE_INDEXED_MESH;
class SIMPLE_INDEXED_MESH_MANAGER;
class SPAposition;
class SPAunit_vector;
class SPAbox;
class MESH;
/**
 * @file idmeshmg.hxx
 * @CAA2Level L1
 * @CAA2Usage U2
 *! \addtogroup FCTAPI
 *  \brief Declared at <idmeshmg.hxx>
 *  @{
 */
//***************************************************
// An SIMPLE_INDEXED_MESH is a mesh format that minimizes memory,
//	places its data in contiguous arrays, thus presumably
//	minimizing memory fragmentation.
//
// The indexed mesh manager suite includes the following files:
//
//	idmeshmg.cxx (This file) -
//		-- the SIMPLE_INDEXED_MESH class, which is the actual format
//			of the stored mesh
//		-- SIMPLE_INDEXED_MESH_MANAGER class, which implements MESH_MANAGER
//			virtual functions in the "indexed protocol" so as to
//			build an indexed mesh.
//
//	The indexed mesh is stored as (a) an array of nodes and (b) a set
//	of polygons defined by (numerical) indices into the node set.
//	This is a 'packed' representation in which both polygons and
//	nodes are referenced via integer which index into arrays.
//
//
// This class is an appropriate appropriate mesh storage format	if
//	(a) reducing memory fragmentation is a priority
//	(b) all mesh data is available for one-time construction
//	(c) no subsequent editing is to be performed.
//	(d) the user culture prefers dealing with indices (rather than
//		pointers).
//
// Naming convention:
//	ipoly is a polygon number.
//	inode is a global node number.
//	i  is a node SPAposition when numbered only within a single polygon.
//
// All numbering is 0 based.
//
// Example:
//  0-------------1-------------2
//  |             |             |    npoly=2
//  |             |             6    nnode=7
//  | ipoly=0     |  ipoly=1    |    npolynode=9  (4 on ipoly=0, 5 on ipoly=2)
//  |             |             |
//  3-------------4-------------5
//
//	polynode0[] = { 0,4,9 } = array leading to the zero'th index per polygon
//                  | | |
//                  | | |--------------|
//                  | |                |
//                  | --------         |
//                  |        |         |
//  polynode[]  = { 0,3,4,1, 4,5,6,2,1 } = array of node indices
//
//	polynode0[npoly+1] points to one past then end of the polynode.
//
// Application code can access the mesh via the following functions:
//
//	int get_npoly() -- returns the polygon count.
//	int get_nnode() -- returns the node count.
//	int get_npolynode(ipoly) -- returns the number of nodes on polygon ipoly
//	int get_sum_polynodes() -- returns the sum of the per-polygon node counts.
//
//	const SPAposition &get_position(inode) -- returns the SPAposition (XYZ) of
//			the i'th node.
//	const SPAunit_vector &get_normal(inode) -- returns the normal (XYZ)
//			stored with global node inode.
//	int get_polynode(ipoly,i) -- returns the global node index of
//			the i'th node within polygon ipoly.
//	void get_polynode_array(ipoly,const int *&id,int &n) -- return
//			n=number of nodes on polygon ipoly
//			id=pointer to the first of the array of the n global node
//				ids on the polygon.
//
// Error handling conventions:
//
//	(a) all ipoly and inode indices are checked and adjusted to 0
//		if out of bounds.
//	(b) the SPAposition and normal of node 0 are pre-initialized to
//		(0,0,0) so that references to them can be returned even if
//		no nodes are actually defined.
//
// A SIMPLE_INDEXED_MESH_MANAGER is the class which constructs the
// SIMPLE_INDEXED_MESH from data which the facetter announces to the
// MESH_MANAGER base class.
//***************************************************


/**
 * @nodoc
 */
DECL_FCT SIMPLE_INDEXED_MESH *replace_in_list(SIMPLE_INDEXED_MESH *newmesh,
		SIMPLE_INDEXED_MESH*&first,
		void *id);


/**
 * @nodoc
 */
DECL_FCT void checklist(SIMPLE_INDEXED_MESH *first);

// INEXED_MESH class.  Internal structures as defined above.

/**
 * A mesh format that stores indices to a vertex array.
 * <br>
 * <b>Role:</b> A <tt>SIMPLE_INDEXED_MESH</tt> is a mesh format that minimizes memory by
 * placing data in contiguous arrays. This should also give fast access to the
 * data by minimizing the number of allocations required, and by minimizing
 * fragmentation. The indexed mesh is stored as an array of nodes and a set of
 * polygons defined by (numerical) indices into the node set. This is a packed
 * representation in which both polygons and nodes are referenced via integers
 * that index into arrays.
 * <br><br>
 * This class is an appropriate mesh storage format if reducing memory
 * fragmentation is a priority, all mesh data is available for one-time
 * construction, and no subsequent editing (other than moving vertices) is to be
 * performed.
 * <br><br>
 * In this example, ipoly is a polygon number, inode is a global node number,
 * i is a node position when numbered only within a single polygon, and all
 * numbering is 0 based. This shows an indexed mesh of two polygons, ipoly=0 and
 * ipoly=1. For this mesh, npoly = 2, nnode = 7, and npolynode = 9 (4 on ipoly=0,
 * 5 on ipoly=2).
 * <pre>
 *  0-------------1-------------2
 *  |             |             |
 *  |             |             6
 *  | ipoly=0     |  ipoly=1    |
 *  |             |             |
 *  3-------------4-------------5
 * </pre>
 * An array leading to the zero'th index per polygon is given by
 * <pre>
 *  polynode0[] = { 0,4,9 } = array leading to the zero'th index per polygon
 *                  | | |
 *                  | | +--------------+
 *                  | |                |
 *                  | +------+         |
 *                  |        |         |
 *  polynode[]  = { 0,3,4,1, 4,5,6,2,1 } = array of node indices
 * </pre>
 * Error handling conventions for this class are as follows:
 * <ol type="a">
 * <li>all <i>ipoly</i> and <i>inode</i> indices are checked and adjusted to 0 if out of bounds.</li>
 * <li>the position and normal of node 0 are pre-initialized to (0,0,0) so that
 * references to them can be returned even if no nodes are actually defined.</li>
 * </ol>
 * polynode0[npoly+1] points to one past the end of the polynode.
 * @see SIMPLE_INDEXED_MESH_MANAGER, SPAposition, SPAunit_vector
 */
class DECL_FCT SIMPLE_INDEXED_MESH : public MESH {
	private:
		SIMPLE_INDEXED_MESH *next;
		void *appid;
		friend DECL_FCT SIMPLE_INDEXED_MESH *replace_in_list(SIMPLE_INDEXED_MESH *newmesh,
				SIMPLE_INDEXED_MESH *&first, void *id);
		friend DECL_FCT void checklist(SIMPLE_INDEXED_MESH *M);
		friend class SIMPLE_INDEXED_MESH_MANAGER;

	// These counts are upper-limits passed at constructor time.
		int mpoly;
		int mnode;
		int mpolynode;
	// Actual counts.
		int npoly;
		int nnode;
		int npolynode;
	// packed data arrays
		SPAposition *X;		// array of mpoly positions.
		SPAunit_vector *N;		// array of mpoly normal vectors.
		int *polynode0;// polynode0[ipoly] = index of the
							// polynode SPAposition of the 0th polynode of
							// polygon i. polynode0[npoly] is the
							// index for a hypothetical polygon npoly+1,
							//	so the node count for ipoly is
							//		polynode0[ipoly+1]-polynode0[ipoly]
		int *polynode;		// array of polynodes, packed as described with
							//		polynode0.
	public:
/**
 * C++ constructor, creating a <tt>SIMPLE_INDEXED_MESH</tt> using the specified parameters.
 * <br><br>
 * @param mpoly
 * maximum vertex.
 * @param mnode
 * maximum polygon.
 * @param mpolynode
 * maximum polynode.
 */
	SIMPLE_INDEXED_MESH(int mpoly,int mnode,int mpolynode);
/**
 * C++ destructor, deleting a SIMPLE_INDEXED_MESH.
 */
	~SIMPLE_INDEXED_MESH();
/**
 * Returns the polygon count.
 */
	int get_npoly();
/**
 * Returns the node count.
 */
	int get_nnode();
/**
 * Returns the number of nodes on polygon.
 * <br><br>
 * @param ipoly
 * polygon index.
 */
	int get_npolynode(int ipoly);
/**
 * Returns the sum of the per-polygon node counts.
 */
	int get_sum_polynodes();
/**
 * Returns the next simple indexed mesh.
 */
	SIMPLE_INDEXED_MESH *get_next();

/**
 * Returns the position (x,y,z) of the ith node.
 * <br><br>
 * @param inode
 * index of node.
 */
	const SPAposition &get_position(int inode);
/**
 * Returns the normal (x,y,z) stored at node.
 * <br><br>
 * @param inode
 * index of node.
 */
 const SPAunit_vector &get_normal(int inode);
/**
 * Returns the global node index of the ith node of a polygon.
 * <br><br>
 * @param ipoly
 * polygon index.
 * @param i
 * local index of node.
 */
	int get_polynode(int ipoly,int i);
/**
 * Returns node indices of polygon.
 * <br><br>
 * @param ipoly
 * polygon index.
 * @param id
 * array of node indices.
 * @param n
 * number of nodes on polygon.
 */
	void get_polynode_array(int ipoly,const int *&id,int &n);
/**
 * Returns the bounding box of the mesh.
 */
	SPAbox get_box();
};

// SIMPLE_INDEXED_MESH_MANAGER -- handles the construction protocol for
//	SIMPLE_INDEXED_MESH

/**
 * To show an example of a mesh manager using the indexed protocol.
 * <br>
 * <b>Role:</b> <tt>SIMPLE_INDEXED_MESH_MANAGER</tt> uses the indexed protocol to output
 * the the facet data. It stores the data into <tt>SIMPLE_INDEXED_MESH</tt>.
 * @see SIMPLE_INDEXED_MESH
 */
class DECL_FCT SIMPLE_INDEXED_MESH_MANAGER : public MESH_MANAGER {

 protected:

/**
 * Pointer to <tt>SIMPLE_INDEXED_MESH</tt>
 */
	SIMPLE_INDEXED_MESH *mesh;

/**
 * Function to print an error message.
 */
	void sequence_error(char *s);

 public:
 /**
  * C++ constructor, creating a <tt>SIMPLE_INDEXED_MESH_MANAGER</tt> using the specified parameters.
  */
   SIMPLE_INDEXED_MESH_MANAGER();

 /**
  * C++ destructor.
  */
   virtual ~SIMPLE_INDEXED_MESH_MANAGER();


	// These functions are invoked by the facatter (via the virtual
	//	function mechanism) to construct the mesh.

  /**
   * Announces the beginning of the output of a mesh.
   * <br><br>
   * @param entity
   * faceted entity.
   * @param app_ref
   * refinement entity.
   * @param format
   * format entity.
   */
  virtual void begin_mesh_output(
		ENTITY *entity,
		ENTITY *app_ref,
		ENTITY *format);

  /**
   * Ends mesh output.
   * <br><br>
   * @param entity
   * faceted entity.
   * @param app_ref
   * refinement entity.
   * @param format
   * format entity.
   */
  virtual void end_mesh_output(
		ENTITY *entity,
		ENTITY *app_ref,
		ENTITY *format);

/**
 * Announces the number of polygons, nodes, and node references by polygons.
 * <br><br>
 * <b>Role:</b> Creates a mesh with the proper storage.
 * <br><br>
 * @param npoly
 * polygon count.
 * @param nnode
 * node count.
 * @param nref
 * node reference count.
 */
  virtual void announce_counts(int npoly,int nnode,int nref);

 /**
  * If this function returns <tt>TRUE</tt>, the indexed protocol is turned on.
  * <br><br>
  * <b>Role:</b> The default returns <tt>FALSE</tt>. Used with the indexed protocol
  * to flag requested data.
  */
  virtual logical need_indexed_polygons();

  /**
   * Gets a node.
   * <br><br>
   * <b>Role:</b> Stores it into the global node array of mesh.
   * <br><br>
   * @param inode
   * global node number.
   * @param uv
   * surface parameter.
   * @param iX
   * position.
   * @param N
   * normal.
   */
  virtual void *announce_indexed_node(
		int inode,
		const SPApar_pos &uv,
		const SPAposition &iX,
		const SPAunit_vector &N);
  //STI mka added last optional SPAparameter
  /**
   * A polygon starts.
   * <br><br>
   * <b>Role:</b> Update the polygon data in the mesh.
   * <tt>SIMPLE_INDEXED_MESH_MANAGER::end_indexed_polygon</tt> is not redefined (omitted)
   * because <tt>MESH_MANAGER::end_indexed_polygon</tt> is sufficient.
   * <br><br>
   * @param ipoly
   * polygon index.
   * @param npolynode
   * number of polygon nodes.
   * @param ishare
   * info about which edge of previous polygon is shared with this one.
   */
  virtual void start_indexed_polygon(int ipoly,int npolynode, int ishare = -2);

  // This null implementation is just to get rid of a warning more likely due
  // to compiler's own confusion about function being hidden.
  /**
   * Announces an indexed node on a polygon, including coedge and coordinate information.
   * <br><br>
   * @param ent
   * entity.
   * @param ipoly
   * polygon index.
   * @param i
   * node position.
   * @param id
   * node identifier.
   */
   virtual void announce_indexed_polynode(ENTITY *ent,int ipoly,int i,void *id) 
   {
		SPAUNUSED(ent)
		SPAUNUSED(ipoly)
		SPAUNUSED(i)
		SPAUNUSED(id)
   }

/**
 * Announces an indexed node on a polygon.
 * <br><br>
 * @param ipoly
 * polygon index.
 * @param i
 * node position.
 * @param pnode
 * node identifier.
 */
  virtual void announce_indexed_polynode(int ipoly,int i,void *pnode);

 /**
  * Announces an indexed node on a polygon, including coedge information.
  * <br><br>
  * @param ent
  * coedge along the edge following the polynode.
  * @param ipoly
  * polygon index.
  * @param i
  * local node index.
  * @param id
  * node identifier.
  * @param edge_tpar
  * if the node lies on an edge, then returns tpar.
  * @param uv
  * parametric coordinates.
  * @param iX
  * Cartesian coordinates.
  * @param uvec
  * surface normal.
  */
 virtual void announce_indexed_polynode(
	ENTITY *ent /*E*/,					// Entity (actually, COEDGE or null) along the
									// edge following the polynode.
	int ipoly/*ipoly*/,					// 0-based polygon index. This is the
									//	same as the immediately preceding call to
									//  start_indexed_polygon().
	int i/*i*/,						// 0-based counter within the polygon.  This
									//	increments sequentially on successive calls.
	void *id /*id*/,					// Node identifer as previously received from
									// announce_indexed_node
    const double &edge_tpar /*edge_tpar*/,	//If the node lies on an edge ten returns tpar otherwise garbage;
	const SPApar_pos &uv /*uv*/,			// parametric coordinates
	const SPAposition &iX /*X*/,			// cartesian coordinates
	const SPAunit_vector &uvec /*N*/		// surface normal
	)
 {
	SPAUNUSED(ent)
	SPAUNUSED(ipoly)
	SPAUNUSED(i)
	SPAUNUSED(id)
	SPAUNUSED(edge_tpar)
	SPAUNUSED(uv)
	SPAUNUSED(iX)
	SPAUNUSED(uvec)
 }

};
/* @} */
#endif
