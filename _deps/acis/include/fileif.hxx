/*******************************************************************/
/*    Copyright (c) 1989-2020 by Spatial Corp.                     */
/*    All rights reserved.                                         */
/*    Protected by U.S. Patents 5,257,205; 5,351,196; 6,369,815;   */
/*                              5,982,378; 6,462,738; 6,941,251    */
/*    Protected by European Patents 0503642; 69220263.3            */
/*    Protected by Hong Kong Patent 1008101A                       */
/*******************************************************************/

#ifndef FILEIF_H
#define FILEIF_H

//---------------------------------------------------------------------
// Define the abstract base class which defines the interface that ACIS
// uses to save and restore ENTITY data.

// All ACIS save and restore operations use an object of this class to
// control the reading or writing of the data.  There are two main
// reasons for having this class:

// The first reason is to allow saving and restoring ENTITY data to
// targets other than a standard C stream file (ie. a FILE*).  To
// do this, you derive a new subclass from this one which implements
// reading and writing for the new target.

// The second reason is to allow saving and restoring unknown ENTITY
// data in binary format.  This is the reason that this class has so
// many virtual methods.  To support unknown ENTITY data, we tag the
// data with its type when it is written to a file.  This allows us to
// do something reasonable with the data when we load it back in even
// if we do not know what the data is.

// If you are deriving a new class to support a different kind of
// storage target, you should probably derive it from the BinaryFile
// class which is derived from this one and which is declared in
// binfile.hxx.  The BinaryFile class has a standard implementation
// for most of the virtual methods of this class which already take
// care of the details for saving and restoring the unknown ENTITY
// data.  You will only have to implement the actual read and write
// methods.

//---------------------------------------------------------------------

#include "acis.hxx"
#include "dcl_kern.h"
#include "logical.h"
#include <stddef.h>

/**
* @file fileif.hxx
 * @CAA2Level L1
 * @CAA2Usage U1
 * \addtogroup ACISSAVERESTORE
 *
 * @{
 */

class FileInterface;
class TaggedData;
class SPAposition;
class SPAvector;

class enum_table;

//---------------------------------------------------------------------

/**
 * @nodoc
 */
typedef int64_t FilePosition;

/**
 * Defines a data structure that describes the data in a save file.
 * Currently this data structure contains only the size of the save file.
 */
struct FileData 
{
	size_t size;  ///< The size in bytes of the save file.
};

//---------------------------------------------------------------------
// Pointer to the active FileInterface.  This performs the same funciton

/**
 * @nodoc
 */
#define ActiveFile GetActiveFile()

/**
 * Obtain the currently installed FileInterface.
 * <br><br>
 * <b>Role:</b> Obtain the currently installed FileInterface.
 * @see SetActiveFile, FileInterface
 */
DECL_KERN FileInterface* GetActiveFile( void );

/**
 * Install a new FileInterface.
 * <br><br>
 * <b>Role:</b> Install a new FileInterface.
 * @see GetActiveFile, FileInterface
 */
DECL_KERN void SetActiveFile( FileInterface * fi );

//---------------------------------------------------------------------
/**
 * Defines an abstract base class for saving and restoring ACIS ENTITY data.
 * <br><br>
 * <b>Role:</b> All <tt>ACIS</tt> save and restore operations use an object of this 
 * class to control the reading or writing of the data. There are two main reasons 
 * for having this class:
 * <br><br>
 * 1) To allow saving and restoring <tt>ENTITY</tt> data to targets other
 * than a standard C stream file; i.e., a <tt>FILE*</tt>.
 * <br><br>
 * 2) To allow saving and restoring unknown <tt>ENTITY</tt> data in binary
 * format. This is the reason that this class has so many virtual methods.
 * To support unknown <tt>ENTITY</tt> data, the data is tagged with its type when it is
 * written to a file. This allows manipulation of the data when it is loaded back
 * in even if the data is unknown.
 * <br><br>
 * We do not recommend deriving from this class directly. Instead, derive from 
 * <tt>BinaryFile</tt>. The <tt>BinaryFile</tt> class has a standard implementation 
 * for most of the virtual methods of this class that take care of the details 
 * for saving and restoring the unknown <tt>ENTITY</tt> data. Implementation of the 
 * actual read and write methods are all that is necessary. For additional information
 * on customing the save and restore behavior refer to the Technical Article 
 * <i>Customizing Save and Restore</i>.
 * <br><br>
 * @see GetActiveFile, SetActiveFile, BinaryFile
 */
