/*******************************************************************/
/*    Copyright (c) 1989-2020 by Spatial Corp.                     */
/*    All rights reserved.                                         */
/*    Protected by U.S. Patents 5,257,205; 5,351,196; 6,369,815;   */
/*                              5,982,378; 6,462,738; 6,941,251    */
/*    Protected by European Patents 0503642; 69220263.3            */
/*    Protected by Hong Kong Patent 1008101A                       */
/*******************************************************************/
//--------------------------------------------------------------------
//--------------------------------------------------------------------

#ifndef HH_NODE__HXX
#define HH_NODE__HXX
//--------------------------------------------------------------------

#include	<stdio.h>
#include 	"acis.hxx"
#include	"entity.hxx"
#include	"usecount.hxx"
#include	"vlists.hxx"
#include	"eelists.hxx"
#include	"att_hh.hxx"
#include	"dcl_heal.h"

//--------------------------------------------------------------------
// HH_Node and HH_Arc together represents a undirected general
// graph of degree n. We can convert this graph to DAG ( Directed
// Acyclic Graph ) by giving directions in the arc of the graph. The
// result may not be a tree but may have cross arcs.
//--------------------------------------------------------------------
// Represents the node in the graph

class HH_Arc;

//--------------------------------------------------------------------
/**
 * @nodoc
 */
ENTITY_IS_PROTOTYPE(HH_Node, HEAL)
#if 0
; // semicolon needed for mkman (doc tool) parsing
#endif
#define HH_Node_LEVEL (ATTRIB_HH_LEVEL + 1)
extern DECL_HEAL int HH_Node_TYPE;


//--------------------------------------------------------------------
//--------------------------------------------------------------------

class DECL_HEAL HH_Node : public ATTRIB_HH//ENTITY
{
	// can add reference.
	friend class HH_Arc;

protected:

	// The incident arcs at this node

	EE_LIST	*	arc_list;
	EE_LIST	*	neighbour_list;

	// Cache the outgoing nodes, and incoming nodes.
	// These are changed whenever the arc is marked with an direction.

	EE_LIST*	outgoing_nodes;
	EE_LIST*	incoming_nodes;


	// Required for saving, copying and fix pointers
	virtual void	copy_from( ENTITY_LIST & , const HH_Node * );
	virtual void	add_to_list( ENTITY_LIST & ) const;
	virtual void	fix_from_array( ENTITY * array [] );

public:
	HH_Node();

public:

	// @ Query
	const EE_LIST *	    arcs_orig() const;
	EE_LIST *	    arcs_for_change() ;
	const int 			degree() const;

	void			add_arc( HH_Arc* );
	void			remove_arc( HH_Arc* );


	//get arc between node
	logical get_arc_with_node(HH_Node*,HH_Arc* &);


	const EE_LIST *	neighbours_orig() const { return neighbour_list; }
	EE_LIST *	neighbours_for_change() { return neighbour_list; }

	const int			out_degree() const { return outgoing_nodes->count(); }
	const int			virtual_out_degree();
	const int			in_degree() const { return incoming_nodes->count(); }
	const int			virtual_in_degree();
	const int			marked_degree() const { return (incoming_nodes->count() +
														outgoing_nodes->count()); }

	EE_LIST*   set_outgoing_nodes() { return outgoing_nodes;}

	const EE_LIST*   out_nodes() const { return outgoing_nodes;}
	const EE_LIST*   in_nodes() const { return incoming_nodes;}
	EE_LIST*   out_nodes_for_change() { return outgoing_nodes;}
	EE_LIST*   in_nodes_for_change() { return incoming_nodes;}

	// Removes this node from the graph. Deletes all other arcs that
	// emanate from this node.
	void				remove_node();


	// debug
	virtual void		debug( FILE * ) const;

	//MG cleanup function
	virtual void cleanup();

	/**
	 * @nodoc
	 */
	ATTRIB_FUNCTIONS( HH_Node, HEAL)
#if 0
; // semicolon needed for mkman (doc tool) parsing)
#endif
// Because mkman isn't aware that ATTRIB_FUNCTIONS changes access to "public", we do so
// explicitly here:
public:

	// this is a use counted class
	//USE_COUNTED_DECL
};

//--------------------------------------------------------------------
#endif

//--------------------------------------------------------------------
