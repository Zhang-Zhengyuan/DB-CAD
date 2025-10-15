/*******************************************************************/
/*    Copyright (c) 1989-2020 by Spatial Corp.                     */
/*    All rights reserved.                                         */
/*    Protected by U.S. Patents 5,257,205; 5,351,196; 6,369,815;   */
/*                              5,982,378; 6,462,738; 6,941,251    */
/*    Protected by European Patents 0503642; 69220263.3            */
/*    Protected by Hong Kong Patent 1008101A                       */
/*******************************************************************/

#ifndef BINARYFILE_H
#define BINARYFILE_H

//---------------------------------------------------------------------
// Define the BinaryFile class for doing ACIS save and restore to
// binary files.  This is an abstract base class.  It implements most
// of the virtual methods which are used by all of the binary file
// formats.

// If you want to save and restore ACIS ENTITY data in binary form
// to a target other than a FILE*, then you probably want to derive
// a new class from this one rather than directly from FileInterface.

//---------------------------------------------------------------------

#include <stdio.h>
#include "dcl_kern.h"
#include "fileif.hxx"
#include "tagdata.hxx"
#include "logical.h"
#include "hashstr.hxx"

/**
* @file binfile.hxx
 * @CAA2Level L1
 * @CAA2Usage U2
 * \addtogroup ACISSAVERESTORE
 *
 * @{
 */

class unknown_ascii_data;
class FileInterface;
class restore_data;

//---------------------------------------------------------------------
/**
 * Defines the <tt>BinaryFile</tt> class for doing ACIS save and restore to binary files.
 * <br>
 * <b>Role:</b> This is an abstract base class. It implements most of the virtual methods
 * which are used by all of the binary file formats.
 * <br><br>
 * If there is a need to save and restore ACIS <tt>ENTITY</tt> data in binary form to a target
 * other than a <tt>FILE*</tt>, then it is advisable to derive a new class from this class rather 
 * than directly from <tt>FileInterface</tt>. For additional information on customing the save 
 * and restore behavior refer to the Technical Article <i>Customizing Save and Restore</i>.
 */
class DECL_KERN BinaryFile : public SummaryFileInterface 
{
	// Thread-hot save and restore call the read and write methods directly.
	friend class RestoreMemoryFile;
	friend class SaveMemoryFile;

private:

	unknown_ascii_data* m_pUnknownData;

protected:

	// These two data members were in SabFile, but were promoted
	// to this class to allow other derived types to use them.
	/**
	 * Big/little-endian flag.
	 */
	logical big_end;	// Denotes if current platform is big endian
	/**
	 * Flag to indicate that big/little-endian byte swapping is needed.
	 */
	logical need_swap;	// Denotes if reading or writing requires
						// big-little endian byte swapping.

	// For transferring between 32-bit and 64-bit platforms
	/**
	 * Length of a long (in bytes) during read operations.
	 */
	int read_long_size;		// length of longs in bytes (to read them)
	/**
	 * Length of a long (in bytes) during write operations.
	 */
	int write_long_size;	// length of longs in bytes (to write them)

    // These are the virtual methods which actually do the reading
    // and writing of the data.  They must be implemented for all
    // derived classes.

/**
 * Reads data.
 * <br><br>
 * <b>Role:</b> This method must be implemented for each class derived from <tt>BinaryFile</tt>.
 * <br><br>
 * @param buffer
 * the read buffer.
 * @param length
 * the number of bytes to be read.
 * @param swap
 * specifies whether byte swapping is supported.
 */
    virtual size_t read(void* buffer, size_t length, logical swap) = 0;
/**
 * Writes the given data.
 * <br><br>
 * <b>Role:</b> This method must be implemented for each class derived from <tt>BinaryFile</tt>.
 * <br><br>
 * @param buffer
 * the write buffer.
 * @param length
 * the number of bytes to be written.
 * @param swap
 * specifies whether byte swapping is supported.
 */
    virtual void write(const void* buffer, size_t length, logical swap) = 0;

public:

	/**
	 * Method for read-only access to <tt>big_end</tt> data member.
	 */
	logical is_big_endian( void );

	/**
	 * Method for read-only access to <tt>need_swap</tt> data member.
	 */
	logical is_byte_swapping( void );

