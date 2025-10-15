/* $Id: mutex.hxx,v 1.11 2001/09/19 20:37:59 ktatters Exp $ */
/*******************************************************************/
/*    Copyright (c) 1989-2020 by Spatial Corp.                     */
/*    All rights reserved.                                         */
/*    Protected by U.S. Patents 5,257,205; 5,351,196; 6,369,815;   */
/*                              5,982,378; 6,462,738; 6,941,251    */
/*    Protected by European Patents 0503642; 69220263.3            */
/*    Protected by Hong Kong Patent 1008101A                       */
/*******************************************************************/

#ifndef MUTEX_HXX_INCLUDED
#define MUTEX_HXX_INCLUDED

/**
 * @file mutex.hxx
 * @CAA2Level L1
 * @CAA2Usage U1
 */

#include "base.hxx"

#ifdef __cplusplus

#ifdef THREAD_SAFE_ACIS

#ifdef _MSC_VER
# ifndef WIN32_LEAN_AND_MEAN
#  define WIN32_LEAN_AND_MEAN
# endif
#include <windows.h>

#else

#include <pthread.h>

#endif

#endif

#if (defined(OS_Darwin) || defined(OS_Linux)) && defined(__cplusplus) 
#define SpaACIS 1
#if defined(_MK_CONTNAME_) && (1 == _MK_CONTNAME_)
#undef NULL
#define NULL spa_mac_null::nl
#endif
#undef SpaACIS
#endif

#include "dcl_base.h"
#include "logical.h"

#ifdef THREAD_SAFE_ACIS

/**
 * Wrapper macro used to simplify the proper ownership of mutex resources.
 * @param RESOURCE
 * the mutex_resource to manage.
 * <br><br>
 * <b>Note:</b> Refer to the technical article <i>Thread-Safe ACIS</i> for more information.
 * <br><br>
 * @see mutex_resource, mutex_object
 */
#define CRITICAL_BLOCK(RESOURCE) mutex_object this_mutex(RESOURCE);

# ifdef SPA_DEBUG
/**
 * @nodoc
 */
#  define DEBUG_CRITICAL_BLOCK(RESOURCE) mutex_object this_mutex(RESOURCE);

# else
/**
 * @nodoc
 */
#  define DEBUG_CRITICAL_BLOCK(RESOURCE)

# endif

#else
/**
 * @nodoc
 */
# define CRITICAL_BLOCK(RESOURCE) 
/**
 * @nodoc
 */
# define DEBUG_CRITICAL_BLOCK(RESOURCE)

#endif

#ifdef ENABLE_TSA_STATS
/**
 * @nodoc
 */
DECL_BASE int get_mutex_hits(void);

#endif /* ENABLE_TSA_STATS */


/**
 * Defines a class containing OS specific mutual exclusion primitives.
 * <br>
 * <b>Role:</b> Use this class to avoid concurrent accesses to shared resources.
 * In other words, as a synchronization resource for mutual exclusion logic.
 * The resource can only be owned by one thread at a time. 
 * Each thread must acquire the resource to assume ownership and must release it
 * before other threads can acquire it. 
 * The resource can be acuquired multiple times by the same thread.
 * Threads wating to acquire the resource yield all procesor utilization until the resource becomes available.
 * <br><br>
 * <b>Note:</b> Refer to the technical article <i>Thread-Safe ACIS</i> for more information.
 * @see mutex_object, CRITICAL_BLOCK
 */
class DECL_BASE mutex_resource {

	enum mutex_state { raw = 0, alive };

	// The current state of the mutex.
	mutex_state state;

	unsigned _count;

#if defined(THREAD_SAFE_ACIS)
# ifdef _MSC_VER

	CRITICAL_SECTION Mutex;

# else

	pthread_mutex_t Mutex;

# endif
#endif

public:
/**
 * Constructs an <tt>mutex_resource</tt> object.
 * <br><br>
 * @param call_create
 * call create method during construction.
 */
	mutex_resource( logical call_create = TRUE);
/**
 * Destructs an <tt>mutex_resource</tt> object.
 */
	~mutex_resource();
/**
 * Initializes the <tt>mutex_resource</tt> object.
 */
	logical create();
/**
 * Terminates the <tt>mutex_resource</tt> object.
 */
	logical destroy();
/**
 * Acquires the <tt>mutex_resource</tt> object.
 */
	void acquire();
/**
 * Releases the <tt>mutex_resource</tt> object.
 */
	void release();
/**
 * Returns TRUE if the <tt>mutex_resource</tt> is acquired.
 */
	logical acquired();
};

/**
 * Defines a helper class to assure proper ownership of <tt>mutex_resource</tt> objects.
 * <br>
 * <b>Role:</b> Use this class to simplify the proper acquisition and release 
 * of <tt>mutex_resource</tt> objects via C++ construction/destruction semantics.
 * <br><br>
 * <b>Note:</b> Refer to the technical article <i>Thread-Safe ACIS</i> for more information.
 * @see mutex_resource, CRITICAL_BLOCK
 */
class DECL_BASE mutex_object {

	mutex_object();
	int operator=(mutex_object) {return 0;}
	
	mutex_resource& the_resource;

public:
/**
 * Constructs an <tt>mutex_object</tt> for a specific mutex_resource and acquires it.
 * <br><br>
 * @param in_resource
 * the mutex_resource to acquire.
 */
	mutex_object( mutex_resource& in_resource);
/**
 * Destructs the <tt>mutex_object</tt> an releases its encapsulated mutex_resource.
 * <br><br>
 */
	~mutex_object();
/**
 * Returns TRUE if the encapsulated <tt>mutex_resource</tt> is acquired.
 */
	logical acquired() { return the_resource.acquired(); };
};

#endif /* CPLUSPLUS */

#endif /* MUTEX_HXX_INCLUDED */


