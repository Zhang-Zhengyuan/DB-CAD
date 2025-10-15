#ifndef MESHGEMS_CVM_H
#define MESHGEMS_CVM_H

#include <meshgems/mdecl.h>
#include <meshgems/basic_types.h>
#include <meshgems/status.h>
#include <meshgems/context.h>
#include <meshgems/interrupt.h>
#include <meshgems/message.h>
#include <meshgems/mesh.h>
#include <meshgems/sizemap.h>


struct meshgems_cvm_size_specification_t_;

/**
 * Opaque type to store size_specification data.
 */
typedef struct meshgems_cvm_size_specification_t_ meshgems_cvm_size_specification_t;

/**
 * Type for functions that define a size specification boundary layer distribution
 * @param layer (in): the layer you want to compute (layer > 0)
 * @param h (out): the size at the layer computed by the distribution
 * @param user_data (in): a generic user pointer
 * @return error code (STATUS_OK means that computation was correct).
 *
 */
typedef meshgems_status_t (*meshgems_cvm_size_specification_boundary_layer_distribution_t)(meshgems_integer layer,
        meshgems_real *h, void *user_data);

/**
 * Simple size specification constructor.
 * @param ctx (in) : the parent context.
 * @return a new size specification object or NULL.
 */
MESHGEMS_METHOD meshgems_cvm_size_specification_t *meshgems_cvm_size_specification_new(
    meshgems_context_t *ctx);

/**
 * Size specification destructor.
 */
MESHGEMS_METHOD void meshgems_cvm_size_specification_delete(
        meshgems_cvm_size_specification_t *s);

/**
  * Add a new null boundary layer constraint on a given mesh face.
  * Force this face to not create boundary layers.
  * @param s (in) : the size specification object.
  * @param face_type (in) : integer containing the face type.
  * @param face_idx (in) : integer containing the face index.
  * @param face_orientation (in) : integer containing the face orientation. Value can be
  *                                    MESH_ORIENTATION_FORWARD or MESH_ORIENTATION_REVERSE.
  * @return error code.
  */
MESHGEMS_METHOD meshgems_status_t meshgems_cvm_size_specification_add_boundary_layer_null_constraint_on_face(
        meshgems_cvm_size_specification_t *s, meshgems_integer face_type,
        meshgems_integer face_idx, meshgems_integer face_orientation);

/**
  * Add a new arithmetic boundary layer constraint on a given mesh face.
  * @param s (in) : the size specification object.
  * @param h0 (in) : the height of the first layer in the boundary layer.
  * @param difference (in) : the common difference between two consecutive layers, only for layers between 1 and n_min.
  * @param blending (in) : the common difference between two consecutive layers, only for layers between n_min and n_max.
  * @param n_min (in) : the minimum number of desired boundary layers.
  * @param n_max (in) : the maximum number of desired boundary layers or 0 for an automatic selection.
  * @param relative (in) : 1 if the size is relative 0 if the size is constant.
  * @param face_type (in) : integer containing the face type.
  * @param face_idx (in) : integer containing the face index.
  * @param face_orientation (in) : integer containing the face orientation. Value can be
  *                                    MESH_ORIENTATION_FORWARD or MESH_ORIENTATION_REVERSE.
  * @return error code.
  */
MESHGEMS_METHOD meshgems_status_t meshgems_cvm_size_specification_add_boundary_layer_arithmetic_constraint_on_face(
        meshgems_cvm_size_specification_t *s, meshgems_real h0, meshgems_real difference,
        meshgems_real blending, meshgems_integer n_min, meshgems_integer n_max, meshgems_integer relative,
        meshgems_integer face_type, meshgems_integer face_idx, meshgems_integer face_orientation);

/**
  * Add a new geometric boundary layer constraint on a given mesh face.
  * @param s (in) : the size specification object.
  * @param h0 (in) : the height of the first layer in the boundary layer.
  * @param progression (in) : the progression coefficient between two consecutive layers, only for layers between 1 and n_min.
  * @param blending (in) : the blending coefficient between two consecutive layers, only for layers between n_min and n_max.
  * @param n_min (in) : the minimum number of desired boundary layers.
  * @param n_max (in) : the maximum number of desired boundary layers or 0 for an automatic selection.
  * @param relative (in) : 1 if the size is relative 0 if the size is constant.
  * @param face_type (in) : integer containing the face type.
  * @param face_idx (in) : integer containing the face index.
  * @param face_orientation (in) : integer containing the face orientation. Value can be
  *                                    MESH_ORIENTATION_FORWARD or MESH_ORIENTATION_REVERSE.
  * @return error code.
  */
MESHGEMS_METHOD meshgems_status_t meshgems_cvm_size_specification_add_boundary_layer_geometric_constraint_on_face(
        meshgems_cvm_size_specification_t *s, meshgems_real h0, meshgems_real progression,
        meshgems_real blending, meshgems_integer n_min, meshgems_integer n_max, meshgems_integer relative,
        meshgems_integer face_type, meshgems_integer face_idx, meshgems_integer face_orientation);

