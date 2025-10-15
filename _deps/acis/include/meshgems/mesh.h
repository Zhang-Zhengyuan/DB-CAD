#ifndef __MESHGEMS_MESH_H__
#define __MESHGEMS_MESH_H__

/**
\defgroup mesh meshgems_mesh_t

@{

Abstract structure to store a mesh description.

## Introduction
This structure embeds the complete descriptions of the mesh. It is an
abstract object that can be seen as a virtual class (using C++ terminology).
The set/get methods for all the mesh entities and properties shall be 
written and provided to this structure with the corresponding methods.
The mesh can thus be seen as an accessor/translator and it is 
possible to implement it with different backends for storage ; for example :
- a specific memory area
- your own mesh structure 
- a database


The main usage for the integration of a MeshGems component is :
- The caller provides a mesh that the MeshGems components will query for
  their input.
- After its computations, the MeshGems components provides a mesh that
  the caller will query for his results.


A mesh using its own memory storage with all its accessor is provided by the MeshGems
API. It can be constructed with the \ref meshgems_mesh_new_in_memory constructor. 

## Conventions

All indices in the mesh structure start at 1.

## Creating a mesh with its own memory storage
A mesh using its own memory storage with all its accessor can be constructed
with the \ref meshgems_mesh_new_in_memory constructor.
It is then possible to set the number and caracteristics of all entities with
functions using the following naming convention :
meshgems_mesh_set_{entity}_{property}.
For example, the number of vertices can be set using the
\ref meshgems_mesh_set_triangle_count and the vertices of a triangle
with \ref meshgems_mesh_set_triangle_vertices.


We list below the most commonly functions used to fill a mesh for
 a MeshGems component:
- \ref meshgems_mesh_set_vertex_count
- \ref meshgems_mesh_set_vertex_coordinates
- \ref meshgems_mesh_set_vertex_tag
- \ref meshgems_mesh_set_edge_count
- \ref meshgems_mesh_set_edge_vertices
- \ref meshgems_mesh_set_edge_tag
- \ref meshgems_mesh_set_triangle_count
- \ref meshgems_mesh_set_triangle_vertices
- \ref meshgems_mesh_set_triangle_tag
- \ref meshgems_mesh_set_quadrangle_count
- \ref meshgems_mesh_set_quadrangle_vertices
- \ref meshgems_mesh_set_quadrangle_tag
- \ref meshgems_mesh_set_tetrahedron_count
- \ref meshgems_mesh_set_tetrahedron_vertices
- \ref meshgems_mesh_set_tetrahedron_tag
- \ref meshgems_mesh_set_prism_count
- \ref meshgems_mesh_set_prism_vertices
- \ref meshgems_mesh_set_prism_tag
- \ref meshgems_mesh_set_pyramid_count
- \ref meshgems_mesh_set_pyramid_vertices
- \ref meshgems_mesh_set_pyramid_tag
- \ref meshgems_mesh_set_hexahedron_count
- \ref meshgems_mesh_set_hexahedron_vertices
- \ref meshgems_mesh_set_hexahedron_tag


## Creating a mesh accessing your data

A mesh accessing your own data can be constructed with the
\ref meshgems_mesh_new_in_memory constructor.
It is then possible to set the accessor for all caracteristics
of all entities with functions using the following naming convention :
meshgems_mesh_set_get_{entity}_{property}.
For example, the accessor to the number of vertices can be set using the
\ref meshgems_mesh_set_get_triangle_count and the vertices of a triangle
with \ref meshgems_mesh_set_get_triangle_vertices.

We list below the most commonly functions used to set the accessor in
a  mesh for a MeshGems component:
- \ref meshgems_mesh_set_get_vertex_count
- \ref meshgems_mesh_set_get_vertex_coordinates
- \ref meshgems_mesh_set_get_vertex_tag
- \ref meshgems_mesh_set_get_edge_count
- \ref meshgems_mesh_set_get_edge_vertices
- \ref meshgems_mesh_set_get_edge_tag
- \ref meshgems_mesh_set_get_triangle_count
- \ref meshgems_mesh_set_get_triangle_vertices
- \ref meshgems_mesh_set_get_triangle_tag
- \ref meshgems_mesh_set_get_quadrangle_count
- \ref meshgems_mesh_set_get_quadrangle_vertices
- \ref meshgems_mesh_set_get_quadrangle_tag
- \ref meshgems_mesh_set_get_tetrahedron_count
- \ref meshgems_mesh_set_get_tetrahedron_vertices
- \ref meshgems_mesh_set_get_tetrahedron_tag
- \ref meshgems_mesh_set_get_prism_count
- \ref meshgems_mesh_set_get_prism_vertices
- \ref meshgems_mesh_set_get_prism_tag
- \ref meshgems_mesh_set_get_pyramid_count
- \ref meshgems_mesh_set_get_pyramid_vertices
- \ref meshgems_mesh_set_get_pyramid_tag
- \ref meshgems_mesh_set_get_hexahedron_count
- \ref meshgems_mesh_set_get_hexahedron_vertices
- \ref meshgems_mesh_set_get_hexahedron_tag


## Querying a mesh
Independently on how the mesh accesses or stores its data, the mesh entities and
caracteristics can be accessed with functions using the following naming
convention : meshgems_mesh_get_{entity}_{property}.
For example, the number of vertices can be accessed using the
\ref meshgems_mesh_get_triangle_count and the vertices of a triangle
with \ref meshgems_mesh_get_triangle_vertices.


We list below the most commonly functions used to query a mesh generated
by a MeshGems component:
- \ref meshgems_mesh_get_vertex_count
- \ref meshgems_mesh_get_vertex_coordinates
- \ref meshgems_mesh_get_vertex_tag
- \ref meshgems_mesh_get_edge_count
- \ref meshgems_mesh_get_edge_vertices
- \ref meshgems_mesh_get_edge_tag
- \ref meshgems_mesh_get_triangle_count
- \ref meshgems_mesh_get_triangle_vertices
- \ref meshgems_mesh_get_triangle_tag
- \ref meshgems_mesh_get_quadrangle_count
- \ref meshgems_mesh_get_quadrangle_vertices
- \ref meshgems_mesh_get_quadrangle_tag
- \ref meshgems_mesh_get_tetrahedron_count
- \ref meshgems_mesh_get_tetrahedron_vertices
- \ref meshgems_mesh_get_tetrahedron_tag
- \ref meshgems_mesh_get_prism_count
- \ref meshgems_mesh_get_prism_vertices
- \ref meshgems_mesh_get_prism_tag
- \ref meshgems_mesh_get_pyramid_count
- \ref meshgems_mesh_get_pyramid_vertices
- \ref meshgems_mesh_get_pyramid_tag
- \ref meshgems_mesh_get_hexahedron_count
- \ref meshgems_mesh_get_hexahedron_vertices
- \ref meshgems_mesh_get_hexahedron_tag




## File IO with a mesh
The file IO functions relative to the \ref meshgems_mesh_t are described in
the \ref mesh_files module.


*/

/**
\defgroup mesh_vertex mesh vertices
\ingroup mesh


\defgroup mesh_edge mesh edges
\ingroup mesh

\defgroup mesh_triangle mesh triangles
\ingroup mesh

\defgroup mesh_quadrangle mesh quandrangles
\ingroup mesh

\defgroup mesh_tetrahedron mesh tetrahedra
\ingroup mesh

\defgroup mesh_prism mesh prisms
\ingroup mesh

\defgroup mesh_pyramid mesh pyramids
\ingroup mesh

\defgroup mesh_hexahedron mesh hexahedra
\ingroup mesh

\defgroup mesh_subdomains mesh subdomains
\ingroup mesh

\defgroup mesh_tag_system mesh tag system
\ingroup mesh

\defgroup mesh_iterators mesh iterators
\ingroup mesh

\defgroup mesh_statistics mesh statistics
\ingroup mesh

\defgroup mesh_files file IO
\ingroup mesh

The following mesh file formats are available :

<table>
<caption> List of Mesh file formats in MeshGems API </caption>
<tr>
  <th> File format (extension) </th>
  <th> Reading function </th>
  <th> Writing function </th>
  <th> Comment </th>
</tr>

<tr>
  <td> mesh[b] </td>
  <td> \ref meshgems_mesh_new_read_mesh </td>
  <td> \ref meshgems_mesh_write_mesh </td>
  <td>  Official MeshGems format.
  a.k.a. Gamma Mesh Format. ASCII or Binary format. Version 3. </td>
</tr>

<tr>
  <td> msh2 </td>
  <td>  </td>
  <td> \ref meshgems_mesh_write_msh2 </td>
  <td> Legacy format </td>
</tr>

<tr>
  <td> bdf </td>
  <td>  </td>
  <td> \ref meshgems_mesh_write_bdf </td>
  <td> Nastran ASCII format </td>
</tr>

</table>


*/


struct meshgems_mesh_t_;
/**
 * Opaque type storing mesh data.
 */
typedef struct meshgems_mesh_t_ meshgems_mesh_t;

/* **************************************** */

#include <stdio.h>

#include <meshgems/mdecl.h>
#include <meshgems/basic_types.h>
#include <meshgems/status.h>
#include <meshgems/context.h>
#include <meshgems/tag_system.h>
#include <meshgems/mesh_statistics.h>

/**
 *
 * \name Macros defining element type
 * @{
 */
 /** Macro to define an unknown element type */
#define MESHGEMS_MESH_ELEMENT_TYPE_UNKNOWN      0000
/** Macro to define an element type corresponding to a linear edge */
#define MESHGEMS_MESH_ELEMENT_TYPE_EDGE2        1000
/** Macro to define an element type corresponding to a 3 nodes quadratic edge */
#define MESHGEMS_MESH_ELEMENT_TYPE_EDGE3        1001
/** Macro to define an element type corresponding to a linear edge */
#define MESHGEMS_MESH_ELEMENT_TYPE_EDGE         MESHGEMS_MESH_ELEMENT_TYPE_EDGE2
/** Macro to define an element type corresponding to a linear triangle */
#define MESHGEMS_MESH_ELEMENT_TYPE_TRIA3        2000
/** Macro to define an element type corresponding to a 6 nodes quadratic triangle */
#define MESHGEMS_MESH_ELEMENT_TYPE_TRIA6        2003
/** Macro to define an element type corresponding to a linear triangle */
#define MESHGEMS_MESH_ELEMENT_TYPE_TRIA         MESHGEMS_MESH_ELEMENT_TYPE_TRIA3
/** Macro to define an element type corresponding to a linear quadrangle */
#define MESHGEMS_MESH_ELEMENT_TYPE_QUAD4        3000
/** Macro to define an element type corresponding to a 9 nodes quadratic quadrangle */
#define MESHGEMS_MESH_ELEMENT_TYPE_QUAD9        3005
/** Macro to define an element type corresponding to a linear quadrangle */
#define MESHGEMS_MESH_ELEMENT_TYPE_QUAD         MESHGEMS_MESH_ELEMENT_TYPE_QUAD4
/** Macro to define an element type corresponding to a linear tetrahedron */
#define MESHGEMS_MESH_ELEMENT_TYPE_TETRA4       4000
/** Macro to define an element type corresponding to a 10 nodes quadratic tetrahedron */
#define MESHGEMS_MESH_ELEMENT_TYPE_TETRA10      4006
/** Macro to define an element type corresponding to a linear tetrahedron */
#define MESHGEMS_MESH_ELEMENT_TYPE_TETRA        MESHGEMS_MESH_ELEMENT_TYPE_TETRA4
/** Macro to define an element type corresponding to a linear hexahedron */
#define MESHGEMS_MESH_ELEMENT_TYPE_HEXA8        5000
/** Macro to define an element type corresponding to a 27 nodes quadratic hexahedron */
#define MESHGEMS_MESH_ELEMENT_TYPE_HEXA27       5019
/** Macro to define an element type corresponding to a linear hexahedron */
#define MESHGEMS_MESH_ELEMENT_TYPE_HEXA         MESHGEMS_MESH_ELEMENT_TYPE_HEXA8
/** Macro to define an element type corresponding to a linear prism */
#define MESHGEMS_MESH_ELEMENT_TYPE_PRISM6       6000
/** Macro to define an element type corresponding to a 18 nodes quadratic prism */
#define MESHGEMS_MESH_ELEMENT_TYPE_PRISM18      6012
/** Macro to define an element type corresponding to a linear prism */
#define MESHGEMS_MESH_ELEMENT_TYPE_PRISM        MESHGEMS_MESH_ELEMENT_TYPE_PRISM6
/** Macro to define an element type corresponding to a linear pyramid */
#define MESHGEMS_MESH_ELEMENT_TYPE_PYRA5        7000
/** Macro to define an element type corresponding to a 14 nodes quadratic pyramid */
#define MESHGEMS_MESH_ELEMENT_TYPE_PYRA14       7009
/** Macro to define an element type corresponding to a linear pyramid */
#define MESHGEMS_MESH_ELEMENT_TYPE_PYRA         MESHGEMS_MESH_ELEMENT_TYPE_PYRA5
/**@}*/

/** Macro that defines the current maximum number of extra vertices for any element */
#define MESHGEMS_MESH_ELEMENT_MAX_VERTICES_COUNT	27

/** Macro that provides the number of extra vertices for an element type */
#define MESHGEMS_MESH_ELEMENT_TYPE_EXTRA_VERTICES_COUNT(t_) ((t_) % 1000)

/**
 * \ingroup mesh
 * Simple constructor.
 * @return a new \ref meshgems_mesh_t object
 */
MESHGEMS_METHOD meshgems_mesh_t *meshgems_mesh_new(meshgems_context_t *ctx);

/**
 * \ingroup mesh_memory
 * Memory mesh constructor.
 * @return a new in memory mesh
 */
MESHGEMS_METHOD meshgems_mesh_t *meshgems_mesh_new_in_memory(meshgems_context_t *ctx);

/**
 * \ingroup mesh_memory
 * Deep copy constructor.
 * @return a new \ref meshgems_mesh_t object
 */
MESHGEMS_METHOD meshgems_mesh_t *meshgems_mesh_new_in_memory_copy(meshgems_mesh_t *mesh);

/**
 * \ingroup mesh
 * Destructor. Frees all allocated data.
 */
MESHGEMS_METHOD void meshgems_mesh_delete(meshgems_mesh_t *m);

/**
 * \ingroup mesh
 * Allocate a buffer suited to handle any request on any elementary data in a mesh.
 * @param[in] mesh	: the working mesh structure.
 * @return a pointer
 */
MESHGEMS_METHOD void *meshgems_mesh_calloc_generic_buffer(meshgems_mesh_t *mesh);

/**
 * \ingroup mesh
 * Free a buffer allocated by meshgems_mesh_calloc_generic_buffer.
 * @param[in] v	: the working buffer.
 * @return a pointer
 */
MESHGEMS_METHOD void meshgems_mesh_free_generic_buffer(void *v);

/**
 * \ingroup mesh_periodicity
 * Type for functions that define a periodicity transformation between two sets of mesh faces.
 * @param xyz (in) : a real[3] array filled with the xyz coordinate of a point
 * @param xyz_image (out) : a real[3] array filled with the xyz coordinate of the image of xyz by the transformation
 * @param user_data (in) : a generic user pointer
 * @return a status code
 */
typedef meshgems_status_t (*meshgems_mesh_periodicity_transformation_t)(
    meshgems_real *xyz, meshgems_real *xyz_image, void *user_data);

/**
 * \ingroup mesh
 * Set a required discretization to be respected when generating a new mesh out this mesh using MG-CADSurf.
 * @param[in] mesh	: the working mesh structure.
 * @param[in] etag	: the edge tag on which the required discretization is set.
 * @param[in] nv	: the number of point of the required discretization.
 * @param[in] tags	: the tags of each required point.
 * @param[in] xyzs	: the 3d coordinates of the ordered set of required vertices.
 * @return a pointer
 */
MESHGEMS_METHOD meshgems_status_t meshgems_mesh_set_required_discretization_on_reference_line(meshgems_mesh_t *mesh, meshgems_integer etag, meshgems_integer nv, meshgems_integer *tags, meshgems_real *xyzs);

/**
 * \ingroup mesh_periodicity
 * Set the periodicity property between two sets of mesh faces given by their tag, and the associated transformation given by its function.
 * @param msh (in) : the working mesh object
 * @param tag1 (in) : the list of the working mesh face tags
 * @param size1 (in) : the number of faces in the first group of faces tag1
 * @param tag2 (in) : the list of mesh face tags to set periodic with tag1
 * @param size2 (in) : the number of faces in the second group of faces tag2
 * @param transf (in) : the transformation from faces in tag1 to faces in tag2
 * @param user_data (in) : a generic user pointer
 * @return a status code
 */
MESHGEMS_METHOD meshgems_status_t meshgems_mesh_add_face_multiple_periodicity_with_transformation_function(
    meshgems_mesh_t *msh, meshgems_integer *tag1, meshgems_integer size1,
    meshgems_integer *tag2, meshgems_integer size2,
    meshgems_mesh_periodicity_transformation_t transf, void *user_data);

/**
 * \ingroup mesh_periodicity
 * Set the periodicity property between two sets of mesh faces given by their tag, and the associated transformation given by some points.
 * An affine isometry can be fully defined by a collection of 4 non coplanar points and their images. However,
 * some transformations can be guessed with fewer information. Below is the behavior of this function depending on the
 * number of points given as parameter :
 * - one point (and its image) is provided : we suppose that the transformation is a simple translation.
 * - two points (and their images) are provided : we suppose that the transformation is a simple
 *   translation. If it is not coherent with the given coordinates, a STATUS ERROR is returned.
 * - three points (and their images) are provided : we can determine one of the two possible affine
 *   isometries defined by these three couples of points. If the three points are colinear a STATUS ERROR
 *   is returned. Please note that this option is adequate for flat periodic entities (plane or edges wich can
 *   be included in a plane).
 * - four points (and their images) are provided : we can determine the unique affine isometry
 *   defined by the four couples of points. If the four points are coplanar, a STATUS ERROR is returned.
 *
 * @param msh (in) : the working mesh object
 * @param tag1 (in) : the list of the working mesh face tags
 * @param size1 (in) : the number of faces in the first group of faces tag1
 * @param tag2 (in) : the list of mesh face tags to set periodic with tag1
 * @param size2 (in) : the number of faces in the second group of faces tag2
 * @param p (in) : (size 3*nb_pt1) : the xyz coordinates of the point of tag1
 * @param nb_pt1 (in) : the number of points in p
 * @param p_im (in) : (size 3*nb_pt2) : the xyz coordinates of the point of tag2 (periodic with the ones in p)
 * @param nb_pt2 (in) : the number of points in p_im : has to be equal to nb_pt1
 * @return a status code
 */
MESHGEMS_METHOD meshgems_status_t meshgems_mesh_add_face_multiple_periodicity_with_transformation_function_by_points(
    meshgems_mesh_t *msh, meshgems_integer *tag1, meshgems_integer size1,
    meshgems_integer *tag2, meshgems_integer size2, meshgems_real *p,
    meshgems_integer nb_pt1, meshgems_real *p_im, meshgems_integer nb_pt2);


/* **************************************** */

/** Macro to recognize an undefined normal */
#define UNDEFINED_NORMAL(nxyz_) ( ((nxyz_)[0] == 0.0) && ((nxyz_)[1] == 0.0) && ((nxyz_)[2] == 0.0) )







/* **************************************** */
/**
 * \ingroup mesh_vertex
 * Gets the number of vertices in the mesh (generic method called by this \ref meshgems_mesh_t object user).
 * @param[in]	m	: the mesh
 * @param[out]	nbvtx	: the number of vertices
 * @return a status code
 */
MESHGEMS_METHOD meshgems_status_t meshgems_mesh_get_vertex_count(meshgems_mesh_t *m, meshgems_integer *nbvtx);

/**
 * \ingroup mesh_vertex
 * Implementation declaration of how the number of vertices in the mesh is obtained.
 * @param[out]	nbvtx	: the number of vertices
 * @param     user_data	: a user pointer.
 * @return a status code
 */
typedef meshgems_status_t (*meshgems_mesh_get_vertex_count_t)(meshgems_integer *nbvtx, void *user_data);

/**
 * \ingroup mesh_vertex
 * Sets the implementation to use when \ref meshgems_mesh_get_vertex_count is called by this \ref meshgems_mesh_t object user.
 * @param[in]	m	: the mesh
 * @param[in]	func	: the function implementation
 * @param     user_data	: a user pointer to pass to the implementation.
 * @return a status code
 */
MESHGEMS_METHOD meshgems_status_t meshgems_mesh_set_get_vertex_count(meshgems_mesh_t *m,
						        meshgems_mesh_get_vertex_count_t func,
						        void *user_data);
/**
 * \ingroup mesh_vertex
 * Gets the coordinates of a mesh vertex (generic method called by this \ref meshgems_mesh_t object user).
 * @param[in]	m	: the mesh
 * @param[in]	ivtx	: index of vertex from 1 to nbvtx
 * @param[out]	xyz	: real[3] array containing the coordinates of the vertex
 * @return a status code
 */
MESHGEMS_METHOD meshgems_status_t meshgems_mesh_get_vertex_coordinates(meshgems_mesh_t *m, meshgems_integer ivtx, meshgems_real *xyz);

/**
 * \ingroup mesh_vertex
 * Implementation declaration of how the coordinates of a mesh vertex are obtained.
 * @param[in]	ivtx	: index of vertex from 1 to nbvtx
 * @param[out]	xyz	: real[3] array containing the coordinates of the vertex
 * @param	user_data: a user pointer.
 * @return a status code
 */
typedef meshgems_status_t (*meshgems_mesh_get_vertex_coordinates_t)(meshgems_integer ivtx, meshgems_real *xyz, void *user_data);

/**
 * \ingroup mesh_vertex
 * Sets the implementation to use when \ref meshgems_mesh_get_vertex_coordinates is called by this \ref meshgems_mesh_t object user.
 * @param[in]	m	: the mesh
 * @param[in]	func	: the function implementation
 * @param	user_data: a user pointer to pass to the implementation.
 * @return a status code
 */
MESHGEMS_METHOD meshgems_status_t meshgems_mesh_set_get_vertex_coordinates(meshgems_mesh_t *m,
							meshgems_mesh_get_vertex_coordinates_t func,
							void *user_data);
/**
 * \ingroup mesh_vertex
 * Gets the normal coordinates at a mesh vertex (generic method called by this \ref meshgems_mesh_t object user).
 * @param[in]	m	: the mesh
 * @param[in]	ivtx	: index of vertex from 1 to nbvtx
 * @param[out]	nxyz	: real[3] array containing the normal coordinates at the vertex
 * @return a status code
 */
MESHGEMS_METHOD meshgems_status_t meshgems_mesh_get_vertex_normal_coordinates(meshgems_mesh_t *m, meshgems_integer ivtx, meshgems_real *nxyz);

/**
 * \ingroup mesh_vertex
 * Implementation declaration of how the normal coordinates at a mesh vertex are obtained.
 * @param[in]	ivtx	: index of vertex from 1 to nbvtx
 * @param[out]	nxyz	: real[3] array containing the normal coordinates at the vertex
 * @param	user_data: a user pointer.
 * @return a status code
 */
typedef meshgems_status_t (*meshgems_mesh_get_vertex_normal_coordinates_t)(meshgems_integer ivtx, meshgems_real *nxyz, void *user_data);

/**
 * \ingroup mesh_vertex
 * Sets the implementation to use when \ref meshgems_mesh_get_vertex_normal_coordinates is called by this \ref meshgems_mesh_t object user.
 * @param[in]	m	: the mesh
 * @param[in]	func	: the function implementation
 * @param	user_data: a user pointer to pass to the implementation.
 * @return a status code
 */
MESHGEMS_METHOD meshgems_status_t meshgems_mesh_set_get_vertex_normal_coordinates(meshgems_mesh_t *m,
							meshgems_mesh_get_vertex_normal_coordinates_t func,
							void *user_data);
/**
 * \ingroup mesh_vertex
 * Gets the tag of a mesh vertex (generic method called by this \ref meshgems_mesh_t object user).
 * @param[in]	m	: the mesh
 * @param[in]	ivtx	: index of vertex from 1 to nbvtx
 * @param[out]	tagvtx	: integer tag of the vertex
 * @return a status code
 */
MESHGEMS_METHOD meshgems_status_t meshgems_mesh_get_vertex_tag(meshgems_mesh_t *m, meshgems_integer ivtx, meshgems_integer *tagvtx);

/**
 * \ingroup mesh_vertex
 * Implementation declaration of how the tag of a mesh vertex is obtained.
 * @param[in]	ivtx	: index of vertex from 1 to nbvtx
 * @param[out]	tagvtx	: integer tag of the vertex
 * @param	user_data: a user pointer.
 * @return a status code
 */
typedef meshgems_status_t (*meshgems_mesh_get_vertex_tag_t)(meshgems_integer ivtx, meshgems_integer *tagvtx, void *user_data);

/**
 * \ingroup mesh_vertex
 * Sets the implementation to use when \ref meshgems_mesh_get_vertex_tag is called by this \ref meshgems_mesh_t object user.
 * @param[in]	m	: the mesh
 * @param[in]	func	: the function implementation
 * @param	user_data: a user pointer to pass to the implementation.
 * @return a status code
 */
MESHGEMS_METHOD meshgems_status_t meshgems_mesh_set_get_vertex_tag(meshgems_mesh_t *m,
							meshgems_mesh_get_vertex_tag_t func,
							void *user_data);
/**
 * \ingroup mesh_vertex
 * Gets the periodic id of a mesh vertex (generic method called by this \ref meshgems_mesh_t object user).
 * @param[in]	m	: the mesh
 * @param[in]	ivtx	: index of vertex from 1 to nbvtx
 * @param[out]	periodic_id	: integer periodic id of the vertex
 * @return a status code
 */
MESHGEMS_METHOD meshgems_status_t meshgems_mesh_get_vertex_periodic_id(meshgems_mesh_t *m, meshgems_integer ivtx, meshgems_integer *periodic_id);

/**
 * \ingroup mesh_vertex
 * Implementation declaration of how the periodic id of a mesh vertex is obtained.
 * @param[in]	ivtx	: index of vertex from 1 to nbvtx
 * @param[out]	periodic_id	: integer periodic id of the vertex
 * @param	user_data: a user pointer.
 * @return a status code
 */
typedef meshgems_status_t (*meshgems_mesh_get_vertex_periodic_id_t)(meshgems_integer ivtx, meshgems_integer *periodic_id, void *user_data);

/**
 * \ingroup mesh_vertex
 * Sets the implementation to use when \ref meshgems_mesh_get_vertex_periodic_id is called by this \ref meshgems_mesh_t object user.
 * @param[in]	m	: the mesh
 * @param[in]	func	: the function implementation
 * @param	user_data: a user pointer to pass to the implementation.
 * @return a status code
 */
MESHGEMS_METHOD meshgems_status_t meshgems_mesh_set_get_vertex_periodic_id(meshgems_mesh_t *m,
							meshgems_mesh_get_vertex_periodic_id_t func,
							void *user_data);
/**
 * \ingroup mesh_vertex
 * Gets the required property of a mesh vertex (generic method called by this \ref meshgems_mesh_t object user).
 * @param[in]	m	: the mesh
 * @param[in]	ivtx	: index of vertex from 1 to nbvtx
 * @param[out]	reqtx	: integer required property of the vertex
 * @return a status code
 */
MESHGEMS_METHOD meshgems_status_t meshgems_mesh_get_vertex_required_property(meshgems_mesh_t *m, meshgems_integer ivtx, meshgems_integer *reqtx);

/**
 * \ingroup mesh_vertex
 * Implementation declaration of how the required property of a mesh vertex is obtained.
 * @param[in]	ivtx	: index of vertex from 1 to nbvtx
 * @param[out]	reqtx	: integer required property of the vertex
 * @param	user_data: a user pointer.
 * @return a status code
 */
typedef meshgems_status_t (*meshgems_mesh_get_vertex_required_property_t)(meshgems_integer ivtx, meshgems_integer *reqtx, void *user_data);

/**
 * \ingroup mesh_vertex
 * Sets the implementation to use when \ref meshgems_mesh_get_vertex_required_property is called by this \ref meshgems_mesh_t object user.
 * @param[in]	m	: the mesh
 * @param[in]	func	: the function implementation
 * @param	user_data: a user pointer to pass to the implementation.
 * @return a status code
 */
MESHGEMS_METHOD meshgems_status_t meshgems_mesh_set_get_vertex_required_property(meshgems_mesh_t *m,
							meshgems_mesh_get_vertex_required_property_t func,
							void *user_data);
/**
 * \ingroup mesh_vertex
 * Gets the corner property of a mesh vertex (generic method called by this \ref meshgems_mesh_t object user).
 * @param[in]	m	: the mesh
 * @param[in]	ivtx	: index of vertex from 1 to nbvtx
 * @param[out]	cornvtx	: integer corner property of the vertex
 * @return a status code
 */
MESHGEMS_METHOD meshgems_status_t meshgems_mesh_get_vertex_corner_property(meshgems_mesh_t *m, meshgems_integer ivtx, meshgems_integer *cornvtx);

/**
 * \ingroup mesh_vertex
 * Implementation declaration of how the corner property of a mesh vertex is obtained.
 * @param[in]	ivtx	: index of vertex from 1 to nbvtx
 * @param[out]	cornvtx	: integer corner property of the vertex
 * @param	user_data: a user pointer.
 * @return a status code
 */
