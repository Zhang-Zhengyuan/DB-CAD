/*******************************************************************/
/*    Copyright (c) 1989-2020 by Spatial Corp.                     */
/*    All rights reserved.                                         */
/*    Protected by U.S. Patents 5,257,205; 5,351,196; 6,369,815;   */
/*                              5,982,378; 6,462,738; 6,941,251    */
/*    Protected by European Patents 0503642; 69220263.3            */
/*    Protected by Hong Kong Patent 1008101A                       */
/*******************************************************************/

#ifndef ASM_PROP_API_HXX
#define ASM_PROP_API_HXX

#include "dcl_asm.h"
#include "asm_event_type.hxx"
#include "api.hxx"
#include "asm_api.hxx"
#include "asm_model.hxx"

class ENTITY_LIST;
class ASM_ASSEMBLY;
class ASM_MODEL_REF;
class asm_model;
class asm_model_list;
class asm_model_entity_mgr;
class entity_mgr_factory;
class entity_handle;
class entity_handle_list;
class component_handle;
class component_handle_list;
class asm_model_info;
class asm_get_options;
class asm_cleanup_options;
class asm_save_options;
class asm_restore_options;
class ATTRIB_COMPONENT_PROP_OWNER;
class SPAbox;
class SPAboxing_options;
class ATTRIB;

/**
* @file asm_prop_api.hxx
 * @CAA2Level L1
 * @CAA2Usage U1
 * \addtogroup ACISASSEMBLIES
 *
 * @{
 */

class DECL_ASM asm_property_options
{
	asm_model* property_owner;
public:
	asm_property_options() : property_owner( NULL ) {}
	asm_property_options( asm_model* property_owning_model ) : property_owner( property_owning_model ) {}

	asm_model* get_property_owning_model() { return property_owner; }
};
/**
 *.
 */
typedef asm_property_options asm_options;

//////////////////////
// Interfaces for managing properties (attributes)
//////////////////////
 
/**
 * Adds a property to the specified component.
 * <br><br>
 * <b>Role:</b> This API attaches a property attribute to an
 * @href ATTRIB_COMPONENT_PROPERTY_OWNER entity, which points to the component, in an assembly model.
 * The model in which the property will be added is specified by the <tt>asm_property_options</tt> object;
 * by default the property is added in the component's root model.
 * <br><br>
 * If no @href ATTRIB_COMPONENT_PROPERTY_OWNER exists for the component, one is created.  
 * Refer to the section <i>Properties in Assembly Modeling</i> for more details.
 * Note that adding a property to the root component of a part model (with no assembly) is
 * not allowed. Because this is equivalent to modifying the part, part-modeling operations
 * should be used to change the contents of the part instead. Modifying the root component
 * of an assembly model is allowed, however.
 * <br><br>
 * Also note that the property attribute must be created and that this API must be called
 * while the history stream of the model in which the property will be added is active.
 * This is because this API attaches the property attribute to an entity in that model.
 * <br><br>
 * <b>Errors:</b> Non-existent component, root component of part model, property allocated
 * in different model, incorrect history stream active.
 * <br><br>
 * <b>Effect:</b> Changes model
 * <br><br>
 * <b>Journal: </b> Not Available
 * <br><br>
 * <b>Product(s):</b> 3D ACIS Exchange, 3D Viz Exchange, 3D ACIS Modeler
 * <br><br>
* @param component
 * component which is modifed by the property.
 * @param prop
 * the property attribute being attached.
 * @param asmo
 * options object - controls model in which property is applied.
 * @param ao
 * ACIS options.
 **/
DECL_ASM outcome api_asm_component_add_property(
    component_handle*      component, 
    ATTRIB*                prop, 
	asm_property_options*  asmo = NULL,
    AcisOptions*           ao = NULL);

