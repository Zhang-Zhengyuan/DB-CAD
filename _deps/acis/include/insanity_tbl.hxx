// $Id: insanity_tbl.hxx,v 1.18 2002/08/23 20:49:56 bloyd Exp $
/*******************************************************************/
/*    Copyright (c) 1989-2020 by Spatial Corp.                     */
/*    All rights reserved.                                         */
/*    Protected by U.S. Patents 5,257,205; 5,351,196; 6,369,815;   */
/*                              5,982,378; 6,462,738; 6,941,251    */
/*    Protected by European Patents 0503642; 69220263.3            */
/*    Protected by Hong Kong Patent 1008101A                       */
/*******************************************************************/

#ifndef INSANITY_TBL
#define INSANITY_TBL

// Replaced with the following with a msgsrc
/* // - start comment 
// Symbols used in intr/intersct/sg_husk/sanity/sanity.cxx
#define   	ACIS_ERROR					0
#define     APPROX_TEST_FAIL			1
#define     BAD_CLOSURE					2
#define     BAD_CRV_CLS					3
#define     BAD_CTL_PNT_COIN			4
#define     BAD_FACE_LOOP				5
#define     BAD_SINGULAR				6
#define     BAD_TCOED_DOMAIN			7
#define     BAD_VTX_EDGE_PTR			8
#define     BODY_FAIL_CONT_CHK			9
#define     BODY_WARN_CONT_CHK			10
#define     BODY_WITHOUT_LUMP			11
#define     BS3C_CLOSED_MRKD_OPEN		12
#define     BS3C_CLOSED_MRKD_PEROID		13
#define   	BS3C_OPEN_NOT_MRKD_OPEN		14
#define   	BS3C_PEROID_MRKD_CLOSED		15
#define   	BS3S_U_NOT_CLOSED			16
#define   	BS3S_U_NOT_OPEN				17
#define   	BS3S_U_NOT_PERIODIC			18
#define   	BS3S_V_NOT_CLOSED			19
#define   	BS3S_V_NOT_OPEN				20
#define   	BS3S_V_NOT_PERIODIC			21
#define   	COED_EDGE_BAD_PTR			22
#define   	COED_END_OUTSIDE_TOL_VTX	23
#define   	COED_END_VTX_ON_POLE		24
#define   	COED_HAS_TEDGE				25
#define   	COED_NEXT_NOT_WELL_CONN		26
#define   	COED_NO_EDGE				27
#define   	COED_NO_OWNER				28
#define   	COED_NO_PCUR				29
#define   	COED_NO_PTR_TO_LOOP			30
#define   	COED_NOT_POINT_TO_WIRE		31
#define   	COED_OUT_OF_ORDER			32
#define   	COED_POS_OFF_FACE			33
#define   	COED_PREV_NOT_WELL_CONN		34
#define   	COED_RANGE_NOT_CONT			35
#define   	COED_START_OUTSIDE_TOL_VTX	36
#define   	COED_START_VTX_ON_POLE		37
#define   	CONE_ANGLE_INCONS			38
#define   	CONE_ELL_NORM_NOT_UNITVEC	39
#define   	CONE_ELL_NOT_PERP_TO_AXIS	40
#define   	CONE_ELL_RATIO_GT_1			41
#define   	CONE_ELL_ZERO_MAJ_AXIS		42
#define   	CONE_ELL_ZERO_RATIO			43
#define   	CONVEX_POINT				44
#define   	CRV_BAD_DEGEN				45
#define   	CRV_BAD_SINGULAR			46
#define   	CRV_NOT_C1					47
#define   	CRV_NOT_G0					48
#define   	CRV_NOT_G1					49
#define   	CRV_NOT_G2					50
#define   	CRV_NOT_ON_SURF				51
#define   	CRV_SELF_INT				52
#define   	DIFF_LOC					53
#define   	DIFF_PCUR_KNOT_TAN			54
#define   	DUP_VTX						55
#define   	EDGE_BOX_TOO_SMALL			56
#define   	EDGE_NO_BACKPTR				57
#define   	EDGE_NO_CRV1				58
#define   	EDGE_NO_EVTX				59
#define   	EDGE_NO_SVTX				60
#define   	EDGE_PCUR_OUT_OF_RANGE		61
#define   	ELL_NORM_NOT_UNITVEC		62
#define   	ELL_NOT_PERP				63
#define   	ELL_RATIO_GT_1				64
#define   	ELL_ZERO_MAJ_AXIS			65
#define   	ELL_ZERO_RATIO				66
#define   	END_INCONS_TOL_GEOM			67
#define   	END_VTX_NOT_ON_SURF			68
#define   	EPAR_ERR					69
#define   	EVTX_NOT_ON_CRV				70
#define   	EXTREME_PAR					71
#define   	FACE_BAD_LOOP_ORIENT		72
#define   	FACE_BOX_TOO_SMALL			73
#define   	FACE_NEGA_AREA				74
#define   	FACE_NO_BACKPTR				75
#define   	FACE_NO_SURF1				76
#define   	FACE_NO_SURF2				77
#define   	FACE_NOT_POINT_TO_SHL		78
#define   	FACE_WITHOUT_LOOP			79
#define   	FACE_ZERO_AREA				80
#define   	INCONS_DIR					81
#define   	INCONS_PERIOD				82
#define   	IRREG_CRV					83
#define   	IRREG_SURF					84
#define   	ISO_VTX_EMBEDDED			85
#define   	LAW_APPROX_ERR				86
#define   	LOOP_NO_COED				87
#define   	LOOP_NO_FACE				88
#define   	LUMP_NOT_POINT_TO_BODY		89
#define   	LUMP_WITHOUT_BODY			90
#define   	LUMP_WITHOUT_SHL			91
#define   	MULT_EDGE					92
#define   	NEXT_NOT_TO_START			93
#define   	NO_BS3CRV					94
#define   	NO_BS3SURF					95
#define   	NO_PARTNER					96
#define   	NULL_EDGE_AT_APEX			97
#define   	NULL_EDGE_CURVE				98
#define   	OPEN_IN_NEXT				99
#define		OPEN_IN_PREV				100
#define   	OVERLAP_TVERT				101
#define   	OVERLAP_VERT				102
#define   	PARAM_TEST_FAIL				103
#define   	PARTNER_MISMATCH			104
#define   	PCUR_CLOSED_DIFF_INTCUR		105
#define   	PCUR_DIFF_LOC				106
#define   	PCUR_DIFF_PERIOD			107
#define   	PCUR_EVAL_PROB				108
#define   	PCUR_HULL_SELF_INT			109
#define   	PCUR_HULL_SHARP_ANG			110
#define   	PCUR_KNOT_NOT_ON_CRV		111
#define   	PCUR_NO_CUR					112
#define   	PCUR_NOT_CLOSED				113
#define   	PCUR_NOT_OPEN				114
#define   	PCUR_NOT_OPEN_ON_STR		115
#define   	PCUR_NOT_PERIODIC			116
#define   	PCUR_OPEN_DIFF_INTCUR		117
#define   	PCUR_OUT_OF_RANGE			118
#define   	PCUR_PEROID_DIFF_INTCUR		119
#define   	PCUR_SURF_DIFF				120
#define   	PCUR_TOO_SHORT				121
#define   	PCUR_UNKNOWN_FORM			122
#define   	PLN_NORM_NOT_UNITVEC		123
#define   	POINT_PERP_TEST_FAIL		124
#define   	PREV_NOT_TO_START			125
#define   	PROC_CRV_DIFF_APPROX		126
#define   	PROC_SURF_DIFF_APPROX		127
#define   	REV_APPROX_CRV				128
#define   	SHL_NOT_POINT_TO_LUMP		129
#define   	SHL_WITHOUT_FACE			130
#define   	SHL_WITHOUT_LUMP			131
#define   	SPAR_ERR					132
#define   	SPH_ZERO_RAD				133
#define   	SPL_CRV_NO_APPROX			134
#define   	SPLSUR_NO_APPROX			135
#define   	START_INCONS_TOL_GEOM		136
#define   	START_VTX_NOT_ON_SURF		137
#define   	STR_NOT_UNITVEC				138
#define   	STR_ZERO_PAR_SCALE			139
#define   	SUB_SPL_SUR_FOUND			140
#define   	SURF_BAD_DEGEN				141
#define   	SURF_FAIL_DERIV_TEST		142
#define   	SURF_FAIL_EXT_TEST			143
#define   	SURF_NOT_C1					144
#define   	SURF_NOT_G0					145
#define   	SURF_NOT_G1					146
#define   	SURF_NOT_G2					147
#define   	SURF_SEFL_INT				148
#define   	SVTX_NOT_ON_CRV				149
#define   	TCOED_BAD_CTL_PNT_COIN		150
#define   	TCOED_BAD_DEGEN				151
#define   	TCOED_BAD_SINGULAR			152
#define   	TCOED_DIFF_RANGE			153
#define   	TCOED_HAS_EDGE				154
#define   	TCOED_IRREG					155
#define   	TCOED_NO_BS3CUR				156
#define   	TCOED_NO_PCUR				157
#define   	TCOED_NOT_C1				158
#define   	TCOED_NOT_G0				159
#define   	TCOED_NOT_G1				160
#define   	TCOED_NOT_G2				161
#define   	TCOED_OFF_SURF				162
#define   	TCOED_PCUR_NOT_G1			163
#define   	TCOED_SELF_INT				164
#define   	TCOED_STRAY					165
#define   	TCOED_WRONG_CLOSURE			166
#define   	TEDGE_BOX_NOT_INT_VTX_BOX	167
#define   	TEDGE_HAS_LOCAL_SELF_INT	168
#define   	TEDGE_NULL_PAR_RANGE		169
#define   	TOL_BAD_FACE_BOX			170
#define   	TOL_GEOM_INCOMPATIBLE		171
#define   	TRANS_TEST_FAIL				172
#define   	TRANSF_BAD_DETERMINENT		173
#define   	TRANSF_BAD_REFLECTION		174
#define   	TRANSF_LARGE_TRANSLATION	175	
#define   	TRANSF_NOT_IDENTITY			176
#define   	TRANSF_NOT_UNIT_VEC			177
#define   	TRS_BAD_LOOP				178
#define   	TRS_BAD_MAJ_RAD				179
#define   	TRS_NORM_NOT_UNITVEC		180
#define   	TRS_ZERO_MAJ_RAD			181
#define   	TRS_ZERO_MIN_RAD			182
#define   	UNBOUND_CONE_FACE			183
#define   	VTX_NO_EDGE					184
#define   	VTX_NO_PNT					185
#define   	VTX_NOT_SHARED				186
#define   	WIRE_NOT_POINT_TO_SHL		187
#define   	WIRE_WITHOUT_COED			188
#define   	WIRE_WITHOUT_OWNER			189
#define   	WRONG_FACE_BACKPTR			190

// Symbols used in the files in api_check_entity_ff_ints in BOOL.
#define		FF_INT_EDGE_INT				191
#define		FF_INT_LMP_CONT				192
#define		FF_INT_LMP_INT				193
#define		FF_INT_FACE_COIN			194
#define		FF_INT_FACE_INT				195
#define		FF_INT_FAIL					196
#define		FF_INT_SHL_CONT				197
#define		FF_INT_SHL_INT				198

// Symbols used in the files in api_check_ct in CT.
#define		CT_2D_CELL_NO_CFACE				199
#define		CT_ATTRIB_NOT_POINT_TO_FACE		200
#define		CT_CFACE_NOT_POINT_TO_FACE		201
#define		CT_CFACE_NOT_POINT_TO_OWNER		202
#define		CT_CSHL_NO_CFACE				203
#define		CT_DIFF_INWARD_OWNER			204
#define		CT_DSF_NO_BACK_CFACE			205
#define		CT_DSF_NO_FRONT_CFACE			206
#define		CT_NO_ATTRIB					207
#define		CT_NO_CLOSED_INWARD				208
#define		CT_NO_CSHL						209
#define		CT_NO_OWNER						210
#define		CT_NOT_IN_HIERACHY				211
#define		CT_NOT_INSIDE_CSHL				212
#define		CT_NOT_POINT_TO_CELL			213
#define		CT_NOT_POINT_TO_LUMP			214
#define		CT_NOT_VALIDATED				215
#define		CT_POINT_TO_NO_ATTRIB_FACE		216
#define		CT_SSF_NO_BACK_CFACE			217
#define		CT_WRONG_SENSE_OR_ATTRIB		218
#define         INTERNAL_LAW_IS_BAD             219

// STL jb: New symbol used in intr/intersct/sg_husk/sanity/sanity.cxx
#define 	COED_RANGE_NOT_CONT_1           220

// GSSL hcm (01/22/02) begin: bug 67458
// Symbols used in checking the cross links of attribs 
#define		ATTRIB_CORRUPT_CHAIN			221
#define		ATTRIB_BAD_OWNER				222
// GSSL hcm (01/22/02) end: bug 67458

// STL dal and yz (05/02/02): New symbol used in intr/intersct/sg_husk/sanity/sanity.cxx
#define		PARTNER_POINT_TO_ITSELF				223

// ywoo 7Feb02: sliver face checking
#define		SLIVER_FACE_DETECTED			224
#define		SLIVER_FACE_REMOVED			225

// tgg 12Feb02: vertex chain checking
#define		VERTEX_CHAIN_OPEN					226
#define		VERTEX_COEDGE_CLOSURE_MISMATCH		227
#define		NO_CW_VERTEX_CLOSURE				228
#define		NO_CCW_VERTEX_CLOSURE				229
#define		COEDGE_CW_START_MISMATCH			230
#define		COEDGE_CCW_END_MISMATCH				231
#define     COEDGE_CW_NEXT_INCONSISTENT			232
#define		COEDGE_CCW_NEXT_INCONSISTENT		233

// sbg 21Feb02: convexity check on tangent edge if version>=8.0
#define		CVXTY_PTS_ON_TANGENT_EDGE     234
#define     CVXTY_ON_EDGE_MARKED_TANGENT_WARN    235
#define     CVXTY_ON_EDGE_MARKED_TANGENT    236

// apc 30May02: intcurve supporting surface not defined
//		pcurve not checked due to curve evaluation failure
// both used in sanity.cxx
#define		INTCRV_INVALID_SUPPORT_SURF			237
#define		PCUR_CURVE_EVAL_FAILURE				238

// BWC-START 1Aug02: error messages for cases when surface errors
// are found when checking curves and when curve
// errors are found when checking surfaces
#define		IRREG_SUPPORT_SURF				239
#define		SUPPORT_SURF_SELF_INT			240
#define		BAD_SUPPORT_SURF_CLS			241
#define		NO_BS3_SUPPORT_SURF				242
#define		BAD_SUPPORT_SURF_CTL_PNT_COIN	243
#define		SUPPORT_SURF_BAD_DEGEN			244
#define		SUPPORT_SURF_BAD_SINGULAR		245
#define		SUPPORT_SURF_NOT_G0				246
#define		SUPPORT_SURF_NOT_G1				247
#define		SUPPORT_SURF_NOT_G2				248
#define		SUPPORT_SURF_NOT_C1				249
// BWC-END

// Skinning check for easier debugging
#define     SURF_BAD_APPROX                 250
// STI rr 10/08/02 begin: If the curve is near tangent to 1 degree at discontinuities
// we may have chosen not to split at such discontinuities. Warn appropriately.
#define		CRV_NOT_G1_NEAR_TAN				251
#define		SUPPORT_SURF_NOT_G1_NEAR_TAN	252
// STI rr end.

// STL apc 17Jan03 Need an error to report a failure to calculate the area of
// a face (bug 70703)
#define	    FACE_AREA_FAILURE               253
// STL jb 29 Jan 03 New warning if silver face test didn't work
#define 	NO_SLIVER_FACE_TEST				254
// jb end
// STL apc 13Feb03 Error for reporting an illegal vertex blend (bug 70973)
#define		BAD_VERTEX_BLEND				255
// ...and a generic 'unexpected problem' type (some surface check_statuses are
// not translated to insanities)
#define		UNEXPECTED_PROBLEM				256
#define     SURF_NOT_G1_NEAR_TAN            257
// STL apc 28Feb03 Warning about encountering an evaluation failure on a curve
// duringchecking. (bug 71081) 
#define 	CURVE_EVAL_FAILURE				258
// STL apc 17Mar03 edge/tedge not on face errors
#define     EDGE_OFF_FACE					259
#define     TEDGE_OFF_FACE					260
//STL nay 24Mar03 - If subsetted u_range/v_range is not within or equal to
//the u_range/v_range of Base spline surface. (67038)
#define     BAD_SUBSET_RANGE                261
#define     SUBSET_RANGE_MODIFIED           262
#define		BAD_FACE_DOMAIN					263
#define     CURVE_MISSING_DISC_INFO	        264
#define     SURF_MISSING_DISC_INFO          265
// STL apc 08May03 Warning that we have found
// a point on a curve with a zero derivative
#define     CRV_ZERO_DERIV                  266

// knt June 2003. Insanity errors that map to specific check_status errors.
#define		SURF_DATA_MISSING				267
#define		SURF_DATA_CONFLICTING			268
#define		CRV_DATA_MISSING				269
#define		CRV_DATA_CONFLICTING			270

// apc 1Aug03 Warning for when containment check cannot be performed
#define 	NO_CONT_CHK						271

// apc 14Aug03 Warning for when loop orientation check cannot be performed
#define		LOOP_ORIENT_CHECK_FAIL			272
*/ // - end comment

