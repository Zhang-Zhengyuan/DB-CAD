/*******************************************************************/
/*    Copyright (c) 2005-2020 by Spatial Corp.                     */
/*    All rights reserved.                                         */
/*    Protected by U.S. Patents 5,257,205; 5,351,196; 6,369,815;   */
/*                              5,982,378; 6,462,738; 6,941,251    */
/*    Protected by European Patents 0503642; 69220263.3            */
/*    Protected by Hong Kong Patent 1008101A                       */
/*******************************************************************/
// Jeff 01-21-05 Revived a portion of PHL to support legacy data. BTS 78294
#if !defined( PHL_CAMERA_CLASS )
#define PHL_CAMERA_CLASS

// =============================================================================
// Includes:
// -----------------------------------------------------------------------------
#include "logical.h"
#include "position.hxx"

#include "dcl_kern.h"
#include "usecount.hxx"
#include "ent_phl.hxx"
/**
* @file phl_cam.hxx
 * @CAA2Level L1
 * @CAA2Usage U1
 * \addtogroup ACISVISUALIZATION
 *
 * @{
 */
// =============================================================================
// Forwards:
// -----------------------------------------------------------------------------

// =============================================================================
// Defines:
// -----------------------------------------------------------------------------

// This is a derived class of ENTITY_PHL
#define PHL_CAMERA_LEVEL ( ENTITY_PHL_LEVEL + 1)

// =============================================================================
// Declarations:
// -----------------------------------------------------------------------------

/**
 * @nodoc
 */
ENTITY_IS_PROTOTYPE(PHL_CAMERA, KERN)
#if 0
; // semicolon needed for mkman (doc tool) parsing
#endif
// Identifier for the type of ENTITY
extern DECL_KERN int PHL_CAMERA_TYPE;

// =============================================================================
// Class:	PHL_CAMERA
// -----------------------------------------------------------------------------
/**
 * @nodoc
 * Defines a camera viewpoint against which precise hidden lines are calculated.
 * <br>
 * <b>Role:</b> Each camera is defined by its position (eye position) in global
 * model space, its aim (target position) in global model space, and by a
 * perspective flag. The perspective flag is <tt>TRUE</tt> for a simple perspective
 * projection, where two positions define the view direction and the distance
 * between eye and target. The flag is <tt>FALSE</tt> for a parallel projection, where
 * the eye and target positions define the view direction, and the distance is not relevant.
 * <br><br>
 * The orientation of the camera (i.e., rotation about its own view axis) has no
 * influence on the result of the precise hidden line calculation.
 * <br><br>
 * <b>Note:</b> The PHL functionality for which this class was intended is no longer available.
 * This class is provided to support lagacy data. The capabilities of this object
 * have been reduced to allow simple evaluation, save, restore, and deletion.
 * <br><br>
 * @see ATTRIB_PHL_VW,SPAposition.
 */
class DECL_KERN PHL_CAMERA : public ENTITY_PHL
{

private:

	SPAposition		eyepos_data;	// eye SPAposition
	SPAposition		target_data;	// target SPAposition
	logical			persp_data;		// perspective flag
									//   TRUE:  perspective view
									//   FALSE: orthographic view
protected:

	// protected member functions for ACIS:
	/**
	 * @nodoc
	 */
	ENTITY_FUNCTIONS( PHL_CAMERA, KERN)
#if 0
; // semicolon needed for mkman (doc tool) parsing)
#endif
// Because mkman isn't aware that ENTITY_FUNCTIONS changes access to "public", we do so
// explicitly here:
public:


public:
// =============================================================================
// Public Function:	PHL_CAMERA	Constructor
// -----------------------------------------------------------------------------
/**
* @nodoc
*/
PHL_CAMERA();

// =============================================================================
// Public Function:	PHL_CAMERA	Constructor
// -----------------------------------------------------------------------------
/**
* @nodoc
*/
PHL_CAMERA(
    const SPAposition&	eye,		// I: eye SPAposition
    const SPAposition&	tgt,		// I: target SPAposition
    logical		psp				// I: perspective flag
  );

// =============================================================================
// Public Function:	set_eyepos	Set eye SPAposition
// -----------------------------------------------------------------------------
/**
 * @nodoc
 */
void set_eyepos( const SPAposition& eye );

// =============================================================================
// Public Function:	set_target	Set target SPAposition
// -----------------------------------------------------------------------------
/**
 * @nodoc
 */
void set_target( const SPAposition& tgt );

// =============================================================================
// Public Function:	set_perspective	Set perspective flag
// -----------------------------------------------------------------------------
/**
 * @nodoc
 */
void set_perspective( logical psp );

// =============================================================================
// Public Function:	eyepos		Return eye SPAposition
// -----------------------------------------------------------------------------
/**
 * Returns the eye position.
 */
SPAposition eyepos() const { return( eyepos_data ); };

// =============================================================================
// Public Function:	target		Return target SPAposition
// -----------------------------------------------------------------------------
/**
 * Returns the target position.
 */
SPAposition target() const { return( target_data ); };

// =============================================================================
// Public Function:	perspective	Return perspective flag
// -----------------------------------------------------------------------------
/**
 * Returns the perspective flag.
 * <br><br>
 * <b>Role:</b> <tt>TRUE</tt> indicates a perspective view, and <tt>FALSE</tt> indicates
 * an orthographic view.
 */
logical perspective() const { return( persp_data ); };

// STI jmb begin:  Need to check camera equivalence
// ==============================================================================
// Public Function: operator==  Return nonzero if arg is same as this
// O: True if camera same within SPAresabs
// ------------------------------------------------------------------------------
/**
 * Compares the eye position and target position of the current camera with those of the given camera .
 * <br><br>
 * <b>Role:</b> Returns  <tt>TRUE</tt> if the cameras' positions are the same within <tt>SPAresabs</tt>, and their perspective flags are the same else it returns <tt>FALSE</tt>.
 * <br><br>
 * @param c
 * other camera to check.
 */
logical operator==( PHL_CAMERA const& c) const;

// ==============================================================================
// Public Function: operator!=  Return zero if arg is same as this
// O: False if camera same within SPAresabs
// ------------------------------------------------------------------------------
/**
 * Compares the eye position and target position of the current camera with those of the given camera .
 * <br><br>
 * <b>Role:</b> Returns <tt>FALSE</tt> if the cameras' positions are the same within <tt>SPAresabs</tt>, and their perspective flags are the same else it returns <tt>TRUE</tt>.
 * <br><br>
 * @param c
 * other camera to check.
 */
logical operator!=( PHL_CAMERA const& c ) const { return ( ! ( *this == c ) ); };

/**
 * @nodoc
 */
 USE_COUNTED_DECL
#if 0
; // semicolon needed for mkman (doc tool) parsing)
#endif

};

/** @} */
#endif
