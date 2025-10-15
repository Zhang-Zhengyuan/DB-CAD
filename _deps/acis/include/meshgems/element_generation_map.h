#ifndef __MESHGEMS_ELEMENT_GENERATION_MAP_H__
#define __MESHGEMS_ELEMENT_GENERATION_MAP_H__

#include <meshgems/mdecl.h>
#include <meshgems/basic_types.h>
#include <meshgems/mesh.h>
#include <meshgems/cad.h>

struct meshgems_element_generation_map_t_;

/**
 * Opaque type to store element_generation_map function and data.
 */
typedef struct meshgems_element_generation_map_t_ meshgems_element_generation_map_t;

enum meshgems_generation_type_t_ {
  meshgems_generation_triangle,
  meshgems_generation_quad_dominant,
  meshgems_generation_full_quad
};
typedef enum meshgems_generation_type_t_ meshgems_generation_type_t;

/**
 * Type for functions that define the element type on a parametric surface
 * @param fid (in) : the id of the working cad_face 
 * @param user_data (in): a generic user pointer
 * @param type(out) : the associated element type.
 * @return error code (STATUS_OK means that type was set)
 *
 */
typedef meshgems_status_t (*meshgems_element_generation_map_cad_face_t)(meshgems_integer fid, meshgems_generation_type_t *type, void *user_data);


enum meshgems_element_generation_map_type_t_ {
  meshgems_element_generation_map_type_none = 0,
  meshgems_element_generation_map_type_cad_edge,
  meshgems_element_generation_map_type_cad_face,
  meshgems_element_generation_map_type_last
};
typedef enum meshgems_element_generation_map_type_t_ meshgems_element_generation_map_type_t;


/**
 * Element_Generation_Map object destructor.
 */
MESHGEMS_METHOD void meshgems_element_generation_map_delete(meshgems_element_generation_map_t *t);

/**
 * Simple element_generation_map constructor.
 * @param parent (in) : the associated parent object the element_generation_map is made for : either a cad_t or a mesh_t object
 * @param ttype (in) : the type of the element_generation_map to create
 * @param fun (in) : the callback function of type meshgems_element_generation_map_cad_* or element_generation_map_mesh_*
 * @return a new cad object or NULL.
 */
MESHGEMS_METHOD meshgems_element_generation_map_t *meshgems_element_generation_map_new(void *parent, meshgems_element_generation_map_type_t ttype,
							 void *fun, void *user_data);

/** Private part of the mesh interface. Only Distene products should use it. **/

MESHGEMS_METHOD meshgems_status_t meshgems_element_generation_map_set_delete_function(meshgems_element_generation_map_t *t,  meshgems_delete_fun f);
MESHGEMS_METHOD meshgems_status_t meshgems_element_generation_map_get_type(meshgems_element_generation_map_t *t, meshgems_element_generation_map_type_t *type);
MESHGEMS_METHOD meshgems_status_t meshgems_element_generation_map_get_parent_cad(meshgems_element_generation_map_t *t, meshgems_cad_t **cad);
MESHGEMS_METHOD meshgems_status_t meshgems_element_generation_map_get_parent_mesh(meshgems_element_generation_map_t *t, meshgems_mesh_t **msh);
MESHGEMS_METHOD meshgems_status_t meshgems_element_generation_map_get_user_data(meshgems_element_generation_map_t *t, void **user_data);
MESHGEMS_METHOD meshgems_status_t meshgems_element_generation_map_get_function(meshgems_element_generation_map_t *t, void **fun);
MESHGEMS_METHOD meshgems_status_t meshgems_element_generation_map_attach_object(meshgems_element_generation_map_t *t, void *object, meshgems_delete_fun delfun);


#endif
