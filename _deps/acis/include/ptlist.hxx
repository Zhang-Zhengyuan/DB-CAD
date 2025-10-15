/*******************************************************************/
/*    Copyright (c) 1989-2020 by Spatial Corp.                     */
/*    All rights reserved.                                         */
/*    Protected by U.S. Patents 5,257,205; 5,351,196; 6,369,815;   */
/*                              5,982,378; 6,462,738; 6,941,251    */
/*    Protected by European Patents 0503642; 69220263.3            */
/*    Protected by Hong Kong Patent 1008101A                       */
/*******************************************************************/
#ifndef _PTLIST
#define _PTLIST
#include "base.hxx"
/**
 * @file ptlist.hxx
 * @CAA2Level L1
 * @CAA2Usage U1
 *! \addtogroup FCTAPI
 *  \brief Declared at <ptlist.hxx>
 *  @{
 */

//DOCTEXT
// TOPIC(AF_POINT,Faceter classes;AF_POINT class)
//
// An AF_POINT is a point created by the faceter and attached
// to an ENTITY for subsequent reuse. An ATTRIB_EYE_POINTLIST_HEADER
// is an ACIS attribute used to attach a list of AF_POINT instances to an ENTITY.
//
// The data in an AF_POINT is:
//	-- xyz coordinates.
//	-- a parametric coordinate in the underlying ENTITY's parameter
//			space
//	-- pointers to predecessor and successor in a circular, doubly linked list
//	-- an identifier field used only by the faceter.
//
// All  insertion and traversal operations have a sense bit
//	that determines whether to deal with the list in a forward
//	(sense=0) or reverse (sense=1) order.  When the list is attached
//	to an edge, the forward sense of the list agrees with the forward
//	sense of the edge.
//
//END_DOCTEXT
//
/**
 * The identifier in an AF_POINT is typedefed as an AF_POINT_ID.
 */
typedef uintptr_t AF_POINT_ID;
#include "dcl_fct.h"
#include "entity.hxx"
#include "param.hxx"
#include "position.hxx"
#include "atteye3d.hxx"
#include "edge.hxx"
class law;

ENTITY_IS_PROTOTYPE( ATTRIB_EYE_POINTLIST_HEADER, FCT )

/*
// tbrv
*/
/**
 * @nodoc
 */
extern DECL_FCT int ATTRIB_EYE_POINTLIST_HEADER_TYPE;

#define ATTRIB_EYE_POINTLIST_HEADER_LEVEL ( ATTRIB_EYE_LEVEL + 1 )
class AF_POINT;
class AF_POINT_LIST;

/**
 * <tt>AF_POINT</tt> is an element in a circular, doubly linked list, used when
 * approximating a curve with a consecutive series of points. A line between each
 * pair of consecutive points can be considered a facet. <tt>AF_POINT</tt> is 
 * typically associated with a curve: it contains a position that lies on the
 * curve, and a parameter value that is the t-parameter of the curve at that point.
 * <br><br>
 * For additional information refer to the <i>Edge Faceting</i> section of the
 * <i>Faceting Algorithm and Interface</i> Technical Article.
 */
class DECL_FCT AF_POINT : public ACIS_OBJECT {
	// Points are joined in CYCLIC DOUBLY LINKED LISTs via these pointers:
	AF_POINT *next_point;		
	AF_POINT *prev_point;
	SPAparameter t;		// Parameteric coordinate;
	SPAposition X;		// Cartesian coordinates
	AF_POINT_ID id;		// Identifier for pointer-free cross edge connectivity
	friend class AF_POINT_LIST;
	friend class af_point_private_interface;
	friend DECL_FCT void delete_all_connected_af_points( AF_POINT* pts );

public:
	/**
	 * C++ constructor, creating a <tt>AF_POINT</tt> using the specified parameters.
	 * <br><br>
	 * @param _id
	 * Provides a way to identify this item uniquely. 
	 * @param prev
	 * If NULL, the point is created as a singleton linked list.
	 * Otherwise the new point is inserted into that linked list
	 * so that it follows the previous point.
	 * @param sense
	 * The sense flag determines whether the existing list is interpreted in a
	 * forward or reverse sense during the insertion.
	 */
	AF_POINT(
		AF_POINT_ID _id=0,
		AF_POINT* prev = (AF_POINT*)NULL,
		int sense = 0
	);
	/**
	 * Set the position of this AF_POINT.
	 * <br><br>
	 * @param Xtemp
	 * The position.
	 */
	void set_position(
		const SPAposition &Xtemp		// Cartesian coordinates
	);


