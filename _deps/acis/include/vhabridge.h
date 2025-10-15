/*******************************************************************/
/*    Copyright (c) 1989-2020 by Spatial Corp.                     */
/*    All rights reserved.                                         */
/*    Protected by U.S. Patents 5,257,205; 5,351,196; 6,369,815;   */
/*                              5,982,378; 6,462,738; 6,941,251    */
/*    Protected by European Patents 0503642; 69220263.3            */
/*    Protected by Hong Kong Patent 1008101A                       */
/*******************************************************************/
#ifndef __VHABRIDGE_H_
#define __VHABRIDGE_H_

// ACIS includes-
#include "dcl_vhabridge.h"
#include "vabridge.h"

/**
 * @file vhabridge.h
 *
 * @addtogroup Mesh
 * @brief Declared at <vhabridge.h>
 * @{
 */

/**
 * ACIS style initialization.
 *
 * @role	Initializes the VKI/HOOPS/ACIS bridge.
 * This should be called once during run-time.
 *
 * @return
 * 	Indicates success or failure.
**/
DECL_VHABRIDGE outcome api_vha_initialize( );
/**
 * ACIS style termination.
 *
 * @role  Terminates the VKI/HOOPS/ACIS bridge.
 * This should be called once during run-time.
 *
 * @return Indicates success or failure.
**/
DECL_VHABRIDGE outcome api_vha_terminate( );

/**
 * Enumerator to control rendering, use a bitwise OR to get combinations.
 *
 */
enum VHA_RenderMode_e
	{
		VHA_RENDER_ALL            = 0xffff, //!< All objects rendered.
		VHA_RENDER_NONE           = 0x0000, //!< No objects rendered.
		VHA_RENDER_EDGES          = 0x0001, //!< edges are rendered.
		VHA_RENDER_FACES          = 0x0002	//!< faces are rendered.
	};

/**
 * @brief Render a mesh to the open HOOPS segment.
 *
 * @role  HOOPS data is created in the open segment. All
 * responsibility for opening segments and setting of visual
 * properties is left to the application.
 *
 *
 * @param p_mesh 		Mesh to render.
 * @param render_mode 	Bitwise OR of values in VHA_RenderMode_e to control what is rendered.
 * @param ao			ACIS options object
 *
 * @return
 * Indicates success or failure with an error.
**/
DECL_VHABRIDGE
outcome api_vha_render_mesh(va_mesh* p_mesh,
                            const int render_mode = VHA_RENDER_ALL,
                            AcisOptions* ao = NULL);

/**
 * @brief Render specific elements of a mesh to the open HOOPS segment.
 *
 * @role  HOOPS data is created in the open segment. All
 * responsibility for opening segments and setting of visual
 * properties is left to the application.
 *
 * Elements to be rendered are specified using the array of elements.
 * The number of elements in the array is specified using the num_elem arguement.
 *
 * Elements in the VKI connect object for the input geometry can be displayed 
 * by setting the <tt>useInternalConnect</tt> flag in the <tt>va_base_mesh_options</tt> 
 * arguement to <tt>TRUE</tt>.
 *
 * @param p_mesh 		Mesh to render.
 * @param num_elem		Number of elements in array
 * @param elem_array	Array of elements
 * @param msopts		Mesh options
 * @param render_mode 	Bitwise OR of values in VHA_RenderMode_e to control what is rendered.
 * @param ao			ACIS options object
 *
 * @return
 * Indicates success or failure with an error.
**/
DECL_VHABRIDGE
outcome api_vha_render_mesh(va_mesh*               p_mesh,
					        int                    num_elem,
					        const int*             elem_array,
					        va_base_mesh_options*  msopts = NULL,
                            const int              render_mode = VHA_RENDER_ALL,
                            AcisOptions*           ao = NULL);

/** @} */
#endif
