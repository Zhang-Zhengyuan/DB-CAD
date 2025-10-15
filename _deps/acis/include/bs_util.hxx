/*******************************************************************/
/*    Copyright (c) 1989-2020 by Spatial Corp.                     */
/*    All rights reserved.                                         */
/*    Protected by U.S. Patents 5,257,205; 5,351,196; 6,369,815;   */
/*                              5,982,378; 6,462,738; 6,941,251    */
/*    Protected by European Patents 0503642; 69220263.3            */
/*    Protected by Hong Kong Patent 1008101A                       */
/*******************************************************************/
#ifndef BASE_UTIL_HXX
#define BASE_UTIL_HXX
//--------------------------------------------------------------------------
#include	"lists.hxx"
#include	"vlists.hxx"

#include	"glb_node.hxx"
#include	"nd_fc.hxx"
#include	"nd_fclst.hxx"
#include	"hh_bsslv.hxx"
#include	"glb_arc.hxx"
#define xor_heal(a,b) ((a && !b) || (b && !a))
class FACE;
class surface;
//--------------------------------------------------------------------------
class Entity_List_Iterator
{
	int 		current;	// current index
	void 		*curvoid;	// previous entry
	ENTITY 		*curent;	// current entity
	ENTITY_LIST *list;		// list to iterate

public:
	Entity_List_Iterator( const ENTITY_LIST & l );

	const Entity_List_Iterator & operator = ( const Entity_List_Iterator &);

	ENTITY * operator ++ ();
	ENTITY * operator ++ ( int );

	ENTITY * operator * ();
};
//-------------------------------------------------------------------
//-------------------------------------------------------------------
//Classes which will help in sorting.

class Node_Wrapper : public ACIS_OBJECT
{
	HH_GlobalNode  * pNode;
	int			sort_degree;

public:
	Node_Wrapper()
		: pNode(0),sort_degree(0){}

	Node_Wrapper( HH_GlobalNode * node )
		: pNode( node ), sort_degree(0)
	{}

	// set node
	void set_node( HH_GlobalNode * n )
	{ pNode = n; }
	HH_GlobalNode  * get_node_for_change() { return pNode;}

	// return degree of a node.
	int get_sort_deg() const { return sort_degree; }
	void set_sort_deg(int i) { sort_degree = i;}
};
//---------------------------------------------------------------------------------
//---------------------------------------------------------------------------------
class Node_Sorter : public ACIS_OBJECT
{
	HH_GlobalNode * pnode;
	HH_GlobalNode * root;
public:
	Node_Sorter()
		: pnode(0),root(0){}

	Node_Sorter( HH_GlobalNode * node ,HH_GlobalNode * r)
		: pnode( node ),root(r)
	{}

	void set_node( HH_GlobalNode * n )
	{ pnode = n; }

	void set_root( HH_GlobalNode * n )
	{ root = n; }

	HH_GlobalNode * node() { return pnode; }
	HH_GlobalNode * root_node() { return root; }
};
//---------------------------------------------------------------------------------
//---------------------------------------------------------------------------------
class HH_DAGWrap : public ACIS_OBJECT
{
public:
	VOID_LIST node_list;
	VOID_LIST arc_list;
public:
	HH_DAGWrap(){}
};
//---------------------------------------------------------------------------------
//---------------------------------------------------------------------------------
// A structure to assist in sorting the array.

class NodePair : public ACIS_OBJECT
{
	HH_GlobalNode  * pNode;
	int			unmarked_degree;

public:
	NodePair()
		: pNode(0), unmarked_degree(0){}

	NodePair( HH_GlobalNode * node )
		: pNode( node ), unmarked_degree(0)
	{
		update();
	}

	// Update the degree
	//MG 20 July 99 new function for marking arc
	void update()
	{
		unmarked_degree = 0;
		pNode->arcs_orig()->init();
		for( HH_GlobalArc * arc; arc = (HH_GlobalArc*)pNode->arcs_orig()->next(); )
			if( hh_nodir == arc->direction())
				unmarked_degree++;
	}

	// set node
	void set_node( HH_GlobalNode * n )
	{ pNode = n; }

	// return number of unmarked arcs in a node.
	int udeg() const { return unmarked_degree; }

	//return degree for sorting
	int sort_deg()
	{
		int node_deg = node()->in_degree() + unmarked_degree ;
		return node_deg;
	}

	// mark direction in the arc
	void mark_arcs()
	{
		node()->arcs_orig()->init();
		HH_GlobalArc * arc;
		while( (arc = (HH_GlobalArc*) node()->arcs_orig()->next()) )
		{
			if( arc->direction() == hh_nodir )
				arc->set_direction(
				arc->first_vertex() == node() ?
				hh_forward : hh_backward );
		}
	}

	// Current pair
	HH_GlobalNode * node() { return pNode; }
};

