/*******************************************************************/
/*    Copyright (c) 1989-2020 by Spatial Corp.                     */
/*    All rights reserved.                                         */
/*    Protected by U.S. Patents 5,257,205; 5,351,196; 6,369,815;   */
/*                              5,982,378; 6,462,738; 6,941,251    */
/*    Protected by European Patents 0503642; 69220263.3            */
/*    Protected by Hong Kong Patent 1008101A                       */
/*******************************************************************/

#ifndef AT_COMP_PROP_H
#define AT_COMP_PROP_H

#include "dcl_asm.h"
#include "at_sys.hxx"
#include "comp_handle_holder.hxx"

class component_handle;
class asm_model;

/**
 * @nodoc
 */
ENTITY_IS_PROTOTYPE(ATTRIB_COMPONENT_PROP_OWNER, ASM)
#if 0
; // semicolon needed for mkman (doc tool) parsing
#endif

extern DECL_ASM int ATTRIB_COMPONENT_PROP_OWNER_TYPE;
#define ATTRIB_COMPONENT_PROP_OWNER_LEVEL ( ATTRIB_SYS_LEVEL + 1 )

class DECL_ASM ATTRIB_COMPONENT_PROP_OWNER : public ATTRIB_SYS
{
	component_handle_holder comp_hldr;

	asm_model const* owning_model_ptr; // used during lose and roll ONLY for cache invalidation
	                         // do NOT dereference since it will be an invalid pointer if model was cleand up.
public:

	ATTRIB_COMPONENT_PROP_OWNER(
        ENTITY* owner = NULL,
        component_handle* comp = NULL,
		asm_model const* owning_model = NULL);

    virtual logical pattern_compatible() const;
                                    // returns TRUE 

    component_handle* get_component() const;

	void set_owning_model(asm_model const* owning_model);

	/**
	 * @nodoc
	 */
	ATTRIB_FUNCTIONS(ATTRIB_COMPONENT_PROP_OWNER, ASM)
	/**
	 * @nodoc
	 */
	ROLL_NOTIFY_FUNCTION
#if 0
; // semicolon needed for mkman (doc tool) parsing)
#endif
// Because mkman isn't aware that ATTRIB_FUNCTIONS changes access to "public", we do so
// explicitly here:
public:
};

// do not doc
// finds the first attribute consistent with the given component
// (interprets NULL argument to mean "any")
/**
 * @nodoc
 */
DECL_ASM ATTRIB_COMPONENT_PROP_OWNER* 
find_comp_prop_attrib(
	ENTITY*						owner, 
	const component_handle*	comp = NULL);

/**
 * @nodoc
 */
DECL_ASM ATTRIB_COMPONENT_PROP_OWNER* 
find_next_comp_prop_attrib(
	ATTRIB*						att, 
	const component_handle*	comp = NULL); 

#endif // AT_COMP_PROP_H

