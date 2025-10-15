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
//   A wrapper for ENTITY_LIST that is an ENTITY, with full support for
//   save/restore and rollback.
//
//   We do this for ENTITY_LIST specifically and not for VOID_LIST because
//   save/restore and rollback of arbitrary types is too hard.
//-----------------------------------------------------------------------

#ifndef __EELISTS_HXX__
#define __EELISTS_HXX__

#include "entity.hxx"
#include "lists.hxx"

/**
 * @file eelists.hxx
 * @CAA2Level L1
 * @CAA2Usage U1
 * \addtogroup ACISLISTS
 *
 * @{
 */

#define EE_LIST_LEVEL (ENTITY_LEVEL + 1)
/*
// tbrv
*/
/**
 * @nodoc
 */
extern DECL_KERN int EE_LIST_TYPE;
/**
 * @nodoc
 */
ENTITY_IS_PROTOTYPE( EE_LIST, KERN )
#if 0
; // semicolon needed for mkman (doc tool) parsing
#endif
/**
 * Defines a wrapper for <tt>ENTITY_LIST</tt> that is an <tt>ENTITY</tt>, with full support for save/restore and rollback.
 * <br>
 * <b>Role:</b> A wrapper for <tt>ENTITY_LIST</tt> that is an <tt>ENTITY</tt>, with full support for save/restore
 * and rollback. Note that this is done for <tt>ENTITY_LIST</tt> only and not for <tt>VOID_LIST</tt>.
 * <br> For best performance in loops that step through this list, have the loops increment
 * rather than decrement the index counter. Internal operations for methods like <tt>operator[]</tt>
 * and <tt>remove</tt> store the index counter from the previous operation allowing faster access to
 * the next indexed item when indexing up.
 *<br><br>
 * @see ENTITY, ENTITY_LIST
 */

class DECL_KERN EE_LIST : public ENTITY {
	// The live data of this list
	ENTITY_LIST	this_list;

	// If this list is "owned" by another, provide a back pointer to it.
	ENTITY* the_owner;

	// Ownership policy.  This affects copy and delete via copy_scan
	unsigned int own_entries : 1;

	// Need some help in save/restore and copy
	unsigned int zero_index : 1;

public:
	/**
	 * @nodoc
	 */
	ENTITY_FUNCTIONS(EE_LIST, KERN)
#if 0
; // semicolon needed for mkman (doc tool) parsing)
#endif
// Because mkman isn't aware that ENTITY_FUNCTIONS changes access to "public", do so
// explicitly here:
public:

	/**
	 * @nodoc
	 */
	ROLL_NOTIFY_FUNCTION
#if 0
; // semicolon needed for mkman (doc tool) parsing)
#endif
	/**
	 * @nodoc
	 */
	FULLSIZE_FUNCTION
#if 0
; // semicolon needed for mkman (doc tool) parsing)
#endif
/**
 * C++ initialize constructor.
 * <br><br>
 * <b>Role:</b> Requests memory for this object and populates it with the data supplied as
 * arguments. Applications should call this constructor only with the overloaded new operator,
 * because this reserves the memory on the heap, a requirement to support roll back and history
 * management.
 *<br><br>
 * @param oe
 * flag for output entity.
 */
 	EE_LIST(logical oe = TRUE);
/**
 * C++ initialize constructor.
 * <br><br>
 * <b>Role:</b> Requests memory for this object and populates it with the data supplied as arguments.
 *<br><br>
 * @param owner
 * pointer to owner.
 * @param oe
 * flag for output entity.
 */
 	EE_LIST(ENTITY* owner, logical oe = TRUE);
/**
 * If this list is "owned" by another, this provides a back pointer to it.
 */
 	virtual ENTITY *owner() const { return the_owner; }
/**
 * Returns whether or not the list can be deleted.
 */
 	virtual logical deletable() const;
/**
 * Specifies the owning entity for this list.
 *<br><br>
 * @param o
 * owning entity.
 */
 	void set_owner(ENTITY* o) { the_owner = o; }

	// Empty a list ready for construction of a new one.
/**
 * Empties a list ready for construction of a new one.
 */
 	void clear();

	// Add an ENTITY to the list if not already there, and
	// always return the index.
/**
 * Adds an entity to the EE_LIST if not already there.
 * <br><br>
 * <b>Role:</b> It always returns the index.
 *<br><br>
 * @param e_add
 * entity to add.
 */
 	int add( ENTITY *e_add );

  // virtual void ENTITY::add() is hidden by add(ENTITY*).
  // Expose it again by defining a pass thru here.
/**
 * virtual void <tt>ENTITY::add()</tt> is hidden by <tt>add(ENTITY*)</tt>.
 * <br><br>
 * <b>Role:</b> Expose it again by defining a pass through here.
 */
   void add() { ENTITY::add(); }

	// Search for an ENTITY in the list.
/*
// tbrv
*/
/**
 * @nodoc
 */
 	int lookup( ENTITY const * e) const {
		return this_list.lookup(e);
	}

  // virtual int ENTITY::lookup(logical) const is hidden by
  //    lookup(ENTITY const*) const.
  // Expose it again by defining a pass thru here.
/*
// tbrv
*/
/**
 * @nodoc
 */

  int lookup(logical l) const { return ENTITY::lookup(l); }


