/* ORIGINAL: 3dt2.1/scheme/scm_init.hxx */
// $Id: scm_init.hxx,v 1.5 2000/12/26 19:05:56 products Exp $
/*******************************************************************/
/*    Copyright (c) 1989-2020 by Spatial Corp.                     */
/*    All rights reserved.                                         */
/*    Protected by U.S. Patents 5,257,205; 5,351,196; 6,369,815;   */
/*                              5,982,378; 6,462,738; 6,941,251    */
/*    Protected by European Patents 0503642; 69220263.3            */
/*    Protected by Hong Kong Patent 1008101A                       */
/*******************************************************************/
//----------------------------------------------------------------------
// purpose---
//    Define class user_scm_init for registering scheme initialization 
//    procedures
//
//----------------------------------------------------------------------

#ifndef scm_init_hxx
#define scm_init_hxx

#include "mmgr.hxx"

#ifndef _EXPORT_MACRO
#define _EXPORT_MACRO
#endif

// Procedure to call all of the initialization procedures
_EXPORT_MACRO void Call_Init_Procs();

//======================================================================
class _EXPORT_MACRO user_scm_init : public ACIS_OBJECT
{
  private:
    user_scm_init* next;

  protected:
    void (*proc_ptr)();

  public:
    user_scm_init(void(*)());
    virtual ~user_scm_init();

    virtual void do_initialization();

	friend _EXPORT_MACRO void Call_Init_Procs();

};


//======================================================================
#endif
