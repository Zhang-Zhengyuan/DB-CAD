#ifndef __MESHGEMS_MESH_CHECKER_H__
#define __MESHGEMS_MESH_CHECKER_H__

#include <meshgems/mdecl.h>
#include <meshgems/basic_types.h>
#include <meshgems/status.h>
#include <meshgems/context.h>
#include <meshgems/mesh.h>

/**
 *
 * Check a \ref meshgems_mesh_t tetrahedron subdomains coherence.
 *
 * @param[in]	m		: the mesh.
 * @param[out]	res		: 1 if the subdomains are correct, 0 if a problem has been found.
 *
 * @retval STATUS_OK : on success, the mesh was successfully checked;
 *
 * @retval STATUS_ERROR : on failure.
 *
 */

MESHGEMS_METHOD meshgems_status_t meshgems_mesh_check_subdomains_coherency(meshgems_mesh_t *m,integer *res);

#endif
