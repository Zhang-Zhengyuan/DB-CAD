/*******************************************************************/
/*    Copyright (c) 1989-2020 by Spatial Corp.                     */
/*    All rights reserved.                                         */
/*    Protected by U.S. Patents 5,257,205; 5,351,196; 6,369,815;   */
/*                              5,982,378; 6,462,738; 6,941,251    */
/*    Protected by European Patents 0503642; 69220263.3            */
/*    Protected by Hong Kong Patent 1008101A                       */
/*******************************************************************/

//--------------------------------------------------------------------
// Constraint Snapper graph node for analytic Solver
//--------------------------------------------------------------------

#ifndef HH_GNODE__HXX
#define HH_GNODE__HXX
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
// HH_GNode and HH_GArc together represents a undirected general
// graph of degree n. We can convert this graph to DAG ( Directed
// Acyclic Graph ) by giving directions in the arc of the graph. The
// result may not be a tree but may have cross arcs.
//--------------------------------------------------------------------
// Represents the node in the graph

class HH_GArc;

//--------------------------------------------------------------------
/**
 * @nodoc
 */
ENTITY_IS_PROTOTYPE(HH_GNode, HEAL)
#if 0
; // semicolon needed for mkman (doc tool) parsing
#endif
#define HH_GNode_LEVEL (ATTRIB_HH_LEVEL + 1)
extern DECL_HEAL int HH_GNode_TYPE;




// Determines type of snapping to be done with the neighbouring nodes.
// In weak snapping, we snap any one of the possible snap pairs.
// Whereas in strong snapping, we snap only if all the pairs can be
// snapped. Weak snapping is quite useful in snapping group with other
// group.
// "no_snap" is added for future use.

enum HH_Snap_Type
{
	weak_snap,
	strong_snap,
	no_snap
};
//--------------------------------------------------------------------
//--------------------------------------------------------------------

class DECL_HEAL HH_GNode : public ATTRIB_HH//ENTITY
{
	// can add reference.
	friend class HH_GArc;
	friend class HH_UV_GArc_Snap;

protected:

	// The incident arcs at this node

	EE_LIST	*	arc_list;
	EE_LIST	*	neighbour_list;
	BODY * body;

	// Cache the outgoing nodes, and incoming nodes.
	// These are changed whenever the arc is marked with an direction.

	EE_LIST*	outgoing_nodes;
	EE_LIST*	incoming_nodes;
	//Tell if snapping was a succ. or a failure
	logical		snap_outcome;
	logical     surface_modify;

	void			add_arc( HH_GArc* );
	void			remove_arc( HH_GArc* );

	// Required for saving, copying and fix pointers
	virtual void	copy_from( ENTITY_LIST & , const HH_GNode * );
	virtual void	add_to_list( ENTITY_LIST & ) const;
	virtual void	fix_from_array( ENTITY * array [] );

public:
	HH_GNode();
	// const HH_GNode & operator = ( const HH_GNode & n);

	//virtual ~HH_GNode();

public:

    virtual logical pattern_compatible() const;
                                    // returns TRUE
	// @ Query
	const EE_LIST *	    arcs_orig() const;
	const int 			degree() const;

	const BODY* get_body() {return body;}

	// should be for changing the body attributes
	BODY* get_body_for_change() {return body;}

	//should be used for setting the body
	void set_body_for_change(BODY * b) {
		body = b;
	}

	// Get Set functions for surface modification bit.
	void set_surface_modify(logical );
	logical get_surface_modify();

	void set_snap_outcome(logical outcome){ snap_outcome = outcome;}
	const logical get_snap_outcome() { return snap_outcome;}

	//get arc between node
	logical get_arc_with_node(HH_GNode*,HH_GArc* &);

	//sort outgoing nodes
	void sort_outgoing_nodes();

	const EE_LIST *	neighbours_orig() const { return neighbour_list; }

	const int			out_degree() const { return outgoing_nodes->count(); }
	const int			in_degree() const { return incoming_nodes->count(); }

	EE_LIST*   set_outgoing_nodes() { return outgoing_nodes;}

	const EE_LIST*   out_nodes() const { return outgoing_nodes;}
	const EE_LIST*   in_nodes() const { return incoming_nodes;}

	// Removes this node from the graph. Deletes all other arcs that
	// emanate from this node.
	void				remove_node();

	virtual logical snap_out( HH_Snap_Type = strong_snap ) {return FALSE;}

	// debug
	virtual void		debug( FILE * ) const;

	//MG cleanup function
	virtual void cleanup();

	// Entity functions.
	//ENTITY_FUNCTIONS( HH_GNode, HEAL);
	/**
	 * @nodoc
	 */
	ATTRIB_FUNCTIONS( HH_GNode, HEAL)
#if 0
; // semicolon needed for mkman (doc tool) parsing)
#endif
// Because mkman isn't aware that ATTRIB_FUNCTIONS changes access to "public", we do so
// explicitly here:
public:


	// this is a use counted class
	/**
	 * @nodoc
	 */
	USE_COUNTED_DECL
#if 0
; // semicolon needed for mkman (doc tool) parsing)
#endif
};

//--------------------------------------------------------------------
#endif

//--------------------------------------------------------------------