class DECL_KERN FileInterface : public ACIS_OBJECT 
{
public:
/**
 * C++ constructor, creating a FileInterface.
 */
    FileInterface();
/**
 * C++ destructor, deleting a FileInterface.
 */
    virtual ~FileInterface();

    //-----------------------------------------------------------------
/**
 * Writes a logical.
 * <br><br>
 * <b>Role:</b> (<tt>false_string</tt>,<tt>true_string</tt>, {or <tt>any_valid_string</tt>}): Appropriate
 * string written with C printf format <tt>"%s"</tt>.
 * <br><br>
 * @param value
 * logical value
 * <br><br>
 * @param f
 * FALSE keyword.
 * @param t
 * TRUE keyword.
 */
    virtual void write_logical(logical value, const char* f="F", const char* t="T") = 0;
/**
 * Reads a logical.
 * <br><br>
 * <b>Role:</b> (<tt>false_string</tt>,  <tt>true_string</tt>, {or <tt> any_valid_string</tt>}): Appropriate
 * string written with C printf format <tt>"%s"</tt>.
 * <br><br>
 * @param f
 * FALSE keyword.
 * @param t
 * TRUE keyword.
 */
    virtual logical read_logical(const char* f="F", const char* t="T") = 0;
/**
 * Writes enumeration table.
 * <br><br>
 * <b>Role:</b> The <tt><identifier></tt> specifies which enumeration is active and its
 * valid values. The <tt><identifier></tt> is not written to the file. A valid value only
 * is written to the file. This is a character string or a long value from the
 * enumeration <tt><identifier></tt> written with C <tt>printf</tt> format <tt>"%s"</tt>.
 * <br><br>
 * @param value
 * value.
 * @param table
 * enumeration table.
 */
    virtual void write_enum(int value, enum_table const &table) = 0;
/**
 * Read an enumeration table.
 * <br><br>
 * <b>Role:</b> The <tt><identifier></tt> specifies which enumeration is active and its
 * valid values. The <tt><identifier></tt> is not written to the file. A valid value only
 * is written to the file. This is a character string or a long value from the
 * enumeration <tt><identifier></tt> written with C <tt>printf</tt> format <tt>"%s"</tt>.
 * <br><br>
 * @param table
 * enumeration table.
 */
    virtual int read_enum(enum_table const &table) = 0;
/**
 * Writes a character.
 * <br><br>
 * <b>Role:</b> Written with C <tt>printf</tt> format <tt>"%c"</tt>.
 * <br><br>
 * @param c
 * character.
 */
    virtual void write_char(char c) = 0;
/**
 * Reads a character.
 * <br><br>
 * <b>Role:</b> Written with C <tt>printf</tt> format <tt>"%c"</tt>.
 */
    virtual char read_char() = 0;
/**
 * Writes a short.
 * <br><br>
 * <b>Role:</b> Written with C <tt>printf</tt> format <tt>"%d"</tt>.
 * <br><br>
 * @param d
 * short.
 */
    virtual void write_short(short d) = 0;
/**
 * Reads a short.
 * <br><br>
 * <b>Role:</b> Written with C <tt>printf</tt> format <tt>"%d"</tt>.
 */
    virtual short read_short() = 0;
/**
 * Writes a long.
 * <br><br>
 * <b>Role:</b> Written with C <tt>printf</tt> format <tt>"%ld"</tt>.
 * <br><br>
 * @param ld
 * long.
 */
    virtual void write_long(long ld) = 0;
/**
 * Reads a long.
 * <br><br>
 * <b>Role:</b> Written with C <tt>printf</tt> format <tt>"%ld"</tt>.
 */
    virtual long read_long() = 0;
/**
 * Writes a float.
 * <br><br>
 * <b>Role:</b> Written with C <tt>printf</tt> format <tt>"%g"</tt>.
 * <br><br>
 * @param g
 * float
 */
    virtual void write_float(float g) = 0;
/**
 * Reads a float.
 * <br><br>
 * <b>Role:</b> Written with C <tt>printf</tt> format <tt>"%g "</tt>.
 */
    virtual float read_float() = 0;
/**
 * Writes a real.
 * <br><br>
 * <b>Role:</b> Written with C <tt>printf</tt> format <tt>"%g "</tt>.
 * <br><br>
 * @param d
 * double
 */
    virtual void write_double(double d) = 0;
/**
 * Reads a double.
 * <br><br>
 * <b>Role:</b> Written with C <tt>printf</tt> format <tt>"%g "</tt>.
 */
    virtual double read_double() = 0;
/**
 * Writes a string.
 * <br><br>
 * <b>Role:</b> Length written as long followed by string written with C <tt>printf</tt> format <tt>"%s"</tt>.
 * <br><br>
 * @param str
 * string.
 * @param len
 * length.
 */
    virtual void write_string(const char* str, size_t len=0) = 0;
/**
 * Reads a string.
 * <br><br>
 * <b>Role:</b> Allocates memory for it, and the argument returns the length of
 * the string. Length written as long followed by string written with C <tt>printf</tt>
 * format <tt>"%s"</tt>.
 * <br><br>
 * @param len
 * length.
 */
    virtual char* read_string(int& len) = 0;
/**
 * Reads a string.
 * <br><br>
 * <b>Role:</b> Allocates memory for it, and the argument returns the length of
 * the string. Length written as long followed by string written with C <tt>printf</tt>
 * format <tt>"%s"</tt>.
 * <br><br>
 * @param buf
 * buffer.
 * @param maxlen
 * maximum length.
 */
    virtual size_t read_string(char* buf, size_t maxlen = 0) = 0;

