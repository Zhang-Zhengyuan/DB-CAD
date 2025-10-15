/* ORIGINAL: 3dt2.1/geomhusk/crv_utl.hxx */
// $Id: crv_utl.hxx,v 1.14 2002/08/09 17:21:35 jeff Exp $
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
//    Utility procedures for curves
//
//----------------------------------------------------------------------

#ifndef crv_utl_hxx
#define crv_utl_hxx

#include "api.hxx"
#include "dcl_cstr.h"
#include "logical.h"

class outcome;
class SPAposition;
class SPAunit_vector;
class SPAvector;
class pick_ray;
class ENTITY;
class EDGE;
class bounded_curve;
class entity_with_ray;
class curve_curve_int;

//======================================================================

//----------------------------------------------------------------------
// Curve Creation
//----------------------------------------------------------------------

DECL_CSTR outcome create_line_2pt(const SPAposition&, const SPAposition&, EDGE*&);

DECL_CSTR outcome create_line_tangent(const SPAposition*,
                            const entity_with_ray*,
                            const SPAposition*,
                            const entity_with_ray*,
                            EDGE*&
                            );

DECL_CSTR outcome create_arc_center_radius(const SPAposition&,
                                 double radius,
                                 double start_angle,
                                 double end_angle,
                                 EDGE*&);

DECL_CSTR outcome create_arc_3pt(const SPAposition&,
                       const SPAposition&,
                       const SPAposition&,
                       logical,
                       EDGE*&);

DECL_CSTR outcome create_arc_center_edge(const SPAposition&,
                               const SPAposition&,
                               const SPAposition&,
                               const SPAunit_vector*,
                               EDGE*&);

DECL_CSTR outcome create_arc_diagonal(const SPAposition&,
                            const SPAposition&,
                            logical,
                            EDGE*&);

DECL_CSTR outcome create_ellipse(const SPAposition&,
						const SPAposition&,
						double,
                    	double,
                    	double,
                    	EDGE*&);

// Create a spline that interpolates positions
DECL_CSTR outcome create_spline_curve(int,
                            const SPAposition*,
                            const SPAunit_vector*,
                            const SPAunit_vector*,
                            EDGE*&,
                            logical = TRUE,
							logical = FALSE
							  );

// Create a spline that interpolates positions at given parameters
DECL_CSTR outcome create_spline_curve2(int,
                            const SPAposition*,
                            const double* params,
                            const SPAvector*,
                            const SPAvector*,
                            EDGE*&
							  );


// Create a cubic Bezier curve given the four control points
DECL_CSTR outcome create_bezier_curve(
    const SPAposition& pt1,       // first control point
    const SPAposition& pt2,       // second control point
    const SPAposition& pt3,       // third control point
    const SPAposition& pt4,       // last control point
    EDGE*& crv                 // Bezier curve created
    );

DECL_CSTR outcome curve_arc_3curve(
    const entity_with_ray& eary1, // first curve + approximate tangent location
    const entity_with_ray& eary2, // second curve + approximate tangent location
    const entity_with_ray& eary3, // third curve + approximate tangent location
    logical full,                 // == TRUE for full circle
    EDGE*& arc                    // circle created
    );

//----------------------------------------------------------------------
// Curve Modification
//----------------------------------------------------------------------

DECL_CSTR outcome modify_line(EDGE*,
					const SPAposition&,
					const SPAposition&);

DECL_CSTR outcome modify_ellipse(
    EDGE*      ell,
    const SPAposition&     center,
    const SPAunit_vector&  normal,
    const SPAvector&       major_axis,
    double              radius_ratio,
    double              start_angle,
    double              end_angle
    );

DECL_CSTR outcome edge_to_spline(
    EDGE* ,                  // original edge 
    EDGE*&                   // new edge
    );

//======================================================================
#endif



