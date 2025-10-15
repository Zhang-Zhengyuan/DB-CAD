/* $Id: mvtolgeo.hxx,v 1.5 2002/08/09 17:15:19 jeff Exp $ */
/*******************************************************************/
/*    Copyright (c) 1989-2020 by Spatial Corp.                     */
/*    All rights reserved.                                         */
/*    Protected by U.S. Patents 5,257,205; 5,351,196; 6,369,815;   */
/*                              5,982,378; 6,462,738; 6,941,251    */
/*    Protected by European Patents 0503642; 69220263.3            */
/*    Protected by Hong Kong Patent 1008101A                       */
/*******************************************************************/

// Header for moving the tolerant geometry and recalaculate the tolerance of
// the surrounding topology.


#if !defined( MOVE_TOLERANT_GEOM )
#define MOVE_TOLERANT_GEOM

#include "dcl_kern.h"
#include "logical.h"

class ENTITY;
class SPAvector;

DECL_KERN void
move_tolerant_geometry ( 
	ENTITY *entity, 
	const SPAvector& vec
);

#endif
