/*******************************************************************/
/*    Copyright (c) 1989-2020 by Spatial Corp.                     */
/*    All rights reserved.                                         */
/*    Protected by U.S. Patents 5,257,205; 5,351,196; 6,369,815;   */
/*                              5,982,378; 6,462,738; 6,941,251    */
/*    Protected by European Patents 0503642; 69220263.3            */
/*    Protected by Hong Kong Patent 1008101A                       */
/*******************************************************************/
//------------------------------------------------------------------------------
#ifndef HISTORY_H
#define HISTORY_H

#include "dcl_kern.h"
#include "logical.h"

#include "bulletin.hxx"
#include "glists.hxx"
#include "api.hxx"
/**
 * \defgroup ACISHISTORY History
 * \ingroup KERNAPI
 *
 */
/**
 * @file history.hxx
 * @CAA2Level L1
 * @CAA2Usage U1
 * \addtogroup ACISHISTORY
 *
 * @{
 */

/// forward decl
class DELTA_STATE;
class HISTORY_STREAM;
class HistoryObject;
class ENTITY;
class ENTITY_LIST;
class BULLETIN;
class BULLETIN_BOARD;
class ENTITY_ARRAY;

// typedef logical (*AssocArrayEqFunction) (void*, void*);
// typedef void (*getHistoryEntitiesFunction) (ENTITY_LIST&);

class StreamFinder;

/**
 * Creates a history state on the specified history stream.
 * <br>
 * <b>Role:</b> Takes the bulletins in the current delta state and creates a history state on
 * the specified history stream. The current delta is left with nothing in it. Returns the newly
 * created history state. If the current delta state is empty (has no bulletins), <tt>NULL</tt> is returned.
 * When all is done, the current delta state is (optionally) cleared.
 * <br><br>
 * @see StreamFinder
 */
class DECL_KERN HISTORY_MANAGER : public ACIS_OBJECT {

friend class DELTA_STATE;
friend class HISTORY_STREAM;
friend class ATTRIB_HISTORY;
friend class StreamFinder;

public:

/**
 * Gets the number of bulletins in the given delta state
 * <br><br>
 * @param pState
 * delta state.
 */
    static int count_bulletins(DELTA_STATE* pState);

	// take the bulletins in the current delta state and create a
	// history state on the specified history stream. The current
	// delta is left with nothing in it.  Returns the newly created
	// history state. If the current delta state is empty (has no
	// bulletins), NULL is returned.  When all is done, the current
	// delta state is (optionally) cleared
/**
 * Takes the bulletins in the current delta state and creates a history state on the specified history stream.
 * <br><br>
 * <b>Role:</b> The current delta is left with nothing in it. Returns the newly created history state.
 * If the current delta state is empty (has no bulletins), <tt>NULL</tt> is returned. When all is done,
 * the current delta state is (optionally) cleared.
 * <br><br>
 * @param h_stream
 * history stream.
 * @param clearDeltaState
 * clear status.
 */
   static DELTA_STATE* acquireCurrentDelta(
		HISTORY_STREAM* h_stream,
		logical clearDeltaState = TRUE);

	// take the bulletins in the current delta state and "distribute"
	// them onto history streams based on their "owning entities".
	// Bulletins that do not belong to any entity that has a history
	// stream attached to it are simple left in the current delta
	// state.  When all is done, the current delta state is
	// (optionally) cleared
/**
 * Takes the bulletins in the current delta state and "distributes" them onto history streams based on their "owning entities".
 * <br><br>
 * <b>Role:</b> Bulletins that do not belong to any entity that has a history stream attached to it
 * are simple left in the current delta state. When all is done, the current delta state is
 * (optionally) cleared.
 * <br><br>
 * @param pState
 * delta state.
 * @param pStreamFinder
 * stream finder.
 * @param clearDeltaState
 * stream finder.
 * @param hideStates
 * hide delta state.
 */
	static outcome distributeDeltaState(
		DELTA_STATE*  pState,
		StreamFinder* pStreamFinder,
		logical       clearDeltaState = TRUE,
		logical       hideStates = FALSE);

