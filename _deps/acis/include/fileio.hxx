/*******************************************************************/
/*    Copyright (c) 1989-2020 by Spatial Corp.                     */
/*    All rights reserved.                                         */
/*    Protected by U.S. Patents 5,257,205; 5,351,196; 6,369,815;   */
/*                              5,982,378; 6,462,738; 6,941,251    */
/*    Protected by European Patents 0503642; 69220263.3            */
/*    Protected by Hong Kong Patent 1008101A                       */
/*******************************************************************/

#if !defined( FILEIO_HEADER )
#define FILEIO_HEADER

#include "dcl_kern.h"
#include "logical.h"

/**
 * \defgroup ACISSAVERESTORE Save and Restore
 * \ingroup KERNAPI
 */
/**
 * @file fileio.hxx
 * @CAA2Level L1
 * @CAA2Usage U1
 * \addtogroup ACISSAVERESTORE
 *
 * @{
 */

// STI dgh begin
#if defined (sun4) && !defined (solaris)
/**
 * @nodoc
 */
#define SEEK_SET 0
#endif
// STI dgh end

class SPAinterval;
class SPAposition;
class SPAvector;
class SPAunit_vector;
class SPAmatrix;
class SPAtransf;
class ENTITY;
class ENTITY_LIST;
class enum_table;
class FileInterface;
class TaggedData;

// ************************************************************
//	User-level I/O functions
// ************************************************************
/**
* Read a logical.
* <br><br>
* <b>Role:</b> As part of the restore process, this function reads a logical value using 
* the currently installed <tt>FileInterface</tt>'s <tt>read_logical</tt> method.
* @param false_str
* string for FALSE.
* @param true_str
* string for TRUE.
* @see FileInterface, SetActiveFile, GetActiveFile, write_logical
**/
DECL_KERN logical read_logical( char const * false_str = "F", char const * true_str = "T", FileInterface* = NULL);

/**
* Write a logical.
* <br><br>
* <b>Role:</b> As part of the save process, this function writes a logical value using 
* the currently installed <tt>FileInterface</tt>'s <tt>write_logical</tt> method.
* @param val
* logical value to write.
* @param false_str
* string for FALSE.
* @param true_str
* string for TRUE.
* @see FileInterface, SetActiveFile, GetActiveFile, read_logical
*/
DECL_KERN void write_logical( logical val, char const * false_str = "F", char const * true_str = "T", FileInterface* = NULL );

/**
* Read an enumeration value.
* <br><br>
* <b>Role:</b> As part of the restore process, this function reads an enumeration value using 
* the currently installed <tt>FileInterface</tt>'s <tt>read_enum</tt> method.
* <br><br>
* @param tbl
* enumeration table.
* @see enum_table, FileInterface, SetActiveFile, GetActiveFile, write_enum
**/
DECL_KERN int read_enum( enum_table const &tbl, FileInterface* = NULL );

/**
* Write an enumeration value.
* <br><br>
* <b>Role:</b> As part of the save process, this function writes an enumeration value using 
* the currently installed <tt>FileInterface</tt>'s <tt>write_enum</tt> method.
* @param val
* integral value of enumeration.
* @param tbl
* enumeration table.
* @see enum_table, FileInterface, SetActiveFile, GetActiveFile, read_enum
*/
DECL_KERN void write_enum( int val, enum_table const & tbl, FileInterface* = NULL );

/**
* Read an integer.
* <br><br>
* <b>Role:</b> As part of the restore process, this function reads an integer using 
* the currently installed <tt>FileInterface</tt>'s <tt>read_int</tt> method.
* @see FileInterface, SetActiveFile, GetActiveFile, write_int
**/
DECL_KERN int read_int( FileInterface* = NULL );

/**
* Write an integer.
* <br><br>
* <b>Role:</b> As part of the save process, this function writes an integer using 
* the currently installed <tt>FileInterface</tt>'s <tt>write_int</tt> method.
* @param val
* integral value to write.
* @see FileInterface, SetActiveFile, GetActiveFile, read_int
*/
DECL_KERN void write_int( int val, FileInterface* = NULL );