typedef meshgems_status_t (*meshgems_mesh_get_vertex_corner_property_t)(meshgems_integer ivtx, meshgems_integer *cornvtx, void *user_data);

/**
 * \ingroup mesh_vertex
 * Sets the implementation to use when \ref meshgems_mesh_get_vertex_corner_property is called by this \ref meshgems_mesh_t object user.
 * @param[in]	m	: the mesh
 * @param[in]	func	: the function implementation
 * @param	user_data: a user pointer to pass to the implementation.
 * @return a status code
 */
MESHGEMS_METHOD meshgems_status_t meshgems_mesh_set_get_vertex_corner_property(meshgems_mesh_t *m,
							meshgems_mesh_get_vertex_corner_property_t func,
							void *user_data);
/* **************************************** */
/**
 * \ingroup mesh_edge
 * Gets the number of edges in the mesh (generic method called by this \ref meshgems_mesh_t object user).
 * @param[in]	m	: the mesh
 * @param[out]	nbedg	: the number of edges
 * @return a status code
 */
MESHGEMS_METHOD meshgems_status_t meshgems_mesh_get_edge_count(meshgems_mesh_t *m, meshgems_integer *nbedg);

/**
 * \ingroup mesh_edge
 * Implementation declaration of how the number of edges in the mesh is obtained.
 * @param[out]	nbedg	: the number of edges
 * @param     user_data	: a user pointer.
 * @return a status code
 */
typedef meshgems_status_t (*meshgems_mesh_get_edge_count_t)(meshgems_integer *nbedg, void *user_data);

/**
 * \ingroup mesh_edge
 * Sets the implementation to use when \ref meshgems_mesh_get_edge_count is called by this \ref meshgems_mesh_t object user.
 * @param[in]	m	: the mesh
 * @param[in]	func	: the function implementation
 * @param     user_data	: a user pointer to pass to the implementation.
 * @return a status code
 */
MESHGEMS_METHOD meshgems_status_t meshgems_mesh_set_get_edge_count(meshgems_mesh_t *m,
						        meshgems_mesh_get_edge_count_t func,
						        void *user_data);
/**
 * \ingroup mesh_edge
 * Gets the vertices of a mesh edge (generic method called by this \ref meshgems_mesh_t object user).
 * @param[in]	m	: the mesh
 * @param[in]	iedg	: index of edge from 1 to nbedg
 * @param[out]	vedg	: integer[2] array containing the vertices of the edge
 * @return a status code
 */
MESHGEMS_METHOD meshgems_status_t meshgems_mesh_get_edge_vertices(meshgems_mesh_t *m, meshgems_integer iedg, meshgems_integer *vedg);

/**
 * \ingroup mesh_edge
 * Implementation declaration of how the vertices of a mesh edge are obtained.
 * @param[in]	iedg	: index of edge from 1 to nbedg
 * @param[out]	vedg	: integer[2] array containing the vertices of the edge
 * @param	user_data: a user pointer.
 * @return a status code
 */
typedef meshgems_status_t (*meshgems_mesh_get_edge_vertices_t)(meshgems_integer iedg, meshgems_integer *vedg, void *user_data);

/**
 * \ingroup mesh_edge
 * Sets the implementation to use when \ref meshgems_mesh_get_edge_vertices is called by this \ref meshgems_mesh_t object user.
 * @param[in]	m	: the mesh
 * @param[in]	func	: the function implementation
 * @param	user_data: a user pointer to pass to the implementation.
 * @return a status code
 */
MESHGEMS_METHOD meshgems_status_t meshgems_mesh_set_get_edge_vertices(meshgems_mesh_t *m,
							meshgems_mesh_get_edge_vertices_t func,
							void *user_data);
/**
 * \ingroup mesh_edge
 * Gets the tag of a mesh edge (generic method called by this \ref meshgems_mesh_t object user).
 * @param[in]	m	: the mesh
 * @param[in]	iedg	: index of edge from 1 to nbedg
 * @param[out]	tagedg	: integer tag of the edge
 * @return a status code
 */
MESHGEMS_METHOD meshgems_status_t meshgems_mesh_get_edge_tag(meshgems_mesh_t *m, meshgems_integer iedg, meshgems_integer *tagedg);

/**
 * \ingroup mesh_edge
 * Implementation declaration of how the tag of a mesh edge is obtained.
 * @param[in]	iedg	: index of edge from 1 to nbedg
 * @param[out]	tagedg	: integer tag of the edge
 * @param	user_data: a user pointer.
 * @return a status code
 */
typedef meshgems_status_t (*meshgems_mesh_get_edge_tag_t)(meshgems_integer iedg, meshgems_integer *tagedg, void *user_data);

/**
 * \ingroup mesh_edge
 * Sets the implementation to use when \ref meshgems_mesh_get_edge_tag is called by this \ref meshgems_mesh_t object user.
 * @param[in]	m	: the mesh
 * @param[in]	func	: the function implementation
 * @param	user_data: a user pointer to pass to the implementation.
 * @return a status code
 */
MESHGEMS_METHOD meshgems_status_t meshgems_mesh_set_get_edge_tag(meshgems_mesh_t *m,
							meshgems_mesh_get_edge_tag_t func,
							void *user_data);
/**
 * \ingroup mesh_edge
 * Gets the tangent coordinates at a mesh edge vertex (generic method called by this \ref meshgems_mesh_t object user).
 * @param[in]	m	: the mesh
 * @param[in]	iedg	: index of edge from 1 to nbedg
 * @param[in]	iedgloc	: index of edge vertex from 1 to 2
 * @param[out]	txyz	: real[3] array containing the tangent coordinates at the edge vertex
 * @return a status code
 */
MESHGEMS_METHOD meshgems_status_t meshgems_mesh_get_edge_vertex_tangent_coordinates(meshgems_mesh_t *m, meshgems_integer iedg, meshgems_integer iedgloc, meshgems_real *txyz);

/**
 * \ingroup mesh_edge
 * Implementation declaration of how the tangent coordinates at a mesh edge vertex are obtained.
 * @param[in]	iedg	: index of edge from 1 to nbedg
 * @param[in]	iedgloc	: index of edge vertex from 1 to 2
 * @param[out]	txyz	: real[3] array containing the tangent coordinates at the edge vertex
 * @param	user_data: a user pointer.
 * @return a status code
 */
typedef meshgems_status_t (*meshgems_mesh_get_edge_vertex_tangent_coordinates_t)(meshgems_integer iedg, meshgems_integer iedgloc, meshgems_real *txyz, void *user_data);

/**
 * \ingroup mesh_edge
 * Sets the implementation to use when \ref meshgems_mesh_get_edge_vertex_tangent_coordinates is called by this \ref meshgems_mesh_t object user.
 * @param[in]	m	: the mesh
 * @param[in]	func	: the function implementation
 * @param	user_data: a user pointer to pass to the implementation.
 * @return a status code
 */
MESHGEMS_METHOD meshgems_status_t meshgems_mesh_set_get_edge_vertex_tangent_coordinates(meshgems_mesh_t *m,
							meshgems_mesh_get_edge_vertex_tangent_coordinates_t func,
							void *user_data);
/**
 * \ingroup mesh_edge
 * Gets the periodic id of a mesh edge (generic method called by this \ref meshgems_mesh_t object user).
 * @param[in]	m	: the mesh
 * @param[in]	iedg	: index of edge from 1 to nbedg
 * @param[out]	periodic_id	: integer periodic id of the edge
 * @return a status code
 */
MESHGEMS_METHOD meshgems_status_t meshgems_mesh_get_edge_periodic_id(meshgems_mesh_t *m, meshgems_integer iedg, meshgems_integer *periodic_id);

/**
 * \ingroup mesh_edge
 * Implementation declaration of how the periodic id of a mesh edge is obtained.
 * @param[in]	iedg	: index of edge from 1 to nbedg
 * @param[out]	periodic_id	: integer periodic id of the edge
 * @param	user_data: a user pointer.
 * @return a status code
 */
typedef meshgems_status_t (*meshgems_mesh_get_edge_periodic_id_t)(meshgems_integer iedg, meshgems_integer *periodic_id, void *user_data);

/**
 * \ingroup mesh_edge
 * Sets the implementation to use when \ref meshgems_mesh_get_edge_periodic_id is called by this \ref meshgems_mesh_t object user.
 * @param[in]	m	: the mesh
 * @param[in]	func	: the function implementation
 * @param	user_data: a user pointer to pass to the implementation.
 * @return a status code
 */
MESHGEMS_METHOD meshgems_status_t meshgems_mesh_set_get_edge_periodic_id(meshgems_mesh_t *m,
							meshgems_mesh_get_edge_periodic_id_t func,
							void *user_data);
/**
 * \ingroup mesh_edge
 * Gets the required property of a mesh edge (generic method called by this \ref meshgems_mesh_t object user).
 * @param[in]	m	: the mesh
 * @param[in]	iedg	: index of edge from 1 to nbedg
 * @param[out]	reqedg	: integer required property of the edge
 * @return a status code
 */
MESHGEMS_METHOD meshgems_status_t meshgems_mesh_get_edge_required_property(meshgems_mesh_t *m, meshgems_integer iedg, meshgems_integer *reqedg);

/**
 * \ingroup mesh_edge
 * Implementation declaration of how the required property of a mesh edge is obtained.
 * @param[in]	iedg	: index of edge from 1 to nbedg
 * @param[out]	reqedg	: integer required property of the edge
 * @param	user_data: a user pointer.
 * @return a status code
 */
typedef meshgems_status_t (*meshgems_mesh_get_edge_required_property_t)(meshgems_integer iedg, meshgems_integer *reqedg, void *user_data);

/**
 * \ingroup mesh_edge
 * Sets the implementation to use when \ref meshgems_mesh_get_edge_required_property is called by this \ref meshgems_mesh_t object user.
 * @param[in]	m	: the mesh
 * @param[in]	func	: the function implementation
 * @param	user_data: a user pointer to pass to the implementation.
 * @return a status code
 */
MESHGEMS_METHOD meshgems_status_t meshgems_mesh_set_get_edge_required_property(meshgems_mesh_t *m,
							meshgems_mesh_get_edge_required_property_t func,
							void *user_data);
/**
 * \ingroup mesh_edge
 * Gets the ridge property of a mesh edge (generic method called by this \ref meshgems_mesh_t object user).
 * @param[in]	m	: the mesh
 * @param[in]	iedg	: index of edge from 1 to nbedg
 * @param[out]	ridgedg	: integer ridge property of the edge
 * @return a status code
 */
MESHGEMS_METHOD meshgems_status_t meshgems_mesh_get_edge_ridge_property(meshgems_mesh_t *m, meshgems_integer iedg, meshgems_integer *ridgedg);

/**
 * \ingroup mesh_edge
 * Implementation declaration of how the ridge property of a mesh edge is obtained.
 * @param[in]	iedg	: index of edge from 1 to nbedg
 * @param[out]	ridgedg	: integer ridge property of the edge
 * @param	user_data: a user pointer.
 * @return a status code
 */
typedef meshgems_status_t (*meshgems_mesh_get_edge_ridge_property_t)(meshgems_integer iedg, meshgems_integer *ridgedg, void *user_data);

/**
 * \ingroup mesh_edge
 * Sets the implementation to use when \ref meshgems_mesh_get_edge_ridge_property is called by this \ref meshgems_mesh_t object user.
 * @param[in]	m	: the mesh
 * @param[in]	func	: the function implementation
 * @param	user_data: a user pointer to pass to the implementation.
 * @return a status code
 */
MESHGEMS_METHOD meshgems_status_t meshgems_mesh_set_get_edge_ridge_property(meshgems_mesh_t *m,
							meshgems_mesh_get_edge_ridge_property_t func,
							void *user_data);
/**
 * \ingroup mesh_edge
 * Gets the extra vertices of a mesh edge (generic method called by this \ref meshgems_mesh_t object user).
 * @param[in]	m	: the mesh
 * @param[in]	iedg	: index of edge from 1 to nbedg
 * @param[out]	typeedg	: integer containing the type of the edge
 * @param[out]	evedg	: integer[] array containing the extra vertices of the edge
 * @return a status code
 */
MESHGEMS_METHOD meshgems_status_t meshgems_mesh_get_edge_extra_vertices(meshgems_mesh_t *m, meshgems_integer iedg,
                                                    meshgems_integer *typeedg, meshgems_integer *evedg);

/**
 * \ingroup mesh_edge
 * Implementation declaration of how the extra vertices of a mesh edge are obtained.
 * @param[in]	iedg	: index of extra vertices from 1 to nbedg
 * @param[out]	typeedg	: integer containing the type of the edge
 * @param[out]	evedg	: integer[] array containing the extra vertices of the edge
 * @param	user_data: a user pointer.
 * @return a status code
 */
typedef meshgems_status_t (*meshgems_mesh_get_edge_extra_vertices_t)(meshgems_integer iedg, meshgems_integer *typeedg, meshgems_integer *evedg, void *user_data);

/**
 * \ingroup mesh_edge
 * Sets the implementation to use when \ref meshgems_mesh_get_edge_extra_vertices is called by this \ref meshgems_mesh_t object user.
 * @param[in]	m	: the mesh
 * @param[in]	func	: the function implementation
 * @param	user_data: a user pointer to pass to the implementation.
 * @return a status code
 */
MESHGEMS_METHOD meshgems_status_t meshgems_mesh_set_get_edge_extra_vertices(meshgems_mesh_t *m,
							meshgems_mesh_get_edge_extra_vertices_t func,
							void *user_data);
/* **************************************** */
/**
 * \ingroup mesh_triangle
 * Gets the number of triangles in the mesh (generic method called by this \ref meshgems_mesh_t object user).
 * @param[in]	m	: the mesh
 * @param[out]	nbtri	: the number of triangles
 * @return a status code
 */
MESHGEMS_METHOD meshgems_status_t meshgems_mesh_get_triangle_count(meshgems_mesh_t *m, meshgems_integer *nbtri);

/**
 * \ingroup mesh_triangle
 * Implementation declaration of how the number of triangles in the mesh is obtained.
 * @param[out]	nbtri	: the number of triangles
 * @param     user_data	: a user pointer.
 * @return a status code
 */
typedef meshgems_status_t (*meshgems_mesh_get_triangle_count_t)(meshgems_integer *nbtri, void *user_data);

/**
 * \ingroup mesh_triangle
 * Sets the implementation to use when \ref meshgems_mesh_get_triangle_count is called by this \ref meshgems_mesh_t object user.
 * @param[in]	m	: the mesh
 * @param[in]	func	: the function implementation
 * @param     user_data	: a user pointer to pass to the implementation.
 * @return a status code
 */
MESHGEMS_METHOD meshgems_status_t meshgems_mesh_set_get_triangle_count(meshgems_mesh_t *m,
						        meshgems_mesh_get_triangle_count_t func,
						        void *user_data);
/**
 * \ingroup mesh_triangle
 * Gets the vertices of a mesh triangle (generic method called by this \ref meshgems_mesh_t object user).
 * @param[in]	m	: the mesh
 * @param[in]	itri	: index of triangle from 1 to nbtri
 * @param[out]	vtri	: integer[3] array containing the vertices of the triangle
 * @return a status code
 */
MESHGEMS_METHOD meshgems_status_t meshgems_mesh_get_triangle_vertices(meshgems_mesh_t *m, meshgems_integer itri, meshgems_integer *vtri);

/**
 * \ingroup mesh_triangle
 * Implementation declaration of how the vertices of a mesh triangle are obtained.
 * @param[in]	itri	: index of triangle from 1 to nbtri
 * @param[out]	vtri	: integer[3] array containing the vertices of the triangle
 * @param	user_data: a user pointer.
 * @return a status code
 */
typedef meshgems_status_t (*meshgems_mesh_get_triangle_vertices_t)(meshgems_integer itri, meshgems_integer *vtri, void *user_data);

/**
 * \ingroup mesh_triangle
 * Sets the implementation to use when \ref meshgems_mesh_get_triangle_vertices is called by this \ref meshgems_mesh_t object user.
 * @param[in]	m	: the mesh
 * @param[in]	func	: the function implementation
 * @param	user_data: a user pointer to pass to the implementation.
 * @return a status code
 */
MESHGEMS_METHOD meshgems_status_t meshgems_mesh_set_get_triangle_vertices(meshgems_mesh_t *m,
							meshgems_mesh_get_triangle_vertices_t func,
							void *user_data);
/**
 * \ingroup mesh_triangle
 * Gets the tag of a mesh triangle (generic method called by this \ref meshgems_mesh_t object user).
 * @param[in]	m	: the mesh
 * @param[in]	itri	: index of triangle from 1 to nbtri
 * @param[out]	tagtri	: integer tag of the triangle
 * @return a status code
 */
MESHGEMS_METHOD meshgems_status_t meshgems_mesh_get_triangle_tag(meshgems_mesh_t *m, meshgems_integer itri, meshgems_integer *tagtri);

/**
 * \ingroup mesh_triangle
 * Implementation declaration of how the tag of a mesh triangle is obtained.
 * @param[in]	itri	: index of triangle from 1 to nbtri
 * @param[out]	tagtri	: integer tag of the triangle
 * @param	user_data: a user pointer.
 * @return a status code
 */
typedef meshgems_status_t (*meshgems_mesh_get_triangle_tag_t)(meshgems_integer itri, meshgems_integer *tagtri, void *user_data);

/**
 * \ingroup mesh_triangle
 * Sets the implementation to use when \ref meshgems_mesh_get_triangle_tag is called by this \ref meshgems_mesh_t object user.
 * @param[in]	m	: the mesh
 * @param[in]	func	: the function implementation
 * @param	user_data: a user pointer to pass to the implementation.
 * @return a status code
 */
MESHGEMS_METHOD meshgems_status_t meshgems_mesh_set_get_triangle_tag(meshgems_mesh_t *m,
							meshgems_mesh_get_triangle_tag_t func,
							void *user_data);
/* xxxAPPLY_TO_MESH_ARRAY(normal at triangle vertices,real,triangle_normal_coordinates_old,nxyz,tri,3*3,triangle); */
/**
 * \ingroup mesh_triangle
 * Gets the normal coordinates at a mesh triangle vertex (generic method called by this \ref meshgems_mesh_t object user).
 * @param[in]	m	: the mesh
 * @param[in]	itri	: index of triangle from 1 to nbtri
 * @param[in]	itriloc	: index of triangle vertex from 1 to 3
 * @param[out]	nxyz	: real[3] array containing the normal coordinates at the triangle vertex
 * @return a status code
 */
MESHGEMS_METHOD meshgems_status_t meshgems_mesh_get_triangle_vertex_normal_coordinates(meshgems_mesh_t *m, meshgems_integer itri, meshgems_integer itriloc, meshgems_real *nxyz);

/**
 * \ingroup mesh_triangle
 * Implementation declaration of how the normal coordinates at a mesh triangle vertex are obtained.
 * @param[in]	itri	: index of triangle from 1 to nbtri
 * @param[in]	itriloc	: index of triangle vertex from 1 to 3
 * @param[out]	nxyz	: real[3] array containing the normal coordinates at the triangle vertex
 * @param	user_data: a user pointer.
 * @return a status code
 */
typedef meshgems_status_t (*meshgems_mesh_get_triangle_vertex_normal_coordinates_t)(meshgems_integer itri, meshgems_integer itriloc, meshgems_real *nxyz, void *user_data);

/**
 * \ingroup mesh_triangle
 * Sets the implementation to use when \ref meshgems_mesh_get_triangle_vertex_normal_coordinates is called by this \ref meshgems_mesh_t object user.
 * @param[in]	m	: the mesh
 * @param[in]	func	: the function implementation
 * @param	user_data: a user pointer to pass to the implementation.
 * @return a status code
 */
MESHGEMS_METHOD meshgems_status_t meshgems_mesh_set_get_triangle_vertex_normal_coordinates(meshgems_mesh_t *m,
							meshgems_mesh_get_triangle_vertex_normal_coordinates_t func,
							void *user_data);
/**
 * \ingroup mesh_triangle
 * Gets the periodic id of a mesh triangle (generic method called by this \ref meshgems_mesh_t object user).
 * @param[in]	m	: the mesh
 * @param[in]	itri	: index of triangle from 1 to nbtri
 * @param[out]	periodic_id	: integer periodic id of the triangle
 * @return a status code
 */
MESHGEMS_METHOD meshgems_status_t meshgems_mesh_get_triangle_periodic_id(meshgems_mesh_t *m, meshgems_integer itri, meshgems_integer *periodic_id);

/**
 * \ingroup mesh_triangle
 * Implementation declaration of how the periodic id of a mesh triangle is obtained.
 * @param[in]	itri	: index of triangle from 1 to nbtri
 * @param[out]	periodic_id	: integer periodic id of the triangle
 * @param	user_data: a user pointer.
 * @return a status code
 */
typedef meshgems_status_t (*meshgems_mesh_get_triangle_periodic_id_t)(meshgems_integer itri, meshgems_integer *periodic_id, void *user_data);

/**
 * \ingroup mesh_triangle
 * Sets the implementation to use when \ref meshgems_mesh_get_triangle_periodic_id is called by this \ref meshgems_mesh_t object user.
 * @param[in]	m	: the mesh
 * @param[in]	func	: the function implementation
 * @param	user_data: a user pointer to pass to the implementation.
 * @return a status code
 */
MESHGEMS_METHOD meshgems_status_t meshgems_mesh_set_get_triangle_periodic_id(meshgems_mesh_t *m,
							meshgems_mesh_get_triangle_periodic_id_t func,
							void *user_data);
/**
 * \ingroup mesh_triangle
 * Gets the required property of a mesh triangle (generic method called by this \ref meshgems_mesh_t object user).
 * @param[in]	m	: the mesh
 * @param[in]	itri	: index of triangle from 1 to nbtri
 * @param[out]	reqtri	: integer required property of the triangle
 * @return a status code
 */
MESHGEMS_METHOD meshgems_status_t meshgems_mesh_get_triangle_required_property(meshgems_mesh_t *m, meshgems_integer itri, meshgems_integer *reqtri);

/**
 * \ingroup mesh_triangle
 * Implementation declaration of how the required property of a mesh triangle is obtained.
 * @param[in]	itri	: index of triangle from 1 to nbtri
 * @param[out]	reqtri	: integer required property of the triangle
 * @param	user_data: a user pointer.
 * @return a status code
 */
typedef meshgems_status_t (*meshgems_mesh_get_triangle_required_property_t)(meshgems_integer itri, meshgems_integer *reqtri, void *user_data);

/**
 * \ingroup mesh_triangle
 * Sets the implementation to use when \ref meshgems_mesh_get_triangle_required_property is called by this \ref meshgems_mesh_t object user.
 * @param[in]	m	: the mesh
 * @param[in]	func	: the function implementation
 * @param	user_data: a user pointer to pass to the implementation.
 * @return a status code
 */
MESHGEMS_METHOD meshgems_status_t meshgems_mesh_set_get_triangle_required_property(meshgems_mesh_t *m,
							meshgems_mesh_get_triangle_required_property_t func,
							void *user_data);
/**
 * \ingroup mesh_triangle
 * Gets the extra vertices of a mesh triangle (generic method called by this \ref meshgems_mesh_t object user).
 * @param[in]	m	: the mesh
 * @param[in]	itri	: index of triangle from 1 to nbtri
 * @param[out]	typetri	: integer containing the type of the triangle
 * @param[out]	evtri	: integer[] array containing the extra vertices of the triangle
 * @return a status code
 */
MESHGEMS_METHOD meshgems_status_t meshgems_mesh_get_triangle_extra_vertices(meshgems_mesh_t *m, meshgems_integer itri,
                                                    meshgems_integer *typetri, meshgems_integer *evtri);

/**
 * \ingroup mesh_triangle
 * Implementation declaration of how the extra vertices of a mesh triangle are obtained.
 * @param[in]	itri	: index of extra vertices from 1 to nbtri
 * @param[out]	typetri	: integer containing the type of the triangle
 * @param[out]	evtri	: integer[] array containing the extra vertices of the triangle
 * @param	user_data: a user pointer.
 * @return a status code
 */
typedef meshgems_status_t (*meshgems_mesh_get_triangle_extra_vertices_t)(meshgems_integer itri, meshgems_integer *typetri, meshgems_integer *evtri, void *user_data);

/**
 * \ingroup mesh_triangle
 * Sets the implementation to use when \ref meshgems_mesh_get_triangle_extra_vertices is called by this \ref meshgems_mesh_t object user.
 * @param[in]	m	: the mesh
 * @param[in]	func	: the function implementation
 * @param	user_data: a user pointer to pass to the implementation.
 * @return a status code
 */
MESHGEMS_METHOD meshgems_status_t meshgems_mesh_set_get_triangle_extra_vertices(meshgems_mesh_t *m,
							meshgems_mesh_get_triangle_extra_vertices_t func,
							void *user_data);
/* **************************************** */
/**
 * \ingroup mesh_quadrangle
 * Gets the number of quadrilaterals in the mesh (generic method called by this \ref meshgems_mesh_t object user).
 * @param[in]	m	: the mesh
 * @param[out]	nbquad	: the number of quadrilaterals
 * @return a status code
 */
MESHGEMS_METHOD meshgems_status_t meshgems_mesh_get_quadrangle_count(meshgems_mesh_t *m, meshgems_integer *nbquad);

/**
 * \ingroup mesh_quadrangle
 * Implementation declaration of how the number of quadrilaterals in the mesh is obtained.
 * @param[out]	nbquad	: the number of quadrilaterals
 * @param     user_data	: a user pointer.
 * @return a status code
 */
typedef meshgems_status_t (*meshgems_mesh_get_quadrangle_count_t)(meshgems_integer *nbquad, void *user_data);

/**
 * \ingroup mesh_quadrangle
 * Sets the implementation to use when \ref meshgems_mesh_get_quadrangle_count is called by this \ref meshgems_mesh_t object user.
 * @param[in]	m	: the mesh
 * @param[in]	func	: the function implementation
 * @param     user_data	: a user pointer to pass to the implementation.
 * @return a status code
 */
MESHGEMS_METHOD meshgems_status_t meshgems_mesh_set_get_quadrangle_count(meshgems_mesh_t *m,
						        meshgems_mesh_get_quadrangle_count_t func,
						        void *user_data);
/**
 * \ingroup mesh_quadrangle
 * Gets the vertices of a mesh quadrangle (generic method called by this \ref meshgems_mesh_t object user).
 * @param[in]	m	: the mesh
 * @param[in]	iquad	: index of quadrangle from 1 to nbquad
 * @param[out]	vquad	: integer[4] array containing the vertices of the quadrangle
 * @return a status code
 */
MESHGEMS_METHOD meshgems_status_t meshgems_mesh_get_quadrangle_vertices(meshgems_mesh_t *m, meshgems_integer iquad, meshgems_integer *vquad);

/**
 * \ingroup mesh_quadrangle
 * Implementation declaration of how the vertices of a mesh quadrangle are obtained.
 * @param[in]	iquad	: index of quadrangle from 1 to nbquad
 * @param[out]	vquad	: integer[4] array containing the vertices of the quadrangle
 * @param	user_data: a user pointer.
 * @return a status code
 */
typedef meshgems_status_t (*meshgems_mesh_get_quadrangle_vertices_t)(meshgems_integer iquad, meshgems_integer *vquad, void *user_data);

/**
 * \ingroup mesh_quadrangle
 * Sets the implementation to use when \ref meshgems_mesh_get_quadrangle_vertices is called by this \ref meshgems_mesh_t object user.
 * @param[in]	m	: the mesh
 * @param[in]	func	: the function implementation
 * @param	user_data: a user pointer to pass to the implementation.
 * @return a status code
 */
MESHGEMS_METHOD meshgems_status_t meshgems_mesh_set_get_quadrangle_vertices(meshgems_mesh_t *m,
							meshgems_mesh_get_quadrangle_vertices_t func,
							void *user_data);
/**
 * \ingroup mesh_quadrangle
 * Gets the tag of a mesh quadrangle (generic method called by this \ref meshgems_mesh_t object user).
 * @param[in]	m	: the mesh
 * @param[in]	iquad	: index of quadrangle from 1 to nbquad
 * @param[out]	tagquad	: integer tag of the quadrangle
 * @return a status code
 */
MESHGEMS_METHOD meshgems_status_t meshgems_mesh_get_quadrangle_tag(meshgems_mesh_t *m, meshgems_integer iquad, meshgems_integer *tagquad);

/**
 * \ingroup mesh_quadrangle
 * Implementation declaration of how the tag of a mesh quadrangle is obtained.
 * @param[in]	iquad	: index of quadrangle from 1 to nbquad
 * @param[out]	tagquad	: integer tag of the quadrangle
 * @param	user_data: a user pointer.
 * @return a status code
 */
typedef meshgems_status_t (*meshgems_mesh_get_quadrangle_tag_t)(meshgems_integer iquad, meshgems_integer *tagquad, void *user_data);

/**
 * \ingroup mesh_quadrangle
 * Sets the implementation to use when \ref meshgems_mesh_get_quadrangle_tag is called by this \ref meshgems_mesh_t object user.
 * @param[in]	m	: the mesh
 * @param[in]	func	: the function implementation
 * @param	user_data: a user pointer to pass to the implementation.
 * @return a status code
 */
MESHGEMS_METHOD meshgems_status_t meshgems_mesh_set_get_quadrangle_tag(meshgems_mesh_t *m,
							meshgems_mesh_get_quadrangle_tag_t func,
							void *user_data);
