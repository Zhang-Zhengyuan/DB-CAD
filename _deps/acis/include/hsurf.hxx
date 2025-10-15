/*******************************************************************/
/*    Copyright (c) 1989-2020 by Spatial Corp.                     */
/*    All rights reserved.                                         */
/*    Protected by U.S. Patents 5,257,205; 5,351,196; 6,369,815;   */
/*                              5,982,378; 6,462,738; 6,941,251    */
/*    Protected by European Patents 0503642; 69220263.3            */
/*    Protected by Hong Kong Patent 1008101A                       */
/*******************************************************************/

//------------------------------------------------------------------------------
/*
	Snap node for the surface in the tangent snapper graph. The HH_Surfsnap
	maintains bridge between surface through an attribute pointer maintained
	internally. It also makes it possible to instance as many graphs for the
	given body and solve it separately.
*/

//------------------------------------------------------------------------------
#ifndef HH_SurfSnap__HXX
#define HH_SurfSnap__HXX

//------------------------------------------------------------------------------
#include "lists.hxx"
#include "gnode.hxx"
#include "snapper.hxx"

#include "vector.hxx"
#include "matrix.hxx"
#include "transf.hxx"

#include    "hh_trans.hxx"


//------------------------------------------------------------------------------

class ATTRIB_HH_SURFACE_SNAP;
class SURFACE;
class ENTITY_LIST;
class HH_Trans;
//--------------------------------------------------------------------

class HH_SurfSnap_Node_Type;
logical operator == (const HH_SurfSnap_Node_Type &,const HH_SurfSnap_Node_Type &);
/**
 * @nodoc
 */
ENTITY_IS_PROTOTYPE( HH_Unstable_SurfSnap, HEAL )
#if 0
; // semicolon needed for mkman (doc tool) parsing
#endif
/**
 * @nodoc
 */
ENTITY_IS_PROTOTYPE(HH_SurfSnap, HEAL)
#if 0
; // semicolon needed for mkman (doc tool) parsing
#endif

//------------------------------------------------------------------------------

class DECL_HEAL HH_SurfSnap_Node_Type {
public:
	friend logical operator == (
		const HH_SurfSnap_Node_Type &,
		const HH_SurfSnap_Node_Type &);

       // STI jdh - needed a default constructor on osf1.
       HH_SurfSnap_Node_Type() {}

};

//------------------------------------------------------------------------------

//--------------------------------------------------------------------
#define HH_SurfSnap_LEVEL (HH_GNode_LEVEL + 1)


//------------------------------------------------------------------------------
class DECL_HEAL HH_SurfSnap : public HH_GNode
{
protected:
	friend class ATTRIB_HH_SURFACE_SNAP;
	ATTRIB_HH_SURFACE_SNAP *	surface_snap;


	// Required for saving, copying and fix pointers
	virtual void	copy_from( ENTITY_LIST & , const HH_GNode * );
	virtual void	add_to_list( ENTITY_LIST & ) const;
	virtual void	fix_from_array( ENTITY * array [] );
	HH_Trans  trans; // for rotation, translation and scaling.
	double node_tol;

public:

	HH_SurfSnap();
	HH_SurfSnap( SURFACE * );
	//virtual ~HH_SurfSnap();

    virtual logical pattern_compatible() const;
                                    // returns TRUE
	void free_surface_snap();

	//Check snapping
	logical check_snapping();

	// Returns the owner node through the bridge
	SURFACE * owner_surface() const;

	SURFACE * owner_surface_for_change() ;

	virtual const HH_SurfSnap_Node_Type & type() const/* = 0*/;

	// Snaps incoming snap nodes to this node. This node remains
	// fixed.
	virtual void snap_in( HH_Snap_Type = strong_snap ){}

	virtual void cleanup();

	// Snaps this node to outgoing nodes. The outgoing nodes
	// remain fixed.
	virtual logical snap_out( HH_Snap_Type = strong_snap );

	const HH_Trans& get_transformation();
	const double & get_node_tol();
	void set_node_tol(double);
	void set_transformation(const HH_Trans&);

