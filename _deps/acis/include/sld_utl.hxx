/* ORIGINAL: 3dt2.1/geomhusk/sld_utl.hxx */
// $Id: sld_utl.hxx,v 1.11 2002/08/09 17:21:36 jeff Exp $
/*******************************************************************/
/*    Copyright (c) 1989-2020 by Spatial Corp.                     */
/*    All rights reserved.                                         */
/*    Protected by U.S. Patents 5,257,205; 5,351,196; 6,369,815;   */
/*                              5,982,378; 6,462,738; 6,941,251    */
/*    Protected by European Patents 0503642; 69220263.3            */
/*    Protected by Hong Kong Patent 1008101A                       */
/*******************************************************************/
//----------------------------------------------------------------------
// purpose---
//    Declare procedures for creating solids
//
//----------------------------------------------------------------------

#ifndef sld_utl_hxx
#define sld_utl_hxx

#include "dcl_cstr.h"
#include "logical.h"
#include "api.hxx"

class BODY;
class WCS;
class SPAposition;
class SPAunit_vector;
class tensor;

//======================================================================

DECL_CSTR outcome create_block(
                     const SPAposition&,
                     const SPAposition&,
					 WCS *,
                     BODY*&
                     );

DECL_CSTR outcome create_solid_block(
                            const SPAposition&,
                            const SPAposition&,
                            BODY*&
                            );

DECL_CSTR outcome create_solid_cyl_cone(
    const SPAposition& pt1, // SPAposition at bottom
    const SPAposition& pt2, // SPAposition at top
    double major_radius, // major radius at bottom
    double minor_radius, // minor radius at bottom
    double top_radius,   // major radius at top
    const SPAposition* xpt, // SPAposition in direction of major axis (or NULL)
    BODY*& cyl_or_cone   // cylinder or cone created
    );

DECL_CSTR outcome create_solid_sphere(
                            const SPAposition&,
                            double,
                            BODY*&
                            );

DECL_CSTR outcome create_solid_torus(const SPAposition& center,
                           double major_radius,
                           double minor_radius,
                           BODY*& new_torus
                           );

DECL_CSTR outcome calculate_solid_mass_properties( BODY*,
                                         int,
                                         double,
                                         SPAposition&,
                                         logical,
                                         SPAunit_vector&,
                                         double&,
                                         SPAposition&,
                                         double*, // [3]
                                         SPAunit_vector*, // [3]
                                         tensor&,
                                         double&,
										 double sheet_thickness
                                         );

//======================================================================
#endif
