/*******************************************************************/
/*    Copyright (c) 1989-2020 by Spatial Corp.                     */
/*    All rights reserved.                                         */
/*    Protected by U.S. Patents 5,257,205; 5,351,196; 6,369,815;   */
/*                              5,982,378; 6,462,738; 6,941,251    */
/*    Protected by European Patents 0503642; 69220263.3            */
/*    Protected by Hong Kong Patent 1008101A                       */
/*******************************************************************/
#ifndef _PPMFACE
#define _PPMFACE
#include "dcl_fct.h"
#include "ppmeshmg.hxx"
/**
 * @file ppmface.hxx
 * @CAA2Level L1
 * @CAA2Usage U2
 *! \addtogroup FCTAPI
 *  \brief Declared at <ppmface.hxx>
 *  @{
 */

//****************************************************************************
// PPM_ON_FACE_MESH_MANAGER is derived from the POLYGON_POINT_MESH_MANAGER.
// It performs the additional task of attaching the PPM to the owning face.
//
//****************************************************************************
/**
 * Shows how a <tt>MESH</tt>-derived mesh is attached to a face.
 * <br>
 * <b>Role:</b> The functionality to attach the mesh is added to what have been
 * provided by <tt>POLYGON_POINT_MESH_MANAGER</tt>.
 */
class DECL_FCT PPM_ON_FACE_MESH_MANAGER: public POLYGON_POINT_MESH_MANAGER {
public:
/**
 * C++ allocation constructor requests memory for this object but does not populate it.
 */
	PPM_ON_FACE_MESH_MANAGER() {};
/**
 * Announces the beginning of the output of a mesh.
 * <br><br>
 * <b>Role:</b> Creates the mesh and attaches it to the owning face.
 * <br><br>
 * @param face
 * entity being faceted.
 * @param refinement_control
 * REFINEMENT.
 * @param output_format
 * output format entity.
 */
	virtual void begin_mesh_output(
		ENTITY *face,
		ENTITY *refinement_control,
		ENTITY *output_format
	);
};
/* @} */
#endif
