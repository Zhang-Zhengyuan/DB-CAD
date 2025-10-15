/* ORIGINAL: acis2.1/sg_husk/attrib/at_sg.hxx */
/* $Id: at_sg.hxx,v 1.7 2002/08/09 17:15:34 jeff Exp $ */
/*******************************************************************/
/*    Copyright (c) 1989-2020 by Spatial Corp.                     */
/*    All rights reserved.                                         */
/*    Protected by U.S. Patents 5,257,205; 5,351,196; 6,369,815;   */
/*                              5,982,378; 6,462,738; 6,941,251    */
/*    Protected by European Patents 0503642; 69220263.3            */
/*    Protected by Hong Kong Patent 1008101A                       */
/*******************************************************************/

// This attribute is designed for use with husks

#if !defined( ATTRIB_SG_CLASS )
#define ATTRIB_SG_CLASS

#include "dcl_kern.h"
#include "attrib.hxx"

// This is a derived class of ATTRIB

extern DECL_KERN int ATTRIB_SG_TYPE;
#define ATTRIB_SG_LEVEL (ATTRIB_LEVEL + 1)

/*
// The ATTRIB_SG class is not doc'ed in R10, so it will
// not be doc'ed in R11.
*/
/**
 * @nodoc
 */
MASTER_ATTRIB_DECL( ATTRIB_SG , KERN);
#if 0
; // semicolon needed for mkman (doc tool) parsing)
#endif

#endif
