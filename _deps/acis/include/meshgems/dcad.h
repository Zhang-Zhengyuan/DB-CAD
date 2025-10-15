#ifndef __MESHGEMS_DCAD_H__
#define __MESHGEMS_DCAD_H__

/**
 *
 * \defgroup dcad meshgems_dcad_t
 * @{
 *
 * Structure to describe a (partially) discretized CAD object (DCAD).
 * The \ref meshgems_dcad_t object can be seen as an extension of the \ref
 * meshgems_cad_t object.
 * The only part implemented for the moment are the \ref dcad_edge_discretization
 * and \ref dcad_wire_discretization  which allow to specify respectively
 * a required discretization on a given set of CAD edges or CAD wires.
 *
 *
 * The basic working principle of this structure is that once created with \ref meshgems_dcad_new,
 * one can get the required discretization structure corresponding to a given CAD entity with
 * \ref meshgems_dcad_get_edge_discretization or \ref meshgems_dcad_get_wire_discretization. It is then
 * possible to specify the discretization of the CAD entity directly in the obtained structure.
 * See \ref meshgems_dcad_edge_discretization_t \ref meshgems_dcad_wire_discretization_t for further details.
 *
 */

/**
 * \defgroup dcad_face_discretization meshgems_dcad_face_discretization_t
 * \ingroup dcad
 * This object stores the data from a CAD face discretization. It belongs to a DCAD object.
 * Not yet implemented.
 *
 * \defgroup dcad_edge_discretization meshgems_dcad_edge_discretization_t
 * \ingroup dcad
 * This object stores the data from a CAD edge discretization. It belongs to a DCAD object.
 * To specify a required discretization on a CAD edge, one has to set the number of vertex on the
 * edge (including extremities) with \ref meshgems_dcad_edge_discretization_set_vertex_count and then
 * set these vertices one by one with \ref meshgems_dcad_edge_discretization_set_vertex_coordinates
 * and \ref meshgems_dcad_edge_discretization_set_vertex_tag.
 *
 * It is possible to specify that the extremities
 * should not be present in the generated mesh by setting the \ref MESHGEMS_DCAD_PROPERTY_GHOST_EXTREMITY_TMIN
 * and/or \ref MESHGEMS_DCAD_PROPERTY_GHOST_EXTREMITY_TMAX properties using the function
 *  \ref meshgems_dcad_edge_discretization_set_property.
 *
 *
 * \defgroup dcad_wire_discretization meshgems_dcad_wire_discretization_t
 * \ingroup dcad
 * This object stores the data from a DCAD wire discretization. It belongs to a DCAD object.
 * To specify a required discretization on a CAD wire, one has to set the number of vertex on the
 * wire (including extremities) with \ref meshgems_dcad_wire_discretization_set_vertex_count and then
 * set these vertices one by one with \ref meshgems_dcad_wire_discretization_set_vertex_coordinates
 * and \ref meshgems_dcad_wire_discretization_set_vertex_tag.
 *
 *
 */

struct meshgems_dcad_t_;
struct meshgems_dcad_edge_discretization_t_;

/**
 * \ingroup dcad
 * Opaque type to store a (partially or completely) discretized CAD data.
 */
typedef struct meshgems_dcad_t_ meshgems_dcad_t;

/**
 * \ingroup dcad_face_discretization
 * Opaque type to store a CAD face discretization.
 */
typedef struct meshgems_dcad_face_discretization_t_ meshgems_dcad_face_discretization_t;

/**
 * \ingroup dcad_edge_discretization
 * Opaque type to store a CAD edge discretization.
 */
typedef struct meshgems_dcad_edge_discretization_t_ meshgems_dcad_edge_discretization_t;

/**
 * \ingroup dcad_wire_discretization
 * Opaque type to store a CAD wire discretization.
 */
typedef struct meshgems_dcad_wire_discretization_t_ meshgems_dcad_wire_discretization_t;

