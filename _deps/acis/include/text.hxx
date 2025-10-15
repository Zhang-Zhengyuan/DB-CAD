/*******************************************************************/
/*    Copyright (c) 1989-2020 by Spatial Corp.                     */
/*    All rights reserved.                                         */
/*    Protected by U.S. Patents 5,257,205; 5,351,196; 6,369,815;   */
/*                              5,982,378; 6,462,738; 6,941,251    */
/*    Protected by European Patents 0503642; 69220263.3            */
/*    Protected by Hong Kong Patent 1008101A                       */
/*******************************************************************/
//      A TEXT_ENT entity is used to store simple text display information.
/*******************************************************************/
#ifndef text_hxx
#define text_hxx
#include "dcl_kern.h"
#include "entity.hxx"
#include "position.hxx"
/**
 * @file text.hxx
 * @CAA2Level L1
 * @CAA2Usage U1
 * \addtogroup ACISVISUALIZATION
 *
 * @{
 */
//----------------------------------------------------------------------
/**
 * @nodoc
 */
ENTITY_IS_PROTOTYPE(TEXT_ENT, KERN )
#if 0
; // semicolon needed for mkman (doc tool) parsing
#endif

//======================================================================

// Identifier used to find out (via identity() defined below) to what
// an entity pointer refers.

extern DECL_KERN int TEXT_ENT_TYPE;


// Identifier that gives number of levels of derivation of this class
// from ENTITY.

#define TEXT_ENT_LEVEL 1


// TEXT_ENT declaration proper.

/**
 * Stores simple text display information.
 * @see SPAposition
 */
class DECL_KERN TEXT_ENT: public ENTITY {

	// Location of baseline at start of first character.

	SPAposition Location;

	// String to be displayed.

	char *String;

	// Font style used to display string. (NULL -> use current default)

	char *Font;

	// Size (in points) of font used to display string. (<= 0 -> default)

	int Size;

	// Include the standard member functions for all entities.

	/**
	 * @nodoc
	 */
	ENTITY_FUNCTIONS( TEXT_ENT, KERN )
#if 0
; // semicolon needed for mkman (doc tool) parsing)
#endif
// Because mkman isn't aware that ENTITY_FUNCTIONS changes access to "public", we do so
// explicitly here:
public:

	/**
	 * @nodoc
	 */
	FULLSIZE_FUNCTION
#if 0
; // semicolon needed for mkman (doc tool) parsing)
#endif

	/**
	 * @nodoc
	 */
	TRANSFORM_FUNCTION
#if 0
; // semicolon needed for mkman (doc tool) parsing)
#endif

	/**
	 * @nodoc
	 */
	LOOKUP_FUNCTION
#if 0
; // semicolon needed for mkman (doc tool) parsing)
#endif

	void internal_set_string(const char*);
	void internal_set_font_name(const char*);

public:

    //----------------------------------------
    // Constructors

    /**
     * C++ allocation constructor requests memory for this object but does not populate it.
     * <br><br>
     * <b>Role:</b> The allocation constructor is used primarily by restore.
     * Applications should call this constructor only with the overloaded <tt>new</tt>
     * operator, because this reserves the memory on the heap, a requirement to
     * support roll back and history management.
     */
	TEXT_ENT();

	/**
     * C++ initialize constructor requests memory for this object and populates it with the data supplied as arguments.
     * <br><br>
     * <b>Role:</b> Applications should call this constructor only with the overloaded
     * new operator, because this reserves the memory on the heap, a requirement
     * to support roll back and history management.
     * <br><br>
     * @param loc
     * location of text.
     * @param string
     * string of text to be displayed.
     * @param font
     * size of font.
     * @param size
     * size in points of font.
     */
	TEXT_ENT(
		const SPAposition&loc,
		const char *string,
		const char *font = NULL,
		int size = -1
		);

    //----------------------------------------
    // Destructor
    //  The destructor of TEXT_ENT is private.  To delete a TEXT_ENT entity, use the
	//	lose method.


    //----------------------------------------
    // Inquiry Functions
    /**
     * Gets the location of the text.
     */
	SPAposition location() const { return Location; }
    /**
     * Gets the text string.
     */
	const char *string() const { return String; }
    /**
     * Gets the name of the text font.
     */
	const char *font_name() const { return Font; }
    /**
     * Gets the size of the font.
     */
	int font_size() const { return Size; }

    //----------------------------------------
    // Change Functions

	/**
     * Indicates that the tolerance needs to be recalculated.
     * <br><br>
     * @param loc
     * location.
     */
	void set_location( const SPAposition& loc );
	/**
     * Sets the size of the font.
     * <br><br>
     * @param size
     * size of font in points.
     */
	void set_font_size( int size );
    /**
     * Changes the text string.
     * <br><br>
     * @param string
     * text string.
     */
	void set_string( const char * string );
    /**
     * Sets the text font.
     * <br><br>
     * @param font
     * text font.
     */
	void set_font_name( const char * font);
    /**
     * Changes the location of text by applying a transformation.
     * <br><br>
     * @param transf
     * transformation .
     */
	void operator*=( const SPAtransf& transf);
};
/** @} */
/**
 * \addtogroup ACISQUERIES
 * @{
 */
/**
* Determines if an <tt>ENTITY</tt> is a <tt>TEXT_ENT</tt>.
* <br><br>
* <b> Effect:</b> Read-only
* <br><br>
* @param ent
* entity to test.
**/

DECL_KERN logical is_TEXT_ENT(const ENTITY* ent);

//======================================================================
/** @} */
#endif
