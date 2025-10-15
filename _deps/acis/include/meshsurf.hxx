/* ORIGINAL: acis2.1/meshhusk/geom/meshsurf.hxx */
/*******************************************************************/
/*    Copyright (c) 1989-2020 by Spatial Corp.                     */
/*    All rights reserved.                                         */
/*    Protected by U.S. Patents 5,257,205; 5,351,196; 6,369,815;   */
/*                              5,982,378; 6,462,738; 6,941,251    */
/*    Protected by European Patents 0503642; 69220263.3            */
/*    Protected by Hong Kong Patent 1008101A                       */
/*******************************************************************/

#if !defined( MESHSURF_CLASS )
#define MESHSURF_CLASS

// Header for MESHSURF.

// A MESHSURF, a class derived from SURFACE, records a 
// mesh surface as a (lower-case) meshsurf.  The meshsurf
// holds the arrays of coordinate nodes and elements that
// define a patchwork surface.

#include "dcl_kern.h"
#include "surface.hxx"

#include "mshdef.hxx"

class LOOP;
class SPAtransf;
class SPAbox;
/**
 * @nodoc
 */
ENTITY_IS_PROTOTYPE( MESHSURF, KERN )
#if 0
; // semicolon needed for mkman (doc tool) parsing
#endif

// Identifier used to find out (via identity() defined below) to what 
// an entity pointer refers.

extern DECL_KERN int MESHSURF_TYPE;


// Identifier that gives number of levels of derivation of this class
// from ENTITY.

#define MESHSURF_LEVEL 2


// MESHSURF declaration proper.

class DECL_KERN MESHSURF: public SURFACE {

	// Record a MESHSURF as a meshsurf.

	meshsurf def;


// STI ROLL begin - added virtual compare function for api_get_modified_faces
protected:
    virtual logical bulletin_no_change_vf(ENTITY const* other, logical identical_comparator) const;
// STI ROLL end

	// Include the standard member functions for all entities.

	/**
	 * @nodoc
	 */
	ENTITY_FUNCTIONS( MESHSURF , KERN)
#if 0
; // semicolon needed for mkman (doc tool) parsing)
#endif
// Because mkman isn't aware that ENTITY_FUNCTIONS changes access to "public", we do so
// explicitly here:
public:


	// Now the functions specific to MESHSURF.

	// Make a bare MESHSURF to be filled in later.

	MESHSURF();


	// Create a MESHSURF from a meshsurf.

	MESHSURF( meshsurf const & );


	// Data changing routine.  This routine checks 
	// that the record has been posted on the bulletin-board before 
	// performing the change.  If not, the routine provokes an error, 
	// so that the omission (forgetting to call backup() first) can 
	// be rectified in the program.  In production versions of the 
	// program, these checks may be disabled, to improve efficiency.

	void set_def( meshsurf const & );


	// Return the surface equation for reading only.

	surface const &equation() const;


	// Return the surface equation for reading only.

	surface &equation_for_update();


	// Get a new (lower-case) surface being the meshsurf of the 
	// MESHSURF, transformed if the given SPAtransf is non-null and 
	// reversed in sense if the logical is true.

	surface *trans_surface(
						SPAtransf const & = *(SPAtransf *)NULL_REF,
						logical = FALSE
					) const;


	// Transform the stored mesh in place.

	void operator*=( SPAtransf const & );


	// Make a bounding SPAbox for this surface, surrounded by a set of loops
	// of edges.  Use the SPAparameter-space bounding SPAbox of the loop to
	// limit the face SPAbox.

	//wseibold (Feb`02) added extra argument to create tighter face bounding boxes

	SPAbox make_box( LOOP * = NULL, // STL amt 26Jun03: default value added
			SPAtransf const *t = NULL, 
			logical tight_box = FALSE, 
			SPAbox *untransformed_box = NULL) const;

	// Functions for accessing and editing underlying data.

	ELEM2D *first_element();
	P2NODE *first_node();

	// Access to bounding boxes.

	par2_tree *boxes();

	// Remove element or node.  Takes care of bounding box
	// editing.  Does not check for node/element coherence, in other
	// words, a orphan node from an element unhook is not processed.

	void unhook(ENTITY *);

	// Blindly add a node or element to the global lists.  Update
	// the bounding boxes to know about any new element.

	void add(ENTITY *);

  // virtual void ENTITY::add() is hidden by add(ENTITY*).  
  // Expose it again by defining a pass thru here.
  void add() { SURFACE::add(); }

	// Do all of the prancing and dancing required to break up
	// an element given the original and the list of new ones.
	// Note:  this routine assumes that the old element will remain.

	void split_element(ELEM2D *, const ENTITY_LIST &);

	// STI ROLL
	void full_size(SizeAccumulator& est, logical countSelf = TRUE) const;
    // STI ROLL
};




#endif