/**
  * Add a new uniform boundary layer constraint on a given mesh face.
  * @param s (in) : the size specification object.
  * @param h (in) : the height of the first layer in the boundary layer.
  * @param n (in) : the minimum number of desired layers in the boundary.
  * @param relative (in) : 1 if the size is relative 0 if the size is constant.
  * @param face_type (in) : integer containing the face type of the face.
  * @param face_idx (in) : integer containing the face index of the face.
  * @param face_orientation (in) : integer containing the face orientation. Value can be
  *                                    MESH_ORIENTATION_FORWARD or MESH_ORIENTATION_REVERSE.
  * @return error code.
  */
MESHGEMS_METHOD meshgems_status_t meshgems_cvm_size_specification_add_boundary_layer_uniform_constraint_on_face(
        meshgems_cvm_size_specification_t *s, meshgems_real h, meshgems_integer n,
        meshgems_integer relative, meshgems_integer face_type,
        meshgems_integer face_idx, meshgems_integer face_orientation);

/**
  * Add a new distribution boundary layer constraint on a given mesh face.
  * @param s (in) : the size specification object.
  * @param n_min (in) : the minimum number of desired boundary layers.
  * @param n_max (in) : the maximum number of desired boundary layers or 0 for an automatic selection.
  * @param distribution (in) : the distribution callback.
  * @param user_data (in) : a generic user pointer.
  * @param relative (in) : 1 if the size is relative 0 if the size is constant.
  * @param face_type (in) : integer containing the face type of the face.
  * @param face_idx (in) : integer containing the face index of the face.
  * @param face_orientation (in) : integer containing the face orientation of the face. Value can be
  *                                    MESH_ORIENTATION_FORWARD or MESH_ORIENTATION_REVERSE.
  * @return error code.
  */
MESHGEMS_METHOD meshgems_status_t meshgems_cvm_size_specification_add_boundary_layer_distribution_constraint_on_face(
        meshgems_cvm_size_specification_t *s, meshgems_integer n_min, meshgems_integer n_max,
        meshgems_cvm_size_specification_boundary_layer_distribution_t distribution, void *user_data,
        meshgems_integer relative, meshgems_integer face_type,
        meshgems_integer face_idx, meshgems_integer face_orientation);

/**
  * Add a new null boundary layer constraint on a given mesh element tag.
  * Force this tag to not create boundary layers.
  * @param s (in) : the working size specification object.
  * @param tag (in) : the mesh element tag on which to create this boundary layer.
  * @return error code.
  */
MESHGEMS_METHOD meshgems_status_t meshgems_cvm_size_specification_add_boundary_layer_null_constraint_on_face_tag(
        meshgems_cvm_size_specification_t *s, meshgems_integer tag);

/**
  * Add a new arithmetic boundary layer constraint on a given mesh element tag.
  * @param s (in) : the size specification object.
  * @param h0 (in) : the height of the first layer in the boundary layer.
  * @param difference (in) : the common difference between two consecutive layers, only for layers between 1 and n_min.
  * @param blending (in) : the common difference between two consecutive layers, only for layers between n_min and n_max.
  * @param n_min (in) : the minimum number of desired boundary layers.
  * @param n_max (in) : the maximum number of desired boundary layers or 0 for an automatic selection.
  * @param relative (in) : 1 if the size is relative 0 if the size is constant.
  * @param tag (in) : the mesh element tag on which to create this boundary layer.
  * @return error code.
  */
MESHGEMS_METHOD meshgems_status_t meshgems_cvm_size_specification_add_boundary_layer_arithmetic_constraint_on_face_tag(
        meshgems_cvm_size_specification_t *s, meshgems_real h0,
        meshgems_real difference, meshgems_real blending, meshgems_integer n_min,
        meshgems_integer n_max, meshgems_integer relative, meshgems_integer tag);

/**
  * Add a new geometric boundary layer constraint on a given mesh element tag.
  * @param s (in) : the size specification object.
  * @param h0 (in) : the height of the first layer in the boundary layer.
  * @param progression (in) : the progression coefficient between two consecutive layers, only for layers between 1 and n_min.
  * @param blending (in) : the blending coefficient between two consecutive layers, only for layers between n_min and n_max.
  * @param n_min (in) : the minimum number of desired boundary layers.
  * @param n_max (in) : the maximum number of desired boundary layers or 0 for an automatic selection.
  * @param relative (in) : 1 if the size is relative 0 if the size is constant.
  * @param tag (in) : the mesh element tag on which to create this boundary layer.
  * @return error code.
  */
MESHGEMS_METHOD meshgems_status_t meshgems_cvm_size_specification_add_boundary_layer_geometric_constraint_on_face_tag(
        meshgems_cvm_size_specification_t *s, meshgems_real h0,
        meshgems_real progression, meshgems_real blending, meshgems_integer n_min,
        meshgems_integer n_max, meshgems_integer relative, meshgems_integer tag);

/**
  * Add a new uniform boundary layer constraint on a given mesh element tag.
  * @param s (in) : the size specification object.
  * @param h (in) : the height of the first layer in the boundary layer.
  * @param n (in) : the number of desired layers in the boundary.
  * @param relative (in) : 1 if the size is relative 0 if the size is constant.
  * @param tag (in) : the mesh element tag on which to create this boundary layer.
  * @return error code.
  */
