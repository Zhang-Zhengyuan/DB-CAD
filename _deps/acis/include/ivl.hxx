/* ORIGINAL: acis2.1/kernutil/d3_ext/ivl.hxx */
/* $Id: ivl.hxx,v 1.8 2000/12/26 18:47:55 products Exp $ */
/*******************************************************************/
/*    Copyright (c) 1989-2020 by Spatial Corp.                     */
/*    All rights reserved.                                         */
/*    Protected by U.S. Patents 5,257,205; 5,351,196; 6,369,815;   */
/*                              5,982,378; 6,462,738; 6,941,251    */
/*    Protected by European Patents 0503642; 69220263.3            */
/*    Protected by Hong Kong Patent 1008101A                       */
/*******************************************************************/

#ifndef IVLH
#define IVLH

#include "dcl_kern.h"
#include "interval.hxx"

#if defined D3_STANDALONE || defined D3_DEBUG  

class D3_ostream;
class D3_istream;

DECL_KERN D3_ostream& operator<<( D3_ostream &s, const SPAinterval& ivl );

DECL_KERN D3_istream& operator>>( D3_istream &s, SPAinterval& ivl );

#endif
#endif
