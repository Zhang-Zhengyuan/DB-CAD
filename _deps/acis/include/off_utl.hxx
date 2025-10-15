/* ORIGINAL: acis2.1/sg_husk/cur_off/off_utl.hxx */
/* $Id: off_utl.hxx,v 1.7 2001/01/11 18:13:55 ywoo Exp $ */
/*******************************************************************/
/*    Copyright (c) 1989-2020 by Spatial Corp.                     */
/*    All rights reserved.                                         */
/*    Protected by U.S. Patents 5,257,205; 5,351,196; 6,369,815;   */
/*                              5,982,378; 6,462,738; 6,941,251    */
/*    Protected by European Patents 0503642; 69220263.3            */
/*    Protected by Hong Kong Patent 1008101A                       */
/*******************************************************************/


#if !defined( TRIM_OFF_UTIL_HXX )
#define TRIM_OFF_UTIL_HXX

#include "dcl_ofst.h"
#include "dcl_base.h"
#include "logical.h"
// ywoo 08Jan01: added the header file.
#include "mmgr.hxx"
// ywoo: end

// Forward Declarations

class curve;
class ENTITY;

//STI HDR_INDEP: nvp
class curve_curve_int;
class intcurve;
extern DECL_BASE FILE *debug_file_ptr;

// Following are some classes for grouping intersection points together.
// Currently, these are used by the routines that do refinement on the
// curve self-intersections.

// container class for linking together intersection records without
// destroying the curve_curve_int links.

class DECL_OFST sg_cci_point : public ACIS_OBJECT {
public:
	sg_cci_point* next;
	curve_curve_int* cci;

	// Constructors
	sg_cci_point();
	sg_cci_point( curve_curve_int * );

	// Change routines
	void insert( sg_cci_point* previous_point );
};

// class representing a node of common intersection points

class DECL_OFST sg_cci_node : public ACIS_OBJECT {
	
public:
	// Constructors
	sg_cci_node();
	sg_cci_node( curve_curve_int * );

	// Routines for traversing nodes
	inline sg_cci_node* next_node();

	// Routines for traversing the intersection points
	inline void init_cci();
	curve_curve_int* next_cci();
	inline curve_curve_int* first_cci();

	// Query routines
	inline int cci_count();
	
	// Change routines
	void append_cci( curve_curve_int * );
	void insert_node( sg_cci_node* previous_node );
	
	// Destructor
	~sg_cci_node();
	
	void debug( FILE* dfp = debug_file_ptr );

private:
	int cci_point_count;
	sg_cci_point head_point;
	sg_cci_point* current_point;
	sg_cci_point* last_point;
	sg_cci_node* next_cci_node;
};

// main class for tying the nodes together

class DECL_OFST sg_cci_group : public ACIS_OBJECT {
	
public:
	// Constructors
	sg_cci_group();
	
	// Routines for traversing the group of nodes
	inline void init();
	sg_cci_node* next();

	// Change routines
	void append_node( sg_cci_node * );

	// Destructor
	~sg_cci_group();
	
	void debug( FILE* dfp = debug_file_ptr );
	
private:
	sg_cci_node head_node;
	sg_cci_node* current_node;
	sg_cci_node* last_node;
};


// Routine for reordering a self-intersection list to look
// like a curve-curve intersection list.

DECL_OFST void
sg_reorder_self_cci(
	curve_curve_int *& );


// Routine for refining approximate curve intersections
// to the true curves.

DECL_OFST void
sg_refine_intersections(
	curve_curve_int *&,
	const intcurve& true_cur1,
	const intcurve& true_cur2,
	logical negate1,
	logical negate2,
	logical remove_failures = FALSE );

// Function to ensure that wire's coedges cycle through
// next and previous to step through wire.

DECL_OFST void order_wire_coedges(ENTITY *wire_body);

#endif
