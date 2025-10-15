/* ORIGINAL: acis2.1/kerndata/bulletin/bulletin.hxx */
// $Id: bulletin.hxx,v 1.78 2002/03/06 17:24:06 jenglund Exp $
/*******************************************************************/
/*    Copyright (c) 1989-2020 by Spatial Corp.                     */
/*    All rights reserved.                                         */
/*    Protected by U.S. Patents 5,257,205; 5,351,196; 6,369,815;   */
/*                              5,982,378; 6,462,738; 6,941,251    */
/*    Protected by European Patents 0503642; 69220263.3            */
/*    Protected by Hong Kong Patent 1008101A                       */
/*******************************************************************/

// Header for delta-state, bulletin-board and bulletin.

// Bulletins are chained into bulletin-boards which are chained into
// delta-states.

#ifndef BULLETIN_CLASS
#define BULLETIN_CLASS

// STIPORT TAC macintosh needs this
#include "acis.hxx"
#include "dcl_kern.h"

#include "entity.hxx"
#include <stddef.h>
#include "logical.h"

#include "debugmsc.hxx"
#include "glists.hxx"
#include "bullsmal.hxx"
#include "option.hxx"
#include "vlists.hxx"

/**
* @file bulletin.hxx
 * @CAA2Level L1
 * @CAA2Usage U1
 * \addtogroup ACISHISTORY
 *
 * @{
 */
class SizeAccumulator;
typedef int tag_id_type;
class outcome;
class DELTA_STATE;
class BULLETIN_BOARD;
class BULLETIN;
class HISTORY_STREAM;
class ATTRIB_HISTORY;
class FileInterface; 
/**
 * \class DELTA_STATE_LIST
 * Implements a variable length list of delta states.
 * <br><br>
 * <b>Role:</b> This class provides a constructor (which creates an empty list), a destructor, a
 * function to add an delta state (only if not already there), a function to look up a delta state by
 * pointer value, and a function to count the number of delta states listed. Also provides an
 * overloaded <tt>[ ]</tt> operator for access by position.
 * <br><br>
 * The functions are all essentially dummy; just indirecting through the header pointer. This is done
 * in order to insulate the application programs completely from the implementation of lists.
 * <br><br>
 * The current implementation uses hashing so that look up is fast provided lists are not very long;
 * it is also efficient for very short lists and for repeated lookups of the same delta state.
 * <br><br>
 * When a group of similar arguments must be returned, and the number of arguments is not known in
 * advance, the system returns the arguments as an <tt>DELTA_STATE_LIST</tt>. The number of members of an
 * <tt>DELTA_STATE_LIST</tt> can be found using the member function count, and individual members can be accessed
 * with the subscript operator <tt>[ ]</tt>.
 * <br><br>
 * The <tt>DELTA_STATE_LIST</tt> class is a variable length associative array of <tt>DELTA_STATE</tt> pointers. When using
 * the subscript operator, a cast is required to change the <tt>DELTA_STATE</tt> pointer into the correct type.
 * Many ACIS internal algorithms use <tt>DELTA_STATE_LIST</tt> including the part copy, save, and restore
 * algorithms. <tt>DELTA_STATE_LIST</tt> is also useful in ACIS components and applications.
  * @see DELTA_STATE, VOID_LIST
 */
LIST(DELTA_STATE, KERN);
/**
 * \class BULLETIN_BOARD_LIST
 */
LIST(BULLETIN_BOARD, KERN);

/**
 * \class BULLETIN_LIST
 * Implements a variable length list of bulletins.
 * <br><br>
 * <b>Role:</b> This class provides a constructor (which creates an empty list), a destructor, a
 * function to add an bulletin (only if not already there), a function to look up a bulletin by
 * pointer value, and a function to count the number of bulletins. Also provides an
 * overloaded <tt>[ ]</tt> operator for access by position.
 * <br><br>
 * The functions are all essentially dummy; just indirecting through the header pointer. This is done
 * in order to insulate the application programs completely from the implementation of lists.
 * <br><br>
 * The current implementation uses hashing so that look up is fast provided lists are not very long;
 * it is also efficient for very short lists and for repeated lookups of the same bulletin.
 * <br><br>
 * When a group of similar arguments must be returned, and the number of arguments is not known in
 * advance, the system returns the arguments as an <tt>BULLETIN_LIST</tt>. The number of members of an
 * <tt>BULLETIN_LIST</tt> can be found using the member function count, and individual members can be accessed
 * with the subscript operator <tt>[ ]</tt>.
 * <br><br>
 * The <tt>BULLETIN_LIST class</tt> is a variable length associative array of <tt>BULLETIN</tt> pointers. When using
 * the subscript operator, a cast is required to change the <tt>BULLETIN</tt> pointer into the correct type.
 * Many ACIS internal algorithms use <tt>BULLETIN_LIST</tt> including the part copy, save, and restore
 * algorithms. <tt>BULLETIN_LIST</tt> is also useful in ACIS components and applications.
 * <br><br>
 * @see BULLETIN, VOID_LIST
 */
LIST(BULLETIN, KERN);		// The mkman documentation for this class definition is at the bottom of this file.
/**
 * \class HISTORY_STREAM_LIST
 * Implements a variable length list of HISTORY_STREAMs.
 * <br><br>
 * <b>Role:</b> This class provides a constructor (which creates an empty list), a destructor, a
 * function to add an history stream (only if not already there), a function to look up a history stream by
 * pointer value, and a function to count the number of history stream listed. Also provides an
 * overloaded <tt>[ ]</tt> operator for access by position.
 * <br><br>
 * The functions are all essentially dummy; just indirecting through the header pointer. This is done
 * in order to insulate the application programs completely from the implementation of lists.
 * <br><br>
 * The current implementation uses hashing so that look up is fast provided lists are not very long;
 * it is also efficient for very short lists and for repeated lookups of the same history stream.
 * <br><br>
 * When a group of similar arguments must be returned, and the number of arguments is not known in
 * advance, the system returns the arguments as an <tt>HISTORY_STREAM_LIST</tt>. The number of members of an
 * <tt>HISTORY_STREAM_LIST</tt> can be found using the member function count, and individual members can be accessed
 * with the subscript operator <tt>[ ]</tt>.
 * <br><br>
 * The <tt>HISTORY_STREAM_LIST</tt> class is a variable length associative array of <tt>HISTORY_STREAM</tt> pointers. When using
 * the subscript operator, a cast is required to change the <tt>HISTORY_STREAM pointer</tt> into the correct type.
 * Many ACIS internal algorithms use <tt>HISTORY_STREAM_LIST</tt> including the part copy, save, and restore
 * algorithms. <tt>HISTORY_STREAM_LIST</tt> is also useful in ACIS components and applications.
 * @see HISTORY_STREAM, VOID_LIST
 */
LIST(HISTORY_STREAM, KERN);	// The mkman documentation for this class definition is at the bottom of this file.


// Externally-useful functions to control rollback.

// Start a new bulletin-board. At the outermost level of checkpointing,
// removes any failed previous bulletin-board and constructs a new one
// to run sequentially with any previous successful ones. At any other
// level of checkpointing, if the argument is FALSE, takes no action,
// so that the current bulletin-board continues in use. If the argument
// is TRUE, in effect pushes a new bulletin-board on a stack, whence it
// may subsequently be removed by a matching close_bulletin_board(
// TRUE ), restoring everything to its previous state.

/**
 * @nodoc
 */

DECL_KERN void
open_bulletin_board(
			logical = FALSE	// TRUE if the current bulletin-board
							// should be pushed down, so that it
							// can be restored after closing the new
							// one.
		);

// Close off the current bulletin board. If it was a stacked one,
// discard it, to get back to the state in which it was created, and
// then reinstate the previously-open one if any. Marks the bulletin-
// board with the supplied success value, and returns it for
// inspection.

/**
 * @nodoc
 */

DECL_KERN BULLETIN_BOARD *
close_bulletin_board(
			outcome&,		// outcome from the operation, could be changed if streams mixed
			logical,		// must match the argument of the
							// corresponding "open_bulletin_board" - not true anymore for API_TRIAL_BEGIN/END
            logical = FALSE // whether or not a stacked bulletin board is to be deleted
		);

// Release a bulletin-board returned in an API outcome, and clean up
// the data structure, either by rolling back if it was unsuccessful,
// or just by clearing rollback pointers. Normally unnecessary, as it
// is done automatically before the next bulletin-board operation, but
// possibly useful for an application which wishes to examine the data
// structure in its initial state after a failed API function.

/**
 * @nodoc
 */

DECL_KERN void release_bb(BULLETIN_BOARD *bb = NULL);

// Abort the current operation, rolling back any incomplete bulletin
// boards, and resetting the logging level to zero. Used in the API on
// user interrupt before returning to the application.

/**
 * @nodoc
 */

DECL_KERN void abort_bb(HISTORY_STREAM* hs = NULL);


// Clear all rollback pointers in entities referenced in all bulletin
// boards of a delta state and all bulletins in the bulletin boards.
// Must clear in old as well as new entities otherwise a subsequent
// roll will leave rollback pointers set for change bulletins (memcpy
// is used to exchange contents of old and new records including their
// rollback pointers))

/**
 * @nodoc
 */

DECL_KERN void
clear_rollback_ptrs(
			DELTA_STATE *ds
		);

/**
 * @nodoc
 */

DECL_KERN void clear_bb_rollback_ptrs(BULLETIN_BOARD *bb);

// STI ROLL begin:  Needed for distribution

/**
 * @nodoc
 */

DECL_KERN void set_rollback_ptrs(BULLETIN_BOARD *bb);
// STI ROLL end

// STI ROLL - remove incorrent and unused function
// DECL_KERN void clear_current_ds(HISTORY_STREAM* hs);

// Return all the changes since the last call or the start of the
// session, NULL if there are none.

/**
 * @nodoc
 */

DECL_KERN DELTA_STATE *note_state(HISTORY_STREAM* = NULL, logical delete_if_empty = FALSE);

// STI ROLL begin: Function that deletes the current delta state and returns the
//                 model to the state before construction of the current state began

/**
 * @nodoc
 */

DECL_KERN void abort_state(HISTORY_STREAM* = NULL);

// function aid in debbuging of history streams

/**
 * @nodoc
 */

DECL_KERN void debug_history_stream(HISTORY_STREAM *hs = NULL,
                                    FILE *fp = debug_file_ptr,
                                    int id = 0, int ent_level = 0, int level = 1);
// STI ROLL end

// Change modeller state from current state, A say, to different state, B,
// as determined by the given DELTA_STATE, and change the delta-state
// so that it describes the reverse state change (B to A).
// Note that change of state is permitted only if the currently-stored
// model state number matches the delta-state <tt>from_state</tt>, and there
// are no changes recorded in the current delta-state, except possibly
// for one unsuccessful bulletin-board, which is always undone first.

/**
 * @nodoc
 */

DECL_KERN logical change_state( DELTA_STATE *, HISTORY_STREAM* = NULL );

