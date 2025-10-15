#ifndef __MESHGEMS_SIZEMAP_CREATOR_H__
#define __MESHGEMS_SIZEMAP_CREATOR_H__

#include <meshgems/mdecl.h>
#include <meshgems/basic_types.h>

/**

\defgroup sizemap_creator meshgems_sizemap_creator_t

@{

Tool to create a sizemap structure (local sizing constraint) using
intuitive definition.

The \ref meshgems_sizemap_creator_t object is 

*/

struct meshgems_sizemap_creator_t_;

/**
 * Opaque type to store sizemap creator data.
 */
typedef struct meshgems_sizemap_creator_t_ meshgems_sizemap_creator_t;

/**
 * Enum type to describe the sizemap creator type.
 */
enum meshgems_sizemap_creator_type_t_ {
  /** No type */
  meshgems_sizemap_creator_type_none = 0,
  /** 3d isotropic sizemap creator */
  meshgems_sizemap_creator_type_iso_3d,
  /** 3d anisotropic sizemap creator */
  meshgems_sizemap_creator_type_aniso_3d,
  /** Last type (internal use) */
  meshgems_sizemap_creator_type_last
};

/**
 * Typedef for the enum type that describes the sizemap creator type.
 */
typedef enum meshgems_sizemap_creator_type_t_ meshgems_sizemap_creator_type_t;

/**
 * Sizemap creator object destructor.
 */
MESHGEMS_METHOD void meshgems_sizemap_creator_delete(meshgems_sizemap_creator_t *s);

/**
 * Simple sizemap creator constructor.
 * @param parent (in) : the associated parent object the sizemap creator is made for : only a context for now
 * @param stype (in) : the type of the sizemap creator to create
 * @param xyz_min (in) : the coordinates of the minimum point of the bounding box of the geometry
 * @param xyz_max (in) : the coordinates of the maximum point of the bounding box of the geometry
 * @return a new sizemap creator object or NULL.
 */
MESHGEMS_METHOD meshgems_sizemap_creator_t *meshgems_sizemap_creator_new(void *parent, meshgems_sizemap_creator_type_t stype,
									 meshgems_real xyz_min[3], meshgems_real xyz_max[3]);

/**
 * Set the default size requested outside of each constraint.
 * @param sc (in) : the working sizemap creator object
 * @param size (in) : the default size
 * @return error code (STATUS_OK means that size was set)
 */
MESHGEMS_METHOD meshgems_status_t meshgems_sizemap_creator_add_constraint_default(meshgems_sizemap_creator_t *sc, meshgems_real size);

/**
 * Set the default gradation requested between all the constraints.
 * @param sc (in) : the working sizemap creator object
 * @param size (in) : the default gradation
 * @return error code (STATUS_OK means that size was set)
 */
MESHGEMS_METHOD meshgems_status_t meshgems_sizemap_creator_add_constraint_default_gradation(meshgems_sizemap_creator_t *sc, meshgems_real gradation);

/**
 * Add a new constraint : specify a size inside a sphere.
 * @param sc (in) : the working sizemap creator object
 * @param center (in) : the coordinates of the center of the sphere
 * @param radius (in) : the radius of the sphere
 * @param size (in) : the size to be used inside the sphere
 * @return error code (STATUS_OK means that size was set)
 */
MESHGEMS_METHOD meshgems_status_t meshgems_sizemap_creator_add_constraint_on_sphere(meshgems_sizemap_creator_t *sc, meshgems_real center[3], 
                                                                                    meshgems_real radius, meshgems_real size);

/**
  * Add a new constraint : specify a size inside a cylinder.
  * @param sc (in) : the working sizemap creator object
  * @param p1 (in) : the coordinates of the first point of the cylinder axis
  * @param p2 (in) : the coordinates of the second point of the cylinder axis
  * @param radius (in) : the radius of the sphere
  * @param size (in) : the size to be used inside the sphere
  * @return error code (STATUS_OK means that size was set)
  */
MESHGEMS_METHOD meshgems_status_t meshgems_sizemap_creator_add_constraint_on_cylinder(meshgems_sizemap_creator_t *sc, meshgems_real p1[3],
                                                                                      meshgems_real p2[3], meshgems_real radius, meshgems_real size);

/**
 * Add a new constraint : specify a size inside an hexahedron.
 * @param sc (in) : the working sizemap creator object
 * @param v1 (in) : the coordinates of the first vertex of the hexahedron
 * @param v2 (in) : the coordinates of the second vertex of the hexahedron
 * @param v3 (in) : the coordinates of the third vertex of the hexahedron
 * @param v4 (in) : the coordinates of the fourth vertex of the hexahedron
 * @param v5 (in) : the coordinates of the fifth vertex of the hexahedron
 * @param v6 (in) : the coordinates of the sixth vertex of the hexahedron
 * @param v7 (in) : the coordinates of the seventh vertex of the hexahedron
 * @param v8 (in) : the coordinates of the eighth vertex of the hexahedron
 * @param size (in) : the size to be used inside the sphere
 * @return error code (STATUS_OK means that size was set)
 */
MESHGEMS_METHOD meshgems_status_t meshgems_sizemap_creator_add_constraint_on_hexahedron(meshgems_sizemap_creator_t *sc, meshgems_real v1[3],
                                                                                        meshgems_real v2[3], meshgems_real v3[3], meshgems_real v4[3],
                                                                                        meshgems_real v5[3], meshgems_real v6[3], meshgems_real v7[3],
                                                                                        meshgems_real v8[3], meshgems_real size);

/**
 * Add a new constraint : specify a size inside a cylinder.
 * @param sc (in) : the working sizemap creator object
 * @param p (in) : the coordinates of the point
 * @param size (in) : the size to be used inside the sphere
 * @return error code (STATUS_OK means that size was set)
 */
MESHGEMS_METHOD meshgems_status_t meshgems_sizemap_creator_add_constraint_on_point(meshgems_sizemap_creator_t *sc, meshgems_real p[3],
                                                                                   meshgems_real size);

/**
 * Effectively create a sizemap out of the sizemap creator.
 * Acts as a \ref meshgems_sizemap_t constructor.
 * @param sc (in) : the working sizemap creator object.
 * @return a new meshgems_sizemap_t object embeding the constraints
 *  stored in sc (or NULL in case of failure).
 */
MESHGEMS_METHOD meshgems_sizemap_t *meshgems_sizemap_creator_generate_sizemap(meshgems_sizemap_creator_t *sc);

/** Private part of the mesh interface. Only Distene products should use it. **/


#endif
