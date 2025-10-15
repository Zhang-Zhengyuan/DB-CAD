/* ORIGINAL: acis2.1/kerndata/savres/versions.hxx */
/*******************************************************************/
/*    Copyright (c) 1989-2020 by Spatial Corp.                     */
/*    All rights reserved.                                         */
/*    Protected by U.S. Patents 5,257,205; 5,351,196; 6,369,815;   */
/*                              5,982,378; 6,462,738; 6,941,251    */
/*    Protected by European Patents 0503642; 69220263.3            */
/*    Protected by Hong Kong Patent 1008101A                       */
/*******************************************************************/

//  08-Mar-06 al : Added HELIX version numbers.
//
// This file contains a list of the version numbers of Acis at which
// the format of "save" files has changed. It is used in individual
// ENTITY (and other) "restore" routines to allow old save files to be
// restored into new Acis programs.

// First declare the variables which contain the version numbers
// of the current save file being restored. There is a major and
// minor component, and a portmanteau number which combines the two,
// and is the value actually placed in the save file.

#if !defined( VERSIONS_HDR_DEF )
#define  VERSIONS_HDR_DEF

#include "dcl_kern.h"

extern DECL_KERN int& get_restore_major_version();
extern DECL_KERN int& get_restore_minor_version();
extern DECL_KERN int& get_restore_version_number();	// Combined version number.
#ifndef restore_version_number
#define restore_version_number get_restore_version_number()
#endif

// Now the same for the current "save" file. This is normally set
// to the version number of this version of Acis, but can be set
// backwards by the application to cause backwards-compatible save
// files to be produced (provided the data structure is itself
// backwards-compatible).

extern DECL_KERN int& get_save_major_version();
extern DECL_KERN int& get_save_minor_version();
extern DECL_KERN int& get_save_version_number();		// Combined version number.
#ifndef save_version_number
#define save_version_number get_save_version_number()
#endif

// Macros to define the way the portmanteau version number is
// obtained from the major and minor versions.

#define PORTMANTEAU( maj, min ) (100 * maj + min)
#define MAJOR_VERSION( port ) (port / 100)
#define MINOR_VERSION( port ) (port % 100)

// Release version numbers.

// The version at which the start and end SPAparameter values of
// edges were removed from the save file, and recomputed when
// required.

const int PARAM_MAJOR = 1;
const int PARAM_MINOR = 1;
const int PARAM_VERSION = PORTMANTEAU( PARAM_MAJOR, PARAM_MINOR );

// The version at which LUMPs were introduced, to subdivide bodies
// into disjoint connected regions.

const int LUMP_MAJOR = 1;
const int LUMP_MINOR = 1;
const int LUMP_VERSION = PORTMANTEAU( LUMP_MAJOR, LUMP_MINOR );

// The version at which curve types were first output to the save
// file as character strings instead of integers.

const int CURVE_MAJOR = 1;
const int CURVE_MINOR = 3;
const int CURVE_VERSION = PORTMANTEAU( CURVE_MAJOR, CURVE_MINOR );

// The version at which surface types were first output to the save
// file as character strings instead of integers.

const int SURFACE_MAJOR = 1;
const int SURFACE_MINOR = 3;
const int SURFACE_VERSION =
	PORTMANTEAU( SURFACE_MAJOR, SURFACE_MINOR );

// The version at which subtypes of intcurves were defined requiring
// classification in the save file.

const int INTCURVE_MAJOR = 1;
const int INTCURVE_MINOR = 3;
const int INTCURVE_VERSION =
	PORTMANTEAU( INTCURVE_MAJOR, INTCURVE_MINOR );

// The version at which subtypes of splines were defined requiring
// classification in the save file.

const int SPLINE_MAJOR = 1;
const int SPLINE_MINOR = 3;
const int SPLINE_VERSION = PORTMANTEAU( SPLINE_MAJOR, SPLINE_MINOR );

// The version at which pointers were first distinguished from integers
// in text files by a dollar sign.

const int DOLLAR_MAJOR = 1;
const int DOLLAR_MINOR = 3;
const int DOLLAR_VERSION = PORTMANTEAU( DOLLAR_MAJOR, DOLLAR_MINOR );

// The version at which sharable objects like int_cur and spl_sur
// were first surrounded by curly braces, and could be shared in the
// save file.

const int SHARABLE_MAJOR = 1;
const int SHARABLE_MINOR = 4;
const int SHARABLE_VERSION = PORTMANTEAU( SHARABLE_MAJOR, SHARABLE_MINOR );

// The version at which blend attributes lost their "form" integer
// value (which was always zero, and so redundant).

const int BLEND_MAJOR = 1;
const int BLEND_MINOR = 5;
const int BLEND_VERSION = PORTMANTEAU( BLEND_MAJOR, BLEND_MINOR );

// The version at which SPAparameter curves may have the defining
// SPAparameter curve and surface in either slot 1 or 2.

const int PARCUR_MAJOR = 1;
const int PARCUR_MINOR = 5;
const int PARCUR_VERSION = PORTMANTEAU( PARCUR_MAJOR, PARCUR_MINOR );

// The version at which pcurves may have different subtypes, and so
// need a type code.

