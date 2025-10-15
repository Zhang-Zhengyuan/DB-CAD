/* ORIGINAL: acis2.1/sg_husk/query/sgquertn.hxx */
// $Id: sgquertn.hxx,v 1.11 2002/08/09 17:19:03 jeff Exp $
/*******************************************************************/
/*    Copyright (c) 1989-2020 by Spatial Corp.                     */
/*    All rights reserved.                                         */
/*    Protected by U.S. Patents 5,257,205; 5,351,196; 6,369,815;   */
/*                              5,982,378; 6,462,738; 6,941,251    */
/*    Protected by European Patents 0503642; 69220263.3            */
/*    Protected by Hong Kong Patent 1008101A                       */
/*******************************************************************/

// Header File, which contains the prototypes for the utility routines
// defined in sg_query directory.  Note that the routine which were previously
// defined in the other header files in this directory are still intact.
// Except, one now can just include this file for everything.

#if !defined(SG_QUERY_FUNCTIONS)
#define SG_QUERY_FUNCTIONS


#include "dcl_intr.h"
#include "q_wire.hxx"
#include "q_vert.hxx"
#include "ptfcenum.hxx"

/**
* @file sgquertn.hxx
* @CAA2Level L1
* @CAA2Usage U1
*/

/** \addtogroup INTRMAIN
*  \brief Declared at <sgquertn.hxx>
*  @{
*/

//STI HDR_INDEP: nvp
class curve;
class SPAposition;
class surface;
class SPApar_pos;
class SPAparameter;


// Note: Functions in this file are for internal use only!

 //* Specifies whether a point lies inside, outside or on the boundary of the body.
 //* <br>
 //* <b>Role:</b> Used by <tt>point_in_body()</tt>.
 //* @param point_unknown_body
 //* unknown.
 //* @param point_inside_body
 //* point lies inside body.
 //* @param point_boundary_body
 //* point lies on the boundary of the body.
 //* @param point_outside_body
 //* point lies outside the body.

 /**
 * @nodoc
 */
enum point_body_containment {
	point_unknown_body,
	point_inside_body,
	point_boundary_body,
	point_outside_body
};

// Determine if a point is on the boundary of a face. Return the
// identifier of the first COEDGE or VERTEX within SPAresabs.
/*
// tbrv
*/

/**
 * @nodoc
 */
DECL_INTR logical
sg_point_on_boundary(
	const SPAposition& pos,		// point to test against boundary
								// in coordinate space of face
	FACE*			face,		// face to test against
	ENTITY*&		nearest,		// COEDGE or VERTEX of first closest
								// portion of boundary.
	double			tol=SPAresabs, // pos tolerance value.
	logical			project_on_surf = FALSE		// Use projection on surface
	);


// Routine which determines the containment of a point with-respect to
// the given face. Preferred internal interface.

/**
* @nodoc
*/
DECL_INTR point_face_containment
sg_point_in_face(
				 SPAposition const &,	// Given SPAposition
				 FACE*,					// Given face
				 SPAtransf const&,		// Any transformation to the given face
				 SPApar_pos const&,		// Guess SPApar_pos
				 logical,				// use point-in-face cache
				 int					// cache size
				 );

// Routine which determines the containment of a point with-respect to
// the given face. Can use a previous result for a point on that face 
// for better performance.

/**
* @nodoc
*/
DECL_INTR point_face_containment
sg_point_in_face(
				 SPAposition const &,	// Given SPAposition
				 FACE*,					// Given face
				 SPAtransf const&,		// Any transformation to the given face
				 SPApar_pos const&,		// Guess SPApar_pos
				 SPAposition const&,	// Previous point for which answer is known
				 point_face_containment,// Previous known answer
				 logical,				// use point-in-face cache
				 int					// cache size
				 );
				 




// Routine which determines the containment of a point with-respect to
// the given face.

/**
* @nodoc
*/
DECL_INTR point_face_containment
sg_point_in_face(
	SPAposition const &,		// Given SPAposition
	FACE const *,			// Given face
	SPAtransf const &,			// Any transformation to the given face
	surface const &,		// Transforment surface
	ENTITY *&,				// Entity-type hit returned.
	SPApar_pos const &,		// Guess SPAparameter.
	SPApar_pos &				// Face surface SPAparameter returned.
);

/**
 * This enumeration describes the relation between an edge and a point.
 * @param point_on_edge
 * Point is on the edge.
 * @param point_off_edge
 * Point is not on the edge.
 **/
enum sg_edge_point_relation
{
	point_on_edge,			// Point is on the edge
	point_off_edge			// point is not on the edge
};


typedef sg_edge_point_relation point_edge_containment;

// Routine which determines the containment of a point with-respect to
// the given edge.

/**
* @nodoc
*/
DECL_INTR point_edge_containment
sg_point_in_edge(
	SPAposition const &,		// Given SPAposition
	EDGE const*,			// Given edge
	SPAtransf const &,			// Any transformation
	ENTITY *&,				// Entity-type hit returned.
	SPAparameter &				// Edge SPAparameter returned.
);

// This function determines if the curve associated with a coedge
// lies on the boundary of a face whose underlying geometry is
// is spline surface.  This can be used to determine if a spline
// surface is trimmed or not.  It assumes the bs2_curves underlying
// the isoparameter pcurves are very accurate.  (If they are not
// isoparameter curves, they are not boundary curves.)

/**
* @nodoc
*/
DECL_INTR logical 
is_spline_boundary_curve( 
			const COEDGE *	// Input: Coedge associated with curve
			);

/** @} */

#endif
