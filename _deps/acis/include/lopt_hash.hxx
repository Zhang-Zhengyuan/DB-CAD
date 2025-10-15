/* $Id: lopt_hash.hxx,v 1.7 2002/02/28 17:16:11 rajesh Exp $ */
/*******************************************************************/
/*    Copyright (c) 1989-2020 by Spatial Corp.                     */
/*    All rights reserved.                                         */
/*    Protected by U.S. Patents 5,257,205; 5,351,196; 6,369,815;   */
/*                              5,982,378; 6,462,738; 6,941,251    */
/*    Protected by European Patents 0503642; 69220263.3            */
/*    Protected by Hong Kong Patent 1008101A                       */
/*******************************************************************/
#ifndef LOPT_HASH
#define LOPT_HASH

#include "dcl_lopt.h"
#include "entity.hxx"

class DECL_LOPT LOPT_ENTRY;
class DECL_LOPT LOPT_PTR_DICT;
#if defined D3_STANDALONE || defined D3_DEBUG
class D3_ostream;
class D3_istream;

DECL_LOPT D3_ostream &operator<<( D3_ostream &os, LOPT_PTR_DICT const & );
DECL_LOPT D3_ostream &operator<<( D3_ostream &os, LOPT_ENTRY const & );
#endif


typedef logical (*lop_entry_fn)( LOPT_ENTRY *, void * );


class DECL_LOPT LOPT_PTR_DICT : public ACIS_OBJECT
    {
private:
    
    long int 	_size;

    LOPT_ENTRY 	**_rows;

public:

    LOPT_PTR_DICT( long int size = 0 );

    // Copy constructor
    LOPT_PTR_DICT( const LOPT_PTR_DICT &old );

    virtual ~LOPT_PTR_DICT();
    
    unsigned long hash( void *key ) const;

    // The rehash function is called in the case of an overflow.
    void rehash();

    logical scan_table( lop_entry_fn fn, void *data );

    void* operator[] ( void *key ) const;

    void*& operator[] ( void *key );

    void* lookup( void *key ) const;

    void insert( void *data, void *key );

	// data access
	long int size() 	{ return _size; }

	LOPT_ENTRY **rows()	{ return _rows; }

#if defined D3_STANDALONE || defined D3_DEBUG

	friend DECL_LOPT D3_ostream &operator<<(
				D3_ostream &os,
				LOPT_PTR_DICT const &
			);

	void print( D3_ostream & os, int level )	const;

#endif
    };


class LOPT_ENTRY : public ACIS_OBJECT
    {
    LOPT_ENTRY *_next;

    void *_data;
    
    void *_key;

public:

    // Construct an entry with a specific successor:
    LOPT_ENTRY( LOPT_ENTRY *next, void *data, void *key );
    
    LOPT_ENTRY();
    
    ~LOPT_ENTRY();
    
    friend class LOPT_PTR_DICT;
    
    void* data() 	{ return _data; }

    void* key() 	{ return _key; }

	LOPT_ENTRY *next()	{ return _next; }

#if defined D3_STANDALONE || defined D3_DEBUG

	friend DECL_LOPT D3_ostream &operator<<(
				D3_ostream &os,
				LOPT_ENTRY const &
			);

	void print( D3_ostream & os, int level )	const;

#endif
    };

#endif
