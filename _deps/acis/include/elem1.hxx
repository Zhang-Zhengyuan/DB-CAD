/* ORIGINAL: acis2.1/meshhusk/mesh/elem1.hxx */
/*******************************************************************/
/*    Copyright (c) 1989-2020 by Spatial Corp.                     */
/*    All rights reserved.                                         */
/*    Protected by U.S. Patents 5,257,205; 5,351,196; 6,369,815;   */
/*                              5,982,378; 6,462,738; 6,941,251    */
/*    Protected by European Patents 0503642; 69220263.3            */
/*    Protected by Hong Kong Patent 1008101A                       */
/*******************************************************************/

#if !defined( ELEM1D_CLASS )
#define ELEM1D_CLASS

// Header for base one-d element class.

#include "dcl_kern.h"
#include "elem.hxx"
#include "curdef.hxx"

class SPAposition;
class SPAvector;
class SPAunit_vector;
class surf_princurv;
class SPApar_pos;
class SPApar_vec;
class SPAtransf;
class SPAbox;
class SPApar_box;
class P1NODE;
class scan_list;
class large_ptr_array;
class surf_normcone;
class curve_surf_int;
class surf_surf_int;
class curve;
class straight;
class ellipse;
class intcurve;
class plane;
class cone;
class sphere;
class torus;
class spline;
class meshsurf;
class curve_bounds;
/**
 * @nodoc
 */
ENTITY_IS_PROTOTYPE(ELEM1D , KERN)
#if 0
; // semicolon needed for mkman (doc tool) parsing
#endif

// Identifier constant

extern DECL_KERN int ELEM1D_TYPE;

// Level of derivation from ENTITY

#define ELEM1D_LEVEL 2

// Define the element base class.

class DECL_KERN ELEM1D: public ELEM{

	// Node pointers.

	P1NODE *start_ptr;
	P1NODE *end_ptr;

	// Curve data.  Currently in heap storage to avoid make a union or
	// three types of 1D element, but could be changed if curves in heap
	// storage do not behave well.  No compcurvs allowed.

	curve *curve_ptr;

	// Store SPAbox pointer to save time.

	SPAbox *bound_ptr;

	// Standard ENTITY member functions.

	/**
	 * @nodoc
	 */
	ENTITY_FUNCTIONS( ELEM1D , KERN)
#if 0
; // semicolon needed for mkman (doc tool) parsing)
#endif
// Because mkman isn't aware that ENTITY_FUNCTIONS changes access to "public", do so
// explicitly here:
public:

//	TRANSFORM_FUNCTION

	// Generic constructor.

	ELEM1D():ELEM(){
		start_ptr = NULL;
		end_ptr = NULL;
		curve_ptr = NULL;
		bound_ptr = NULL;
	}

	// Basic constructor.

	ELEM1D(P1NODE *start, P1NODE *end, curve *cur):ELEM(){
		start_ptr = start;
		end_ptr = end;
		curve_ptr = cur;
		bound_ptr = NULL;
	}

	// Copy (bitwise).

	ELEM1D(const ELEM1D *old)
	{
		*this = *old;
	}

	// Sketch element in 3D.

	virtual void sketch(const SPAtransf &) const;

	// Check element validity.

	virtual void check(logical = FALSE) const;

	// Node reading functions.

	P1NODE *start() const { return start_ptr; }
	P1NODE *end() const { return end_ptr; }

	// Handy next and previous functions that do the type casting.

	ELEM1D *next() const { return (ELEM1D *)next_ptr; }
	ELEM1D *previous() const { return (ELEM1D *)previous_ptr; }

	// Curve reading function.

	const curve & equation() const { return *curve_ptr; }

	// Node setting function.

	void set_start(P1NODE *node) {
		backup();
		start_ptr = node;
	}

	void set_end(P1NODE *node) {
		backup();
		end_ptr = node;
	}

	// Bound and curve setting.

	void set_bound( SPAbox * );
	void set_equation(curve *c)
	{
		backup();

		if (curve_ptr != NULL && curve_ptr != c)
			ACIS_DELETE curve_ptr;

		curve_ptr = c;
	}

	// Pointer fixing.  Define this despite the existence of fix
	// common so that we can keep the indices of nodes and elements
	// seperate and also so that we can use our large_ptr_array
	// construct.

	virtual void fix_pointers(scan_list &);

	// Specialized copy routine uses

	virtual ELEM *copy( scan_list & ) const;

	// Specialized save routine.

	virtual void save(scan_list &) const;

	// Transform operator.  Does nothing at this level,
	// but may be needed if something geometric (like normals)
	// are stored in the element.

	virtual void operator*= (const SPAtransf &);

	// Comparison operator.  Default version compares node coordinates.

	virtual logical operator==(const ELEM *) const;

	// Const SPAbox computation.  Handy for the intersectors.

	virtual SPAbox bound() const;

	// Const version of param range.

	virtual SPAinterval param_range() const;

	// Compute the SPAparameter range SPAbox of this element, if
	// mesh parametrized.  The default returns an infinite
	// (note: not empty) SPAinterval so that the SPAbox will
	// overlap with any other (non-empty) SPApar_box.

	virtual SPAinterval param_range();

	// Box computation.  The default version does a node SPAbox, which may
	// be ok for simple elements.  It does not store the SPAbox.

	virtual SPAbox bound();

	// Not needed.  (Yet).

//	virtual logical inside(const real &) const;

//	virtual logical inside(const SPAposition&) const;

};

#endif