/**
* Read a long.
* <br><br>
* <b>Role:</b> As part of the restore process, this function reads a long using 
* the currently installed <tt>FileInterface</tt>'s <tt>read_long</tt> method.
* @see FileInterface, SetActiveFile, GetActiveFile, write_long
**/
DECL_KERN long read_long( FileInterface* = NULL );

/**
* Write a long.
* <br><br>
* <b>Role:</b> As part of the restore process, this function writes a long using 
* the currently installed <tt>FileInterface</tt>'s <tt>write_long</tt> method.
* @param val
* long value to write.
* @see FileInterface, SetActiveFile, GetActiveFile, read_long
*/
DECL_KERN void write_long( long val );

/**
* Read a double.
* <br><br>
* <b>Role:</b> As part of the restore process, this function reads a double using 
* the currently installed <tt>FileInterface</tt>'s <tt>read_double</tt> method.
* @see FileInterface, SetActiveFile, GetActiveFile, write_real
**/
DECL_KERN double read_real( FileInterface* = NULL );

/**
* Write a double.
* <br><br>
* <b>Role:</b> As part of the save process, this function writes a double using 
* the currently installed <tt>FileInterface</tt>'s <tt>write_double</tt> method.
* @param val
* double value to write.
* @see FileInterface, SetActiveFile, GetActiveFile, read_real
*/
DECL_KERN void write_real( double val, FileInterface* = NULL  );

/**
* Read a string.
* <br><br>
* <b>Role:</b> As part of the restore process, this function reads a string using 
* the currently installed <tt>FileInterface</tt>'s <tt>read_string(char* buf, size_t max_len)</tt> method
* (with max_len set to 0).
* <br><br>
* When using the default <tt>FileInterface</tt>, if the supplied buffer isn't big enough, a
* buffer overrun will occur, likely causing a crash. For this reason, we suggest using the
* other version of <tt>read_string</tt>, which allocates memory on your behalf.
* <br><br>
* Returns the length of the string read.
* @param buf
* character string.
* @see FileInterface, SetActiveFile, GetActiveFile, write_string
**/
DECL_KERN int read_string( char * buf);

/**
* Read a string.
* <br><br>
* <b>Role:</b> As part of the restore process, this function reads a string using 
* the currently installed <tt>FileInterface</tt>'s <tt>read_string(int& len)</tt> method.
* <br><br>
* When using the default FileInterface, memory for the string is allocated on the caller's
* behalf. Caller should free allocated memory using <tt>ACIS_DELETE [] STD_CAST [str]</tt>.
* @param len
* length of string returned.
* @see FileInterface, SetActiveFile, GetActiveFile, write_string
**/
DECL_KERN char * read_string( int& len);

/**
* @nodoc
*/
DECL_KERN char * read_summary_string( int& len);

/**
* Write a string.
* <br><br>
* <b>Role:</b> As part of the save process, this function writes a string using 
* the currently installed <tt>FileInterface</tt>'s <tt>write_string</tt> method.
* @param buf
* character string.
* @see FileInterface, SetActiveFile, GetActiveFile, read_string
**/
DECL_KERN void write_string( char const * buf, FileInterface* = NULL );

/**
* @nodoc
*/
DECL_KERN void write_summary_string( char const * buf, FileInterface* = NULL );

/**
* Write a literal string.
* <br><br>
* <b>Role:</b> As part of the save process, this function writes a string using 
* the currently installed <tt>FileInterface</tt>'s <tt>write_string</tt> method.
* @param buf
* character string.
* @param maxlen
* Maximum number of characters to write; 0 means write all.
* @see FileInterface, SetActiveFile, GetActiveFile, write_string, read_string
**/
DECL_KERN void write_literal( char const * buf, int maxlen = 0 );

