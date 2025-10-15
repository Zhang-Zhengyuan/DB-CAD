/*******************************************************************/
/*    Copyright (c) 1989-2020 by Spatial Corp.                     */
/*    All rights reserved.                                         */
/*    Protected by U.S. Patents 5,257,205; 5,351,196; 6,369,815;   */
/*                              5,982,378; 6,462,738; 6,941,251    */
/*    Protected by European Patents 0503642; 69220263.3            */
/*    Protected by Hong Kong Patent 1008101A                       */
/*******************************************************************/
// Enum defining the kind of transition blending is required to make
// when it runs off an underlying entity. In general, we may attempt to
// cap the end of the blend against the things we ran into, we may
// attempt to carry on rolling over them, or we may attempt to run a
// blend out.
#include "acis.hxx"
/**
 * @file bl_trans.hxx
 * @CAA2Level L1
 * @CAA2Usage U1
 * \addtogroup BLNDAPI
 *
 * @{
 */
#ifndef BL_TRANS
#define BL_TRANS
/**
 * Defines the kind of transition blending is required to make when the blend runs off an underlying entity.
 * @param blend_unknown
 * unknown blend.
 * @param blend_runout
 * run the blend out.
 * @param blend_cap
 * cap the blend.
 * @param blend_rollon
 * roll on over the entity.
 **/
enum blend_transition {
	blend_unknown,
	blend_runout,
	blend_cap,
	blend_rollon
	};



// Some D3 debug to make these more readable.

#if defined D3_DEBUG || defined D3_STANDALONE

#include "stream.hxx"
/*
// tbrv
*/
/**
 * @nodoc
 */

inline D3_ostream &operator<<( D3_ostream &os, blend_transition b )
{
	char const *str = b == blend_unknown ? "blend_unknown" :
						  b == blend_cap ? "blend_cap" :
					   b == blend_rollon ? "blend_rollon" :
					   b == blend_runout ? "blend_runout" :
										   "blend_???";
	return os << str;
}

#endif

 /** @} */
#endif
