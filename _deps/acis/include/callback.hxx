/* ORIGINAL: acis2.1/kernutil/callback/callback.hxx */
/* $Id: callback.hxx,v 1.5 2001/01/11 17:55:31 ywoo Exp $ */
/*******************************************************************/
/*    Copyright (c) 1989-2020 by Spatial Corp.                     */
/*    All rights reserved.                                         */
/*    Protected by U.S. Patents 5,257,205; 5,351,196; 6,369,815;   */
/*                              5,982,378; 6,462,738; 6,941,251    */
/*    Protected by European Patents 0503642; 69220263.3            */
/*    Protected by Hong Kong Patent 1008101A                       */
/*******************************************************************/

// Classes to allow decentralised closedown and restart of Acis. Primarily
// to allow modules to clear away static lists of heap objects which are
// normally retained for efficiency or other reasons. Any module which
// may retain allocated memory for reuse should declare a static stop_callback
// object with a function which returns the memory to free store.

// Rarely, a module may require to reinitialise its static structures before
// the modeller can resume operation - in most cases this will be done already
// by recognising the uninitialised state. If it does, it may declare a static
// instance of restart_callback with a function to be called to restore
// normal operation.

// Great care must be taken to avoid order-dependent effects of these
// closedown and restart functions, as there is no defined order in which
// the callback objects' "execute" functions are called.

#ifndef CALLBACK_HXX
#define CALLBACK_HXX

#include "dcl_base.h"
// ywoo 08Jan01: The following header files needed for this header file 
// to be stand alone.
#include "mmgr.hxx"
// ywoo:end

class callback;
DECL_BASE void call_callbacks( callback *head );

// Generic class, which places the object on a supplied list.

class DECL_BASE callback : public ACIS_OBJECT {
protected:
	void (*func)();
	// Record the function pointer here while the callback is
	// temporarily disabled.
	void (*disabled_func)();
	callback *next;
	callback *prev;

public:

	callback( void (*fn)(), callback *&head );
	~callback();
	void execute();
	void disable();
	void enable();

	friend DECL_BASE void call_callbacks( callback *head );
};

// Stop callback, activated whenever Acis is being shut down temporarily.
// Simply supplies a specific list to the callback constructor.

class DECL_BASE stop_callback: public callback {
public:
	stop_callback( void (*fn)() );
	~stop_callback();
};

// Restart callback, activated whenever Acis is being restarted after a
// temporary shutdown. Simply supplies a specific list to the callback
// constructor.

class DECL_BASE restart_callback: public callback {
public:
	restart_callback( void (*fn)() );
	~restart_callback();
};

// Action routines which activate all the objects on the appropriate
// callback list.

DECL_BASE void call_stop_callbacks();
DECL_BASE void call_restart_callbacks();

#endif

