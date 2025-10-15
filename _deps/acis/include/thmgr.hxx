/*******************************************************************/
/*    Copyright (c) 1989-2020 by Spatial Corp.                     */
/*    All rights reserved.                                         */
/*    Protected by U.S. Patents 5,257,205; 5,351,196; 6,369,815;   */
/*                              5,982,378; 6,462,738; 6,941,251    */
/*    Protected by European Patents 0503642; 69220263.3            */
/*    Protected by Hong Kong Patent 1008101A                       */
/*******************************************************************/
#ifndef __thmgr_hxx__
#define __thmgr_hxx__

/**
 * @file thmgr.hxx
 * @CAA2Level L1
 * @CAA2Usage U2
 */

#include "dcl_base.h"
#include "mmgr.hxx" // for ACIS_OBJECT

/**
 * @nodoc
 */
DECL_BASE int internal_testing_thread_count();

/**
 * Defines a function signiture to supply init/term functions for worker threads.
 */
typedef int (*thread_method)(void);

/**
 * @nodoc
 */
DECL_BASE int get_logical_tid();

/**
 * @nodoc
 */
DECL_BASE int set_max_active_acis_threads(int max_active_threads);

/**
 * Defines a simple producer-consumer work queue with thread management.
 * <br>
 * <b>Role:</b> This class provides a producer-consumer queue to be used with 
 * Thread-Safe-ACIS (TS-ACIS). The parent thread places work into
 * the work queue and waits for worker threads to complete it.
 * <br><br>
 * <b>Note:</b> Refer to the technical article <i>[Thread-Safe ACIS](https://doc.spatial.com/get_doc_page/articles/t/h/r/Thread-Safe_ACIS_bfc6.html)</i> for more information.
 */
class DECL_BASE thread_work_base : public ACIS_OBJECT {
	
	/**
	 * Worker threads wait to be given work in thread_idle_func.
	 */
	friend void thread_idle_func(void*);

public:

	/**
	 * Called once by the parent thread to initialize the thread queue.
	 * <br>
	 * <b>Role:</b> Creates the specified number of worker threads and blocks until 
	 * all have called the supplied initialization method and have entered the queue.
	 * <br><br>
	 * @param num_threads
	 * the number of worker threads to create
	 * @param init_method
	 * called by worker threads before entering work queue
	 * @param term_method
	 * called by worker threads after exiting work queue
	 */
	static int initialize( int num_threads, thread_method init_method, thread_method term_method);
	/**
	 * Called once by the parent thread to terminate the thread queue.
	 * <br>
	 * <b>Role:</b> Blocks until all threads have exited the queue and have called the supplied termination method.
	 */
	static int terminate();
	/**
	 * Returns the number of threads in the thread queue.
	 * <br>
	 * <b>Note:</b> Returns a valid answer only when called from the main thread.
	 */
	static int thread_count();
	/**
	 * Called by the parent thread to place work into the queue.
	 * <br>
	 * <b>Role:</b> The run method makes work available to worker threads. 
	 * It returns when an available worker thread accepts the work.
	 * It returns an error number if one was set during processing.
	 * The argument is passed on to the worker thread.
	 * <br><br>
	 * @param arg
	 * the argument to pass to the process method
	 */
	int run(void* arg);
	/**
	 * Called by workers threads to process work from the queue.
	 * <br>
	 * <b>Role:</b> The process method is called by worker threads as scheduled by the thread manager.
	 * This method will execute concurrently on multi-processor systems.
	 * All code that can be reached by this method must be thread safe.
	 * <br><br>
	 * @param arg
	 * the argument from the run method
	 */
	virtual void process(void* arg) = 0;
	/**
	 * Provides a synchronization barrier that is not crossed until all thread work has been completed.
	 * <br>
	 * It returns an error number if one was set during processing.
	 */
	int sync();
};

#endif // __thmgr_hxx__

