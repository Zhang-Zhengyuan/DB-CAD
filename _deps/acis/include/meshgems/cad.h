#ifndef __MESHGEMS_CAD_H__
#define __MESHGEMS_CAD_H__

/**
 * \defgroup cad meshgems_cad_t
 *
 * @{
 *
 * Abstract structure to store a CAD geometry description.
 *
 * This structure embeds the geometry and the topology descriptions of the CAD:
 * - The geometry is descibed through parameterization functions (see \ref meshgems_cad_surf_t or
 *   \ref meshgems_cad_curv_t for example )
 * - The topology is described :
 * - with pointers to the upper level structure, given at construction time (eg : a CAD edge is
 *   constructed with a pointer to the CAD face it belongs to)
 * - with strictly positiv integer ids to describe how same dimension entities are connected
 *   (eg : two points with the same id are topologically identical)
 *
 * The construction of full CAD object is hierachical:
 * - The CAD root object can be created by the \ref meshgems_cad_new constructor.
 * - CAD faces can then be added to this CAD using the \ref meshgems_cad_face_new constructor.
 *   The created \ref meshgems_cad_face_t can then be manipulated using the
 *   meshgems_cad_face_* functions. See \ref cad_face for details on this object.
 * - CAD edges can be added to this CAD face using the \ref meshgems_cad_edge_new or
 *   \ref meshgems_cad_edge_new0 constructors. The created \ref meshgems_cad_face_t
 *   can then be manipulated using the meshgems_cad_edge_* functions. See \ref cad_edge
 *   for details on this object.
 * - CAD wires can be added to this CAD using the \ref meshgems_cad_wire_new constructor.
 *   The created \ref meshgems_cad_wire_t can then be manipulated using the
 *   meshgems_cad_wire_* functions. See \ref cad_wire for details on this object.
 *
 *
 * It is also possible to describe a periodicity property in the CAD (eg : two faces should be periodic
 * with each other). See \ref cad_periodicity for details on this topic.
 */

/**
 * \defgroup cad_face meshgems_cad_face_t
 * \ingroup cad
 * This object stores all the data from a CAD face. It belongs to a CAD object and
 * shall be constructed with \ref meshgems_cad_face_new
 *
 *
 * \defgroup cad_polygonal_face meshgems_cad_polygonal_face_t
 * \ingroup cad
 * This object stores all the data from a polygonal CAD face.
 * It is most often for Distene internal use only.
 *
 *
 * \defgroup cad_edge meshgems_cad_edge_t
 * \ingroup cad
 * This object stores all the data from a CAD edge. It belongs to a CAD face and
 * shall be constructed with \ref meshgems_cad_edge_new or \ref meshgems_cad_edge_new0.
 *
 *
 * \defgroup cad_point meshgems_cad_point_t
 * \ingroup cad
 * This object stores all the data from a CAD point internal to a CAD face. It
 *  belongs to a CAD face and shall be constructed with \ref meshgems_cad_point_new.
 *
 *
 * \defgroup cad_vertex meshgems_cad_vertex_t
 * \ingroup cad
 * This object stores all the data from a CAD vertex.
 * It belongs to a CAD and shall be constructed with \ref meshgems_cad_vertex_new.
 *
 *
 * \defgroup cad_wire meshgems_cad_wire_t
 * \ingroup cad
 * This object stores all the data from a CAD wire. It belongs to a CAD and
 * shall be constructed with \ref meshgems_cad_wire_new.
 *
 *
 * \defgroup cad_periodicity CAD periodicity
 * \ingroup cad
 * Powerfull functions to define a generic periodicity.
 *
 * It is possible to describe a periodicity property in the CAD, between two set of CAD faces.
 * This can be done through the following functions:
 * - \ref meshgems_cad_add_face_multiple_periodicity_with_transformation_function
 * - \ref meshgems_cad_add_face_multiple_periodicity_with_transformation_function_by_points
 * - \ref meshgems_cad_add_edge_multiple_periodicity_with_transformation_function
 * - \ref meshgems_cad_add_edge_multiple_periodicity_with_transformation_function_by_points
 *
 * If you are using an above function ending with "_function" to set the periodicity, it is
 * necessary to provide the exact transforamtion function (see \ref meshgems_cad_periodicity_transformation_t).
 * On the other hand, if you are using a function ending with "_function_by_points", MeshGems will
 * try to guess the desired transformation depending on the number of given points (see each function documentation
 * for the precise rules).
 *
 *
 * \defgroup cad_basic_periodicity Basic CAD periodicity
 * \ingroup cad
 * Basic functions to define periodicity (internal use only).
 *
 *
 */

struct meshgems_cad_t_;
struct meshgems_cad_face_t_;
struct meshgems_cad_polygonal_face_t_;
struct meshgems_cad_edge_t_;
struct meshgems_cad_point_t_;
struct meshgems_cad_vertex_t_;

/**
 * \ingroup cad
 * Opaque type to store a CAD object.
 * This object is the root object.
 */
typedef struct meshgems_cad_t_ meshgems_cad_t;

/**
 * \ingroup cad_face
 * Opaque type to store a CAD face (patch).
 */
typedef struct meshgems_cad_face_t_ meshgems_cad_face_t;

/**
 * \ingroup cad_polygonal_face
 * Opaque type to store a CAD polynomial face (patch).
 */
typedef struct meshgems_cad_polygonal_face_t_ meshgems_cad_polygonal_face_t;

/**
 * \ingroup cad_edge
 * Opaque type to store a CAD edge (curve).
 */
typedef struct meshgems_cad_edge_t_ meshgems_cad_edge_t;

/**
 * \ingroup cad_point
 * Opaque type to store a CAD point.
 */
typedef struct meshgems_cad_point_t_ meshgems_cad_point_t;

/**
 * \ingroup cad_vertex
 * Opaque type to store a CAD vertex.
 */
typedef struct meshgems_cad_vertex_t_ meshgems_cad_vertex_t;

/**
 * \ingroup cad_wire
 * Opaque type to store a CAD wire (pure 3D curve).
 */
typedef struct meshgems_cad_wire_t_ meshgems_cad_wire_t;

#include <meshgems/mdecl.h>
#include <meshgems/basic_types.h>
#include <meshgems/status.h>
#include <meshgems/context.h>
#include <meshgems/mesh.h>

/**
 * \ingroup cad_edge
 * Type for functions that define a curve parametrization.
 * @param t (in) : the 'time value' 
 * @param uv (out): a real[2] array filled with \f$ (uv[0],uv[1]) = (u,v) = curv(t) \f$
 * @param dt (out) : a real[2] array filled with
 * \f[ (dt[0],dt[1]) = \left(\frac{\partial f_u}{{\partial t}}, \frac{\partial f_v }{{\partial t}}\right)(t) \f]
 * @param dtt (out) : a real[2] array filled with
 * \f[ (dtt[0],dtt[1]) = \left(\frac{\partial^2 f_u}{{\partial t^2}}, \frac{\partial^2 f_v }{{\partial t^2}}\right)(t) \f]
 * where \f$ f = (f_u, f_v)= curv \f$
 * @param user_data (in) : a generic user pointer

 * @return a status code
 */
typedef meshgems_status_t (*meshgems_cad_curv_t)(meshgems_real t,
                                                 meshgems_real *uv,
                                                 meshgems_real *dt,
                                                 meshgems_real *dtt,
                                                 void *user_data);

/**
 * \ingroup cad_wire
 * Type for functions that define a wire parametrization.
 * @param t (in) : the 'time value'
 * @param xyz (out): a real[3] array filled with \f$ (xyz[0],xyz[1],xyz[2]) = (x,y,z) = curv(t) \f$
 * @param dt (out) : a real[3] array filled with
 * \f[ (dt[0],dt[1,dt[2]) = \left(\frac{\partial f_x}{{\partial t}}, \frac{\partial f_y }{{\partial t}}, \frac{\partial f_z }{{\partial t}}\right)(t) \f]
 * @param dtt (out) : a real[2] array filled with
 * \f[ (dtt[0],dtt[1]) = \left(\frac{\partial^2 f_x}{{\partial t^2}}, \frac{\partial^2 f_y }{{\partial t^2}}, \frac{\partial^2 f_z }{{\partial t^2}}\right)(t) \f]
 * where \f$ f = (f_x, f_y, f_z)= curv \f$
 * @param user_data (in) : a generic user pointer
 * @return a status code
 */
typedef meshgems_status_t (*meshgems_cad_curv3d2_t)(meshgems_real t,
                                                    meshgems_real *xyz,
                                                    meshgems_real *dt,
                                                    meshgems_real *dtt,
                                                    void *user_data);

/**
 * \ingroup cad_edge
 * Type for functions that define a 3D curve parametrization.
 * @param t (in) : the 'time value'
 * @param xyz (out): a real[3] array filled with \f$ (xyz[0],xyz[1],xyz[2]) = (x,y,z) = curv(t) \f$
 * @param user_data (in) : a generic user pointer
 * @return a status code
 */
typedef meshgems_status_t (*meshgems_cad_curv3d_t)(meshgems_real t,
                                                   meshgems_real *xyz,
                                                   void *user_data);

