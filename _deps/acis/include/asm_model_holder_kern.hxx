/*******************************************************************/
/*    Copyright (c) 1989-2020 by Spatial Corp.                     */
/*    All rights reserved.                                         */
/*    Protected by U.S. Patents 5,257,205; 5,351,196; 6,369,815;   */
/*                              5,982,378; 6,462,738; 6,941,251    */
/*    Protected by European Patents 0503642; 69220263.3            */
/*    Protected by Hong Kong Patent 1008101A                       */
/*******************************************************************/

#ifndef ASM_MODEL_HOLDER_KERN_HXX
#define ASM_MODEL_HOLDER_KERN_HXX

#include "base.hxx"
#include "dcl_kern.h"

class asm_model_kern;

// This is an internal class used in the implementation of asm_model_holder and is not part of the public interface
// As such, it is not documented.
// INTERNAL USE ONLY

class DECL_KERN asm_model_holder_kern
{
	asm_model_kern* model_ptr;

	// special routines to handle restore, don't adjust use count when dealing with sequence numbers
	friend class asm_model_holder;
public:
	void set_sequence_num( asm_model_kern* seq_num );
	void fixup_ptr( asm_model_kern* model );

public:
	asm_model_holder_kern();

	asm_model_holder_kern( asm_model_kern* model );

	~asm_model_holder_kern();

	asm_model_holder_kern& operator=( const asm_model_holder_kern& model_hldr );

	void set( asm_model_kern* model );

	asm_model_kern* get() const;

	void release();
};

#endif // ASM_MODEL_HOLDER_KERN_HXX

