#ifndef __MESHGEMS_MESH_STATISTICS_H__
#define __MESHGEMS_MESH_STATISTICS_H__


#include <stdio.h>

#include <meshgems/mdecl.h>
#include <meshgems/basic_types.h>
#include <meshgems/status.h>
#include <meshgems/context.h>
#include <meshgems/mesh.h>

/**
 * Gets the element ratio for faces 
 * @param[in]	m	: the mesh
 * @return error code
 */
MESHGEMS_METHOD meshgems_status_t meshgems_mesh_compute_statistics_2D_element_ratio(meshgems_mesh_t *m);
/**
 * Gets the number of each element
 * @param[in]	m	: the mesh
 * @return error code
 */
MESHGEMS_METHOD meshgems_status_t meshgems_mesh_compute_statistics_element_count(meshgems_mesh_t *m);
/**
 * Gets the statistics of aspect ratio for triangles
 * @param[in]	m	: the mesh
 * @return error code
 */
MESHGEMS_METHOD meshgems_status_t meshgems_mesh_compute_statistics_triangle_aspect_ratio(meshgems_mesh_t *m);
/**
 * Gets the statistics of the inverse of aspect ratio for triangles
 * @param[in]	m	: the mesh
 * @return error code
 */
MESHGEMS_METHOD meshgems_status_t meshgems_mesh_compute_statistics_triangle_inverse_aspect_ratio(meshgems_mesh_t *m);
/**
 * Gets the statistics of angles for triangles
 * @param[in]	m	: the mesh
 * @return error code
 */
MESHGEMS_METHOD meshgems_status_t meshgems_mesh_compute_statistics_triangle_angles(meshgems_mesh_t *m);
/**
 * Gets the statistics of equiangle skew for triangles
 * @param[in]	m	: the mesh
 * @return error code
 */
MESHGEMS_METHOD meshgems_status_t meshgems_mesh_compute_statistics_triangle_equiangle_skew(meshgems_mesh_t *m);
/**
 * Gets the statistics of area for triangles
 * @param[in]	m	: the mesh
 * @return error code
 */
MESHGEMS_METHOD meshgems_status_t meshgems_mesh_compute_statistics_triangle_area(meshgems_mesh_t *m);
/**
 * Gets the statistics of aspect ratio for quadrangles
 * @param[in]	m	: the mesh
 * @return error code
 */
MESHGEMS_METHOD meshgems_status_t meshgems_mesh_compute_statistics_quadrangle_aspect_ratio(meshgems_mesh_t *m);
/**
 * Gets the statistics of the inverse of aspect ratio for quadrangles
 * @param[in]	m	: the mesh
 * @return error code
 */
MESHGEMS_METHOD meshgems_status_t meshgems_mesh_compute_statistics_quadrangle_inverse_aspect_ratio(meshgems_mesh_t *m);
/**
 * Gets  the statistics of angles for quadrangles
 * @param[in]	m	: the mesh
 * @return error code
 */
MESHGEMS_METHOD meshgems_status_t meshgems_mesh_compute_statistics_quadrangle_angles(meshgems_mesh_t *m);
/**
 * Gets the statistics of equiangle skew for  quadrangles
 * @param[in]	m	: the mesh
 * @return error code
 */
MESHGEMS_METHOD meshgems_status_t meshgems_mesh_compute_statistics_quadrangle_equiangle_skew(meshgems_mesh_t *m);
/**
 * Gets the statistics of planarity for quadrangles
 * @param[in]	m	: the mesh
 * @return error code
 */
MESHGEMS_METHOD meshgems_status_t meshgems_mesh_compute_statistics_quadrangle_planarity(meshgems_mesh_t *m);
/**
 * Gets the statistics of aspect ratio for tetrahedra
 * @param[in]	m	: the mesh
 * @return error code
 */
MESHGEMS_METHOD meshgems_status_t meshgems_mesh_compute_statistics_tetrahedron_aspect_ratio(meshgems_mesh_t *m);
/**
 * Gets the statistics of the inverse of aspect ratio for tetrahedra
 * @param[in]	m	: the mesh
 * @return error code
 */
MESHGEMS_METHOD meshgems_status_t meshgems_mesh_compute_statistics_tetrahedron_inverse_aspect_ratio(meshgems_mesh_t *m);
/**
 * Gets the statistics of shape factor for tetrahedra
 * @param[in]	m	: the mesh
 * @return error code
 */
MESHGEMS_METHOD meshgems_status_t meshgems_mesh_compute_statistics_tetrahedron_shape_factor(meshgems_mesh_t *m);
/**
 * Gets the statistics of element validity for tetrahedra
 * @param[in]	m	: the mesh
 * @return error code
 */
