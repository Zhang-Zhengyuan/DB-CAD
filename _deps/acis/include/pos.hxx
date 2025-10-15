/* ORIGINAL: acis2.1/kernutil/d3_ext/pos.hxx */
/* $Id: pos.hxx,v 1.8 2002/08/09 17:15:28 jeff Exp $ */
/*******************************************************************/
/*    Copyright (c) 1989-2020 by Spatial Corp.                     */
/*    All rights reserved.                                         */
/*    Protected by U.S. Patents 5,257,205; 5,351,196; 6,369,815;   */
/*                              5,982,378; 6,462,738; 6,941,251    */
/*    Protected by European Patents 0503642; 69220263.3            */
/*    Protected by Hong Kong Patent 1008101A                       */
/*******************************************************************/

#ifndef POSH
#define POSH
 
#include "dcl_kern.h"
#include "position.hxx"
#include "vec.hxx"


inline DECL_KERN SPAposition POS_mid_point( const SPAposition& P, const SPAposition& Q )
    { return SPAposition( 
	0.5*( P.x()+Q.x() ), 0.5*( P.y()+Q.y() ), 0.5*( P.z()+Q.z() ) ); }

inline DECL_KERN logical VEC_equal( const SPAposition& P, const SPAposition& Q, const double eps = SPAresabs ) 
    { SPAvector W = P-Q; return VEC_zero( W, eps ); }


#if defined D3_STANDALONE || defined D3_DEBUG

class D3_ostream;
class D3_istream;

extern	DECL_KERN D3_ostream& operator<<( D3_ostream &s, const SPAposition& P );
extern	DECL_KERN D3_istream& operator>>( D3_istream &s, SPAposition& P );

#endif
#endif