	// Delete an entity from the list. This does not free space, and
	// leaves a tombstone in the linear list which count() and
	// operator[] will notice, but subsequently lookup() will not
	// find this entity, nor will init()/next(). The return value is
	// the lookup() value of the old entry.
/**
 * Deletes an entity from the list.
 * <br><br>
 * <b>Role:</b> This does not free space and leaves a tombstone in the linear list which
 * <tt>count</tt> and <tt>operator[]</tt> will notice. But, <tt>lookup</tt> will not find this entity, nor will <tt>init</tt>
 * or <tt>next</tt>. The return value is the lookup value of the old entry.
 *<br><br>
 *@param e_rem
 * entity to remove.
 */
 	int remove( ENTITY const *e_rem );

  // virtual void ENTITY::remove(int) is hidden by remove(ENTITY const*).
  // Expose it again by defining a pass thru here.
/**
 * Returns the indexed entity.
 * <br><br>
 * <b>Role:</b> <tt>NULL</tt> if the index is out of range, or <tt>LIST_ENTRY_DELETED</tt> if the indexed entry has
 * been deleted.
 * <br><br>
 * @param i
 * index into list.
 */
   void remove(int i) { ENTITY::remove(i); }

	// STI let (r3891): added another removal method that is more efficient
	// if the ENTITY's index value is already known.

	// STI jmb: renamed from remove to remove_index because remove(int) is
	// now a virtual of the base ENTTIY class (related to usecounts).
	// This is not a problem for normal ENTITY_LIST because they are not
	// derived from ENTITY.
/**
 * Deletes an entity from the list.
 * <br><br>
 * <b>Role:</b> This is another removal method that is more efficient if the entity's
 * index value is already known.
 *<br><br>
 * @param val
 * index value.
 */
 	int remove_index( int val );

	// Count how many entities there are in the list (including
	// deleted entries).
/**
 * Returns the number of entities in the list.
 */
 	int count() const {
		return this_list.count();
	}

// STI jmb
	// Count how many entities there are in the list
	// not including deleted entries.  Uses the iterator
/**
 * Counts how many entities there are in the list not including deleted entries,Uses the iterator.
 */
 	int iteration_count() const {
		return this_list.iteration_count();
	}
// STI jmb

	// Return the indexed entity, or NULL if the index is out of
	// range, or LIST_ENTRY_DELETED if the indexed entry has been
	// deleted.
/**
 * Returns the indexed entity, <tt>NULL</tt> if the index is out of range, or <tt>LIST_ENTRY_DELETED</tt> if the indexed entry has been deleted.
 *<br><br>
 * @param i
 * index into list
 */
 	ENTITY *operator[]( int i ) const {
		return this_list[i];
	}


	// Return entities in list order, ignoring deleted items. Call
	// init() once, then next() repeatedly until it returns NULL.
	// Note that next() returns the undeleted item most closely
	// following the one most recently returned by next() or
	// operator[], except that if that value was NULL the value
	// of next() is undefined.
/**
 * Returns entities in list order, ignoring deleted items.
 * <br><br>
 * <b>Role:</b> Call <tt>init</tt> once, then <tt>next</tt> repeatedly until it returns <tt>NULL</tt>. Note that <tt>next</tt>
 * returns the undeleted item most closely following the one most recently returned by <tt>next</tt>
 * or <tt>operator[]</tt>, except that if that value was <tt>NULL</tt> the value of <tt>next</tt> is undefined.
 */
 	void init() const {
		this_list.init();
	}
/**
 * Returns entities in list order, ignoring deleted items.
 * <br><br>
 * <b>Role:</b> Call <tt>init</tt> once, then <tt>next</tt> repeatedly until it returns <tt>NULL</tt>. Note that <tt>next</tt>
 * returns the undeleted item most closely following the one most recently returned by <tt>next</tt>
 * or <tt>operator[]</tt>, except that if that value was <tt>NULL</tt> the value of <tt>next</tt> is undefined.
 */
 	ENTITY *next() const {
		return this_list.next();
	}

	// Return the next non deleted entry after the index given
	// without affecting the member variables used by init and
	// next.  This allows clients to create iterators that can
	// be multiply instantiated and which run independently of
	// each other. This is accomplished simply by giving the
	// the user the appropriate variables to save them selves.

	// STI rr (10/16/01) begin: removed unnecessary void *&ptr
	// that was never being used, from the arguement list.
/**
 * Returns the next non-deleted entry after the index given without affecting the member variables used by <tt>init</tt> and <tt>next</tt>.
 * <br><br>
 * <b>Role:</b> This allows clients to create iterators that can be multiply instantiated and which
 * run independently of each other. This is accomplished simply by giving the user the appropriate
 * variables to save themselves.
 *<br><br>
 * @param i
 * index.
 */
 	ENTITY *next_from(int& i) {
		return this_list.next_from(i);
	}
	// STI rr end.

/**
 * Returns a logical indicating whether this EE_LIST owns its entries.
 * <br><br>
 * <b>Role:</b> An EE_LIST that has ownership of its entries will delete them when it destructs.
 */
	logical owns_entries() { return own_entries; }

/**
 * Sets the entry ownership flag to the desired state.
 * <br><br>
 * <b>Role:</b> An EE_LIST that has ownership of its entries will delete them when it destructs. This will
 * cause unexpected bahavior if an already owned ENTITY is added to an EE_LIST that has its ownership
 * flag set to TRUE.
 *<br><br>
 * @param owns
 * logical ownership flag.
 */
	void set_ownership( logical owns) { own_entries = owns; }
};

/** @} */
#endif //__EELISTS_HXX__
