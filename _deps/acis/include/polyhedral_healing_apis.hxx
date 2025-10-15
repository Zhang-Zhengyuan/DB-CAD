/*******************************************************************/
/*    Copyright (c) 1989-2020 by Spatial Corp.                     */
/*    All rights reserved.                                         */
/*    Protected by U.S. Patents 5,257,205; 5,351,196; 6,369,815;   */
/*                              5,982,378; 6,462,738; 6,941,251    */
/*    Protected by European Patents 0503642; 69220263.3            */
/*    Protected by Hong Kong Patent 1008101A                       */
/*******************************************************************/
#ifndef polyhedral_healing_apis_hxx
#define polyhedral_healing_apis_hxx

#include "dcl_polyhlc.h"
#include "api.hxx"
#include "SPAuse_counted_impl_holder.h"

class BODY;
class EDGE;
class FACE;
class insanity_list;
class cover_circuit_options;

/**
 * @file polyhedral_healing_apis.hxx
 * @CAA2Level L1
 * @CAA2Usage U1
 * \defgroup POLYHEALAPI Polyhedral Healing
 * \ingroup POLYHLAPI 
 * @{
 */

/**
* <tt>polyhedral_decimate_options</tt> allows customization of behavior for <tt>api_polyhedral_decimate</tt>.
**/
class DECL_POLYHLC polyhedral_decimate_options : public ACIS_OBJECT
{
	SPAuse_counted_impl_holder m_holder;
public:

	/** 
	* Disallow triangle edges on the boundary from being decimated.  True by default. 
	**/
	void set_freeze_boundary( bool do_freeze );

	/** 
	* Query whether triangle edges on the boundary are frozen during decimation.  True by default. 
	**/
	bool get_freeze_boundary() const;

	/**
	* Use a number of triangles as the criterion for decimation.  By default, only the sag value is used for controlling the decimation operation.  
	* If num_target_triangles is set to a positive value, the sag value is ignored.
	**/
	void set_num_target_triangles(int num_target_triangles);

	/**
	* Get the number of target triangles to decimate down to.
	**/
	int get_num_target_triangles() const;

	polyhedral_decimate_options();

	/**
	* @nodoc. do not use
	**/
	class Impl;
	/**
	* @nodoc. do not use
	**/
	Impl* get_impl();
	/**
	* @nodoc. do not use
	**/
	Impl const* get_impl() const;
};

/**
* Decimates the polyhedral geometry of an ACIS body.
* <b>Technical Article:</b> <i>[Decimation](http://doc.spatial.com/articles/d/e/c/Decimation.html)</i>
* <br><br>
* <b>Role: </b> This API reduces the number of triangles ("decimation") in the polyhedral geometry underneath the input ACIS body. 
* The amount of decimation is controlled by the input sag value, which determines 
* the amount by which the output mesh is allowed to deviate from the input mesh.
* <br><br>
* <b>Errors:</b>
* <br><br>
* <b>Limitations:</b>
* <br><br>
* <b>Journal:</b> Available
* <br><br>
* <b>Product(s):</b> 3D ACIS Polyhedral
* <br><br>
* @param body
* ACIS BODY with polyhedral mesh geometry
* @param sag
* Double value specifying the maximum allowed deviation between the output mesh and the input mesh
* @param pdc
* polyhedral_decimate_options
* @param ao
* ACIS options.
**/
DECL_POLYHLC outcome api_polyhedral_decimate( BODY* body,
											  double sag,
											  polyhedral_decimate_options* pdc = NULL,
											  AcisOptions* ao = NULL);

/**
* This object stores a range of input nodes (identified by which face
* and which node in the face) correspond to a given output node.
**/
class DECL_POLYHLC polyhedral_face_index_pair_container
{
public:
	/**
	* Returns the number of nodes in this object.
	**/
	int get_size() const;
	/**
	* Returns the face corresponding to a point in this object.  Valid
	* values for which_point are 0 up to get_size()-1.
	**/
	FACE* get_face(int which_point) const;
	/**
	* Returns the index corresponding to a point in this object.  Valid
	* values for which_point are 0 up to get_size()-1.
	**/
	int get_index(int which_point) const;

