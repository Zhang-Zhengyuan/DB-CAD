/* ORIGINAL: acis2.1/kernutil/fileio/oldsab.hxx */
/* $Id: oldsab.hxx,v 1.9 2002/07/18 17:52:46 jeff Exp $ */
/*******************************************************************/
/*    Copyright (c) 1989-2020 by Spatial Corp.                     */
/*    All rights reserved.                                         */
/*    Protected by U.S. Patents 5,257,205; 5,351,196; 6,369,815;   */
/*                              5,982,378; 6,462,738; 6,941,251    */
/*    Protected by European Patents 0503642; 69220263.3            */
/*    Protected by Hong Kong Patent 1008101A                       */
/*******************************************************************/

#ifndef OLDSABFILE_H
#define OLDSABFILE_H

//---------------------------------------------------------------------
// Define the OldSabFile class for doing ACIS save and restore to
// stream files using the old binary format.

//---------------------------------------------------------------------

#include <stdio.h>
#include "dcl_kern.h"
#include "fileif.hxx"

//---------------------------------------------------------------------

class DECL_KERN OldSabFile : public FileInterface {

private:
    FILE* m_pFILE;

    void write(const void* buf, size_t len);
    size_t read(void* buf, size_t length);

public:
    OldSabFile(FILE*);
    virtual ~OldSabFile();

    //--------------------
    // virtual methods from FileInterface
    virtual void write_logical(logical, const char* f="F", const char* t="T");
    virtual logical read_logical(const char* f="F", const char* t="T");

    virtual void write_enum(int, enum_table const &);
    virtual int read_enum(enum_table const &);

    virtual void write_char(char);
    virtual char read_char();

    virtual void write_short(short);
    virtual short read_short();

    virtual void write_long(long);
    virtual long read_long();

    virtual void write_float(float);
    virtual float read_float();

    virtual void write_double(double);
    virtual double read_double();

    virtual void write_string(const char*, size_t len=0);
    virtual char* read_string(int&);
    virtual size_t read_string(char* buf, size_t maxlen = 0);

    // The following procedures are for writing specific kinds of data
    // which must be recognized when saving and restoring unknown
    // ENTITY types, so they cannot be implemented in terms of the
    // procedures for the basic types.

    virtual void write_pointer(void*);
    virtual void* read_pointer();

    virtual void write_header( int, int, int, int );
    virtual logical read_header( int &, int &, int &, int & );

    virtual void write_id( const char*, int );
    virtual int read_id( char *, int = 0 );

    virtual void write_terminator();

    virtual void write_subtype_start();
    virtual logical read_subtype_start();

    virtual void write_subtype_end();
    virtual logical read_subtype_end();

    //----------------------
    // Methods for positioning within the file
    virtual FilePosition set_mark();
    virtual FilePosition goto_mark(FilePosition);

	// get information about the file
	virtual logical get_file_data( FileData* );

    //----------------------
    // determine if unknown ENTITY types are OK
    virtual logical unknown_types_ok();

	// Targets binary format
	virtual const logical binary_target() const { return TRUE; }

};

//---------------------------------------------------------------------
#endif
