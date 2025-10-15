/*******************************************************************/
/*    Copyright (c) 1989-2020 by Spatial Corp.                     */
/*    All rights reserved.                                         */
/*    Protected by U.S. Patents 5,257,205; 5,351,196; 6,369,815;   */
/*                              5,982,378; 6,462,738; 6,941,251    */
/*    Protected by European Patents 0503642; 69220263.3            */
/*    Protected by Hong Kong Patent 1008101A                       */
/*******************************************************************/
//------------------------------------------------------------------------------------
#ifndef HH_GLOBALFACELISTNODE_HXX
#define HH_GLOBALFACELISTNODE_HXX
//------------------------------------------------------------------------------------
//------------------------------------------------------------------------------------
#include 	"acis.hxx"
#include	"att_hh.hxx"
#include 	"dcl_heal.h"
#include 	"hh_node.hxx"
#include 	"glb_node.hxx"

class HH_BaseNodeSolver;
class ENTITY_LIST;
class SURFACE;
//------------------------------------------------------------------------------------
/**
 * @nodoc
 */
ENTITY_IS_PROTOTYPE( HH_GlobalFaceListNode,HEAL)
#if 0
; // semicolon needed for mkman (doc tool) parsing
#endif
#define HH_GlobalFaceListNode_LEVEL (HH_GlobalNode_LEVEL + 1)
extern int DECL_HEAL HH_GlobalFaceListNode_TYPE;

//------------------------------------------------------------------------------------

class DECL_HEAL HH_GlobalFaceListNode : public HH_GlobalNode
{
protected:
	EE_LIST * face_list;
	double tol;
public:
	HH_GlobalFaceListNode(HH_BaseNodeSolver * = NULL);
	const EE_LIST * get_node_facelist();
	EE_LIST * get_node_facelist_for_change();


	virtual ENTITY * get_node_base();
	virtual double get_tol();
	virtual void		debug( FILE * ) const;
	virtual void cleanup();
	
	/**
	 * @nodoc
	 */
	ENTITY_FUNCTIONS( HH_GlobalFaceListNode,HEAL)
#if 0
; // semicolon needed for mkman (doc tool) parsing)
#endif
// Because mkman isn't aware that ENTITY_FUNCTIONS changes access to "public", do so
// explicitly here:
public:

};

//------------------------------------------------------------------------------------
#endif
