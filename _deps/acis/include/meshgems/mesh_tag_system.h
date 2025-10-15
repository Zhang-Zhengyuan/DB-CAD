#ifndef __MESHGEMS_MESH_TAG_SYSTEM_H__
#define __MESHGEMS_MESH_TAG_SYSTEM_H__


#include <meshgems/mdecl.h>
#include <meshgems/status.h>
#include <meshgems/mesh.h>
#include <meshgems/tag_system.h>


MESHGEMS_METHOD meshgems_status_t meshgems_mesh_tag_system_get_modified_surface(meshgems_tag_system_t *ts1, meshgems_tag_system_t *ts2, meshgems_mesh_t *msh, meshgems_mesh_t **outmsh);


#endif
