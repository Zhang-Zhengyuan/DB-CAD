/*******************************************************************/
/*    Copyright (c) 1989-2020 by Spatial Corp.                     */
/*    All rights reserved.                                         */
/*    Protected by U.S. Patents 5,257,205; 5,351,196; 6,369,815;   */
/*                              5,982,378; 6,462,738; 6,941,251    */
/*    Protected by European Patents 0503642; 69220263.3            */
/*    Protected by Hong Kong Patent 1008101A                       */
/*******************************************************************/

#if !defined (BHL_SNAP_COMPARE2)
#define BHL_SNAP_COMPARE2

#include "dcl_stitch.h"
#include "logical.h"

class SPAposition;

DECL_STITCH logical
DEQUAL(double ,  double ,  double);

DECL_STITCH logical
POSEQ ( SPAposition const&, SPAposition const&, double);

#endif


// ---------------------------------------------------------------------------
// ---------------------------------------------------------------------------
