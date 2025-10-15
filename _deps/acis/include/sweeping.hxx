/* ORIGINAL: acis2.1/kernwire/sweeping/sweeping.hxx */
/* $Id: sweeping.hxx,v 1.11 2002/08/09 17:21:36 jeff Exp $ */
/*******************************************************************/
/*    Copyright (c) 1989-2020 by Spatial Corp.                     */
/*    All rights reserved.                                         */
/*    Protected by U.S. Patents 5,257,205; 5,351,196; 6,369,815;   */
/*                              5,982,378; 6,462,738; 6,941,251    */
/*    Protected by European Patents 0503642; 69220263.3            */
/*    Protected by Hong Kong Patent 1008101A                       */
/*******************************************************************/

// Header for sweeping routines.

// These routines handle the making of wires, the covering of closed
// wires to make sheets, the covering of sheets to make solids, and 
// the sweeping of faces of sheets and solids.
// (Wires cannot yet be swept into sheets.)

#if !defined( SWPG_HDR_DEF )
#define  SWPG_HDR_DEF

#include "dcl_cstr.h"
#include "unitvec.hxx"

class BODY;
class FACE;
class VERTEX;

class SPAposition;


// Given a direction d, find a planar face of the given body, 
// with normal n such that d.n is positive and has the greatest 
// value for any face of the body.  If several faces have equal 
// greatest value of d.n, then return the face with greatest 
// value of p.d, where p is a point on the face.
// Return NULL if no face is found with d.n positive.

DECL_CSTR FACE*
find_face( BODY*, 
 		   SPAunit_vector const & );	// direction d


// Given a SPAposition and a body, find the vertex of the body
// closest to that SPAposition.  If several vertices are 
// equi-distant from the given SPAposition, any one of them is
// returned.
// Return NULL if no vertex found.

DECL_CSTR VERTEX*
find_vertex( BODY*, SPAposition const &);


// Count the number of faces in a body (ignores sub-shells).

DECL_CSTR int
number_faces( BODY* );


// Find normal to a planar face (takes account of sense data).

DECL_CSTR SPAunit_vector
planar_face_normal( FACE* );


// Construct a sheet body, consisting of a single face with
// external edges, as a copy of the given face.

DECL_CSTR BODY*
sheet_from_face( FACE* );


#endif
