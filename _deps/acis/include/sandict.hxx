/* ORIGINAL: acis2.1/sg_husk/sanity/sandict.hxx */
/* $Id: sandict.hxx,v 1.8 2001/01/11 18:06:58 ywoo Exp $ */
/*******************************************************************/
/*    Copyright (c) 1989-2020 by Spatial Corp.                     */
/*    All rights reserved.                                         */
/*    Protected by U.S. Patents 5,257,205; 5,351,196; 6,369,815;   */
/*                              5,982,378; 6,462,738; 6,941,251    */
/*    Protected by European Patents 0503642; 69220263.3            */
/*    Protected by Hong Kong Patent 1008101A                       */
/*******************************************************************/

#ifndef DICT_HXX
#define DICT_HXX

#include "dcl_intr.h"
// ywoo 08Jan01: added the header file.
#include "mmgr.hxx"
// ywoo: end

struct DECL_INTR dict_node_c {
   void* word;
   void* def;
   dict_node_c* next;
   dict_node_c( void* w, void* v, dict_node_c* n ) 
      { word = w; def = v; next = n; }

   /**
    * @nodoc
    */
   MMGR_SUPPORT_THIS
#if 0
; // semicolon needed for mkman (doc tool) parsing
#endif
};

struct DECL_INTR dictionary_c {
   dict_node_c* head; 
   dictionary_c();
   ~dictionary_c(); 

   /**
    * @nodoc
    */
   MMGR_SUPPORT_THIS
#if 0
; // semicolon needed for mkman (doc tool) parsing
#endif

   void empty();
   void* lookup( void* word );
   dict_node_c* browse( void*& );
   dict_node_c* find( void* word );
   dict_node_c* add( void* word, void* definition );
};

#endif