/**
 * Determines if a property of the specified type has been added to the specified component.
 * <br><br>
 * <b>Role:</b> This API is used to determine if a property has been added to a component in
 * a model by @href api_asm_component_add_property. It is intended to be called before
 * calling @href api_asm_component_add_property to determine if a new property attribute needs
 * to be added to a component, or if an existing one (returned by this routine) can be re-used.
 * <br><br>
 * The routine searches the @href ATTRIB_COMPONENT_PROPERTY_OWNER entity associated with the component
 * in that model for an attribute of the specified type, and returns a pointer to the first such
 * attribute that is found. 
 * If no @href ATTRIB_COMPONENT_PROPERTY_OWNER exists for the component in that model, or if
 * the component does not have an attribute of the specified type attached, a <tt>NULL</tt> 
 * pointer is returned.
 * <br><br>
 * <b>Note:&nbsp;&nbsp;</b>This API must be called while the history stream of the model 
 * being searched is active.
 * <br><br>
 * <b>Errors:</b> Incorrect history stream active.
 * <br><br>
 * <b>Effect:</b> Changes model
 * <br><br>
 * <b>Journal: </b> Not Available
 * <br><br>
 * <b>Product(s):</b> 3D ACIS Exchange, 3D Viz Exchange, 3D ACIS Modeler
 * <br><br>
* @param component
 * component that the property modifies.
 * @param attrib_type
 * the type ID of the attribute being sought.
 * @param prop
 * the property attribute that was found, or NULL.
 * @param asmo
 * options object - controls model in which property is applied.
 * @param ao
 * ACIS options.
 **/
DECL_ASM outcome api_asm_component_has_property(
    component_handle*      component, 
	int                    attrib_type,
    ATTRIB*	&              prop, 
	asm_property_options*  asmo = NULL,
    AcisOptions*           ao = NULL);

/**
 * Suppresses the specified component of an assembly model.
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
 * component to be suppressed.
 * @param asmo
 * options object - controls model in which suppression is applied.
 * @param ao
 * ACIS options.
 **/
DECL_ASM outcome asmi_component_suppress(
	component_handle* component,
	asm_property_options* asmo = NULL,
	AcisOptions* ao = NULL);

/**
 * Determines if the specified component is suppressed by any property in its
 * owning model's assembly tree.
 * <br><br>
 * <b>Role:</b> This routine searches for any "suppress" properties which affect the input component.
 * If one or more suppress properties are found, TRUE is returned.  
 * If no suppress properties are found, FALSE is returned.
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
 * @param is_suppressed
 * TRUE if component is suppressed.
 * @param ao
 * ACIS options.
 **/
DECL_ASM outcome asmi_component_is_suppressed(
	component_handle* component,
	logical& is_suppressed,
	AcisOptions* ao = NULL);

/**
 * Removes the suppression property from the specified component of an assembly model.
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
 * component to have suppression removed.
 * @param asmo
 * options object - controls model in which suppression was applied.
 * @param ao
 * ACIS options.
 **/
DECL_ASM outcome asmi_component_unsuppress(
	component_handle* component,
	asm_property_options* asmo = NULL,
	AcisOptions* ao = NULL);

/**
 * Determines if there are physical properties (such as suppression) that affect the
 * specified component of an assembly model.
 * <br><br>
 * <b>Role:</b> A physical property is one that changes one or more components of the
 * assembly in such a way that the (un-instanced) B-Rep 
 * model equivalent to the assembly would be different.
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
 * TRUE if one or more proper sub-components of the component are modified by one or more physical properties 
 * owned by a model above the component's end model.
 * @param has_component_modifications
 * TRUE if the component is modified by one or more physical properties owned by a model above the component's end model.
 * @param has_parent_modifications
 * TRUE if one or more parents of the component are modified by one or more physical properties.
 * @param ao
 * ACIS options.
 **/
DECL_ASM outcome asmi_component_has_physical_modifications(
	component_handle* component,
	logical& has_child_modifications,
	logical& has_component_modifications,
	logical& has_parent_modifications,
	AcisOptions* ao = NULL);

/**
 * Hides the specified component within an assembly model.
 * <br><br>
 * <b>Errors:</b> non-existent component, root component of part model.
 * <br><br>
 * <b>Effect:</b> Changes model
 * <br><br>
 * <b>Journal: </b> Not Available
 * <br><br>
 * <b>Product(s):</b> 3D ACIS Exchange, 3D Viz Exchange, 3D ACIS Modeler
 * <br><br>
* @param component
 * component to be hidden.
 * @param asmo
 * options object - controls model in which hiding is applied.
 * @param ao
 * ACIS options.
 **/
DECL_ASM outcome asmi_component_hide(
	component_handle* component,
	asm_property_options* asmo = NULL,
	AcisOptions* ao = NULL);

/**
 * Determines if the specified component is hidden by any property in its
 * owning model's assembly tree.
 * <br><br>
 * <b>Role:</b> This routine searches for any "hiding" properties which affect the input component.
 * If one or more hiding properties are found, TRUE is returned.  
 * If no hiding properties are found, FALSE is returned.
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
 * @param is_hidden
 * TRUE if component is hidden.
 * @param ao
 * ACIS options.
 **/
