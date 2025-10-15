/*******************************************************************/
/*    Copyright (c) 1989-2020 by Spatial Corp.                     */
/*    All rights reserved.                                         */
/*    Protected by U.S. Patents 5,257,205; 5,351,196; 6,369,815;   */
/*                              5,982,378; 6,462,738; 6,941,251    */
/*    Protected by European Patents 0503642; 69220263.3            */
/*    Protected by Hong Kong Patent 1008101A                       */
/*******************************************************************/
#ifndef HH_GLOBALNODE_HXX
#define HH_GLOBALNODE_HXX

//------------------------------------------------------------------------------------

#include 	"acis.hxx"
#include	"att_hh.hxx"
#include 	"dcl_heal.h"
#include 	"hh_node.hxx"
#include	"hh_bsslv.hxx"

class HH_BaseNodeSolver;
//------------------------------------------------------------------------------------

/**
 * @nodoc
 */
ENTITY_IS_PROTOTYPE(HH_GlobalNode,HEAL)
#if 0
; // semicolon needed for mkman (doc tool) parsing
#endif
#define HH_GlobalNode_LEVEL (HH_Node_LEVEL + 1)
extern int DECL_HEAL HH_GlobalNode_TYPE;

//------------------------------------------------------------------------------------

class DECL_HEAL HH_GlobalNode : public HH_Node
{
protected:
	HH_BaseNodeSolver * solver;
public:
	HH_GlobalNode(HH_BaseNodeSolver * = NULL);
	HH_BaseNodeSolver * get_node_solver();
	void set_node_solver(HH_BaseNodeSolver *);
	void suck_all_arcs();
	void suck_specific_arc(HH_GlobalNode*);

	virtual double get_tol(){return -1;}
	virtual ENTITY * get_node_base(){return NULL;}
	virtual void		debug( FILE * ) const;
	virtual void cleanup();

	/**
	 * @nodoc
	 */
	ENTITY_FUNCTIONS( HH_GlobalNode,HEAL)
#if 0
; // semicolon needed for mkman (doc tool) parsing)
#endif
// Because mkman isn't aware that ENTITY_FUNCTIONS changes access to "public", do so
// explicitly here:
public:

};


//------------------------------------------------------------------------------------
#endif //HH_GlobalNode_HXX
