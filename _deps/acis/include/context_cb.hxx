// $Id: context_cb.hxx,v 1.5 2002/01/08 17:58:11 ktatters Exp $
/*******************************************************************/
/*    Copyright (c) 1989-2020 by Spatial Corp.                     */
/*    All rights reserved.                                         */
/*    Protected by U.S. Patents 5,257,205; 5,351,196; 6,369,815;   */
/*                              5,982,378; 6,462,738; 6,941,251    */
/*    Protected by European Patents 0503642; 69220263.3            */
/*    Protected by Hong Kong Patent 1008101A                       */
/*******************************************************************/
//----------------------------------------------------------------------
// Base class for component context objects and file context objects
//
// Links everthing into callback lists so there is one call to clear
// or clear_no_dtors everthing at once
//----------------------------------------------------------------------

#ifndef CONTEXT_CB_HXX
#define CONTEXT_CB_HXX

#include "dcl_base.h"
#include "tlkit_cb.hxx"

class context_callbacks_list;

// All the context objects are derived from context_callbacks
// which enables us to call several standard methods on all
// contexts at once.
class DECL_BASE context_callbacks : public toolkit_callback {
public:

	context_callbacks();
	~context_callbacks();

	virtual void clear() = 0;
	virtual void clear_no_dtors() = 0;
	virtual void print_access() = 0;
};

class DECL_BASE context_callbacks_list : public toolkit_callback_list_not_owner
{
public:
	void add(context_callbacks* cb) { toolkit_callback_list::add(cb); }
	void append(context_callbacks* cb) { toolkit_callback_list::append(cb); }

	void clear();
	void clear_no_dtors();
	void print_access();
};

extern DECL_BASE context_callbacks_list& get_context_callbacks_list();


// #define the body of the inc_access functions.  All implemnetations
// are inline.  When _DEBUG is not #defined a sufficiently proficient 
// optimizer should eliminate all associated object code entirely.
#ifdef SPA_DEBUG
#define INCREMENT_ACCESS_COUNT count_accesses++;
#else 
#define INCREMENT_ACCESS_COUNT count_accesses=count_accesses; 
#endif

#endif
