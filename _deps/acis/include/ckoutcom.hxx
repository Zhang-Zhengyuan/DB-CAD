/*******************************************************************/
/*    Copyright (c) 1989-2020 by Spatial Corp.                     */
/*    All rights reserved.                                         */
/*    Protected by U.S. Patents 5,257,205; 5,351,196; 6,369,815;   */
/*                              5,982,378; 6,462,738; 6,941,251    */
/*    Protected by European Patents 0503642; 69220263.3            */
/*    Protected by Hong Kong Patent 1008101A                       */
/*******************************************************************/
#ifndef _CKOUTCOM_HXX_
#define _CKOUTCOM_HXX_
#include "dcl_kern.h"
#include "api.hxx"
/**
 * @file ckoutcom.hxx
 * @CAA2Level L1
 * @CAA2Usage U1
 * \addtogroup ACISERRORMANAGEMENT
 *
 * @{
 */
 /**
 * Check an outcome from an API and signal an exception if it is not OK.
 * <br><br>
 * <b>Role:</b> Checks the outcome and calls <tt>sys_error</tt> if it is not OK. This
 * routine should be called after all routines that return an outcome and after
 * each <tt>API_END</tt>. If the outcome is not checked, exception conditions will not
 * propagate up the stack and get reported to the user.
 * <br><br>
 * <b>Effect:</b> System routine
 * <br><br>
 * @param result
 * the outcome to check
**/
DECL_KERN void check_outcome( const outcome &result);
DECL_KERN void gme_check_outcome( const outcome &result);
/** @} */
#endif
