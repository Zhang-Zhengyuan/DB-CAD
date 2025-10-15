/*******************************************************************/
/*    Copyright (c) 1989-2020 by Spatial Corp.                     */
/*    All rights reserved.                                         */
/*    Protected by U.S. Patents 5,257,205; 5,351,196; 6,369,815;   */
/*                              5,982,378; 6,462,738; 6,941,251    */
/*    Protected by European Patents 0503642; 69220263.3            */
/*    Protected by Hong Kong Patent 1008101A                       */
/*******************************************************************/

//------------------------------------------------------------------------------
#ifndef HH_Graph__HXX
#define HH_Graph__HXX
//--------------------------------------------------------------------
class HH_GNode;
class HH_GArc;

//--------------------------------------------------------------------

#include	<stdio.h>
#include 	"acis.hxx"
#include	"vlists.hxx"
#include 	"dcl_heal.h"

//--------------------------------------------------------------------
class HH_GNode;
class HH_GArc;

//--------------------------------------------------------------------

class DECL_HEAL HH_Graph  : public ACIS_OBJECT
{
private:
	HH_GNode* 	root_node;	// Root of the graph
	VOID_LIST node_list;	// nodes in the graph
	VOID_LIST arc_list;
	
	// Intermediate level functions for making DAG ( Directed Acyclic
	// Graph ) for one level only
	void make_DAG_one_level( VOID_LIST &, VOID_LIST & );

public:

	// Graph 
	HH_Graph( HH_GNode* r )
		: root_node(r){}

	virtual ~HH_Graph(){}

	// @Query 
	const VOID_LIST & nodes() const { return node_list; }
	const VOID_LIST & arcs() const { return arc_list; }

	// Modify lists
	VOID_LIST & nodes_for_update() { return node_list; }
	VOID_LIST & arcs_for_update() { return arc_list; }

	// @Operations
	void delete_graph();

	//Return root node
	HH_GNode* get_root_node() {return root_node;}

	// Make a DAG from a undirected graph. Starts making DAG
	// from current root point.
	void make_DAG(); 

	// Remove directions from all the arcs. Make an
	// undirected graph.
	void unDAG();

	// Add node to graph.
	void add_node( HH_GNode* );

	// Add arc to graph.
	void add_arc( HH_GArc * );

	int solver_stage();

	// debug current graph.
	virtual void debug( FILE * );

	//MG (2Dec98)printing node in graph
	 virtual void print_node_info(HH_GNode * node, FILE * fp);

	 virtual void print_uv_node_info(HH_GNode * node, FILE * fp);

	// return root node only to be used in analysis stage.
	const HH_GNode * root() const { return root_node; }

	// set root
	void set_root( HH_GNode *  );

	//MG (3May99)print root nodes. The member variable root_node does not contain the 
	//actual root.Can be made to do so if HH_Graphis derived from ENTITY
	void print_roots(FILE *);
	
};

//--------------------------------------------------------------------


#endif