const int PCURVE_MAJOR = 1;
const int PCURVE_MINOR = 5;
const int PCURVE_VERSION = PORTMANTEAU( PCURVE_MAJOR, PCURVE_MINOR );

// The version at which faces may be double-sided, either free sheets
// or embedded in material.

const int TWOSIDE_MAJOR = 1;
const int TWOSIDE_MINOR = 5;
const int TWOSIDE_VERSION = PORTMANTEAU( TWOSIDE_MAJOR, TWOSIDE_MINOR );

// The version at which logical values are written to the save file
// as "T" or "F" instead of as integers.

const int LOGIO_MAJOR = 1;
const int LOGIO_MINOR = 5;
const int LOGIO_VERSION = PORTMANTEAU( LOGIO_MAJOR, LOGIO_MINOR );

// The version at which coedges were first expected to be sorted clockwise
// around the edge.

const int SORTCOED_MAJOR = 1;
const int SORTCOED_MINOR = 5;
const int SORTCOED_VERSION = PORTMANTEAU( SORTCOED_MAJOR, SORTCOED_MINOR );

// The version at which multiple entities can be saved into a single save
// file unit (as opposed to multiple self-contained sections of a file).

const int MULTSAV_MAJOR = 1;
const int MULTSAV_MINOR = 5;
const int MULTSAV_VERSION = PORTMANTEAU( MULTSAV_MAJOR, MULTSAV_MINOR );

// The version at which intervals could be (semi-)infinite, and so require
// different save file format.

const int INFINT_MAJOR = 1;
const int INFINT_MINOR = 6;
const int INFINT_VERSION = PORTMANTEAU( INFINT_MAJOR, INFINT_MINOR );

// The version at which curves have a subset range to bound them within
// their natural range.

const int BNDCUR_MAJOR = 1;
const int BNDCUR_MINOR = 6;
const int BNDCUR_VERSION = PORTMANTEAU( BNDCUR_MAJOR, BNDCUR_MINOR );

// The version at which surfaces have a subset range to bound them within
// their natural range.

const int BNDSUR_MAJOR = 1;
const int BNDSUR_MINOR = 6;
const int BNDSUR_VERSION = PORTMANTEAU( BNDSUR_MAJOR, BNDSUR_MINOR );

// The version in which save and restore started using a FileInterface
// object to do the io.

const int FILEINTERFACE_MAJOR = 1;
const int FILEINTERFACE_MINOR = 6;
const int FILEINTERFACE_VERSION =
	PORTMANTEAU( FILEINTERFACE_MAJOR, FILEINTERFACE_MINOR );

// The version in which "Wire Booleans" were implemented, requiring
// extensions to the data structure.

const int WIREBOOL_MAJOR = 1;
const int WIREBOOL_MINOR = 7;
const int WIREBOOL_VERSION = PORTMANTEAU( WIREBOOL_MAJOR, WIREBOOL_MINOR );

// The version in which "3D eye refinements" were implemented.

const int THREEDEYE_REF_MAJOR = 1;
const int THREEDEYE_REF_MINOR = 7;
const int THREEDEYE_REF_VERSION = 
	PORTMANTEAU(THREEDEYE_REF_MAJOR,THREEDEYE_REF_MINOR);

// STI jmb: Version in which the History Manager was introduced

const int HISTORY_MAJOR = 1;
const int HISTORY_MINOR = 7;
const int HISTORY_VERSION =
	PORTMANTEAU( HISTORY_MAJOR, HISTORY_MINOR);
// STI jmb: end
										
// The version in which "Safe Ranges" for intcurves were implemented, requiring
// an extra field in the data structure.

const int SAFERANGE_MAJOR = 1;
const int SAFERANGE_MINOR = 7;
const int SAFERANGE_VERSION = PORTMANTEAU( SAFERANGE_MAJOR, SAFERANGE_MINOR );

// The version which introduced angled cross curves at the ends of
// face-face blends, requiring extra fields in ATTRIB_FFBLEND.

const int ANG_XCUR_MAJOR = 1;
const int ANG_XCUR_MINOR = 7;
const int ANG_XCUR_VERSION = PORTMANTEAU( ANG_XCUR_MAJOR, ANG_XCUR_MINOR );

// The version at which advanced blending facilities were first made
// available.

const int ADV_BL_MAJOR = 1;
const int ADV_BL_MINOR = 8;
const int ADV_BL_VERSION = PORTMANTEAU( ADV_BL_MAJOR, ADV_BL_MINOR );

// STI dgh - begin
// The version where the Intergraph spline library was first introduced.

//const int IGRSPLINE_MAJOR = 2;
//const int IGRSPLINE_MINOR = 0;
//const int IGRSPLINE_VERSION = PORTMANTEAU( IGRSPLINE_MAJOR, IGRSPLINE_MINOR );
// STI dgh - end

// The version where the save/restore of logicals, enums was made consistent

const int CONSISTENT_MAJOR = 2;
const int CONSISTENT_MINOR = 0;
const int CONSISTENT_VERSION = PORTMANTEAU(CONSISTENT_MAJOR, CONSISTENT_MINOR);


// The version where the additional header information was added

const int FILEINFO_MAJOR = 2;
const int FILEINFO_MINOR = 0;
const int FILEINFO_VERSION = PORTMANTEAU(FILEINFO_MAJOR, FILEINFO_MINOR);