MESHGEMS_METHOD meshgems_status_t meshgems_cvm_size_specification_add_boundary_layer_uniform_constraint_on_face_tag(
        meshgems_cvm_size_specification_t *s, meshgems_real h, meshgems_integer n,
        meshgems_integer relative, meshgems_integer tag);

/**
  * Add a new distribution boundary layer constraint on a given mesh element tag.
  * @param s (in) : the size specification object.
  * @param n_min (in) : the minimum number of desired boundary layers.
  * @param n_max (in) : the maximum number of desired boundary layers or 0 for an automatic selection.
  * @param distribution (in) : the distribution callback.
  * @param user_data (in) : a generic user pointer.
  * @param relative (in) : 1 if the size is relative 0 if the size is constant.
  * @param tag (in) : the mesh element tag on which to create this boundary layer.
  * @return error code.
  */
MESHGEMS_METHOD meshgems_status_t meshgems_cvm_size_specification_add_boundary_layer_distribution_constraint_on_face_tag(
        meshgems_cvm_size_specification_t *s, meshgems_integer n_min, meshgems_integer n_max,
        meshgems_cvm_size_specification_boundary_layer_distribution_t distribution, void *user_data,
        meshgems_integer relative, meshgems_integer tag);

/**
  * Add a new null boundary layer constraint on a given mesh subdomain.
  * Force this subdomain to not create boundary layers.
  * @param s (in) : the size specification object.
  * @param seed_type (in) : integer containing the subdomain seed element type of the subdomain.
  * @param seed_idx (in) : integer containing the subdomain seed element index of the subdomain.
  * @param seed_orientation (in) : integer containing the subdomain seed element orientation of the subdomain
  *                                    (normal is assumed to point toward the inside of the subdomain). Value can be
  *                                    MESH_ORIENTATION_FORWARD or MESH_ORIENTATION_REVERSE.
  * @return error code.
  */
MESHGEMS_METHOD meshgems_status_t meshgems_cvm_size_specification_add_boundary_layer_null_constraint_on_subdomain(
        meshgems_cvm_size_specification_t *s, meshgems_integer seed_type,
        meshgems_integer seed_idx, meshgems_integer seed_orientation);

/**
  * Add a new arithmetic boundary layer constraint on a given mesh subdomain.
  * @param s (in) : the size specification object.
  * @param h0 (in) : the height of the first layer in the boundary layer.
  * @param difference (in) : the common difference between two consecutive layers, only for layers between 1 and n_min.
  * @param blending (in) : the common difference between two consecutive layers, only for layers between n_min and n_max.
  * @param n_min (in) : the minimum number of desired boundary layers.
  * @param n_max (in) : the maximum number of desired boundary layers or 0 for an automatic selection.
  * @param relative (in) : 1 if the size is relative 0 if the size is constant.
  * @param seed_type (in) : integer containing the subdomain seed element type of the subdomain.
  * @param seed_idx (in) : integer containing the subdomain seed element index of the subdomain.
  * @param seed_orientation (in) : integer containing the subdomain seed element orientation of the subdomain
  *                                    (normal is assumed to point toward the inside of the subdomain). Value can be
  *                                    MESH_ORIENTATION_FORWARD or MESH_ORIENTATION_REVERSE.
  * @return error code.
  */
MESHGEMS_METHOD meshgems_status_t meshgems_cvm_size_specification_add_boundary_layer_arithmetic_constraint_on_subdomain(
        meshgems_cvm_size_specification_t *s, meshgems_real h0, meshgems_real difference,
        meshgems_real blending, meshgems_integer n_min, meshgems_integer n_max, meshgems_integer relative,
        meshgems_integer seed_type, meshgems_integer seed_idx, meshgems_integer seed_orientation);

/**
  * Add a new geometric boundary layer constraint on a given mesh subdomain.
  * @param s (in) : the size specification object.
  * @param h0 (in) : the height of the first layer in the boundary layer.
  * @param progression (in) : the progression coefficient between two consecutive layers, only for layers between 1 and n_min.
  * @param blending (in) : the blending coefficient between two consecutive layers, only for layers between n_min and n_max.
  * @param n_min (in) : the minimum number of desired boundary layers.
  * @param n_max (in) : the maximum number of desired boundary layers or 0 for an automatic selection.
  * @param relative (in) : 1 if the size is relative 0 if the size is constant.
  * @param seed_type (in) : integer containing the subdomain seed element type of the subdomain.
  * @param seed_idx (in) : integer containing the subdomain seed element index of the subdomain.
  * @param seed_orientation (in) : integer containing the subdomain seed element orientation of the subdomain
  *                                    (normal is assumed to point toward the inside of the subdomain). Value can be
  *                                    MESH_ORIENTATION_FORWARD or MESH_ORIENTATION_REVERSE.
  * @return error code.
  */
