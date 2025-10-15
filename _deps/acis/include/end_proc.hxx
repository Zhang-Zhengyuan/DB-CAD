/* ORIGINAL: acis2.1/kernbool/blend1/end_proc.hxx */
/* $Id: end_proc.hxx,v 1.5 2002/02/06 15:07:17 tgreen Exp $ */
/*******************************************************************/
/*    Copyright (c) 1989-2020 by Spatial Corp.                     */
/*    All rights reserved.                                         */
/*    Protected by U.S. Patents 5,257,205; 5,351,196; 6,369,815;   */
/*                              5,982,378; 6,462,738; 6,941,251    */
/*    Protected by European Patents 0503642; 69220263.3            */
/*    Protected by Hong Kong Patent 1008101A                       */
/*******************************************************************/


#ifndef END_PROC_H
#define END_PROC_H


// This file declares any functions needed to communicate with the
// blending husk. The definitions are all in end_proc.cxx. The intention
// is that the blending husk is only accessed through these functions so
// that kernel blending does not get littered with "ifdef ABL_HUSK". If
// the blending husk is not present these functions will do nothing
// (except maybe produce an error).


class ENTITY_LIST;
class segend;
class ATTRIB_FFBLEND;

#include "dcl_blnd.h"
#include "logical.h"


// Do the abl_husk processing. This function should end up making a new
// attribute to continue the blend appropriately. It is an error to call
// it if the above function returned FALSE. Arguments are the blend
// attribute to be continued, the list of all blend attributes, and the
// segend (which should look like a cross curve segend) where the blend
// is to be taken up. It must be indicated whether the segend is at the
// start of its segment or not. Additionally, all the same arguments as
// were given to abl_husk_processing_needed should be supplied again
// (some are unnecessary, but we may as well request them all again to
// make things more obvious).

// NOTES:
// 1. Currently we only deal with intercepted edges, and runouts are for
// the moment not implemented.

DECL_BLND void
do_alternative_processing(
			   ATTRIB_FFBLEND *att,
			   ENTITY_LIST &att_list,
			   segend *seg,
			   logical at_start
		   );

#endif
