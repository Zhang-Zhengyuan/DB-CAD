// $Id: checkint.hxx,v 1.8 2001/09/05 13:17:58 susannaw Exp $
/*******************************************************************/
/*    Copyright (c) 1989-2020 by Spatial Corp.                     */
/*    All rights reserved.                                         */
/*    Protected by U.S. Patents 5,257,205; 5,351,196; 6,369,815;   */
/*                              5,982,378; 6,462,738; 6,941,251    */
/*    Protected by European Patents 0503642; 69220263.3            */
/*    Protected by Hong Kong Patent 1008101A                       */
/*******************************************************************/

// Emacs indenting style choice
// Indent style: D3

#if !defined( BOOL_CHECK_FF_HXX )
#define BOOL_CHECK_FF_HXX

class ENTITY;
class BODY;
class EDGE;
class ENTITY_LIST;
class insanity_list;

#include <stdio.h>
#include "dcl_bool.h"
#include "logical.h"
#include "base.hxx"
#include "intcucu.hxx"

// ywoo 08Jan01: added the header file.
// ywoo: end.

extern DECL_BOOL logical improper_ints_cont( ENTITY_LIST& face_list0,
					     ENTITY_LIST& face_list1,
					     ENTITY_LIST* insane_ents,
					     FILE *file_ptr = NULL,
						 insanity_list*& list = *(insanity_list **)NULL_REF );

extern DECL_BOOL BODY* make_error_body( ENTITY_LIST *insane_ents );

extern DECL_BOOL void clean_error_list( ENTITY_LIST *insane_ents );

extern DECL_BOOL logical valid_entity( ENTITY *entity, 
				       ENTITY_LIST* insane_ents );

extern DECL_BOOL logical entities_intersect( ENTITY* ent0, 
					     ENTITY* ent1,
					     ENTITY_LIST* insane_ents );

extern DECL_BOOL logical edge1_inside_edge2( EDGE *edge1, EDGE *edge2, curve_curve_int** inters = NULL );


extern DECL_BOOL logical improper_el_el_ints( ENTITY_LIST& edge_list0,
					      ENTITY_LIST& edge_list1,
					      ENTITY_LIST* insane_ents,
					      FILE *file_ptr,
					      logical check_abort,
					      insanity_list *list = *(insanity_list **)NULL_REF );

#endif
