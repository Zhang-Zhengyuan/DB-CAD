/*******************************************************************/
/*    Copyright (c) 1989-2020 by Spatial Corp.                     */
/*    All rights reserved.                                         */
/*    Protected by U.S. Patents 5,257,205; 5,351,196; 6,369,815;   */
/*                              5,982,378; 6,462,738; 6,941,251    */
/*    Protected by European Patents 0503642; 69220263.3            */
/*    Protected by Hong Kong Patent 1008101A                       */
/*******************************************************************/
#ifndef _PPMESHMG
#define _PPMESHMG
class ENTITY;
class FACE;
class SPAposition;
class SPApar_pos;
class SPAunit_vector;
#include "dcl_fct.h"
#include "logical.h"
#include "ppm.hxx"
#include "meshmg.hxx"
class POLYGON_POINT_MESH;
class POLYGON;
class POLYGON_VERTEX;

/**
 * @file ppmeshmg.hxx
 * @CAA2Level L1
 * @CAA2Usage U1
 *! \addtogroup FCTAPI
 *  \brief Declared at <ppmeshmg.hxx>
 *  @{
 */
//************************************************
// The POLYGON_POINT_MESH_MANAGER specializes the MESH_MANAGER class for the
//	case where a mesh
//	is maintained as a POLYGON_POINT_MESH
//
// This class demonstrates use of the 'coordinate polygon'
//	mode of output from the faceter.  That is, polygons
//  are accepted as point-by-point, woth explicit coordinates
//	(NOT pointers to (possibly shared) coordinates) to each
//	node within each polygon.
//
// At the end of the 'coordinate polygon' protocol, the
//	current_mesh pointer points to the just-constructed mesh.
//	It is assumed that this class is used as a base class for
//	another class that takes responsibility for finding
//	a permanent storage location for that mesh (e.g. attaches
//	it to the ACIS model).
//
//************************************************

/**
 * Specializes the <tt>MESH_MANAGER</tt> class when a mesh is maintained as a <tt>POLYGON_POINT_MESH</tt>.
 * <br>
 * <b>Role:</b> This class demonstrates use of the <i>coordinate polygon mode</i> of
 * output from the faceter. That is, polygons are accepted as point-by-point, with
 * explicit coordinates (NOT pointers to possibly shared coordinates) to each node
 * within each polygon.
 * <br><br>
 * At the end of the <i>coordinate polygon protocol</i>, the <tt>current_mesh</tt> pointer points
 * to the just constructed mesh. It is assumed that this class is used as a base
 * class for another class that takes responsibility for finding a permanent
 * storage location for that mesh (e.g., attaches it to the ACIS model).
 * @see POLYGON, POLYGON_POINT_MESH, POLYGON_VERTEX, ENTITY, FACE
 */
class DECL_FCT POLYGON_POINT_MESH_MANAGER :
	public MESH_MANAGER {

	FACE				*current_face;

 protected:
	POLYGON_POINT_MESH	*current_mesh;
	POLYGON 			*current_poly;
	POLYGON_VERTEX		*current_poly_vertex;

	ENTITY *mesh_node_template;
	ENTITY *refinement;

 public:
/**
 * C++ constructor, creating a <tt>POLYGON_POINT_MESH_MANAGER</tt>.
 */
  POLYGON_POINT_MESH_MANAGER();
/**
 * Announces the beginning of the output of a mesh.
 * <br><br>
 * <b>Role:</b> Creates a new mesh for it. The faceted entity should be a FACE
 * and the format a <tt>VERTEX_TEMPLATE</tt>.
 * <br><br>
 * @param facetted_entity
 * faceted entity.
 * @param refinement_control_entity
 * applicable REFINEMENT.
 * @param output_format_entity
 * output format entity.
 */
  virtual void begin_mesh_output(
		ENTITY *facetted_entity,
		ENTITY *refinement_control_entity,
		ENTITY *output_format_entity);
/**
 * Ends the mesh output.
 * <br><br>
 * @param facetted_entity
 * faceted entity.
 * @param refinement_control_entity
 * applicable REFINEMENT.
 * @param output_format_entity
 * output format entity.
 */
  virtual void end_mesh_output(
		ENTITY *facetted_entity,
		ENTITY *refinement_control_entity,
		ENTITY *output_format_entity);

/**
 * Tells the faceter we need coordinate polygons.
 */
  virtual logical need_coordinate_polygons();
/**
 * Starts a polygon output.
 * <br><br>
 * <b>Role:</b> Creates it and adds it to the current mesh.
 * <br><br>
 * @param ipoly
 * polygon index.
 * @param nnode
 * number of nodes.
 */
  virtual void start_coordinate_polygon(int ipoly,int nnode);
/**
 * Announces one node on a polygon.
 * <br><br>
 * <b>Role:</b> Used with the coordinate protocol.
 * <br><br>
 * @param ipoly
 * polygon index.
 * @param ipt
 * local node index.
 * @param uv
 * parameter position.
 * @param iX
 * point.
 * @param N
 * normal.
 */
  virtual void announce_coordinate_polygon_node(
				int ipoly,
				int ipt,
				const SPApar_pos &uv,
				const SPAposition &iX,
				const SPAunit_vector &N);

};

/* @} */
#endif