MESHGEMS_METHOD meshgems_status_t meshgems_mesh_compute_statistics_tetrahedron_element_validity(meshgems_mesh_t *m);
/**
 * Gets the statistics of face element validity for tetrahedra
 * @param[in]	m	: the mesh
 * @return error code
 */
MESHGEMS_METHOD meshgems_status_t meshgems_mesh_compute_statistics_tetrahedron_face_element_validity(meshgems_mesh_t *m);
/**
 * Gets the statistics of angles for tetrahedra
 * @param[in]	m	: the mesh
 * @return error code
 */
MESHGEMS_METHOD meshgems_status_t meshgems_mesh_compute_statistics_tetrahedron_angles(meshgems_mesh_t *m);
/**
 * Gets the statistics of equiangle skew for tetrahedra
 * @param[in]	m	: the mesh
 * @return error code
 */
MESHGEMS_METHOD meshgems_status_t meshgems_mesh_compute_statistics_tetrahedron_diedral_equiangle_skew(meshgems_mesh_t *m);
/**
 * Gets the statistics of volume for tetrahedra
 * @param[in]	m	: the mesh
 * @return error code
 */
MESHGEMS_METHOD meshgems_status_t meshgems_mesh_compute_statistics_tetrahedron_volume(meshgems_mesh_t *m);

/**
 * Gets the statistics of volume for tetrahedra
 * @param[in]	m	: the mesh
 * @return error code
 */
MESHGEMS_METHOD meshgems_status_t meshgems_mesh_compute_statistics_tetrahedron_height(meshgems_mesh_t *m);

/**
 * Gets the statistics of aspect ratio for hexahedra
 * @param[in]	m	: the mesh
 * @return error code
 */
MESHGEMS_METHOD meshgems_status_t meshgems_mesh_compute_statistics_hexahedron_aspect_ratio(meshgems_mesh_t *m);
/**
 * Gets the statistics of the inverse of aspect ratio for hexahedra
 * @param[in]	m	: the mesh
 * @return error code
 */
MESHGEMS_METHOD meshgems_status_t meshgems_mesh_compute_statistics_hexahedron_inverse_aspect_ratio(meshgems_mesh_t *m);
/**
 * Gets the statistics of shape factor for hexahedra
 * @param[in]	m	: the mesh
 * @return error code
 */
MESHGEMS_METHOD meshgems_status_t meshgems_mesh_compute_statistics_hexahedron_shape_factor(meshgems_mesh_t *m);
/**
 * Gets the statistics of element validity for hexahedra
 * @param[in]	m	: the mesh
 * @return error code
 */
MESHGEMS_METHOD meshgems_status_t meshgems_mesh_compute_statistics_hexahedron_element_validity(meshgems_mesh_t *m);
/**
 * Gets the statistics of face element validity for hexahedra
 * @param[in]	m	: the mesh
 * @return error code
 */
MESHGEMS_METHOD meshgems_status_t meshgems_mesh_compute_statistics_hexahedron_face_element_validity(meshgems_mesh_t *m);
/**
 * Gets the statistics of shape factor for pyramids
 * @param[in]	m	: the mesh
 * @return error code
 */
MESHGEMS_METHOD meshgems_status_t meshgems_mesh_compute_statistics_pyramid_shape_factor(meshgems_mesh_t *m);
/**
 * Gets the statistics of element validity for pyramids
 * @param[in]	m	: the mesh
 * @return error code
 */
MESHGEMS_METHOD meshgems_status_t meshgems_mesh_compute_statistics_pyramid_element_validity(meshgems_mesh_t *m);
/**
 * Gets the statistics of face element validity for pyramids
 * @param[in]	m	: the mesh
 * @return error code
 */
MESHGEMS_METHOD meshgems_status_t meshgems_mesh_compute_statistics_pyramid_face_element_validity(meshgems_mesh_t *m);
/**
 * Gets the statistics of shape factor for prisms
 * @param[in]	m	: the mesh
 * @return error code
 */
MESHGEMS_METHOD meshgems_status_t meshgems_mesh_compute_statistics_prism_shape_factor(meshgems_mesh_t *m);
/**
 * Gets the statistics of element validity for prisms
 * @param[in]	m	: the mesh
 * @return error code
 */
MESHGEMS_METHOD meshgems_status_t meshgems_mesh_compute_statistics_prism_element_validity(meshgems_mesh_t *m);
/**
 * Gets the statistics of face element validity for prisms
 * @param[in]	m	: the mesh
 * @return error code
 */
