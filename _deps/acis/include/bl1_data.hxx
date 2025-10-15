/*******************************************************************/
/*    Copyright (c) 1989-2020 by Spatial Corp.                     */
/*    All rights reserved.                                         */
/*    Protected by U.S. Patents 5,257,205; 5,351,196; 6,369,815;   */
/*                              5,982,378; 6,462,738; 6,941,251    */
/*    Protected by European Patents 0503642; 69220263.3            */
/*    Protected by Hong Kong Patent 1008101A                       */
/*******************************************************************/
//
// Declarations of classes for stage one of blending: making the sheet body.
//

// Note: this could be made much more clever, with more conveniences,
// but things are probably going to change substantially before too long.


//
// Modification---
//      25-Jan-02 tgg   Fix for bug 66863.
//	20-Apr-99 bd	New routine bl1_data::make_ent_ent_blend()
//	31-Mar-99 bd	New code.
//

#if !defined( BLEND1_DATA_DEF )
#define  BLEND1_DATA_DEF

#include "acis.hxx"

#include <stdio.h>
#include "logical.h"
#include <math.h>

#include "dcl_blnd.h"

#include "face.hxx"
#include "lists.hxx"

#include "bl_att.hxx"

/**
* @file bl1_data.hxx
 * @CAA2Level L1
 * @CAA2Usage U1
 * \addtogroup BLNDAPI
 *
 * @{
 */

//cbi TODO:
// - maintain the count in here?

/**
 * Implementation of common data for stage one of blending.
 * <br>
 * <b>Role:</b> Implementation of common data for stage one of blending for
 * single step blending APIs: <tt>api_init_blend_ss</tt>, <tt>api_do_one_blend_ss</tt>,
 * <tt>api_concl_blend_ss</tt>.  If other blending APIs are used, this class is
 * handled internally and does not have to be created by the user.
 */
class DECL_BLND blend1_data : public ACIS_OBJECT
{
public:
	// The optional argument repeat_limit is the number of times
	// that a single attribute will be attempted before giving up.

/**
 * C++ initialize constructor requests memory for this object and populates it with the data supplied as arguments.
 * <br><br>
 * @param att_list
 * list of attributes to be processed.
 * @param repeat_limit
 * number of times a single attribute will be attempted.
 */
	blend1_data( ENTITY_LIST &att_list, int repeat_limit = 3 );


/**
 * @nodoc
 */
	~blend1_data();


	// Data retrival

/*
// tbrv
*/
/**
 * @nodoc
 */
	ATTRIB_BLEND *blend_att( int index ) const
		{ return (ATTRIB_BLEND*)att_list[index]; }
/*
// tbrv
*/
/**
 * @nodoc
 */
	logical is_initialized() const
		{ return init_flag; }
/*
// tbrv
*/
/**
 * @nodoc
 */
	logical uninitialized() const
		{ return !init_flag; }
/*
// tbrv
*/
/**
 * @nodoc
 */
	logical in_list( ATTRIB_BLEND *bl_att ) const
		{ return (att_list.lookup( bl_att ) >= 0 ); }
/*
// tbrv
*/
/**
 * @nodoc
 */
	logical seen_this_att( ATTRIB_BLEND *bl_att ) const
		{ return bl_att == NULL ? TRUE : bl_att->been_seen(); }
/*
// tbrv
*/
/**
 * @nodoc
 */
	logical seen_this_att( int idx ) const
		{ return idx >= init_list_len; }
/*
// tbrv
*/
/**
 * @nodoc
 */
	logical tried_all_atts() const;
/*
// tbrv
*/
/**
 * @nodoc
 */
	logical none_succeeded() const;
/*
// tbrv
*/
/**
 * @nodoc
 */
	logical too_many_repeats( ATTRIB_BLEND *att = NULL, int idx = -1 ) const;

	// Try to find an error message in the blend attributes.
	// Returns the first non-zero error message found,
	// or the default provided by the caller.
	// It also returns the attribute, if any, responsible for
	// the error message.

/**
 * Returns the first non-zero error message found, or the default provided by the caller.
 * <br><br>
 * <b>Role:</b> It also returns the attribute, if any, responsible for the error message.
 * Can be called when a blend fails.
 * <br><br>
 * @param default_msg
 * error message.
 * @param att
 * attribute.
 */
	err_mess_type get_error_msg(
			err_mess_type default_msg,
			ATTRIB_BLEND *&att
		) const;

	// The same, without returning the attribute.
/**
 * Returns the first non-zero error message found, or the default provided by the caller.
 * <br><br>
 * <b>Role:</b> It does not return the attribute, if any, responsible for the error message.
 * Can be called when a blend fails.
 * <br><br>
 * @param default_msg
 * error message.
 */

	err_mess_type get_error_msg( err_mess_type default_msg) const
		{ ATTRIB_BLEND *att; return get_error_msg( default_msg, att ); }

	// If a face-face blend will not fit anywhere along the blend,
	// attempt to create an entity-entity blend.


/**
 * @nodoc
 */
	ATT_BL_ENT_ENT *make_ent_ent_blend();


	// Member-setting methods:

	// Reset everything, clearing out all entity lists.

/**
 * @nodoc
 */
	void clear();


/**
 * @nodoc
 */
	void set_bbf_data( FACE *blend_body_face );

	// This resets everything except the attribute list and counts,
	// and marks us as initialized.


/**
 * @nodoc
 */
	void initialize();

	// Method that calls the "lose" method on attributes that
	// point to this object.  Does this by searching each
	// listed entity that might have such attributes.  Written
	// by TGG as part of a fix for bug 66863.

/**
 * @nodoc
 */
	void lose_attributes_from_entities( );


	// Data members.  No reason these shouldn't be public.

/**
 * @nodoc
 */
	ENTITY_LIST att_list;	// list of blend attributes

/**
 * @nodoc
 */
	ENTITY_LIST del_list;	// list of ents to be deleted

/**
 * @nodoc
 */
	ENTITY_LIST rev_list;	// list of faces that we've reversed

/**
 * @nodoc
 */
	int num_given_atts;		// Initial number of actual attributes.

/**
 * @nodoc
 */
	int init_list_len;		// Count of list after intialization
							// Same as num_given_atts unless pre-1.6 blends
							// are being processed.

/**
 * @nodoc
 */
	int max_repeats;		// Number of times that a single attribute
							//   will be attempted before giving up.

	// info about the blend-body faces

/**
 * @nodoc
 */
	SIDESBIT bbf_sides;		// single- or double-sided

/**
 * @nodoc
 */
	CONTBIT  bbf_cont;		// free surface or embedded

/**
 * @nodoc
 */
	logical  bbf_sides_and_cont_found;

/**
 * @nodoc
 */
	logical init_flag;
};


 /** @} */
#endif
