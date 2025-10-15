#ifndef __MESHGEMS_CONTEXT_H__
#define __MESHGEMS_CONTEXT_H__

/**

\defgroup context meshgems_context_t

@{

Root object for the MeshGems API.

The \ref meshgems_context_t object is the root object for the MeshGems
API and stores \em contextual information.  To be constructed, any
MeshGems object needs an existing \ref meshgems_context_t object to
link to (directly or indirectly).  When the \ref
meshgems_context_delete method is called to destroy an existing
context, all the objects that refer to this context will also be
destroyed. This object thus gives a simple way to cleanup the whole
data of a potentially complex set of MeshGems operations.

*/

struct meshgems_context_t_;
/**
 * Opaque data type to store the context data.
 */
typedef struct meshgems_context_t_ meshgems_context_t;

#include <stdlib.h>
#include <stdio.h>

#include <meshgems/mdecl.h>
#include <meshgems/basic_types.h>
#include <meshgems/status.h>
#include <meshgems/interrupt.h>
#include <meshgems/message.h>

/**
 * Simple constructor.
 * @return
 *    + on sucess : a pointer to a new context
 *    + on failure : 0
 */
MESHGEMS_METHOD meshgems_context_t *meshgems_context_new(void);

/**
 * Destructor.
 * Frees all data allocated in this context.
 * It will thus free any structure associated to this context,
 * like components sessions, meshes, CADs...
 */
MESHGEMS_METHOD void meshgems_context_delete(meshgems_context_t *ctx);


/**
 * Set the context message callback function.
 * @param ctx (in) : the working context.
 * @param cb (in) : the message callback (or NULL to remove any previous callback)
 * @param user_data (in) : the user pointer which will be given to cb as a parameter
 * @return a status code
 */
MESHGEMS_METHOD meshgems_status_t meshgems_context_set_interrupt_callback(meshgems_context_t *ctx,
								       meshgems_interrupt_callback_t cb,
								       void *user_data);

/**
 * Set the context interrupt callback function.
 * @param ctx (in) : the working context.
 * @param cb (in) : the interrupt callback (or NULL to remove any previous callback)
 * @param user_data (in) : the user pointer which will be given to cb as a parameter
 * @return a status code
 */
MESHGEMS_METHOD meshgems_status_t meshgems_context_set_message_callback(meshgems_context_t *ctx,
								     meshgems_message_callback_t cb,
								     void *user_data);

/** @} */


/** Private part of the interface. Only Distene products should use it. **/

typedef void (*meshgems_delete_fun)(void *);
typedef void* (*meshgems_clone_fun)(void *);

MESHGEMS_METHOD meshgems_status_t meshgems_context_attach_object(meshgems_context_t *ctx,
							      void *object,
							      meshgems_delete_fun delfun);
MESHGEMS_METHOD void meshgems_context_delete_attached_object(meshgems_context_t *ctx, void *object);
MESHGEMS_METHOD void meshgems_context_detach_object(meshgems_context_t *ctx, void *object);

MESHGEMS_METHOD meshgems_status_t meshgems_context_get_interrupt_callback(meshgems_context_t *ctx,
								       meshgems_interrupt_callback_t *cb,
								       void **user_data);
MESHGEMS_METHOD meshgems_status_t meshgems_context_get_message_callback(meshgems_context_t *ctx,
								     meshgems_message_callback_t *cb,
								     void **user_data);

MESHGEMS_METHOD meshgems_status_t meshgems_context_send_message(meshgems_context_t *ctx,
							     meshgems_integer errornum,
							     const char *msg);
MESHGEMS_METHOD meshgems_status_t meshgems_context_send_message_with_data(meshgems_context_t *ctx,
    meshgems_integer errornum, meshgems_integer *idata, meshgems_integer n_idata,
    meshgems_real *rdata, meshgems_integer n_rdata, const char *msg);

MESHGEMS_METHOD meshgems_status_t meshgems_context_set_banner(meshgems_context_t *ctx, meshgems_integer type);

MESHGEMS_METHOD meshgems_status_t meshgems_context_get_banner(meshgems_context_t *ctx, meshgems_integer *type);
MESHGEMS_METHOD meshgems_context_t *meshgems_context_new_sub_context(meshgems_context_t *ctx);


#endif
