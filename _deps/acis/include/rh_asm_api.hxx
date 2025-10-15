/*******************************************************************/
/*    Copyright (c) 1989-2020 by Spatial Corp.                     */
/*    All rights reserved.                                         */
/*    Protected by U.S. Patents 5,257,205; 5,351,196; 6,369,815;   */
/*                              5,982,378; 6,462,738; 6,941,251    */
/*    Protected by European Patents 0503642; 69220263.3            */
/*    Protected by Hong Kong Patent 1008101A                       */
/*******************************************************************/

#ifndef RH_ASM_API_HXX
#define RH_ASM_API_HXX

#include "dcl_asm.h"
#include "api.hxx"

class asm_model;
class entity_handle;
class component_handle;
class rgb_color;
class asm_property_options;

/**
* @file rh_asm_api.hxx
 * @CAA2Level L1
 * @CAA2Usage U1
 * \addtogroup ACISASSEMBLIES
 *
 * @{
 */


/**
 * Sets the color at the assembly level of the specified component within an assembly.
 * <br><br>
 * <b>Errors:</b> non-existent component, root component of part model
 * <br><br>
 * <b>Effect:</b> Changes model
 * <br><br>
 * <b>Journal: </b> Not Available
 * <br><br>
 * <b>Product(s):</b> 3D ACIS Exchange, 3D Viz Exchange, 3D ACIS Modeler
 * <br><br>
* @param component
 * component whose color is to be set.
 * @param color
 * rgb color.
 * @param asmo
 * options object - controls model in which color is applied.
 * @param ao
 * ACIS options.
 **/
DECL_ASM outcome asmi_component_set_color(
	component_handle* component,
	rgb_color& color,
	asm_property_options* asmo = NULL,
	AcisOptions* ao = NULL);

/**
 * Gets the highest priority color property affecting a component of an assembly.
 * <br><br>
 * <b>Role:</b> This routine searches for all color properties which affect the input component.
 * If one or more color properties are found, the color of the one with highest priority
 * (see the technical article Properties in Assembly Modeling for a discussion of
 * the priority rules) is returned and the <tt>found</tt> flag is set to TRUE.  
 * If no color properties are found, the 
 * default color is returned and the <tt>found</tt> flag is set to FALSE.
 * <br><br>
 * <b>Errors:</b> non-existent component
 * <br><br>
 * <b>Effect:</b> Read-only.
 * <br><br>
 * <b>Journal: </b> Not Available
 * <br><br>
 * <b>Product(s):</b> 3D ACIS Exchange, 3D Viz Exchange, 3D ACIS Modeler
 * <br><br>
* @param component
 * component to query.
 * @param color
 * rgb color.
 * @param found
 * logical: TRUE if color property is found, otherwise FALSE.
 * @param ao
 * ACIS options.
 **/
DECL_ASM outcome asmi_component_find_color(
	component_handle* component,
	rgb_color& color,
	logical& found,
	AcisOptions* ao = NULL);

/**
 * Removes the assembly-level color property of the specified component within an assembly.
 * <br><br>
 * <b>Errors:</b> non-existent component
 * <br><br>
 * <b>Effect:</b> Changes model
 * <br><br>
 * <b>Journal: </b> Not Available
 * <br><br>
 * <b>Product(s):</b> 3D ACIS Exchange, 3D Viz Exchange, 3D ACIS Modeler
 * <br><br>
* @param component
 * component whose color attribute is to be removed.
 * @param asmo
 * options object - controls model in which color was applied.
 * @param ao
 * ACIS options.
 **/
DECL_ASM outcome asmi_component_remove_color(
	component_handle* component,
	asm_property_options* asmo = NULL,
	AcisOptions* ao = NULL);

