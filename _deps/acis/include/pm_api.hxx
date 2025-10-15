/*******************************************************************/
/*    Copyright (c) 1989-2020 by Spatial Corp.                     */
/*    All rights reserved.                                         */
/*    Protected by U.S. Patents 5,257,205; 5,351,196; 6,369,815;   */
/*                              5,982,378; 6,462,738; 6,941,251    */
/*    Protected by European Patents 0503642; 69220263.3            */
/*    Protected by Hong Kong Patent 1008101A                       */
/*******************************************************************/
//    Declare pmhusk api routines
#ifndef pmhusk_api_routines_hxx
#define pmhusk_api_routines_hxx
#include "api.hxx"
#include "entityid.hxx"
#include "part_api.hxx"
/**
 * @file pm_api.hxx
 * @CAA2Level L1
 * @CAA2Usage U1
 * \addtogroup SCMAPI
 * \Declared at <pm_api.hxx>
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
 * Creates a new <tt>PART</tt>.
 * <br><br>
 * <b>Role:</b> This api  initially allocates enough space
 * to contain the specified size (the number of entities). All <tt>api_pm</tt> functions
 * should be thought of as requiring the use of the <tt>PART_CONTEXT</tt> class.
 * <br><br>
 * <b>Effect:</b> Changes model
 * <br><br>
 * @param size
 * initial size of entity table for part.
 * @param part1
 * returns part.
 **/
outcome api_pm_create_part(unsigned int size, PART *&part1);
/**
 * Deletes a <tt>PART</tt>.
 * <br><br>
 * <b>Role:</b> All <tt>api_pm</tt> functions should be thought of as requiring the use of the <tt>PART_CONTEXT</tt> class.
 * <br><br>
 * <b>Effect:</b> Changes model
 * <br><br>
 * @param part1
 * part.
 **/
outcome api_pm_delete_part( PART *part1 );
/**
 * Adds an <tt>ENTITY</tt> to a <tt>PART</tt>.
 * <br><br>
 * <b>Role:</b> This API adds a specified entity to a specified part. If the
 * entity is already in a different <tt>PART</tt>, it is first removed from the old part.
 * All <tt>api_pm</tt> functions should be thought of as requiring the use of the <tt>PART_CONTEXT</tt> class.
 * <br><br>
 * <b>Effect:</b> Changes model
 * <br><br>
 * @param entity1
 * entity to be added.
 * @param part1
 * part to which to add entity.
 **/
outcome api_pm_add_entity(   ENTITY* entity1,    PART* part1 );
/**
 * Removes an <tt>ENTITY</tt> from a part.
 * <br><br>
 * <b>Role:</b>  All <tt>api_pm</tt> functions should be thought of as requiring the use of the <tt>PART_CONTEXT</tt> class.
 * <br><br>
 * <b>Effect:</b> Changes model
 * <br><br>
 * @param entity1
 * entity to be removed.
 **/
outcome api_pm_remove_entity(    ENTITY *entity1    );
/**
 * Gets an entity given an ID and a <tt>PART</tt>.
 * <br><br>
 * <b>Role:</b> This API looks up an entity in a part given an entity <tt>id</tt>. If <tt>id </tt>
 * does not exist in the part, this API returns <tt>NULL</tt>. All <tt>api_pm</tt> functions should
 * be thought of as requiring the use of the <tt>PART_CONTEXT</tt> class.
 * <br><br>
 * <b>Effect:</b> Read-only
 * <br><br>
 * @param id
 * entity ID.
 * @param part
 * part in which to look for entity.
 * @param ent
 * found entity.
 **/
outcome api_pm_lookup_entity(    entity_id_t id,    PART* part,    ENTITY*& ent    );
/**
 * Gets the entity ID and part for an <tt>ENTITY</tt>.
 * <br><br>
 * <b>Role:</b> This API returns the entity ID (<tt>id</tt>) and the part containing the
 * specified entity. If the entity is not in the part, this API returns the entity
 * ID (<tt>id</tt>) as 0 and the <tt>part</tt> as <tt>NULL</tt>. All <tt>api_pm</tt> functions should be thought of as
 * requiring the use of the <tt>PART_CONTEXT</tt> class.
 * <br><br>
 * <b>Effect:</b> Read-only
 * <br><br>
 * @param ent
 * entity to identify.
 * @param id
 * entity to identify.
 * @param part
 * part containing the entity.
 **/
outcome api_pm_entity_id(    ENTITY* ent,    entity_id_t& id,    PART*& part
    );
