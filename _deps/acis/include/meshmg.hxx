/*******************************************************************/
/*    Copyright (c) 1989-2020 by Spatial Corp.                     */
/*    All rights reserved.                                         */
/*    Protected by U.S. Patents 5,257,205; 5,351,196; 6,369,815;   */
/*                              5,982,378; 6,462,738; 6,941,251    */
/*    Protected by European Patents 0503642; 69220263.3            */
/*    Protected by Hong Kong Patent 1008101A                       */
/*******************************************************************/
#ifndef _MESHMG
#define _MESHMG
#include "acis.hxx"
/**
 * @file meshmg.hxx
 * @CAA2Level L1
 * @CAA2Usage U2
 *! \addtogroup FCTAPI
 *  \brief Declared at <meshmg.hxx>
 *  @{
 */
//**************************************************************
//DOCTEXT
// TOPIC(MESH_MANAGER,MESH_MANAGER class;faceter classes)
//
//    A MESH_MANAGER class directs output of the facet data.
//
//    This base class defines numerous virtual member functions stubs that are
//    grouped into protocols.  Derived classes can redefine these functions
//    to switch on the desired protocol.  When the faceter has computed a set
//    of facets, it will output them according to the protocol specified.
//    Despite the size of the base class, the derived classes are very small.
//
//    Three protocols are provided.  The coordinate protocol outputs the
//    polygon vertices as explicit coordinates.  The indexed protocol first
//    outputs the list of coordinates of all the vertices in a face mesh, then
//    outputs the polygon vertices as indices refering to items of this list.
//    The global indexed protocol is similar to the indexed protocol, but
//    considers the entire body as a single mesh.
//
//    The general flow of output is first the counts, then the nodes, then the
//    edges, then for each mesh, the polygons are output.  The polygons are
//    output node-by-node.
//
//    For pre-1.7 compatibility, the faceter uses a MESH_MANAGER that attaches
//    the facets to the face from which they were derived as POLYGON_POINT_MESH.
//    Applications that do not use POLYGON_POINT_MESH directly should probably
//    derive a new class from MESH_MANAGER to handle the facet data.  Before
//    faceting any entities, a pointer to an instance of the derived class must
//    be passed to the faceter through an API.  Then as the faceter calls the
//    various virtual functions, the redefined functions of the derived class
//    will be invoked to handle the data.
//
//    Even though the primary purpose of a mesh manager, since it is allowed to
//    change the refinement during faceting, it can also control how each face
//    is faceted.  This can prove very useful if the application needs to tweak
//    the refinement based on its current run-time context.
//
// There are several 'protocols' for announcing mesh structure:
//	-- XREF(MESH_MANAGER INDEXED PROTOCOL) and
//		XREF(MESH_MANAGER GLOBAL INDEXED PROTOCOL) -- used by
//		derived mesh manager classes that wish to
//		maintain only one copy of each shared node, with each polygon
//		consisting of pointers to the shared nodes, and all construction
//		streamed through in a way that permits this connectivity data to
//		be laid out in continguous memory buffers.   The (nonglobal) protocol
//		creates separate indexed sets on each face.  The global protocol
//		creates a single set of facets that includes all faces and edges
//		in one interconnected mesh.
//
//		In the non-global form, the faceter will optionally (a) organize
//		nodes so that all nodes along each model edges are announced
//		contiguously and (b) for each model edge, announce the node ids
//		at the vertices and the range of node ids along the edge.
//
//	-- XREF(MESH_MANAGER COORDINATE PROTOCOL) -- used by
//		derived mesh manager classes that
//		save the coordinates of nodes around each polygon without any
//		regard for sharing.
//
// The base class defines query functions so that the faceter can
//	ask whether each protocol is required.  The base class implementation
//	answers FALSE to all protocols, so the faceter skips the
//	voluminous output calls.
//
// Remarks:
//	The 'announcement' protocols contain some redundant data.
//	For instance, the polygon counter ipoly that is passed with each polygon
//	can be deduced by keeping count of the number of polygons constructed
//	to date.  The design policy is to pass redundant data to every member function
//	that might be interested, on the assumption that it is easier to ignore
//	extra parameters than to maintain state information.
//
//  The faceter's calls to the node and polynode announcements are
//	strictly ordered -- node 0 is always announced before node 1,
//	polygon 0 before polygon 1, and within each polygon polynode 0
//	is announced before polynode 1.  Derived classes may utilize this
//	fact to allow contiguous memory use.
//
//	On the other hand, the initial announcements of the maximum number of
//	nodes, polygons, and polynodes to follow may EXCEED the actual number.
//
// Terminology:
//	-- A NODE is an xyz SPAposition in the mesh.
//	-- A POLYGON is an ordered loop of nodes.
//	-- A POLYNODE is the use of a node by a polygon.
//END


//DOCTEXT
// TOPIC(MESH_MANAGER INDEXED PROTOCOL,MESH_MANAGER class)
//
// An 'indexed polygon' is defined by references to its nodes.
// The logic in the following pseudo-code is carried out once for
// each face.
// (If global node numbering is required, with edge and vertex nodes
//	given the same id from all incident faces, use the global indexed
//  polygon protocol.)
//
// See also XREF(Member functions for MESH_MANAGER INDEXED PROTOCOL)
// See also XREF(MESH_MANAGER)
//
//PCODE
//	if(MM->need_indexed_polygons()){
//		MM->begin_mesh_output(face,refinement,format_entity);
//		... faceter predetermines the number of polygons, nodes,
//		... and references to nodes.   Facet applications that preallocate
//		... arrays will use this information to determine the size of their
//		... arrays.  Linked list implementations will ignore the counts.
//		if(MM->need_counts()){
//			MM->announce_counts(npoly,nnode,nref);
//			}
//
//		if(MM->need_edge_indices()){
//		... Announce nodes sorted so that interior points along an edge
//		... are contiguous
//			foreach shared node{
//				void *id = MM->announce_indexed_node(int inode,uv,X,N);
//			}
//		... Announce each edge.  If there are no interior points, the
//		... node id is the value returned by the null_node_id virtual function.
//		... If there is one interior point id_edge0 and id_edgeN are equal.
//		... If there are multiple interior points, they are the ones
//		... that were contiguously announced from id_edge0 to id_edgeN inclusive.
//			foreach model edge{
//				MM->announce_edge_indices(
//					ENTITY *edge,					// The model edge.
//					void *id_vert0,					// Node id at start vertex
//					void *id_edge0,void *id_edgeN,	// First and last interior node id's
//					void *id_vert1);				// Node id at end vertex
//			}
//		}else{
//		  foreach shared node{
//			void *id = MM->announce_indexed_node(int inode,uv,X,N);
//			... faceter retains node identifier p in its
//			...	internal structure for subsequent use in
//			...	defining polygons.
//		  }
//		}
//
//		int ipoly = 0;
//		foreach polygon {
//			... faceter precounts the nodes and then passes them
//			... one at a time:
//			MM->start_indexed_polygon(ipoly,n);
//			i = 0;
//			foreach node id of polygon {// id is a node identifier
//				MM->announce_indexed_polynode(ipoly,i,id);
//			}
//			MM->end_indexed_polygon(ipoly);
//		}
//		MM->end_mesh_output(face,refinement,format_entity);
//		MM->save_mesh_output(face,refinement,format_entity);
//	}
//END

