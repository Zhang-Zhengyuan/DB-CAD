/*******************************************************************/
/*    Copyright (c) 1989-2020 by Spatial Corp.                     */
/*    All rights reserved.                                         */
/*    Protected by U.S. Patents 5,257,205; 5,351,196; 6,369,815;   */
/*                              5,982,378; 6,462,738; 6,941,251    */
/*    Protected by European Patents 0503642; 69220263.3            */
/*    Protected by Hong Kong Patent 1008101A                       */
/*******************************************************************/

#ifndef ASSEMBLY_CLASS
#define ASSEMBLY_CLASS

#include "dcl_kern.h"
#include "entity.hxx"
#include "logical.h"
class asm_model_list;
class ASM_MODEL_REF;
class asm_model_entity_mgr;
class ENTITY_LIST;
class asm_model;
class entity_handle;
#include "en_macro.hxx"
/**
* @file asm_assembly.hxx
 * @CAA2Level L1
 * @CAA2Usage U1
 * \addtogroup KERNAPI
 *
 * @{
 */
/**
 * @nodoc
 */
ENTITY_IS_PROTOTYPE( ASM_ASSEMBLY, KERN )
#if 0
; // semicolon needed for mkman (doc tool) parsing
#endif

// Identifier used to find out (via identity() defined below) to what
// an entity pointer refers.

extern DECL_KERN int ASM_ASSEMBLY_TYPE;


// Identifier that gives number of levels of derivation of this class
// from ENTITY

#define ASM_ASSEMBLY_LEVEL 1
// Never derive from ASM_ASSEMBLY
/**
 * Assembly Entity
 */
class DECL_KERN ASM_ASSEMBLY : public ENTITY
{
	ASM_MODEL_REF* model_ref_ptr;
	asm_model_entity_mgr* mgr_ptr; // pointer to owning asm_model_entity_mgr
	                     // customer's application is responsible for maintaining this pointer
	                     // needed so that low items in tree can return lists of things that depend on them

	// utility routines

	void get_model_refs_engine(ENTITY_LIST & model_refs, asm_model const* model) const; // appends only model_refs of model owned by this assembly (sub-assembly model_refs not included)

protected:
////////// ENTITY DEFINITION STUFF //////////////////////
	/**
	 * @nodoc
	 */
//    virtual logical bulletin_no_change_vf(ENTITY const* other, logical identical_comparator) const;
	// Include the standard member functions for all entities.

	/**
	 * @nodoc
	 */
	ENTITY_FUNCTIONS( ASM_ASSEMBLY , KERN)
#if 0
; // semicolon needed for mkman (doc tool) parsing)
#endif
// Because mkman isn't aware that ENTITY_FUNCTIONS changes access to "public", do so
// explicitly here:
public:

	/**
	 * @nodoc
	 */
	TRANSFORM_FUNCTION
#if 0
; // semicolon needed for mkman (doc tool) parsing)
#endif


	// Search a private list for this object, used for debugging.

	/**
	 * @nodoc
	 */
	LOOKUP_FUNCTION
#if 0
; // semicolon needed for mkman (doc tool) parsing)
#endif
////////// end ENTITY DEFINITION STUFF //////////////////////

public:
/**
 * @nodoc
 */
	ASM_MODEL_REF* model_ref() const; // first model_ref in model_ref list - expert user only

private:
    friend void sg_asm_assembly_add_model_ref(ASM_ASSEMBLY* asmy, ASM_MODEL_REF* mref, logical const pushback);
    friend void sg_asm_assembly_remove_model_ref( ASM_ASSEMBLY* asmy, ASM_MODEL_REF* mref );
	//void add_model_ref_internal(ASM_MODEL_REF* model_ref); // adds to head of list - no model tree updates
    void add_model_ref_internal(ASM_MODEL_REF* model_ref, logical const pushback = FALSE); // adds to tail of list
	void remove_model_ref_internal(ASM_MODEL_REF* model_ref); // checks that model_ref belongs to assembly and removes it - no model tree updates

public:
/**
 * Returns the entity manager which manages this assembly.
 * <br><br>
 * <b>Role:</b> Returns a pointer to the entity manager object associated with the model in which this assembly lives.
*/
	asm_model_entity_mgr* get_mgr() const;

/**
 * Notifies the assembly of the entity manager which manages it.
 * <br><br>
 * <b>Role:</b> Resets the asm_entity_mgr pointer within the assembly object to point to the new_mgr.
 * @param new_mgr
 * new entity manager which manages the assembly.
*/
	void set_mgr(asm_model_entity_mgr* new_mgr);

// un-doc'd methods: INTERNAL USE ONLY
/**
 * @nodoc
 */
	logical is_top_level() const; 

/**
 * @nodoc
 */
	void get_model_refs(ENTITY_LIST & model_refs) const; // appends model_refs owned by this assembly (sub-assembly model_refs not included)
/**
 * @nodoc
 */
	void get_model_refs(ENTITY_LIST & model_refs, asm_model const* model) const; // appends only model_refs of model owned by this assembly (sub-assembly model_refs not included)
	                                                                  // sys_error if model is NULL
/**
 * Contructor
 */
	ASM_ASSEMBLY()
	{
		model_ref_ptr = NULL;
		mgr_ptr = NULL;
	}
};

/** @} */
#endif // ASSEMBLY_CLASS