/**
 * \ingroup mesh_quadrangle
 * Gets the normal coordinates at a mesh quadrangle vertex (generic method called by this \ref meshgems_mesh_t object user).
 * @param[in]	m	: the mesh
 * @param[in]	iquad	: index of quadrangle from 1 to nbquad
 * @param[in]	iquadloc	: index of quadrangle vertex from 1 to 4
 * @param[out]	nxyz	: real[3] array containing the normal coordinates at the quadrangle vertex
 * @return a status code
 */
MESHGEMS_METHOD meshgems_status_t meshgems_mesh_get_quadrangle_vertex_normal_coordinates(meshgems_mesh_t *m, meshgems_integer iquad, meshgems_integer iquadloc, meshgems_real *nxyz);

/**
 * \ingroup mesh_quadrangle
 * Implementation declaration of how the normal coordinates at a mesh quadrangle vertex are obtained.
 * @param[in]	iquad	: index of quadrangle from 1 to nbquad
 * @param[in]	iquadloc	: index of quadrangle vertex from 1 to 4
 * @param[out]	nxyz	: real[3] array containing the normal coordinates at the quadrangle vertex
 * @param	user_data: a user pointer.
 * @return a status code
 */
typedef meshgems_status_t (*meshgems_mesh_get_quadrangle_vertex_normal_coordinates_t)(meshgems_integer iquad, meshgems_integer iquadloc, meshgems_real *nxyz, void *user_data);

/**
 * \ingroup mesh_quadrangle
 * Sets the implementation to use when \ref meshgems_mesh_get_quadrangle_vertex_normal_coordinates is called by this \ref meshgems_mesh_t object user.
 * @param[in]	m	: the mesh
 * @param[in]	func	: the function implementation
 * @param	user_data: a user pointer to pass to the implementation.
 * @return a status code
 */
MESHGEMS_METHOD meshgems_status_t meshgems_mesh_set_get_quadrangle_vertex_normal_coordinates(meshgems_mesh_t *m,
							meshgems_mesh_get_quadrangle_vertex_normal_coordinates_t func,
							void *user_data);
/**
 * \ingroup mesh_quadrangle
 * Gets the periodic id of a mesh quadrangle (generic method called by this \ref meshgems_mesh_t object user).
 * @param[in]	m	: the mesh
 * @param[in]	iquad	: index of quadrangle from 1 to nbquad
 * @param[out]	periodic_id	: integer periodic id of the quadrangle
 * @return a status code
 */
MESHGEMS_METHOD meshgems_status_t meshgems_mesh_get_quadrangle_periodic_id(meshgems_mesh_t *m, meshgems_integer iquad, meshgems_integer *periodic_id);

/**
 * \ingroup mesh_quadrangle
 * Implementation declaration of how the periodic id of a mesh quadrangle is obtained.
 * @param[in]	iquad	: index of quadrangle from 1 to nbquad
 * @param[out]	periodic_id	: integer periodic id of the quadrangle
 * @param	user_data: a user pointer.
 * @return a status code
 */
typedef meshgems_status_t (*meshgems_mesh_get_quadrangle_periodic_id_t)(meshgems_integer iquad, meshgems_integer *periodic_id, void *user_data);

/**
 * \ingroup mesh_quadrangle
 * Sets the implementation to use when \ref meshgems_mesh_get_quadrangle_periodic_id is called by this \ref meshgems_mesh_t object user.
 * @param[in]	m	: the mesh
 * @param[in]	func	: the function implementation
 * @param	user_data: a user pointer to pass to the implementation.
 * @return a status code
 */
MESHGEMS_METHOD meshgems_status_t meshgems_mesh_set_get_quadrangle_periodic_id(meshgems_mesh_t *m,
							meshgems_mesh_get_quadrangle_periodic_id_t func,
							void *user_data);
/**
 * \ingroup mesh_quadrangle
 * Gets the required property of a mesh quadrangle (generic method called by this \ref meshgems_mesh_t object user).
 * @param[in]	m	: the mesh
 * @param[in]	iquad	: index of quadrangle from 1 to nbquad
 * @param[out]	reqquad	: integer required property of the quadrangle
 * @return a status code
 */
MESHGEMS_METHOD meshgems_status_t meshgems_mesh_get_quadrangle_required_property(meshgems_mesh_t *m, meshgems_integer iquad, meshgems_integer *reqquad);

/**
 * \ingroup mesh_quadrangle
 * Implementation declaration of how the required property of a mesh quadrangle is obtained.
 * @param[in]	iquad	: index of quadrangle from 1 to nbquad
 * @param[out]	reqquad	: integer required property of the quadrangle
 * @param	user_data: a user pointer.
 * @return a status code
 */
typedef meshgems_status_t (*meshgems_mesh_get_quadrangle_required_property_t)(meshgems_integer iquad, meshgems_integer *reqquad, void *user_data);

/**
 * \ingroup mesh_quadrangle
 * Sets the implementation to use when \ref meshgems_mesh_get_quadrangle_required_property is called by this \ref meshgems_mesh_t object user.
 * @param[in]	m	: the mesh
 * @param[in]	func	: the function implementation
 * @param	user_data: a user pointer to pass to the implementation.
 * @return a status code
 */
MESHGEMS_METHOD meshgems_status_t meshgems_mesh_set_get_quadrangle_required_property(meshgems_mesh_t *m,
							meshgems_mesh_get_quadrangle_required_property_t func,
							void *user_data);
/**
 * \ingroup mesh_quadrangle
 * Gets the extra vertices of a mesh quadrangle (generic method called by this \ref meshgems_mesh_t object user).
 * @param[in]	m	: the mesh
 * @param[in]	iquad	: index of quadrangle from 1 to nbquad
 * @param[out]	typequad	: integer containing the type of the quadrangle
 * @param[out]	evquad	: integer[] array containing the extra vertices of the quadrangle
 * @return a status code
 */
MESHGEMS_METHOD meshgems_status_t meshgems_mesh_get_quadrangle_extra_vertices(meshgems_mesh_t *m, meshgems_integer iquad,
                                                    meshgems_integer *typequad, meshgems_integer *evquad);

/**
 * \ingroup mesh_quadrangle
 * Implementation declaration of how the extra vertices of a mesh quadrangle are obtained.
 * @param[in]	iquad	: index of extra vertices from 1 to nbquad
 * @param[out]	typequad	: integer containing the type of the quadrangle
 * @param[out]	evquad	: integer[] array containing the extra vertices of the quadrangle
 * @param	user_data: a user pointer.
 * @return a status code
 */
typedef meshgems_status_t (*meshgems_mesh_get_quadrangle_extra_vertices_t)(meshgems_integer iquad, meshgems_integer *typequad, meshgems_integer *evquad, void *user_data);

/**
 * \ingroup mesh_quadrangle
 * Sets the implementation to use when \ref meshgems_mesh_get_quadrangle_extra_vertices is called by this \ref meshgems_mesh_t object user.
 * @param[in]	m	: the mesh
 * @param[in]	func	: the function implementation
 * @param	user_data: a user pointer to pass to the implementation.
 * @return a status code
 */
MESHGEMS_METHOD meshgems_status_t meshgems_mesh_set_get_quadrangle_extra_vertices(meshgems_mesh_t *m,
							meshgems_mesh_get_quadrangle_extra_vertices_t func,
							void *user_data);
/* **************************************** */
/**
 * \ingroup mesh_tetrahedron
 * Gets the number of tetrahedra in the mesh (generic method called by this \ref meshgems_mesh_t object user).
 * @param[in]	m	: the mesh
 * @param[out]	nbtet	: the number of tetrahedra
 * @return a status code
 */
MESHGEMS_METHOD meshgems_status_t meshgems_mesh_get_tetrahedron_count(meshgems_mesh_t *m, meshgems_integer *nbtet);

/**
 * \ingroup mesh_tetrahedron
 * Implementation declaration of how the number of tetrahedra in the mesh is obtained.
 * @param[out]	nbtet	: the number of tetrahedra
 * @param     user_data	: a user pointer.
 * @return a status code
 */
typedef meshgems_status_t (*meshgems_mesh_get_tetrahedron_count_t)(meshgems_integer *nbtet, void *user_data);

/**
 * \ingroup mesh_tetrahedron
 * Sets the implementation to use when \ref meshgems_mesh_get_tetrahedron_count is called by this \ref meshgems_mesh_t object user.
 * @param[in]	m	: the mesh
 * @param[in]	func	: the function implementation
 * @param     user_data	: a user pointer to pass to the implementation.
 * @return a status code
 */
MESHGEMS_METHOD meshgems_status_t meshgems_mesh_set_get_tetrahedron_count(meshgems_mesh_t *m,
						        meshgems_mesh_get_tetrahedron_count_t func,
						        void *user_data);
/**
 * \ingroup mesh_tetrahedron
 * Gets the vertices of a mesh tetrahedron (generic method called by this \ref meshgems_mesh_t object user).
 * @param[in]	m	: the mesh
 * @param[in]	itet	: index of tetrahedron from 1 to nbtet
 * @param[out]	vtet	: integer[4] array containing the vertices of the tetrahedron
 * @return a status code
 */
MESHGEMS_METHOD meshgems_status_t meshgems_mesh_get_tetrahedron_vertices(meshgems_mesh_t *m, meshgems_integer itet, meshgems_integer *vtet);

/**
 * \ingroup mesh_tetrahedron
 * Implementation declaration of how the vertices of a mesh tetrahedron are obtained.
 * @param[in]	itet	: index of tetrahedron from 1 to nbtet
 * @param[out]	vtet	: integer[4] array containing the vertices of the tetrahedron
 * @param	user_data: a user pointer.
 * @return a status code
 */
typedef meshgems_status_t (*meshgems_mesh_get_tetrahedron_vertices_t)(meshgems_integer itet, meshgems_integer *vtet, void *user_data);

/**
 * \ingroup mesh_tetrahedron
 * Sets the implementation to use when \ref meshgems_mesh_get_tetrahedron_vertices is called by this \ref meshgems_mesh_t object user.
 * @param[in]	m	: the mesh
 * @param[in]	func	: the function implementation
 * @param	user_data: a user pointer to pass to the implementation.
 * @return a status code
 */
MESHGEMS_METHOD meshgems_status_t meshgems_mesh_set_get_tetrahedron_vertices(meshgems_mesh_t *m,
							meshgems_mesh_get_tetrahedron_vertices_t func,
							void *user_data);
/**
 * \ingroup mesh_tetrahedron
 * Gets the tag of a mesh tetrahedron (generic method called by this \ref meshgems_mesh_t object user).
 * @param[in]	m	: the mesh
 * @param[in]	itet	: index of tetrahedron from 1 to nbtet
 * @param[out]	tagtet	: integer tag of the tetrahedron
 * @return a status code
 */
MESHGEMS_METHOD meshgems_status_t meshgems_mesh_get_tetrahedron_tag(meshgems_mesh_t *m, meshgems_integer itet, meshgems_integer *tagtet);

/**
 * \ingroup mesh_tetrahedron
 * Implementation declaration of how the tag of a mesh tetrahedron is obtained.
 * @param[in]	itet	: index of tetrahedron from 1 to nbtet
 * @param[out]	tagtet	: integer tag of the tetrahedron
 * @param	user_data: a user pointer.
 * @return a status code
 */
typedef meshgems_status_t (*meshgems_mesh_get_tetrahedron_tag_t)(meshgems_integer itet, meshgems_integer *tagtet, void *user_data);

/**
 * \ingroup mesh_tetrahedron
 * Sets the implementation to use when \ref meshgems_mesh_get_tetrahedron_tag is called by this \ref meshgems_mesh_t object user.
 * @param[in]	m	: the mesh
 * @param[in]	func	: the function implementation
 * @param	user_data: a user pointer to pass to the implementation.
 * @return a status code
 */
MESHGEMS_METHOD meshgems_status_t meshgems_mesh_set_get_tetrahedron_tag(meshgems_mesh_t *m,
							meshgems_mesh_get_tetrahedron_tag_t func,
							void *user_data);
/**
 * \ingroup mesh_tetrahedron
 * Gets the required property of a mesh tetrahedron (generic method called by this \ref meshgems_mesh_t object user).
 * @param[in]	m	: the mesh
 * @param[in]	itet	: index of tetrahedron from 1 to nbtet
 * @param[out]	reqtet	: integer required property of the tetrahedron
 * @return a status code
 */
MESHGEMS_METHOD meshgems_status_t meshgems_mesh_get_tetrahedron_required_property(meshgems_mesh_t *m, meshgems_integer itet, meshgems_integer *reqtet);

/**
 * \ingroup mesh_tetrahedron
 * Implementation declaration of how the required property of a mesh tetrahedron is obtained.
 * @param[in]	itet	: index of tetrahedron from 1 to nbtet
 * @param[out]	reqtet	: integer required property of the tetrahedron
 * @param	user_data: a user pointer.
 * @return a status code
 */
typedef meshgems_status_t (*meshgems_mesh_get_tetrahedron_required_property_t)(meshgems_integer itet, meshgems_integer *reqtet, void *user_data);

/**
 * \ingroup mesh_tetrahedron
 * Sets the implementation to use when \ref meshgems_mesh_get_tetrahedron_required_property is called by this \ref meshgems_mesh_t object user.
 * @param[in]	m	: the mesh
 * @param[in]	func	: the function implementation
 * @param	user_data: a user pointer to pass to the implementation.
 * @return a status code
 */
MESHGEMS_METHOD meshgems_status_t meshgems_mesh_set_get_tetrahedron_required_property(meshgems_mesh_t *m,
							meshgems_mesh_get_tetrahedron_required_property_t func,
							void *user_data);
/**
 * \ingroup mesh_tetrahedron
 * Gets the extra vertices of a mesh tetrahedron (generic method called by this \ref meshgems_mesh_t object user).
 * @param[in]	m	: the mesh
 * @param[in]	itet	: index of tetrahedron from 1 to nbtet
 * @param[out]	typetet	: integer containing the type of the tetrahedron
 * @param[out]	evtet	: integer[] array containing the extra vertices of the tetrahedron
 * @return a status code
 */
MESHGEMS_METHOD meshgems_status_t meshgems_mesh_get_tetrahedron_extra_vertices(meshgems_mesh_t *m, meshgems_integer itet,
                                                    meshgems_integer *typetet, meshgems_integer *evtet);

/**
 * \ingroup mesh_tetrahedron
 * Implementation declaration of how the extra vertices of a mesh tetrahedron are obtained.
 * @param[in]	itet	: index of extra vertices from 1 to nbtet
 * @param[out]	typetet	: integer containing the type of the tetrahedron
 * @param[out]	evtet	: integer[] array containing the extra vertices of the tetrahedron
 * @param	user_data: a user pointer.
 * @return a status code
 */
typedef meshgems_status_t (*meshgems_mesh_get_tetrahedron_extra_vertices_t)(meshgems_integer itet, meshgems_integer *typetet, meshgems_integer *evtet, void *user_data);

/**
 * \ingroup mesh_tetrahedron
 * Sets the implementation to use when \ref meshgems_mesh_get_tetrahedron_extra_vertices is called by this \ref meshgems_mesh_t object user.
 * @param[in]	m	: the mesh
 * @param[in]	func	: the function implementation
 * @param	user_data: a user pointer to pass to the implementation.
 * @return a status code
 */
MESHGEMS_METHOD meshgems_status_t meshgems_mesh_set_get_tetrahedron_extra_vertices(meshgems_mesh_t *m,
							meshgems_mesh_get_tetrahedron_extra_vertices_t func,
							void *user_data);
/* **************************************** */
/**
 * \ingroup mesh_hexahedron
 * Gets the number of hexahedra in the mesh (generic method called by this \ref meshgems_mesh_t object user).
 * @param[in]	m	: the mesh
 * @param[out]	nbhex	: the number of hexahedra
 * @return a status code
 */
MESHGEMS_METHOD meshgems_status_t meshgems_mesh_get_hexahedron_count(meshgems_mesh_t *m, meshgems_integer *nbhex);

/**
 * \ingroup mesh_hexahedron
 * Implementation declaration of how the number of hexahedra in the mesh is obtained.
 * @param[out]	nbhex	: the number of hexahedra
 * @param     user_data	: a user pointer.
 * @return a status code
 */
typedef meshgems_status_t (*meshgems_mesh_get_hexahedron_count_t)(meshgems_integer *nbhex, void *user_data);

/**
 * \ingroup mesh_hexahedron
 * Sets the implementation to use when \ref meshgems_mesh_get_hexahedron_count is called by this \ref meshgems_mesh_t object user.
 * @param[in]	m	: the mesh
 * @param[in]	func	: the function implementation
 * @param     user_data	: a user pointer to pass to the implementation.
 * @return a status code
 */
MESHGEMS_METHOD meshgems_status_t meshgems_mesh_set_get_hexahedron_count(meshgems_mesh_t *m,
						        meshgems_mesh_get_hexahedron_count_t func,
						        void *user_data);
/**
 * \ingroup mesh_hexahedron
 * Gets the vertices of a mesh hexahedron (generic method called by this \ref meshgems_mesh_t object user).
 * @param[in]	m	: the mesh
 * @param[in]	ihex	: index of hexahedron from 1 to nbhex
 * @param[out]	vhex	: integer[8] array containing the vertices of the hexahedron
 * @return a status code
 */
MESHGEMS_METHOD meshgems_status_t meshgems_mesh_get_hexahedron_vertices(meshgems_mesh_t *m, meshgems_integer ihex, meshgems_integer *vhex);

/**
 * \ingroup mesh_hexahedron
 * Implementation declaration of how the vertices of a mesh hexahedron are obtained.
 * @param[in]	ihex	: index of hexahedron from 1 to nbhex
 * @param[out]	vhex	: integer[8] array containing the vertices of the hexahedron
 * @param	user_data: a user pointer.
 * @return a status code
 */
typedef meshgems_status_t (*meshgems_mesh_get_hexahedron_vertices_t)(meshgems_integer ihex, meshgems_integer *vhex, void *user_data);

/**
 * \ingroup mesh_hexahedron
 * Sets the implementation to use when \ref meshgems_mesh_get_hexahedron_vertices is called by this \ref meshgems_mesh_t object user.
 * @param[in]	m	: the mesh
 * @param[in]	func	: the function implementation
 * @param	user_data: a user pointer to pass to the implementation.
 * @return a status code
 */
MESHGEMS_METHOD meshgems_status_t meshgems_mesh_set_get_hexahedron_vertices(meshgems_mesh_t *m,
							meshgems_mesh_get_hexahedron_vertices_t func,
							void *user_data);
/**
 * \ingroup mesh_hexahedron
 * Gets the tag of a mesh hexahedron (generic method called by this \ref meshgems_mesh_t object user).
 * @param[in]	m	: the mesh
 * @param[in]	ihex	: index of hexahedron from 1 to nbhex
 * @param[out]	taghex	: integer tag of the hexahedron
 * @return a status code
 */
MESHGEMS_METHOD meshgems_status_t meshgems_mesh_get_hexahedron_tag(meshgems_mesh_t *m, meshgems_integer ihex, meshgems_integer *taghex);

/**
 * \ingroup mesh_hexahedron
 * Implementation declaration of how the tag of a mesh hexahedron is obtained.
 * @param[in]	ihex	: index of hexahedron from 1 to nbhex
 * @param[out]	taghex	: integer tag of the hexahedron
 * @param	user_data: a user pointer.
 * @return a status code
 */
typedef meshgems_status_t (*meshgems_mesh_get_hexahedron_tag_t)(meshgems_integer ihex, meshgems_integer *taghex, void *user_data);

/**
 * \ingroup mesh_hexahedron
 * Sets the implementation to use when \ref meshgems_mesh_get_hexahedron_tag is called by this \ref meshgems_mesh_t object user.
 * @param[in]	m	: the mesh
 * @param[in]	func	: the function implementation
 * @param	user_data: a user pointer to pass to the implementation.
 * @return a status code
 */
MESHGEMS_METHOD meshgems_status_t meshgems_mesh_set_get_hexahedron_tag(meshgems_mesh_t *m,
							meshgems_mesh_get_hexahedron_tag_t func,
							void *user_data);
/**
 * \ingroup mesh_hexahedron
 * Gets the required property of a mesh hexahedron (generic method called by this \ref meshgems_mesh_t object user).
 * @param[in]	m	: the mesh
 * @param[in]	ihex	: index of hexahedron from 1 to nbhex
 * @param[out]	reqhex	: integer required property of the hexahedron
 * @return a status code
 */
MESHGEMS_METHOD meshgems_status_t meshgems_mesh_get_hexahedron_required_property(meshgems_mesh_t *m, meshgems_integer ihex, meshgems_integer *reqhex);

/**
 * \ingroup mesh_hexahedron
 * Implementation declaration of how the required property of a mesh hexahedron is obtained.
 * @param[in]	ihex	: index of hexahedron from 1 to nbhex
 * @param[out]	reqhex	: integer required property of the hexahedron
 * @param	user_data: a user pointer.
 * @return a status code
 */
typedef meshgems_status_t (*meshgems_mesh_get_hexahedron_required_property_t)(meshgems_integer ihex, meshgems_integer *reqhex, void *user_data);

/**
 * \ingroup mesh_hexahedron
 * Sets the implementation to use when \ref meshgems_mesh_get_hexahedron_required_property is called by this \ref meshgems_mesh_t object user.
 * @param[in]	m	: the mesh
 * @param[in]	func	: the function implementation
 * @param	user_data: a user pointer to pass to the implementation.
 * @return a status code
 */
MESHGEMS_METHOD meshgems_status_t meshgems_mesh_set_get_hexahedron_required_property(meshgems_mesh_t *m,
							meshgems_mesh_get_hexahedron_required_property_t func,
							void *user_data);
/**
 * \ingroup mesh_hexahedron
 * Gets the extra vertices of a mesh hexahedron (generic method called by this \ref meshgems_mesh_t object user).
 * @param[in]	m	: the mesh
 * @param[in]	ihex	: index of hexahedron from 1 to nbhex
 * @param[out]	typehex	: integer containing the type of the hexahedron
 * @param[out]	evhex	: integer[] array containing the extra vertices of the hexahedron
 * @return a status code
 */
MESHGEMS_METHOD meshgems_status_t meshgems_mesh_get_hexahedron_extra_vertices(meshgems_mesh_t *m, meshgems_integer ihex,
                                                    meshgems_integer *typehex, meshgems_integer *evhex);

/**
 * \ingroup mesh_hexahedron
 * Implementation declaration of how the extra vertices of a mesh hexahedron are obtained.
 * @param[in]	ihex	: index of extra vertices from 1 to nbhex
 * @param[out]	typehex	: integer containing the type of the hexahedron
 * @param[out]	evhex	: integer[] array containing the extra vertices of the hexahedron
 * @param	user_data: a user pointer.
 * @return a status code
 */
typedef meshgems_status_t (*meshgems_mesh_get_hexahedron_extra_vertices_t)(meshgems_integer ihex, meshgems_integer *typehex, meshgems_integer *evhex, void *user_data);

/**
 * \ingroup mesh_hexahedron
 * Sets the implementation to use when \ref meshgems_mesh_get_hexahedron_extra_vertices is called by this \ref meshgems_mesh_t object user.
 * @param[in]	m	: the mesh
 * @param[in]	func	: the function implementation
 * @param	user_data: a user pointer to pass to the implementation.
 * @return a status code
 */
MESHGEMS_METHOD meshgems_status_t meshgems_mesh_set_get_hexahedron_extra_vertices(meshgems_mesh_t *m,
							meshgems_mesh_get_hexahedron_extra_vertices_t func,
							void *user_data);
/* **************************************** */
/**
 * \ingroup mesh_pyramid
 * Gets the number of pyramids in the mesh (generic method called by this \ref meshgems_mesh_t object user).
 * @param[in]	m	: the mesh
 * @param[out]	nbpyr	: the number of pyramids
 * @return a status code
 */
MESHGEMS_METHOD meshgems_status_t meshgems_mesh_get_pyramid_count(meshgems_mesh_t *m, meshgems_integer *nbpyr);

/**
 * \ingroup mesh_pyramid
 * Implementation declaration of how the number of pyramids in the mesh is obtained.
 * @param[out]	nbpyr	: the number of pyramids
 * @param     user_data	: a user pointer.
 * @return a status code
 */
typedef meshgems_status_t (*meshgems_mesh_get_pyramid_count_t)(meshgems_integer *nbpyr, void *user_data);

/**
 * \ingroup mesh_pyramid
 * Sets the implementation to use when \ref meshgems_mesh_get_pyramid_count is called by this \ref meshgems_mesh_t object user.
 * @param[in]	m	: the mesh
 * @param[in]	func	: the function implementation
 * @param     user_data	: a user pointer to pass to the implementation.
 * @return a status code
 */
MESHGEMS_METHOD meshgems_status_t meshgems_mesh_set_get_pyramid_count(meshgems_mesh_t *m,
						        meshgems_mesh_get_pyramid_count_t func,
						        void *user_data);
/**
 * \ingroup mesh_pyramid
 * Gets the vertices of a mesh pyramid (generic method called by this \ref meshgems_mesh_t object user).
 * @param[in]	m	: the mesh
 * @param[in]	ipyr	: index of pyramid from 1 to nbpyr
 * @param[out]	vpyr	: integer[5] array containing the vertices of the pyramid
 * @return a status code
 */
MESHGEMS_METHOD meshgems_status_t meshgems_mesh_get_pyramid_vertices(meshgems_mesh_t *m, meshgems_integer ipyr, meshgems_integer *vpyr);

/**
 * \ingroup mesh_pyramid
 * Implementation declaration of how the vertices of a mesh pyramid are obtained.
 * @param[in]	ipyr	: index of pyramid from 1 to nbpyr
 * @param[out]	vpyr	: integer[5] array containing the vertices of the pyramid
 * @param	user_data: a user pointer.
 * @return a status code
 */
typedef meshgems_status_t (*meshgems_mesh_get_pyramid_vertices_t)(meshgems_integer ipyr, meshgems_integer *vpyr, void *user_data);

/**
 * \ingroup mesh_pyramid
 * Sets the implementation to use when \ref meshgems_mesh_get_pyramid_vertices is called by this \ref meshgems_mesh_t object user.
 * @param[in]	m	: the mesh
 * @param[in]	func	: the function implementation
 * @param	user_data: a user pointer to pass to the implementation.
 * @return a status code
 */
MESHGEMS_METHOD meshgems_status_t meshgems_mesh_set_get_pyramid_vertices(meshgems_mesh_t *m,
							meshgems_mesh_get_pyramid_vertices_t func,
							void *user_data);
/**
 * \ingroup mesh_pyramid
 * Gets the tag of a mesh pyramid (generic method called by this \ref meshgems_mesh_t object user).
 * @param[in]	m	: the mesh
 * @param[in]	ipyr	: index of pyramid from 1 to nbpyr
 * @param[out]	tagpyr	: integer tag of the pyramid
 * @return a status code
 */
MESHGEMS_METHOD meshgems_status_t meshgems_mesh_get_pyramid_tag(meshgems_mesh_t *m, meshgems_integer ipyr, meshgems_integer *tagpyr);

/**
 * \ingroup mesh_pyramid
 * Implementation declaration of how the tag of a mesh pyramid is obtained.
 * @param[in]	ipyr	: index of pyramid from 1 to nbpyr
 * @param[out]	tagpyr	: integer tag of the pyramid
 * @param	user_data: a user pointer.
 * @return a status code
 */
typedef meshgems_status_t (*meshgems_mesh_get_pyramid_tag_t)(meshgems_integer ipyr, meshgems_integer *tagpyr, void *user_data);

/**
 * \ingroup mesh_pyramid
 * Sets the implementation to use when \ref meshgems_mesh_get_pyramid_tag is called by this \ref meshgems_mesh_t object user.
 * @param[in]	m	: the mesh
 * @param[in]	func	: the function implementation
 * @param	user_data: a user pointer to pass to the implementation.
 * @return a status code
 */
MESHGEMS_METHOD meshgems_status_t meshgems_mesh_set_get_pyramid_tag(meshgems_mesh_t *m,
							meshgems_mesh_get_pyramid_tag_t func,
							void *user_data);
/**
 * \ingroup mesh_pyramid
 * Gets the required property of a mesh pyramid (generic method called by this \ref meshgems_mesh_t object user).
 * @param[in]	m	: the mesh
 * @param[in]	ipyr	: index of pyramid from 1 to nbpyr
 * @param[out]	reqpyr	: integer required property of the pyramid
 * @return a status code
 */
MESHGEMS_METHOD meshgems_status_t meshgems_mesh_get_pyramid_required_property(meshgems_mesh_t *m, meshgems_integer ipyr, meshgems_integer *reqpyr);

/**
 * \ingroup mesh_pyramid
 * Implementation declaration of how the required property of a mesh pyramid is obtained.
 * @param[in]	ipyr	: index of pyramid from 1 to nbpyr
 * @param[out]	reqpyr	: integer required property of the pyramid
 * @param	user_data: a user pointer.
 * @return a status code
 */
typedef meshgems_status_t (*meshgems_mesh_get_pyramid_required_property_t)(meshgems_integer ipyr, meshgems_integer *reqpyr, void *user_data);

/**
 * \ingroup mesh_pyramid
 * Sets the implementation to use when \ref meshgems_mesh_get_pyramid_required_property is called by this \ref meshgems_mesh_t object user.
 * @param[in]	m	: the mesh
 * @param[in]	func	: the function implementation
 * @param	user_data: a user pointer to pass to the implementation.
 * @return a status code
 */
