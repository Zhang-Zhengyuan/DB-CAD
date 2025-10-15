/*******************************************************************/
/*    Copyright (c) 1989-2020 by Spatial Corp.                     */
/*    All rights reserved.                                         */
/*    Protected by U.S. Patents 5,257,205; 5,351,196; 6,369,815;   */
/*                              5,982,378; 6,462,738; 6,941,251    */
/*    Protected by European Patents 0503642; 69220263.3            */
/*    Protected by Hong Kong Patent 1008101A                       */
/*******************************************************************/

#ifndef SATFILE_H
#define SATFILE_H

//---------------------------------------------------------------------
// Define the SatFile class for doing ACIS save and restore to stream
// files in text format.

//---------------------------------------------------------------------

#include <stdio.h>
#include "dcl_kern.h"
#include "fileif.hxx"
#include "hashstr.hxx"

#include "option.hxx"

/**
* @file satfile.hxx
 * @CAA2Level L1
 * @CAA2Usage U1
 * \addtogroup ACISSAVERESTORE
 *
 * @{
 */

//---------------------------------------------------------------------
/**
 * Defines the SatFile class for doing ACIS save and restore to stream files in text format.
 */
class DECL_KERN SatFile : public SummaryFileInterface {

private:
    FILE* m_pFILE;
	int read(void* buf, size_t length);
    virtual void write(const void*, size_t);

public:
/**
 * C++ constructor, creating a SatFile using the specified parameters.
 * @param fp
 * file name
 */
    SatFile(FILE* fp);
/**
 * C++ destructor, deleting a SatFile.
 */
    virtual ~SatFile();

    //--------------------
    // virtual methods from FileInterface
/**
 * Writes a logical.
 * <br>
 * <b>Role</b>: (false_string, true_string, {or any_valid_string}): Appropriate
 * string written with C printf format "%s ".
 * @param flag
 * logical
 * @param f
 * character
 * @param t
 * character
 */
    virtual void write_logical(logical flag, const char* f="F", const char* t="T");
/**
 * Reads a logical.
 * <br>
 * <b>Role</b>: (false_string, true_string {or any_valid_string}): Appropriate
 * string written with C printf format "%s ".
 * @param f
 * title
 * @param t
 * title
 */
    virtual logical read_logical(const char* f="F", const char* t="T");
/**
 * Writes enumeration table.
 * <br>
 * <b>Role</b>: The <identifier> specifies which enumeration is active and its valid
 * values. The <identifier> is not written to the file. A valid value only is
 * written to the file. This is a character string or a long value from the
 * enumeration <identifier> written with C printf format "%s".
 * @param num
 * number in
 * @param tab
 * enumeration table
 */
	virtual void write_enum(int num, enum_table const &tab);
/**
 * Read an enumeration table.
 * <br>
 * <b>Role</b>: The <identifier> specifies which enumeration is active and its
 * valid values. The <identifier> is not written to the file. A valid value only
 * is written to the file. This is a character string or a long value from the
 * enumeration <identifier> written with C printf format "%s".
 * @param tab
 * enumeration table
 */
	virtual int read_enum(enum_table const &tab);
/**
 * Writes a character.
 * <br>
 * <b>Role</b>: Written with C printf format "%c".
 * @param c
 * character
 */
    virtual void write_char(char c);
/**
 * Reads a character.
 * <br>
 * <b>Role</b>: Written with C printf format "%c".
 */
    virtual char read_char();
/**
 * Writes a short.
 * <br>
 * <b>Role</b>: Written with C printf format "%d".
 * @param val
 * short
 */
    virtual void write_short(short val);
/**
 * Reads a short.
 * <br>
 * <b>Role</b>: Written with C printf format "%d".
 */
    virtual short read_short();
/**
 * Writes a long.
 * <br>
 * <b>Role</b>: Written with C printf format "%ld".
 * @param val
 * long
 */
    virtual void write_long(long val);
/**
 * Reads a long.
 * <br>
 * <b>Role</b>: Written with C printf format "%ld".
 */
    virtual long read_long();
/**
 * Writes a float.
 * <br>
 * <b>Role</b>: Written with C printf format "%g ".
 * @param val
 * float
 */
    virtual void write_float(float val);
/**
 * Reads a float.
 * <br>
 * <b>Role</b>: Written with C printf format "%g ".
 */
    virtual float read_float();
/**
 * Writes a real.
 * <br>
 * <b>Role</b>: Written with C printf format "%g ".
 * @param val
 * parameter
 */
    virtual void write_double(double val);
/**
 * Reads a double.
 * <br>
 * <b>Role</b>: Written with C printf format "%g ".
 */
    virtual double read_double();

/**
 * Writes a string.
 * <br>
 * <b>Role</b>: Length written as long followed by string written with C printf
 * format "%s".
 * @param c
 * character
 * @param len
 * length
 */
    virtual void write_string(const char* c, size_t len=0);
/**
 * Reads a string, allocates memory for it, and the argument returns the length of the string.
 * <br>
 * <b>Role</b>: Length written as long followed by string written with C printf format "%s".
 * @param len
 * integer
 */
    virtual char* read_string(int& len);
/**
 * Reads a string into a supplied buffer of a given size, maxlen.
 * @param buf
 * buffer
 * @param maxlen
 * maximum length
 */
    virtual size_t read_string(char* buf, size_t maxlen = 0);
/**
 * Writes a literal string.
 * @param c
 * character
 * @param len
 * length
 */
	virtual void write_literal_string(const char* c, size_t len = 0);

