/*******************************************************************/
/*    Copyright (c) 1989-2020 by Spatial Corp.                     */
/*    All rights reserved.                                         */
/*    Protected by U.S. Patents 5,257,205; 5,351,196; 6,369,815;   */
/*                              5,982,378; 6,462,738; 6,941,251    */
/*    Protected by European Patents 0503642; 69220263.3            */
/*    Protected by Hong Kong Patent 1008101A                       */
/*******************************************************************/
#ifndef __process_mt_hxx__
#define __process_mt_hxx__

#pragma once

#include <vector>

#include "api.err"
#include "api.hxx"
#include "bulletin.hxx"
#include "ckoutcom.hxx"
#include "getowner.hxx"
#include "history.hxx"
#include "kernapi.hxx"
#include "mutex.hxx"
#include "thmgr.hxx"

// TODO: Add doc comments

/**
 * @file process_mt.hxx
 */

/**
* The struct <tt>work_packet</tt> serves as a base class for user-defined
* and application-specific work packets.
* <br>
* <b>Role:</b> The base structure provides all the functionality
* that is needed to process a list of any type of work_packets in 
* parallel using either the class <tt>work_packet_queue</tt> or the function <tt>api_process_mt</tt>.
* Users would have to derive application-specific work packets using
* this structure while overriding the process routine to do specific work.
* <br><br>
**/
struct DECL_KERN work_packet : public ACIS_OBJECT
{
	/**
	* Create a deep-down copy of the input entity 
	* such that a worker thread can work with the
	* copy without affecting its progenitor. The history stream
	* of the worker thread gets merged with the main application's
	* history stream after processing. This working copy would then
	* exist in the main history stream. If this is not intended,
	* the copy would have to be explicitly deleted once used.
	**/
	virtual ENTITY* create_working_copy(ENTITY* e);

	/**
	* @nodoc. internal use only.
	**/
	virtual void ensure_history_stream(void);

	/**
	* @nodoc. internal use only.
	**/
	virtual void cleanup_history_stream(void);

	/**
	* @nodoc. internal use only.
	**/
	HISTORY_STREAM* hs = nullptr;

	/**
	* If anything goes wrong when work_packet::process
	* is executed then the error will be captured and 
	* an ACIS error number will be provided to the 
	* caller through the data member 'status'.
	**/
	int status = 0;

	/**
	* Function to be overridden to perform specific work.
	**/
	virtual void process(void) = 0;
};


/**
* @nodoc Internal use
**/
static mutex_resource mutex_for_work_packet_queue;

/**
* The class <tt>work_packet_queue</tt> provides a high-level producer-consumer work queue
* for user-defined and application-specific work packets.
* <br>
* <b>Role:</b> The class <tt>work_packet_queue</tt> allows a somewhat greater flexibility as compared to 
* <tt>api_process_mt</tt>.
* Work packets can be added to the queue as one sees fit, i.e. it is 
* possible to add work packets while previously added work packets are 
* being processed. It is possible to add work packets of various types to the same queue.
* The application programmer is in full control of the synchronization 
* barrier that is not crossed until all the work packets in the queue have been processed.
* After a synchronization has happened new work packets can be added to
* and processed from the same queue.
* <br><br>
**/
class DECL_KERN work_packet_queue : protected thread_work_base
{
public:

	/**
	 * C++ initialize constructor requests memory for this object and populates it with the data supplied as arguments.
	 * <br><br>
	 * @param num_work_packets
	 * Number of work packets the queue is expected to process
	 * @param num_threads_to_create
	 * the number of worker threads to create
	 * @param init_method
	 * called by worker threads before entering work queue
	 * @param term_method
	 * called by worker threads after exiting work queue
	 */
	work_packet_queue(const size_t num_work_packets = 1000, const int num_threads_to_create = 0, thread_method init_method = nullptr, thread_method term_method = nullptr);

	/**
	 * Reserve capacity for the queue based on the number of packets to be processed.
	 * <br><br>
	 * @param num_work_packets
	 * Number of work packets the queue is expected to process
	 */
	void reserve(const size_t num_work_packets);

