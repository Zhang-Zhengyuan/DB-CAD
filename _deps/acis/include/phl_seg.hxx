/*******************************************************************/
/*    Copyright (c) 2005-2020 by Spatial Corp.                     */
/*    All rights reserved.                                         */
/*    Protected by U.S. Patents 5,257,205; 5,351,196; 6,369,815;   */
/*                              5,982,378; 6,462,738; 6,941,251    */
/*    Protected by European Patents 0503642; 69220263.3            */
/*    Protected by Hong Kong Patent 1008101A                       */
/*******************************************************************/
// Jeff 01-21-05 Revived a portion of PHL to support legacy data. BTS 78294
#if !defined( PHL_SEGMENT_CLASS )
#define PHL_SEGMENT_CLASS

// =============================================================================
// Includes:
// -----------------------------------------------------------------------------
#include "logical.h"
#include "interval.hxx"

#include "dcl_kern.h"
#include "ent_phl.hxx"
#include "phlentmc.hxx"
/**
* @file phl_seg.hxx
 * @CAA2Level L1
 * @CAA2Usage U1
 * \addtogroup ACISVISUALIZATION
 *
 * @{
 */
/**
 * @nodoc
 */
ENTITY_IS_PROTOTYPE(PHL_SEGMENT, KERN)
#if 0
; // semicolon needed for mkman (doc tool) parsing
#endif
// STI jmb: begin
#ifdef OUT
// This gets #defined in some Windows headers
#undef OUT
#endif
// STI jmb: end

// =============================================================================
// Forwards:
// -----------------------------------------------------------------------------
class BODY;

// =============================================================================
// Defines:
// -----------------------------------------------------------------------------

// This is a derived class of ENTITY_PHL
#define PHL_SEGMENT_LEVEL ( ENTITY_PHL_LEVEL + 1)

// =============================================================================
// Declarations:
// -----------------------------------------------------------------------------

// Identifier for the type of ENTITY
// tbrv
/**
 * @nodoc
 */
extern DECL_KERN int PHL_SEGMENT_TYPE;

// =============================================================================
// Class:	PHL_SEGMENT
// -----------------------------------------------------------------------------

/**
 * @nodoc
 * Defines a line segment with visibility information.
 * <br>
 * <b>Role:</b> A <tt>PHL_SEGMENT</tt> object defines the visibility of an edge.
 * <br><br>
 * An interval (parameter range) indicates which piece of the
 * edge is covered by the segment.
 * <br><br>
 * Status information records whether the segment is an inner or outer segment.
 * <br><br>
 * Visibility information records whether the segment is visible, hidden by
 * a face, or occluded by an edge.
 * <br><br>
 * Segments can be hooked together in doubly-linked lists. One list of segments,
 * showing the outlook of the edge, occurs everywhere an edge's owning body occurs.
 * <br><br>
 * <b>Note:</b> The PHL functionality for which this class was intended is no longer available.
 * This class is provided to support lagacy data. The capabilities of this object
 * have been reduced to allow simple evaluation, save, restore, and deletion.
 * <br><br>
 * @see PHL_EDGE,SPAinterval
 */
class DECL_KERN PHL_SEGMENT : public ENTITY_PHL
{
public:
/**
 * Segment state enumerated type, where <tt>OUT</tt> is an outer segment and <tt>INN</tt> is an inner segment.
 * <br><br>
 * @param OUT
 * outer segment ("silhouette").
 * @param INN
 * inner segment.
 */
	enum	PhlSegSta	{
				OUT,		// outer segment ("silhouette")
				INN			// inner segment
				};
/**
 * Segment visibility enumerated type, where <tt>VIS</tt> is a visible segment, <tt>HID</tt> is a segment hidden by a face, and <tt>OCC</tt> is a segment occluded by an edge.
 * <br><br>
 * @param VIS
 * visible segment.
 * @param HID
 * hidden segment.
 * @param OCC
 * occluded segment.
 * @param UND
 * undefined visibility.
 * @param VIS_SMOOTH
 * smooth but calculated as visible.
 * @param HID_SMOOTH
 * smooth but calculated as hidden.
 */
	enum	PhlSegVis	{
				VIS,		// visible segment
				HID,		// hidden segment
				OCC,		// occluded segment
				UND,		// undefined visibility
				VIS_SMOOTH, // smooth but calculated as visible
				HID_SMOOTH	// smooth but calculated as hidden
				};

protected:
	SPAinterval		intval_data;	// SPAinterval with start and end
									// parameters of segment on its
									// curve
	PhlSegSta		state_data;		// status of segment
	PhlSegVis		visi_data;		// visibility of segment

	// protected members for chain hook/unhook:
	PHL_CHAIN_DEC( PHL_SEGMENT)

	// protected member functions for ACIS:
	/**
	 * @nodoc
	 */
	ENTITY_FUNCTIONS( PHL_SEGMENT, KERN)
#if 0
; // semicolon needed for mkman (doc tool) parsing)
#endif
// Because mkman isn't aware that ENTITY_FUNCTIONS changes access to "public", we do so
// explicitly here:
public:

public:

	// public member functions for chain hook/unhook:
/**
 * @nodoc
 */
PHL_HOOK_DEC( PHL_SEGMENT)
#if 0
	; // semicolon needed for mkman (doc tool) parsing)
#endif
/**
 * @nodoc
 */
PHL_UNHOOK_DEC( PHL_SEGMENT)
#if 0
; // semicolon needed for mkman (doc tool) parsing)
#endif

// =============================================================================
// Public Function:	PHL_SEGMENT	Constructor
// -----------------------------------------------------------------------------
/**
 * @nodoc
 */
PHL_SEGMENT();

// =============================================================================
// Public Function:	PHL_SEGMENT	Constructor
// -----------------------------------------------------------------------------
/**
 * @nodoc
 */
PHL_SEGMENT(
    SPAinterval const & iv,	// I: SPAparameter SPAinterval
    PhlSegSta	st,			// I: state
    PhlSegVis	vi			// I: visibility
  );

// =============================================================================
// Public Function:	intval		Return SPAinterval of this segment
// -----------------------------------------------------------------------------
/**
 * Returns the interval of the segment.
 */
SPAinterval intval() const { return( intval_data ); };

// =============================================================================
// Public Function:	start_pt	Return start SPAparameter of this segment
// -----------------------------------------------------------------------------
/**
 * Returns the start parameter of the segment.
 */
double start_pt() const { return( intval_data.start_pt() ); };

// =============================================================================
// Public Function:	end_pt		Return end SPAparameter of this segment
// -----------------------------------------------------------------------------
/**
 * Returns the end parameter of the segment.
 */
double end_pt() const { return( intval_data.end_pt() ); };

// =============================================================================
// Public Function:	state		Return state of this segment
// -----------------------------------------------------------------------------
/**
 * Returns the state of the segment, where <tt>OUT</tt> means the segment is an outer segment and <tt>INN</tt> means the segment is an inner segment.
 */
PhlSegSta state() const { return( state_data ); };

// =============================================================================
// Public Function:	visibility	Return visibility of this segment
// -----------------------------------------------------------------------------
/**
 * Returns the visibility of the segment, where <tt>VIS</tt> means the segment is visible, <tt>HID</tt> means the segment is hidden by a face, and <tt>OCC</tt> means the segment is occluded by an edge.
 */
PhlSegVis visibility() const { return( visi_data ); };

};

/** @} */
#endif
