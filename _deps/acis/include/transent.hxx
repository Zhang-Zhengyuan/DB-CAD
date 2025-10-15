/* ORIGINAL: acis2.1/kerndata/transent/transent.hxx */
/* $Id: transent.hxx,v 1.10 2002/08/09 17:15:21 jeff Exp $ */
/*******************************************************************/
/*    Copyright (c) 1989-2020 by Spatial Corp.                     */
/*    All rights reserved.                                         */
/*    Protected by U.S. Patents 5,257,205; 5,351,196; 6,369,815;   */
/*                              5,982,378; 6,462,738; 6,941,251    */
/*    Protected by European Patents 0503642; 69220263.3            */
/*    Protected by Hong Kong Patent 1008101A                       */
/*******************************************************************/

// Header for routines for changing the coordinate systems of bodies.

// Each body contains a transformation matrix giving
// the relationship between the internal coordinate system and
// that of the world. These routines allow either the relationship to
// be changed by compounding the stored transformation and the given
// transformation, or the stored transformation to be changed to the
// given transformation while keeping the relationship unchanged.

// They are primarily used when combining two bodies together, to 
// bring the tool body into the coordinate system of the blank.


#if !defined( TRANSENT_HDR_DEF )
#define  TRANSENT_HDR_DEF

#include "dcl_kern.h"
#include "logical.h"

class ENTITY;
class BODY;

class TRANSFORM;
class SPAtransf;

/**
* @file transent.hxx
 * @CAA2Level L1
 * @CAA2Usage U2
 * \addtogroup ACISTRANSFORM
 *
 * @{
 */

/**
 * @nodoc
 * Apply a transformation to an entity (currently only a body),
 * compounding it with whatever is currently there.  If the given
 * transformation is <tt>NULL</tt>, the existing transformation is 
 * simply discarded.
 */

DECL_KERN logical transform_entity( ENTITY *, SPAtransf const & );
DECL_KERN void transform_body( BODY *, SPAtransf const & );

/**
 * @nodoc
 * Change an entity's transform, without changing the shape or
 * position of the entity itself in object space (i.e., modify the 
 * local space geometry of the body).  In the case of a body, it can
 * be negated at the same time by setting the logical to <tt>TRUE</tt>.
 */

DECL_KERN logical change_entity_trans( ENTITY *, TRANSFORM *, logical = FALSE );
DECL_KERN void change_body_trans( BODY *, TRANSFORM *, logical = FALSE );

/**
 * Returns the transformation required to transform the given body's geometry
 * into the coordinate system defined by a given transformation. This function
 * calculates and returns the product of the body's existing transformation with 
 * the inverse of the given transformation. Returns either a reference to the
 * supplied <tt>workspace_trans</tt> (suitably initialized) or, if both 
 * transformations are null or identical, returns a null reference. This function 
 * is primary used by Boolean operations to transform the geometry of one body 
 * into the coordinate system of the other body.
 * <br>
 * @param this_body
 * the given body
 * @param new_transform
 * the given transform
 * @param workspace_trans
 * provides working memory space for relative_body_trans
 */

DECL_KERN SPAtransf const & relative_body_trans( 
	BODY const& this_body, 
	TRANSFORM const* new_transform, 
	SPAtransf& workspace_trans 
	);

/** @} */
#endif
