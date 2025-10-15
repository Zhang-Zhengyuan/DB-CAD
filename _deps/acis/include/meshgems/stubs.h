#ifndef __MESHGEMS_STUBS_H__
#define __MESHGEMS_STUBS_H__

#include <meshgems/mdecl.h>

/* Needed for size_t type */
#include <stdio.h>

/**
 * This file defines the prototypes of the stubs the user can redefine for MeshGems API.
 * For now, only the memory management functions are available.
 */


/**
 *  Stub for malloc() function
 */
MESHGEMS_METHOD void *meshgems_malloc(size_t size);


/**
 *  Stub for calloc() function
 */
MESHGEMS_METHOD void *meshgems_calloc(size_t nmemb, size_t size);


/**
 *  Stub for realloc() function
 */
MESHGEMS_METHOD void *meshgems_realloc(void *ptr, size_t size);


/**
 *  Stub for free() function
 */
MESHGEMS_METHOD void meshgems_free(void *ptr);


#endif
