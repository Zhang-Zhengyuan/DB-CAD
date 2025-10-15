/*******************************************************************/
/*    Copyright (c) 1989-2020 by Spatial Corp.                     */
/*    All rights reserved.                                         */
/*    Protected by U.S. Patents 5,257,205; 5,351,196; 6,369,815;   */
/*                              5,982,378; 6,462,738; 6,941,251    */
/*    Protected by European Patents 0503642; 69220263.3            */
/*    Protected by Hong Kong Patent 1008101A                       */
/*******************************************************************/
#ifndef PAGEMAN_HXX_INCLUDED
#define PAGEMAN_HXX_INCLUDED
#include <stdio.h>
#include <stdlib.h>
#include "dcl_base.h"
#include "logical.h"
#include "static_types.hxx"
#include "mutex.hxx"


/**
 * @file pageman.hxx
 * @CAA2Level L1
 * @CAA2Usage U1
 * \addtogroup ACISMEMORYMANAGEMENT
 *
 * @{
 */
/**
 * The page system statistics.
 * <br><br>
 * <b>Role: </b>This structure contains the statistics of the page system.
 * The information may be useful to determine the effectiveness of memory minimization.
 * @see get_page_statistics, api_minimize_entities
 */
struct page_statistics {
	/**
	 * The page_count determines the number of entries in the page file.
	 */
	uint64_t page_count;
	/**
	 * The unpage_count determines the number of entries that have been unpaged from the page file.
	 */
	uint64_t unpage_count;
	/**
	 * The trash_count determines the number of entries that have been deleted from the page file.
	 */
	uint64_t trash_count;
	/**
	 * The page_bytes determines the number of bytes paged to the page file.
	 */
	uint64_t page_bytes;
	/**
	 * The unpage_bytes determines the number of bytes that have been unpaged from the page file.
	 */
	uint64_t unpage_bytes;
	/**
	 * The trash_bytes determines the number of bytes that have been reclaimed in the page file.
	 */
	uint64_t trash_bytes;
	/**
	 * The biggest_entry determines the largest entry in the page file.
	 */
	uint64_t biggest_entry;
	/**
	 * The smallest_entry determines the smallest entry in the page file.
	 */
	uint64_t smallest_entry;
	/**
	 * The file_size determines the size of the page file in bytes.
	 */
	uint64_t file_size;
};

/**
 * @nodoc
 */
struct page_record {
	uint64_t m_pos;
	size_t m_next;
	size_t m_size;
};

/**
 * @nodoc
 */
enum page_run_mode { PS_EMPTY_DELETE, PS_EMPTY_KEEP, PS_EMPTY_TERM };

/**
 * @nodoc
 */
enum page_system_state { UNINITIALIZED = 0, ACTIVE, SUSPENDED };

/**
 * @nodoc
 */
class DECL_BASE page_system {

	page_record *m_records;
	size_t m_record_array[12];
	size_t m_num_records;
	size_t m_recycled;
	size_t m_record;
	
	FILE * m_file;
	char * m_file_name;
	char * m_buffer;
	uint64_t m_initial_pos;
	size_t m_size;
	size_t m_buffer_size;
	size_t m_requested_size;

	page_statistics m_stats;

	page_run_mode m_run_mode;
	page_system_state m_state;

	mutex_resource page_system_mtx;

private:

	size_t init_internal( page_run_mode run_mode);
	size_t term_internal();
	size_t reset();

public:

	page_system();
	~page_system();

	size_t init( char* page_file_name = NULL, page_run_mode run_mode = PS_EMPTY_DELETE );
	size_t init( FILE* page_file, page_run_mode run_mode = PS_EMPTY_DELETE );
	page_system_state term( logical force );
	
	size_t begin_write( size_t size );
	size_t write( void* data, size_t size);
	size_t end_write();

	size_t begin_read( size_t record );
	size_t read( void* data, size_t size);
	size_t end_read();

	size_t trash_record( size_t record);
	page_statistics const* get_statistics() { return &m_stats;}

	page_system_state state() const { return m_state; }
	const char* page_file_name() const;

	size_t debug();
};

/**
 * @nodoc
 */
DECL_BASE page_system *get_page_system();

// User interface
/**
 * Initialize the page system used by @href api_minimize_entities.
 * <br><br>
 * <b>Role: </b>Use this function to initialize the page system with a file name.
 * One can specify the name and location of the file to use.
 * The system will by default create a file called acis.swp in the current working directory,
 * if another name has not been specified.
 * The swap file is created by the paging system during initialization
 * and is deleted during termination.
 * One can also specify the action the paging system takes when the page file reaches an
 * empty state.
 * By default, the action is set to delete all records and reinitialize the paging system.
 * The other option is to retain all records in the page file.
 * <br><br>
 * @param page_file_name
 * the name of the page file
 * @param run_mode
 * the action to take when the page file reaches an empty state
**/
DECL_BASE size_t initialize_page_system( char* page_file_name = NULL, 
									 page_run_mode run_mode = PS_EMPTY_DELETE );
/**
 * Initialize the page system used by @href api_minimize_entities.
 * <br><br>
 * <b>Role: </b>Use this function to initialize the page system with a file handle.
 * The input file must be opened to allow both read and write operations
 * in untranslated binary mode. We suggest access modes "rb+" or "wb+".
 * The page system will begin writing at the current write location and will reset
 * to this location during termination. The file will not be closed or deleted.
 * One can also specify the action the paging system takes when the page file reaches an
 * empty state.
 * By default, the action is set to delete all records and reinitialize the paging system.
 * The other option is to retain all records in the page file.
 * <br><br>
 * @param page_file
 * file handle of the page file
 * @param run_mode
 * the action to take when the page file reaches an empty state
**/
DECL_BASE size_t initialize_page_system( FILE* page_file, 
									 page_run_mode run_mode = PS_EMPTY_DELETE );
/**
 * Terminate the page system used by @href api_minimize_entities.
 * <br><br>
 * <b>Role: </b>Use this function to terminate or suspend the page system.
 * If the page file is no longer needed, this command will close all open files and deallocate
 * all memory associated with the page system.
 * If the page file is not empty, the page system is set to a suspended state where entities
 * stored in the page file may be restored, but new entries are not saved to the file.
 * The return value indicates the state of the page system after executing this command.
 * Possible values of @href page_system_state enumeration type are UNINITIALIZED, ACTIVE, or
 * SUSPENDED.
 */
DECL_BASE page_system_state terminate_page_system();
/**
 * Get the page system statistics.
 * <br><br>
 * <b>Role: </b>Returns a pointer to the @href page_statistics object owned by the page system.
 * The object contains information that may be useful to determine the effectiveness 
 * of memory minimization.
 * @see page_statistics, api_minimize_entities
 */
DECL_BASE page_statistics const *get_page_statistics();
/**
 * Get the page system state.
 * <br><br>
 * <b>Role: </b>Returns a page_system_state enumeration type.  The possible states are 
 * UNINITIALIZED, ACTIVE, or SUSPENDED.  
 * @see page_statistics, api_minimize_entities
 */
DECL_BASE page_system_state get_page_system_state();

/**
 * Get the page file name.
 * <br><br>
 * <b>Role: </b>Returns the current page file name.  
 * @see page_statistics, api_minimize_entities
 */
DECL_BASE const char* get_page_file_name();

/** @} */
#endif
 
