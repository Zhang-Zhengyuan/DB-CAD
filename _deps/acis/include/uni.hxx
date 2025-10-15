/* ORIGINAL: acis2.1/kernutil/d3_ext/uni.hxx */
/* $Id: uni.hxx,v 1.9 2002/08/09 17:15:28 jeff Exp $ */
/*******************************************************************/
/*    Copyright (c) 1989-2020 by Spatial Corp.                     */
/*    All rights reserved.                                         */
/*    Protected by U.S. Patents 5,257,205; 5,351,196; 6,369,815;   */
/*                              5,982,378; 6,462,738; 6,941,251    */
/*    Protected by European Patents 0503642; 69220263.3            */
/*    Protected by Hong Kong Patent 1008101A                       */
/*******************************************************************/

#ifndef UNIH
#define UNIH
 
#include "dcl_kern.h"
#include "vector.hxx"
#include "unitvec.hxx"
#include "res.hxx"
#include "math.hxx"


#if defined D3_STANDALONE || defined D3_DEBUG

#include "stream.hxx"

DECL_KERN D3_ostream& operator<<( D3_ostream &s, const SPAunit_vector& U );
DECL_KERN D3_istream& operator>>( D3_istream &s, SPAunit_vector& U );

#endif

// Parallel and orthogonal tests test the angle between the vectors against
// SPAresnor by default. 

inline DECL_KERN logical VEC_parallel( const SPAvector& U, const SPAunit_vector& V, 
			     const double res = SPAresnor )
    { SPAvector W=U*V; return W%W < res * res * (U%U); }

inline DECL_KERN logical VEC_parallel( const SPAunit_vector& U, const SPAvector& V, 
			    const double res = SPAresnor )
    { SPAvector W=U*V; return W%W < res * res * (V%V); }

inline DECL_KERN logical VEC_parallel( const SPAunit_vector& U, const SPAunit_vector& V,
			     const double res = SPAresnor )
    { SPAvector W=U*V; return W%W < res * res; }


inline DECL_KERN logical VEC_orthogonal( const SPAvector& U, const SPAunit_vector& V,
			       const double res = SPAresnor )
    { double temp = U%V; return temp*temp < res * res * (U%U); }

inline DECL_KERN logical VEC_orthogonal( const SPAunit_vector& U, const SPAvector& V,
			       const double res = SPAresnor )
    { double temp = U%V; return temp*temp < res * res * (V%V); }

inline DECL_KERN logical VEC_orthogonal( const SPAunit_vector& U, const SPAunit_vector& V,
			       const double res = SPAresnor )
    { double temp = U%V; return temp*temp < res * res; }

#endif
