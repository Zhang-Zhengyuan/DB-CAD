#ifndef __MESHGEMS_INTERRUPT_H__
#define __MESHGEMS_INTERRUPT_H__

#include <meshgems/mdecl.h>
#include <meshgems/basic_types.h>
#include <meshgems/status.h>

/**

\defgroup interrupt_callback meshgems_interrupt_callback_t

@{

Type to define the interrupt callback implemented by the user.
The \ref meshgems_interrupt_callback_t type is the base of the
MeshGems interrupt system which allows the caller to cleanly stop a
computation he launched. During a computation, a MeshGems component
calls the interrupt callback which was implemented and provided by the
caller. Depending on the result of this call (see \ref
meshgems_interrupt_callback_t), component will continue or stop the
current processing.  The function to provide this interrupt callback
to a MeshGems component session always terminate with
set_interrupt_callback suffix.

*/

/** Value to use in the implementation of an interrupt callback to
    request the process to cleanly stop */
#define MESHGEMS_INTERRUPT_STOP 1
/** Value to use in the implementation of an interrupt callback
    request the process to immediately abort */
#define MESHGEMS_INTERRUPT_ABORT 2
/** Value to use in the implementation of an interrupt callback to
    request the process to continue */
#define MESHGEMS_INTERRUPT_CONTINUE 0

/**
 * Type for an interrupt callback function. This function will be called by a module to
 * to know whether it should stop its curent computation or not.
 * @param user_data (in) : a user pointer.
 * @param interrupt_status (out) : the user decision to stop or not.
 *  See \ref MESHGEMS_INTERRUPT_STOP, \ref MESHGEMS_INTERRUPT_CONTINUE
 * and , \ref MESHGEMS_INTERRUPT_ABORT for the values that can be assigned
 * when implementing this callback.
 * @return a status code
 *
 * @see MESHGEMS_INTERRUPT_STOP
 * @see MESHGEMS_INTERRUPT_ABORT
 * @see MESHGEMS_INTERRUPT_CONTINUE
 */
typedef meshgems_status_t (*meshgems_interrupt_callback_t)(meshgems_integer *interrupt_status, void *user_data);

/**@}*/

#endif