MESHGEMS_METHOD meshgems_status_t meshgems_mesh_set_get_pyramid_required_property(meshgems_mesh_t *m,
							meshgems_mesh_get_pyramid_required_property_t func,
							void *user_data);
/**
 * \ingroup mesh_pyramid
 * Gets the extra vertices of a mesh pyramid (generic method called by this \ref meshgems_mesh_t object user).
 * @param[in]	m	: the mesh
 * @param[in]	ipyr	: index of pyramid from 1 to nbpyr
 * @param[out]	typepyr	: integer containing the type of the pyramid
 * @param[out]	evpyr	: integer[] array containing the extra vertices of the pyramid
 * @return a status code
 */
MESHGEMS_METHOD meshgems_status_t meshgems_mesh_get_pyramid_extra_vertices(meshgems_mesh_t *m, meshgems_integer ipyr,
                                                    meshgems_integer *typepyr, meshgems_integer *evpyr);

/**
 * \ingroup mesh_pyramid
 * Implementation declaration of how the extra vertices of a mesh pyramid are obtained.
 * @param[in]	ipyr	: index of extra vertices from 1 to nbpyr
 * @param[out]	typepyr	: integer containing the type of the pyramid
 * @param[out]	evpyr	: integer[] array containing the extra vertices of the pyramid
 * @param	user_data: a user pointer.
 * @return a status code
 */
typedef meshgems_status_t (*meshgems_mesh_get_pyramid_extra_vertices_t)(meshgems_integer ipyr, meshgems_integer *typepyr, meshgems_integer *evpyr, void *user_data);

/**
 * \ingroup mesh_pyramid
 * Sets the implementation to use when \ref meshgems_mesh_get_pyramid_extra_vertices is called by this \ref meshgems_mesh_t object user.
 * @param[in]	m	: the mesh
 * @param[in]	func	: the function implementation
 * @param	user_data: a user pointer to pass to the implementation.
 * @return a status code
 */
MESHGEMS_METHOD meshgems_status_t meshgems_mesh_set_get_pyramid_extra_vertices(meshgems_mesh_t *m,
							meshgems_mesh_get_pyramid_extra_vertices_t func,
							void *user_data);
/* **************************************** */
/**
 * \ingroup mesh_prism
 * Gets the number of prisms in the mesh (generic method called by this \ref meshgems_mesh_t object user).
 * @param[in]	m	: the mesh
 * @param[out]	nbprism	: the number of prisms
 * @return a status code
 */
MESHGEMS_METHOD meshgems_status_t meshgems_mesh_get_prism_count(meshgems_mesh_t *m, meshgems_integer *nbprism);

/**
 * \ingroup mesh_prism
 * Implementation declaration of how the number of prisms in the mesh is obtained.
 * @param[out]	nbprism	: the number of prisms
 * @param     user_data	: a user pointer.
 * @return a status code
 */
typedef meshgems_status_t (*meshgems_mesh_get_prism_count_t)(meshgems_integer *nbprism, void *user_data);

/**
 * \ingroup mesh_prism
 * Sets the implementation to use when \ref meshgems_mesh_get_prism_count is called by this \ref meshgems_mesh_t object user.
 * @param[in]	m	: the mesh
 * @param[in]	func	: the function implementation
 * @param     user_data	: a user pointer to pass to the implementation.
 * @return a status code
 */
MESHGEMS_METHOD meshgems_status_t meshgems_mesh_set_get_prism_count(meshgems_mesh_t *m,
						        meshgems_mesh_get_prism_count_t func,
						        void *user_data);
/**
 * \ingroup mesh_prism
 * Gets the vertices of a mesh prism (generic method called by this \ref meshgems_mesh_t object user).
 * @param[in]	m	: the mesh
 * @param[in]	iprism	: index of prism from 1 to nbprism
 * @param[out]	vprism	: integer[6] array containing the vertices of the prism
 * @return a status code
 */
MESHGEMS_METHOD meshgems_status_t meshgems_mesh_get_prism_vertices(meshgems_mesh_t *m, meshgems_integer iprism, meshgems_integer *vprism);

/**
 * \ingroup mesh_prism
 * Implementation declaration of how the vertices of a mesh prism are obtained.
 * @param[in]	iprism	: index of prism from 1 to nbprism
 * @param[out]	vprism	: integer[6] array containing the vertices of the prism
 * @param	user_data: a user pointer.
 * @return a status code
 */
typedef meshgems_status_t (*meshgems_mesh_get_prism_vertices_t)(meshgems_integer iprism, meshgems_integer *vprism, void *user_data);

/**
 * \ingroup mesh_prism
 * Sets the implementation to use when \ref meshgems_mesh_get_prism_vertices is called by this \ref meshgems_mesh_t object user.
 * @param[in]	m	: the mesh
 * @param[in]	func	: the function implementation
 * @param	user_data: a user pointer to pass to the implementation.
 * @return a status code
 */
MESHGEMS_METHOD meshgems_status_t meshgems_mesh_set_get_prism_vertices(meshgems_mesh_t *m,
							meshgems_mesh_get_prism_vertices_t func,
							void *user_data);
/**
 * \ingroup mesh_prism
 * Gets the tag of a mesh prism (generic method called by this \ref meshgems_mesh_t object user).
 * @param[in]	m	: the mesh
 * @param[in]	iprism	: index of prism from 1 to nbprism
 * @param[out]	tagprism	: integer tag of the prism
 * @return a status code
 */
MESHGEMS_METHOD meshgems_status_t meshgems_mesh_get_prism_tag(meshgems_mesh_t *m, meshgems_integer iprism, meshgems_integer *tagprism);

/**
 * \ingroup mesh_prism
 * Implementation declaration of how the tag of a mesh prism is obtained.
 * @param[in]	iprism	: index of prism from 1 to nbprism
 * @param[out]	tagprism	: integer tag of the prism
 * @param	user_data: a user pointer.
 * @return a status code
 */
typedef meshgems_status_t (*meshgems_mesh_get_prism_tag_t)(meshgems_integer iprism, meshgems_integer *tagprism, void *user_data);

/**
 * \ingroup mesh_prism
 * Sets the implementation to use when \ref meshgems_mesh_get_prism_tag is called by this \ref meshgems_mesh_t object user.
 * @param[in]	m	: the mesh
 * @param[in]	func	: the function implementation
 * @param	user_data: a user pointer to pass to the implementation.
 * @return a status code
 */
MESHGEMS_METHOD meshgems_status_t meshgems_mesh_set_get_prism_tag(meshgems_mesh_t *m,
							meshgems_mesh_get_prism_tag_t func,
							void *user_data);
/**
 * \ingroup mesh_prism
 * Gets the required property of a mesh prism (generic method called by this \ref meshgems_mesh_t object user).
 * @param[in]	m	: the mesh
 * @param[in]	iprism	: index of prism from 1 to nbprism
 * @param[out]	reqprism	: integer required property of the prism
 * @return a status code
 */
MESHGEMS_METHOD meshgems_status_t meshgems_mesh_get_prism_required_property(meshgems_mesh_t *m, meshgems_integer iprism, meshgems_integer *reqprism);

/**
 * \ingroup mesh_prism
 * Implementation declaration of how the required property of a mesh prism is obtained.
 * @param[in]	iprism	: index of prism from 1 to nbprism
 * @param[out]	reqprism	: integer required property of the prism
 * @param	user_data: a user pointer.
 * @return a status code
 */
typedef meshgems_status_t (*meshgems_mesh_get_prism_required_property_t)(meshgems_integer iprism, meshgems_integer *reqprism, void *user_data);

/**
 * \ingroup mesh_prism
 * Sets the implementation to use when \ref meshgems_mesh_get_prism_required_property is called by this \ref meshgems_mesh_t object user.
 * @param[in]	m	: the mesh
 * @param[in]	func	: the function implementation
 * @param	user_data: a user pointer to pass to the implementation.
 * @return a status code
 */
MESHGEMS_METHOD meshgems_status_t meshgems_mesh_set_get_prism_required_property(meshgems_mesh_t *m,
							meshgems_mesh_get_prism_required_property_t func,
							void *user_data);
/**
 * \ingroup mesh_prism
 * Gets the extra vertices of a mesh prism (generic method called by this \ref meshgems_mesh_t object user).
 * @param[in]	m	: the mesh
 * @param[in]	iprism	: index of prism from 1 to nbprism
 * @param[out]	typeprism	: integer containing the type of the prism
 * @param[out]	evprism	: integer[] array containing the extra vertices of the prism
 * @return a status code
 */
MESHGEMS_METHOD meshgems_status_t meshgems_mesh_get_prism_extra_vertices(meshgems_mesh_t *m, meshgems_integer iprism,
                                                    meshgems_integer *typeprism, meshgems_integer *evprism);

/**
 * \ingroup mesh_prism
 * Implementation declaration of how the extra vertices of a mesh prism are obtained.
 * @param[in]	iprism	: index of extra vertices from 1 to nbprism
 * @param[out]	typeprism	: integer containing the type of the prism
 * @param[out]	evprism	: integer[] array containing the extra vertices of the prism
 * @param	user_data: a user pointer.
 * @return a status code
 */
typedef meshgems_status_t (*meshgems_mesh_get_prism_extra_vertices_t)(meshgems_integer iprism, meshgems_integer *typeprism, meshgems_integer *evprism, void *user_data);

/**
 * \ingroup mesh_prism
 * Sets the implementation to use when \ref meshgems_mesh_get_prism_extra_vertices is called by this \ref meshgems_mesh_t object user.
 * @param[in]	m	: the mesh
 * @param[in]	func	: the function implementation
 * @param	user_data: a user pointer to pass to the implementation.
 * @return a status code
 */
MESHGEMS_METHOD meshgems_status_t meshgems_mesh_set_get_prism_extra_vertices(meshgems_mesh_t *m,
							meshgems_mesh_get_prism_extra_vertices_t func,
							void *user_data);
/* **************************************** */


/* **************************************** */








/* **************************************** */
/**
 * \ingroup mesh_vertex
 * Sets the number of vertices in the mesh (generic method called by this \ref meshgems_mesh_t object user).
 * @param[in]	m	: the mesh
 * @param[in]	nbvtx	: the number of vertices
 * @return a status code
 */
MESHGEMS_METHOD meshgems_status_t meshgems_mesh_set_vertex_count(meshgems_mesh_t *m, meshgems_integer nbvtx);

/**
 * \ingroup mesh_vertex
 * Implementation declaration of how the number of vertices in the mesh is obtained.
 * @param[in]	nbvtx	: the number of vertices
 * @param     user_data	: a user pointer.
 * @return a status code
 */
typedef meshgems_status_t (*meshgems_mesh_set_vertex_count_t)(meshgems_integer nbvtx, void *user_data);

/**
 * \ingroup mesh_vertex
 * Sets the implementation to use when \ref meshgems_mesh_set_vertex_count is called by this \ref meshgems_mesh_t object user.
 * @param[in]	m	: the mesh
 * @param[in]	func	: the function implementation
 * @param     user_data	: a user pointer to pass to the implementation.
 * @return a status code
 */
MESHGEMS_METHOD meshgems_status_t meshgems_mesh_set_set_vertex_count(meshgems_mesh_t *m,
						        meshgems_mesh_set_vertex_count_t func,
						        void *user_data);
/**
 * \ingroup mesh_vertex
 * Sets the coordinates of a mesh vertex (generic method called by this \ref meshgems_mesh_t object user).
 * @param[in]	m	: the mesh
 * @param[in]	ivtx	: index of vertex from 1 to nbvtx
 * @param[in]	xyz	: real[3] array containing the coordinates of the vertex
 * @return a status code
 */
MESHGEMS_METHOD meshgems_status_t meshgems_mesh_set_vertex_coordinates(meshgems_mesh_t *m, meshgems_integer ivtx, meshgems_real *xyz);

/**
 * \ingroup mesh_vertex
 * Implementation declaration of how the coordinates of a mesh vertex are set.
 * @param[in]	ivtx	: index of vertex from 1 to nbvtx
 * @param[in]	xyz	: real[3] array containing the coordinates of the vertex
 * @param	user_data: a user pointer.
 * @return a status code
 */
typedef meshgems_status_t (*meshgems_mesh_set_vertex_coordinates_t)(meshgems_integer ivtx, meshgems_real *xyz, void *user_data);

/**
 * \ingroup mesh_vertex
 * Sets the implementation to use when \ref meshgems_mesh_set_vertex_coordinates is called by this \ref meshgems_mesh_t object user.
 * @param[in]	m	: the mesh
 * @param[in]	func	: the function implementation
 * @param	user_data: a user pointer to pass to the implementation.
 * @return a status code
 */
MESHGEMS_METHOD meshgems_status_t meshgems_mesh_set_set_vertex_coordinates(meshgems_mesh_t *m,
							meshgems_mesh_set_vertex_coordinates_t func,
							void *user_data);
/**
 * \ingroup mesh_vertex
 * Sets the normal coordinates at a mesh vertex (generic method called by this \ref meshgems_mesh_t object user).
 * @param[in]	m	: the mesh
 * @param[in]	ivtx	: index of vertex from 1 to nbvtx
 * @param[in]	nxyz	: real[3] array containing the normal coordinates at the vertex
 * @return a status code
 */
MESHGEMS_METHOD meshgems_status_t meshgems_mesh_set_vertex_normal_coordinates(meshgems_mesh_t *m, meshgems_integer ivtx, meshgems_real *nxyz);

/**
 * \ingroup mesh_vertex
 * Implementation declaration of how the normal coordinates at a mesh vertex are set.
 * @param[in]	ivtx	: index of vertex from 1 to nbvtx
 * @param[in]	nxyz	: real[3] array containing the normal coordinates of the vertex
 * @param	user_data: a user pointer.
 * @return a status code
 */
typedef meshgems_status_t (*meshgems_mesh_set_vertex_normal_coordinates_t)(meshgems_integer ivtx, meshgems_real *nxyz, void *user_data);

/**
 * \ingroup mesh_vertex
 * Sets the implementation to use when \ref meshgems_mesh_set_vertex_normal_coordinates is called by this \ref meshgems_mesh_t object user.
 * @param[in]	m	: the mesh
 * @param[in]	func	: the function implementation
 * @param	user_data: a user pointer to pass to the implementation.
 * @return a status code
 */
MESHGEMS_METHOD meshgems_status_t meshgems_mesh_set_set_vertex_normal_coordinates(meshgems_mesh_t *m,
							meshgems_mesh_set_vertex_normal_coordinates_t func,
							void *user_data);
/**
 * \ingroup mesh_vertex
 * Sets the tag of a mesh vertex (generic method called by this \ref meshgems_mesh_t object user).
 * @param[in]	m	: the mesh
 * @param[in]	ivtx	: index of vertex from 1 to nbvtx
 * @param[in]	tagvtx	: integer tag of the vertex
 * @return a status code
 */
MESHGEMS_METHOD meshgems_status_t meshgems_mesh_set_vertex_tag(meshgems_mesh_t *m, meshgems_integer ivtx, meshgems_integer *tagvtx);

/**
 * \ingroup mesh_vertex
 * Implementation declaration of how the tag of a mesh vertex is set.
 * @param[in]	ivtx	: index of vertex from 1 to nbvtx
 * @param[in]	tagvtx	: integer tag of the vertex
 * @param	user_data: a user pointer.
 * @return a status code
 */
typedef meshgems_status_t (*meshgems_mesh_set_vertex_tag_t)(meshgems_integer ivtx, meshgems_integer *tagvtx, void *user_data);

/**
 * \ingroup mesh_vertex
 * Sets the implementation to use when \ref meshgems_mesh_set_vertex_tag is called by this \ref meshgems_mesh_t object user.
 * @param[in]	m	: the mesh
 * @param[in]	func	: the function implementation
 * @param	user_data: a user pointer to pass to the implementation.
 * @return a status code
 */
MESHGEMS_METHOD meshgems_status_t meshgems_mesh_set_set_vertex_tag(meshgems_mesh_t *m,
							meshgems_mesh_set_vertex_tag_t func,
							void *user_data);
/**
 * \ingroup mesh_vertex
 * Sets the periodic id of a mesh vertex (generic method called by this \ref meshgems_mesh_t object user).
 * @param[in]	m	: the mesh
 * @param[in]	ivtx	: index of vertex from 1 to nbvtx
 * @param[in]	periodic_id	: integer periodic id of the vertex
 * @return a status code
 */
MESHGEMS_METHOD meshgems_status_t meshgems_mesh_set_vertex_periodic_id(meshgems_mesh_t *m, meshgems_integer ivtx, meshgems_integer *periodic_id);

/**
 * \ingroup mesh_vertex
 * Implementation declaration of how the periodic id of a mesh vertex is set.
 * @param[in]	ivtx	: index of vertex from 1 to nbvtx
 * @param[in]	periodic_id	: integer periodic id of the vertex
 * @param	user_data: a user pointer.
 * @return a status code
 */
typedef meshgems_status_t (*meshgems_mesh_set_vertex_periodic_id_t)(meshgems_integer ivtx, meshgems_integer *periodic_id, void *user_data);

/**
 * \ingroup mesh_vertex
 * Sets the implementation to use when \ref meshgems_mesh_set_vertex_periodic_id is called by this \ref meshgems_mesh_t object user.
 * @param[in]	m	: the mesh
 * @param[in]	func	: the function implementation
 * @param	user_data: a user pointer to pass to the implementation.
 * @return a status code
 */
MESHGEMS_METHOD meshgems_status_t meshgems_mesh_set_set_vertex_periodic_id(meshgems_mesh_t *m,
							meshgems_mesh_set_vertex_periodic_id_t func,
							void *user_data);
/**
 * \ingroup mesh_vertex
 * Sets the required property of a mesh vertex (generic method called by this \ref meshgems_mesh_t object user).
 * @param[in]	m	: the mesh
 * @param[in]	ivtx	: index of vertex from 1 to nbvtx
 * @param[in]	reqtx	: integer required property of the vertex
 * @return a status code
 */
MESHGEMS_METHOD meshgems_status_t meshgems_mesh_set_vertex_required_property(meshgems_mesh_t *m, meshgems_integer ivtx, meshgems_integer *reqtx);

/**
 * \ingroup mesh_vertex
 * Implementation declaration of how the required property of a mesh vertex is set.
 * @param[in]	ivtx	: index of vertex from 1 to nbvtx
 * @param[in]	reqtx	: integer required property of the vertex
 * @param	user_data: a user pointer.
 * @return a status code
 */
typedef meshgems_status_t (*meshgems_mesh_set_vertex_required_property_t)(meshgems_integer ivtx, meshgems_integer *reqtx, void *user_data);

/**
 * \ingroup mesh_vertex
 * Sets the implementation to use when \ref meshgems_mesh_set_vertex_required_property is called by this \ref meshgems_mesh_t object user.
 * @param[in]	m	: the mesh
 * @param[in]	func	: the function implementation
 * @param	user_data: a user pointer to pass to the implementation.
 * @return a status code
 */
MESHGEMS_METHOD meshgems_status_t meshgems_mesh_set_set_vertex_required_property(meshgems_mesh_t *m,
							meshgems_mesh_set_vertex_required_property_t func,
							void *user_data);
/**
 * \ingroup mesh_vertex
 * Sets the corner property of a mesh vertex (generic method called by this \ref meshgems_mesh_t object user).
 * @param[in]	m	: the mesh
 * @param[in]	ivtx	: index of vertex from 1 to nbvtx
 * @param[in]	cornvtx	: integer corner property of the vertex
 * @return a status code
 */
MESHGEMS_METHOD meshgems_status_t meshgems_mesh_set_vertex_corner_property(meshgems_mesh_t *m, meshgems_integer ivtx, meshgems_integer *cornvtx);

/**
 * \ingroup mesh_vertex
 * Implementation declaration of how the corner property of a mesh vertex is set.
 * @param[in]	ivtx	: index of vertex from 1 to nbvtx
 * @param[in]	cornvtx	: integer corner property of the vertex
 * @param	user_data: a user pointer.
 * @return a status code
 */
typedef meshgems_status_t (*meshgems_mesh_set_vertex_corner_property_t)(meshgems_integer ivtx, meshgems_integer *cornvtx, void *user_data);

/**
 * \ingroup mesh_vertex
 * Sets the implementation to use when \ref meshgems_mesh_set_vertex_corner_property is called by this \ref meshgems_mesh_t object user.
 * @param[in]	m	: the mesh
 * @param[in]	func	: the function implementation
 * @param	user_data: a user pointer to pass to the implementation.
 * @return a status code
 */
MESHGEMS_METHOD meshgems_status_t meshgems_mesh_set_set_vertex_corner_property(meshgems_mesh_t *m,
							meshgems_mesh_set_vertex_corner_property_t func,
							void *user_data);
/* **************************************** */
/**
 * \ingroup mesh_edge
 * Sets the number of edges in the mesh (generic method called by this \ref meshgems_mesh_t object user).
 * @param[in]	m	: the mesh
 * @param[in]	nbedg	: the number of edges
 * @return a status code
 */
MESHGEMS_METHOD meshgems_status_t meshgems_mesh_set_edge_count(meshgems_mesh_t *m, meshgems_integer nbedg);

/**
 * \ingroup mesh_edge
 * Implementation declaration of how the number of edges in the mesh is obtained.
 * @param[in]	nbedg	: the number of edges
 * @param     user_data	: a user pointer.
 * @return a status code
 */
typedef meshgems_status_t (*meshgems_mesh_set_edge_count_t)(meshgems_integer nbedg, void *user_data);

/**
 * \ingroup mesh_edge
 * Sets the implementation to use when \ref meshgems_mesh_set_edge_count is called by this \ref meshgems_mesh_t object user.
 * @param[in]	m	: the mesh
 * @param[in]	func	: the function implementation
 * @param     user_data	: a user pointer to pass to the implementation.
 * @return a status code
 */
MESHGEMS_METHOD meshgems_status_t meshgems_mesh_set_set_edge_count(meshgems_mesh_t *m,
						        meshgems_mesh_set_edge_count_t func,
						        void *user_data);
/**
 * \ingroup mesh_edge
 * Sets the vertices of a mesh edge (generic method called by this \ref meshgems_mesh_t object user).
 * @param[in]	m	: the mesh
 * @param[in]	iedg	: index of edge from 1 to nbedg
 * @param[in]	vedg	: integer[2] array containing the vertices of the edge
 * @return a status code
 */
MESHGEMS_METHOD meshgems_status_t meshgems_mesh_set_edge_vertices(meshgems_mesh_t *m, meshgems_integer iedg, meshgems_integer *vedg);

/**
 * \ingroup mesh_edge
 * Implementation declaration of how the vertices of a mesh edge are set.
 * @param[in]	iedg	: index of edge from 1 to nbedg
 * @param[in]	vedg	: integer[2] array containing the vertices of the edge
 * @param	user_data: a user pointer.
 * @return a status code
 */
typedef meshgems_status_t (*meshgems_mesh_set_edge_vertices_t)(meshgems_integer iedg, meshgems_integer *vedg, void *user_data);

/**
 * \ingroup mesh_edge
 * Sets the implementation to use when \ref meshgems_mesh_set_edge_vertices is called by this \ref meshgems_mesh_t object user.
 * @param[in]	m	: the mesh
 * @param[in]	func	: the function implementation
 * @param	user_data: a user pointer to pass to the implementation.
 * @return a status code
 */
MESHGEMS_METHOD meshgems_status_t meshgems_mesh_set_set_edge_vertices(meshgems_mesh_t *m,
							meshgems_mesh_set_edge_vertices_t func,
							void *user_data);
/**
 * \ingroup mesh_edge
 * Sets the tag of a mesh edge (generic method called by this \ref meshgems_mesh_t object user).
 * @param[in]	m	: the mesh
 * @param[in]	iedg	: index of edge from 1 to nbedg
 * @param[in]	tagedg	: integer tag of the edge
 * @return a status code
 */
MESHGEMS_METHOD meshgems_status_t meshgems_mesh_set_edge_tag(meshgems_mesh_t *m, meshgems_integer iedg, meshgems_integer *tagedg);

/**
 * \ingroup mesh_edge
 * Implementation declaration of how the tag of a mesh edge is set.
 * @param[in]	iedg	: index of edge from 1 to nbedg
 * @param[in]	tagedg	: integer tag of the edge
 * @param	user_data: a user pointer.
 * @return a status code
 */
typedef meshgems_status_t (*meshgems_mesh_set_edge_tag_t)(meshgems_integer iedg, meshgems_integer *tagedg, void *user_data);

/**
 * \ingroup mesh_edge
 * Sets the implementation to use when \ref meshgems_mesh_set_edge_tag is called by this \ref meshgems_mesh_t object user.
 * @param[in]	m	: the mesh
 * @param[in]	func	: the function implementation
 * @param	user_data: a user pointer to pass to the implementation.
 * @return a status code
 */
MESHGEMS_METHOD meshgems_status_t meshgems_mesh_set_set_edge_tag(meshgems_mesh_t *m,
							meshgems_mesh_set_edge_tag_t func,
							void *user_data);
/**
 * \ingroup mesh_edge
 * Sets the tangent coordinates at a mesh edge vertex (generic method called by this \ref meshgems_mesh_t object user).
 * @param[in]	m	: the mesh
 * @param[in]	iedg	: index of edge from 1 to nbedg
 * @param[in]	iedgloc	: index of edge vertex from 1 to 2
 * @param[in]	txyz	: real[3] array containing the tangent coordinates at the edge vertex
 * @return a status code
 */
MESHGEMS_METHOD meshgems_status_t meshgems_mesh_set_edge_vertex_tangent_coordinates(meshgems_mesh_t *m, meshgems_integer iedg, meshgems_integer iedgloc, meshgems_real *txyz);

/**
 * \ingroup mesh_edge
 * Implementation declaration of how the tangent coordinates at a mesh edge vertex are set.
 * @param[in]	iedg	: index of edge from 1 to nbedg
 * @param[in]	iedgloc	: index of edge vertex from 1 to 2
 * @param[in]	txyz	: real[3] array containing the tangent coordinates of the edge vertex
 * @param	user_data: a user pointer.
 * @return a status code
 */
typedef meshgems_status_t (*meshgems_mesh_set_edge_vertex_tangent_coordinates_t)(meshgems_integer iedg, meshgems_integer iedgloc, meshgems_real *txyz, void *user_data);

/**
 * \ingroup mesh_edge
 * Sets the implementation to use when \ref meshgems_mesh_set_edge_vertex_tangent_coordinates is called by this \ref meshgems_mesh_t object user.
 * @param[in]	m	: the mesh
 * @param[in]	func	: the function implementation
 * @param	user_data: a user pointer to pass to the implementation.
 * @return a status code
 */
MESHGEMS_METHOD meshgems_status_t meshgems_mesh_set_set_edge_vertex_tangent_coordinates(meshgems_mesh_t *m,
							meshgems_mesh_set_edge_vertex_tangent_coordinates_t func,
							void *user_data);
/**
 * \ingroup mesh_edge
 * Sets the periodic id of a mesh edge (generic method called by this \ref meshgems_mesh_t object user).
 * @param[in]	m	: the mesh
 * @param[in]	iedg	: index of edge from 1 to nbedg
 * @param[in]	periodic_id	: integer periodic id of the edge
 * @return a status code
 */
MESHGEMS_METHOD meshgems_status_t meshgems_mesh_set_edge_periodic_id(meshgems_mesh_t *m, meshgems_integer iedg, meshgems_integer *periodic_id);

/**
 * \ingroup mesh_edge
 * Implementation declaration of how the periodic id of a mesh edge is set.
 * @param[in]	iedg	: index of edge from 1 to nbedg
 * @param[in]	periodic_id	: integer periodic id of the edge
 * @param	user_data: a user pointer.
 * @return a status code
 */
typedef meshgems_status_t (*meshgems_mesh_set_edge_periodic_id_t)(meshgems_integer iedg, meshgems_integer *periodic_id, void *user_data);

/**
 * \ingroup mesh_edge
 * Sets the implementation to use when \ref meshgems_mesh_set_edge_periodic_id is called by this \ref meshgems_mesh_t object user.
 * @param[in]	m	: the mesh
 * @param[in]	func	: the function implementation
 * @param	user_data: a user pointer to pass to the implementation.
 * @return a status code
 */
MESHGEMS_METHOD meshgems_status_t meshgems_mesh_set_set_edge_periodic_id(meshgems_mesh_t *m,
							meshgems_mesh_set_edge_periodic_id_t func,
							void *user_data);
/**
 * \ingroup mesh_edge
 * Sets the required property of a mesh edge (generic method called by this \ref meshgems_mesh_t object user).
 * @param[in]	m	: the mesh
 * @param[in]	iedg	: index of edge from 1 to nbedg
 * @param[in]	reqedg	: integer required property of the edge
 * @return a status code
 */
MESHGEMS_METHOD meshgems_status_t meshgems_mesh_set_edge_required_property(meshgems_mesh_t *m, meshgems_integer iedg, meshgems_integer *reqedg);

/**
 * \ingroup mesh_edge
 * Implementation declaration of how the required property of a mesh edge is set.
 * @param[in]	iedg	: index of edge from 1 to nbedg
 * @param[in]	reqedg	: integer required property of the edge
 * @param	user_data: a user pointer.
 * @return a status code
 */
