/*******************************************************************/
/*    Copyright (c) 2005-2020 by Spatial Corp.                     */
/*    All rights reserved.                                         */
/*    Protected by U.S. Patents 5,257,205; 5,351,196; 6,369,815;   */
/*                              5,982,378; 6,462,738; 6,941,251    */
/*    Protected by European Patents 0503642; 69220263.3            */
/*    Protected by Hong Kong Patent 1008101A                       */
/*******************************************************************/

#ifndef GA_ASM_HXX
#define GA_ASM_HXX

#include "dcl_asm.h"
#include "api.hxx"

class entity_handle;
class component_handle;
class AcisOptions;

/**
* @file ga_asm_api.hxx
 * @CAA2Level L1
 * @CAA2Usage U1
 * \addtogroup ACISASSEMBLIES
 *
 * @{
 */

/**
 * Sets the name of the specified component in the assembly model.
 * <br><br>
 * <b>Errors:</b> non-existent component, root component of part model, wrong history stream active.
 * <br><br>
 * <b>Effect:</b> Changes model
 * <br><br>
 * <b>Journal: </b> Not Available
 * <br><br>
 * <b>Product(s):</b> 3D ACIS Exchange, 3D Viz Exchange, 3D ACIS Modeler
 * <br><br>
* @param component
 * component to be named.
 * @param name
 * component name.
 * @param ao
 * ACIS options.
 **/
DECL_ASM outcome asmi_component_set_name(
	component_handle* component,
	const SPAWCHAR_T* name,
	AcisOptions* ao = NULL);

/**
 * Finds the name of the specified component in the assembly model.
 * <br><br>
 * <b>Errors:</b> non-existent component, root component of part model, wrong history stream active.
 * <br><br>
 * <b>Effect:</b> Read-only.
 * <br><br>
 * <b>Journal: </b> Not Available
 * <br><br>
 * <b>Product(s):</b> 3D ACIS Exchange, 3D Viz Exchange, 3D ACIS Modeler
 * <br><br>
* @param component
 * named component.
 * &name
 * component name.
 * @param ao
 * ACIS options.
 **/
DECL_ASM outcome asmi_component_find_name(
	component_handle* component,
	const SPAWCHAR_T*& name,
	AcisOptions* ao = NULL);

// INTERNAL USE ONLY - DO NOT USE
/**
 * @nodoc
 */
DECL_ASM outcome asmi_component_get_name(
	component_handle* component,
	const SPAWCHAR_T*& name,
	AcisOptions* ao = NULL);

 /** @} */
#endif
