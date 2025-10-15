/* ORIGINAL: acis2.1/kernutil/d3_utl/msc.hxx */
/* $Id: msc.hxx,v 1.14 2001/08/15 15:20:22 jeff Exp $ */
/*******************************************************************/
/*    Copyright (c) 1989-2020 by Spatial Corp.                     */
/*    All rights reserved.                                         */
/*    Protected by U.S. Patents 5,257,205; 5,351,196; 6,369,815;   */
/*                              5,982,378; 6,462,738; 6,941,251    */
/*    Protected by European Patents 0503642; 69220263.3            */
/*    Protected by Hong Kong Patent 1008101A                       */
/*******************************************************************/

#ifndef MSCH
#define MSCH

// This file allows the debugging system to be compiled in or out. 
// If the system is required then it is included from the msc module. 
// If it is not required then the corresponding macros are nullified and no 
// other files are included. Thus, this file either provides a gateway to the 
// msc module, or replaces it. 


// Make sure that NULL is defined. Some acis .h files assume this has 
// already been included. 

#include <stdio.h>

#ifdef D3_STANDALONE

#include "stream.hxx"

#endif

#include "deb.hxx"

#ifdef D3_DEBUG
#include "psf.hxx"
#endif

#endif