typedef meshgems_status_t (*meshgems_mesh_set_edge_required_property_t)(meshgems_integer iedg, meshgems_integer *reqedg, void *user_data);

/**
 * \ingroup mesh_edge
 * Sets the implementation to use when \ref meshgems_mesh_set_edge_required_property is called by this \ref meshgems_mesh_t object user.
 * @param[in]	m	: the mesh
 * @param[in]	func	: the function implementation
 * @param	user_data: a user pointer to pass to the implementation.
 * @return a status code
 */
MESHGEMS_METHOD meshgems_status_t meshgems_mesh_set_set_edge_required_property(meshgems_mesh_t *m,
							meshgems_mesh_set_edge_required_property_t func,
							void *user_data);
/**
 * \ingroup mesh_edge
 * Sets the ridge property of a mesh edge (generic method called by this \ref meshgems_mesh_t object user).
 * @param[in]	m	: the mesh
 * @param[in]	iedg	: index of edge from 1 to nbedg
 * @param[in]	ridgedg	: integer ridge property of the edge
 * @return a status code
 */
MESHGEMS_METHOD meshgems_status_t meshgems_mesh_set_edge_ridge_property(meshgems_mesh_t *m, meshgems_integer iedg, meshgems_integer *ridgedg);

/**
 * \ingroup mesh_edge
 * Implementation declaration of how the ridge property of a mesh edge is set.
 * @param[in]	iedg	: index of edge from 1 to nbedg
 * @param[in]	ridgedg	: integer ridge property of the edge
 * @param	user_data: a user pointer.
 * @return a status code
 */
typedef meshgems_status_t (*meshgems_mesh_set_edge_ridge_property_t)(meshgems_integer iedg, meshgems_integer *ridgedg, void *user_data);

/**
 * \ingroup mesh_edge
 * Sets the implementation to use when \ref meshgems_mesh_set_edge_ridge_property is called by this \ref meshgems_mesh_t object user.
 * @param[in]	m	: the mesh
 * @param[in]	func	: the function implementation
 * @param	user_data: a user pointer to pass to the implementation.
 * @return a status code
 */
MESHGEMS_METHOD meshgems_status_t meshgems_mesh_set_set_edge_ridge_property(meshgems_mesh_t *m,
							meshgems_mesh_set_edge_ridge_property_t func,
							void *user_data);
/**
 * \ingroup mesh_edge
 * Sets the extra vertices of a mesh edge (generic method called by this \ref meshgems_mesh_t object user).
 * @param[in]	m	: the mesh
 * @param[in]	iedg	: index of edge from 1 to nbedg
 * @param[in]	typeedg	: integer containing the type of the edge
 * @param[in]	evedg	: integer[] array containing the extra vertices of the edge
 * @return a status code
 */
MESHGEMS_METHOD meshgems_status_t meshgems_mesh_set_edge_extra_vertices(meshgems_mesh_t *m, meshgems_integer iedg,
                                                    meshgems_integer typeedg, meshgems_integer *evedg);

/**
 * \ingroup mesh_edge
 * Implementation declaration of how the extra vertices of a mesh edge are set.
 * @param[in]	iedg	: index of extra vertices from 1 to nbedg
 * @param[in]	typeedg	: integer containing the type of the edge
 * @param[in]	evedg	: integer[] array containing the extra vertices of the edge
 * @param	user_data: a user pointer.
 * @return a status code
 */
typedef meshgems_status_t (*meshgems_mesh_set_edge_extra_vertices_t)(meshgems_integer iedg, meshgems_integer typeedg, meshgems_integer *evedg, void *user_data);

/**
 * \ingroup mesh_edge
 * Sets the implementation to use when \ref meshgems_mesh_set_edge_extra_vertices is called by this \ref meshgems_mesh_t object user.
 * @param[in]	m	: the mesh
 * @param[in]	func	: the function implementation
 * @param	user_data: a user pointer to pass to the implementation.
 * @return a status code
 */
MESHGEMS_METHOD meshgems_status_t meshgems_mesh_set_set_edge_extra_vertices(meshgems_mesh_t *m,
							meshgems_mesh_set_edge_extra_vertices_t func,
							void *user_data);
/* **************************************** */
/**
 * \ingroup mesh_triangle
 * Sets the number of triangles in the mesh (generic method called by this \ref meshgems_mesh_t object user).
 * @param[in]	m	: the mesh
 * @param[in]	nbtri	: the number of triangles
 * @return a status code
 */
MESHGEMS_METHOD meshgems_status_t meshgems_mesh_set_triangle_count(meshgems_mesh_t *m, meshgems_integer nbtri);

/**
 * \ingroup mesh_triangle
 * Implementation declaration of how the number of triangles in the mesh is obtained.
 * @param[in]	nbtri	: the number of triangles
 * @param     user_data	: a user pointer.
 * @return a status code
 */
typedef meshgems_status_t (*meshgems_mesh_set_triangle_count_t)(meshgems_integer nbtri, void *user_data);

/**
 * \ingroup mesh_triangle
 * Sets the implementation to use when \ref meshgems_mesh_set_triangle_count is called by this \ref meshgems_mesh_t object user.
 * @param[in]	m	: the mesh
 * @param[in]	func	: the function implementation
 * @param     user_data	: a user pointer to pass to the implementation.
 * @return a status code
 */
MESHGEMS_METHOD meshgems_status_t meshgems_mesh_set_set_triangle_count(meshgems_mesh_t *m,
						        meshgems_mesh_set_triangle_count_t func,
						        void *user_data);
/**
 * \ingroup mesh_triangle
 * Sets the vertices of a mesh triangle (generic method called by this \ref meshgems_mesh_t object user).
 * @param[in]	m	: the mesh
 * @param[in]	itri	: index of triangle from 1 to nbtri
 * @param[in]	vtri	: integer[3] array containing the vertices of the triangle
 * @return a status code
 */
MESHGEMS_METHOD meshgems_status_t meshgems_mesh_set_triangle_vertices(meshgems_mesh_t *m, meshgems_integer itri, meshgems_integer *vtri);

/**
 * \ingroup mesh_triangle
 * Implementation declaration of how the vertices of a mesh triangle are set.
 * @param[in]	itri	: index of triangle from 1 to nbtri
 * @param[in]	vtri	: integer[3] array containing the vertices of the triangle
 * @param	user_data: a user pointer.
 * @return a status code
 */
typedef meshgems_status_t (*meshgems_mesh_set_triangle_vertices_t)(meshgems_integer itri, meshgems_integer *vtri, void *user_data);

/**
 * \ingroup mesh_triangle
 * Sets the implementation to use when \ref meshgems_mesh_set_triangle_vertices is called by this \ref meshgems_mesh_t object user.
 * @param[in]	m	: the mesh
 * @param[in]	func	: the function implementation
 * @param	user_data: a user pointer to pass to the implementation.
 * @return a status code
 */
MESHGEMS_METHOD meshgems_status_t meshgems_mesh_set_set_triangle_vertices(meshgems_mesh_t *m,
							meshgems_mesh_set_triangle_vertices_t func,
							void *user_data);
/**
 * \ingroup mesh_triangle
 * Sets the tag of a mesh triangle (generic method called by this \ref meshgems_mesh_t object user).
 * @param[in]	m	: the mesh
 * @param[in]	itri	: index of triangle from 1 to nbtri
 * @param[in]	tagtri	: integer tag of the triangle
 * @return a status code
 */
MESHGEMS_METHOD meshgems_status_t meshgems_mesh_set_triangle_tag(meshgems_mesh_t *m, meshgems_integer itri, meshgems_integer *tagtri);

/**
 * \ingroup mesh_triangle
 * Implementation declaration of how the tag of a mesh triangle is set.
 * @param[in]	itri	: index of triangle from 1 to nbtri
 * @param[in]	tagtri	: integer tag of the triangle
 * @param	user_data: a user pointer.
 * @return a status code
 */
typedef meshgems_status_t (*meshgems_mesh_set_triangle_tag_t)(meshgems_integer itri, meshgems_integer *tagtri, void *user_data);

/**
 * \ingroup mesh_triangle
 * Sets the implementation to use when \ref meshgems_mesh_set_triangle_tag is called by this \ref meshgems_mesh_t object user.
 * @param[in]	m	: the mesh
 * @param[in]	func	: the function implementation
 * @param	user_data: a user pointer to pass to the implementation.
 * @return a status code
 */
MESHGEMS_METHOD meshgems_status_t meshgems_mesh_set_set_triangle_tag(meshgems_mesh_t *m,
							meshgems_mesh_set_triangle_tag_t func,
							void *user_data);
/* xxxAPPLY_TO_MESH_ARRAY(normal at triangle vertices,real,triangle_normal_coordinates_old,nxyz,tri,3*3,triangle); */
/**
 * \ingroup mesh_triangle
 * Sets the normal coordinates at a mesh triangle vertex (generic method called by this \ref meshgems_mesh_t object user).
 * @param[in]	m	: the mesh
 * @param[in]	itri	: index of triangle from 1 to nbtri
 * @param[in]	itriloc	: index of triangle vertex from 1 to 3
 * @param[in]	nxyz	: real[3] array containing the normal coordinates at the triangle vertex
 * @return a status code
 */
MESHGEMS_METHOD meshgems_status_t meshgems_mesh_set_triangle_vertex_normal_coordinates(meshgems_mesh_t *m, meshgems_integer itri, meshgems_integer itriloc, meshgems_real *nxyz);

/**
 * \ingroup mesh_triangle
 * Implementation declaration of how the normal coordinates at a mesh triangle vertex are set.
 * @param[in]	itri	: index of triangle from 1 to nbtri
 * @param[in]	itriloc	: index of triangle vertex from 1 to 3
 * @param[in]	nxyz	: real[3] array containing the normal coordinates of the triangle vertex
 * @param	user_data: a user pointer.
 * @return a status code
 */
typedef meshgems_status_t (*meshgems_mesh_set_triangle_vertex_normal_coordinates_t)(meshgems_integer itri, meshgems_integer itriloc, meshgems_real *nxyz, void *user_data);

/**
 * \ingroup mesh_triangle
 * Sets the implementation to use when \ref meshgems_mesh_set_triangle_vertex_normal_coordinates is called by this \ref meshgems_mesh_t object user.
 * @param[in]	m	: the mesh
 * @param[in]	func	: the function implementation
 * @param	user_data: a user pointer to pass to the implementation.
 * @return a status code
 */
MESHGEMS_METHOD meshgems_status_t meshgems_mesh_set_set_triangle_vertex_normal_coordinates(meshgems_mesh_t *m,
							meshgems_mesh_set_triangle_vertex_normal_coordinates_t func,
							void *user_data);
/**
 * \ingroup mesh_triangle
 * Sets the periodic id of a mesh triangle (generic method called by this \ref meshgems_mesh_t object user).
 * @param[in]	m	: the mesh
 * @param[in]	itri	: index of triangle from 1 to nbtri
 * @param[in]	periodic_id	: integer periodic id of the triangle
 * @return a status code
 */
MESHGEMS_METHOD meshgems_status_t meshgems_mesh_set_triangle_periodic_id(meshgems_mesh_t *m, meshgems_integer itri, meshgems_integer *periodic_id);

/**
 * \ingroup mesh_triangle
 * Implementation declaration of how the periodic id of a mesh triangle is set.
 * @param[in]	itri	: index of triangle from 1 to nbtri
 * @param[in]	periodic_id	: integer periodic id of the triangle
 * @param	user_data: a user pointer.
 * @return a status code
 */
typedef meshgems_status_t (*meshgems_mesh_set_triangle_periodic_id_t)(meshgems_integer itri, meshgems_integer *periodic_id, void *user_data);

/**
 * \ingroup mesh_triangle
 * Sets the implementation to use when \ref meshgems_mesh_set_triangle_periodic_id is called by this \ref meshgems_mesh_t object user.
 * @param[in]	m	: the mesh
 * @param[in]	func	: the function implementation
 * @param	user_data: a user pointer to pass to the implementation.
 * @return a status code
 */
MESHGEMS_METHOD meshgems_status_t meshgems_mesh_set_set_triangle_periodic_id(meshgems_mesh_t *m,
							meshgems_mesh_set_triangle_periodic_id_t func,
							void *user_data);
/**
 * \ingroup mesh_triangle
 * Sets the required property of a mesh triangle (generic method called by this \ref meshgems_mesh_t object user).
 * @param[in]	m	: the mesh
 * @param[in]	itri	: index of triangle from 1 to nbtri
 * @param[in]	reqtri	: integer required property of the triangle
 * @return a status code
 */
MESHGEMS_METHOD meshgems_status_t meshgems_mesh_set_triangle_required_property(meshgems_mesh_t *m, meshgems_integer itri, meshgems_integer *reqtri);

/**
 * \ingroup mesh_triangle
 * Implementation declaration of how the required property of a mesh triangle is set.
 * @param[in]	itri	: index of triangle from 1 to nbtri
 * @param[in]	reqtri	: integer required property of the triangle
 * @param	user_data: a user pointer.
 * @return a status code
 */
typedef meshgems_status_t (*meshgems_mesh_set_triangle_required_property_t)(meshgems_integer itri, meshgems_integer *reqtri, void *user_data);

/**
 * \ingroup mesh_triangle
 * Sets the implementation to use when \ref meshgems_mesh_set_triangle_required_property is called by this \ref meshgems_mesh_t object user.
 * @param[in]	m	: the mesh
 * @param[in]	func	: the function implementation
 * @param	user_data: a user pointer to pass to the implementation.
 * @return a status code
 */
MESHGEMS_METHOD meshgems_status_t meshgems_mesh_set_set_triangle_required_property(meshgems_mesh_t *m,
							meshgems_mesh_set_triangle_required_property_t func,
							void *user_data);
/**
 * \ingroup mesh_triangle
 * Sets the extra vertices of a mesh triangle (generic method called by this \ref meshgems_mesh_t object user).
 * @param[in]	m	: the mesh
 * @param[in]	itri	: index of triangle from 1 to nbtri
 * @param[in]	typetri	: integer containing the type of the triangle
 * @param[in]	evtri	: integer[] array containing the extra vertices of the triangle
 * @return a status code
 */
MESHGEMS_METHOD meshgems_status_t meshgems_mesh_set_triangle_extra_vertices(meshgems_mesh_t *m, meshgems_integer itri,
                                                    meshgems_integer typetri, meshgems_integer *evtri);

/**
 * \ingroup mesh_triangle
 * Implementation declaration of how the extra vertices of a mesh triangle are set.
 * @param[in]	itri	: index of extra vertices from 1 to nbtri
 * @param[in]	typetri	: integer containing the type of the triangle
 * @param[in]	evtri	: integer[] array containing the extra vertices of the triangle
 * @param	user_data: a user pointer.
 * @return a status code
 */
typedef meshgems_status_t (*meshgems_mesh_set_triangle_extra_vertices_t)(meshgems_integer itri, meshgems_integer typetri, meshgems_integer *evtri, void *user_data);

/**
 * \ingroup mesh_triangle
 * Sets the implementation to use when \ref meshgems_mesh_set_triangle_extra_vertices is called by this \ref meshgems_mesh_t object user.
 * @param[in]	m	: the mesh
 * @param[in]	func	: the function implementation
 * @param	user_data: a user pointer to pass to the implementation.
 * @return a status code
 */
MESHGEMS_METHOD meshgems_status_t meshgems_mesh_set_set_triangle_extra_vertices(meshgems_mesh_t *m,
							meshgems_mesh_set_triangle_extra_vertices_t func,
							void *user_data);
/* **************************************** */
/**
 * \ingroup mesh_quadrangle
 * Sets the number of quadrilaterals in the mesh (generic method called by this \ref meshgems_mesh_t object user).
 * @param[in]	m	: the mesh
 * @param[in]	nbquad	: the number of quadrilaterals
 * @return a status code
 */
MESHGEMS_METHOD meshgems_status_t meshgems_mesh_set_quadrangle_count(meshgems_mesh_t *m, meshgems_integer nbquad);

/**
 * \ingroup mesh_quadrangle
 * Implementation declaration of how the number of quadrilaterals in the mesh is obtained.
 * @param[in]	nbquad	: the number of quadrilaterals
 * @param     user_data	: a user pointer.
 * @return a status code
 */
typedef meshgems_status_t (*meshgems_mesh_set_quadrangle_count_t)(meshgems_integer nbquad, void *user_data);

/**
 * \ingroup mesh_quadrangle
 * Sets the implementation to use when \ref meshgems_mesh_set_quadrangle_count is called by this \ref meshgems_mesh_t object user.
 * @param[in]	m	: the mesh
 * @param[in]	func	: the function implementation
 * @param     user_data	: a user pointer to pass to the implementation.
 * @return a status code
 */
MESHGEMS_METHOD meshgems_status_t meshgems_mesh_set_set_quadrangle_count(meshgems_mesh_t *m,
						        meshgems_mesh_set_quadrangle_count_t func,
						        void *user_data);
/**
 * \ingroup mesh_quadrangle
 * Sets the vertices of a mesh quadrangle (generic method called by this \ref meshgems_mesh_t object user).
 * @param[in]	m	: the mesh
 * @param[in]	iquad	: index of quadrangle from 1 to nbquad
 * @param[in]	vquad	: integer[4] array containing the vertices of the quadrangle
 * @return a status code
 */
MESHGEMS_METHOD meshgems_status_t meshgems_mesh_set_quadrangle_vertices(meshgems_mesh_t *m, meshgems_integer iquad, meshgems_integer *vquad);

/**
 * \ingroup mesh_quadrangle
 * Implementation declaration of how the vertices of a mesh quadrangle are set.
 * @param[in]	iquad	: index of quadrangle from 1 to nbquad
 * @param[in]	vquad	: integer[4] array containing the vertices of the quadrangle
 * @param	user_data: a user pointer.
 * @return a status code
 */
typedef meshgems_status_t (*meshgems_mesh_set_quadrangle_vertices_t)(meshgems_integer iquad, meshgems_integer *vquad, void *user_data);

/**
 * \ingroup mesh_quadrangle
 * Sets the implementation to use when \ref meshgems_mesh_set_quadrangle_vertices is called by this \ref meshgems_mesh_t object user.
 * @param[in]	m	: the mesh
 * @param[in]	func	: the function implementation
 * @param	user_data: a user pointer to pass to the implementation.
 * @return a status code
 */
MESHGEMS_METHOD meshgems_status_t meshgems_mesh_set_set_quadrangle_vertices(meshgems_mesh_t *m,
							meshgems_mesh_set_quadrangle_vertices_t func,
							void *user_data);
/**
 * \ingroup mesh_quadrangle
 * Sets the tag of a mesh quadrangle (generic method called by this \ref meshgems_mesh_t object user).
 * @param[in]	m	: the mesh
 * @param[in]	iquad	: index of quadrangle from 1 to nbquad
 * @param[in]	tagquad	: integer tag of the quadrangle
 * @return a status code
 */
MESHGEMS_METHOD meshgems_status_t meshgems_mesh_set_quadrangle_tag(meshgems_mesh_t *m, meshgems_integer iquad, meshgems_integer *tagquad);

/**
 * \ingroup mesh_quadrangle
 * Implementation declaration of how the tag of a mesh quadrangle is set.
 * @param[in]	iquad	: index of quadrangle from 1 to nbquad
 * @param[in]	tagquad	: integer tag of the quadrangle
 * @param	user_data: a user pointer.
 * @return a status code
 */
typedef meshgems_status_t (*meshgems_mesh_set_quadrangle_tag_t)(meshgems_integer iquad, meshgems_integer *tagquad, void *user_data);

/**
 * \ingroup mesh_quadrangle
 * Sets the implementation to use when \ref meshgems_mesh_set_quadrangle_tag is called by this \ref meshgems_mesh_t object user.
 * @param[in]	m	: the mesh
 * @param[in]	func	: the function implementation
 * @param	user_data: a user pointer to pass to the implementation.
 * @return a status code
 */
MESHGEMS_METHOD meshgems_status_t meshgems_mesh_set_set_quadrangle_tag(meshgems_mesh_t *m,
							meshgems_mesh_set_quadrangle_tag_t func,
							void *user_data);
/**
 * \ingroup mesh_quadrangle
 * Sets the normal coordinates at a mesh quadrangle vertex (generic method called by this \ref meshgems_mesh_t object user).
 * @param[in]	m	: the mesh
 * @param[in]	iquad	: index of quadrangle from 1 to nbquad
 * @param[in]	iquadloc	: index of quadrangle vertex from 1 to 4
 * @param[in]	nxyz	: real[3] array containing the normal coordinates at the quadrangle vertex
 * @return a status code
 */
MESHGEMS_METHOD meshgems_status_t meshgems_mesh_set_quadrangle_vertex_normal_coordinates(meshgems_mesh_t *m, meshgems_integer iquad, meshgems_integer iquadloc, meshgems_real *nxyz);

/**
 * \ingroup mesh_quadrangle
 * Implementation declaration of how the normal coordinates at a mesh quadrangle vertex are set.
 * @param[in]	iquad	: index of quadrangle from 1 to nbquad
 * @param[in]	iquadloc	: index of quadrangle vertex from 1 to 4
 * @param[in]	nxyz	: real[3] array containing the normal coordinates of the quadrangle vertex
 * @param	user_data: a user pointer.
 * @return a status code
 */
typedef meshgems_status_t (*meshgems_mesh_set_quadrangle_vertex_normal_coordinates_t)(meshgems_integer iquad, meshgems_integer iquadloc, meshgems_real *nxyz, void *user_data);

/**
 * \ingroup mesh_quadrangle
 * Sets the implementation to use when \ref meshgems_mesh_set_quadrangle_vertex_normal_coordinates is called by this \ref meshgems_mesh_t object user.
 * @param[in]	m	: the mesh
 * @param[in]	func	: the function implementation
 * @param	user_data: a user pointer to pass to the implementation.
 * @return a status code
 */
MESHGEMS_METHOD meshgems_status_t meshgems_mesh_set_set_quadrangle_vertex_normal_coordinates(meshgems_mesh_t *m,
							meshgems_mesh_set_quadrangle_vertex_normal_coordinates_t func,
							void *user_data);
/**
 * \ingroup mesh_quadrangle
 * Sets the periodic id of a mesh quadrangle (generic method called by this \ref meshgems_mesh_t object user).
 * @param[in]	m	: the mesh
 * @param[in]	iquad	: index of quadrangle from 1 to nbquad
 * @param[in]	periodic_id	: integer periodic id of the quadrangle
 * @return a status code
 */
MESHGEMS_METHOD meshgems_status_t meshgems_mesh_set_quadrangle_periodic_id(meshgems_mesh_t *m, meshgems_integer iquad, meshgems_integer *periodic_id);

/**
 * \ingroup mesh_quadrangle
 * Implementation declaration of how the periodic id of a mesh quadrangle is set.
 * @param[in]	iquad	: index of quadrangle from 1 to nbquad
 * @param[in]	periodic_id	: integer periodic id of the quadrangle
 * @param	user_data: a user pointer.
 * @return a status code
 */
typedef meshgems_status_t (*meshgems_mesh_set_quadrangle_periodic_id_t)(meshgems_integer iquad, meshgems_integer *periodic_id, void *user_data);

/**
 * \ingroup mesh_quadrangle
 * Sets the implementation to use when \ref meshgems_mesh_set_quadrangle_periodic_id is called by this \ref meshgems_mesh_t object user.
 * @param[in]	m	: the mesh
 * @param[in]	func	: the function implementation
 * @param	user_data: a user pointer to pass to the implementation.
 * @return a status code
 */
MESHGEMS_METHOD meshgems_status_t meshgems_mesh_set_set_quadrangle_periodic_id(meshgems_mesh_t *m,
							meshgems_mesh_set_quadrangle_periodic_id_t func,
							void *user_data);
/**
 * \ingroup mesh_quadrangle
 * Sets the required property of a mesh quadrangle (generic method called by this \ref meshgems_mesh_t object user).
 * @param[in]	m	: the mesh
 * @param[in]	iquad	: index of quadrangle from 1 to nbquad
 * @param[in]	reqquad	: integer required property of the quadrangle
 * @return a status code
 */
MESHGEMS_METHOD meshgems_status_t meshgems_mesh_set_quadrangle_required_property(meshgems_mesh_t *m, meshgems_integer iquad, meshgems_integer *reqquad);

/**
 * \ingroup mesh_quadrangle
 * Implementation declaration of how the required property of a mesh quadrangle is set.
 * @param[in]	iquad	: index of quadrangle from 1 to nbquad
 * @param[in]	reqquad	: integer required property of the quadrangle
 * @param	user_data: a user pointer.
 * @return a status code
 */
typedef meshgems_status_t (*meshgems_mesh_set_quadrangle_required_property_t)(meshgems_integer iquad, meshgems_integer *reqquad, void *user_data);

/**
 * \ingroup mesh_quadrangle
 * Sets the implementation to use when \ref meshgems_mesh_set_quadrangle_required_property is called by this \ref meshgems_mesh_t object user.
 * @param[in]	m	: the mesh
 * @param[in]	func	: the function implementation
 * @param	user_data: a user pointer to pass to the implementation.
 * @return a status code
 */
MESHGEMS_METHOD meshgems_status_t meshgems_mesh_set_set_quadrangle_required_property(meshgems_mesh_t *m,
							meshgems_mesh_set_quadrangle_required_property_t func,
							void *user_data);
/**
 * \ingroup mesh_quadrangle
 * Sets the extra vertices of a mesh quadrangle (generic method called by this \ref meshgems_mesh_t object user).
 * @param[in]	m	: the mesh
 * @param[in]	iquad	: index of quadrangle from 1 to nbquad
 * @param[in]	typequad	: integer containing the type of the quadrangle
 * @param[in]	evquad	: integer[] array containing the extra vertices of the quadrangle
 * @return a status code
 */
MESHGEMS_METHOD meshgems_status_t meshgems_mesh_set_quadrangle_extra_vertices(meshgems_mesh_t *m, meshgems_integer iquad,
                                                    meshgems_integer typequad, meshgems_integer *evquad);

/**
 * \ingroup mesh_quadrangle
 * Implementation declaration of how the extra vertices of a mesh quadrangle are set.
 * @param[in]	iquad	: index of extra vertices from 1 to nbquad
 * @param[in]	typequad	: integer containing the type of the quadrangle
 * @param[in]	evquad	: integer[] array containing the extra vertices of the quadrangle
 * @param	user_data: a user pointer.
 * @return a status code
 */
typedef meshgems_status_t (*meshgems_mesh_set_quadrangle_extra_vertices_t)(meshgems_integer iquad, meshgems_integer typequad, meshgems_integer *evquad, void *user_data);

/**
 * \ingroup mesh_quadrangle
 * Sets the implementation to use when \ref meshgems_mesh_set_quadrangle_extra_vertices is called by this \ref meshgems_mesh_t object user.
 * @param[in]	m	: the mesh
 * @param[in]	func	: the function implementation
 * @param	user_data: a user pointer to pass to the implementation.
 * @return a status code
 */
MESHGEMS_METHOD meshgems_status_t meshgems_mesh_set_set_quadrangle_extra_vertices(meshgems_mesh_t *m,
							meshgems_mesh_set_quadrangle_extra_vertices_t func,
							void *user_data);
/* **************************************** */
/**
 * \ingroup mesh_tetrahedron
 * Sets the number of tetrahedra in the mesh (generic method called by this \ref meshgems_mesh_t object user).
 * @param[in]	m	: the mesh
 * @param[in]	nbtet	: the number of tetrahedra
 * @return a status code
 */
MESHGEMS_METHOD meshgems_status_t meshgems_mesh_set_tetrahedron_count(meshgems_mesh_t *m, meshgems_integer nbtet);

/**
 * \ingroup mesh_tetrahedron
 * Implementation declaration of how the number of tetrahedra in the mesh is obtained.
 * @param[in]	nbtet	: the number of tetrahedra
 * @param     user_data	: a user pointer.
 * @return a status code
 */
typedef meshgems_status_t (*meshgems_mesh_set_tetrahedron_count_t)(meshgems_integer nbtet, void *user_data);

/**
 * \ingroup mesh_tetrahedron
 * Sets the implementation to use when \ref meshgems_mesh_set_tetrahedron_count is called by this \ref meshgems_mesh_t object user.
 * @param[in]	m	: the mesh
 * @param[in]	func	: the function implementation
 * @param     user_data	: a user pointer to pass to the implementation.
 * @return a status code
 */
MESHGEMS_METHOD meshgems_status_t meshgems_mesh_set_set_tetrahedron_count(meshgems_mesh_t *m,
						        meshgems_mesh_set_tetrahedron_count_t func,
						        void *user_data);
/**
 * \ingroup mesh_tetrahedron
 * Sets the vertices of a mesh tetrahedron (generic method called by this \ref meshgems_mesh_t object user).
 * @param[in]	m	: the mesh
 * @param[in]	itet	: index of tetrahedron from 1 to nbtet
 * @param[in]	vtet	: integer[4] array containing the vertices of the tetrahedron
 * @return a status code
 */
MESHGEMS_METHOD meshgems_status_t meshgems_mesh_set_tetrahedron_vertices(meshgems_mesh_t *m, meshgems_integer itet, meshgems_integer *vtet);

/**
 * \ingroup mesh_tetrahedron
 * Implementation declaration of how the vertices of a mesh tetrahedron are set.
 * @param[in]	itet	: index of tetrahedron from 1 to nbtet
 * @param[in]	vtet	: integer[4] array containing the vertices of the tetrahedron
 * @param	user_data: a user pointer.
 * @return a status code
 */
