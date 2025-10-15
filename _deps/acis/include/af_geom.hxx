/* ORIGINAL: acis2.1/faceter/acisintf/af_geom.hxx */
// $Id: af_geom.hxx,v 1.13 2002/08/09 17:21:50 jeff Exp $
/*******************************************************************/
/*    Copyright (c) 1989-2020 by Spatial Corp.                     */
/*    All rights reserved.                                         */
/*    Protected by U.S. Patents 5,257,205; 5,351,196; 6,369,815;   */
/*                              5,982,378; 6,462,738; 6,941,251    */
/*    Protected by European Patents 0503642; 69220263.3            */
/*    Protected by Hong Kong Patent 1008101A                       */
/*******************************************************************/
#ifndef _AF_GEOM
#define _AF_GEOM

// General headers
#include "dcl_fct.h"
#include "logical.h"
#include "sps3srtn.hxx"
#include "af_ladut.hxx"

#define DEFAULT_SPLINE_SAMPLING 8
//
// af_get_max_curvature is overloaded for the following ACIS surface entities:
//
// af_get_max_curvature(SURFACE *C,PAR_BOX,
//                      rhou,rhov,rdsdu,rdsdv,dsdu,dsdv,drhou,drhov,mdsdu,mdsdv)
// af_get_max_curvature(CONE   *C, ...)
// af_get_max_curvature(PLANE  *P, ...)
// af_get_max_curvature(SPHERE *S, ...)
// af_get_max_curvature(TORUS  *T, ...)
// af_get_max_curvature(SPLINE *S, ...)
//
// Each determines maximal curvature and ds/dparam for lines of constant u and v
// rhou,rhov = radius of curvature on lines of constant u and v that has the
//             maximum value of rhou/rdsdv and rhov/rdsdu.
// rdsdu,rdsdv = dsdu, dsdv corresponding to rhov and rhou
// dsdu,dsdv = surface step per SPAparameter step in given direction that has the
//             maximum value of sqrt(drhov)/dsdu and sqrt(drhou)/dsdv
// drhou,drhov = rhou, rhov corresponding to dsdv and dsdu.
// mdsdu,mdsdv = maximum dsdu and dsdv
//

class CONE;
class SPHERE;
class PLANE;
class TORUS;
class SPLINE;
class SURFACE;
class SPAposition;
class SPAvector;
class INTERVAL;
class PAR_BOX;
class AF_PAR_TRANS;
class AF_WORKING_FACE;
class Param_Line_Ranges;

struct SURFACE_PROP {
	double		rhou;
	double		rhov;
	double		rdsdu;
	double		rdsdv;
	double		dsdu;
	double		dsdv;
	double		drhou;
	double		drhov;
	double		mdsdu;
	double		mdsdv;
	int			face_sense;
	double		face_size;
	double		body_size;
	double		face_body_ratio;
	PARAMARR	parU;
	PARAMARR	parV;
	DOUBLEARR	CrvU;
	DOUBLEARR	CrvV;
	DOUBLEARR	Fdsdu;
	DOUBLEARR	Fdsdv;
	SURFACE_PROP() {
		rhou = rhov = rdsdu = rdsdv = dsdu = dsdv = drhou = drhov 
			 = mdsdu = mdsdv = 0.0;
		face_sense = 0;
		face_size = 0.0;
		body_size = 0.0;
		face_body_ratio = 0.0;
	}
	logical		twistedU;
	logical		twistedV;
};

DECL_FCT void af_get_max_curvature(
	const CONE		*C,
	const PAR_BOX	&uv_box,
	SURFACE_PROP	&prop
);

DECL_FCT void af_get_max_curvature(
	const PLANE		*,
	const PAR_BOX	&,
	SURFACE_PROP	&prop
);

DECL_FCT void af_get_max_curvature(
	const SPHERE	*S,
	const PAR_BOX	&,
	SURFACE_PROP	&prop	
	);

DECL_FCT void af_get_max_curvature(
	const TORUS		*T,
	const PAR_BOX	&,
	SURFACE_PROP	&prop
);

//-------------------------------------------------------------------------
// Return the various curvature parameters for determining the grid on the
// parametric space.
//-------------------------------------------------------------------------
DECL_FCT void af_get_max_curvature(
	AF_WORKING_FACE *	fw,
	const SPLINE	*S,
	const PAR_BOX	&B,
	SURFACE_PROP	&prop,
	double			dmax,
	double			dNmax,
	logical			average = FALSE
);

// PRS added u and v param_step
DECL_FCT void af_get_max_curvature(
	AF_WORKING_FACE *	fw,
	const SURFACE	*S,
	const PAR_BOX	&uv_box,
	AF_PAR_TRANS	*trans,
	SURFACE_PROP	&prop,
	double			&dumax,
	double			&dvmax,
	double			dmax,
	double			dNmax,
	double          u_param_step,
	double          v_param_step
);

//*******************************************************
// Function: af_normal_to_polygon
// Return an estimated normal for a polygon through an array
//		of vertices.
// Input: XX[0..n-1] = array of positions
// Function value: 	0==> no good normal found
// 					1==> normal computed
//*****************************************************

// STIPORT TAC ultrix whines
//int af_pseudo_polygon_normal(SPAposition X[],int nX,SPAvector &N);
DECL_FCT int af_pseudo_polygon_normal(SPAposition *iX,int nX,SPAvector &N);

//*******************************************************
// Function: af_pseudo_polygon_area
// Return an estimated area for a polygon through an array
//		of vertices, using a given SPAvector as the
//		out of plane normal.
// Input: XX[0..n-1] = array of positions
// Function value: 	0==> no good normal found
// 					1==> normal computed
//*****************************************************
// STIPORT TAC ultrix doesn't like this, same as line 150
//double af_pseudo_polygon_area(SPAposition X[],int nX,SPAvector N);
DECL_FCT double af_pseudo_polygon_area(SPAposition *iX,int nX,const SPAvector & N);

DECL_FCT void af_check_singular_u(const SURFACE *S,const INTERVAL &range,int &smin,int &smax,int orientation);

DECL_FCT void af_check_singular_v(const SURFACE *S,const INTERVAL &range,int &smin,int &smax,int orientation);

#endif