/**
 * @nodoc
 */

DECL_KERN logical merge_states( DELTA_STATE* = NULL, DELTA_STATE* = NULL,
                                HISTORY_STREAM* = NULL, logical = FALSE,
                                logical = FALSE );

// Debug printout of all bulletin boards in a delta_state.
// Runs down bulletin-board and for each bulletin, prints entries
// in order, giving address, type and rollback address.

/**
 * @nodoc
 */

DECL_KERN void debug_delta_state( DELTA_STATE const *, FILE * );

// Deletes a DELTA_STATE and all DELTA_STATEs below it in the tree.

/**
 * @nodoc
 */

DECL_KERN void delete_ds_branch(DELTA_STATE *, logical = FALSE);

// Delete all recorded DELTA_STATEs
// This is used by api_stop_modeller to safely release as much memory as
// possible before naively releasing all freelist blocks.

// STI ROLL begin:  Add logical to keep the stream, but empty it - also
//                  return TRUE only if everything is deleted.
/**
 * Deletes all recorded <tt>DELTA_STATEs</tt>.
 * <br><br>
 * <b>Role:</b> This method is used by the <tt>api_stop_modeller</tt> to safely release
 * as much memory as possible before releasing all free-list blocks.
 * If <tt>keep_default</tt> is set <tt>TRUE</tt>, it will keep the default stream, but empty it,
 * and also return <tt>TRUE</tt> only if everything is deleted.
 * <br>
 * <br><br>
 * <b>Effect:</b> System routine
 * <br><br>
 * @param del_stream
 * history stream.
 * <br><br>
 * @param keep_stream
 * keep the default stream if flag is <tt>TRUE</tt>.
 **/
DECL_KERN logical delete_all_delta_states(HISTORY_STREAM *& del_stream, logical keep_stream = FALSE);
// STI ROLL end

/**
 * @nodoc
 */

DECL_KERN void initialize_delta_states();

// A function pointer type used in various search routines.
/**
 * @nodoc
 */
typedef logical (*spa_is_function)(const ENTITY*);

// The STATE_ID is an (integer) identifier used to give a unique
// name to each state of the modeller;  the state changes at each
// call to the ai routine note_state.
// When the model is rolled back to earlier state, the current
// state number is reset to the state number of the earlier state,
// but subsequent new states are given fresh numbers.
// State numbers run from 1.

/**
 * @nodoc
 * Identifier for a STATE.
 * <br>
 * <b>Role:</b> Unique identifier for a given STATE.
 */
typedef int STATE_ID;

// The DELTA_STATE class returns a sequence of bulletin-boards
// that will be used to change the modeller from the <tt>from_state</tt> to
// the to_state.  The bulletin-boards are created between
// successive calls to note_state.  They are chained together in
// a singly-linked list beginning at bb_ptr.

// An option constrolling whether forward states are deleted as
// new ones are added, effectively forcing an unbranched stream.
// Only used in the kernel, so not DECL'd
//extern SESSION_GLOBAL_VAR option_header delete_forward;

class DELTA_STATE_LIST;
class ENTITY_LIST;
class ENTITY_ARRAY;
class DELTA_STATE_LIST;

/**
 * @nodoc
 */

DECL_KERN DELTA_STATE* current_delta_state();

/**
 * @nodoc
 */

logical save_history(FileInterface* file_ptr,
                     HISTORY_STREAM* hs,
                     logical active_ents_only,
                     logical mainline_only);


// An enum specifying the type of entity recorded in a bulletin we are
// looking for in find_entities functions.

/**
 * Tags that classify ENTITY's within a specific delta state or history stream.
 * @param ENTITY_CREATED
 * A tag that identifies ENTITY's that were created within the scope of a specific delta state or history stream.
 * @param ENTITY_CHANGED
 * A tag that identifies ENTITY's that were changed within the scope of a specific delta state or history stream.
 * @param ENTITY_DELETED
 * A tag that identifies ENTITY's that were deleted within the scope of a specific delta state or history stream.
 * @param ENTITY_CHANGED_OR_DELETED
 * A tag that identifies ENTITY's that were changed or deleted within the scope of a specific delta state or history stream.
 * @param ENTITY_SURVIVORS
 * A tag that identifies ENTITY's that were created, but not deleted, within the scope of a specific delta state or history stream.
 */
enum ENTITY_TYPE {
	ENTITY_CREATED,
	ENTITY_CHANGED,
	ENTITY_DELETED,
	ENTITY_CHANGED_OR_DELETED,
	ENTITY_SURVIVORS
	};

// STI ROLL begin: Option and enumerated type to control how history checking is done

/**
 * @nodoc
 */

enum hs_checks_level {
    hs_checks_off,
    hs_checks_warning,
    hs_checks_error,
	hs_checks_max
};


/**
 * An abstract base class that defines the entity reference tag management system interface.
 * <br>
 * Entity reference tags are, by default, managed by a basic tag management system 
 * derived from BASE_TAG_MANAGER. Each history stream contains a dedicated instance 
 * of this basic tag management system, which means that entity references are only 
 * unique within the respective stream. An application can implement it's own tag management system 
 * by deriving from BASE_TAG_MANAGER, and supplying the required functionality. 
 * This application specific tag manager can then be installed into the history stream by calling 
 * the BASE_TAG_MANAGER::set_active_tag_manager method. Replacing the default tag manager can be useful 
 * in applications that require unique entity references throughout multiple history streams. 
*/
class DECL_KERN BASE_TAG_MANAGER : public ACIS_OBJECT {

	static BASE_TAG_MANAGER * external_tag_manager;

protected:

	void assign_tag( ENTITY * ent, tag_id_type id) { ent->assign_tag(id); }

public:

/**
 * Sets the specified tag array index to the input entity.
 * <br><br>
 * @param index
 * tag array index
 * @param entity
 * input entity
 */
	virtual void       set(int index, const ENTITY * entity )=0;

/**
 * Gets the entity from the specified tag array index, optionally returning an outcome on error.
 * <br><br>
 * @param index
 * tag array index
 * @param result
 * retrieval error result
 */
	virtual ENTITY   * get(int index, outcome & result)=0;

/**
 * Returns the current size of the tag array index.
 */
	virtual int        size()=0;

/**
 * Increases the size of the tag array to the requested amount.
 * <br><br>
 * @param size
 * new size of tag array
 */
	virtual void       grow(int size)=0;

/**
 * Returns the next available tag array index.
 * <br><br>
 * @param set_next
 * post-increment tag array index and grow array as required
 */
	virtual int        get_next_tag(logical post_increment=FALSE)=0;

/**
 * Sets the next available tag array index, growing the tag array as required.
 * @param index
 * next available tag index
 */
	virtual void       set_next_tag(int index)=0;

/**
 * Returns the currently active tag manager.
 */
	BASE_TAG_MANAGER * get_active_tag_manager() { return external_tag_manager==NULL?this:external_tag_manager; }

/**
 * Returns global manager status
 */
	logical is_global() const { return external_tag_manager==NULL?FALSE:TRUE; }

/**
 * Sets the active tag manager to the one specified.
 * @param tag_manager
 * new tag manager
 */
	static void        set_active_tag_manager(BASE_TAG_MANAGER * tag_manager) { external_tag_manager=tag_manager; }
};

/**
 * @nodoc
 * ACIS implementation of tag manager, internal use only.
 */
class DECL_KERN BASE_ACIS_TAG_MANAGER : public BASE_TAG_MANAGER {

    tag_id_type tag_array_size;
    tag_id_type next_tag_num;
    ENTITY   ** tag_array;

public:

	BASE_ACIS_TAG_MANAGER();
	~BASE_ACIS_TAG_MANAGER();

	void     set(int, const ENTITY *);
	ENTITY * get(int, outcome &);

	void     grow(int);
	int      size() { return tag_array_size; }

	int      get_next_tag( logical post_increment);
	void     set_next_tag(int inTag);
};


/**
 * @nodoc
 */
class DECL_KERN HISTORY_STREAM_user_data : public ACIS_OBJECT {

	// A class to allow applications to attach their own data to the
	// HISTORY_STREAM.  We provide virtual functions for the destructor
	// and to debug the data.  The virtual destructor provides a
	// callback to the application indicating the owning HISTORY_STREAM
	// is going away.  The debug function used called by the
	// HISTORY_STREAM debug function

 public:
	virtual ~HISTORY_STREAM_user_data();
	virtual void debug( FILE* = NULL );
};

/**
 * Contains a sequence of delta states that represents a model's history.
 * <br>
 * <b>Role:</b> This class contains a sequence of delta states representing
 * the history of changes made to the model. The delta states are made by calls
 * <tt>api_note_state</tt>. They are chained together in a doubly-linked list beginning at
 * the root state.  The active state is most recently closed delta state.  The
 * current state is the delta state where changes to the model are accumulated.
 * @see DELTA_STATE, DELTA_STATE_LIST, ENTITY, ENTITY_LIST, ENTITY_TYPE, BULLETIN, HISTORY_MANAGER 
 */
class DECL_KERN HISTORY_STREAM : public ACIS_OBJECT {
  
    BASE_ACIS_TAG_MANAGER   m_acis_tag_manager; // do not use directly, use m_tag_manager
    BASE_TAG_MANAGER      * m_tag_manager;

    // STI ROLL begin: id for stream and previous and next pointers
    HISTORY_STREAM *previous;
    HISTORY_STREAM *next;
    // STI ROLL end

    // STI ROLL begin:
    logical owns_entities_flag; // TRUE if stream owns entities - FALSE if only temporary
    logical distribute_flag;    // TRUE means distribution will take place - allows
                                // temporarily mixed streams
    // STI ROLL end

// Jeff 09.11.06 NESTED_HISTORY
// public for now
public:

/**
 * @nodoc
 */
	int nested_level; // depth of stream push stack
	stream_logging logging_flag; // if set, overrides global api_logging value

public:
// Jeff 03.17.08 TSA
// Testing feasibility of these methods.
/**
 * @nodoc
 */
	logical unhook_stream();
/**
 * @nodoc
 */
	logical hook_stream( HISTORY_STREAM*);
public:
   /**
    * Access function to tag manager
    */
	const BASE_TAG_MANAGER * tag_manager() const { return m_tag_manager; }
/**
 * @nodoc
 */
	virtual void full_size(SizeAccumulator&, logical = TRUE) const;
/**
 * Used in checking the history stream.
 */
 	ENTITY_LIST* active_check_list;
/**
 * Limit on the number of states to be kept to control the memory used by a stream.
 * <br><br>
 * <b>Role:</b> Enforced in note_state by pruning. Hidden states are not counted.
 */
 	unsigned         max_states_to_keep;  // zero means no limit
/**
 * Pointer to a list of delta states merged into this list.
 */
 	DELTA_STATE_LIST*  merged_states;

