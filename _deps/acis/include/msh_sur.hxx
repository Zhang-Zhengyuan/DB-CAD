/*******************************************************************/
/*    Copyright (c) 1989-2020 by Spatial Corp.                     */
/*    All rights reserved.                                         */
/*    Protected by U.S. Patents 5,257,205; 5,351,196; 6,369,815;   */
/*                              5,982,378; 6,462,738; 6,941,251    */
/*    Protected by European Patents 0503642; 69220263.3            */
/*    Protected by Hong Kong Patent 1008101A                       */
/*******************************************************************/
#ifndef msh_sur_hxx
#define msh_sur_hxx

#include "acis.hxx"
#include "dcl_kern.h"
#include "logical.h"
#include "tree.hxx"
class P2NODE;
class ELEM2D;
class ENTITY;
class meshsurf;
class curve;
class SPAposition;
class SPAvector;
class SPAunit_vector;
class SPAinterval;
class SPAbox;
class SPApar_box;
class SizeAccumulator;
typedef logical REVBIT;

/**
 * @nodoc
 * Class to indicate which element to evaluate on if not
 * averaging.
 */
class DECL_KERN neighborhood : public ACIS_OBJECT {
public:
    /**
     *If set, this element is our neighborhood.
     */
	ELEM2D *elem;			// 
    /**
     *
     */
	neighborhood() { elem = NULL; }
    /**
     *
     */
	~neighborhood() {};
    /**
	 * Determine whether a given element is in our neighborhood.
	 * For now, just see if it is the same as the element set.
     */
	logical in(ELEM2D *test_elem){
		return test_elem == elem;
	}
};


/**
 * @nodoc
 * Actual container class for mesh data.  Defined separately with a use
 * count so that we can defer copying the enormous quantity of data
 * and so that we can derive from this class to define new mesh classes.
 * There are virtual functions to support all the functions defined
 * for the class meshsurf which depend on the true definition of the
 * surface.
 * The base class is pure: classes for real mesh surface types must be
 * derived from it.
 */
class DECL_KERN msh_sur : public ACIS_OBJECT {
protected:

	// Use count data.

	int use_count;

	// Store current evaluation mode (local or averaged).

	logical average_evaluation;
	neighborhood neighborhood_data;

	// Flag to indicate whether contents have already been lost.

	logical lost;

	// Lists for save, restore, copy.

	scan_list scan_list_data;

	// Bounding scheme.  We subdivide in parameter space and bound
	// each of these in three-space.

	par2_tree *boxes_ptr;

	// Store the current node and element for fast sequential traversal.

	P2NODE *current_node_ptr;
	ELEM2D *current_elem_ptr;

	// The actual heads of the node and element lists.

	P2NODE *node_ptr;
	ELEM2D *element_ptr;

	// Store a current element for speedup of the eval functions.

	mutable ELEM2D *current_element;
	ENTITY_LIST *link_attribs;

	// Null constructor.

	msh_sur();

	// Default destructor can do a lot of the work.

	virtual ~msh_sur();

	// Define a lose function because the nodes and elements are
	// entities.

	void lose();

	// Build the bounding tree.  Make this private because
	// we only create these in specific circumstances.

	void build_boxes();

private:
	// Use count manipulation

	void operator++();
	void operator--();
	logical mult();

public:

	// STI ROLL
	int ref_count() { return use_count; };

	// Reading and setting functions.  We allow reading of
	// nodes and elements even for const meshes since it
	// is changes to the msh_sur class we wish to avoid.
	// Dealing with const entities is too troublesome.

	P2NODE *node() const { return node_ptr; }
	ELEM2D *element() const { return element_ptr; }

	void set_node(P2NODE *n) { node_ptr = n; }
	void set_element(ELEM2D *el) { element_ptr = el; }

	// Set the lose data.

	void set_lost(logical l) { lost = l; }

	// Read the scan list.  Non-const.

	scan_list &get_scan_list() { return scan_list_data; }

