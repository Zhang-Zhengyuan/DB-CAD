/*******************************************************************/
/*    Copyright (c) 1989-2020 by Spatial Corp.                     */
/*    All rights reserved.                                         */
/*    Protected by U.S. Patents 5,257,205; 5,351,196; 6,369,815;   */
/*                              5,982,378; 6,462,738; 6,941,251    */
/*    Protected by European Patents 0503642; 69220263.3            */
/*    Protected by Hong Kong Patent 1008101A                       */
/*******************************************************************/

#ifndef _STITCH_PROGRESS_INFO
#define _STITCH_PROGRESS_INFO

/**
 * @file stitch_progress_info.hxx
 * @CAA2Level L1
 * @CAA2Usage U1
 * \addtogroup STITCHAPI
 *
 * @{
 */
#include "dcl_stitch.h"
#include "spa_progress_info.hxx"

/**
 * Abstract base class for providing stitch progress information.<br>
 * <b>Role:</b> This class can not be instantiated and should not be derived from
 * by customers. A pointer of an object derived from <tt>stitch_progress_info</tt> is passed into
 * a function of type <tt>proc_stitch_progress_callback</tt> to implement a progress meter. 
 * The <tt>stitch_progress_info</tt> object
 * has two methods available to the application that can be used to modify the default behavior of
 * the callback and to retrieve the progress data.
 */
class DECL_STITCH stitch_progress_info : public SPA_progress_info {
public:
/**
 * Returns a positive integer indicating the percentage of stitch,
 * which has completed.
 */
	virtual int percentage () const = 0;

/**
 * Returns a positive integer indicating the current
 * callback frequency, which is set to 1 by default. The method accepts an
 * integer indicating the customer desired callback frequency on the very
 * first call to the callback. A frequency of 1 causes the callback to get
 * called roughly every percent. A frequency of 2 causes the callback to get
 * called every time the progress is updated, which could be quite often.
 * Setting the frequency to 0 (zero) will disable the callback for the current
 * stitch session. (Alternatively, one can simply return a -1 from the custom
 * callback to disable the callback for the current stitch). If the <tt>interval</tt>
 * method is called with any value other than 0, 1 and 2, then the method does
 * not do anything but simply return the current value of frequency.
 * <br><br>
 * @param input_interval
 * desired callback frequency.
 */
	virtual int interval( int input_interval = -1) = 0;

/**
 * User callbacks can query for which ACIS functionality is calling. Typically this  
 * is done on the first call to the callback, to customize the dialog window, and 
 * on subsequent calls to branch to the approriate callback code path, if the same 
 * callback is metering several different ACIS API functions.
 */
	virtual SPA_progress_info_type_id get_type() const;

};

/**
 * Function pointer to implement stitch progress meter.
 */
typedef int (*proc_stitch_progress_callback)(stitch_progress_info*);

/**
 * Function for setting the stitch progress callback pointer.
 * <br>
 * <b>Role: </b>The callback mechanism is enabled by installing a custom callback function with
 * the <tt>set_stitch_progress_callback</tt> function. The <tt>set_stitch_progress_callback </tt>
 * function accepts one argument, which is the custom callback function.
 * <br><br>
 * @param callback_func_ptr
 * custom callback function pointer.
 */
DECL_STITCH void set_stitch_progress_callback(proc_stitch_progress_callback callback_func_ptr);

/**
 * Function for getting the stitch progress callback pointer.
 * <br>
 * <b>Role: </b>The stitch progress callback function pointer can be obtained by
 * <tt>get_stitch_progress_callback</tt>.
 */
DECL_STITCH proc_stitch_progress_callback get_stitch_progress_callback();


// GSSL/RA - end header changes for stitch_progress
/** @} */
#endif

