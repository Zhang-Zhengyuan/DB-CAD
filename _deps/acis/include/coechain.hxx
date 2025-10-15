/*******************************************************************/
/*    Copyright (c) 1989-2020 by Spatial Corp.                     */
/*    All rights reserved.                                         */
/*    Protected by U.S. Patents 5,257,205; 5,351,196; 6,369,815;   */
/*                              5,982,378; 6,462,738; 6,941,251    */
/*    Protected by European Patents 0503642; 69220263.3            */
/*    Protected by Hong Kong Patent 1008101A                       */
/*******************************************************************/

#if !defined( COEDGE_CHAIN_CLASS )
#define COEDGE_CHAIN_CLASS

#include "dcl_heal.h"
#include "position.hxx"
#include "param.hxx"
#include "container.hxx"


class ENTITY_LIST;
class COEDGE;
class PCURVE;
class VERTEX;
class FACE;
class curve;
class SURFACE;
class SPAposition;
class SPApar_pos;
class VOID_LIST;


class DECL_HEAL COEDGE_PART : public ACIS_OBJECT {

	// An optional pointer to coedge

	COEDGE* m_coedge;
	
	// Pointer to geometry in 2D

	curve* m_geometry_ptr;

	// Pointer to old edge geometry and sense

	curve* m_geometry3d_ptr;
	REVBIT m_edge_sense;

	// Pointer to the surface linked to the chain

	FACE* m_face_ptr;

	// Hold a logical to denote whether it is pseudo or not
	// Pseudo coedges wont have coedge pointers

	logical m_pseudo;

	// Positions denoting start and end in 2d and in 3d

	SPApar_pos m_start_2d;
	SPApar_pos m_end_2d;

   	public:

	// Default constructor

	COEDGE_PART();

	// Destructor

	~COEDGE_PART();

	// Data reading routines.

	COEDGE* coedge() const { return m_coedge; }
	curve* geometry() const { return m_geometry_ptr; }
	curve* geometry3d() const { return m_geometry3d_ptr; }
	REVBIT edge_sense() const { return m_edge_sense; }
	logical pseudo() const { return m_pseudo; }
	SPApar_pos start_2d() const;
	SPApar_pos end_2d() const;
	logical iso() const;

	// Data changing routines.  Each of these routines checks 
	// that the record has been posted on the bulletin-board before 
	// performing the change.  If not, the routine provokes an error, 
	// so that the omission (forgetting to call backup() first) can 
	// be rectified in the program.  In production versions of the 
	// program, these checks may be disabled, to improve efficiency.

	void set_geometry( curve * );
	void set_geometry3d( curve * );
	void set_face(FACE* );
	logical set_coedge( COEDGE* );
	void set_pseudo(logical);
	void set_start_2d(SPApar_pos);
	void set_end_2d(SPApar_pos);
	void set_edge_sense( REVBIT );

	// Member function to copy coedge chain

	void operator=(const COEDGE_PART&);

	// Function to compute the geometry

// STI cpp  5/22/00 begin (change return from void to logical)
	logical compute_geometry();

	// Function to interpolate 3d for pseudo

	void interpolate_geometry();

	// Function to project a point to the coedge chain. It would either
	// go to the 3d-curve or the surface is it is not pseudo.

	logical project_point(const SPAposition& , const SPApar_pos& , SPAposition&, logical&) const;

	// Functions to return start and end points in 3-space

	SPAposition start() const;
	SPAposition end() const;

};

class DECL_HEAL COEDGE_CHAIN : public ACIS_OBJECT {

	// Pointer to an array of coedge-parts

	COEDGE_PART* coedge_list;

	// Also a pointer to the number of coedge parts

	int m_coedges;

	// Pointer to geometry of the compound coedge

	curve* geometry_ptr;

	// Pointer to 3d geometry

	curve* geometry3d_ptr;
	
	// Pointer to the face geometry

	FACE* m_face_ptr;

	// Hold a logical to denote degenerate SPAposition

	logical m_degen;
	SPAposition m_degen_pos;

	// Hold two logicals for chop junctions. The first one denotes
	// whether any of the coedges in chain is fixed. The second
	// one denotes whether this chain should be used in patch

	logical m_fixed;
	logical m_used;

	// Holds a bit to say if the 3d curve is going in the opposite 
	// direction of the 2d

	logical m_sense;

   	public:

	// Default constructor

	COEDGE_CHAIN();

	// Destructor

	~COEDGE_CHAIN();

	// Data reading routines.

	int num_coedges() const;
	COEDGE* coedge(int j) const;
	curve* geometry() const { return geometry_ptr; }
	curve* geometry3d() const { return geometry3d_ptr; }
	logical degenerate() const { return m_degen; }
	SPAposition degen_pos() const { return m_degen_pos; } 
	logical fixed() const { return m_fixed; }
	logical used() const { return m_used; }
	COEDGE_PART* list() const { return coedge_list; }	
	logical sense() const { return m_sense; }
	logical is_present(COEDGE*) const;
	curve* geometry_on_surface() const;
	FACE* get_face() const { return m_face_ptr; }
	logical iso() const;

	// Data changing routines.  Each of these routines checks 
	// that the record has been posted on the bulletin-board before 
	// performing the change.  If not, the routine provokes an error, 
	// so that the omission (forgetting to call backup() first) can 
	// be rectified in the program.  In production versions of the 
	// program, these checks may be disabled, to improve efficiency.

	void set_geometry( curve * );
	void set_geometry3d( curve * );
	void set_face( FACE * );
	void set_list(COEDGE_PART*);
	void set_degenerate(logical);
	void set_degen_pos(const SPAposition&);
	void set_fixed(logical);
	void set_used(logical);
	void set_sense(logical);
	void set_blend(logical);
	void set_num_coedges(int);

	// Member function to copy coedge chain

	void operator=(const COEDGE_CHAIN&);
	
	// Member function to compute the new pcurves from the underlying ones

	logical compute_geometry();

	// Member function to add a coedge into the coedge chain

	logical add ( COEDGE* );

	// Member function to add a pseudo coedge in ith portion of chain

	void add_pseudo( int, SPApar_pos , SPApar_pos , curve* , FACE* );

	// Get the nearest coedge to a point in the coedge chain

	void get_vertices(VOID_LIST &);

	// Get the nearest coedge to a point in the coedge chain

	logical get_nearest_coedge(const SPApar_pos& , int& ) const;

	// Function to project a point to the coedge chain. It would either
	// go to the 3d-curve or the surface whichever appropriate

	logical project_point(const SPAposition& , const SPApar_pos&, SPAposition&) const;

	// Function to project a set of points to a coedge chain

	logical project_points( SPAposition* , SPApar_pos* , int ) const;

	// Function to project a 3d-curve to a chain

	logical project_curve(curve*&);

	// "Pseudo-member" functions, returning vertex pointers from the 
	// associated edge, if any.  These take into account the sense of 
	// the coedge.

	SPAposition start() const;
	SPAposition end() const;

	SPApar_pos start_2d() const;
	SPApar_pos end_2d() const;

	// MA (16 Aug 1999): Added member function to inquire the tolerance
	// for the coedge chain (max of all edge tols)
	double tol() const;
};

#endif
