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
//    Declare procedures for working with asm_model objects in scheme
//
//----------------------------------------------------------------------

#ifndef asm_model_typ_hxx
#define asm_model_typ_hxx

//======================================================================

#include "scheme.hxx"
#include "logical.h"
#include "gc_obj.hxx"

class asm_model;
class asm_model_list;

//======================================================================

logical     is_Scm_Asm_Model(ScmObject);
logical     is_Scm_Asm_Model_alive(ScmObject);
logical     is_Scm_Asm_Model_List(ScmObject mdls);

ScmObject   make_Scm_Asm_Model(asm_model*);
ScmObject   make_Scm_Asm_Model_List( int npts, asm_model** ars );
ScmObject   make_Scm_Asm_Model_List(  asm_model_list & ll  );

asm_model*  get_Scm_Asm_Model(ScmObject);
void        get_Scm_Asm_Model_List(  ScmObject scm_mdls, asm_model_list& mdls );

void        delete_GC_Asm_Models_Array();

asm_model*  scm_lookup_asm_model(int model_id);
int         scm_lookup_asm_model(asm_model *model);
logical     is_asm_model_alive(asm_model *model);

//======================================================================

// Global constants.
#define MODELS_MIN_IDX_VALUE  1
#define ID_NOT_FOUND  -1

#endif