	// Entity functions.
	/**
	 * @nodoc
	 */
	ENTITY_FUNCTIONS( HH_SurfSnap, HEAL)
#if 0
; // semicolon needed for mkman (doc tool) parsing)
#endif
// Because mkman isn't aware that ENTITY_FUNCTIONS changes access to "public", do so
// explicitly here:
public:

};

//------------------------------------------------------------------------------
// Unstable node pair
struct unstable_pair : public ACIS_OBJECT
{
	SURFACE * first_;
	SURFACE * second_;

	unstable_pair( const unstable_pair & p )
		: first_( p.first_ ), second_( p.second_ ){}

	unstable_pair( SURFACE * one, SURFACE * two )
				   : first_(one), second_(two){}

	const SURFACE * first() const { return first_; }
	const SURFACE * second() const { return second_; }

	SURFACE * first() { return first_; }
	SURFACE * second() { return second_; }

	SURFACE * match( SURFACE * m )
	{
		return ( m == first() ) ? second() :
		       ( m == second()) ? first()  :
			   ( SURFACE*)NULL;
	}

	logical operator == ( const unstable_pair & p )
	{
		return ( (first() == p.first() && second() == p.second())
			||   (first() == p.second() && second() == p.first()));
	}
};

//--------------------------------------------------------------------
#define HH_Unstable_SurfSnap_LEVEL (HH_SurfSnap_LEVEL + 1)
extern DECL_HEAL int HH_Unstable_SurfSnap_TYPE;

//------------------------------------------------------------------------------
/*
	Node for unstable vertex.
*/

class HH_Unstable_SurfSnap : public HH_SurfSnap
{
	VOID_LIST pairs;	// Subgraph of nodes which actually take part
						// in snapping.

	int get_matching_nodes( SURFACE*, ENTITY_LIST & );
protected:

	// Required for saving, copying and fix pointers
	virtual void	copy_from( ENTITY_LIST & , const HH_GNode * );
	virtual void	add_to_list( ENTITY_LIST & ) const;
	virtual void	fix_from_array( ENTITY * array [] );

public:
	HH_Unstable_SurfSnap()
		: HH_SurfSnap(){}

	HH_Unstable_SurfSnap( SURFACE * );

    virtual logical pattern_compatible() const;
                                    // returns TRUE
	virtual void snap_in( HH_Snap_Type = strong_snap ){}
	virtual logical snap_out( HH_Snap_Type = strong_snap );

	virtual const HH_SurfSnap_Node_Type & type() const;

	virtual void cleanup();

	SURFACE * get_matching_node( const HH_SurfSnap * );
	int get_matching_nodes( const HH_SurfSnap *, ENTITY_LIST & );

	void add_pair( SURFACE *, SURFACE *);
	int  pair_lookup( SURFACE *, SURFACE *);

	// Entity functions.
	/**
	 * @nodoc
	 */
	ENTITY_FUNCTIONS( HH_Unstable_SurfSnap, HEAL)
#if 0
; // semicolon needed for mkman (doc tool) parsing)
#endif
// Because mkman isn't aware that ENTITY_FUNCTIONS changes access to "public", do so
// explicitly here:
public:

};


//------------------------------------------------------------------------------
struct Snap_Pack : public ACIS_OBJECT
{
	SURFACE * snode;		// Surface we are going to snap.
	ENTITY_LIST group_list; // List of groups to which we are doing snapping
	double snap_tol;

	Snap_Pack(): snode(0){snap_tol =-1;}

	~Snap_Pack();

	// Try snapping to all the nodes.
	// strong condition.
	snap strong_snapping( HH_Trans & tr );
	double get_toler(){ return snap_tol;}
	void set_toler(double t){ snap_tol = t;}

	// Try snapping to any of the nodes.
	snap weak_strong_snapping( HH_Trans & tr );
	snap weak_weak_snapping( HH_Trans & tr );
};

//------------------------------------------------------------------------------
#endif

//------------------------------------------------------------------------------

