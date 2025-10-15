/*******************************************************************/
/*    Copyright (c) 1989-2020 by Spatial Corp.                     */
/*    All rights reserved.                                         */
/*    Protected by U.S. Patents 5,257,205; 5,351,196; 6,369,815;   */
/*                              5,982,378; 6,462,738; 6,941,251    */
/*    Protected by European Patents 0503642; 69220263.3            */
/*    Protected by Hong Kong Patent 1008101A                       */
/*******************************************************************/

#ifndef ASM_MODEL_KERN_HXX
#define ASM_MODEL_KERN_HXX

#include "base.hxx"
#include "dcl_kern.h"

/**
* @file asm_model_kern.hxx
 * @CAA2Level L1
 * @CAA2Usage U1
 *! \addtogroup KERNAPI
 *
 * @{
 */

class entity_handle;
class asm_model;

/**
 * Use-counted base class for <tt>asm_model</tt>.
 * <b>Role:</b> This is an internal class that is used to allow use counting of asm_model
 * objects by ENTITY objects.
 */
class DECL_KERN asm_model_kern : public ACIS_OBJECT
{
protected:
	int use_cnt;	// number of uses of this model (includes active and lost model_refs as well as other instantiations of asm_model_holder)

	friend class asm_model_holder;	// these are the only classes allowed to modify the use count
	friend class asm_model_holder_kern;
	friend class asm_model_list_eng;

	void add_count();
	virtual void rem_count()=0;

	asm_model_kern();

	// need access to unhold functions so they can notify owning model of destruction
	friend class ASM_ASSEMBLY;
	friend class ASM_MODEL_REF;
	virtual void unhold_assembly_handle(entity_handle* asm_handle)=0;
	virtual void unhold_mref_handle(entity_handle* mref_handle)=0;

	// notify (and de-notify) this model of sub-model
	virtual void   link_sub_model(asm_model* sub_model)=0;
	virtual void unlink_sub_model(asm_model* sub_model)=0;

public:
/**
 * @nodoc
 */
	int use_count();
};

/*! @} */
#endif // ASM_MODEL_KERN_HXX

