// $Id: att_tag.hxx,v 1.18 2002/08/09 17:15:16 jeff Exp $
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
//	  Tags for input entities used in annotations
//-----------------------------------------------------------------------

#ifndef __ATT_TAG_HXX__
#define __ATT_TAG_HXX__

#include "dcl_kern.h"
#include "entity.hxx"
#include "bullsmal.hxx"
#include "attrib.hxx"
#include "at_sys.hxx"
#include "option.hxx"
#include "errorsys.hxx"

#include "usecount.hxx"

/**
 * @file att_tag.hxx
 * @CAA2Level L1
 * @CAA2Usage U2
 * \addtogroup ACISATTRIBUTES
 *
 * @{
 */

// For efficiency, we would like to use use counts instead
// of actual copies of the ATTRIB_TAG when adding
// a tag to an ANNOTATION.  Most of the code is written
// and #if'd, but not tested.  The big problem is when
// an input entity is merged, resulting in it being put
// into an EE_LIST.  EE_LIST does not know whether its
// contents are use counted or not, so how do we know to
// adjust the use count during roll.  We do have ANNOTATION
// adjusting the active use counts as it adds and removes
// from the EE_LISTs, but rolling things is still a problem.
/**
 * @nodoc
 */
#define USE_COUNT_ATTRIB_TAG 0

/**
 * @nodoc
 */
#define ATTRIB_TAG_LEVEL (ATTRIB_SYS_LEVEL + 1)

/*
// tbrv
*/
/**
 * @nodoc
 */
extern DECL_KERN int ATTRIB_TAG_TYPE;

/**
 * @nodoc
 */
ENTITY_IS_PROTOTYPE( ATTRIB_TAG, KERN )
#if 0
; // semicolon needed for mkman (doc tool) parsing
#endif

class EE_LIST;

/**
 * Defines a base class that contains a pointer to the original input entity as well as a flag indicating if the entity still exists.
 * <br>
 * <b>Role:</b> Depending upon the modeling operation, a given input's entities may or may
 * not exist after operation completion. They are represented in the <tt>ANNOTATION</tt> with an
 * <tt>ATTRIB_TAG</tt>. <tt>ATTRIB_TAG</tt> contains a pointer to the original input entity and a flag
 * indicating if the entity has since been lost.
 * <br><br>
 * <tt>ATTRIB_TAG</tt> is designed to be a base class that can be derived from and enhanced.
 * An application can use this to attach additional data to the input entities
 * before an operation and have it carried forward to any annotations that reference it,
 * even if the original entity is lost.
 * <br><br>
 * Applications that derive from <tt>ATTRIB_TAG</tt> may want to set the <tt>error_no_input_tag</tt>
 * option to <tt>TRUE</tt>, to cause a <tt>sys_error</tt> if an input entity is not tagged.
 * This assumes that the additional information is necessary. It is also a useful debugging
 * aid, which helps to find places where the application did not tag all the inputs. If the
 * <tt>error_no_input_tag</tt> option is <tt>FALSE</tt>, tags are simply generated on the fly, as needed.
 *
 * Applications should not change the attribute migration behavior of derived classes. 
 * @see ENTITY
 */
class DECL_KERN ATTRIB_TAG : public ATTRIB_SYS
{
	ENTITY* m_origin;
	logical m_origin_lost : 2;
	logical m_need_fix_pointer : 2;
	logical m_made_internally : 2;

public:
#if USE_COUNT_ATTRIB_TAG
	// Count uses in addtion to the owning entities
	// attribute list.  In particular, any ANNOTATIONs
	/**
	 * @nodoc
	 */
	USE_COUNTED_DECL
#if 0
; // semicolon needed for mkman (doc tool) parsing)
#endif
#endif
/**
 * Constructs an <tt>ATTRIB_TAG</tt> from a given original <tt>ENTITY</tt> and attaches it to a given
 * owning <tt>ENTITY</tt>.
 * <br><br>
 * <b>Role</b>: Requests memory for this object and populates it with the data
 * supplied as the argument. Applications should call this constructor only with
 * the overloaded <tt>new</tt> operator, because this reserves the memory on the heap,
 * a requirement to support roll back and history management.
 * <br><br>
 * @param e
 * the owning ENTITY for the constructed attribute.
 * @param o
 * the original ENTITY for the constructed attribute.
 * @param internal
 * flag the constructed attribute as having been made internally.
 */
	ATTRIB_TAG(ENTITY* e = NULL, ENTITY* o = NULL, logical internal = FALSE);

