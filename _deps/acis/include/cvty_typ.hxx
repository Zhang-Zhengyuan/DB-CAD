// $Id: cvty_typ.hxx,v 1.4 2000/12/26 18:43:17 products Exp $
/*******************************************************************/
/*    Copyright (c) 1989-2020 by Spatial Corp.                     */
/*    All rights reserved.                                         */
/*    Protected by U.S. Patents 5,257,205; 5,351,196; 6,369,815;   */
/*                              5,982,378; 6,462,738; 6,941,251    */
/*    Protected by European Patents 0503642; 69220263.3            */
/*    Protected by Hong Kong Patent 1008101A                       */
/*******************************************************************/

#ifndef CVTY_TYP_H
#define CVTY_TYP_H

// Scheme convexity (cvty) classes.

#include "scheme.hxx"

#include "cvxty.hxx"
#include "pt_cvty.hxx"
#include "ed_cvty.hxx"

// First, for the cvty class.

logical is_Scm_Cvty( ScmObject );
cvty get_Scm_Cvty( ScmObject );
ScmObject make_Scm_Cvty( cvty const & );

// And the pt_cvty_info class.

logical is_Scm_Pt_Cvty_Info( ScmObject );
pt_cvty_info get_Scm_Pt_Cvty_Info( ScmObject );
ScmObject make_Scm_Pt_Cvty_Info( pt_cvty_info const & );

// And the ed_cvty_info class.

logical is_Scm_Ed_Cvty_Info( ScmObject );
ed_cvty_info get_Scm_Ed_Cvty_Info( ScmObject );
ScmObject make_Scm_Ed_Cvty_Info( ed_cvty_info const & );

#endif
