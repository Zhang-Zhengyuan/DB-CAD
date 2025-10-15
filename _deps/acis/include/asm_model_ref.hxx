/*******************************************************************/
/*    Copyright (c) 1989-2020 by Spatial Corp.                     */
/*    All rights reserved.                                         */
/*    Protected by U.S. Patents 5,257,205; 5,351,196; 6,369,815;   */
/*                              5,982,378; 6,462,738; 6,941,251    */
/*    Protected by European Patents 0503642; 69220263.3            */
/*    Protected by Hong Kong Patent 1008101A                       */
/*******************************************************************/

#ifndef ASM_MODEL_REF_HXX
#define ASM_MODEL_REF_HXX

#include "dcl_kern.h"
#include "entity.hxx"
#include "transfrm.hxx"
#include "asm_model_holder_kern.hxx"
#include "en_macro.hxx"
class ASM_ASSEMBLY;
class ASM_MODEL_REF;
class asm_model;
class entity_handle;

/**
* @file asm_model_ref.hxx
 * @CAA2Level L1
 * @CAA2Usage U1
 * \addtogroup KERNAPI
 * @{
 */


/**
 * @nodoc
 */
ENTITY_IS_PROTOTYPE( ASM_MODEL_REF, KERN )
#if 0
; // semicolon needed for mkman (doc tool) parsing
#endif

// Identifier used to find out (via identity() defined below) to what
// an entity pointer refers.

extern DECL_KERN int ASM_MODEL_REF_TYPE;


// Identifier that gives number of levels of derivation of this class
// from ENTITY

#define ASM_MODEL_REF_LEVEL 1
////////// end ENTITY DEFINITION STUFF //////////////////////
/**
 * Assembly model reference
 */
class DECL_KERN ASM_MODEL_REF : public ENTITY
{
	ASM_ASSEMBLY* asm_ptr; // the assembly owning this model_ref
	ASM_MODEL_REF* prev_ptr; // previous model_ref owned by assembly
	ASM_MODEL_REF* next_ptr; // next     model_ref owned by assembly

	asm_model_holder_kern model_hldr;	// note that the model is NOT an entity
									// ownership is shared between all the model_refs pointing to the model
									// use a model holder to simplify use counting

	TRANSFORM* transf_ptr;  // transform of this model_ref relative to coordinate system of owning model

	// internal set methods for list pointers
	void set_next(ASM_MODEL_REF* new_next);
	void set_prev(ASM_MODEL_REF* new_prev);

	// list management functions should only be called by ASM_ASSEMBLY
	friend class ASM_ASSEMBLY;
	void insert_before(ASM_MODEL_REF* model_ref);
        void insert_after(ASM_MODEL_REF* model_ref);// adds to tail of list
	void remove_from_list();
	void set_assembly(ASM_ASSEMBLY* assembly);
 
	// CAUTION - lose() does NOT correct list pointers, so lose() should never be called directly
	// by customer code
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
	ENTITY_FUNCTIONS( ASM_MODEL_REF , KERN)

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
	/**
	 * @nodoc
	 */
	ROLL_NOTIFY_FUNCTION
#if 0
; // semicolon needed for mkman (doc tool) parsing)
#endif
////////// end ENTITY DEFINITION STUFF //////////////////////
public:
/**
 * Returns a pointer to the owning ASM_ASSEMBLY object.
 */
	ENTITY *owner() const;

// un-doc'd methods: INTERNAL USE ONLY
/**
 * @nodoc
 */
	ASM_ASSEMBLY* assembly() const; // returns pointer to this model_ref's assembly - expert use only

/**
 * @nodoc
 */
	ASM_MODEL_REF* previous() const; // returns pointer to previous model_ref owned by assembly
/**
 * @nodoc
 */
	ASM_MODEL_REF* next()     const; // returns pointer to next model_ref owned by assembly
/**
 * @nodoc
 */
	asm_model const* model() const; // returns pointer to this model_ref's model
/**
 * @nodoc
 */
	asm_model      * model();       // use with care - modifying the part bound to the model
	                                        // will modify ALL model_refs of the model
	                                        // technically, this is a const method, but since the model
	                                        // is part of the state of the model_ref we'll declare it non-const
/**
 * @nodoc
 */
	asm_model* get_owning_model() const;

/**
 * @nodoc
 */
	void set_transform( SPAtransf const & transf );
/**
 * @nodoc
 */
	TRANSFORM const* transform() const; // returns pointer to this model_ref's transform
/**
 * @nodoc
 */
	TRANSFORM      * transform();       // use with care - modifying transform will modify model_ref
	                                         // technically, this is a const method, but since the transform
	                                         // is part of the state of the model_ref we'll declare it non-const

	// constructor - require a model and a transform to be supplied
/**
 * @nodoc
 */
	ASM_MODEL_REF(asm_model_kern* model, // the model
	             SPAtransf const & transform); // 
/**
 * @nodoc
 */
	ASM_MODEL_REF() {} // default constructor; needed for ENTITY functions
};

/*! @} */
#endif // ASM_MODEL_REF_HXX