	// persistent only to make a connection during save/restore
/**
 * Persistent only to make a connection during save and restore.
 */
 	ATTRIB_HISTORY*                   attribute;
/**
 * Current state.
 */
 	STATE_ID current_state;
/**
 * Acts as a state number server giving a new (unused) state number on request.
 * <br><br>
 * <b>Role:</b> When model is rolled back to earlier state, the current state number is
 * reset to the state number of the earlier state, but subsequent new states are taken
 * from <tt>next_state</tt>. State numbers increment from 1.
 */
 	STATE_ID next_state;
/**
 * Pointer to the current delta state.
 */
 	DELTA_STATE *current_ds;
/**
 * Pointer to the root delta state.
 */
 	DELTA_STATE *root_ds;
/**
 * Pointer to the active delta state.
 */
 	DELTA_STATE *active_ds;     // Model is as it was when active_ds
                                // was noted.

/**
 * Pointer to optional application data attached to the <tt>HISTORY_STREAM</tt>.
 */
 	HISTORY_STREAM_user_data *user_data;  // Not saved.

/**
 * Indicates if there are link states, Used by api_stop_modeler.
 */
 	logical link_states;	    // Link states for stop_modeller?

	// Flag controlling auto-checkpointing. At the start of every API
	// operation a checkpoint is placed (i.e. a new bulletin-board is
	// started), but this only has effect at the outermost level - nested
	// API calls share the same bulletin-board. The flag is incremented
	// for each call, and decremented for each return, and action is only
	// taken when it is zero.
/**
 * The number of api_begin's minus the number of api_end's made so far.
 * <br><br>
 * <b>Role:</b> In effect, this is the current API nesting level.
 */
 	int logging_level;

	// Deliver and set a new modeller state identifier
/**
 * Creates a new modeler state identifier.
 */
	STATE_ID new_state() {
		current_state = ++next_state;
		return current_state;
	}

	// Reset modeller state number to the given one.
/**
 * Resets the modeler state to the given state.
 * <br><br>
 * @param n
 * state to reset to.
 */
 	void reset_state(STATE_ID n) {
		current_state = n;
	}

 public:
/**
 * C++ allocation constructor requests memory for this object but does not populate it.
 */
     HISTORY_STREAM();
/**
 * C++ destructor, deleting a <tt>HISTORY_STREAM</tt>.
 */
 	~HISTORY_STREAM();
/**
 * Deletes delta states from this history stream.
 */
   	void delete_delta_states();
/**
 * Initialilzes the history stream.
 */
 	void initialize_delta_states();

/**
 * Retrieves the delta state matching the state parameters.
 * <br><br>
 * @param cs
 * current state.
 * <br><br>
 * @param ns
 * next state.
 * <br><br>
 * @param ds
 * delta state.
 */
	void get_delta_state(STATE_ID& cs, STATE_ID& ns, DELTA_STATE*& ds);
/**
 * Sets the current delta state to the given state.
 * <br><br>
 * @param cs
 * current state.
 * <br><br>
 * @param ns
 * next state
 * <br><br>
 * @param ds
 * delta state.
 */
 	logical set_delta_state(STATE_ID cs, STATE_ID ns, DELTA_STATE* ds);
/**
 * Gets the current state.
 */
 	STATE_ID get_current_state();
/**
 * Sets the current state to the given ID in the <tt>DELTA_STATE</tt>.
 * <br><br>
 * @param cs
 * current state.
 * <br><br>
 * @param ds
 * delta state.
 */
 	logical set_current_state(STATE_ID cs, DELTA_STATE* ds);
/**
 * Sets the state link.
 * <br><br>
 * @param sl
 * state link flag.
 */
 	logical set_state_linking( logical sl );
/**
 * Retrieves the root <tt>DELTA_STATE</tt>.
 */
 	DELTA_STATE* get_root_ds();
/**
 * Retrieves the active <tt>DELTA_STATE</tt>.
 */
	DELTA_STATE* get_active_ds();
/**
 * Gets the current <tt>DELTA_STATE</tt>.
 */
 	DELTA_STATE* get_current_ds();

	// Re-Initialize to an empty stream w/just the root_ds
	// sys_error if logging_level != 0
/**
 * Re-initializes to an empty stream with just the root_dssys_error if logging_level is not equal to zero.
 */
 	void clear();

    // STI ROLL begin:
    // Clear reference to this history from entities in stream.
/**
 * Clears reference to this history from entities in delta state.
 */
     void clear_history_ptrs();

    // read and set flag for owning entities
/**
 * Reads the flag for owning entities.
 */
     logical owns_entities() const { return owns_entities_flag; }
/**
 * Sets the flag for owning entities.
 * <br><br>
 * @param o
 * owning flag.
 */
     void set_owns_entities(logical o) { owns_entities_flag = o; }
/**
 * Sets the flag for distribution.
 *<br><br>
 * @param o
 * distribution flag.
 */
     void set_distribute_flag(logical o) { distribute_flag = o; }
/**
 * Returns the distribute_flag value.
 */
     logical distribution_on() const { return distribute_flag; }
    // STI ROLL end

	// Get the current bulletin-board.
/**
 * Obtains access to the current bulletin board for update functions.
 */
 	BULLETIN_BOARD * current_bb();
/**
 * Returns the pointer to the current <tt>DELTA_STATE</tt>.
 */
 	DELTA_STATE* current_delta_state()
	{
		return current_ds;
	}
/**
 * Saves the id level, next state, maximum states to keep, pointers to the current delta state, active delta state, and root delta state, and attributes.
 * <br><br>
 * @param elist
 * entity list.
 * <br><br>
 * @param hslist
 * history stream list
 * <br><br>
 * @param dslist
 * delta state.
 */
 	logical save(
		ENTITY_LIST& elist,
		HISTORY_STREAM_LIST& hslist,
		DELTA_STATE_LIST& dslist
		);
/**
 * Restores history stream.
 */
 	logical restore();
/**
 * @nodoc
 */
	logical fix_pointers(
		ENTITY_ARRAY const& earray,
		HISTORY_STREAM_LIST& hslist,
		DELTA_STATE_LIST& dslist
		);

/**
 * @nodoc
 */
	void roll_links(DELTA_STATE* dstate);
/**
 * Adds a delta state to this history stream.
 * <br><br>
 * @param dstate
 * delta state.
 */
 	void add(DELTA_STATE* dstate);
/**
 * Removes a delta state from this history stream.
 * <br><br>
 * @param dstate
 * delta state.
 */
 	void remove(DELTA_STATE* dstate);
/**
 * Disconnects a particular branch of delta states from the history stream. 
 * The branch to disconnect begins at the supplied delta state and is returned in a new history stream.
 * <br><br>
 * @param dstate
 * delta state
 */
 	HISTORY_STREAM* detach(DELTA_STATE* dstate);
/**
 * Attaches two delta states to one another in history stream.
 * <br><br>
 * @param dstate1
 * delta state.
 * <br><br>
 * @param dstate2
 * delta state.
 */
 	void attach(DELTA_STATE* dstate1, DELTA_STATE* dstate2);
	// Reset the owning stream after moving states between streams
/**
 * Resets the owning stream after moving states between streams.
 */
 	void set_owners();
/**
 * Merges this delta state in this history stream.
 * <br><br>
 * @param hist
 * history stream.
 */
 	void merge(HISTORY_STREAM* hist);
/**
 * Number of history stream levels to save to output file.
 * <br><br>
 * @param level
 * number of levels.
 */
  	void dump(int level = 0);
/**
 * Prints debugging information about the history stream.
 * <br><br>
 * @param id
 * entity id.
 * <br><br>
 * @param ent_level
 * entity derivation level.
 * <br><br>
 * @param level
 * delta state debugging level (0, 1, or 2).
 * <br><br>
 * @param fp
 * debug file pointer.
 */
	void debug( int id = 0, int ent_level = 0, int level = 1, FILE* fp = debug_file_ptr);
/**
 * Finds the entity from the history stream based on functionality.
 * <br><br>
 * @param flag
 * flag for an if entity.
 * <br><br>
 * @param elist
 * entity list.
 */
 	void find_entities(spa_is_function flag,
					   ENTITY_LIST& elist
					   );
/**
 * Finds the entity from the history stream based on type.
 * <br><br>
 * @param type
 * type of entity.
 * <br><br>
 * @param elist
 * entity list.
 */

	void find_entities(	enum ENTITY_TYPE type, ENTITY_LIST& elist);
/**
 * Lists the delta states.
 * <br><br>
 * @param dslist
 * delta state list.
 */
 	void list_delta_states( DELTA_STATE_LIST& dslist );

    // STI ROLL begin: adds create bulletins to root DELTA_STATE, for
    //                 use when pruning or saving/restoring empty histories
/**
 * Adds create bulletins to root <tt>DELTA_STATE</tt>, for use when pruning or saving and restoring empty histories.
 * <br><br>
 * @param survivors
 * survivors.
 * <br><br>
 * @param remove_existing_from_survivors
 * remove or not.
 */
    void add_create_bulletins_to_root_ds(ENTITY_LIST & survivors,
                                         logical remove_existing_from_survivors);
    // STI ROLL end

	// Snips the graph of <tt>DELTA_STATEs</tt> just before the given state and
	// deletes the piece that does not include active_ds.  Thus one can
	// prune forward branches by passing a state after active_ds.  One
	// can prune past history by passing active_ds or one prior to it.
	// It is impossible to prune away active_ds.
/**
 * Snips the graph of <tt>DELTA_STATEs</tt> just before the given state and deletes the piece that does not include active_ds.
 * <br><br>
 * <b>Role:</b> Thus one can prune forward branches by passing a state after active_ds.
 * One can prune past history by passing active_ds or one prior to it. It is impossible
 * to prune away active_ds.
 * <br><br>
 * @param ds
 * delta state.
 */
 	void prune(DELTA_STATE* ds);

	// A few of callers of the above, for common situations.

	// Prune away the earlier parts of the stream, saving active_ds and
	// numToSave earlier states.  This can be used to control the
	// memory required for history by limiting the number of states to
	// keep.
/**
 * Prunes away the earlier parts of the stream, saving active_ds and numToSave earlier states.
 * <br><br>
 * <b>Role:</b> This can be used to control the memory required for history by limiting
 * the number of states to keep.
 * <br><br>
 * @param numToSave
 * number to save.
 */
	void prune_previous(int numToSave);

	// Prune away all states after active_ds.  There is no numToSave
	// here because we don't know what branch to chop if there is a
	// branch.  This is not believed to be a practical limitation.
/**
 * Prunes away all states after active_ds.
 * <br><br>
 * <b>Role:</b> There is no numToSave here because we don't know what branch to chop
 * if there is a branch. This is not believed to be a practical limitation.
 */
 	void prune_following();

	// The active path runs from the root, to the current state of
	// the model (active_ds).  This routine prunes away states not in
	// the active path.
/**
 * The active path runs from the root, to the current state of the model (active_ds).
 * <br><br>
 * <b>Role:</b> This routine prunes away states not in the active path.
 */
 	void prune_inactive();

