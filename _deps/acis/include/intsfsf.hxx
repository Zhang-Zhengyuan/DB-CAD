/* ORIGINAL: acis2.1/kernint/intsfsf/intsfsf.hxx */
/* $Id: intsfsf.hxx,v 1.16 2002/08/09 17:18:52 jeff Exp $ */
/*******************************************************************/
/*    Copyright (c) 1989-2020 by Spatial Corp.                     */
/*    All rights reserved.                                         */
/*    Protected by U.S. Patents 5,257,205; 5,351,196; 6,369,815;   */
/*                              5,982,378; 6,462,738; 6,941,251    */
/*    Protected by European Patents 0503642; 69220263.3            */
/*    Protected by Hong Kong Patent 1008101A                       */
/*******************************************************************/

// Header file for surface-surface intersection.

// Intersect the surfaces of two faces, returning zero or more 
// curves. We may assume that every edge of each face has already 
// been intersected with the other surface, so the intersection 
// points may be used to assist us (for example if the surfaces 
// are parametric).

#if !defined( INT_SURF_SURF )
#define INT_SURF_SURF

#include "logical.h"

#include "acis.hxx"
#include "dcl_intr.h"
#include "sfsfint.hxx"

class surface;
class FACE;
class SPAtransf;

class plane;
class cone;
class sphere;
class torus;
class spline;

class SPAbox;

class DECL_INTR ssi_bool_info : public ACIS_OBJECT
{
private:
    logical edges_all_coi;
	logical abort_on_illegal_surf;

public:
    ssi_bool_info()
		:edges_all_coi(FALSE),
	     abort_on_illegal_surf(FALSE)
		{};
    virtual ~ssi_bool_info(){};

    logical edges_all_coincident() const {return edges_all_coi;}
    void set_edges_all_coincident(logical eac) { edges_all_coi = eac; }

	logical abort_on_illegal_surface() const {
		return abort_on_illegal_surf;
	}
	void set_abort_on_illegal_surface(logical abort) {
		abort_on_illegal_surf = abort;
	}
	

};


// This is the general (lower-case) surface-surface intersection 
// routine (in intsfsf.cxx).  Depending on the types of the two 
// surfaces, it calls the specific routines below, returning a 
// pointer to a chain of surf_surf_int's on the heap.

// In intsfsf.cxx

DECL_INTR surf_surf_int *int_surf_surf(
			surface const &,
			FACE *,
			SPAtransf const &,
			surface const &,
			FACE *,
			SPAtransf const &,
			SPAbox const & = *(SPAbox *)NULL_REF,
            ssi_bool_info * = NULL
		);

// Tolerant version of int_surf_surf
//
// Should be called when an intersection tolerance is required

DECL_INTR surf_surf_int *int_surf_surf(
			surface const &,
			surface const &,
			double const,
			SPAbox const & = *(SPAbox *)NULL_REF,
			ssi_bool_info * = NULL,
			SPApar_box const & = *(SPApar_box *)NULL_REF,
			SPApar_box const & = *(SPApar_box *)NULL_REF
		);

// 球面和球面求交
DECL_INTR surf_surf_int *gme_int_surf_surf(
			sphere &sph1,
			sphere &sph2,
			double tol = SPAresabs,
			SPAbox const & = *(SPAbox *)NULL_REF,
			ssi_bool_info * = NULL,
			SPApar_box const & = *(SPApar_box *)NULL_REF,
			SPApar_box const & = *(SPApar_box *)NULL_REF
		);

// 平面和平面求交
DECL_INTR surf_surf_int *gme_int_surf_surf(
			plane &pla1,
			plane &pla2,
			double tol = SPAresabs, 
			SPAbox const & = *(SPAbox *)NULL_REF, 
			ssi_bool_info * = NULL, 
			SPApar_box const & = *(SPApar_box *)NULL_REF, 
			SPApar_box const & = *(SPApar_box *)NULL_REF
		);

// 三个平面求交
DECL_INTR logical gme_intersect_3_planes(SPAposition &p1, SPAvector &n1, 
	SPAposition &p2, SPAvector &n2, 
	SPAposition &p3, SPAvector &n3, 
	SPAposition &int_pt, double tol = SPAresabs);

// 平面和球面求交
DECL_INTR surf_surf_int *gme_int_surf_surf(
			plane &,
			sphere &, 
			double tol = SPAresabs, 
			SPAbox const & = *(SPAbox *)NULL_REF, 
			ssi_bool_info * = NULL, 
			SPApar_box const & = *(SPApar_box *)NULL_REF, 
			SPApar_box const & = *(SPApar_box *)NULL_REF
		);

// 平面和圆柱(圆锥)求交
DECL_INTR surf_surf_int *gme_int_surf_surf(
			plane &pla,
			cone &con,
			double tol = SPAresabs,
			SPAbox const & = *(SPAbox *)NULL_REF,
			ssi_bool_info * = NULL,
			SPApar_box const & = *(SPApar_box *)NULL_REF,
			SPApar_box const & = *(SPApar_box *)NULL_REF
		);

// 平面和圆环求交
DECL_INTR surf_surf_int *gme_int_surf_surf(
			plane &pla,
			torus &tor,
			double tol = SPAresabs,
			SPAbox const & = *(SPAbox *)NULL_REF,
			ssi_bool_info * = NULL,
			SPApar_box const & = *(SPApar_box *)NULL_REF,
			SPApar_box const & = *(SPApar_box *)NULL_REF
		);

// 球面和圆环求交
DECL_INTR surf_surf_int *gme_int_surf_surf(
			sphere &sph,
			torus &tor,
			double tol = SPAresabs,
			SPAbox const & = *(SPAbox *)NULL_REF,
			ssi_bool_info * = NULL,
			SPApar_box const & = *(SPApar_box *)NULL_REF,
			SPApar_box const & = *(SPApar_box *)NULL_REF
		);

// 圆柱和圆柱求交
DECL_INTR surf_surf_int *gme_int_surf_surf(
			cone &con1,
			cone &con2,
			double tol = SPAresabs,
			SPAbox const & = *(SPAbox *)NULL_REF,
			ssi_bool_info * = NULL,
			SPApar_box const & = *(SPApar_box *)NULL_REF,
			SPApar_box const & = *(SPApar_box *)NULL_REF
		);

// 球和圆柱(圆锥)求交
DECL_INTR surf_surf_int *gme_int_surf_surf(
			sphere &sph,
			cone &con,
			double tol = SPAresabs,
			SPAbox const & = *(SPAbox *)NULL_REF,
			ssi_bool_info * = NULL,
			SPApar_box const & = *(SPApar_box *)NULL_REF,
			SPApar_box const & = *(SPApar_box *)NULL_REF
		);

// 圆环和圆环求交
DECL_INTR surf_surf_int *gme_int_surf_surf(
			torus &tor1,
			torus &tor2,
			double tol = SPAresabs,
			SPAbox const & = *(SPAbox *)NULL_REF,
			ssi_bool_info * = NULL,
			SPApar_box const & = *(SPApar_box *)NULL_REF,
			SPApar_box const & = *(SPApar_box *)NULL_REF
		);
#endif
