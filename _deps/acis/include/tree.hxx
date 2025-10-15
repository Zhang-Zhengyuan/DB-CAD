/* ORIGINAL: acis2.1/meshhusk/mesh/tree.hxx */
/* $Id: tree.hxx,v 1.21 2002/08/09 17:15:34 jeff Exp $ */
/*******************************************************************/
/*    Copyright (c) 1989-2020 by Spatial Corp.                     */
/*    All rights reserved.                                         */
/*    Protected by U.S. Patents 5,257,205; 5,351,196; 6,369,815;   */
/*                              5,982,378; 6,462,738; 6,941,251    */
/*    Protected by European Patents 0503642; 69220263.3            */
/*    Protected by Hong Kong Patent 1008101A                       */
/*******************************************************************/

#if !defined (MESH_BOUND_TREE)
#define MESH_BOUND_TREE

// Include file for subdivision bounding tree classes used by
// meshsurf/compcurve. Used to hierarchically bound the elements
// that compose a mesh or composite curve.

// The root of the tree is either an r2tree or r1tree class.  The r2tree
// is a 2D SPAparameter space bounding tree and the r1tree is a 1D SPAparameter
// space bounding tree.  Each of these root objects contains a SPApar_box or
// an SPAinterval and a pointer to a tree branch instance.  The top level
// SPAbox is the outermost extent of the bounded region.

// The tree proper is composed of two classes, tree_branches and tree_leaf
// which are both derived from tree.  They share a number of virtual functions,
// which means they can be dealt with without much type casting.  Declaring
// them distinct types saves a little memory, since leaves (which are
// always plentiful) need not store unused branch information.  Each leaf
// contains a pointer to a tree_list instance, which store the pointers
// to elements.

// The spatial subdivision is a straightforward binary scheme.  Each tree
// is divided into two subtrees, a left, and a right.  They represent
// a division of the domain into halves along the longest dimension of
// the tree's SPAbox.  Any elements that straddle the dividing line are stored
// in the subtree into which their SPAbox "mostly" falls, with perfect
// straddling going to the left side.

// Each tree also stores its split direction (u or v) if relevant
// its partition value (SPAposition in the split direction at which the
// split occurs, usually the middle) and the high and low partition
// values of the center SPAbox.  Storing the partition value permits arbitrary
// expansion of the root level SPAbox (although not currently implemented).
// Storing the SPAbox hi/lo bounds takes care of the overlap that the two
// boxes will almost always have.  Each tree can also optionally store
// R3 bounding boxes and normal/tangent cones.  These are only computed
// upon demand and are allocated on the heap.  Thus, these subdivision
// partitions are also used as spatial bounding schemes.

// This approach permits the incremental addition of elements to
// the tree but does not take up the amount of memory demanded
// by octrees (due to their multiple references to objects bounded).

// The size of the largest element with respect to the current tree SPAbox
// is used to control subdivision.  If the current tree has a "large"
// element, it is marked as FULL and never subdivided any further as
// subdivision will usually be pointless.

#include "dcl_kern.h"
#include "logical.h"
#include <stddef.h>

#include "param.hxx"
#include "box.hxx"
#include "surdef.hxx"

// Forward references.

class mesh_tree;
class tree_branches;
class tree_leaf;
class tree_list;

class ELEM;
class ELEM1D;
class ELEM2D;
class SPAtransf;
class scan_list;

// 2D SPAparameter tree class definition.  This makes a search for an element
// associated with a uv value fast.

class DECL_KERN par2_tree : public ACIS_OBJECT {

	// The domain of the par_tree.

	SPApar_box domain_data;

	// Pointer to the branch data.

	tree_branches *branches_ptr;

public:

	// Constructor and destructor.  The destructor deletes the branches.

	par2_tree(const SPApar_box& domain, tree_branches *bptr = NULL)
	{
		domain_data = domain;
		branches_ptr = bptr;
	}
	~par2_tree();

	// Make a copy.  If the scan lists are non-null, make a copy/fix
	// copy.  Otherwise point to the same elements.

