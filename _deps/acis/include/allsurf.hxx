/* ORIGINAL: acis2.1/kerndata/geom/allsurf.hxx */
/* $Id: allsurf.hxx,v 1.10 2000/12/26 18:46:50 products Exp $ */
/*******************************************************************/
/*    Copyright (c) 1989-2020 by Spatial Corp.                     */
/*    All rights reserved.                                         */
/*    Protected by U.S. Patents 5,257,205; 5,351,196; 6,369,815;   */
/*                              5,982,378; 6,462,738; 6,941,251    */
/*    Protected by European Patents 0503642; 69220263.3            */
/*    Protected by Hong Kong Patent 1008101A                       */
/*******************************************************************/

#if !defined( ALLSURF_HDR_DEF )
#define  ALLSURF_HDR_DEF

// Haul in all the currently-defined surface types

#include "surface.hxx"

#include "plane.hxx"
#include "cone.hxx"
#include "sphere.hxx"
#include "torus.hxx"
#include "spline.hxx"

#ifndef NO_MESH_CLASSES
#include "meshsurf.hxx"
#endif

#endif
