/* ORIGINAL: acis2.1/kernutil/d3_ext/vec.hxx */
/* $Id: vec.hxx,v 1.9 2002/08/09 17:15:28 jeff Exp $ */
/*******************************************************************/
/*    Copyright (c) 1989-2020 by Spatial Corp.                     */
/*    All rights reserved.                                         */
/*    Protected by U.S. Patents 5,257,205; 5,351,196; 6,369,815;   */
/*                              5,982,378; 6,462,738; 6,941,251    */
/*    Protected by European Patents 0503642; 69220263.3            */
/*    Protected by Hong Kong Patent 1008101A                       */
/*******************************************************************/

#ifndef VECH
#define VECH
 
#include "dcl_kern.h"
#include "vector.hxx"
#include "res.hxx"
#include "math.hxx"

#if defined D3_STANDALONE || defined D3_DEBUG

#include "stream.hxx"

DECL_KERN D3_ostream& operator<<( D3_ostream &s, const SPAvector& U );
DECL_KERN D3_istream& operator>>( D3_istream &s, SPAvector& U );

#endif

// A note on resolution: the tests for SPAvector equality and zero vectors test the
// length of the SPAvector against SPAresabs by default; the tests on parallelism or
// orthogonality test the angle between the vectors against SPAresnor by default. 
// These defaults can be changed by supplying the optional 2nd or 3rd argument,
// but note that for the parallel and orthogonal tests this must be a small (<<
// unity) number. 

inline DECL_KERN logical VEC_zero( const SPAvector& V, const double eps = SPAresabs ) 
    { 
    return V%V <= eps * eps;
    }

inline DECL_KERN logical VEC_equal( const SPAvector& U, const SPAvector& V, 
			  const double eps = SPAresabs )
    {
    return VEC_zero( U - V, eps ); 
    } 

extern DECL_KERN double VEC_angle( const SPAvector& U, const SPAvector& V );

extern DECL_KERN double VEC_acute_angle( const SPAvector& U, const SPAvector& V );

extern DECL_KERN logical VEC_orthogonal( const SPAvector& U, const SPAvector& V, 
			       const double res = SPAresnor );

extern DECL_KERN logical VEC_parallel( const SPAvector& U, const SPAvector& V, 
			     const double res = SPAresnor );


#endif