	par2_tree *copy(scan_list & = *(scan_list *)NULL_REF,
				scan_list & = *(scan_list *)NULL_REF ) const;

	// Reading functions.  The tree classes are sometimes searched
	// in non-member functions.

	SPApar_box domain() const { return domain_data; }
	tree_branches *branches() const { return branches_ptr; }

	// Add new element to tree.  The logical returns false if the
	// add failed.  Since there is a sliding partition in each
	// tree, this can be made to work if needed by merely expanding
	// the top level SPAbox.  Note that adding requires that the new
	// element be compatible in SPAparameter value anyway, so this function
	// may have limited applicability.  It does not update the bounding
	// boxes or the normal cones.

	logical add(ELEM2D *);

	// Remove all traces of this element.

	void remove(ELEM2D *ptr);

	// Mark all of the boundary elements at all levels, one deep.

	void mark_boundaries();

	// Find a leaf that is relatively "close" to the given SPAposition.
	// No guarantees are made here, so this should be used as a (we
	// hope) good start point for a search.

	const tree_leaf *find_close_leaf(const SPAposition&) const;

	// Find a list of elements, one of which is guaranteed to contain
	// the SPAposition if any do.

	tree_list *find_candidates(const SPApar_pos &) const;

	// Find the list_ptr that contains the given element;

	tree_list *find_list(ELEM2D *);

	// Range-based bounding SPAbox function.

	virtual SPAbox bound(const SPApar_box & = *(SPApar_box *)NULL_REF);

	// Transform.

	virtual void operator*=(const SPAtransf &);

	// Debugging.

	void debug(FILE * = debug_file_ptr) const;

// STI cpp begin ( add byte counter )
	int byte_count();
// STI cpp end

};

// 1D SPAparameter tree class definition.  This makes a search for an element
// associated with a t value fast.

class DECL_KERN par1_tree : public ACIS_OBJECT {

	// The domain of the par_tree.

	SPAinterval domain_data;

	// Pointer to the branch data.

	tree_branches *branches_ptr;

public:

	// Constructor and destructor.  The destructor deletes the branches.

	par1_tree(const SPAinterval& domain, tree_branches *bptr = NULL)
	{
		domain_data = domain;
		branches_ptr = bptr;
	}
	~par1_tree();

	// Make a copy.  If the scan lists are non-null, make a copy/fix
	// copy.  Otherwise point to the same elements.

	par1_tree *copy(scan_list & = *(scan_list *)NULL_REF,
				scan_list & = *(scan_list *)NULL_REF ) const;

	// Reading functions.  The tree classes are sometimes searched
	// in non-member functions.

	SPAinterval domain() const { return domain_data; }
	tree_branches *branches() const { return branches_ptr; }

	// Add new element to tree.  The logical returns false if the
	// add failed.  Since there is a sliding partition in each
	// tree, this can be made to work if needed by merely expanding
	// the top level SPAbox.

	// Add does not update the bounding boxes or tangent cones.

	logical add(ELEM1D *);

	// Add a group at once (for speed).  The two elements given
	// delimit the group.

	logical add(ELEM1D *, ELEM1D *, int);

	// Remove all traces of this element.

	void remove(ELEM1D *ptr);

	// Find a leaf that is relatively "close" to the given SPAposition.
	// No guarantees are made here, so this should be used as a good
	// start point for a search.

	const tree_leaf *find_close_leaf(const SPAposition&) const;

	// Find one (of the possibly two) elements that contains this
	// param.

	ELEM1D *find_element(const double &) const;

	// Transform.

	virtual void operator*=(const SPAtransf &);

	// Debugging.

	void debug(FILE * = debug_file_ptr) const;

// STI cpp begin ( add byte_counter )
	int byte_count();
// STI cpp end
};

// Abstract tree class from which tree_branches and tree_leaf
// are derived.  The member functions reflect the needs of both classes,
// as well as the two spaces (R3 and R2) that can be subdivided.

class DECL_KERN mesh_tree : public ACIS_OBJECT {

protected:

	// Normal or tangent cone in R3 for contained elements.  This
	// is used by the intersectors.  Use the surface normal cone.
	// It is basically the same (as of this writing) as the curve
	// tangent cone and can be used in the same manner.