	// Run some advance checks on a DELTA_STATE to make sure it is OK to
	// distribute it.  Thus, problems can be detected before changing
	// any of the data structure.
/**
 * Performs advance checks on a <tt>DELTA_STATE</tt> to make sure it is OK to distribute it and problems can be detected before changing any of the data structure.
 * <br><br>
 * @param pState
 * delta state.
 * @param pStreamFinder
 * finds stream.
 */
	static outcome checkDeltaForDistribute(
		DELTA_STATE*   pState,
		StreamFinder*  pStreamFinder);


	// discard all the bulletins in the current delta state and clear it.
/**
 * Discards all the bulletins in the current delta state and clears it.
 */
	static void clearCurrentDelta();

	// dump the BULLETIN_BOARD(s) in the current delta
/**
 * Dumps bulletin board into current delta.
 */
	static void debugCurrentDelta();

	// get the history stream attached to this entity, if any provided
	// here to hide details of the connecting attribute
/**
 * Gets the history stream attached to this entity, if any provided here to hide details of the connecting attribute.
 * <br><br>
 * @param ent
 * entity.
 */
	static HISTORY_STREAM* getAttachedStream(ENTITY* ent);

	// detach the given entity from any history stream it may be
	// attached to.  strips the entity of the connecting attribute.

/**
 * Detaches the given entity from any history stream it may be attached to. Strips the entity of the connecting attribute.
 * <br><br>
 * @param ent
 * entity.
 */
	static void detach(ENTITY* ent);


/**
 * Saves the history stream and associated entities.
 * <br><br>
 * @param l_stream
 * history stream list.
 * @param ent_list
 * entity list.
 * @param activeOnly
 * active branch only if TRUE.
 */
	static logical save(
		HISTORY_STREAM* l_stream,
		ENTITY_LIST& ent_list,
		logical activeOnly);

/**
 * Restores a history stream that was saved.
 * <br><br>
 * @param h_stream
 * history stream.
 * @param ent
 * entity.
 */
	static logical restore(
		HISTORY_STREAM*& h_stream,
		ENTITY** ent);

/**
 * Sets the given <tt>BULLETIN</tt> to point to the given <tt>ENTITY</tt>, and sets its next and previous pointers to <tt>NULL</tt>.
 * <br><br>
 * @param b
 * bulletin.
 * @param n
 * entity.
 */
	static void setNewBulletin(BULLETIN* b, ENTITY* n) {
		b->set_entity_ptrs(NULL, n);
		b->previous_ptr = NULL;
	}

 private:
	// A recursive function that finds the current state and rolls
	// intervening states until it is OK to roll the given state.
	static void makeLegal(
		DELTA_STATE* pTarget,  // The state you want make legal to roll
		int&         statesChanged
	);

	// Used by makeLegal
	static logical isLegal(
		DELTA_STATE* pTarget  // The state to test.
	);

 public:
	// In this one the state knows which stream it is in, so it
	// does not need to be passed.

/**
 * The state knows which stream it is in, so it does not need to be passed.
 * <br><br>
 * @param pStream
 * source stream.
 * @param pTarget
 * target.
 * @param statesChanged
 * counter for rolling states.
 */
	static void changeToState(
		HISTORY_STREAM* pStream,
		DELTA_STATE*    pTarget,  // The state you want to change to
		int&            statesChanged
	);

	// Roll a stream a given number of states or to the end of a branch,
	// which ever comes first.  Returns the number of states actually rolled


/**
 * Rolls a stream a given number of states or to the end of a branch, which ever comes first and returns the number of states actually rolled.
 * <br><br>
 * @param pStream
 * history stream.
 * @param nstates
 * number of states.
 */
	static int rollNStates(
		HISTORY_STREAM* pStream, // The stream you want to roll
		int            nstates  // How many states to roll, positive for forward
	);

