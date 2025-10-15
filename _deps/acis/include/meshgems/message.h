#ifndef __MESSAGE_H__
#define __MESSAGE_H__


/**

\defgroup message meshgems_message_t

@{

Structure describing the structured message sent by a MeshGems
component to the caller.

During a MeshGems computation function call, MeshGems components can
communicate with the calling program through real-time messages which
can contain many kind of information like progress, information on the
ongoing process, warnings about the given data or even errors.

For this purpose, a component function can send a \ref
meshgems_message_t object to the calling program using a callback
function of type \ref meshgems_message_callback_t which has been
implemented and provided by the caller (a default one that simply
writes to `stdout` is set by default).

The \ref meshgems_message_t object is always constructed and
destructed by the emitting function itself. The user is provided with
functions to interrogate it inside the \ref
meshgems_message_callback_t function he has provided.

The \ref meshgems_message_t object contains the following data :
- a message code identifying the nature of the message (see \ref
meshgems_message_get_code)
- a text message description (see \ref meshgems_message_get_description )
- associated integer or real data (see \ref meshgems_message_get_integer_data_count
meshgems_message_get_real_data_count )

*/

struct meshgems_message_t_;

/**
 * Opaque type to store a message description.
 */
typedef struct meshgems_message_t_ meshgems_message_t;

#include <meshgems/mdecl.h>
#include <meshgems/basic_types.h>
#include <meshgems/status.h>
#include <meshgems/code.h>

/**
 * Type for a message callback function.
 * This is the type for the function which will be called by MeshGems
 * to send the user a message during a computation.
 * @param user_data (in) : a user pointer.
 * @param msg (in) : the message
 * @return a status code
 */
typedef meshgems_status_t (*meshgems_message_callback_t)(meshgems_message_t *msg, void *user_data);

#include <meshgems/context.h>

/**
 * Function to query a message for its code.
 * @param msg (in) : the message
 * @param code (out) : the message code (see \ref code for a description)
 * @return a status code
 */
MESHGEMS_METHOD meshgems_status_t meshgems_message_get_code(meshgems_message_t *msg, meshgems_integer *code);

/**
 * Function to query a message for its description.
 * @param msg (in) : the message
 * @param desc (out) : the error message (may be 0 in case of error). (*desc) can be decoded as a C string.
 * @return a status code
 */
MESHGEMS_METHOD meshgems_status_t meshgems_message_get_description(meshgems_message_t *msg, char **desc);

/**
 * Function to query a message for the size of its integer data.
 * @param msg (in) : the message
 * @param n_idata (out) : the size of the attached integer data
 * @return a status code
 */
MESHGEMS_METHOD meshgems_status_t meshgems_message_get_integer_data_count(meshgems_message_t *msg, meshgems_integer *n_idata);

/**
 * Function to query a message for the size of its real data.
 * @param msg (in) : the message
 * @param n_rdata (out) : the size of the attached real data
 * @return a status code
 */
MESHGEMS_METHOD meshgems_status_t meshgems_message_get_real_data_count(meshgems_message_t *msg, meshgems_integer *n_rdata);

/**
 * Function to query a message for a part of its integer data.
 * This function should be used in conjunction with \ref meshgems_message_get_integer_data_count
 * to get the count of integer data n_idata.
 * @param msg (in) : the message
 * @param istart (in) : the index (from 1 to n_idata) of the start of the desired data
 * @param iend (in) : the index (from 1 to n_idata) of the end of the desired data
 * @param idata (out) : the attached integer data
 * @return a status code
 */
MESHGEMS_METHOD meshgems_status_t meshgems_message_get_integer_data(meshgems_message_t *msg,
								 meshgems_integer istart, meshgems_integer iend,
								 meshgems_integer *idata);

/**
 * Function to query a message for a part of its real data.
 * This function should be used in conjunction with \ref meshgems_message_get_real_data_count
 * to get the count of real data n_rdata.
 * @param msg (in) : the message
 * @param istart (in) : the index (from 1 to n_rdata) of the start of the desired data
 * @param iend (in) : the index (from 1 to n_rdata) of the end of the desired data
 * @param rdata (out) : the attached real data
 * @return a status code
 */
MESHGEMS_METHOD meshgems_status_t meshgems_message_get_real_data(meshgems_message_t *msg,
							      meshgems_integer istart, meshgems_integer iend,
							      meshgems_real *rdata);

/**
 * Function to query a message for its sender.
 * @param msg (in) : the message
 * @param sender (out) : the sender name as a C string (0 in case of error)
 * @return a status code
 */
MESHGEMS_METHOD meshgems_status_t meshgems_message_get_sender(meshgems_message_t *msg, char **sender);

/* Backward compatibility interface */

/**
 * Backward compatibility function to query a message for its error number.
 * This function shoud not be called anymore. Please use \ref meshgems_message_get_code instead.
 * @param msg (in) : the messgae
 * @param number (out) : the code number (see \code for a description)
 * @return a status code
 */
MESHGEMS_METHOD meshgems_status_t meshgems_message_get_number(meshgems_message_t *msg, meshgems_integer *number);

/** @} */

/* Private part of the interface. Only Distene products should use it. **/

MESHGEMS_METHOD meshgems_message_t *meshgems_message_new(meshgems_context_t *ctx, meshgems_integer code, const char *desc, const char *sender);
MESHGEMS_METHOD meshgems_message_t *meshgems_message_new_copy(meshgems_message_t *imsg);
MESHGEMS_METHOD void meshgems_message_delete(meshgems_message_t *msg);
MESHGEMS_METHOD meshgems_status_t meshgems_message_set_integer_data(meshgems_message_t *msg, meshgems_integer *idata, meshgems_integer n_idata);
MESHGEMS_METHOD meshgems_status_t meshgems_message_set_real_data(meshgems_message_t *msg, meshgems_real *rdata, meshgems_integer n_rdata);
MESHGEMS_METHOD meshgems_status_t meshgems_message_set_code(meshgems_message_t *msg, meshgems_integer code);
MESHGEMS_METHOD meshgems_status_t meshgems_message_set_sender(meshgems_message_t *msg, char *sender);

#endif