DECL_ASM outcome asmi_component_is_hidden(
	component_handle* component,
	logical& is_hidden,
	AcisOptions* ao = NULL);

/**
 * Removes the hide property from the specified component of an assembly model.
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
 * component to be unhidden.
 * @param asmo
 * options object - controls model in which hiding was applied.
 * @param ao
 * ACIS options.
 **/
DECL_ASM outcome asmi_component_unhide(
	component_handle* component,
	asm_property_options* asmo = NULL,
	AcisOptions* ao = NULL);

/**
 * Determines if there are properties that hide the specified component of an assembly model.
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
 * TRUE if one or more proper sub-components of the component are hidden by one or more properties 
 * owned by a model above the component's end model.
 * @param has_component_modifications
 * TRUE if the component is hidden by one or more properties owned by a model above the component's end model.
 * @param has_parent_modifications
 * TRUE if one or more parents of the component are hidden by one or more properties.
 * @param ao
 * ACIS options.
 **/
DECL_ASM outcome asmi_component_has_hiding_modifications(
	component_handle* component,
	logical& has_child_modifications,
	logical& has_component_modifications,
	logical& has_parent_modifications,
	AcisOptions* ao = NULL);

/**
 * Searches an assembly tree for the highest priority property attribute, of the specified type, that modifies
 * a component of an assembly.
 * <br><br>
 * <b>Errors:</b> non-existent component. 
 * <br><br>
 * <b>Effect:</b> Read-only
 * <br><br>
 * <b>Journal: </b> Not Available
 * <br><br>
 * <b>Product(s):</b> 3D ACIS Exchange, 3D Viz Exchange, 3D ACIS Modeler
 * <br><br>
* @param component
 * component that is modified by the property to be found.
 * @param attrib_type
 * type identifier for the attribute class.  The routine will search for attributes
 * of this type or derived from this type.
 * @param the_property
 * entity handle for the attribute of the specified type (or a sub-type).
 * @param ao
 * ACIS options.
 **/
DECL_ASM outcome asmi_component_find_property(
    component_handle*	component, 
	int					attrib_type,
    entity_handle*	&	the_property, 
    AcisOptions*		ao = NULL);

/**
 * Searches an assembly tree for the next highest priority property attribute, of the specified type, that modifies
 * a component of an assembly.
 * <br><br>
 * <b>Errors:</b> non-existent component. 
 * <br><br>
 * <b>Effect:</b> Read-only
 * <br><br>
 * <b>Journal: </b> Not Available
 * <br><br>
 * <b>Product(s):</b> 3D ACIS Exchange, 3D Viz Exchange, 3D ACIS Modeler
 * <br><br>
* @param component
 * component that is modified by the property to be found.
 * @param attrib_type
 * type identifier for the attribute class.  The routine will search for attributes of
 * this type or derived from this type.
 * @param start_attrib
 * entity handle for the attribute at which the search will continue.
 * @param the_property
 * entity handle for the attribute of the specified type (or a sub-type).
 * @param ao
 * ACIS options.
 **/
DECL_ASM outcome asmi_component_find_next_property(
    component_handle*	component, 
	int					attrib_type,
	entity_handle* start_attrib,
    entity_handle*	&	the_property, 
    AcisOptions*		ao = NULL);

/**
 * Determines if there are property attributes, of the specified type, that affect the
 * specified component of an assembly model.
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
 * @param attrib_type
 * type identifier for the attribute class.  The routine will search for attributes of this
 * type or derived from this type.
 * @param has_child_modifications
 * TRUE if one or more proper sub-components of the component are modified by one or more
 * properties owned by a model above the component's end model.
 * @param has_component_modifications
 * TRUE if the component is modified by one or more properties owned by a model above the
 * component's end model.
 * @param has_parent_modifications
 * TRUE if one or more parents of the component are modified by one or more properties.
 * @param ao
 * ACIS options.
 **/
DECL_ASM outcome asmi_component_has_property_modifications(
	component_handle* component,
	int		 attrib_type,
	logical& has_child_modifications,
	logical& has_component_modifications,
	logical& has_parent_modifications,
	AcisOptions* ao = NULL);

