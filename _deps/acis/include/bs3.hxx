/* ORIGINAL: acis2.1/kerngeom/d3_bs3/bs3.hxx */
/* $Id: bs3.hxx,v 1.8 2000/12/26 18:47:15 products Exp $ */
/*******************************************************************/
/*    Copyright (c) 1989-2020 by Spatial Corp.                     */
/*    All rights reserved.                                         */
/*    Protected by U.S. Patents 5,257,205; 5,351,196; 6,369,815;   */
/*                              5,982,378; 6,462,738; 6,941,251    */
/*    Protected by European Patents 0503642; 69220263.3            */
/*    Protected by Hong Kong Patent 1008101A                       */
/*******************************************************************/

#ifndef BS3H
#define BS3H

// STI jmb:  This file should be moved to ag, since 
//           all the declared functions appear to be 
//           implemented there.

#include "dcl_spl.h"
#include "sp3crtn.hxx"
#include "bs3curve.hxx"
#include "bs2curve.hxx"
#include "sp2crtn.hxx"
#include "bs3surf.hxx"
// STI dgh #include "spline/bs3_srf/srf_def.hxx"
#include "sp3srtn.hxx"

#include "spd3rtn.hxx"

#if defined D3_STANDALONE || defined D3_DEBUG

class D3_ostream;
class D3_istream;

extern	DECL_SPLINE D3_ostream &operator<<( D3_ostream &os, const bs3_curve& cu );
extern	DECL_SPLINE D3_ostream &operator<<( D3_ostream &os, const bs2_curve& cu );
 
extern	DECL_SPLINE D3_istream &operator>>( D3_istream &is, bs3_curve& cu );
extern	DECL_SPLINE D3_istream &operator>>( D3_istream &is, bs2_curve& cu );


extern	DECL_SPLINE D3_ostream &operator<<( D3_ostream &os, const bs3_surface& su );
 
extern	DECL_SPLINE D3_istream &operator>>( D3_istream &is, bs3_surface& su );

#endif
#endif
