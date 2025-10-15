/*ORIGINAL: acis2.1/sg_husk/face/face_utl.hxx */
// $Id: faceutil.hxx,v 1.7 2002/08/09 17:15:35 jeff Exp $
/*******************************************************************/
/*    Copyright (c) 1989-2020 by Spatial Corp.                     */
/*    All rights reserved.                                         */
/*    Protected by U.S. Patents 5,257,205; 5,351,196; 6,369,815;   */
/*                              5,982,378; 6,462,738; 6,941,251    */
/*    Protected by European Patents 0503642; 69220263.3            */
/*    Protected by Hong Kong Patent 1008101A                       */
/*******************************************************************/

#if !defined (SG_FACE)

#define SG_FACE

#include "dcl_kern.h"
#include "unitvec.hxx"
#include "position.hxx"
#include "param.hxx"

class FACE;
class surface_max_curvature_options;

/**
 * @file faceutil.hxx
 * @CAA2Level L1
 * @CAA2Usage U1
 * \addtogroup ACISEVALUATORS
 * @{
 */
/**
 * Calculate the normal to a face at a given SPAposition.
 */
DECL_KERN SPAunit_vector 
sg_get_face_normal(
		FACE* face,
		const SPAposition& p
	);
/**
 * This function determines a parameter space SPApar_box encompassing the given face.
 * It returns TRUE if the calculation was successful.
 * Please note that in general this is not a minimal SPApar_box.
 */
DECL_KERN logical
sg_get_face_par_box(
			FACE *this_face, 		// Input: the given face
			SPApar_box& face_range		// Output: the SPAparameter space SPAbox
		);
/**
 * Returns one or two of the extreme curvatures of a FACE.
 * <br><br>
 * <b>Role:</b> Returns the face's mimimum and maximum curvatures of the face.
 * Samples the entire face par-box for the maximum curvature of the surface (may not be within the FACE boundaries).
 * If the face is reversed from the surface, the face's curvature will be the negative of the surface's curvature.
 * The precision of the returned values being the most extreme curvatures is dependent on the sampling rate, which is fairly high. 
 * <br><br>
 * <b>Effect:</b> Read-only
 * <br><br>
 * <b>Journal: </b> Not Available
 * <br><br>
 * @param in_face
 * Points to the face to be searched.
 * @param out_max_curvature
 * The returned maximum curvature is concavity-up, normally convex. This would normally be the sharpest outwards bump of a face.
 * This may be negative or positve. For example, the inside of a cup can have a constant negative curvature,
 * and therfore the maximum and minimum curvatures returned may be equal.
 * This may == nullptr, in which performance is achieved by skipping that calculation.
 * @param out_min_curvature
 * The returned minimum curvature is concavity down, normally concave. This would be the sharpest inwards dimple of a face.
 * This may be negative or positve. For example, a sphere has a constant positive curvature,
 * and therfore the maximum and minimum curvatures returned may be equal.
 * This may == nullptr, in which performance is achieved by skipping that calculation.
 * @param out_max_curv_uv
 * The UV where the maximum curvature was found.
 * There may be more than one UV coordinate on the surface that has the same maximum curvature. This is only one of those coordinates.
 * @param out_min_curv_uv
 * The UV where the minimum curvature was found.
 * There may be more than one UV coordinate on the surface that has the same minimum curvature. This is only one of those coordinates.
 * @param in_smco
 * SurfacetMaximumCurvatureOptions. For future use.
 */
DECL_KERN void face_extreme_curvatures(
	FACE *							in_face,
	double *						out_max_curvature = nullptr,
	double *						out_min_curvature = nullptr,
	SPApar_pos *					out_max_curv_uv = nullptr,
	SPApar_pos *					out_min_curv_uv = nullptr,
	surface_max_curvature_options *	in_smco = nullptr);

/** @} */
#endif

