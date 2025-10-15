/* ORIGINAL: acis2.1/ihl_husk/attrib/at_ihl.hxx */
// $Id: at_ihl.hxx,v 1.4 2000/12/26 18:41:47 products Exp $
/*******************************************************************/
/*    Copyright (c) 1989-2020 by Spatial Corp.                     */
/*    All rights reserved.                                         */
/*    Protected by U.S. Patents 5,257,205; 5,351,196; 6,369,815;   */
/*                              5,982,378; 6,462,738; 6,941,251    */
/*    Protected by European Patents 0503642; 69220263.3            */
/*    Protected by Hong Kong Patent 1008101A                       */
/*******************************************************************/
#if !defined( ATTRIB_IHL_CLASS )
#define ATTRIB_IHL_CLASS

#include "dcl_ihl.h"
#include "attrib.hxx"

/**
 * @file at_ihl.hxx
 * @CAA2Level L1
 * @CAA2Usage U1
 *! \addtogroup IHLAPI
 *  \brief Declared at <at_ihl.hxx>
 *  @{
 */


/**
 * @nodoc
 */
extern DECL_IHL int ATTRIB_IHL_TYPE;

/**
 * @nodoc
 */
#define ATTRIB_IHL_LEVEL (ATTRIB_LEVEL + 1)

/**
 * \class ATTRIB_IHL
 * Defines the organization attribute class for the Interactive Hidden Line Component (IHL).
 * <br>
 * <b>Role:</b> This is an organization attribute class. Its methods are virtual and do nothing.
 * The user is supposed to implement these methods for classes derived from <tt>ATTRIB_IHL</tt>.
 * @see ATTRIB
 */
MASTER_ATTRIB_DECL( ATTRIB_IHL, IHL );
#if 0
; // semicolon needed for mkman (doc tool) parsing)
#endif

/*! @} */
#endif
