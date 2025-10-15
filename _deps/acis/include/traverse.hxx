/* ORIGINAL: acis2.1/sg_husk/sanity/traverse.hxx */
/* $Id: traverse.hxx,v 1.9 2002/08/09 17:19:04 jeff Exp $ */
/*******************************************************************/
/*    Copyright (c) 1989-2020 by Spatial Corp.                     */
/*    All rights reserved.                                         */
/*    Protected by U.S. Patents 5,257,205; 5,351,196; 6,369,815;   */
/*                              5,982,378; 6,462,738; 6,941,251    */
/*    Protected by European Patents 0503642; 69220263.3            */
/*    Protected by Hong Kong Patent 1008101A                       */
/*******************************************************************/

#ifndef TRAVERSE_HXX
#define TRAVERSE_HXX

#include "dcl_intr.h"
#include "err_info.hxx"

class ENTITY;
class BODY;
class LUMP;
class SHELL;
class FACE;
class LOOP;
class COEDGE;
class EDGE;
class VERTEX;
class SPAtransf;
class ENTITY_LIST;
class insanity_list;

DECL_INTR void traverse_entity( ENTITY* body, int (*fn) ( ENTITY*, const SPAtransf*, insanity_list* ), 
   int type, ENTITY_LIST* duplicates = NULL, insanity_list* = NULL );

DECL_INTR void traverse_body( BODY* body, int (*fn) ( ENTITY*, const SPAtransf*, insanity_list* ), 
   int type, ENTITY_LIST* duplicates = NULL, insanity_list* = NULL );

DECL_INTR void traverse_lump( BODY* body, int (*fn) ( ENTITY*, const SPAtransf*, insanity_list*), 
   const SPAtransf*, int type, ENTITY_LIST* duplicates = NULL, insanity_list* = NULL  );

DECL_INTR void traverse_shell( LUMP* body, int (*fn) ( ENTITY*, const SPAtransf*, insanity_list*), 
   const SPAtransf*, int type, ENTITY_LIST* duplicates = NULL, insanity_list* = NULL  );

DECL_INTR void traverse_face( SHELL* body, int (*fn) ( ENTITY*, const SPAtransf*, insanity_list*), 
   const SPAtransf*, int type, ENTITY_LIST* duplicates = NULL, insanity_list* = NULL  );

DECL_INTR void traverse_loop( FACE* body, int (*fn) ( ENTITY*, const SPAtransf*, insanity_list*), 
   const SPAtransf*, int type, ENTITY_LIST* duplicates = NULL, insanity_list* = NULL  );

DECL_INTR void traverse_coedge( LOOP* body, int (*fn) ( ENTITY*, const SPAtransf*, insanity_list*), 
   const SPAtransf*, int type, ENTITY_LIST* duplicates = NULL, insanity_list* = NULL  );

DECL_INTR void traverse_edge( COEDGE* body, int (*fn) ( ENTITY*, const SPAtransf*, insanity_list*), 
   const SPAtransf*, int type, ENTITY_LIST* duplicates = NULL, insanity_list* = NULL  );

DECL_INTR void traverse_vertex( EDGE* body, int (*fn) ( ENTITY*, const SPAtransf*, insanity_list*), 
   const SPAtransf*, int type, ENTITY_LIST* duplicates = NULL, insanity_list* = NULL );

#endif
