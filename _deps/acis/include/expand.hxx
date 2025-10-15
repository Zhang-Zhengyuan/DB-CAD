/* ORIGINAL: acis2.1/kerndata/expand/expand.hxx */
/* $Id: expand.hxx,v 1.4 2000/12/26 18:33:42 products Exp $ */
/*******************************************************************/
/*    Copyright (c) 1989-2020 by Spatial Corp.                     */
/*    All rights reserved.                                         */
/*    Protected by U.S. Patents 5,257,205; 5,351,196; 6,369,815;   */
/*                              5,982,378; 6,462,738; 6,941,251    */
/*    Protected by European Patents 0503642; 69220263.3            */
/*    Protected by Hong Kong Patent 1008101A                       */
/*******************************************************************/

// Functions for subdividing shells into subshells, and for flattening
// them out again.

#if !defined( EXPAND_HDR_DEF )
#define  EXPAND_HDR_DEF

#include "dcl_eulr.h"

class BODY;
class LUMP;
class SHELL;
class SUBSHELL;
class decomp_options;

// The subdivision functions.
DECL_EULR void expand_body(
			BODY *b,
			decomp_options *opts = (decomp_options*)NULL
		);
DECL_EULR void expand_lump(
			LUMP *l,
			decomp_options *opts
		);
DECL_EULR void expand_shell(
			SHELL *s,
			decomp_options *opts
		);
DECL_EULR void expand_subshell(
			SUBSHELL *s,
			int,
			decomp_options *opts
		);

// The flattening functions.
DECL_EULR void flatten_body(
			BODY *b
		);
DECL_EULR void flatten_lump(
			LUMP *l
		);
DECL_EULR void flatten_shell(
			SHELL *s
		);
DECL_EULR void flatten_subshell(
			SUBSHELL *s
		);
#endif