MESHGEMS_METHOD meshgems_status_t meshgems_cvm_size_specification_add_boundary_layer_geometric_constraint_on_subdomain(
        meshgems_cvm_size_specification_t *s, meshgems_real h0, meshgems_real progression,
        meshgems_real blending, meshgems_integer n_min, meshgems_integer n_max, meshgems_integer relative,
        meshgems_integer seed_type, meshgems_integer seed_idx, meshgems_integer seed_orientation);

/**
  * Add a new uniform boundary layer constraint on a given mesh subdomain.
  * @param s (in) : the size specification object.
  * @param h (in) : the height of the first layer in the boundary layer.
  * @param n (in) : the minimum number of desired layers in the boundary.
  * @param relative (in) : 1 if the size is relative 0 if the size is constant.
  * @param seed_type (in) : integer containing the subdomain seed element type of the subdomain.
  * @param seed_idx (in) : integer containing the subdomain seed element index of the subdomain.
  * @param seed_orientation (in) : integer containing the subdomain seed element orientation of the subdomain
  *                                    (normal is assumed to point toward the inside of the subdomain). Value can be
  *                                    MESH_ORIENTATION_FORWARD or MESH_ORIENTATION_REVERSE.
  * @return error code.
  */
MESHGEMS_METHOD meshgems_status_t meshgems_cvm_size_specification_add_boundary_layer_uniform_constraint_on_subdomain(
        meshgems_cvm_size_specification_t *s, meshgems_real h, meshgems_integer n,
        meshgems_integer relative, meshgems_integer seed_type,
        meshgems_integer seed_idx, meshgems_integer seed_orientation);

/**
  * Add a new distribution boundary layer constraint on a given mesh subdomain.
  * @param s (in) : the size specification object.
  * @param n_min (in) : the minimum number of desired boundary layers.
  * @param n_max (in) : the maximum number of desired boundary layers or 0 for an automatic selection.
  * @param distribution (in) : the distribution callback.
  * @param user_data (in) : a generic user pointer.
  * @param relative (in) : 1 if the size is relative 0 if the size is constant.
  * @param seed_type (in) : integer containing the subdomain seed element type of the subdomain.
  * @param seed_idx (in) : integer containing the subdomain seed element index of the subdomain.
  * @param seed_orientation (in) : integer containing the subdomain seed element orientation of the subdomain
  *                                    (normal is assumed to point toward the inside of the subdomain). Value can be
  *                                    MESH_ORIENTATION_FORWARD or MESH_ORIENTATION_REVERSE.
  * @return error code.
  */
MESHGEMS_METHOD meshgems_status_t meshgems_cvm_size_specification_add_boundary_layer_distribution_constraint_on_subdomain(
        meshgems_cvm_size_specification_t *s, meshgems_integer n_min, meshgems_integer n_max,
        meshgems_cvm_size_specification_boundary_layer_distribution_t distribution, void *user_data,
        meshgems_integer relative, meshgems_integer seed_type,
        meshgems_integer seed_idx, meshgems_integer seed_orientation);

/**
  * Add a new null boundary layer constraint on a given mesh surface tag subdomain.
  * Force this subdomain to not create boundary layers.
  * @param s (in) : the size specification object.
  * @param tag (in) : the mesh element tag on which to create this boundary layer.
  * @param seed_type (in) : integer containing the subdomain seed element type of the subdomain.
  * @param seed_idx (in) : integer containing the subdomain seed element index of the subdomain.
  * @param seed_orientation (in) : integer containing the subdomain seed element orientation of the subdomain
  *                                    (normal is assumed to point toward the inside of the subdomain). Value can be
  *                                    MESH_ORIENTATION_FORWARD or MESH_ORIENTATION_REVERSE.
  * @return error code.
  */
MESHGEMS_METHOD meshgems_status_t meshgems_cvm_size_specification_add_boundary_layer_null_constraint_on_face_tag_in_subdomain(
        meshgems_cvm_size_specification_t *s, meshgems_integer tag, meshgems_integer seed_type,
        meshgems_integer seed_idx, meshgems_integer seed_orientation);

/**
  * Add a new arithmetic boundary layer constraint on a given mesh surface tag subdomain.
  * @param s (in) : the size specification object.
  * @param h0 (in) : the height of the first layer in the boundary layer.
  * @param difference (in) : the common difference between two consecutive layers, only for layers between 1 and n_min.
  * @param blending (in) : the common difference between two consecutive layers, only for layers between n_min and n_max.
  * @param n_min (in) : the minimum number of desired boundary layers.
  * @param n_max (in) : the maximum number of desired boundary layers or 0 for an automatic selection.
  * @param relative (in) : 1 if the size is relative 0 if the size is constant.
  * @param tag (in) : the mesh element tag on which to create this boundary layer.
  * @param seed_type (in) : integer containing the subdomain seed element type of the subdomain.
  * @param seed_idx (in) : integer containing the subdomain seed element index of the subdomain.
  * @param seed_orientation (in) : integer containing the subdomain seed element orientation of the subdomain
  *                                    (normal is assumed to point toward the inside of the subdomain). Value can be
  *                                    MESH_ORIENTATION_FORWARD or MESH_ORIENTATION_REVERSE.
  * @return error code.
  */
