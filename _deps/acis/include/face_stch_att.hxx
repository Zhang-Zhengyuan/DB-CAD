/*******************************************************************/
/*    Copyright (c) 1989-2020 by Spatial Corp.                     */
/*    All rights reserved.                                         */
/*    Protected by U.S. Patents 5,257,205; 5,351,196; 6,369,815;   */
/*                              5,982,378; 6,462,738; 6,941,251    */
/*    Protected by European Patents 0503642; 69220263.3            */
/*    Protected by Hong Kong Patent 1008101A                       */
/*******************************************************************/

#if !defined (ATTRIB_HH_ENT_STITCH_FACE_CLASS)
#define ATTRIB_HH_ENT_STITCH_FACE_CLASS


#include "att_hh.hxx"
#include "dcl_stitch.h"
#include "entstchbase.hxx"
#include "at_enty_stitch.hxx"

class FACE;


// For internal use only
/**
 * @nodoc
 */
ENTITY_IS_PROTOTYPE(ATTRIB_HH_ENT_STITCH_FACE, STITCH)
#if 0
; // semicolon needed for mkman (doc tool) parsing
#endif
// For internal use only
/**
 * @nodoc
 */
typedef void KEY_TYPE;
// For internal use only
/**
 * @nodoc
 */
typedef void DATA_TYPE;

// For internal use only
/**
 * @nodoc
 */
DECL_STITCH extern int ATTRIB_HH_ENT_STITCH_FACE_TYPE;

/**
 * @nodoc
 */
#define ATTRIB_HH_ENT_STITCH_FACE_LEVEL (ATTRIB_HH_ENT_STITCH_BASE_LEVEL + 1)


class ATTRIB_HH_ENT_STITCH_FACE : public ATTRIB_HH_ENT_STITCH_BASE
{
protected:

	logical m_not_to_be_stitched;
	// STI cpp begin
	logical save_sw;
	// STI cpp end


public:

// Constructor
/**
 *@nodoc
 */
//C++ initialize constructor requests memory for this object and populates it with the data supplied as arguments.

	ATTRIB_HH_ENT_STITCH_FACE ( FACE* f = NULL);

/**
 * Returns <tt>TRUE</tt> if this is pattern compatible.
 */
    virtual logical pattern_compatible() const;

/**
 *@nodoc
 */
	logical is_face_not_to_be_stitched();
	void set_face_not_to_be_stitched(logical choice);

// Standard Attrib functions

/**
 * @nodoc
 */
	ATTRIB_FUNCTIONS (ATTRIB_HH_ENT_STITCH_FACE, STITCH)
#if 0
; // semicolon needed for mkman (doc tool) parsing)
#endif
// Because mkman isn't aware that ATTRIB_FUNCTIONS changes access to "public", we do so
// explicitly here:
public:

	/**
	 * @nodoc
	 */
	ROLL_NOTIFY_DECL
#if 0
; // semicolon needed for mkman (doc tool) parsing)
#endif

};

// For internal use only
/**
 * @nodoc
 */
DECL_STITCH ATTRIB_HH_ENT_STITCH_FACE* find_att_face_stitch(FACE*);




#endif // ATTRIB_HH_ENT_STITCH_FACE_CLASS
