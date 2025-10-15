/* ORIGINAL: 3dt2.1/scheme/scmproc.hxx */
// $Id: scmproc.hxx,v 1.6 2002/03/29 20:08:00 mgoertz Exp $
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
//    Define class user_scm_proc for registering new scheme procedures
//
//----------------------------------------------------------------------

#ifndef scmproc_hxx
#define scmproc_hxx

#ifndef _EXPORT_MACRO
#define _EXPORT_MACRO
#include "mmgr.hxx"
#include "scmobject.h"
#endif

// Procedure to call all of the initialization procedures
_EXPORT_MACRO void Register_User_Procs();

//======================================================================
class _EXPORT_MACRO user_scm_proc : public ACIS_OBJECT
{
protected:
    int   min_num_arg;          // minimum number of arguments
    int   max_num_arg;          // maximum number of arguments
    user_scm_proc* next;
    const char* proc_name;      // name as registered in scheme
    const char* proc_help;      // help as registered in scheme
    ScmObject (*proc_ptr)(...);

public:
    user_scm_proc(int, int, const char*, ScmObject(*)(...), const char* help = NULL);
    virtual ~user_scm_proc();

    virtual void register_proc();

    // mgoertz: allow access
    virtual user_scm_proc* get_next_proc()
    {return next;}
    virtual const char* get_proc_name()
    {return proc_name;}
    virtual const char* get_proc_help()
    {return proc_help;}

	friend _EXPORT_MACRO void Register_User_Procs();

};


// Procedure to return head of all user procedures
_EXPORT_MACRO user_scm_proc* Get_User_Procs();

//======================================================================
#endif