	// Test for the existance of any BULLETINs in the given state

/**
 * Tests for the existence of any <tt>BULLETINs</tt> in the given state.
 * <br><br>
 * @param pState
 * delta_state.
 */
	static logical isStateEmpty(
		DELTA_STATE* pState);

/**
 * Makes the given <tt>pState</tt> the root delta state of the history stream it is a part of.
 * <br><br>
 * <b>Role:</b> States prior to the given <tt>pState</tt> are deleted. The effect is that one cannot roll
 * back over the changes in that delta state. It is useful when initializing the system to prevent
 * rolling back over the initialization. The toolkit uses it to prevent rolling back over
 * <tt>api_initialize_faceter</tt>.
 * <br><br>
 * @param pState
 * given pState.
 */
	static logical makeRootDS(
		DELTA_STATE* pState);


 private:
	// Find the last BULLETIN_BOARD in the given state
	static BULLETIN_BOARD* lastBB(
		DELTA_STATE* pState
		);

	// get the next bulletin in the given bulletin board chain and set
	// the bulletin board in which it lives in pBB.
	static BULLETIN* nextBulletin(
		BULLETIN* pB,
		BULLETIN_BOARD*& pBB);

	// get the first bulletin in the given delta state and set the
	// bulletin board in which it lives in pBB.
	static BULLETIN* firstBulletin(
		DELTA_STATE* pState,
		BULLETIN_BOARD*& pBB);

	// detach the given bulletin from the given bulletin board
	static void detachBulletin(
		BULLETIN* b,
		BULLETIN_BOARD* bb);

	static void addBToState(
		BULLETIN* b,
		DELTA_STATE* pBB);

	static logical isGeometryOwnerInBB(
		BULLETIN_BOARD* pBB,
		ENTITY* pGeometry);

	static ENTITY* findBulletinEntity(
		BULLETIN*);

	static logical fix_pointers(
		HISTORY_STREAM*, ENTITY**);

	static ENTITY* findOwningEntity(
		ENTITY* ent
		);

	static logical entityCreatedInStream(
		HISTORY_STREAM* pStream,
		ENTITY*         pEntity
		);

	static void initChecks(
		HISTORY_STREAM* pStream
		);

	static void finishChecks(
		HISTORY_STREAM* pStream
		);

 private:

};

#include "lists.hxx"

//LIST(HISTORY_STREAM,KERN);
/**
 * @nodoc
 */
LIST(ENTITY_LIST,KERN);

/*
// tbrv
*/
/**
 * @nodoc
 */
class DECL_KERN EntityStreamMap : public ACIS_OBJECT {
	HISTORY_STREAM_LIST	few;
	ENTITY_LIST_LIST	many;
public:
	void put(ENTITY* m, HISTORY_STREAM* f) {
		int i = few.lookup(f);
		if(i == -1) {
			i = few.add(f);
			int j = many.add( ACIS_NEW ENTITY_LIST);
			if(i != j) sys_error(12345);
		}
		if(i == -1) sys_error(12345);
		ENTITY_LIST* ents_for_i = many[i];
		ents_for_i->add(m);
	}
	HISTORY_STREAM* get(ENTITY* m) {
		for(int i = 0; i < few.count(); i++ ) {
			ENTITY_LIST* ents_for_i = many[i];
			if(ents_for_i->lookup(m) != -1) {
				return few[i];
			}
		}
		return NULL;
	}
	void clear() {
		for(int i = 0; i < few.count(); i++ ) {
			ENTITY_LIST* ents_for_i = many[i];
			ACIS_DELETE ents_for_i;
		}
		few.clear();
		many.clear();
	}
	EntityStreamMap() {}
	~EntityStreamMap() {
		clear();
	}
};



