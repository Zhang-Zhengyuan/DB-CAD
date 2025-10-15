#ifndef __STATUS_H__
#define __STATUS_H__

#include <meshgems/mdecl.h>
#include <meshgems/basic_types.h>

/**

\defgroup status meshgems_status_t

@{

The standard MeshGems status.

This type is mostly used as a return value for non-void MeshGems API
functions.

*/

/**
 * Status type.  Defines the type for the status values that can be
 *  returned by non-void MeshGems functions.  These return values
 *  should be manipulated with provided macros MESHGEMS_STATUS_*
 */
typedef meshgems_integer meshgems_status_t;

/**
 * \name Most common error status
 */
/**@{*/

/** The status value for succes. */
#define MESHGEMS_STATUS_OK (0)

/** The status value for an unspecified error. */
#define MESHGEMS_STATUS_ERROR (-1)

/** The status value for an unspecified warning. */
#define MESHGEMS_STATUS_WARNING (1)

/** The status value for an error caused by unsufficient memory. */
#define MESHGEMS_STATUS_NOMEM (-5)

/** The status value for an error caused by an unknown parameter. */
#define MESHGEMS_STATUS_UNKNOWN_PARAMETER (-100)

/** The status value for an error caused by an user interrupt request.
    The interrupt request most likely came from the user defined
    interrupt callback.*/
#define MESHGEMS_STATUS_USER_INTERRUPT (-200)

/**@}*/

/**
 * \name Usefull macros
 */
/**@{*/

/** A macro to test whether a status is an error. Return 1 if true and
    0 if false */
#define MESHGEMS_STATUS_IS_ERROR(x_) ((x_) < 0)

/** A macro to test whether a status is a warning. Return 1 if true and
    0 if false */
#define MESHGEMS_STATUS_IS_WARNING(x_) ((x_) > 0)

/**@}*/

/**
 * \name Various license management error status
 */
/**@{*/
#define MESHGEMS_STATUS_NOLICENSE (-6000)
#define MESHGEMS_STATUS_DLIM_NOKEY (-6001)
#define MESHGEMS_STATUS_OEM_KEYERROR (-6002)
#define MESHGEMS_STATUS_OEM_NOSIG (-6003)
#define MESHGEMS_STATUS_OEM_IDERROR (-6004)
#define MESHGEMS_STATUS_OEM_SIGMISMATCH (-6005)
#define MESHGEMS_STATUS_OEM_SIGINTERNAL0 (-6006)
#define MESHGEMS_STATUS_OEM_SIGINTERNAL1 (-6007)
#define MESHGEMS_STATUS_OEM_SIGINTERNAL2 (-6008)
#define MESHGEMS_STATUS_OEM_SIGINTERNAL3 (-6009)
#define MESHGEMS_STATUS_OEM_SIGINTERNAL4 (-6010)
#define MESHGEMS_STATUS_OEM_SIGINTERNAL5 (-6011)
#define MESHGEMS_STATUS_OEM_SIGINTERNAL6 (-6012)
/**@}*/

#endif
