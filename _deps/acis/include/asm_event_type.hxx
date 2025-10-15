/*******************************************************************/
/*    Copyright (c) 1989-2020 by Spatial Corp.                     */
/*    All rights reserved.                                         */
/*    Protected by U.S. Patents 5,257,205; 5,351,196; 6,369,815;   */
/*                              5,982,378; 6,462,738; 6,941,251    */
/*    Protected by European Patents 0503642; 69220263.3            */
/*    Protected by Hong Kong Patent 1008101A                       */
/*******************************************************************/

#ifndef ASM_EVENT_TYPE_HXX
#define ASM_EVENT_TYPE_HXX

/**
* @file asm_event_type.hxx
 * @CAA2Level L1
 * @CAA2Usage U1
 * \addtogroup ACISASSEMBLIES
 *
 * @{
 */

/**
 * Enumeration to indicate the degree of change which occurred to entities within a model during a MODEL_BEGIN/MODEL_END block.
 * <br>
 * <b>Role:</b> <tt>asm_event_type</tt> is an enumerated type used by <tt>MODEL_END</tt> and <tt>API_MODEL_END</tt>
 * to indicate the type of change that was made to entities in a model while it was active.  Only the most severe
 * type of change should be reported.
 * <br>
 * @param ASM_BREP_GEOM_CHANGE
 * indicates that a change occurred to entities within the model that affects the model's physical properties (e.g. mass properties).
 * An example of an action which would require this notification would be performing a Boolean operation on an entity within the model.
 * @param ASM_COMP_GEOM_CHANGE
 * indicates that a change occurred to the assembly structure of the model that affects the model's physical properties (e.g. mass properties).
 * Examples would be: deleting the model's assembly; adding, deleting or moving model references owned by the assembly;
 * or applying a physical property, such as suppression, in the assembly model.
 * @param ASM_BREP_RENDER_CHANGE
 * indicates that a change occurred to entities within the model that affects the way the model is rendered,
 * and that the change was not of type ASM_GEOM_CHANGE.  An example of this would be applying a color to a body within the model.
 * @param ASM_COMP_RENDER_CHANGE
 * indicates that a change occurred to the assembly that affects the way the model is rendered,
 * and that the change was not of type ASM_GEOM_CHANGE.  An example would be adding a color property to a component.
 * @param ASM_OTHER_CHANGE
 * indicates that a change occurred to entities within the model (e.g. adding a name), but the
 * the change was not of type ASM_BREP_GEOM_CHANGE, ASM_COMP_GEOM_CHANGE, ASM_BREP_RENDER_CHANGE, or ASM_COMP_RENDER_CHANGE.
 * Examples would be adding a name to a component or adding a user attribute to a model.
 * @param ASM_NO_CHANGE
 * indicates that no significant change to entities within the model occurred.
 * <br><br>
 * @see MODEL_BEGIN, MODEL_END, asm_model_entity_mgr
 **/

enum asm_event_type
{
	ASM_BREP_GEOM_CHANGE,
	ASM_COMP_GEOM_CHANGE,
	ASM_BREP_RENDER_CHANGE,
	ASM_COMP_RENDER_CHANGE,
	ASM_OTHER_CHANGE,
	ASM_NO_CHANGE
};


 /** @} */
#endif // ASM_EVENT_TYPE_HXX

