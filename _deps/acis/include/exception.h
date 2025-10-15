/* ORIGINAL: 3dt2.1/scheme/elk/except.h */
/*******************************************************************/
/*    Copyright (c) 1989-2020 by Spatial Corp.                     */
/*    All rights reserved.                                         */
/*    Protected by U.S. Patents 5,257,205; 5,351,196; 6,369,815;   */
/*                              5,982,378; 6,462,738; 6,941,251    */
/*    Protected by European Patents 0503642; 69220263.3            */
/*    Protected by Hong Kong Patent 1008101A                       */
/*******************************************************************/
/*
// $Id: exception.h,v 1.1 2001/04/03 17:54:05 jeff Exp $
//----------------------------------------------------------------------
// purpose---
//    Exception Handeling in Scheme
//    
//----------------------------------------------------------------------
*/

#include "funcproto.h"

C_LINKAGE_BEGIN
extern int Intr_Was_Ignored;
extern unsigned long Intr_Level;
C_LINKAGE_END

#ifdef POSIX_SIGNALS
    extern sigset_t Sigset_Old, Sigset_Block;
#else
#ifdef BSD_SIGNALS
    extern int Sigmask_Old, Sigmask_Block;
#else
    C_LINKAGE_BEGIN
    extern void Intr_Handler P_((int));
    C_LINKAGE_END
#endif
#endif

#ifdef BSD_SIGNALS
#  ifndef sigmask
#    define sigmask(n)  (1 << ((n)-1))
#  endif
#endif

#define Disable_Interrupts {\
    if (Intr_Level++ == 0) Force_Disable_Interrupts;\
}

#define Enable_Interrupts {\
    if (Intr_Level > 0 && --Intr_Level == 0) Force_Enable_Interrupts;\
}

#ifdef BSD_SIGNALS
#define Force_Disable_Interrupts  (void)sigblock (Sigmask_Block)
#define Force_Enable_Interrupts   (void)sigsetmask (Sigmask_Old)
#else
#ifdef POSIX_SIGNALS
#define Force_Disable_Interrupts  (void)sigprocmask (SIG_BLOCK, &Sigset_Block,\
				      (sigset_t *)0)
#define Force_Enable_Interrupts   (void)sigprocmask (SIG_SETMASK, &Sigset_Old,\
				      (sigset_t *)0)
#else
#define Force_Disable_Interrupts  {\
    if (!Intr_Was_Ignored) (void)signal (SIGINT, SIG_IGN);\
}
#define Force_Enable_Interrupts   {\
    if (!Intr_Was_Ignored) (void)signal (SIGINT, Intr_Handler);\
}
#endif
#endif