	struct Impl;
	/**
	* @nodoc.  Internal use only.
	**/
	polyhedral_face_index_pair_container(Impl* impl);
	/**
	* @nodoc.  Internal use only.
	**/
	Impl* get_impl();
private:
	SPAuse_counted_impl_holder _impl;
};
/**
* This class tracks the relationship between nodes in the output polyhedral body
* and nodes in the input polyhedral body
**/
class DECL_POLYHLC polyhedral_node_map
{
public:
	/**
	* Given an output point in a polyhedral body, identified by which face it is on and which node
	* index it has in the face, this method returns an object to provide access to the corresponding input points.
	**/
	polyhedral_face_index_pair_container get_input_nodes(FACE* output_face, int output_node) const;

	struct Impl;
	/**
	* @nodoc.  Internal use only.
	**/
	polyhedral_node_map(Impl* impl);
	/**
	* @nodoc.  Internal use only.
	**/
	Impl* get_impl();
	/**
	* @nodoc.  Internal use only.
	**/
	Impl const* get_impl() const;

private:
	SPAuse_counted_impl_holder _impl;
};


/**
* <tt>polyhedral_fix_orientation_options</tt> allows customization of behavior for <tt>api_polyhedral_fix_orientation</tt>.
**/
class DECL_POLYHLC polyhedral_fix_orientation_options : public ACIS_OBJECT
{
public:
	/**
	* If this flag is enabled (true by default), the API checks whether the body is inside out, and flips
	* it if it is.
	**/
	void set_fix_global_orientation(bool do_fix);
	/**
	*@nodoc
	**/
	bool get_fix_global_orientation() const;
	/**
	* If this flag is enabled (true by default), the API checks whether any triangle has a neighboring triangle
	* with opposite orientation and attempts to flip it.  Note that not every mesh is orientable (e.g., Moebius band); 
	* the API will fail if it cannot orient the input consistently.
	**/
	void set_fix_triangle_consistency(bool do_fix);
	/**
	*@nodoc
	**/
	bool get_fix_triangle_consistency() const;
	/**
	* If this flag is enabled (true by default), the API removes duplicate triangles (those with exactly the same vertices).
	**/
	void set_fix_duplicate_triangles(bool do_fix);
	/**
	*@nodoc
	**/
	bool get_fix_duplicate_triangles() const;
	/**
	* If this flag is enabled (false by default), the API checks if there are nonmanifold edges (those touching more than 2 triangles), and attempts
	* to remove them by contracting them.  Note that contraction may not always be possible; in these cases the API will
	* instead attempt other simplifications.
	**/
	void set_fix_nonmanifold_edges(bool do_fix);
	/**
	*@nodoc
	**/
	bool get_fix_nonmanifold_edges() const;

	/**
	*@nodoc
	**/
	polyhedral_fix_orientation_options();
	/**
	*@nodoc
	**/
	~polyhedral_fix_orientation_options();
	/**
	*@nodoc
	**/
	polyhedral_fix_orientation_options(polyhedral_fix_orientation_options const&);
	/**
	*@nodoc
	**/
	polyhedral_fix_orientation_options& operator=(polyhedral_fix_orientation_options const&);

	bool get_need_polyhedral_node_map() const;
	void set_need_polyhedral_node_map(bool need_map);

	polyhedral_node_map get_node_map() const;

	class Impl;
private:
	Impl* impl;
public:
	/**
	*@nodoc
	* Internal use only.
	**/
	Impl* get_impl();
	/**
	*@nodoc
	* Internal use only.
	**/
	Impl const* get_impl() const;
};

