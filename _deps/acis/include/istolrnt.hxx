/* $Id: istolrnt.hxx,v 1.4 2000/12/26 18:47:04 products Exp $ */
/*******************************************************************/
/*    Copyright (c) 1989-2020 by Spatial Corp.                     */
/*    All rights reserved.                                         */
/*    Protected by U.S. Patents 5,257,205; 5,351,196; 6,369,815;   */
/*                              5,982,378; 6,462,738; 6,941,251    */
/*    Protected by European Patents 0503642; 69220263.3            */
/*    Protected by Hong Kong Patent 1008101A                       */
/*******************************************************************/

#if !defined( IS_TOLERANT_HXX )
#define IS_TOLERANT_HXX

// Functions to determine if an entity is or has
// tolerant topology.

#include "dcl_kern.h"
#include "logical.h"

class ENTITY;
class BODY;
class WIRE;
class LUMP;
class SHELL;
class SUBSHELL;
class FACE;
class LOOP;
class EDGE;
class COEDGE;
class VERTEX;

DECL_KERN logical is_tolerant( ENTITY * );
DECL_KERN logical is_body_tolerant( BODY * );
DECL_KERN logical is_wire_tolerant( WIRE * );
DECL_KERN logical is_lump_tolerant( LUMP * );
DECL_KERN logical is_shell_tolerant( SHELL * );
DECL_KERN logical is_subshell_tolerant( SUBSHELL * );
DECL_KERN logical is_face_tolerant( FACE * );
DECL_KERN logical is_loop_tolerant( LOOP * );
DECL_KERN logical is_coedge_tolerant( COEDGE * );
DECL_KERN logical is_edge_tolerant( EDGE * );
DECL_KERN logical is_vertex_tolerant( VERTEX * );

#endif
