/* ORIGINAL: acis2.1/sg_husk/face/face_utl.hxx */
// $Id: cnv2spl.hxx,v 1.7 2000/12/26 18:26:36 products Exp $
/*******************************************************************/
/*    Copyright (c) 1989-2020 by Spatial Corp.                     */
/*    All rights reserved.                                         */
/*    Protected by U.S. Patents 5,257,205; 5,351,196; 6,369,815;   */
/*                              5,982,378; 6,462,738; 6,941,251    */
/*    Protected by European Patents 0503642; 69220263.3            */
/*    Protected by Hong Kong Patent 1008101A                       */
/*******************************************************************/

#if !defined (CNV2SPL_HXX)
#define CNV2SPL_HXX

#include "dcl_bool.h"
#include "logical.h"

class ENTITY;

// Convert all of the faces within the specified entity (BODY, SHELL, or FACE)
// to splines.

DECL_BOOL logical
sg_convert_to_spline(
	ENTITY* given_entity
	);

#endif