/**
* Read a wide character string.
* <br><br>
* <b>Role:</b> As part of the restore process, this function reads a wide character string
* using the currently installed <tt>FileInterface</tt>'s <tt>read_wstring</tt> method.
* <br><br>
* When using the default FileInterface, memory for the wstring is allocated on the caller's
* behalf. Caller should free allocated memory using <tt>ACIS_DELETE [] STD_CAST [wstr]</tt>.
* @see FileInterface, SetActiveFile, GetActiveFile, write_wstring
**/
DECL_KERN SPAWCHAR_T* read_wstring( void );

/**
* Write a wide character string.
* <br><br>
* <b>Role:</b> As part of the save process, this function writes a wide character string
* using the currently installed <tt>FileInterface</tt>'s <tt>write_wstring</tt> method.
* @param wstr
* Given wide string to write.
* @see FileInterface, SetActiveFile, GetActiveFile, read_wstring
**/
DECL_KERN void write_wstring( const SPAWCHAR_T* wstr );

/**
* Read an interval.
* <br><br>
* <b>Role:</b> As part of the restore process, this function reads an interval using the
* currently installed <tt>FileInterface</tt>'s <tt>read_real</tt> and <tt>read_logical</tt> methods.
* @see FileInterface, SetActiveFile, GetActiveFile, write_interval
**/
DECL_KERN SPAinterval read_interval( void );

/**
* Write an interval.
* <br><br>
* <b>Role:</b> As part of the save process, this function writes an interval using the
* currently installed <tt>FileInterface</tt>'s <tt>write_real</tt> and <tt>write_logical</tt> methods.
* @param inter
* Given interval to write.
* @see FileInterface, SetActiveFile, GetActiveFile, read_interval
*/
DECL_KERN void write_interval( SPAinterval const & inter );

/**
* Read a position.
* <br><br>
* <b>Role:</b> As part of the restore process, this function reads a position using the
* currently installed <tt>FileInterface</tt>'s <tt>read_double</tt> method.
* @see FileInterface, SetActiveFile, GetActiveFile, write_position
**/
DECL_KERN SPAposition read_position( void );

/**
* Write a position.
* <br><br>
* <b>Role:</b> As part of the save process, this function writes a position using the
* currently installed <tt>FileInterface</tt>'s <tt>write_double</tt> method.
* @param pos
* Given position to write.
* @see FileInterface, SetActiveFile, GetActiveFile, read_position
*/
DECL_KERN void write_position( SPAposition const& pos, FileInterface* = NULL );

/**
* Read a vector.
* <br><br>
* <b>Role:</b> As part of the restore process, this function reads a vector using the
* currently installed <tt>FileInterface</tt>'s <tt>read_double</tt> method.
* @see FileInterface, SetActiveFile, GetActiveFile, write_vector
**/
DECL_KERN SPAvector read_vector();

/**
* Write a vector.
* <br><br>
* <b>Role:</b> As part of the save process, this function writes a vector using the
* currently installed <tt>FileInterface</tt>'s <tt>write_double</tt> method.
* @param vec
* Given vector to write.
* @see FileInterface, SetActiveFile, GetActiveFile, read_vector
*/
DECL_KERN void write_vector( SPAvector const & vec );

/**
* Read a <tt>SPAunit_vector</tt> as a vector and then normalize it.
* <br><br>
* <b>Role:</b> As part of the restore process, this function reads a vector using <tt>read_vector</tt>, 
* then normalizes it.
* @see FileInterface, SetActiveFile, GetActiveFile, write_vector
**/
DECL_KERN SPAunit_vector read_unit_vector( void );

/**
* Read a <tt>SPAmatrix</tt> as three row vectors.
* <br><br>
* <b>Role:</b> As part of the restore process, this function reads a <tt>SPAmatrix</tt> by making three calls
* to <tt>read_vector</tt>.
* @see FileInterface, SetActiveFile, GetActiveFile, read_vector
**/
DECL_KERN SPAmatrix read_matrix( void );

