#ifndef __MESHGEMS_MESH_OPERATIONS_H__
#define __MESHGEMS_MESH_OPERATIONS_H__


#include <meshgems/mdecl.h>
#include <meshgems/status.h>
#include <meshgems/mesh.h>
#include <meshgems/context.h>


/**
 * Create a new mesh_t which is the concatenation of two other mesh_t
 * @param[in]   ctx :  the context
 * @param[in]	m1	: the first mesh_t
 * @param[in]	m2	: the second mesh_t
 * @param[out]	outpush_msh	: the resulting mesh_t of the concatenation of m1 and m2
 * @return error code
 */
MESHGEMS_METHOD meshgems_status_t meshgems_mesh_new_concatenate(meshgems_context_t *ctx, 
	meshgems_mesh_t *m1,
	meshgems_mesh_t *m2,
	meshgems_mesh_t **output_msh);
/**
 * Add a mesh_t into a previously created one
 * @param[in]	pmsh : the first mesh_t (this mesh_t must have the implementation of the functions of the type mesh_set_XXX)
 * @param[in]	mshtoadd : the second mesh_t which will be added into the first mesh_t
 * @return error code
 */
MESHGEMS_METHOD meshgems_status_t meshgems_mesh_concatenate(meshgems_mesh_t *msh, meshgems_mesh_t *mshtoadd);
/**
 * Extract the surface of a mesh_t
 * @param[in]   ctx :  the context
 * @param[in]	msh :  a mesh_t 
 * @param[out]	msh_surface : a mesh_t which contains only the surface elements of the msh mesh_t
 * @return error code
 */
MESHGEMS_METHOD meshgems_status_t meshgems_mesh_new_extract_surface_mesh(meshgems_context_t *ctx, 
	meshgems_mesh_t *msh, 
	meshgems_mesh_t **msh_surface);
#endif
