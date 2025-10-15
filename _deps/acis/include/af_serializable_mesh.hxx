/*******************************************************************/
/*    Copyright (c) 1989-2020 by Spatial Corp.                     */
/*    All rights reserved.                                         */
/*    Protected by U.S. Patents 5,257,205; 5,351,196; 6,369,815;   */
/*                              5,982,378; 6,462,738; 6,941,251    */
/*    Protected by European Patents 0503642; 69220263.3            */
/*    Protected by Hong Kong Patent 1008101A                       */
/*******************************************************************/
#ifndef af_serializable_mesh_hxx
#define af_serializable_mesh_hxx

class FACE;
#include "mesh.hxx"
#include "dcl_fct.h"
/**
 * @file af_serializable_mesh.hxx
 * @CAA2Level L1
 * @CAA2Usage U2
 *! \addtogroup FCTAPI
 *  \brief Declared at <af_serializable_mesh.hxx>
 *  @{
 **/

#define SERIAL_MESH_APP 0x00494456

/**
 * Defines the user identification for the INDEXED_MESH facet data
 * in the ATTRIB_EYE_ATTACHED_MESH attribute attached to a FACE. Currently
 * only one INDEXED_MESH can be attached to a FACE.
 */
#define SERIAL_MESH_ID 0x00494456

class af_internal_mesh_impl;
class internal_indexed_mesh;
/**
* Provides a base class for meshes which can convert themselves into a form that is
* immediately digestable by visualization systems.
*
* The HA_bridge can render any serializable mesh to the screen.
**/
class DECL_FCT af_serializable_mesh : public MESH
{
public:
	/**
	* constructor.
	**/
	af_serializable_mesh(){}
	/**
	* destructor.
	**/
	virtual ~af_serializable_mesh(){}
	/**
	* returns the number of vertices (i.e., corners of triangles in the mesh).
	**/
	virtual int number_of_vertices() const = 0;
	/**
	* returns the number of polygons in the mesh.
	**/
	virtual int number_of_polygons() const = 0;

	/**
	* Number of edges in polygons in this mesh, with edges shared by two 
	* polygons counted twice.
	**/
	virtual int number_of_polygon_coedges() const = 0;

	/**
	* Writes the coordinates of the mesh vertices into an array of doubles.
	* the array to write into must be at least 3*number_of_vertices() in size.
	**/
	virtual void serialize_positions( double* out_coords ) const=0;
	/**
	* Writes the coordinates of the mesh vertices into an array of floats.
	* the array to write into must be at least 3*number_of_vertices() in size.
	*
	* Default implementation calls serialize_positions(double*), then iterates
	* over the array and truncates the coords.
	**/
	virtual void serialize_positions( float* out_coords ) const=0;

	/**
	* Writes the coordinates of the normal vectors at the mesh vertices into 
	* an array of doubles.  The array needs at least 3*number_of_vertices() doubles in it.
	**/ 
	virtual void serialize_normals(  double* out_normal_coords ) const =0;
	/**
	* Writes the coordinates of the normal vectors at the mesh vertices into 
	* an array of doubles.  The array needs at least 3*number_of_vertices() doubles in it.
	*
	* Default implementation calls serialize_normals(double*) then truncates each coordinate.
	**/ 
	virtual void serialize_normals(  float* out_normal_coords ) const=0;

	/**
	* Fills the array of integers with a series of blocks of three indices into the vertices array.
	* This is a format like open gl would take for glDrawIndices.
	*
	* The return value indicates the number of triangles written to the buffer.  This is at most number_of_polygons().
	* In rare cases, a polygon in the mesh may not be a triangle even though triangles were requested.  To deal with this,
	* the method only writes the triangles into the buffer.
	**/
	virtual int serialize_triangles( int* out_triangle_indices ) const=0;

	/**
	* Fills the array out_hoops_connectivity_array into a form suitable for the function HA_Insert_Shell.
	* Array must have number_of_polygons() + number_of_polygon_edges() ints in it.
	**/
	virtual void serialize_polygons_for_hoops( int* out_hoops_connectivity_array ) const=0;

	/**
	* fills <tt>out_uv_buffer</tt> with the surface par pos data for the vertices.
	* <tt>out_uv_buffer</tt> must have size at least 2*number_of_vertices().
	**/
	virtual void serialize_uv_data( float* out_uv_buffer, logical normalize_uv ) const = 0;

	/**
	* fills <tt>out_uv_buffer</tt> with the surface par pos data for the vertices.
	* <tt>out_uv_buffer</tt> must have size at least 2*number_of_vertices().
	**/
	virtual void serialize_uv_data( double* out_uv_buffer, logical normalize_uv ) const = 0;
	/**
	* client should check if uv and normals are there before serializing
	**/
	virtual logical has_uv() const
	{
		return TRUE;
	}
	/**
	* client should check if uv and normals are there before serializing
	**/
	virtual logical has_normals() const
	{
		return TRUE;
	}
	/**
	* @nodoc.  do not use.  do not overload.
	**/
	virtual af_internal_mesh_impl* get_internal_mesh()
	{
		return NULL;
	}
	/**
	* @nodoc.  do not use.  do not overload.
	**/
	virtual internal_indexed_mesh* get_iim()
	{
		return NULL;
	}

	/**
	* @nodoc
	**/
	virtual af_serializable_mesh* copy() const
	{
		return NULL;
	}
};

/**
* Queries FACE to see if there is an ATTRIB_EYE_ATTACHED_MESH containing an <tt>af_serializable_mesh</tt>
**/
DECL_FCT af_serializable_mesh* GetSerializableMesh( FACE* f );
/**
* Attaches af_serializable_mesh to the face.
**/
DECL_FCT void attach_serializable_mesh_to_face(FACE* face, af_serializable_mesh* mesh );

DECL_FCT double af_SPAfct_uv_tol();

/* @} */
#endif
