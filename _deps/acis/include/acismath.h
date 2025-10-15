/*******************************************************************/
/*    Copyright (c) 1989-2020 by Spatial Corp.                     */
/*    All rights reserved.                                         */
/*    Protected by U.S. Patents 5,257,205; 5,351,196; 6,369,815;   */
/*                              5,982,378; 6,462,738; 6,941,251    */
/*    Protected by European Patents 0503642; 69220263.3            */
/*    Protected by Hong Kong Patent 1008101A                       */
/*******************************************************************/

#ifndef __ACISMATH_H__
#define __ACISMATH_H__


#include <math.h>
#if (_MSC_VER >= 1700) && (_MSC_VER < 1924) 
#include <xtgmath.h> //atan2
#endif
#if defined(__cplusplus) && (_MSC_VER >= 1900)
#include <cmath> //Many transcendentals
#endif
#include "logical.h"
#include "dcl_base.h"


/* 
** By default, all ACIS husks will define USING_ACIS_MATH and hence,
** will use these mathematic replacements.
*/

#if defined(USING_ACIS_MATH)

#ifndef NOT_USING_ACIS_MATH

#ifdef cos
#undef cos
#endif
#define cos acis_cos

#ifdef acos
#undef acos
#endif
#define acos acis_acos

#ifdef sin
#undef sin
#endif
#define sin acis_sin

#ifdef asin
#undef asin
#endif
#define asin acis_asin

#ifdef tan
#undef tan
#endif
#define tan acis_tan

#ifdef atan
#undef atan
#endif
#define atan acis_atan

#ifdef atan2
#undef atan2
#endif
#define atan2 acis_atan2

#ifdef log
#undef log
#endif
#define log acis_log

#ifdef sqrt
#undef sqrt
#endif
#define sqrt acis_sqrt

#endif /* !NOT_USING_ACIS_MATH */

#endif /* USING_ACIS_MATH */


#ifdef __cplusplus

extern "C" {

extern DECL_BASE double (*acis_cos)(double);
extern DECL_BASE double (*acis_acos)(double);
extern DECL_BASE double (*acis_sin)(double);
extern DECL_BASE double (*acis_asin)(double);
extern DECL_BASE double (*acis_tan)(double);
extern DECL_BASE double (*acis_atan)(double);
extern DECL_BASE double (*acis_atan2)(double, double);
extern DECL_BASE double (*acis_log)(double);

}

#else

#if (defined __STDC__) || (defined MSVC)

extern DECL_BASE double (*acis_cos)(double);
extern DECL_BASE double (*acis_acos)(double);
extern DECL_BASE double (*acis_sin)(double);
extern DECL_BASE double (*acis_asin)(double);
extern DECL_BASE double (*acis_tan)(double);
extern DECL_BASE double (*acis_atan)(double);
extern DECL_BASE double (*acis_atan2)(double, double);
extern DECL_BASE double (*acis_log)(double);

#else


extern DECL_BASE double (*acis_cos)();
extern DECL_BASE double (*acis_acos)();
extern DECL_BASE double (*acis_sin)();
extern DECL_BASE double (*acis_asin)();
extern DECL_BASE double (*acis_tan)();
extern DECL_BASE double (*acis_atan)();
extern DECL_BASE double (*acis_atan2)();
extern DECL_BASE double (*acis_log)();

#endif
#endif

extern DECL_BASE void acis_sincos(double inRadians, double * outSin, double * outCos);
extern DECL_BASE logical is_little_endian(void);

#ifdef __cplusplus
extern "C" {
#endif

extern DECL_BASE double acis_sqrt( double);

#ifdef __cplusplus
}
#endif

#endif /* __ACISMATH_H__ */