#include <meshgems/mdecl.h>
#include <meshgems/basic_types.h>
#include <meshgems/status.h>
#include <meshgems/context.h>
#include <meshgems/cad.h>

/**
 * \ingroup dcad
 * Simple DCAD constructor.
 * @param c (in) : the associated CAD object
 * @return a new dcad object or NULL.
 */
MESHGEMS_METHOD meshgems_dcad_t *meshgems_dcad_new(meshgems_cad_t *c);

/**
 * \ingroup dcad
 * DCAD object destructor.
 */
MESHGEMS_METHOD void meshgems_dcad_delete(meshgems_dcad_t *d);

/**
 * \ingroup dcad_edge_discretization
 * Get the CAD edge discretization corresponding to a CAD edge.
 * @param d (in) : the working DCAD object
 * @param e (in) : the working CAD edge object
 * @param de (out) : the CAD edge discretization object
 * @return a status code.
 */
MESHGEMS_METHOD meshgems_status_t meshgems_dcad_get_edge_discretization(
    meshgems_dcad_t *d, meshgems_cad_edge_t *e,
    meshgems_dcad_edge_discretization_t **de);

/**
 * \ingroup dcad_edge_discretization
 * CAD edge discretization set vertrex count.
 * @param d (in) : the working DCAD object edge discretization object
 * @param n (in) : the number of vertex in the edge discretization
 * @return a status code.
 */
MESHGEMS_METHOD meshgems_status_t meshgems_dcad_edge_discretization_set_vertex_count(
    meshgems_dcad_edge_discretization_t *d, meshgems_integer n);
/**
 * \ingroup dcad_edge_discretization
 * CAD edge discretization get vertrex count.
 * @param d (in) : the working DCAD object edge discretization object
 * @param n (out) : the number of vertex in the edge discretization
 * @return a status code.
 */
MESHGEMS_METHOD meshgems_status_t meshgems_dcad_edge_discretization_get_vertex_count(
    meshgems_dcad_edge_discretization_t *d, meshgems_integer *n);

/**
 * \ingroup dcad_edge_discretization
 * CAD edge discretization set vertex coordinates.
 * @param d (in) : the working DCAD object edge discretization object
 * @param i (in) : the indice of the desired vertex in the edge discretization
 * @param t (in) : the t coordinates of the vertex (size : 1 real)
 * @param uv (in) : the uv coordinates of the vertex (size : 2 real)
 * @param xyz (in) : the xyz coordinates of the vertex (size : 3 real)
 * @return a status code.
 */
MESHGEMS_METHOD meshgems_status_t meshgems_dcad_edge_discretization_set_vertex_coordinates(
    meshgems_dcad_edge_discretization_t *d, meshgems_integer i, meshgems_real t,
    meshgems_real *uv, meshgems_real *xyz);

/**
 * \ingroup dcad_edge_discretization
 * CAD edge discretization get vertex coordinates.
 * @param d (in) : the working DCAD object edge discretization object
 * @param i (in) : the indice of the queried vertex in the edge discretization
 * @param t (out) : the t coordinates of the vertex (size : 1 real)
 * @param uv (out) : the uv coordinates of the vertex (size : 2 real)
 * @param xyz (out) : the xyz coordinates of the vertex (size : 3 real)
 * @return a status code.
 */
MESHGEMS_METHOD meshgems_status_t meshgems_dcad_edge_discretization_get_vertex_coordinates(
    meshgems_dcad_edge_discretization_t *d, meshgems_integer i,
    meshgems_real *t, meshgems_real *uv, meshgems_real *xyz);

/**
 * \ingroup dcad_edge_discretization
 * CAD edge discretization set vertex tag.
 * @param d (in) : the working DCAD object edge discretization object
 * @param i (in) : the indice of the desired vertex in the edge discretization
 * @param tag (in) : the tag of the vertex (size : 1 integer)
 * @return a status code.
 */