/**
 * \ingroup cad_face
 * Type for functions that define a surface parametrization.
 * @param uv (in) : a real[2] array filled with the parametric location. 
 * @param user_data (in) : a generic user pointer
 * @param xyz (out) : a real[3] array filled with
 * @param du (out) : a real[3] array filled with
 * \f[ (du[0], du[1], du[2]) = \left(\frac{\partial f_x}{{\partial u}}, \frac{\partial f_y }{{\partial u}}, \frac{\partial f_z }{{\partial u}}\right)(t) \f]
 * @param dv (out) : a real[3] array filled with
 * \f[ (dv[0], dv[1], dv[2]) = \left(\frac{\partial f_x}{{\partial v}}, \frac{\partial f_y }{{\partial v}}, \frac{\partial f_z }{{\partial v}}\right)(t) \f] 
 * @param duu (out): a real[3] array filled with
 * \f[ (duu[0],duu[1],duu[3]) = \left(\frac{\partial^2 f_x}{{\partial u^2}}, \frac{\partial^2 f_y }{{\partial u^2}}, \frac{\partial^2 f_z }{{\partial u^2}}\right)(t) \f]
 * @param duv : a real[3] array filled with
 * \f[ (duv[0], duv[1],duv[3]) =  \left(\frac{\partial^2 f_x}{{\partial u \partial v}}, \frac{\partial^2 f_y }{{\partial u \partial v}}, \frac{\partial^2 f_z }{{\partial u \partial v}}\right)(t) \f]
 * @param dvv : a real[3] array filled with
 * \f[ (dvv[0],dvv[1],duv[3]) = \left(\frac{\partial^2 f_x}{{\partial v^2}}, \frac{\partial^2 f_y }{{\partial v^2}}, \frac{\partial^2 f_z }{{\partial v^2}}\right)(t) \f]

 * where \f$ f = (f_x, f_y, f_z)= surf \f$ 
 * @return a status code
 *
 */
typedef meshgems_status_t (*meshgems_cad_surf_t)(meshgems_real *uv,
                                                 meshgems_real *xyz,
                                                 meshgems_real*du,
                                                 meshgems_real *dv,
                                                 meshgems_real *duu,
                                                 meshgems_real *duv,
                                                 meshgems_real *dvv,
                                                 void *user_data);

/**
 * \ingroup cad_periodicity
 * Type for functions that define a periodicity transformation between two set of faces.
 * @param xyz (in) : a real[3] array filled with the xyz coordinate of a point
 * @param xyz_image (out) : a real[3] array filled with the xyz coordinate of the image of xyz by the transformation
 * @param user_data (in) : a generic user pointer
 * @return a status code
 */
typedef meshgems_status_t (*meshgems_cad_periodicity_transformation_t)(
    meshgems_real *xyz, meshgems_real *xyz_image, void *user_data);

/**
 * Simple CAD constructor.
 * @param ctx (in) : the associated context
 * @return a new cad object or NULL.
 */
MESHGEMS_METHOD meshgems_cad_t *meshgems_cad_new(meshgems_context_t *ctx);

/**
 * CAD object destructor.
 */
MESHGEMS_METHOD void meshgems_cad_delete(meshgems_cad_t *c);

/**
 * \ingroup cad_face
 * cad_face object constructor. Adds the newly created face to the cad object
 * @param cad (in) : the associated cad object
 * @param fid (in) : the face id
 * @param surf (in) : the parametric definition of this surface
 * @param user_data (in) : the user pointer surf0,surf1 and surf2 will be given
 * @return a new cad_face object or NULL.
 */
MESHGEMS_METHOD meshgems_cad_face_t *meshgems_cad_face_new(
    meshgems_cad_t *cad, meshgems_integer fid, meshgems_cad_surf_t surf,
    void *user_data);

/**
 * \ingroup cad_face
 * cad_face object constructor. The face is defined by a discrete definition. 
 * For Hybrid CAD use only. Adds the newly created face to the cad object.
 * @param cad (in) : the associated cad object
 * @param fid (in) : the face id
 * @param m2d (in) : Optionnal. The mesh in the parametric space
 * @param m3d (in) : A mesh describing the geometry of the CAD face
 * @return a new cad_face object or NULL.
 */
MESHGEMS_METHOD meshgems_cad_face_t *meshgems_cad_face_new_discrete(
    meshgems_cad_t *cad, meshgems_integer fid, meshgems_mesh_t *m2d, meshgems_mesh_t *m3d);

/**
 * \ingroup cad_face
 * Destructor. Does not remove the face from the cad object.
 */
MESHGEMS_METHOD void meshgems_cad_face_delete(meshgems_cad_face_t *face);

/**
 * \ingroup cad_face
 * Removes the cad face from its associated cad object.
 * Does not delete the cad face object.
 */
MESHGEMS_METHOD void meshgems_cad_face_remove(meshgems_cad_face_t *face);

/**
 * \ingroup cad_face
 * Set the tag of a face.
 * @param face (in) : the working face
 * @param tag (in) : the tag to set
 * @return a status code
 */
MESHGEMS_METHOD meshgems_status_t meshgems_cad_face_set_tag(
    meshgems_cad_face_t *face, meshgems_integer tag);

/**
 * \ingroup cad_face
 * Set the uid of a face. The uid should identify the face uniquely in all the CAD.
 * (usefull to do mesh CAD associativity for instance)
 * It is different of the face id which is a topological attribute.
 * @param face (in) : the working face
 * @param uid (in) : the uid to set
 * @return a status code
 */
MESHGEMS_METHOD meshgems_status_t meshgems_cad_face_set_uid(
    meshgems_cad_face_t *face, meshgems_integer uid);

/**
 * \ingroup cad_face
 * Set the orientation of a face.
 * @param face (in) : the working face
 * @param orientation (in) : the orientation of the face : CAD_ORIENTATION_FORWARD or  CAD_ORIENTATION_REVERSED
 *   or CAD_ORIENTATION_UNKNOWN (default).
 * @return a status code
 */
MESHGEMS_METHOD meshgems_status_t meshgems_cad_face_set_orientation(
    meshgems_cad_face_t *face, meshgems_integer orientation);

/**
 * \ingroup cad_face
 * Set the surface periodicity attibute of a face.
 * @param face (in) : the working face
 * @param is_periodic_u (in) : 1 if the underlying surface is periodic in "u" direction, 0 else.
 * @param is_periodic_v (in) : 1 if the underlying surface is periodic in "v" direction, 0 else.
 * @param period_u (in) : in case is_periodic_u == 1, the period of the surface in "u" direction.
 * @param period_v (in) : in case is_periodic_v == 1, the period of the surface in "v" direction.
 * @return a status code
 */
MESHGEMS_METHOD meshgems_status_t meshgems_cad_face_set_surface_periodicity(
    meshgems_cad_face_t *face, meshgems_integer is_periodic_u,
    meshgems_integer is_periodic_v, meshgems_real period_u,
    meshgems_real period_v);

/**
 * \ingroup cad_face
 * Set the parametric range of the surface a face lies on.
 * @param face (in) : the working face
 * @param range_u (in) : a real[2] array describing the parametric range of the face in "u" direction (u_min, u_max).
 * @param range_v (in) : a real[2] array describing the parametric range of the face in "v" direction (v_min, v_max).
 * @return a status code
 */
MESHGEMS_METHOD meshgems_status_t meshgems_cad_face_set_surface_parametric_range(
    meshgems_cad_face_t *face, meshgems_real *range_u, meshgems_real *range_v);

/**
 * \ingroup cad_face
 * Set the property of a face.
 * @param face (in) : the working face
 * @param property (in) : the property to set.
 * @return a status code
 */
MESHGEMS_METHOD meshgems_status_t meshgems_cad_face_set_property(
  meshgems_cad_face_t *face, meshgems_integer property);

/**
 * \ingroup cad_face
 * Unset a property of a face.
 * @param face (in) : the working face
 * @param property (in) : the property to unset
 * @return a status code
 */
MESHGEMS_METHOD meshgems_status_t meshgems_cad_face_unset_property(
  meshgems_cad_face_t *face, meshgems_integer property);

/**
 * \ingroup cad_face
 * Queries a property of a face.
 * @param face (in) : the working face
 * @param property (in) : the property to query
 * @param has_property (out) : 1 if the face has the property, 0 otherwise
 * @return a status code
 */
MESHGEMS_METHOD meshgems_status_t meshgems_cad_face_query_property(
  meshgems_cad_face_t *face, meshgems_integer property,
  meshgems_integer *has_property);

/**
 * \ingroup cad_face
 * Impose the discrete geometry on this discrete CAD face.
 * @param face (in) : the working face
 * @return a status code
 */
MESHGEMS_METHOD meshgems_status_t meshgems_cad_face_impose_discrete_geometry(meshgems_cad_face_t *face);

/**
 * \ingroup cad_edge
 * Constructor. Adds the new edge to the associated cad_face object
 * @param face (in) : the associated cad_face object supporting the edge
 * @param eid (in) : the edge id
 * @param tmin (in) : the min boundary of the parametric range for this curve
 * @param tmax (in) :the max boundary of the parametric range for this curve
 * @param curv (in) : the parametric definition of this curve including
 * its first and second order derivatives
 * @param user_data (in) : the user pointer curve function will be given
 * @return a new cad_edge object or NULL.
 */
MESHGEMS_METHOD meshgems_cad_edge_t *meshgems_cad_edge_new(
    meshgems_cad_face_t *face, meshgems_integer eid, meshgems_real tmin,
    meshgems_real tmax, meshgems_cad_curv_t curv, void *user_data);

/**
 * \ingroup cad_edge
 * Constructor. Adds the new edge to the associated cad_face object
 * @param face (in) : the associated cad_face object supporting the edge
 * @param eid (in) : the edge id
 * @param tmin (in) : the min boundary of the parametric range for this curve
 * @param tmax (in) :the max boundary of the parametric range for this curve
 * @param curv (in) : the parametric definition of this curve without
 * its first and second order derivatives
 * @param user_data (in) : the user pointer curve function will be given
 * @return a new cad_edge object or NULL.
 */
MESHGEMS_METHOD meshgems_cad_edge_t *meshgems_cad_edge_new0(
    meshgems_cad_face_t *face, meshgems_integer eid, meshgems_real tmin,
    meshgems_real tmax, meshgems_cad_curv_t curv, void *user_data);

