/*******************************************************************/
/*    Copyright (c) 1989-2020 by Spatial Corp.                     */
/*    All rights reserved.                                         */
/*    Protected by U.S. Patents 5,257,205; 5,351,196; 6,369,815;   */
/*                              5,982,378; 6,462,738; 6,941,251    */
/*    Protected by European Patents 0503642; 69220263.3            */
/*    Protected by Hong Kong Patent 1008101A                       */
/*******************************************************************/
#if !defined(_ATTEYE3D) && !defined(ATTRIB_EYE_CLASS)
#define _ATTEYE3D

#include "dcl_fct.h"
#include "attrib.hxx"

/**
 * @file atteye3d.hxx
 * @CAA2Level L1
 * @CAA2Usage U1
 *! \addtogroup FCTAPI
 *  \brief Declared at <atteye3d.hxx>
 *  @{
 */

/**
 * @nodoc
 */
extern DECL_FCT int ATTRIB_EYE_TYPE;

/**
 * @nodoc
 */
#define ATTRIB_EYE_LEVEL (ATTRIB_LEVEL + 1)

/**
 * \class ATTRIB_EYE
 * Defines the organization attribute class for the Faceter Component.
 * <br>
 * <b>Role:</b> This is an organization attribute class. Its methods are virtual and do nothing.
 * User is supposed to implement these methods for classes derived from <tt>ATTRIB_EYE</tt>.
 * @see ATTRIB
 */
MASTER_ATTRIB_DECL( ATTRIB_EYE, FCT );
#if 0
; // semicolon needed for mkman (doc tool) parsing)
#endif

/* @} */
#endif