	// This is used to write a block of unknown text data
/**
 * Writes a literal string.
 * <br><br>
 * @param str
 * string.
 * @param len
 * length.
 */
	virtual void write_literal_string(const char* str, size_t len = 0);

    // These could have been implemented in terms of the basic funcitons,
    // but they are provided here just so that you can get a little more
    // information when you read unknown entity types from a binary file.
/**
 * Writes a position.
 * <br><br>
 * <b>Role:</b> x, y, z coordinates written as real numbers.
 * <br><br>
 * @param pos
 * position.
 */
    virtual void write_position(const SPAposition& pos);
/**
 * Reads the position.
 * <br><br>
 * <b>Role:</b> x, y, z coordinates written as real numbers.
 */
    virtual SPAposition read_position();
/**
 * Writes a vector.
 * <br><br>
 * <b>Role:</b> x, y, z components written as real numbers.
 * <br><br>
 * @param vec
 * vector name.
 */
    virtual void write_vector(const SPAvector& vec);
/**
 * Reads the vector.
 * <br><br>
 * <b>Role:</b> x, y, z components written as real numbers.
 */
    virtual SPAvector read_vector();

/**
 * Writes a wide character string.
 * <br><br>
 * <b>Role:</b> Write a null-terminated wide character string. The string will be encoded
 * into a single-byte string so it can be saved. (Encoding makes the output string unreadable.)
 * <br><br>
 * @param wstr
 * wide character string.
 */
	virtual void write_wstring( const SPAWCHAR_T* wstr );

/**
 * Reads a wide character string.
 * <br><br>
 * <b>Role:</b> Reads and returns a null-terminated wide character string.
 */
	virtual SPAWCHAR_T* read_wstring( void );

