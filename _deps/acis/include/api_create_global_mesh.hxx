/*******************************************************************/
/*    Copyright (c) 1989-2020 by Spatial Corp.                     */
/*    All rights reserved.                                         */
/*    Protected by U.S. Patents 5,257,205; 5,351,196; 6,369,815;   */
/*                              5,982,378; 6,462,738; 6,941,251    */
/*    Protected by European Patents 0503642; 69220263.3            */
/*    Protected by Hong Kong Patent 1008101A                       */
/*******************************************************************/

#ifndef api_create_global_mesh_hxx
#define api_create_global_mesh_hxx

/**
 * @file api_create_global_mesh.hxx
 * @CAA2Level L1
 * @CAA2Usage U1
 *! \addtogroup FCTAPI
 *  \brief Declared at <api_create_global_mesh.hxx>
 * @{
 */


#include "dcl_fct.h"
#include "base.hxx"
#include "facet_options.hxx"
#include "api.hxx"
#include "param.hxx"
#include "unitvec.hxx"
#include "position.hxx"

class ENTITY;
class VERTEX;
class EDGE;
class FACE;

/**
*  This class provides an interface for streaming a global mesh to a customer mesh manager.
*  It is intended for use with api_create_global_mesh.  
*
*  Please see the technical article api_create_global_meshes at http://doc.spatial.com/ for more detailed information
*  and sample code.
*
* For backward compatibility reasons, the method signatures are the same as those relevant to 
* global meshes from the class MESH_MANAGER.  Some of the parameters are no longer particularly meaningful
* (e.g., begin/end_mesh_output provide a pointer to the entity being faceted, as well as the refinement
* in place and the vertex template in place, even though refinements and vertex templates are no longer recommended).
*
* The sequence of events in api_create_global_mesh is as follows:
*
* 1) The faceter is called in multithreaded mode to produce a mesh on each face in multithreaded mode (if the customer has activated threading),
* 2) announce_counts is called.
* 3) Each node (facet point) in the mesh is announced using one of the signuatures of announce_global_node.
* 4) For each face
*		a) announce_polygon_model_face is called.
*		b) For each polygon on that face
*			i) start_indexed_polygon is called
*			ii) announce_indexed_polynode is called (both signatures) for each polynode in the polygon.
*			iii) end_indexed polygon is called
**/
class DECL_FCT af_global_mesh_manager : public ACIS_OBJECT
{
protected:
	~af_global_mesh_manager(){}
public:

	/**
	* Called to notify the user of the data size so that memory can be preallocated.
	**/
	virtual void announce_counts( int npolygons, int nnodes, int npolynodes) 
	{
		SPAUNUSED(npolygons);
		SPAUNUSED(nnodes);
		SPAUNUSED(npolynodes);
	}

	/**
	* This method is called for each vertex on the model to provide the information about 
	* vertex nodes on the mesh.
	**/
	virtual void* announce_global_node(int inode, VERTEX *ver, const SPAposition &Xi)=0;
	/**
	* This method is called for each mesh node on an EDGE.
	**/
	virtual void* announce_global_node(int inode, EDGE* mod_edge, const SPAposition &Xi,double t)=0;
	/**
	* This method is called for each facet node on a FACE.
	**/
	virtual void* announce_global_node(int inode, FACE* mod_face, const SPAposition &Xi, const SPApar_pos &uv)=0;

	/**
	* This method is called at the beginning of the process of announcing triangles for a face ent.  
	*
	* This method does not need to be used.
	**/
	virtual void announce_polygon_model_face(ENTITY* ent){ SPAUNUSED(ent); }

	/**
	* This method is called for each polynode (polygonal half-edge) in the mesh.  It provides full detailed geometric information
	* about the polynonde.
	*
	* Customers will need to implement at least one signature of <tt>announce_indexed_polynode</tt> in order to
	* get the triangles in the mesh.
	**/
	virtual void announce_indexed_polynode(
					ENTITY* ent,				// entity containing the polygon 
					int ipoly,					// which polygon in the mesh is this polynode a part of
					int i,						// i is the index which says that this is the first, second, etc, polynode of this polygon
					void* idptr,				// idptr is the label of the global node that the user returned from announce_global_node.
					const double &edge_tpar,	// DBL_MAX if the point is not on an edge, or the curve parameter for the point if it is on the edge.
					const SPApar_pos &uv,		// par pos on the face.
					const SPAposition &pos,		// position of the node
					const SPAunit_vector &uvec) // normal at the node.
	{
		SPAUNUSED(ent);
		SPAUNUSED(ipoly);
		SPAUNUSED(i);
		SPAUNUSED(idptr);
		SPAUNUSED(edge_tpar);
		SPAUNUSED(uv);
		SPAUNUSED(pos);
		SPAUNUSED(uvec);
	}