    // These two methods for repositioning the file pointer must
    // also be implemented for each class derived from BinaryFile.
    // In a normal save in which you do not write the ENTITY count
    // to the ACIS header, they are only used to reposition the
    // file pointer if there was an error reading the header - which
    // usually means that you did not have an ACIS part file.
/**
 * Returns the current file pointer position.
 * <br><br>
 * <b>Role:</b> This method must be implemented for each class derived from <tt>BinaryFile</tt>.
 * In a normal save that does not require writing the <tt>ENTITY</tt> count to the ACIS header,
 * it is only used for repositioning the file pointer if there is an error reading the header.
 */
    virtual FilePosition set_mark() = 0;

/**
 * Repositions the file pointer.
 * <br><br>
 * <b>Role:</b> This method must be implemented for each class derived from <tt>BinaryFile</tt>.
 * In a normal save that does not require writing the <tt>ENTITY</tt> count to
 * the ACIS header, it is only used to reposition the file pointer if there is an
 * error reading the header.
 * <br><br>
 * @param file_pos
 * new file position.
 */
    virtual FilePosition goto_mark(FilePosition file_pos) = 0;

public:

/**
 * Constructs a <tt>BinaryFile</tt> (default constructor).
 */
    BinaryFile();

/**
 * C++ destructor for <tt>BinaryFile</tt> which deallocates memory.
 */

    virtual ~BinaryFile();

    //===============================================================
    // The remaining methods are implemented in BinaryFile in terms of
    // the preceeding methods, and probably do not need to be overloaded
    // in derived classes.

protected:
/**
 * Writes tagged data.
 * <br><br>
 * @param type
 * the data type to be written.
 * @param data
 * the data to be written.
 * @param size
 * the number of bytes to be written.
 * @param swap
 * specifies whether byte swapping is supported.
 */
    virtual void write_tagged(TaggedData::DataType type,
                              const void* data,
                              size_t size,
							  logical swap);

/**
 * Reads a specific data type.
 */
    virtual TaggedData::DataType read_type();

/**
 * Reads the next data type tag from the file, regardless of what it is.
 * <br><br>
 * <b>Role:</b> The return type is <tt>TaggedData::DataType</tt>. This method is a virtual
 * method of <tt>BinaryFile</tt>.
 */
    virtual TaggedData::DataType read_data_type();

	// Reads the next data type tag, and checks to see if
	// it is the required type.  If it is not the required type, it
	// signals an error
/**
 * Reads the next data type tag, and checks whether it is the specified type.
 * <br><br>
 * <b>Role:</b> Signals an error if it is not the required type.
 * <br><br>
 * @param type_wanted
 * the data type wanted.
 * @param error_num
 * error code that should be signaled if the required type is not found.
 */
	virtual TaggedData::DataType test_type(
			TaggedData::DataType type_wanted,
			int error_num = 0);

	// Read in a given number of bytes of data and signal an
	// error if not enough was read.
/**
 * Reads the given number of bytes.
 * <br><br>
 * <b>Role:</b> Signals a <tt>sys_error</tt> if not enough was read.
 * <br><br>
 * @param data
 * the read buffer.
 * @param num_bytes
 * the number of bytes to be read.
 * @param swap
 * specifies whether byte swapping is supported.
 */
    virtual size_t read_and_test(void* data, size_t num_bytes, logical swap);

	// "Helper" method to safely read an integer in an ACIS header
	// before the binary file format is known.
/**
 * Reads an integer.
 * <br><br>
 * @param long_size
 * size of a long.
 * @param retval
 * the returned integer.
 */
	logical read_an_int( int long_size, int &retval );

