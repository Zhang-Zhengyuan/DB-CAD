/* ORIGINAL: acis2.1/sg_husk/sweep/sgswertn.hxx */
// $Id: off_misc.hxx,v 1.6 2002/08/09 17:27:14 jeff Exp $
/*******************************************************************/
/*    Copyright (c) 1989-2020 by Spatial Corp.                     */
/*    All rights reserved.                                         */
/*    Protected by U.S. Patents 5,257,205; 5,351,196; 6,369,815;   */
/*                              5,982,378; 6,462,738; 6,941,251    */
/*    Protected by European Patents 0503642; 69220263.3            */
/*    Protected by Hong Kong Patent 1008101A                       */
/*******************************************************************/

#if !defined( OFF_MISC_HXX )
#define  OFF_MISC_HXX

#include "dcl_ofst.h"
// ywoo 08Jan01: added the header file.
#include "base.hxx"
// ywoo: end

class COEDGE;

class SPAinterval;
class curve;

class SPAvector;
class SPAunit_vector;

class law;


// paul-2.2b removed draft added twist and dist_law

// return the offset of a planar curve in the plane of the curve by the
// given distance towards the right side ( normal * tan direction).
DECL_OFST curve *
sg_offset_planar_curve(
    const curve&,                   // original curve
    SPAinterval&,                      // curve range
    double,                         // fit tolerance
    law*,							// offset distance law
	law*,							// twist law
    SPAunit_vector&,                   // offset normal
    logical return_spline = TRUE,   // Return always spline curves.
	double tol = SPAresabs		        // Optional tolerance for the draft and offset dist
);

// paul-2.2b removed draft dist

// Return the offset of a coedge
DECL_OFST curve *
sg_offset_planar_coedge(
    COEDGE *,                       // Coedge to be offset
    SPAvector const &,                 // offset curve.
    SPAunit_vector const &coedge_normal = *(SPAunit_vector *)NULL_REF,
    logical return_spline = TRUE,   // Return always spline curves
	double tol = SPAresabs				// Optional tolerance for the draft and
									// offset distance
);

// Return the offset of a coedge   paul-2.2b

DECL_OFST curve *
sg_offset_planar_coedge(
    COEDGE *,                        // Coedge to be offset
    SPAvector const &,                  // offset curve.
	law *dist_law,					 //	Distance law
	law *twist_law,					 // Twist law
    SPAunit_vector const &coedge_normal //	Normal SPAvector to the plane
	= *(SPAunit_vector *)NULL_REF,
    logical return_spline = TRUE,    // Return always spline curves
	double tol = SPAresabs				 // Optional tolerance for the draft and
									 // offset distance
);

#endif