	// There are no traversal, indexing, or lookup functions.
	// If fast indexing is required, a temporary large_ptr_array
	// should be created.  If fast lookup is required, a temporary
	// entity list should be created.  These requirements will
	// crop up largely in save, restore, and copy, which
	// are implemented below anyway.
    /**
	 * Read the bounding SPAbox trees.  Create them if they do
	 * not exist (from rollback, for instance).
    */
	par2_tree *boxes()
	{
		if (boxes_ptr == NULL) build_boxes();

		return boxes_ptr;
	}

	// Const version for looking.

	const par2_tree *boxes() const
	{
		if (boxes_ptr == NULL) ((msh_sur *)this)->build_boxes();

		return boxes_ptr;
	}

	// Remove boxes when they become invalid.

	void remove_boxes()
	{
		ACIS_DELETE boxes_ptr;
		boxes_ptr = NULL;
	}
   /**
	* Find the nodes on the outer boundary (the one with the most
	* nodes) and optionally the hole boundaries.
    */
	virtual large_ptr_array * find_seam_nodes(
								int &,
								large_ptr_array **&,
								int &,
								int *&
							) const;
   /**
    * Find all non-boundary (i.e. closed) nodes and return them
	* in a large_ptr_array.
    */
	virtual large_ptr_array * find_interior_nodes( int & ) const;
    /**
	 * Return value indicating whether surface is parametric.
	 * (TRUE for the default).
     */
	virtual logical parametric() const;
    /**
	 * Mesh checker.  As this function will print to the debug_file_ptr,
	 * it is only to be used during debugging.
     */
	virtual void check() const;
    /**
	 * Set the current evaluation neighborhood.  If called, the evaluators
	 * will compute data based on a single element instead of averaging
	 * when within tolerance of an element boundary or node.
     */
	void set_neighborhood(const neighborhood &neigh)
	{
		average_evaluation = FALSE;
		neighborhood_data = neigh;
	}

	// Simple object-space bounding SPAbox, for a subset of the surface.
	// The default version uses the object-space mesh, expanding
	// its SPAbox by the fit tolerance. Note that this cannot be const,
	// as it uses subset(), which is itself not const.

	virtual SPAbox bound( SPApar_box const & = *(SPApar_box *)NULL_REF );

	// Return a cone bounding the normal direction of the surface.

	virtual surf_normcone normal_cone(
				SPApar_box const & = *(SPApar_box *)NULL_REF,
				logical = FALSE
			);

	// Here is the copy that we will really use since we require
	// the many pointers to be converted to integers prior to
	// fixing.

	virtual msh_sur *copy(const ENTITY_LIST & = *(ENTITY_LIST *)NULL_REF) const = 0;

	// Just copy pointers to nodes and elements.  Used in backup.

	virtual msh_sur *copy_pointers() const = 0;

	// Non-virtual copy to contain the generic copy functionality
	// that can be called from most copy() functions.  Needs the
	// same arguments as our specialized copy above.

	void copy_data(
				const msh_sur *,
				const ENTITY_LIST &
		);


	// Make a copy without sharing subdata.
	virtual msh_sur *deep_copy(pointer_map * pm = NULL) const = 0;

	
	// Pointer fixing for after copy and restore.  The entity array
	// is the global pointer fixing array, and the msh_sur
	// holds all the local (to this surface) arrays.

	virtual void fix_pointers( ENTITY** );

	// Scan function to fill up a msh_sur's entity lists
	// with nodes, elements and mesh attributes.

	virtual void scan();

	// Zero tolerance equality check.

	virtual logical operator==( msh_sur const & ) const;

	// Transformation operator.  Transforms mesh in place.

	virtual void operator*=( SPAtransf const & );

	// Find the normal to the surface at the given point.

	virtual SPAunit_vector point_normal(
				SPAposition const &,
				SPApar_pos const & = *(SPApar_pos *)NULL_REF
			) const;

	// Return a direction which points outward from the surface.

	virtual SPAunit_vector point_outdir(
				SPAposition const &,
				SPApar_pos const & = *(SPApar_pos *)NULL_REF
			) const;

