/*******************************************************************/
/*    Copyright (c) 1989-2020 by Spatial Corp.                     */
/*    All rights reserved.                                         */
/*    Protected by U.S. Patents 5,257,205; 5,351,196; 6,369,815;   */
/*                              5,982,378; 6,462,738; 6,941,251    */
/*    Protected by European Patents 0503642; 69220263.3            */
/*    Protected by Hong Kong Patent 1008101A                       */
/*******************************************************************/

#ifndef ASM_MODEL_MGR_HXX
#define ASM_MODEL_MGR_HXX

#include "acis.hxx"
#include "dcl_asm.h"
#include "asm_model_list.hxx"


class asm_cleanup_options;


class DECL_ASM asm_model_mgr : public ACIS_OBJECT
{
	asm_model_list model_list;

	// asm_model registers and deregisters itself
	friend class asm_model;
	void add( asm_model* model );	// add the model
	void remove( asm_model* model );	// remove the model

public:
	asm_model_mgr();
	~asm_model_mgr();	// clean up all remaining models

	void cleanup( logical force_clear, asm_cleanup_options const* co );// go through list of models and clean up handles and delete the model if the use count is zero

	// nodoc - used during save/restore
	int model_count();
	int lookup(asm_model const* model);

	asm_model* operator[](int i);
	asm_model* first();
	asm_model* next();

	void link_models() const;
	void unlink_models() const;

};

// global model manager routines
DECL_ASM asm_model_mgr* sg_asm_model_mgr();
DECL_ASM void sg_asm_model_mgr_cleanup( logical force_clear, asm_cleanup_options const* co = NULL );
DECL_ASM void sg_asm_model_cleanup( asm_model_list& model_list, logical force_clear, asm_cleanup_options const* co = NULL );

logical terminate_assembly();

#endif // ASM_MODEL_MGR_HXX

