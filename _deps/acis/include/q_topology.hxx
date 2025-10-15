/*******************************************************************/
/*    Copyright (c) 1989-2020 by Spatial Corp.                     */
/*    All rights reserved.                                         */
/*    Protected by U.S. Patents 5,257,205; 5,351,196; 6,369,815;   */
/*                              5,982,378; 6,462,738; 6,941,251    */
/*    Protected by European Patents 0503642; 69220263.3            */
/*    Protected by Hong Kong Patent 1008101A                       */
/*******************************************************************/

//  Declare routines for topological queries about bodies, lumps, and shells.

#if !defined (Q_TOPOLOGY_HXX)
#define Q_TOPOLOGY_HXX

#include "dcl_kern.h"
#include "logical.h"

class BODY;
class LUMP;
class SHELL;
class ENTITY;
class ENTITY_LIST;

DECL_KERN logical is_incomplete_shell( SHELL *sh );
DECL_KERN logical is_incomplete_lump( LUMP *lu );
DECL_KERN logical is_incomplete_body( BODY *by );
DECL_KERN logical is_double_sided_shell( SHELL  *sh );
DECL_KERN void find_connected_entities(
	ENTITY *seed_ent,
	ENTITY_LIST &entlist,
	logical use_wires = TRUE);

#endif
