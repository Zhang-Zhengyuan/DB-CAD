/*******************************************************************/
/*    Copyright (c) 1989-2020 by Spatial Corp.                     */
/*    All rights reserved.                                         */
/*    Protected by U.S. Patents 5,257,205; 5,351,196; 6,369,815;   */
/*                              5,982,378; 6,462,738; 6,941,251    */
/*    Protected by European Patents 0503642; 69220263.3            */
/*    Protected by Hong Kong Patent 1008101A                       */
/*******************************************************************/
#ifndef _GMESHMG
#define _GMESHMG
#ifndef NULL
#define NULL 0
#endif
class ENTITY;
class FACE;
class EDGE;
class VERTEX;
class REFINEMENT;
class VERTEX_TEMPLATE;
class SPAposition;
class SPApar_pos;
class SPAunit_vector;
#include "dcl_fct.h"
#include "logical.h"
#include "meshmg.hxx"
#include "param.hxx"
#include "position.hxx"
#include "unitvec.hxx"
/**
* @file gmeshmg.hxx
 * @CAA2Level L1
 * @CAA2Usage U2
 *! \addtogroup FCTAPI
 *  \brief Declared at <gmeshmg.hxx>
 *  @{
 */
//**************************************************************
// TOPIC(GLOBAL_MESH_MANAGER,Faceter classes;MESH_MANAGER)
//
// The GLOBAL_MESH_MANAGER class is an example of a mesh manager
//	using the 'global' output protocol.This protocol is generally used when
//  one mesh is needed for entire body.
// The mesh is written to the standard output.  Because there are no graphics
//	or permanent storage services, the entire mesh manager functionality
//	is in the single file gmeshmg.cxx, rather than being
//	spread through several files that distinguish among the formatting,
//	storage, and display functionality.
//
// The file format is an echo of the sequence of calls that define
//	the global mesh.  Each line has a keyword, followed by data.
//  The keywords and data fields are as follows.
//PCODE
//	member function(s)..                 produces output
//  announce_counts                      polygons NNN
//                                       nodes NNN
//                                       polynodes NNN
//
//  announce_global_node(VERTEX *,..)    node_at_vertex id vertexaddr x y z
//  announce_global_node(EDGE*,..)       node_at_edge id edgeaddr x y z t
//  announce_global_node(FACE*,..)       node_at_face id faceaddr x y z u v
//
//  start_indexed_polygon
//      announce_indexed_polynode         polygon id npolynode id0 id1 ....
//	end_indexed_polygon
//
// where
//	NNN 	= a count
//	vertexaddr edgeaddr faceadr = addresses of entities.
//	x y z   =	cartesian coordinates
//  u v     =   parameters on face
//  t       =   SPAparameter on edge
//END
//**************************************************************
/**
 * This class provides an example of a mesh manager using the global-indexed protocol.
 * <br>
 * <b>Role:</b> The <tt>GLOBAL_MESH_MANAGER</tt> class is an example of a mesh manager
 * using the global-indexed protocol. This protocol is generally used when a single
 * mesh is required for an entire body. The mesh is written to the standard output.
 * The output is an echo of the sequence of calls that define the global mesh.
 * <br><br>
 * This example demonstrates the virtual member functions of <tt>MESH_MANAGER</tt>
 * that need to be redefined for this protocol.
 */
class DECL_FCT GLOBAL_MESH_MANAGER : public MESH_MANAGER {

 public:
/**
 * C++ constructor, creating a <tt>GLOBAL_MESH_MANAGER</tt>.
 */
	GLOBAL_MESH_MANAGER();

  /**
   * Announces the number of polygons, nodes and node references by polygons, and prints them.
   * <br><br>
   * @param npoly
   * number of polygons.
   * @param nnode
   * number of nodes.
   * @param npolynode
   * number of node references.
   */
  virtual void announce_counts(
		int npoly,		// Number of polygons to follow.
		int nnode,		// Number of nodes to follow.
		int npolynode	// Number of nodes when counted each time
						// they are used by a polygon.
	);

  /**
   * A new polygon starts and prints the polygon header.
   * <br><br>
   * @param ipoly
   * polygon index.
   * @param npolynode
   * number of nodes.
   * @param ishare
   * info about which edge of previous polygon is shared with this one; <tt>-2</tt> means not used.
   */
  virtual void start_indexed_polygon(
	int ipoly,				// 0-based polygon index
	int npolynode,			// Number of nodes around the polygon
	int ishare = -2			// info about which edge of previous polgon is shared with this one
	);