//DOCTEXT
// TOPIC(MESH_MANAGER COORDINATE PROTOCOL,MESH_MANAGER class)
//
// A 'coordinate polygon' is announced as a sequence of direct
// coordinates.  The logic in the following pseudocode is carried
//	out once for each faceted face.
//
// See also XREF(Member functions for MESH_MANAGER COORDINATE PROTOCOL)
// See also XREF(MESH_MANAGER)
//
//PCODE
// if(MM->need_coordinate_polygons()){
//		MM->begin_mesh_output(face,refinement,format_entity);
//		if(MM->need_counts())
//		   {MM->announce_counts(npoly,nnode,nref);}
//
//		ipoly = 0;
//		foreach polygon {
//			... faceter precounts the nodes and then passes them
//			... one at a time:
//			MM->start_coordinate_polygon(ipoly,n);
//			i = 0;
//			foreach node of polygon {// p is a node identifier
//				MM->announce_coordinate_polygon_node(ipoly,i,uv,X,N);
//			}
//			MM->end_coordinate_polygon(ipoly);
//			npoly++;
//		}
//		MM->end_mesh_output(face,refinement,format_entity);
//		MM->save_mesh_output(face,refinement,format_entity);
//	}
//END

//DOCTEXT
// TOPIC(MESH_MANAGER GLOBAL INDEXED PROTOCOL,MESH_MANAGER class)
//
// An 'global indexed polygon' is defined by references to its nodes.
// Nodes are given numbers (or pointers) globally; polygons are then
// announced with reference to the global node ids.
//
// See also XREF(Member functions for MESH_MANAGER GLOBAL INDEXED PROTOCOL)
// See also XREF(MESH_MANAGER)
//
//PCODE
//	if(MM->need_global_indexed_polygons()){
//		MM->begin_global_mesh_output(ENTITY E)
//
//		if(MM->need_precount_of_global_indexed_polygons()){
//		... faceter predetermines the number of polygons, nodes,
//		... and references to nodes.   Facet applications that preallocate
//		... arrays will use this information to determine the size of their
//		... arrays.  Linked list implementations will ignore the counts.
//			MM->announce_counts(npoly,nnode,nref);
//		}
//
//		foreach node {
//			... Each node is announced (only) once via one of the
//			...  following:
//			void *id= MM->announce_global_node(inode,VERTEX *V,X);
//			void *id= MM->announce_global_node(inode,EDGE *E,X,t);
//			void *id= MM->announce_global_node(inode,FACE *F,X,uv);
//			... faceter retains node identifier id in its
//			...	internal structure for subsequent use in
//			...	defining polygons.
//		}
//
//		int ipoly = 0;
//		foreach polygon {
//			... faceter precounts the nodes and then passes them
//			... one at a time:
//			MM->start_indexed_polygon(ipoly,n);
//			i = 0;
//			foreach node id of polygon {// id is a node identifier
//				MM->announce_indexed_polynode(ipoly,i,id);
//			}
//			MM->end_indexed_polygon(ipoly);
//		}
//		MM->end_global_mesh_output(ENTITY *E)
//		MM->save_global_mesh_output(ENTITY *E)
//	}
//END
//
//ENUM


/**
 * Specifies the search order during mesh output.
 * <br><br>
 * @param MESH_MANAGER_SEARCH_NONE
 * Do not attempt to share edges between successive polygons.
 * @param MESH_MANAGER_SEARCH_ALTERNATE
 * Alternately reuse the last and second last. In a mesh of all triangles, this produces a "tristrip."
 * @param MESH_MANAGER_SEARCH_REUSE_LAST
 * Always try to reuse the last edge. This emits all polygons around the start node before continuing.
 * @param MESH_MANAGER_SEARCH_REUSE_SECOND_LAST
 * Always try to reuse the second to last edge. In a quad mesh, this produces a quad strip. 
 * In a triangle mesh it produces all polygons around the second vertex of the first triangle.
 * @param MESH_MANAGER_SEARCH_ANY
 * Search for any possible reusable edge.
 * @param MESH_MANAGER_SEARCH_TRI_STRIP_FAN
 * Produces triangle fans triangle strips and quad strips simultaneouly in a mesh which contains triangles and/or quads.
 */
typedef enum {
		MESH_MANAGER_SEARCH_NONE,		// Do not attempt to share edges between
										// successive polygons
		MESH_MANAGER_SEARCH_ALTERNATE,	// Alternately resue the last and second last
					// In a mesh of all triangles, this produces a 'tristrip'
		MESH_MANAGER_SEARCH_REUSE_LAST,	// Always try to reuse the last. This emits all
			// polygons around the start node before continuing.
		MESH_MANAGER_SEARCH_REUSE_SECOND_LAST,	// Always try to reuse the 2nd last
			// edge.  In a quad mesh, this produces a quad strip.  In a triangle mesh
			// it produces all polygons around the second vertex of the first triangle.
		MESH_MANAGER_SEARCH_ANY,			// Search for any possible reuesable edge.
		MESH_MANAGER_SEARCH_TRI_STRIP_FAN   //01-16-03 SC mka added search order for triangle fan strips and quad strips
	}MESH_MANAGER_SEARCH_ORDER;
//END

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
class af_internal_mesh_manager;


#include "dcl_fct.h"
#include "logical.h"
#include "af_enum.hxx"

class REFINEMENT;


/**
 * Directive to <tt>MESH_MANAGER</tt> to reuse or recalculate edge points during faceting.
 * <br><br>
 * @param AF_EDGE_RECOMPUTE
 * recompute model edge points during faceting.
 * @param AF_EDGE_USE_CURRENT_POINTS
 * use the existing points on the MODEL edge which are attached thru attribute.
 */
typedef enum {
	AF_EDGE_RECOMPUTE,
	AF_EDGE_USE_CURRENT_POINTS,
	AF_EDGE_RECOMPUTE_IF_APPROPRIATE
	} AF_EDGE_DIRECTIVE;

//DOCTEXT
// TOPIC(MESH_MANAGER shared member functions,MESH_MANAGER class;faceter classes)
//
// The following lists all member functions that are in the MESH_MANAGER base class
// and are not specialized to one of the mesh protocols.
//
// Each protocol (XREF(MESH_MANAGER INDEXED PROTOCOL),
// XREF(MESH_MANAGER COORDINATE PROTOCOL),
// XREF(MESH_MANAGER GLOBAL INDEXED PROTOCOL)) has separate entries
// for (a) a summary of the output sequence and (b) specialized member functions.
//
//END

// These are used to identify fmesh attributes which have
// been restored but not set.
/**
* Defined for unsetting the application id of the mesh.
*/
/**
 * @nodoc
 */
#define UNSET_APP_ID -1234

/**
* Defined for unsetting the user id of the mesh.
*/
/**
 * @nodoc
 */
#define UNSET_USER_ID -5678

/**
* The id used mainly to search mesh attached to entity thru attribute.
* <br><br>
* @param MESH_APP_ID
* unique application id of the mesh.
*/
typedef uintptr_t MESH_APP_ID;

/**
* Another id used mainly to search mesh attached to entity thru attribute.
* <br><br>
* @param MESH_USER_ID
* unique user id of the mesh.
*/
typedef uintptr_t MESH_USER_ID;