MESHGEMS_METHOD meshgems_status_t meshgems_cvm_size_specification_add_boundary_layer_arithmetic_constraint_on_face_tag_in_subdomain(
        meshgems_cvm_size_specification_t *s, meshgems_real h0, meshgems_real difference,
        meshgems_real blending, meshgems_integer n_min, meshgems_integer n_max, meshgems_integer relative,
        meshgems_integer tag, meshgems_integer seed_type, meshgems_integer seed_idx, meshgems_integer seed_orientation);

/**
  * Add a new geometric boundary layer constraint on a given mesh surface tag subdomain.
  * @param s (in) : the size specification object.
  * @param h0 (in) : the height of the first layer in the boundary layer.
  * @param progression (in) : the progression coefficient between two consecutive layers, only for layers between 1 and n_min.
  * @param blending (in) : the blending coefficient between two consecutive layers, only for layers between n_min and n_max.
  * @param n_min (in) : the minimum number of desired boundary layers.
  * @param n_max (in) : the maximum number of desired boundary layers or 0 for an automatic selection.
  * @param relative (in) : 1 if the size is relative 0 if the size is constant.
  * @param tag (in) : the mesh element tag on which to create this boundary layer.
  * @param seed_type (in) : integer containing the subdomain seed element type of the subdomain.
  * @param seed_idx (in) : integer containing the subdomain seed element index of the subdomain.
  * @param seed_orientation (in) : integer containing the subdomain seed element orientation of the subdomain
  *                                    (normal is assumed to point toward the inside of the subdomain). Value can be
  *                                    MESH_ORIENTATION_FORWARD or MESH_ORIENTATION_REVERSE.
  * @return error code.
  */
MESHGEMS_METHOD meshgems_status_t meshgems_cvm_size_specification_add_boundary_layer_geometric_constraint_on_face_tag_in_subdomain(
        meshgems_cvm_size_specification_t *s, meshgems_real h0, meshgems_real progression,
        meshgems_real blending, meshgems_integer n_min, meshgems_integer n_max, meshgems_integer relative,
        meshgems_integer tag, meshgems_integer seed_type, meshgems_integer seed_idx, meshgems_integer seed_orientation);

/**
  * Add a new uniform boundary layer constraint on a given mesh surface tag subdomain.
  * @param s (in) : the size specification object.
  * @param h (in) : the height of the first layer in the boundary layer.
  * @param n (in) : the minimum number of desired layers in the boundary.
  * @param relative (in) : 1 if the size is relative 0 if the size is constant.
  * @param tag (in) : the mesh element tag on which to create this boundary layer.
  * @param seed_type (in) : integer containing the subdomain seed element type of the subdomain.
  * @param seed_idx (in) : integer containing the subdomain seed element index of the subdomain.
  * @param seed_orientation (in) : integer containing the subdomain seed element orientation of the subdomain
  *                                    (normal is assumed to point toward the inside of the subdomain). Value can be
  *                                    MESH_ORIENTATION_FORWARD or MESH_ORIENTATION_REVERSE.
  * @return error code.
  */
MESHGEMS_METHOD meshgems_status_t meshgems_cvm_size_specification_add_boundary_layer_uniform_constraint_on_face_tag_in_subdomain(
        meshgems_cvm_size_specification_t *s, meshgems_real h, meshgems_integer n,
        meshgems_integer relative, meshgems_integer tag, meshgems_integer seed_type,
        meshgems_integer seed_idx, meshgems_integer seed_orientation);

/**
  * Add a new distribution boundary layer constraint on a given mesh surface tag subdomain.
  * @param s (in) : the size specification object.
  * @param n_min (in) : the minimum number of desired boundary layers.
  * @param n_max (in) : the maximum number of desired boundary layers or 0 for an automatic selection.
  * @param distribution (in) : the distribution callback.
  * @param user_data (in) : a generic user pointer.
  * @param relative (in) : 1 if the size is relative 0 if the size is constant.
  * @param tag (in) : the mesh element tag on which to create this boundary layer.
  * @param seed_type (in) : integer containing the subdomain seed element type of the subdomain.
  * @param seed_idx (in) : integer containing the subdomain seed element index of the subdomain.
  * @param seed_orientation (in) : integer containing the subdomain seed element orientation of the subdomain
  *                                    (normal is assumed to point toward the inside of the subdomain). Value can be
  *                                    MESH_ORIENTATION_FORWARD or MESH_ORIENTATION_REVERSE.
  * @return error code.
  */
MESHGEMS_METHOD meshgems_status_t meshgems_cvm_size_specification_add_boundary_layer_distribution_constraint_on_face_tag_in_subdomain(
        meshgems_cvm_size_specification_t *s, meshgems_integer n_min, meshgems_integer n_max,
        meshgems_cvm_size_specification_boundary_layer_distribution_t distribution, void *user_data,
        meshgems_integer relative, meshgems_integer tag, meshgems_integer seed_type,
        meshgems_integer seed_idx, meshgems_integer seed_orientation);