/**
* Write a <tt>SPAmatrix</tt> as three row vectors.
* <br><br>
* <b>Role:</b> As part of the save process, this function writes a <tt>SPAmatrix</tt> by making three calls
* to <tt>write_vector</tt>.
* @param matr
* Given matrix to write.
* @see FileInterface, SetActiveFile, GetActiveFile, write_vector
*/
DECL_KERN void write_matrix( SPAmatrix const & matr );

/**
* Reads a transformation.
* <br><br>
* <b>Role:</b> As part of the restore process, this function reads a <tt>SPAtransf</tt>.
* <br><br>
* Although this internal function is intended strictly for <tt>ACIS</tt>
* usage, a minimal amount of information about this function is provided for the
* sole purpose of being able to understand and trace restoration from a <tt>SAT</tt> file.
* This function should never be called directly, because it makes assumptions
* about the availability of a <tt>SAT</tt> file, the location of the input pointer into the
* <tt>SAT</tt> file, and the validity of <tt>SAT</tt> data it expects to read in. It also may start
* a lengthy process of nested function or class method calls, which have many of
* the same assumptions.
* <br><br>
* Uses the currently installed <tt>FileInterface</tt>'s <tt>read_real</tt> and <tt>read_logical</tt>
* methods.
* @see FileInterface, SetActiveFile, GetActiveFile, write_transf
**/
DECL_KERN SPAtransf read_transf();

/**
* Write a transformation.
* <br><br>
* <b>Role:</b> As part of the save process, this function writes a <tt>SPAtransf</tt>.
* @param tr
* Given transform to write.
* @see FileInterface, SetActiveFile, GetActiveFile, read_transf
*/
DECL_KERN void write_transf( SPAtransf const & tr );

/**
* Read an explicit record sequence number.
* <br><br>
* <b>Role</b>: As part of the restore process, this function reads a sequence number using the
* currently installed <tt>FileInterface</tt>'s <tt>read_sequence</tt> method.
* <br><br>
* An ACIS SAT file can be thought of as a series of records. When sequencing is enabled, 
* each record in the written SAT file is preceded by this sequence number. When using the 
* default FileInterface for SAT files (<tt>SatFile</tt>), sequence numbers consist of a minus 
* sign following by a sequential non-negative integer.
* <br><br>
* Sequence numbers do not appear in binary files.
* @see FileInterface, SetActiveFile, GetActiveFile, SatFile, BinaryFile
**/
DECL_KERN int read_sequence( FileInterface* = NULL );

/**
* Write an explicit record sequence number.
* <br><br>
* <b>Role</b>: As part of the save process, this function writes a sequence number using the
* currently installed <tt>FileInterface</tt>'s <tt>write_sequence</tt> method.
* <br><br>
* An ACIS SAT file can be thought of as a series of records. When sequencing is enabled, 
* each record in the written SAT file is preceded by this sequence number. When using the 
* default FileInterface for SAT files (<tt>SatFile</tt>), sequence numbers consist of a minus 
* sign following by a sequential non-negative integer.
* <br><br>
* Sequence numbers do not appear in binary files.
* @see FileInterface, SetActiveFile, GetActiveFile, SatFile, BinaryFile
*/
DECL_KERN void write_sequence(int num);

/**
* Read an entity identifier.
* <br><br>
* <b>Role:</b> As part of the restore process, this function reads an identifier using the
* currently installed <tt>FileInterface</tt>'s <tt>read_id</tt> method.
* <br><br>
* When an <tt>ENTITY</tt> is saved its identifier is a "path" describing its derivation. For example,
* <tt>ATTRIB_RGB</tt>'s entity identifier is <tt>rgb_color-st-attrib</tt>. This says that 
* <tt>ATTRIB_RGB</tt> is derived from <tt>ATTRIB_ST</tt>, which is derived from <tt>ATTRIB</tt>.
* Similarly, when an <tt>ELLIPSE</tt> is saved its entity identifier is <tt>ellipse-curve</tt>. 
* This says that <tt>ELLIPSE</tt> is derived from <tt>CURVE</tt>.
* <br><br>
* Using the <tt>ATTRIB_RGB</tt> example, <tt>rgb_color</tt> and <tt>st</tt> are thought of
* as sub-id's with level=2, and the last one, <tt>attrib</tt>, is thought of simply as an id 
* with level=1.
* @param buf
* id string.
* @param buflen
* length of buffer.
* @see FileInterface, SetActiveFile, GetActiveFile, write_id, write_sub_id, write_id_level
**/
DECL_KERN int read_id( char *buf, int buflen = 0, FileInterface* = NULL );