// The version where the mesh classes were added

const int MESH_MAJOR = 2;
const int MESH_MINOR = 0;
const int MESH_VERSION = PORTMANTEAU(MESH_MAJOR, MESH_MINOR);

// The version where extended curves and surfaces were introduced. 

const int EXT_CU_SF_MAJOR = 2;
const int EXT_CU_SF_MINOR = 1;
const int EXT_CU_SF_VERSION = PORTMANTEAU(EXT_CU_SF_MAJOR, EXT_CU_SF_MINOR);

// The version in which taper_spl_surs came into existence

const int TAPER_EXISTENCE_MAJOR = 2;
const int TAPER_EXISTENCE_MINOR = 1;
const int TAPER_EXISTENCE_VERSION = 
    PORTMANTEAU( TAPER_EXISTENCE_MAJOR, TAPER_EXISTENCE_MINOR );

// The version where coedges were given sense enumeration strings.

const int COEDGE_SENSE_MAJOR = 2;
const int COEDGE_SENSE_MINOR = 2;
const int COEDGE_SENSE_VERSION =
	PORTMANTEAU(COEDGE_SENSE_MAJOR, COEDGE_SENSE_MINOR);

// The version at which Skin/Loft surfaces have a subset range to bound them within
// their natural range.

const int ARCWISE_SKIN_MAJOR = 2;
const int ARCWISE_SKIN_MINOR = 2;
const int ARCWISE_SKIN_VERSION =
	PORTMANTEAU( ARCWISE_SKIN_MAJOR, ARCWISE_SKIN_MINOR );

// The version in which adv_var_blend attributes output a logical 
// specifying if two radii functions are used

const int ADV_VAR_BLEND_TWO_RADII_MAJOR = 2;
const int ADV_VAR_BLEND_TWO_RADII_MINOR = 2;
const int ADV_VAR_BLEND_TWO_RADII_VERSION = 
	PORTMANTEAU( ADV_VAR_BLEND_TWO_RADII_MAJOR, ADV_VAR_BLEND_TWO_RADII_MINOR);

// The version in which laws first where added to ACIS

const int LAW_MAJOR = 2;
const int LAW_MINOR = 2;
const int LAW_VERSION = PORTMANTEAU( LAW_MAJOR, LAW_MINOR );

// The version in which reflection of offset_spl_surs was handled

const int OFFSET_REV_MAJOR = 2;
const int OFFSET_REV_MINOR = 2;
const int OFFSET_REV_VERSION = PORTMANTEAU( OFFSET_REV_MAJOR, OFFSET_REV_MINOR);

// The version in which discontinuity information was stored in int_curs and
// spl_surs 

const int DISCONTINUITY_MAJOR = 3;
const int DISCONTINUITY_MINOR = 0;
const int DISCONTINUITY_VERSION = 
    PORTMANTEAU( DISCONTINUITY_MAJOR, DISCONTINUITY_MINOR);

// The version in which taper_spl_surs were split into derived classes

const int TAPER_MAJOR = 3;
const int TAPER_MINOR = 0;
const int TAPER_VERSION = 
    PORTMANTEAU( TAPER_MAJOR, TAPER_MINOR);

// The version in which net surface was added to ACIS

const int NET_SPL_MAJOR = 3;
const int NET_SPL_MINOR = 0;
const int NET_SPL_VERSION = PORTMANTEAU( NET_SPL_MAJOR, NET_SPL_MINOR );

// The version in which law curves and surfaces where added to ACIS

const int LAW_SPL_MAJOR = 4;
const int LAW_SPL_MINOR = 0;
const int LAW_SPL_VERSION = PORTMANTEAU( LAW_SPL_MAJOR, LAW_SPL_MINOR );

// The version at which cones have a new member, representing the
// scaling factor of the u SPAparameter.

const int CONE_SCALING_MAJOR = 4;
const int CONE_SCALING_MINOR = 0;
const int CONE_SCALING_VERSION =
	PORTMANTEAU( CONE_SCALING_MAJOR, CONE_SCALING_MINOR );

// The version in which laws in lofts where added to ACIS

const int LOFT_LAW_MAJOR = 4;
const int LOFT_LAW_MINOR = 0;
const int LOFT_LAW_VERSION = PORTMANTEAU( LOFT_LAW_MAJOR, LOFT_LAW_MINOR );

// The version in which refinement "min_u_grid_lines and min_v_grid_lines" were added. 

const int REF_MIN_UV_GRID_MAJOR = 4;
const int REF_MIN_UV_GRID_MINOR = 0;
const int REF_MIN_UV_GRID_VERSION =
	PORTMANTEAU(REF_MIN_UV_GRID_MAJOR,REF_MIN_UV_GRID_MINOR);

// The version at which vertex blend attributes were given a new member 
// describing the method of auto setback calculation, if any.

const int VBLEND_AUTO_MAJOR = 4;
const int VBLEND_AUTO_MINOR = 0;
const int VBLEND_AUTO_VERSION =
	PORTMANTEAU(VBLEND_AUTO_MAJOR,VBLEND_AUTO_MINOR);

// The version at which the true rolling-ball envelope surface was introduced
// for var_blend_spl_sur's.

