/*******************************************************************/
/*    Copyright (c) 1989-2020 by Spatial Corp.                     */
/*    All rights reserved.                                         */
/*    Protected by U.S. Patents 5,257,205; 5,351,196; 6,369,815;   */
/*                              5,982,378; 6,462,738; 6,941,251    */
/*    Protected by European Patents 0503642; 69220263.3            */
/*    Protected by Hong Kong Patent 1008101A                       */
/*******************************************************************/

#ifndef ASM_MODEL_INFO_HXX
#define ASM_MODEL_INFO_HXX

#include "acis.hxx"
#include "dcl_asm.h"

/**
* @file asm_model_info.hxx
 * @CAA2Level L1
 * @CAA2Usage U1
 * \addtogroup ACISASSEMBLIES
 *
 * @{
 */


/**
 * Structure to contain information about an assembly modeling model.
 * <br>
 * <b>Role:</b> The asm_model_info object is intended to consolidate information about the properties
 * of an asm_model object into a single class object. Its primary use is during ASAT save and restore.
 * During save, each model being saved is queried for its model info, which is persisted to the ASAT file.
 * During restore, this model info is passed to the entity manager factory that constructs the 
 * corresponding restored model's entity manager.
 * <br><br>
 * @see asm_model, asm_model_entity_mgr
 */
class DECL_ASM asm_model_info : public ACIS_OBJECT
{
	SPAWCHAR_T* model_name; // customer-controlled name

public:
/**
 * Number of millimeters which make up one modeling unit in this model, 
 * for example, 1000 for a part in meters.
 */
	double model_units;
/**
 * Positional tolerance for this model.
 */
	double model_resabs;

/**
 * Angular tolerance for this model.
 * Note that customers should NEVER change SPAresnor away from its initial value of 1.0e-10.
 */
	double model_resnor; // angular tolerance for this model.  THIS SHOULD NOT BE MODIFIED BY CUSTOMERS
	
/**
 * Flag indicating whether or not the model contains an assembly.
 */
	logical model_has_asm; // true if model has an assembly

// INTERNAL USE ONLY - DO NOT USE!!
/**
 * @nodoc
 */
	int model_id;

/**
 * Returns the model's name.
 * <br><br>
 * <b>Role:</b> 
 * This method returns the customer-assigned name string associated with the model.
 */
	SPAWCHAR_T const* get_model_name() const;

/**
 * Sets the model name field of the asm_model_info object.
 * <br><br>
 * <b>Role:</b> 
 * This method allows the customer to assign a value to the model-name data member of this 
 * <tt>asm_model_info</tt> object.
 * Note that the function <tt>asmi_model_get_info</tt> returns a copy of the model's information, 
 * so customers should not try to change the name of a model by calling <tt>asmi_model_get_info</tt> 
 * and then calling <tt>set_model_name</tt> on the result.
 * Instead, they should call an appropriate method on the model's concrete entity manager object.
 */
	void set_model_name(SPAWCHAR_T const* new_name);

/**
 * Default constructor.
 */
	asm_model_info();

/**
 * Copy constructor.
 * <br><br>
 * @param other
 * other model.
 */
	asm_model_info(const asm_model_info & other);

/**
 * Assignment operator.
 * <br><br>
 * @param other
 * other model.
 */
	asm_model_info & operator=(const asm_model_info & other);

/**
 * Destructor.
 */
	~asm_model_info();
};

 /** @} */
#endif // ASM_MODEL_INFO_HXX

