/*******************************************************************/
/*    Copyright (c) 1989-2020 by Spatial Corp.                     */
/*    All rights reserved.                                         */
/*    Protected by U.S. Patents 5,257,205; 5,351,196; 6,369,815;   */
/*                              5,982,378; 6,462,738; 6,941,251    */
/*    Protected by European Patents 0503642; 69220263.3            */
/*    Protected by Hong Kong Patent 1008101A                       */
/*******************************************************************/
//----------------------------------------------------------------------
// purpose---
//    Declare part api routines
//
//----------------------------------------------------------------------

#ifndef part_api_hxx
#define part_api_hxx

#include "dcl_part.h"
#include "api.hxx"
#include "entityid.hxx"
#include "actpart.hxx"
/**
 * \defgroup PARTAPI Part Management
 *
 */
/**
 * @file part_api.hxx
 * @CAA2Level L1
 * @CAA2Usage U1
 * \addtogroup PARTAPI
 *
 * @{
 */


class outcome;
class PART;
class ENTITY;
class ENTITY_LIST;
class entity_filter;
class HISTORY_STREAM;

//======================================================================
// API routines related to PARTS

/**
* Initializes the part manager library.
* <br><br>
* <b>Effect:</b> System routine
* <br><br>
* <b>Journal: </b> Not Available
* <br><br>
* <b>Product(s):</b> 3D ACIS Modeler
 * <br><br>
**/
DECL_PART outcome api_initialize_part_manager();


/**
* Terminates the part manager library.
* <br><br>
* <b>Effect:</b> System routine
* <br><br>
* <b>Journal: </b> Not Available
* <br><br>
* <b>Product(s):</b> 3D ACIS Modeler
 * <br><br>
**/
DECL_PART outcome api_terminate_part_manager();



// Create a new PART

/**
* Creates a new PART.
* <br><br>
* <b>Role:</b> This API creates a new part.
* It initially allocates enough space to contain the specified size (the number of entities).
* All <tt>api_part</tt> functions should be thought of as requiring the use of the <tt>PART</tt> class.
* <br><br>
* <b>Effect:</b> Changes model
* <br><br>
* <b>Journal: </b> Not Available
* <br><br>
* <b>Product(s):</b> 3D ACIS Modeler
 * <br><br>
* @param size
* initial size of entity table for part.
* @param part
* returns part.
**/
DECL_PART outcome api_part_create(
    unsigned int size,
    PART *& part
    );

// Delete a PART
/**
* Deletes a <tt>PART</tt>.
* <br><br>
* <b>Role:</b> This API deletes the specified part.
* All <tt>api_part</tt> functions should be thought of as requiring the use of the <tt>PART</tt> class.
* <br><br>
* <b>Effect:</b> Changes model
* <br><br>
* <b>Journal: </b> Not Available
* <br><br>
* <b>Product(s):</b> 3D ACIS Modeler
 * <br><br>
* @param part
* part.
**/
DECL_PART outcome api_part_delete(
    PART* part
    );

// STI ROLL begin: Get and set the distribution mode

/**
* Sets history distribution on or off.
* <br><br>
* <b>Role:</b> This API sets the distribution mode for history streams.
* If set to <tt>TRUE</tt>, bulletins are distributed to the appropriate part's stream during note state.
* <br><br>
* The distribution mode can only be changed once.
* Further, once bulletins for a part history are created, the distribution mode cannot be changed.
* By default, distribution is <tt>FALSE</tt>.
* <br><br>
* If your application did not explicitly set the option distributed_history to  <tt>FALSE</tt>
* (i.e., you either left it at its default value, which was <tt>TRUE</tt>, or
* explicitly set it to <tt>TRUE</tt>), you must now call <tt>api_part_set_distribution_mode</tt>
* to  get the same behavior in your application.
* <br><br>
* <b>Effect:</b> System routine
* <br><br>
* <b>Journal: </b> Not Available
* <br><br>
* <b>Product(s):</b> 3D ACIS Modeler
 * <br><br>
* @param distribute
* TRUE if history is distributed to part stream after or during note state.
**/

DECL_PART outcome api_part_set_distribution_mode(logical distribute);