	/**
	* This method is called for each polynode (polygonal half-edge) in the mesh.  It provides summary information
	* about the polynode.  The first parameter-- ipoly-- indicates which polygon this node is in, the second --i -- indicates
	* the location of this polynode in the polygon (unique only up to cyclic permutation), and the third parameter --pnode --
	* provides a pointer to the node value the client returned from announce_global_node.
	*
	* This simpler version of <tt>announce_indexed_polynode</tt> provides minimal information, an ordinal numbber
	* for the polygon <tt>ipoly</tt>, an ordinal number <tt>i</tt> within the polygon and a label of a polynode.
	**/
	virtual void announce_indexed_polynode(int ipoly, int i, void *pnode) { SPAUNUSED(ipoly); SPAUNUSED(i); SPAUNUSED(pnode); }

	/**
	* Called once per polygon, before any of the polynodes are announced.
	*
	* Typically customers should implement this, but in rare cases it may be omitted.
	**/
	virtual void start_indexed_polygon(int ipoly, int npolynode, int ishare){SPAUNUSED(ipoly);SPAUNUSED(npolynode);SPAUNUSED(ishare);}

	/**
	* Called once per polygon, after all polynodes are announced.
	*
	* Typically customers should implement this, but in rare cases it may be omitted.
	**/
	virtual void end_indexed_polygon(int iPoly){SPAUNUSED(iPoly);}

	/**
	* Allows customers a hook to call their code at the beginning of announcing a mesh.
	*
	* This is optional.
	**/
	virtual void	begin_global_mesh_output(ENTITY *pEntity){SPAUNUSED(pEntity);}

	/**
	* Allows customers a hook to call their code at the beginning of announcing a mesh.
	*
	* This is optional.
	**/
	virtual void	end_global_mesh_output(ENTITY *pEntity){SPAUNUSED(pEntity);}
};

#include "SPAuse_counted_impl_holder.h"
class create_global_mesh_impl;

/**
*  The <tt>create_global_mesh_options</tt> class allows you to customize the behavior of <tt>api_create_global_mesh</tt>
*  and <tt>api_create_global_meshes</tt>.  It holds a <tt>facet_options</tt> object which functions the same way as it
*  does for the other faceter APIs.  
*
*  It also allows you to do more advanced workflows.  These APIs create global meshes by making per face indexed meshes with edge
*  index data, and combining them into a global mesh per <tt>BODY</tt>.  If desired, you can keep both the indexed meshes and
*  global meshes.  Also, you can use the APIs to create a global mesh from existing indexed meshes if you set the options correctly:
*  specifically, the faceter must have been called using the default mesh manager (c.f., <tt>api_set_mesh_manager_to_default</tt>),
*  and need_edge_indices must have been set to <tt>TRUE</tt>.  (The functions for requesting and accessing edge index data are declared in
*  api_create_global_mesh.hxx.)  This could allow, e.g., incremental faceting and use of global meshes.
*
*  Please see the technical article api_create_global_meshes at http://doc.spatial.com/ for more detailed information
*  and sample code.
*
**/
class DECL_FCT create_global_mesh_options
{
public:
	/**
	* Get the facet options associated with this options object.
	**/ 
	facet_options* get_facet_options();
	/**
	* Get the facet options associated with this options object.
	**/ 
	facet_options const* get_facet_options() const;
	/**
	* Set the facet options associated with this options object.
	**/
	void set_facet_options(facet_options* fo);

	/**
	* This api creates a global mesh by first putting an indexed mesh on each
	* face then stitching them together.  By default, the API deletes the indexed
	* meshes to save memory.  It is possible to keep these for other uses later.
	**/
	void set_keep_indexed_meshes( logical tf );

	/**
	* Get the value of the keep indexed meshes flag.
	**/
	logical get_keep_indexed_meshes() const;

