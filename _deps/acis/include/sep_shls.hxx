/* $Id: sep_shls.hxx,v 1.5 2000/12/26 18:52:07 products Exp $ */
/*******************************************************************/
/*    Copyright (c) 1989-2020 by Spatial Corp.                     */
/*    All rights reserved.                                         */
/*    Protected by U.S. Patents 5,257,205; 5,351,196; 6,369,815;   */
/*                              5,982,378; 6,462,738; 6,941,251    */
/*    Protected by European Patents 0503642; 69220263.3            */
/*    Protected by Hong Kong Patent 1008101A                       */
/*******************************************************************/

#include "logical.h"

#ifndef LTSE
#define LTSE

#include "dcl_lopt.h"

#include "position.hxx"
#include "errorsys.hxx"

class   SHELL;

DECL_LOPT logical lopt_separate_shells( SHELL *sh, logical const &void_shell, 
					  logical const purge = TRUE );

#endif



