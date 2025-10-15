// $Id: textannotation.hxx,v 1.7 2001/03/18 18:31:16 goetting Exp $
/*******************************************************************/
/*    Copyright (c) 1989-2020 by Spatial Corp.                     */
/*    All rights reserved.                                         */
/*    Protected by U.S. Patents 5,257,205; 5,351,196; 6,369,815;   */
/*                              5,982,378; 6,462,738; 6,941,251    */
/*    Protected by European Patents 0503642; 69220263.3            */
/*    Protected by Hong Kong Patent 1008101A                       */
/*******************************************************************/
//-------------------------------------------------------------------
// purpose---
//   A simple annotation attribute for testing annotations
//-------------------------------------------------------------------


#ifndef __TEXTANNOTATION_HXX__
#define __TEXTANNOTATION_HXX__

#include "annotation.hxx"
/**
 * @file textannotation.hxx
 * @CAA2Level L1
 * @CAA2Usage U2
 * \addtogroup ACISANNOTATIONS
 *
 * @{
 */
/**
 * @nodoc
 */
ENTITY_IS_PROTOTYPE(TEXT_ANNOTATION, KERN)
#if 0
; // semicolon needed for mkman (doc tool) parsing
#endif

extern DECL_KERN int TEXT_ANNOTATION_TYPE;
#define TEXT_ANNOTATION_LEVEL ANNOTATION_LEVEL + 1

/**
 * Defines a simple annotation attribute for testing annotations.
 */
class DECL_KERN TEXT_ANNOTATION : public ANNOTATION
{
	char* m_string;
protected:
	/**
     * Sets the string for a <tt>TEXT_ANNOTATION</tt> entity without backing up.
     */
	void internal_set_string(const char* s);

	// STI ROLL
	/**
	 * @nodoc
	 */
	void full_size(SizeAccumulator& est) const;
    // STI ROLL

public:
	/**
	 * @nodoc
	 */
	ENTITY_FUNCTIONS(TEXT_ANNOTATION, KERN)
#if 0
; // semicolon needed for mkman (doc tool) parsing)
#endif
// Because mkman isn't aware that ENTITY_FUNCTIONS changes access to "public", we do so
// explicitly here:
public:

	/**
	 * @nodoc
	 */
	ANNOTATION_MAP_DECL_NO_ENTS()
#if 0
; // semicolon needed for mkman (doc tool) parsing)
#endif
	ANNOTATION_EXTRA_DATA_FUNCTIONS(TEXT_ANNOTATION)


// Public functions to doc.

    /**
     * C++ allocation constructor requests memory for this object but does not populate it.
     * <br><br>
     * <b>Role:</b> Applications should call this constructor only with the overloaded <tt>new</tt> operator,
	 * because this reserves the memory on the heap, a requirement to support roll back and history
	 * management.
     */
	TEXT_ANNOTATION() : m_string(NULL) {}
	/**
     * C++ initialize constructor requests memory for this object and populates it with the data supplied as arguments.
     * <br><br>
     * <b>Role:</b> Applications should call this constructor only with the overloaded <tt>new</tt> operator,
	 * because this reserves the memory on the heap, a requirement to support roll back and history
	 * management.
	 * <br><br>
     * @param s
     * text to use.
     */
	TEXT_ANNOTATION(const char* s) { internal_set_string(s); }
	/**
     * Stores a text string as annotation data.
     * <br><br>
     * @param s
     * text string .
     */
	void set_string(const char* s) { backup(); internal_set_string(s); }
    /**
     * Returns the string that is stored as the annotation data.
     */
	const char* string() const     { return m_string; }
};

/** @} */
#endif //__TEXTANNOTATION_HXX__
