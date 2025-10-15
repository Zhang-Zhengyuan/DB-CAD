/*******************************************************************/
/*    Copyright (c) 1989-2020 by Spatial Corp.                     */
/*    All rights reserved.                                         */
/*    Protected by U.S. Patents 5,257,205; 5,351,196; 6,369,815;   */
/*                              5,982,378; 6,462,738; 6,941,251    */
/*    Protected by European Patents 0503642; 69220263.3            */
/*    Protected by Hong Kong Patent 1008101A                       */
/*******************************************************************/
//------------------------------------------------------------------------------
#ifndef HH_CGraph__HXX
#define HH_CGraph__HXX
//--------------------------------------------------------------------
class HH_Node;
class HH_Arc;

//--------------------------------------------------------------------

#include	<stdio.h>
#include 	"acis.hxx"
#include	"vlists.hxx"
#include 	"dcl_heal.h"
#include	"entity.hxx"
#include	"eelists.hxx"
#include	"att_hh.hxx"


//--------------------------------------------------------------------
class HH_Node;
class HH_Arc;

//--------------------------------------------------------------------
/**
 * @nodoc
 */
ENTITY_IS_PROTOTYPE(HH_CGraph, HEAL)
#if 0
; // semicolon needed for mkman (doc tool) parsing
#endif
#define HH_CGraph_LEVEL (ATTRIB_HH_LEVEL + 1)
extern DECL_HEAL int HH_CGraph_TYPE;

class DECL_HEAL HH_CGraph : public ATTRIB_HH
{
private:
	EE_LIST * node_list;	// nodes in the graph
	EE_LIST * arc_list;

	// Intermediate level functions for making DAG ( Directed Acyclic
	// Graph ) for one level only
	void make_DAG_one_level( VOID_LIST &, VOID_LIST & );

public:

	// Graph
	HH_CGraph();

	// @Query
	const EE_LIST *  nodes() const { return node_list; }
	const EE_LIST *  arcs() const { return arc_list; }

	// Modify lists
	EE_LIST *  nodes_for_update() { return node_list; }
	EE_LIST *  arcs_for_update() { return arc_list; }



	// Make a DAG from a undirected graph. Starts making DAG
	// from current root point.
	void make_DAG();

	// Remove directions from all the arcs. Make an
	// undirected graph.
	void unDAG();

	// Add node to graph.
	void add_node( HH_Node* );

	// Add arc to graph.
	void add_arc( HH_Arc * );

	int solver_stage();

	// debug current graph.
	virtual void debug( FILE * );
	virtual void cleanup();

	virtual void restore_arcs_nodes();
	virtual void copy_from(ENTITY_LIST& ,const HH_CGraph*);
	virtual void add_to_list( ENTITY_LIST & ) const;
	virtual void fix_from_array( ENTITY * array [] );

	//MG (3May99)print root nodes. The member variable root_node does not contain the
	//actual root.Can be made to do so if HH_CGraphis derived from ENTITY
	void print_roots(FILE *);

	/**
	 * @nodoc
	 */
	ATTRIB_FUNCTIONS( HH_CGraph, HEAL)
#if 0
; // semicolon needed for mkman (doc tool) parsing)
#endif
// Because mkman isn't aware that ATTRIB_FUNCTIONS changes access to "public", we do so
// explicitly here:
public:

	//USE_COUNTED_DECL

};

//--------------------------------------------------------------------


#endif
