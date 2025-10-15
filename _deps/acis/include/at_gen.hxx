/* ORIGINAL: acis2.1/ga_husk/attrib/at_gen.hxx */
// $Id: at_gen.hxx,v 1.3 2000/12/26 18:35:52 products Exp $
/*******************************************************************/
/*    Copyright (c) 1989-2020 by Spatial Corp.                     */
/*    All rights reserved.                                         */
/*    Protected by U.S. Patents 5,257,205; 5,351,196; 6,369,815;   */
/*                              5,982,378; 6,462,738; 6,941,251    */
/*    Protected by European Patents 0503642; 69220263.3            */
/*    Protected by Hong Kong Patent 1008101A                       */
/*******************************************************************/
// The Organization class for generic attributes

#if !defined( ATTRIB_GENERIC_CLASS )
#define ATTRIB_GENERIC_CLASS

#include "dcl_ga.h"
#include "attrib.hxx"

/**
* @file at_gen.hxx
 * @CAA2Level L1
 * @CAA2Usage U1
 * \addtogroup GAAPI
 *
 * @{
 */

/**
 * @nodoc
 */
extern DECL_GA int ATTRIB_GENERIC_TYPE;
/**
 * @nodoc
 */
#define ATTRIB_GENERIC_LEVEL (ATTRIB_LEVEL + 1)

/**
 * \class ATTRIB_GENERIC
 * Defines the organization attribute class for the Generic Attributes Component.
 * <br>
 * <b>Role:</b> This is an organization attribute class. Its methods are virtual and do nothing.
 * User is supposed to implement these methods for classes derived from <tt>ATTRIB_GENERIC</tt>.
 * @see ATTRIB
 */
MASTER_ATTRIB_DECL( ATTRIB_GENERIC , GA);
#if 0
; // semicolon needed for mkman (doc tool) parsing)
#endif

 /** @} */
#endif