	/**
	* By default, api_create_global_mesh facets a body, then combines the 
	* resulting indexed meshes into a global mesh.  If the input already has
	* indexed meshes attached to all faces, this option allows the API
	* to use the existing meshes.
	*
	* This option is considered an expert workflow.  If any meshes are missing,
	* the mesh manager was not the default (set by calling api_set_mesh_manager_to_default)
	* or if the edge data was not enabled (set by calling set_need_edge_data(<tt>TRUE</tt>) ),
	* the API call will fail.
	**/
	void set_use_existing_indexed_meshes( logical tf );
	/**
	* Returns the value of the use_exisitng_meshes_flag.
	**/ 
	logical use_existing_indexed_meshes() const;

	/**
	* Constructor
	**/
	create_global_mesh_options();
	/**
	* Constructor
	**/
	create_global_mesh_options(create_global_mesh_options const&);
	/**
	* Assignment operator
	**/
	create_global_mesh_options& operator=(create_global_mesh_options const&);
	/**
	* Destructor
	**/
	~create_global_mesh_options();

	/**
	* @nodoc
	**/
	create_global_mesh_impl* get_impl();
	/**
	* @nodoc
	**/
	create_global_mesh_impl const* get_impl() const;
private:
	SPAuse_counted_impl_holder m_holder;
};

/**
* Facets body and announces results to a global mesh.  This api is recommended over setting the default mesh manager and using api_facet_entity,
* because it provides support for multithreading.
*
* If no options object is provided, surface tolerance is set to 1/500 of bounding box diagonal, normal tolerance is set to 30, and
* all other tolerances are not specified.
*
*  Please see the technical article api_create_global_meshes at https://doc.spatial.com/get_doc_page/articles/u/s/i/Using_api_create_global_meshes.html for more detailed information
*  and sample code.
**/
DECL_FCT outcome api_create_global_mesh( BODY* body, af_global_mesh_manager& MM, create_global_mesh_options* cgmo=NULL, AcisOptions* ao=NULL );

/**
* Multibody version of api_create_global_mesh.  Allows parallelism at a higher level.  Prefer this api when faceting multiple bodies.
* There is no harm to calling this API with an entity list of length 1.  
*
* Precondition: for each entity in the bodies list, it must be its own owner.
**/
DECL_FCT outcome api_create_global_meshes( ENTITY_LIST const& bodies, af_global_mesh_manager& MM, create_global_mesh_options* cgmo=NULL, AcisOptions* ao=NULL );

/**
* Facets the selected faces of body and announces results to a global mesh.  
* If no options object is provided, surface tolerance is set to 1/500 of bounding box diagonal, normal tolerance is set to 30, and
* all other tolerances are not specified.
*
*  Please see the technical article api_create_global_meshes at https://doc.spatial.com/get_doc_page/articles/u/s/i/Using_api_create_global_meshes.html for more detailed information
*  and sample code.
**/
DECL_FCT outcome api_create_global_mesh(ENTITY* body, ENTITY_LIST const& faces, af_global_mesh_manager& MM, create_global_mesh_options* cgmo=nullptr, AcisOptions* ao=nullptr);

#include "logical.h"
#include "dcl_fct.h"
#include "container_utils.hxx"
class FACE;
class COEDGE;


// The following functions pertain to edge index information.  An indexed mesh consists of two arrays:
// one storing node data (positions, and possibly normals and uvs), the other storing triangles
// as blocks indices into the node data array.  Edge index data refers to lists which specify
// which nodes of an indexed mesh occur on which edge.


/**
* Returns the value of a flag indicating whether the default mesh manager
* retains for edge index information.
**/
DECL_FCT logical need_edge_indices();

/**
* Allows you to set the flag determining whether default mesh manager will collect 
* edge index information.
**/
DECL_FCT void set_need_edge_indices( logical tf );

/**
* Called after faceting a function, it queries the mesh attributes to 
* get the indices of the points on the given coedge within the mesh.
*
* Will return an empty array, unless you have previously called set_need_edge_indices(TRUE).
*
* If nonempty, the returned array gives a list of the points in c->face()'s mesh which are incident
* to c.  The indices are listed in order of traversal along the coedge.
**/
DECL_FCT void get_coedge_indices( COEDGE* c, SPAint_vector& out_indices);

/** @} */

#endif