/**
 * \ingroup cad_edge
 * Constructor. Adds the new edge to the associated cad_face object, defined by
 * a discrete definition. For Hybrid CAD use only.
 * @param face (in) : the associated cad_face object supporting the edge
 * @param eid (in) : the edge id
 * @param nv (in) : the nuber of points describing this discrete edge
 * @param vtx (in) : the ordered list of point indices in the associated CAD 
 face meshe describing the edge geometry
 * @return a new cad_edge object or NULL.
 */
MESHGEMS_METHOD meshgems_cad_edge_t *meshgems_cad_edge_new_discrete(
    meshgems_cad_face_t *face, meshgems_integer eid, meshgems_integer nv, 
    meshgems_integer *vtx);


/**
 * \ingroup cad_edge
 * Edge destructor. Also removes the edge from the associated cad_face object.
 */
MESHGEMS_METHOD void meshgems_cad_edge_delete(meshgems_cad_edge_t *edge);

/**
 * \ingroup cad_edge
 * Special constructor. Adds a new segment edge to the associated cad_face
 * object. It is parametrized with t in [0..1]
 * @param face (in) : the associated cad_face object supporting the edge
 * @param eid (in) : the edge id
 * @param a (in) : a real[2] array containing the UV-space coordinates
 * of the first segment extremity.
 * @param b (in) : a real[2] array containing the UV-space coordinates
 * of the second segment extremity.
 * @return a new cad_edge object or NULL.
 */
MESHGEMS_METHOD meshgems_cad_edge_t *meshgems_cad_edge_new_segment(
    meshgems_cad_face_t *face, meshgems_integer eid, meshgems_real *a,
    meshgems_real *b);

/**
 * \ingroup cad_edge
 * Set the orientation of an edge in its face.
 * @param edge (in) : the working edge
 * @param orientation (in) : the orientation of the edge in its face : CAD_ORIENTATION_FORWARD or  CAD_ORIENTATION_REVERSED
 *   or CAD_ORIENTATION_UNKNOWN (default).
 * @return a status code
 */
MESHGEMS_METHOD meshgems_status_t meshgems_cad_edge_set_orientation(
    meshgems_cad_edge_t *edge, meshgems_integer orientation);

/**
 * \ingroup cad_edge
 * Set the orientation of an edge in its edge class.
 * @param edge (in) : the working edge
 * @param coorientation (in) : the orientation of the edge in its edge class : CAD_ORIENTATION_FORWARD or  CAD_ORIENTATION_REVERSED
 *   or CAD_ORIENTATION_UNKNOWN (default).
 * @return a status code
 */
MESHGEMS_METHOD meshgems_status_t meshgems_cad_edge_set_coorientation(
    meshgems_cad_edge_t *edge, meshgems_integer coorientation);

/**
 * \ingroup cad_edge
 * Set a property of an edge.
 * @param edge (in) : the working edge
 * @param property (in) : the property to set
 * @return a status code
 */
MESHGEMS_METHOD meshgems_status_t meshgems_cad_edge_set_property(
    meshgems_cad_edge_t *edge, meshgems_integer property);

/**
 * \ingroup cad_edge
 * Unset a property of an edge.
 * @param edge (in) : the working edge
 * @param property (in) : the property to unset
 * @return a status code
 */
MESHGEMS_METHOD meshgems_status_t meshgems_cad_edge_unset_property(
    meshgems_cad_edge_t *edge, meshgems_integer property);

/**
 * \ingroup cad_edge
 * Queries a property of an edge.
 * @param edge (in) : the working edge
 * @param property (in) : the property to query
 * @param has_property (out) : 1 if the edge has the property, 0 otherwise
 * @return a status code
 */
MESHGEMS_METHOD meshgems_status_t meshgems_cad_edge_query_property(
    meshgems_cad_edge_t *edge, meshgems_integer property,
    meshgems_integer *has_property);

/**
 * \ingroup cad_edge
 * Set the extremities references of the edge.
 * @param edge (in) : the working edge
 * @param pid1 (in) : the point id of first extremity
 * @param pid2 (in) : the point id of second extremity
 * @return a status code
 */
MESHGEMS_METHOD meshgems_status_t meshgems_cad_edge_set_extremities(
    meshgems_cad_edge_t *edge, meshgems_integer pid1, meshgems_integer pid2);

/**
 * \ingroup cad_edge
 * Set the extremities tags of the edge.
 * @param edge (in) : the working edge
 * @param tag1 (in) : the tag of first extremity
 * @param tag2 (in) : the tag of second extremity
 * @return a status code
 */
MESHGEMS_METHOD meshgems_status_t meshgems_cad_edge_set_extremities_tag(
    meshgems_cad_edge_t *edge, meshgems_integer tag1, meshgems_integer tag2);

/**
 * \ingroup cad_edge
 * Set the extremities required properties of the edge.
 * @param edge (in) : the working edge
 * @param req1 (in) : the required property of first extremity
 * @param req2 (in) : the required property of second extremity
 * @return a status code
 */
MESHGEMS_METHOD meshgems_status_t meshgems_cad_edge_set_extremities_required_property(meshgems_cad_edge_t *edge,
										      meshgems_integer req1, meshgems_integer req2);

/**
 * \ingroup cad_edge
 * Set the extremities references of the edge in the UV domain.
 * @param edge (in) : the working edge
 * @param uvid1 (in) : the point id of first extremity
 * @param uvid2 (in) : the point id of second extremity
 * @return a status code
 */
MESHGEMS_METHOD meshgems_status_t meshgems_cad_edge_set_uv_extremities(
    meshgems_cad_edge_t *edge, meshgems_integer uvid1, meshgems_integer uvid2);

/**
 * \ingroup cad_edge
 * Set the tag of an edge.
 * @param edge (in) : the working edge
 * @param tag (in) : the tag to set
 * @return a status code
 */
MESHGEMS_METHOD meshgems_status_t meshgems_cad_edge_set_tag(
    meshgems_cad_edge_t *edge, meshgems_integer tag);

/**
 * \ingroup cad_edge
 * Set the uid of an edge. The uid should identify the edge uniquely in all the CAD 
 * (usefull to do mesh CAD associativity for instance)
 * It is different of the edge id which will be the same for all the edges in the same topological class.
 * @param edge (in) : the working edge
 * @param uid (in) : the uid to set
 * @return a status code
 */
MESHGEMS_METHOD meshgems_status_t meshgems_cad_edge_set_uid(
    meshgems_cad_edge_t *edge, meshgems_integer uid);

/**
 * \ingroup cad_point
 * Constructor. Adds the new point to the associated cad_face object
 * @param face (in) : the associated cad_face object supporting the point
 * @param pid (in) : the point id
 * @param uv (in) : a real[2] array filled with point coordinates
 * @return a new cad_point object or NULL.
 */
MESHGEMS_METHOD meshgems_cad_point_t *meshgems_cad_point_new(
    meshgems_cad_face_t *face, meshgems_integer pid, meshgems_real *uv);

/**
 * \ingroup cad_point
 * Constructor. Adds the new point to the associated cad_face object, defined by
 * a discrete definition. For Hybrid CAD use only.
 * @param face (in) : the associated cad_face object supporting the point
 * @param pid (in) : the point id
 * @param idx (in) : the point index in the associated CAD face mesh describing the point geometry
 * @return a new cad_point object or NULL.
 */
MESHGEMS_METHOD meshgems_cad_point_t *meshgems_cad_point_new_discrete(
    meshgems_cad_face_t *face, meshgems_integer pid, meshgems_integer idx);

/**
 * \ingroup cad_point
 * Point destructor. Also removes the point from the associated cad_face object.
 */
MESHGEMS_METHOD void meshgems_cad_point_delete(meshgems_cad_point_t *point);

/**
 * \ingroup cad_point
 * Set the tag of a point.
 * @param point (in) : the working point
 * @param tag (in) : the tag to set
 * @return a status code
 */
MESHGEMS_METHOD meshgems_status_t meshgems_cad_point_set_tag(
    meshgems_cad_point_t *point, meshgems_integer tag);

/**
 * \ingroup cad_vertex
 * Constructor. Adds the new vertex to the associated cad object
 * @param c (in) : the associated cad object supporting the point
 * @param pid (in) : the point id
 * @param xyz (in) : a real[3] array filled with point coordinates
 * @return a new cad_vertex object if no vertex has been created with the same pid or NULL.
 */
MESHGEMS_METHOD meshgems_cad_vertex_t *meshgems_cad_vertex_new(
    meshgems_cad_t *c, meshgems_integer pid, meshgems_real *xyz);

/**
 * \ingroup cad_vertex
 * Set the uid of a vertex. The uid should identify the vertex uniquely in all the CAD.
 * (usefull to do mesh CAD associativity for instance)
 * It is different of the vertex id which is a topological attribute.
 * @param v (in) : the working vertex
 * @param uid (in) : the uid to set
 * @return a status code
 */
MESHGEMS_METHOD meshgems_status_t meshgems_cad_vertex_set_uid(
    meshgems_cad_vertex_t *v, meshgems_integer uid);

/**
 * Set the thread safety attribute of a cad.
 * @param cad (in) : the working cad
 * @param is (in) : 1 if the cad is thread safe, else 0
 * @return a status code
 */
MESHGEMS_METHOD meshgems_status_t meshgems_cad_set_thread_safety(
    meshgems_cad_t *cad, meshgems_integer is);

/**
 * Set the thread safety attribute of a cad.
 * @param cad (in) : the working cad
 * @param is (out) : 1 if the cad is thread safe, else 0
 * @return a status code
 */
MESHGEMS_METHOD meshgems_status_t meshgems_cad_get_thread_safety(
    meshgems_cad_t *cad, meshgems_integer *is);

/**
 *
 * \name Macros defining edge properties
 * @{
 */

/** \ingroup cad_edge
 * Edge property for an internal edge */
#define MESHGEMS_EDGE_PROPERTY_INTERNAL (1 << 0)
/** \ingroup cad_edge
 * Edge property for a required edge */