MESHGEMS_METHOD meshgems_status_t meshgems_mesh_compute_statistics_prism_face_element_validity(meshgems_mesh_t *m);
/**
 * Gets the statistics of the length of edge for surface
 * @param[in]	m	: the mesh
 * @return error code
 */
MESHGEMS_METHOD meshgems_status_t meshgems_mesh_compute_statistics_surface_edge_length(meshgems_mesh_t *m);
/**
 * Gets the statistics of the length of edge for volume
 * @param[in]	m	: the mesh
 * @return error code
 */
MESHGEMS_METHOD meshgems_status_t meshgems_mesh_compute_statistics_volume_edge_length(meshgems_mesh_t *m);
/**
 * Gets the statistics of the length of edge for triangles
 * @param[in]	m	: the mesh
 * @return error code
 */
MESHGEMS_METHOD meshgems_status_t meshgems_mesh_compute_statistics_triangle_edge_length(meshgems_mesh_t *m);
/**
 * Gets the statistics of the length of edge for quadrangles
 * @param[in]	m	: the mesh
 * @return error code
 */
MESHGEMS_METHOD meshgems_status_t meshgems_mesh_compute_statistics_quadrangle_edge_length(meshgems_mesh_t *m);
/**
 * Gets the statistics of the length of edge for tetrahedra
 * @param[in]	m	: the mesh
 * @return error code
 */
MESHGEMS_METHOD meshgems_status_t meshgems_mesh_compute_statistics_tetrahedron_edge_length(meshgems_mesh_t *m);
/**
 * Gets the statistics of the length of edge for hexahedra
 * @param[in]	m	: the mesh
 * @return error code
 */
MESHGEMS_METHOD meshgems_status_t meshgems_mesh_compute_statistics_hexahedron_edge_length(meshgems_mesh_t *m);
/**
 * Gets the statistics of the length of edge for pyramids
 * @param[in]	m	: the mesh
 * @return error code
 */
MESHGEMS_METHOD meshgems_status_t meshgems_mesh_compute_statistics_pyramid_edge_length(meshgems_mesh_t *m);
/**
 * Gets the statistics of the length of edge for prisms
 * @param[in]	m	: the mesh
 * @return error code
 */
MESHGEMS_METHOD meshgems_status_t meshgems_mesh_compute_statistics_prism_edge_length(meshgems_mesh_t *m);
/**
 * Gets the statistics of equiangle skew for tetrahedra
 * @param[in]	m	: the mesh
 * @return error code
 */
MESHGEMS_METHOD meshgems_status_t meshgems_mesh_compute_statistics_tetrahedron_equiangle_skew(meshgems_mesh_t *m);
/**
 * Gets the statistics of equiangle skew for pyramid
 * @param[in]	m	: the mesh
 * @return error code
 */
MESHGEMS_METHOD meshgems_status_t meshgems_mesh_compute_statistics_pyramid_equiangle_skew(meshgems_mesh_t *m);
/**
 * Gets the statistics of equiangle skew for a prism
 * @param[in]	m	: the mesh
 * @return error code
 */
MESHGEMS_METHOD meshgems_status_t meshgems_mesh_compute_statistics_prism_equiangle_skew(meshgems_mesh_t *m);
/**
 * Gets the statistics of equiangle skew for an hexahedron
 * @param[in]	m	: the mesh
 * @return error code
 */
MESHGEMS_METHOD meshgems_status_t meshgems_mesh_compute_statistics_hexahedron_equiangle_skew(meshgems_mesh_t *m);
/**
 * Gets the statistics of planarity for pyramid
 * @param[in]	m	: the mesh
 * @return error code
 */
MESHGEMS_METHOD meshgems_status_t meshgems_mesh_compute_statistics_pyramid_planarity(meshgems_mesh_t *m);
/**
 * Gets the statistics of planarity for a prism
 * @param[in]	m	: the mesh
 * @return error code
 */
MESHGEMS_METHOD meshgems_status_t meshgems_mesh_compute_statistics_prism_planarity(meshgems_mesh_t *m);
/**
 * Gets the statistics of planarity for an hexahedron
 * @param[in]	m	: the mesh
 * @return error code
 */
MESHGEMS_METHOD meshgems_status_t meshgems_mesh_compute_statistics_hexahedron_planarity(meshgems_mesh_t *m);

/** Private part of the mesh interface. Only Distene products should use it. **/


