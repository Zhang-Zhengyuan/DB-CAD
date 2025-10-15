/* ORIGINAL: acis2.1/kernutil/version/version.hxx */
/* $Id: version.hxx,v 1.17 2001/12/11 18:52:04 ktatters Exp $ */
/*******************************************************************/
/*    Copyright (c) 1989-2020 by Spatial Corp.                     */
/*    All rights reserved.                                         */
/*    Protected by U.S. Patents 5,257,205; 5,351,196; 6,369,815;   */
/*                              5,982,378; 6,462,738; 6,941,251    */
/*    Protected by European Patents 0503642; 69220263.3            */
/*    Protected by Hong Kong Patent 1008101A                       */
/*******************************************************************/

/* Routines for version identification. */

#if !defined( VERSION_HDR_DEF )
#define  VERSION_HDR_DEF

#include <stdio.h>
#include "dcl_kern.h"
#include "vers.hxx"

DECL_KERN char const *get_version_qualifier();
DECL_KERN char const *get_date_stamp();
DECL_KERN char const * const *get_copyright();
DECL_KERN char const * const *get_version_specs();
DECL_KERN char const * const *get_attribution();
DECL_KERN char const * const *get_story();

DECL_KERN void print_version( FILE * );
DECL_KERN void print_version_specs( FILE * );
DECL_KERN void print_copyright( FILE * );
DECL_KERN void print_attribution( FILE * );
DECL_KERN void print_story( FILE * );

#endif