	/**
	 * @nodoc
	 */
	ATTRIB_FUNCTIONS(ATTRIB_TAG, KERN)
#if 0
; // semicolon needed for mkman (doc tool) parsing)
#endif
// Because mkman isn't aware that ATTRIB_FUNCTIONS changes access to "public", we do so
// explicitly here:
public:

/**
 * Returns the target of a copy operation.
 */
	ATTRIB_TAG* copy()
#if USE_COUNT_ATTRIB_TAG == 0
	const
#endif
	;

// This function is hidden from mkman in the RESTORE_DEF macro; to have it documented,
// we include it here:
#if 0
/**
 * Restores this <tt>ATTRIB_TAG</tt> from a SAT file.
 * <br><br>
 * <b>Role:</b> The <tt>RESTORE_DEF</tt> macro expands to the <tt>restore_common</tt> method,
 * which is used in reading information from a SAT file. This method is never called
 * directly. It is called by a higher hierarchical function if an item in the SAT file
 * is determined to be of this class type. An instance of this class will already have
 * been created through the allocation constructor. This method then populates the class
 * instance with the appropriate data from the SAT file.
 * <pre>
 *    read_logical  True if the origin has been lost.
 *    if (m_origin_lost)
 *       read_int   Integer for the entity
 *    else
 *       read_ptr   Pointer to the origin.</pre>
 */
    void restore_common();
#endif

// These function are hidden from mkman in the ATTRIB_FUNCTIONS macro; to have them documented,
// we include them here:
#if 0
/**
 * Returns a type identifier for this object.
 * <br><br>
 * <b>Role:</b> If <tt>level</tt> is unspecified or 0, returns the type identifier <tt>ATTRIB_TAG_TYPE</tt>. If
 * <tt>level</tt> is specified, returns <tt>ATTRIB_TAG_TYPE</tt> for that level of derivation from <tt>ENTITY</tt>. The
 * level of this class is defined as <tt>ATTRIB_TAG_LEVEL</tt>.
 * <br><br>
 * @param level
 * the derivation level at which the object is to be identified.
 */
	virtual int identity( int level = 0 ) const;
/**
 * Returns the string <tt>"tag"</tt>.
 */
	virtual const char *type_name() const;
/**
 * Posts a delete bulletin to the bulletin board indicating this <tt>ATTRIB_TAG</tt> is no longer used in the active model.
 * <br><br>
 * <b>Role:</b> The <tt>lose</tt> methods for attached attributes are also called.
 */
	virtual void lose();
#endif

/**
 *  Records that the original <tt>ENTITY</tt> has been lost.
 */
	void lose_origin();

/**
 * Determines whether or not the original <tt>ENTITY</tt> has been lost.
 */
	logical origin_lost() const { return m_origin_lost; }

/**
 * Returns a pointer to the original <tt>ENTITY</tt>.
 */
	ENTITY* origin() const { return m_origin; }

/**
 * Determines whether this <tt>ATTRIB_TAG</tt> is made internally.
 */
	logical internal() const { return m_made_internally; }

	// Indicate that this attribute should not be included when the owning
	// entity is copied.  The default is to call duplicatble().
/**
 * Indicates that this <tt>ATTRIB_TAG</tt> should not be included when the owning <tt>ENTITY</tt> is copied.
 * <br><br>
 * <b>Role:</b> The default is to call <tt>duplicatable</tt>.
 */
	virtual logical copyable() const;

	// Indicate that this attribute should not be included when the owning
	// entity is moved.  The default is to call copyable().
/**
 * Returns <tt>TRUE</tt> if this <tt>ATTRIB_TAG</tt> is moveable from one owning <tt>ENTITY</tt> to another.
 * <br><br>
 * <b>Role:</b>  The default is to call <tt>copyable()</tt>.
 */
	virtual logical moveable() const;

