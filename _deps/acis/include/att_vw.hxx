/*******************************************************************/
/*    Copyright (c) 2005-2020 by Spatial Corp.                     */
/*    All rights reserved.                                         */
/*    Protected by U.S. Patents 5,257,205; 5,351,196; 6,369,815;   */
/*                              5,982,378; 6,462,738; 6,941,251    */
/*    Protected by European Patents 0503642; 69220263.3            */
/*    Protected by Hong Kong Patent 1008101A                       */
/*******************************************************************/
// Jeff 01-21-05 Revived a portion of PHL to support legacy data. BTS 78294
#if !defined( ATTRIB_PHL_VW_CLASS )
#define ATTRIB_PHL_VW_CLASS

// =============================================================================
// Includes:
// -----------------------------------------------------------------------------
#include <stdio.h>

#include "entity.hxx"

#include "dcl_kern.h"
#include "usecount.hxx"
#include "att_phl.hxx"

// =============================================================================
// Forwards:
// -----------------------------------------------------------------------------

class PHL_CAMERA;
class PHL_EDGE;
class ENTITY;
class BODY;

// =============================================================================
// Defines:
// -----------------------------------------------------------------------------

/**
 * @nodoc
 */
ENTITY_IS_PROTOTYPE(ATTRIB_PHL_VW, KERN)
#if 0
; // semicolon needed for mkman (doc tool) parsing
#endif

// This is a derived class of ATTRIB_PHL
/**
 * @nodoc
 */
#define ATTRIB_PHL_VW_LEVEL ( ATTRIB_PHL_LEVEL + 1)

// =============================================================================
// Declarations:
// -----------------------------------------------------------------------------

// Identifier for the type of ATTRIB
// tbrv
/**
 * @nodoc
 */
extern DECL_KERN int ATTRIB_PHL_VW_TYPE;

// =============================================================================
// Class:	ATTRIB_PHL_VW
// -----------------------------------------------------------------------------
/**
 * @nodoc
 * Attaches precise hidden line data and viewing parameters to bodies.
 * <br>
 * <b>Role:</b> The <tt>ATTRIB_PHL_VW</tt> class attaches to bodies precise hidden line data
 * and viewing parameters in effect at the time of hidden line calculation.
 * <br><br>
 * The precise hidden line data is stored as a doubly-linked list of
 * <tt>PHL_EDGEs</tt> and a camera definition.
 * <br><br>
 * A view token is also stored to distinguish attributes from others of the same type.
 * Multiple sets of hidden line data may exist by attaching more than one <tt>ATTRIB_PHL_VW</tt>.
 * This attribute does not attempt to detect changes to the model that could invalidate
 * the precise hidden line data. It is the user's responsibility to maintain validity of data.
 * <br><br>
 * <b>Note:</b> The PHL functionality for which this class was intended is no longer available.
 * This class is provided to support lagacy data. The capabilities of this object
 * have been reduced to allow simple evaluation, save, restore, and deletion.
 * <br><br>
 * @see PHL_CAMERA, PHL_EDGE.
 */
class DECL_KERN ATTRIB_PHL_VW: public ATTRIB_PHL
{

private:

	PHL_CAMERA*	camera_ptr;	    // Camera
	PHL_EDGE*   edge_list_ptr;  // Linked list of PHL_EDGEs
	int         view_token;     // Used to identify this attribute.

public:

	/**
	 * @nodoc
	 */
	ATTRIB_FUNCTIONS( ATTRIB_PHL_VW, KERN)
#if 0
; // semicolon needed for mkman (doc tool) parsing)
#endif
// Because mkman isn't aware that ATTRIB_FUNCTIONS changes access to "public", we do so
// explicitly here:
public:

/**
 * @nodoc
 */
logical duplicatable() const;
/**
 * @nodoc
 */
logical savable() const;

// =============================================================================
// Public Function: constructor
// -----------------------------------------------------------------------------
/**
 * @nodoc
 */
ATTRIB_PHL_VW(
      ENTITY*        by = 0,    // I: body
      PHL_CAMERA*    cm = 0,    // I: camera
      PHL_EDGE*      ed = 0,    // I: edge_list
      int            tk = 0     // I: view token
   );

// =============================================================================
// Public Function:	camera		Return camera
// -----------------------------------------------------------------------------
/**
 * Returns the pointer to the <tt>PHL_CAMERA</tt>.
 */
PHL_CAMERA*	camera() const { return( camera_ptr ); };

// =============================================================================
// Public Function: edge_list   Return edge_list
// -----------------------------------------------------------------------------
/**
 * Returns a pointer to the list of <tt>PHL_EDGES</tt>.
 */
PHL_EDGE* edge_list() const { return( edge_list_ptr ); };

// =============================================================================
// Public Function: view        Return view_token
// -----------------------------------------------------------------------------
/**
 * Returns the view token.
 */
int view() const { return( view_token ); };

// =============================================================================
// Public Function:	clean		Remove owning body from scene
// -----------------------------------------------------------------------------
/**
* Deletes all of the PHL data associated with this attribute.
*<br><br>
* <b>Role:</b> This method looses all of the associated PHL entities (PHL_CAMERA,PHL_EDGE). 
* This is also called when the attribute is lost, which is the preferd method to remove
* legacy PHL data from bodies.
*/
void clean();

// =============================================================================
// Public Function: add_edge    Add one PHL_EDGE to the edge_list
// -----------------------------------------------------------------------------
/**
 * @nodoc
 */
void add_edge( PHL_EDGE* ed );

protected:

// =============================================================================
// Protected Function:	set_camera	Set a new camera pointer
// -----------------------------------------------------------------------------
void set_camera( PHL_CAMERA* cm );

// =============================================================================
// Protected. Function:	set_edge_list	return ptr to first PHL_EDGE in list
// -----------------------------------------------------------------------------
void set_edge_list( PHL_EDGE* ed );

// =============================================================================
// Protected. Function:	set_view	Set a new view_token
// -----------------------------------------------------------------------------
void set_view( int tk );

public:

// =============================================================================
// Public Function:	attrib		Return ATTRIB_PHL_VW of entity
// O: pointer to ATTRIB_PHL_VW NULL if no matching ATTRIB_PHL_VW attached
// -----------------------------------------------------------------------------
/**
* Returns the <tt>ATTRIB_PHL_VW</tt> of an <tt>ENTITY</tt> corresponding to the given view token, or <tt>0</tt> if the <tt>ENTITY</tt> has no matching <tt>ATTRIB_PHL_VW</tt> attached.
* <br><br>
* @param by
* owning ENTITY.
* @param tk
* view token.
*/
static ATTRIB_PHL_VW* attrib( const ENTITY*	by, int tk );

/**
 * @nodoc
 */
 USE_COUNT_REFERENCE_DECL
#if 0
; // semicolon needed for mkman (doc tool) parsing)
#endif


};

#endif