	// Find the principal axes of curvature of the surface at a
	// given point, and the curvatures in those directions.

	virtual void point_prin_curv(
				SPAposition const &,
				SPAunit_vector &,		// first axis direction
				double &,			// curvature in first direction
				SPAunit_vector &,		// second axis direction
				double &,			// curvature in second direction
				SPApar_pos const & = *(SPApar_pos *)NULL_REF
			) const;

	// Find the curvature of a cross-section curve of the surface at
	// the point on the surface closest to the given point.

	virtual double point_cross(
				SPAposition const &,
				SPAunit_vector const &,
				SPApar_pos const & = *(SPApar_pos *)NULL_REF
			) const;

	// Return a (or one of the) elements near this point.

	virtual ELEM2D *point_element(
				SPAposition const &,
				SPApar_pos const & = *(SPApar_pos *)NULL_REF
			) const;

	// Utility function to do some recursion on an par2_tree.  This
	// function really doesn't belong in the par2_tree class proper,
	// so we do it here.

	SPAposition closest_point(
					mesh_tree *,
					const SPAposition&,
					double &,
					ELEM2D *&,
					SPApar_pos &
				) const;

	// Find the point on the surface nearest to the given point, 
	// iterating from the given parameter values (if supplied).
	// Optionally return an element hit.

	virtual void point_perp(
				SPAposition const &,
				SPAposition &,
				SPAunit_vector &,
				surf_princurv &,
				SPApar_pos const & = *(SPApar_pos *)NULL_REF,
				SPApar_pos & = *(SPApar_pos *)NULL_REF,
				ELEM2D *& = *(ELEM2D **)NULL_REF
			) const;

	// Find the parameter values of a point on the surface.

	virtual SPApar_pos param(
				SPAposition const &,
				SPApar_pos const & = *(SPApar_pos *)NULL_REF
			) const;

	// Find the change in surface parameter corresponding to a unit
	// offset in a given direction at a given position, the position
	// and direction both lying in the surface.

	virtual SPApar_vec param_unitvec(
				SPAunit_vector const &,
				SPApar_pos const &
			) const;

	// Find the position and first and second derivatives of the
	// surface at a given point.

	virtual void eval(
				SPApar_pos const &uv,
				SPAposition &pos,
				SPAvector *dpos,	// first derivatives - array of
								// length 2 in order xu, xv
				SPAvector *ddpos	// second derivatives - array of
								// length 3 in order xuu, xuv, xvv
			) const;

   /**
	* Find one of the elements that is associated with this
	* parameter value, if any.
    */
	ELEM2D *eval_element(const SPApar_pos &) const;

	// Find the point on the surface with given parameter values.

	virtual SPAposition eval_position( SPApar_pos const & ) const;

 	// Find the normal to the surface at the point with given 
	// parameter values.

	virtual SPAunit_vector eval_normal( SPApar_pos const & ) const;

	// Return a direction which points outward from the surface. This
	// will be the same as eval_normal, since eval_normal will by
	// default do normal averaging on seams.

	virtual SPAunit_vector eval_outdir( SPApar_pos const & ) const;

	// Find the principal axes of curvature of the surface at a
	// point with given parameter values, and the curvatures in those
	// directions.

	virtual void eval_prin_curv(
				SPApar_pos const &,
				SPAunit_vector &,		// first axis direction
				double &,			// curvature in first direction
				SPAunit_vector &,		// second axis direction
				double &			// curvature in second direction
			) const;

	// Find the curvature of a cross-section curve of the surface at
	// the point on the surface with given parameter values.

	virtual double eval_cross( SPApar_pos const &, SPAunit_vector const & ) const;

	// The evaluate() function calculates derivatives, of any order
	// up to the number requested, and stores them in vectors provided
	// by the user. It returns the number it was able to calculate;
	// this will be equal to the number requested in all but the most
	// exceptional circumstances. A certain number will be evaluated
	// directly and (more or less) accurately; higher derivatives will
	// be automatically calculated by finite differencing; the accuracy
	// of these decreases with the order of the derivative, as the
	// cost increases.

