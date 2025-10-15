#ifndef __MESHGEMS_SIZEMAP_H__
#define __MESHGEMS_SIZEMAP_H__

#include <meshgems/mdecl.h>
#include <meshgems/basic_types.h>
#include <meshgems/mesh.h>
#include <meshgems/cad.h>

/**

\defgroup sizemap meshgems_sizemap_t

@{

Structure to describe a local (callback based) sizing constraint.

The \ref meshgems_sizemap_t object is a structure containing a
callback based local sizing constaint that can apply for example to
mesh entities or to CAD entities or directly to the xyz space
coordinates.

At construction time (with \ref meshgems_sizemap_new), the type of the
sizing constraint is provided () together with a callback ( meshgems_sizemap_iso_cad_face_t)



*/


struct meshgems_sizemap_t_;

/**
 * Opaque type to store sizemap function and data.
 */
typedef struct meshgems_sizemap_t_ meshgems_sizemap_t;


/**
 * Type for functions that define isometric sizemap on a parametric surface
 * @param fid (in) : the id of the working cad_face 
 * @param uv (in) : a real[2] array filled with the UV parametric coordinates of the desired surface XYZ point.
 * @param user_data (in): a generic user pointer
 * @param size(out) : the associated size.
 * @return a status code :
 *   + STATUS_OK means that size was set
 *   + any other value means that the size returned cannot be trusted
 *
 */
typedef meshgems_status_t (*meshgems_sizemap_iso_cad_face_t)(meshgems_integer fid, meshgems_real *uv,
							   meshgems_real *size, void *user_data);

/**
 * Type for functions that define anisometric sizemap on a parametric surface
 * @param fid (in) : the id of the working cad_face 
 * @param uv (in) : a real[2] array filled with the UV parametric coordinates of the desired surface XYZ point.
 * @param user_data (in): a generic user pointer
 * @param met(out) : the associated metric.
 * @return a status code :
 *   + STATUS_OK means that the metric was set
 *   + any other value means that the metric returned cannot be trusted
 *
 */
typedef meshgems_status_t (*meshgems_sizemap_aniso_cad_face_t)(meshgems_integer fid, meshgems_real *uv,
							   meshgems_real *met, void *user_data);

/**
 * Type for functions that define isometric sizemap on a parametric curve not associated to a surface
 * @param eid (in) : the id of the working cad_edge 
 * @param t (in) : the time parameter corresponding to the desired curve point. Note : If the edge id corresponds to
 * several parametric curves, the main one is retained.
 * @param user_data (in): a generic user pointer
 * @param size(out) : the associated size.
 * @return error code (STATUS_OK means that size was set)
 *
 */
typedef meshgems_status_t (*meshgems_sizemap_iso_cad_edge_t)(meshgems_integer eid, meshgems_real t,
							   meshgems_real *size, void *user_data);

/**
 * Type for functions that define isometric sizemap at a specific CAD point
 * @param pid (in) : the id of the working cad_point 
 * @param user_data (in): a generic user pointer
 * @param size(out) : the associated size.
 * @return error code (STATUS_OK means that size was set)
 *
 */
typedef meshgems_status_t (*meshgems_sizemap_iso_cad_point_t)(meshgems_integer pid, meshgems_real *size,
							    void *user_data);

/**
 * Type for functions that defines an implicit sizemap by the geometric approximation on a parametric surface
 * @param fid (in) : the id of the working cad_face 
 * @param user_data (in): a generic user pointer
 * @param geometric_approximation(out) : the associated geometic approximation (angule in degree). (0 means none)
 * @param tolerance(out) : the associated tolerance (distance). (0 means none)
 * @param size_min(out) : the associated min size. (0 means none)
 * @param size_max(out) : the associated max size. (0 means none)
 * @return error code (STATUS_OK means that size was set)
 *
 */
typedef meshgems_status_t (*meshgems_sizemap_geometric_cad_face_t)(meshgems_integer fid,
								 meshgems_real *geometric_approximation,
								 meshgems_real *tolerance,
								 meshgems_real *size_min, meshgems_real *size_max,
								 void *user_data);