	/**
	 * Set the parameter (t value of the curve) corresponding to the 3d position of this AF_POINT.
	 * <br><br>
	 * @param _t
	 * The t value on the curve corresponding to the 3D position of this AF_POINT.
	 */
	void set_parameter(
		const double &_t			// Parametric coordinate
	);

	/**
	 * Set the unique identifier (if needed).
	 * <br><br>
	 * @param _id
	 * Any value.
	 */
	inline void set_user_id(
		const AF_POINT_ID _id		// Id field
	)
	{ id = _id; }

	/**
	 * Get the 3D position of this AF_POINT.
	 */
	inline const SPAposition & get_position() const { return X; }	// Returns cartesian coordinates.

	/**
	 * Get the parameter (t value of the curve) corresponding to the 3d position of this AF_POINT.
	 */
	inline const double get_parameter() const { return t; }		// Returns parametric coordinate.

	/**
	 * Get the unique identifier (if needed).
	 */
	inline const AF_POINT_ID get_user_id(){ return id;}	// Returns identifier

	/**
	 * Return the 'next' point in a given direction of traversal.
	 * <br><br>
	 * @param sense
	 * The direction of traversal: FORWARD or REVERSE.
	 */
	AF_POINT *next(int sense) const
	{
		if(sense){ return prev_point;
		}else{ return next_point; }
	}

	/**
	 * Search the given entity for an attached pointlist. If found,
	 * fill in the head and tail pointers in the indicated sense.
	 * <br><br>
	 * @param E
	 * Entity on which pointlist is sought, normally an EDGE
	 * @param sense
	 * 0=forward order, 1=reverse order. Use the defined FORWARD or REVERSE.
	 * @param P0
	 * Start point (according to sense)
	 * @param P1
	 * End point (according to sense)
	 */
	LOCAL_PROC logical find(
			ENTITY*E,		// Entity on which pointlist is sought.
			int sense,		// 0=forward order, 1=reverse order.
			AF_POINT *&P0,	// Start point (according to sense)
			AF_POINT *&P1	// End point (according to sense)
			);
	/**
	 * Attach an AF_POINT to the given entity.  Any prior AF_POINT
	 * is removed.
	 * @param E
	 * Entity on which the pointlist is attached, normally an EDGE
	 */
	void attach(
		ENTITY *E		// Entity to which _THIS_ is to be attached.
	);

	/**
	 * @nodoc
	 */
	void dump();
	/**
	 * @nodoc
	 */
	void dump_loop();
	/**
	 * @nodoc
	 */
	void create_SCM_file(char* filename, int flags);
};

/**
* @nodoc.  
**/
DECL_FCT void delete_all_connected_af_points( AF_POINT* pts );


//=======================================================================
// JOE
//
// The AF_POINT_LIST class is used to maintain a use count for a list
// of AF_POINTS.

/*
// tbrv
*/
/**
 * @nodoc
 */
class DECL_FCT AF_POINT_LIST : public ACIS_OBJECT
{
private:
	AF_POINT* m_pAfPoint;
	int m_useCount;

	~AF_POINT_LIST();
public:
	AF_POINT_LIST(AF_POINT*);

	// Get the real list
	AF_POINT* GetPoint() { return m_pAfPoint; }

	// Maintain the use count
	int AddRef();
	int Release();
};

//=======================================================================