	virtual int evaluate(
                SPApar_pos const &,	// Parameter
                SPAposition &,			// Point on surface at given parameter
                SPAvector ** = NULL, 	// Array of pointers to arrays of
									// vectors, of size nd. Any of the
									// pointers may be null, in which
									// case the corresponding derivatives
									// will not be returned. Otherwise
									// they must point to arrays long
									// enough for all the derivatives of
									// that order - i.e. 2 for the first
									// derivatives, 3 for the second, etc.
                int = 0,       		// Number of derivatives required (nd) 
				evaluate_surface_quadrant = evaluate_surface_unknown
									// the evaluation location - above,
									// below for each parameter direction,
									// or don't care.
            ) const;

	// Return the number of derivatives which evaluate() can find
	// "accurately" (and fairly directly), rather than by finite
	// differencing, over the given portion of the surface. If there
	// is no limit to the number of accurate derivatives, returns
	// the value ALL_SURFACE_DERIVATIVES, which is large enough to be
	// more than anyone could reasonably want.

	virtual int accurate_derivs( SPApar_box const & = *(SPApar_box *)NULL_REF ) const;

	// Report whether a parametric surface is periodic in either
	// parameter direction (i.e. it is smoothly closed, so faces can
	// run over the seam).

	virtual logical periodic_u() const;
	virtual logical periodic_v() const;

	// Report whether the surface is closed, smoothly or not, in
	// either parameter direction.

	virtual logical closed_u() const;
	virtual logical closed_v() const;

	// Return the period of a periodic parametric surface, zero if
	// the surface is not periodic in the chosen parameter or not
	// parametric.

	virtual double param_period_u() const;
	virtual double param_period_v() const;

	// Return the principal parameter range of a parametric surface in
	// a chosen parameter direction, or in both.

	// If a SPAbox is provided, the parameter range returned may be
	// restricted to a portion of the surface which is guaranteed to
	// contain all portions of the surface which lie within the region
	// of interest.

	virtual SPApar_box param_range( SPAbox const & = *(SPAbox *)NULL_REF ) const;
	virtual SPAinterval param_range_u( SPAbox const & = *(SPAbox *)NULL_REF ) const;
	virtual SPAinterval param_range_v( SPAbox const & = *(SPAbox *)NULL_REF ) const;

	// Indicate whether the parameter coordinate system of the surface
	// is right- or left-handed.

	virtual logical left_handed_uv() const;

	// Construct a parameter line on the surface. A u parameter line
	// runs in the direction of increasing u parameter, at constant v.

	virtual curve *u_param_line(
				double,			// constant v parameter
				meshsurf const &// owning surface
			) const;
	virtual curve *v_param_line(
				double,			// constant u parameter
				meshsurf const &// owning surface
			) const;

	// Test whether a point lies on the surface, within a user-defined
	// tolerance.

	virtual logical test_point_tol(
				SPAposition const &,
				double,
				SPApar_pos const & = *(SPApar_pos *)NULL_REF,
				SPApar_pos & = *(SPApar_pos *)NULL_REF
			) const;

	// Save and restore.

	virtual char const *type_name() const = 0;

	virtual void save(ENTITY_LIST &) const = 0;
	virtual void save_data(ENTITY_LIST &) const;

//	friend subtype_object *restore_msh_sur();
	void restore_data();
    /**
	 * Debug printout.  Define debug_data to do much of the
	 * generic work.
    */
	virtual void debug( char const *, logical, FILE * ) const = 0;
    /**
	 * Debug printout.  Define debug_data to do much of the
	 * generic work.
    */
	void debug_data( FILE * ) const;

	// STI ROLL
	virtual void full_size(SizeAccumulator&, logical = TRUE) const;
	// STI ROLL

	// Make the meshsurf a friend so that it can access any
	// private data or functions.

	friend class meshsurf;
	friend class pointer_map;
};

// Declare data for restore dispatch table

#undef THIS
#define THIS() msh_sur
DISPATCH_DECL(KERN)
#undef THIS

#endif