//---------------------------------------------------------------------------------
//---------------------------------------------------------------------------------
class HH_Wrapper : public ACIS_OBJECT
{
	void  * pNode;
	int			sort_degree;

public:
	HH_Wrapper()
		: pNode(0),sort_degree(0){}

	HH_Wrapper( void * node )
		: pNode( node ), sort_degree(0)
	{}

	// set node
	void set_node( void * n )
	{ pNode = n; }
	void  * get_node_for_change() { return pNode;}

	// return degree of a node.
	int get_sort_deg() const { return sort_degree; }
	void set_sort_deg(int i) { sort_degree = i;}
};
//---------------------------------------------------------------------------------
//---------------------------------------------------------------------------------

//com 4 debug
void DECL_HEAL copy( const ENTITY_LIST & , EE_LIST* );
void DECL_HEAL copy( const EE_LIST*  , ENTITY_LIST &);
/*
void copy( const VOID_LIST &, VOID_LIST & );
void copy( const VOID_LIST & , EE_LIST * );
void copy( const EE_LIST * , VOID_LIST & );
void copy( const EE_LIST * , EE_LIST *  );
void copy( const ENTITY_LIST & , ENTITY_LIST & );
void lose( ENTITY_LIST & );*/
//-------------------------------------------------------------------

#define FOREACH_IN_LIST( LIST, TYPE, FUNC) \
	{ \
		LIST.init(); TYPE * tent; \
		while( tent = (TYPE*)LIST.next()) \
			tent->FUNC; \
	}

//-------------------------------------------------------------------

#define FOREACH_IN_EE_LIST( LIST, TYPE, FUNC) \
	{ \
		if (LIST != NULL)   {  \
			LIST->init(); TYPE * tent; \
			while( tent = (TYPE*)LIST->next()) \
				tent->FUNC; \
		}  \
	}

//-------------------------------------------------------------------

#define USE_COUNT_LIST_NOTIFY( LIST, TYPE,ADDF, REMOVEF) \
	if(LIST){  \
	switch( type) { \
		case DELETE_BULLETIN: \
			FOREACH_IN_LIST( LIST, TYPE, REMOVEF) \
			break; \
		case CREATE_BULLETIN: \
			FOREACH_IN_LIST( LIST, TYPE, ADDF) \
			break; \
		case CHANGE_BULLETIN: \
			if (other_ent->LIST != NULL) { \
				if( other_ent->LIST.count() > 0) \
					FOREACH_IN_LIST( LIST, TYPE, ADDF) \
			} \
			if (LIST != NULL) {\
				if( LIST.count() > 0) { \
					FOREACH_IN_LIST( LIST, TYPE, REMOVEF) \
			}	\
			break; \
		}\
	}

//-------------------------------------------------------------------

#define USE_COUNT_DEF_EE_LIST_NOTIFY( LIST, TYPE,ADDF, REMOVEF) \
if(LIST) { \
  switch( type) { \
		case DELETE_BULLETIN: \
			FOREACH_IN_EE_LIST( LIST, TYPE, REMOVEF) \
			break; \
		case CREATE_BULLETIN: \
			FOREACH_IN_EE_LIST( LIST, TYPE, ADDF) \
			break; \
		case CHANGE_BULLETIN: \
			if (other_ent->LIST != NULL) { \
				if( other_ent->LIST->count() > 0) \
					FOREACH_IN_EE_LIST( LIST, TYPE, ADDF) \
			} \
			if (LIST != NULL) {\
				if( LIST->count() > 0) \
					FOREACH_IN_EE_LIST( LIST, TYPE, REMOVEF) \
			}	\
			break; \
		}\
	}
//--------------------------------------------------------------------------
//--------------------------------------------------------------------------
//--------------------------------------------------------------------------
HH_GlobalFaceListNode * replace_face_nodes_by_facelist(ENTITY_LIST&);

void  sort_outgoing_nodes(HH_GlobalNode* );

void get_surface_and_vertices_linked_to_node(HH_GlobalNode*,ENTITY_LIST&,ENTITY_LIST&);

FACE * get_face_from_node(HH_GlobalNode*);

void print_surface_details(const surface& ,FILE*);

void merge_face_geometry(FACE*,FACE*,logical);

HH_GlobalNode* get_node_linked_with_node_arc(HH_GlobalNode*,HH_GlobalArc*);

void get_node_constraint_list(HH_GlobalNode*,ENTITY_LIST&);

int get_face_neighbors(HH_GlobalNode*,ENTITY_LIST& = *(ENTITY_LIST*)NULL_REF);

logical are_all_arcs_similar(HH_GlobalNode*,HH_ArcDir);
//--------------------------------------------------------------------------
//--------------------------------------------------------------------------
//--------------------------------------------------------------------------
#endif