/**
 * A <tt>MESH_MANAGER</tt> class directs output of the facet data.
 * <br><br>
 * <b>Role:</b> This base class defines numerous virtual member function stubs
 * that are grouped into protocols. Derived classes can redefine these functions
 * to switch on the desired protocol. When the faceter has computed a set of
 * facets, it will output them according to the protocol specified. Three
 * protocols are provided, and the base class defines query functions so that
 * the faceter can ask whether each protocol is required. 
 * <br><br>
 * When deriving a class from <tt>MESH_MANAGER</tt> one of the three protocols must be selected. 
 * This is accomplished by overloading the corresponding <tt>need_</tt> method and
 * having it return <tt>TRUE</tt> rather than the default <tt>FALSE</tt>. Applications
 * should never instantiate the base class: it outputs nothing. 
 * <ul>
 * <li><b>Coordinate Protocol</b> &ndash; <tt>need_coordinate_polygons</tt>
 * should return <tt>TRUE</tt>.</li>
 * <li><b>Indexed Protocol</b> &ndash; <tt>need_indexed_polygons</tt>
 * should return <tt>TRUE</tt>.</li>
 * <li><b>Global-Indexed Protocol</b> &ndash; <tt>need_global_indexed_polygons</tt>
 * should return <tt>TRUE</tt>.</li>
 * </ul>
 * The <i>coordinate protocol</i> outputs the polygon vertices as explicit coordinates,
 * without any regard for shared nodes. The <i>indexed protocol</i> first outputs the
 * list of coordinates of all the vertices in a face mesh, then outputs the
 * polygon vertices as indices referring to items of this list. The <i>global-indexed 
 * protocol</i> is similar to the indexed protocol, but considers the entire
 * body as a single mesh. Both the indexed protocol and the global-indexed
 * protocol maintain only one copy of each shared node, with each polygon
 * consisting of pointers to the shared nodes.
 * <br><br>
 * The general flow of output is first the counts, then the nodes, then the
 * edges, and then the polygons (for each mesh). The polygons are output node
 * by node. The faceter's calls to the node and polynode announcements are
 * strictly ordered -- node 0 is always announced before node 1, polygon 0
 * before polygon 1, and so forth. The initially announced counts may exceed
 * the actual number of nodes, polygons and polynodes to follow.
 * <br><br>
 * The "announcement" methods contain some redundant data. For example, the
 * polygon counter <tt>ipoly</tt> that is passed with each polygon can also be deduced
 * by keeping count of the number of polygons constructed to date. The design
 * intent was to pass redundant data on to every member function that might be
 * interested, on the assumption that it is easier to ignore extra parameters
 * than to maintain state information.
 * <br><br>
 * The following terminology is used in <tt>MESH_MANAGER</tt> classes:
 * <br><br>
 * <pre>
 * <i>node</i>					an <i>(x,y,z)</i> position in the mesh
 * <i>polygon</i>				an ordered loop of nodes
 * <i>coordinate polygon</i>			defined by a series of direct coordinates
 * <i>indexed polygon</i>			defined by references to its nodes
 * <i>globally indexed polygon</i>		defined by references to its nodes, using global id pointers
 * <i>polynode</i>				the use of a node by a polygon
 * <i>polyedge</i>				the polygon edge immediately following a polynode,
 *					in counterclockwise order
 * <i>polyedge mate</i>				the corresponding polyedge on the adjacent facet,
 *					 which is oriented in the opposite direction
 * </pre>
 * <br><br>
 * By default the Faceter uses an instance of the @href INDEXED_MESH_MANAGER when faceting.
 * If an application desires to use a different mesh manager, the application should call
 * @href api_set_mesh_manager before faceting any entities. 
 * <br><br>
 * For additional information on mesh managers refer to the Technical Article <i>Mesh Managers</i>. 
 * For additional information on deriving an application-specific mesh manager refer to 
 * the Technical Article <i>Creating a Mesh Manager Class</i>.
 */
class DECL_FCT MESH_MANAGER : public ACIS_OBJECT {

 private:
	// The faceter terminates mesh announcement (and makes a call to
	//	abort_mesh_output) when it observes a non-zero errorid after any call.
	// Not implemented
	int  errorid;
	MESH_APP_ID app_id;
	MESH_USER_ID user_id;

 public:
 	//Constructor and virtual destructor added STI mka
/**
 * C++ constructor, creating a <tt>MESH_MANAGER</tt>.
 */
	MESH_MANAGER();

/**
 * C++ destructor, deleting a <tt>MESH_MANAGER</tt>.
 */
	virtual ~MESH_MANAGER();

/**
 * Returns the application identification number of the mesh.
 */
	virtual MESH_APP_ID get_app_id() {return app_id;}

/**
 * Returns the user identification number of the mesh.
 */
	virtual MESH_USER_ID get_user_id() {return user_id;}

/**
 * Specifies the application identification number of the mesh.
 * <br><br>
 * @param appid
 * id to use.
 */
	virtual void set_app_id(MESH_APP_ID appid) {app_id = appid;}

/**
 * Specifies the user identification number of the mesh.
 * <br><br>
 * @param userid
 * id to use.
 */
	virtual void set_user_id(MESH_USER_ID userid) {user_id = userid;}

/**
 * Announces the beginning of the output of a mesh on a single face.
 * <br><br>
 * <b>Role:</b> The arguments on <tt>begin_mesh_output</tt>, <tt>end_mesh_output</tt>, and
 * <tt>save_mesh_output</tt> are identical. This is done because different mesh managers
 * may need the pointers at different times. This simplifies mesh manager
 * coding because it is not necessary to make local copies of the refinement and
 * output format. Used with the coordinate and indexed protocols.
 * <br><br>
 * @param faceted_entity
 * faceted entity.
 * @param refinement_control_entity
 * applicable refinement control entity.
 * @param output_format_entity
 * output format entity.
 */
  	virtual void begin_mesh_output(
		ENTITY *faceted_entity,				//	entity being faceted
		ENTITY *refinement_control_entity,	//	applicable REFINEMENT
		ENTITY *output_format_entity		//	output format entity
	);

	//MEMBER
	// These two calls mark the end of mesh output on a single face
	//	of (non-global) indexed or coordinate protocols.  The arguments on
	// begin_mesh_output,end_mesh_output, and save_mesh_output are identical.
	// This is done because different mesh managers will need the pointers
	// at different times.  This simplifies mesh manager coding because
	// it is not necessary to make local copies.  It is expected that
	// the derived class responsible for formatting will implement
	// end_mesh_output, while the derived class responsible for
	// storage will implement save_mesh_output.  The output format entity
	// may be a VERTEX_TEMPLATE for pre-1.7 compatibility.
/**
 * Announces the completion of the output of a mesh for a single face. 
 * <br><br>
 * <b>Role:</b> The arguments on <tt>begin_mesh_output</tt>, <tt>end_mesh_output</tt>, and
 * <tt>save_mesh_output</tt> are identical. This is done because different mesh managers
 * may need the pointers at different times. This simplifies mesh manager
 * coding because it is not necessary to make local copies of the refinement and
 * output format. Used with the coordinate and indexed protocols.
 * <br><br>
 * @param faceted_entity
 * faceted entity.
 * @param refinement_control_entity
 * applicable refinement control entity.
 * @param output_format_entity
 * output format entity.
 */
  	virtual void end_mesh_output(
		ENTITY *faceted_entity,				// entity being faceted
		ENTITY *refinement_control_entity,	// applicable REFINEMENT
		ENTITY *output_format_entity		// output format entity
	);

/**
 * Saves the mesh output for a single face.
 * <br><br>
 * <b>Role:</b> The arguments on <tt>begin_mesh_output</tt>, <tt>end_mesh_output</tt>, and
 * <tt>save_mesh_output</tt> are identical. This is done because different mesh managers
 * may need the pointers at different times. This simplifies mesh manager
 * coding because it is not necessary to make local copies of the refinement and
 * output format. Used with the coordinate and indexed protocols.
 * <br><br>
 * @param faceted_entity
 * faceted entity.
 * @param refinement_control_entity
 * applicable refinement control entity.
 * @param output_format_entity
 * output format entity.
 */
  	virtual void save_mesh_output(
		ENTITY *faceted_entity,				// entity being faceted.
		ENTITY *refinement_control_entity,	// applicable REFINEMENT.
		ENTITY *output_format_entity		// output format entity
	);