/**
  * Add a growth constraint on a given mesh subdomain.
  * @param s (in) : the size specification object.
  * @param gradation (in) : the gradation (growth rate) of cell size in the subdomain
  * @param hmin (in) : the minimum cell size in the subdomain
  * @param hmax (in) : the maximum cell size in the subdomain
  * @param seed_type (in) : integer containing the subdomain seed element type of the subdomain.
  * @param seed_idx (in) : integer containing the subdomain seed element index of the subdomain.
  * @param seed_orientation (in) : integer containing the subdomain seed element orientation of the subdomain
  *                                    (normal is assumed to point toward the inside of the subdomain). Value can be
  *                                    MESH_ORIENTATION_FORWARD or MESH_ORIENTATION_REVERSE.
  * @return error code.
  */
MESHGEMS_METHOD meshgems_status_t
meshgems_cvm_size_specification_add_growth_constraint_on_subdomain(
    meshgems_cvm_size_specification_t *s,
    meshgems_real gradation,
    meshgems_real hmin,
    meshgems_real hmax,
    meshgems_integer seed_type,
    meshgems_integer seed_idx,
    meshgems_integer seed_orientation);

struct meshgems_cvm_session_t_;

/**
 * Opaque type to store session data.
 */
typedef struct meshgems_cvm_session_t_ meshgems_cvm_session_t;

/**
 * Simple constructor.
 *
 * @param[in] ctx : the context this session is attached to.
 *
 * @retval a new meshgems_cvm_session_t on success, NULL on failure.
 */
MESHGEMS_METHOD meshgems_cvm_session_t *meshgems_cvm_session_new(
    meshgems_context_t *ctx);

/**
 * Destructor.
 *
 * @param[in] cvms : the cvm session (is freed and \b unusable afterwards).
 */
MESHGEMS_METHOD void meshgems_cvm_session_delete(
    meshgems_cvm_session_t *cvms);

/**
 * Sets the message callback function.
 *
 * This is where the user should detect and print phase changes,
 * timings, intersections, ... \n May be followed by a call to the
 * interrupt callback in particular in case of warning or error.
 *
 * @param[in] cvms : the cvm session.
 * @param[in] cb  : the message callback (or NULL to remove any previous callback and use the callback from the context)
 * @param[in] user_data : the user pointer which will be given to cb as a parameter
 *
 * @retval STATUS_OK in case of success
 */
MESHGEMS_METHOD meshgems_status_t meshgems_cvm_session_set_message_callback(
    meshgems_cvm_session_t *cvms, meshgems_message_callback_t cb,
    void *user_data);

/**
 * Sets the interrupt callback function.
 *
 * This is where the user indicates if he wants the process to cleanly
 * end the ongoing session call.  This is where the user may detect
 * and print timings, ...  \n May follow a call to the message
 * callback in particular in case of warning or error.
 *
 * @param[in] cvms : the cvm session.
 * @param[in] cb  : the interrupt callback (or NULL to remove any previous callback and use the callback from the context)
 * @param[in] user_data : the user pointer which will be given to cb as a parameter
 *
 * @retval STATUS_OK in case of success
 */
MESHGEMS_METHOD meshgems_status_t meshgems_cvm_session_set_interrupt_callback(
    meshgems_cvm_session_t *cvms, meshgems_interrupt_callback_t cb,
    void *user_data);


/**
 * Type for a user callback function for computing the quality of a tetrahedron, given the
 * coordinates of its vertices and a metric.
 *
 * For an oriented tetrahedron given by the coordinates of its vertices xyz1, xyz2,
 * xyz3 and xyz4, this function returns its quality in the metric given by m. The resulting
 * quality is as a real number between 0. (worst quality) and 1. (perfect quality)
 *
 * @param[in] xyz1 : a real[3] array containing the coordinates of the 1st tetrahedron vertex
 * @param[in] xyz2 : a real[3] array containing the coordinates of the 2nd tetrahedron vertex
 * @param[in] xyz3 : a real[3] array containing the coordinates of the 3rdt tetrahedron vertex
 * @param[in] xyz4 : a real[3] array containing the coordinates of the 4th tetrahedron vertex
 * @param[in] metric : a real[6] array containing the metric to be considered. If NULL, there is no metric to
 *   consider (we are in an isotropic situation)
 * @param[in] volume : a real containing the (already known) volume of the tetrahedron in the metric
 * @param[in] user_data : a user pointer.
 *
 * @retval the computed quality between 0. (worst quality) and 1. (perfect quality).
 *
 */
typedef meshgems_real (*meshgems_cvm_tetrahedron_quality_callback_t)(
    meshgems_real xyz1[3], meshgems_real xyz2[3],
    meshgems_real xyz3[3], meshgems_real xyz4[3],
    meshgems_real metric[6], meshgems_real volume,
    void *user_data);

/**
 * Sets a user callback function for computing the quality of a tetrahedron, given the
 * coordinates of its vertices and a metric.
 *
 * @param[in] cvms : the cvm session.
 * @param[in] cb  : the quality callback (or NULL to remove any previous callback)
 * @param[in] user_data : the user pointer which will be given to cb as a parameter
 *
 * @retval STATUS_OK in case of success
 */
MESHGEMS_METHOD meshgems_status_t meshgems_cvm_session_set_tetrahedron_quality_callback(
    meshgems_cvm_session_t *cvms, meshgems_cvm_tetrahedron_quality_callback_t cb,
    void *user_data);

