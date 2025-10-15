/*******************************************************************/
/*    Copyright (c) 1989-2020 by Spatial Corp.                     */
/*    All rights reserved.                                         */
/*    Protected by U.S. Patents 5,257,205; 5,351,196; 6,369,815;   */
/*                              5,982,378; 6,462,738; 6,941,251    */
/*    Protected by European Patents 0503642; 69220263.3            */
/*    Protected by Hong Kong Patent 1008101A                       */
/*******************************************************************/
#ifndef polyhedral_creation_apis_hxx
#define polyhedral_creation_apis_hxx

#include "dcl_polyhlc.h"
#include "api.hxx"

class BODY;

/**
 * \defgroup POLYHLAPI Polyhedral
 *
 */
/**
 * @file polyhedral_creation_apis.hxx
 * @CAA2Level L1
 * @CAA2Usage U1
 * \defgroup POLYCRTAPI Polyhedral Creation
 * \ingroup POLYHLAPI
 * @{
 */

/**
* Options for the creation of a polyhedral body. 
* <br>
* <b>Role :</b> This class contains information to be used to control
* the creation of a polyhedral body.
* <br>
**/
class DECL_POLYHLC make_polyhedral_body_options : public ACIS_OBJECT
{
public:
	class Impl;
private:
	Impl *impl;
public:
	
	/**
	* Default Constructor
	**/
	make_polyhedral_body_options();

	/**
	* Destructor
	**/
	~make_polyhedral_body_options();

	/**
	* @nodoc
	**/
	make_polyhedral_body_options& operator=( make_polyhedral_body_options const& other );

	/**
	* Copy constructor
	**/
	make_polyhedral_body_options( make_polyhedral_body_options const& other );

	/**
	*  The surface tolerance controls the maximum deviation between the polybody and the B-rep body.
	*
	* The unset/unspecified value for surf_tol is -1; if unset, the API will use as a default value
	* 1/500 of the bounding box diagonal for the body.
	**/
	void set_surface_tolerance(double surf_tol);
	/**
	* Returns the surface tolerance used.
	**/
	double get_surface_tolerance() const;

	/**
	* Specifies the maximum length of a facet edge.  The default is 0, which means that
	* no prespecified upper bound on facet edge length is used.
	**/
	void set_max_edge_length(double max_edge_length);
	/**
	* Returns the maximum edge length.
	**/
	double get_max_edge_length() const;

	/**
	* Specifies whether all the topology from the input B-Rep body needs to be preserved 
	* for the newly constructed polyhedral body or not. For example, if the input B-Rep body
	* has multiple faces and edges, the output polyhedral body will have only one face if this flag is set to FALSE. 
	* By default, the topology is preserved. The default value of this flag is TRUE.
	**/
	void set_preserve_topology(bool preserve_topology);

	/**
	* Returns whether the topology will be preserved in the new polyhedral body.
	**/
	bool get_preserve_topology();

	/**
	* Specifies whether duplicate nodes in the input triangular information should be removed or not while
	* constructing the ACIS polyhedral body. The default value of this flag is TRUE.
	**/
	void set_remove_duplicate_nodes(bool rem_dup_nodes);

	/**
	* Returns whether the remove-duplicate-nodes is enabled or not.
	**/
	bool get_remove_duplicate_nodes();

	/**
	* Specifies the positional tolerance to be used for removal of duplicate nodes. Default value is SPAresabs.
	**/
	void set_remove_duplicate_nodes_tolerance(double positional_tol = SPAresabs);

	/**
	* Returns the tolerance that will be used for removal of duplicate nodes.
	**/
	double get_remove_duplicate_nodes_tolerance();

	/**
	* @nodoc
	**/
	Impl* get_impl();
};

