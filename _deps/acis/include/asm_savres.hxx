/*******************************************************************/
/*    Copyright (c) 1989-2020 by Spatial Corp.                     */
/*    All rights reserved.                                         */
/*    Protected by U.S. Patents 5,257,205; 5,351,196; 6,369,815;   */
/*                              5,982,378; 6,462,738; 6,941,251    */
/*    Protected by European Patents 0503642; 69220263.3            */
/*    Protected by Hong Kong Patent 1008101A                       */
/*******************************************************************/

#ifndef ASM_SAVRES_HXX
#define ASM_SAVRES_HXX

#include "dcl_asm.h"
#include "savres.hxx"

class asm_model_holder;
class entity_handle_holder;
class component_handle_holder;
class component_entity_handle_holder;

// bitmask to distinguish between SAT and ASAT
#define IS_ASAT_FILE 0x02

// helper routines for ENTITY to read/write holder objects

DECL_ASM void write_asm_model_hldr(const asm_model_holder& model_hldr);
DECL_ASM void read_asm_model_hldr(asm_model_holder& model_hldr);
DECL_ASM void fixup_asm_model_hldr(asm_model_holder& model_hldr);

DECL_ASM void write_comp_hdl_hldr( const component_handle_holder& hdl_hldr );
DECL_ASM void read_comp_hdl_hldr( component_handle_holder& hdl_hldr );
DECL_ASM void fixup_comp_hdl_hldr( component_handle_holder& hdl_hldr );

DECL_ASM void write_ent_hdl_hldr( const entity_handle_holder& hdl_hldr );
DECL_ASM void read_ent_hdl_hldr( entity_handle_holder& hdl_hldr );
DECL_ASM void fixup_ent_hdl_hldr( entity_handle_holder& hdl_hldr );

DECL_ASM void write_comp_ent_hdl_hldr( const component_entity_handle_holder& hdl_hldr );
DECL_ASM void read_comp_ent_hdl_hldr( component_entity_handle_holder& hdl_hldr );
DECL_ASM void fixup_comp_ent_hdl_hldr( component_entity_handle_holder& hdl_hldr );

#endif // ASM_SAVRES_HXX
