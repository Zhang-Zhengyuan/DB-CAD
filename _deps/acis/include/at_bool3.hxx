/* ORIGINAL: acis2.1/kernbool/bool3/at_bool3.hxx */
/* $Id: at_bool3.hxx,v 1.9 2002/08/20 11:47:49 vdelos Exp $ */
/*******************************************************************/
/*    Copyright (c) 1989-2020 by Spatial Corp.                     */
/*    All rights reserved.                                         */
/*    Protected by U.S. Patents 5,257,205; 5,351,196; 6,369,815;   */
/*                              5,982,378; 6,462,738; 6,941,251    */
/*    Protected by European Patents 0503642; 69220263.3            */
/*    Protected by Hong Kong Patent 1008101A                       */
/*******************************************************************/

// Header file defining data structures private to the Boolean
// operator code phase 3.

#if !defined( BOOL3_ATTRIB )
#define BOOL3_ATTRIB

#include "logical.h"

#include "acis.hxx"
#include "dcl_bool.h"

#include "lists.hxx"
#include "at_sys.hxx"

// This attribute is attached to each face of the input bodies, to
// indicate its disposal. Simple cases are simply "dispose" or "keep",
// but it may be necessary to convert a double-sided face to a single-
// sided one, or vice versa.

extern DECL_BOOL int ATTRIB_DISPOSE_TYPE;
#define ATTRIB_DISPOSE_LEVEL (ATTRIB_SYS_LEVEL + 1)

class FACE;
class EDGE;
/**
 * @nodoc
 */
ENTITY_IS_PROTOTYPE(ATTRIB_DISPOSE, BOOL )
#if 0
; // semicolon needed for mkman (doc tool) parsing
#endif

// Enumerated type to list the various disposal options.

enum face_dispose {
	unknown_disposal,
	discard_face,
	retain_face,
	convert_to_forward,
	convert_to_reverse,
	convert_to_double_outside,
	convert_to_double_inside,
	reverse_sense,

    //  Equivalents to the above but for faces which will be in the outside
    //  result of a chop operation (i.e. those faces in the subtraction part
    //  of the result).  For chop, the above values imply the face will be in
    //  the inside part of the result.
	retain_face_ext,
	convert_to_forward_ext,
	convert_to_reverse_ext,
	convert_to_dbl_outside_ext,
	reverse_sense_ext,

    //  Combinations of disposals for faces which will be copied.  First
    //  disposal remains with the original, the second disposal is for the
    //  copy face.
    retain_n_retain,                  //  Retain both faces
    retain_n_convert_to_do,           //  Retain and convert to double outside
    convert_to_do_n_retain,           //  Convert to double outside and retain
    retain_n_reverse,                 //  Retain and reverse
    convert_to_forward_n_reverse,     //  Convert to forward and convert to reverse
    convert_to_reverse_n_forward      //  Convert to reverse and convert to forward
};

//  Function to print equivalent strings for enums for use in debug output.
DECL_BOOL const char* face_dispose_text(face_dispose);

class DECL_BOOL ATTRIB_DISPOSE: public ATTRIB_SYS {
public:
	face_dispose disp;
	logical in_blank_body;	// TRUE if face was originally in the
							// blank body, FALSE if it was in the tool

	ATTRIB_DISPOSE(
				ENTITY * = NULL,
				face_dispose = retain_face,
				logical = TRUE
			);

    virtual logical pattern_compatible() const;
                                    // returns TRUE 
	/**
	 * @nodoc
	 */
	ATTRIB_FUNCTIONS( ATTRIB_DISPOSE, BOOL )
#if 0
; // semicolon needed for mkman (doc tool) parsing)
#endif
// Because mkman isn't aware that ATTRIB_FUNCTIONS changes access to "public", we do so
// explicitly here:
public:

};


// This class has been designed to improve performances, the disposal 
// state is stored in an array (i.e. not in an attribute anymore).

class DECL_BOOL ENTITY_DISP_LIST {
private:
	ENTITY_LIST _stored_entities;
	// The disposals are stored in an array, when it is full another
	// array is allocated, etc ...  Up to 1024*1024 disposals can be
	// stored.  Same thing for the logical values telling if a given
	// entity belongs to the tool or the blank.
	face_dispose** _disposals;
	logical** _on_blank;

	// The memory is allocated array by array, as soon as we need it.
	// So it is important to know how many entities have been already
	// stored.
	int _number_of_stored_entities_in_col;
	int _number_of_allocated_rows;

	void deallocate_array();

	void allocate_new_array();

public:
	ENTITY_DISP_LIST();
	~ENTITY_DISP_LIST();

	int lookup(ENTITY* this_ent) const;

	// Counts the number of entities in the list.
	int count() const;

	// Initializes the correct SPAposition in the internal list for next.
	void init() const;

	// Returns the next undeleted entry.  
	ENTITY* next() const;  

	void remove(ENTITY* this_ent);  

	ENTITY* operator[] (int i) const;  

	// Are we on the blank or on the tool?
	logical is_blank(const ENTITY* the_ent) const;

	// Returns the disposal value (returns FALSE if the_ent is not known).
	logical get_disposal(const ENTITY* the_ent, face_dispose& the_disp) const;

	// Sets the disposal.
	logical set_disposal(const ENTITY* the_ent, const face_dispose the_disp,
		const logical is_blank = TRUE);

	// Sets an entity, its disposal and the body it belongs to (blank/tool).
	logical add(ENTITY* the_ent, const face_dispose the_disp = unknown_disposal,
		const logical is_blank = TRUE);
};

#endif