	//END

	// abort_mesh_output is invoked when an error flag is
	//	set by a derived class.
/**
 * @nodoc
 */
  	virtual void abort_mesh_output(	// Not supported
		ENTITY *faceted_entity,
		ENTITY *refinement_control_entity,
		ENTITY *output_format_entity);

/**
 * Announces the number of polygons, nodes, and node references by polygons.
 * <br><br>
 * <b>Role:</b> Used with the coordinate, indexed and global-indexed protocols.
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
 * Used with the coordinate and indexed protocols to flag counts are needed before the polygon data is output.
 * <br><br>
 * <b>Role:</b> If this function returns <tt>TRUE</tt>, the number of polygons, number
 * of nodes, and number of polygon references of node are calculated and
 * announced. If these are not needed, it should return <tt>FALSE</tt>. The default
 * returns <tt>TRUE</tt>. 
 * <br><br>
 * A note on efficiency: It is more efficient if the mesh manager does not pre-announce the counts. 
 * In other words, it is more efficient if this function returns a value of <tt>FALSE</tt>.
 * This typically requires an application to use dynamically allocated arrays or linked lists
 * to capture the announced polygons, nodes, and node references. 
 */
	virtual logical need_counts();

/**
 * This is called to reannounce the exact number of nodes and polygons that were already announced.
 * <br><br>
 * <b>Role:</b> This is useful if <tt>need_approx_counts</tt> returns <tt>TRUE</tt>.
 * <br><br>
 * @param npoly
 * Number of polygons already announced.
 * @param nnode
 * Number of nodes already announced.
 * @param npolynode
 * Number of nodes when counted each time used by a polygon.
 */
  	virtual void reannounce_counts(
		int npoly,		// Number of polygons to follow.
		int nnode,		// Number of nodes to follow.
		int npolynode	// Number of nodes when counted each time
						// they are used by a polygon.
	);

	// Should return true if user is only interested in getting approx counts
	// of polygons nodes and edges this can save time but may use more memory.
/**
 * Returns <tt>TRUE</tt> if you are only interested in getting approximate counts 
 * of polygons, nodes and edges.
 * <br><br>
 * <b>Role:</b> Returning <tt>TRUE</tt> can save time calculating the counts,
 * but may cause your application to use more memory. If this returns <tt>TRUE</tt>, 
 * <tt>announce_counts</tt> will return approximate counts; however, <tt>reannounce_counts</tt>
 * will return exact counts. The default returns <tt>FALSE</tt>. 
 */
	virtual logical need_approx_counts();

#if 0
// Not implemented
	//MEMBER
	// This inquires what output is desired for hidden parts of
	// faces that are only partially faceted due to view-specific logic.
	//
	// Return TRUE if polygons on completely hidden faces are to
	// be passed to the mesh manager.   These polygons will may have
	// many non-planar nodes.
 /**
  * @nodoc
  */
	virtual logical need_polygons_on_fully_hidden_faces();

	// Return TRUE if polygons are to be emitted for the hidden
	// parts of faces that have some visible portions.
 /**
 * @nodoc
 */
	virtual logical need_hidden_polygons_on_partially_hidden_faces();
	//END_MEMBER
#endif

	//DOC
	// TOPIC(Member functions for MESH_MANAGER INDEXED PROTOCOL,Faceter classes)
	//
	// The following functions are specific to the the 'indexed' protocol for
	//	describing polygons.

/**
 * Used with the indexed protocol to flag requested data.
 * <br><br>
 * <b>Role:</b> If this function returns <tt>TRUE</tt>, the facets will be output with the
 * indexed protocol. The default returns <tt>FALSE</tt>.
 */
  	virtual logical need_indexed_polygons();

/**
 * This method is called if the user wants duplicate nodes where the %surface uv's differ 
 * at the same position on the %surface. The default returns <tt>FALSE</tt>.
 */
  	virtual logical need_duplicate_indexed_nodes_on_surface_seams();

/**
 * When facets meet at a singularity, such as at the apex of a %cone, each facet may have different normals.
 * <br><br>
 * <b>Role:</b> If this function returns <tt>TRUE</tt>, then duplicate nodes will be
 * placed at singularities. If this function returns <tt>FALSE</tt>, then only one node
 * will be placed at the singularity. Default is <tt>TRUE</tt>.
 */
  	virtual logical need_duplicate_indexed_nodes_at_singularities();