const int BL_ENV_SF_MAJOR = 4;
const int BL_ENV_SF_MINOR = 0;
const int BL_ENV_SF_VERSION =
	PORTMANTEAU(BL_ENV_SF_MAJOR,BL_ENV_SF_MINOR);

// The version in which ellipes offset with an ellipse base curve so that they
// will extend the ellipse and not linearly.

const int ELLIPSE_OFFSET_MAJOR = 5;
const int ELLIPSE_OFFSET_MINOR = 0;
const int ELLIPSE_OFFSET_VERSION = PORTMANTEAU( ELLIPSE_OFFSET_MAJOR, ELLIPSE_OFFSET_MINOR );

// The version at which Tolerant Modeling was introduced.

const int TOL_MODELING_MAJOR = 5;
const int TOL_MODELING_MINOR = 0;
const int TOL_MODELING_VERSION =
	PORTMANTEAU(TOL_MODELING_MAJOR,TOL_MODELING_MINOR);

// The version in which approximating geometry could be stored in summary form. 

const int APPROX_SUMMARY_MAJOR = 5;
const int APPROX_SUMMARY_MINOR = 0;
const int APPROX_SUMMARY_VERSION =
	PORTMANTEAU( APPROX_SUMMARY_MAJOR, APPROX_SUMMARY_MINOR );

// The version at which a scaling factor was saved for tapers.

const int TAPER_SCALING_MAJOR = 5;
const int TAPER_SCALING_MINOR = 0;
const int TAPER_SCALING_VERSION =
	PORTMANTEAU(TAPER_SCALING_MAJOR,TAPER_SCALING_MINOR);

// The version at which lazy bspline approximations were introduced.

const int LAZY_B_SPLINE_MAJOR = 5;
const int LAZY_B_SPLINE_MINOR = 0;
const int LAZY_B_SPLINE_VERSION =
	PORTMANTEAU(LAZY_B_SPLINE_MAJOR,LAZY_B_SPLINE_MINOR);

// The version at which multi-surfaces were introduced to Deformable Modelling

const int DM_MULTI_SURF_MAJOR = 5;
const int DM_MULTI_SURF_MINOR = 0;
const int DM_MULTI_SURF_VERSION =
	PORTMANTEAU(DM_MULTI_SURF_MAJOR,DM_MULTI_SURF_MINOR);

// The version at which law_par_curs were introduced.

const int LAW_PAR_CUR_MAJOR = 5;
const int LAW_PAR_CUR_MINOR = 0;
const int LAW_PAR_CUR_VERSION =
	PORTMANTEAU(LAW_PAR_CUR_MAJOR,LAW_PAR_CUR_MINOR);

// The version at which Skin/Loft surfaces error should be recalulated
// so that the intersecor will not SPAbox out valid skin intersections.

const int RECAL_SKIN_ERROR_MAJOR = 5;
const int RECAL_SKIN_ERROR_MINOR = 2;
const int RECAL_SKIN_ERROR_VERSION =
	PORTMANTEAU( RECAL_SKIN_ERROR_MAJOR, RECAL_SKIN_ERROR_MINOR );

// The version at which the copy_owner method was added to ATTRIB_GEN_NAME.

const int GA_COPY_ACTION_MAJOR = 6;
const int GA_COPY_ACTION_MINOR = 0;
const int GA_COPY_ACTION_VERSION =
	PORTMANTEAU(GA_COPY_ACTION_MAJOR,GA_COPY_ACTION_MINOR);

// The version at which link constraint color persistence was introduced to Deformable Modelling

const int DM_MULTI_SURF_COLOR_MAJOR = 6;
const int DM_MULTI_SURF_COLOR_MINOR = 0;
const int DM_MULTI_SURF_COLOR_VERSION =
	PORTMANTEAU(DM_MULTI_SURF_COLOR_MAJOR,DM_MULTI_SURF_COLOR_MINOR);

// The version at which the fact whether the ruled taper was
// ruled in u or in v was saved for tapers.

const int TAPER_U_RULED_MAJOR = 6;
const int TAPER_U_RULED_MINOR = 0;
const int TAPER_U_RULED_VERSION =
	PORTMANTEAU(TAPER_U_RULED_MAJOR,TAPER_U_RULED_MINOR);

// Multiple changes to DM SAT file in 6.0
// including point tangent constraints
// and saving of default shapes

const int DM_60_MAJOR = 6;
const int DM_60_MINOR = 0;
const int DM_60_VERSION =
	PORTMANTEAU(DM_60_MAJOR,DM_60_MINOR);

// The version in which pcurves in lofts where added to ACIS

const int LOFT_PCURVE_MAJOR = 6;
const int LOFT_PCURVE_MINOR = 0;
const int LOFT_PCURVE_VERSION = 
	PORTMANTEAU( LOFT_PCURVE_MAJOR, LOFT_PCURVE_MINOR );

// The version in which an EELIST saves its owner and ownership policy

const int EELIST_OWNER_MAJOR = 6;
const int EELIST_OWNER_MINOR = 0;
const int EELIST_OWNER_VERSION =
	PORTMANTEAU( EELIST_OWNER_MAJOR, EELIST_OWNER_MINOR );

// The version in which an ANNOTATION saves whether members are hooked

