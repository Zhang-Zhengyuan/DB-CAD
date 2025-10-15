/*******************************************************************/
/*    Copyright (c) 1989-2020 by Spatial Corp.                     */
/*    All rights reserved.                                         */
/*    Protected by U.S. Patents 5,257,205; 5,351,196; 6,369,815;   */
/*                              5,982,378; 6,462,738; 6,941,251    */
/*    Protected by European Patents 0503642; 69220263.3            */
/*    Protected by Hong Kong Patent 1008101A                       */
/*******************************************************************/

#ifdef ENABLE_CACHE_SWITCHING

#ifndef CACHE_HXX_INCLUDED
#define CACHE_HXX_INCLUDED

#include "dcl_kern.h"
#include "logical.h"
/**
 * \defgroup ACISCACHE Cache
 * \ingroup KERNAPI
 *
 */
/**
* @file cache.hxx
 * @CAA2Level L1
 * @CAA2Usage U1
 * \addtogroup ACISCACHE
 *
 * @{
 */

/**
 * Global cache manager run mode.
 * <b>Role:</b> In the <tt>GCMGR_NO_LIMIT</tt> mode the global cache manager allows
 * the list of cache objects to grow without bound.  In the <tt>GCMGR_HARD_LIMIT</tt>
 * mode the number of cache objects is limited.
 * @param GCMGR_NO_LIMIT
 * @param GCMGR_HARD_LIMIT
 * <br><br>
 * @see set_ic_global_cache_run_mode, set_ss_global_cache_run_mode
 */
enum gcmgr_run_mode
{
	GCMGR_NO_LIMIT,
	GCMGR_HARD_LIMIT
};

/**
 * Global cache manager statistics.
 * <b>Role:</b> The gcmgr_stats struct is used to retrieve information about the
 * current state of the global cache manager.  <tt>actual_size</tt> refers to the number of
 * active cache objects, <tt>alloc_size</tt> lists the number of cache objects for which
 * memory has been allocated and <tt>max_size</tt> is the maximum number of cache objects
 * permitted in the <tt>GCMGR_HARD_LIMIT</tt> mode.  <tt>run_mode</tt> specifies the current run
 * mode of the global cache manager.
 * @see gcmgr_run_mode, get_ic_global_cache_statistics, get_ss_global_cache_statistics
 */
struct gcmgr_stats
{
	int actual_size;
	int alloc_size;
	int max_size;
	gcmgr_run_mode run_mode;
};

/**
 * Sets the maximum number of int_cur cache objects maintained by the global cache manager.
 * <br><br>
 * <b>Role:</b> Returns <tt>TRUE</tt> if the change in maximum number of int_cur cache objects
 * is successful; otherwise, it returns <tt>FALSE</tt>.  The argument must be an integer
 * greater than 2.  The maximum size is enforced only when running the global cache manager
 * in the <tt>GCMGR_HARD_LIMIT</tt> mode.
 * <br><br>
 * @param new_size
 * integer greater than 2
 */
DECL_KERN logical set_ic_global_cache_size( int new_size );

/**
 * Sets the global cache manager run mode for int_cur cache objects.
 * <br><br>
 * <b>Role:</b> Sets the run mode of the global cache manager for int_cur cache objects.
 * Available modes are <tt>GCMGR_HARD_LIMIT</tt> for which the size of the list of cache
 * objects is limited and <tt>GCMGR_NO_LIMIT</tt> for which the cache is allowed to grow
 * without bound.
 * <br><br>
 * @param run_mode
 * gcmgr_run_mode type
 */
DECL_KERN void set_ic_global_cache_run_mode( gcmgr_run_mode run_mode );

/**
 * Gets the global cache manager statistics for int_cur cache objects.
 * <br><br>
 * <b>Role:</b> Retrieves the current statistics of the global cache manager for int_cur
 * cache objects.  The data is returned using a <tt>gcmgr_stats</tt> object.  Available
 * statistics include the run mode, the maximum size, the actual number of active cache
 * objects and the total number of cache objects for which memory is allocated.
 * <br><br>
 * @param gc_stats
 * address of a gcmgr_stats object
 */
DECL_KERN void get_ic_global_cache_statistics( gcmgr_stats* gc_stats );

/**
 * Sets the maximum number of spl_sur cache objects maintained by the global cache manager.
 * <br><br>
 * <b>Role:</b> Returns <tt>TRUE</tt> if the change in maximum number of spl_sur cache objects
 * is successful; otherwise, it returns <tt>FALSE</tt>.  The argument must be an integer
 * greater than 2.  The maximum size is enforced only when running the global cache manager
 * in the <tt>GCMGR_HARD_LIMIT</tt> mode.
 * <br><br>
 * @param new_size
 * integer larger than 2
 */
DECL_KERN logical set_ss_global_cache_size( int new_size );

/**
 * Sets the global cache manager run mode for spl_sur cache objects.
 * <br><br>
 * <b>Role:</b> Sets the run mode of the global cache manager for spl_sur cache objects.
 * Available modes are <tt>GCMGR_HARD_LIMIT</tt> for which the size of the list of cache
 * objects is limited and <tt>GCMGR_NO_LIMIT</tt> for which the cache is allowed to grow
 * without bound.
 * <br><br>
 * @param run_mode
 * gcmgr_run_mode type
 */
DECL_KERN void set_ss_global_cache_run_mode( gcmgr_run_mode run_mode );

/**
 * Gets the global cache manager statistics for spl_sur cache objects.
 * <br><br>
 * <b>Role:</b> Retrieves the current statistics of the global cache manager for spl_sur
 * cache objects.  The data is returned using a <tt>gcmgr_stats</tt> object.  Available
 * statistics include the run mode, the maximum size, the actual number of active cache
 * objects and the total number of cache objects for which memory is allocated.
 * <br><br>
 * @param gc_stats
 * address of a gcmgr_stats object
 */
DECL_KERN void get_ss_global_cache_statistics( gcmgr_stats* gc_stats );


/** @} */
#endif

#endif