#define MESHGEMS_EDGE_PROPERTY_REQUIRED  (1 << 1)
/** \ingroup cad_edge
 * Edge property for a soft required edge. Legacy only. Should not be used. */
#define MESHGEMS_EDGE_PROPERTY_SOFT_REQUIRED  (1 << 2)
/** \ingroup cad_edge
 * Edge property for the main edge in an edge class */
#define MESHGEMS_EDGE_PROPERTY_MAIN  (1 << 3)
/** \ingroup cad_edge
 * Edge property for a periodic edge */
#define MESHGEMS_EDGE_PROPERTY_PERIODIC  (1 << 4)
/** Edge property for an artificial edge (seam or alike) */
#define MESHGEMS_EDGE_PROPERTY_ARTIFICIAL  (1 << 5)
/** Edge property for a ridge edge */
#define MESHGEMS_EDGE_PROPERTY_RIDGE  (1 << 6)
/**@}*/

/**
 * \name Macros defining edge orientation
 * @{
 */

/** \ingroup cad_edge
 * Edge orientation is forward : surface is on the left of
 * the curve when going from t_min to t_max */
#define MESHGEMS_CAD_ORIENTATION_FORWARD 1
/** \ingroup cad_edge
 * Edge orientation is unknown */
#define MESHGEMS_CAD_ORIENTATION_UNKNOWN 0
/** \ingroup cad_edge
 * Edge orientation is reversed : surface is on the right of
 * the curve when going from t_min to t_max */
#define MESHGEMS_CAD_ORIENTATION_REVERSED -1
/**@}*/

/**
 *
 * \name Macros defining surface properties
 * @{
 */

 /** \ingroup cad_face
  * Face property for a planar geometry */
#define MESHGEMS_FACE_PROPERTY_PLANAR_SURFACE (1 << 0)
#define MESHGEMS_FACE_PROPERTY_SLIVER (1 << 1)
/**@}*/

/**
 * \ingroup cad_wire
 * Constructor. Adds the new wire to the associated cad object
 * @param cad (in) : the associated cad object supporting the wire
 * @param eid (in) : the wire id
 * @param tmin (in) : the min boundary of the parametric range for this wire
 * @param tmax (in) :the max boundary of the parametric range for this wire
 * @param curv (in) : the parametric definition of this curve
 * @param user_data (in) : the user pointer curve function will be given
 * @return a new cad_wire object or NULL.
 */
MESHGEMS_METHOD meshgems_cad_wire_t *meshgems_cad_wire_new(
    meshgems_cad_t *cad, meshgems_integer eid, meshgems_real tmin,
    meshgems_real tmax, meshgems_cad_curv3d2_t curv, void *user_data);

/**
 * \ingroup cad_wire
 * Constructor. Adds the new wire to the associated cad object, defined by
 * a discrete definition. For Hybrid CAD use only.
 * @param cad (in) : the associated cad object supporting the wire
 * @param eid (in) : the wire id
 * @param m3d (in) : a 1d mesh describing the discrete geometry of the wire
 * @return a new cad_wire object or NULL.
 */
MESHGEMS_METHOD meshgems_cad_wire_t *meshgems_cad_wire_new_discrete(
    meshgems_cad_t *cad, meshgems_integer eid, meshgems_mesh_t *m3d);


/**
 * \ingroup cad_wire
 * Wire destructor. Also removes the wire from the associated cad object.
 */
MESHGEMS_METHOD void meshgems_cad_wire_delete(meshgems_cad_wire_t *wire);

/**
 * \ingroup cad_wire
 * Set the extremities references of the wire.
 * @param wire (in) : the working wire
 * @param pid1 (in) : the point id of first extremity
 * @param pid2 (in) : the point id of second extremity
 * @return a status code
 */
MESHGEMS_METHOD meshgems_status_t meshgems_cad_wire_set_extremities(
    meshgems_cad_wire_t *wire, meshgems_integer pid1, meshgems_integer pid2);

/**
 * \ingroup cad_wire
 * Set the extremities tags of the wire.
 * @param wire (in) : the working wire
 * @param tag1 (in) : the tag of first extremity
 * @param tag2 (in) : the tag of second extremity
 * @return a status code
 */
MESHGEMS_METHOD meshgems_status_t meshgems_cad_wire_set_extremities_tag(
    meshgems_cad_wire_t *wire, meshgems_integer tag1, meshgems_integer tag2);

/**
 * \ingroup cad_wire
 * Set the extremities required properties of the wire.
 * @param wire (in) : the working wire
 * @param req1 (in) : the required property of first extremity
 * @param req2 (in) : the required property of second extremity
 * @return a status code
 */
MESHGEMS_METHOD meshgems_status_t meshgems_cad_wire_set_extremities_required_property(meshgems_cad_wire_t *wire,
										      meshgems_integer req1, meshgems_integer req2);
/**
 * \ingroup cad_wire
 * Set the tag of a wire.
 * @param wire (in) : the working wire
 * @param tag (in) : the tag to set
 * @return a status code
 */
MESHGEMS_METHOD meshgems_status_t meshgems_cad_wire_set_tag(
    meshgems_cad_wire_t *wire, meshgems_integer tag);

/**
 * \ingroup cad_wire
 * Set the uid of a wire. The uid should identify the wire uniquely in all the CAD
 * (usefull to do mesh CAD associativity for instance)
 * It is different of the wire id which is a topological attribute.
 * @param wire (in) : the working wire
 * @param uid (in) : the uid to set
 * @return a status code
 */
MESHGEMS_METHOD meshgems_status_t meshgems_cad_wire_set_uid(
    meshgems_cad_wire_t *wire, meshgems_integer uid);

/**
 * \ingroup cad_basic_periodicity
 * Set the periodicity property between two cad faces given by their id.
 * @param cad (in) : the working cad object
 * @param fid1 (in) : the working cad face id
 * @param fid2 (in) : the cad face id to set periodic with fid1.
 * @return a status code
 */
MESHGEMS_METHOD meshgems_status_t meshgems_cad_add_face_periodicity(
    meshgems_cad_t *cad, meshgems_integer fid1, meshgems_integer fid2);

/**
 * \ingroup cad_basic_periodicity
 * Set the periodicity property between two cad edges given by their id eid1 and eid2.
 * @param cad (in)  : the working cad object
 * @param fid1 (in) : the id of the periodic face eid1 belongs to (can be 0 if eid1 does not belong to a periodic face)
 * @param eid1 (in) : the working cad edge id
 * @param fid2 (in) : the id of the periodic face eid2 belongs to. fid1 and fid2 should have been set periodic before this call  (can be 0)
 * @param eid2 (in) : the cad edge id we want to declare periodic with eid1 (can be 0 if eid2 does not belong to a periodic face).
 * @param orientation (in) : the orientation of the periodicity between eid1 and eid2 (can be set to MESHGEMS_CAD_ORIENTATION_UNKNOWN unless the edges are closed or point periodicity is not given) (see coorientation).
 * @return a status code
 */
MESHGEMS_METHOD meshgems_status_t meshgems_cad_add_edge_periodicity(
    meshgems_cad_t *cad, meshgems_integer fid1, meshgems_integer eid1,
    meshgems_integer fid2, meshgems_integer eid2, meshgems_integer orientation);

/**
 * \ingroup cad_basic_periodicity
 * Set the periodicity property between two points (cad point or edge extremity) given by their id.
 * @param cad (in) : the working cad object
 * @param eid1 (in) : the id of the periodic edge pid1 is an extremity of (can be 0 if pid1 does not belong to a periodic edge).
 * @param pid1 (in) : the working point id
 * @param eid2 (in) : the id of the periodic edge pid2 is an extremity of. eid1 and eid2 should have been set periodic before this call (can be 0 if pid2 does not belong to a periodic edge).
 * @param pid2 (in) : the id of the point we want to declare periodic with pid1.
 * @return a status code
 */
MESHGEMS_METHOD meshgems_status_t meshgems_cad_add_point_periodicity(
    meshgems_cad_t *cad, meshgems_integer eid1, meshgems_integer pid1,
    meshgems_integer eid2, meshgems_integer pid2);

/**
 * \ingroup cad_periodicity
 * Set the periodicity property between two sets of cad faces given by their id, and the associated transformation given by its function.
 * @param cad (in) : the working cad object
 * @param fid1 (in) : the list of the working cad face ids
 * @param size1 (in) : the number of faces in the first group of faces fid1
 * @param fid2 (in) : the list of cad face ids to set periodic with fid1
 * @param size2 (in) : the number of faces in the second group of faces fid2
 * @param transf (in) : the transformation from faces in fid1 to faces in fid2
 * @param user_data (in) : a generic user pointer
 * @return a status code
 */
MESHGEMS_METHOD meshgems_status_t meshgems_cad_add_face_multiple_periodicity_with_transformation_function(
    meshgems_cad_t *cad, meshgems_integer *fid1, meshgems_integer size1,
    meshgems_integer *fid2, meshgems_integer size2,
    meshgems_cad_periodicity_transformation_t transf, void *user_data);

/**
 * \ingroup cad_periodicity
 * Set the periodicity property between two sets of cad faces given by their id, and the associated transformation given by some points.
 * An affine isometry can be fully defined by a collection of 4 non coplanar points and their images. However,
 * some transformations can be guessed with fewer information. Below is the behavior of this function depending on the
 * number of points given as parameter :
 * - one point (and its image) is provided : we suppose that the transformation is a simple translation.
 * - two points (and their images) are provided : we suppose that the transformation is a simple
 *   translation. If it is not coherent with the given coordinates, a STATUS ERROR is returned.
 * - three points (and their images) are provided : we can determine one of the two possible affine
 *   isometries defined by these three couples of points. If the three points are colinear a STATUS ERROR
 *   is returned. Please note that this option is adequate for flat periodic entities (plane or edges wich can
 *   be included in a plane).
 * - four points (and their images) are provided : we can determine the unique affine isometry
 *   defined by the four couples of points. If the four points are coplanar, a STATUS ERROR is returned.
 *
 * @param cad (in) : the working cad object
 * @param fid1 (in) : the list of the working cad face ids
 * @param size1 (in) : the number of faces in the first group of faces fid1
 * @param fid2 (in) : the list of cad face ids to set periodic with fid1
 * @param size2 (in) : the number of faces in the second group of faces fid2
 * @param p (in) : (size 3*nb_pt1) : the xyz coordinates of the point of fid1
 * @param nb_pt1 (in) : the number of points in p
 * @param p_im (in) : (size 3*nb_pt2) : the xyz coordinates of the point of fid2 (periodic with the ones in p)
 * @param nb_pt2 (in) : the number of points in p_im : has to be equal to nb_pt1
 * @return a status code
 */
