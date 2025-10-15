#ifndef __MESHGEMS_H__
#define __MESHGEMS_H__

#define MESHGEMS_VERSION_MAJOR 2
#define MESHGEMS_VERSION_MINOR 14
#define MESHGEMS_VERSION_PATCH 2
#define MESHGEMS_VERSION "2.14"
#define MESHGEMS_PATCHRE "2"
#define MESHGEMS_RELEASE_YEAR "2021"
#define MESHGEMS_RELEASE "December, " MESHGEMS_RELEASE_YEAR
#define MESHGEMS_VERSION_LONG "2.14-2"

#include <meshgems/mdecl.h>
#include <meshgems/stubs.h>
#include <meshgems/basic_types.h>
#include <meshgems/status.h>
#include <meshgems/context.h>
#include <meshgems/code.h>
#include <meshgems/element_generation_map.h>
#include <meshgems/interrupt.h>
#include <meshgems/cad.h>
#include <meshgems/dcad.h>
#include <meshgems/message.h>
#include <meshgems/sizemap.h>
#include <meshgems/sizemap_creator.h>
#include <meshgems/tag_system.h>
#include <meshgems/threads.h>
#include <meshgems/mesh.h>
#include <meshgems/mesh_cracker.h>
#include <meshgems/mesh_operations.h>
#include <meshgems/mesh_tag_system.h>
#include <meshgems/meshgems_short_names.h>
#include <meshgems/meshgems_old_names.h>

/**
 * Get the meshgems core major version number.
 * @return the major version number of the called library
 */
MESHGEMS_METHOD meshgems_integer meshgems_core_get_version_major(void);

/**
 * Get the meshgems core minor version number.
 * @return the minor version number of the called library
 */
MESHGEMS_METHOD meshgems_integer meshgems_core_get_version_minor(void);

/**
 * Get the meshgems core patch version number.
 * @return the patch version number of the called library
 */
MESHGEMS_METHOD meshgems_integer meshgems_core_get_version_patch(void);

/**
 * Get the meshgems core version string.
 * @return the version string of the called library
 */
MESHGEMS_METHOD const char *meshgems_core_get_version_string(void);

/**
 * Get the meshgems core ident string.
 * @return the version string of the called library
 */
MESHGEMS_METHOD const char *meshgems_core_get_version_ident_string(void);


#endif