	// Just inactive branches emminating from the give delta state.
/**
 * The active path runs from the root, to the current state of the model (active_ds).
 * <br><br>
 * <b>Role:</b> This routine prunes away states not in the active path, limiting the prune
 * to inactive branches.
 * <br><br>
 * @param ds
 * delta state.
 */
	void prune_inactive_branch(DELTA_STATE* ds);

	// This prunes as well, if we already have more than max_states
/**
 * Establishes the maximum number of states to keep in history stream.
 * <br><br>
 * <b>Role:</b> Additional states above this maximum are pruned.
 * <br><br>
 * @param
 * numToSet
 * number to set.
 */
 	void set_max_states_to_keep(int numToSet);

	// Some simple tests
/**
 * Simple test to see whether stream can be rolled back.
 */
 	logical can_roll_back();
/**
 * Simple test to see whether stream can be rolled forward.
 */
 	logical can_roll_forward();

    // STI ROLL begin: some functions to verify the stream is correct
/**
 * Function to verify the stream is not mixed with other streams.
 * <br><br>
 * @param alternate_hs
 * alternate stream.
 */
     logical mixed_streams(HISTORY_STREAM*& alternate_hs);
/**
 * Function to check that the tags on <tt>ENTITYs</tt> in the <tt>HISTORY_STREAM</tt> are valid.
 */
    outcome check_tags_validity();
/**
 * Function returns all tagged entities in the history stream - even
 * those that are not active.
 * <br><br>
 * @param elist
 * entity list.
 */
     void get_tagged_entities(ENTITY_LIST& elist);
    // STI ROLL end

	// Detirmine if a given state is in this stream
/**
 * Determines whether a given state is in this stream.
 * <br><br>
 * @param ds
 * delta state.
 */
 	logical in_stream(DELTA_STATE* ds);

	// For debugging, we expose the logging_level as a read only
	// Thus people can assert they are at the outermost level
/**
 * The logging level is the number of api_begin calls minus the number of api_end calls, and represents the current nesting of api calls.
 */
 	int get_logging_level() { return logging_level; }
/**
 * Returns the stream specific application logging value.
 * @see api_get_stream_logging, api_set_stream_logging
 */
	stream_logging get_logging() { return logging_flag; }

/**
 * Sets the stream specific application logging value.
 * @param sl
 * stream_logging value to set.
 * @see api_set_stream_logging, api_get_stream_logging
 */
	void set_logging( stream_logging sl) { logging_flag = sl; }

	// How much space taken by this history stream
	// This includes all the history stream structure
	// and possibly the backup entities, but not the
	// active entities.
/**
 * Returns the amount of space taken by this history stream.
 * <br><br>
 * <b>Role:</b> This includes all the history stream structure and optionally the backup
 * entities, but not the active entities.
 * <br><br>
 * @param include_backups
 * backup entities counted if <tt>TRUE</tt>.
 */
 	int size(logical include_backups = TRUE) const;

    // STI ROLL begin:

    // methods to get next and previous streams
/**
 * Gets the next stream.
 */
    HISTORY_STREAM* next_stream() const { return next; }
/**
 * Gets the previous stream.
 */
    HISTORY_STREAM* previous_stream() const { return previous; }

    // Method to get an ENTITY from the given tag
/**
 * Returns the entity referred to by a given tag.  If result is supplied, no sys_error will be
 * thrown on error and the result will be changed.  Otherwise, sys_error on error.
 * <br><br>
 * @param tag_no
 * tag.
 * <br><br>
 * @param result
 * result.
 */
     ENTITY* get_entity_from_tag(tag_id_type tag_no, outcome& result = *(outcome*)NULL_REF);

    // Method to return the tag on an ENTITY in the HISTORY_STREAM
/**
 * Returns the tag on an <tt>ENTITY</tt> in the <tt>HISTORY_STREAM</tt>.
 * <br><br>
 * @param ent
 * entity.
 * <br><br>
 * @param check
 * perform checks or not.
 * <br><br>
 * @param required_id
 * required flag.
 */
    tag_id_type tag(const ENTITY *ent,
                    logical check = TRUE,
                    tag_id_type required_id = -1);

    // Methods to add and remove ENTITY from TAG array
/**
 * Removes an <tt>ENTITY</tt> from the <tt>TAG</tt> array.
 * <br><br>
 * @param tag_no
 * entity id.
 */
    ENTITY* remove_tag_reference(tag_id_type tag_no);
/**
 * Adds an <tt>ENTITY</tt> to the <tt>TAG</tt> array.
 * <br><br>
 * @param ent
 * entity.
 */
     tag_id_type restore_tag_reference(const ENTITY *ent);
/**
 * Assigns a tag to an entity.
 * <br><br>
 * @param ent
 * entity.
 *<br><br>
 * @param id
 * tag.
 */
    logical assign_tag(const ENTITY *ent, tag_id_type id);

    // method to get the delta_state from a given tag
/**
 * Gets the delta state from a given tag.
 *<br><br>
 * @param id
 * tag.
 */
    DELTA_STATE* get_state_from_id(STATE_ID id);
/**
 * Permits users to change user data in <tt>HISTORY_STREAM</tt>.
 * <br><br>
 * @param d
 * pointer to data.
 */
 	void set_user_data( HISTORY_STREAM_user_data* d ) { user_data = d; }
/**
 * Returns the user data that was attached to the <tt>HISTORY_STREAM</tt>.
 */
 	HISTORY_STREAM_user_data* get_user_data() { return user_data; }

private:
    // routine to allocate memory for pointers to tagged entities
    void ensure_tag_storage_size(tag_id_type max_tag_value);
    // allow save_history access so that it can copy tag information
    // to empty history when saving active_only
    friend logical save_history(FileInterface* file_ptr,
                                HISTORY_STREAM* hs,
                                logical active_ents_only,
                                logical mainline_only);
    // STI ROLL end:
};

/**
 * @nodoc
 */
class DECL_KERN DELTA_STATE_user_data : public ACIS_OBJECT {

	// A class to allow applications to attach their own data to the
	// DELTA_STATE.  We provide virtual functions for the destructor
	// and to debug the data.  The virtual destructor provides a
	// callback to the application indicating the owning DELTA_STATE
	// is going away.  The debug function used called by the
	// DELTA_STATE debug function

 public:
	virtual ~DELTA_STATE_user_data();
	virtual void debug( FILE* = NULL );
};

/**
 * Contains a sequence of bulletin boards.
 * <br><br>
 * <b>Role:</b> This class contains a sequence of bulletin boards that change the modeler
 * from the <tt>from_state</tt> to the <tt>to_state</tt>. The bulletin boards are created between successive
 * calls to <tt>api_note_state</tt>. They are chained together in a singly-linked list beginning at <tt>bb_ptr</tt>.
 * @see  BULLETIN_BOARD, DELTA_STATE_LIST, ENTITY_TYPE, HISTORY_STREAM, BULLETIN_BOARD
 */

class DECL_KERN DELTA_STATE : public ACIS_OBJECT {
    // STI ROLL begin: logical indicating if history stream should be reset on delete
    logical reset_history;
    // STI ROLL end
public:
/**
 * Previous modeler state.
 */
 	STATE_ID from_state;
/**
 * Next modeler state to change to.
 */
 	STATE_ID to_state;
/**
 * Sets when state is noted.
 */
 	STATE_ID this_state;
/**
 * Delta records a backward change.
 */
 	logical  rolls_back;
/**
 * Not counted for <tt>roll_n_states</tt> or <tt>max_states</tt>.
 */
 	logical  hidden;
/**
 * Pointer to bulletin board.
 */
 	BULLETIN_BOARD *bb_ptr;
/**
 * The delta state whose <tt>to_state</tt> equals this <tt>from_state</tt>.
 */
 	DELTA_STATE *prev_ds;
/**
 * A delta state whose <tt>from_state</tt> equals this <tt>to_state</tt>.
 */
 	DELTA_STATE *next_ds;
/**
 * Circular list of delta states with same <tt>from_state</tt>.
 */
 	DELTA_STATE *partner_ds;
/**
 * Allows history stream to be found from delta state.
 */
 	HISTORY_STREAM *owner_stream;
/**
 * Pointer to optional application data attached to the <tt>DELTA_STATE</tt>.
 */
 	DELTA_STATE_user_data *user_data;  // Not saved.
/**
 * The delta states merged into this one.
 */
 	DELTA_STATE_LIST* merged_states;
/**
 * The delta state this one merges into.
 */
 	DELTA_STATE* merged_with_ds;  // State this one merges into
	                           // A state which must have rolls_back == FALSE
	                           // before this one can roll back
/**
 * Name string for the <tt>DELTA_STATE</tt>.
 */
 	char *name_str;

// Jeff 09.11.06 NESTED_HISTORY
// public for now
public:

/**
 * @nodoc
 */
 	DELTA_STATE_LIST* nested_states;   // nested states that were pushed

public:
/**
 * @nodoc
 */
	logical save(ENTITY_LIST& elist, 
				 HISTORY_STREAM_LIST& hslist, 
				 DELTA_STATE_LIST& dslist, 
				 logical mainline=FALSE) const; // STI ROLL
/**
 * Restores <tt>DELTA_STATE</tt> to the state provided by a previous bulletin board.
 */
 	logical restore();
/**
 * The <tt>fix_pointers</tt> method for each entity in the restore array is called, with the array as argument.
 * <br><br>
 * <b>Role:</b> This calls <tt>fix_common</tt>, which calls its parent's <tt>fix_common</tt>, and then
 * corrects any pointers in the derived class. In practice there is never anything
 * special for <tt>fix_pointers</tt> to do, but it is retained for consistency and compatibility.
 * (Supplied by the <tt>ENTITY_FUNCTIONS</tt> and <tt>UTILITY_DEF</tt> macros.)
 * <br><br>
 * @param elist
 * pointers to fix.
 * <br><br>
 * @param hslist
 * hist stream list.
 * <br><br>
 * @param dslist
 * delta state list.
 */
	logical fix_pointers(ENTITY_ARRAY& elist, HISTORY_STREAM_LIST& hslist, DELTA_STATE_LIST& dslist);
/**
 * Permits users to change user data in <tt>DELTA_STATE</tt>.
 * <br><br>
 * @param d
 * pointer to data.
 */
 	void set_user_data( DELTA_STATE_user_data* d ) { user_data = d; }
/**
 * Returns the user data that was attached to the <tt>DELTA_STATE</tt>.
 */
 	DELTA_STATE_user_data* get_user_data() { return user_data; }
/**
 * Returns a name string of the <tt>DELTA_STATE</tt>.
 */
 	const char* get_name() { return name_str; }
/**
 * Changes name of <tt>DELTA_STATE</tt>.
 * <br><br>
 * @param n
 * name.
 */
	void set_name( const char* n);
/**
 * Returns the name of the delta state.
 * <br><br>
 * @param n
 * name of delta state.
 */
 	logical is_named(const char* n);
/**
 * Returns the owner of the stream.
 */
 	HISTORY_STREAM* history_stream() { return owner_stream; }
/**
 * Hides the given <tt>DELTA_STATE</tt>.
 * <br><br>
 * @param h
 * hide or not.
 */
 	logical hide(logical h) { logical r = hidden; hidden = h; return r; }

