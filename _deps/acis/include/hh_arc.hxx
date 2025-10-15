/*******************************************************************/
/*    Copyright (c) 1989-2020 by Spatial Corp.                     */
/*    All rights reserved.                                         */
/*    Protected by U.S. Patents 5,257,205; 5,351,196; 6,369,815;   */
/*                              5,982,378; 6,462,738; 6,941,251    */
/*    Protected by European Patents 0503642; 69220263.3            */
/*    Protected by Hong Kong Patent 1008101A                       */
/*******************************************************************/

#ifndef HH_ARC__HXX
#define HH_ARC__HXX

//--------------------------------------------------------------------
#include	"entity.hxx"
#include	"usecount.hxx"
#include	"vlists.hxx"
#include	"eelists.hxx"

#include	"dcl_heal.h"
#include	"att_hh.hxx"

//--------------------------------------------------------------------

enum HH_ArcDir
{
	hh_forward,
	hh_backward,
	hh_nodir,
	hh_virtual,
	hh_virtual_forw,
	hh_virtual_bkwrd
};


//--------------------------------------------------------------------

class HH_Node;
/**
 * @nodoc
 */
ENTITY_IS_PROTOTYPE(HH_Arc, HEAL)
#if 0
; // semicolon needed for mkman (doc tool) parsing
#endif
#define HH_Arc_LEVEL (ATTRIB_HH_LEVEL+1)
extern DECL_HEAL int HH_Arc_TYPE;

//--------------------------------------------------------------------

class DECL_HEAL HH_Arc : public ATTRIB_HH // ENTITY
{
protected:
	// List of nodes, this arc is connecting to.
	EE_LIST  * node_list;

	// Direction of arc from first vertex to second vertex.
	HH_ArcDir 			 arcDir;

protected:

	// Arcs are not allowed locally.
	HH_Arc( HH_Node *, HH_Node *, HH_ArcDir = hh_nodir);

	//virtual ~HH_Arc();

	// Required for saving, copying and fix pointers
	virtual void	copy_from( ENTITY_LIST & , const HH_Arc * );
	virtual void	add_to_list( ENTITY_LIST & ) const;
	virtual void	fix_from_array( ENTITY * array [] );

public:

	HH_Arc();

	// Create arc from two nodes with / without direction.
	static HH_Arc * create( HH_Node *, HH_Node *, HH_ArcDir = hh_nodir);

	// Query
	const int degree() const; // 2 always

	HH_ArcDir * get_direction_for_change() {return (&arcDir);}

	const EE_LIST *	vertices() const { return node_list; }

	 const HH_Node*		first_vertex() const
	{ return (HH_Node*)(node_list->init(),node_list->next());}
	const HH_Node*		second_vertex() const
	{ return (HH_Node*)(node_list->init(),node_list->next(),node_list->next());}

	// Modify
	// Remove this arc from the graph. If the node is no more
	// referenced, remove it too.
	void remove_arc();

	//MG cleanup function
	virtual void cleanup();


	// Collapse the arc, the two nodes become coincidence after
	// collapsing this arc. If two nodes have arc which is connecting
	// to a common vertex then merge of that arc is called.
	//virtual void collapse();

	// Get/Set direction of arc
	HH_ArcDir direction() const { return arcDir; }
	virtual void set_direction( HH_ArcDir d );
	virtual void force_direction( HH_ArcDir d );


	// Entity functions.
	//ENTITY_FUNCTIONS( HH_Arc, HEAL);
	/**
	 * @nodoc
	 */
	ATTRIB_FUNCTIONS( HH_Arc, HEAL)
#if 0
; // semicolon needed for mkman (doc tool) parsing)
#endif
// Because mkman isn't aware that ATTRIB_FUNCTIONS changes access to "public", we do so
// explicitly here:
public:

	// use count
	//USE_COUNTED_DECL

protected:

	// Merges this arc if two of the nodes are coinciding with each
	// other. make it virtual so that derived classes can use it
	// fruitfully.
	//virtual void merge( HH_Arc * garc );
};

//--------------------------------------------------------------------

#endif

//--------------------------------------------------------------------