	surf_normcone *cone_ptr;

	// R3 SPAbox of contained elements.  Used by the intersectors.

	SPAbox *bound_ptr;

	// Marker field used in intersection and element add algorithm.

	short bitfield;

public:

	// Constructor and destructor.

	mesh_tree()
	{
		cone_ptr = NULL;
		bound_ptr = NULL;
		bitfield = 0;
	}

	virtual ~mesh_tree();

	logical marked(int value = 1) const { return (bitfield & value) != 0; }

	void set_mark( int value = 1) { bitfield |= value; }
	void set_unmark( int value = 1) { bitfield &= ~value; }

	// Read the normal/tangent cone and SPAbox data.

	virtual surf_normcone cone() const = 0;
	virtual SPAbox bound() const = 0;
	virtual SPAbox bound(const SPApar_box &, const SPApar_box &) const = 0;

	// We also have non-const readers.

	virtual surf_normcone cone() = 0;
	virtual SPAbox bound() = 0;
	virtual SPAbox bound(const SPApar_box &, const SPApar_box &) = 0;

	// Make a copy.  If the scan lists are non-null, make a copy/fix
	// copy.  Otherwise point to the same elements.

	virtual mesh_tree *copy(scan_list & = *(scan_list *)NULL_REF,
				scan_list & = *(scan_list *)NULL_REF ) const = 0;

	// Return the list of elements in a leaf (if leaf, otherwise NULL).

	virtual tree_list *list() const = 0;

	// Entry count for leaf (if leaf, otherwise 0).

	virtual int count() const = 0;

	// Return the left, right branch (always NULL for leaf).

	virtual mesh_tree *left() const = 0;
	virtual mesh_tree *right() const = 0;

	virtual void set_left(mesh_tree *) = 0;
	virtual void set_right(mesh_tree *) = 0;

	// Add a new element to a par2_tree.

	virtual void add(
				const SPApar_box &,
				ELEM2D *,
				const SPApar_box &,
				int = -1
			) = 0;

	// Add a new element to a par1_tree.

	virtual void add(
				const SPAinterval &,
				ELEM1D *,
				const SPAinterval &
			) = 0;

	// Add a par1 group at once (for speed).  The two elements given
	// delimit the group.

	virtual void add(
					ELEM1D *,
					ELEM1D *,
					const SPAinterval &,
					int) = 0;

	// Find and remove all traces of this element from a par2_tree.

	virtual logical remove(const SPApar_box &, ELEM2D *, const SPApar_box &,
		logical * = NULL ) = 0;

	// Find and remove all traces of this element from a par1_tree.

	virtual logical remove(const SPAinterval &, ELEM1D *, const SPAinterval &) = 0;

	// Mark all of the boundary elements.

	virtual void mark_boundaries(int, logical) = 0;

	// Marking utilities.

	virtual void search_mark(int, logical) = 0;
	virtual void tickmark(logical) = 0;

	// Clear out R3 boxes associated with this element.

	virtual void clear_boxes(const SPApar_box &, ELEM2D *, const SPApar_box &) = 0;
	virtual void clear_boxes(const SPAinterval &, ELEM1D *, const SPAinterval &) = 0;

	// Find a leaf that is relatively "close" to the given SPAposition.
	// No guarantees are made here, so this should be used as a good
	// start point for a search.

	const tree_leaf *find_close_leaf(const SPAposition&) const;

	// Find one (of the possibly two) elements that contains this
	// param.

	virtual ELEM1D *find_element(const double &, const SPAinterval &) const = 0;

	// Transform.

	virtual void operator*=(const SPAtransf &) = 0;

	// Count branches, leaves, etc, for debugging.

	virtual void count_stuff(int = 0) const = 0;

};