    // The following procedures are for writing specific kinds of data
    // which must be recognized when saving and restoring unkn\own
    // ENTITY types, so they cannot be implemented in terms of the
    // procedures for the basic types.
/**
 * Writes a pointer.
 * <br><br>
 * <b>Role:</b> Pointer reference to a save file record index. Written as <tt>"$"</tt>
 * followed by index number written as a long.
 * <br><br>
 * @param ptr
 * pointer.
 */
    virtual void write_pointer(void* ptr) = 0;
/**
 * Reads a pointer.
 * <br><br>
 * <b>Role:</b> Pointer reference to a save file record index. Written as <tt>"$"</tt>
 * followed by index number written as a long.
 */
    virtual void* read_pointer() = 0;
/**
 * Writes a header.
 * <br><br>
 * <b>Role:</b> The first record of the <tt>ACIS</tt> save file is a header, such as: 200 0 1 0.
 * <br><br>
 * <dl><dt>First Integer</dt><dd> An encoded version number. In the example, this is "200".
 * This value is 100 times the major version plus the minor version (e.g., 107
 * for <tt>ACIS</tt> version 1.7). For point releases, the final value is truncated.
 * Part save data for the .sat files is not affected by a point release
 * (e.g., 105 for <tt>ACIS</tt> version 1.5.2)</dd>
 * <dt>Second Integer</dt><dd> The total number of saved data records, or zero. If zero, then
 * there needs to be an end mark.</dd>
 * <dt>Third Integer</dt><dd> A count of the number of entities in the original entity list
 * saved to the part file.</dd>
 * <dt>Fourth Integer</dt><dd> The least significant bit of this number is used to indicate
 * whether or not history has been saved in this save file.</dd>
 * </dl>
 * <br><br>
 * @param i1
 * first integer.
 * @param i2
 * second integer.
 * @param i3
 * third integer.
 * @param i4
 * fourth integer.
 */
    virtual void write_header( int i1, int i2, int i3, int i4);
/**
 * Reads a header.
 * <br><br>
 * <b>Role:</b> The first record of the <tt>ACIS</tt> save file is a header, such as:
 * 200 0 1 0
 * <br><br>
 * <dl><dt>First Integer</dt><dd> An encoded version number. In the example, this is "200".
 * This value is 100 times the major version plus the minor version (e.g., 107
 * for <tt>ACIS</tt> version 1.7). For point releases, the final value is truncated.
 * Part save data for the .sat files is not affected by a point release
 * (e.g., 105 for <tt>ACIS</tt> version 1.5.2).</dd>
 * <dt>Second Integer</dt><dd> The total number of saved data records, or zero. If zero,
 * then there needs to be an end mark.</dd>
 * <br>
 * <dt>Third Integer</dt><dd> A count of the number of entities in the original entity list
 * saved to the part file.</dd>
 * <br>
 * <dt>Fourth Integer</dt><dd> The least significant bit of this number is used to indicate
 * whether or not history has been saved in this save file.</dd>
 * </dl>
 * <br><br>
 * @param i1
 * first integer.
 * @param i2
 * second integer.
 * @param i3
 * third integer.
 * @param i4
 * fourth integer.
 */
    virtual logical read_header( int &i1, int &i2, int &i3, int &i4 );
/**
 * Writes an identifier.
 * <br><br>
 * <b>Role:</b> The save identifier written with C <tt>printf</tt> format "%s ".
 * <br><br>
 * @param str
 * ID string.
 * @param level
 * ID level (1 or 2).
 */
    virtual void write_id( const char* str, int level) = 0;
/**
 * Reads an identifier.
 * <br><br>
 * <b>Role:</b> The save identifier written with C <tt>printf</tt> format "%s ".
 * <br><br>
 * @param str
 * IS string buffer.
 * @param length
 * buffer length.
 */
    virtual int read_id( char *str, int length= 0 ) = 0;
/**
 * Writes a sequence.
 * <br><br>
 * <b>Role:</b> Written as <tt>"-"</tt> followed by the entity index written as long.
 * <br><br>
 * @param i
 * integer.
 */
    virtual void write_sequence(int i);
/**
 * Reads a sequence.
 * <br><br>
 * <b>Role:</b> Written as <tt>"-"</tt> followed by the entity index written as long.
 */
    virtual int read_sequence();
/**
 * Writes a new line character.
 * <br><br>
 * @param count
 * number of new lines.
 */
    virtual void write_newline(int count = 1);

/**
 * Writes a subtype start.
 * <br><br>
 * <b>Role:</b> Braces around the subtypes, written as <tt>"{ "</tt> in the <tt>SAT</tt> file.
 */
    virtual void write_subtype_start() = 0;
/**
 * Reads subtype start.
 * <br><br>
 * <b>Role:</b> Braces around the subtypes, written as <tt>"{ "</tt> in the <tt>SAT</tt> file.
 */
    virtual logical read_subtype_start() = 0;
/**
 * Writes a subtype end.
 * <br><br>
 * <b>Role:</b> Braces around the subtypes, written as <tt>"} "</tt> in the <tt>SAT</tt> file.
 */
    virtual void write_subtype_end() = 0;
/**
 * Reads subtype end.
 * <br><br>
 * <b>Role:</b> Braces around the subtypes, written as <tt>"} "</tt> in the <tt>SAT</tt> file.
 */
    virtual logical read_subtype_end() = 0;
/**
 * Writes the data.
 * <br><br>
 * @param tdata
 * tagged data.
 */
    virtual void write_data(const TaggedData& tdata);
/**
 * Reads the data.
 */
    virtual TaggedData* read_data();

