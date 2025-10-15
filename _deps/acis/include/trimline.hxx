/* ORIGINAL: acis2.1/kerndata/trimline/trimline.hxx */
/* $Id: trimline.hxx,v 1.11 2002/08/09 17:18:48 jeff Exp $ */
/*******************************************************************/
/*    Copyright (c) 1989-2020 by Spatial Corp.                     */
/*    All rights reserved.                                         */
/*    Protected by U.S. Patents 5,257,205; 5,351,196; 6,369,815;   */
/*                              5,982,378; 6,462,738; 6,941,251    */
/*    Protected by European Patents 0503642; 69220263.3            */
/*    Protected by Hong Kong Patent 1008101A                       */
/*******************************************************************/

// Routines for finding the portions of a line drawn in a surface
// which lie within a face on that surface.

#if !defined( TRIMLINE_HEADER )
#define TRIMLINE_HEADER

#include "dcl_intr.h"
#include "logical.h"
// ywoo 08Jan01: added the header file.
#include "base.hxx"
// ywoo: end

class surf_surf_int;
class FACE;
class SPAtransf;
class surface;
class ff_header;

// Trim one or more lines lying in a surface to the bounds of a face
// also on that surface.

DECL_INTR surf_surf_int *trim_line_to_face(
			FACE *,
			SPAtransf const &,
			surface const &,
			surf_surf_int *,
			ff_header *&,
			surface const & = *(surface *)NULL_REF,
				// This is a "help surface", which intersects the
				// surface of the face in the lines to be trimmed. If
				// present, the trimming of all the lines can be done
				// in one go, otherwise they are done one at a time,
				// by calling this routine repeatedly.
			logical skip_surf_boundaries = FALSE
				// This indicates that the curve is already trimmed
				// to the surface boundaries, so do not bother to
				// reintersect the curve with face boundaries that
				// are coincident with the surface boundaries.
		);

#endif