// Tree branch class.  Each branch is a binary subdivision in either
// x (or u), y (or v), or z, with the group of elements that straddle
// the center stored separately.  This is a VERY flexible and tight
// approach, as elements can be added one-by-one yet each element
// is only referenced once in the tree.  (In a standard octree, either
// each element can be referenced multiple times or the center elements
// remain at the top.  The former is very fast but VERY large.  The latter
// is smaller but slow because the SPAbox for the center elements is much
// too large.)  This approach is flexible because a sliding partition
// can be implemented to allow the domain to grow and the center
// elements can be split in a hierarchy when appropriate.

class DECL_KERN tree_branches:public mesh_tree{

	// Direction (u or v) of the split.  Irrelevant for par1_trees,
	// since they are smaller anyway (in general), share the class.

	short direction_data;

	double center_data;
	double lo_data;
	double hi_data;

	// The subtrees.  The right and left tree reflect complete
	// containment to the right and left of the partitions.

	mesh_tree *left_ptr;
	mesh_tree *right_ptr;

public:

	// Constructors and destructor.

	tree_branches(const SPApar_box &);
	tree_branches(const SPAinterval &);
	tree_branches(const tree_branches *old)
	{
		*this = *old;
		this->left_ptr = NULL;
		this->right_ptr = NULL;
		this->bound_ptr = NULL;
		this->cone_ptr = NULL;
		bitfield = 0;
	}

	virtual ~tree_branches();

	// Make a copy.  If the scan lists are non-null, make a copy/fix
	// copy.  Otherwise point to the same elements.

	virtual mesh_tree *copy(scan_list & = *(scan_list *)NULL_REF,
				scan_list & = *(scan_list *)NULL_REF ) const;

	// Bound computation.  Const and non-const.

	virtual surf_normcone cone() const;
	virtual surf_normcone cone();
	virtual SPAbox bound() const;
	virtual SPAbox bound();
	virtual SPAbox bound(const SPApar_box &, const SPApar_box &) const;
	virtual SPAbox bound(const SPApar_box &, const SPApar_box &);

	// Returns NULL.

	virtual tree_list *list() const;

	// Entry count for leaf.  Returns 0.

	virtual int count() const;

	// Return the left, right, and center branch.

	virtual mesh_tree *left() const;
	virtual mesh_tree *right() const;

	// Set the branches.

	virtual void set_left(mesh_tree *);
	virtual void set_right(mesh_tree *);

	// Add a surface element to a tree branch.

	virtual void add(
				const SPApar_box &,
				ELEM2D *,
				const SPApar_box &,
				int = -1
			);

	// Add a curve element to a tree branch.

	virtual void add(
				const SPAinterval &,
				ELEM1D *,
				const SPAinterval &
			);

	// Add a par1 group at once (for speed).  The two elements given
	// delimit the group.

	virtual void add(
				ELEM1D *,
				ELEM1D *,
				const SPAinterval &,
				int);

	// Find and remove all traces of this element from a tree.

	virtual logical remove(const SPApar_box &, ELEM2D *, const SPApar_box &,
		logical * = NULL);
	virtual logical remove(const SPAinterval &, ELEM1D *, const SPAinterval &);

	// Mark all of the boundary elements.

	virtual void mark_boundaries(int, logical);

	// Marking utilities.

	virtual void search_mark(int, logical);
	virtual void tickmark(logical);

	// Clear out R3 boxes associated with this element.

	virtual void clear_boxes(const SPApar_box &, ELEM2D *, const SPApar_box &);
	virtual void clear_boxes(const SPAinterval &, ELEM1D *, const SPAinterval &);

	// Transform.

	virtual void operator*=(const SPAtransf &);

	// Compute the SPAparameter SPAbox of the split or center given
	// the current SPAbox.

	SPApar_box left_box(const SPApar_box&) const;
	SPApar_box right_box(const SPApar_box&) const;

	SPAinterval left_box(const SPAinterval&) const;
	SPAinterval right_box(const SPAinterval&) const;

	// Compute the NOMINAL SPAparameter SPAbox.  This uses the
	// center point instead of the hi or lo points.

	SPApar_box left_nominal_box(const SPApar_box&) const;
	SPApar_box right_nominal_box(const SPApar_box&) const;

	SPAinterval left_nominal_box(const SPAinterval&) const;
	SPAinterval right_nominal_box(const SPAinterval&) const;