/**
 * Removes a previously applied property attribute from its owner.  
 * <br><br>
 * <b>Role:</b> This routine unhooks and loses the attribute. 
 * If the property being removed is the last attribute attached to the property-owner
 * attribute, the property-owner attribute is also unhooked and lost.
 * <br><br>
 * <b>Errors:</b> non-existent component entity. 
 * <br><br>
 * <b>Effect:</b> Changes model
 * <br><br>
 * <b>Journal: </b> Not Available
 * <br><br>
 * <b>Product(s):</b> 3D ACIS Exchange, 3D Viz Exchange, 3D ACIS Modeler
 * <br><br>
* @param the_property
 * entity handle for the attribute that is to be removed.
 * @param ao
 * ACIS options.
 **/
DECL_ASM outcome asmi_property_remove(
	entity_handle* the_property,
    AcisOptions*		ao = NULL);

/**
 * Finds the component property-owner attribute in a specific model that
 * modifies a specific component.
 * <br><br>
 * <b>Role:</b> If an attribute is not found, one will be created if the
 * <tt>create_if_absent</tt> flag is TRUE.
 * Note that <tt>owning_model</tt> is <b>not</b> necessarily the root model of the
 * component; it can be above the component's root model in the assembly tree.
 * <br><br>
 * <b>Errors:</b> NULL component, NULL model, NULL assembly. 
 * <br><br>
 * <b>Effect:</b> Changes model
 * <br><br>
 * <b>Journal: </b> Not Available
 * <br><br>
 * <b>Product(s):</b> 3D ACIS Exchange, 3D Viz Exchange, 3D ACIS Modeler
 * <br><br>
* @param comp
 * model component to which the attribute refers.
 * @param owner_attrib
 * the attribute.
 * @param create_if_absent
 * create an attribute if one is not found.
 * @param asmo
 * options object - controls model which owns property owner.
 * @param ao
 * ACIS options.
 **/
DECL_ASM outcome api_asm_component_get_property_owner(
	component_handle*            	comp, 
	ATTRIB_COMPONENT_PROP_OWNER*&	owner_attrib, 
	logical create_if_absent,
	asm_property_options*		asmo = NULL,
    AcisOptions*		ao = NULL);

/**
 * Returns all of the component property-owner attributes that modify a specific component.
 * <br><br>
 * <b>Errors:</b> NULL component. 
 * <br><br>
 * <b>Effect:</b> Read-only
 * <br><br>
 * <b>Journal: </b> Not Available
 * <br><br>
 * <b>Product(s):</b> 3D ACIS Exchange, 3D Viz Exchange, 3D ACIS Modeler
 * <br><br>
* @param component
 * component for which to find property-owner attributes.
 * @param prop_atts
 * prioritized handle list for the attributes found;
 * property owners earlier in the list over-ride those later in the list.
 * @param ao
 * ACIS options.
 **/
DECL_ASM outcome asmi_component_get_property_owners(
	const component_handle*	component, 
	entity_handle_list&				prop_atts,
    AcisOptions*				ao = NULL); 

/**
 * Returns the component property-owner attributes that are owned by the specified model.
 * <br><br>
 * <b>Errors:</b> NULL model pointer.
 * <br><br>
 * <b>Effect:</b> Read-only.
 * <br><br>
 * <b>Journal: </b> Not Available
 * <br><br>
 * <b>Product(s):</b> 3D ACIS Exchange, 3D Viz Exchange, 3D ACIS Modeler
 * <br><br>
* @param model
 * model owning the property owners.
 * @param prop_atts
 * list of component property-owner attributes owned by the model.
 * @param ao
 * ACIS options.
 **/
DECL_ASM outcome api_asm_model_get_owned_property_owners(
	const asm_model* model,
	ENTITY_LIST& prop_atts,
	AcisOptions* ao = NULL);

/**
 * Returns a list of all components of the specified model that have a given property.
 * <br><br>
 * <b>Errors:</b> NULL model pointer.
 * <br><br>
 * <b>Effect:</b> Read-only.
 * <br><br>
 * <b>Journal: </b> Not Available
 * <br><br>
 * <b>Product(s):</b> 3D ACIS Exchange, 3D Viz Exchange, 3D ACIS Modeler
 * <br><br>
* @param model
 * model whose components are examined for the given property.
 * @param attrib_type
 * attribute type for the property being sought.
 * @param prop_comps
 * list of components having the given property.
 * @param ao
 * ACIS options.
 **/
DECL_ASM outcome api_asm_model_find_components_with_property(
	const asm_model*		model,
	int						attrib_type,
	component_handle_list&	prop_comps,
	AcisOptions*			ao = NULL);


 /** @} */
#endif // ASM_PROP_API_HXX