/**
 * Gets a list of entities in a <tt>PART</tt>.
 * <br><br>
 * <b>Role:</b> This API returns the list of entities fount in a part that match
 * the specified <tt>filter</tt>. If <tt>filter</tt> is <tt>NULL</tt>, this API returns all entities in the
 * part. All <tt>api_pm</tt> functions should be thought of as requiring the use of the
 * <tt>PART_CONTEXT</tt> class.
 * <br><br>
 * <b>Effect:</b> Read-only
 * <br><br>
 * @param part1
 * part from which to get entities.
 * @param filter
 * filter used to select entities or NULL.
 * @param ent
 * returns list of entities found.
 **/

outcome api_pm_part_entities(    PART* part1,    entity_filter* filter,    ENTITY_LIST& ent    );
/**
 * Saves a <tt>PART</tt> to a file.
 * <br><br>
 * <b>Role:</b> This API saves the entities contained in a <tt>PART</tt> to an open file
 * (<tt>fp</tt>). The file must be open and positioned to the location to which the entities
 * are to be written.
 * <br><br>
 * If the optional <tt>with_history</tt> is specified as <tt>TRUE</tt>, roll back history data will
 * be saved as well. If the optional <tt>mainline_only</tt> flag is specified as <tt>TRUE</tt>, only
 * un-rolled states will be saved to the file. All <tt>api_pm</tt> functions should be
 * thought of as requiring the use of the <tt>PART_CONTEXT</tt> class.
 * <br><br>
 * <b>Effect:</b> Changes model
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
outcome api_pm_save_part(
                        FILE* fp,
                        logical text_mode,
                        PART* the_part,
                        logical with_history = 0,
                        logical mainline_only = 0  // STI ROLL
                        );
/**
 * Loads a file into a <tt>PART</tt>.
 * <br><br>
 * <b>Role:</b> This API loads the entities defined in an open file <tt>fp</tt> into the
 * specified part. The file must be open and positioned to the start of the entity
 * data to be read. All <tt>api_pm</tt> functions should be thought of as requiring the use
 * of the <tt>PART_CONTEXT</tt> class.
 * <br><br>
 * <b>Effect:</b> Changes model
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
outcome api_pm_load_part(
                        FILE* fp,
                        logical text_mode,
                        PART* the_part,
	                    logical with_history,
                        ENTITY_LIST& new_entities
                        );
/**
 * Marks the start of a state.
 * <br><br>
 * <b>Role:</b> Match calls to <tt>api_pm_start_state</tt> with later calls to <tt>api_pm_note_state</tt>.
 * Pairs may be nested to create larger operations. A new delta state is started for the
 * outermost call only. All <tt>api_pm</tt> functions should be thought of as requiring the use
 * of the <tt>PART_CONTEXT</tt> class.
 * <br><br>
 * The calls to <tt>api_pm_start_state</tt> and <tt>api_pm_note_state</tt> must be strictly paired
 * regardless of errors. Start state and note state are paired by the use of a
 * static level counter. If the note state were skipped when there was an error,
 * the counter would be off by one and subsequent states would not be noted.
 * <pre>
 * int depth;
 * api_pm_start_state(depth);
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
 * api_pm_note_state(outcome(API_SUCCESS), depth);</pre>
 * If an error occurs, it will be caught by <tt>API_END</tt>. The <tt>api_pm_note_state</tt> is
 * always called regardless of error. Note that the outcome is checked before
 * recording or updating entities, so the part manager and graphics don't see
 * anything bad.
 * <br><br>
 * It is also acceptable to use <tt>API_SYS_BEGIN/END</tt> or <tt>EXCEPTION_BEGIN/TRY/CATCH/END</tt>
 * with <tt>api_pm_start_state</tt> in the <tt>EXCEPTION_BEGIN</tt> block and <tt>api_pm_note_state</tt> in an
 * <tt>EXCEPTION_CATCH( TRUE )</tt> block.
 * <br><br>
 * <b>Effect:</b> System routine
 * <br><br>
 * @param depth
 * depth of nesting of operations after call.
 **/