    // The following procedures are for writing specific kinds of data
    // which must be recognized when saving and restoring unknown
    // ENTITY types, so they cannot be implemented in terms of the
    // procedures for the basic types.
/**
 * Writes a pointer.
 * <br>
 * <b>Role</b>: Pointer reference to a save file record index. Written as "$"
 * followed by index number written as a long.
 * @param param
 * parameter
 */
    virtual void write_pointer(void* param);
/**
 * Reads a pointer.
 * <br>
 * <b>Role</b>: Pointer reference to a save file record index. Written as "$"
 * followed by index number written as a long.
 */
    virtual void* read_pointer();
/**
 * Writes a header.
 * <br>
 * <b>Role</b>: The first record of the ACIS save file is a header, such as: 200 0 1 0
 * <br>
 * First Integer: An encoded version number. In the example, this is "200".
 * This value is 100 times the major version plus the minor version (e.g., 107
 * for ACIS version 1.7). For point releases, the final value is truncated.
 * Part save data for the .sat files is not affected by a point release
 * (e.g., 105 for ACIS version 1.5.2).
 * <br>
 * Second Integer: The total number of saved data records, or zero. If zero, then
 * there needs to be an end mark.
 * <br>
 * Third Integer: A count of the number of entities in the original entity list
 * saved to the part file.
 * <br>
 * Fourth Integer: The least significant bit of this number is used to indicate
 * whether or not history has been saved in this save file.
 * @param val1
 * first integer
 * @param val2
 * second integer
 * @param val3
 * third integer
 * @param val4
 * fourth integer
 */
    virtual void write_header( int val1, int val2, int val3, int val4);
/**
 * Reads a header.
 * <br>
 * <b>Role</b>: The first record of the ACIS save file is a header, such as: 200 0 1 0
 * <br>
 * First Integer: An encoded version number. In the example, this is "200".
 * This value is 100 times the major version plus the minor version (e.g., 107
 * for ACIS version 1.7). For point releases, the final value is truncated.
 * Part save data for the .sat files is not affected by a point release
 * (e.g., 105 for ACIS version 1.5.2).
 * <br>
 * Second Integer: The total number of saved data records, or zero. If zero, then
 * there needs to be an end mark.
 * <br>
 * Third Integer: A count of the number of entities in the original entity list
 * saved to the part file.
 * <br>
 * Fourth Integer: The least significant bit of this number is used to indicate
 * whether or not history has been saved in this save file.
 * @param val1
 * first integer
 * @param val2
 * second integer
 * @param val3
 * third integer
 * @param val4
 * fourth integer
 */
    virtual logical read_header( int &val1, int &val2, int &val3, int &val4 );
/**
 * Writes an identifier.
 * <br>
 * <b>Role</b>: The save identifier written with C printf format "%s ".
 * @param c
 * character
 * @param val
 * integer
 */
    virtual void write_id( const char* c, int val);
/**
 * Reads an identifier.
 * <br>
 * <b>Role</b>: The save identifier written with C printf format "%s ".
 * @param title
 * title
 * @param val
 * integer
 */
    virtual int read_id( char * title, int val= 0 );
/**
 * Writes a sequence.
 * <br>
 * <b>Role</b>: Written as "-" followed by the entity index written as long.
 * @param val
 * integer
 */
    virtual void write_sequence(int val);
/**
 * Reads a sequence.
 * <br>
 * <b>Role</b>: Written as "-" followed by the entity index written as long.
 */
    virtual int read_sequence();
/**
 * Writes a new line.
 * @param newl
 * number of newlines
 */
    virtual void write_newline(int newl= 1);
/**
 * Writes a terminator.
 * <br>
 * <b>Role</b>: Written as "#".
 */
    virtual void write_terminator();
/**
 * Writes a subtype start.
 * <br>
 * <b>Role</b>: Braces around the subtypes, written as "{ ".
 */
    virtual void write_subtype_start();
/**
 * Reads subtype start.
 * <br>
 * <b>Role</b>: Braces around the subtypes, written as "{ ".
 */
    virtual logical read_subtype_start();
/**
 * Writes a subtype end.
 * <br>
 * <b>Role</b>: Braces around the subtypes, written as "} ".
 */
    virtual void write_subtype_end();
/**
 * Reads subtype end.
 * <br>
 * <b>Role</b>: Braces around the subtypes, written as "} ".
 */
    virtual logical read_subtype_end();
/**
 * Reads the data for an unknown ENTITY until the end of record terminator is reached.
 */
    virtual TaggedData* read_data();

    //----------------------
    // Nethods for positioning withing the file
/**
 * Returns the current file position within the SatFile.
 */
    virtual FilePosition set_mark();
/**
 * Moves the file pointer to the specified pointer in the SatFile.
 * @param fpos
 * file position
 */
    virtual FilePosition goto_mark(FilePosition fpos);

	// get information about the file
/*
// tbrv
*/
/**
 * @nodoc
 */
	virtual logical get_file_data( FileData* );

	
/**
 * Returns the string "satfile".
 */
	virtual const char *file_type_name() const;

	// Targets test format
	virtual const logical binary_target() const { return FALSE; }

};

//---------------------------------------------------------------------
/** @} */
#endif