/**
* Heals inconsistent triangle orientation.
* <b>Technical Article:</b> <i>[Polyhedral Healing](https://doc.spatial.com/get_doc_page/articles/h/e/a/Healing_in_CGM_Polyhedra_dde3.html)</i>
* <br><br>
* <b>Role: </b> This API consistently orients the triangles in the polyhedral geometry underneath the input ACIS body. 
* This API modifies the input ACIS body.
* <br><br>
* <b>Errors:</b>
* <br><br>
* <b>Limitations:</b>  
* <ul>
* <li> Note that not every mesh is orientable (e.g., Moebius band); the API will fail if it cannot orient the input consistently. </li>
* <li> The high level topology will not be maintained after the call to this API. The result body will consist of a single mesh surface. </li>
* </ul>
* <br><br>
* <b>Journal:</b> Available
* <br><br>
* <b>Product(s):</b> 3D ACIS Polyhedral
* <br><br>
* @param body
* ACIS BODY with polyhedral mesh geometry
* @param pofo
* polyhedral_fix_orientation_options to control the behavior of the API
* @param ao
* ACIS options.
**/
DECL_POLYHLC outcome api_polyhedral_fix_orientation(BODY* body, polyhedral_fix_orientation_options* pofo = NULL, AcisOptions* ao = NULL);


/**
* <tt>polyhedral_heal_by_voxel_options</tt> allows customization of behavior for <tt>api_polyhedral_heal_by_voxel</tt>.
**/
class DECL_POLYHLC polyhedral_heal_by_voxel_options : public ACIS_OBJECT
{

	SPAuse_counted_impl_holder m_holder;
public:

	polyhedral_heal_by_voxel_options();

	/**
	* Specifies the level of detail on a logarithmic scale.
	* The recommended values are between 6 and 12. 
	* Using values near 6 will cause the operator to run fast and give a lightweight, coarse approximation 
	* to the result, while higher values increase accuracy and cause the operator to take longer to run. 
	* Too high a value can use an excessive amount of memory.
	* The default value is -1, in which case the algorithm computes an appropriate value internally.
	**/
	void set_level_of_detail(int lod);

	/**
	* Returns the level of detail.
	**/
	int get_level_of_detail() const;

	/**
	* Returns the internal voxel size used for the current level of detail.
	**/
	double get_voxel_size(BODY* body) const;

	/** 
	* The <tt> normal_tolerance</tt> is an angle used during the step of the algorithm that converts voxels to triangles (aka contouring). 
	* The recommended values are between 0 and 15; lower values will result in more triangles in the output and better preservation of features.
	* A value of 0 will result in no simplification.  The default value is 15.
	**/
	void set_normal_tolerance( double angle_in_degrees );
	/** 
	* Returns the normal tolerance used by the internal simplification algorithm. 
	**/
	double get_normal_tolerance() const;

	/**
	*@nodoc
	**/
	~polyhedral_heal_by_voxel_options();
	/**
	*@nodoc
	**/
	polyhedral_heal_by_voxel_options(polyhedral_heal_by_voxel_options const&);
	/**
	*@nodoc
	**/
	polyhedral_heal_by_voxel_options& operator=(polyhedral_heal_by_voxel_options const&);


	class Impl;
private:
	Impl* impl;
public:
	/**
	*@nodoc
	* Internal use only.
	**/
	Impl* get_impl();
	/**
	*@nodoc
	* Internal use only.
	**/
	Impl const* get_impl() const;

};