class polyhedral_default_options_impl;
/**
* Options class that encapsulates the parameters used implicitly by the polyhedral component.
* <br>
* <b>Role :</b> This class contains information that is used by the polyhedral component to control
* the quality and performance of operations such boolean, offsetting, and distance operations.
* APIs that support <tt>Hybrid Modeling</tt> such as booleans, entity-entity clash etc. rely on 
* converting the input body into a polyhedral body during the operation. The quality of the  
* polyhedral mesh geometry depends on the quality of faceting. This can be controlled by setting 
* the <tt>polyhedral_default_options</tt> object. The methods of this class can throw for bad inputs
* so clients must keep this in mind.  No ENTITYs are modified.
* <br>
**/
class DECL_POLYHLC polyhedral_default_options
{
public:

	/**
	* Returns the polyhedral_default_options object.
	**/
	static polyhedral_default_options& get();

	/**
	* Returns the options object that controls the quality of the polyhedral mesh creation.
	**/
	virtual make_polyhedral_body_options get_make_poly_body_options() const= 0;

	/**
	* Specifies the options object to control the quality of the polyhedral mesh creation.
	**/
	virtual void set_make_poly_body_options(make_polyhedral_body_options const& opts) = 0;

	/**
	* Returns the level of detail this is used for operations such as offsetting and healing.
	**/
	virtual int get_level_of_detail() const = 0;

	/**
	* Specifies the level of detail that is to be used for operations such as offsetting and healing.
	* The recommended value for the level of detail is an integer between 6 and 12. 
	* Using a value of 6 will result in a coarse mesh and will be performant, while 
	* a value of 12 will result in super-fine mesh and slow performance. 
	* The default value of the level of detail is -1, which means that the algorithm
	* computes an appropriate value internally.
	**/
	virtual void set_level_of_detail(int max_lod)=0;

	/**
	* @nodoc
	**/
	static polyhedral_default_options_impl& get_impl();

protected:
	polyhedral_default_options(); 
	virtual ~polyhedral_default_options(){}
private:
	polyhedral_default_options(polyhedral_default_options const&);
	polyhedral_default_options& operator=(polyhedral_default_options const&);
};

/**
* Makes an ACIS BODY that contains polyhedral mesh geometry.
* <b>Technical Article:</b> <i>[Creating Polyhedral Bodies](http://doc.spatial.com/articles/c/r/e/Creating_Polyhedral_Bodies_d4f0.html)</i>
* <br><br>
* <b>Role:</b> This API constructs an ACIS body with polyhedral mesh geometry using an input ACIS body.
* The input ACIS body can have one or more faces with each surface geometry being an analytic or spline surface.
* The new ACIS body created will have one or more faces with each surface geometry being a polyhedral mesh. The topology
* of the input body and output body will be similar, but not necessarily the same. Sliver faces in the input body
* may not have corresponding entities in the output body. Also, the edge count in the output may be different from that of the input.
* The quality of the polyhedral mesh is controlled by the <tt>make_polyhedral_body_options</tt> object.
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
* @param out_polyhedral_body
* Output ACIS body with polyhedral geometry
* @param mpbo
* make polyhedral body options. A NULL argument will cause the API to use default settings.
* @param ao
* ACIS options.
**/
DECL_POLYHLC outcome api_make_polyhedral_body( BODY* body, 
											   BODY*& out_polyhedral_body, 
											   make_polyhedral_body_options* mpbo = NULL, 
											   AcisOptions* ao = NULL);