	// These three methods are used to convert between 32 and 64 bit formats.
	// Reading and writing 32 bit longs on a 64 bit machine is easier than
	// reading and writing 64 bit longs on a 32 bit machine, but care must
	// be taken in both cases to output the correct bytes.
/**
 * Writes a tagged long, converting between 32- and 64-bit formats.
 * <br><br>
 * @param type
 * the data type to be written.
 * @param val
 * the logical to be written.
 */
    virtual void safe_write_long_tagged(TaggedData::DataType type, long val);
/**
 * Writes a long, converting between 32- and 64-bit formats.
 * <br><br>
 * @param val
 * the logical to be written.
 */
    virtual void safe_write_long(long val);
/**
 * Reads a long, converting between 32- and 64-bit formats.
 */
    virtual long safe_read_long();

public:

/**
 * Writes a given logical.
 * <br><br>
 * @param val
 * the logical to be written.
 * @param f
 * the logical to be written represents FALSE.
 * @param t
 * the logical to be written represents TRUE.
 */
    virtual void write_logical(logical val, const char* f = "F", const char* t = "T");

/**
 * Reads a logical value.
 * <br><br>
 * @param f
 * character string that requests FALSE.
 * @param t
 * character string that requests TRUE.
 */
    virtual logical read_logical(const char* f = "F", const char* t = "T");

/**
 * Writes a value to an enumeration table.
 * <br><br>
 * @param val
 * the value to be written.
 * @param table
 * the enumeration table to which the value is written.
 */
    virtual void write_enum(int val, const enum_table& table);

/**
 * Reads in an enumeration table.
 * <br><br>
 * @param table
 * the table to be read.
 */
    virtual int read_enum(const enum_table& table);

/**
 * Writes a given character.
 * <br><br>
 * @param val
 * the character to be written.
 */
    virtual void write_char(char val);

/**
 * Reads a character.
 */
    virtual char read_char();

/**
 * Writes a given short.
 * <br><br>
 * @param val
 * the short to be written.
 */
    virtual void write_short(short val);

/**
 * Reads a short.
 */
    virtual short read_short();

/**
 * Writes a given long.
 * <br><br>
 * @param val
 * the long to be written.
 */
    virtual void write_long(long val);

/**
 * Reads a long.
 */
    virtual long read_long();

/**
 * Writes a given float.
 * <br><br>
 * @param val
 * the float to be written.
 */
    virtual void write_float(float val);

/**
 * Reads a float.
 */
    virtual float read_float();

/**
 * Writes a given double.
 * <br><br>
 * @param val
 * the double to be written.
 */
    virtual void write_double(double val);

/**
 * Read a double.
 */
    virtual double read_double();

/**
 * Writes a given string.
 * <br><br>
 * @param val
 * the string to be written.
 * @param length
 * length of the string to be written.
 */
    virtual void write_string(const char* val, size_t length = 0);

/**
 * Reads a string length.
 * <br><br>
 * <b>Role:</b> The data-type argument must be <tt>TaggedData::short_char_string</tt>,
 * <tt>TaggedData::char_string</tt>, or <tt>TaggedData::long_char_string</tt>.
 * <br><br>
 * @param dt
 * data type of the characters in the string.
 */
    virtual size_t read_string_length(TaggedData::DataType dt);

/**
 * Reads a string.
 * <br><br>
 * @param length
 * number of characters to read.
 */
    virtual char* read_string(int& length);

/**
 * Reads a string into a given buffer.
 * <br><br>
 * @param buffer
 * buffer to hold the string.
 * @param maxlen
 * maximum number of characters to read.
 */
    virtual size_t read_string(char* buffer, size_t maxlen = 0);

/**
 * Writes a literal string.
 * <br><br>
 * @param val
 * the string to be written.
 * @param length
 * number of characters to write.
 */
	virtual void write_literal_string(const char* val, size_t length = 0);

    // These could have been implemented in terms of the basic funcitons,
    // but they are provided here just so that you can get a little more
    // information when you read unknown entity types from a binary file.
/**
 * Writes a given position.
 * <br><br>
 * @param val
 * the position to be written.
 */
    virtual void write_position(const SPAposition& val);

/**
 * Reads a position.
 */
    virtual SPAposition read_position();

/**
 * Writes a given vector.
 * <br><br>
 * @param val
 * the vector to be written.
 */
    virtual void write_vector(const SPAvector& val);

/**
 * Reads a vector.
 */
    virtual SPAvector read_vector();

    // The following procedures are for writing specific kinds of data
    // which must be recognized when saving and restoring unkn\own
    // ENTITY types, so they cannot be implemented in terms of the
    // procedures for the basic types.
/**
 * Writes a given pointer.
 * <br><br>
 * @param val
 * the pointer to be written.
 */
    virtual void write_pointer(void* val);

/**
 * Reads a pointer.
 */
    virtual void* read_pointer();

/**
 * Writes a header.
 * <br><br>
 * @param first
 * the first integer written.
 * @param second
 * the second integer written.
 * @param third
 * the third integer written.
 * @param fourth
 * the fourth integer written.
 */
    virtual void write_header( int first, int second, int third, int fourth );

/**
 * Reads a header string.
 * <br><br>
 * @param first
 * the first integer read.
 * @param second
 * the second integer read.
 * @param third
 * the third integer read.
 * @param fourth
 * the fourth integer read.
 */
    virtual logical read_header( int& first, int& second, int& third, int& fourth );

/**
 * Writes an entity identifier.
 * <br><br>
 * @param entity
 * the entity identifier to be written.
 * @param level
 * level of the identifier requested.
 */
    virtual void write_id( const char* entity, int level );

/**
 * Reads an identifier.
 * <br><br>
 * @param buff
 * buffer from which to read.
 * @param size
 * the buffer size (or -1, for no limit).
 */
    virtual int read_id( char *buff, int size = 0 );

/**
 * Writes a terminator.
 */
    virtual void write_terminator();

/**
 * Writes a subtype start.
 */
    virtual void write_subtype_start();

/**
 * Reads a subtype start.
 */
    virtual logical read_subtype_start();

/**
 * Writes a subtype end.
 */
    virtual void write_subtype_end();

/**
 * Reads a subtype end.
 */
    virtual logical read_subtype_end();

//    virtual void write_data(const TaggedData&);
/**
 * Read a data type and the subsequent datum of that type.
 */
    virtual TaggedData* read_data();

	// Targets binary format
	virtual const logical binary_target() const { return TRUE; }

};

/** @} */
#endif
