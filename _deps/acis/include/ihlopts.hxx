/*******************************************************************/
/*    Copyright (c) 1989-2020 by Spatial Corp.                     */
/*    All rights reserved.                                         */
/*    Protected by U.S. Patents 5,257,205; 5,351,196; 6,369,815;   */
/*                              5,982,378; 6,462,738; 6,941,251    */
/*    Protected by European Patents 0503642; 69220263.3            */
/*    Protected by Hong Kong Patent 1008101A                       */
/*******************************************************************/
//$Id: //
// Header file for the ihl options.

#if !defined(IHLOPTS_H)
#define IHLOPTS_H

#include "dcl_ihl.h"
class option_header;

// ihl\ihl_husk\api\ihl_occlusion.cxx:
// SESSION_GLOBAL_VAR option_header ihl_occlusion_opt("ihl_occlusion", FALSE);
extern DECL_IHL option_header ihl_occlusion_opt;

// ihl\ihl_husk\api\ihl_occlusion.cxx:
// SESSION_GLOBAL_VAR option_header ihl_occlusion_tol("ihl_occlusion_tol", SPAresfit);
extern DECL_IHL option_header ihl_occlusion_tol;

// ihl\ihl_husk\ihl\ihl.cxx:
// SESSION_GLOBAL_VAR option_header ihl_sil_opt("ihl_sil#houette", TRUE);
extern DECL_IHL option_header ihl_sil_opt;

// ihl\ihl_husk\ihl\ihl.cxx:
// SESSION_GLOBAL_VAR option_header ihl_wire( "ihl_wire", TRUE );
extern DECL_IHL option_header ihl_wire;

#endif