MESHGEMS_METHOD meshgems_status_t meshgems_cad_add_face_multiple_periodicity_with_transformation_function_by_points(
    meshgems_cad_t *cad, meshgems_integer *fid1, meshgems_integer size1,
    meshgems_integer *fid2, meshgems_integer size2, meshgems_real *p,
    meshgems_integer nb_pt1, meshgems_real *p_im, meshgems_integer nb_pt2);

/**
 * \ingroup cad_periodicity
 * Set the periodicity property between two cad edges given by their id, and the associated transformation given by its function.
 * @param cad (in) : the working cad object
 * @param eid1 (in) : the array of the working cad edge ids
 * @param size1 (in) : the number of edges in the first group of edges eid1
 * @param eid2 (in) : the array of cad edge ids to set periodic with eid1.
 * @param size2 (in) : the number of edges in the second group of edges eid2
 * @param transf (in) : the transformation from edges in eid1 to edges in eid2.
 * @param user_data (in) : a generic user pointer
 * @return a status code
 */
MESHGEMS_METHOD meshgems_status_t meshgems_cad_add_edge_multiple_periodicity_with_transformation_function(
    meshgems_cad_t *cad, meshgems_integer *eid1, meshgems_integer size1,
    meshgems_integer *eid2, meshgems_integer size2,
    meshgems_cad_periodicity_transformation_t transf, void *user_data);

/**
 * \ingroup cad_periodicity
 * Set the periodicity property between two cad edges given by their id, and the associated transformation given by some points.
 * An affine isometry can be fully defined by a collection of 4 non coplanar points and their images. However,
 * some transformations can be guessed with fewer information. Below is the behavior of this function depending on the
 * number of points given as parameter :
 * - one point (and its image) is provided : we suppose that the transformation is a simple translation.
 * - two points (and their images) are provided : we suppose that the transformation is a simple
 *   translation. If it is not coherent with the given coordinates, a STATUS ERROR is returned.
 * - three points (and their images) are provided : we can determine one of the two possible affine
 *   isometries defined by these three couples of points. If the three points are colinear a STATUS ERROR
 *   is returned. Please note that this option is adequate for flat periodic entities (plane or edges wich can
 *   be included in a plane).
 * - four points (and their images) are provided : we can determine the unique affine isometry
 *   defined by the four couples of points. If the four points are coplanar, a STATUS ERROR is returned.
 *
 * @param cad (in) : the working cad object
 * @param eid1 (in) : the array of the working cad edge ids
 * @param size1 (in) : the number of edges in the first group of edges
 * @param eid2 (in) : the array of cad edge ids to set periodic with eid1.
 * @param size2 (in) : the number of edges in the second group of edges
 * @param p (in) : a real[3*nb_pt1] array containing the xyz coordinates of the point of eid1
 * @param nb_pt1 (in) : the number of points in p
 * @param p_im (in) : a real[3*nb_pt2] array containing the xyz coordinates of the point of eid2 (periodic with the ones in stored in p)
 * @param nb_pt2 (in) : the number of points in p_im : has to be equal to nb_pt1.
 * @return a status code
 */
MESHGEMS_METHOD meshgems_status_t meshgems_cad_add_edge_multiple_periodicity_with_transformation_function_by_points(
    meshgems_cad_t *cad, meshgems_integer *eid1, meshgems_integer size1,
    meshgems_integer *eid2, meshgems_integer size2, meshgems_real *p,
    meshgems_integer nb_pt1, meshgems_real *p_im, meshgems_integer nb_pt2);

/**
 * Get the list of composite tags associated to this cad.
 *
 * @param[in] cad : the working cad
 * @param[out] n_composite_tag : the number of composite tags 
 * @param[out] composite_tag : the integer array containing the list of composite tags. This buffer must be allocated by the caller with the function meshgems_cad_calloc_generic_buffer(cad)
 * @return a status code
 */
MESHGEMS_METHOD meshgems_status_t meshgems_cad_get_composite_tag_list(
    meshgems_cad_t *cad, meshgems_integer *n_composite_tag,
    meshgems_integer *composite_tag);

/**
 * Get the list of basic tags that are combined in a composite tag.
 *
 * @param[in] cad : the working cad
 * @param[in] composite_tag : the queried composite tag
 * @param[out] n_basic_tag : the number of basic tags 
 * @param[out] basic_tag : the integer array containing the list of basic tags. This buffer must be allocated by the caller with the function meshgems_cad_calloc_generic_buffer(cad)
 * @return a status code
 */
MESHGEMS_METHOD meshgems_status_t meshgems_cad_get_composite_tag_definition(
    meshgems_cad_t *cad, meshgems_integer composite_tag,
    meshgems_integer *n_basic_tag, meshgems_integer *basic_tag);

/* @} */

/** Private part of the CAD interface. Only MESHGEMS products should use it. **/


typedef meshgems_status_t (*meshgems_cad_face_geometry_translator_fun_t)(meshgems_real *uv_new, meshgems_cad_face_t **f_old,
    meshgems_real *uv_old, meshgems_real *eps_old, void *user_data);
typedef meshgems_status_t (*meshgems_cad_edge_geometry_translator_fun_t)(meshgems_real t_new, meshgems_cad_edge_t **e_old, meshgems_real *t_old,
    meshgems_real *tmin_old, meshgems_real *tmax_old, void *user_data);
typedef meshgems_status_t (*meshgems_cad_wire_geometry_translator_fun_t)(meshgems_real t_new, meshgems_cad_wire_t **w_old, meshgems_real *t_old,
    meshgems_real *tmin_old, meshgems_real *tmax_old, void *user_data);

MESHGEMS_METHOD meshgems_status_t meshgems_cad_get_context(meshgems_cad_t *cad, meshgems_context_t **ctx);
MESHGEMS_METHOD meshgems_status_t meshgems_cad_has_discrete_definition(meshgems_cad_t *c, meshgems_integer *has);
MESHGEMS_METHOD meshgems_cad_face_t *meshgems_cad_face_new_pafftrans(meshgems_cad_t *c, meshgems_cad_face_t *f, meshgems_integer fid,
								  meshgems_real *alpha, meshgems_real *beta,
								  meshgems_integer ke, meshgems_integer kp);

MESHGEMS_METHOD meshgems_status_t meshgems_cad_get_face_count(meshgems_cad_t *cad, meshgems_integer *n);
MESHGEMS_METHOD meshgems_status_t meshgems_cad_get_face(meshgems_cad_t *cad, meshgems_integer i, meshgems_cad_face_t **f);
MESHGEMS_METHOD meshgems_status_t meshgems_cad_get_wire_count(meshgems_cad_t *cad, meshgems_integer *n);
MESHGEMS_METHOD meshgems_status_t meshgems_cad_get_wire(meshgems_cad_t *cad, meshgems_integer i, meshgems_cad_wire_t **w);
MESHGEMS_METHOD meshgems_cad_wire_t *meshgems_cad_wire_new_newton1(meshgems_cad_t *c, meshgems_integer eid, meshgems_integer n, meshgems_real *xyz);
MESHGEMS_METHOD meshgems_status_t meshgems_cad_wire_get_support_vertices(meshgems_cad_wire_t *w, meshgems_integer *nv,
    meshgems_real **t, meshgems_real **xyz);
MESHGEMS_METHOD meshgems_cad_wire_t *meshgems_cad_wire_new_copy(meshgems_cad_wire_t *w);
MESHGEMS_METHOD meshgems_cad_wire_t *meshgems_cad_wire_clone(meshgems_cad_wire_t *w);
MESHGEMS_METHOD meshgems_status_t meshgems_cad_wire_attach(meshgems_cad_wire_t *w, meshgems_cad_t *c);
MESHGEMS_METHOD meshgems_status_t meshgems_cad_get_vertex(meshgems_cad_t *c, meshgems_integer pid, meshgems_cad_vertex_t **v);
MESHGEMS_METHOD meshgems_status_t meshgems_cad_vertex_get_coordinates(meshgems_cad_vertex_t *v, meshgems_real *xyz);
MESHGEMS_METHOD meshgems_status_t meshgems_cad_vertex_get_uid(meshgems_cad_vertex_t *v, meshgems_integer *uid);
MESHGEMS_METHOD meshgems_status_t meshgems_cad_vertex_get_original_geometry(meshgems_cad_vertex_t *v, meshgems_cad_vertex_t **ov);

MESHGEMS_METHOD meshgems_status_t meshgems_cad_face_set_id(meshgems_cad_face_t *face, meshgems_integer id);
MESHGEMS_METHOD meshgems_status_t meshgems_cad_face_set_from_tesselation(meshgems_cad_face_t *face, meshgems_integer is);

MESHGEMS_METHOD meshgems_status_t meshgems_cad_face_get_id(meshgems_cad_face_t *face, meshgems_integer *id);
MESHGEMS_METHOD meshgems_status_t meshgems_cad_face_get_tag(meshgems_cad_face_t *face, meshgems_integer *tag);
MESHGEMS_METHOD meshgems_status_t meshgems_cad_face_get_uid(meshgems_cad_face_t *face, meshgems_integer *uid);
MESHGEMS_METHOD meshgems_status_t meshgems_cad_face_get_orientation(meshgems_cad_face_t *face, meshgems_integer *orientation);
MESHGEMS_METHOD meshgems_status_t meshgems_cad_face_get_surface_periodicity(meshgems_cad_face_t *face, meshgems_integer *is_periodic_u,
									 meshgems_integer *is_periodic_v,
									 meshgems_real *period_u, meshgems_real *period_v);