const int ANNO_HOOKED_MAJOR = 7;
const int ANNO_HOOKED_MINOR = 0;
const int ANNO_HOOKED_VERSION =
	PORTMANTEAU( ANNO_HOOKED_MAJOR, ANNO_HOOKED_MINOR );

// The version in which patterns were added to ACIS

const int PATTERN_MAJOR = 7;
const int PATTERN_MINOR = 0;
const int PATTERN_VERSION =
	PORTMANTEAU( PATTERN_MAJOR, PATTERN_MINOR );

// The version in which tags were added to ENTITYs

const int ENTITY_TAGS_MAJOR = 7;
const int ENTITY_TAGS_MINOR = 0;
const int ENTITY_TAGS_VERSION =
	PORTMANTEAU( ENTITY_TAGS_MAJOR, ENTITY_TAGS_MINOR );

// The version at which strings were first distinguished
// in text files by a @ sign.

const int AT_MAJOR = 7;
const int AT_MINOR = 0;
const int AT_VERSION = PORTMANTEAU( AT_MAJOR, AT_MINOR );

// The version in which net law surface were added to ACIS

const int NET_LAW_MAJOR = 7;
const int NET_LAW_MINOR = 0;
const int NET_LAW_VERSION = PORTMANTEAU( NET_LAW_MAJOR, NET_LAW_MINOR );

// The version in which the "bulletin" and "bulletin_board" strings
// were removed from the history portion of the save file.

const int STRINGLESS_HISTORY_MAJOR = 7;
const int STRINGLESS_HISTORY_MINOR = 0;
const int STRINGLESS_HISTORY_VERSION = 
	PORTMANTEAU( STRINGLESS_HISTORY_MAJOR, STRINGLESS_HISTORY_MINOR );

// The version in which the rendering husk material
// will save and restore the enabling flags.

const int RBASE_MATERIAL_ENABLE_MAJOR = 8;
const int RBASE_MATERIAL_ENABLE_MINOR = 0;
const int RBASE_MATERIAL_ENABLE_VERSION = 
	PORTMANTEAU( RBASE_MATERIAL_ENABLE_MAJOR, RBASE_MATERIAL_ENABLE_MINOR );

// The version in which the ruled surface came into existence

const int RULED_SPL_MAJOR = 8;
const int RULED_SPL_MINOR = 0;
const int RULED_SPL_ENABLE_VERSION = 
	PORTMANTEAU( RULED_SPL_MAJOR, RULED_SPL_MINOR );

// The version in which the bounding boxes will be saved and restored.
const int SAVE_BOX_MAJOR = 8;
const int SAVE_BOX_MINOR = 0;
const int SAVE_BOX_VERSION = 
	PORTMANTEAU( SAVE_BOX_MAJOR, SAVE_BOX_MINOR );

// The version in which users may save their sat files using "summary mode"
const int SAVE_SUMMARY_MODE_MAJOR = 8;
const int SAVE_SUMMARY_MODE_MINOR = 0;
const int SAVE_SUMMARY_MODE_VERSION = PORTMANTEAU(SAVE_SUMMARY_MODE_MAJOR, SAVE_SUMMARY_MODE_MINOR);

// The version in which face uv SPAbox is saved and restored
const int SAVE_UV_BOX_MAJOR = 10;
const int SAVE_UV_BOX_MINOR = 0;
const int SAVE_UV_BOX_VERSION = 
	PORTMANTEAU( SAVE_UV_BOX_MAJOR, SAVE_UV_BOX_MINOR );

// The version in which loop type is saved and restored
const int SAVE_LOOP_TYPE_MAJOR = 10;
const int SAVE_LOOP_TYPE_MINOR = 0;
const int SAVE_LOOP_TYPE_VERSION = 
	PORTMANTEAU( SAVE_LOOP_TYPE_MAJOR, SAVE_LOOP_TYPE_MINOR );

// The version in which pattern index is saved and restored
const int SAVE_PATTERN_IDX_MAJOR = 11;
const int SAVE_PATTERN_IDX_MINOR = 0;
const int SAVE_PATTERN_IDX_VERSION = 
	PORTMANTEAU( SAVE_PATTERN_IDX_MAJOR, SAVE_PATTERN_IDX_MINOR );

// nay: Reset the safe uv box version to 12 to fix top10 79933.
// The version before which some uv boxes could be too small and need to be
// re-calculated. (STL dal 16Dec02)
const int SAFE_UV_BOX_MAJOR = 12;
const int SAFE_UV_BOX_MINOR = 0;
const int SAFE_UV_BOX_VERSION = 
	PORTMANTEAU( SAFE_UV_BOX_MAJOR, SAFE_UV_BOX_MINOR );

// The version in which parametric tolerance is saved and restored
const int PARTOL_MAJOR = 11;
const int PARTOL_MINOR = 0;
const int PARTOL_VERSION = 
	PORTMANTEAU( PARTOL_MAJOR, PARTOL_MINOR );

// The version in which envelope_taper_spl_surs came into existence
const int ENVELOPE_TAPER_MAJOR = 11;
const int ENVELOPE_TAPER_MINOR = 0;
const int ENVELOPE_TAPER_VERSION = 
    PORTMANTEAU( ENVELOPE_TAPER_MAJOR, ENVELOPE_TAPER_MINOR );