/**
 * Type for functions that defines an implicit sizemap by the geometric approximation on a parametric curve
 * @param eid (in) : the id of the working cad_edge
 * @param user_data (in): a generic user pointer
 * @param geometric_approximation(out) : the associated  geometic approximation (angle in degree). (0 means none)
 * @param tolerance(out) : the associated tolerance (distance). (0 means none)
 * @param size_min(out) : the associated min size. (0 means none)
 * @param size_max(out) : the associated max size. (0 means none)
 * @return error code (STATUS_OK means that size was set)
 *
 */
typedef meshgems_status_t (*meshgems_sizemap_geometric_cad_edge_t)(meshgems_integer eid,
								 meshgems_real *geometric_approximation,
								 meshgems_real *tolerance,
								 meshgems_real *size_min, meshgems_real *size_max,
								 void *user_data);

/**
 * Type for functions that defines an implicit sizemap at a specific CAD point
 * @param pid (in) : the id of the working cad_edge
 * @param user_data (in): a generic user pointer
 * @param size_min(out) : the associated min size. (0 means none)
 * @param size_max(out) : the associated max size. (0 means none)
 * @return error code (STATUS_OK means that size was set)
 *
 */
typedef meshgems_status_t (*meshgems_sizemap_geometric_cad_point_t)(meshgems_integer pid,
								 meshgems_real *size_min, meshgems_real *size_max,
								 void *user_data);


/**
 * Type for functions that define isometric sizemap at a specific mesh point
 * @param i (in) : the index of the point
 * @param user_data (in): a generic user pointer
 * @param size(out) : the associated size.
 * @return error code (STATUS_OK means that size was set)
 *
 */
typedef meshgems_status_t (*meshgems_sizemap_iso_mesh_vertex_t)(meshgems_integer pid, meshgems_real *size,
							      void *user_data);

/**
 * Type for functions that define isometric sizemap by tag on a discrete line
 * @param i (in) : the tag of the mesh edges
 * @param user_data (in): a generic user pointer
 * @param size(out) : the associated size.
 * @return error code (STATUS_OK means that size was set)
 *
 */
typedef meshgems_status_t (*meshgems_sizemap_iso_mesh_edge_tag_t)(meshgems_integer etag, meshgems_real *size,
							      void *user_data);

/**
 * Type for functions that define an isotropic sizemap by tag on a discrete surface
 * @param ftag (in) : the tag of the mesh faces
 * @param user_data (in): a generic user pointer
 * @param size(out) : the associated size.
 * @return error code (STATUS_OK means that size was set)
 *
 */
typedef meshgems_status_t (*meshgems_sizemap_iso_mesh_face_tag_t)(meshgems_integer ftag, meshgems_real *size,
                    void *user_data);

/**
 * Type for functions that define a geometric sizemap by tag on a discrete line
 * @param ftag (in) : the tag of the mesh edges
 * @param geometric_approximation(out) : the associated  geometic approximation (angle in degree). (0 means none)
 * @param chordal_error(out) : the associated chordal error (0 means none)
 * @param size_min(out) : the associated min size. (0 means none)
 * @param size_max(out) : the associated max size. (0 means none)
 * @param user_data (in): a generic user pointer
 * @return error code (STATUS_OK means that size was set)
 *
 */
typedef meshgems_status_t (*meshgems_sizemap_geometric_mesh_edge_tag_t)(meshgems_integer etag,
                 meshgems_real *geometric_approximation,
                 meshgems_real *chordal_error,
                 meshgems_real *size_min,
                 meshgems_real *size_max,
                 void *user_data);

/**
 * Type for functions that define a geometric sizemap by tag on a discrete surface
 * @param ftag (in) : the tag of the mesh face
 * @param geometric_approximation(out) : the associated  geometic approximation (angle in degree). (0 means none)
 * @param chordal_error(out) : the associated chordal error (0 means none)
 * @param size_min(out) : the associated min size. (0 means none)
 * @param size_max(out) : the associated max size. (0 means none)
 * @param user_data (in): a generic user pointer
 * @return error code (STATUS_OK means that size was set)
 *
 */
typedef meshgems_status_t (*meshgems_sizemap_geometric_mesh_face_tag_t)(meshgems_integer ftag,
                 meshgems_real *geometric_approximation,
                 meshgems_real *chordal_error,
                 meshgems_real *size_min,
                 meshgems_real *size_max,
                 void *user_data);

