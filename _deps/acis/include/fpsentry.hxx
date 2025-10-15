/*******************************************************************/
/*    Copyright (c) 1989-2020 by Spatial Corp.                     */
/*    All rights reserved.                                         */
/*    Protected by U.S. Patents 5,257,205; 5,351,196; 6,369,815;   */
/*                              5,982,378; 6,462,738; 6,941,251    */
/*    Protected by European Patents 0503642; 69220263.3            */
/*    Protected by Hong Kong Patent 1008101A                       */
/*******************************************************************/
#ifndef __fpsentry_hxx__
#define __fpsentry_hxx__

#include <float.h>

#if defined(OS_Darwin) || defined(_LINUX_SOURCE)
#include <fenv.h>
#endif

// Helper class for helping us call into app code
// that might cause an fp-exception. (We don't care about
// fp-exceptions in app code.) Used for interrupt callbacks
// and some HOOPs calls.
//

/**
 * @nodoc
 */
class fp_sentry
{
private:
#ifdef _MSC_VER
        unsigned int save_fp_exception_status;
#elif defined(OS_Darwin) || defined(_LINUX_SOURCE)
        fenv_t save_fenv;
#endif

public:
        fp_sentry()
        {
#ifdef _MSC_VER
                _controlfp_s( &save_fp_exception_status, 0, 0 );
                unsigned int current_word = 0;
                _controlfp_s( &current_word, _CW_DEFAULT, _MCW_EM );
#elif defined(OS_Darwin) || defined(_LINUX_SOURCE)
                fegetenv( &save_fenv);
                fesetenv( FE_DFL_ENV);
#endif
        }
        ~fp_sentry()
        {
#ifdef _MSC_VER
                // We must reset the FPU chip, to avoid passing a bad signal on.
                _fpreset();
                unsigned int current_word = 0;
                _controlfp_s( &current_word, save_fp_exception_status, _MCW_EM);
#elif defined(OS_Darwin) || defined(_LINUX_SOURCE)
                feclearexcept( FE_ALL_EXCEPT);
                fesetenv( &save_fenv);
#endif
        }
};

#endif // __fpsentry_hxx__