/**
* Makes an ACIS body that contains polyhedral mesh geometry.
* <b>Technical Article:</b> <i>[Creating Polyhedral Bodies](http://doc.spatial.com/articles/c/r/e/Creating_Polyhedral_Bodies_d4f0.html)</i>
* <br><br>
* <b>Role:</b> Makes an ACIS BODY from input triangles. The face(s) of the constructed ACIS body has polyhedral mesh geometry. 
* The input triangles are provided in the form of a double array that stores the position coordinates, along with an integer array that stores
* the triangle connectivity information. The array of position coordinates is of size three times the number of triangle vertices. 
* Similarly, the size of the array of triangle connectivity is three times the number of triangles.
* <br><br>
* <b>Errors:</b>
* <br><br>
* <b>Limitations:</b> 
* <br><br>
* <b>Journal:</b> Available
* <br><br>
* <b>Product(s):</b> 3D ACIS Polyhedral
* <br><br>
* @param num_points
* Number of positions
* @param point_coords
* Position coordinates; the size of this array is three times the number of vertices
* @param num_triangles
* Number of triangles
* @param triangle_connectivity
* Triangle connectivity; the size of this array is three times the number of triangles
* @param out_polyhedral_body
* Output ACIS body with polyhedral geometry
* @param mpbo
* make polyhedral body options. (For future use)
* @param ao
* ACIS options.
**/
DECL_POLYHLC outcome api_make_polyhedral_body( int num_points, 
											   double const* point_coords, 
											   int num_triangles, 
											   int const* triangle_connectivity, 
											   BODY*& out_polyhedral_body,
											   make_polyhedral_body_options* mpbo = NULL, 
											   AcisOptions* ao = NULL);



class polyhedral_add_topology_options;
/**
* Adds topology (edges) along the free boundaries of the input polyhedral geometry.
* <b>Role:</b> Given an input ACIS body with a single face whose geometry is polyhedral mesh, this API
* will create edges along the free boundaries of the mesh. When an ACIS polyhedral body is constructed 
* from raw triangular mesh data, no topology is assigned to edges or vertices. This API can 
* be used to create edges on free boundaries, if they exist. Also, if the mesh geometry contains disjoint
* regions, separate face(s) and shell(s) are created. 
* <br><br>
* <b>Errors:</b>
* <br><br>
* <b>Limitations:</b> 
* <br><br>
* <b>Journal:</b> Available
* <br><br>
* <b>Product(s):</b> 3D ACIS Polyhedral
* <br><br>
* @param ioBody
* Input polyhedral ACIS body
* @param polyhedral_add_topology_options
* polyhedral add topology options (For future use).
* @param ao
* ACIS options.
**/
DECL_POLYHLC outcome api_polyhedral_add_topology(BODY* ioBody, 
												 polyhedral_add_topology_options* pto = NULL, 
												 AcisOptions* ao = NULL);


class poly_segmentation_options;
/**
* Makes an ACIS body that is segmented (contains more than one FACE). 
* <b>Technical Article:</b> <i>[Polyhedral Segmentation and Canonic Surface Recognition](http://doc.spatial.com/articles/p/o/l/Polyhedral_Segmentation_and_Canonic_Surface_Recognition_ccbe.html)</i>, <i>[Hybrid ACIS Body](http://doc.spatial.com/articles/h/y/b/Hybrid_ACIS_Body_b003.html)</i>
* <br><br>
* <b>Role:</b> Makes an ACIS body that is segmented (contains more than one FACE) by taking an input polyhedral body   
* that contains only one face. The segmented body is created as a result of canonic recognition of the input.
* All portions of the input polyhedral mesh that can be fitted with canonic surfaces become FACEs with canonic
* geometry while the rest of the portions become FACEs with polyhedral mesh geometry.
* <br><br>
* <b>Errors:</b>
* <br><br>
* <b>Limitations:</b>
* <br><br>
* <b>Journal:</b> Available
* <br><br>
* <b>Product(s):</b> 3D ACIS Polyhedral
* <br><br>
* @param in_polyhedral_body
* Input polyhedral body that contains only one FACE
* @param out_segmented_body
* Output body that may contain more than one FACE whose geometries can be canonic or polyhedral
* @poly_segmentation_options
* Options object (for future use)
* @param ao
* ACIS options
**/
DECL_POLYHLC outcome api_polyhedral_make_segmented_body(BODY* in_polyhedral_body,
														BODY*& out_segmented_body,
													    poly_segmentation_options* pso = NULL,
														AcisOptions* ao = NULL);

/** @} */
#endif
