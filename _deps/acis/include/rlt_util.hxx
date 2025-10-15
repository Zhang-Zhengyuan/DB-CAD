/*******************************************************************/
/*    Copyright (c) 1989-2020 by Spatial Corp.                     */
/*    All rights reserved.                                         */
/*    Protected by U.S. Patents 5,257,205; 5,351,196; 6,369,815;   */
/*                              5,982,378; 6,462,738; 6,941,251    */
/*    Protected by European Patents 0503642; 69220263.3            */
/*    Protected by Hong Kong Patent 1008101A                       */
/*******************************************************************/
//----------------------------------------------------------------------
// purpose---
//		Define a nice set of access functions to strip useful data 
//		out of RH_LIGHT entities.
//----------------------------------------------------------------------

#ifndef rlt_util_hxx
#define rlt_util_hxx

// ACIS includes
#include "dcl_rb.h"
#include "logical.h"

#include "entity.hxx"
#include "position.hxx"
#include "unitvec.hxx"

// RBASE includes
#include "rh_args.hxx"
#include "rh_enty.hxx"

// Toolkit includes
#include "rgbcolor.hxx"

//----------------------------------------------------------------------
// Macros
//----------------------------------------------------------------------

#define IS_LIGHT(ent)	(ent && (ent->identity(RH_LIGHT_LEVEL) == RH_LIGHT_TYPE))

//----------------------------------------------------------------------
// light enumerations
//----------------------------------------------------------------------

enum RHLType	{ INVALID_LIGHT, DEFAULT_LIGHT, AMBIENT_LIGHT, DISTANT_LIGHT, EYE_LIGHT, POINT_LIGHT, SPOT_LIGHT };

//----------------------------------------------------------------------
// prototypes
//----------------------------------------------------------------------

DECL_RB RHLType RHLight_Type(RH_LIGHT *);
DECL_RB logical RHLight_IsOn(RH_LIGHT *);
DECL_RB logical RHLight_Shadows(RH_LIGHT *);
DECL_RB logical	RHLight_BeamDistribution(RH_LIGHT *, float *);
DECL_RB logical RHLight_Color(RH_LIGHT *, rgb_color *);
DECL_RB logical RHLight_ConeAngle(RH_LIGHT *, float *);
DECL_RB logical RHLight_ConeDeltaAngle(RH_LIGHT *, float *);
DECL_RB logical RHLight_FallOff(RH_LIGHT *, Fall_Off_Type *);
DECL_RB logical RHLight_Intensity(RH_LIGHT *, float *);
DECL_RB logical RHLight_Location(RH_LIGHT *, SPAposition *);
DECL_RB logical RHLight_Direction(RH_LIGHT *, SPAunit_vector *);
DECL_RB logical RHLight_Target(RH_LIGHT *, SPAposition *);

#endif