	// Jeff 04-18-05 These are not deleteable, whether owned or not.
/**
 * @nodoc
 */
	virtual logical deletable() const;

/**
 * @nodoc
 */
    virtual logical pattern_compatible() const; // documented in base class
};

/**
 * @nodoc
 */
DECL_KERN ATTRIB_TAG* find_ATTRIB_TAG(const ENTITY* ent);

/**
 * @nodoc
 */
DECL_KERN ENTITY* copy_ATTRIB_TAG(ENTITY* ent, ENTITY *owner);

/**
 * Gets the <tt>ENTITY</tt> associated with the given <tt>ATTRIB_TAG</tt>. 
 * If the given <tt>ENTITY</tt> is not an <tt>ATTRIB_TAG</tt>, 
 * simply returns the given <tt>ENTITY</tt>. 
 * <br><br>
 * <b>Effect:</b> Read-only.
 */
DECL_KERN ENTITY* get_actual_entity(ENTITY* ent);

/**
 * Gets the <tt>ENTITY</tt> associated with the given <tt>ATTRIB_TAG</tt>, if 
 * the <tt>ENTITY</tt> has not been lost. If the <tt>ENTITY</tt> has been lost, 
 * returns NULL. If the given <tt>ENTITY</tt> is not an <tt>ATTRIB_TAG</tt>, 
 * simply returns the given <tt>ENTITY</tt>. 
 * <br><br>
 * <b>Effect:</b> Read-only.
 */
DECL_KERN ENTITY* get_actual_live_entity(ENTITY* ent);

/**
 * @nodoc
 */
DECL_KERN ENTITY* get_actual_live_entity_or_attrib_tag(ENTITY* ent);

/**
 * @nodoc
 */
DECL_KERN ATTRIB_TAG* insure_ATTRIB_TAG(ENTITY* ent);

/**
 * @nodoc
 */
DECL_KERN logical is_live_entity(ENTITY* ent);

/**
 * @nodoc
 */
DECL_KERN logical is_tag_in_list(const EE_LIST* list, const ENTITY* entity);

// A callback class allowing applications to provide there own derived
// versions of ATTRIB_TAG, which may contain application specific data
// unknown to ACIS.

#include "tlkit_cb.hxx"

/*
// tbrv
*/
/**
 * @nodoc
 */
class DECL_KERN attrib_tag_callbacks : public toolkit_callback {
public:
	// Insure that the given ENTITY has an ATTRIB_TAG attached.  This
	// is called when the annotation support must have an ATTRIB_TAG to
	// to ensure the annotation algorithm will work. This callback gives
	// the application a chance to attach it own class, derived from
	// ATTRIB_TAG, which may contain application specific information.
	//
	// Each callback in the list is called until an ATTRIB_TAG derivation
	// is attached.  If none attaches an ATTRIB_TAG derivation, the system
	// will attach an ATTRIB_TAG base ATTRIB.

	virtual ATTRIB_TAG* Insure_Attrib_Tag(ENTITY* pEntity);

	// Make a copy of the ATTRIB_TAG or derivation therefrom for the
	// given ENTITY.  This is called when the system needs its own
	// copy, which it will lose() when it is done.
	virtual ATTRIB_TAG* Copy_Attrib_Tag(ENTITY* pEntity);
};

/*
// tbrv
*/
/**
 * @nodoc
 */
class DECL_KERN attrib_tag_callbacks_list : public toolkit_callback_list {
public:
	ATTRIB_TAG* Insure_Attrib_Tag(ENTITY* pEntity);
	ATTRIB_TAG* Copy_Attrib_Tag(ENTITY* pEntity);

	// Add callbacks to the beginning of the list
	void add(attrib_tag_callbacks* cb) { toolkit_callback_list::add(cb); }

	// Add callbacks to the end of the list
	void append(attrib_tag_callbacks* cb) { toolkit_callback_list::append(cb); }
};

/**
 * Gets the global list of WCS callbacks.
 * <br><br>
 * <b>Effect:</b> Read-only.
**/
DECL_KERN attrib_tag_callbacks_list& get_attrib_tag_callbacks_list();
/** @} */
#endif //__ATT_TAG_HXX__
