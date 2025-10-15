/* ORIGINAL: acis2.1/sg_husk/cur_off/sgcofrtn.hxx */
/* $Id: sgcofrtn.hxx,v 1.10 2002/08/09 17:27:14 jeff Exp $ */
/*******************************************************************/
/*    Copyright (c) 1989-2020 by Spatial Corp.                     */
/*    All rights reserved.                                         */
/*    Protected by U.S. Patents 5,257,205; 5,351,196; 6,369,815;   */
/*                              5,982,378; 6,462,738; 6,941,251    */
/*    Protected by European Patents 0503642; 69220263.3            */
/*    Protected by Hong Kong Patent 1008101A                       */
/*******************************************************************/
//
// History :
// MMM-DD-YY nam : Description
// Dec-12-02 jkf : Added check_offst_self_intersection to avoid self
//                 intersecting check (69236) for performance improvmt
//                 in sweeping, we already know at one point the body
//                 does no self intersect.
//
#if !defined( SGCOFRTN_HDR_DEF )
#define  SGCOFRTN_HDR_DEF

#include "dcl_ofst.h"
#include "logical.h"
#include "off_wire.hxx"

class BODY;
class curve;
class law;
class offset_segment_list;
class option_header;
class SPAinterval;
class SPAunit_vector;
class TRANSFORM;
class WIRE;
class wire_offset_options;

extern DECL_OFST option_header check_offst_self_intersection;

DECL_OFST BODY *sg_offset_planar_wire // Return offset wire
   (		
	   BODY *,					   // Wire body to offset
      wire_offset_options *  //wire offset options
	  //SC mka I don't why following were not included in wire_offset_options
	   //logical = FALSE,		   // Whether or not to add relationship attributes
	  // logical = FALSE         // Should return zero_length wire when offset to a point?
   );


DECL_OFST BODY *
sg_offset_planar_wire(		// Return offset wire
	BODY *,					// Wire body to offset
	double,					// Offset distance
	const SPAunit_vector &,	// Normal to planar wire
	sg_gap_type = arc,		// Type of closure to be used for gap
	logical = FALSE,		// Whether or not to add relationship attributes
	logical = TRUE,  		// Whether or not to trim
	logical = FALSE,        // Should return zero_length wire when offset to a point?
	logical = FALSE);	    // Should keep overlaped segments

DECL_OFST BODY *
sg_offset_planar_wire(		// Return offset wire
	BODY *,					// Wire body to offset
	law *,					// Offset law
	law *,                  // Twist law
	const SPAunit_vector &,	// Normal to planar wire
	sg_gap_type = arc,		// Type of closure to be used for gap
	logical = FALSE,		// Whether or not to add relationship attributes
	logical = TRUE,  		// Whether or not to trim
	logical = FALSE,        // Should return zero_length wire when offset to a point?
	logical = FALSE,	    // Should keep overlaped segments
	logical = FALSE);  // Should keep minimum topology

DECL_OFST BODY *
sg_offset_planar_wire(		// Return offset wire
	WIRE *,					// Wire to offset
	const TRANSFORM *,		// Transform for resulting body
	double,					// Offset distance
	const SPAunit_vector &,	// Normal to planar wire (in wire space)
	sg_gap_type = arc,		// Type of closure to be used for gap
	logical = FALSE,		// Whether or not to add relationship attributes
	logical = TRUE, 		// Whether or not to trim
	logical = FALSE,        // Should return zero_length wire when offset to a point?
	logical = FALSE);	    // Should keep overlaped segments

DECL_OFST BODY *
sg_offset_planar_wire(      // Return offset wire
    WIRE *,                 // Wire to offset
	const TRANSFORM *,      // Transform for resulting body
	law *dist_law,          // Offset distance law
	law *twist_law,         // Twist law
	const SPAunit_vector &,    // Normal to planar wire (in wire space)
	sg_gap_type = arc,      // Type of closure to be used for gap
	logical = FALSE,		// Whether or not to add relationship attributes
	logical = TRUE,  		// Whether or not to trim
	logical = FALSE,        // Should return zero_length wire when offset to a point?
	logical = FALSE,	    // Should keep overlaped segments
   logical = FALSE);  // Should keep minimum topology

DECL_OFST logical
sg_trim_offset_wire(
	offset_segment_list &,	// List containing offset information
	logical zero_len=FALSE, // Do not create zero-length offset when offsets to a point
	logical = FALSE);	    // Should keep overlaped segments

DECL_OFST curve *
offset_geometry(
	const curve*,			// Curve to offset
	const SPAunit_vector &,			// Normal to planar curve
	law*,					// Distance law
	law*,					// Twist law
	const SPAinterval &);  			// Domain of offset curve 
#endif
