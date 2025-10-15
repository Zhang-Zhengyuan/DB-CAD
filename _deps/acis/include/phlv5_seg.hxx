// COPYRIGHT DASSAULT SYSTEMES 1989
#if !defined( PHLV5_SEGMENT_CLASS )
#define PHLV5_SEGMENT_CLASS
#include "logical.h"
#include "interval.hxx"
#include "dcl_phlv5.h"
#include "ent_phlv5.hxx"
#include "phlv5entmc.hxx"
/**
 * @file phlv5_seg.hxx
 * @CAA2Level L1
 * @CAA2Usage U1
 *! \addtogroup PHLV5API
 *  \brief Declared at <phlv5_seg.hxx>
 *  @{
 */
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
/**
 * This is a derived class of ENTITY_PHLV5
 */
#define PHLV5_SEGMENT_LEVEL ( ENTITY_PHLV5_LEVEL + 1)

// =============================================================================
// Declarations:
// -----------------------------------------------------------------------------

// Identifier for the type of ENTITY
extern DECL_PHLV5 int PHLV5_SEGMENT_TYPE;


//
// enums
//

/**
 * Specifies the status information.
 * @param PHLV5_SEGMENT_OUT
 * outer segment (silhouette).
 * @param PHLV5_SEGMENT_INN
 * inner segment.
 **/

enum	Phlv5SegSta	{
				PHLV5_SEGMENT_OUT,		// outer segment ("silhouette")
				PHLV5_SEGMENT_INN			// inner segment
				};

/**
 * Specifies the type of visibility.
 * @param PHLV5_SEGMENT_VIS
 * visible segment.
 * @param PHLV5_SEGMENT_HID
 * hidden segment.
 * @param PHLV5_SEGMENT_OCC
 * occluded segment.
 * @param PHLV5_SEGMENT_UND
 * undefined visibility.
 * @param PHLV5_SEGMENT_VIS_SMOOTH
 * smooth but calculated as visible.
 * @param PHLV5_SEGMENT_HID_SMOOTH
 * smooth but calculated as hidden.
 **/

enum	Phlv5SegVis	{
				PHLV5_SEGMENT_VIS,		// visible segment
				PHLV5_SEGMENT_HID,		// hidden segment
				PHLV5_SEGMENT_OCC,		// occluded segment
				PHLV5_SEGMENT_UND,		// undefined visibility
				PHLV5_SEGMENT_VIS_SMOOTH, // smooth but calculated as visible
				PHLV5_SEGMENT_HID_SMOOTH	// smooth but calculated as hidden
				};

// =============================================================================
// Class:	PHLV5_SEGMENT
// -----------------------------------------------------------------------------

/*
// tbrv
*/
/**
 * @nodoc
 */
class phlv5_segment : public ACIS_OBJECT
{
	public:

	phlv5_segment(){};
	phlv5_segment(
		SPAinterval const& iv,	    // I: SPAparameter SPAinterval
		Phlv5SegSta	st,			// I: state
		Phlv5SegVis	vi			// I: visibility
		) {intval_data = iv; state_data = st;  visi_data = vi; };

	public:
	SPAinterval		intval_data;	// SPAinterval with start and end
									// parameters of segment on its curve
	Phlv5SegSta		state_data;		// status of segment
	Phlv5SegVis		visi_data;		// visibility of segment
};

/**
 * Defines an PHLV5 segment.
 * <br>
 * <b>Role:</b> The <tt>PHLV5_SEGMENT</tt> contains information about each section of the edge,
 * including its visibility and parameter range data.
 * @see PHLV5_EDGE, SPAinterval
 */
class DECL_PHLV5 PHLV5_SEGMENT : public ENTITY_PHLV5
{
protected:
	// Do not doc these data members
	SPAinterval		intval_data;	// SPAinterval with start and end
									// parameters of segment on its
									// curve
	Phlv5SegSta		state_data;		// status of segment
	Phlv5SegVis		visi_data;		// visibility of segment
	logical			persistable;

