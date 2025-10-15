/*******************************************************************/
/*    Copyright (c) 1989-2020 by Spatial Corp.                     */
/*    All rights reserved.                                         */
/*    Protected by U.S. Patents 5,257,205; 5,351,196; 6,369,815;   */
/*                              5,982,378; 6,462,738; 6,941,251    */
/*    Protected by European Patents 0503642; 69220263.3            */
/*    Protected by Hong Kong Patent 1008101A                       */
/*******************************************************************/
#if !defined( ADPCUR_HDR_DEF )
#define   ADPCUR_HDR_DEF
#include "dcl_kern.h"
#include "logical.h"
#include "bs2curve.hxx"
class BODY;
class COEDGE;
class ENTITY;
class curve;
class surface;
class SPApar_pos;
class SPAvector;
class SPAposition;
class SPApar_dir;
/**
 * @file add_pcu.hxx
 * @CAA2Level L1
 * @CAA2Usage U1
 * \addtogroup ACISPCURVES
 *
 * @{
 */
/**
* Specifies on which boundary of a surface, a pcurve lies.
* @param bndy_unknown
* Unknown boundary.
* @param bndy_low_u
* low u-parameter boundary.
* @param bndy_high_u
* high u-parameter boundary.
* @param bndy_low_v
* low v-parameter boundary.
* @param bndy_high_v
* high v-parameter boundary.
**/
enum bndy_type {
	bndy_unknown,
	bndy_low_u,
	bndy_high_u,
	bndy_low_v,
	bndy_high_v
};
/**
 * Adds pcurves to any <tt>ENTITY</tt>.
 * <br><br>
 * <b>Role:</b> This function does not assume that the curve is on the boundary of the face
 * surface. It computes the pcurves by inverse mapping the curve onto the surface. 
 * The tolerance of the pcurve fit is SPAresfit. 
 * If pcurves are already assigned to spline geometry (for example, intcurves) then the pcurves are
 * not necessarily recomputed.
 * <br><br>
 * <b>Effect:</b> Changes model
 * <br><br>
 * @param ent
 * ENTITY to which pcurves are to be added.
 * @param reset_pattern
 * Internal Use Only
 **/
DECL_KERN void sg_add_pcurves_to_entity(ENTITY *ent, logical reset_pattern = TRUE);
/**
 * This routine puts a pcurve on the coedge and also on edge geometry,
 * if necessary.  If the exact flag is TRUE, then this routine
 * assumes that the intcurve is the boundary of the face_surface.
 * This way the pcurve computation will be simpler and faster.  Moreover,
 * subsequent operations on this parent entity will be faster.
 * @param coedge
 * The COEDGE to which to add a pcurve.
 * @param exact
 * Flags whether or not this is an exact fit to a surface boundary.
 * @param bndy_unknown
 * If this is an exact boundary, which one.
 * @param approx_ok
 * Flags the use of the approximation curve and surface to make the pcurve.
 * @param reset_pattern
 * Internal Use Only.
 */
DECL_KERN void sg_add_pcurve_to_coedge(
	                                    COEDGE*,					// Coedge on which to add pcurve
	                                    logical	exact = FALSE,		// Is this an exact fit to a surface boundary?
	                                    bndy_type = bndy_unknown,	// If an exact boundary pcurve, which one?
	                                    logical approx_ok = FALSE,	// Use the approx curve and surface to make the pcurve
										logical reset_pattern = TRUE										
);

/**
 * @nodoc
 * Function checks to see if new_surf and original_surf are
 * related by extension and if so whether the par_box of bs2_crv
 * lies inside the domain of new_surf
 */
DECL_KERN logical same_ext_surf_contains_pc(const surface &new_surf,
								  const surface &original_surf,
								  bs2_curve bs2_crv);

/** @} */
#endif
