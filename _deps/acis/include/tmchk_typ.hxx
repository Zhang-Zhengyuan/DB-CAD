// $Id: tmchk_typ.hxx,v 1.3 2000/12/26 18:43:18 products Exp $
/*******************************************************************/
/*    Copyright (c) 1989-2020 by Spatial Corp.                     */
/*    All rights reserved.                                         */
/*    Protected by U.S. Patents 5,257,205; 5,351,196; 6,369,815;   */
/*                              5,982,378; 6,462,738; 6,941,251    */
/*    Protected by European Patents 0503642; 69220263.3            */
/*    Protected by Hong Kong Patent 1008101A                       */
/*******************************************************************/

// Definition of the Scheme version of the tm_chk_info class.

#ifndef TMCHK_TYP_HEADER
#define TMCHK_TYP_HEADER

#include "scheme.hxx"
#include "logical.h"

class tm_chk_info;

logical is_Scm_Tm_Chk_Info( ScmObject );

tm_chk_info *get_Scm_Tm_Chk_Info( ScmObject );

// This one should only be used if the given tm_chk_info has a NULL next
// pointer, i.e. does not form a list. It will return a single Scheme
// object, and any subsequent "tm_chk_info"s that there might have been
// will be hidden and inaccessible.
ScmObject make_Scm_Tm_Chk_Info( tm_chk_info * );

// This is the function to use if the tm_chk_info may be part of a list
// (therefore with the next pointer possibly non-NULL). A Scheme list of
// tm_chk_info objects is returned. (The actual C++ tm_chk_info objects
// are unhooked each from the next.)
ScmObject make_Scm_Tm_Chk_Info_List( tm_chk_info * );

#endif