MESHGEMS_METHOD meshgems_status_t meshgems_dcad_edge_discretization_set_vertex_tag(
    meshgems_dcad_edge_discretization_t *d, meshgems_integer i,
    meshgems_integer tag);

/**
 * \ingroup dcad_edge_discretization
 * CAD edge discretization get vertex tag.
 * @param d (in) : the working DCAD object edge discretization object
 * @param i (in) : the indice of the queried vertex in the edge discretization
 * @param tag (out) : the tag of the vertex (size : 1 integer)
 * @return a status code.
 */
MESHGEMS_METHOD meshgems_status_t meshgems_dcad_edge_discretization_get_vertex_tag(
    meshgems_dcad_edge_discretization_t *d, meshgems_integer i,
    meshgems_integer *tag);

/**
 * \ingroup dcad_edge_discretization
 * CAD edge discretization set property.
 * @param d (in) : the working DCAD edge discretization object
 * @param property (in) : the properties to set on the edge discretization
 * @return a status code.
 */
MESHGEMS_METHOD meshgems_status_t meshgems_dcad_edge_discretization_set_property(
    meshgems_dcad_edge_discretization_t *d, meshgems_integer property);

/**
 * \ingroup dcad_edge_discretization
 * CAD edge discretization query property.
 * @param d (in) : the working DCAD edge discretization object
 * @param property (in) : the properties to query  on the edge discretization
 * @param has_property (out) : 1 if the edge discretization has the property or 0 otherwise
 * @return a status code.
 */
MESHGEMS_METHOD meshgems_status_t meshgems_dcad_edge_discretization_query_property(
    meshgems_dcad_edge_discretization_t *d, meshgems_integer property,
    meshgems_integer *has_property);

/**
 * \ingroup dcad_wire_discretization
 * Get the CAD wire discretization corresponding to a CAD wire.
 * @param d (in) : the working DCAD object
 * @param w (in) : the working CAD wire object
 * @param dw (out) : the CAD wire discretization object
 * @return a status code.
 */
MESHGEMS_METHOD meshgems_status_t meshgems_dcad_get_wire_discretization(
    meshgems_dcad_t *d, meshgems_cad_wire_t *w,
    meshgems_dcad_wire_discretization_t **dw);

/**
 * \ingroup dcad_wire_discretization
 * CAD wire discretization set vertex count.
 * @param d (in) : the working DCAD object wire discretization object
 * @param n (in) : the number of vertex in the wire discretization
 * @return a status code.
 */
MESHGEMS_METHOD meshgems_status_t meshgems_dcad_wire_discretization_set_vertex_count(
    meshgems_dcad_wire_discretization_t *d, meshgems_integer n);
/**
 * \ingroup dcad_wire_discretization
 * CAD wire discretization get vertex count.
 * @param d (in) : the working DCAD object wire discretization object
 * @param n (out) : the number of vertex in the wire discretization
 * @return a status code.
 */
MESHGEMS_METHOD meshgems_status_t meshgems_dcad_wire_discretization_get_vertex_count(
    meshgems_dcad_wire_discretization_t *d, meshgems_integer *n);

/**
 * \ingroup dcad_wire_discretization
 * CAD wire discretization set vertex coordinates.
 * @param d (in) : the working DCAD object wire discretization object
 * @param i (in) : the indice of the desired vertex in the wire discretization
 * @param t (in) : the t coordinates of the vertex (size : 1 real)
 * @param xyz (in) : the xyz coordinates of the vertex (size : 3 real)
 * @return a status code.
 */
MESHGEMS_METHOD meshgems_status_t meshgems_dcad_wire_discretization_set_vertex_coordinates(
    meshgems_dcad_wire_discretization_t *d, meshgems_integer i, meshgems_real t,
    meshgems_real *xyz);

