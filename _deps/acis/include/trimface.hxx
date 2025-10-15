/* ORIGINAL: acis2.1/kernbool/trimface/trimface.hxx */
/* $Id: trimface.hxx,v 1.8 2000/12/26 18:26:28 products Exp $ */
/*******************************************************************/
/*    Copyright (c) 1989-2020 by Spatial Corp.                     */
/*    All rights reserved.                                         */
/*    Protected by U.S. Patents 5,257,205; 5,351,196; 6,369,815;   */
/*                              5,982,378; 6,462,738; 6,941,251    */
/*    Protected by European Patents 0503642; 69220263.3            */
/*    Protected by Hong Kong Patent 1008101A                       */
/*******************************************************************/

// Routine definitions for trimming face surfaces back to minimum
// range.

#if !defined( TRIMF_HDR_DEF )
#define  TRIMF_HDR_DEF

/**
 * @file trimface.hxx
 * @CAA2Level L1
 * @CAA2Usage U1
 */

#include "dcl_bool.h"
#include "base.hxx"

class BODY;
class FACE;
class EDGE;
class ENTITY_LIST;

/*! \addtogroup BOOLADVSPLITS
 *  \brief Declared at <boolapi.hxx>, SPAbool
 *  @{
 */

// Both these functions may be passed a string corresponding to the
// value of the "subset" option. If the string is passed, the "subset"
// option is not actually used, but its value is taken to be the given
// string. If the string is NULL, the "subset" option is used. This
// allows calling code effectively to "set" the option, without having
// to do anything ugly, thread-unsafe etc. (such as pushing and popping
// a value onto the option).

// The string/option controls whether surfaces are subset
// (non-destructive to the spl_sur underlying a spline), or split (which
// does trim the spl_sur). For analytic surfaces, subsetting works as
// for a spline, but splitting them does nothing.
// Valid options are:
//	none
//	spline
//	torus
//	all

// Generally we recommend using the default NULL value for the string.
// Under certain special circumstances it may be necessary to split a
// spline (thereby trimming back the spl_sur below it), so the value
// "none" might be passed.

// Limitations:
// These functions operate only on splines and tori. The surface is trimmed to the 
// parametric box of a face's pcurves. Thus, the boundaries of underlined trimmed surfaces 
// may not be exactly in the same place as face boundaries (even if the face boundary is 
// an isoparametric line). 

/**
 * @nodoc
 */
DECL_BOOL void trim_body_faces(
			BODY *this_body,			
			char const *subset_option = NULL,
            ENTITY_LIST* trimmed_faces = NULL
		);

/**
 * Trims or subsets the surface underlying a face.
 * <br><br>
 * <b>Role:</b>
 * This Direct Interface function will either split or subset the surface underlying
 * a face, depending on the type of the surface and the value of the global option 
 * <tt>subsetting</tt>. It returns a logical value indicating whether the surface was
 * split/subset.
 * <br><br>
 * This function may be passed a string corresponding to a valid value of the 
 * <tt>subsetting</tt> option. If the string is passed, the <tt>subsetting</tt> 
 * option is not actually used, but its value is taken to be the given
 * string. If the string is <tt>NULL</tt>, the <tt>subsetting</tt> option is used. 
 * This allows calling code effectively to "set" the option, without having
 * to do anything ugly, thread-unsafe, etc. (such as pushing and popping
 * a value onto the option).
 * <br><br>
 * The option (or string argument) controls whether surfaces are subset
 * (non-destructive to the <tt>spl_sur</tt> underlying a <tt>spline</tt>), 
 * or split (which does trim the <tt>spl_sur</tt>). For analytic surfaces (tori), 
 * subsetting works as for a <tt>spline</tt>, but splitting them does nothing.
 * Valid options are:
 * <ul>
 * <li><tt>"none"  </tt>   - never subset: trim splines, ignore all other types.</li>
 * <li><tt>"spline"</tt>   - subset splines, ignore all other types.</li>
 * <li><tt>"torus" </tt>   - subset tori, trim splines, ignore all other types.</li>
 * <li><tt>"all"   </tt>   - subset splines and tori, ignore all other types.</li>
 * </ul>
 * Generally we recommend using the default <tt>NULL</tt> value for the string argument.
 * Under certain special circumstances it may be necessary to split a
 * <tt>spline</tt> surface (thereby trimming back the <tt>spl_sur</tt> below it), 
 * so the value <tt>"none"</tt> might be passed.
 * <br><br>
 * This Direct Interface function should be used with caution. If an application uses the 
 * API interface calling this function directly is unneeded: it is called when necessary 
 * by ACIS API functions. 
 * <br><br>
 * <b>Limitations:</b>
 * This function operates only on splines and tori. The surface is trimmed or subsetted to the 
 * parametric box of a face's pcurves. Thus, the boundaries of underlined trimmed surfaces 
 * may not be exactly in the same place as face boundaries (even if the face boundary is 
 * an isoparametric line). 
 * <br><br>
 * @param this_face
 * face to be trimmed.
 * @param subset_option
 * temporary value for the <tt>subsetting</tt> option.
 */
DECL_BOOL logical trim_face(
			FACE *this_face,
			char const *subset_option = NULL
		);

/**
 * @nodoc
 */
DECL_BOOL void trim_edge(
			EDGE *this_edge
		);

// STI let (8/98): Removed rem_pcurve_face(), because function 
// duplicates functionality in sg_rm_pcurves_from_entity().
//DECL_BOOL void rem_pcurve_face(
//			FACE *this_face
//		);
// STI let: end

/** @} */
#endif