	//MEMBER
	// announce_indexed_node(..) is called once for each node of the mesh.  The
	//	nodes are visited in strict order (inode=0..nnode-1),
	//	where nnode is count previously passed to announce_counts()
	// announce_indexed_node(..) returns the identifier
	//	that it wishes to use in subsequent reference to this node
	//  by the incident polygons.  This may be a pointer or a
	//  (long) integer typecast to a void *.  Mesh managers that
	//  allocate contiguous arrays typically return (void*)inode.
/**
 * Announces an indexed node with its %surface parameters, position, and normal.
 * <br><br>
 * <b>Role:</b> Used with the indexed protocol. 
 * <br><br>
 * A note on overloading this function in classes derived from <tt>MESH_MANAGER</tt>: 
 * The value returned by this function is critical. The returned value must
 * be a unique identifier for this node. It will most likely be the memory address 
 * or index of this node in the application's data structure. This unique identifier 
 * will be passed as an input argument to <tt>announce_indexed_polynode</tt>.
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
		int inode,				// 0-based index of the node
		const SPApar_pos &uv,		// parametric coordinates
		const SPAposition &iX,		// cartesian coordinates
		const SPAunit_vector &N);	// surface normal

	//MEMBER
	// start_indexed_polygon(..), announce_indexed_node(..), and
	//	end_indexed_polygon(..) are called to announce the node
	//	ids around a polygon.
/**
 * Announces the beginning of the output of a new polygon. 
 * <br><br>
 * <b>Role:</b> Used with the indexed and global-indexed protocols.
 * <br><br>
 * @param ipoly
 * 0-based polygon index.
 * @param npolynode
 * number of nodes in this polygon.
 * @param ishare
 * info about which edge of previous polygon is shared with this one; <tt>-2</tt> means not used.
 */
  	virtual void start_indexed_polygon(
	int ipoly,				// 0-based polygon index
	int npolynode,			// Number of nodes around the polygon
	int ishare = -2			//Which edge of previous poly is shared with this
							//used for opengl orders
	);
/**
 * Announces the beginning of the output of a new shareable polygon. 
 * <br><br>
 * <b>Role:</b> Used with the indexed and global-indexed protocols.
 * <br><br>
 * @param ipoly
 * 0-based polygon index.
 * @param npolynode
 * number of nodes in this polygon.
 * @param ishare
 * info about which edge of previous polygon is shared with this one.
 */
  	virtual void start_shareable_indexed_polygon(
	int ipoly,				// 0-based polygon index
	int npolynode,			// Number of nodes around the polygon
	int ishare				// index of the shared edge on the prior polygon
	);

/**
 * Announces the completion of the output of a polygon. 
 * <br><br>
 * <b>Role:</b> Used with the indexed and global-indexed protocols.
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
 * Announces an indexed node on a polygon.
 * <br><br>
 * <b>Role:</b> This short signature is called if both <tt>need_coedge_pointers_on_polyedges</tt>
 * and <tt>need_indexed_polynode_with_data</tt> return <tt>FALSE</tt>, which is the default. The
 * node identifier is the one previously received from <tt>announce_indexed_node</tt>.
 * Used with the indexed and global-indexed protocols.
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

	//MEMBER
	// If the mesh manager answers TRUE to need_coedge_pointers_on_polyedges,
	//	each polygon point is passed through the extended argument list
	//  form of announce_indexed_polynode.
/**
 * Used with the indexed protocol to flag coedge pointers are needed for nodes lying on edges.
 * <br><br>
 * <b>Role:</b> If this function returns <tt>TRUE</tt>, each polygon node that lies on a
 * model edge will be output together with the corresponding coedge. The
 * default returns <tt>FALSE</tt>.
 */
  	virtual logical need_coedge_pointers_on_polyedges();

/**
 * Announces an indexed node on a polygon, including coedge pointers.
 * <br><br>
 * <b>Role:</b> This signature is called if <tt>need_coedge_pointers_on_polyedges</tt>
 * returns <tt>TRUE</tt>. The node identifier is the one previously received from
 * <tt>announce_indexed_node</tt>. Used with the indexed and global-indexed protocols.
 * <br><br>
 * If the entity pointer is non-null, the entity arguement will be a coedge.
 * The presence of this coedge pointer implies that the polyedge from the this polynode 
 * to the next polynode of this polygon lies on a model edge. For example, if this is
 * 0th node of this polygon, the polyedge from node 0 to node 1 lies on a model edge.
 * The given coedge is the coedge of this edge lying in the face of this polygon. 
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
  	virtual void announce_indexed_polynode(
	ENTITY *E,				// Entity (actually, COEDGE or null) along the
							// edge following the polynode.
	int ipoly,				// 0-based polygon index. This is the
							//	same as the immediately preceding call to
							//  start_indexed_polygon().
	int i,					// 0-based counter within the polygon.  This
							//	increments sequentially on successive calls.
	void *id				// Node identifer as previously received from
							// announce_indexed_node
	);

/**
 * Used with the indexed protocol to flag requested data.
 * <br><br>
 * <b>Role:</b> If this function returns <tt>TRUE</tt>, each polygon node will be output
 * together with the corresponding coedge, coordinates and %surface normal.
 * The default returns <tt>FALSE</tt>.
 */
  	virtual logical need_indexed_polynode_with_data();

/**
 * Announces an indexed node on a polygon, including polynode coordinate data.
 * <br><br>
 * <b>Role:</b> This extended arguement signature is called if
 * <tt>need_indexed_polynode_with_data</tt> returns <tt>TRUE</tt>. The node identifier is the one
 * previously received from <tt>announce_indexed_node</tt>. Used with the indexed and global-indexed protocols.
 * <br><br>
 * If the entity pointer is non-null, the entity arguement will be a coedge.
 * The presence of this coedge pointer implies that the polyedge from the this polynode 
 * to the next polynode of this polygon lies on a model edge. For example, if this is
 * 0th polynode of this polygon, the polyedge from polynode 0 to polynode 1 lies on a model edge.
 * The given coedge is the coedge of this edge lying in the face of this polygon. 
 * <br><br>
 * If the announced polynode lies on the interior of a model edge, the parameter value of the curve
 * underlying this edge is given by the <i>tpar</i> arguement. If the polynode lies on 
 * the start or end vertex of a model edge, the curve parameter value can be obtained from 
 * the parameter value at the start or end of the edge, negated if the sense of the edge is
 * reversed with respect to the curve.
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
 * if the node lies on the interior of a model edge, then returns the curve parameter value.  If no
 * t parameter is avialable, this is set to DBL_MAX.
 * @param uv
 * parametric coordinates.
 * @param iX
 * cartesian coordinates.
 * @param N
 * %surface normal.
 */
  	virtual void announce_indexed_polynode(
	ENTITY *E,				// Entity (actually, COEDGE or null) along the
							// edge following the polynode.
	int ipoly,				// 0-based polygon index. This is the
							//	same as the immediately preceding call to
							//  start_indexed_polygon().
	int i,					// 0-based counter within the polygon.  This
							//	increments sequentially on successive calls.
	void *id,				// Node identifer as previously received from
							// announce_indexed_node
    const double &tpar,		//If the node lies on an edge then returns tpar otherwise garbage;
	const SPApar_pos &uv,		// parametric coordinates
	const SPAposition &iX,		// cartesian coordinates
	const SPAunit_vector &N	// surface normal
	);

	//MEMBER
	// The indexed protocol also offers the mesh manager additional information
	//	about which nodes lie along model edges.  The mesh manager requests
	//	this by answering TRUE to this query:
/**
 * If this function returns <tt>TRUE</tt>, then the model edges will be announced with the node indices.
 * <br><br>
 * <b>Role:</b> The default returns <tt>FALSE</tt>. Used with the indexed and
 * global-indexed protocols to flag requested data.
 */
	virtual logical need_edge_indices();

	// Each edge is subsequently announced via announce_edge_indices.  Note that
	// idE0 and idEN define a sequential range of node identifiers.  The vertex
	// nodes are shared among edges on the face and thus cannot be included in
	// this range.
/**
 * Announces an edge with its coedge and node indices.
 * <br><br>
 * <b>Role:</b> The starting and ending vertex nodes define the end points of the
 * edge. The first and last interior nodes define a sequential range of nodes in
 * between. 
 * <br><br>
 * This method is called if <tt>need_edge_indices</tt> returns <tt>TRUE</tt>.
 * Used with the indexed and global-indexed protocols.
 * <br><br>
 * @param coedge
 * coedge of current face.
 * @param idV0
 * starting vertex node.
 * @param idE0
 * first interior node.
 * @param idEN
 * last interior node.
 * @param idV1
 * ending vertex node.
 */
	virtual	void announce_edge_indices(
		ENTITY *coedge,		// coedge of on the current face
		void *idV0,			// node identifier of starting vertex
		void *idE0,			// node identifier of first interior node
		void *idEN,			// node identifier of last interior node
		void *idV1			// node identifier of ending vertex
	);

	//DOC
	// TOPIC(Member functions for MESH_MANAGER COORDINATE PROTOCOL,Faceter classes)
	//
	// The following functions are specific to the the 'coordinate' protocol for
	//	describing polygons.
	//MEMBER
	// need_coordinate_polygons() is called to ask if polygon data is
	// to be announced in the 'coordinate polygon' protocol.
/**
 * If this function returns <tt>TRUE</tt>, the data will be announced in the coordinate protocol.
 * <br><br>
 * <b>Role:</b> The default returns <tt>FALSE</tt>. Used with the coordinate protocol to
 * flag requested data.
 */
  virtual logical need_coordinate_polygons();

