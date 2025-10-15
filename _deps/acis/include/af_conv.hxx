/* ORIGINAL: acis2.1/faceter/meshing/af_conv.hxx */
// $Id: af_conv.hxx,v 1.10 2001/01/11 18:02:46 ywoo Exp $
/*******************************************************************/
/*    Copyright (c) 1989-2020 by Spatial Corp.                     */
/*    All rights reserved.                                         */
/*    Protected by U.S. Patents 5,257,205; 5,351,196; 6,369,815;   */
/*                              5,982,378; 6,462,738; 6,941,251    */
/*    Protected by European Patents 0503642; 69220263.3            */
/*    Protected by Hong Kong Patent 1008101A                       */
/*******************************************************************/
#ifndef _AF_CONV
#define _AF_CONV

#include "dcl_fct.h"
// ywoo 08Jan01: added the header file.
#include "logical.h"
// ywoo: end

class AF_ENVIRONMENT;
class AF_WORKING_FACE;
class AF_WORKING_FACE_SET;
class AF_WORKING_EDGE_SET;
typedef struct {
    double emax2;		// PERMITTED deviation from surface.
	double min_ar;	// Subdivision is permitted if either of
					//  1) Existing aspect ratios are improved
					//	2) New aspect ratios are still above this
	double hmax2;	// PERMITTED max edge length.
	double mindot;	// minimum permitted dot product between
					//	adjacent normals.
	AF_WORKING_FACE *fw;
	int change_count;
	logical test_centroid;
 } EDGE_ERROR_CACHE;

void af_merge_nodes_to_neighbors(AF_WORKING_FACE *fw, AF_WORKING_FACE_SET *WorkingFaces, AF_WORKING_EDGE_SET *WorkingEdges);

int af_triangulate_by_surface_error(AF_WORKING_FACE *fw,
	//double max_error,double hmax,double dNmax,double armin = 0.4);
	double max_error,double hmax,double dNmax,logical test_centroid=FALSE,double armin = 0.3);

//
// Enforce tolerances strictly within existing
// mesh structure.
DECL_FCT void af_postcheck_tolerances(
	AF_WORKING_FACE *fw,
	double dmax,
	double hmax,
	double dNmax
	);

//
// Enforce tolerances strictly within existing
// mesh structure.
DECL_FCT void af_postcheck_view_dependent(
	AF_WORKING_FACE *fw,
	AF_ENVIRONMENT *env,
	double dsil,
	double dview,
	double pixel_area
	);




#endif
