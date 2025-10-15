/*******************************************************************/
/*    Copyright (c) 1989-2020 by Spatial Corp.                     */
/*    All rights reserved.                                         */
/*    Protected by U.S. Patents 5,257,205; 5,351,196; 6,369,815;   */
/*                              5,982,378; 6,462,738; 6,941,251    */
/*    Protected by European Patents 0503642; 69220263.3            */
/*    Protected by Hong Kong Patent 1008101A                       */
/*******************************************************************/

//------------------------------------------------------------------------------


#ifndef HH_Tangent_Analytic_Snapper__HXX
#define HH_Tangent_Analytic_Snapper__HXX

//--------------------------------------------------------------------

#include	"graph.hxx"
#include	<stdio.h>
#include	"vlists.hxx"
#include	"lists.hxx"
#include    "allsfdef.hxx"
#include	"eelists.hxx"
//--------------------------------------------------------------------
class BODY;
class SURFACE;
class HH_GNode;
//--------------------------------------------------------------------

//UTility functions
void copy( const VOID_LIST & , EE_LIST *  );
// Cluster of tangent surfaces.
class HH_Tangent_Cluster : public HH_Graph
{
	VOID_LIST snapper_stack;

	void clear_stack();

public:
	HH_Tangent_Cluster( HH_GNode * root = NULL )
		: HH_Graph( root ){}

	 virtual ~HH_Tangent_Cluster(){clear_stack();}

	// Create solve stack from the DAG.
	void create_solve_stack();

	// solve graph from the stack.
	void solve();

	// stack
	const VOID_LIST & solve_stack() const { return snapper_stack; }

	// debug
	void debug( FILE * );
};

//--------------------------------------------------------------------
//--------------------------------------------------------------------
//--------------------------------------------------------------------
// A structure to assist in sorting the array.

class Node_Pair : public ACIS_OBJECT
{
	HH_GNode  * pNode;
	int			unmarked_degree;

public:
	Node_Pair()
		: pNode(0), unmarked_degree(0){}

	Node_Pair( HH_GNode * node )
		: pNode( node ), unmarked_degree(0)
	{
		update();
	}

	// Update the degree
	//MG 20 July 99 new function for marking arc
	void update();

	// set node
	void set_node( HH_GNode * n )
	{ pNode = n; }

	// return number of unmarked arcs in a node.
	int udeg() const { return unmarked_degree; }

	//return degree for sorting
	int sort_deg();

	// mark direction in the arc
	void mark_arcs();
	void force_arcs();
	void force_specific_arc(HH_GNode * );

	// Current pair
	HH_GNode * node() { return pNode; }
};
//--------------------------------------------------------------------
//--------------------------------------------------------------------

//MG If at any stage HH_Tangent_Analytic_Snapper is to be derived take
//care of copy defs etc..


class DECL_HEAL HH_Tangent_Analytic_Snapper
{
protected:
	BODY * body;
// Collections of arcs and nodes in the graph.
	EE_LIST * arc_list;
	EE_LIST * node_list;


	int	  deg;

	ENTITY_LIST edgelist;
	// Clusters - stores pointers to graphs.
	VOID_LIST  cluster_list;
	// HH_VECTOR(HH_GraphP) clusters;

	// Set default tolerances used by analytic solver.
	void set_default_tolerances();

	// Debug functions. Called internally for debug purpose.
	// Gives step by step account of the solver.
	void debug_solver();

	void cluster_from_node( HH_GNode  *, HH_Graph  &,
							VOID_LIST &, VOID_LIST & );

	void cluster_from_node( HH_Graph  &,
							VOID_LIST &, VOID_LIST & );

	virtual void mark_initial_arcs(Node_Pair *,unsigned long  );

	virtual Node_Pair * find_root_make_DAG( Node_Pair * , unsigned long , int&  );

	virtual void find_degree_root_and_make_DAG( HH_Graph & graph );


public:

	// Required for saving, copying and fix pointers

	virtual void	copy_from( ENTITY_LIST & , const HH_Tangent_Analytic_Snapper & );
	virtual void	add_to_list( ENTITY_LIST & ) const;
	virtual void	fix_from_array( ENTITY * array [] );

	void fixup_copy_nodes_and_arcs( HH_Tangent_Analytic_Snapper & snapper ) const;
	void fixup_copy_clusters( HH_Tangent_Analytic_Snapper & snapper ) const;

public:

	HH_Tangent_Analytic_Snapper( BODY * );

	HH_Tangent_Analytic_Snapper( const HH_Tangent_Analytic_Snapper &);

	// ..
	~HH_Tangent_Analytic_Snapper();

	const HH_Tangent_Analytic_Snapper &
		operator = ( const HH_Tangent_Analytic_Snapper & s);

	// Initialize snapper.
	void init();

	void initialize();

	// Analyze snapper. Just create a graph of tangencies.
	virtual void analyze();

	// Solve the graph.
	virtual void calculate();


	// Debug graph.
	virtual void debug_graph( FILE * );


	// form clusters
	virtual void form_clusters();

	// make DAGS for individual graphs
	virtual void find_roots_and_make_DAGS();

	// clear the nodes and hence the graph.
	virtual void clear();

	// clear the nodes and hence the graph.
	virtual void clean_snapper();

	// check / correct snapping, a simple tree propagation
	logical check_snapping();

	virtual void restore_arcs_nodes();

	// get the maximum degree of the snapper
	virtual int degree() const { return deg; }

	void convert_ee_list_to_void_list( const EE_LIST* from, VOID_LIST & to);

	void convert_void_list_to_ee_list( const VOID_LIST & from, EE_LIST * to) ;

	const VOID_LIST & clusters() ;

	const EE_LIST * arcs() ;

	const EE_LIST * nodes() ;

    EE_LIST * arcs_for_change() ;

	EE_LIST * nodes_for_change() ;

};

//--------------------------------------------------------------------
//--------------------------------------------------------------------

class DECL_HEAL HH_UV_Analytic_Snapper : public HH_Tangent_Analytic_Snapper
{
public:
	HH_UV_Analytic_Snapper( BODY * );

	void group_clusters();

	virtual void analyze();
	virtual void mark_initial_arcs(Node_Pair *,unsigned long  );
	virtual Node_Pair * find_root_make_DAG( Node_Pair * , unsigned long , int&  );

};
//--------------------------------------------------------------------


//--------------------------------------------------------------------
#endif