MESHGEMS_METHOD meshgems_status_t meshgems_mesh_compute_statistics(meshgems_mesh_t *m);
MESHGEMS_METHOD meshgems_status_t meshgems_mesh_compute_statistics0(meshgems_mesh_t *m);
MESHGEMS_METHOD meshgems_status_t meshgems_mesh_compute_topological_statistics(meshgems_mesh_t *m);
MESHGEMS_METHOD meshgems_status_t meshgems_mesh_compute_topological_statistics_valid(meshgems_mesh_t *m, const char *prefix);
MESHGEMS_METHOD meshgems_status_t meshgems_mesh_compute_statistics_valid(meshgems_mesh_t *m, const char *prefix);

MESHGEMS_METHOD meshgems_status_t meshgems_mesh_compute_statistics_2D_element_ratio_valid(meshgems_mesh_t *m, const char *prefix);
MESHGEMS_METHOD meshgems_status_t meshgems_mesh_compute_statistics_element_count_valid(meshgems_mesh_t *m, const char *prefix);
MESHGEMS_METHOD meshgems_status_t meshgems_mesh_compute_statistics_triangle_inverse_aspect_ratio_valid(meshgems_mesh_t *m, const char *prefix);
MESHGEMS_METHOD meshgems_status_t meshgems_mesh_compute_statistics_triangle_aspect_ratio_valid(meshgems_mesh_t *m, const char *prefix);
MESHGEMS_METHOD meshgems_status_t meshgems_mesh_compute_statistics_triangle_angles_valid(meshgems_mesh_t *m, const char *prefix);
MESHGEMS_METHOD meshgems_status_t meshgems_mesh_compute_statistics_triangle_equiangle_skew_valid(meshgems_mesh_t *m, const char *prefix);
MESHGEMS_METHOD meshgems_status_t meshgems_mesh_compute_statistics_triangle_area_valid(meshgems_mesh_t *m, const char *prefix);
MESHGEMS_METHOD meshgems_status_t meshgems_mesh_compute_statistics_quadrangle_aspect_ratio_valid(meshgems_mesh_t *m, const char *prefix);
MESHGEMS_METHOD meshgems_status_t meshgems_mesh_compute_statistics_quadrangle_inverse_aspect_ratio_valid(meshgems_mesh_t *m, const char *prefix);
MESHGEMS_METHOD meshgems_status_t meshgems_mesh_compute_statistics_quadrangle_angles_valid(meshgems_mesh_t *m, const char *prefix);
MESHGEMS_METHOD meshgems_status_t meshgems_mesh_compute_statistics_quadrangle_equiangle_skew_valid(meshgems_mesh_t *m, const char *prefix);
MESHGEMS_METHOD meshgems_status_t meshgems_mesh_compute_statistics_quadrangle_planarity_valid(meshgems_mesh_t *m, const char *prefix);
MESHGEMS_METHOD meshgems_status_t meshgems_mesh_compute_statistics_tetrahedron_aspect_ratio_valid(meshgems_mesh_t *m, const char *prefix);
MESHGEMS_METHOD meshgems_status_t meshgems_mesh_compute_statistics_tetrahedron_inverse_aspect_ratio_valid(meshgems_mesh_t *m, const char *prefix);
MESHGEMS_METHOD meshgems_status_t meshgems_mesh_compute_statistics_tetrahedron_shape_factor_valid(meshgems_mesh_t *m, const char *prefix);
MESHGEMS_METHOD meshgems_status_t meshgems_mesh_compute_statistics_tetrahedron_element_validity_valid(meshgems_mesh_t *m, const char *prefix);
MESHGEMS_METHOD meshgems_status_t meshgems_mesh_compute_statistics_tetrahedron_face_element_validity_valid(meshgems_mesh_t *m, const char *prefix);
MESHGEMS_METHOD meshgems_status_t meshgems_mesh_compute_statistics_tetrahedron_angles_valid(meshgems_mesh_t *m, const char *prefix);
MESHGEMS_METHOD meshgems_status_t meshgems_mesh_compute_statistics_tetrahedron_equiangle_skew_valid(meshgems_mesh_t *m, const char *prefix);
MESHGEMS_METHOD meshgems_status_t meshgems_mesh_compute_statistics_tetrahedron_volume_valid(meshgems_mesh_t *m, const char *prefix);
MESHGEMS_METHOD meshgems_status_t meshgems_mesh_compute_statistics_hexahedron_aspect_ratio_valid(meshgems_mesh_t *m, const char *prefix);
MESHGEMS_METHOD meshgems_status_t meshgems_mesh_compute_statistics_hexahedron_inverse_aspect_ratio_valid(meshgems_mesh_t *m, const char *prefix);
MESHGEMS_METHOD meshgems_status_t meshgems_mesh_compute_statistics_hexahedron_shape_factor_valid(meshgems_mesh_t *m, const char *prefix);
MESHGEMS_METHOD meshgems_status_t meshgems_mesh_compute_statistics_hexahedron_element_validity_valid(meshgems_mesh_t *m, const char *prefix);
MESHGEMS_METHOD meshgems_status_t meshgems_mesh_compute_statistics_hexahedron_face_element_validity_valid(meshgems_mesh_t *m, const char *prefix);
MESHGEMS_METHOD meshgems_status_t meshgems_mesh_compute_statistics_pyramid_shape_factor_valid(meshgems_mesh_t *m, const char *prefix);
MESHGEMS_METHOD meshgems_status_t meshgems_mesh_compute_statistics_pyramid_element_validity_valid(meshgems_mesh_t *m, const char *prefix);
MESHGEMS_METHOD meshgems_status_t meshgems_mesh_compute_statistics_pyramid_face_element_validity_valid(meshgems_mesh_t *m, const char *prefix);
MESHGEMS_METHOD meshgems_status_t meshgems_mesh_compute_statistics_prism_shape_factor_valid(meshgems_mesh_t *m, const char *prefix);
MESHGEMS_METHOD meshgems_status_t meshgems_mesh_compute_statistics_prism_element_validity_valid(meshgems_mesh_t *m, const char *prefix);
MESHGEMS_METHOD meshgems_status_t meshgems_mesh_compute_statistics_prism_face_element_validity_valid(meshgems_mesh_t *m, const char *prefix);
MESHGEMS_METHOD meshgems_status_t meshgems_mesh_compute_statistics_surface_edge_length_valid(meshgems_mesh_t *m, const char *prefix);
MESHGEMS_METHOD meshgems_status_t meshgems_mesh_compute_statistics_volume_edge_length_valid(meshgems_mesh_t *m, const char *prefix);
MESHGEMS_METHOD meshgems_status_t meshgems_mesh_compute_statistics_triangle_edge_length_valid(meshgems_mesh_t *m, const char *prefix);
MESHGEMS_METHOD meshgems_status_t meshgems_mesh_compute_statistics_quadrangle_edge_length_valid(meshgems_mesh_t *m, const char *prefix);
MESHGEMS_METHOD meshgems_status_t meshgems_mesh_compute_statistics_tetrahedron_edge_length_valid(meshgems_mesh_t *m, const char *prefix);
MESHGEMS_METHOD meshgems_status_t meshgems_mesh_compute_statistics_hexahedron_edge_length_valid(meshgems_mesh_t *m, const char *prefix);
MESHGEMS_METHOD meshgems_status_t meshgems_mesh_compute_statistics_pyramid_edge_length_valid(meshgems_mesh_t *m, const char *prefix);
MESHGEMS_METHOD meshgems_status_t meshgems_mesh_compute_statistics_prism_edge_length_valid(meshgems_mesh_t *m, const char *prefix);