MESHGEMS_METHOD meshgems_status_t meshgems_cad_face_get_surface_parametric_range(meshgems_cad_face_t *face, meshgems_real *range_u, meshgems_real *range_v);
MESHGEMS_METHOD meshgems_status_t meshgems_cad_face_get_surf(meshgems_cad_face_t *face, meshgems_cad_surf_t *surf, void **udata);
MESHGEMS_METHOD meshgems_status_t meshgems_cad_face_surface(meshgems_cad_face_t *face, meshgems_real *uv, meshgems_real *xyz,
							 meshgems_real*du, meshgems_real *dv,
							 meshgems_real *duu, meshgems_real *duv, meshgems_real *dvv);
MESHGEMS_METHOD meshgems_status_t meshgems_cad_face_has_discrete_definition(meshgems_cad_face_t *face, meshgems_integer *has);
MESHGEMS_METHOD meshgems_status_t meshgems_cad_face_get_discrete_definition(meshgems_cad_face_t *face, meshgems_mesh_t **m2d, meshgems_mesh_t **m3d);
MESHGEMS_METHOD meshgems_status_t meshgems_cad_face_get_from_tesselation(meshgems_cad_face_t *face, meshgems_integer *is);

MESHGEMS_METHOD meshgems_status_t meshgems_cad_face_get_edge_count(meshgems_cad_face_t *face, meshgems_integer *n);
MESHGEMS_METHOD meshgems_status_t meshgems_cad_face_get_point_count(meshgems_cad_face_t *face, meshgems_integer *n);
MESHGEMS_METHOD meshgems_status_t meshgems_cad_face_get_edge(meshgems_cad_face_t *face, meshgems_integer i, meshgems_cad_edge_t **e);
MESHGEMS_METHOD meshgems_status_t meshgems_cad_face_get_point(meshgems_cad_face_t *face, meshgems_integer i, meshgems_cad_point_t **e);
MESHGEMS_METHOD meshgems_status_t meshgems_cad_face_get_cad(meshgems_cad_face_t *face, meshgems_cad_t **c);
MESHGEMS_METHOD meshgems_status_t meshgems_cad_face_geometry_is_imposed(meshgems_cad_face_t *face, meshgems_integer *is);

MESHGEMS_METHOD meshgems_cad_edge_t *meshgems_cad_edge_new_newton1(meshgems_cad_face_t *f, meshgems_integer eid, meshgems_integer n, meshgems_real *uv, meshgems_real *xyz);
MESHGEMS_METHOD meshgems_cad_edge_t *meshgems_cad_edge_new_newton1_with_ea(meshgems_cad_face_t *f, meshgems_integer eid,
									meshgems_integer n, meshgems_real *uv, meshgems_real *xyz,
									meshgems_cad_edge_t **eae, meshgems_real *eat);
MESHGEMS_METHOD meshgems_cad_edge_t *meshgems_cad_edge_new_afftrans(meshgems_cad_face_t *f, meshgems_cad_edge_t *e, meshgems_integer eid,
								 meshgems_real *alpha, meshgems_real *beta);
MESHGEMS_METHOD meshgems_status_t meshgems_cad_edge_set_id(meshgems_cad_edge_t *edge, meshgems_integer id);
MESHGEMS_METHOD meshgems_status_t meshgems_cad_edge_set_sid(meshgems_cad_edge_t *edge, meshgems_integer sid);
MESHGEMS_METHOD meshgems_status_t meshgems_cad_edge_set_lsid(meshgems_cad_edge_t *edge, meshgems_integer lsid);
MESHGEMS_METHOD meshgems_status_t meshgems_cad_edge_set_range(meshgems_cad_edge_t *edge, meshgems_real tmin, meshgems_real tmax);
MESHGEMS_METHOD meshgems_status_t meshgems_cad_edge_set_degenerated(meshgems_cad_edge_t *edge, meshgems_integer is_degenerated);
MESHGEMS_METHOD meshgems_status_t meshgems_cad_edge_set_from_tesselation(meshgems_cad_edge_t *edge, meshgems_integer is);

MESHGEMS_METHOD meshgems_status_t meshgems_cad_edge_get_range(meshgems_cad_edge_t *edge, meshgems_real *tmin, meshgems_real *tmax);
MESHGEMS_METHOD meshgems_status_t meshgems_cad_edge_get_orientation(meshgems_cad_edge_t *edge, meshgems_integer *orientation);
MESHGEMS_METHOD meshgems_status_t meshgems_cad_edge_get_coorientation(meshgems_cad_edge_t *edge, meshgems_integer *coorientation);
MESHGEMS_METHOD meshgems_status_t meshgems_cad_edge_get_extremities(meshgems_cad_edge_t *edge, meshgems_integer *pid1, meshgems_integer *pid2);
MESHGEMS_METHOD meshgems_status_t meshgems_cad_edge_get_extremities_tag(meshgems_cad_edge_t *edge, meshgems_integer *tag1, meshgems_integer *tag2);
MESHGEMS_METHOD meshgems_status_t meshgems_cad_edge_get_extremities_required_property(meshgems_cad_edge_t *edge, meshgems_integer *req1, meshgems_integer *req2);
MESHGEMS_METHOD meshgems_status_t meshgems_cad_edge_get_uv_extremities(meshgems_cad_edge_t *edge, meshgems_integer *uvid1, meshgems_integer *uvid2);
MESHGEMS_METHOD meshgems_status_t meshgems_cad_edge_get_id(meshgems_cad_edge_t *edge, meshgems_integer *tag);
MESHGEMS_METHOD meshgems_status_t meshgems_cad_edge_get_sid(meshgems_cad_edge_t *edge, meshgems_integer *sid);
MESHGEMS_METHOD meshgems_status_t meshgems_cad_edge_get_lsid(meshgems_cad_edge_t *edge, meshgems_integer *lsid);
MESHGEMS_METHOD meshgems_status_t meshgems_cad_edge_get_tag(meshgems_cad_edge_t *edge, meshgems_integer *tag);
MESHGEMS_METHOD meshgems_status_t meshgems_cad_edge_get_uid(meshgems_cad_edge_t *edge, meshgems_integer *uid);
MESHGEMS_METHOD meshgems_status_t meshgems_cad_edge_get_curv(meshgems_cad_edge_t *edge, meshgems_cad_curv_t *curv, void **udata);
MESHGEMS_METHOD meshgems_status_t meshgems_cad_edge_curve(meshgems_cad_edge_t *edge, meshgems_real t, meshgems_real *uv, meshgems_real *dt, meshgems_real *dtt);
MESHGEMS_METHOD meshgems_status_t meshgems_cad_edge_has_discrete_definition(meshgems_cad_edge_t *edge, meshgems_integer *has);
MESHGEMS_METHOD meshgems_status_t meshgems_cad_edge_get_discrete_definition(meshgems_cad_edge_t *edge, meshgems_integer *nv, meshgems_integer **vtx);
MESHGEMS_METHOD meshgems_status_t meshgems_cad_edge_get_from_tesselation(meshgems_cad_edge_t *edge, meshgems_integer *is);

MESHGEMS_METHOD meshgems_status_t meshgems_cad_edge_set_curve3d(meshgems_cad_edge_t *edge, meshgems_cad_curv3d_t curv3d, void *udata);
MESHGEMS_METHOD meshgems_status_t meshgems_cad_edge_curve3d(meshgems_cad_edge_t *edge, meshgems_real t, meshgems_real *xyz);
MESHGEMS_METHOD meshgems_status_t meshgems_cad_edge_get_curve3d(meshgems_cad_edge_t *edge, meshgems_cad_curv3d_t *curv3d, void **udata);
MESHGEMS_METHOD meshgems_status_t meshgems_cad_edge_has_curve3d(meshgems_cad_edge_t *edge, meshgems_integer *has);

MESHGEMS_METHOD meshgems_status_t meshgems_cad_edge_get_face(meshgems_cad_edge_t *edge, meshgems_cad_face_t **face);
MESHGEMS_METHOD meshgems_status_t meshgems_cad_edge_get_wdef(meshgems_cad_edge_t *edge, meshgems_integer *wdef);
MESHGEMS_METHOD meshgems_status_t meshgems_cad_edge_get_degenerated(meshgems_cad_edge_t *edge, meshgems_integer *degen);


MESHGEMS_METHOD meshgems_status_t meshgems_cad_point_get_id(meshgems_cad_point_t *point, meshgems_integer *id);
MESHGEMS_METHOD meshgems_status_t meshgems_cad_point_get_tag(meshgems_cad_point_t *point, meshgems_integer *tag);
MESHGEMS_METHOD meshgems_status_t meshgems_cad_point_get_corner_property(meshgems_cad_point_t *point, meshgems_integer *cor);
MESHGEMS_METHOD meshgems_status_t meshgems_cad_point_set_corner_property(meshgems_cad_point_t *point, meshgems_integer is_corner);
MESHGEMS_METHOD meshgems_status_t meshgems_cad_point_get_uv(meshgems_cad_point_t *point, meshgems_real *uv);
MESHGEMS_METHOD meshgems_status_t meshgems_cad_point_get_discrete_definition(meshgems_cad_point_t *point, meshgems_integer *idx);
MESHGEMS_METHOD meshgems_status_t meshgems_cad_get_use_vtag(meshgems_cad_t *point, meshgems_integer *use);
MESHGEMS_METHOD meshgems_status_t meshgems_cad_get_use_etag(meshgems_cad_t *point, meshgems_integer *use);
MESHGEMS_METHOD meshgems_status_t meshgems_cad_get_use_ftag(meshgems_cad_t *point, meshgems_integer *use);