/**
 * Type for functions that define anisometric sizemap at a specific mesh point
 * @param i (in) : the index of the point
 * @param user_data (in): a generic user pointer
 * @param met(out) : the associated metric.
 * @return error code (STATUS_OK means that size was set)
 *
 */
typedef meshgems_status_t (*meshgems_sizemap_aniso_mesh_vertex_t)(meshgems_integer pid, meshgems_real *met,
							      void *user_data);

/**
 * Type for functions that define isometric sizemap at a specific 3d space point
 * @param xyz (in) : the coordinates of the point we are querying a size for 
 * @param user_data (in): a generic user pointer
 * @param size(out) : the associated size.
 * @return error code (STATUS_OK means that size was set)
 *
 */
typedef meshgems_status_t (*meshgems_sizemap_iso_3d_t)(meshgems_real *xyz, meshgems_real *size,
						     void *user_data);


/**
 * Type for functions that define anisometric sizemap at a specific 3d space point
 * @param xyz (in) : the coordinates of the point we are querying a size for 
 * @param user_data (in): a generic user pointer
 * @param size(out) : the associated size.
 * @return error code (STATUS_OK means that size was set)
 *
 */
typedef meshgems_status_t (*meshgems_sizemap_aniso_3d_t)(meshgems_real *xyz, meshgems_real *met,
							 void *user_data);

/**
 * Type for functions that define the number of point to be generated on a specific CAD edge
 * @param eid (in) : the id of the working cad_edge 
 * @param n (out) : the number of point to be generated on the CAD edge with id eid. Note : If the edge id corresponds to
 * several parametric curves, the main one is retained.
 * @param user_data (in): a generic user pointer
 * @return error code (STATUS_OK means that size was set)
 *
 */
typedef meshgems_status_t (*meshgems_sizemap_number_of_vertices_cad_edge_t)(meshgems_integer eid, meshgems_integer *n,
									    void *user_data);

/**
 * Type for functions that define anisotropic sizemap at a CAD edge
 * @param eid (in) : the id of the working cad_edge 
 * @param h1 (out): the size on the direction of the cad edge
 * @param h2 (out): the size on all the direction orthogonal to the cad edge
 * @param user_data (in): a generic user pointer
 * @return error code (STATUS_OK means that size was set)
 *
 */
typedef meshgems_status_t (*meshgems_sizemap_axisymmetric_along_cad_edge_t)(meshgems_integer eid, meshgems_real *h1,
							       meshgems_real *h2, void *user_data);

/**
 * Type for functions that define boundary layers sizemap around a CAD edge
 * @param eid (in) : the id of the working cad_edge
 * @param nlayer (out): the number of required boundary layers
 * @param h1 (out): the size on the direction of the cad edge
 * @param h2 (out): the size on all the direction orthogonal to the cad edge, on the first layer
 * @param propagation (out): the sizemap propagation between the layers
 * @param user_data (in): a generic user pointer
 * @return error code (STATUS_OK means that size was set)
 *
 */
typedef meshgems_status_t (*meshgems_sizemap_boundary_layer_around_cad_edge_t)(meshgems_integer eid, meshgems_integer *nlayer,
									       meshgems_real *h1, meshgems_real *h2,
									       meshgems_real *propagation, void *user_data);

/**
 * Type for functions that define boundary layers sizemap around a mesh face
 * @param face_type (in) : the type of the considered face
 * @param iface (in) : the id of the face
 * @param layer (out): a specific layer
 * @param orient (out): the orientation of the normal of the face
 * @param h (out): the size along the oriented normal direction at the face, at a specific layer
 * @param mandatory(out): enforce the generation of this layer
 * @param user_data (in): a generic user pointer
 * @return error code (STATUS_OK means that size was set)
 *
 */
typedef meshgems_status_t (*meshgems_sizemap_boundary_layer_around_mesh_face_t)(meshgems_integer face_type,meshgems_integer iface,
									       meshgems_integer layer, meshgems_integer orient, meshgems_real *h,
									       meshgems_integer *mandatory, void *user_data);

