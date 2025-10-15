// $Id: hashstr.hxx,v 1.4 2002/08/09 17:15:31 jeff Exp $
/*******************************************************************/
/*    Copyright (c) 1989-2020 by Spatial Corp.                     */
/*    All rights reserved.                                         */
/*    Protected by U.S. Patents 5,257,205; 5,351,196; 6,369,815;   */
/*                              5,982,378; 6,462,738; 6,941,251    */
/*    Protected by European Patents 0503642; 69220263.3            */
/*    Protected by Hong Kong Patent 1008101A                       */
/*******************************************************************/

#ifndef __HASHSTR_HXX__
#define __HASHSTR_HXX__

#include "base.hxx"
#include "dcl_kern.h"
#include "string.h"
#include "fileif.hxx"


#define HASH_SIZE 64

/**
* @file hashstr.hxx
 * @CAA2Level L1
 * @CAA2Usage U1
 * \addtogroup ACISSAVERESTORE
 *
 * @{
 */


// ********************************************************************
// Class:
//    str
// 
// Access:
//    do not document
//
// Purpose:
//    A very basic string class.  It was created so we could store
//    string data in containers that are set-associative, e.g., they
//    maintain sorted data.  Since these containers do not know what
//    type of data they store, they simply call <, >, and == to
//    maintain sorted-ness (which would never work for char *'s).
// ********************************************************************

class str;
class str_index_node;
class index_str_node;

class DECL_KERN str : public ACIS_OBJECT
{
	char *s;

public:
	str(): s(NULL) 
		{}
	~str()
		{ if (s) ACIS_FREE(s); }
	str(const str& _s)
		{ if (_s.s) s = ACIS_STRDUP(_s.s); }
	str(const char *psz)
		{ if (psz) s = ACIS_STRDUP(psz); }

	str& operator=(const str& _s)
	{
		if (s) ACIS_FREE(s);
		s = ACIS_STRDUP(_s.s);
		return *this;
	}

	char *c_str(void) const
		{ return s; }

	logical operator==(const str& sR) const
	{ return strcmp(this->s, sR.s) == 0;}

	logical operator<(const str& sR) const
	{ return strcmp(this->s, sR.s) < 0; }

	logical operator>(const str& sR) const
	{ return strcmp(this->s, sR.s) > 0; }

};






// ********************************************************************
// Class:
//    str_index_node
// 
// Access:
//    do not document
//
// Purpose:
//    An object that stores a str and an int.  It can be (and is) used as
//    a node within a "map" style container that stores str's as the 
//    key/lookup value and int's as the stored value.
// ********************************************************************
class DECL_KERN str_index_node : public ACIS_OBJECT
{
	str key;
	int val;

public:
	str_index_node *next;

	str_index_node() 
		{ next = NULL; }
	str_index_node(const str& s, int i)
		{ key = s, val = i; }
	str_index_node(const str_index_node& sin)
		{ *this = sin; }
	
	str_index_node& operator=(const str_index_node& sin)
	{ 
		key = sin.key;
		val = sin.val;
		return *this; 
	}

	str get_key(void)
		{ return key; }
	int get_val(void)
		{ return val; }

	logical operator<( str_index_node const &sinR)const
	{ return this->key < sinR.key; }
	logical operator>( str_index_node const &sinR)
	{ return this->key > sinR.key; }
	logical operator==( str_index_node const &sinR)
	{ return this->key == sinR.key; }

};


// ********************************************************************
// Class:
//    index_str_node
// 
// Access:
//    do not document
//
// Purpose:
//    An object that stores an int and a str.  It can be (and is) used as
//    a node within a "map" style container that stores int's as the 
//    key/lookup value and str's as the stored value.
// ********************************************************************
class DECL_KERN index_str_node : public ACIS_OBJECT
{
	int key;
	str val;

public:
	index_str_node *next;

	index_str_node() 
		{ next = NULL; }
	index_str_node(int i, const str& s)
		{ key = i, val = s; }
	index_str_node(const index_str_node& isn)
		{ *this = isn; }
	
	index_str_node& operator=(const index_str_node& isn)
	{ 
		key = isn.key;
		val = isn.val;
		return *this;
	}

	int get_key(void)
		{ return key; }
	str get_val(void)
		{ return val; }

	logical operator<(const index_str_node& isnR)
		{ return this->key < isnR.key; }
	logical operator>(const index_str_node& isnR)
		{ return this->key > isnR.key; }
	logical operator==(const index_str_node& isnR)
	{ return this->key == isnR.key; 	}

};



// ********************************************************************
// Class:
//    hash_str_index
// 
// Access:
//    do not document
//
// Purpose:
//    An object that implements a hash of str_index_node's.  As such, it
//    is basically a map container for mapping str's to int's.
// ********************************************************************
class DECL_KERN hash_str_index : public ACIS_OBJECT
{
	str_index_node *table[HASH_SIZE];
	int index;