outcome api_pm_start_state(	int& depth);
/**
 * Marks the end of a state.
 * <br><br>
 * <b>Role:</b> Match calls to <tt>api_pm_note_state</tt> with earlier calls to <tt>api_pm_start_state</tt>.
 * Pairs can be nested to create larger operations. A new delta state is created for the
 * outermost call only. All <tt>api_pm</tt> functions should be thought of as requiring the use of
 * the <tt>PART_CONTEXT</tt> class.
 * <br><br>
 * The calls to <tt>api_pm_start_state</tt> and <tt>api_pm_note_state</tt> must be strictly paired
 * regardless of errors. Start state and note state are paired by the use of a
 * static level counter. If the note state were skipped when there was an error,
 * the counter would be off by one and subsequent states would not be noted.
 * <pre>
 * int depth;
 * api_pm_start_state(depth);
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
 * api_pm_note_state(outcome(API_SUCCESS), depth);</pre>
 * If an error occurs, it will be caught by <tt>API_END</tt>. The <tt>api_pm_note_state</tt> is
 * always called regardless of error. Note that the outcome is checked before
 * recording or updating entities, so the part manager and graphics don't see
 * anything bad.
 * <br><br>
 * It is also acceptable to use <tt>API_SYS_BEGIN/END</tt> or <tt>EXCEPTION_BEGIN/TRY/CATCH/END</tt>
 * with <tt>api_pm_start_state</tt> in the <tt>EXCEPTION_BEGIN</tt> block and <tt>api_pm_note_state</tt> in an
 * <tt>EXCEPTION_CATCH( TRUE )</tt> block.
 * <br><br>
 * <b>Effect:</b> System routine
 * <br><br>
 * @param out
 * outcome of operation.
 * @param depth
 * depth of operation nesting after call.
 **/
outcome api_pm_note_state(
    outcome out,                   // outcome for the operation
	int& depth				   // operation nesting depth after call
    );
/**
 * Names the current state.
 * <br><br>
 * <b>Role:</b> This API assigns a name to the recent operation. Call
 * <tt>api_pm_name_state</tt> immediately after <tt>api_pm_note_state</tt> and before opening the
 * next sate with to <tt>api_pm_start_state</tt>. <tt>api_pm_name_state</tt> names the most recent
 * noted state. <tt>api_pm_name_state</tt> can also be called immediately following starting
 * the modeler if it were desired that the "root" state be named. Use the specified
 * name in calls to <tt>api_pm_roll_to_state</tt> to roll to the start of the current
 * operation. All <tt>api_pm</tt> functions should be thought of as requiring the use of the
 * <tt>PART_CONTEXT</tt> class.
 * <br><br>
 * <b>Effect:</b> System routine
 * <br><br>
 * @param name
 * name to give to current operation.
 * @param hs
 * returns history stream.
 **/

outcome api_pm_name_state(
    const char* name,          // name to give to the current state
	HISTORY_STREAM* hs = NULL	// What stream to use if not default
    );
/**
 * Rolls to the start of a named state.
 * <br><br>
 * <b>Role:</b> If multiple operations have the same name, the latest one before the current state
 * is used. If no operations with the given name occur before the current state,
 * the first one after the current state is used. All <tt>api_pm</tt> functions should be
 * thought of as requiring the use of the <tt>PART_CONTEXT</tt> class.
 * <br><br>
 * <b>Effect:</b> System routine
 * <br><br>
 * @param name
 * name of state to which to roll.
 * @param hs
 * history stream.
 * @param n_actual
 * number of states actually rolled.
 **/

outcome api_pm_roll_to_state(
    const char* name,          // Name of state you want to roll to
	HISTORY_STREAM* hs,        // What stream to use if not default
	int& n_actual              // number of delta states actually rolled.
    );
/**
 * Rolls forward or backward a specified number of states.
 * <br><br>
 * <b>Role:</b>  A negative number rolls to an earlier state; a positive number rolls to
 * a later state. All <tt>api_pm</tt> functions should be thought of as requiring the use of the
 * <tt>PART_CONTEXT</tt> class.
 * <br><br>
 * <b>Effect:</b> System routine
 * <br><br>
 * @param n_wanted
 * number of states to roll.
 * @param hs
 * history stream to roll.
 * @param n_actual
 * returns actual number of states rolled.
 **/

outcome api_pm_roll_n_states(
    int n_wanted,              // The number of states you want to roll
	HISTORY_STREAM* hs,           // What stream to use if not default
    int& n_actual              // The number of states actually rolled
    );
/**
 * Deletes all states.
 * <br><br>
 * <b>Role:</b> This API deletes all operations defined using <tt>api_pm_start_state</tt>
 * and <tt>api_pm_note_state</tt> for the given history stream. Use this API when clearing a
 * part in preparation for loading or creating a new part. All <tt>api_pm</tt> functions
 * should be thought of as requiring the use of the <tt>PART_CONTEXT</tt> class.
 * <br><br>
 * <b>Effect:</b> System routine
 * <br><br>
 * @param hs
 * history stream to delete.
 **/

outcome api_pm_delete_all_states(	HISTORY_STREAM* hs = NULL);

/** @} */
#endif