	//MEMBER
	// start_coordinate_polygon(..), announce_coordinate_polygon(..),
	//	and end_coordinate_polygon(..) describe the nodal coordinates of
	//	each polygon.
/**
 * Announces the beginning of the output of a new shareable polygon. 
 * <br><br>
 * <b>Role:</b> Used with the coordinate protocol.
 * <br><br>
 * @param ipoly
 * 0-based polygon index.
 * @param nnode
 * number of nodes in this polygon.
 * @param i
 * shared index.
 */
  virtual void start_shareable_coordinate_polygon(int ipoly, int nnode, int i);

/**
 * Announces the beginning of the output of a new polygon. 
 * <br><br>
 * <b>Role:</b> Used with the coordinate protocol.
 * <br><br>
 * @param ipoly
 * 0-based polygon index.
 * @param nnode
 * number of nodes in this polygon.
 */
  virtual void start_coordinate_polygon(int ipoly, int nnode);

/**
 * Announces one node on a polygon.
 * <br><br>
 * <b>Role:</b> Used with the coordinate protocol.
 * <br><br>
 * @param ipoly
 * 0-based polygon index.
 * @param i
 * 0-based node index within this polygon.
 * @param uv
 * %surface parameter.
 * @param iX
 * point.
 * @param N
 * normal.
 */
  virtual void announce_coordinate_polygon_node(
				int ipoly,
				int i,
				const SPApar_pos &uv,
				const SPAposition &iX,
				const SPAunit_vector &N);

/**
 * Announces the completion of the output of a polygon.
 * <br><br>
 * <b>Role:</b> Used with the coordinate protocol.
 * <br><br>
 * @param ipoly
 * 0-based polygon index.
 */
  virtual void end_coordinate_polygon(int ipoly);

	//MEMBER
/**
 * Returns the order of how the internal mesh of the faceter should be searched to provide output.
 * <br><br>
 * <b>Role:</b> The order specifies how successive polygons ought to share edges
 * and can help the application to construct the desired data format. The default
 * is to search any polygon.
 */
  virtual MESH_MANAGER_SEARCH_ORDER query_search_order();

  //SC mka 01-16-03 provided a method to set search order and implemented for indexed mesh manager.
/**
 * Sets the order of how the internal mesh of the faceter should be searched to provide output.
 * <br><br>
 * <b>Role:</b> The order specifies how successive polygons ought to share edges
 * and can help the application to construct the desired data format. The default
 * is to search any polygon. 
 * <br><br>
 * @param mmso
 * specified search order.
 */
  virtual void set_search_order(MESH_MANAGER_SEARCH_ORDER mmso); 
	//END

#if 0
// Not implemented - faceter does not check errorid yet.
/**
 * @nodoc
 */
  void set_errorid(int id);

/**
 * @nodoc
 */
  int get_errorid();
#endif

	//MEMBER
	// TOPIC(Member functions for MESH_MANAGER GLOBAL INDEXED PROTOCOL,Faceter classes)
	//
	// The following functions are specific to the 'global indexed coordinate
	// protocol'.  Node announcements give the coordinates of each node,
	// both in cartesian form and by as a parent entity pointer and a parametric
	// coordinate.  Polygon announcement is exactly like the indexed protocol
	// except that there is a single global set of polygons instead of separate
	// sets per face.
	// (See XREF(Member functions for MESH_MANAGER INDEXED PROTOCOL)).

	//MEMBER
	//
/**
 * If this function returns <tt>TRUE</tt>, the facets will be output with the global-indexed protocol.
 * <br><br>
 * <b>Role:</b> Used with the global-indexed protocol to flag requested data. The default returns <tt>FALSE</tt>. 
 */
	virtual logical need_global_indexed_polygons();

	//MEMBER
	// Signal the beginning of global mesh output.
/**
 * Announces the beginning of the output of a global mesh. 
 * <br><br>
 * <b>Role:</b> The top level entity is the top level entity of the entire mesh,
 * typically a <tt>BODY</tt>, but can be a <tt>LUMP</tt>, <tt>SHELL</tt>, or <tt>FACE</tt>. Used with the 
 * global-indexed protocol.
 * <br><br>
 * @param ent
 * top level entity.
 */
	virtual void begin_global_mesh_output(
		ENTITY *ent		// Top level entity being faceted.
		);

	//MEMBER
	// Inquiry to determine if the mesh manager needs to know the
	// item counts before starting output.
	//MEMBER
/**
 * If this function returns <tt>TRUE</tt>, the number of polygons, nodes, 
 * and node references by polygons are announced before other output.
 * <br><br>
 * <b>Role:</b> Used with the global-indexed protocol to flag requested data.
 * The default returns <tt>FALSE</tt>. 
 */
	virtual logical need_precount_of_global_indexed_polygons();

	//MEMBER
	// Announcements of individual nodes, their parent vertex, edge, or
	// face pointers, and the appropriate type of parametric coordinates.
/**
 * Announces a node on a model <tt>VERTEX</tt>.
 * <br><br>
 * <b>Role:</b> Used with the global-indexed protocol.
 * <br><br>
 * A note on overloading this function in classes derived from <tt>MESH_MANAGER</tt>: 
 * The value returned by this function is critical. The returned value must
 * be a unique identifier for this node. It will most likely be the memory address 
 * or index of this node in the application's data structure. This unique identifier 
 * will be passed as an input argument to <tt>announce_indexed_polynode</tt>.
 * <br><br>
 * @param inode
 * 0-based node index.
 * @param ver
 * model vertex.
 * @param iX
 * node coordinates.
 */
	virtual void *announce_global_node(
		int inode, VERTEX * ver, const SPAposition &iX);

/**
 * Announces a node on the interior of a model <tt>EDGE</tt> with its %curve parameter.
 * <br><br>
 * <b>Role:</b> Used with the global-indexed protocol.
 * <br><br>
 * A note on overloading this function in classes derived from <tt>MESH_MANAGER</tt>: 
 * The value returned by this function is critical. The returned value must
 * be a unique identifier for this node. It will most likely be the memory address 
 * or index of this node in the application's data structure. This unique identifier 
 * will be passed as an input argument to <tt>announce_indexed_polynode</tt>.
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
		int inode, EDGE * ed, const SPAposition &iX, double tpar);

/**
 * Announces a node on the interior of a model <tt>FACE</tt> with its %surface parameters.
 * <br><br>
 * <b>Role:</b> Used with the global-indexed protocol.
 * <br><br>
 * A note on overloading this function in classes derived from <tt>MESH_MANAGER</tt>: 
 * The value returned by this function is critical. The returned value must
 * be a unique identifier for this node. It will most likely be the memory address 
 * or index of this node in the application's data structure. This unique identifier 
 * will be passed as an input argument to <tt>announce_indexed_polynode</tt>.
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
		int inode, FACE *face, const SPAposition &iX, const SPApar_pos &uv);

	//MEMBER
	// Return the node id value that is guaranteed to be invalid.
	//   Applications using numeric indices will typically return -1.
	//	Applications using pointers will typically return 0.
/**
 * Returns the node id value that is guaranteed to be invalid.
 * <br><br>
 * <b>Role:</b> Applications using indices will typically return <tt>-1</tt>. Applications
 * using pointers will typically return <tt>0</tt>. The default returns <tt>-1</tt>.
 */
	virtual void *null_node_id();

//STI mka
        //MEMBER
        // Announce the face pointer for oncoming polygons
 /**
  * Announces the <tt>FACE</tt> pointer for oncoming polygons.
  * <br><br>
  * <b>Role:</b> Used with all protocols.
  * <br><br>
  * @param ent
  * model face pointer for the group of oncoming polygons.
  */
 	virtual void announce_polygon_model_face(
                ENTITY * ent         // Face pointer for the group of oncoming polygons.
                );