MESHGEMS_METHOD meshgems_cad_edge_t *meshgems_cad_edge_clone(meshgems_cad_edge_t *e);
MESHGEMS_METHOD meshgems_cad_edge_t *meshgems_cad_edge_new_copy(meshgems_cad_edge_t *e);
MESHGEMS_METHOD meshgems_cad_face_t *meshgems_cad_face_clone(meshgems_cad_face_t *f);
MESHGEMS_METHOD meshgems_cad_face_t *meshgems_cad_face_new_copy(meshgems_cad_face_t *f);
MESHGEMS_METHOD meshgems_status_t meshgems_cad_edge_attach(meshgems_cad_edge_t *edge, meshgems_cad_face_t *face);
MESHGEMS_METHOD meshgems_status_t meshgems_cad_face_attach(meshgems_cad_face_t *face, meshgems_cad_t *cad);

MESHGEMS_METHOD meshgems_cad_edge_t *meshgems_cad_merge_edges(meshgems_cad_edge_t *e1, meshgems_real tmin1, meshgems_real tmax1, meshgems_integer inv_e1,
							   meshgems_cad_edge_t *e2, meshgems_real tmin2, meshgems_real tmax2, meshgems_integer inv_e2);

MESHGEMS_METHOD meshgems_status_t meshgems_cad_face_clone_points(meshgems_cad_face_t *face, meshgems_cad_face_t *fd);

MESHGEMS_METHOD meshgems_status_t meshgems_cad_edge_set_clone_function(meshgems_cad_edge_t *edge, meshgems_clone_fun f);
MESHGEMS_METHOD meshgems_status_t meshgems_cad_face_set_clone_function(meshgems_cad_face_t *face, meshgems_clone_fun f);
MESHGEMS_METHOD meshgems_status_t meshgems_cad_edge_set_delete_function(meshgems_cad_edge_t *edge, meshgems_delete_fun f);
MESHGEMS_METHOD meshgems_status_t meshgems_cad_face_set_delete_function(meshgems_cad_face_t *face, meshgems_delete_fun f);

MESHGEMS_METHOD meshgems_status_t meshgems_cad_edge_get_support_vertices(meshgems_cad_edge_t *e, meshgems_integer *nv, meshgems_real **t, meshgems_real **uv, meshgems_real **xyz);

MESHGEMS_METHOD meshgems_status_t meshgems_cad_edge_set_support_vertex_count(meshgems_cad_edge_t *e, meshgems_integer nv);
MESHGEMS_METHOD meshgems_status_t meshgems_cad_edge_set_support_vertex(meshgems_cad_edge_t *e, meshgems_integer i, meshgems_real t, meshgems_real *uv, meshgems_real *xyz);
MESHGEMS_METHOD meshgems_status_t meshgems_cad_edge_set_extremities_corner_property(meshgems_cad_edge_t *edge, meshgems_integer cor1, meshgems_integer cor2);
MESHGEMS_METHOD meshgems_status_t meshgems_cad_edge_get_extremities_corner_property(meshgems_cad_edge_t *edge, meshgems_integer *cor1, meshgems_integer *cor2);
MESHGEMS_METHOD meshgems_status_t meshgems_cad_edge_set_extremities_artificial_property(meshgems_cad_edge_t *edge, meshgems_integer art1, meshgems_integer art2);
MESHGEMS_METHOD meshgems_status_t meshgems_cad_edge_get_extremities_artificial_property(meshgems_cad_edge_t *edge, meshgems_integer *art1, meshgems_integer *art2);

MESHGEMS_METHOD meshgems_status_t meshgems_cad_get_face_id_periodicity_count(meshgems_cad_t *cad, meshgems_integer fid, meshgems_integer *n);
MESHGEMS_METHOD meshgems_status_t meshgems_cad_get_face_id_periodicity(meshgems_cad_t *cad, meshgems_integer fid, meshgems_integer i, meshgems_integer *fid2);

MESHGEMS_METHOD meshgems_status_t meshgems_cad_get_edge_id_periodicity_count(meshgems_cad_t *cad, meshgems_integer eid, meshgems_integer *n);
MESHGEMS_METHOD meshgems_status_t meshgems_cad_get_edge_id_periodicity(meshgems_cad_t *cad, meshgems_integer eid, meshgems_integer i,
								    meshgems_integer *fid1, meshgems_integer *fid2, meshgems_integer *eid2, meshgems_integer *orientation);

MESHGEMS_METHOD meshgems_status_t meshgems_cad_get_point_id_periodicity_count(meshgems_cad_t *cad, meshgems_integer pid, meshgems_integer *n);
MESHGEMS_METHOD meshgems_status_t meshgems_cad_get_point_id_periodicity(meshgems_cad_t *cad, meshgems_integer pid, meshgems_integer i,
								     meshgems_integer *eid1, meshgems_integer *eid2, meshgems_integer *pid2);


MESHGEMS_METHOD meshgems_status_t meshgems_cad_get_face_multiple_periodicity_count(meshgems_cad_t *cad, meshgems_integer *n);
MESHGEMS_METHOD meshgems_status_t meshgems_cad_get_face_multiple_periodicity(meshgems_cad_t *cad, meshgems_integer i, meshgems_integer **fid1, meshgems_integer *size1, meshgems_integer **fid2, meshgems_integer *size2,
									  meshgems_cad_periodicity_transformation_t *transf, void **user_data);

MESHGEMS_METHOD meshgems_status_t meshgems_cad_get_edge_multiple_periodicity_count(meshgems_cad_t *cad, meshgems_integer *n);
MESHGEMS_METHOD meshgems_status_t meshgems_cad_get_edge_multiple_periodicity(meshgems_cad_t *cad, meshgems_integer i, meshgems_integer **eid1, meshgems_integer *size1, meshgems_integer **eid2, meshgems_integer *size2,
									  meshgems_cad_periodicity_transformation_t *transf, void **user_data);
MESHGEMS_METHOD void *meshgems_cad_calloc_generic_buffer(meshgems_cad_t *cad);
MESHGEMS_METHOD void meshgems_cad_free_generic_buffer(void *v);

MESHGEMS_METHOD meshgems_status_t meshgems_cad_face_check_point_parametric_range(meshgems_cad_face_t *f, meshgems_real *uv, meshgems_integer *res);

MESHGEMS_METHOD meshgems_status_t meshgems_cad_face_has_surface_parametric_range(meshgems_cad_face_t *f, meshgems_integer *res);
MESHGEMS_METHOD meshgems_status_t meshgems_cad_face_has_surface_periodicity(meshgems_cad_face_t *f, meshgems_integer *res);

MESHGEMS_METHOD meshgems_status_t meshgems_cad_set_tag_system(meshgems_cad_t *c, void *);
MESHGEMS_METHOD meshgems_status_t meshgems_cad_get_tag_system(meshgems_cad_t *c, void *);

MESHGEMS_METHOD meshgems_status_t meshgems_cad_face_check_uv(meshgems_cad_face_t *f, meshgems_real *uv, meshgems_real eps, meshgems_integer *is_inside);

MESHGEMS_METHOD meshgems_status_t meshgems_cad_face_get_original_geometry(meshgems_cad_face_t *f, meshgems_real *uv,
								       meshgems_cad_face_t **of, meshgems_real *ouv,
								       meshgems_real *oeps);
MESHGEMS_METHOD meshgems_status_t meshgems_cad_edge_get_original_geometry(meshgems_cad_edge_t *e, meshgems_real t,
								       meshgems_cad_edge_t **oe, meshgems_real *ot,
								       meshgems_real *otmin, meshgems_real *otmax);
MESHGEMS_METHOD meshgems_status_t meshgems_cad_wire_get_original_geometry(meshgems_cad_wire_t *w, meshgems_real t,
                       meshgems_cad_wire_t **ow, meshgems_real *ot,
                       meshgems_real *otmin, meshgems_real *otmax);