	int hash_func(const str& s)
	{
		char *psz = s.c_str();
		int i = 0;
		int chksum = 0;
		while ((psz[i] != '\0') && (i != 5))
			chksum += (int)psz[i++];

		return chksum % HASH_SIZE;
	}

public:
	hash_str_index(): index(0)
	{ 
		memset(table, 0, HASH_SIZE * sizeof(str_index_node *)); 
	}

	void clear()
	{
		for (int i = 0; i < HASH_SIZE; i++)
		{
			str_index_node *sin = table[i], *next;
			while (sin)
			{
				next = sin->next;
				ACIS_DELETE sin;
				sin = next;
			}
		}
		memset(table, 0, HASH_SIZE * sizeof(str_index_node *));
		index = 0;
	}
	~hash_str_index()
	{
		clear();
	}

	logical insert(const str& s, int& insert_index)
	{
		str_index_node *head, *walker;

		int hash = hash_func(s);
		head = walker = table[hash];

		logical found = FALSE;
		while (walker && !found)
		{
			found = (walker->get_key() == s);
			if (!found)
				walker = walker->next;
		}

		if (found)
		{
			insert_index = walker->get_val();
			return TRUE;
		}

		insert_index = ++index;
		str_index_node *n = ACIS_NEW str_index_node(s, insert_index);
		n->next = table[hash];
		table[hash] = n;
		return FALSE;
	}
};



// ********************************************************************
// Class:
//    hash_index_str
// 
// Access:
//    do not document
//
// Purpose:
//    An object that implements a hash of index_str_node's.  As such, it
//    basically a map container for mapping int's to str's.
// ********************************************************************
class DECL_KERN hash_index_str : public ACIS_OBJECT
{
	index_str_node *table[HASH_SIZE];

	int hash_func(const int& index)
	{
		return index % HASH_SIZE;
	}

public:
	hash_index_str()
	{ 
		memset(table, 0, HASH_SIZE * sizeof(index_str_node *)); 
	}

	void clear()
	{
		for (int i = 0; i < HASH_SIZE; i++)
		{
			index_str_node *isn = table[i], *next;
			while (isn)
			{
				next = isn->next;
				ACIS_DELETE isn;
				isn = next;
			}
		}
		memset(table, 0, HASH_SIZE * sizeof(index_str_node *)); 
	}
	~hash_index_str()
	{
		clear();
	}

	logical find(const int& index, str& s)
	{
		index_str_node *head, *walker;

		int hash = hash_func(index);
		head = walker = table[hash];

		logical found = FALSE;
		while (walker && !found)
		{
			found = (walker->get_key() == index);
			if (!found)
				walker = walker->next;
		}

		if (found)
		{
			s = walker->get_val();
			return TRUE;
		}
		else
			return FALSE;		
	}
	
	void insert(const int& index, str& s)
	{
		int hash = hash_func(index);
		index_str_node *n = ACIS_NEW index_str_node(index, s);
		n->next = table[hash];
		table[hash] = n;
	}

};

#define SPA_ID_SEPARATOR	'-'
#define SPA_INDEXING_CHAR	'%'

#define SPA_TRUE_CHAR		'T'
#define SPA_FALSE_CHAR		'F'
#define SPA_ENUM_LEAD_CHAR	'E'


/**
 * Defines an abstract class for saving and restoring ACIS <tt>ENTITY</tt> data.
 * <br><br>
 * <b>Role:</b> All ACIS save and restore operations use an object of this 
 * class to control the reading or writing of the data. This class contains
 * the functionality to implement the "save_summary_mode" global option.
 * <br><br>
 * We do not recommend deriving from this class directly. Instead, derive from 
 * <tt>BinaryFile</tt>. The <tt>BinaryFile</tt> class has a standard implementation 
 * for most of the virtual methods of this class that take care of the details 
 * for saving and restoring the unknown <tt>ENTITY</tt> data. Implementation of the 
 * actual read and write methods are all that is necessary.
 * <br><br>
 * @see FileInterface, BinaryFile, SatFile, SabFile
 */

class DECL_KERN SummaryFileInterface : public FileInterface {

	// These two maps are used for mapping strings to their
	// corresponding index, and vice versa, when saving and
	// restoring sat files.
	hash_str_index storing_map;
	hash_index_str restoring_map;
	hash_index_str backup_restoring_map;

protected:

	// If a user sets the option "save_summary_mode" to TRUE, then
	// this will be set true.
	logical saving_summary_mode;

	// Will be true if, when restoring a sat file, we find that
	// indexes are used to store id's.  Else, defaults to false.
	// After being set to true, we also anticipate the remainder
	// of the sat file to be fully indexed and summarized.
	logical restoring_summary_mode;

public:

	SummaryFileInterface();
	~SummaryFileInterface();

	logical saving_summary();
	logical restoring_summary();

	void set_saving_summary( logical ssm );
	void set_restoring_summary( logical srm);
	void restoring_map_insert(const int& index, str& s);

	logical storing_map_insert(const str& s, int& insert_index);
	logical restoring_map_find(const int& index, str& s, bool search_in_backup = false);

	virtual void write_summary( const char*);
	virtual char* read_summary( int&);
	virtual void clear();
};

/** @} */
#endif // __HASHSTR_HXX__