/**
* Gets history distribution mode which is either <tt>TRUE</tt> or <tt>FALSE</tt>.
* <br><br>
* <b>Role:</b> API gets the distribution mode for history streams.
* If set to <tt>TRUE</tt>, bulletins are distributed to the appropriate part's stream during note state.
* <br><br>
* <b>Effect:</b> System routine
* <br><br>
* <b>Journal: </b> Not Available
* <br><br>
* <b>Product(s):</b> 3D ACIS Modeler
 * <br><br>
* @param distribute
* TRUE if history is distributed to part stream after or during note state
**/
DECL_PART outcome api_part_get_distribution_mode(logical &distribute);
// STI ROLL end

// Add an ENTITY to a PART
/**
* Adds an <tt>ENTITY</tt> to a <tt>PART</tt>.
* <br><br>
* <b>Role:</b> This API adds a specified entity to a specified part.
* If the entity is already in a different <tt>PART</tt>, it is first removed from the old part.
* All <tt>api_part</tt> functions should be thought of as requiring the use of the <tt>PART</tt> class.
* <br><br>
* Limitation: it is not possible to move an entity into a part if part distribution is enabled and
* the entity being moved is on a different history stream than the history stream associated with 
* the part. An attempt to do this will result in an error.
* <br><br>
* <b>Effect:</b> Changes model
* <br><br>
* <b>Journal: </b> Not Available
* <br><br>
* <b>Product(s):</b> 3D ACIS Modeler
 * <br><br>
* @param entity
* entity to be added.
* @param part
* part to which to add entity.
**/
DECL_PART outcome api_part_add_entity(
    ENTITY *entity,
    PART *part
    );

// Remove an ENTITY from its PART

/**
* Removes an <tt>ENTITY</tt> from a part.
* <br><br>
* <b>Role:</b> This API removes an <tt>ENTITY</tt> from a part.
* All <tt>api_part</tt> functions should be thought of as requiring the use of the <tt>PART</tt> class.
* <br><br>
* <b>Effect:</b> Changes model
* <br><br>
* <b>Journal: </b> Not Available
* <br><br>
* <b>Product(s):</b> 3D ACIS Modeler
 * <br><br>
* @param entity
* entity to be removed.
**/
DECL_PART outcome api_part_remove_entity(
    ENTITY* entity
    );

// Find an ENTITY given an ID and a PART

/**
* Gets an entity given an <tt>ID</tt> and a <tt>PART</tt>.
* <br><br>
* <b>Role:</b> This API looks up an entity in a part given an entity id.
* If id does not exist in the part, this API returns <tt>NULL</tt>.
* All <tt>api_part</tt> functions should be thought of as requiring the use of the <tt>PART</tt> class.
* <br><br>
* <b>Effect:</b> Read-only
* <br><br>
* <b>Journal: </b> Not Available
* <br><br>
* <b>Product(s):</b> 3D ACIS Modeler
 * <br><br>
* @param id
* entity ID.
* @param part
* part in which to look for entity.
* @param entity
* found entity.
**/
DECL_PART outcome api_part_lookup_entity(
    entity_id_t id,
    PART * part,
    ENTITY*& entity
    );

// Get the ID and PART for an ENTITY
/**
* Gets the entity <tt>ID</tt> and part for an <tt>ENTITY</tt>.
* <br><br>
* <b>Role:</b> This API returns the entity <tt>ID</tt> (id) and the part containing the specified entity.
* If the entity is not in the part, this API returns the entity <tt>ID</tt> (id) as 0
* and the part as <tt>NULL</tt>. All <tt>api_part</tt> functions should be thought of
* as requiring the use of the <tt>PART</tt> class.
* <br><br>
* <b>Effect:</b> Read-only
* <br><br>
* <b>Journal: </b> Not Available
* <br><br>
* <b>Product(s):</b> 3D ACIS Modeler
 * <br><br>
* @param entity
* entity to identify.
* @param id
* entity to identify.
* @param part
* part containing the entity.
**/
DECL_PART outcome api_part_entity_id(
    ENTITY * entity,
    entity_id_t & id,
    PART *& part
    );