/**
* Reconstructs the polyhedral mesh geometry using a voxel approximation of the input body.
* <b>Technical Article:</b> <i>[Polyhedral Healing](https://doc.spatial.com/get_doc_page/articles/h/e/a/Healing_in_CGM_Polyhedra_dde3.html)</i>
* <br><br>
* <b>Role: </b> This API remeshes the input body by creating a volumetric representation of it, then contouring a level set of this representation.
* The output converges to the shape of the input as <tt>level_of_detail</tt> increases.  See polyhedral_heal_by_voxel_options.
* <br><br>
* <b>Errors:</b>
* <br><br>
* <b>Limitations:</b> 
* <ul> 
* <li> The high level topology will not be maintained after the call to this API. The result body will consist of a single mesh surface. </li>
* <li> Sharp features on the input body may be lost, especially when the level-of-detail is small (8 or less). Similarly, features that are too small to represent on the voxel grid (for the given level of detail) will be lost. </li>
* <li> If the input is one or many sheet bodies, there is no guarantee that the output will represent the original geometry in any way; the output of this operator is always a collection of watertight solids. </li>
* <li> For input with a large surface area-to-volume ratio (such as a lattice), the performance may drastically decrease. This is due to the fact that the operator builds a sparse volumetric representation of the body, and the advantage of sparsity is lost with such input.</li>
* </ul>
* <br><br>
* <b>Journal:</b> Available
* <br><br>
* <b>Product(s):</b> 3D ACIS Polyhedral
* <br><br>
* @param body
* ACIS BODY with polyhedral mesh geometry
* @param hbvo
* polyhedral_heal_by_voxel_options
* @param ao
* ACIS options.
**/
DECL_POLYHLC outcome api_polyhedral_heal_by_voxel(BODY* body, polyhedral_heal_by_voxel_options* hbvo = NULL, AcisOptions* ao = NULL);


/**
* Splits the input body into multiple bodies.
* <b>Technical Article:</b> <i>[Polyhedral Healing](https://doc.spatial.com/get_doc_page/articles/h/e/a/Healing_in_CGM_Polyhedra_dde3.html)</i>
* <br><br>
* <b>Role: </b> This API splits the input polyhedral body into one body for each face in the input body.
* <br><br>
* <b>Errors:</b>
* <br><br>
* <b>Limitations:</b> 
* <br><br>
* <b>Journal:</b> Available
* <br><br>
* <b>Product(s):</b> 3D ACIS Polyhedral
* <br><br>
* @param body
* ACIS BODY with polyhedral geometry
* @param output_bodies
* ENTITY_LIST of separated output bodies
* @param ao
* ACIS options.
**/
DECL_POLYHLC outcome api_polyhedral_separate_all_faces(BODY* body, ENTITY_LIST& output_bodies, AcisOptions* ao = NULL);

class polyhedral_discard_bad_facets_options;
/**
* Removes the facets marked as input insanities from the mesh geometry. 
* <b>Technical Article:</b> <i>[Polyhedral Healing](https://doc.spatial.com/get_doc_page/articles/h/e/a/Healing_in_CGM_Polyhedra_dde3.html)</i>
* <br><br>
* <b>Role: </b> This API removes the bad facets from the insanity list generated by an internal call to the checker. 
* <br><br>
* <b>Errors:</b>
* <br><br>
* <b>Limitations:</b> 
* <br><br>
* <b>Journal:</b> Available
* <br><br>
* <b>Product(s):</b> 3D ACIS Polyhedral
* <br><br>
* @param body
* ACIS BODY with polyhedral mesh geometry
* @param insanities
* insanity_list
* @param pdbfo
* polyhedral_discard_bad_facets_options  (for future use)
* @param ao
* ACIS options.
**/
DECL_POLYHLC outcome api_polyhedral_discard_bad_facets(BODY* body, insanity_list* insanities, polyhedral_discard_bad_facets_options* pdbfo = NULL, AcisOptions* ao = NULL);

class polyhedral_remove_cracks_options;
/**
* Fixes thin cracks in the input polyhedral geometry.
* <b>Technical Article:</b> <i>[Polyhedral Healing](https://doc.spatial.com/get_doc_page/articles/h/e/a/Healing_in_CGM_Polyhedra_dde3.html)</i>
* <br><br>
* <b>Role: </b> Finds all geometrically 3- or 4-sided holes in the mesh and attempts to fill or remove them.  
* The <tt>polyhedral_remove_cracks_options</tt> parameter is for future expansion.
* <br><br>
* <b>Errors:</b>
* <br><br>
* <b>Limitations:</b> 
* <ul> 
* <li> The high level topology may not be maintained after the call to this API. The result body will consist of a single mesh surface. </li>
* </ul>
* <br><br>
* <b>Journal:</b> Available
* <br><br>
* <b>Product(s):</b> 3D ACIS Polyhedral
* <br><br>
* @param body
* ACIS BODY with polyhedral mesh geometry
* @param max_gap
* Double value specifying the size of the maximum gap that can be removed
* @param prco
* polyhedral_remove_cracks_options  (for future use)
* @param ao
* ACIS options.
**/
DECL_POLYHLC outcome api_polyhedral_remove_cracks(BODY* body, double max_gap, polyhedral_remove_cracks_options* prco=NULL, AcisOptions* ao=NULL);

