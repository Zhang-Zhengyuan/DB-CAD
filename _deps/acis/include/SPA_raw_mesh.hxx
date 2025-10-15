/*******************************************************************/
/*    Copyright (c) 1989-2020 by Spatial Corp.                     */
/*    All rights reserved.                                         */
/*    Protected by U.S. Patents 5,257,205; 5,351,196; 6,369,815;   */
/*                              5,982,378; 6,462,738; 6,941,251    */
/*    Protected by European Patents 0503642; 69220263.3            */
/*    Protected by Hong Kong Patent 1008101A                       */
/*******************************************************************/
#ifndef SPA_raw_mesh_hxx
#define SPA_raw_mesh_hxx

#include "dcl_kern.h"
#include "SPAUseCounted.h"
#include "transf.hxx"

/**
* \defgroup ACISPOLYGEOMETRY Polyhedral Geometry
* \ingroup ACISGEOMETRY
*
*/

/**
* @file SPA_raw_mesh.hxx
* @CAA2Level L1
* @CAA2Usage U2
* \addtogroup ACISPOLYGEOMETRY
*
* @{
*/

/**
* Contains the mesh related geometry data such as positions and triangle connectivity.
* <br>
* <b>Role :</b> This class contains all the mesh geometry information namely the total number of
* vertex positions, position coordinates, total number of triangles and the triangle connectivity.
* It also contains the transform that is associated with the mesh. 
* <br>
**/
class DECL_KERN Spa_raw_mesh : public SPAUseCounted
{
protected:
	/**
	* Transform associated with the mesh
	**/
	SPAtransf _transform;
public:

	/**
	* @nodoc
	**/
	virtual ~Spa_raw_mesh(){}

	/**
	* Get the total number of triangles in the mesh
	**/
	virtual int num_triangles() const=0;

	/**
	* Get the total number of vertices in the mesh
	**/
	virtual int num_vertices() const=0;

	/**
	* Get the triangle connectivity. The size of this array is 3 times the number of triangles in mesh.
	**/
	virtual int const* const triangle_connectivity() const=0;

	/**
	* Get the vertex position coordinates. The size of this array is 3 times the number of vertices in mesh.
	**/
	virtual double const* const vertex_coordinates() const=0;

	/**
	* Get the vertex uvs. Vertex uvs are optional and may not be available.
	* So client must check for null return value.  
	* The size of array (if it exists) should by 2x the number of vertices.
	**/
	virtual double const* const vertex_uvs() const
	{
		return NULL;
	}

	/**
	* Changes the unfixed transform on this mesh to supplied transform T
	**/
	void set_transform(SPAtransf const& T);

	/**
	* Apply an input transform T.  If a transform already exists on mesh, then the supplied
	* transform T is multiplied with the existing transform.
	**/
	void apply_transform(SPAtransf const& T);

	/**
	* Get the transform on the mesh.
	**/
	SPAtransf const& get_transform() const;

	/**
	* Fix the transform on the mesh. The mesh geometry is modified as per the existing transform
	* and the transform is set to NULL.
	**/
	virtual void fix_transform()=0;

	enum class Spa_raw_mesh_type
	{
		Spa_raw_mesh,
		ExpandableRawMesh,
		Spa_BVH_raw_mesh,
		ExpandableRawMesh_ParameterizedExpandableMesh
	};

	// RTTI
	virtual Spa_raw_mesh::Spa_raw_mesh_type GetTypeID() const
	{
		return Spa_raw_mesh::Spa_raw_mesh_type::Spa_raw_mesh;
	}
};

typedef DerivedFromSPAUseCounted_sptr<Spa_raw_mesh> Spa_raw_mesh_sptr;

/** @} */
#endif
