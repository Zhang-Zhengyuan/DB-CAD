/* ORIGINAL: acis2.1/kerndata/data/checks.hxx */
/* $Id: checks.hxx,v 1.7 2000/12/26 18:46:46 products Exp $ */
/*******************************************************************/
/*    Copyright (c) 1989-2020 by Spatial Corp.                     */
/*    All rights reserved.                                         */
/*    Protected by U.S. Patents 5,257,205; 5,351,196; 6,369,815;   */
/*                              5,982,378; 6,462,738; 6,941,251    */
/*    Protected by European Patents 0503642; 69220263.3            */
/*    Protected by Hong Kong Patent 1008101A                       */
/*******************************************************************/

// Header for entity checking routines

#include "dcl_kern.h"
#include "logical.h"

// STI dgh 
class ENTITY;

DECL_KERN void check_valid( ENTITY * );
		// Checks that the entity has a bulletin-board record.
		// Used before any change is made to the record. Formerly
		// complained if there was no bulletin: now just silently
		// constructs one.
