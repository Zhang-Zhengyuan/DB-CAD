/* ORIGINAL: acis2.1/kernutil/d3_utl/res.hxx */
/* $Id: res.hxx,v 1.9 2002/08/09 17:15:31 jeff Exp $ */
/*******************************************************************/
/*    Copyright (c) 1989-2020 by Spatial Corp.                     */
/*    All rights reserved.                                         */
/*    Protected by U.S. Patents 5,257,205; 5,351,196; 6,369,815;   */
/*                              5,982,378; 6,462,738; 6,941,251    */
/*    Protected by European Patents 0503642; 69220263.3            */
/*    Protected by Hong Kong Patent 1008101A                       */
/*******************************************************************/

#ifndef RESH
#define RESH

#include "acis.hxx"  
#include "dcl_kern.h"
#include "logical.h"

//extern	DECL_KERN double SPAresabs;
//extern	DECL_KERN double SPAresnor;


inline logical RES_zero( double a, double const epsilon = SPAresabs ) 
    { return -epsilon <= a && a <= epsilon; }

inline logical RES_equal( double a, double b, double const epsilon = SPAresabs ) { return RES_zero( a-b, epsilon ); }

inline logical RES_significant( double a ) 
    { return a < -SPAresnor || a > SPAresnor; }

inline logical RES_significant( double a, double b ) 
    { double c = b<0 ? -b*SPAresnor : b*SPAresnor; return a < -c || a > c; }

#endif
