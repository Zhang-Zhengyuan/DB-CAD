/*******************************************************************/
/*    Copyright (c) 1989-2020 by Spatial Corp.                     */
/*    All rights reserved.                                         */
/*    Protected by U.S. Patents 5,257,205; 5,351,196; 6,369,815;   */
/*                              5,982,378; 6,462,738; 6,941,251    */
/*    Protected by European Patents 0503642; 69220263.3            */
/*    Protected by Hong Kong Patent 1008101A                       */
/*******************************************************************/
//$Id:
// Header file for the oper options.

#if !defined(OPEROPTS_H)

#define OPEROPTS_H

#include "dcl_warp.h"
class option_header;

// oper\operator\sg_husk\warp\warp_space.cxx:
// SESSION_GLOBAL_VAR DECL_WARP option_header warp_tolerant_hot ("warp_tolerant_hot", FALSE); // FALSE by default, but users can turn it on.
extern DECL_WARP option_header warp_tolerant_hot;

// oper\operator\sg_husk\warp\warp_space.cxx:
// SESSION_GLOBAL_VAR DECL_WARP option_header fast_first_derivatives ("fast_first_derivatives", FALSE); // Internal use only. Leave TRUE for speed, unless there is a problem.
extern DECL_WARP option_header fast_first_derivatives;

// oper\operator\sg_husk\warp\warp_space.cxx:
// SESSION_GLOBAL_VAR DECL_WARP option_header fast_second_derivatives ("fast_second_derivatives", FALSE); // Internal use only. Leave TRUE for speed, unless there is a problem.
extern DECL_WARP option_header fast_second_derivatives;


//SPAwarp\operator_sg_husk_warp.m\src\warp_space.cpp
//SESSION_GLOBAL_VAR option_header warp_checking( "warp_checking", TRUE );
extern DECL_WARP option_header warp_checking;

//SESSION_GLOBAL_VAR option_header warp_GSM( "springback_GSM", FALSE );
extern DECL_WARP option_header springback_GSM;
#endif
