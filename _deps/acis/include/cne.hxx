/* ORIGINAL: acis2.1/kernutil/d3_utl/cne.hxx */
/* $Id: cne.hxx,v 1.9 2002/08/09 17:15:31 jeff Exp $ */
/*******************************************************************/
/*    Copyright (c) 1989-2020 by Spatial Corp.                     */
/*    All rights reserved.                                         */
/*    Protected by U.S. Patents 5,257,205; 5,351,196; 6,369,815;   */
/*                              5,982,378; 6,462,738; 6,941,251    */
/*    Protected by European Patents 0503642; 69220263.3            */
/*    Protected by Hong Kong Patent 1008101A                       */
/*******************************************************************/

#ifndef CNEH
#define CNEH

#include "dcl_kern.h"
#include "math.hxx"
#include "uni.hxx"

#if defined D3_STANDALONE || defined D3_DEBUG

class D3_ostream;
class D3_istream;

#endif

// A BOUNDING_CONE contains information about the range of normal directions for
// a surface (i.e. a normal cone) or the tangent directions of a curve (i.e. a
// tangent cone )

struct DECL_KERN BOUNDING_CONE
    {
    SPAunit_vector _Axis;                  // Cone axis
    double      _lower_angle;           // Cone half angle, lower bound
    double      _upper_angle;           // Cone half angle, upper bound
    
    BOUNDING_CONE()     :  _lower_angle(SPAnull), _upper_angle(SPAnull)       {}
    
#if defined D3_STANDALONE || defined D3_DEBUG
    
    void        print( D3_ostream& os, int level )      const;
    void        input( D3_istream& is );
    
#endif
    };

extern DECL_KERN BOUNDING_CONE merge_bounding_cones( 
						     BOUNDING_CONE const &c1,
						     BOUNDING_CONE const &c2,
						     logical           lower );

#if defined D3_STANDALONE || defined D3_DEBUG

extern  DECL_KERN D3_ostream& operator<<( D3_ostream& os, const BOUNDING_CONE& cone );
DECL_KERN D3_istream& operator>>( D3_istream& is, BOUNDING_CONE& cone );

#endif
#endif
