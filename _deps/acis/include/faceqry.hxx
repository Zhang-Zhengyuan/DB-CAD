/* ORIGINAL: 3dt2.1/geomhusk/faceq.hxx */
// $Id: faceqry.hxx,v 1.6 2002/08/09 17:15:14 jeff Exp $
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
//    Declare procedures for doing queries about FACES
//
//----------------------------------------------------------------------

#ifndef faceqry_hxx
#define faceqry_hxx

//======================================================================

#include "dcl_kern.h"
#include "logical.h"

enum face_type {
	face_unknown,
	face_plane,
	face_cylinder,
	face_cone,
	face_sphere,
	face_torus,
	face_spline
};

class SPAposition;
class SPAunit_vector;
class FACE;

//----------------------------------------------------------------------

// Get the center of a spherical face
DECL_KERN logical get_spherical_face_center(FACE const*, SPAposition&);

// Get the radius of a cylindrical or spherical face
DECL_KERN logical get_face_radius(FACE const*, double&);

// Get the axis of a cylindrical, conical, toroidal or rotational face
DECL_KERN logical get_face_axis(FACE const*, SPAposition&, SPAunit_vector&);

// Get the cone angle of a conical face
DECL_KERN logical get_cone_angle(FACE const*, double&);

// Get the plane of a planar face
DECL_KERN logical get_face_plane(FACE const*, SPAposition&, SPAunit_vector&);

// Get the normal of a planar face
DECL_KERN logical get_face_normal(FACE const*, SPAunit_vector&);

// Get the face type
DECL_KERN face_type get_face_type(FACE const* in_face);

//======================================================================
#endif
