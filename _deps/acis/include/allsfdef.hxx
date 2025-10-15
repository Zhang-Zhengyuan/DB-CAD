/* ORIGINAL: acis2.1/kerngeom/surface/allsfdef.hxx */
/* $Id: allsfdef.hxx,v 1.9 2000/12/26 18:47:45 products Exp $ */
/*******************************************************************/
/*    Copyright (c) 1989-2020 by Spatial Corp.                     */
/*    All rights reserved.                                         */
/*    Protected by U.S. Patents 5,257,205; 5,351,196; 6,369,815;   */
/*                              5,982,378; 6,462,738; 6,941,251    */
/*    Protected by European Patents 0503642; 69220263.3            */
/*    Protected by Hong Kong Patent 1008101A                       */
/*******************************************************************/

// Portmanteau header for all (lower-case) surfaces.

#if !defined( ALLSFDEF_HDR_DEF )
#define  ALLSFDEF_HDR_DEF

#include "surdef.hxx"

// Non-parametric surfaces
#include "pladef.hxx"
#include "condef.hxx"
#include "sphdef.hxx"
#include "tordef.hxx"

// Parametric surfaces
#include "spldef.hxx"

#ifndef NO_MESH_CLASSES
// Mesh surface.

#include "mshdef.hxx"
#endif

#endif
