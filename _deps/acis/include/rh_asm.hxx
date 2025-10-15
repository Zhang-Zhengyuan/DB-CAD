/*******************************************************************/
/*    Copyright (c) 1989-2020 by Spatial Corp.                     */
/*    All rights reserved.                                         */
/*    Protected by U.S. Patents 5,257,205; 5,351,196; 6,369,815;   */
/*                              5,982,378; 6,462,738; 6,941,251    */
/*    Protected by European Patents 0503642; 69220263.3            */
/*    Protected by Hong Kong Patent 1008101A                       */
/*******************************************************************/

#ifndef __RH_ASM_H__
#define __RH_ASM_H__

#include "dcl_asm.h"
#include "logical.h"
#include "rgbcolor.hxx"

class asm_model;
class component_handle;
class ATTRIB_COMPONENT_PROP_OWNER;

DECL_ASM rgb_color
get_comp_color(
	component_handle* comp, 
	ATTRIB_COMPONENT_PROP_OWNER*& owner); // owner set to NULL if color not found

DECL_ASM void 
set_comp_color(
	asm_model* owning_model,
	component_handle* comp,
	const rgb_color& color);


#endif // __RH_ASM_H__
