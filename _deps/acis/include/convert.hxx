/* ORIGINAL: acis2.1/kerndata/convert/convert.hxx */
/* $Id: convert.hxx,v 1.6 2000/12/26 18:46:44 products Exp $ */
/*******************************************************************/
/*    Copyright (c) 1989-2020 by Spatial Corp.                     */
/*    All rights reserved.                                         */
/*    Protected by U.S. Patents 5,257,205; 5,351,196; 6,369,815;   */
/*                              5,982,378; 6,462,738; 6,941,251    */
/*    Protected by European Patents 0503642; 69220263.3            */
/*    Protected by Hong Kong Patent 1008101A                       */
/*******************************************************************/

// Various conversions, mainly for cases where data structure semantics
// have changed between versions. Forward conversions take old-style
// semantics and convert to new, and are useful as post-processes for old
// algorithms before they have been converted, and have a continuing use
// in restore file for reading old save files. Backward conversions are
// useful as pre-processes for old algorithms, but would rarely have
// long-term prospects. During a change, conversions may be used the other
// way round to bracket new algorithms being developed before the change
// is implemented.

#include "dcl_kern.h"

class BODY;
class LUMP;
class SHELL;
class ENTITY;
class WIRE;

// Convert a body, lump or shell with dangling wires from pre R1.7 form
// (coedges owned by the shell but only accessible via vertices) to R1.7
// form (coedges owned by a wire owned by the shell).

DECL_KERN void forward_convert_entity_wires(
		ENTITY *
	);

DECL_KERN void forward_convert_body_wires(
		BODY *
	);

DECL_KERN void forward_convert_lump_wires(
		LUMP *
	);

DECL_KERN void forward_convert_shell_wires(
		SHELL *
	);

// Convert a body, lump or shell with dangling wires from R1.7 form
// (coedges owned by a wire owned by the shell) to pre R1.7 form
// (coedges owned by the shell but only accessible via vertices).

DECL_KERN void backward_convert_entity_wires(
		ENTITY *
	);

DECL_KERN void backward_convert_body_wires(
		BODY *
	);

DECL_KERN void backward_convert_lump_wires(
		LUMP *
	);

DECL_KERN void backward_convert_shell_wires(
		SHELL *
	);

// Convert from either form into whichever is currently selected by an
// option. For the first the entity structure may be in either form,
// for the others it is assumed to be in old or new form respectively,
// so conversion is not attempted if that is the target.

DECL_KERN void convert_entity_wires(
		ENTITY *
	);

DECL_KERN void convert_old_entity_wires(
		ENTITY *
	);

DECL_KERN void convert_new_entity_wires(
		ENTITY *
	);

// To make wire body from WIRE.

DECL_KERN BODY* make_wire_body( WIRE *wire );
