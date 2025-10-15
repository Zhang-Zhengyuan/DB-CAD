/* ORIGINAL: acis2.1/kerngeom/curve/allcudef.hxx */
/* $Id: allcudef.hxx,v 1.9 2000/12/26 18:47:11 products Exp $ */
/*******************************************************************/
/*    Copyright (c) 1989-2020 by Spatial Corp.                     */
/*    All rights reserved.                                         */
/*    Protected by U.S. Patents 5,257,205; 5,351,196; 6,369,815;   */
/*                              5,982,378; 6,462,738; 6,941,251    */
/*    Protected by European Patents 0503642; 69220263.3            */
/*    Protected by Hong Kong Patent 1008101A                       */
/*******************************************************************/
// al 26Oct05: Added helix.

// Just include all currently-defined curve types.

#include "curdef.hxx"
#include "strdef.hxx"
#include "elldef.hxx"
#include "intdef.hxx"
#include "heldef.hxx"
#include "unddef.hxx"
#include "pntdef.hxx"

#ifndef NO_MESH_CLASSES
#include "compdef.hxx"
#endif
