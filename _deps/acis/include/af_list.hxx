/* ORIGINAL: acis2.1/faceter/util/af_list.hxx */
// $Id: af_list.hxx,v 1.8 2001/01/11 18:02:48 ywoo Exp $
/*******************************************************************/
/*    Copyright (c) 1989-2020 by Spatial Corp.                     */
/*    All rights reserved.                                         */
/*    Protected by U.S. Patents 5,257,205; 5,351,196; 6,369,815;   */
/*                              5,982,378; 6,462,738; 6,941,251    */
/*    Protected by European Patents 0503642; 69220263.3            */
/*    Protected by Hong Kong Patent 1008101A                       */
/*******************************************************************/
#ifndef _AF_LIST
#define _AF_LIST



#include <stdio.h>
#include "dcl_fct.h"
// ywoo 08Jan01: added the header file.
#include "mmgr.hxx"
// ywoo: end.

class AF_LIST_HEADER;
//***********************************************************
// AF_LIST implements a collection of pointers as
//   a cyclic linked list of AF_LIST_CELL's.
//
// Caller program deals with the list via pointer to a header record
//  (AF_LIST_HEADER*).
//
//
// AF_LIST_CELL manipulates pairing cells with two pointers.
//	The caller is responsible for using linear and cyclic lists
//	consistently!!!!  (E.g. don't ask for a linear list count
//	in a cyclic list........)
//***********************************************************

#include "af_cllct.hxx"

class AF_LIST_CELL;
DECL_FCT void swap_successors(AF_LIST_CELL *p,AF_LIST_CELL *q);

typedef AF_LIST_CELL *P_AF_LIST_CELL;
int af_count_linear_list(P_AF_LIST_CELL);

class DECL_FCT AF_LIST_CELL  : public ACIS_OBJECT
{
   AF_LIST_CELL *next;
   void *data;
   friend DECL_FCT void swap_successors(AF_LIST_CELL*,AF_LIST_CELL*);
 public:
   friend class AF_LIST_HEADER;
   friend class AF_IHASH_HEADER;
	// Use the following functions to manipulate cells of
	// circular lists:
   AF_LIST_CELL(void *_data = NULL);

	// Use the following functions to manipulate cells of
	// linear linked lists. 
	// 1) Construct a cell with a specific successor:
   AF_LIST_CELL(P_AF_LIST_CELL _next,void *_data);
   friend int af_count_linear_list(P_AF_LIST_CELL);
};

class DECL_FCT AF_LIST_HEADER : public ACIS_OBJECT {
protected:
   AF_LIST_CELL *last;	// Tail pointer in cyclic list.
   // bmt - 20-Mar-09 the iterator is mixed up in the container, so make it mutable to allow iterations to be const; seems to be the lesser of evils.
   mutable AF_LIST_CELL *rover;	// Predecessor of next cell in traversal and
						//	and search operations.
 public:

	// Create an empty list.
  AF_LIST_HEADER();

	// Add a pointer to the list. Special placement modes
	// are:
	//		ADD_AT_TAIL (default)
	//		ADD_AT_HEAD
	//		ADD_UNIQUE
  void add(void *,AF_ADD_MODE mode = AF_ADD_AT_TAIL);

	// Reset the traversal pointer to read the entire list.
  void reset_traversal() const;

	// Read one pointer from the list and advance the traversal pointer.
  int read_traversal(void *&) const;

	// Inquire if a pointer is already on the list.
	// This is linear search from the true list head.
	// If found, (a) set the traversal rover to precede the cell
	//					containing the pointer and
	//			(b) return 0.
	// If not found, leave the traversal pointer alone and return 0.
  int search(void*);

	// Count the entries in the list.
  int count();

	// Clear the list, deleting cells along the way.
  void clear();

	// Clean up the header (and delete the lists cells)
  ~AF_LIST_HEADER();
};



#endif