//DOCTEXT
// TOPIC(ATTRIB_EYE_POINTLIST_HEADER, Faceter Attributes; ATTRIB_EYE_POINTLIST_HEADER class)
//
// The ATTRIB_EYE_POINTLIST_HEADER class is an ACIS attribute
// that can be attached to any entity to associate a cyclic
// doubly linked list of AF_POINT instances with the entity.
//
//END


// Flag bits for application-controlled semantics
/*
// tbrv
*/
/**
 * @nodoc
 */
#define AF_POINTLIST_PROTECTED 0x00000001
// Flag bits for use by the facter
/*
// tbrv
*/
/**
 * @nodoc
 */
#define AF_POINTLIST_DIRTY 				0x00010000
/*
// tbrv
*/
/**
 * @nodoc
 */
#define AF_POINTLIST_HEAD_VTX_VISITED	0x00020000
/*
// tbrv
*/
/**
 * @nodoc
 */
#define AF_POINTLIST_TAIL_VTX_VISITED	0x00040000


/*
// tbrv
*/
/**
 * The ATTRIB_EYE_POINTLIST_HEADER class stores the edge facets between faceter calls.
 * In this way, incremental faceting can occur with the facets remaining watertight.
 *
 * The class is intended for internal use only.  Customers should only manipulate edge facets 
 * via Faceter APIs.  Do not derive from this class.
 *
 * Do not delete these attributes.
 */
class DECL_FCT ATTRIB_EYE_POINTLIST_HEADER : public ATTRIB_EYE
{
   AF_POINT_LIST* m_pPointList;
	unsigned long flags;

   public:
		//MEMBER
		// Construct a pointlist header attached to the  given entity.
      ATTRIB_EYE_POINTLIST_HEADER( ENTITY* = NULL);

		//MEMBER
		// Return the first AF_POINT of the cyclic list in the header.
	  AF_POINT *get_pointlist();
		//MEMBER
		// Replace the AF_POINT list.  All members of the previous
		// list are deleted.
	  void replace_pointlist(AF_POINT *);

		//MEMBER
		// Search the given entity for an ATTRIB_EYE_POINTLIST_HEADER
	LOCAL_PROC ATTRIB_EYE_POINTLIST_HEADER *find(ENTITY*E);

		//MEMBER
		// set and query control flags:
		unsigned int get_flag(unsigned long mask);
		void set_flag(unsigned long mask, int value);

		//END

      // returns TRUE
      virtual logical pattern_compatible() const;

      // Functions called to aid attribute migration during 
      // modelling operations.

      // the owner of this attribute is about to be split in two
      // - the argument is the new piece 

      virtual void split_owner( ENTITY* ) ;

      // the owner of this attribute is about to be merged 
      // with the given entity.
      // The logical argument is TRUE if the owner is to be deleted 
      // in the merge.

      virtual void merge_owner( ENTITY*, logical ) ;

      virtual void trans_owner( SPAtransf const & ) ;

	  virtual void warp_owner( law *);
   
      virtual void lop_change_owner();

      virtual void replace_owner_geometry( ENTITY* );

	  /**
	  * @nodoc. do not use.
	  **/
	  AF_POINT_LIST* get_point_list();

	  /**
	  * @nodoc.  do not use
	  **/
	  void set_point_list(AF_POINT_LIST* new_list );

/**
 * @nodoc
 */
	logical savable() const;


      /**
       * @nodoc
       */
      ATTRIB_FUNCTIONS( ATTRIB_EYE_POINTLIST_HEADER, FCT )
#if 0
; // semicolon needed for mkman (doc tool) parsing)
#endif
// Because mkman isn't aware that ATTRIB_FUNCTIONS changes access to "public", we do so
// explicitly here:
public:

};

/*
// tbrv
*/
/**
 * @nodoc
 */
logical af_get_facet_edge_points(EDGE * edge, SPAposition *& polyline, int & num_pts);

/* @} */

#endif