	//MEMBER
	// Signal the end of global mesh output.
/**
 * Announces the completion of the output of a global mesh. 
 * <br><br>
 * <b>Role:</b> Used with the global-indexed protocol.
 * <br><br>
 * @param ent
 * top level entity being faceted.
 */
 	virtual void end_global_mesh_output(
		ENTITY * ent		// Top level entity being faceted.
		);

/**
 * Signals the save of a global-indexed mesh.
 * <br><br>
 * @param ent
 * top level entity being faceted.
 */
	virtual void save_global_mesh_output(
		ENTITY * ent	// Top level entity being faceted.
		);
	//END

//DOCTEXT
// TOPIC(MESH MANAGER REFINEMENT CONTROL, MESH_MANAGER class;REFINEMENT class)
//
// Before starting facet construction, the faceter offers the mesh
//  manager the opportunity to revise the REFINEMENT and output format entity
//	being applied.
//
// While faceting a body, lump, or shell, check_applicable_refinement
//	is called at each member of	the hierarchy, in top down order,
//	and for each possible surface mode at each non-face member.
//	check_applicable_format_entity(..) is called once for each node.
//
// NOTE: The output format entity is used to hold a VERTEX_TEMPLATE
//  for pre-1.7 compatibility, and to provide a placeholder for future
//  enhancements.
//
//MEMBER
/**
 * Before starting facet construction with refinements,
 * the faceter offers the mesh manager the opportunity to revise the refinement.
 * <br><br>
 * <b>Role:</b> While faceting a body, lump, or shell, <tt>check_applicable_refinement</tt> is called 
 * at each member of the topological hierarchy, in top-down order, and for each 
 * possible %surface type at each non-face member.
 * <br><br>
 * The refinement passed in is what the faceter found for the given
 * %surface type. The application can assign a different refinement to it.
 * <br><br>
 * @param entity
 * entity with refinement.
 * @param mode
 * refinement %surface type.
 * @param R
 * input/output refinement.
 */
	virtual void check_applicable_refinement(
		ENTITY *entity,			// The ENTITY for which a refinement
								// revision is needed.
								// A NULL ENTITY pointer requests a default.
		AF_SURF_MODE mode,		// The surface type or class for which a
								//	refinement is desired.
		REFINEMENT *&R			// On input, the REFINEMENT determined to
								//	applicable via prior search.
								// The mesh manager may modify this or leave
								//	it unchanged.
	);

/**
 * Before starting facet construction with refinements,
 * the faceter offers the mesh manager the opportunity to revise the vertex template.
 * <br><br>
 * <b>Role:</b> While faceting a body, lump, or shell, <tt>check_applicable_format_entity</tt> 
 * is called at each member of the topological hierarchy, in top-down order.
 * <br><br>
 * The application can assign a different format to it. The output
 * format entity can be a <tt>VERTEX_TEMPLATE</tt> for pre-1.7 compatibility.
 * <br><br>
 * @param entity
 * entity with format.
 * @param format
 * input/output format entity, a <tt>VERTEX_TEMPLATE</tt>.
 */
	virtual void check_applicable_format_entity(
		ENTITY *entity,			// The ENTITY for which an output format
								// entity revision is needed.
		ENTITY *&format			// On input, the vertex template that prior
								// search rules have determined.
								// The mesh manager may modify this or leave
								// it unchanged.
	);

//MEMBER
	// check_edge_refinement is called to determine how to subdivide
	// 	an edge.
	// on input, dmax, hmax, and dNmax are the finest discretization controls
	//	found on any incident face.  check_edge_refinement may alter them
	//	to other values if desired.  It may also choose to compute
	//  the edge discretization itself.   DI calls to install edge points
	//	are described in XREF(AF_POINT).
	//
	// check_edge_refinement may return the following directives:
	//	-- AF_EDGE_RECOMPUTE - The faceter should recompute the edge
	//		discretization.
	//	-- AF_EDGE_USE_CURRENT_POINTS - The faceter should use the
	//		points its currently finds attached.  This is typically
	//		used in two cases:  (a) individual faces are being faceted,
	//		and are required to maintain discretization that is consistent
	//		with others that are not being faceted, and (b) the mesh manager
	//		is computing edge points itself;  in this case, the mesh
	//		manager attaches its own edge points and returns this code
	//		to be indicate that they must be used.
	//
	// The default implementation returns AF_EDGE_USE_CURRENT_POINTS if
	//	the number of uses is less than the number of faces.
/**
 * Determine if the faceter should recompute the facets for an edge.
 * <br><br>
 * <b>Role:</b> The parameters that control the faceting of an edge are provided.
 * They have been determined to be the finest in the discretization
 * controls of the incident faces. The default behavior of this function is not
 * to change any value, and to request the faceting to be done if the number of
 * incidents faces equals the total number of incident faces. If first number
 * is smaller, it requests any existing faceting to be used.
 * <br><br>
 * @param E
 * edge to check.
 * @param dmax
 * chordal deviation.
 * @param hmax
 * madimum chordal length.
 * @param dNmax
 * maximal angle change between chords.
 * @param nuse
 * number of incident faces being faceted.
 * @param nf
 * total number of incident faces.
 */
	virtual AF_EDGE_DIRECTIVE check_edge_refinement(
		EDGE *E,			// The edge being checked.
		double &dmax,		// finest distance-to-surface tolerance
							//	on any incident face being faceted.
		double &hmax,		// finest max-edge-length tolerance on
							//	any incident face being faceted.
		double &dNmax,		// finest normal tolerance on any incident face
							//  being faceted.
		int nuse,			// The number of incident faces being faceted.
		int nf				// The total number of incident faces.
	);

	// ************************************************************************
	// *****  Edge grading functionality is no longer supported.          *****
	// *****  The following two methods are being deprecated (June 2014)  *****
	// ************************************************************************

	//DOC
	// TOPIC(Edge Grading;MESH_MANAGER class)
	//MEMBER
	// After edges are discretized according to local accuracy criteria,
	// there is an optional second pass through the model to further
	// subdivide edges so that the faceted edge lengths on consecutive coedges
	// have controlled size relationships.
	//
	// The application enables the second pass by responding TRUE
	// to the query function need_edge_grading.

/*
 * If this function returns <tt>TRUE</tt>, there is a second pass through the model 
 * to further subdivide edges so that consecutive faceted edge lengths have controlled size relationships.
 * <br><br>
 * <b>Role:</b> The default return value is <tt>TRUE</tt> and the default value for <tt>mu</tt> is <tt>2.0</tt>, 
 * which may be changed by the application or further modified for each individual face when the face is
 * passed to <tt>need_edge_grading_on_face</tt>.
 * <br><br>
 * @param mu
 * relative size of faceted edge lengths.
 */

/**
 * @nodoc
 */
	virtual logical need_edge_grading(
			double &mu	// The default value to use for the relative
						// size.  This is set to 2.0 by the faceter.
						// The application may change it here and further
						// modify it for each individual face when
						// the face is passed to need_edge_grading_on_face.
		);

