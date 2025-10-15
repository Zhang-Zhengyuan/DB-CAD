/*******************************************************************/
/*    Copyright (c) 1989-2020 by Spatial Corp.                     */
/*    All rights reserved.                                         */
/*    Protected by U.S. Patents 5,257,205; 5,351,196; 6,369,815;   */
/*                              5,982,378; 6,462,738; 6,941,251    */
/*    Protected by European Patents 0503642; 69220263.3            */
/*    Protected by Hong Kong Patent 1008101A                       */
/*******************************************************************/
//----------------------------------------------------------------------
// purpose---
//    Declare procedures for working with component_entity_handle objects in scheme
//
//----------------------------------------------------------------------

#ifndef comp_ent_handle_typ_hxx
#define comp_ent_handle_typ_hxx

//======================================================================

#include "scheme.hxx"
#include "logical.h"

class component_entity_handle;
class component_entity_handle_list;


//======================================================================

logical is_Scm_Component_Entity_Handle(ScmObject);
logical is_Scm_Component_Entity_Handle_alive( ScmObject p );

ScmObject make_Scm_Component_Entity_Handle(component_entity_handle*);
ScmObject make_Scm_Component_Entity_Handle_List( int npts, component_entity_handle** cehs);

component_entity_handle* get_Scm_Component_Entity_Handle(ScmObject);
void get_Scm_Component_Entity_Handle_List( ScmObject scm_ces, component_entity_handle_list& ces);

void delete_GC_Component_Entity_Handle_Array();

component_entity_handle* scm_lookup_component_entity_handle( int comp_ent_idx, int model_idx);
int                      scm_lookup_component_entity_handle(component_entity_handle *comp_ent_hnd, 
                                                           int model_idx);

//======================================================================
#endif