#include "dcl_intr.h"

/**
* @file insanity_tbl.hxx
 * @CAA2Level L1
 * @CAA2Usage U1
 */

/** \addtogroup ACISCHECKING
 *  \brief Declared at <insanity_tbl.hxx>, SPAintr
 *  @{
 */

/**
 * This class allows the representation of different sub-categories of
 * entity insanity. 
 * <br>
 * <b>Role:</b> A static instance of this class is defined for each sub-category
 * that ACIS reports. These instances are used in the same manner as a C++
 * enumerated type to identify and compare sub-category data. In addition, an
 * instance can return a string describing the insanity sub-category it represents.
 * Each <tt>insanity_data</tt> object has a sub-category assigned to it.
 * <br>

 * The sub-categories currently defined are:
 * <br>
 * <tt>NO_SUB_CATEGORY</tt> - no sub-category for this insanity<br>
 * <tt>HIGH_CURVATURE</tt> - the insanity is due to a region of high curvature in the geometry<br>
 * <tt>BAD_APPROXIMATING_SURFACE</tt> - the insanity is due to errors in the approximating surface for the geometry<br>
 *
 * @see insanity_data
 */
class DECL_INTR insanity_sub_category
{
private:
	const char* description;
public:
	/**
	 * Standard constructor for an insanity_sub_category. 
	 * @param desc
	 * Textual description of this instance.
	 */
	explicit insanity_sub_category(const char* desc)
		: description(desc) {}

	/**
	 * Copy constructor.
	 * @param other
     * Instance to copy.
	 */
	insanity_sub_category(const insanity_sub_category& other)
		: description(other.description) {}

	/**
	 * Return the textual description of this insanity_sub_category.
	 */
	const char* get_description() const
	{
		return description;
	}
	
	/**
     * Equality operator.
     * @param other
     * Instance to compare this instance to.
	 */
	logical operator==(const insanity_sub_category& other) const
    {
		return description==other.description;
    }

	/**
     * Inequality operator.
     * @param other
     * Instance to compare this instance to.
	 */
	logical operator!=(const insanity_sub_category& other) const
    {
    return description!=other.description;
    }

};

extern DECL_INTR insanity_sub_category NO_SUB_CATEGORY;
extern DECL_INTR insanity_sub_category HIGH_CURVATURE;
extern DECL_INTR insanity_sub_category BAD_APPROXIMATING_SURFACE;

/** @} */

#endif


