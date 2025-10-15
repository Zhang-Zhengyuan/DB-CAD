/*******************************************************************/
/*    Copyright (c) 1989-2020 by Spatial Corp.                     */
/*    All rights reserved.                                         */
/*    Protected by U.S. Patents 5,257,205; 5,351,196; 6,369,815;   */
/*                              5,982,378; 6,462,738; 6,941,251    */
/*    Protected by European Patents 0503642; 69220263.3            */
/*    Protected by Hong Kong Patent 1008101A                       */
/*******************************************************************/

//------------------------------------------------------------------------------

#ifndef UNSTABLE_VERTEX_SOLVER__HXX
#define UNSTABLE_VERTEX_SOLVER__HXX
//------------------------------------------------------------------------------
#include "dcl_heal.h"
#include "lists.hxx"
#include "tganasnp.hxx"

class BODY;
class SURFACE;

//------------------------------------------------------------------------------
//MG If at any stage Unstable_Vertex_Solveris to be derived take 
//care of copy defs etc..

class DECL_HEAL Unstable_Vertex_Solver 
{
protected:
	
	ENTITY_LIST unstable_island_list; // list of virtual surfaces
	BODY * body;

	void set_tolerances();
public:

	Unstable_Vertex_Solver( BODY * bdy );
	~Unstable_Vertex_Solver();

	logical fix_unstable_vertices();
	logical snap();
	
	void copy_from(ENTITY_LIST &,const Unstable_Vertex_Solver& );
	void restore_body() { body = (BODY*)-1;}
	void add_to_list(ENTITY_LIST&) const;
	virtual void	fix_from_array( ENTITY * array [] );
	void clear();
};

class DECL_HEAL HH_Unstable_Vertex_Solver
{
private:
	BODY * body;
	HH_UV_Analytic_Snapper uv_snapper;
public:
	HH_Unstable_Vertex_Solver( BODY * bdy );
	~HH_Unstable_Vertex_Solver();

	logical solve();
	void analyze();
	void calculate();
	void set_tolerances();
	void cleanup();
	void copy_from(ENTITY_LIST &,const HH_Unstable_Vertex_Solver& );
	void add_to_list(ENTITY_LIST&) const;
	void restore_body();
	virtual void fix_from_array( ENTITY * array [] );
};

//------------------------------------------------------------------------------
#endif
