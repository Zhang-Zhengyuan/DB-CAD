/* ORIGINAL: acis2.1/kernutil/d3_ext/bxx.hxx */
/* $Id: bxx.hxx,v 1.10 2002/08/09 17:15:28 jeff Exp $ */
/*******************************************************************/
/*    Copyright (c) 1989-2020 by Spatial Corp.                     */
/*    All rights reserved.                                         */
/*    Protected by U.S. Patents 5,257,205; 5,351,196; 6,369,815;   */
/*                              5,982,378; 6,462,738; 6,941,251    */
/*    Protected by European Patents 0503642; 69220263.3            */
/*    Protected by Hong Kong Patent 1008101A                       */
/*******************************************************************/

#ifndef BOXH
#define BOXH
 
#include "dcl_kern.h"
#include "logical.h"
#include "box.hxx"
#include "pos.hxx"


inline	DECL_KERN logical	BOX_null( const SPAbox& b )	
	{ SPAinterval i = b.x_range(); return i.empty() || !i.bounded(); }

extern 	DECL_KERN SPAposition	BOX_nearpoint( const SPAbox& b, const SPAposition& P );

extern 	DECL_KERN double	BOX_distance( const SPAbox& b, const SPAposition& P );

extern 	DECL_KERN SPAposition	BOX_midpoint( const SPAbox& b );

extern DECL_KERN logical BOX_clash( const SPAbox&, const SPAposition&, double epsilon=SPAresabs );

extern DECL_KERN logical BOX_clash( const SPAbox&, const SPAbox&, double epsilon = SPAresabs );


#if defined D3_STANDALONE || defined D3_DEBUG

class D3_ostream;
class D3_istream;

DECL_KERN D3_ostream& operator<<( D3_ostream &s, const SPAbox& b );

DECL_KERN D3_istream& operator>>( D3_istream &s, SPAbox& b );

#ifdef D3_DEBUG

DECL_KERN void	BOX_plot( D3_ostream &gp, const SPAbox& b );

#endif
#endif
#endif
