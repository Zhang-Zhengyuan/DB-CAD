/*******************************************************************/
/*    Copyright (c) 1989-2020 by Spatial Corp.                     */
/*    All rights reserved.                                         */
/*    Protected by U.S. Patents 5,257,205; 5,351,196; 6,369,815;   */
/*                              5,982,378; 6,462,738; 6,941,251    */
/*    Protected by European Patents 0503642; 69220263.3            */
/*    Protected by Hong Kong Patent 1008101A                       */
/*******************************************************************/

#ifndef ASM_UTILS_HXX
#define ASM_UTILS_HXX

#include "asm_model_mgr.hxx"
#include "asm_model_entity_mgr.hxx"
#include "vlists.hxx"
#include "lists.hxx"
#include "dcl_asm.h"

class VOID_LIST;
class component_handle;
class component_entity_handle;
class asm_flatten_attrib_helper;
class ATTRIB;
class AcisOptions;

/**
 * @nodoc
 */
outcome DECL_ASM
asmi_flatten_assembly(
				asm_model *i_model,
				asm_model_entity_mgr *o_new_entity_mgr,
				asm_flatten_attrib_helper *cust_transfer_att = NULL,
				AcisOptions* ao = NULL
				);

/**
 * @nodoc
 */
class DECL_ASM asm_flatten_attrib_helper : public ACIS_OBJECT
{
	VOID_LIST _component_maps;

	friend outcome DECL_ASM asmi_flatten_assembly(
						asm_model *i_model,
						asm_model_entity_mgr *o_new_entity_mgr,
						asm_flatten_attrib_helper *cust_transfer_att,
						AcisOptions* ao
						);
public:
	virtual bool process_attribute( ATTRIB *att, ENTITY_LIST *new_owners, const component_handle *parent_path ) = 0;
	ENTITY_LIST* get_flattened_ents(
						const component_handle *parent_path,
						const component_handle *sub_path,
						const component_entity_handle *comp_ent
						);
	virtual ~asm_flatten_attrib_helper();
};

#endif // ASM_UTILS_HXX
