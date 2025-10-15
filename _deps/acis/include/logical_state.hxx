/*******************************************************************/
/*    Copyright (c) 1989-2020 by Spatial Corp.                     */
/*    All rights reserved.                                         */
/*    Protected by U.S. Patents 5,257,205; 5,351,196; 6,369,815;   */
/*                              5,982,378; 6,462,738; 6,941,251    */
/*    Protected by European Patents 0503642; 69220263.3            */
/*    Protected by Hong Kong Patent 1008101A                       */
/*******************************************************************/

#if !defined( LOGICAL_STATE_HXX )
#define LOGICAL_STATE_HXX			

#include "dcl_base.h"
#include "logical.h"

// do not doc
enum logical_state
{
    LOGICAL_TRUE,
    LOGICAL_FALSE,
    LOGICAL_DEFAULT
};
DECL_BASE logical logical_state_to_logical(logical_state input_ls, logical default_value);
// this function returns TRUE if input is LOGICAL_TRUE,
// false if input is LOGICAL_FALSE,
// and default_vaule if input is LOGICAL_DEFAULT.
DECL_BASE logical_state logical_to_logical_state(logical input);
#endif // LOGICAL_STATE_HXX