// Get all ENTITIES from a PART

/**
* Gets a list of entities in a <tt>PART</tt>.
* <br><br>
* <b>Role:</b> This API returns the list of entities fount in a part that match the specified filter.
* If filter is <tt>NULL</tt>, this API returns all entities in the part.
* All <tt>api_part</tt> functions should be thought of as requiring the use of the <tt>PART</tt> class.
* <br><br>
* <b>Effect:</b> Read-only
* <br><br>
* <b>Journal: </b> Not Available
* <br><br>
* <b>Product(s):</b> 3D ACIS Modeler
 * <br><br>
* @param part
* part from which to get entities.
* @param filter
* filter used to select entities or NULL.
* @param list
* returns list of entities found.
**/
DECL_PART outcome api_part_entities(
    PART *part,
    entity_filter *filter,
    ENTITY_LIST &list
    );

// Save a PART

/**
* Saves a <tt>PART</tt> to a file.
* <br><br>
* <b>Role:</b> This API saves the entities contained in a <tt>PART</tt> to an open file (<tt>fp</tt>).
* The file must be open and positioned to the location to which the entities are to be written.
* <br><br>
* If the optional with_history is specified as <tt>TRUE</tt>, roll back history
* data will be saved as well. If the optional <tt>mainline_only</tt> flag is specified
* as <tt>TRUE</tt>, only  un-rolled states will be saved to the file.
*  All <tt>api_part</tt> functions should be thought of as requiring the use of the <tt>PART</tt> class.
* <br><br>
* <b>Effect:</b> Changes model
* <br><br>
* <b>Journal: </b> Not Available
* <br><br>
* <b>Product(s):</b> 3D ACIS Modeler
 * <br><br>
* @param fp
* file in which to save entities.
* @param text_mode
* TRUE (text) or FALSE (binary).
* @param the_part
* PART containing entities to save.
* @param with_history
* TRUE to save history stream to the file.
* @param mainline_only
* TRUE to ignore rolled states.
**/
DECL_PART outcome api_part_save(
    FILE* fp,
    logical text_mode,
    PART* the_part,
    logical with_history = 0,
    logical mainline_only = 0  // STI ROLL
    );

// Load a file into a PART
/**
* Loads a file into a <tt>PART</tt>.
* <br><br>
* <b>Role:</b> This API loads the entities defined in an open file <tt>fp</tt> into the specified part.
*  The file must be open and positioned to the start of the entity data to be read.
*  All <tt>api_part</tt> functions should be thought of as requiring the  use of the <tt>PART</tt> class.
* <br><br>
* <b>Effect:</b> Changes model
* <br><br>
* <b>Journal: </b> Not Available
* <br><br>
* <b>Product(s):</b> 3D ACIS Modeler
 * <br><br>
* @param fp
* file containing entities to load.
* @param text_mode
* TRUE (text) or FALSE (binary).
* @param the_part
* part in which to load entities.
* @param with_history
* TRUE to restore history if it exists in the file.
* @param new_entities
* returns list of entities loaded into part.
**/
DECL_PART outcome api_part_load(
    FILE* fp,
    logical text_mode,
    PART* the_part,
	logical with_history,
    ENTITY_LIST& new_entities
    );

//----------------------------------------------------------------------
// API routines related to rollback

// Mark the start of an operation
// api_part_start_state and api_part_note_state must occur in pairs.  They can
// be nested to create bigger logical operations.

