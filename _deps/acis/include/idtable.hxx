/* ORIGINAL: 3dt2.1/pmhusk/idtable.hxx */
// $Id: idtable.hxx,v 1.9 2001/01/11 18:16:02 ywoo Exp $
/*******************************************************************/
/*    Copyright (c) 1989-2020 by Spatial Corp.                     */
/*    All rights reserved.                                         */
/*    Protected by U.S. Patents 5,257,205; 5,351,196; 6,369,815;   */
/*                              5,982,378; 6,462,738; 6,941,251    */
/*    Protected by European Patents 0503642; 69220263.3            */
/*    Protected by Hong Kong Patent 1008101A                       */
/*******************************************************************/
//----------------------------------------------------------------------
// purpose---
//    Define an id table for keeping track of ENTITIES in a part
//
//----------------------------------------------------------------------

#ifndef idtable_hxx
#define idtable_hxx

#include "dcl_part.h"
#include "logical.h"
#include "mmgr.hxx"
#include <stdio.h>


class ENTITY;
class ENTITY_LIST;
class ent_hash_entry;
class HASHTABLE_PART;
class entity_filter;

#include "entityid.hxx"

//======================================================================
// entity hash table

class DECL_PART entity_hash_table: public ACIS_OBJECT
{
  private:
    unsigned long table_size;
    ent_hash_entry** entries;

    unsigned long hash(entity_id_t);
    logical find(entity_id_t id,
                 unsigned long&,
                 ent_hash_entry*&,
                 ent_hash_entry*&);

    // Allow access to the add and remove methods only from the
    // HASHTABLE_PART class
friend class HASHTABLE_PART;
    logical add(ENTITY*, entity_id_t);
    logical remove(entity_id_t);

  public:
    entity_hash_table(unsigned long);
    ~entity_hash_table();

    ENTITY* lookup(entity_id_t);
    void get_all_entities(ENTITY_LIST&, entity_filter* = NULL);
    void purge();
    void debug(FILE*);
};

//----------------------------------------------------------------------

//======================================================================
#endif
