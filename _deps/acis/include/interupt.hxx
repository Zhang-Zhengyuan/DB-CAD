/* ORIGINAL: 3dt2.1/main/interupt.hxx */
/*******************************************************************/
/*    Copyright (c) 1989-2020 by Spatial Corp.                     */
/*    All rights reserved.                                         */
/*    Protected by U.S. Patents 5,257,205; 5,351,196; 6,369,815;   */
/*                              5,982,378; 6,462,738; 6,941,251    */
/*    Protected by European Patents 0503642; 69220263.3            */
/*    Protected by Hong Kong Patent 1008101A                       */
/*******************************************************************/
/*
 // $Id: interupt.hxx,v 1.10 2000/12/26 18:48:34 products Exp $
 * This file included by .c files as well
 */
#ifndef INTERUPT_H
#define INTERUPT_H

#include "dcl_kern.h"
#include "logical.h"
#include <time.h>

extern DECL_KERN time_t get_timer();
extern DECL_KERN void set_timer(time_t t);

#ifdef __cplusplus
extern "C" DECL_KERN logical check_interrupt();
extern "C" DECL_KERN void clear_interrupt();
extern "C" DECL_KERN logical signal_interrupt();
extern "C" DECL_KERN logical enable_interrupt();
extern "C" DECL_KERN logical disable_interrupt();

// This procedure allows you to supply a hook procedure which will be
// called by the check_interrupt procedure to determine if the
// user wants to interrupt the current operation.  The Interrupt_Checker
// procedure returns TRUE if the user has requested an interrupt, otherwise
// it returns FALSE.
typedef logical (*Interrupt_Checker)(void);
DECL_KERN void set_interrupt_checker(Interrupt_Checker);

#else
extern DECL_KERN logical check_interrupt();
extern DECL_KERN void clear_interrupt();
extern DECL_KERN logical signal_interrupt();
extern DECL_KERN logical enable_interrupt();
extern DECL_KERN logical disable_interrupt();
#endif

#endif