/**
* Marks the start of a state.
* <br><br>
* <b>Role:</b> This API marks the start an operation.
* Match calls to <tt>api_part_start_state</tt> with later calls to <tt>api_part_note_state</tt>.
* Pairs may be nested to create larger operations.
* A new delta state is started for the outermost call only.
* All <tt>api_part</tt> functions should be thought of as requiring the use of the PART class.
* <br><br>
* The calls to <tt>api_part_start_state</tt> and <tt>api_part_note_state</tt> must be strictly paired regardless of errors.
* Start state and note state are paired by the use of a static level counter.
* If the note state were skipped when there was an error,the counter would be off by one and subsequent states would not be noted.
* <pre>
* int depth;
* api_part_start_state(depth);
* API_BEGIN
* <br>
* result = api_do_stuff_1(args);
* check_outcome(result);  // If result is not ok,
*                        // jump to API_END
*
*                       // Alternate style of using check_outcome
* check_outcome(api_do_stuff_2(args));
*
*                      // Tell the part manager and graphics what happened
* record_entity(new top level entity);
* update_entity(modified top level entity);
*
* API_END
* api_part_note_state(outcome(API_SUCCESS), depth);</pre>
* If an error occurs, it will be caught by <tt>API_END</tt>.
* The <tt>api_part_note_state</tt> is always called regardless of error.
* Note that the outcome is checked before recording or updating entities, so the part manager and graphics don't see anything bad.
* <br><br>
* It is also acceptable to use <tt>API_SYS_BEGIN/END</tt> or <tt>EXCEPTION_BEGIN/TRY/CATCH/END</tt>
* with <tt>api_part_start_state</tt> in the <tt>EXCEPTION_BEGIN</tt> block and <tt>api_part_note_state</tt>
* in an <tt>EXCEPTION_CATCH( TRUE )</tt> block.
* <br><br>
* <b>Effect:</b> Read-only
* <br><br>
* <b>Journal: </b> Not Available
* <br><br>
* <b>Product(s):</b> 3D ACIS Modeler
 * <br><br>
* @param depth
* depth of nesting of operations after call.
**/
DECL_PART outcome api_part_start_state(
	int& depth				   // operation nesting depth after call
	);

// Mark the end of an operation
/**
* Marks the end of a state.
* <br><br>
* <b>Role:</b> This API marks the end of an operation.
* Match calls to <tt>api_part_note_state</tt> with earlier calls to <tt>api_part_start_state</tt>.
* Pairs can be nested to create larger operations.
* A new delta state is created for the outermost call only.
* All <tt>api_part</tt> functions should be thought of as requiring the use of the <tt>PART</tt> class.
* <br><br>
* The calls to <tt>api_part_start_state</tt> and <tt>api_part_note_state</tt> must be strictly paired regardless of errors.
* Start state and note state are paired by the use of a static level counter.
* If the note state were skipped when there was an error,the counter would be off by one and subsequent states would not be noted.
* <br><br>
* <pre>
* int depth;
* api_part_start_state(depth);
* API_BEGIN
*
* result = api_do_stuff_1(args);
* check_outcome(result);  // If result is not ok,
*       // jump to API_END
*
* // Alternate style of using check_outcome
* check_outcome(api_do_stuff_2(args));
*
* // Tell the part manager and graphics what happened
* record_entity(new top level entity);
* update_entity(modified top level entity);
*
* API_END
* api_part_note_state(outcome(API_SUCCESS), depth);</pre>
* If an error occurs, it will be caught by <tt>API_END</tt>.
* The <tt>api_part_note_state</tt> is always called regardless of error.
* Note that the outcome is checked before recording or updating entities, so the
* part manager and graphics don't see  anything bad.
* <br><br>
* It is also acceptable to use <tt>API_SYS_BEGIN/END</tt> or <tt>EXCEPTION_BEGIN/TRY/CATCH/END</tt>
* with <tt>api_part_start_state</tt> in the <tt>EXCEPTION_BEGIN</tt> block and <tt>api_part_note_state</tt>
* in an <tt>EXCEPTION_CATCH( TRUE )</tt> block.
* <br><br>
* <b>Effect:</b> Read-only
* <br><br>
* <b>Journal: </b> Not Available
* <br><br>
* <b>Product(s):</b> 3D ACIS Modeler
 * <br><br>
* @param status
* outcome of operation.
* @param depth
* depth of operation nesting after call.
**/
DECL_PART outcome api_part_note_state(
    outcome status,                   // outcome for the operation
	int& depth				   // operation nesting depth after call
    );