MESHGEMS_METHOD meshgems_status_t meshgems_cad_face_reset_original_id(meshgems_cad_face_t *f);
MESHGEMS_METHOD meshgems_status_t meshgems_cad_face_get_original_id_count(meshgems_cad_face_t *f, meshgems_integer *nid);
MESHGEMS_METHOD meshgems_status_t meshgems_cad_face_get_original_id(meshgems_cad_face_t *f, meshgems_integer i, meshgems_integer *id);
MESHGEMS_METHOD meshgems_status_t meshgems_cad_face_add_original_id(meshgems_cad_face_t *f, meshgems_integer id);
MESHGEMS_METHOD meshgems_status_t meshgems_cad_face_set_original_geometry_translator(meshgems_cad_face_t *f, meshgems_cad_face_geometry_translator_fun_t fun);
MESHGEMS_METHOD meshgems_status_t meshgems_cad_face_get_sd_info(meshgems_cad_face_t *face, meshgems_integer *tin, meshgems_integer *tou);
MESHGEMS_METHOD meshgems_status_t meshgems_cad_face_set_sd_info(meshgems_cad_face_t *face, meshgems_integer tin, meshgems_integer tou);
MESHGEMS_METHOD meshgems_status_t meshgems_cad_edge_reset_original_id(meshgems_cad_edge_t *e);
MESHGEMS_METHOD meshgems_status_t meshgems_cad_edge_get_original_id_count(meshgems_cad_edge_t *e, meshgems_integer *nid);
MESHGEMS_METHOD meshgems_status_t meshgems_cad_edge_get_original_id(meshgems_cad_edge_t *e, meshgems_integer i, meshgems_integer *id);
MESHGEMS_METHOD meshgems_status_t meshgems_cad_edge_add_original_id(meshgems_cad_edge_t *e, meshgems_integer id);
MESHGEMS_METHOD meshgems_status_t meshgems_cad_wire_get_original_id_count(meshgems_cad_wire_t *w, meshgems_integer *nid);
MESHGEMS_METHOD meshgems_status_t meshgems_cad_wire_get_original_id(meshgems_cad_wire_t *w, meshgems_integer i, meshgems_integer *id);
MESHGEMS_METHOD meshgems_status_t meshgems_cad_wire_add_original_id(meshgems_cad_wire_t *w, meshgems_integer id);
MESHGEMS_METHOD meshgems_status_t meshgems_cad_wire_reset_original_id(meshgems_cad_wire_t *w);
MESHGEMS_METHOD meshgems_status_t meshgems_cad_wire_set_id(meshgems_cad_wire_t *w, meshgems_integer id);
MESHGEMS_METHOD meshgems_status_t meshgems_cad_wire_set_sid(meshgems_cad_wire_t *w, meshgems_integer sid);
MESHGEMS_METHOD meshgems_status_t meshgems_cad_wire_set_lsid(meshgems_cad_wire_t *w, meshgems_integer lsid);
MESHGEMS_METHOD meshgems_status_t meshgems_cad_wire_set_from_tesselation(meshgems_cad_wire_t *wire, meshgems_integer is);
MESHGEMS_METHOD meshgems_status_t meshgems_cad_wire_get_range(meshgems_cad_wire_t *w, meshgems_real *tmin, meshgems_real *tmax);
MESHGEMS_METHOD meshgems_status_t meshgems_cad_wire_get_extremities(meshgems_cad_wire_t *wire, meshgems_integer *pid1, meshgems_integer *pid2);
MESHGEMS_METHOD meshgems_status_t meshgems_cad_wire_get_extremities_tag(meshgems_cad_wire_t *wire, meshgems_integer *tag1, meshgems_integer *tag2);
MESHGEMS_METHOD meshgems_status_t meshgems_cad_wire_get_extremities_required_property(meshgems_cad_wire_t *wire, meshgems_integer *req1, meshgems_integer *req2);
MESHGEMS_METHOD meshgems_status_t meshgems_cad_wire_set_extremities_corner_property(meshgems_cad_wire_t *wire, meshgems_integer cor1, meshgems_integer cor2);
MESHGEMS_METHOD meshgems_status_t meshgems_cad_wire_get_extremities_corner_property(meshgems_cad_wire_t *wire, meshgems_integer *cor1, meshgems_integer *cor2);
MESHGEMS_METHOD meshgems_status_t meshgems_cad_wire_set_extremities_artificial_property(meshgems_cad_wire_t *wire, meshgems_integer art1, meshgems_integer art2);
MESHGEMS_METHOD meshgems_status_t meshgems_cad_wire_get_extremities_artificial_property(meshgems_cad_wire_t *wire, meshgems_integer *art1, meshgems_integer *art2);
MESHGEMS_METHOD meshgems_status_t meshgems_cad_wire_get_from_tesselation(meshgems_cad_wire_t *wire, meshgems_integer *is);
MESHGEMS_METHOD meshgems_status_t meshgems_cad_wire_get_id(meshgems_cad_wire_t *wire, meshgems_integer *id);
MESHGEMS_METHOD meshgems_status_t meshgems_cad_wire_get_sid(meshgems_cad_wire_t *wire, meshgems_integer *sid);
MESHGEMS_METHOD meshgems_status_t meshgems_cad_wire_get_lsid(meshgems_cad_wire_t *wire, meshgems_integer *lsid);
MESHGEMS_METHOD meshgems_status_t meshgems_cad_wire_get_cad(meshgems_cad_wire_t *wire, meshgems_cad_t **c);
MESHGEMS_METHOD meshgems_status_t meshgems_cad_wire_get_tag(meshgems_cad_wire_t *wire, meshgems_integer *tag);
MESHGEMS_METHOD meshgems_status_t meshgems_cad_wire_get_uid(meshgems_cad_wire_t *wire, meshgems_integer *uid);
MESHGEMS_METHOD meshgems_status_t meshgems_cad_wire_get_curv(meshgems_cad_wire_t *wire, meshgems_cad_curv3d2_t *curv, void **udata);
MESHGEMS_METHOD meshgems_status_t meshgems_cad_wire_curve(meshgems_cad_wire_t *wire, meshgems_real t, meshgems_real *xyz,meshgems_real *dt, meshgems_real *dtt);
MESHGEMS_METHOD meshgems_status_t meshgems_cad_wire_has_discrete_definition(meshgems_cad_wire_t *wire, meshgems_integer *has);
MESHGEMS_METHOD meshgems_status_t meshgems_cad_wire_get_discrete_definition(meshgems_cad_wire_t *wire, meshgems_mesh_t **m3d);

MESHGEMS_METHOD meshgems_status_t meshgems_cad_is_signed(meshgems_cad_t *cad, meshgems_integer *is);
MESHGEMS_METHOD meshgems_status_t meshgems_cad_get_signature(meshgems_cad_t *, char **, unsigned int *);
MESHGEMS_METHOD meshgems_status_t meshgems_cad_set_signature(meshgems_cad_t *, char *, unsigned int);

MESHGEMS_METHOD meshgems_status_t meshgems_cad_attach_object(meshgems_cad_t *, void *object, meshgems_delete_fun delfun);
MESHGEMS_METHOD meshgems_status_t meshgems_cad_get_origin(meshgems_cad_t *, meshgems_integer *ori);
MESHGEMS_METHOD meshgems_status_t meshgems_cad_set_origin(meshgems_cad_t *, meshgems_integer ori);

MESHGEMS_METHOD meshgems_status_t meshgems_cad_get_polygonal_face_count(meshgems_cad_t *cad, meshgems_integer *n);
MESHGEMS_METHOD meshgems_status_t meshgems_cad_get_polygonal_face(meshgems_cad_t *cad, meshgems_integer i, meshgems_cad_polygonal_face_t **f);
MESHGEMS_METHOD meshgems_cad_polygonal_face_t *meshgems_cad_polygonal_face_new(meshgems_cad_t *cad, meshgems_integer fid, meshgems_integer nseg, meshgems_real *xyz);
MESHGEMS_METHOD void meshgems_cad_polygonal_face_delete(meshgems_cad_polygonal_face_t *face);
MESHGEMS_METHOD meshgems_status_t meshgems_cad_polygonal_face_get_id(meshgems_cad_polygonal_face_t *pf, meshgems_integer *id);
MESHGEMS_METHOD meshgems_status_t meshgems_cad_polygonal_face_set_id(meshgems_cad_polygonal_face_t *pf, meshgems_integer id);
MESHGEMS_METHOD meshgems_status_t meshgems_cad_polygonal_face_get_tag(meshgems_cad_polygonal_face_t *pf, meshgems_integer *tag);
MESHGEMS_METHOD meshgems_status_t meshgems_cad_polygonal_face_set_tag(meshgems_cad_polygonal_face_t *pf, meshgems_integer tag);
MESHGEMS_METHOD meshgems_status_t meshgems_cad_polygonal_face_get_side_count(meshgems_cad_polygonal_face_t *pf, meshgems_integer *n);
MESHGEMS_METHOD meshgems_status_t meshgems_cad_polygonal_face_get_points_coordinates(meshgems_cad_polygonal_face_t *f, meshgems_real *xyz);
MESHGEMS_METHOD meshgems_status_t meshgems_cad_polygonal_face_set_points_ids(meshgems_cad_polygonal_face_t *f, meshgems_integer *pids);
MESHGEMS_METHOD meshgems_status_t meshgems_cad_polygonal_face_get_points_ids(meshgems_cad_polygonal_face_t *f, meshgems_integer *pids);
MESHGEMS_METHOD meshgems_status_t meshgems_cad_polygonal_face_set_points_tags(meshgems_cad_polygonal_face_t *f, meshgems_integer *ptags);
MESHGEMS_METHOD meshgems_status_t meshgems_cad_polygonal_face_get_points_tags(meshgems_cad_polygonal_face_t *f, meshgems_integer *ptags);
MESHGEMS_METHOD meshgems_status_t meshgems_cad_polygonal_face_set_points_required_properties(meshgems_cad_polygonal_face_t *f, meshgems_integer *preq);
MESHGEMS_METHOD meshgems_status_t meshgems_cad_polygonal_face_get_points_required_properties(meshgems_cad_polygonal_face_t *f, meshgems_integer *preq);
MESHGEMS_METHOD meshgems_status_t meshgems_cad_polygonal_face_set_points_corner_properties(meshgems_cad_polygonal_face_t *f, meshgems_integer *pcor);
MESHGEMS_METHOD meshgems_status_t meshgems_cad_polygonal_face_get_points_corner_properties(meshgems_cad_polygonal_face_t *f, meshgems_integer *pcor);
MESHGEMS_METHOD meshgems_status_t meshgems_cad_polygonal_face_set_edges_ids(meshgems_cad_polygonal_face_t *f, meshgems_integer *eids);
MESHGEMS_METHOD meshgems_status_t meshgems_cad_polygonal_face_get_edges_ids(meshgems_cad_polygonal_face_t *f, meshgems_integer *eids);
MESHGEMS_METHOD meshgems_status_t meshgems_cad_polygonal_face_set_edges_tags(meshgems_cad_polygonal_face_t *f, meshgems_integer *etags);
MESHGEMS_METHOD meshgems_status_t meshgems_cad_polygonal_face_get_edges_tags(meshgems_cad_polygonal_face_t *f, meshgems_integer *etags);
MESHGEMS_METHOD meshgems_status_t meshgems_cad_polygonal_face_set_edges_required_properties(meshgems_cad_polygonal_face_t *f, meshgems_integer *ereq);
MESHGEMS_METHOD meshgems_status_t meshgems_cad_polygonal_face_get_edges_required_properties(meshgems_cad_polygonal_face_t *f, meshgems_integer *ereq);

#endif