/**
 * Sets the sizemap function.
 *
 * @param[in] cvms : the cvm session.
 * @param[in] s  : the sizemap. Accepted types are meshgems_sizemap_type_iso_mesh_vertex,
 * meshgems_sizemap_type_aniso_mesh_vertex, meshgems_sizemap_type_iso_3d
 * and meshgems_sizemap_type_aniso_3d. The sizemap function must be thread safe.
 *
 * @retval STATUS_OK in case of success
 */
MESHGEMS_METHOD meshgems_status_t meshgems_cvm_session_set_sizemap(
    meshgems_cvm_session_t *cvms, meshgems_sizemap_t *s);

/**
 * Sets the size_specification structure.
 *
 * @param[in] cvms : the cvm session.
 * @param[in] s  : the size_specifiction.
 *
 * @retval STATUS_OK in case of success
 */
MESHGEMS_METHOD meshgems_status_t meshgems_cvm_session_set_size_specification(
    meshgems_cvm_session_t *cvms, meshgems_cvm_size_specification_t *s);

/**
 * Sets the input mesh.
 *
 * @param[in] cvms : the cvm session.
 * @param[in] msh  : the input mesh structure, describing either a surface mesh
 *   (made of vertices, edges and triangles) or a volume mesh (made of vertices,
 *   edges, triangles and tetrahedra).
 *
 * @retval STATUS_OK in case of success
 */
MESHGEMS_METHOD meshgems_status_t meshgems_cvm_session_set_input_mesh(
    meshgems_cvm_session_t *cvms, meshgems_mesh_t *msh);

/**
 * Sets the input mesh.
 *
 * @param[in] cvms : the cvm session.
 * @param[in] msh  : the input mesh structure.
 *
 * @retval STATUS_OK in case of success
 */
MESHGEMS_METHOD meshgems_status_t meshgems_cvm_session_set_param(
    meshgems_cvm_session_t *cvms, const char *param_name,
    const char *param_value);

/**
 *  Generates a volume mesh from the surface input mesh.
 *
 * @param[in] cvms : the cvm session.
 *
 * @retval STATUS_OK in case of success
 */
MESHGEMS_METHOD meshgems_status_t meshgems_cvm_session_compute_mesh(
    meshgems_cvm_session_t *cvms);

/**
 *  Gets the resulting volume.
 *
 * @param[in] cvms : the cvm session.
 * @param[out] msh  : the output mesh structure.
 *
 * @retval STATUS_OK in case of success
 */
MESHGEMS_METHOD meshgems_status_t meshgems_cvm_session_get_mesh(
    meshgems_cvm_session_t *cvms, meshgems_mesh_t **msh);

/**
 *  Gets the surface of the resulting volume.
 *
 * @param[in] cvms : the cvm session.
 * @param[out] msh  : the output surface mesh structure.
 *
 * @retval STATUS_OK in case of success
 */
MESHGEMS_METHOD meshgems_status_t meshgems_cvm_session_get_surface_mesh(
    meshgems_cvm_session_t *cvms, meshgems_mesh_t **msh);

/**
 *  Gives back the resulting volume mesh to the cvm session structure.
 *
 * @param[in] cvms : the cvm session.
 * @param[in] msh : the output mesh structure.
 *
 * @retval STATUS_OK in case of success
 */
MESHGEMS_METHOD meshgems_status_t meshgems_cvm_session_regain_mesh(
    meshgems_cvm_session_t *cvms, meshgems_mesh_t *msh);

/**
 *  Gives back the surface of the resulting volume mesh to the cvm session structure.
 *
 * @param[in] cvms : the cvm session.
 * @param[in] msh : the output surface mesh structure.
 *
 * @retval STATUS_OK in case of success
 */
MESHGEMS_METHOD meshgems_status_t meshgems_cvm_session_regain_surface_mesh(
    meshgems_cvm_session_t *cvms, meshgems_mesh_t *msh);

/**
 *  Gets the resulting sizemap at mesh vertices.
 *
 * @param[in] cvms : the cvm session.
 * @param[out] s  : the output sizemap structure.
 *
 * @retval STATUS_OK in case of success
 */
MESHGEMS_METHOD meshgems_status_t meshgems_cvm_session_get_sizemap(
    meshgems_cvm_session_t *cvms, meshgems_sizemap_t **s);

/**
 *  Gives back the resulting distance to the original surface as a sizemap.
 *
 * @param[in] cvms : the cvm session.
 * @param[in] s : the output sizemap structure.
 *
 * @retval STATUS_OK in case of success
 */

MESHGEMS_METHOD meshgems_status_t meshgems_cvm_session_get_distance_to_original_surface_sizemap(
    meshgems_cvm_session_t *cvms, meshgems_sizemap_t **s);

/**
 *  Gives back the resulting surface mesh to the cvm session structure.
 *
 * @param[in] cvms : the cvm session.
 * @param[in] s : the output distance to original surface sizemap structure 
 *                  for resilient option.
 *
 * @retval STATUS_OK in case of success
 */

