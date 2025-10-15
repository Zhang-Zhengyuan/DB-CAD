/* ORIGINAL: acis2.1/kernutil/fileio/tagdata.hxx */
/* $Id: tagdata.hxx,v 1.12 2002/08/09 17:15:31 jeff Exp $ */
/*******************************************************************/
/*    Copyright (c) 1989-2020 by Spatial Corp.                     */
/*    All rights reserved.                                         */
/*    Protected by U.S. Patents 5,257,205; 5,351,196; 6,369,815;   */
/*                              5,982,378; 6,462,738; 6,941,251    */
/*    Protected by European Patents 0503642; 69220263.3            */
/*    Protected by Hong Kong Patent 1008101A                       */
/*******************************************************************/

#ifndef TAGDATA_H
#define TAGDATA_H

//---------------------------------------------------------------------
// Define the class TaggedData which is used to keep track of data and
// its type for saving and restoring unknown ENTITY types.
//---------------------------------------------------------------------

#include "dcl_kern.h"
// ywoo 08Jan01: added the header file.
#include "mmgr.hxx"
// ywoo: end

#include "entity.hxx"
/**
* @file tagdata.hxx
 * @CAA2Level L1
 * @CAA2Usage U1
 * \addtogroup ACISSAVERESTORE
 *
 * @{
 */
class SPAposition;
class SPAvector;

//---------------------------------------------------------------------
/**
 *  Tracks data and its type for saving and restoring unknown <tt>ENTITY</tt> types.
 * <br>
 * <b>Role:</b> This class reads in data from unknown <tt>ENTITY</tt> types. It stores
 * the data and its type so that it can be written back out in binary mode.
 */
class DECL_KERN TaggedData : public ACIS_OBJECT {

public:

// NOTE: The values of the items in this enumeration are written to a
// binary save file to identify the type of data items in an unknown ENTITY.
// Because these values are used when restoring unknown ENTITY data, they
// are given explicitly and must not be changed.
/**
 * The type of data.
 * <br><br>
 * <b>Role:</b> The values of the items in this enumeration are written to a binary
 * save file to identify the type of data items in an unknown <tt>ENTITY</tt>. Because these
 * values are used when restoring unknown <tt>ENTITY</tt> data, they are given explicitly
 * and must not be changed.
 *<br><br>
 * Data type tags:
 *<br><br>
 * @param no_type
 * 0.
 * @param byte_type
 * 1.
 * @param char_type
 * 2.
 * @param short_type
 * 3.
 * @param long_type
 * 4.
 * @param float_type
 * 5.
 * @param double_type
 * 6.
 * @param short_char_string
 * 7.
 * @param char_string
 * 8.
 * @param long_char_string
 * 9.
 * @param logical_true
 * 10.
 * @param logical_false
 * 11.
 * @param pointer_type
 * 12.
 * @param level_1_id
 * 13.
 * @param level_2_id
 * 14.
 * @param subtype_start
 * 15.
 * @param subtype_end
 * 16.
 * @param terminator
 * 17.
 * @param literal_char_string
 * 18.
 * @param position_type
 * 19.
 * @param vector_type
 * 20.
 * @param enum_type
 * 21.
 */
enum DataType {
    no_type				= 0,
    byte_type			= 1,
    char_type			= 2,
    short_type			= 3,
    long_type			= 4,
    float_type			= 5,
    double_type			= 6,
    short_char_string	= 7,
    char_string			= 8,
    long_char_string	= 9,
    logical_true		= 10,
    logical_false		= 11,
    pointer_type		= 12,
    level_1_id			= 13,
    level_2_id			= 14,
    subtype_start		= 15,
    subtype_end			= 16,
    terminator			= 17,
    literal_char_string	= 18,
    position_type		= 19,
    vector_type			= 20,
	enum_type			= 21
};

    DataType m_dataType;
    /**
	     * A union with  data types.
	     *<br><br>
	     * @param unsigned char
	     * uc.
	     * @param char
	     * c.
	     * @param short
	     * s.
	     * @param long
	     * l.
	     * @param float
	     * f.
	     * @param double
	     * d.
	     * @param char
	     * pc.
	     * @param void
	     * pv.
	     * @param SPAposition
	     * pos.
	     * @param SPAvector
	     * vec.
     */
    union {
        unsigned char uc;
        char c;
        short s;
        long l;
        float f;
        double d;
        char* pc;
        void* pv;
        SPAposition* pos;
        SPAvector* vec;
    } m_data;

    /*
    // tbrv
    */

    /**
     * @nodoc
     */
    logical m_owns_ptr;

	// GSSL RNC BEGINS.
	/*
	// tbrv
	*/

	/**
	 * @nodoc
	 */

	tag_id_type m_ent_id;
	// GSSL RNC ENDS.


public:
    /**
     * C++ copy constructor requests memory for this object and populates it with the data from the object supplied as an argument.
     *<br><br>
     * @param tag
     * Tagged Data.
     */
    TaggedData(const TaggedData& tag);
    /**
     * C++ constructor, creating a TaggedData using an unsigned character value.
     *<br><br>
     * @param str
     */
    TaggedData(unsigned char str);
    /**
     * C++ constructor, creating a TaggedData using a single character.
     *<br><br>
     * @param str
     * single character.
     */
    TaggedData(char str);
    /**
     * C++ constructor, creating a TaggedData using a short value.
     *<br><br>
     * @param val
     * short value.
     */
    TaggedData(short val);
    /**
     * C++ constructor, creating a TaggedData using an integer value.
     *<br><br>
     * @param val
     * integer value.
     */
    TaggedData(int val);
    /**
	 * C++ constructor, creating a TaggedData using a long value.
	 *<br><br>
	 * @param val
	 * long value.
     */
    TaggedData(long val);
    /**
	 * C++ constructor, creating a TaggedData using a floating value.
	 *<br><br>
	 * @param val
	 * float value.
     */
    TaggedData(float val);
    /**
	 * C++ constructor, creating a TaggedData using a real value.
	 *<br><br>
	 * @param val
	 * double value.
     */
    TaggedData(double val);
    /**
     * C++ constructor, creating a TaggedData using a position.
     *<br><br>
     * @param pos
     * position.
     */
    TaggedData(const SPAposition& pos);
    /**
     * C++ constructor, creating a TaggedData using a vector.
     *<br><br>
     * @param vec
     * vector.
     */
    TaggedData(const SPAvector& vec);

    // The TaggedData object assumes ownership of the character string
    // and deletes it when it is deleted.  The string MUST be allocated
    // on the heap.
    /**
     * C++ constructor, creating a TaggedData using a string.
     *<br><br>
     * @param str
     * string.
     */
    TaggedData(char* str);
    /**
     * C++ constructor, creating a TaggedData using the specified parameters.
     *<br><br>
     * @param data
     * data type.
     * @param p
     * pointer to data.
     */
    TaggedData(DataType data, void* p=0);
    /**
     * C++ destructor, deleting a TaggedData.
     *<br><br>
     * <b>Role:</b> The character strings are deleted when TaggedData is deleted.
     * For this reason, all character strings that are given to it must be allocated
     * on the heap.
     */
    ~TaggedData();

    // Write debug info
    /**
     * Writes debug information about the TaggedData to standard output or to the specified file.
     *<br><br>
     * @param filename
     * file name.
     */
    void debug_value( FILE* filename) const;

    // Get a description of the data type
    /**
     * Returns the string data type, that is, <tt>"char_type"</tt>, <tt>"no_type"</tt>.
     */
    const char* type_name() const;
};

//---------------------------------------------------------------------
/** @} */
#endif