	// STI ROLL
/**
 * @nodoc
 */
	virtual void full_size(SizeAccumulator&, logical = TRUE) const;
	// STI ROLL


	// Constructor allows one to specify the owning history stream
/**
 * C++ initialize constructor requests memory for this object and populates it with the data supplied as arguments.
 * <br><br>
 * <b>Role:</b> Creates a new state, with the <tt>to_state</tt> set to the current state, the <tt>from_state</tt> set to 0,
 * and the <tt>bb_ptr</tt> set to <tt>NULL</tt>.
 * <br><br>
 * @param hist
 * history.
 */
 	DELTA_STATE(HISTORY_STREAM* hist = NULL);		// constructor


/**
 * C++ destructor, deleting a <tt>DELTA_STATE</tt> and their bulletins, that constitute the <tt>DELTA_STATE</tt>.
 */
 	~DELTA_STATE();		// destructor

	// Read only access to private data.

/**
 * Read only access to the originating <tt>DELTA_STATE</tt>.
 */
 	STATE_ID from() const;
/**
 * Read only access to the destination <tt>DELTA_STATE</tt>.
 */
 	STATE_ID to() const;
/**
 * Returns the <tt>STATE_ID</tt>.
 */
 	STATE_ID id() const;
/**
 * Returns <tt>TRUE</tt> if this <tt>DELTA_STATE</tt> rolls backward.
 */
 	logical  backward() const { return rolls_back; }
/**
 * Returns <tt>TRUE</tt> if this <tt>DELTA_STATE</tt> rolls forward.
 */
 	logical  forward() const { return ! backward(); }
/**
 * Returns the <tt>bb_ptr</tt>.
 */
 	BULLETIN_BOARD *bb() const { return bb_ptr; }
/**
 * Sets the identification of the <tt>from_state</tt> to the given <tt>STATE_ID</tt>.
 * <br><br>
 * @param from_id
 * state ID number.
 */
 	void set_from(STATE_ID from_id) { from_state = from_id; }
/**
 * Sets the identification of the <tt>to_state</tt> to the given <tt>STATE_ID</tt>.
 * <br><br>
 * @param to_id
 * state ID number.
 */
 	void set_to(STATE_ID to_id) { to_state = to_id; }
/**
 * Returns the next <tt>DELTA_STATE</tt>.
 */
 	DELTA_STATE *next() const { return next_ds; }
/**
 * Returns the previous <tt>DELTA_STATE</tt>.
 */
 	DELTA_STATE *prev() const { return prev_ds; }
/**
 * Returns the partner <tt>DELTA_STATE</tt>.
 */
 	DELTA_STATE *partner() const { return partner_ds; }


/**
 * Adds a new bulletin board to this delta state.
 * <br><br>
 * @param bb
 * bulletin board.
 */
 	void add( BULLETIN_BOARD *bb );

/**
 * Removes a new bulletin board from this delta state.
 * <br><br>
 * @param bb
 * bulletin board.
 */
 	void remove( BULLETIN_BOARD *bb );

/**
 * Rolls back over a complete delta state, inverting it so as to allow roll forward the next time.
 */
 	void roll();

/**
 * Returns <tt>TRUE</tt> if the <tt>DELTA_STATE</tt> contains no <tt>BULLETINs</tt>.
 */
 	logical is_empty() const;

/**
 * Outputs debug information about <tt>DELTA_STATE</tt> to the debug file or to the specified file.
 * <br><br>
 * @param fname
 * file name.
 */
 	void debug( FILE *fname = debug_file_ptr ) const;

/**
 * Aids in debugging the <tt>DELTA_STATE</tt>.
 * <br><br>
 * @param dslist
 * delta state.
 * <br><br>
 * @param level
 * debugging level (0, 1, or 2).
 * <br><br>
 * @param fname
 * file name.
 */
 	void debug_list( DELTA_STATE_LIST& dslist,
					int level = 0,
					FILE* fname = debug_file_ptr );

	// Annotation support

	// debugging.  The first two arguments specify a branch of the
	// entity derivation hierarchy to call debug_ent on, in addition to the
	// normal bulletin board debugging stuff.  For annotations we use
	// ANNOTATION_TYPE and ANNOTATION_LEVEL
/**
 * Outputs debug information about <tt>DELTA_STATE</tt> to the debug file or to the specified file.
 * <br><br>
 * @param id
 * entity id.
 * <br><br>
 * @param level
 * entity level.
 * <br><br>
 * @param fname
 * file name.
 */
 	void debug( int id, int level, FILE *fname = debug_file_ptr ) const;

/**
 * Prints debugging information with annotation support.
 * <br><br>
 * <b>Role:</b> The second and third arguments specify a branch of the entity derivation
 * hierarchy to call <tt>debug_ent</tt> on, in addition to the normal bulletin board debugging
 * information. For annotations we use <tt>ANNOTATION_TYPE</tt> and <tt>ANNOTATION_LEVEL</tt>.
 * <br><br>
 * @param dslist
 * delta state.
 * <br><br>
 * @param id
 * entity id.
 * <br><br>
 * @param ent_level
 * entity level.
 * <br><br>
 * @param level
 * debugging level (0, 1, or 2).
 *<br><br>
 * @param fname
 * file name.
 */
 	void debug_list( DELTA_STATE_LIST& dslist,
					int id, int ent_level,
					int level = 0,
					FILE* fname = debug_file_ptr );

	// finding annotations.  The first two arguments specify a branch of
	// the entity derivation hierarchy to return bulletins for.  For
	// annotation use, we can use <tt>ANNOTATION_TYPE</tt> and <tt>ANNOTATION_LEVEL</tt>.
	// It may also be useful to be more specific, such as
	// SWEEP_ANNOTATION_TYPE and SWEEP_ANNOTATION_LEVEL.
	//
	// in the second form the tester identifys the type of entity to look for
	// The is_XXXX functions generated by the ENTITY_DEF macro work well.
/**
 *  Function for finding annotations.
 * <br><br>
 * <b>Role:</b> The first two arguments specify a branch of the entity derivation
 * hierarchy to return bulletins for. For annotation use, we can use <tt>ANNOTATION_TYPE</tt>
 * and <tt>ANNOTATION_LEVEL</tt>. It may also be useful to be more specific, such as
 * <tt>SWEEP_ANNOTATION_TYPE</tt> and <tt>SWEEP_ANNOTATION_LEVEL</tt>. The <tt>is_XXXX</tt> functions generated
 * by the <tt>ENTITY_DEF</tt> macro work well.
 * <br><br>
 * @param type
 * entity type.
 * <br><br>
 * @param level
 * entity level.
 * <br><br>
 * @param blist
 * bulletin list.
 */
 	void find_bulletins(int type, int level, BULLETIN_LIST& blist) const;

/**
 * Function for finding annotations.
 * <br><br>
 * <b>Role:</b> The first two arguments specify a branch of the entity derivation
 * hierarchy to return bulletins for. In this form the tester identifies the type
 * of entity to look for. For annotation use, we can use <tt>ANNOTATION_TYPE</tt> and
 * <tt>ANNOTATION_LEVEL</tt>. It may also be useful to be more specific, such as
 * <tt>SWEEP_ANNOTATION_TYPE</tt> and <tt>SWEEP_ANNOTATION_LEVEL</tt>. The <tt>is_XXXX</tt> functions generated
 * by the <tt>ENTITY_DEF</tt> macro work well.
 * <br><br>
 * @param tester
 * test function.
 * <br><br>
 * @param blist
 * bulletin list.
 */
 	void find_bulletins(spa_is_function tester, BULLETIN_LIST& blist) const;
/**
 * Searches in entity list for a type of entity recorded in the bulletin.
 * <br><br>
 * @param type
 * type of entity.
 * <br><br>
 * @param elist
 * entity list.
 */
 	void find_entities(enum ENTITY_TYPE type , ENTITY_LIST& elist);

/**
 * Performs compression on a given <tt>DELTA_STATE</tt>.
 */
 	void compress();

	// merge with the next DELTA_STATE, keeping all the BULLETINs
	// and BULLETIN_BOARDs from both states in the correct order.
	// in this state and then deleting next.  If the next state had
	// partners, indicating a branch, the branch is pruned as there
	// would no longer be a sensible way to roll the model to states
	// on the branch.
	//
	// Repeated calls can be used to compress any linear range of delta
	// states with the same roll direction, into one state.
/**
 * Merge with the next <tt>DELTA_STATE</tt>.
 * <br><br>
 * <b>Role:</b> Keeping all the <tt>BULLETINs</tt> and <tt>BULLETIN_BOARDs</tt> from both states
 * in the correct order in this state, and then deleting next. If the next state
 * had partners, indicating a branch, the branch is pruned as there would no longer
 * be a sensible way to roll the model to states on the branch. Repeated calls can
 * be used to compress any linear range of delta states with the same roll direction,
 * into one state.
 */
 	void merge_next();

/**
 * Adds connectees to the delta state list.
 * <br><br>
 * @param dslist
 * change state list.
 */
 	void scan( DELTA_STATE_LIST& dslist ) const;

/**
 * @nodoc
 */
	friend DECL_KERN DELTA_STATE *note_state(HISTORY_STREAM*, logical);

	// How much space taken by this delta state
	// This includes all the history stream structure
	// and possibly the backup entities, but not the
	// active entities.
/**
 * Returns the size of the <tt>DELTA_STATE</tt>.
 * <br><br>
 * @param include_backups
 * include backups as part of size.
 */
  	int size(logical include_backups = TRUE) const;

    // STI ROLL begin
/**
 * Clears reference to this history from entities in the delta state.
 */
     void clear_history_ptrs();
/**
 * Sets history pointers.
 */
     void set_history_ptrs();
/**
 * Reset the history stream on deletion.
 */
     void reset_history_on_delete() { reset_history = TRUE; }
    // STI ROLL end

    // STI ROLL begin
/**
 * Checks for mixed history streams.
 * <br><br>
 * @param alternate_hs
 * alternate stream.
 */
     logical mixed_streams(HISTORY_STREAM*& alternate_hs);
    // STI ROLL end
};


// A BULLETIN_BOARD contains a list of BULLETINs, each of which records
// the changes to a single ENTITY during the current operation on the
// model. There are two types of current bulletin-board, mainline and
// stacked, and completed ones may be successful or failed, depending
// on the reported success of the completed operation.

/**
 * @nodoc
 */

enum bb_status {
	bb_open_mainline,
	bb_open_stacked,
	bb_closed_succeeded,
	bb_closed_failed,
	bb_suspended_mainline,	// Jeff 09.11.06 NESTED_HISTORY
	bb_suspended_stacked,	// added 5 new enums	
	bb_open_nested,
	bb_suspended_nested,
	bb_closed_nested
};

