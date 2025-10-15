/*******************************************************************/
/*    Copyright (c) 1989-2020 by Spatial Corp.                     */
/*    All rights reserved.                                         */
/*    Protected by U.S. Patents 5,257,205; 5,351,196; 6,369,815;   */
/*                              5,982,378; 6,462,738; 6,941,251    */
/*    Protected by European Patents 0503642; 69220263.3            */
/*    Protected by Hong Kong Patent 1008101A                       */
/*******************************************************************/
//    Declare scheme_process( const char * )
/**
 * @file sprocess.h
 * @CAA2Level L1
 * @CAA2Usage U1
 * \addtogroup SCMAPI
 * \Declared at <sprocess.h>
 * @{
 */

#ifndef sprocess_h
#define sprocess_h

//======================================================================

#ifdef __cplusplus
extern "C" {
#endif

/**
 * Builds and evaluates a Scheme command, optionally echoing the prompt and result.
 * <br><br>
 * <b>Role:</b> <tt>scheme_process</tt> is called repeatedly with input lines that
 * partially form a Scheme command. After each invocation, the function returns the
 * current nesting level of parenthesis. When it has compiled a complete Scheme
 * command with matching parentheses and quotes (the nesting level returns as 0),
 * it evaluates the command by calling <tt>do_scheme</tt>. All Scheme procedures in the
 * input string are evaluated before returning.
 * <br><br>
 * <b>Effect:</b> System routine
 * <br><br>
 * @param inpLine
 * command string.
 * @param echo
 * echo.
 **/
int scheme_process( const char *inpLine, int echo);

/*
// tbrv
*/
/**
 * @nodoc
 */
const char *LastResult();

#ifdef __cplusplus
typedef void (*close_evt_cb)();
//extern close_evt_cb close_event_callback;

}
#endif
/** @} */
#endif
