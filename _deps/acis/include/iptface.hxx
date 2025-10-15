/* ORIGINAL: acis2.1/kerndata/ptinface/iptface.hxx */
/* $Id: iptface.hxx,v 1.13 2002/08/22 16:20:54 sallen Exp $ */
/*******************************************************************/
/*    Copyright (c) 1989-2020 by Spatial Corp.                     */
/*    All rights reserved.                                         */
/*    Protected by U.S. Patents 5,257,205; 5,351,196; 6,369,815;   */
/*                              5,982,378; 6,462,738; 6,941,251    */
/*    Protected by European Patents 0503642; 69220263.3            */
/*    Protected by Hong Kong Patent 1008101A                       */
/*******************************************************************/
/*******************************************************************/
// thm 13Jan05 Add structure for points_in_loop.
/*******************************************************************/
// Routines used internally to point(s)_in_face testing.

//
// Modification---
//	21-Nov-99 bd	New arg to point_in_spline_loop() saying whether to cache.
//

#if !defined( IPTFACE_HDR_DEF )
#define  IPTFACE_HDR_DEF

#include "dcl_intr.h"
#include "logical.h"

#include "ptfcenum.hxx"

class SPAposition;
class SPApar_pos;
class LOOP;
class SPAtransf;
class surface;
class plane;
class cone;
class sphere;
class torus;
class spline;

DECL_INTR point_face_containment point_in_surface_loop(
		  SPAposition const &,
		  LOOP *,
		  SPAtransf const &,
		  surface const *,
		  logical = FALSE,
		  SPApar_pos const & = *(SPApar_pos *)NULL_REF
	  );

DECL_INTR void points_in_surface_loop(
		  SPAposition* const,
          int,
          point_face_containment*,
		  LOOP *,
		  SPAtransf const &,
		  surface const *,
		  logical = FALSE,
		  SPApar_pos* const = NULL
	  );

DECL_INTR point_face_containment point_in_plane_loop(
			SPAposition const &,
			LOOP *,
			SPAtransf const &,
			plane const &,
			logical = FALSE,
			logical = FALSE,
			logical = FALSE,
			SPAposition *& = (SPAposition *&) NULL_REF,
			int & = *(int *) NULL_REF,
			SPAposition * = NULL,
			int = 0,
			logical * = NULL ,
			SPAposition * = NULL
		);
DECL_INTR point_face_containment point_in_cone_loop(
			SPAposition const &,
			LOOP *,
			SPAtransf const &,
			cone const &,
			logical = FALSE
		);
DECL_INTR point_face_containment point_in_sphere_loop(
			SPAposition const &,
			LOOP *,
			SPAtransf const &,
			sphere const &,
			logical = FALSE,
			logical = FALSE,
			SPAposition *&  = (SPAposition *&) NULL_REF,
			int & = *(int *) NULL_REF,
			SPAposition * = NULL,
			int = 0,
			logical * = NULL
		);
DECL_INTR point_face_containment point_in_torus_loop(
			SPAposition const &,
			LOOP *,
			SPAtransf const &,
			torus const &,
			logical = FALSE
		);
DECL_INTR void points_in_torus_loop(
			SPAposition* const test_points,
			int npoints,
			point_face_containment* results,
			LOOP* test_loop,
			SPAtransf const &loop_trans,
			torus const &tor,
			logical multi_loop );

DECL_INTR point_face_containment vortex_in_torus_loop(
			SPAposition const &,
			LOOP *,
			SPAtransf const &,
			torus const &,
			logical = FALSE,
			logical = FALSE
		);
DECL_INTR point_face_containment point_in_spline_loop_cache( // pif-cache optional
			SPApar_pos const &,
			LOOP *,
			SPAtransf const &,
			spline const &,
			logical = FALSE,
			logical = FALSE,		// whether to use cache.
			int = 10,				// size of cache
			SPAposition * = NULL
		);
DECL_INTR point_face_containment point_in_spline_loop( // no pif-cache
			SPApar_pos const &,
			LOOP *,
			SPAtransf const &,
			spline const &,
			logical = FALSE,
			SPAposition * = NULL
		);
DECL_INTR point_face_containment point_in_surface_loop_cache( // with pif-cache
			SPApar_pos const &,
			LOOP *,
			SPAtransf const &,
			surface const &,
			logical = FALSE,
			int = 10,				// size of cache
			SPAposition * = NULL
		);

#endif