/**
* Creates a covering surface for a given open circuit.
* <b>Technical Article:</b> <i>[Polyhedral Healing](https://doc.spatial.com/get_doc_page/articles/h/e/a/Healing_in_CGM_Polyhedra_dde3.html)</i>
* <br><br>
* <b>Role:</b> Covers a given open circuit. This API returns the body generated to cover the hole.  
* The cover surface is returned as a separate ACIS BODY and is not stitched into the input BODY.
* If multiple circuits need to be covered and stitched, use <tt>api_polyhedral_cover_all_circuits</tt>. 
* <br><br>
* <b>Errors:</b>
* <br><br>
* <b>Limitations:</b> Input body must be manifold.
* <br><br>
* <b>Journal:</b> Available
* <br><br>
* <b>Product(s):</b> 3D ACIS Polyhedral
* <br><br>
* @param an_edge_in_circuit
* An EDGE that is part of an open circuit.
* @param out_body
* Output ACIS BODY that forms the cover 
* @param cco
* cover_circuit_options (for future use)
* @param ao
* ACIS options.
**/
DECL_POLYHLC outcome api_polyhedral_cover_circuit( EDGE* an_edge_in_circuit, BODY*& out_body, cover_circuit_options* cco = NULL, AcisOptions* ao = NULL);

/**
* Detects and covers all the open circuits in the given polyhedral ACIS BODY.
* <b>Technical Article:</b> <i>[Polyhedral Healing](https://doc.spatial.com/get_doc_page/articles/h/e/a/Healing_in_CGM_Polyhedra_dde3.html)</i>
* <br><br>
* <b>Role:</b> All open circuits in ACIS body with polyhedral mesh geometry are detected and covered.
* The covering surfaces are stitched into the input BODY. 
* <br>The topology of the input BODY may not be preserved after the operation.
* <br><br>
* <b>Errors:</b>
* <br><br>
* <b>Limitations:</b> Input body must be manifold.
* <br><br>
* <b>Journal:</b> Available
* <br><br>
* <b>Product(s):</b> 3D ACIS Polyhedral
* <br><br>
* @param body
* Input ACIS BODY
* @param cco
* cover_circuit_options (for future use)
* @param ao
* ACIS options.
**/
DECL_POLYHLC outcome api_polyhedral_cover_all_circuits( BODY*& body, cover_circuit_options* cco = NULL, AcisOptions* ao = NULL);

class polyhedral_remove_inter_sliver_options;
/**
* Detects and removes sliver triangles that have improper intersections in the given polyhedral ACIS BODY.
* <b>Technical Article:</b> <i>[Polyhedral Healing](https://doc.spatial.com/get_doc_page/articles/h/e/a/Healing_in_CGM_Polyhedra_dde3.html)</i>
* <br><br>
* <b>Role:</b> All sliver triangles that contribute to improper triangle-triangle intersections are detected and removed.
* <br><br>
* <b>Errors:</b>
* <br><br>
* <b>Limitations:</b> Input body must be manifold.
* <br><br>
* <b>Journal:</b> Available
* <br><br>
* <b>Product(s):</b> 3D ACIS Polyhedral
* <br><br>
* @param body
* Input ACIS BODY
* @param prso
* polyhedral_remove_inter_sliver_options (for future use)
* @param ao
* ACIS options.
**/
DECL_POLYHLC outcome api_polyhedral_remove_intersecting_slivers(BODY* body, polyhedral_remove_inter_sliver_options* prso = nullptr, const AcisOptions* ao = nullptr);


/** @} */
#endif