  // This null implementation is just to get rid of a warning more likely due
  // to compiler's own confusion about function being hidden.
/**
 * Announces a polynode of a polygon and prints its information.
 * <br><br>
 * <b>Role:</b> This signature has a NULL implementation in the <tt>GLOBAL_MESH_MANAGER</tt>.
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
 * Announces a polynode of a polygon and prints its information.
 * <br><br>
 * <b>Role:</b> The polygon index and counter are zero-based. The counter
 * increments sequentially on successive calls. The node identifier is as previously
 * received from <tt>announce_indexed_node</tt>.
 * <br><br>
 * @param ipoly
 * 0-based polygon index.
 * @param i
 * 0-based node index within this polygon.
 * @param id
 * node identifier as previously received from <tt>announce_indexed_node</tt>.
 */
  virtual void announce_indexed_polynode(
	int ipoly,				// 0-based polygon index. This is the
							//	same as the immediately preceding call to
							//  start_indexed_polygon().
	int i,					// 0-based counter within the polygon.  This
							//	increments sequentially on successive calls.
	void *id				// Node identifer as previously received from
							// announce_indexed_node
	);

/**
 * Announces a polynode of a polygon and prints its information.
 * <br><br>
 * <b>Role:</b> This signature has a NULL implementation in the <tt>GLOBAL_MESH_MANAGER</tt>.
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
	ENTITY* E,					// Entity (actually, COEDGE or null) along the
									// edge following the polynode.
	int ipoly,					// 0-based polygon index. This is the
									//	same as the immediately preceding call to
									//  start_indexed_polygon().
	int i,						// 0-based counter within the polygon.  This
									//	increments sequentially on successive calls.
	void *id,					// Node identifer as previously received from
									// announce_indexed_node
    const double & tpar,        //If the node lies on an edge ten returns tpar otherwise garbage;
	const SPApar_pos & uv,			// parametric coordinates
	const SPAposition & iX,			// cartesian coordinates
	const SPAunit_vector & N		// surface normal
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
 * Announces the completion of the output of a polygon. 
 * <br><br>
 * <b>Role:</b> In the <tt>GLOBAL_MESH_MANAGER</tt> this method prints a new line.
 * <br><br>
 * @param ipoly
 * 0-based polygon index.
 */
 virtual void end_indexed_polygon(
	int ipoly				// 0-based polygon index.  This matches the
							// immediately preceding call to start_indexed_polygon(..)
							// and the (multiple) calls to announce_indexed_polynode(..)
	);

/**
 * Tells faceter that globally-indexed polygon output is needed for this class.
 * <br><br>
 * <b>Role:</b> In the <tt>GLOBAL_MESH_MANAGER</tt> this method returns <tt>TRUE</tt>. The default is <tt>FALSE</tt>.
 */
	virtual logical need_global_indexed_polygons();
/**
 * If this function returns <tt>TRUE</tt>, the number of polygons, nodes and node references 
 * by polygons are announced before other output.
 * <br><br>
 * <b>Role:</b> In the <tt>GLOBAL_MESH_MANAGER</tt> this method returns <tt>TRUE</tt>. The default is <tt>FALSE</tt>.
 */
	virtual logical need_precount_of_global_indexed_polygons();
/**
* Make this method return <tt>TRUE</tt> in the derived class if you want different uv values and nodes at singularities.
* <br><br>
* <b>Role:</b> In the <tt>GLOBAL_MESH_MANAGER</tt> this method returns <tt>FALSE</tt>. The default is <tt>TRUE</tt>.
*/
	virtual logical need_duplicate_indexed_nodes_at_singularities();
/**
 * Announces a node on a model <tt>VERTEX</tt> and prints it.
 * <br><br>
 * @param inode
 * 0-based node index.
 * @param ver
 * model vertex.
 * @param iX
 * node coordinates.
 */
	virtual void *announce_global_node(
			int inode, 
			VERTEX *ver, 
			const SPAposition &iX
		);
/**
 * Announces a node on a model <tt>EDGE</tt> with its %curve parameter and prints it.
 * <br><br>
 * @param inode
 * 0-based node index.
 * @param ed
 * model edge.
 * @param iX
 * node coordinates.
 * @param tpar
 * %curve parameter value.
 */
 virtual void *announce_global_node(
			int inode,
			EDGE *ed,
			const SPAposition &iX,
			double tpar
		);
/**
 * Announces a node on a model <tt>FACE</tt> with its %surface parameters and prints it.
 * <br><br>
 * @param inode
 * 0-based node index.
 * @param face
 * model face.
 * @param iX
 * node coordinates.
 * @param uv
 * %surface parameters.
 */
	virtual void *announce_global_node(
			int inode,
			FACE * face,
			const SPAposition &iX,
			const SPApar_pos &uv
		);

/**
 * Returns the node id value that is guaranteed to be invalid.
 * <br><br>
 * <b>Role:</b> Applications using indices will typically return <tt>-1</tt>. Applications
 * using pointers will typically return <tt>0</tt>. The default returns <tt>-1</tt>.
 * In the <tt>GLOBAL_MESH_MANAGER</tt> this method returns <tt>-1</tt>. 
 */
	virtual void *null_node_id();

};
/* @} */
#endif
