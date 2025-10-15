/* ORIGINAL: acis2.1/kernutil/subtype/subtrans.hxx */
/* $Id: subtrans.hxx,v 1.11 2002/08/09 17:15:33 jeff Exp $ */
/*******************************************************************/
/*    Copyright (c) 1989-2020 by Spatial Corp.                     */
/*    All rights reserved.                                         */
/*    Protected by U.S. Patents 5,257,205; 5,351,196; 6,369,815;   */
/*                              5,982,378; 6,462,738; 6,941,251    */
/*    Protected by European Patents 0503642; 69220263.3            */
/*    Protected by Hong Kong Patent 1008101A                       */
/*******************************************************************/

// Define a shared-subtype class which is subject to transformations,
// and is designed to ensure that transforming several references to
// the same object with the same transformation retains the sharing.
// We cannot do everything, so the owning objects have to be aware of
// what is happening, but at least the guts of the mechanism can be
// shared.

#if !defined( SUBTRANS_OBJECT )
#define SUBTRANS_OBJECT

// This object is derived from the shared subtype object.

#include "dcl_kern.h"
#include "subtype.hxx"

#include "transf.hxx"

class subtrans_trans;

// STI ROLL
class SizeAccumulator;
// STI ROLL
/**
* @file subtrans.hxx
 * @CAA2Level L1
 * @CAA2Usage U1
 *! \addtogroup KERNAPI
 *
 * @{
 */

/**
 * Defines a <tt>shared-subtype</tt> class that is subject to transformations.
 * <br>
 * <b>Role:</b> This class defines a <tt>shared-subtype</tt> class that is subject to
 * transformations, and is designed to ensure that transforming several references
 * to the same object with the same transformation retains the sharing.
 */
class DECL_KERN subtrans_object: public subtype_object {
private:
	// Pointers to link shared objects which differ only in a
	// transformation. This allows the separate transformation of
	// shared objects to maintain the sharing - we can scan the
	// transform list for a match, and if found just flip the
	// pointer.

	subtrans_trans *trans_list;// list of objects which differ from
							// this one by just a transform.

	subtrans_object *parent;// object in whose trans_list this one
							// appears.

	subtrans_object *next_obj;// next object in list of different
							// extensions of the same spline or intcurve.

public:
	// External-level transformation. Searches the transform list to
	// find a match, and returns the corresponding transformed object
	// if found, failing that constructs a new transformed object,
	// enters it into the transformation list for future reference,
	// and returns the new object. If this object is only singly-
	// referenced, however, and has no transformed counterpart, then
	// it is just transformed directly without copying, and "this"
	// returned.
/**
 * Transforms the <tt>subtrans_object</tt> on an external level.
 * <br><br>
 * <b>Role:</b> This method searches the transform list to find a match.
 * It returns the corresponding transformed object if it is found; otherwise,
 * it constructs a new transformed object, enters it into the transformation list
 * for future reference, and returns the new object. If this object is only
 * singly-referenced and has no transformed counterpart, then it transforms
 * directly without copying, and <tt>"this"</tt> returns.
 * <br><br>
 * @param trans
 * transformation.
 */
	subtrans_object *make_trans( SPAtransf const &trans );

	// Clear any references to this object via transformation lists,
	// prior to incompatible changes.
/**
 * Clears any references to this object using transformation lists, before incompatible changes.
 */
	void clear_trans();
/**
 * Returns the next object in the list.
 */
	subtrans_object *get_next() const {return next_obj;}
/**
 * Sets the next object in the list.
 * <br><br>
 * @param obj
 * object.
 */
	void set_next( subtrans_object* obj);

	// STI ROLL
/*
// tbrv
*/
/**
 * @nodoc
 */
	virtual void full_size(SizeAccumulator&, logical = TRUE) const;
	// STI ROLL

protected:
	// The constructor initialises the pointers to NULL.

	subtrans_object();

	// Constuctor to be used when a linked list of extensions is made.
	subtrans_object( subtrans_trans*, subtrans_object* );

 	// The destructor removes references in the trans_list, and also
	// the one referring to this in the parent's list.

	~subtrans_object();

	// Duplicate this object, virtual so that the true derived object
	// gets copied.

	virtual subtrans_object *copy() const = 0;

	// Transformation

	virtual void operator*=( SPAtransf const & ) = 0;

	friend class subtrans_trans;
};

// Class to implement a list of objects which are transformed from
// the owner.
/*
// tbrv
*/
/**
 * @nodoc
 */
class DECL_KERN subtrans_trans : public ACIS_OBJECT {
	subtrans_trans *next;		// list pointer

	subtrans_object *child;		// object which is a transformation
								// of the owner of this list
	SPAtransf trans;				// transformation of owner to give
								// child

	// Constructor transforms the given parent to make a new child,
	// and hooks up all the pointers.

	subtrans_trans( subtrans_object *, SPAtransf const & );

	friend class subtrans_object;
};

/*! @} */
#endif