typedef meshgems_status_t (*meshgems_mesh_set_tetrahedron_vertices_t)(meshgems_integer itet, meshgems_integer *vtet, void *user_data);

/**
 * \ingroup mesh_tetrahedron
 * Sets the implementation to use when \ref meshgems_mesh_set_tetrahedron_vertices is called by this \ref meshgems_mesh_t object user.
 * @param[in]	m	: the mesh
 * @param[in]	func	: the function implementation
 * @param	user_data: a user pointer to pass to the implementation.
 * @return a status code
 */
MESHGEMS_METHOD meshgems_status_t meshgems_mesh_set_set_tetrahedron_vertices(meshgems_mesh_t *m,
							meshgems_mesh_set_tetrahedron_vertices_t func,
							void *user_data);
/**
 * \ingroup mesh_tetrahedron
 * Sets the tag of a mesh tetrahedron (generic method called by this \ref meshgems_mesh_t object user).
 * @param[in]	m	: the mesh
 * @param[in]	itet	: index of tetrahedron from 1 to nbtet
 * @param[in]	tagtet	: integer tag of the tetrahedron
 * @return a status code
 */
MESHGEMS_METHOD meshgems_status_t meshgems_mesh_set_tetrahedron_tag(meshgems_mesh_t *m, meshgems_integer itet, meshgems_integer *tagtet);

/**
 * \ingroup mesh_tetrahedron
 * Implementation declaration of how the tag of a mesh tetrahedron is set.
 * @param[in]	itet	: index of tetrahedron from 1 to nbtet
 * @param[in]	tagtet	: integer tag of the tetrahedron
 * @param	user_data: a user pointer.
 * @return a status code
 */
typedef meshgems_status_t (*meshgems_mesh_set_tetrahedron_tag_t)(meshgems_integer itet, meshgems_integer *tagtet, void *user_data);

/**
 * \ingroup mesh_tetrahedron
 * Sets the implementation to use when \ref meshgems_mesh_set_tetrahedron_tag is called by this \ref meshgems_mesh_t object user.
 * @param[in]	m	: the mesh
 * @param[in]	func	: the function implementation
 * @param	user_data: a user pointer to pass to the implementation.
 * @return a status code
 */
MESHGEMS_METHOD meshgems_status_t meshgems_mesh_set_set_tetrahedron_tag(meshgems_mesh_t *m,
							meshgems_mesh_set_tetrahedron_tag_t func,
							void *user_data);
/**
 * \ingroup mesh_tetrahedron
 * Sets the required property of a mesh tetrahedron (generic method called by this \ref meshgems_mesh_t object user).
 * @param[in]	m	: the mesh
 * @param[in]	itet	: index of tetrahedron from 1 to nbtet
 * @param[in]	reqtet	: integer required property of the tetrahedron
 * @return a status code
 */
MESHGEMS_METHOD meshgems_status_t meshgems_mesh_set_tetrahedron_required_property(meshgems_mesh_t *m, meshgems_integer itet, meshgems_integer *reqtet);

/**
 * \ingroup mesh_tetrahedron
 * Implementation declaration of how the required property of a mesh tetrahedron is set.
 * @param[in]	itet	: index of tetrahedron from 1 to nbtet
 * @param[in]	reqtet	: integer required property of the tetrahedron
 * @param	user_data: a user pointer.
 * @return a status code
 */
typedef meshgems_status_t (*meshgems_mesh_set_tetrahedron_required_property_t)(meshgems_integer itet, meshgems_integer *reqtet, void *user_data);

/**
 * \ingroup mesh_tetrahedron
 * Sets the implementation to use when \ref meshgems_mesh_set_tetrahedron_required_property is called by this \ref meshgems_mesh_t object user.
 * @param[in]	m	: the mesh
 * @param[in]	func	: the function implementation
 * @param	user_data: a user pointer to pass to the implementation.
 * @return a status code
 */
MESHGEMS_METHOD meshgems_status_t meshgems_mesh_set_set_tetrahedron_required_property(meshgems_mesh_t *m,
							meshgems_mesh_set_tetrahedron_required_property_t func,
							void *user_data);
/**
 * \ingroup mesh_tetrahedron
 * Sets the extra vertices of a mesh tetrahedron (generic method called by this \ref meshgems_mesh_t object user).
 * @param[in]	m	: the mesh
 * @param[in]	itet	: index of tetrahedron from 1 to nbtet
 * @param[in]	typetet	: integer containing the type of the tetrahedron
 * @param[in]	evtet	: integer[] array containing the extra vertices of the tetrahedron
 * @return a status code
 */
MESHGEMS_METHOD meshgems_status_t meshgems_mesh_set_tetrahedron_extra_vertices(meshgems_mesh_t *m, meshgems_integer itet,
                                                    meshgems_integer typetet, meshgems_integer *evtet);

/**
 * \ingroup mesh_tetrahedron
 * Implementation declaration of how the extra vertices of a mesh tetrahedron are set.
 * @param[in]	itet	: index of extra vertices from 1 to nbtet
 * @param[in]	typetet	: integer containing the type of the tetrahedron
 * @param[in]	evtet	: integer[] array containing the extra vertices of the tetrahedron
 * @param	user_data: a user pointer.
 * @return a status code
 */
typedef meshgems_status_t (*meshgems_mesh_set_tetrahedron_extra_vertices_t)(meshgems_integer itet, meshgems_integer typetet, meshgems_integer *evtet, void *user_data);

/**
 * \ingroup mesh_tetrahedron
 * Sets the implementation to use when \ref meshgems_mesh_set_tetrahedron_extra_vertices is called by this \ref meshgems_mesh_t object user.
 * @param[in]	m	: the mesh
 * @param[in]	func	: the function implementation
 * @param	user_data: a user pointer to pass to the implementation.
 * @return a status code
 */
MESHGEMS_METHOD meshgems_status_t meshgems_mesh_set_set_tetrahedron_extra_vertices(meshgems_mesh_t *m,
							meshgems_mesh_set_tetrahedron_extra_vertices_t func,
							void *user_data);
/* **************************************** */
/**
 * \ingroup mesh_hexahedron
 * Sets the number of hexahedra in the mesh (generic method called by this \ref meshgems_mesh_t object user).
 * @param[in]	m	: the mesh
 * @param[in]	nbhex	: the number of hexahedra
 * @return a status code
 */
MESHGEMS_METHOD meshgems_status_t meshgems_mesh_set_hexahedron_count(meshgems_mesh_t *m, meshgems_integer nbhex);

/**
 * \ingroup mesh_hexahedron
 * Implementation declaration of how the number of hexahedra in the mesh is obtained.
 * @param[in]	nbhex	: the number of hexahedra
 * @param     user_data	: a user pointer.
 * @return a status code
 */
typedef meshgems_status_t (*meshgems_mesh_set_hexahedron_count_t)(meshgems_integer nbhex, void *user_data);

/**
 * \ingroup mesh_hexahedron
 * Sets the implementation to use when \ref meshgems_mesh_set_hexahedron_count is called by this \ref meshgems_mesh_t object user.
 * @param[in]	m	: the mesh
 * @param[in]	func	: the function implementation
 * @param     user_data	: a user pointer to pass to the implementation.
 * @return a status code
 */
MESHGEMS_METHOD meshgems_status_t meshgems_mesh_set_set_hexahedron_count(meshgems_mesh_t *m,
						        meshgems_mesh_set_hexahedron_count_t func,
						        void *user_data);
/**
 * \ingroup mesh_hexahedron
 * Sets the vertices of a mesh hexahedron (generic method called by this \ref meshgems_mesh_t object user).
 * @param[in]	m	: the mesh
 * @param[in]	ihex	: index of hexahedron from 1 to nbhex
 * @param[in]	vhex	: integer[8] array containing the vertices of the hexahedron
 * @return a status code
 */
MESHGEMS_METHOD meshgems_status_t meshgems_mesh_set_hexahedron_vertices(meshgems_mesh_t *m, meshgems_integer ihex, meshgems_integer *vhex);

/**
 * \ingroup mesh_hexahedron
 * Implementation declaration of how the vertices of a mesh hexahedron are set.
 * @param[in]	ihex	: index of hexahedron from 1 to nbhex
 * @param[in]	vhex	: integer[8] array containing the vertices of the hexahedron
 * @param	user_data: a user pointer.
 * @return a status code
 */
typedef meshgems_status_t (*meshgems_mesh_set_hexahedron_vertices_t)(meshgems_integer ihex, meshgems_integer *vhex, void *user_data);

/**
 * \ingroup mesh_hexahedron
 * Sets the implementation to use when \ref meshgems_mesh_set_hexahedron_vertices is called by this \ref meshgems_mesh_t object user.
 * @param[in]	m	: the mesh
 * @param[in]	func	: the function implementation
 * @param	user_data: a user pointer to pass to the implementation.
 * @return a status code
 */
MESHGEMS_METHOD meshgems_status_t meshgems_mesh_set_set_hexahedron_vertices(meshgems_mesh_t *m,
							meshgems_mesh_set_hexahedron_vertices_t func,
							void *user_data);
/**
 * \ingroup mesh_hexahedron
 * Sets the tag of a mesh hexahedron (generic method called by this \ref meshgems_mesh_t object user).
 * @param[in]	m	: the mesh
 * @param[in]	ihex	: index of hexahedron from 1 to nbhex
 * @param[in]	taghex	: integer tag of the hexahedron
 * @return a status code
 */
MESHGEMS_METHOD meshgems_status_t meshgems_mesh_set_hexahedron_tag(meshgems_mesh_t *m, meshgems_integer ihex, meshgems_integer *taghex);

/**
 * \ingroup mesh_hexahedron
 * Implementation declaration of how the tag of a mesh hexahedron is set.
 * @param[in]	ihex	: index of hexahedron from 1 to nbhex
 * @param[in]	taghex	: integer tag of the hexahedron
 * @param	user_data: a user pointer.
 * @return a status code
 */
typedef meshgems_status_t (*meshgems_mesh_set_hexahedron_tag_t)(meshgems_integer ihex, meshgems_integer *taghex, void *user_data);

/**
 * \ingroup mesh_hexahedron
 * Sets the implementation to use when \ref meshgems_mesh_set_hexahedron_tag is called by this \ref meshgems_mesh_t object user.
 * @param[in]	m	: the mesh
 * @param[in]	func	: the function implementation
 * @param	user_data: a user pointer to pass to the implementation.
 * @return a status code
 */
MESHGEMS_METHOD meshgems_status_t meshgems_mesh_set_set_hexahedron_tag(meshgems_mesh_t *m,
							meshgems_mesh_set_hexahedron_tag_t func,
							void *user_data);
/**
 * \ingroup mesh_hexahedron
 * Sets the required property of a mesh hexahedron (generic method called by this \ref meshgems_mesh_t object user).
 * @param[in]	m	: the mesh
 * @param[in]	ihex	: index of hexahedron from 1 to nbhex
 * @param[in]	reqhex	: integer required property of the hexahedron
 * @return a status code
 */
MESHGEMS_METHOD meshgems_status_t meshgems_mesh_set_hexahedron_required_property(meshgems_mesh_t *m, meshgems_integer ihex, meshgems_integer *reqhex);

/**
 * \ingroup mesh_hexahedron
 * Implementation declaration of how the required property of a mesh hexahedron is set.
 * @param[in]	ihex	: index of hexahedron from 1 to nbhex
 * @param[in]	reqhex	: integer required property of the hexahedron
 * @param	user_data: a user pointer.
 * @return a status code
 */
typedef meshgems_status_t (*meshgems_mesh_set_hexahedron_required_property_t)(meshgems_integer ihex, meshgems_integer *reqhex, void *user_data);

/**
 * \ingroup mesh_hexahedron
 * Sets the implementation to use when \ref meshgems_mesh_set_hexahedron_required_property is called by this \ref meshgems_mesh_t object user.
 * @param[in]	m	: the mesh
 * @param[in]	func	: the function implementation
 * @param	user_data: a user pointer to pass to the implementation.
 * @return a status code
 */
MESHGEMS_METHOD meshgems_status_t meshgems_mesh_set_set_hexahedron_required_property(meshgems_mesh_t *m,
							meshgems_mesh_set_hexahedron_required_property_t func,
							void *user_data);
/**
 * \ingroup mesh_hexahedron
 * Sets the extra vertices of a mesh hexahedron (generic method called by this \ref meshgems_mesh_t object user).
 * @param[in]	m	: the mesh
 * @param[in]	ihex	: index of hexahedron from 1 to nbhex
 * @param[in]	typehex	: integer containing the type of the hexahedron
 * @param[in]	evhex	: integer[] array containing the extra vertices of the hexahedron
 * @return a status code
 */
MESHGEMS_METHOD meshgems_status_t meshgems_mesh_set_hexahedron_extra_vertices(meshgems_mesh_t *m, meshgems_integer ihex,
                                                    meshgems_integer typehex, meshgems_integer *evhex);

/**
 * \ingroup mesh_hexahedron
 * Implementation declaration of how the extra vertices of a mesh hexahedron are set.
 * @param[in]	ihex	: index of extra vertices from 1 to nbhex
 * @param[in]	typehex	: integer containing the type of the hexahedron
 * @param[in]	evhex	: integer[] array containing the extra vertices of the hexahedron
 * @param	user_data: a user pointer.
 * @return a status code
 */
typedef meshgems_status_t (*meshgems_mesh_set_hexahedron_extra_vertices_t)(meshgems_integer ihex, meshgems_integer typehex, meshgems_integer *evhex, void *user_data);

/**
 * \ingroup mesh_hexahedron
 * Sets the implementation to use when \ref meshgems_mesh_set_hexahedron_extra_vertices is called by this \ref meshgems_mesh_t object user.
 * @param[in]	m	: the mesh
 * @param[in]	func	: the function implementation
 * @param	user_data: a user pointer to pass to the implementation.
 * @return a status code
 */
MESHGEMS_METHOD meshgems_status_t meshgems_mesh_set_set_hexahedron_extra_vertices(meshgems_mesh_t *m,
							meshgems_mesh_set_hexahedron_extra_vertices_t func,
							void *user_data);
/* **************************************** */
/**
 * \ingroup mesh_pyramid
 * Sets the number of pyramids in the mesh (generic method called by this \ref meshgems_mesh_t object user).
 * @param[in]	m	: the mesh
 * @param[in]	nbpyr	: the number of pyramids
 * @return a status code
 */
MESHGEMS_METHOD meshgems_status_t meshgems_mesh_set_pyramid_count(meshgems_mesh_t *m, meshgems_integer nbpyr);

/**
 * \ingroup mesh_pyramid
 * Implementation declaration of how the number of pyramids in the mesh is obtained.
 * @param[in]	nbpyr	: the number of pyramids
 * @param     user_data	: a user pointer.
 * @return a status code
 */
typedef meshgems_status_t (*meshgems_mesh_set_pyramid_count_t)(meshgems_integer nbpyr, void *user_data);

/**
 * \ingroup mesh_pyramid
 * Sets the implementation to use when \ref meshgems_mesh_set_pyramid_count is called by this \ref meshgems_mesh_t object user.
 * @param[in]	m	: the mesh
 * @param[in]	func	: the function implementation
 * @param     user_data	: a user pointer to pass to the implementation.
 * @return a status code
 */
MESHGEMS_METHOD meshgems_status_t meshgems_mesh_set_set_pyramid_count(meshgems_mesh_t *m,
						        meshgems_mesh_set_pyramid_count_t func,
						        void *user_data);
/**
 * \ingroup mesh_pyramid
 * Sets the vertices of a mesh pyramid (generic method called by this \ref meshgems_mesh_t object user).
 * @param[in]	m	: the mesh
 * @param[in]	ipyr	: index of pyramid from 1 to nbpyr
 * @param[in]	vpyr	: integer[5] array containing the vertices of the pyramid
 * @return a status code
 */
MESHGEMS_METHOD meshgems_status_t meshgems_mesh_set_pyramid_vertices(meshgems_mesh_t *m, meshgems_integer ipyr, meshgems_integer *vpyr);

/**
 * \ingroup mesh_pyramid
 * Implementation declaration of how the vertices of a mesh pyramid are set.
 * @param[in]	ipyr	: index of pyramid from 1 to nbpyr
 * @param[in]	vpyr	: integer[5] array containing the vertices of the pyramid
 * @param	user_data: a user pointer.
 * @return a status code
 */
typedef meshgems_status_t (*meshgems_mesh_set_pyramid_vertices_t)(meshgems_integer ipyr, meshgems_integer *vpyr, void *user_data);

/**
 * \ingroup mesh_pyramid
 * Sets the implementation to use when \ref meshgems_mesh_set_pyramid_vertices is called by this \ref meshgems_mesh_t object user.
 * @param[in]	m	: the mesh
 * @param[in]	func	: the function implementation
 * @param	user_data: a user pointer to pass to the implementation.
 * @return a status code
 */
MESHGEMS_METHOD meshgems_status_t meshgems_mesh_set_set_pyramid_vertices(meshgems_mesh_t *m,
							meshgems_mesh_set_pyramid_vertices_t func,
							void *user_data);
/**
 * \ingroup mesh_pyramid
 * Sets the tag of a mesh pyramid (generic method called by this \ref meshgems_mesh_t object user).
 * @param[in]	m	: the mesh
 * @param[in]	ipyr	: index of pyramid from 1 to nbpyr
 * @param[in]	tagpyr	: integer tag of the pyramid
 * @return a status code
 */
MESHGEMS_METHOD meshgems_status_t meshgems_mesh_set_pyramid_tag(meshgems_mesh_t *m, meshgems_integer ipyr, meshgems_integer *tagpyr);

/**
 * \ingroup mesh_pyramid
 * Implementation declaration of how the tag of a mesh pyramid is set.
 * @param[in]	ipyr	: index of pyramid from 1 to nbpyr
 * @param[in]	tagpyr	: integer tag of the pyramid
 * @param	user_data: a user pointer.
 * @return a status code
 */
typedef meshgems_status_t (*meshgems_mesh_set_pyramid_tag_t)(meshgems_integer ipyr, meshgems_integer *tagpyr, void *user_data);

/**
 * \ingroup mesh_pyramid
 * Sets the implementation to use when \ref meshgems_mesh_set_pyramid_tag is called by this \ref meshgems_mesh_t object user.
 * @param[in]	m	: the mesh
 * @param[in]	func	: the function implementation
 * @param	user_data: a user pointer to pass to the implementation.
 * @return a status code
 */
MESHGEMS_METHOD meshgems_status_t meshgems_mesh_set_set_pyramid_tag(meshgems_mesh_t *m,
							meshgems_mesh_set_pyramid_tag_t func,
							void *user_data);
/**
 * \ingroup mesh_pyramid
 * Sets the required property of a mesh pyramid (generic method called by this \ref meshgems_mesh_t object user).
 * @param[in]	m	: the mesh
 * @param[in]	ipyr	: index of pyramid from 1 to nbpyr
 * @param[in]	reqpyr	: integer required property of the pyramid
 * @return a status code
 */
MESHGEMS_METHOD meshgems_status_t meshgems_mesh_set_pyramid_required_property(meshgems_mesh_t *m, meshgems_integer ipyr, meshgems_integer *reqpyr);

/**
 * \ingroup mesh_pyramid
 * Implementation declaration of how the required property of a mesh pyramid is set.
 * @param[in]	ipyr	: index of pyramid from 1 to nbpyr
 * @param[in]	reqpyr	: integer required property of the pyramid
 * @param	user_data: a user pointer.
 * @return a status code
 */
typedef meshgems_status_t (*meshgems_mesh_set_pyramid_required_property_t)(meshgems_integer ipyr, meshgems_integer *reqpyr, void *user_data);

/**
 * \ingroup mesh_pyramid
 * Sets the implementation to use when \ref meshgems_mesh_set_pyramid_required_property is called by this \ref meshgems_mesh_t object user.
 * @param[in]	m	: the mesh
 * @param[in]	func	: the function implementation
 * @param	user_data: a user pointer to pass to the implementation.
 * @return a status code
 */
MESHGEMS_METHOD meshgems_status_t meshgems_mesh_set_set_pyramid_required_property(meshgems_mesh_t *m,
							meshgems_mesh_set_pyramid_required_property_t func,
							void *user_data);
/**
 * \ingroup mesh_pyramid
 * Sets the extra vertices of a mesh pyramid (generic method called by this \ref meshgems_mesh_t object user).
 * @param[in]	m	: the mesh
 * @param[in]	ipyr	: index of pyramid from 1 to nbpyr
 * @param[in]	typepyr	: integer containing the type of the pyramid
 * @param[in]	evpyr	: integer[] array containing the extra vertices of the pyramid
 * @return a status code
 */
MESHGEMS_METHOD meshgems_status_t meshgems_mesh_set_pyramid_extra_vertices(meshgems_mesh_t *m, meshgems_integer ipyr,
                                                    meshgems_integer typepyr, meshgems_integer *evpyr);

/**
 * \ingroup mesh_pyramid
 * Implementation declaration of how the extra vertices of a mesh pyramid are set.
 * @param[in]	ipyr	: index of extra vertices from 1 to nbpyr
 * @param[in]	typepyr	: integer containing the type of the pyramid
 * @param[in]	evpyr	: integer[] array containing the extra vertices of the pyramid
 * @param	user_data: a user pointer.
 * @return a status code
 */
typedef meshgems_status_t (*meshgems_mesh_set_pyramid_extra_vertices_t)(meshgems_integer ipyr, meshgems_integer typepyr, meshgems_integer *evpyr, void *user_data);

/**
 * \ingroup mesh_pyramid
 * Sets the implementation to use when \ref meshgems_mesh_set_pyramid_extra_vertices is called by this \ref meshgems_mesh_t object user.
 * @param[in]	m	: the mesh
 * @param[in]	func	: the function implementation
 * @param	user_data: a user pointer to pass to the implementation.
 * @return a status code
 */
MESHGEMS_METHOD meshgems_status_t meshgems_mesh_set_set_pyramid_extra_vertices(meshgems_mesh_t *m,
							meshgems_mesh_set_pyramid_extra_vertices_t func,
							void *user_data);
/* **************************************** */
/**
 * \ingroup mesh_prism
 * Sets the number of prisms in the mesh (generic method called by this \ref meshgems_mesh_t object user).
 * @param[in]	m	: the mesh
 * @param[in]	nbprism	: the number of prisms
 * @return a status code
 */
MESHGEMS_METHOD meshgems_status_t meshgems_mesh_set_prism_count(meshgems_mesh_t *m, meshgems_integer nbprism);

/**
 * \ingroup mesh_prism
 * Implementation declaration of how the number of prisms in the mesh is obtained.
 * @param[in]	nbprism	: the number of prisms
 * @param     user_data	: a user pointer.
 * @return a status code
 */
typedef meshgems_status_t (*meshgems_mesh_set_prism_count_t)(meshgems_integer nbprism, void *user_data);

/**
 * \ingroup mesh_prism
 * Sets the implementation to use when \ref meshgems_mesh_set_prism_count is called by this \ref meshgems_mesh_t object user.
 * @param[in]	m	: the mesh
 * @param[in]	func	: the function implementation
 * @param     user_data	: a user pointer to pass to the implementation.
 * @return a status code
 */
MESHGEMS_METHOD meshgems_status_t meshgems_mesh_set_set_prism_count(meshgems_mesh_t *m,
						        meshgems_mesh_set_prism_count_t func,
						        void *user_data);
/**
 * \ingroup mesh_prism
 * Sets the vertices of a mesh prism (generic method called by this \ref meshgems_mesh_t object user).
 * @param[in]	m	: the mesh
 * @param[in]	iprism	: index of prism from 1 to nbprism
 * @param[in]	vprism	: integer[6] array containing the vertices of the prism
 * @return a status code
 */
MESHGEMS_METHOD meshgems_status_t meshgems_mesh_set_prism_vertices(meshgems_mesh_t *m, meshgems_integer iprism, meshgems_integer *vprism);

/**
 * \ingroup mesh_prism
 * Implementation declaration of how the vertices of a mesh prism are set.
 * @param[in]	iprism	: index of prism from 1 to nbprism
 * @param[in]	vprism	: integer[6] array containing the vertices of the prism
 * @param	user_data: a user pointer.
 * @return a status code
 */
typedef meshgems_status_t (*meshgems_mesh_set_prism_vertices_t)(meshgems_integer iprism, meshgems_integer *vprism, void *user_data);

/**
 * \ingroup mesh_prism
 * Sets the implementation to use when \ref meshgems_mesh_set_prism_vertices is called by this \ref meshgems_mesh_t object user.
 * @param[in]	m	: the mesh
 * @param[in]	func	: the function implementation
 * @param	user_data: a user pointer to pass to the implementation.
 * @return a status code
 */
MESHGEMS_METHOD meshgems_status_t meshgems_mesh_set_set_prism_vertices(meshgems_mesh_t *m,
							meshgems_mesh_set_prism_vertices_t func,
							void *user_data);
/**
 * \ingroup mesh_prism
 * Sets the tag of a mesh prism (generic method called by this \ref meshgems_mesh_t object user).
 * @param[in]	m	: the mesh
 * @param[in]	iprism	: index of prism from 1 to nbprism
 * @param[in]	tagprism	: integer tag of the prism
 * @return a status code
 */
MESHGEMS_METHOD meshgems_status_t meshgems_mesh_set_prism_tag(meshgems_mesh_t *m, meshgems_integer iprism, meshgems_integer *tagprism);

/**
 * \ingroup mesh_prism
 * Implementation declaration of how the tag of a mesh prism is set.
 * @param[in]	iprism	: index of prism from 1 to nbprism
 * @param[in]	tagprism	: integer tag of the prism
 * @param	user_data: a user pointer.
 * @return a status code
 */
typedef meshgems_status_t (*meshgems_mesh_set_prism_tag_t)(meshgems_integer iprism, meshgems_integer *tagprism, void *user_data);

/**
 * \ingroup mesh_prism
 * Sets the implementation to use when \ref meshgems_mesh_set_prism_tag is called by this \ref meshgems_mesh_t object user.
 * @param[in]	m	: the mesh
 * @param[in]	func	: the function implementation
 * @param	user_data: a user pointer to pass to the implementation.
 * @return a status code
 */
MESHGEMS_METHOD meshgems_status_t meshgems_mesh_set_set_prism_tag(meshgems_mesh_t *m,
							meshgems_mesh_set_prism_tag_t func,
							void *user_data);
/**
 * \ingroup mesh_prism
 * Sets the required property of a mesh prism (generic method called by this \ref meshgems_mesh_t object user).
 * @param[in]	m	: the mesh
 * @param[in]	iprism	: index of prism from 1 to nbprism
 * @param[in]	reqprism	: integer required property of the prism
 * @return a status code
 */
MESHGEMS_METHOD meshgems_status_t meshgems_mesh_set_prism_required_property(meshgems_mesh_t *m, meshgems_integer iprism, meshgems_integer *reqprism);

/**
 * \ingroup mesh_prism
 * Implementation declaration of how the required property of a mesh prism is set.
 * @param[in]	iprism	: index of prism from 1 to nbprism
 * @param[in]	reqprism	: integer required property of the prism
 * @param	user_data: a user pointer.
 * @return a status code
 */
typedef meshgems_status_t (*meshgems_mesh_set_prism_required_property_t)(meshgems_integer iprism, meshgems_integer *reqprism, void *user_data);

/**
 * \ingroup mesh_prism
 * Sets the implementation to use when \ref meshgems_mesh_set_prism_required_property is called by this \ref meshgems_mesh_t object user.
 * @param[in]	m	: the mesh
 * @param[in]	func	: the function implementation
 * @param	user_data: a user pointer to pass to the implementation.
 * @return a status code
 */
MESHGEMS_METHOD meshgems_status_t meshgems_mesh_set_set_prism_required_property(meshgems_mesh_t *m,
							meshgems_mesh_set_prism_required_property_t func,
							void *user_data);
/**
 * \ingroup mesh_prism
 * Sets the extra vertices of a mesh prism (generic method called by this \ref meshgems_mesh_t object user).
 * @param[in]	m	: the mesh
 * @param[in]	iprism	: index of prism from 1 to nbprism
 * @param[in]	typeprism	: integer containing the type of the prism
 * @param[in]	evprism	: integer[] array containing the extra vertices of the prism
 * @return a status code
 */
MESHGEMS_METHOD meshgems_status_t meshgems_mesh_set_prism_extra_vertices(meshgems_mesh_t *m, meshgems_integer iprism,
                                                    meshgems_integer typeprism, meshgems_integer *evprism);

/**
 * \ingroup mesh_prism
 * Implementation declaration of how the extra vertices of a mesh prism are set.
 * @param[in]	iprism	: index of extra vertices from 1 to nbprism
 * @param[in]	typeprism	: integer containing the type of the prism
 * @param[in]	evprism	: integer[] array containing the extra vertices of the prism
 * @param	user_data: a user pointer.
 * @return a status code
 */
typedef meshgems_status_t (*meshgems_mesh_set_prism_extra_vertices_t)(meshgems_integer iprism, meshgems_integer typeprism, meshgems_integer *evprism, void *user_data);

/**
 * \ingroup mesh_prism
 * Sets the implementation to use when \ref meshgems_mesh_set_prism_extra_vertices is called by this \ref meshgems_mesh_t object user.
 * @param[in]	m	: the mesh
 * @param[in]	func	: the function implementation
 * @param	user_data: a user pointer to pass to the implementation.
 * @return a status code
 */
