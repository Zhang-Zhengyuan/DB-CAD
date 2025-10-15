/*******************************************************************/
/*    Copyright (c) 1989-2020 by Spatial Corp.                     */
/*    All rights reserved.                                         */
/*    Protected by U.S. Patents 5,257,205; 5,351,196; 6,369,815;   */
/*                              5,982,378; 6,462,738; 6,941,251    */
/*    Protected by European Patents 0503642; 69220263.3            */
/*    Protected by Hong Kong Patent 1008101A                       */
/*******************************************************************/
//------------------------------------------------------------------------------------
#ifndef HH_BASENODE_SOLVER_HXX
#define HH_BASENODE_SOLVER_HXX

//------------------------------------------------------------------------------------
//------------------------------------------------------------------------------------

#include	"entity.hxx"
#include	"usecount.hxx"
#include	"vlists.hxx"
#include	"eelists.hxx"

#include	"att_hh.hxx"
#include	"dcl_heal.h"
#include 	"glb_node.hxx"

class HH_GlobalNode;
//------------------------------------------------------------------------------------
/**
 * @nodoc
 */
ENTITY_IS_PROTOTYPE(HH_BaseNodeSolver, HEAL)
#if 0
; // semicolon needed for mkman (doc tool) parsing
#endif
#define HH_BaseNodeSolver_LEVEL (ATTRIB_HH_LEVEL+1)
extern DECL_HEAL int HH_BaseNodeSolver_TYPE;

//------------------------------------------------------------------------------------
//struct to see if solver has sucd. in solving
enum SolverSuc
{
	NOT_ATTEMPTED,
	SUCCESS,
	FAILURE
};
//------------------------------------------------------------------------------------
class DECL_HEAL HH_BaseNodeSolver : public ATTRIB_HH
{
protected:
	HH_GlobalNode * node;
	SolverSuc suc;
public:
	HH_BaseNodeSolver(HH_GlobalNode *  = NULL);
	HH_GlobalNode* get_node();
	void set_node(HH_GlobalNode* n);
	void set_solver_suc(SolverSuc);
	SolverSuc get_solver_suc();

	virtual logical solve();
	virtual void cleanup();

	virtual logical check_status_with_node(HH_GlobalNode*);

	/**
	 * @nodoc
	 */
	ATTRIB_FUNCTIONS( HH_BaseNodeSolver, HEAL)
#if 0
; // semicolon needed for mkman (doc tool) parsing)
#endif
// Because mkman isn't aware that ATTRIB_FUNCTIONS changes access to "public", we do so
// explicitly here:
public:

	//USE_COUNTED_DECL
};




//------------------------------------------------------------------------------------
#endif
