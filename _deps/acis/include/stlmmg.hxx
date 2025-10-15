/*******************************************************************/
/*    Copyright (c) 1989-2020 by Spatial Corp.                     */
/*    All rights reserved.                                         */
/*    Protected by U.S. Patents 5,257,205; 5,351,196; 6,369,815;   */
/*                              5,982,378; 6,462,738; 6,941,251    */
/*    Protected by European Patents 0503642; 69220263.3            */
/*    Protected by Hong Kong Patent 1008101A                       */
/*******************************************************************/
#ifndef _STLMMG
#define _STLMMG
#ifndef NULL
#define NULL 0
#endif
class ENTITY;
class FACE;
class EDGE;
class VERTEX;
class SPAposition;
class SPApar_pos;
class SPAunit_vector;
#include "dcl_fct.h"
#include "logical.h"
#include "refine.hxx"
#include "meshmg.hxx"
#include "param.hxx"
#include "position.hxx"
#include "vector.hxx"
#include "unitvec.hxx"

/**
 * @file stlmmg.hxx
 * @CAA2Level L1
 * @CAA2Usage U2
 *! \addtogroup FCTAPI
 *  \brief Declared at <stlmmg.hxx>
 *  @{
 */
//**************************************************************
//DOC
// TOPIC(STL_MESH_MANAGER,Faceter classes;MESH_MANAGER)
//
// The STL_MESH_MANAGER uses the "global indexed" protocol
//	to write data to an stl formatted file.  The applicaton must
//	identify the output file by calling the member function
//	set_output_file(FILE*).
//
// The mesh is not saved for reuse.
//
//  Two arrays are used during the output:
//		-- Xi[..] = array of node positions.  This is sized from
//			the precount provided by the faceter.
//		-- pnode[..] = array of node indices on the current polygon.
//			This is built up from the announce_indexed_polynode calls
//			so that end_indexed_polygon can have all the indices
//			(and hence coordiantes) available for normall computation.
//  Both arrays are freed at the end.
//
// REMARK: The file format itself is actually a "coordinate polygon"
//	style -- each polygons vertices have coordinates given locally
//	rather than by reference to nodes defined elsewhere.  However,
//	using the global scheme makes it clearer that shared nodes are
//	actually getting the same coordinates, since they are printed
//  out of the shared array.
//
//PCODE
//  member function(s)..                 produces output
//  announce_counts                      solid ascii
//
//  start_indexed_polygon
//  announce_indexed_polynode
//  end_indexed_polygon
//                                       facet normal Nx Ny Nz
//                                         outer loop
//                                           vertex x y z
//                                           vertex x y z
//                                           vertex x y z
//                                           ....
//                                         endloop
//                                       endfacet
//
//  end_global_mesh_output(..)			end solid
//
// where
//  NNN     = a count
//  vertexaddr edgeaddr faceadr = addresses of entities.
//  x y z   =   cartesian coordinates
//  u v     =   parameters on face
//  t       =   SPAparameter on edge
//END
//**************************************************************

// Up to MAX_PNODE indices are allowed on a single polygon.
// REMARK: stl files seem to like only triangles, so only 3 are
//	really allowed at that level.
// Jeff Jan2015: STL (currently) only supports triangles. Setting MAX_PNODE to 3.
// See: 150119-000008
/**
 * The STL format requires each facet to have exactly three vertices.
 */
#define MAX_PNODE 3

/**
 * Writes data to an stl formatted file.
 * <br>
 * <b>Role:</b> The <tt>STL_MESH_MANAGER</tt> uses the "global indexed" protocol to write
 * data to an stl formatted file. The application must identify the output file
 * by calling the member function <tt>set_output_file</tt>.
 * @see REFINEMENT, SPAposition
 */
class DECL_FCT STL_MESH_MANAGER : public MESH_MANAGER {

 private:
	// All nodes are kept in a contiguous array.
	// The array is allocated in announce_counts(..)
	//
	SPAposition *X;
	int nX;

	// A single polygon's indices are kept as pnode[0..npnode-1]
	// This array is allocated in announce_counts(..)
	long *pnode;
	int npnode;

	// The output file where stl data is written:
	FILE *outfile;

	// Optional refinement used for stereolithography output.
	REFINEMENT *stl_ref;

	// private utility to insert a  SPAposition into X[..] with bounds checking.
	void * addnode(int id,const SPAposition &);
 public:
/**
 * C++ constructor, creating a <tt>STL_MESH_MANAGER</tt> using the specified parameters.
 * <br><br>
 * @param R
 * refinement.
 */
	STL_MESH_MANAGER(REFINEMENT *R=NULL);
/**
 * C++ destructor, deleting a <tt>STL_MESH_MANAGER</tt>.
 */
	~STL_MESH_MANAGER();
/**
 * Set the output file.
 * <br><br>
 * @param fp
 * file.
 */
	void set_output_file(FILE *fp = stdout){outfile = fp;}
/**
 * Announces the number of polygons, nodes, and node references by polygons.
 * <br><br>
 * @param npoly
 * number of polygons to follow.
 * @param nnode
 * number of nodes to follow.
 * @param npolynode
 * number of nodes when counted each time they are used by a polygon.
 */
  virtual void announce_counts(
		int npoly,		// Number of polygons to follow.
		int nnode,		// Number of nodes to follow.
		int npolynode	// Number of nodes when counted each time
						// they are used by a polygon.
	);

/**
 * Terminates global mesh output.
 * <br><br>
 * @param topent
 * Top level entity being faceted.
 */
  virtual void end_global_mesh_output(ENTITY* topent);
/**
 * Start the index.
 * <br><br>
 * @param ipoly
 * polygon index.
 * @param npolynode
 * number of polynodes.
 * @param ishare
 * info about which edge of previous polygon is shared with this one.
 */
  virtual void start_indexed_polygon(
	int ipoly,				// 0-based polygon index
	int npolynode,			// Number of nodes around the polygon
	int ishare=-2			//Which edge from prev polygon is common with this one
	);

