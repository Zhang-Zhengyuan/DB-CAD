/*******************************************************************/
/*    Copyright (c) 2005-2020 by Spatial Corp.                     */
/*    All rights reserved.                                         */
/*    Protected by U.S. Patents 5,257,205; 5,351,196; 6,369,815;   */
/*                              5,982,378; 6,462,738; 6,941,251    */
/*    Protected by European Patents 0503642; 69220263.3            */
/*    Protected by Hong Kong Patent 1008101A                       */
/*******************************************************************/
// Jeff 01-21-05 Revived a portion of PHL to support legacy data. BTS 78294
#if !defined( PHL_EDGE_CLASS )
#define PHL_EDGE_CLASS

// =============================================================================
// Includes:
// -----------------------------------------------------------------------------
#include "dcl_kern.h"
#include "logical.h"
#include "ent_phl.hxx"
#include "phlentmc.hxx"
/**
 * @file phl_edge.hxx
 * @CAA2Level L1
 * @CAA2Usage U1
 * \addtogroup ACISVISUALIZATION
 *
 * @{
 */

// =============================================================================
// Forwards:
// -----------------------------------------------------------------------------

class EDGE;
class BODY;
class FACE;
class PHL_SEGMENT;
class ENTITY_LIST;
/**
 * @nodoc
 */
ENTITY_IS_PROTOTYPE(PHL_EDGE, KERN)
#if 0
; // semicolon needed for mkman (doc tool) parsing
#endif

// =============================================================================
// Defines:
// -----------------------------------------------------------------------------

// This is a derived class of ENTITY_PHL
#define PHL_EDGE_LEVEL ( ENTITY_PHL_LEVEL + 1)

// =============================================================================
// Declarations:
// -----------------------------------------------------------------------------

// Identifier for the type of ENTITY
// tbrv
/**
 * @nodoc
 */
extern DECL_KERN int PHL_EDGE_TYPE;

// =============================================================================
// Class:	PHL_EDGE
// -----------------------------------------------------------------------------
/**
 * @nodoc
 * Defines a regular or silhouette edge.
 * <br>
 * <b>Role:</b> A <tt>PHL_EDGE</tt> represents one edge (regular edge or silhouette edge).
 * <br><br>
 * A <tt>PHL_EDGE</tt> points to the edge, to the owning body, to the owning face for
 * silhouette edges, and to a list of segments.
 * <br><br>
 * <tt>PHL_EDGE</tt> constructors make a new bulletin entry in the current bulletin board
 * to record the creation of the <tt>PHL_EDGE</tt>.
 * <br><br>
 * <b>Note:</b> The PHL functionality for which this class was intended is no longer available.
 * This class is provided to support lagacy data. The capabilities of this object
 * have been reduced to allow simple evaluation, save, restore, and deletion.
 * <br><br>
 * @see PHL_SEGMENT,ATTRIB_PHL_VW,BODY, EDGE, FACE
 */
class DECL_KERN PHL_EDGE : public ENTITY_PHL
{

private:

	EDGE*			edge_ptr;	// edge
	BODY*			body_ptr;	// body
	PHL_SEGMENT*	segment_ptr;// 1st PHL_SEGMENT of this edge
	FACE*			face_ptr;	// face (0 if edge is NOT silh.)


protected:
	
	// protected members for chain hook/unhook:
	PHL_CHAIN_DEC( PHL_EDGE )

	// protected member functions for ACIS:
	/**
	 * @nodoc
	 */
	ENTITY_FUNCTIONS( PHL_EDGE, KERN)
#if 0
; // semicolon needed for mkman (doc tool) parsing)
#endif
// Because mkman isn't aware that ENTITY_FUNCTIONS changes access to "public", we do so
// explicitly here:

public:
/**
 * @nodoc
 */
PHL_HOOK_DEC( PHL_EDGE)
#if 0
; // semicolon needed for mkman (doc tool) parsing)
#endif
/**
 * @nodoc
 */
PHL_UNHOOK_DEC( PHL_EDGE)
#if 0
; // semicolon needed for mkman (doc tool) parsing)
#endif


// =============================================================================
// Public Function:	PHL_EDGE	Constructor
// -----------------------------------------------------------------------------
/**
 * @nodoc
 */
PHL_EDGE(
    EDGE*		ed = 0,		// I: edge
    BODY*		by = 0,		// I: body
    PHL_SEGMENT*hs = 0,		// I: segment chain
    FACE*		fa = 0		// I: owning face (only for silh. edge)
  );

// =============================================================================
// Public Function:	PHL_EDGE	Deep Copy Constructor
// -----------------------------------------------------------------------------
/**
 * @nodoc
 */
PHL_EDGE( PHL_EDGE const& e );

// =============================================================================
// Public Function:	edge		Return edge of this PHL_EDGE
// -----------------------------------------------------------------------------
/**
 * Returns the <tt>EDGE</tt> that this <tt>PHL_EDGE</tt> references.
 */
EDGE* edge() const { return( edge_ptr); };

// =============================================================================
// Public Function:	body		Return owning body of this PHL_EDGE
// -----------------------------------------------------------------------------
/**
 * Returns the owning <tt>BODY</tt> of the <tt>PHL_EDGE</tt>.
 */
BODY* body() const { return( body_ptr); };

// =============================================================================
// Public Function:	segment		Return 1st PHL_SEGMENT of this PHL_EDGE
// -----------------------------------------------------------------------------
/**
 * Returns the first segment of this <tt>PHL_EDGE</tt>.
 */
PHL_SEGMENT* segment() const { return( segment_ptr); };

// =============================================================================
// Public Function:	segment_list	Return all PHL_SEGMENTs of this PHL_EDGE
// -----------------------------------------------------------------------------
/**
 * Returns a list of all the <tt>PHL_SEGMENTs</tt> of this <tt>PHL_EDGE</tt>.
 *<br><br>
 * @param seg_list
 * list of PHL_SEGMENTS.
 */
void segment_list( ENTITY_LIST&	seg_list ) const;

// =============================================================================
// Public Function:	face Return owning face of this PHL_EDGE (NULL if not a sil)
// -----------------------------------------------------------------------------
/**
 * Returns the owning <tt>FACE</tt> of this <tt>PHL_EDGE</tt>, or <tt>0</tt> if the <tt>PHL_EDGE</tt> is not a silhouette edge.
 */
FACE* face() const { return( face_ptr); };

// =============================================================================
// Public Function:  clean       Remove things pointed to.
// -----------------------------------------------------------------------------
/**
 * Removes all <tt>PHL_SEGMENTs</tt> beginning with the first <tt>PHL_SEGMENT</tt>.
 *<br><br>
 * <b>Role:</b> Unhooks the <tt>PHL_SEGMENT</tt> from the list of segments.
 * Removes the <tt>EDGE</tt> if it is a silhouette edge
 */
void clean();

protected:

// =============================================================================
// Protected Function:	set_edge	Set a new edge pointer
// -----------------------------------------------------------------------------
void set_edge( EDGE* ed );

// =============================================================================
// Protected Function:	set_body	Set a new body pointer
// -----------------------------------------------------------------------------
void set_body( BODY* by );

// =============================================================================
// Protected Function:	set_segment	Set a new PHL_SEGMENT pointer
// -----------------------------------------------------------------------------
void set_segment( PHL_SEGMENT* sg );

// =============================================================================
// Protected Function:	set_face	Set a new face pointer
// -----------------------------------------------------------------------------
void set_face( FACE* fa );

};

/** @} */
#endif
