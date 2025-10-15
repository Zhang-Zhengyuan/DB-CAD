/* ORIGINAL: acis2.1/sg_husk/make/del_ent.hxx */
/* $Id: del_ent.hxx,v 1.10 2000/12/26 18:48:54 products Exp $ */
/*******************************************************************/
/*    Copyright (c) 1989-2020 by Spatial Corp.                     */
/*    All rights reserved.                                         */
/*    Protected by U.S. Patents 5,257,205; 5,351,196; 6,369,815;   */
/*                              5,982,378; 6,462,738; 6,941,251    */
/*    Protected by European Patents 0503642; 69220263.3            */
/*    Protected by Hong Kong Patent 1008101A                       */
/*******************************************************************/


#if !defined( DEL_ENT_HDR_DEF )
#define  DEL_ENT_HDR_DEF

#include "dcl_kern.h"
#include "tophdr.hxx"

class ENTITY_LIST;

// Prototypes

DECL_KERN void del_entity	(		ENTITY *	);
DECL_KERN void del_entity_list  (   ENTITY_LIST&    );
DECL_KERN void del_body		( 		BODY * 		);
DECL_KERN void del_lump		( 		LUMP * 		);
DECL_KERN void del_shell		( 		SHELL * 	);
DECL_KERN void del_subshell	( 		SUBSHELL * 	);
DECL_KERN void del_face		(		FACE * 		);
DECL_KERN void del_loop		( 		LOOP *		);
DECL_KERN void del_coedge	( 		COEDGE * 	);
DECL_KERN void del_edge		( 		EDGE * 		);
DECL_KERN void del_vertex	( 		VERTEX * 	);
DECL_KERN void del_wire		( 		WIRE * 		);
DECL_KERN void del_point		( 		APOINT * 	);
DECL_KERN void del_curve		( 		CURVE * 	);
DECL_KERN void del_pcurve	( 		PCURVE * 	);
DECL_KERN void del_surface	( 		SURFACE * 	);
DECL_KERN void del_transform	( 		TRANSFORM * );
DECL_KERN void del_attrib	( 		ATTRIB * 	);
DECL_KERN void lose_list		(		ENTITY *	);

#endif
