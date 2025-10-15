/* ORIGINAL: acis2.1/kerngeom/d3_bs3/sur.hxx */
/* $Id: sur.hxx,v 1.16 2002/08/09 17:15:22 jeff Exp $ */
/*******************************************************************/
/*    Copyright (c) 1989-2020 by Spatial Corp.                     */
/*    All rights reserved.                                         */
/*    Protected by U.S. Patents 5,257,205; 5,351,196; 6,369,815;   */
/*                              5,982,378; 6,462,738; 6,941,251    */
/*    Protected by European Patents 0503642; 69220263.3            */
/*    Protected by Hong Kong Patent 1008101A                       */
/*******************************************************************/

#ifndef SURH
#define SURH

#include "dcl_kern.h"
#include "logical.h"
#include "surdef.hxx"

class SPAposition;
class spline;
class spl_sur;
class cone;
class sphere;
class torus;


// Find a point on the surface whose normal goes through X. If there is more 
// than one such point, return the one nearest to Q. 

extern DECL_KERN SPAposition SUR_relax( const surface&, const SPAposition& iX, 
                                           const SPAposition& Q );

extern DECL_KERN logical SUR_equiv_parametrisation( const surface& );


// Test for certain types of surface. 

extern	DECL_KERN logical	SUR_is_plane( const surface& );
extern	DECL_KERN logical	SUR_is_cone( const surface& );
extern	DECL_KERN logical	SUR_is_elliptical_cone( const surface& );
extern  DECL_KERN logical   SUR_is_cylinder( const surface& sf );
extern  DECL_KERN logical 	SUR_is_squashed_cylinder( const surface&, 
											  double const& t = SPAresabs );
extern	DECL_KERN logical	SUR_is_sphere( const surface& );
extern	DECL_KERN logical	SUR_is_torus( const surface& );
extern	DECL_KERN logical	SUR_is_spline( const surface& );
extern	DECL_KERN logical	SUR_is_exact_spline( const surface& );
extern	DECL_KERN logical	SUR_is_pipe( const surface& );
extern	DECL_KERN logical	SUR_is_vertex_blend( const surface& );
extern	DECL_KERN logical	SUR_is_offset_vertex_blend( const surface& );
extern	DECL_KERN logical	SUR_is_rb_blend( const surface& );
extern	DECL_KERN logical	SUR_is_var_blend( const surface& );
extern  DECL_KERN logical   SUR_is_gsm_blend( const surface& );
extern  DECL_KERN logical   SUR_is_cc_blend( const surface& );
extern	DECL_KERN logical	SUR_is_offset( const surface& );
extern	DECL_KERN logical	SUR_is_swept_taper( const surface& );
extern	DECL_KERN logical	SUR_is_ruled_taper( const surface& );
extern	DECL_KERN logical	SUR_is_ortho_surface( const surface& );
extern	DECL_KERN logical	SUR_is_new_taper( const surface& );
extern  DECL_KERN logical   SUR_is_skin_surface( const surface& sf );
extern  DECL_KERN logical   SUR_is_net_surface( const surface& sf );
extern	DECL_KERN logical   SUR_is_ruled_surface( const surface& sf );
extern	DECL_KERN logical   SUR_is_rot_surface( const surface& sf );
extern  DECL_KERN logical   SUR_is_sum_spl( const surface& sf );
extern  DECL_KERN logical   SUR_is_sweep( const surface& sf);
extern  DECL_KERN logical   SUR_is_spherical_spline( const surface& sf );
extern  DECL_KERN logical   SUR_is_conical_spline( const surface& sf );
extern  DECL_KERN logical   SUR_is_cylindrical_spline( const surface& sf );
extern  DECL_KERN logical   SUR_is_toridal_spline( const surface& sf );
extern	DECL_KERN const surface& SUR_offset_progenitor( const surface& );

// Whether a procedural blend surface, i.e a constant / variable / gsm
// blend surface. The check for pipe surface is optional.

extern  DECL_KERN logical   
SUR_is_procedural_blend( const surface&, logical include_pipe = FALSE );

// Find the u-SPAparameter value of a cone's apex, and flag whether it is at the
// high or low end of the SPAparameter range. Return null for a cylinder.

extern DECL_KERN double SUR_cone_apex( const cone&, logical& low );


// Offset a surface which is guaranteed to be a vertex blend surface, as
// determined by SUR_is_vertex_blend() above. Also requires a signed
// offset distance, with positive meaning offset in the direction of the
// surface normal. This function may return NULL if an excessively large
// offset has been requested, or the initial vertex blend is degenerate.

extern DECL_KERN spline *SUR_offset_vertex_blend( const surface& sf, double offset );
					
extern DECL_KERN logical SUR_is_underlying_periodic_u(const surface& sf, double& period);
extern DECL_KERN logical SUR_is_underlying_periodic_v(const surface& sf, double& period);

#if defined D3_STANDALONE || defined D3_DEBUG

class D3_ostream;
class D3_istream;

DECL_KERN D3_ostream &operator<<( D3_ostream &os, const surface& su );
DECL_KERN D3_istream &operator>>( D3_istream &is, surface*& su );

DECL_KERN D3_ostream &operator<<( D3_ostream &os, const spl_sur& su );
DECL_KERN D3_istream &operator>>( D3_istream &is, spl_sur*& su );

#endif
#endif

