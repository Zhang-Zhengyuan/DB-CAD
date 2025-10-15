/*******************************************************************/
/*    Copyright (c) 1989-2020 by Spatial Corp.                     */
/*    All rights reserved.                                         */
/*    Protected by U.S. Patents 5,257,205; 5,351,196; 6,369,815;   */
/*                              5,982,378; 6,462,738; 6,941,251    */
/*    Protected by European Patents 0503642; 69220263.3            */
/*    Protected by Hong Kong Patent 1008101A                       */
/*******************************************************************/
#ifndef BL_INST
#define BL_INST
// Header file defining the "instruction" attributes to be hung onto the
// edges and vertices of a blend sequence, to instruct the algorithm
// what to do when it runs into things. For the moment, instructions can
// be:
// * "cap" - extend the blend surface if necessary, and cap it
// with any adjacent capping faces or faces of the blend sheet.
// * "run out" - run the blend out, vanishing into a point.
// * "roll on" - attempt to continue the blend by rolling/sliding onto
// the things run into.

// The idea is that these attributes are hung on to the body entities
// actually intercepted by the spring curves. The algorithm thus reads
// from the intercepted entities what to do. When more than one
// attribute is found, the closest one applies.

// When the attribute is found by blending, the idea is that it is told
// that it has been seen. Then, at the end of blending, all the "seen"
// instruction attributes can be deleted.
#include "dcl_blnd.h"
#include "acis.hxx"
#include "bl_trans.hxx"
#include "bl_att.hxx"
#include "position.hxx"

/**
* @file bl_inst.hxx
 * @CAA2Level L1
 * @CAA2Usage U1
 * \addtogroup BLNDAPI
 *
 * @{
 */

class ENTITY;

/**
 * @nodoc
 */
ENTITY_IS_PROTOTYPE(ATT_BL_INST, BLND )
#if 0
; // semicolon needed for mkman (doc tool) parsing
#endif

// tbrv
/**
 * @nodoc
 */
extern DECL_BLND int ATT_BL_INST_TYPE;
#define ATT_BL_INST_LEVEL ( ATTRIB_BLINFO_LEVEL + 1 )

/**
 * Defines the instruction attributes that hang onto the edges and vertices of a blend sequence, that instruct the blend algorithm.
 * <br>
 * <b>Role:</b> This class defines the attributes that provide special processing instructions to
 * the blend algorithm. The possible instructions are the values of the enumerated type
 * blend_transition:
 * <br><br>
 * <tt>blend_unknown</tt> deletes the blend.
 * <br><br>
 * <tt>blend_cap</tt> extends the blend surface if necessary, and caps it with any adjacent capping faces or faces of the blend sheet.
 */
class DECL_BLND ATT_BL_INST : public ATTRIB_BLINFO {

public:
/**
 * C++ initialize constructor requests memory for this object and populates it with the data supplied as arguments.
 * <br><br>
 * <b>Role:</b> Applications should call this constructor only with the overloaded <tt>new</tt> operator,
 * because this reserves the memory on the heap, a requirement to support roll back and history
 * management.
 * <br><br>
 * @param entity
 * entity.
 * @param blnd_trans
 * blend transition.
 * @param pos
 * position.
 */
	ATT_BL_INST( ENTITY * entity = NULL, blend_transition blnd_trans = blend_unknown,
		 SPAposition const & pos=  *(const class SPAposition *)NULL_REF );
/**
 * Returns <tt>TRUE</tt> if this is pattern compatible.
 */
    virtual logical pattern_compatible() const;
                                    // returns TRUE
/**
 * Returns the blend transition action.
 */
	blend_transition action() const { return _action; }
/**
 * Returns the position.
 */

	SPAposition const &pos() const { return _pos; }
/**
 * Returns the set position.
 */

	logical pos_set() const { return _pos_set; }
/**
 * Returns the unset position.
 */
	logical pos_unset() const { return !_pos_set; }
/**
 * Returns <tt>TRUE</tt> if the blend instruction has been seen; otherwise, it returns <tt>FALSE</tt>.
 */

	logical seen() const { return _seen; }
/**
 * Returns the seen note.
 */
	void note_seen() { backup(); _seen = TRUE; }

	// Owner split.
/**
 * Notifies the <tt>ATT_BL_INST</tt> that its owner is about to be split into two parts.
 * <br><br>
 * <b>Role:</b> The application has the chance to duplicate or otherwise modify the attribute.
 * The default action is to do nothing. This function is supplied by the application whenever it
 * defines a new attribute, and is called when a split occurs.
 * <br><br>
 * @param entity
 * new entity.
 */
	virtual void split_owner( ENTITY * entity);

	// A descriptive string for the object, for example "roll_on" or
	// "run_out" etc.

/**
 * Returns the descriptive string for the object, for example <tt>roll_on</tt>.
 */
	virtual char const *label() const;

private:

	blend_transition _action;

	SPAposition _pos;

	logical _pos_set;

	logical _seen;

public:

	/**
	 * @nodoc
	 */
	ATTRIB_FUNCTIONS( ATT_BL_INST, BLND )
#if 0
; // semicolon needed for mkman (doc tool) parsing)
#endif
// Because mkman isn't aware that ATTRIB_FUNCTIONS changes access to "public", we do so
// explicitly here:
public:

};


// Function to find the closest instruction to a given point
// for an entity.


/**
 * @nodoc
 */
DECL_BLND ATT_BL_INST *find_inst_attrib( ENTITY const *, SPAposition const & );

// As above, but return it only if it is a cap/rollon type (according to
// the function name). For convenience.


/**
 * @nodoc
 */
DECL_BLND ATT_BL_INST *find_cap_inst_attrib( ENTITY const *,
									SPAposition const & );


/**
 * @nodoc
 */
DECL_BLND ATT_BL_INST *find_rollon_inst_attrib( ENTITY const *,
									   SPAposition const & );


 /** @} */
#endif
