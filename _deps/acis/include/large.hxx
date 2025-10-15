/* ORIGINAL: acis2.1/meshhusk/mesh/large.hxx */
/* $Id: large.hxx,v 1.15 2001/03/18 18:31:34 goetting Exp $ */
/*******************************************************************/
/*    Copyright (c) 1989-2020 by Spatial Corp.                     */
/*    All rights reserved.                                         */
/*    Protected by U.S. Patents 5,257,205; 5,351,196; 6,369,815;   */
/*                              5,982,378; 6,462,738; 6,941,251    */
/*    Protected by European Patents 0503642; 69220263.3            */
/*    Protected by Hong Kong Patent 1008101A                       */
/*******************************************************************/

#if !defined (LARGE_PTR_ARRAY)
#define LARGE_PTR_ARRAY

// Header file for pointer container class.  This class is intended
// to be used when a really long list of pointers needs to be
// stored and then indexed.  No facility for lookup is provided.
// This class allocates pointer arrays in page-sized blocks and
// stores them in a tree structure for fast random indexing.
// Less than random indexing is enhanced via a current block and
// current index pointer.

#include <stddef.h>
#include "dcl_kern.h"
// ywoo 08Jan01: added the header file.
#include "base.hxx"
// ywoo: end

// Tree branch factor.

#define BRANCH_FACTOR 8

class array_branches;

// Root level class.  Contains either a page-based block of void
// pointers or a subtree.  Stores the current open index in the
// block (if present), the recursion level (with the leaf level
// at 0) and a flag indicating if the pointer block or subtree
// is full and thus not eligible to receive new pointers.

class DECL_KERN large_ptr_array : public ACIS_OBJECT {
	void **ptr_block;		// Block of pointers.
	array_branches *child;	// Subtree container. 
	int open_index;			// Next unused slot in block.
	int level;				// Tree level.  Leaf = 0.
	logical full;			// Block or subtree is full.

public:

	int byte_count() const;

	// Constructor.
	large_ptr_array()
	{
		ptr_block = NULL;
		child = NULL;
		open_index = 0;
		level = 0;
		full = FALSE;
	}

	// Destructor gets rid of everything below the instance.

	~large_ptr_array();

	// Add new pointer.  No attempt is made to eliminate
	// duplicates.

	int add(void *, logical & = *(logical *)NULL_REF);

	// Index contents.  Contents are indexed in order added.

	void * operator[] (int) const;

	// Set contents.  Does nothing if outside range.

	void set(int, void *);

	// Clear out the array for reuse.

	void clear();

	// Is it empty?

	logical empty() const { return ptr_block == NULL; }

private:

	// Function to expand an empty tree branch all the way
	// to the current leaf level.  Expansion is leftmost only.

	void expand(void * = NULL);

	// Make the branch class a friend so that it can access member
	// data.

	friend class array_branches;
};

// Branch container class.  Contains a freelist pointer and an
// array of branch large_array_tables.

class DECL_KERN array_branches : public ACIS_OBJECT {
	array_branches *free_next;				// Freelist pointer.
	large_ptr_array table[BRANCH_FACTOR];	// Branches.
	const large_ptr_array *cur_array;		// Not-so-random-access
	int cur_index;							//   speedup data.
	int cur_local_index;

public:

	// Constructor.  The integer argument is the level to assign
	// to each branch.

	array_branches(int);

private:

	// Destructor.  Everything is erased automatically.

	~array_branches(){}

	// Branch class add routine.

	int add(void *, logical & = *(logical *)NULL_REF);

	// Branch class indexing.

	void * operator[] (int);

	// Branch class setting.

	void set(int, void *);

	// Make the array a friend so it has control over everything.

	friend class large_ptr_array;
};

#endif