    //----------------------
    // Methods for positioning withing the file

/**
 * Sets the mark.
 */
    virtual FilePosition set_mark() = 0;
/**
 * Goes to the mark.
 * <br><br>
 * @param fp
 * file position.
 */
    virtual FilePosition goto_mark(FilePosition fp) = 0;

/**
 * Gets information about ACIS data contained in this <tt>FileInterface</tt> object.
 * <br><br>
 * <b>Role:</b> This function returns <tt>TRUE</tt> if the <tt>FileData</tt> object 
 * was successfully populated. Otherwise, it returns <tt>FALSE</tt>. The 
 * <tt>FileData</tt> object is created by and owned by the calling function. 
 * <br><br>
 * Customers that derive custom <tt>FileInterface</tt> class objects should overload 
 * this virtual method to supply the appropriate data. Currently, only the size of the 
 * data is queried, but more items may be added in the future. This method is particularly 
 * important when monitoring restore progress, because the data size is used in determining 
 * progress. When called, the <tt>FileData</tt> argument must be a valid pointer to an 
 * internal storage location where the retrieved values are stored.
 * <br><br>
 * @param fd
 * file data object.
 */
	virtual logical get_file_data( FileData* fd );

/**
 * Determines if unknown <tt>ENTITY</tt> types are OK.
 * <br><br>
 * <b>Role:</b> This returns <tt>TRUE</tt> for everything except old style binary files,
 * so it has a default implementation.
 */
    virtual logical unknown_types_ok();

/**
 * Returns a name for this <tt>FileInterface</tt>'s type.
 * <br><br>
 * <b>Role:</b> For the <tt>FileInterface</tt> class, <tt>unknown_fi</tt> is returned.
 */
	virtual const char *file_type_name() const;

/**
 * Returns TRUE if the <tt>FileInterface</tt> targets binary format.
 * <br><br>
 * <b>Role:</b> The <tt>FileInterface</tt> class inplementation returns FALSE.
 */
	virtual const logical binary_target() const;

// We don't recommend that customers call this. ACIS takes responsibility
// for calling write_terminator at the end of records. We also don't recommend
// that customers implement this method.
/**
 * @nodoc
 */
    virtual void write_terminator() = 0;

/**
 * @nodoc
 */
	virtual void write_summary( const char*);

/**
 * @nodoc
 */
	virtual char* read_summary( int&);

/**
 * @nodoc
 */
	virtual void clear();
};

//---------------------------------------------------------------------
/** @} */
#endif