enum meshgems_sizemap_type_t_ {
  meshgems_sizemap_type_none = 0,
  meshgems_sizemap_type_iso_cad_point,
  meshgems_sizemap_type_iso_cad_edge,
  meshgems_sizemap_type_iso_cad_face,
  meshgems_sizemap_type_iso_mesh_vertex,
  meshgems_sizemap_type_geometric_cad_face,
  meshgems_sizemap_type_geometric_cad_edge,
  meshgems_sizemap_type_iso_3d,
  meshgems_sizemap_type_aniso_mesh_vertex,
  meshgems_sizemap_type_aniso_3d,
  meshgems_sizemap_type_number_of_vertices_cad_edge,
  meshgems_sizemap_type_axisymmetric_along_cad_edge,
  meshgems_sizemap_type_geometric_cad_point,
  meshgems_sizemap_type_iso_mesh_face_tag,
  meshgems_sizemap_type_geometric_mesh_face_tag,
  meshgems_sizemap_type_boundary_layer_around_cad_edge,
  meshgems_sizemap_type_iso_mesh_edge_tag,
  meshgems_sizemap_type_geometric_mesh_edge_tag,
  meshgems_sizemap_type_aniso_cad_face,
  meshgems_sizemap_type_boundary_layer_around_mesh_face,
  meshgems_sizemap_type_last,
};
typedef enum meshgems_sizemap_type_t_ meshgems_sizemap_type_t;

/**
 * Sizemap object destructor.
 */
MESHGEMS_METHOD void meshgems_sizemap_delete(meshgems_sizemap_t *s);

/**
 * Simple sizemap constructor.
 * @param parent (in) : the associated parent object the sizemap is made for :
 * - a context_t object in the case of a structured sizemap object on the XYZ space, ie when the stype parameter is one of the following :
 * 	- meshgems_sizemap_type_iso_3d
 * 	- meshgems_sizemap_type_aniso_3d
 * - a cad_t object in the case of a structured sizemap object on analytic CAD, ie when the stype parameter is one of the following :
 * 	- meshgems_sizemap_type_iso_cad_point
 * 	- meshgems_sizemap_type_iso_cad_edge
 * 	- meshgems_sizemap_type_iso_cad_face
 * 	- meshgems_sizemap_type_geometric_cad_face
 * 	- meshgems_sizemap_type_geometric_cad_edge
 * 	- meshgems_sizemap_type_number_of_vertices_cad_edge
 * 	- meshgems_sizemap_type_axisymmetric_along_cad_edge
 * 	- meshgems_sizemap_type_geometric_cad_point
 * 	- meshgems_sizemap_type_boundary_layer_around_cad_edge
 * - a mesh_t object in the case of a structured sizemap object on a mesh, ie when the stype parameter is one of the following :
 * 	- meshgems_sizemap_type_iso_mesh_vertex
 * 	- meshgems_sizemap_type_aniso_mesh_vertex
 * 	- meshgems_sizemap_type_iso_mesh_face_tag
 * 	- meshgems_sizemap_type_geometric_mesh_face_tag
 * @param stype (in) : the type of the sizemap to create
 * @param fun (in) : the callback function of type corresponding to the value of the stype parameter
 * (meshgems_sizemap_*cad*, meshgems_sizemap_*mesh* or meshgems_sizemap_*3d*)
 * @param user_data (in) : the user data pointer `fun` will be called with
 * @return a new sizemap object or NULL.
 */
MESHGEMS_METHOD meshgems_sizemap_t *meshgems_sizemap_new(void *parent, meshgems_sizemap_type_t stype,
						      void *fun, void *user_data);

/**
 * Memory sizemap constructor.
 * @param parent (in) : the associated parent object the sizemap is made for : a mesh_t object
 * @param stype (in) : the type of the sizemap to create
 * @return a new szemap object or NULL.
 */
MESHGEMS_METHOD meshgems_sizemap_t *meshgems_sizemap_new_in_memory(meshgems_mesh_t *parent, meshgems_sizemap_type_t stype);

/**
 * Deep copy constructor.
 * @return a new \ref meshgems_sizemap_t object
 */
MESHGEMS_METHOD meshgems_sizemap_t *meshgems_sizemap_new_in_memory_copy(meshgems_sizemap_t *siz);

/**
 * Memory sizemap companion functions used to fill in the values.
 * @param s (in) : the memory sizemap_t object
 * @param pid (in) : the id of the working mesh vertex 
 * @param size(in) : the associated size.
 * @return a new sizemap object or NULL.
 */
