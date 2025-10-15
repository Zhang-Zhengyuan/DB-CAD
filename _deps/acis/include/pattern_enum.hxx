/*******************************************************************/
/*    Copyright (c) 1989-2020 by Spatial Corp.                     */
/*    All rights reserved.                                         */
/*    Protected by U.S. Patents 5,257,205; 5,351,196; 6,369,815;   */
/*                              5,982,378; 6,462,738; 6,941,251    */
/*    Protected by European Patents 0503642; 69220263.3            */
/*    Protected by Hong Kong Patent 1008101A                       */
/*******************************************************************/

#ifndef PATTERN_ENUM_HXX
#define PATTERN_ENUM_HXX

#include "logical.h"

/**
* @file pattern_enum.hxx
 * @CAA2Level L1
 * @CAA2Usage U1
 * \addtogroup ACISPATTERNS
 *
 * @{
 */

// For entities with next() methods, include a switch to specify how it
// should work when the entity has a pattern holder
/**
 * For entities with <tt>next()</tt> methods, include this switch to specify
 * how the method should work when the entity has a pattern holder.
 * @param PAT_CAN_CREATE
 * Create a new "next" from the pattern, if appropriate.
 * @param PAT_NO_CREATE
 * Return the current <tt>next_ptr</tt>.
 * @param PAT_IGNORE
 * Skip over patterned entities other than seed entities.
 */
enum PAT_NEXT_TYPE
{
    PAT_CAN_CREATE, // Create a new "next" from the pattern, if appropriate
    PAT_NO_CREATE,  // Return next_ptr
    PAT_IGNORE      // Ignore the pattern except for the seed entities
};


// This enum is for use in api_pattern_apply_to_entity and in
// api_pattern_update_entities, to specify checking behavior
// to follow the application of a pattern [in vers. 7 and 8,
// a logical was used instead, so in adding "check-and-fix",
// we do so in a way that is backward-compatible with the
// former type and the associated behavior]
/**
 * Used in <tt>api_pattern_apply_to_entity</tt>, <tt>api_pattern_apply_to_faces</tt>,
 * and <tt>api_pattern_update_entities</tt> to specify the type of checking that accompanies
 * the application of a pattern.
 * @param PAT_CHECK_AND_FIX
 * Apply the pattern and check the results; if the results don't check, change the pattern in the way needed to fix the results,then reapply it.
 * @param PAT_DONT_CHECK
 * Apply the pattern without checking if the results are valid.
 * @param PAT_CHECK_DONT_FIX
 * Apply the pattern and check for valid results, roll back if checking fails.
 * @param PAT_CHECK_FOR_BOOLEAN
 * (For internal use only)
 */
enum PAT_CHECK_TYPE
{
    PAT_CHECK_AND_FIX = -1,     // Apply the pattern and check
                                // the results; if the results
                                // don't check, change the pattern
                                // in the way needed to fix the
                                // results, then reapply it
    PAT_DONT_CHECK = FALSE,     // Apply the pattern without
                                // checking if the results are
                                // valid
    PAT_CHECK_DONT_FIX = TRUE,  // Apply the pattern and check
                                // for valid results, but simply
                                // roll back if checking fails
    PAT_CHECK_FOR_BOOLEAN       // For internal use only
};


/** @} */
#endif