/**
 * Used by <tt>api_distribute_state_to_streams</tt> to find the <tt>HISTORY_STREAM</tt> corresponding to a given entity.
 * <br>
 * <b>Role:</b> This class is used by <tt>api_distribute_state_to_streams</tt> to find the <tt>HISTORY_STREAM</tt>
 * corresponding to a given entity. It gives a great deal of flexibility and supports a variety of
 * usage scenarios. At the same time the user needs to be careful during usage. This is a class,
 * instead of function pointers, hence it can cache data or arguments without using the global data
 * hack. 
 * <br><br> 
 * <tt>StreamFinder</tt> is an abstract base class with one pure virtual function,
 * <tt>findStream</tt>, which must return the <tt>HISTORY_STREAM</tt> associated with the
 * given <tt>ENTITY</tt>. The <tt>findStream</tt> function may be called more than once
 * for each <tt>ENTITY</tt>. In a topology based search, the <tt>StreamFinder</tt> can
 * cache data in an early pass that can be used in a later pass.
 * <br><br>
 * Examples of reasonable implementations are:
 *<ul>
 * <li>Find the top level owner and get the stream from an <tt>ATTRIB_HISTORY</tt> found there.</li>
 * <li>Find the <tt>PART</tt> for the <tt>ENTITY</tt> and return the stream associated with the <tt>PART</tt>.</li>
 * <li>Always return the same stream. 
 * This is useful when the application has some knowledge about what has transpired and knows all
 * <tt>BULLETINs</tt> go on the same stream.</li>
 * </ul>
 * <br><br>
 * For an example of the implementation and use of a <tt>StreamFinder</tt> refer to the 
 * <i>History Management</i> article.
 * @see HISTORY_MANAGER
 */
 class DECL_KERN StreamFinder : public ACIS_OBJECT {
 friend class HISTORY_MANAGER;
	EntityStreamMap mmEntityStreamMap;

 public:
	// This class is used by the DistributeStates function below to
	// find the HISTORY_STREAM corresponding to a given entity.  It
	// gives one a great deal of flexibility and supports a variety of
	// usage scenarios.  It gives one enough rope to hang oneself too,
	// so be carefull.

	// Note, that because StreamFinder is a class, instead of function
	// pointers, it can cache data or arguments without using the
	// global data hack.

	// Examples of reasonable implementations are:
	// .	Find the top level owner and get the stream from an
	// 		ATTRIB_HISTORY found there.
	//
	// .	Find the PART for the ENTITY and return the stream
	// 		associated with the PART.
	//
	// .	Always return the same stream.  This is useful when the
	// 		application has some knowledge about what has transpired
	// 		and knows all BULLETINs go on the same stream.

	/**
	 * @nodoc
	 */
	enum LookStrategy {
		LookNormal,
		LookReallyHard
	};

/**
 * Returns the <tt>HISTORY_STREAM</tt> for any entity.
 *<br><br>
 * @param ent
 * Entity Object.
 * @param strategy
 * Type of look strategy. Value may be either <tt>LookNormal</tt> or <tt>LookReallyHard</tt>.
 */
  virtual HISTORY_STREAM* findStream( ENTITY* ent, LookStrategy strategy = LookNormal) = 0;

	// STI ROLL begin: Function to return the history stream for any entity whose
	//                 stream is quickly determined.  This check should be very fast.
    //                 This was added so ID_ATTRIBs will not be moved into the wrong
    //                 part's stream - since parts are not entities, just checking
    //                 the streams for mixing is not good enough.

/**
 * Returns the <tt>HISTORY_STREAM</tt> for any entity whose stream needs to be quickly determined.
 *<br><br>
 * @param ent
 * Entity Object.
 */
  virtual HISTORY_STREAM* quick_findStream( ENTITY* ent )
  {
	  SPAUNUSED(ent)
	  return NULL;
  }

	// STI ROLL end

/**
 * C++ allocation constructor requests memory for this object.
 */
 	StreamFinder();

 protected:
	ENTITY* findOwningEntity(ENTITY* pEntity) {
		return HISTORY_MANAGER::findOwningEntity(pEntity);
	}
	HISTORY_STREAM* findStreamFromAttribute( ENTITY* );
	HISTORY_STREAM* findInStreamMap(ENTITY*);
	void clearMap() { mmEntityStreamMap.clear(); }
	void addToStreamMap(ENTITY*, HISTORY_STREAM*);
	void addGeometryFromTopology(ENTITY*, HISTORY_STREAM*);
};

/** @} */
#endif


