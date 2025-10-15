/*******************************************************************/
/*    Copyright (c) 1989-2020 by Spatial Corp.                     */
/*    All rights reserved.                                         */
/*    Protected by U.S. Patents 5,257,205; 5,351,196; 6,369,815;   */
/*                              5,982,378; 6,462,738; 6,941,251    */
/*    Protected by European Patents 0503642; 69220263.3            */
/*    Protected by Hong Kong Patent 1008101A                       */
/*******************************************************************/
/**********************************************************************/
/* Defines the basic class for all other IHL entity classes.          */
/**********************************************************************/
#if !defined ( EN_IHL_INCLUDE )
#define EN_IHL_INCLUDE

#include "dcl_kern.h"
#include "logical.h"
#include "en_macro.hxx"
#include "dcl_ihl.h"

/**
 * @file en_ihl.hxx
 * @CAA2Level L1
 * @CAA2Usage U2
 *! \addtogroup IHLAPI
 *  \brief Declared at <en_ihl.hxx>
 *  @{
 */

extern DECL_IHL int ENTITY_IHL_TYPE;
/**
 * Identifier that gives number of levels of derivation of the ENTITY_IHL class
 * from ENTITY.
 */
#define ENTITY_IHL_LEVEL (ENTITY_LEVEL + 1)

/**
 * \class ENTITY_IHL
 * Defines organizational class for all IHL entity classes.
 * <br>
 * <b>Role:</b> All classes representing permanent objects in the IHL model (except attributes)
 * are derived from the base class <tt>ENTITY_IHL</tt>, which is derived from the <tt>ENTITY</tt> class.
 * <br><br>
 * The <tt>ENTITY_IHL</tt> class does not represent any specific object within the modeler, but rather
 * allows consistent performance of operations, such as system debugging, change records (bulletin
 * board), roll back, attributes, and model archiving and communication, which are generic
 * methods of the <tt>ENTITY</tt> class.
 * @see ENTITY
 */
MASTER_ENTITY_DECL( ENTITY_IHL, IHL );
#if 0
; // semicolon needed for mkman (doc tool) parsing)
#endif

/* @} */
#endif
