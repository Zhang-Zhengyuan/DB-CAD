/*******************************************************************/
/*    Copyright (c) 1989-2020 by Spatial Corp.                     */
/*    All rights reserved.                                         */
/*    Protected by U.S. Patents 5,257,205; 5,351,196; 6,369,815;   */
/*                              5,982,378; 6,462,738; 6,941,251    */
/*    Protected by European Patents 0503642; 69220263.3            */
/*    Protected by Hong Kong Patent 1008101A                       */
/*******************************************************************/

#if !defined (BHL_UV_UTIL2)
#define BHL_UV_UTIL2

#include "bs3surf.hxx"
#include "sps3srtn.hxx"
#include "sps3crtn.hxx"
#include "sp3srtn.hxx"
#include "sp3crtn.hxx"
#include "bs3cutil.hxx"

#include "spldef.hxx"
#include "pladef.hxx"
#include "curdef.hxx"
#include "intdef.hxx"

//#include "healhusk/checks/surf_chk.hxx"
#include "dcl_stitch.h"

DECL_STITCH logical bhl_make_curves_compatible(bs3_curve& bc1, bs3_curve& bc2);

//MG (7 May99) written as a utility for bs3_curve
class DECL_STITCH HH_BS3_Curve
{
	SPAposition *ctl;
	int no_ctl_pt;

public:
	HH_BS3_Curve(SPAposition * =NULL, int = 0);
	logical is_arc_for_splitting(int,double);
};

#endif


// ---------------------------------------------------------------------------
// ---------------------------------------------------------------------------