	// Edge grading is applied one face at a time during the edge grading
	// pass.   On each face, the faceter queries the mesh manager to
	// determine the acceptable size ratio between the end chords
	// of edges that share a common vertex on that face.
	// Return FALSE to suppress grading on the face.
	// The default implementation returns TRUE if and only if the
	// face refinement has a grading, and it resets the ratio mu
	// to the grid_aspect_ratio setting of the refinement.

/*
 * If this function returns <tt>TRUE</tt>, edge grading is applied one face at a time during the edge grading pass.
 * <br><br>
 * <b>Role:</b> On each pass, the faceter queries the mesh manager to determine
 * the acceptable size ratio between the end chords of edges that share a common
 * vertex on that face. Return <tt>FALSE</tt> to suppress grading on that face. The
 * default implementation returns <tt>TRUE</tt> if and only if the face refinement has a
 * grading, and it resets the aspect ratio <tt>mu</tt> to the <tt>grid_aspect_ratio</tt> setting
 * of the refinement. If <tt>mu</tt> returned earlier by <tt>need_edge_grading</tt> is less than
 * <tt>1.5</tt>, it is reset to <tt>1.5</tt>.
 * <br><br>
 * @param F
 * face on which grading is to be done.
 * @param R
 * refinement on the face.
 * @param mu
 * aspect ratio returned by <tt>need_edge_grading</tt>.
 */

/**
 * @nodoc
 */
	virtual logical need_edge_grading_on_face(
			FACE * F,		// Face on which grading is to be done.
			REFINEMENT *R,	// The refinement in effect on the face.
			double &mu		// The aspect ratio that will be applied.
							// On entry, this is set to the value returned
							// earlier by need_edge_grading.
							// Any return value less than 1.5 is raised to
							//	1.5.
		);

//END
//DOC
// TOPIC(Edge mates;MESH_MANAGER class)
//
// In the indexed mesh protocol, the application mesh manager may
// optionally receive  additional information about the 'mate' just after
// each polynode is sent.  To receive this information, the
//	MESH_MANAGER must (a) answer TRUE to the inquiry function
//	need_indexed_mate_ids() and (b) implement member functions
//	(see below) to receive the mate information.
//
// In passing mate information, we refer to a polyedge. A polyedge is
// the polygon edge immediately following a polynode, in counterclockwise
// order.  The mate of a polyedge is the polyedge that is on the
// opposite side of the same polygon edge, and is oriented in the
// opposite direction.
//
// When a polyedge is first announced, the application MESH_MANAGER returns
// a void* identifier that the faceter will use to refer to that polyedge
// if its mate is subsequently reached.
//
// Mate information is emitted immediately following the call to
//	announce_indexed_polynode(..).
//

/**
 * If this function returns <tt>TRUE</tt>, then polyedges will be announced after polynodes.
 * <br><br>
 * <b>Role:</b> Used with the indexed and global-indexed protocols. The default returns <tt>FALSE</tt>.
 */
	virtual logical need_indexed_polyedges();

/**
 * Announces an indexed polyedge that lies on a model edge.
 * <br><br>
 * <b>Role:</b> This signature announces that the polyedge immediately following this
 * polynode lies along the indicated model coedge. (Therefore, it has no 'mate' polyedge.)
 * <br><br>
 * This method is called if <tt>need_indexed_polyedges</tt> returns <tt>TRUE</tt>.
 * Used with the indexed protocol. 
 * <br><br>
 * A note on overloading this function in classes derived from <tt>MESH_MANAGER</tt>: 
 * Because this polyedge does not have a mate, the value returned by this signature
 * is not critical; however, for consistency with other signatures of this method it
 * typically returns a unique identifier for this polyedge.
 * <br><br>
 * @param ipoly
 * 0-based polygon index.
 * @param i
 * 0-based node index within the polygon.
 * @param ce
 * the coedge with which this polyedge coincides.
 */
  	virtual void *announce_indexed_polyedge(
	int ipoly,	// polygon id
	int i,		// polynode index within this polygon
	ENTITY *ce	// the coedge this polyedge parallels
	);

	// This announces a polyedge whose mate polyedge has not previously
	// been visited.
/**
 * Announces an indexed polyedge whose "mate" has not previously been announced.
 * <br><br>
 * <b>Role:</b> This method is called if <tt>need_indexed_polyedges</tt> returns <tt>TRUE</tt>.
 * Used with the indexed protocol.
 * <br><br>
 * A note on overloading this function in classes derived from <tt>MESH_MANAGER</tt>: 
 * Because this polyedge does have a mate, the value returned by this signature
 * is critical. If the mate information is to be used, the returned value must
 * be a unique identifier for this polyedge. It will most likely be the memory address 
 * or index of this polyedge in the application's data structure.
 * <br><br>
 * @param ipoly
 * 0-based polygon index.
 * @param i
 * 0-based node index within the polygon.
 */
  	virtual void *announce_indexed_polyedge(
	int ipoly,	// polygon id
	int i		// polynode index within this polygon
	);

	// This announces a polyedge and its previously visited mate.
/**
 * Announces an indexed polyedge whose "mate" has been previously announced.
 * <br><br>
 * <b>Role:</b> If the "mate" of a polyedge has previously been announced,
 * then this signature will be called. This signature allows an application
 * to connect mates in its data structure. (Mates are coincident polyedges
 * of adjacent facets.) 
 * <br><br>
 * This method is called if <tt>need_indexed_polyedges</tt> returns <tt>TRUE</tt>.
 * Used with the indexed protocol. 
 * <br><br>
 * A note on overloading this function in classes derived from <tt>MESH_MANAGER</tt>: 
 * Because the mate of this polyedge has already been processed, the value returned by 
 * this signature is not critical; however, for consistency with other signatures of 
 * this method it typically returns a unique identifier for this polyedge. If the mate 
 * information is to be used, this signature must be used to connect or associate the 
 * two polyedges. The <tt>mate</tt> value will be the one returned by the mate's 
 * <tt>announce_indexed_polyedge</tt> call. It will most likely be the memory address 
 * or index of the mate in the application's data structure.
 * <br><br>
 * @param ipoly
 * 0-based polygon index.
 * @param i
 * 0-based node index within this polygon.
 * @param mate
 * mate id is the value returned by the mate's <tt>announce_indexed_polyedge</tt> call.
 */
  	virtual void *announce_indexed_polyedge(
	int ipoly,	// polygon id
	int i,		// polynode index within this polygon 
	void *mate	// The id as returned by the prior announce_indexed_polyedge
				// call.
	);

/**
 * Returns the total number of tri strips in the mesh (including single triangle strips).
 */
	virtual int GetNumStrips() const { return 0;}

/**
 * Returns the number of strips that contain only a single triangle.
 */
	virtual int GetNumSingleTris() const { return 0;}

/**
 * @nodoc
 */
	virtual void AddNumStrips(int) {};

/**
 * @nodoc
 */
	virtual void AddNumSingleTris(int) {};

/**
 * @nodoc
 */
	virtual unsigned GetDataSize() const { return 0;}

/**
 * @nodoc
 */
	virtual void AddDataSize(unsigned) {};

	// Some applications need degenerate triangles to be topologically correct.
	// Some don't want degenerate triangles, they want as few triangles as possible
	// Default is false, returning as few triangles as possible.
/**
 * Some applications need degenerate triangles to be topologically correct.
 * <br><br>
 * <b>Role:</b> Some applications do not need degenerate triangles and want as
 * few triangles as possible. Default is <tt>FALSE</tt>, returning as few triangles as possible.
 */
	virtual logical need_degenerate_triangles() const;

//END

	// Not implemented yet. To be done in a future service pack and release.
/**
 * @nodoc
 */
	virtual logical need_single_node_at_singularity() const;


	/**
	* @nodoc.  Internal use only.  Do not overload, do not use.
	**/
	virtual af_internal_mesh_manager* get_internal_mesh_manager();

private:
	friend class LINKED_MESH_MANAGER;
	friend class AF_WORKING_FACE_SET;

/**
 * @nodoc
 */
	virtual logical need_internal_data(void *) { return FALSE; }
};

/* @} */
#endif
