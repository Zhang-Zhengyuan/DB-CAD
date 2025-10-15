/*******************************************************************/
/*    Copyright (c) 1989-2020 by Spatial Corp.                     */
/*    All rights reserved.                                         */
/*    Protected by U.S. Patents 5,257,205; 5,351,196; 6,369,815;   */
/*                              5,982,378; 6,462,738; 6,941,251    */
/*    Protected by European Patents 0503642; 69220263.3            */
/*    Protected by Hong Kong Patent 1008101A                       */
/*******************************************************************/

#ifndef ASM_MODEL_HOLDER_HXX
#define ASM_MODEL_HOLDER_HXX

#include "base.hxx"
#include "dcl_asm.h"
#include "asm_model_holder_kern.hxx"

/**
* @file asm_model_holder.hxx
 * @CAA2Level L1
 * @CAA2Usage U1
 * \addtogroup ACISASSEMBLIES
 *
 * @{
 */
class asm_model;


/**
 * Holds a pointer to an <tt>asm_model</tt>.
 * <br>
 * <b>Role:</b> The @href asm_model_holder object is a container for an asm_model object which automates
 * use counting for the asm_model object. Clients of @href asm_model objects should use this holder
 * object to ensure that the @href asm_model object is not deleted when cleanup is performed. Cleanup is performed
 * by cleanup asmi routines ( @href asmi_cleanup, @href asmi_model_cleanup, @href asmi_model_cleanup_tree, 
 * @href asmi_cleanup_models, @href asmi_model_cleanup_model ), 
 * which examine a set of @href asm_model objects and destroy any object with a use count of zero, that is, which are not being held by an 
 * @href asm_model_holder object or by an @href asm_model_list object with the @href ASM_HOLD behavior set.
 * <br><br>
 * @see asm_model, asm_model_mgr
 */

class DECL_ASM asm_model_holder: public ACIS_OBJECT
{
	//asm_model* model_ptr;
	asm_model_holder_kern model_hldr_kern;

	// special routines to handle restore, don't adjust use count when dealing with sequence numbers
	friend DECL_ASM void read_asm_model_hldr( asm_model_holder& model_hldr );
	void set_sequence_num( asm_model* seq_num );
	friend DECL_ASM void fixup_asm_model_hldr( asm_model_holder& model_hldr );
	void fixup_ptr( asm_model* model );

public:
/**
 * Default constructor.
 * <br>
 * <b>Role:</b> Sets the asm_model pointer to NULL.
 * <br><br>
 */
	asm_model_holder();

/**
 * Constructor.
 * <br>
 * <b>Role:</b> Sets the asm_model pointer.  If this pointer is not NULL, the use count is incremented.
 * <br><br>
 * @param model
 * pointer to asm_model to hold.
 */
	asm_model_holder( asm_model* model );

/**
 * Destructor.
 * <br>
 * <b>Role:</b> Decrements the use count of the asm_model object.
 * <br><br>
 */
	~asm_model_holder();

/**
 * Assignment operator.
 * <br>
 * <b>Role:</b>  Copies the asm_model pointer.  If this pointer is not NULL, the use count is incremented.
 * If the holder already contains a valid asm_model pointer, the corresponding use count is decremented.
 * <br><br>
 * @param model_hldr
 * asm_model_holder.
 */
	asm_model_holder& operator=( const asm_model_holder& model_hldr );

/**
 * Sets the asm_model pointer.
 * <br>
 * <b>Role:</b>  If this pointer is not NULL, the use count is incremented.
 * If the holder already contains a valid asm_model pointer, the corresponding use count is decremented.
 * <br><br>
 * @param model
 * pointer to asm_model to hold.
 */
	void set( asm_model* model );

/**
 * Returns the asm_model pointer.
 * <br><br>
 */
	asm_model* get() const;

/**
 * Releases the asm_model pointer.
 * <br>
 * <b>Role:</b> Decrements the use count of the asm_model object and sets the asm_model pointer to NULL.
 * <br><br>
 */
	void release();
};

 /** @} */
#endif // ASM_MODEL_HOLDER_HXX

