/*******************************************************************/
/*    Copyright (c) 1989-2020 by Spatial Corp.                     */
/*    All rights reserved.                                         */
/*    Protected by U.S. Patents 5,257,205; 5,351,196; 6,369,815;   */
/*                              5,982,378; 6,462,738; 6,941,251    */
/*    Protected by European Patents 0503642; 69220263.3            */
/*    Protected by Hong Kong Patent 1008101A                       */
/*******************************************************************/

#ifndef FACET_BODY_HXX
#define FACET_BODY_HXX

//======================================================================

#include "dcl_kern.h"
#include "entity.hxx"
#include "api.hxx"

/**
* @file apolybody.hxx
 * @CAA2Level L1
 * @CAA2Usage U1
 *! \addtogroup KERNAPI
 *
 * @{
 */

// Identifier used to find out (via identity() defined below) to what
// an entity pointer refers.

extern DECL_KERN int FACET_BODY_TYPE;

// Identifier that gives number of levels of derivation of this class
// from ENTITY.

#define FACET_BODY_LEVEL 1

/**
 * @nodoc
 */
ENTITY_IS_PROTOTYPE( FACET_BODY, KERN )
#if 0
; // semicolon needed for mkman (doc tool) parsing
#endif


/**
* @nodoc
*/
struct FACET_BODY_data;
#include "SPA_raw_mesh.hxx"

/**
 * @nodoc
 */
class DECL_KERN FACET_BODY : public ENTITY 
{
	// allows internal clients to declare themselves friend of FACET_BODY
#ifdef FACET_BODY_FRIENDS
	FACET_BODY_FRIENDS
#endif
	unsigned int _version;
	FACET_BODY_data* _data;
public:

	FACET_BODY();

	/**
	 * @nodoc
	 */
	ENTITY_FUNCTIONS( FACET_BODY, KERN )
#if 0
; // semicolon needed for mkman (doc tool) parsing)
#endif

	int num_faces() const;
	Spa_raw_mesh const& get_face_mesh(int which_face) const;

	FACET_BODY& operator=(FACET_BODY const& other);
};

/*! @} */
#endif

