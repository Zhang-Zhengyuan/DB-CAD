/* ORIGINAL: acis2.1/faceter/util/af_sarr.hxx */
// $Id: af_sarr.hxx,v 1.11 2001/01/11 18:02:48 ywoo Exp $
/*******************************************************************/
/*    Copyright (c) 1989-2020 by Spatial Corp.                     */
/*    All rights reserved.                                         */
/*    Protected by U.S. Patents 5,257,205; 5,351,196; 6,369,815;   */
/*                              5,982,378; 6,462,738; 6,941,251    */
/*    Protected by European Patents 0503642; 69220263.3            */
/*    Protected by Hong Kong Patent 1008101A                       */
/*******************************************************************/
#ifndef _AF_SARR
#define _AF_SARR



#include <stdio.h>
#include "dcl_fct.h"
// ywoo 08Jan01: added the header file.
#include "mmgr.hxx"
// ywoo: end

class AF_STRUCT_ARRAY;
////////////////////////////////////////////////////////////
// AF_STRUCT_ARRAY implements an array of fixed size structures.
//
// Caller program deals with the array via pointer to a header record
//  (AF_STRUCT_ARRAY*).
//
// q=AF_STRUCT_ARRAY(structsize,blocksize)	Create an empty array.
// q->clear()	Remove all members from the array.
// q->count()	Remove all members from the array.
// q->get_member_pointer(i)	Return a pointer to the i'th member.
// q->get_last_member_pointer() Return a pointer to the last member
//		The pointer is NOT valid over any requests for new members.
// index =q->add_members(nadd) 	Return the index of the first of nadd new
//				members.  THIS OPERATION MAY INVALIDATE ALL PRIOR MEMBER
//				POINTERS.
// P=q->add_member() Return the pointer to one new member. This
//		pointer is NOT valid over any requests for new members.
////////////////////////////////////////////////////////////

#include "af_cllct.hxx"
#define MIN_STRUCTURE_BLOCKSIZE 16
#define DEFAULT_STRUCTURE_BLOCKSIZE 32
class DECL_FCT AF_STRUCT_ARRAY /*: public ACIS_OBJECT PRIVATE DERIVATION PROBLEM*/ {
	unsigned int structsize;	// Size of each structure, in void*'s.
	unsigned int blocksize;	// Size of first allocation
	unsigned int m;			// Current allocation
	unsigned int n;			// Current number of entries
	int rover;		// index of next item to read.
	void **ptr;		// the memory block
	void expand(unsigned minsize);
 public:
  AF_STRUCT_ARRAY(int size,int blocksize = DEFAULT_STRUCTURE_BLOCKSIZE);

  /**
   * @nodoc
   */
  MMGR_SUPPORT_THIS
#if 0
; // semicolon needed for mkman (doc tool) parsing
#endif

  void add(void *,AF_ADD_MODE mode = AF_ADD_AT_TAIL);
  void reset_traversal();
  int read_traversal(void *&);
  int count();
  void clear();
  int add_members(unsigned nadd);
  void * add_member();
  void *get_member_pointer(int index);
  void sort(int compare(const void *,const void*));
  void apply(void (*F)(void *,void*),void* arg);

  void *get_last_member_pointer();
  int get_last_member_index();
  void delete_last_member();
  ~AF_STRUCT_ARRAY();
};

DECL_FCT void af_sarr_free_storage();

#endif
