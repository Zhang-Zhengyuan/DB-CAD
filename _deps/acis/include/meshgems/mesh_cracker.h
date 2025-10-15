#ifndef __MESHGEMS_MESH_CRACKER_H__
#define __MESHGEMS_MESH_CRACKER_H__

#include <meshgems/mdecl.h>
#include <meshgems/basic_types.h>

/** Opaque type defining a mesh cracker */

struct meshgems_mesh_cracker_t_;
typedef struct meshgems_mesh_cracker_t_ meshgems_mesh_cracker_t;

/* Parameters for the crack between volumes */

#define MESHGEMS_MESH_CRACKER_PARAMETER_CRACK_EXTREMITIES 1
#define MESHGEMS_MESH_CRACKER_PARAMETER_OFFSET_VERTICES 2

/**
 * Default consructor
 * @param ctx (in) : the working MeshGems context.
 */

MESHGEMS_METHOD meshgems_mesh_cracker_t *meshgems_mesh_cracker_new(meshgems_context_t *ctx);

/** Destructor */

MESHGEMS_METHOD void meshgems_mesh_cracker_delete(meshgems_mesh_cracker_t *mc);


/**
 * Set input mesh to be cracked
 * @param mc (in) : the working mesh_cracker object.
 * @param m (in) : the input mesh object.
 */

MESHGEMS_METHOD meshgems_status_t meshgems_mesh_cracker_set_input_mesh(meshgems_mesh_cracker_t *mc, meshgems_mesh_t *m);

/**
 * Function for cracking between two volumes defined by their tag.
 * @param mc (in) : the working mesh_cracker object.
 * @param volume_tag1 (in) : the tag of volume elements defining the
 *   first volume
 * @param volume_tag2 (in) : the tag of volume elements defining the
 *   second volume
 * @param surface_tag1 (in) : the tag to use for creating the cracking
 *   surface touching the first volume. This tag will be applied to
 *   nodes and surface entities.
 * @param surface_tag2 (in) : the tag to use for creating the cracking
 *   surface touching the second volume. This tag will be applied to
 *   nodes and surface entities.
 * @param parameters (in) : a bitmask of parameters (see MESH_CRACKER_PARAMETER_*)
 * @return a status code
 *
 */



MESHGEMS_METHOD meshgems_status_t meshgems_mesh_cracker_crack_between_volume_elements_with_tags(meshgems_mesh_cracker_t *mc, meshgems_integer volume_tag1,
												meshgems_integer volume_tag2, meshgems_integer surface_tag1,
												meshgems_integer surface_tag2, meshgems_integer parameters);

/**
 * Function for cracking along given triangles.
 * @param mc (in) : the working mesh_cracker object.
 * @param triangles (in) : the triangles defining the cracking surface given as an
 *   array of 3*triangle_blocks[n_triangle_blocks] vertex indices. They are grouped
 *   by block as described by the triangle_blocks parameter.
 *   These triangles must exist as tetrahedra faces in the input mesh.
 *   The orientation of the first triangle of each connected surface component
 *   will be used to define the two cracking surfaces:
 *     First cracking surface is touching the tetrahedron with the first
 *      triangle positively oriented (ie inward).
 *     Second cracking surface is touching the tetrahedron with the first
 *      triangle negatively oriented (ie outward).
 * @param triangle_blocks (in) : the list of triangle blocks in the triangles parameter,
 *   given as an array of size n_triangle_blocks+1
 *   for i=1..n_triangle_blocks, the i-th block is made of the triangles
 *   triangle_blocks[i-1]..triangle_blocks[i] from the triangles array parameter.
 * @param n_triangle_blocks (in) : the number of triangles blocks in the triangles
 *   parameter
 * @param surface_tag1 (in) : array of size n_triangle_blocks-1.
 *   for i=1..n_triangle_blocks , surface_tag1[i-1] contains the tag to use for
 *   creating the first cracking surface of the i-th block of triangles. This tag
 *   will be applied to nodes and surface entities.
 * @param surface_tags2 (in) : array of size n_triangle_blocks-1.
 *   for i=1..n_triangle_blocks , surface_tag2[i-1] contains the tag to use for
 *   creating the second cracking surface of the i-th block of triangles. This tag
 *   will be applied to nodes and surface entities.
 * @param uncracked_edges (in) : array of size 2*n_uncracked_edges containing the
 *   vertex indices defining the edges (usually belonging to the triangles defining
 *   the crack) which shoud not be cracked.
 * @param n_uncracked_edges (in) : the number of edges (usually belonging to
 *   the triangles defining the crack) which shoud not be cracked.
 * @param uncracked_vertices (in) : array of size n_uncracked_vertices containing the
 *   indices of vertices(usually belonging to the triangles defining the crack) which 
 *   shoud not be cracked.
 * @param n_uncracked_vertices (in) : the number of vertices (usually belonging to
 *   the triangles defining the crack) which shoud not be cracked.
 * @param parameters (in) : a bitmask of parameters (see MESH_CRACKER_PARAMETER_*)
 * @return a status code
 */


MESHGEMS_METHOD meshgems_status_t meshgems_mesh_cracker_crack_along_triangles(meshgems_mesh_cracker_t *mc, meshgems_integer *triangles, meshgems_integer *triangle_blocks,
									      meshgems_integer n_triangles_blocks, meshgems_integer *surface_tag1, meshgems_integer *surface_tag2,
									      meshgems_integer *uncracked_edges, meshgems_integer n_uncracked_edges, meshgems_integer *uncracked_vertices, 
									      meshgems_integer n_uncracked_vertices, meshgems_integer parameters);

MESHGEMS_METHOD meshgems_status_t meshgems_mesh_cracker_crack_along_triangles_and_quadrangles(meshgems_mesh_cracker_t *mc, meshgems_integer *triangles, meshgems_integer *triangle_blocks,
							    meshgems_integer n_triangles_blocks, meshgems_integer *tri_surface_tag1, 
							    meshgems_integer *tri_surface_tag2, meshgems_integer *quadrangles, meshgems_integer *quadrangle_blocks, 
							    meshgems_integer n_quadrangles_blocks, meshgems_integer *quad_surface_tag1, meshgems_integer *quad_surface_tag2, 
							    meshgems_integer *uncracked_edges, meshgems_integer n_uncracked_edges, meshgems_integer *uncracked_vertices, 
							    meshgems_integer n_uncracked_vertices, meshgems_integer parameters);

/**
 * Get resulting cracked mesh
 * @param mc (in) : the working mesh_cracker object.
 * @param m (out) : the output mesh object.
 */
MESHGEMS_METHOD meshgems_status_t meshgems_mesh_cracker_get_output_mesh(meshgems_mesh_cracker_t *mc, meshgems_mesh_t **m);

#endif
