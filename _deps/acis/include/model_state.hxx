/*******************************************************************/
/*    Copyright (c) 1989-2020 by Spatial Corp.                     */
/*    All rights reserved.                                         */
/*    Protected by U.S. Patents 5,257,205; 5,351,196; 6,369,815;   */
/*                              5,982,378; 6,462,738; 6,941,251    */
/*    Protected by European Patents 0503642; 69220263.3            */
/*    Protected by Hong Kong Patent 1008101A                       */
/*******************************************************************/
#ifndef __model_state_hxx__
#define __model_state_hxx__

/**
 * @file model_state.hxx
 * @CAA2Level L1
 * @CAA2Usage U2
 */

#include "safe.hxx"
#include "dcl_base.h"
#include "logical.h"
#include "option.hxx"

/**
 * @nodoc
 */
class opt_name_val;


/**
 * Defines a class to capture the current tolerance values and option settings.
 * <br>
 * <b>Role:</b> Use this class to capture the current state of the tolerance values
 * and option_header settings.
 * <br><br>
 * <b>Note:</b> Refer to the technical article <i>Thread-Safe ACIS</i> for more information.
 */
class DECL_BASE modeler_state: public ACIS_OBJECT
{
	double resabs, resfit, resnor, resmch;
	opt_name_val * opt_vals;

	void clear_opt_vals();

public:

/**
 * Constructor captures the sate.
 */
	modeler_state();
/**
 * Destructor releases all associated memory.
 */
	~modeler_state();
/**
 * Activate the currenly captured state.
 * <b>Role:</b> Use this method to set the tolerance values
 * and option_header settings to that of the captured state.
 */
	void activate();
};

#endif

