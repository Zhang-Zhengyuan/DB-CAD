/*******************************************************************/
/*    Copyright (c) 1989-2020 by Spatial Corp.                     */
/*    All rights reserved.                                         */
/*    Protected by U.S. Patents 5,257,205; 5,351,196; 6,369,815;   */
/*                              5,982,378; 6,462,738; 6,941,251    */
/*    Protected by European Patents 0503642; 69220263.3            */
/*    Protected by Hong Kong Patent 1008101A                       */
/*******************************************************************/
//--------------------------------------------------------------------
#ifndef HH_GLOBALGRAPH_HXX
#define HH_GLOBALGRAPH_HXX
//--------------------------------------------------------------------
#include 	"acis.hxx"
#include	"base.hxx"
#include	"entity.hxx"
#include	"att_hh.hxx"
#include	"dcl_heal.h"
#include	"hh_graph.hxx"
#include	"glb_node.hxx"
#include	"glb_arc.hxx"
#include	"bs_util.hxx"
class BODY;
class HH_CGraph;
//--------------------------------------------------------------------
/**
 * @nodoc
 */
#define HH_GlobalGraph_LEVEL (HH_CGraph_LEVEL + 1)
/**
 * @nodoc
 */
ENTITY_IS_PROTOTYPE(HH_GlobalGraph, HEAL)
#if 0
; // semicolon needed for mkman (doc tool) parsing
#endif

//--------------------------------------------------------------------
class DECL_HEAL HH_GlobalGraph : public HH_CGraph
{
protected:
	BODY* body;
public:
	HH_GlobalGraph(BODY* = NULL);

	const BODY * get_body();
	BODY * get_body_for_change();

	//Collect all faces and attach attribute and dummy face nodes on them
	void initialize();
	//Call different factories
	void analyze();
	void make_DAG();
	//Make an ordered stack and solve
	void form_stack_and_solve();

	virtual void cleanup();

	virtual void restore_arcs_nodes();
	virtual void copy_from(ENTITY_LIST& ,const HH_GlobalGraph*);
	virtual void add_to_list( ENTITY_LIST & ) const;
	virtual void fix_from_array( ENTITY * array [] );

	// Entity functions.
	/**
	 * @nodoc
	 */
	ATTRIB_FUNCTIONS( HH_GlobalGraph, HEAL)
#if 0
; // semicolon needed for mkman (doc tool) parsing)
#endif
// Because mkman isn't aware that ATTRIB_FUNCTIONS changes access to "public", we do so
// explicitly here:
public:

};

//--------------------------------------------------------------------
//--------------------------------------------------------------------
DECL_HEAL void find_root_nodes(VOID_LIST & , VOID_LIST & );
DECL_HEAL logical correct_node_solver(Node_Wrapper *);
DECL_HEAL void update_neighbour_list(Node_Wrapper *,VOID_LIST&,const VOID_LIST&);
DECL_HEAL void get_incoming_neigh_nodes(HH_GlobalNode* , VOID_LIST & );
DECL_HEAL int cmp_nodes(const void * , const void * );
DECL_HEAL void sort_list_for_snapping(VOID_LIST & );

//--------------------------------------------------------------------
//--------------------------------------------------------------------

#endif