// The version in which const_extensions flag of var_radius is saved and restored
const int VARRAD_CONSTEXT_MAJOR = 11;
const int VARRAD_CONSTEXT_MINOR = 0;
const int VARRAD_CONSTEXT_VERSION = PORTMANTEAU( VARRAD_CONSTEXT_MAJOR, VARRAD_CONSTEXT_MINOR );

// The version in which tight spline face boxes are saved and restored (STL amt 23Jun03)
const int TIGHT_SPLINE_BOXES_MAJOR = 12;
const int TIGHT_SPLINE_BOXES_MINOR = 0;
const int TIGHT_SPLINE_BOXES_VERSION = 
	PORTMANTEAU( TIGHT_SPLINE_BOXES_MAJOR, TIGHT_SPLINE_BOXES_MINOR );

// The version in which tight int_cur approximations came to be
const int TIGHT_INT_CUR_APPROX_MAJOR = 12;
const int TIGHT_INT_CUR_APPROX_MINOR = 0;
const int TIGHT_INT_CUR_APPROX_VERSION =
	PORTMANTEAU( TIGHT_INT_CUR_APPROX_MAJOR, TIGHT_INT_CUR_APPROX_MINOR );

// The version at which Sweep surfaces deal with non-perpendicular draft is handled 
// with respect to the progenitor surface's normal, not the profile's normal.
const int NONPERP_DRAFT_SWEEP_MAJOR = 12;
const int NONPERP_DRAFT_SWEEP_MINOR = 0;
const int NONPERP_DRAFT_SWEEP_VERSION =
	PORTMANTEAU( NONPERP_DRAFT_SWEEP_MAJOR , NONPERP_DRAFT_SWEEP_MINOR );

// The version in which blend_spl_sur's save/restore new 3 curve evaluator data (bwc 19 Jun 2003)
const int BLND_SPL_SUR_3_CRV_EVAL_MAJOR = 12;
const int BLND_SPL_SUR_3_CRV_EVAL_MINOR = 0;
const int BLND_SPL_SUR_3_CRV_EVAL_VERSION = 
	PORTMANTEAU( BLND_SPL_SUR_3_CRV_EVAL_MAJOR, BLND_SPL_SUR_3_CRV_EVAL_MINOR );
// The version in which off_spl_sur can use the approximation of the progenitor optionally.
// Not yet ready but sat file change is now complete.
const int OFF_SPL_SUR_APPROX_MAJOR = 13;
const int OFF_SPL_SUR_APPROX_MINOR = 0;
const int OFF_SPL_SUR_APPROX_VERSION = 
	PORTMANTEAU( OFF_SPL_SUR_APPROX_MAJOR, OFF_SPL_SUR_APPROX_MINOR );

// The version at which bisector int curves have been first implemented
const int BISECTOR_INTCURVE_MAJOR = 13;
const int BISECTOR_INTCURVE_MINOR = 0;
const int BISECTOR_INTCURVE_VERSION =
	PORTMANTEAU( BISECTOR_INTCURVE_MAJOR , BISECTOR_INTCURVE_MINOR );

// The version at which Mixed Convexity edges was introduced.

const int MIXED_CONVEXITY_MAJOR = 14;
const int MIXED_CONVEXITY_MINOR = 0;
const int MIXED_CONVEXITY_VERSION =
	PORTMANTEAU(MIXED_CONVEXITY_MAJOR,MIXED_CONVEXITY_MINOR);

// The version in which the new constrained evaluation was introduced
// for pointy end blends. (blp, July, 26, 2004)
const int BLEND_CONSTRAINED_EVAL_MAJOR = 14;
const int BLEND_CONSTRAINED_EVAL_MINOR = 0;
const int BLEND_CONSTRAINED_EVAL_VERSION =
	PORTMANTEAU(BLEND_CONSTRAINED_EVAL_MAJOR,BLEND_CONSTRAINED_EVAL_MINOR);

// The version at which extensions of surf_int_curs are made.
const int SURF_INT_CUR_EXTEND_MAJOR = 14;
const int SURF_INT_CUR_EXTEND_MINOR = 0;
const int SURF_INT_CUR_EXTEND_VERSION =
	PORTMANTEAU(SURF_INT_CUR_EXTEND_MAJOR,SURF_INT_CUR_EXTEND_MINOR);

// The version in which the new var-R marching/evaluation was introduced (bsg, September 21, 2004)
const int BLEND_VARR_PT_MARCH_MAJOR = 15;
const int BLEND_VARR_PT_MARCH_MINOR = 0;
const int BLEND_VARR_PT_MARCH_VERSION =
	PORTMANTEAU(BLEND_VARR_PT_MARCH_MAJOR,BLEND_VARR_PT_MARCH_MINOR);

// The version at which SAT files no longer have a line for fmesh-eye-attrib or ptlist-eye-attrib.
const int SAT_MESH_LINE_MAJOR = 15;
const int SAT_MESH_LINE_MINOR = 0;
const int SAT_MESH_LINE_VERSION = 
	PORTMANTEAU(SAT_MESH_LINE_MAJOR, SAT_MESH_LINE_MINOR);

// The version in which u-derivative based blend u-extension was introduced

