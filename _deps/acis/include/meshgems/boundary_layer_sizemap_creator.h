#ifndef __BOUNDARY_LAYER_SIZEMAP_CREATOR_H__
#define __BOUNDARY_LAYER_SIZEMAP_CREATOR_H__

#include <meshgems/mdecl.h>
#include <meshgems/basic_types.h>
#include <meshgems/sizemap.h>

/**

\defgroup boundary_layer_sizemap_creator meshgems_boundary_layer_sizemap_creator_t

@{

Tool to create a boundary layer sizemap structure (local sizing constraint) using
intuitive definition.

The \ref meshgems_boundary_layer_sizemap_creator_t abstract object

*/
struct meshgems_boundary_layer_sizemap_creator_t_;

typedef struct meshgems_boundary_layer_sizemap_creator_t_ meshgems_boundary_layer_sizemap_creator_t;

/**
 * Simple boundary layer sizemap creator constructor.
 * @param msh (in) : the input mesh
 * @return a new sizemap creator object or NULL.
 */
MESHGEMS_METHOD meshgems_boundary_layer_sizemap_creator_t *meshgems_boundary_layer_sizemap_creator_new(
        meshgems_mesh_t *msh);

/**
 * Boundary layer sizemap creator object destructor.
 */
MESHGEMS_METHOD void meshgems_boundary_layer_sizemap_creator_delete(
        meshgems_boundary_layer_sizemap_creator_t *sc);

/**
  * Add a new geometric boundary layer constraint on a given mesh element tag.
  * @param scd (in) : the working sizemap creator object
  * @param h0 (in) : the height of the first layer in the boundary layer
  * @param progression (in) : the progression coefficient between two consecutive layers, only for layers between 1 and n_min
  * @param blending (in) : the blending coefficient between two consecutive layers, only for layers between n_min and n_max
  * @param n_min (in) : the minimum number of desired layers in the boundary
  * @param n_max (in) : the maximum number of desired layers in the boundary or 0 for an automatic selection
  * @param relative (in) : 1 if the size is relative 0 if the size is constant
  * @param tag (in) : the mesh element tag on which to create this boundary layer
  * @return error code
  */
MESHGEMS_METHOD meshgems_status_t meshgems_boundary_layer_sizemap_creator_add_constraint_on_tag(
        meshgems_boundary_layer_sizemap_creator_t *scd, meshgems_real h0,
        meshgems_real progression, meshgems_real blending, meshgems_integer n_min,
        meshgems_integer n_max, meshgems_integer relative, meshgems_integer tag);

/**
 * Effectively create a sizemap out of the boundary layer sizemap creator.
 * Acts as a \ref meshgems_sizemap_t constructor.
 * @param sc (in) : the working sizemap creator object.
 * @return a new meshgems_sizemap_t object embeding the constraints
 *  stored in sc (or NULL in case of failure).
 */
MESHGEMS_METHOD meshgems_sizemap_t *meshgems_boundary_layer_sizemap_creator_generate_sizemap(
        meshgems_boundary_layer_sizemap_creator_t *sc);

/** Private part of the mesh interface. Only Distene products should use it. **/

MESHGEMS_METHOD meshgems_integer meshgems_boundary_layer_sizemap_creator_get_number_of_tags(
        meshgems_boundary_layer_sizemap_creator_t *sc);

#endif