MESHGEMS_METHOD meshgems_status_t meshgems_mesh_set_set_prism_extra_vertices(meshgems_mesh_t *m,
							meshgems_mesh_set_prism_extra_vertices_t func,
							void *user_data);
/* **************************************** */


/* **************************************** */

/**
 * \ingroup mesh_files
 * Writes a \ref meshgems_mesh_t to a "MESH" file.
 *
 * @param[in]	m	: the mesh
 * @param[in]	name	: the file name to write into
 *
 * @retval STATUS_OK : on success, the mesh was successfully written to file \p name;
 *
 * @retval STATUS_ERROR : on failure.
 *
 */

MESHGEMS_METHOD meshgems_status_t meshgems_mesh_write_mesh(meshgems_mesh_t *m, const char *name);

/**
 * \ingroup mesh_files
 * Reads a \ref meshgems_mesh_t from a "MESH" file and creates a new mesh structure.
 *
 * @param[in]	ctx	: the working context (mesh will be attached to)
 * @param[in]	name	: the file name to read from
 *
 * @retval a pointer to a mesh : on success, the mesh was successfully read from file \p name;
 *
 * @retval NULL : on failure (see messages in the context message callback).
 *
 */

MESHGEMS_METHOD meshgems_mesh_t *meshgems_mesh_new_read_mesh(meshgems_context_t *ctx, const char *name);


/**
 * \ingroup mesh_files
 * Writes a \ref meshgems_mesh_t to a "MSH2" file.
 *
 * @param[in]	m	: the mesh
 * @param[in]	name	: the file name prefix to write into
 *
 * @retval STATUS_OK : on success, the mesh was successfully written to file \p name;
 *
 * @retval STATUS_ERROR : on failure.
 *
 */
MESHGEMS_METHOD meshgems_status_t meshgems_mesh_write_msh2(meshgems_mesh_t *m, const char *name);

/**
 * \ingroup mesh_files
 * Writes a \ref meshgems_mesh_t to a "NAS" (BDF) file.
 *
 * @param[in]	m	: the mesh
 * @param[in]	name	: the file name to write into
 *
 * @retval STATUS_OK : on success, the mesh was successfully written to file \p name;
 *
 * @retval STATUS_ERROR : on failure.
 *
 */
MESHGEMS_METHOD meshgems_status_t meshgems_mesh_write_bdf(meshgems_mesh_t *m, const char *name);

/**
 * \ingroup mesh_files
 * Read a \ref meshgems_mesh_t to a "NAS" (BDF) file.
 *
 * @param[in]	ctx	: the context
 * @param[in]	name	: the file name to read into
 *
 * @retval STATUS_OK : on success, the mesh was successfully read to file \p name;
 *
 * @retval STATUS_ERROR : on failure.
 *
 */
MESHGEMS_METHOD meshgems_mesh_t *meshgems_mesh_new_read_bdf(meshgems_context_t *ctx, const char *name);

/**
 * \ingroup mesh_files
 * Writes a \ref meshgems_mesh_t to a "Paraview legacy" (VTK) file.
 *
 * @param[in]	m	: the mesh
 * @param[in]	name	: the file name to write into
 *
 * @retval STATUS_OK : on success, the mesh was successfully written to file \p name;
 *
 * @retval STATUS_ERROR : on failure.
 *
 */
MESHGEMS_METHOD meshgems_status_t meshgems_mesh_write_vtk(meshgems_mesh_t *m, const char *name);

/**
 * \ingroup mesh_files
 * Writes a \ref meshgems_mesh_t to a "STL" file.
 *
 * @param[in]	m	: the mesh
 * @param[in]	name	: the file name to write into
 *
 * @retval STATUS_OK : on success, the mesh was successfully written to file \p name;
 *
 * @retval STATUS_ERROR : on failure.
 *
 */
MESHGEMS_METHOD meshgems_status_t meshgems_mesh_write_stl(meshgems_mesh_t *m, const char *name, meshgems_integer binary);

/**
 * \ingroup mesh_files
 * Read a \ref meshgems_mesh_t to a "STL" file.
 *
 * @param[in]	ctx	: the context
 * @param[in]	name	: the file name to read into
 *
 * @retval STATUS_OK : on success, the mesh was successfully read to file \p name;
 *
 * @retval STATUS_ERROR : on failure.
 *
 */
MESHGEMS_METHOD meshgems_mesh_t *meshgems_mesh_new_read_stl(meshgems_context_t *ctx, const char *name, meshgems_real eps);

/**
 * \ingroup mesh_tag_system
 * Get the list of composite tags associated to this mesh.
 *
 * @param[in] mesh : the working mesh
 * @param[out] n_composite_tag : the number of composite tags 
 * @param[out] composite_tag : the integer array containing the list of composite tags. This buffer must be allocated by the caller with the function meshgems_mesh_calloc_generic_buffer(mesh)
 *
 * @retval STATUS_OK : on success,  \p name;
 * @retval STATUS_ERROR : on failure.
 *
 */
MESHGEMS_METHOD meshgems_status_t meshgems_mesh_get_composite_tag_list(meshgems_mesh_t *m, meshgems_integer *n_composite_tag, meshgems_integer *composite_tag);

/**
 * \ingroup mesh_tag_system
 * Get the list of basic tags that are combined in a composite tag.
 *
 * @param[in] mesh : the working mesh
 * @param[in] composite_tag : the queried composite tag
 * @param[out] n_basic_tag : the number of basic tags 
 * @param[out] basic_tag : the integer array containing the list of basic tags. This buffer must be allocated by the caller with the function meshgems_mesh_calloc_generic_buffer(mesh)
 *
 * @retval STATUS_OK : on success,  \p name;
 * @retval STATUS_ERROR : on failure.
 *
 */
MESHGEMS_METHOD meshgems_status_t meshgems_mesh_get_composite_tag_definition(meshgems_mesh_t *cad, meshgems_integer composite_tag,
								          meshgems_integer *n_basic_tag,
								          meshgems_integer *basic_tag);

/* **************************************** */

/**
 * \ingroup mesh_subdomains
 * Gets the number of volume subdomains in the mesh (generic method called by this \ref meshgems_mesh_t object user).
 * @param[in]	m	: the mesh
 * @param[out]	nbsub	: the number of volume subdomains
 * @return a status code
 */
MESHGEMS_METHOD meshgems_status_t meshgems_mesh_get_subdomain_count(meshgems_mesh_t *m, meshgems_integer *nbsub);

/**
 * \ingroup mesh_subdomains
 * Implementation declaration of how the number of volume subdomains in the mesh is obtained.
 * @param[out]	nbsub	: the number of volume subdomains
 * @param     user_data	: a user pointer.
 * @return a status code
 */
typedef meshgems_status_t (*meshgems_mesh_get_subdomain_count_t)(meshgems_integer *nbsub, void *user_data);

/**
 * \ingroup mesh_subdomains
 * Sets the implementation to use when \ref meshgems_mesh_get_subdomain_count is called by this \ref meshgems_mesh_t object user.
 * @param[in]	m	: the mesh
 * @param[in]	func	: the function implementation
 * @param     user_data	: a user pointer to pass to the implementation.
 * @return a status code
 */
MESHGEMS_METHOD meshgems_status_t meshgems_mesh_set_get_subdomain_count(meshgems_mesh_t *m,
						        meshgems_mesh_get_subdomain_count_t func,
						        void *user_data);
/**
 * \ingroup mesh_subdomains
 * Gets the subdomain seed description of a mesh subdomain (generic method called by this \ref meshgems_mesh_t object user).
 * @param[in]	m	: the mesh
 * @param[in]	isd	: index of subdomain from 1 to nbsd
 * @param[out]	subdomain_tag	: integer containing the tag used for the elements belonging to the subdomain
 * @param[out]	seed_type	: integer containing the subdomain seed element type of the subdomain
 * @param[out]	seed_idx	: integer containing the subdomain seed element index of the subdomain
 * @param[out]	seed_orientation	: integer containing the subdomain seed element orientation of the subdomain
 *                                    (normal is assumed to point toward the inside of the subdomain). Value can be 
 *                                    MESH_ORIENTATION_FORWARD or MESH_ORIENTATION_REVERSE    
 * @return a status code
 */
MESHGEMS_METHOD meshgems_status_t meshgems_mesh_get_subdomain_description(meshgems_mesh_t *m, meshgems_integer isd,
                                meshgems_integer *subdomain_tag, meshgems_integer *seed_type,
                                meshgems_integer *seed_idx, meshgems_integer *seed_orientation);

/**
 * \ingroup mesh_subdomains
 * Implementation declaration of how the subdomain seed description of a mesh subdomain are obtained.
 * @param[in]	isd	: index of subdomain seed description from 1 to nbsd
 * @param[out]	subdomain_tag	: integer containing the tag used for the elements belonging to the subdomain
 * @param[out]	seed_type	: integer containing the subdomain seed element type of the subdomain
 * @param[out]	seed_idx	: integer containing the subdomain seed element index of the subdomain
 * @param[out]	seed_orientation	: integer containing the subdomain seed element orientation of the subdomain
 *                                    (normal is assumed to point toward the inside of the subdomain). Value can be 
 *                                    MESH_ORIENTATION_FORWARD or MESH_ORIENTATION_REVERSE
 * @param	user_data: a user pointer.
 * @return a status code
 */
typedef meshgems_status_t (*meshgems_mesh_get_subdomain_description_t)(meshgems_integer isd,
                        meshgems_integer *subdomain_tag , meshgems_integer *seed_type,
                        meshgems_integer *seed_idx, meshgems_integer *seed_orientation, void *user_data);

/**
 * \ingroup mesh_subdomains
 * Sets the implementation to use when \ref meshgems_mesh_get_subdomain_description is called by this \ref meshgems_mesh_t object user.
 * @param[in]	m	: the mesh
 * @param[in]	func	: the function implementation
 * @param	user_data: a user pointer to pass to the implementation.
 * @return a status code
 */
MESHGEMS_METHOD meshgems_status_t meshgems_mesh_set_get_subdomain_description(meshgems_mesh_t *m,
							meshgems_mesh_get_subdomain_description_t func,
							void *user_data);

/**
 * \ingroup mesh_subdomains
 * Sets the number of volume subdomains in the mesh (generic method called by this \ref meshgems_mesh_t object user).
 * @param[in]	m	: the mesh
 * @param[in]	nbsub	: the number of volume subdomains
 * @return a status code
 */
MESHGEMS_METHOD meshgems_status_t meshgems_mesh_set_subdomain_count(meshgems_mesh_t *m, meshgems_integer nbsub);

/**
 * \ingroup mesh_subdomains
 * Implementation declaration of how the number of volume subdomains in the mesh is obtained.
 * @param[in]	nbsub	: the number of volume subdomains
 * @param     user_data	: a user pointer.
 * @return a status code
 */
typedef meshgems_status_t (*meshgems_mesh_set_subdomain_count_t)(meshgems_integer nbsub, void *user_data);

/**
 * \ingroup mesh_subdomains
 * Sets the implementation to use when \ref meshgems_mesh_set_subdomain_count is called by this \ref meshgems_mesh_t object user.
 * @param[in]	m	: the mesh
 * @param[in]	func	: the function implementation
 * @param     user_data	: a user pointer to pass to the implementation.
 * @return a status code
 */
MESHGEMS_METHOD meshgems_status_t meshgems_mesh_set_set_subdomain_count(meshgems_mesh_t *m,
						        meshgems_mesh_set_subdomain_count_t func,
						        void *user_data);
/**
 * \ingroup mesh_subdomains
 * Sets the subdomain seed description of a mesh subdomain (generic method called by this \ref meshgems_mesh_t object user).
 * @param[in]	m	: the mesh
 * @param[in]	isd	: index of subdomain from 1 to nbsd
 * @param[out]	subdomain_tag	: integer containing the tag used for the elements belonging to the subdomain
 * @param[out]	seed_type	: integer containing the subdomain seed element type of the subdomain
 * @param[out]	seed_idx	: integer containing the subdomain seed element index of the subdomain
 * @param[out]	seed_orientation	: integer containing the subdomain seed element orientation of the subdomain
 *                                    (normal is assumed to point toward the inside of the subdomain). Value can be 
 *                                    MESH_ORIENTATION_FORWARD or MESH_ORIENTATION_REVERSE
 * @return a status code
 */
MESHGEMS_METHOD meshgems_status_t meshgems_mesh_set_subdomain_description(meshgems_mesh_t *m, meshgems_integer isd,
                                meshgems_integer subdomain_tag , meshgems_integer seed_type,
                                meshgems_integer seed_idx, meshgems_integer seed_orientation);

/**
 * \ingroup mesh_subdomains
 * Implementation declaration of how the subdomain seed description of a mesh subdomain are set.
 * @param[in]	isd	: index of subdomain seed description from 1 to nbsd
 * @param[in]	sd_desc	: integer[4] array containing the subdomain seed description of the subdomain
 * @param	user_data: a user pointer.
 * @return a status code
 */
typedef meshgems_status_t (*meshgems_mesh_set_subdomain_description_t)(meshgems_integer isd,
                        meshgems_integer subdomain_tag , meshgems_integer seed_type,
                        meshgems_integer seed_idx, meshgems_integer seed_orientation, void *user_data);

/**
 * \ingroup mesh_subdomains
 * Sets the implementation to use when \ref meshgems_mesh_set_subdomain_description is called by this \ref meshgems_mesh_t object user.
 * @param[in]	m	: the mesh
 * @param[in]	func	: the function implementation
 * @param	user_data: a user pointer to pass to the implementation.
 * @return a status code
 */
MESHGEMS_METHOD meshgems_status_t meshgems_mesh_set_set_subdomain_description(meshgems_mesh_t *m,
							meshgems_mesh_set_subdomain_description_t func,
							void *user_data);
							


/**
 * Various iterators on the mesh entities.
 * 
 */
 
 
struct meshgems_mesh_iterator_tetra_using_triangle_with_tag_t_;
/**
 * \ingroup mesh_iterators
 * Opaque type storing data of a mesh_iterator which will allow the user to enumerate all the
 * tetrahedra which have a triangle with a given tag.
 */
typedef struct meshgems_mesh_iterator_tetra_using_triangle_with_tag_t_ meshgems_mesh_iterator_tetra_using_triangle_with_tag_t;

/**
 * \ingroup mesh_iterators
 * Destructor. Frees all allocated data for the iterator.
 */
MESHGEMS_METHOD void meshgems_mesh_iterator_tetra_using_triangle_with_tag_delete(meshgems_mesh_iterator_tetra_using_triangle_with_tag_t *it);

/**
 * \ingroup mesh_iterators
 * Simple constructor.
 * This the function of the iterator which performs the none trivial (and sometimes relatively expensive for 
 * very large meshes) part of the iterator.  
 * @param[in] m	: the working mesh structure.
 * @return a new \ref meshgems_mesh_iterator_*_t object
 */
MESHGEMS_METHOD meshgems_mesh_iterator_tetra_using_triangle_with_tag_t *meshgems_mesh_iterator_tetra_using_triangle_with_tag_new(meshgems_mesh_t *m);

/**
 * \ingroup mesh_iterators
 * Initialize a mesh_iterator_*_t so that it will enumerate all the tetrahedra with a triangle with the tag "tag" 
 * @param[in] it : the working iterator structure.
 * @return STATUS_OK on success
 */
MESHGEMS_METHOD meshgems_status_t meshgems_mesh_iterator_tetra_using_triangle_with_tag_initialize(meshgems_mesh_iterator_tetra_using_triangle_with_tag_t *it, meshgems_integer tag);

/**
 * \ingroup mesh_iterators
 * Set the iterator to point on the next element in the iterator. 
 * @param[in] it : the working iterator structure.
 * @return a non-null value on success, 0 if there is no next element
 */
MESHGEMS_METHOD meshgems_integer meshgems_mesh_iterator_tetra_using_triangle_with_tag_next(meshgems_mesh_iterator_tetra_using_triangle_with_tag_t *it);

/**
 * \ingroup mesh_iterators
 * Get the current tetrahedron index the iterator is pointing on.
 * @param[in] it : the working iterator structure.
 * @return the current tetrahedron index the iterator is pointing on
 */
MESHGEMS_METHOD meshgems_integer meshgems_mesh_iterator_tetra_using_triangle_with_tag_get(meshgems_mesh_iterator_tetra_using_triangle_with_tag_t *it);



struct meshgems_mesh_iterator_tetra_using_edge_with_tag_t_;
/**
 * \ingroup mesh_iterators
 * Opaque type storing data of a mesh_iterator which will allow the user to enumerate all the
 * tetrahedra which have an edge with a given tag.
 */
typedef struct meshgems_mesh_iterator_tetra_using_edge_with_tag_t_ meshgems_mesh_iterator_tetra_using_edge_with_tag_t;

/**
 * \ingroup mesh_iterators
 * Destructor. Frees all allocated data for the iterator.
 */
MESHGEMS_METHOD void meshgems_mesh_iterator_tetra_using_edge_with_tag_delete(meshgems_mesh_iterator_tetra_using_edge_with_tag_t *it);

/**
 * \ingroup mesh_iterators
 * Simple constructor.
 * This the function of the iterator which performs the none trivial (and sometimes relatively expensive for 
 * very large meshes) part of the iterator.  
 * @param[in] m	: the working mesh structure.
 * @return a new \ref meshgems_mesh_iterator_*_t object
 */
MESHGEMS_METHOD meshgems_mesh_iterator_tetra_using_edge_with_tag_t *meshgems_mesh_iterator_tetra_using_edge_with_tag_new(meshgems_mesh_t *m);

/**
 * \ingroup mesh_iterators
 * Initialize a mesh_iterator_*_t so that it will enumerate all the tetrahedra with an edge with the tag "tag" 
 * @param[in] it : the working iterator structure.
 * @return STATUS_OK on success
 */
MESHGEMS_METHOD meshgems_status_t meshgems_mesh_iterator_tetra_using_edge_with_tag_initialize(meshgems_mesh_iterator_tetra_using_edge_with_tag_t *it, meshgems_integer tag);

/**
 * \ingroup mesh_iterators
 * Set the iterator to point on the next element in the iterator. 
 * @param[in] it : the working iterator structure.
 * @return a non-null value on success, 0 if there is no next element
 */
MESHGEMS_METHOD meshgems_integer meshgems_mesh_iterator_tetra_using_edge_with_tag_next(meshgems_mesh_iterator_tetra_using_edge_with_tag_t *it);

/**
 * \ingroup mesh_iterators
 * Get the current tetrahedron index the iterator is pointing on.
 * @param[in] it : the working iterator structure.
 * @return the current tetrahedron index the iterator is pointing on
 */
MESHGEMS_METHOD meshgems_integer meshgems_mesh_iterator_tetra_using_edge_with_tag_get(meshgems_mesh_iterator_tetra_using_edge_with_tag_t *it);



struct meshgems_mesh_iterator_triangle_using_edge_with_tag_t_;
/**
 * \ingroup mesh_iterators
 * Opaque type storing data of a mesh_iterator which will allow the user to enumerate all the
 * tetrahedra which have an edge with a given tag.
 */
typedef struct meshgems_mesh_iterator_triangle_using_edge_with_tag_t_ meshgems_mesh_iterator_triangle_using_edge_with_tag_t;

/**
 * \ingroup mesh_iterators
 * Destructor. Frees all allocated data for the iterator.
 */
MESHGEMS_METHOD void meshgems_mesh_iterator_triangle_using_edge_with_tag_delete(meshgems_mesh_iterator_triangle_using_edge_with_tag_t *it);

/**
 * \ingroup mesh_iterators
 * Simple constructor.
 * This the function of the iterator which performs the none trivial (and sometimes relatively expensive for 
 * very large meshes) part of the iterator.  
 * @param[in] m	: the working mesh structure.
 * @return a new \ref meshgems_mesh_iterator_*_t object
 */
MESHGEMS_METHOD meshgems_mesh_iterator_triangle_using_edge_with_tag_t *meshgems_mesh_iterator_triangle_using_edge_with_tag_new(meshgems_mesh_t *m);

/**
 * \ingroup mesh_iterators
 * Initialize a mesh_iterator_*_t so that it will enumerate all the triangles with an edge with the tag "tag" 
 * @param[in] it : the working iterator structure.
 * @return STATUS_OK on success
 */
MESHGEMS_METHOD meshgems_status_t meshgems_mesh_iterator_triangle_using_edge_with_tag_initialize(meshgems_mesh_iterator_triangle_using_edge_with_tag_t *it, meshgems_integer tag);

/**
 * \ingroup mesh_iterators
 * Set the iterator to point on the next element in the iterator. 
 * @param[in] it : the working iterator structure.
 * @return a non-null value on success, 0 if there is no next element
 */
MESHGEMS_METHOD meshgems_integer meshgems_mesh_iterator_triangle_using_edge_with_tag_next(meshgems_mesh_iterator_triangle_using_edge_with_tag_t *it);

/**
 * \ingroup mesh_iterators
 * Get the current tetrahedron index the iterator is pointing on.
 * @param[in] it : the working iterator structure.
 * @return the current tetrahedron index the iterator is pointing on
 */
MESHGEMS_METHOD meshgems_integer meshgems_mesh_iterator_triangle_using_edge_with_tag_get(meshgems_mesh_iterator_triangle_using_edge_with_tag_t *it);



/**
 * Functions to compute different quality criteria for mesh elements
 * 
 */


/**
 * \ingroup mesh_statistics
 * Get the shape factor of a given tetrahedron.
 * @param[in] m : the working mesh structure.
 * @param[in] i : the index of the desired tetrahedron.
 * @param[out] q : the shape factor of the i-th tetrahedron.
 * @return STATUS_OK on success
 */
MESHGEMS_METHOD meshgems_status_t meshgems_mesh_get_tetrahedron_shape_factor(meshgems_mesh_t *m, meshgems_integer i, meshgems_real *q);

/**
 * \ingroup mesh_statistics
 * Get the shape factor of a given hexahedron.
 * @param[in] m : the working mesh structure.
 * @param[in] i : the index of the desired hexahedron.
 * @param[out] q : the shape factor of the i-th hexahedron.
 * @return STATUS_OK on success
 */
MESHGEMS_METHOD meshgems_status_t meshgems_mesh_get_hexahedron_shape_factor(meshgems_mesh_t *m, meshgems_integer i, meshgems_real *q);

/**
 * \ingroup mesh_statistics
 * Get the shape factor of a given pyramid.
 * @param[in] m : the working mesh structure.
 * @param[in] i : the index of the desired pyramid.
 * @param[out] q : the shape factor of the i-th pyramid.
 * @return STATUS_OK on success
 */
MESHGEMS_METHOD meshgems_status_t meshgems_mesh_get_pyramid_shape_factor(meshgems_mesh_t *m, meshgems_integer i, meshgems_real *q);

/**
 * \ingroup mesh_statistics
 * Get the shape factor of a given prism.
 * @param[in] m : the working mesh structure.
 * @param[in] i : the index of the desired prism.
 * @param[out] q : the shape factor of the i-th prism.
 * @return STATUS_OK on success
 */
MESHGEMS_METHOD meshgems_status_t meshgems_mesh_get_prism_shape_factor(meshgems_mesh_t *m, meshgems_integer i, meshgems_real *q);

/**
 * \ingroup mesh_statistics
 * Get the aspect ratio of a given triangle.
 * @param[in] m : the working mesh structure.
 * @param[in] i : the index of the desired triangle.
 * @param[out] q : the aspect ratio of the i-th triangle.
 * @return STATUS_OK on success
 */
MESHGEMS_METHOD meshgems_status_t meshgems_mesh_get_triangle_aspect_ratio(meshgems_mesh_t *m, meshgems_integer i, meshgems_real *q);

/**
 * \ingroup mesh_statistics
 * Get the aspect ratio of a given quadrangle.
 * @param[in] m : the working mesh structure.
 * @param[in] i : the index of the desired quadrangle.
 * @param[out] q : the aspect ratio of the i-th quadrangle.
 * @return STATUS_OK on success
 */
MESHGEMS_METHOD meshgems_status_t meshgems_mesh_get_quadrangle_aspect_ratio(meshgems_mesh_t *m, meshgems_integer i, meshgems_real *q);

/**
 * \ingroup mesh_statistics
 * Get the aspect ratio of a given tetrahedron.
 * @param[in] m : the working mesh structure.
 * @param[in] i : the index of the desired tetrahedron.
 * @param[out] q : the aspect ratio of the i-th tetrahedron.
 * @return STATUS_OK on success
 */
MESHGEMS_METHOD meshgems_status_t meshgems_mesh_get_tetrahedron_aspect_ratio(meshgems_mesh_t *m, meshgems_integer i, meshgems_real *q);

/**
 * \ingroup mesh_statistics
 * Get the aspect ratio of a given hexahedron.
 * @param[in] m : the working mesh structure.
 * @param[in] i : the index of the desired hexahedron.
 * @param[out] q : the aspect ratio of the i-th hexahedron.
 * @return STATUS_OK on success
 */
MESHGEMS_METHOD meshgems_status_t meshgems_mesh_get_hexahedron_aspect_ratio(meshgems_mesh_t *m, meshgems_integer i, meshgems_real *q);

/**
 * \ingroup mesh_statistics
 * Get the inverse aspect ratio of a given triangle.
 * @param[in] m : the working mesh structure.
 * @param[in] i : the index of the desired triangle.
 * @param[out] q : the inverse aspect ratio of the i-th triangle.
 * @return STATUS_OK on success
 */
MESHGEMS_METHOD meshgems_status_t meshgems_mesh_get_triangle_inverse_aspect_ratio(meshgems_mesh_t *m, meshgems_integer i, meshgems_real *q);

/**
 * \ingroup mesh_statistics
 * Get the inverse aspect ratio of a given quadrangle.
 * @param[in] m : the working mesh structure.
 * @param[in] i : the index of the desired quadrangle.
 * @param[out] q : the inverse aspect ratio of the i-th quadrangle.
 * @return STATUS_OK on success
 */
MESHGEMS_METHOD meshgems_status_t meshgems_mesh_get_quadrangle_inverse_aspect_ratio(meshgems_mesh_t *m, meshgems_integer i, meshgems_real *q);

/**
 * \ingroup mesh_statistics
 * Get the inverse aspect ratio of a given tetrahedron.
 * @param[in] m : the working mesh structure.
 * @param[in] i : the index of the desired tetrahedron.
 * @param[out] q : the inverse aspect ratio of the i-th tetrahedron.
 * @return STATUS_OK on success
 */
MESHGEMS_METHOD meshgems_status_t meshgems_mesh_get_tetrahedron_inverse_aspect_ratio(meshgems_mesh_t *m, meshgems_integer i, meshgems_real *q);

/**
 * \ingroup mesh_statistics
 * Get the inverse aspect ratio of a given hexahedron.
 * @param[in] m : the working mesh structure.
 * @param[in] i : the index of the desired hexahedron.
 * @param[out] q : the inverse aspect ratio of the i-th hexahedron.
 * @return STATUS_OK on success
 */
MESHGEMS_METHOD meshgems_status_t meshgems_mesh_get_hexahedron_inverse_aspect_ratio(meshgems_mesh_t *m, meshgems_integer i, meshgems_real *q);



/** @} */


/** Backward compatibility interface. Should not be used anymore. **/



/**
 * Gets the required status of a mesh vertex (generic method called by this \ref meshgems_mesh_t object user).
 * @param[in]	m	: the mesh
 * @param[in]	ivtx	: index of vertex from 1 to nbvtx
 * @param[out]	reqvtx	: integer required status of the vertex
 * @return a status code
 */
MESHGEMS_METHOD meshgems_status_t meshgems_mesh_get_vertex_required_status(meshgems_mesh_t *m, meshgems_integer ivtx, meshgems_integer *reqvtx);
 
/**
 * Implementation declaration of how the required status of a mesh vertex is obtained.
 * @param[in]	ivtx	: index of vertex from 1 to nbvtx
 * @param[out]	reqvtx	: integer required status of the vertex
 * @param	user_data: a user pointer.
 * @return a status code
 */
typedef meshgems_status_t (*meshgems_mesh_get_vertex_required_status_t)(meshgems_integer ivtx, meshgems_integer *reqvtx, void *user_data);

/**
 * Sets the implementation to use when \ref meshgems_mesh_get_vertex_required_status is called by this \ref meshgems_mesh_t object user.
 * @param[in]	m	: the mesh
 * @param[in]	func	: the function implementation
 * @param	user_data: a user pointer to pass to the implementation.
 * @return a status code
 */
MESHGEMS_METHOD meshgems_status_t meshgems_mesh_set_get_vertex_required_status(meshgems_mesh_t *m,
							meshgems_mesh_get_vertex_required_status_t func,
							void *user_data);

/**
 * Sets the required status of a mesh vertex (generic method called by this \ref meshgems_mesh_t object user).
 * @param[in]	m	: the mesh
 * @param[in]	ivtx	: index of vertex from 1 to nbvtx
 * @param[in]	reqvtx	: integer required status of the vertex
 * @return a status code
 */
MESHGEMS_METHOD meshgems_status_t meshgems_mesh_set_vertex_required_status(meshgems_mesh_t *m, meshgems_integer ivtx, meshgems_integer *reqvtx);

/**
 * Implementation declaration of how the required status of a mesh vertex is set.
 * @param[in]	ivtx	: index of vertex from 1 to nbvtx
 * @param[in]	reqvtx	: integer required status of the vertex
 * @param	user_data: a user pointer.
 * @return a status code
 */