/**
* Write an entity identifier.
* <br><br>
* <b>Role:</b> As part of the save process, this function writes a (level == 1) entity identifier 
* using the currently installed <tt>FileInterface</tt>'s <tt>write_id</tt> method.
* <br><br>
* See <tt>read_id</tt> for a description of entity id's and levels.
* @param buf
* id string.
* @see FileInterface, SetActiveFile, GetActiveFile, read_id, write_sub_id, write_id_level
*/
DECL_KERN void write_id( char const * buf, FileInterface* = NULL );

/**
* Write an entity "sub" identifier.
* <br><br>
* <b>Role:</b> As part of the save process, this function writes an entity "sub" (level > 1)
* identifier using the currently installed <tt>FileInterface</tt>'s <tt>write_id</tt> method.
* <br><br>
* See <tt>read_id</tt> for a description of entity id's and levels.
* @param buf
* id string.
* @see FileInterface, SetActiveFile, GetActiveFile, read_id, write_sub_id, write_id_level
*/
DECL_KERN void write_sub_id( char const * buf);

/**
* Write an entity identifier.
* <br><br>
* <b>Role:</b> As part of the save process, this function writes an entity identifier 
* using the currently installed <tt>FileInterface</tt>'s <tt>write_id</tt> method.
* <br><br>
* See <tt>read_id</tt> for a description of entity id's and levels.
* @param buf
* id string.
* @param level
* entity level.
* @see FileInterface, SetActiveFile, GetActiveFile, read_id, write_sub_id, write_id_level
*/
DECL_KERN void write_id_level( char const * buf, int level, FileInterface* = NULL);

/**
* Read a header.
* <br><br>
* <b>Role:</b> As part of the restore process, this function reads a header using the
* currently installed <tt>FileInterface</tt>'s <tt>read_header</tt> method.
* <br><br>
* The ACIS header is comprised of four integers, e.g. <tt>200 0 1 0</tt>
* <br><br>
* <dl><dt>First Integer</dt><dd>An encoded version number. In the example, this is <tt>200</tt>. This
* value is 100 times the major version plus the minor version (e.g., <tt>107</tt> for ACIS
* version 1.7). For point releases, the final value is truncated. Part save data
* for the .sat files is not affected by a point release (e.g., <tt>105</tt> for ACIS
* version 1.5.2).</dd>
* <dt>Second Integer</dt><dd>The total number of saved data records, or zero. If zero, then
* there needs to be an end mark.</dd>
* <dt>Third Integer</dt><dd>A count of the number of entities in the original entity list
* saved to the part file.</dd>
* <dt>Fourth Integer</dt><dd>The least significant bit of this number is used to indicate
* whether or not history has been saved in this save file.</dd>
* </dl>
* @param i1
* release version.
* @param i2
* number of data records.
* @param i3
* number of entities.
* @param i4
* history.
* @see FileInterface, SetActiveFile, GetActiveFile, write_header
**/
DECL_KERN logical read_header( int &i1, int &i2, int &i3, int &i4 );

