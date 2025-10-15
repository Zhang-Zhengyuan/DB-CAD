/* ORIGINAL: 3dt2.1/pmhusk/roll_utl.hxx */
// $Id: roll_utl.hxx,v 1.10 2000/12/26 18:55:18 products Exp $
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
#if 1
#include "roll_hm.hxx"
#else
#ifndef roll_utl_hxx
#define roll_utl_hxx
// #define HM_HUSK
// #ifdef HM_HUSK
#include "dcl_part.h"
#include "api.hxx"
#include "roll_hm.hxx"
// #else
class ENTITY;
#include "logical.h"
class DELTA_STATE_LIST;

//======================================================================

// restore_delta_state_links(
// 	DELTA_STATE_LIST& dslist);
// 
// save_delta_state_links(
// 	DELTA_STATE_LIST& dslist
// 	);

// We add _nd for the non distributed versions of the roll routines
	
DECL_PART int start_delta_state_nd();
DECL_PART int note_delta_state_nd(outcome);
DECL_PART void name_current_state_nd( const char* name );
DECL_PART int roll_n_states_nd(int nstates);
DECL_PART int roll_to_named_state_nd(const char* name);

// Delete all delta states.  Returns the number of states deleted
DECL_PART int delete_all_states_nd();

// Delete all delta states before the current one.  You will not be
// able to roll back before the current state after this.
DECL_PART int delete_previous_states_nd();

// Delete all delta states after the current one.
DECL_PART int delete_following_states_nd();

// Set the maximum number of previous states to keep.  Returns the
// number of states deleted _nd(if any) to set this limit.
DECL_PART int set_number_of_states_to_keep_nd(int n);

// Determine if you can roll backwards or forwards
DECL_PART logical can_roll_back_nd();
DECL_PART logical can_roll_forward_nd();

DECL_PART void close_open_states_nd();
DECL_PART void roll_cleanup_nd();
// #endif

//======================================================================
#endif
#endif