MESHGEMS_METHOD meshgems_status_t meshgems_mesh_compute_statistics_triangle_inverse_aspect_ratio_with_custom_histogram(meshgems_mesh_t *m, meshgems_integer nbthreshold, meshgems_real *threshold);
MESHGEMS_METHOD meshgems_status_t meshgems_mesh_compute_statistics_triangle_aspect_ratio_with_custom_histogram(meshgems_mesh_t *m,meshgems_integer nbthreshold, meshgems_real *threshold);
MESHGEMS_METHOD meshgems_status_t meshgems_mesh_compute_statistics_triangle_angles_with_custom_histogram(meshgems_mesh_t *m, meshgems_integer nbthreshold, meshgems_real *threshold);
MESHGEMS_METHOD meshgems_status_t meshgems_mesh_compute_statistics_triangle_area_with_custom_histogram(meshgems_mesh_t *m, meshgems_integer nbthreshold, meshgems_real *threshold);
MESHGEMS_METHOD meshgems_status_t meshgems_mesh_compute_statistics_quadrangle_aspect_ratio_with_custom_histogram(meshgems_mesh_t *m,meshgems_integer nbthreshold, meshgems_real *threshold);
MESHGEMS_METHOD meshgems_status_t meshgems_mesh_compute_statistics_quadrangle_inverse_aspect_ratio_with_custom_histogram(meshgems_mesh_t *m, meshgems_integer nbthreshold, meshgems_real *threshold);
MESHGEMS_METHOD meshgems_status_t meshgems_mesh_compute_statistics_quadrangle_angles_with_custom_histogram(meshgems_mesh_t *m,meshgems_integer nbthreshold, meshgems_real *threshold);
MESHGEMS_METHOD meshgems_status_t meshgems_mesh_compute_statistics_quadrangle_planarity_with_custom_histogram(meshgems_mesh_t *m, meshgems_integer nbthreshold, meshgems_real *threshold);
MESHGEMS_METHOD meshgems_status_t meshgems_mesh_compute_statistics_tetrahedron_aspect_ratio_with_custom_histogram(meshgems_mesh_t *m, meshgems_integer nbthreshold, meshgems_real *threshold);
MESHGEMS_METHOD meshgems_status_t meshgems_mesh_compute_statistics_tetrahedron_inverse_aspect_ratio_with_custom_histogram(meshgems_mesh_t *m, meshgems_integer nbthreshold, meshgems_real *threshold);
MESHGEMS_METHOD meshgems_status_t meshgems_mesh_compute_statistics_tetrahedron_shape_factor_with_custom_histogram(meshgems_mesh_t *m,meshgems_integer nbthreshold, meshgems_real *threshold);
MESHGEMS_METHOD meshgems_status_t meshgems_mesh_compute_statistics_tetrahedron_element_validity_with_custom_histogram(meshgems_mesh_t *m,meshgems_integer nbthreshold, meshgems_real *threshold);
MESHGEMS_METHOD meshgems_status_t meshgems_mesh_compute_statistics_tetrahedron_angles_with_custom_histogram(meshgems_mesh_t *m, meshgems_integer nbthreshold, meshgems_real *threshold);
MESHGEMS_METHOD meshgems_status_t meshgems_mesh_compute_statistics_tetrahedron_volume_with_custom_histogram(meshgems_mesh_t *m,meshgems_integer nbthreshold, meshgems_real *threshold);
MESHGEMS_METHOD meshgems_status_t meshgems_mesh_compute_statistics_hexahedron_aspect_ratio_with_custom_histogram(meshgems_mesh_t *m, meshgems_integer nbthreshold, meshgems_real *threshold);
MESHGEMS_METHOD meshgems_status_t meshgems_mesh_compute_statistics_hexahedron_inverse_aspect_ratio_with_custom_histogram(meshgems_mesh_t *m,meshgems_integer nbthreshold, meshgems_real *threshold);
MESHGEMS_METHOD meshgems_status_t meshgems_mesh_compute_statistics_hexahedron_shape_factor_with_custom_histogram(meshgems_mesh_t *m, meshgems_integer nbthreshold, meshgems_real *threshold);
MESHGEMS_METHOD meshgems_status_t meshgems_mesh_compute_statistics_hexahedron_element_validity_with_custom_histogram(meshgems_mesh_t *m,meshgems_integer nbthreshold, meshgems_real *threshold);
MESHGEMS_METHOD meshgems_status_t meshgems_mesh_compute_statistics_pyramid_shape_factor_with_custom_histogram(meshgems_mesh_t *m, meshgems_integer nbthreshold, meshgems_real *threshold);
MESHGEMS_METHOD meshgems_status_t meshgems_mesh_compute_statistics_pyramid_element_validity_with_custom_histogram(meshgems_mesh_t *m,meshgems_integer nbthreshold, meshgems_real *threshold);
MESHGEMS_METHOD meshgems_status_t meshgems_mesh_compute_statistics_prism_shape_factor_with_custom_histogram(meshgems_mesh_t *m, meshgems_integer nbthreshold, meshgems_real *threshold);
MESHGEMS_METHOD meshgems_status_t meshgems_mesh_compute_statistics_prism_element_validity_with_custom_histogram(meshgems_mesh_t *m,meshgems_integer nbthreshold, meshgems_real *threshold);
MESHGEMS_METHOD meshgems_status_t meshgems_mesh_compute_statistics_surface_edge_length_with_custom_histogram(meshgems_mesh_t *m,meshgems_integer nbthreshold, meshgems_real *threshold);
MESHGEMS_METHOD meshgems_status_t meshgems_mesh_compute_statistics_volume_edge_length_with_custom_histogram(meshgems_mesh_t *m, meshgems_integer nbthreshold, meshgems_real *threshold);
MESHGEMS_METHOD meshgems_status_t meshgems_mesh_compute_statistics_triangle_edge_length_with_custom_histogram(meshgems_mesh_t *m, meshgems_integer nbthreshold, meshgems_real *threshold);
MESHGEMS_METHOD meshgems_status_t meshgems_mesh_compute_statistics_quadrangle_edge_length_with_custom_histogram(meshgems_mesh_t *m,meshgems_integer nbthreshold, meshgems_real *threshold);
MESHGEMS_METHOD meshgems_status_t meshgems_mesh_compute_statistics_tetrahedron_edge_length_with_custom_histogram(meshgems_mesh_t *m,meshgems_integer nbthreshold, meshgems_real *threshold);
MESHGEMS_METHOD meshgems_status_t meshgems_mesh_compute_statistics_hexahedron_edge_length_with_custom_histogram(meshgems_mesh_t *m, meshgems_integer nbthreshold, meshgems_real *threshold);
MESHGEMS_METHOD meshgems_status_t meshgems_mesh_compute_statistics_pyramid_edge_length_with_custom_histogram(meshgems_mesh_t *m,meshgems_integer nbthreshold, meshgems_real *threshold);
MESHGEMS_METHOD meshgems_status_t meshgems_mesh_compute_statistics_prism_edge_length_with_custom_histogram(meshgems_mesh_t *m, meshgems_integer nbthreshold, meshgems_real *threshold);