	// protected members for chain hook/unhook: Don't doc
	PHLV5_CHAIN_DEC( PHLV5_SEGMENT )

	// protected member functions for ACIS: Don't doc

	/*
	// Documentation : To be revisited
	*/
	/**
	 * @nodoc
	 */
	ENTITY_FUNCTIONS( PHLV5_SEGMENT, PHLV5 )
#if 0
; // semicolon needed for mkman (doc tool) parsing)
#endif

public:
	// public member functions for chain hook/unhook: Don't doc
	PHLV5_HOOK_DEC( PHLV5_SEGMENT )
	PHLV5_UNHOOK_DEC( PHLV5_SEGMENT )

	// =============================================================================
	// Public Function:	PHLV5_SEGMENT	Constructor
	// -----------------------------------------------------------------------------
/**
 * C++ allocation constructor requests memory for this object but does not populate it.
 * <br><br>
 * <b>Role:</b> The allocation constructor is used primarily by restore.
 * Applications should call this constructor only with the overloaded new operator,
 * because this reserves the memory on the heap, a requirement to support roll back
 * and history management.
 */
	PHLV5_SEGMENT();

	// =============================================================================
	// Public Function:	PHLV5_SEGMENT	Constructor
	// -----------------------------------------------------------------------------
/**
 * C++ initialize constructor requests memory for this object and populates it with the data supplied as arguments.
 * <br><br>
 * <b>Role:</b> Applications should call this constructor only with the overloaded
 * new operator, because this reserves the memory on the heap, a requirement to
 * support roll back and history management.
 * <br><br>
 * Constructs an instance, initializing the parameter interval, state, and
 * visibility to the given values.
 * <br><br>
 * @param iv
 * parameter interval.
 * @param st
 * state.
 * @param vi
 * visibility.
 */
	PHLV5_SEGMENT(
		SPAinterval const & iv,	// I: SPAparameter SPAinterval
		Phlv5SegSta	st,			// I: state
		Phlv5SegVis	vi,			// I: visibility
		logical     persistable = TRUE
	);

	// Copy constructor
/**
 * C++ copy constructor.
 * <br><br>
 * @param _in_segment
 * segment to copy.
 */
	PHLV5_SEGMENT(phlv5_segment& _in_segment)
	{
		intval_data = _in_segment.intval_data;
		state_data = _in_segment.state_data;
		visi_data = _in_segment.visi_data;
	};

/*
// tbrv
*/
/**
 * @nodoc
 */
    void set_parameter(SPAinterval _in) {intval_data = _in;};

	// =============================================================================
	// Public Function:	intval		Return visibiility SPAinterval of this segment
	// -----------------------------------------------------------------------------
/**
 * Returns the visibility interval of the segment.
 */
	SPAinterval intval() const { return( intval_data ); };

	// =============================================================================
	// Public Function:	start_pt	Return start visibility SPAparameter of this segment
	// -----------------------------------------------------------------------------
/**
 * Returns the start visibility parameter of the segment.
 */
	double start_pt() const { return( intval_data.start_pt() ); };

	// =============================================================================
	// Public Function:	end_pt		Return end visibility SPAparameter of this segment
	// -----------------------------------------------------------------------------
/**
 * Returns the end visibility parameter of the segment.
 */
	double end_pt() const { return( intval_data.end_pt() ); };

	// =============================================================================
	// Public Function:	state		Return state of this segment
	// -----------------------------------------------------------------------------
/**
 * Returns the state of the segment.
 */
	Phlv5SegSta state() const { return( state_data ); };

	// =============================================================================
	// Public Function:	visibility	Return visibility of this segment
	// -----------------------------------------------------------------------------
/**
 * Returns the visibility of the segment.
 */
	Phlv5SegVis visibility() const { return( visi_data ); };
};
/*! @} */
#endif