	// Find a list of elements, one of which contains the test
	// point (if any do).

	tree_list *find_candidates(const SPApar_pos &, const SPApar_box &, SPApar_box) const;

	// Find the list pointer.

	tree_list *find_list(ELEM2D *, const SPApar_box&, const SPApar_box&);

	// Find one (of the possibly two) elements that contains this
	// param.

	virtual ELEM1D *find_element(const double &, const SPAinterval &) const;

	// Count branches, leaves, etc, for debugging.

	virtual void count_stuff(int = 0) const;
};

// Class for tree or tree leaf.  Merely contains the list
// pointer and an entry count so that we can tell that a leaf
// is getting too full.

class DECL_KERN tree_leaf:public mesh_tree{

	tree_list *list_ptr;

	int entry_count_data;

public:

	// Constructor and destructor.

	tree_leaf(int count = 0):mesh_tree()
	{
		list_ptr = NULL;
		entry_count_data = count;
	}
	virtual ~tree_leaf();

	// Make a copy.  If the scan lists are non-null, make a copy/fix
	// copy.  Otherwise point to the same elements.

	virtual mesh_tree *copy(scan_list & = *(scan_list *)NULL_REF,
				scan_list & = *(scan_list *)NULL_REF ) const;

	// Read the list pointer.

	virtual tree_list *list() const;

	// Compute the SPAbox.

	virtual surf_normcone cone() const;
	virtual surf_normcone cone();
	virtual SPAbox bound() const;
	virtual SPAbox bound();
	virtual SPAbox bound(const SPApar_box &, const SPApar_box &) const;
	virtual SPAbox bound(const SPApar_box &, const SPApar_box &);

	// Entry count for leaf.

	virtual int count() const;

	// Returns NULL for this class.

	virtual mesh_tree *left() const;
	virtual mesh_tree *right() const;

	// These do nothing.

	virtual void set_left(mesh_tree *);
	virtual void set_right(mesh_tree *);

	// Both add functions do the same thing.  (They ignore
	// the boxes).  They create a list object, place it at the
	// head of the list, and store the element in it.

	virtual void add(const SPApar_box &, ELEM2D *, const SPApar_box &, int = -1);
	virtual void add(const SPAinterval &, ELEM1D *, const SPAinterval &);

	// Add a par1 group at once (for speed).  The two elements given
	// delimit the group.

	virtual void add( ELEM1D *, ELEM1D *, const SPAinterval &, int );

	// Find and remove all traces of this element from this leaf.

	virtual logical remove(const SPApar_box &, ELEM2D *, const SPApar_box &,
		logical * = NULL);
	virtual logical remove(const SPAinterval &, ELEM1D *, const SPAinterval &);

	// Mark all of the boundary elements.

	virtual void mark_boundaries(int, logical);

	// Marking utilities.

	virtual void search_mark(int, logical);
	virtual void tickmark(logical);

	// Clear out R3 boxes associated with this element.

	virtual void clear_boxes(const SPApar_box &, ELEM2D *, const SPApar_box &);
	virtual void clear_boxes(const SPAinterval &, ELEM1D *, const SPAinterval &);

	// Find one (of the possibly two) elements that contains this
	// param.

	virtual ELEM1D *find_element(const double &, const SPAinterval &) const;

	// Transform.

	virtual void operator*=(const SPAtransf &);

	// Set the list pointer.

	void set_list(tree_list *ptr) { list_ptr = ptr; }

	// Count branches, leaves, etc, for debugging.

	virtual void count_stuff(int = 0) const;
};

// Container class for a set of elements in a leaf.
// Since there will be so many, this is registered
// with the free list manager.

class DECL_KERN tree_list : public ACIS_OBJECT {
public:
	tree_list *next_ptr;
	ELEM *contents_ptr;
	int level;

	tree_list(tree_list *next, ELEM* contents, int lev = -1)
	{
		next_ptr = next;
		contents_ptr = contents;
		level = lev;
	}
	~tree_list()
	{
		if (next_ptr != NULL) ACIS_DELETE next_ptr;
	}
};

#endif
