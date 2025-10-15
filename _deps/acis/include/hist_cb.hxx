/*******************************************************************/
/*    Copyright (c) 1989-2020 by Spatial Corp.                     */
/*    All rights reserved.                                         */
/*    Protected by U.S. Patents 5,257,205; 5,351,196; 6,369,815;   */
/*                              5,982,378; 6,462,738; 6,941,251    */
/*    Protected by European Patents 0503642; 69220263.3            */
/*    Protected by Hong Kong Patent 1008101A                       */
/*******************************************************************/
//-----------------------------------------------------------------------
// Purpose---
//	Callbacks to let people know what is happening during roll.
//-----------------------------------------------------------------------

#ifndef __HISTCB_HXX__
#define __HISTCB_HXX__

#include "dcl_kern.h"
#include "logical.h"
#include "tlkit_cb.hxx"

/**
* @file hist_cb.hxx
 * @CAA2Level L1
 * @CAA2Usage U1
 * \addtogroup ACISHISTORY
 *
 * @{
 */

class DELTA_STATE;
class BULLETIN_BOARD;
class BULLETIN;

/**
 * Provides callbacks for history management.
 */
class DECL_KERN history_callbacks : public toolkit_callback
{
public:

/**
 * Callback method, called before rolling all states.
 */
	virtual void Before_Roll_States();

/**
 * Callback method, called before rolling one state.
 * <br><br>
 * @param d_state
 * delta state.
 */
	virtual void Before_Roll_State(DELTA_STATE* d_state);

/**
 * Callback method, called before rolling a bulletin board.
 * <br><br>
 * <b>Role:</b> If discard is <tt>TRUE</tt>, the roll is due to error processing, and the <tt>BULLETIN_BOARD</tt> will
 * be deleted along with all of its <tt>BULLETINs</tt>.
 * <br><br>
 * @param b_board
 * bulletin board.
 * @param discard
 * discard.
 */
	virtual void Before_Roll_Bulletin_Board(BULLETIN_BOARD* b_board, logical discard);

/**
 * Callback method, called after rolling a bulletin board.
 * <br><br>
 * <b>Role:</b> If discard is <tt>TRUE</tt>, the roll is due to error processing, and the <tt>BULLETIN</tt>
 * <br><br>
 * @param b_board
 * bulletin board.
 * @param discard
 * discard.
 */
	virtual void After_Roll_Bulletin_Board(BULLETIN_BOARD* b_board, logical discard);

/**
 * Callback method, called after rolling one state.
 * <br><br>
 * @param d_state
 * delta state.
 */
	virtual void After_Roll_State(DELTA_STATE* d_state);

/**
 * Callback method, called after rolling all states.
 */
	virtual void After_Roll_States();
/**
 * Callback method, called in delete bb.
 */
	virtual void auto_delete_flg_on();

/**
 * Callback method, called in delete bb.
 */
	virtual logical m_last_bb();
};

/**
 * Provides a list of callbacks for history.
 */

class DECL_KERN history_callbacks_list : public toolkit_callback_list
{
public:

/**
 * Adds a callback to the list.
 * <br><br>
 * @param cb
 * callback.
 */
	void add(history_callbacks* cb) { toolkit_callback_list::add(cb); }

/**
 * Appends a history callback to the callback list.
 * <br><br>
 * @param cb
 * callback.
 */
	void append(history_callbacks* cb) { toolkit_callback_list::append(cb); }

/**
 * Callback method, called before rolling all states.
 */
	virtual void Before_Roll_States();

/**
 * Callback method, called before rolling one state.
 * <br><br>
 * @param d_state
 * delta state.
 */
	virtual void Before_Roll_State(DELTA_STATE* d_state);


/**
 * Callback method, called before rolling a bulletin board.
 * <br><br>
 * <b>Role:</b> If discard is <tt>TRUE</tt>, the roll is due to error processing, and the <tt>BULLETIN_BOARD</tt> will
 * be deleted along with all of its <tt>BULLETINs</tt>.
 * <br><br>
 * @param b_board
 * bulletin board.
 * @param discard
 * will be discarded.
 */
	virtual void Before_Roll_Bulletin_Board(BULLETIN_BOARD* b_board, logical discard);

/**
 * Callback method, called after rolling a bulletin board.
 * <br><br>
 * <b>Role:</b> If discard is <tt>TRUE</tt>, the roll is due to error processing, and the <tt>BULLETIN_BOARD</tt>
 * will be deleted along with all of its <tt>BULLETINs</tt>.
 * <br><br>
 * @param b_board
 * bulletin board.
 * @param discard
 * will be discarded.
 */
	virtual void After_Roll_Bulletin_Board(BULLETIN_BOARD* b_board, logical discard);

/**
 * Callback method, called after rolling one state.
 * <br><br>
 * @param d_state
 * delta state.
 */
	virtual void After_Roll_State(DELTA_STATE* d_state);

/**
 * Callback method, called after rolling all states.
 */
	virtual void After_Roll_States();

/**
 * Callback method, called in delete bb.
 */
	virtual void auto_delete_flg_on();

/**
 * Callback method, called in delete bb.
 */
	virtual logical m_last_bb();
};


/*
// tbrv
*/
/**
 * @nodoc
 */
DECL_KERN history_callbacks_list& get_history_callbacks_list();

#ifdef _HISTCB_EXAMPLE_AND_DOC

/*
// tbrv
*/
/**
 * @nodoc
 */
class my_history_callbacks : public history_callbacks
{
	// declare any data related to handling roll.
public:
	virtual void Before_Roll_States() {
		// Do your before roll stuff for a group of DELTA_STATEs
	}
	virtual void Before_Roll_State(DELTA_STATE*) {
		// Do your before roll stuff for one DELTA_STATE;
	}
	virtual void Before_Roll_Bulletin_Board(BULLETIN_BOARD*, logical discard) {
		// Do your before roll stuff for one BULLETIN_BOARD
		// if discard is true, the  roll is due to  error processing and the
		// BULLETIN_BOARD will be deleted along with all its BULLETINS.
	}
	virtual void After_Roll_Bulletin_Board(BULLETIN_BOARD*, logical discard) {
		// Do your after roll stuff for one BULLETIN_BOARD;
			// if discard is true, the  roll is due to  error processing and the
			// BULLETIN_BOARD will be deleted along with all its BULLETINS.
	}
	virtual void After_Roll_State(DELTA_STATE*) {
		// Do your after roll stuff for one DELTA_STATE;
	}
	virtual void After_Roll_States() {
		// Do your after roll stuff for a group of DELTA_STATES
	}
	virtual void auto_delete_flg_on() {
		//
	}
	virtual logical m_last_bb() {
		return FALSE;
		//
	}
};


/*
// tbrv
*/
/**
 * @nodoc
 */
my_code_initialize() {
	get_history_callbacks_list().add( ACIS_NEW my_history_callbacks);

}

The Before_Roll_States and After_Roll_States functions correspond to the existing
roll_callback::execute method, except they are called from the kernel instead of
from the part manager.  This means they are available to more users.

The other methods have no existing counterpart.
They give the user information previously unavailable.

The before and after methods are always called in pairs.

#endif //_HISTCB_EXAMPLE_AND_DOC

/** @} */
#endif //__HISTCB_HXX__