MESHGEMS_METHOD meshgems_status_t meshgems_mesh_compute_statistics_triangle_inverse_aspect_ratio_with_custom_histogram_valid(meshgems_mesh_t *m, const char *prefix,meshgems_integer nbthreshold, meshgems_real *threshold);
MESHGEMS_METHOD meshgems_status_t meshgems_mesh_compute_statistics_triangle_aspect_ratio_with_custom_histogram_valid(meshgems_mesh_t *m, const char *prefix,meshgems_integer nbthreshold, meshgems_real *threshold);
MESHGEMS_METHOD meshgems_status_t meshgems_mesh_compute_statistics_triangle_angles_with_custom_histogram_valid(meshgems_mesh_t *m, const char *prefix,meshgems_integer nbthreshold, meshgems_real *threshold);
MESHGEMS_METHOD meshgems_status_t meshgems_mesh_compute_statistics_triangle_area_with_custom_histogram_valid(meshgems_mesh_t *m, const char *prefix,meshgems_integer nbthreshold, meshgems_real *threshold);
MESHGEMS_METHOD meshgems_status_t meshgems_mesh_compute_statistics_quadrangle_aspect_ratio_with_custom_histogram_valid(meshgems_mesh_t *m, const char *prefix,meshgems_integer nbthreshold, meshgems_real *threshold);
MESHGEMS_METHOD meshgems_status_t meshgems_mesh_compute_statistics_quadrangle_inverse_aspect_ratio_with_custom_histogram_valid(meshgems_mesh_t *m, const char *prefix,meshgems_integer nbthreshold, meshgems_real *threshold);
MESHGEMS_METHOD meshgems_status_t meshgems_mesh_compute_statistics_quadrangle_angles_with_custom_histogram_valid(meshgems_mesh_t *m, const char *prefix,meshgems_integer nbthreshold, meshgems_real *threshold);
MESHGEMS_METHOD meshgems_status_t meshgems_mesh_compute_statistics_quadrangle_planarity_with_custom_histogram_valid(meshgems_mesh_t *m, const char *prefix,meshgems_integer nbthreshold, meshgems_real *threshold);
MESHGEMS_METHOD meshgems_status_t meshgems_mesh_compute_statistics_tetrahedron_aspect_ratio_with_custom_histogram_valid(meshgems_mesh_t *m, const char *prefix,meshgems_integer nbthreshold, meshgems_real *threshold);
MESHGEMS_METHOD meshgems_status_t meshgems_mesh_compute_statistics_tetrahedron_inverse_aspect_ratio_with_custom_histogram_valid(meshgems_mesh_t *m, const char *prefix,meshgems_integer nbthreshold, meshgems_real *threshold);
MESHGEMS_METHOD meshgems_status_t meshgems_mesh_compute_statistics_tetrahedron_shape_factor_with_custom_histogram_valid(meshgems_mesh_t *m, const char *prefix,meshgems_integer nbthreshold, meshgems_real *threshold);
MESHGEMS_METHOD meshgems_status_t meshgems_mesh_compute_statistics_tetrahedron_element_validity_with_custom_histogram_valid(meshgems_mesh_t *m, const char *prefix,meshgems_integer nbthreshold, meshgems_real *threshold);
MESHGEMS_METHOD meshgems_status_t meshgems_mesh_compute_statistics_tetrahedron_angles_with_custom_histogram_valid(meshgems_mesh_t *m, const char *prefix,meshgems_integer nbthreshold, meshgems_real *threshold);
MESHGEMS_METHOD meshgems_status_t meshgems_mesh_compute_statistics_tetrahedron_volume_with_custom_histogram_valid(meshgems_mesh_t *m, const char *prefix,meshgems_integer nbthreshold, meshgems_real *threshold);
MESHGEMS_METHOD meshgems_status_t meshgems_mesh_compute_statistics_hexahedron_aspect_ratio_with_custom_histogram_valid(meshgems_mesh_t *m, const char *prefix,meshgems_integer nbthreshold, meshgems_real *threshold);
MESHGEMS_METHOD meshgems_status_t meshgems_mesh_compute_statistics_hexahedron_inverse_aspect_ratio_with_custom_histogram_valid(meshgems_mesh_t *m, const char *prefix,meshgems_integer nbthreshold, meshgems_real *threshold);
MESHGEMS_METHOD meshgems_status_t meshgems_mesh_compute_statistics_hexahedron_shape_factor_with_custom_histogram_valid(meshgems_mesh_t *m, const char *prefix,meshgems_integer nbthreshold, meshgems_real *threshold);
MESHGEMS_METHOD meshgems_status_t meshgems_mesh_compute_statistics_hexahedron_element_validity_with_custom_histogram_valid(meshgems_mesh_t *m, const char *prefix,meshgems_integer nbthreshold, meshgems_real *threshold);
MESHGEMS_METHOD meshgems_status_t meshgems_mesh_compute_statistics_pyramid_shape_factor_with_custom_histogram_valid(meshgems_mesh_t *m, const char *prefix,meshgems_integer nbthreshold, meshgems_real *threshold);
MESHGEMS_METHOD meshgems_status_t meshgems_mesh_compute_statistics_pyramid_element_validity_with_custom_histogram_valid(meshgems_mesh_t *m, const char *prefix,meshgems_integer nbthreshold, meshgems_real *threshold);
MESHGEMS_METHOD meshgems_status_t meshgems_mesh_compute_statistics_prism_shape_factor_with_custom_histogram_valid(meshgems_mesh_t *m, const char *prefix,meshgems_integer nbthreshold, meshgems_real *threshold);
MESHGEMS_METHOD meshgems_status_t meshgems_mesh_compute_statistics_prism_element_validity_with_custom_histogram_valid(meshgems_mesh_t *m, const char *prefix,meshgems_integer nbthreshold, meshgems_real *threshold);
MESHGEMS_METHOD meshgems_status_t meshgems_mesh_compute_statistics_surface_edge_length_with_custom_histogram_valid(meshgems_mesh_t *m, const char *prefix,meshgems_integer nbthreshold, meshgems_real *threshold);
MESHGEMS_METHOD meshgems_status_t meshgems_mesh_compute_statistics_volume_edge_length_with_custom_histogram_valid(meshgems_mesh_t *m, const char *prefix,meshgems_integer nbthreshold, meshgems_real *threshold);
MESHGEMS_METHOD meshgems_status_t meshgems_mesh_compute_statistics_triangle_edge_length_with_custom_histogram_valid(meshgems_mesh_t *m, const char *prefix,meshgems_integer nbthreshold, meshgems_real *threshold);
MESHGEMS_METHOD meshgems_status_t meshgems_mesh_compute_statistics_quadrangle_edge_length_with_custom_histogram_valid(meshgems_mesh_t *m, const char *prefix,meshgems_integer nbthreshold, meshgems_real *threshold);
MESHGEMS_METHOD meshgems_status_t meshgems_mesh_compute_statistics_tetrahedron_edge_length_with_custom_histogram_valid(meshgems_mesh_t *m, const char *prefix,meshgems_integer nbthreshold, meshgems_real *threshold);
MESHGEMS_METHOD meshgems_status_t meshgems_mesh_compute_statistics_hexahedron_edge_length_with_custom_histogram_valid(meshgems_mesh_t *m, const char *prefix,meshgems_integer nbthreshold, meshgems_real *threshold);
MESHGEMS_METHOD meshgems_status_t meshgems_mesh_compute_statistics_pyramid_edge_length_with_custom_histogram_valid(meshgems_mesh_t *m, const char *prefix,meshgems_integer nbthreshold, meshgems_real *threshold);
MESHGEMS_METHOD meshgems_status_t meshgems_mesh_compute_statistics_prism_edge_length_with_custom_histogram_valid(meshgems_mesh_t *m, const char *prefix,meshgems_integer nbthreshold, meshgems_real *threshold);
MESHGEMS_METHOD meshgems_status_t meshgems_mesh_get_edge_length(meshgems_mesh_t *m, meshgems_integer *vtx, meshgems_real *q);

#endif