typedef meshgems_status_t (*meshgems_mesh_set_vertex_required_status_t)(meshgems_integer ivtx, meshgems_integer *reqvtx, void *user_data);

/**
 * Sets the implementation to use when \ref meshgems_mesh_set_vertex_required_status is called by this \ref meshgems_mesh_t object user.
 * @param[in]	m	: the mesh
 * @param[in]	func	: the function implementation
 * @param	user_data: a user pointer to pass to the implementation.
 * @return a status code
 */
MESHGEMS_METHOD meshgems_status_t meshgems_mesh_set_set_vertex_required_status(meshgems_mesh_t *m,
 							meshgems_mesh_set_vertex_required_status_t func,
 							void *user_data);
;
/**
 * Gets the required status of a mesh edge (generic method called by this \ref meshgems_mesh_t object user).
 * @param[in]	m	: the mesh
 * @param[in]	iedg	: index of edge from 1 to nbedg
 * @param[out]	reqedg	: integer required status of the edge
 * @return a status code
 */
MESHGEMS_METHOD meshgems_status_t meshgems_mesh_get_edge_required_status(meshgems_mesh_t *m, meshgems_integer iedg, meshgems_integer *reqedg);
 
/**
 * Implementation declaration of how the required status of a mesh edge is obtained.
 * @param[in]	iedg	: index of edge from 1 to nbedg
 * @param[out]	reqedg	: integer required status of the edge
 * @param	user_data: a user pointer.
 * @return a status code
 */
typedef meshgems_status_t (*meshgems_mesh_get_edge_required_status_t)(meshgems_integer iedg, meshgems_integer *reqedg, void *user_data);

/**
 * Sets the implementation to use when \ref meshgems_mesh_get_edge_required_status is called by this \ref meshgems_mesh_t object user.
 * @param[in]	m	: the mesh
 * @param[in]	func	: the function implementation
 * @param	user_data: a user pointer to pass to the implementation.
 * @return a status code
 */
MESHGEMS_METHOD meshgems_status_t meshgems_mesh_set_get_edge_required_status(meshgems_mesh_t *m,
							meshgems_mesh_get_edge_required_status_t func,
							void *user_data);

/**
 * Sets the required status of a mesh edge (generic method called by this \ref meshgems_mesh_t object user).
 * @param[in]	m	: the mesh
 * @param[in]	iedg	: index of edge from 1 to nbedg
 * @param[in]	reqedg	: integer required status of the edge
 * @return a status code
 */
MESHGEMS_METHOD meshgems_status_t meshgems_mesh_set_edge_required_status(meshgems_mesh_t *m, meshgems_integer iedg, meshgems_integer *reqedg);

/**
 * Implementation declaration of how the required status of a mesh edge is set.
 * @param[in]	iedg	: index of edge from 1 to nbedg
 * @param[in]	reqedg	: integer required status of the edge
 * @param	user_data: a user pointer.
 * @return a status code
 */
typedef meshgems_status_t (*meshgems_mesh_set_edge_required_status_t)(meshgems_integer iedg, meshgems_integer *reqedg, void *user_data);

/**
 * Sets the implementation to use when \ref meshgems_mesh_set_edge_required_status is called by this \ref meshgems_mesh_t object user.
 * @param[in]	m	: the mesh
 * @param[in]	func	: the function implementation
 * @param	user_data: a user pointer to pass to the implementation.
 * @return a status code
 */
MESHGEMS_METHOD meshgems_status_t meshgems_mesh_set_set_edge_required_status(meshgems_mesh_t *m,
 							meshgems_mesh_set_edge_required_status_t func,
 							void *user_data);
;
/**
 * Gets the required status of a mesh triangle (generic method called by this \ref meshgems_mesh_t object user).
 * @param[in]	m	: the mesh
 * @param[in]	itri	: index of triangle from 1 to nbtri
 * @param[out]	reqtri	: integer required status of the triangle
 * @return a status code
 */
MESHGEMS_METHOD meshgems_status_t meshgems_mesh_get_triangle_required_status(meshgems_mesh_t *m, meshgems_integer itri, meshgems_integer *reqtri);
 
/**
 * Implementation declaration of how the required status of a mesh triangle is obtained.
 * @param[in]	itri	: index of triangle from 1 to nbtri
 * @param[out]	reqtri	: integer required status of the triangle
 * @param	user_data: a user pointer.
 * @return a status code
 */
typedef meshgems_status_t (*meshgems_mesh_get_triangle_required_status_t)(meshgems_integer itri, meshgems_integer *reqtri, void *user_data);

/**
 * Sets the implementation to use when \ref meshgems_mesh_get_triangle_required_status is called by this \ref meshgems_mesh_t object user.
 * @param[in]	m	: the mesh
 * @param[in]	func	: the function implementation
 * @param	user_data: a user pointer to pass to the implementation.
 * @return a status code
 */
MESHGEMS_METHOD meshgems_status_t meshgems_mesh_set_get_triangle_required_status(meshgems_mesh_t *m,
							meshgems_mesh_get_triangle_required_status_t func,
							void *user_data);

/**
 * Sets the required status of a mesh triangle (generic method called by this \ref meshgems_mesh_t object user).
 * @param[in]	m	: the mesh
 * @param[in]	itri	: index of triangle from 1 to nbtri
 * @param[in]	reqtri	: integer required status of the triangle
 * @return a status code
 */
MESHGEMS_METHOD meshgems_status_t meshgems_mesh_set_triangle_required_status(meshgems_mesh_t *m, meshgems_integer itri, meshgems_integer *reqtri);

/**
 * Implementation declaration of how the required status of a mesh triangle is set.
 * @param[in]	itri	: index of triangle from 1 to nbtri
 * @param[in]	reqtri	: integer required status of the triangle
 * @param	user_data: a user pointer.
 * @return a status code
 */
typedef meshgems_status_t (*meshgems_mesh_set_triangle_required_status_t)(meshgems_integer itri, meshgems_integer *reqtri, void *user_data);

/**
 * Sets the implementation to use when \ref meshgems_mesh_set_triangle_required_status is called by this \ref meshgems_mesh_t object user.
 * @param[in]	m	: the mesh
 * @param[in]	func	: the function implementation
 * @param	user_data: a user pointer to pass to the implementation.
 * @return a status code
 */
MESHGEMS_METHOD meshgems_status_t meshgems_mesh_set_set_triangle_required_status(meshgems_mesh_t *m,
 							meshgems_mesh_set_triangle_required_status_t func,
 							void *user_data);
;
/**
 * Gets the required status of a mesh quadrangle (generic method called by this \ref meshgems_mesh_t object user).
 * @param[in]	m	: the mesh
 * @param[in]	iquad	: index of quadrangle from 1 to nbquad
 * @param[out]	reqquad	: integer required status of the quadrangle
 * @return a status code
 */
MESHGEMS_METHOD meshgems_status_t meshgems_mesh_get_quadrangle_required_status(meshgems_mesh_t *m, meshgems_integer iquad, meshgems_integer *reqquad);
 
/**
 * Implementation declaration of how the required status of a mesh quadrangle is obtained.
 * @param[in]	iquad	: index of quadrangle from 1 to nbquad
 * @param[out]	reqquad	: integer required status of the quadrangle
 * @param	user_data: a user pointer.
 * @return a status code
 */
typedef meshgems_status_t (*meshgems_mesh_get_quadrangle_required_status_t)(meshgems_integer iquad, meshgems_integer *reqquad, void *user_data);

/**
 * Sets the implementation to use when \ref meshgems_mesh_get_quadrangle_required_status is called by this \ref meshgems_mesh_t object user.
 * @param[in]	m	: the mesh
 * @param[in]	func	: the function implementation
 * @param	user_data: a user pointer to pass to the implementation.
 * @return a status code
 */
MESHGEMS_METHOD meshgems_status_t meshgems_mesh_set_get_quadrangle_required_status(meshgems_mesh_t *m,
							meshgems_mesh_get_quadrangle_required_status_t func,
							void *user_data);

/**
 * Sets the required status of a mesh quadrangle (generic method called by this \ref meshgems_mesh_t object user).
 * @param[in]	m	: the mesh
 * @param[in]	iquad	: index of quadrangle from 1 to nbquad
 * @param[in]	reqquad	: integer required status of the quadrangle
 * @return a status code
 */
MESHGEMS_METHOD meshgems_status_t meshgems_mesh_set_quadrangle_required_status(meshgems_mesh_t *m, meshgems_integer iquad, meshgems_integer *reqquad);

/**
 * Implementation declaration of how the required status of a mesh quadrangle is set.
 * @param[in]	iquad	: index of quadrangle from 1 to nbquad
 * @param[in]	reqquad	: integer required status of the quadrangle
 * @param	user_data: a user pointer.
 * @return a status code
 */
typedef meshgems_status_t (*meshgems_mesh_set_quadrangle_required_status_t)(meshgems_integer iquad, meshgems_integer *reqquad, void *user_data);

/**
 * Sets the implementation to use when \ref meshgems_mesh_set_quadrangle_required_status is called by this \ref meshgems_mesh_t object user.
 * @param[in]	m	: the mesh
 * @param[in]	func	: the function implementation
 * @param	user_data: a user pointer to pass to the implementation.
 * @return a status code
 */
MESHGEMS_METHOD meshgems_status_t meshgems_mesh_set_set_quadrangle_required_status(meshgems_mesh_t *m,
 							meshgems_mesh_set_quadrangle_required_status_t func,
 							void *user_data);
;
/**
 * Gets the required status of a mesh tetrahedron (generic method called by this \ref meshgems_mesh_t object user).
 * @param[in]	m	: the mesh
 * @param[in]	itet	: index of tetrahedron from 1 to nbtet
 * @param[out]	reqtet	: integer required status of the tetrahedron
 * @return a status code
 */
MESHGEMS_METHOD meshgems_status_t meshgems_mesh_get_tetrahedron_required_status(meshgems_mesh_t *m, meshgems_integer itet, meshgems_integer *reqtet);
 
/**
 * Implementation declaration of how the required status of a mesh tetrahedron is obtained.
 * @param[in]	itet	: index of tetrahedron from 1 to nbtet
 * @param[out]	reqtet	: integer required status of the tetrahedron
 * @param	user_data: a user pointer.
 * @return a status code
 */
typedef meshgems_status_t (*meshgems_mesh_get_tetrahedron_required_status_t)(meshgems_integer itet, meshgems_integer *reqtet, void *user_data);

/**
 * Sets the implementation to use when \ref meshgems_mesh_get_tetrahedron_required_status is called by this \ref meshgems_mesh_t object user.
 * @param[in]	m	: the mesh
 * @param[in]	func	: the function implementation
 * @param	user_data: a user pointer to pass to the implementation.
 * @return a status code
 */
MESHGEMS_METHOD meshgems_status_t meshgems_mesh_set_get_tetrahedron_required_status(meshgems_mesh_t *m,
							meshgems_mesh_get_tetrahedron_required_status_t func,
							void *user_data);

/**
 * Sets the required status of a mesh tetrahedron (generic method called by this \ref meshgems_mesh_t object user).
 * @param[in]	m	: the mesh
 * @param[in]	itet	: index of tetrahedron from 1 to nbtet
 * @param[in]	reqtet	: integer required status of the tetrahedron
 * @return a status code
 */
MESHGEMS_METHOD meshgems_status_t meshgems_mesh_set_tetrahedron_required_status(meshgems_mesh_t *m, meshgems_integer itet, meshgems_integer *reqtet);

/**
 * Implementation declaration of how the required status of a mesh tetrahedron is set.
 * @param[in]	itet	: index of tetrahedron from 1 to nbtet
 * @param[in]	reqtet	: integer required status of the tetrahedron
 * @param	user_data: a user pointer.
 * @return a status code
 */
typedef meshgems_status_t (*meshgems_mesh_set_tetrahedron_required_status_t)(meshgems_integer itet, meshgems_integer *reqtet, void *user_data);

/**
 * Sets the implementation to use when \ref meshgems_mesh_set_tetrahedron_required_status is called by this \ref meshgems_mesh_t object user.
 * @param[in]	m	: the mesh
 * @param[in]	func	: the function implementation
 * @param	user_data: a user pointer to pass to the implementation.
 * @return a status code
 */
MESHGEMS_METHOD meshgems_status_t meshgems_mesh_set_set_tetrahedron_required_status(meshgems_mesh_t *m,
 							meshgems_mesh_set_tetrahedron_required_status_t func,
 							void *user_data);
;
/**
 * Gets the required status of a mesh hexahedron (generic method called by this \ref meshgems_mesh_t object user).
 * @param[in]	m	: the mesh
 * @param[in]	ihex	: index of hexahedron from 1 to nbhex
 * @param[out]	reqhex	: integer required status of the hexahedron
 * @return a status code
 */
MESHGEMS_METHOD meshgems_status_t meshgems_mesh_get_hexahedron_required_status(meshgems_mesh_t *m, meshgems_integer ihex, meshgems_integer *reqhex);
 
/**
 * Implementation declaration of how the required status of a mesh hexahedron is obtained.
 * @param[in]	ihex	: index of hexahedron from 1 to nbhex
 * @param[out]	reqhex	: integer required status of the hexahedron
 * @param	user_data: a user pointer.
 * @return a status code
 */
typedef meshgems_status_t (*meshgems_mesh_get_hexahedron_required_status_t)(meshgems_integer ihex, meshgems_integer *reqhex, void *user_data);

/**
 * Sets the implementation to use when \ref meshgems_mesh_get_hexahedron_required_status is called by this \ref meshgems_mesh_t object user.
 * @param[in]	m	: the mesh
 * @param[in]	func	: the function implementation
 * @param	user_data: a user pointer to pass to the implementation.
 * @return a status code
 */
MESHGEMS_METHOD meshgems_status_t meshgems_mesh_set_get_hexahedron_required_status(meshgems_mesh_t *m,
							meshgems_mesh_get_hexahedron_required_status_t func,
							void *user_data);

/**
 * Sets the required status of a mesh hexahedron (generic method called by this \ref meshgems_mesh_t object user).
 * @param[in]	m	: the mesh
 * @param[in]	ihex	: index of hexahedron from 1 to nbhex
 * @param[in]	reqhex	: integer required status of the hexahedron
 * @return a status code
 */
MESHGEMS_METHOD meshgems_status_t meshgems_mesh_set_hexahedron_required_status(meshgems_mesh_t *m, meshgems_integer ihex, meshgems_integer *reqhex);

/**
 * Implementation declaration of how the required status of a mesh hexahedron is set.
 * @param[in]	ihex	: index of hexahedron from 1 to nbhex
 * @param[in]	reqhex	: integer required status of the hexahedron
 * @param	user_data: a user pointer.
 * @return a status code
 */
typedef meshgems_status_t (*meshgems_mesh_set_hexahedron_required_status_t)(meshgems_integer ihex, meshgems_integer *reqhex, void *user_data);

/**
 * Sets the implementation to use when \ref meshgems_mesh_set_hexahedron_required_status is called by this \ref meshgems_mesh_t object user.
 * @param[in]	m	: the mesh
 * @param[in]	func	: the function implementation
 * @param	user_data: a user pointer to pass to the implementation.
 * @return a status code
 */
MESHGEMS_METHOD meshgems_status_t meshgems_mesh_set_set_hexahedron_required_status(meshgems_mesh_t *m,
 							meshgems_mesh_set_hexahedron_required_status_t func,
 							void *user_data);
;
/**
 * Gets the required status of a mesh prism (generic method called by this \ref meshgems_mesh_t object user).
 * @param[in]	m	: the mesh
 * @param[in]	iprism	: index of prism from 1 to nbprism
 * @param[out]	reqprism	: integer required status of the prism
 * @return a status code
 */
MESHGEMS_METHOD meshgems_status_t meshgems_mesh_get_prism_required_status(meshgems_mesh_t *m, meshgems_integer iprism, meshgems_integer *reqprism);
 
/**
 * Implementation declaration of how the required status of a mesh prism is obtained.
 * @param[in]	iprism	: index of prism from 1 to nbprism
 * @param[out]	reqprism	: integer required status of the prism
 * @param	user_data: a user pointer.
 * @return a status code
 */
typedef meshgems_status_t (*meshgems_mesh_get_prism_required_status_t)(meshgems_integer iprism, meshgems_integer *reqprism, void *user_data);

/**
 * Sets the implementation to use when \ref meshgems_mesh_get_prism_required_status is called by this \ref meshgems_mesh_t object user.
 * @param[in]	m	: the mesh
 * @param[in]	func	: the function implementation
 * @param	user_data: a user pointer to pass to the implementation.
 * @return a status code
 */
MESHGEMS_METHOD meshgems_status_t meshgems_mesh_set_get_prism_required_status(meshgems_mesh_t *m,
							meshgems_mesh_get_prism_required_status_t func,
							void *user_data);

/**
 * Sets the required status of a mesh prism (generic method called by this \ref meshgems_mesh_t object user).
 * @param[in]	m	: the mesh
 * @param[in]	iprism	: index of prism from 1 to nbprism
 * @param[in]	reqprism	: integer required status of the prism
 * @return a status code
 */
MESHGEMS_METHOD meshgems_status_t meshgems_mesh_set_prism_required_status(meshgems_mesh_t *m, meshgems_integer iprism, meshgems_integer *reqprism);

/**
 * Implementation declaration of how the required status of a mesh prism is set.
 * @param[in]	iprism	: index of prism from 1 to nbprism
 * @param[in]	reqprism	: integer required status of the prism
 * @param	user_data: a user pointer.
 * @return a status code
 */
typedef meshgems_status_t (*meshgems_mesh_set_prism_required_status_t)(meshgems_integer iprism, meshgems_integer *reqprism, void *user_data);

/**
 * Sets the implementation to use when \ref meshgems_mesh_set_prism_required_status is called by this \ref meshgems_mesh_t object user.
 * @param[in]	m	: the mesh
 * @param[in]	func	: the function implementation
 * @param	user_data: a user pointer to pass to the implementation.
 * @return a status code
 */
MESHGEMS_METHOD meshgems_status_t meshgems_mesh_set_set_prism_required_status(meshgems_mesh_t *m,
 							meshgems_mesh_set_prism_required_status_t func,
 							void *user_data);
;
/**
 * Gets the required status of a mesh pyramid (generic method called by this \ref meshgems_mesh_t object user).
 * @param[in]	m	: the mesh
 * @param[in]	ipyra	: index of pyramid from 1 to nbpyra
 * @param[out]	reqpyra	: integer required status of the pyramid
 * @return a status code
 */
MESHGEMS_METHOD meshgems_status_t meshgems_mesh_get_pyramid_required_status(meshgems_mesh_t *m, meshgems_integer ipyra, meshgems_integer *reqpyra);
 
/**
 * Implementation declaration of how the required status of a mesh pyramid is obtained.
 * @param[in]	ipyra	: index of pyramid from 1 to nbpyra
 * @param[out]	reqpyra	: integer required status of the pyramid
 * @param	user_data: a user pointer.
 * @return a status code
 */
typedef meshgems_status_t (*meshgems_mesh_get_pyramid_required_status_t)(meshgems_integer ipyra, meshgems_integer *reqpyra, void *user_data);

/**
 * Sets the implementation to use when \ref meshgems_mesh_get_pyramid_required_status is called by this \ref meshgems_mesh_t object user.
 * @param[in]	m	: the mesh
 * @param[in]	func	: the function implementation
 * @param	user_data: a user pointer to pass to the implementation.
 * @return a status code
 */
MESHGEMS_METHOD meshgems_status_t meshgems_mesh_set_get_pyramid_required_status(meshgems_mesh_t *m,
							meshgems_mesh_get_pyramid_required_status_t func,
							void *user_data);

/**
 * Sets the required status of a mesh pyramid (generic method called by this \ref meshgems_mesh_t object user).
 * @param[in]	m	: the mesh
 * @param[in]	ipyra	: index of pyramid from 1 to nbpyra
 * @param[in]	reqpyra	: integer required status of the pyramid
 * @return a status code
 */
MESHGEMS_METHOD meshgems_status_t meshgems_mesh_set_pyramid_required_status(meshgems_mesh_t *m, meshgems_integer ipyra, meshgems_integer *reqpyra);

/**
 * Implementation declaration of how the required status of a mesh pyramid is set.
 * @param[in]	ipyra	: index of pyramid from 1 to nbpyra
 * @param[in]	reqpyra	: integer required status of the pyramid
 * @param	user_data: a user pointer.
 * @return a status code
 */
typedef meshgems_status_t (*meshgems_mesh_set_pyramid_required_status_t)(meshgems_integer ipyra, meshgems_integer *reqpyra, void *user_data);

/**
 * Sets the implementation to use when \ref meshgems_mesh_set_pyramid_required_status is called by this \ref meshgems_mesh_t object user.
 * @param[in]	m	: the mesh
 * @param[in]	func	: the function implementation
 * @param	user_data: a user pointer to pass to the implementation.
 * @return a status code
 */
MESHGEMS_METHOD meshgems_status_t meshgems_mesh_set_set_pyramid_required_status(meshgems_mesh_t *m,
 							meshgems_mesh_set_pyramid_required_status_t func,
 							void *user_data);
;


/** Private part of the mesh interface. Only Distene products should use it. **/

#define MESHGEMS_MESH_PRIORITY_ARGUMENT  	10
#define MESHGEMS_MESH_FORMAT_ARGUMENT	 	11
#define MESHGEMS_MESH_VERSION_ARGUMENT	 	12
#define MESHGEMS_MESH_WRITE_EXTENDED_CALLBACK 	21
#define MESHGEMS_MESH_PRINT_PROGRESS		22

#define MESHGEMS_MESH_PROGRESS_MINIMAL		0
#define MESHGEMS_MESH_PROGRESS_LEFT		1
#define MESHGEMS_MESH_PROGRESS_LEFT_CHECKPOINTS	-1
#define MESHGEMS_MESH_PROGRESS_CHECKPOINTS	-2

#define MESHGEMS_MESH_MESHGEMS_PRIORITY	 	1
#define MESHGEMS_MESH_INRIA_PRIORITY		2	
#define MESHGEMS_MESH_MESHGEMS_MIXED_PRIORITY	3
#define MESHGEMS_MESH_INRIA_MIXED_PRIORITY	4

#define MESHGEMS_MESH_FORMAT_MESHGEMS		5
#define MESHGEMS_MESH_FORMAT_INRIA		6
#define MESHGEMS_MESH_FORMAT_MIXED		7

#define MESHGEMS_MESH_ORIENTATION_UNKNOWN	0
#define MESHGEMS_MESH_ORIENTATION_FORWARD	1
#define MESHGEMS_MESH_ORIENTATION_REVERSE	-1


MESHGEMS_METHOD meshgems_status_t meshgems_mesh_get_context(meshgems_mesh_t *mesh, meshgems_context_t **ctx);

MESHGEMS_METHOD meshgems_mesh_t *meshgems_mesh_new_read_msh2(meshgems_context_t *ctx, const char *name);
MESHGEMS_METHOD meshgems_status_t meshgems_mesh_check_consistency(meshgems_mesh_t *msh);

MESHGEMS_METHOD meshgems_status_t meshgems_mesh_set_tag_system(meshgems_mesh_t *m, meshgems_tag_system_t *ts);
MESHGEMS_METHOD meshgems_status_t meshgems_mesh_get_tag_system(meshgems_mesh_t *m, meshgems_tag_system_t **ts);

MESHGEMS_METHOD meshgems_mesh_t *meshgems_mesh_new_read_mesh_extended(meshgems_context_t *ctx, const char *name, ...);
MESHGEMS_METHOD meshgems_status_t meshgems_mesh_write_mesh_extended(meshgems_mesh_t *m, const char *fname, ...);

MESHGEMS_METHOD meshgems_status_t meshgems_mesh_is_signed(meshgems_mesh_t *mesh, meshgems_integer *is);
MESHGEMS_METHOD meshgems_status_t meshgems_mesh_get_signature(meshgems_mesh_t *, char **, unsigned int *);
MESHGEMS_METHOD meshgems_status_t meshgems_mesh_set_signature(meshgems_mesh_t *, char *, unsigned int);

MESHGEMS_METHOD meshgems_status_t meshgems_mesh_attach_object(meshgems_mesh_t *, void *object, meshgems_delete_fun delfun);

MESHGEMS_METHOD meshgems_status_t meshgems_mesh_clone_tag_system(meshgems_mesh_t *mo, meshgems_mesh_t *m);
MESHGEMS_METHOD meshgems_status_t meshgems_mesh_clear_tag_system(meshgems_mesh_t *m);
MESHGEMS_METHOD meshgems_status_t meshgems_mesh_create_tag_system_from_existing_tags(meshgems_mesh_t *m);
MESHGEMS_METHOD meshgems_mesh_t *meshgems_mesh_new_tetra_grid(meshgems_context_t *ctx, meshgems_real *xyzmin, meshgems_real * xyzmax, 
                                                              meshgems_integer nx, meshgems_integer ny, meshgems_integer nz);
MESHGEMS_METHOD meshgems_status_t meshgems_mesh_get_dimension(meshgems_mesh_t *, meshgems_integer *dim);
MESHGEMS_METHOD meshgems_status_t meshgems_mesh_set_dimension(meshgems_mesh_t *, meshgems_integer dim);

MESHGEMS_METHOD meshgems_status_t meshgems_mesh_get_triangle_area(meshgems_mesh_t *m, meshgems_integer it, meshgems_real *q);
MESHGEMS_METHOD meshgems_status_t meshgems_mesh_get_triangle_angles(meshgems_mesh_t *m, meshgems_integer i, meshgems_real *q);
MESHGEMS_METHOD meshgems_status_t meshgems_mesh_get_triangle_equiangle_skew(meshgems_mesh_t *m, meshgems_integer i, meshgems_real *q);
MESHGEMS_METHOD meshgems_status_t meshgems_mesh_get_quadrangle_angles(meshgems_mesh_t *m, meshgems_integer i, meshgems_real *q);
MESHGEMS_METHOD meshgems_status_t meshgems_mesh_get_quadrangle_equiangle_skew(meshgems_mesh_t *m, meshgems_integer i, meshgems_real *q);
MESHGEMS_METHOD meshgems_status_t meshgems_mesh_get_quadrangle_planarity(meshgems_mesh_t *m, meshgems_integer i, meshgems_real *q);
MESHGEMS_METHOD meshgems_status_t meshgems_mesh_get_tetrahedron_angles(meshgems_mesh_t *m, meshgems_integer i, meshgems_real *q);
MESHGEMS_METHOD meshgems_status_t meshgems_mesh_get_tetrahedron_diedral_equiangle_skew(meshgems_mesh_t *m, meshgems_integer i, meshgems_real *q);
MESHGEMS_METHOD meshgems_status_t meshgems_mesh_get_tetrahedron_volume(meshgems_mesh_t *m, meshgems_integer it, meshgems_real *q);

MESHGEMS_METHOD meshgems_status_t meshgems_mesh_get_tetrahedron_equiangle_skew(meshgems_mesh_t *m, meshgems_integer it, meshgems_real *q);
MESHGEMS_METHOD meshgems_status_t meshgems_mesh_get_pyramid_equiangle_skew(meshgems_mesh_t *m, meshgems_integer it, meshgems_real *q);
MESHGEMS_METHOD meshgems_status_t meshgems_mesh_get_prism_equiangle_skew(meshgems_mesh_t *m, meshgems_integer it, meshgems_real *q);
MESHGEMS_METHOD meshgems_status_t meshgems_mesh_get_hexahedron_equiangle_skew(meshgems_mesh_t *m, meshgems_integer it, meshgems_real *q);
MESHGEMS_METHOD meshgems_status_t meshgems_mesh_get_pyramid_planarity(meshgems_mesh_t *m, meshgems_integer it, meshgems_real *q);
MESHGEMS_METHOD meshgems_status_t meshgems_mesh_get_prism_planarity(meshgems_mesh_t *m, meshgems_integer it, meshgems_real *q);
MESHGEMS_METHOD meshgems_status_t meshgems_mesh_get_hexahedron_planarity(meshgems_mesh_t *m, meshgems_integer it, meshgems_real *q);


MESHGEMS_METHOD meshgems_mesh_t *meshgems_mesh_new_copy(meshgems_mesh_t *mesh);
MESHGEMS_METHOD meshgems_status_t meshgems_mesh_get_required_discretization_on_reference_line_count(meshgems_mesh_t *mesh, meshgems_integer *nereq);
MESHGEMS_METHOD meshgems_status_t meshgems_mesh_get_required_discretization_on_reference_line(meshgems_mesh_t *mesh, meshgems_integer i, meshgems_integer *etag, meshgems_integer *nv, meshgems_integer **tags, meshgems_real **xyzs);

MESHGEMS_METHOD meshgems_status_t meshgems_mesh_get_face_multiple_periodicity_count(meshgems_mesh_t *m, meshgems_integer *n);
MESHGEMS_METHOD meshgems_status_t meshgems_mesh_get_face_multiple_periodicity(meshgems_mesh_t *m, meshgems_integer i, meshgems_integer **tag1, meshgems_integer *size1, meshgems_integer **tag2, meshgems_integer *size2,
									  meshgems_mesh_periodicity_transformation_t *transf, void **user_data);


typedef meshgems_status_t (*meshgems_mesh_write_mesh_callback_t)(meshgems_integer mesh_idx, void *user_data);

#endif