  // This null implementation is just to get rid of a warning more likely due
  // to compiler's own confusion about function being hidden.
  /**
   * Return the number of indexes.
   * <br><br>
   * @param ent
   * coedge along the edge following the polynode.
   * @param ipoly
   * polygon index.
   * @param i
   * local node index.
   * @param pnode
   * node identifier.
   */
  virtual void announce_indexed_polynode(ENTITY * ent,int ipoly,int i,void* pnode) 
  {
	SPAUNUSED(ent)
	SPAUNUSED(ipoly)
	SPAUNUSED(i)
	SPAUNUSED(pnode)
  }

/**
 * Return the number of indexes.
 * <br><br>
 * @param ipoly
 * 0-based polygon index. Same as immediately preceding call to start_indexed_polygon.
 * @param i
 * 0-based counter within the polygon. This increments sequentially on successive calls.
 * @param pnode
 * Node identifier as previously rcvd from announce_indexed_node.
 */
  virtual void announce_indexed_polynode(
	int ipoly,				// 0-based polygon index. This is the
							//	same as the immediately preceding call to
							//  start_indexed_polygon().
	int i,					// 0-based counter within the polygon.  This
							//	increments sequentially on successive calls.
	void *pnode				// Node identifer as previously received from
							// announce_indexed_node
	);
/**
 * Return the number of indexes.
 * <br><br>
 * @param E
 * coedge along the edge following the polynode.
 * @param ipoly
 * polygon index.
 * @param i
 * local node index.
 * @param id
 * node identifier.
 * @param edge_tpar
 * if the node lies on an edge then returns tpar.
 * @param uv
 * parametric coordinates.
 * @param X_arg
 * Cartesian coordinates.
 * @param N
 * surface normal.
 */
  virtual void announce_indexed_polynode(
	ENTITY * E/*E*/,					// Entity (actually, COEDGE or null) along the
									// edge following the polynode.
	int ipoly/*ipoly*/,					// 0-based polygon index. This is the
									//	same as the immediately preceding call to
									//  start_indexed_polygon().
	int i/*i*/,						// 0-based counter within the polygon.  This
									//	increments sequentially on successive calls.
	void * id/*id*/,					// Node identifer as previously received from
									// announce_indexed_node
    const double & edge_tpar/*edge_tpar*/,	//If the node lies on an edge ten returns tpar otherwise garbage;
	const SPApar_pos & uv/*uv*/,			// parametric coordinates
	const SPAposition & X_arg/*X_arg*/,		// cartesian coordinates
	const SPAunit_vector & N/*N*/		// surface normal
	)
  {
	  SPAUNUSED(E)
	  SPAUNUSED(ipoly)
	  SPAUNUSED(i)
	  SPAUNUSED(id)
	  SPAUNUSED(edge_tpar)
	  SPAUNUSED(uv)
	  SPAUNUSED(X_arg)
	  SPAUNUSED(N)
  }
/**
 * This matches the immediately preceding call to <tt>start_indexed_polygon</tt> and the (multiple) calls to <tt>announce_indexed_polynode</tt>.
 * <br><br>
 * @param ipoly
 * polygon index.
 */
  virtual void end_indexed_polygon(
	int ipoly				// 0-based polygon index.  This matches the
							// immediately preceding call to start_indexed_polygon(..)
							// and the (multiple) calls to announce_indexed_polynode(..)
	);

/**
 * Get the globally indexed polygons.
 */
	virtual logical need_global_indexed_polygons();
/**
 * If this function returns <tt>TRUE</tt>, the number of polygons, nodes, and node references by polygons are announced before other output.
 * <br><br>
 * <b>Role:</b> The default returns <tt>FALSE</tt>.
 */
	virtual logical need_precount_of_global_indexed_polygons();
/**
 * Announces a node on a model <tt>VERTEX</tt>.
 * <br><br>
 * @param inode
 * integer.
 * @param ver
 * vertex.
 * @param iX
 * node coordinates.
 */
	virtual void *announce_global_node(
		int inode,VERTEX * ver,const SPAposition &iX);
/**
 * Announces a node on a model <tt>EDGE</tt> with its curve parameter.
 * <br><br>
 * @param inode
 * integer.
 * @param edge
 * edge.
 * @param iX
 * node coordinates.
 * @param t
 * curve parameter.
 */
	virtual void *announce_global_node(
		int inode,EDGE *edge,const SPAposition &iX,double t);
/**
 * Announces a node on a model <tt>FACE</tt> with its surface parameters.
 * <br><br>
 * @param inode
 * integer.
 * @param face
 * face.
 * @param iX
 * node coordinates.
 * @param uv
 * surface parameter.
 */
	virtual void *announce_global_node(
		int inode,FACE *face,const SPAposition &iX,const SPApar_pos &uv);
/**
 * Returns the node id value that is guaranteed to be invalid.
 * <br><br>
 * <b>Role:</b> Applications using indices will typically return -1. Applications
 * using pointers will typically return 0. The default returns -1.
 */
	virtual void *null_node_id(){ return (void*)-1;}
/**
 * Check for the applicable refinement.
 * <br><br>
 * @param refent
 * entity with refinement.
 * @param surfmode
 * refinement surface type.
 * @param ref
 * refinement.
 */
	virtual void check_applicable_refinement(ENTITY* refent,AF_SURF_MODE surfmode,REFINEMENT*& ref);
};

/* @} */

#endif