	/**
	 * Add the workpacket to the work queue for processing. 
	 * Makes work available to worker threads.
	 * <br><br>
	 * @param wp
	 * Work packet to be added to the queue for processing
	 */
	void add_and_process_work_packet(work_packet* wp);

	/**
	 * Wait for all the threads to process the total work.
	 */
	void wait_for_work_to_complete(void);

public:
	/**
	 * @nodoc The destructor must be public, because class work_packet_queue is 
	 * used by the function template api_process_mt (defined below). 
	 */
	~work_packet_queue();

private:
	/**
	 * @nodoc
	 */
	std::vector<work_packet*> wps;

	/**
	 * @nodoc
	 */
	bool is_thread_management_initialized_here = false;

	/**
	 * @nodoc
	 */
	bool is_single_threaded = false;

	/**
	 * @nodoc
	 */
	void initialize_thread_management(const int num_threads_to_create, thread_method init_method, thread_method term_method);

	/**
	 * @nodoc
	 */
	void terminate_thread_management(void);

	/**
	 * @nodoc
	 */
	void process(void* arg) noexcept final override;
};


/**
 * The API <tt>api_process_mt</tt> processes a list of user-defined work packets in parallel.
 * <br><br>
 * <b>Role:</b> This API facilitates multi-threaded processing of user-defined work packets. It works on a
 * list of user-defined work packets. The specific work packets can be created using the base <tt>work_packet</tt>
 * structure. The API manages the history streams for the individual threads and returns once all
 * the worker threads have consumed the total work. The individual work packets can then be queried
 * to consume the results or check for errors during processing.
 * <br><br>
 * <b>Effect:</b> May or may not change model depending on user-defined work packets
 * <br><br>
 * <b>Journal:</b> Not Available
 * <br><br>
 * <b>Product(s):</b> 3D ACIS Modeler
 * <br><br>
 * @param work_packet_list
 * The list of work packets to be processed
 * @param num_threads_to_create
 * The number of worker threads to use
 * @param init_method
 * Called by worker threads before entering work queue
 * @param term_method
 * Called by worker threads after exiting work queue
**/
template <typename T>
outcome api_process_mt(std::vector<T>& work_packet_list, const int num_threads_to_create = 0, thread_method init_method = nullptr, thread_method term_method = nullptr) noexcept {
	API_BEGIN;

	try
	{
		work_packet_queue wpq(work_packet_list.size(), num_threads_to_create, init_method, term_method);
		for (T& twp : work_packet_list)
			wpq.add_and_process_work_packet(&twp);
		wpq.wait_for_work_to_complete();
	}
	catch (acis_exception& ex)
	{
		throw(ex);
	}
	catch (...)
	{
		check_outcome(API_FAILED);
	}

	API_END;

	return result;
}


/**
 * The API <tt>api_process_mt</tt> processes a list of user-defined work packets in parallel.
 * <br><br>
 * <b>Role:</b> This API facilitates multi-threaded processing of user-defined work packets. It works on a
 * list of user-defined work packets. The specific work packets can be created using the base <tt>work_packet</tt>
 * structure. The API manages the history streams for the individual threads and returns once all
 * the worker threads have consumed the total work. The individual work packets can then be queried
 * to consume the results or check for errors during processing.
 * <br><br>
 * <b>Effect:</b> May or may not change model depending on user-defined work packets
 * <br><br>
 * <b>Journal:</b> Not Available
 * <br><br>
 * <b>Product(s):</b> 3D ACIS Modeler
 * <br><br>
 * @param work_packet_list
 * The list of work packets to be processed
 * @param num_work_packets
 * Number of work packets in the input list
 * @param num_threads_to_create
 * The number of worker threads to use
 * @param init_method
 * Called by worker threads before entering work queue
 * @param term_method
 * Called by worker threads after exiting work queue
**/
DECL_KERN outcome api_process_mt(work_packet** work_packet_list, const int num_work_packets, const int num_threads_to_create = 0, thread_method init_method = nullptr, thread_method term_method = nullptr) noexcept;

#endif
