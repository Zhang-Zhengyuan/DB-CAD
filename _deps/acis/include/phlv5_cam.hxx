// COPYRIGHT DASSAULT SYSTEMES 1989

#if !defined( PHLV5_CAMERA_CLASS )
#define PHLV5_CAMERA_CLASS

#include "logical.h"
#include "position.hxx"

#include "dcl_phlv5.h"
#include "ent_phlv5.hxx"

// This is a derived class of ENTITY_PHLV5
#define PHLV5_CAMERA_LEVEL ( ENTITY_PHLV5_LEVEL + 1)

// =============================================================================
// Declarations:
// -----------------------------------------------------------------------------

// Identifier for the type of ENTITY
extern DECL_PHLV5 int PHLV5_CAMERA_TYPE;

// =============================================================================
// Class:	PHLV5_CAMERA
// -----------------------------------------------------------------------------

class DECL_PHLV5 PHLV5_CAMERA : public ENTITY_PHLV5
{

public:

protected:
	SPAposition		eyepos_data;	// eye SPAposition
	SPAposition		target_data;	// target SPAposition
	logical			persp_data;		// perspective flag
									// TRUE:  perspective view
									// FALSE: orthographic view

	// protected member functions for ACIS:
	/**
	 * @nodoc
	 */
	ENTITY_FUNCTIONS( PHLV5_CAMERA, PHLV5)
#if 0
; // semicolon needed for mkman (doc tool) parsing)
#endif

public:
// =============================================================================
// Public Function:	PHLV5_CAMERA	Constructor
// -----------------------------------------------------------------------------
PHLV5_CAMERA();

// =============================================================================
// Public Function:	PHLV5_CAMERA	Constructor
// -----------------------------------------------------------------------------
PHLV5_CAMERA(
    const SPAposition&	eye,		// I: eye SPAposition
    const SPAposition&	tgt,		// I: target SPAposition
    logical		psp				// I: perspective flag
  );

// =============================================================================
// Public Function:	set_eyepos	Set eye SPAposition
// -----------------------------------------------------------------------------
void set_eyepos( const SPAposition& eye );

// =============================================================================
// Public Function:	set_target	Set target SPAposition
// -----------------------------------------------------------------------------
void set_target( const SPAposition& tgt );

// =============================================================================
// Public Function:	set_perspective	Set perspective flag
// -----------------------------------------------------------------------------
void set_perspective( logical psp );

// =============================================================================
// Public Function:	eyepos		Return eye SPAposition
// -----------------------------------------------------------------------------
SPAposition eyepos() const { return( eyepos_data ); };

// =============================================================================
// Public Function:	target		Return target SPAposition
// -----------------------------------------------------------------------------
SPAposition target() const { return( target_data ); };

// =============================================================================
// Public Function:	perspective	Return perspective flag
// -----------------------------------------------------------------------------
logical perspective() const { return( persp_data ); };

// STI jmb begin:  Need to check camera equivalence
// ==============================================================================
// Public Function: operator==  Return nonzero if arg is same as this
// O: True if camera same within SPAresabs
// ------------------------------------------------------------------------------
logical operator==( PHLV5_CAMERA const& ) const;

// ==============================================================================
// Public Function: operator!=  Return zero if arg is same as this
// O: False if camera same within SPAresabs
// ------------------------------------------------------------------------------
logical operator!=( PHLV5_CAMERA const& c ) const { return ( ! ( *this == c ) ); };

}; 

#endif