/**
 * \ingroup dcad_wire_discretization
 * CAD wire discretization get vertex coordinates.
 * @param d (in) : the working DCAD object wire discretization object
 * @param i (in) : the indice of the queried vertex in the wire discretization
 * @param t (out) : the t coordinates of the vertex (size : 1 real)
 * @param xyz (out) : the xyz coordinates of the vertex (size : 3 real)
 * @return a status code.
 */
MESHGEMS_METHOD meshgems_status_t meshgems_dcad_wire_discretization_get_vertex_coordinates(
    meshgems_dcad_wire_discretization_t *d, meshgems_integer i,
    meshgems_real *t, meshgems_real *xyz);

/**
 * \ingroup dcad_wire_discretization
 * CAD wire discretization set vertex tag.
 * @param d (in) : the working DCAD object wire discretization object
 * @param i (in) : the indice of the desired vertex in the wire discretization
 * @param tag (in) : the tag of the vertex (size : 1 integer)
 * @return a status code.
 */
MESHGEMS_METHOD meshgems_status_t meshgems_dcad_wire_discretization_set_vertex_tag(
    meshgems_dcad_wire_discretization_t *d, meshgems_integer i,
    meshgems_integer tag);

/**
 * \ingroup dcad_wire_discretization
 * CAD wire discretization get vertex tag.
 * @param d (in) : the working DCAD object wire discretization object
 * @param i (in) : the indice of the queried vertex in the wire discretization
 * @param tag (out) : the tag of the vertex (size : 1 integer)
 * @return a status code.
 */
MESHGEMS_METHOD meshgems_status_t meshgems_dcad_wire_discretization_get_vertex_tag(
    meshgems_dcad_wire_discretization_t *d, meshgems_integer i,
    meshgems_integer *tag);

/**
 * \ingroup dcad_wire_discretization
 * CAD wire discretization set property.
 * @param d (in) : the working DCAD wire discretization object
 * @param property (in) : the properties to set on the wire discretization
 * @return a status code.
 */
MESHGEMS_METHOD meshgems_status_t meshgems_dcad_wire_discretization_set_property(
    meshgems_dcad_wire_discretization_t *d, meshgems_integer property);

/**
 * \ingroup dcad_wire_discretization
 * CAD wire discretization query property.
 * @param d (in) : the working DCAD wire discretization object
 * @param property (in) : the properties to query  on the wire discretization
 * @param has_property (out) : 1 if the wire discretization has the property or 0 otherwise
 * @return a status code.
 */
MESHGEMS_METHOD meshgems_status_t meshgems_dcad_wire_discretization_query_property(
    meshgems_dcad_wire_discretization_t *d, meshgems_integer property,
    meshgems_integer *has_property);

/** Property marking something required in a DCAD */
#define MESHGEMS_DCAD_PROPERTY_REQUIRED 0x01

/** Property marking a ghost t_min extremity in a DCAD edge discretization */
#define MESHGEMS_DCAD_PROPERTY_GHOST_EXTREMITY_TMIN 0x02

/** Property marking a ghost t_max extremity in a DCAD edge discretization */
#define MESHGEMS_DCAD_PROPERTY_GHOST_EXTREMITY_TMAX 0x04

/** Property marking that only the XYZ coordinates are available for a given
 * discretization vertex */
#define MESHGEMS_DCAD_PROPERTY_XYZ_ONLY 0x08

/**@}*/

/** Private part of the CAD interface. Only Distene products should use it. **/

MESHGEMS_METHOD meshgems_status_t meshgems_dcad_get_cad(meshgems_dcad_t *dc,
                                                        meshgems_cad_t **c);

MESHGEMS_METHOD meshgems_status_t meshgems_dcad_query_edge_discretization(
    meshgems_dcad_t *d, meshgems_cad_edge_t *e,
    meshgems_dcad_edge_discretization_t **de);

MESHGEMS_METHOD meshgems_status_t meshgems_dcad_query_wire_discretization(
    meshgems_dcad_t *d, meshgems_cad_wire_t *w,
    meshgems_dcad_wire_discretization_t **dw);

#endif