// STI ROLL begin: Status indicating whether bulletin board has been checked or
//                 not, and the result of the checking.  Bad check status will
//                 be reset on merging to no check, since merging can fix the
//                 problem of mixed streams.

/**
 * @nodoc
 */

enum bb_check_status {
    bb_not_checked,
    bb_checked_one_stream,
    bb_checked_mixed_streams,
    bb_checked_one_stream_corrupt,
    bb_checked_mixed_streams_corrupt,
	bb_of_failed_api
};
// STI ROLL end
/**
 * Contains a list of BULLETINS recording changes to <tt>ENTITYs</tt> during the current operation on the model.
 * <br><br>
 * <b>Role:</b> A <tt>BULLETIN_BOARD</tt> contains a list of <tt>BULLETINs</tt>, each of which records
 * the changes to a single <tt>ENTITY</tt> during the current operation on the model.There are two
 * types of current bulletin-board, mainline and stacked, and completed ones may be successful
 * or failed, depending on the reported success of the completed operation.
 * @See BULLETIN, DELTA_STATE, HISTORY_STREAM, DELTA_STATE, outcome
 */
 class DECL_KERN BULLETIN_BOARD : public ACIS_OBJECT {
	// STI ROLL  BB merge  Add flags for merge performance
	logical merge_pending;
	logical rollbacks_severed;

    // STI ROLL begin:
    logical reset_history;        //logical indicating if history stream
                                  //should be reset on delete
    bb_check_status check_status; // indicates whether checking has been done and
                                  // the results of the check
    HISTORY_STREAM *alt_hs;       // history stream that board must be
                                  // moved into to fix mixing
    logical bb_can_be_moved;      // TRUE if the board contains only create bulletins
    VOID_LIST dead_entities;      // list of entities that were lost or deleted
                                  // on this bulletin board - only valid during
                                  // construction - cleared when board is closed
    // STI ROLL end

// Jeff 09.11.06 NESTED_HISTORY
// public for now
public:
/**
 * @nodoc
 */
	int nested_order; // construction order when nested
/**
 * @nodoc
 */
	int nested_level; // depth of push stack of stream when constructed

public:
/**
 * Pointer to first bulletin.
 */
 	BULLETIN *start_b;
/**
 * Pointer to last bulletin.
 */
 	BULLETIN *end_b;
/**
 * Chains bulletin boards from a delta state.
 */
 	BULLETIN_BOARD *next_ptr;	// to chain bulletin-boards from a
								// delta-state
/**
 * The delta state from which this is chained.
 */
 	DELTA_STATE *owner_ptr;		// the delta-state which this hangs
								// from.
/**
 * Status of the bulletin board.
 * <br><br>
 * <b>Role:</b> Possible values are
 * <pre>
 * 	bb_open_mainline,
 * 	bb_open_stacked,
 * 	bb_closed_succeeded,
 * 	bb_closed_failed
 * </pre>
 */
    bb_status status;
/**
 * The number of api_begin's minus the number of api_end's made so far.
 * <br><br>
 * <b>Role:</b> In effect, this is the current API nesting level.
 */
 	int logging_level_when_stacked;

	logical contains_mixed_bulletins;

public:
/**
 * Saves the delta states and entities corresponding to this bulletin board.
 * <br><br>
 * @param elist
 * entities.
 * <br><br>
 * @param dslist
 * delta states.
 * <br><br>
 * @param ignore_string_version
 * ingore version or not.
 */
 	logical save(ENTITY_LIST& elist,DELTA_STATE_LIST& dslist, logical ignore_string_version = FALSE);
/**
 * Restores roll back to previous state.
 * <br><br>
 * @param previous_bb
 * previous state
 * <br><br>
 * @param ignore_string_version
 * ingore version or not
 */
 	logical restore(BULLETIN_BOARD* previous_bb, logical ignore_string_version = FALSE);
/**
 * The <tt>fix_pointers</tt> method for each entity in the restore array is called, with the array as argument.
 * <br><br>
 * <b>Role:</b> This calls <tt>fix_common</tt>, which calls its parent's <tt>fix_common</tt>, and then
 * corrects any pointers in the derived class. In practice there is never anything
 * special for <tt>fix_pointers</tt> to do, but it is retained for consistency and compatibility.
 * (Supplied by the <tt>ENTITY_FUNCTIONS</tt> and <tt>UTILITY_DEF</tt> macros.)
 * <br><br>
 * @param elist.
 * pointers to fix.
 * <br><br>
 * @param dslist
 * delta state list.
 */
	logical fix_pointers(ENTITY_ARRAY& elist, DELTA_STATE_LIST& dslist);
/**
 * Returns a pointer to the owner of the delta state.
 */
     DELTA_STATE* delta_state() const { return owner_ptr; }
/**
 * Returns the history stream associated with the owner pointer.
 */
     HISTORY_STREAM* history_stream() const { return owner_ptr ? owner_ptr->history_stream() : NULL; }


	// Create a new bulletin board at start of list given by bb_ptr
	// in current delta_state.
/**
 * C++ initialize constructor requests memory for this object and populates it with the data supplied as arguments.
 * <br><br>
 * <b>Role:</b> Creates a new bulletin board at start of list given by <tt>bb_ptr</tt> in the  current <tt>DELTA_STATE</tt>.
 *  Applications should call this constructor only with the overloaded new operator, because this reserves the memory on the heap, a requirement to support roll back and history management.
 * <br><br>
 * @param ds
 * change state.
 */
 	BULLETIN_BOARD(DELTA_STATE* ds = NULL);

	// This constructor is NOT for use by application developers.
	// It is for internal use only.
/**
 * C++ constructor.
 * <br><br>
 * @param in_current_ds
 * delta state.
 */
 	BULLETIN_BOARD(logical in_current_ds);

	// Remove and delete bulletin board (usually at head of list of
	// bulletin-boards in delta state) and delete its bulletin entries.
/**
 * C++ destructor, deleting a <tt>BULLETIN_BOARD</tt> (usually at head of list of bulletin-boards in the <tt>delta_state</tt>) and deletes its bulletin entries.
 */
 	~BULLETIN_BOARD();

	// Return private information for read only.
/**
 * Returns the next bulletin in the bulletin board.
 */
 	BULLETIN_BOARD *next() const { return next_ptr; }
/**
 * Returns the last bulletin in the bulletin board.
 */
 	BULLETIN *start_bulletin() const { return start_b; }
/**
 * Returns the last bulletin in the bulletin board.
 */
 	BULLETIN *end_bulletin() const { return end_b; }
/**
 * Returns <tt>TRUE</tt> if the bulletin-board closed successfully; otherwise, it returns <tt>FALSE</tt>.
 */
 	logical successful() const
				{ return (status == bb_closed_succeeded || status == bb_closed_nested); }
/**
 * Returns <tt>TRUE</tt> if the bulletin board failed to close successfully; otherwise, it returns <tt>FALSE</tt>.
 */
 	logical failure() const
				{ return status == bb_closed_failed; }
/**
 * Returns <tt>TRUE</tt> if the bulletin board closed successfully; otherwise, it returns <tt>FALSE</tt>.
 */
 	logical closed() const { return successful() || failure(); }
/**
 * Returns <tt>TRUE</tt> if the bulletin board opened successfully; otherwise, it returns <tt>FALSE</tt>.
 */
 	logical open() const { return !closed(); }

    // STI ROLL begin
/**
 * Returns the check status, whether or not any history streams are corrupt.
 */
     logical corrupt() const { return (check_status == bb_checked_mixed_streams_corrupt ||
        check_status == bb_checked_one_stream_corrupt); }
/**
 * Returns <tt>TRUE</tt> if this is a mixed stream.
 */
     logical mixed() const { return (check_status == bb_checked_mixed_streams ||
        check_status == bb_checked_mixed_streams_corrupt); }
/**
 * Returns whether or not the bulletin board has been checked.
 */
     logical checked() const { return (check_status != bb_not_checked); }
/**
 * Returns the bulleting board check status, indicating whether the bulletin board has been checked or not, and the result of the checking.
 */
     bb_check_status get_check_status() const { return check_status; }
/**
 * Sets the check status.
 * <br><br>
 * @param s
 * status.
 */
     void set_check_status (bb_check_status s) { check_status = s; }
/**
 * Returns whether or not the bulletin board can be moved.
 */
     logical can_be_moved() const { return bb_can_be_moved; }
    // STI ROLL end

	// Add a new bulletin to this bulletin_board.
/**
 * Adds a new <tt>BULLETIN_BOARD</tt> to this delta state.
 * <br><br>
 * @param bb
 * bulletin board.
 */
 	void add( BULLETIN *bb );

	// Remove a bulletin from this bulletin_board.
/**
 * Removes a bulletin board from this delta state.
 * <br><br>
 * @param bb
 * bulletin board.
 */
 	void remove( BULLETIN *bb );

	// Roll every bulletin on the bulletin-board, changing any entities
	// referenced, and flipping the sense of the bulletin-board from
	// "backward" to "forward" or vice versa.
/**
 * Rolls back over a complete delta state, inverting it so as to allow roll forward the next time.
 */
 	void roll();

// Debug printout.
/**
 * Writes information about the bulletin board to the debug file or to the specified file.
 * <br><br>
 * @param fname
 * file name.
 */
 	void debug( FILE *fname = debug_file_ptr ) const;

	// Annotation support

	// debugging.  The first two arguments specify a branch of the
	// entity derivation hierarchy to call debug_ent on, in addition to the
	// normal bulletin board debugging stuff.  For annotations we use
	// ANNOTATION_TYPE and ANNOTATION_LEVEL
/**
 * Writes information about the bulletin board to the debug file or to the specified file.
 * <br><br>
 * <b>Role:</b> The first two arguments specify a branch of the entity derivation hierarchy
 * to call <tt>debug_ent</tt> on, in addition to the normal bulletin board debugging stuff.
 * <br><br>
 * @param id
 * entity id.
 * <br><br>
 * @param level
 * entity level.
 * <br><br>
 * @param fname
 * file name.
 */
 	void debug( int id, int level, FILE *fname = debug_file_ptr ) const;

	// finding annotations.  The first two arguments specify a branch of
	// the entity derivation hierarchy to return bulletins for.  For
	// annotation use, we can use ANNOTATION_TYPE and ANNOTATION_LEVEL.
	// It may also be useful to be more specific, such as
	// SWEEP_ANNOTATION_TYPE and SWEEP_ANNOTATION_LEVEL.
	//
	// in the second form the tester identifys the type of entity to look for
	// The is_XXXX functions generated by the ENTITY_DEF macro work well.
/**
 * Function for finding annotations.
 * <br><br>
 * <b>Role:</b> The first two arguments specify a branch of the entity derivation hierarchy
 * to return bulletins for. For annotation use, we can use <tt>ANNOTATION_TYPE</tt> and
 * <tt>ANNOTATION_LEVEL</tt>. It may also be useful to be more specific, such as <tt>SWEEP_ANNOTATION_TYPE</tt>
 * and <tt>SWEEP_ANNOTATION_LEVEL</tt>. The <tt>is_XXXX</tt> functions generated by the <tt>ENTITY_DEF</tt> macro work well.
 * <br><br>
 * @param type
 * entity type.
 * <br><br>
 * @param level
 * entity level.
 * <br><br>
 * @param blist
 * bulletin list.
 */
 	void find_bulletins(int type, int level, BULLETIN_LIST& blist) const;
/**
 * Function for finding annotations.
 * <br><br>
 * <b>Role:</b> The first two arguments specify a branch of the entity derivation hierarchy
 * to return bulletins for. In this form the tester identifies the type of entity to look for.
 * For annotation use, we can use <tt>ANNOTATION_TYPE</tt> and <tt>ANNOTATION_LEVEL</tt>. It may also be useful
 * to be more specific, such as <tt>SWEEP_ANNOTATION_TYPE</tt> and <tt>SWEEP_ANNOTATION_LEVEL</tt>. The <tt>is_XXXX</tt>
 * functions generated by the <tt>ENTITY_DEF</tt> macro work well.
 * <br><br>
 * @param tester
 * testing function.
 * <br><br>
 * @param blist
 * bulletin list.
 */
 	void find_bulletins(spa_is_function tester, BULLETIN_LIST& blist) const;
/**
 * Merges next bulletin into roll back history.
 * <br><br>
 * @param rollback_set
 * on/off indicator.
 */
     logical	merge_next(logical rollback_set);

	// How much space taken by this bulletin_board
	// This includes all the history stream structure
	// and possibly the backup entities, but not the
	// active entities.
/**
 * Returns the size of the bulletin board.
 * <br><br>
 * @param include_backups
 * include backups as part of the size.
 */
 	int size(logical include_backups = TRUE) const;

    // STI ROLL begin:

    // Clears reference to this history from entities in bulletin board
/**
 * Clears history pointers.
 */
     void clear_history_ptrs();

    // makes sure history pointers are reset when the bb is lost
/**
 * C++ destructor, resets the history stream on deletion.
 */
     void reset_history_on_delete() { reset_history = TRUE; }

    // Returns TRUE if the bulletin board's history is not the same as the
    // history in entities on the bulletin board
/**
 * Returns <tt>TRUE</tt> if the bulletin board's history is not the same as the history in entities on the bulletin board.
 * <br><br>
 * @param alternative_hs
 * alternate stream.
 * <br><br>
 * @param move_fixes
 * move fixes or not.
 * <br><br>
 * @param remove_bulls
 * remove bulletins or not.
 */
     logical mixed_streams(HISTORY_STREAM*& alternative_hs,
                          logical& move_fixes = *(logical *)NULL_REF,
                          logical remove_bulls = FALSE);

	// STI ROLL BB merge Check and set methods
/**
 * Returns whether or not a bulleting board merge is pending.
 */
 	logical pending() const { return merge_pending; }
/**
 * Returns whether or not rollbacks have been cleared.
 */
 	logical rollbacks_cleared() const { return rollbacks_severed; }
/**
 * Sets the pending value.
 * <br><br>
 * @param pending_value
 * value.
 */
 	void set_pending(logical pending_value);
/**
 * Merge method, Sets whether or not rollbacks are cleared on merge.
 * <br><br>
 * @param severed
 * cleared.
 */
 	void set_rollbacks_cleared(logical severed);

    // methods to get and set the history stream that the bulletin board needs to be
/**
 * Sets the history stream that the bulletin board needs to be in.
  * <br><br>
 * @param ahs
 * alternate stream.
 */
     void set_alternate_stream(HISTORY_STREAM *ahs) { alt_hs = ahs; }
/**
 * Gets the history stream that the bulletin board needs to be in.
 */
     HISTORY_STREAM *get_alternate_stream() const { return alt_hs; }

    // methods to query and manipulate the dead entities list
/**
 * Adds this to the dead entities list.
 * <br><br>
 * @param ent
 * entity.
 */
     int add_dead_entity(ENTITY *ent) { return dead_entities.add(ent); }
/**
 * Removes this dead entity.
 * <br><br>
 * @param ent
 * entity.
 */
     int remove_dead_entity(ENTITY *ent) { return dead_entities.remove(ent); }
/**
 * Returns <tt>TRUE</tt> if this is a dead entity.
 * <br><br>
 * @param ent
 * entity.
 */
     logical is_dead_entity(ENTITY *ent) { return (dead_entities.lookup(ent) > -1); }
/**
 * Clears the dead entity list.
 */
     void clear_dead_entity_list() { dead_entities.clear(); }
    // STI ROLL end

	// STI ROLL
/**
 * @nodoc
 */
 	virtual void full_size(SizeAccumulator&, logical = TRUE) const;
	// STI ROLL

private:

	// Private copy constructor and operator =, to ensure that
	// there can be no copying of these objects.  These private
	// methods will not have implementations, so the language
	// will guarantee (via linker errors!) that no use is
	// ever made of them.
	BULLETIN_BOARD &operator = (const BULLETIN_BOARD &bb);
	BULLETIN_BOARD(const BULLETIN_BOARD &bb);
};

