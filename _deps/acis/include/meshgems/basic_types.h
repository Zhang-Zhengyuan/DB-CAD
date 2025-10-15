#ifndef __MESHGEMS_BASIC_TYPES_H__
#define __MESHGEMS_BASIC_TYPES_H__


/**

\defgroup basic_types Basic Types
@{

Basic MeshGems types.

In this section, we give a few basic MeshGems types and macros to work
with integer and real type.  The user should not assume anything about
these types and only use the types and macros defined here. The
implementation of these types may indeed be changed at some point for
some specific versions or platforms.

*/



/**
 * \typedef meshgems_integer
 * Defines the type we use in the API to represent integer numbers.
 * The exact type can depend on the MESHGEMS_INTEGER_SIZE define.
 *
 * \def MESHGEMS_INTEGER_MAX
 * Defines the largest integer number that can be represented.
 * The exact valuer can depend on the MESHGEMS_INTEGER_SIZE define.
 *
 * \def MESHGEMS_INTEGER_FORMAT
 * Defines the format to use in printf for integer numbers.
 * The exact format can depend on the MESHGEMS_INTEGER_SIZE define.
 */
#ifdef MESHGEMS_INTEGER_SIZE

#if MESHGEMS_INTEGER_SIZE == 64

typedef long long meshgems_integer;
/* LLONG_MAX constant is not available on all compilers. Define it as the 64bit signed int max */
#define MESHGEMS_INTEGER_MAX ((meshgems_integer)9223372036854775807)
#define MESHGEMS_INTEGER_FORMAT "%lld"

#elif MESHGEMS_INTEGER_SIZE == 32

typedef int meshgems_integer;
#define MESHGEMS_INTEGER_MAX INT_MAX
#define MESHGEMS_INTEGER_FORMAT "%d"

#endif /* if MESHGEMS_INTEGER_SIZE == 64 */

#else /* ifdef MESHGEMS_INTEGER_SIZE */

/* No bit length specified. Defaulting to 32 for now */
typedef int meshgems_integer;
#define MESHGEMS_INTEGER_MAX INT_MAX
#define MESHGEMS_INTEGER_FORMAT "%d"

#endif /* ifdef MESHGEMS_INTEGER_SIZE */

/**
 * Defines the type we use in the API to represent real numbers.
 */
typedef double meshgems_real;
#define MESHGEMS_REAL_FORMAT "%le"


#include <math.h>
#include <limits.h>

/**
 * \name Usefull macros
 */
/**@{*/

/**
 * Defines the infinity value for real numbers.
 */
#define MESHGEMS_REAL_INFINITY HUGE_VAL

/**@}*/
/**@}*/

#endif
