/*******************************************************************/
/*    Copyright (c) 2005-2020 by Spatial Corp.                     */
/*    All rights reserved.                                         */
/*    Protected by U.S. Patents 5,257,205; 5,351,196; 6,369,815;   */
/*                              5,982,378; 6,462,738; 6,941,251    */
/*    Protected by European Patents 0503642; 69220263.3            */
/*    Protected by Hong Kong Patent 1008101A                       */
/*******************************************************************/
#ifndef SPA_PROGRESS_INFO_HXX
#define SPA_PROGRESS_INFO_HXX
#include "mmgr.hxx" // ACIS_OBJECT
#include "dcl_base.h"
/**
 * @file spa_progress_info.hxx
 * @CAA2Level L1
 * @CAA2Usage U1
 * \addtogroup ACISCALLBACKS
 *
 * @{
 */

/**
* Specifies the ACIS functionality associated with a given progress metering.
* @param SPA_progress_info_UNKNOWN
* unknown - progress metering is not in a valid state.
* @param SPA_progress_info_RESTORE
* SAT file restore.
* @param SPA_progress_info_STITCH
* tolerant stitching.
* @param SPA_progress_info_ADV_COVER
* advanced covering.
* @param SPA_progress_info_DEFEATURE
* defeaturing.
* @param SPA_progress_info_HA_RENDER
* rendering.
* @param SPA_progress_info_ENTITY_POINT_DISTANCE
* entity point distance scanline
**/

enum SPA_progress_info_type_id 
{ 
	SPA_progress_info_UNKNOWN, 
	SPA_progress_info_RESTORE, 
	SPA_progress_info_STITCH, 
	SPA_progress_info_ADV_COVER, 
	SPA_progress_info_DEFEATURE, 
	SPA_progress_info_FACET_CHECK, 
	SPA_progress_info_HA_RENDER,
	SPA_progress_info_ENTITY_ENTITY_DISTANCE,
	SPA_progress_info_MESH, 
	SPA_progress_info_ENTITY_POINT_DISTANCE,
	SPA_progress_info_FACET,
	SPA_progress_info_MT_STITCH,
	SPA_progress_info_POLY,
	SPA_progress_info_N_BODY_UNITE,
	SPA_progress_info_3DPM,
	SPA_progress_info_MODEL_COMPARISON
//if you add another value here please add to end of list and 
// update get_all_progress_info_types.
};


/**
 * Abstract base class for providing progress information.<br>
 * <b>Role:</b> This class cannot be instantiated and should not be derived from
 * by customers. A pointer to an object derived from <tt>SPA_progress_info</tt> 
 * is passed to a <tt>SPA_progress_callback</tt> (that is, a pointer to a callback
 * function with a given signature) to implement a progress meter. The 
 * <tt>SPA_progress_info</tt> class has methods to retrieve the progress data
 * and to identify on which ACIS functionality is being reported.
 */
class DECL_BASE SPA_progress_info : public ACIS_OBJECT
{
public:

	virtual ~SPA_progress_info();
/**
 * Returns a positive integer between 0 and 100 indicating the percentage of the 
 * operation that has completed. The return value 0 indicates this is the first 
 * call to the user callback from the ACIS functionality. The return value 100
 * indicates that this is the last call to the user callback from the ACIS functionality.
 */
	virtual int percentage () const = 0;

/**
 * User callbacks can query for which ACIS functionality is calling. Typically this  
 * is done on the first call to the callback to customize the dialog window, and 
 * on subsequent calls to branch to the appropriate callback code path, if the same 
 * callback is metering several different ACIS API functions.
 */
	virtual SPA_progress_info_type_id get_type() const = 0;


/**
 * Returns TRUE if the progress logic is thread-safe, FALSE otherwise.
 * When enabled, the callbacks may be called from worker threads. However, the calls
 * will never occur concurrently.
 */
	virtual logical thread_safe() const { return FALSE; }

};


/**
 * Pointer to a function implementing a generic ACIS progress meter.
 */
typedef int (*SPA_progress_callback)(SPA_progress_info*);

/**
 * Function for registering a progress callback function pointer.<br><br>
 * <b>Role: </b>The callback mechanism is enabled by registering a user callback 
 * function with the <tt>set_progress_callback</tt> function. This signature of 
 * <tt>set_progress_callback</tt> takes two arguments: the user callback function, 
 * and the type of functionality being instrumented.
 * <br><br>
 * @param callback_func_ptr
 * user callback function pointer.
 * @param type
 * type of ACIS functionality being instrumented.
 */
DECL_BASE logical set_progress_callback(SPA_progress_callback callback_func_ptr, SPA_progress_info_type_id type);


/**
 * Function for registering a progress callback function pointer.<br><br>
 * <b>Role: </b>The callback mechanism is enabled by registering a user callback 
 * function with the <tt>set_progress_callback</tt> function. This signature of 
 * <tt>set_progress_callback</tt> takes a single argument: the user callback function, 
 * and registers it for all available ACIS functionality, which includes part restore, 
 * stitching, advanced covering, defeaturing, and rendering.
 * <br><br>
 * @param callback_func_ptr
 * user callback function pointer.
 */
DECL_BASE logical set_progress_callback(SPA_progress_callback callback_func_ptr);


/**
 * Function for returning a registered progress callback function pointer.<br><br>
 * <b>Role:</b> This routine returns the currently installed progress callback function
 * for a specified <tt>SPA_progress_info_type_id</tt> callback type. 
 * <br><br>
 * @param pitid
 * the progress type id.
 */
DECL_BASE SPA_progress_callback get_progress_callback(SPA_progress_info_type_id pitid);

/** @} */
#endif // SPA_PROGRESS_INFO_HXX