/**
* Write a header.
* <br><br>
* <b>Role:</b> As part of the save process, this function writes a header using the
* currently installed <tt>FileInterface</tt>'s <tt>write_header</tt> method.
* <br><br>
* The ACIS header is comprised of four integers, e.g. <tt>200 0 1 0</tt>.
* <br><br>
* <dl><dt>First Integer</dt><dd>An encoded version number. In the example, this is <tt>200</tt>. This
* value is 100 times the major version plus the minor version (e.g., <tt>107</tt> for ACIS
* version 1.7). For point releases, the final value is truncated. Part save data
* for the .sat files is not affected by a point release (e.g., <tt>105</tt> for ACIS
* version 1.5.2).</dd>
* <dt>Second Integer</dt><dd>The total number of saved data records, or zero. If zero, then
* there needs to be an end mark.</dd>
* <dt>Third Integer</dt><dd>A count of the number of entities in the original entity list
* saved to the part file.</dd>
* <dt>Fourth Integer</dt><dd>The least significant bit of this number is used to indicate
* whether or not history has been saved in this save file.</dd>
* </dl>
* @param i1
* release version.
* @param i2
* number of data records.
* @param i3
* number of entities.
* @param i4
* history.
* @see FileInterface, SetActiveFile, GetActiveFile, read_header
 */
DECL_KERN void write_header( int i1, int i2, int i3, int i4 );

/**
* Read a subtype start identifier.
* <br><br>
* <b>Role:</b> As part of the restore process, this function reads a subtype start identifier using the
* currently installed <tt>FileInterface</tt>'s <tt>read_subtype_start</tt> method.
* <br><br>
* The subtype start identifer for the <tt>SatFile FileInterface</tt> is "{ ".
* @see FileInterface, SetActiveFile, GetActiveFile, SatFile, write_subtype_start
**/
DECL_KERN logical read_subtype_start( void );

/**
* Write a subtype start identifier.
* <br><br>
* <b>Role:</b> As part of the save process, this function writes a subtype start identifier using the
* currently installed <tt>FileInterface</tt>'s <tt>write_subtype_start</tt> method.
* <br><br>
* The subtype start identifer for the <tt>SatFile FileInterface</tt> is "{ ".
* @see FileInterface, SetActiveFile, GetActiveFile, SatFile, read_subtype_start
*/
DECL_KERN void write_subtype_start( void );

/**
* Read a subtype end identifier.
* <br><br>
* <b>Role:</b> As part of the restore process, this function reads a subtype end identifier using the
* currently installed <tt>FileInterface</tt>'s <tt>read_subtype_end</tt> method.
* <br><br>
* The subtype end identifer for the <tt>SatFile FileInterface</tt> is "} ".
* @see FileInterface, SetActiveFile, GetActiveFile, SatFile, write_subtype_end
**/
DECL_KERN logical read_subtype_end( void );

/**
* Write a subtype end identifier.
* <br><br>
* <b>Role:</b> As part of the save process, this function writes a subtype end identifier using the
* currently installed <tt>FileInterface</tt>'s <tt>write_subtype_end</tt> method.
* <br><br>
* The subtype end identifer for the <tt>SatFile FileInterface</tt> is "} ".
* @see FileInterface, SetActiveFile, GetActiveFile, SatFile, read_subtype_end
*/
DECL_KERN void write_subtype_end( void );

/**
* Read a TaggedData item.
* <br><br>
* <b>Role:</b> As part of the restore process, this function reads the next field from the 
* restore file and returns it as a <tt>TaggedData</tt> item using the currently installed <tt>FileInterface</tt>'s
* <tt>read_data</tt> method.
* <br><br>
* This is mainly used by the restore process when reading unknown entities or unknown subtypes.
* <br><br>
* This function returns a new object allocated on the heap. It is the caller's responsibility to free it when
* it is done with it (use ACIS_DELETE).
* <br><br>
* We suggest appending the returned data to a <tt>TaggedDataList</tt>, which will assume responsibility for 
* deleting its entries.
* @see FileInterface, SetActiveFile, GetActiveFile, TaggedData, TaggedDataList, write_data
**/
DECL_KERN TaggedData* read_data( FileInterface* = NULL);