const int BLEND_HERMITE_UINTERP_MAJOR = 15;
const int BLEND_HERMITE_UINTERP_MINOR = 0;
const int BLEND_HERMITE_UINTERP_VERSION =
	PORTMANTEAU( BLEND_HERMITE_UINTERP_MAJOR, BLEND_HERMITE_UINTERP_MINOR );

// The version in 3 curve evaluator curves are saved in the summary mode.

const int BLND_SPL_SUR_3_CRV_SUMMARY_MAJOR = 15;
const int BLND_SPL_SUR_3_CRV_SUMMARY_MINOR = 0;
const int BLND_SPL_SUR_3_CRV_SUMMARY_VERSION =
	PORTMANTEAU( BLND_SPL_SUR_3_CRV_SUMMARY_MAJOR, BLND_SPL_SUR_3_CRV_SUMMARY_MINOR );

// The version in which tight approximations could be stored in summary form. 

const int TIGHT_APPROX_SUMMARY_MAJOR = 16;
const int TIGHT_APPROX_SUMMARY_MINOR = 0;
const int TIGHT_APPROX_SUMMARY_VERSION =
	PORTMANTEAU( TIGHT_APPROX_SUMMARY_MAJOR, TIGHT_APPROX_SUMMARY_MINOR );


// The version in which SPACOLLECTION is introduced

const int SPACOLLECTION_MAJOR = 16;
const int SPACOLLECTION_MINOR = 0;
const int SPACOLLECTION_VERSION =
	PORTMANTEAU( SPACOLLECTION_MAJOR, SPACOLLECTION_MINOR );

// The version in which the base ATTRIB class started to save its behaviors to sat.
// (Makes handling unknown attributes easier.)

const int ATTRIB_BEHAVIORS_MAJOR = 16;
const int ATTRIB_BEHAVIORS_MINOR = 0;
const int ATTRIB_BEHAVIORS_VERSION  =
	PORTMANTEAU( ATTRIB_BEHAVIORS_MAJOR, ATTRIB_BEHAVIORS_MINOR);

// The version at which periodic spline faces without prop edges were supported.
const int PERIODIC_SPLINE_FACES_WITHOUT_PROPS_MAJOR = 16;
const int PERIODIC_SPLINE_FACES_WITHOUT_PROPS_MINOR = 0;
const int PERIODIC_SPLINE_FACES_WITHOUT_PROPS_VERSION =
    PORTMANTEAU( PERIODIC_SPLINE_FACES_WITHOUT_PROPS_MAJOR, PERIODIC_SPLINE_FACES_WITHOUT_PROPS_MINOR );

// The version at which sweep_spl_sur supports rigid sweep with twist
const int SWEEP_SPL_RIGID_WITH_TWIST_MAJOR = 17;
const int SWEEP_SPL_RIGID_WITH_TWIST_MINOR = 0;
const int SWEEP_SPL_RIGID_WITH_TWIST_VERSION =
	PORTMANTEAU (SWEEP_SPL_RIGID_WITH_TWIST_MAJOR , SWEEP_SPL_RIGID_WITH_TWIST_MINOR);

// The version at which helix curves were introduced
const int HELIX_CURVE_MAJOR = 17;
const int HELIX_CURVE_MINOR = 0;
const int HELIX_CURVE_VERSION =
	PORTMANTEAU (HELIX_CURVE_MAJOR , HELIX_CURVE_MINOR);

// The version in which assemblies were introduced
const int ASSEMBLY_MAJOR = 17;
const int ASSEMBLY_MINOR = 0;
const int ASSEMBLY_VERSION =
	PORTMANTEAU (ASSEMBLY_MAJOR , ASSEMBLY_MINOR);

// The version in which offset_int_cur supports linear extension
const int OFFSET_INT_CUR_EXTENSION_MAJOR = 17;
const int OFFSET_INT_CUR_EXTENSION_MINOR = 0;
const int OFFSET_INT_CUR_EXTENSION_VERSION =
	PORTMANTEAU (OFFSET_INT_CUR_EXTENSION_MAJOR , OFFSET_INT_CUR_EXTENSION_MINOR);

// In R16, we introduced the ATTRIB_GEN_WSTRING which allows apps to
// store a wide string in a GA. When saved to SAT/B, we would first convert
// the wide data to utf8 then save - the downside being that we were 
// writing control characters to SAT. While that never caused a problem,
// it didn't seem like a good thing in retrospect. So now in R17 after 
// converting the wide string to utf8 we add another step to encode the
// data. Our encoding set is restricted to valid printable ascii chars.
const int WIDE_STR_ENCODED_MAJOR = 17;
const int WIDE_STR_ENCODED_MINOR = 0;
const int WIDE_STR_ENCODED_VERSION =
	PORTMANTEAU (WIDE_STR_ENCODED_MAJOR , WIDE_STR_ENCODED_MINOR);

// Added support for various cross sections in ATT_BL_CR.
// Also it takes different left and right offset values.
const int BLEND_CR_ALL_XSECT_MAJOR = 18;
const int BLEND_CR_ALL_XSECT_MINOR = 0;
const int BLEND_CR_ALL_XSECT_VERSION = 
	PORTMANTEAU (BLEND_CR_ALL_XSECT_MAJOR ,BLEND_CR_ALL_XSECT_MINOR );