MESHGEMS_METHOD meshgems_status_t meshgems_sizemap_iso_mesh_vertex_set_size(meshgems_sizemap_t *s, meshgems_integer pid, const meshgems_real *size);

/**
 * Memory sizemap companion functions used to fill in the values.
 * @param s (in) : the memory sizemap_t object
 * @param pid (in) : the id of the working mesh vertex 
 * @param met(in) : the associated metric.
 * @return a new sizemap object or NULL.
 */
MESHGEMS_METHOD meshgems_status_t meshgems_sizemap_aniso_mesh_vertex_set_metric(meshgems_sizemap_t *s, meshgems_integer pid, const meshgems_real *met);

/**
 * Read a sizemap from a "SOL" file and creates a new sizemap structure.
 *
 * @param parent (in) : the associated mesh_t parent object the sizemap is made for
 * @param name (in) : the file name to read from
 *
 * @retval a pointer to a sizemap on success (the sizemap was successfully read from file name);
 * @retval NULL : on failure (see messages in the context message callback).
 *
 */
MESHGEMS_METHOD meshgems_sizemap_t *meshgems_sizemap_new_read_sol(meshgems_mesh_t *parent, const char *name);

/**
 * Write a meshgems_mesh_t to a "MESH" file.
 *
 * @param[in] s : the sizemap
 * @param[in] name : the file name to write into
 *
 * @retval STATUS_OK : on success, the sizemap was successfully written to file \p name;
 * @retval STATUS_ERROR : on failure.
 *
 */
MESHGEMS_METHOD meshgems_status_t meshgems_sizemap_write_sol(meshgems_sizemap_t *s, const char *name);

/** @} */

/** Private part of the mesh interface. Only Distene products should use it. **/

MESHGEMS_METHOD meshgems_status_t meshgems_sizemap_set_delete_function(meshgems_sizemap_t *s,  meshgems_delete_fun f);
MESHGEMS_METHOD meshgems_status_t meshgems_sizemap_get_type(meshgems_sizemap_t *s, meshgems_sizemap_type_t *type);
MESHGEMS_METHOD meshgems_status_t meshgems_sizemap_get_parent_cad(meshgems_sizemap_t *s, meshgems_cad_t **cad);
MESHGEMS_METHOD meshgems_status_t meshgems_sizemap_get_parent_mesh(meshgems_sizemap_t *s, meshgems_mesh_t **msh);
MESHGEMS_METHOD meshgems_status_t meshgems_sizemap_get_user_data(meshgems_sizemap_t *s, void **user_data);
MESHGEMS_METHOD meshgems_status_t meshgems_sizemap_get_function(meshgems_sizemap_t *s, void **fun);
MESHGEMS_METHOD meshgems_status_t meshgems_sizemap_iso_mesh_vertex_get_size(meshgems_sizemap_t *s, meshgems_integer pid, meshgems_real *size);
MESHGEMS_METHOD meshgems_sizemap_t *meshgems_sizemap_new_read_bb(meshgems_mesh_t *parent, const char *name);
MESHGEMS_METHOD meshgems_status_t meshgems_sizemap_write_bb(meshgems_sizemap_t *s, const char *name);
MESHGEMS_METHOD meshgems_status_t meshgems_sizemap_aniso_mesh_vertex_get_metric(meshgems_sizemap_t *s, meshgems_integer pid, meshgems_real *met);
MESHGEMS_METHOD meshgems_status_t meshgems_sizemap_attach_object(meshgems_sizemap_t *, void *object, meshgems_delete_fun delfun);
MESHGEMS_METHOD meshgems_sizemap_t *meshgems_sizemap_new_mesh_vertex_from_sizemap_and_mesh(meshgems_sizemap_t *s, meshgems_mesh_t *im);
MESHGEMS_METHOD meshgems_sizemap_t *meshgems_sizemap_new_mesh_vertex_from_sizemap(meshgems_sizemap_t *s, meshgems_real *xyzmin, meshgems_real *xyzmax, meshgems_integer nv);


MESHGEMS_METHOD meshgems_sizemap_t *meshgems_proximity2D_compute_core(meshgems_mesh_t *, const meshgems_real , const meshgems_real , const meshgems_integer , const meshgems_real , const meshgems_real );


#endif
