/*******************************************************************/
/*    Copyright (c) 1989-2020 by Spatial Corp.                     */
/*    All rights reserved.                                         */
/*    Protected by U.S. Patents 5,257,205; 5,351,196; 6,369,815;   */
/*                              5,982,378; 6,462,738; 6,941,251    */
/*    Protected by European Patents 0503642; 69220263.3            */
/*    Protected by Hong Kong Patent 1008101A                       */
/*******************************************************************/
#ifndef __FREELIST_HXX__
#define __FREELIST_HXX__
#include "base.hxx"
#include "dcl_base.h"
#include "mmgr.err"
#include "mutex.hxx"
/**
 * @file freelist.hxx
 * @CAA2Level L1
 * @CAA2Usage U1
 * \addtogroup ACISMEMORYMANAGEMENT
 *
 * @{
 */
/**
 * Set the state of the memory manager to release empty freelist data blocks.
 * <br><br>
 * <b>Role:</b> The ACIS freelist mechanism uses 4k data blocks from which smaller
 * chunks are taken. Calling this function instructs the freelist mechanism to free
 * those 4k data blocks when they are empty. You only need to call this function
 * once to set the memory manager to this operational state.
 * @see collapse_all_free_lists, clear_all_free_lists, keep_all_free_lists
 */
DECL_BASE void collapse_all_free_lists();
DECL_BASE void gme_collapse_all_free_lists();

/**
 * Instruct the memory manager to release all empty freelist data blocks.
 * <br><br>
 * <b>Role:</b> The ACIS freelist mechanism uses 4k data blocks from which smaller
 * chunks are taken. Calling this function instructs the freelist mechanism to free
 * any 4k data blocks that are currently not in use. This gives the application control
 * to return unused freelist data blocks when the application thinks appropriate.
 * @see collapse_all_free_lists, clear_all_free_lists, keep_all_free_lists
 */
DECL_BASE void clear_all_free_lists();
DECL_BASE void gme_clear_all_free_lists();

/**
 * Set the state of the memory manager to keep all freelist data blocks until the application
 * terminates.
 * <br><br>
 * <b>Role:</b> The ACIS freelist mechanism uses 4k data blocks from which smaller
 * chunks are taken. Calling this function instructs the freelist mechanism to keep
 * all freelist data blocks, even those that are empty. Hence, the freelists simply 
 * will continue to grow even if all memory has been returned to the freelist. You only
 * need to call this function once to set the memory manager to this operational state.
 * @see collapse_all_free_lists, clear_all_free_lists, keep_all_free_lists
 */
DECL_BASE void keep_all_free_lists();
DECL_BASE void gme_keep_all_free_lists();

/**
 * @nodoc
 */
DECL_BASE int check_free_lists();
DECL_BASE int gme_check_free_lists();

/**
 * @nodoc
 */
enum empty_status { EMPTY_KEEP,
					EMPTY_CLEAR,
					EMPTY_FORCE,
					EMPTY_DELETE };

/**
 * @nodoc
 */
class free_list_states {
public:
	empty_status state;
};

/**
 * @nodoc
 */
DECL_BASE free_list_states get_state_of_all_free_lists();
DECL_BASE free_list_states gme_get_state_of_all_free_lists();

/**
 * @nodoc
 */
DECL_BASE void set_state_of_all_free_lists(free_list_states & fls);
DECL_BASE void gme_set_state_of_all_free_lists(free_list_states & fls);

class _mmgr_block_desc;

/**
 * @nodoc
 */
class free_list_data {

	free_list_data* next;		// chain pointer for list of all freelists
	_mmgr_block_desc** data_block;

	unsigned int size;			// length of each object of this type
	empty_status status;		// what to do when no objects in use

private:

	void clear( logical all_blocks = FALSE);		// release all blocks
        void gme_clear(logical all_blocks = FALSE);

public:

	free_list_data() {}
	~free_list_data(){};

	void initialize( int ); 	// object size
	void DECL_BASE gme_initialize( int ); 	// object size
	void terminate();
        void DECL_BASE gme_terminate();

	void *allocate();
	void DECL_BASE *gme_allocate();
	void discard( void * );	
	bool DECL_BASE gme_discard( void * );

	void set_status(empty_status);
	void DECL_BASE gme_set_status(empty_status);

	friend void initialize_free_lists();
	friend void gme_initialize_free_lists();

	friend DECL_BASE void clear_all_free_lists();
	friend DECL_BASE void gme_clear_all_free_lists();
	friend DECL_BASE void keep_all_free_lists();
	friend DECL_BASE void gme_keep_all_free_lists();
	friend DECL_BASE void collapse_all_free_lists();
	friend DECL_BASE void gme_collapse_all_free_lists();
	friend DECL_BASE int check_free_lists();
	friend DECL_BASE int gme_check_free_lists();
	friend DECL_BASE free_list_states get_state_of_all_free_lists();
	friend DECL_BASE free_list_states gme_get_state_of_all_free_lists();
	friend DECL_BASE void set_state_of_all_free_lists(free_list_states & fls);
	friend DECL_BASE void gme_set_state_of_all_free_lists(free_list_states & fls);
};

/** @} */
#endif // __FREELIST_HXX__