// Bulletins are the records that are chained into bulletin-boards, in
// a doubly-linked list.
/**
 * Describes a record of the changes to a single <tt>ENTITY</tt> during the current operation on the model.
 * <br>
 * <b>Role:</b> A bulletin has a type signifying the creation, change, or deletion
 * of a model entity. The type is not stored, but deduced from the presence or absence
 * of new and old entity pointers. Bulletins are chained into bulletin-boards, in a
 * doubly-linked list.
 * @see BULLETIN_BOARD, ENTITY
 */

class DECL_KERN BULLETIN : public ACIS_OBJECT {
	ENTITY *old_ptr;		// the model as of creation of this bulletin
	ENTITY *new_ptr;		// the current active state of the model
public:
/**
 * list pointer.
 */
 	BULLETIN *next_ptr; 		// list pointer
/**
 * list pointer.
 */
 	BULLETIN *previous_ptr;		// list pointer
/**
 * pointer to the owner of this bulletin.
 */
 	BULLETIN_BOARD* owner_ptr;
/**
 * next pointer.
 */
 	BULLETIN *next_bb_b_ptr;	// pointer to bulletin for entity on
								// next bulletin board

 /**
  * Swaps one entity for another.
  * <br><br>
  * @param this_ent.
  * this entity
  * <br><br>
  * @param that_ent
  * that entity.
  */
    void swap(ENTITY* this_ent, ENTITY* that_ent);

 public:
/**
 * Saves the entities corresponding to the bulletin.
 * <br><br>
 * @param elist
 * entity list.
 * <br><br>
 * @param ignore_string_version
 * ingore version or not.
 */
 	logical save( ENTITY_LIST& elist, logical ignore_string_version = FALSE);
/**
 * Modifies the bulletin such that the new and old <tt>ENTITY</tt> pointers reflect the change of state.
 * <br><br>
 * @param previous_b
 * previous bull. board.
 * <br><br>
 * @param ignore_string_version
 * ingore version or not.
 */
 	logical restore(BULLETIN* previous_b, logical ignore_string_version = FALSE);
/**
 * The <tt>fix_pointers</tt> method for each entity in the restore array is called, with the array as argument.
 * <br><br>
 * <b>Role:</b> This calls <tt>fix_common</tt>, which calls its parent's <tt>fix_common</tt>, and then corrects
 * any pointers in the derived class. In practice there is never anything special for <tt>fix_pointers</tt>
 * to do, but it is retained for consistency and compatibility. (Supplied by the <tt>ENTITY_FUNCTIONS</tt>
 * and <tt>UTILITY_DEF</tt> macros.)
 * <br><br>
 * @param elist
 * pointers to fix.
 * <br><br>
 * @param owner
 * owner.
 */
 	logical fix_pointers(ENTITY_ARRAY& elist, BULLETIN_BOARD* owner);
/**
 * The <tt>fix_pointers</tt> method for each entity in the restore array is called, with the array as argument.
 * <br><br>
 * <b>Role:</b> This calls <tt>fix_common</tt>, which calls its parent's <tt>fix_common</tt>, and then corrects
 * any pointers in the derived class. In practice there is never anything special for <tt>fix_pointers</tt>
 * to do, but it is retained for consistency and compatibility. (Supplied by the <tt>ENTITY_FUNCTIONS</tt>
 * and <tt>UTILITY_DEF</tt> macros.)
 * <br><br>
 * @param elist
 * pointers to fix.
 * <br><br>
 * @param owner
 * owner.
 */
 	logical fix_pointers(ENTITY *elist[], BULLETIN_BOARD* owner);

	// Create a bulletin for the given old and new entities
	// respectively, and add it to the current bulletin-board (which
	// must already exist).
/**
 * C++ initialize constructor requests memory for this object and populates it with the data supplied as arguments.
 * <br><br>
 * <b>Role:</b> Applications should call this constructor only with the overloaded
 * <tt>new</tt> operator, because this reserves the memory on the heap, a requirement to
 * support roll back and history management.
 * <br> Creates a bulletin for the given old and new entities, and adds it to the current
 * bulletin-board (which must already exist).
 * <br><br>
 * @param old_ent
 * old entity.
 * <br><br>
 * @param new_ent
 * new entity.
 */
	BULLETIN( ENTITY *old_ent, ENTITY *new_ent );

	// This constructor is NOT for use by application developers.
	// It is for internal use only.
/**
 * C++ constructor.
 */
 	BULLETIN();

	// Bulletin destructor - does NOT update pointers in bulletin_board
	// so calling routine must be sure to do so.
/**
 * C++ destructor, deleting a BULLETIN.
 */
 	~BULLETIN();

	// STI ROLL  BB merge  Set the corresponding bulletin in the next BB
/**
 * Sets the corresponding bulletin in the next bulletin board.
 * <br><br>
 * @param b
 * bulletin.
 */
 	void set_next_bb_b( BULLETIN *b);