// R18 onwards, entity blends [blends in which inputs are support entities
// rather than edges] are processed through ATT_BL_ENT_MGR which is capable
// of taking multiple entities in a single support list. This new attribute
// will cater abl:ents-ents-blend, abl:set-cc-blend, abl:blend-three-ent.
// It will not get saved in versions previous to R18.
const int BLEND_ENT_MGR_MAJOR = 18;
const int BLEND_ENT_MGR_MINOR = 0;
const int BLEND_ENT_MGR_VERSION = 
	PORTMANTEAU (BLEND_ENT_MGR_MAJOR ,BLEND_ENT_MGR_MINOR );

// var_rad_const is being used in blending cases R18 onwards
const int SAVE_VAR_RAD_CONST_MAJOR = 18;
const int SAVE_VAR_RAD_CONST_MINOR = 0;
const int SAVE_VAR_RAD_CONST_VERSION = 
	PORTMANTEAU ( SAVE_VAR_RAD_CONST_MAJOR , SAVE_VAR_RAD_CONST_MINOR );


// The version in which curvature continuous blends were introduced.
const int BLEND_CURVATURE_CONTINUOUS_MAJOR = 18;
const int BLEND_CURVATURE_CONTINUOUS_MINOR = 0;
const int BLEND_CURVATURE_CONTINUOUS_VERSION = 
	PORTMANTEAU ( BLEND_CURVATURE_CONTINUOUS_MAJOR, BLEND_CURVATURE_CONTINUOUS_MINOR );

// The version in which owned collections were needed to support blend face attributes.
const int OWNED_COLLECTIONS_MAJOR = 18;
const int OWNED_COLLECTIONS_MINOR = 0;
const int OWNED_COLLECTIONS_VERSION = 
	PORTMANTEAU ( OWNED_COLLECTIONS_MAJOR, OWNED_COLLECTIONS_MINOR );

// The version at which sweep_spl_sur supports fixed axis sweeping
const int SWEEP_SPL_FIXED_AXIS_MAJOR = 19;
const int SWEEP_SPL_FIXED_AXIS_MINOR = 0;
const int SWEEP_SPL_FIXED_AXIS_VERSION =
	PORTMANTEAU (SWEEP_SPL_FIXED_AXIS_MAJOR , SWEEP_SPL_FIXED_AXIS_MINOR);

// The version in which defining curve is saved as const chamfer attribute.
const int CHAMFER_ATTRIB_DEF_CURVE_MAJOR = 21;
const int CHAMFER_ATTRIB_DEF_CURVE_MINOR = 0;
const int CHAMFER_ATTRIB_DEF_CURVE_VERSION = 
	PORTMANTEAU ( CHAMFER_ATTRIB_DEF_CURVE_MAJOR, CHAMFER_ATTRIB_DEF_CURVE_MINOR );

// The version in which thread-hot save and resture were introduced.
const int MULTI_BODY_BLOCKS_MAJOR = 25;
const int MULTI_BODY_BLOCKS_MINOR = 0;
const int MULTI_BODY_BLOCKS_VERSION = 
	PORTMANTEAU ( MULTI_BODY_BLOCKS_MAJOR, MULTI_BODY_BLOCKS_MINOR );

// The version in which we identify save data generated in ACIS.
const int DATA_ORIGIN_MAJOR = 25;
const int DATA_ORIGIN_MINOR = 0;
const int DATA_ORIGIN_VERSION = 
	PORTMANTEAU ( DATA_ORIGIN_MAJOR, DATA_ORIGIN_MINOR );

// The version in which we summarized generic attribute names.
const int SUMMARIZE_STRINGS_MAJOR = 26;
const int SUMMARIZE_STRINGS_MINOR = 0;
const int SUMMARIZE_STRINGS_VERSION = 
	PORTMANTEAU ( SUMMARIZE_STRINGS_MAJOR, SUMMARIZE_STRINGS_MINOR );

// The version in which F_BODY ( for polyhedral component ) was introduced.
const int FACET_BODY_MAJOR = 26;
const int FACET_BODY_MINOR = 0;
const int FACET_BODY_VERSION = PORTMANTEAU ( FACET_BODY_MAJOR, FACET_BODY_MINOR );

// The version in which net_spl_sur corner data is saved and restored.
const int NET_CORNERS_CACHE_MAJOR = 26;
const int NET_CORNERS_CACHE_MINOR = 0;
const int NET_CORNERS_CACHE_VERSION = 
	PORTMANTEAU ( NET_CORNERS_CACHE_MAJOR, NET_CORNERS_CACHE_MINOR );

// The version in which HYBRID flag in BODY ( for polyhedral component ) was introduced.
const int HYBRID_BODY_MAJOR = 27;
const int HYBRID_BODY_MINOR = 0;
const int HYBRID_BODY_VERSION = PORTMANTEAU ( HYBRID_BODY_MAJOR, HYBRID_BODY_MINOR );

// The version in which IOPPID attribute was introduced.
const int IOPPID_ATTRIB_DEF_MAJOR = 28;
const int IOPPID_ATTRIB_DEF_MINOR = 0;
const int IOPPID_ATTRIB_DEF_VERSION = PORTMANTEAU(IOPPID_ATTRIB_DEF_MAJOR, IOPPID_ATTRIB_DEF_MINOR);

#endif