/**
 * Determines if there are color properties that affect the specified component of an
 * assembly model.
 * <br><br>
 * <b>Errors:</b> non-existent component, root component of part model.
 * <br><br>
 * <b>Effect:</b> Read-only.
 * <br><br>
 * <b>Journal: </b> Not Available
 * <br><br>
 * <b>Product(s):</b> 3D ACIS Exchange, 3D Viz Exchange, 3D ACIS Modeler
 * <br><br>
* @param component
 * component to query.
 * @param has_child_modifications
 * TRUE if one or more proper sub-components of the component are modified by one or more color properties 
 * owned by a model above the component's end model.
 * @param has_component_modifications
 * TRUE if the component is modified by one or more color properties owned by a model above the component's end model.
 * @param has_parent_modifications
 * TRUE if one or more parents of the component are modified by one or more color properties.
 * @param ao
 * ACIS options.
 **/
DECL_ASM outcome asmi_component_has_color_modifications(
	component_handle* component,
	logical& has_child_modifications,
	logical& has_component_modifications,
	logical& has_parent_modifications,
	AcisOptions* ao = NULL);

/**
 * Sets the transparency of the specified component of an assembly model.
 * <br><br>
 * <b>Role:</b> The transparency value must be between 0 (transparent) and 1 (opaque).  
 * <br><br>
 * <b>Errors:</b> non-existent component, root component of part model
 * <br><br>
 * <b>Effect:</b> Changes model
 * <br><br>
 * <b>Journal: </b> Not Available
 * <br><br>
 * <b>Product(s):</b> 3D ACIS Exchange, 3D Viz Exchange, 3D ACIS Modeler
 * <br><br>
* @param component
 * component whose transparency is to be set.
 * @param transparency
 * transparency value (0 for transparent, 1 for opaque).
 * @param asmo
 * options object - controls model in which transparency is applied.
 * @param ao
 * ACIS options.
 **/
DECL_ASM outcome asmi_component_set_transparency(
	component_handle* component,
	double transparency,
	asm_property_options* asmo = NULL,
	AcisOptions* ao = NULL);

/**
 * Gets the highest priority transparency property affecting a component of an
 * assembly.
 * <br><br>
 * <b>Role:</b> The transparency value lies between 0 (transparent) and 1 (opaque).
 * <br><br>
 * <b>Errors:</b> Non-existent component
 * <br><br>
 * <b>Effect:</b> Read-only.
 * <br><br>
 * <b>Journal: </b> Not Available
 * <br><br>
 * <b>Product(s):</b> 3D ACIS Exchange, 3D Viz Exchange, 3D ACIS Modeler
 * <br><br>
* @param component
 * component to query.
 * @param transparency
 * transparency value (0 for transparent, 1 for opaque).
 * @param ao
 * ACIS options.
 **/
DECL_ASM outcome asmi_component_find_transparency(
	component_handle* component,
	double& transparency,
	AcisOptions* ao = NULL);

/**
 * Determines if there are material properties which affect the specified component in the assembly model.
 * <br><br>
 * <b>Errors:</b> non-existent component, root component of part model.
 * <br><br>
 * <b>Effect:</b> Read-only.
 * <br><br>
 * <b>Journal: </b> Not Available
 * <br><br>
 * <b>Product(s):</b> 3D ACIS Exchange, 3D Viz Exchange, 3D ACIS Modeler
 * <br><br>
* @param component
 * component to query.
 * @param has_child_modifications
 * TRUE if one or more proper sub-components of the component are modified by one or more material properties 
 * owned by a model above the component's end model.
 * @param has_component_modifications
 * TRUE if the component is modified by one or more material properties owned by a model above the component's end model.
 * @param has_parent_modifications
 * TRUE if one or more parents of the component are modified by one or more material properties.
 * @param ao
 * ACIS options.
 **/
DECL_ASM outcome asmi_component_has_material_modifications(
	component_handle* component,
	logical& has_child_modifications,
	logical& has_component_modifications,
	logical& has_parent_modifications,
	AcisOptions* ao = NULL);

 /** @} */
#endif // RH_ASM_API_HXX

