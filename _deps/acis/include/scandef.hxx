/* ORIGINAL: acis2.1/meshhusk/mesh/scandef.hxx */
/*******************************************************************/
/*    Copyright (c) 1989-2020 by Spatial Corp.                     */
/*    All rights reserved.                                         */
/*    Protected by U.S. Patents 5,257,205; 5,351,196; 6,369,815;   */
/*                              5,982,378; 6,462,738; 6,941,251    */
/*    Protected by European Patents 0503642; 69220263.3            */
/*    Protected by Hong Kong Patent 1008101A                       */
/*******************************************************************/

#if !defined( scan_list_CLASS )
#define scan_list_CLASS

// Class to store save, restore, and copy lists and arrays of
// node, elements, and mesh attributes.  This is required because
// of the special copy, save/restore, fix mechanism for entities
// contained within a mesh surface.  Also contains a use list
// so that each scan list (embedded within a com_cur or msh_sur)
// knows when it can clear itself.  This avoids leaving junk hanging
// around after a copy or fix.

#include "logical.h"

#include "acis.hxx"
#include "dcl_kern.h"

#include "lists.hxx"
#include "large.hxx"

class NODE;
class ELEM;
class ATTRIB_MESH;
class msh_sur;
class com_cur;

class DECL_KERN scan_list : public ACIS_OBJECT {

	// Entity lists for save and copy.

	ENTITY_LIST node_list_data;
	ENTITY_LIST elem_list_data;
	ENTITY_LIST att_list_data;

	// Arrays for fix.

	large_ptr_array node_array_data;
	large_ptr_array elem_array_data;
	large_ptr_array att_array_data;

	// Owner pointer.  Indicates whether owner is cur or
	// sur class.

	msh_sur *sur_ptr;
	com_cur *cur_ptr;

	// Flag indicating if the attribute list has been filled.
	// It is possible that it can be empty, whereas the node
	// and elements lists will contain entries if they have
	// been filled.

	logical atts_done;

	// User list.  Indicates that a curve or surface has or
	// will use the owner.  Note:  list is filled with lower
	// case curves and surfaces, not entities, so that we can
	// deal with it at the curve/surface save() or copy() level.

	ENTITY_LIST use_list_data;
	int use_count;

	// Array and list based functions.

	public:

	// Constructors and destructor.

	scan_list()
	{
		sur_ptr = NULL;
		cur_ptr = NULL;
		atts_done = FALSE;
		use_count = 0;
	}

	scan_list(msh_sur*, com_cur *);

	~scan_list() {}

	// Access functions.  Not const, as they may need to build the list anew.

	NODE *index_node(int);
	ELEM *index_element(int);
	ATTRIB_MESH *index_attrib(int);
	int lookup_node(NODE *);
	int lookup_element(ELEM *);
	int lookup_attrib(ATTRIB_MESH *);
	int byte_count() const;

	// List loading functions.  Useful for copying a portion of
	// a mesh.

	void add_node_list( NODE *node ) { node_list_data.add((ENTITY *)node); }
	void add_element_list( ELEM *elem ) { elem_list_data.add((ENTITY *)elem); }

	// Array loading functions.  Arrays are loaded by copy and
	// restore.

	void add_node( NODE *node ) { node_array_data.add(node); }
	void add_element( ELEM *elem ) { elem_array_data.add(elem); }
	void add_attrib( ATTRIB_MESH *att ) { att_array_data.add(att); }

	// Indicate a use.  If the curve or surface is in the list, it removes
	// it.  If it is not, it adds it.  It increments or decrements
	// the use count based on adding/removing (since list count()
	// does not take removal into account).

	void note_use( const void * );

	// Count functions.  Call lookup() first to ensure lists are
	// full.  Used by save.

	int count_node()
	{
		lookup_node(NULL);
		return node_list_data.count();
	}
	int count_elem()
	{
		lookup_element(NULL);
		return elem_list_data.count();
	}
	int count_attrib()
	{
		lookup_attrib(NULL);
		return att_list_data.count();
	}

	// Clear out the data.

	void clear_lists()
	{
		node_list_data.clear();
		elem_list_data.clear();
		att_list_data.clear();
	}
	void clear_arrays()
	{
		node_array_data.clear();
		elem_array_data.clear();
		att_array_data.clear();
	}

	ATTRIB_MESH *get_attrib_from_list(int i) const
	{
		if (!atts_done)
			((scan_list *)this)->lookup_attrib(0);

		return (ATTRIB_MESH *)att_list_data[i];
	}
};

#endif
