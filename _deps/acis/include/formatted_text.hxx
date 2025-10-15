/*******************************************************************/
/*    Copyright (c) 1989-2020 by Spatial Corp.                     */
/*    All rights reserved.                                         */
/*    Protected by U.S. Patents 5,257,205; 5,351,196; 6,369,815;   */
/*                              5,982,378; 6,462,738; 6,941,251    */
/*    Protected by European Patents 0503642; 69220263.3            */
/*    Protected by Hong Kong Patent 1008101A                       */
/*******************************************************************/

#ifndef _FORMATTED_TEXT_HXX
#define _FORMATTED_TEXT_HXX

#include "acis.hxx"
#include "api.hxx"
#include "dcl_cstr.h"

class formatted_text_impl;
class font_ref;
/**
* @file formatted_text.hxx
**/

/*! \addtogroup CSTRMAKEWBODYAPI
 *  \brief Defined at <cstrapi.hxx>, SPAcstr
 *  @{
 */

/**
* A Text class that represents a sequence of characters and a font format
* that governs the shape of these characters.
**/

class DECL_CSTR formatted_text : public ACIS_OBJECT
{
public:

	/**
	* A class to represents the font specified by the user.
	* The object has full ownership of the underlying data and <b>must</b> be made on the stack.
	**/

	class font;

	/**
	* Primary font creation routine.
	* <br>
	* <b>Note:</b> If the specified font is not found on the device, or is invalid, 
	* this routine will throw an error.
	**/

	static font make_font(	const char*		font_name,
							unsigned		font_style = FS_DEFAULT,
							unsigned		flags = 1); // Bit 0 is on, throw a sys_error. Bit 0 is off, throw a sys_warning.

	/** 
	* Constructor for the formatted_text object. 
	* @param input_str
	* a sequence of characters that represent the input string
	* @param font_ref
	* a reference to a font object created using a call to make_font.
	**/

	formatted_text( const char* input_str, const font & font_ref, double height = 1.0 );

	/** 
	* Constructor for a wide-character formatted_text object. 
	* @param input_str
	* a sequence of wide characters that represent the input string
	* @param font_ref
	* a reference to a font object created using a call to make_font.
	**/

	formatted_text( const SPAWCHAR_T* input_str, const font & font_ref, double height = 1.0 );

	~formatted_text();

	/**
	* An enumeration to provide the font style.
	* <br>
	* A combination of these properties can be selected by doing a <tt>bitwise OR</tt>
	* of the values and passing it as an argument to the <tt>make_font</tt> routine.
	**/

	enum font_style
	{
		FS_DEFAULT = 0,
		FS_BOLD = 1, 
		FS_ITALIC = 2,
	};

private:

	formatted_text_impl* _impl;

	friend class formatted_text_query;
};

/**
* @nodoc
* A class to represents the font specified by the user.
* The object has full ownership of the underlying data and <b>must</b> be made on the stack.
**/

class DECL_CSTR formatted_text::font 
{
public:

	/**
	* @nodoc
	* Copy constructor.
	**/

	font( const font & );

	/**
	* @nodoc
	* Destructor.
	**/

	~font( );

private:

	MMGR_FREELIST_THIS

	// Class to hold the data

	class font_data;

	// Prevent assignment.

	font & operator=( const font & );

	// The object can only be constructed using a call to formatted_text::make_font.

	font( font_data* );

	font_data* _data;

	friend class formatted_text;
	friend class formatted_text_query;
};
/*! @} */
//====================================================================================

#endif