/**
* Write a TaggedData item.
* <br><br>
* <b>Role:</b> As part of the save process, this function writes a <tt>TaggedData</tt> item using the currently 
* installed <tt>FileInterface</tt>'s <tt>write_data</tt> method.
* <br><br>
* This is mainly used by the restore process when writing unknown entities or unknown subtypes.
* @param td
* Given TaggedData.
* @see FileInterface, SetActiveFile, GetActiveFile, TaggedData, TaggedDataList, read_data
*/
DECL_KERN void write_data( const TaggedData & td );


//===============================
// Undocumented I/O calls.
//===============================

// This procedure should NOT be used for writing ENTITY pointers to
// a save file.  It is for internal use only.  To write an ENTITY
// pointer, use write_ptr which does the conversions needed to save 
// and restore ENTITY pointers.
/**
 * @nodoc
 */
DECL_KERN void write_pointer( void*, FileInterface* = NULL );

// Suggest using write_ptr.
/**
* @nodoc
**/
DECL_KERN void * read_pointer( FileInterface* = NULL );

// Not in use.
/**
* @nodoc
*/
DECL_KERN void write_float( float num );
/**
* @nodoc
*/
DECL_KERN float read_float();

// Mark the logical end of an output record.
// We don't recommend that customers call this. ACIS takes responsibility
// for calling write_terminator at the end of records. We also don't recommend
// that customers implement their own FileInterface::write_terminator.
/**
* @nodoc
*/
DECL_KERN void write_terminator( FileInterface* = NULL);

// Break textual output line for convenience. Only implemented by SatFile. 
// Ignored on restore.
/**
 * @nodoc
 */
DECL_KERN void write_newline(int = 1, FileInterface* = NULL);

/**
* @nodoc
*/
DECL_KERN void write_logical( logical, int, char const * = "F",char const * = "T", FileInterface* = NULL );

// Not used. No matching write_char was ever offered.
/**
* @nodoc
**/
DECL_KERN int read_char( FileInterface* = NULL );

// This procedure is used to test whether or not saving and
// restoring unknown ENTITY types is supported for the current
// <tt>FileInterface</tt>.  Tse test used to be whether or not you were
// saving or restoring in text mode, but that test is no longer
// valid.
/**
 * @nodoc
 */
DECL_KERN logical unknown_types_ok();

// Use GetActiveFile and SetActiveFile instead of these.
/**
 * @nodoc
 */
DECL_KERN void set_savres_file( FileInterface * );
/**
* @nodoc
**/
DECL_KERN FileInterface *get_savres_file();


//======================================================================
// NOTE: DO NOT USE THESE PROCEDURES!

//  These procedures for repositioning the FILE pointer are left
//  here for compatibility with the old save file header in which
//  the number of ENTITIES written to the file was written to the
//  header after all of the ENTITIES had been written.  In the
//  new format, an end-of-data record is written so that the ENTITY
//  count is no longer needed.  Use of these procedures should be
//  avoided because their use prevents saving or restoring to
//  targets which do not support repositioning the FILE pointer
//  such as pipes.

// Mark a location in the savres file
/**
 * @nodoc
 */

DECL_KERN long set_savres_file_mark();

// Return to a SPAposition in the savres file
/**
 * @nodoc
 */

DECL_KERN void rewind_savres_file( long );

//======================================================================

// Variables which contain the version numbers of the current save
// file being restored. There is a "major" and "minor" component, and
// a portmanteau number which combines the two, and is the value
// actually placed in the save file. Note that these variables are
// also declared in savres/versions.hxx, for convenience. There may
// also be a "point" version, but this is usually zero.
/**
 * @nodoc
 */
DECL_KERN int& get_restore_major_version();
/**
 * @nodoc
 */
DECL_KERN int& get_restore_minor_version();
/**
 * @nodoc
 */
DECL_KERN int& get_restore_point_version();
/**
 * @nodoc
 */
DECL_KERN int& get_restore_version_number();	// Combined version number.

#ifndef restore_version_number
/**
 * @nodoc
 */
#define restore_version_number get_restore_version_number()
#endif

/** @} */
#endif
