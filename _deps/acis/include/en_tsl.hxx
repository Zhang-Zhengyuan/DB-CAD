/* ORIGINAL: acis2.1/kerndata/data/en_tsl.hxx */
/* $Id: en_tsl.hxx,v 1.7 2000/12/26 18:46:46 products Exp $ */
/*******************************************************************/
/*    Copyright (c) 1989-2020 by Spatial Corp.                     */
/*    All rights reserved.                                         */
/*    Protected by U.S. Patents 5,257,205; 5,351,196; 6,369,815;   */
/*                              5,982,378; 6,462,738; 6,941,251    */
/*    Protected by European Patents 0503642; 69220263.3            */
/*    Protected by Hong Kong Patent 1008101A                       */
/*******************************************************************/

// Header for Three-Space master entity.

// ENTITY_TSL is the common class for all Three-Space private data
// structure entity definitions.


#if !defined( ENTITY_TSL_CLASS )

#include "dcl_kern.h"
#include "logical.h"

#include "en_macro.hxx"


// Identifier used to find out (via identity() defined below) to what 
// an entity pointer refers.

extern DECL_KERN int ENTITY_TSL_TYPE;

           
// Identifier that gives number of levels of derivation of this class
// from ENTITY.

#define ENTITY_TSL_LEVEL (ENTITY_LEVEL + 1)

// The TSL master data structure entity, of which all its private
// specific types are subclasses.

/**
 * @nodoc
 */
MASTER_ENTITY_DECL( ENTITY_TSL, KERN );
#if 0
; // semicolon needed for mkman (doc tool) parsing)
#endif

#define ENTITY_CLASS
#endif
