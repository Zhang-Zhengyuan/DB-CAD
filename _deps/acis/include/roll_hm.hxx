/* ORIGINAL: 3dt2.1/pmhusk/roll_hm.hxx */
// $Id: roll_hm.hxx,v 1.16 2001/05/02 17:34:02 sallen Exp $
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
//    Declare rollback utility functions
//
//----------------------------------------------------------------------

#ifndef roll_hm_hxx
#define roll_hm_hxx

#include "dcl_part.h"
#include "logical.h"
#include "api.hxx"

class ENTITY;
class DELTA_STATE_LIST;
class HISTORY_STREAM;
class DELTA_STATE;

//======================================================================

// restore_delta_state_links(
// 	DELTA_STATE_LIST& dslist);
// 
// save_delta_state_links(
// 	DELTA_STATE_LIST& dslist
// 	);
DECL_PART int get_delta_state_level();
DECL_PART int set_delta_state_level( int level);
DECL_PART int start_delta_state();
DECL_PART int note_delta_state(outcome);
DECL_PART void name_current_state( const char* name, HISTORY_STREAM* = NULL );
DECL_PART int roll_n_states(int nstates, HISTORY_STREAM* = NULL);
DECL_PART int roll_to_named_state(const char* name, HISTORY_STREAM*, int& n_actual);

// Delete all delta states.  Returns the number of states deleted
DECL_PART int delete_all_states(HISTORY_STREAM* = NULL);

// Delete all delta states before the current one.  You will not be
// able to roll back before the current state after this.
DECL_PART int delete_previous_states(HISTORY_STREAM* = NULL);

// Delete all delta states after the current one.
DECL_PART int delete_following_states(HISTORY_STREAM* = NULL);

// Delete all delta states not in the active path
DECL_PART int delete_inactive_states(HISTORY_STREAM* = NULL);

// Set the maximum number of previous states to keep.  Returns the
// number of states deleted (if any) to set this limit.
DECL_PART int set_number_of_states_to_keep(int n, HISTORY_STREAM* = NULL);

// Determine if you can roll backwards or forwards
DECL_PART logical can_roll_back(HISTORY_STREAM* = NULL);
DECL_PART logical can_roll_forward(HISTORY_STREAM* = NULL);

DECL_PART void close_open_states();
DECL_PART void roll_cleanup();
DECL_PART logical distributed_history(logical assign_if_unset = TRUE, logical value = FALSE);

DECL_PART logical make_root_ds( DELTA_STATE* ds );

DECL_PART void list_named_states(const char* namePattern, 
					   HISTORY_STREAM* hs,
					   DELTA_STATE_LIST& dsList
					   );
DECL_PART void note_delta_state_regardless_of_level(outcome status,	logical hide);

// Returns the first found of the input, 
// the stream of the active part or the default stream.
DECL_PART HISTORY_STREAM* get_stream(HISTORY_STREAM* hs);

//======================================================================
#endif