MESHGEMS_METHOD meshgems_status_t meshgems_cvm_session_get_distance_to_original_surface_sizemap_abs(
    meshgems_cvm_session_t *cvms, meshgems_sizemap_t **s);

/**
 *  Gives back the resulting surface sizemap to the cvm session structure.
 *
 * @param[in] cvms : the cvm session.
 * @param[in] s : the output absolute distance to original surface sizemap structure 
 *                  for resilient option.
 *
 * @retval STATUS_OK in case of success
 */

MESHGEMS_METHOD meshgems_status_t meshgems_cvm_session_regain_sizemap(
    meshgems_cvm_session_t *cvms, meshgems_sizemap_t *s);

/* Private part of the API. Undocumented. */

MESHGEMS_METHOD meshgems_status_t meshgems_cvm_session_set_input_wrapped_surface_mesh(
    meshgems_cvm_session_t *cvms, meshgems_mesh_t *msh);

MESHGEMS_METHOD meshgems_status_t meshgems_cvm_session_get_subdomain_count(
    meshgems_cvm_session_t *cvms, meshgems_integer *n);

MESHGEMS_METHOD meshgems_status_t meshgems_cvm_session_get_subdomain_index(
    meshgems_cvm_session_t *cvms, meshgems_integer i, meshgems_integer *k);

MESHGEMS_METHOD meshgems_status_t meshgems_cvm_session_get_subdomain_mesh(
    meshgems_cvm_session_t *cvms, meshgems_integer i, meshgems_mesh_t **msh);

MESHGEMS_METHOD meshgems_status_t meshgems_cvm_session_regain_subdomain_mesh(
    meshgems_cvm_session_t *cvms, meshgems_integer i, meshgems_mesh_t *msh);

MESHGEMS_METHOD meshgems_status_t meshgems_cvm_session_get_subdomain_sizemap(
    meshgems_cvm_session_t *cvms, meshgems_integer i, meshgems_sizemap_t **s);

MESHGEMS_METHOD meshgems_status_t meshgems_cvm_session_regain_subdomain_sizemap(
    meshgems_cvm_session_t *cvms, meshgems_integer i, meshgems_sizemap_t *s);

MESHGEMS_METHOD meshgems_status_t meshgems_cvm_session_get_subdomain_vertices_global_index_count(
    meshgems_cvm_session_t *cvms, meshgems_integer i, meshgems_integer *n);

MESHGEMS_METHOD meshgems_status_t meshgems_cvm_session_get_subdomain_vertices_global_index(
    meshgems_cvm_session_t *cvms, meshgems_integer i, meshgems_integer j,
    meshgems_integer *r);

MESHGEMS_METHOD meshgems_status_t meshgems_cvm_session_get_subdomain_edges_global_index_count(
    meshgems_cvm_session_t *cvms, meshgems_integer i, meshgems_integer *n);

MESHGEMS_METHOD meshgems_status_t meshgems_cvm_session_get_subdomain_edges_global_index(
    meshgems_cvm_session_t *cvms, meshgems_integer i, meshgems_integer j,
    meshgems_integer *r);

MESHGEMS_METHOD meshgems_status_t meshgems_cvm_session_get_subdomain_triangles_global_index_count(
    meshgems_cvm_session_t *cvms, meshgems_integer i, meshgems_integer *n);

MESHGEMS_METHOD meshgems_status_t meshgems_cvm_session_get_subdomain_triangles_global_index(
    meshgems_cvm_session_t *cvms, meshgems_integer i, meshgems_integer j,
    meshgems_integer *r);

MESHGEMS_METHOD meshgems_status_t meshgems_cvm_session_get_subdomain_tetrahedra_global_index_count(
    meshgems_cvm_session_t *cvms, meshgems_integer i, meshgems_integer *n);

MESHGEMS_METHOD meshgems_status_t meshgems_cvm_session_get_subdomain_tetrahedra_global_index(
    meshgems_cvm_session_t *cvms, meshgems_integer i, meshgems_integer j,
    meshgems_integer *r);

MESHGEMS_METHOD meshgems_status_t meshgems_cvm_session_get_vertex_global_count(
    meshgems_cvm_session_t *cvms, meshgems_integer *r);

MESHGEMS_METHOD meshgems_status_t meshgems_cvm_session_get_edge_global_count(
    meshgems_cvm_session_t *cvms, meshgems_integer *r);

MESHGEMS_METHOD meshgems_status_t meshgems_cvm_session_get_triangle_global_count(
    meshgems_cvm_session_t *cvms, meshgems_integer *r);

MESHGEMS_METHOD meshgems_status_t meshgems_cvm_session_get_tetrahedron_global_count(
    meshgems_cvm_session_t *cvms, meshgems_integer *r);

MESHGEMS_METHOD meshgems_integer meshgems_cvm_get_version_major(void);
MESHGEMS_METHOD meshgems_integer meshgems_cvm_get_version_minor(void);
MESHGEMS_METHOD meshgems_integer meshgems_cvm_get_version_patch(void);
MESHGEMS_METHOD const char *meshgems_cvm_get_version_string(void);
MESHGEMS_METHOD const char *meshgems_cvm_get_version_ident_string(void);

#endif