// Name the current state
/**
* Names the current state.
* <br><br>
* <b>Role:</b> This API assigns a name to the recent operation.
* Call <tt>api_part_name_state</tt> immediately after <tt>api_part_note_state</tt>
* and before opening the next state with to <tt>api_part_start_state</tt>.
* <tt>api_part_name_state</tt> names the most  recent noted state.
* <tt>api_part_name_state</tt> can also be called immediately following starting
* the modeler if it were desired that the "root" state be named.
* Use the specified name in calls to <tt>api_part_roll_to_state</tt> to roll to
* the start of the current operation. All <tt>api_part</tt> functions should be
* thought of as requiring the  use of the <tt>PART</tt> class.
* <br><br>
* <b>Effect:</b> System routine
* <br><br>
* <b>Journal: </b> Not Available
* <br><br>
* <b>Product(s):</b> 3D ACIS Modeler
 * <br><br>
* @param name
* name to give to current operation.
* @param hs
* the history stream to use, if not the default.
**/

DECL_PART outcome api_part_name_state(
    const char* name,          // name to give to the current state
	HISTORY_STREAM* hs =  NULL // What stream to use if not default
    );

// Roll to a named state

/**
* Rolls to the start of a named state.
* <br><br>
* <b>Role:</b> This API rolls to the start of a named operation (name).
* If multiple operations have the same name, the latest one before the current state is used.
* If no operations with the given name occur before the current state,the first one after the current state is used.
* All <tt>api_part</tt> functions should be thought of as requiring the use of the <tt>PART</tt> class.
* <br><br>
* <b>Effect:</b> Changes model
* <br><br>
* <b>Journal: </b> Not Available
* <br><br>
* <b>Product(s):</b> 3D ACIS Modeler
 * <br><br>
* @param name
* name of state to which to roll.
* @param hs
* history stream.
* @param n_actual
* number of states actually rolled.
**/
DECL_PART outcome api_part_roll_to_state(
    const char* name,          // Name of state you want to roll to
	HISTORY_STREAM* hs,        // What stream to use if not default
	int& n_actual              // number of delta states actually rolled.
    );

// Roll forward or backward N states
/**
* Rolls forward or backward a specified number of states.
* <br><br>
* <b>Role:</b> This API rolls a specified number (<tt>n_wanted</tt>) of states.
*  A negative number rolls to an earlier state; a positive number rolls to a later state.
*  All <tt>api_part</tt> functions should be thought of as requiring the use of the <tt>PART</tt> class.
* <br><br>
* <b>Effect:</b> Changes model
* <br><br>
* <b>Journal: </b> Not Available
* <br><br>
* <b>Product(s):</b> 3D ACIS Modeler
 * <br><br>
* @param n_wanted
* number of states to roll.
* @param hs
* history stream to roll.
* @param n_actual
* returns actual number of states rolled.
**/
DECL_PART outcome api_part_roll_n_states(
    int n_wanted,              // The number of states you want to roll
	HISTORY_STREAM* hs,           // What stream to use if not default
    int& n_actual              // The number of states actually rolled
    );

// Delete all states
/**
* Deletes all states.
* <br><br>
* <b>Role:</b> This API deletes all operations defined using <tt>api_part_start_state</tt>
* and <tt>api_part_note_state</tt> for the given history stream.
* Use this API when clearing a part in preparation for loading or creating a new part.
* All <tt>api_part</tt> functions should be thought of as requiring the use of the <tt>PART</tt> class.
* <br><br>
* <b>Effect:</b> Changes model
* <br><br>
* <b>Journal: </b> Not Available
* <br><br>
* <b>Product(s):</b> 3D ACIS Modeler
 * <br><br>
* @param hs
* history stream to delete.
**/
DECL_PART outcome api_part_delete_all_states(
	HISTORY_STREAM* hs =  NULL  // What stream to use if not default
	);

//----------------------------------------------------------------------
// API routines related to specifying the active part.


/**
* Gets the active part.
* <br><br>
* <b>Effect:</b> Read-only
* <br><br>
* <b>Journal: </b> Not Available
* <br><br>
*/

DECL_PART PART *api_get_active_part();


/**
* Sets the part active.
* <br><br>
* <b>Effect:</b> System routine
* <br><br>
* <b>Journal: </b> Not Available
* <br><br>
* @param this_part
* selected part.
*/
DECL_PART void api_set_active_part( PART* this_part);


/** @} */
#endif