	// Give read-only access to private data
/**
 * Returns the pointer to the next bulletin on the bulletin board.
 */
 	BULLETIN *next() const { return next_ptr; }
/**
 * Returns the pointer to the previous bulletin on the bulletin board.
 */
 	BULLETIN *previous() const { return previous_ptr; }
/**
 * Returns the pointer to the old entity.
 */
 	ENTITY *old_entity_ptr() const { return old_ptr; }
/**
 * Returns a pointer to the new entity created after an operation on the model.
 */
      ENTITY *new_entity_ptr() const { return new_ptr; }
/**
 * Nullifies the new entity pointer.
 */
     void null_old_entity_ptr();
/**
 * Nullifies the old entity pointer.
 */
     void null_new_entity_ptr();
/**
 * Sets the entity pointers.
 * <br><br>
 * @param old_ent
 * old entity.
 * <br><br>
 * @param new_ent
 * new entity.
 */
     void set_entity_ptrs(ENTITY *old_ent, ENTITY *new_ent);
/**
 * Returns a pointer to the current entity.
 */
 	ENTITY *entity_ptr() const { return new_ptr ? new_ptr : old_ptr; }
/**
 * Returns the owner of the entity corresponding to the bulletin.
 */
 	BULLETIN_BOARD* owner() const { return owner_ptr; }
/**
 * Bulletin for an entity on the next bulletin board.
 */
 	BULLETIN *next_bb_b() const { return next_bb_b_ptr; }
/**
 * Returns the type of <tt>BULLETIN</tt>.
 * <br><br>
 * <b>Role:</b> Four types of bulletins are defined: <tt>NO_BULLETIN</tt>, <tt>CREATE_BULLETIN</tt>,
 * <tt>CHANGE_BULLETIN</tt>, and <tt>DELETE_BULLETIN</tt>.
 */
 	BULLETIN_TYPE type() const;	// find type of bulletin
/**
 * Concatenates a change (or create) operation and a delete bulletin on the same <tt>ENTITY</tt> on the same bulletin board.
 */
 	void make_delete();			// convert a change into a delete
/**
 * Modifies the bulletin such that the new and old <tt>ENTITY</tt> pointers reflect the change of state.
 */
     void roll();				// apply this bulletin to roll back/
								// forward the referenced entity
/**
 * Outputs debug information about <tt>BULLETIN</tt> to standard output or to the specified file.
 * <br><br>
 * @param fp
 * file pointer
 */
 	void debug( FILE *fp = debug_file_ptr ) const;
/**
 * Writes information about the <tt>BULLETIN</tt> to the debug file or to the specified file.
 * <br><br>
 * <b>Role:</b> The first two arguments specify a branch of the entity derivation hierarchy
 * to call <tt>debug_ent</tt> on, in addition to the normal bulletin debugging stuff.
 * <br><br>
 * @param id
 * entity id
 * <br><br>
 * @param level
 * entity level
 * <br><br>
 * @param fp
 * file pointer
 */
 	void debug( int id, int level, FILE *fp = debug_file_ptr ) const;

	// How much space taken by this bulletin
	// This includes all the history stream structure
	// and possibly the backup entities, but not the
	// active entities.
/**
 * Returns the size of the <tt>BULLETIN</tt>.
 * <br><br>
 * @param include_backups
 * include backups as part of size.
 */
 	int size(logical include_backups = TRUE) const;

    // STI ROLL begin: clear reference to the history from entity in bulletin
/**
 * Clears the history stream.
 */
     void clear_history();
/**
 * Sets the current history stream.
 * <br><br>
 * @param hist
 * history stream.
 */
     void set_history (HISTORY_STREAM* hist) const;
    // STI ROLL end

    // STI ROLL begin:
    // Gets history from either bulletin board or entities
/**
 * Gets history from either bulletin board or entities.
 * <br><br>
 * @param from_ents
 * from entities or not.
 */
     HISTORY_STREAM* history_stream(logical from_ents = FALSE) const;

    // Returns TRUE when the entities history, ent_hs, does not
    // match the bulletin_board's history, bb_hs.  The entities
    // history is returned.  The bulletin board's history can
    // either be supplied (for performance) or figured out.
 /**
  * Returns <tt>TRUE</tt> when the entity's history, <tt>ent_hs</tt>, does not match the bulletin board's history, <tt>bb_hs</tt>.
  * <br><br>
  * <b>Role:</b> The entity's history is returned. The bulletin board's history can either be
  * supplied (for performance) or figured out.
  * <br><br>
  * @param ent_hs
  * entity history.
  * <br><br>
  * @param can_be_fixed
  * fixable or not.
  * <br><br>
  * @param stream_corrupt
  * corrupt or not.
  * <br><br>
  * @param bb_hs
  * bulletin board history.
  */
      logical mixed_streams(HISTORY_STREAM*& ent_hs,
                          logical& can_be_fixed,
                          logical& stream_corrupt,
                          HISTORY_STREAM* bb_hs = NULL) const;
    // STI ROLL end
/**
 * Returns whether or not there has been a change to only the attribute.
 */
 	logical attrib_only_change() const; // STI ROLL
 /**
  * Returns whether or not there has been a change.
  */
     logical no_change() const; // STI ROLL

private:
	// Private copy constructor and operator =, to ensure that
	// there can be no copying of these objects.  These private
	// methods will not have implementations, so the language
	// will guarantee (via linker errors!) that no use is
	// ever made of them.
	BULLETIN &operator = (const BULLETIN &bb);
	BULLETIN(const BULLETIN &bb);
};


// There are some applications which wish to manipulate bulletin-boards, and
// the following definitions provide a (rather crude) means by which they
// can access them after each API function which succeeds.

// Function pointer, set by application to point to a function which is called
// after each successful top-level API routine, to inspect and possibly modify
// the associated bulletin-board.

/**
 * @nodoc
 */
typedef void (*update_func_fn)();

#ifdef THREAD_SAFE_ACIS

/**
 * @nodoc
 */
extern DECL_KERN safe_function_type< update_func_fn > update_func;

#else
/**
 * @nodoc
 */
extern DECL_KERN update_func_fn update_func;

#endif

#include "tlkit_cb.hxx"
/**
 * @nodoc
 */
class DECL_KERN bb_close_callback : public toolkit_callback {
public:
	// Called just before a BULLETIN_BOARD is closed
	virtual void Closing_Bulletin_Board(BULLETIN_BOARD* bb);
};
/**
 * @nodoc
 */
class DECL_KERN bb_close_callback_list : public toolkit_callback_list {
public:
	void Closing_Bulletin_Board(BULLETIN_BOARD* bb);

	// Add callbacks to the beginning of the list
	void add(bb_close_callback* cb) { toolkit_callback_list::add(cb); }

	// Add callbacks to the end of the list
	void append(bb_close_callback* cb) { toolkit_callback_list::append(cb); }
};

/**
 * @nodoc
 */
DECL_KERN bb_close_callback_list& get_bb_close_callback_list();
// Function used by update functions (and possibly elsewhere) to obtain access
// to the current bulletin-board.
/**
* Gets access to the current bulletin-board for update functions.
* <br><br>
* <b>Effect:</b> System routine
**/

DECL_KERN BULLETIN_BOARD *current_bb();

/**
* Retrieves a default history stream, which may be necessary when initially creating a history of actions.
* <br><br>
* <b>Role:</b> If <tt>make_if_null</tt> is set to <tt>TRUE</tt>, this function will create a new
* history stream if one is not found.
* <br><br>
* <b>Effect:</b> Read-only
* <br><br>
* @param make_if_null
* make new one.
**/

DECL_KERN HISTORY_STREAM* get_default_stream(logical make_if_null = TRUE);

/**
 * @nodoc
 */
DECL_KERN void set_default_stream(HISTORY_STREAM*);

// Jeff 09.11.06 NESTED_HISTORY
/**
 * Temporarily activates the input @href HISTORY_STREAM.
 * <br><br>
 * <b>Role:</b> This function, along with @href pop_default_stream, provides the ability to 
 * temporarily activate alternate history streams within an active operation. 
 * This may be useful for operations performed on multiple models in different streams 
 * because entity modifications can be logged appropriately. Changing the default working 
 * history stream has previously only been possible as a top-level operation. 
 * This forced the application developer to deal with history distribution issues when 
 * operations modified entities in multiple streams. This new functionality can be used to avoid mixing streams.
 * <br><br>
 * The currently active default working stream is given a suspended status and is the return value of the call. 
 * The input stream is set as the default working stream and is given a nested status. 
 * It is possible to push to a previously suspended stream because entity changes are considered unique 
 * to and are logged at a particular nesting level. The first stream pushed "from" is the top-level stream 
 * and has level 0, the first stream pushed "to" is given level 1, subsequent pushes increment the level. 
 * <br><br>
 * The top-level state knows which streams were modified at the end of an operation and therefore 
 * has the ability to undo all nested changes, even across multiple history streams. 
 * This capability provides analogous behavior of normal API operations, 
 * in that all model changes can be undone in the event of an error. This information, however, 
 * is only available until the top-level state is processed, which typically occurs when the state is noted 
 * or the next API operation begins.
 * <br><br>
 * Entity changes of nested history operations are handled much like changes in 
 * @href API_TRIAL_BEGIN and @href API_TRIAL_END blocks are, 
 * in that the changes are eventually merged into parent bulletin boards. 
 * These merges occur when a stream is reactivated by popping back into it. 
 * Entity changes made at higher nesting levels are merged when popping into lower levels. 
 * This is done to have at most one nested bulletin board per history stream at the end of an operation.
 * <br><br>
 * The application developer must ensure that pushed streams are 
 * eventually popped and that the inputs to the functions are correct. In other words, the application developer 
 * keeps track of the streams and supply them accordingly. Also, this functionality intends to 
 * support temporary activations of alternate streams. Spatial does not recommend its use for prolonged operations 
 * because the required extra history processing may degrade performance.
 * <br><br>
 * This new capability does not remove the restrictions of when history management functions, 
 * such as @href api_note_state and @href api_set_default_history, can be called. 
 * These functions may only be called at upper levels.
 * <br><br>
 * <b>Effect:</b> Changes history
 * <br><br>
 * @param to_stream
 * The stream to activate.
 * @see pop_default_stream, api_set_default_history, api_distribute_state_to_streams
**/
DECL_KERN HISTORY_STREAM* push_default_stream(HISTORY_STREAM* to_stream);

/**
 * Reactivates the previously suspended input @href HISTORY_STREAM.
 * <br><br>
 * <b>Role:</b>This function reactivates the input history stream, which was suspended in an 
 * earlier call to @href push_default_stream. The input stream, in other words, is set as the default 
 * working stream. Significant history processing, mainly bulletin board merges, may take place 
 * on the stream that is being deactivated. Entity modifications from higher history nesting levels 
 * are processed during this call. This stream, the one being deactivated, is the return value. 
 * Refer to @href push_default_stream for more information.
 * <br><br>
 * <b>Effect:</b> Changes history
 * <br><br>
 * @param to_stream
 * The stream to reactivate.
 * @see push_default_stream
**/
DECL_KERN HISTORY_STREAM* pop_default_stream(HISTORY_STREAM* to_stream);

/**
 * @nodoc
 */
DECL_KERN HISTORY_STREAM* get_stream_root();

#ifdef THREAD_SAFE_ACIS
/**
 * Merge bulletins from worker thread stream into master history stream.
 * <br><br>
 * <b>Role:</b> This function merges bulletins from the given <tt>child_stream</tt> into
 * its parent stream. It will merge bulletins from either an open and active delta state, or 
 * a closed and noted delta state. The intended purpose of this function is to merge bulletins
 * from a worker thread's history stream into the main history stream in a multi-threaded 
 * application. 
 * <br><br>
 * <b>Effect:</b> Changes history
 * <br><br>
 * @param child_stream
 * The stream to merge into the master stream.
 */
DECL_KERN outcome merge_child_state( HISTORY_STREAM* child_stream );
#endif

/** @} */

#endif

