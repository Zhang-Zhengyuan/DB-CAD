/* ORIGINAL: acis6.4/kerndata/bulletin/bullutil.hxx */
// $Id: bullutil.hxx,v 1.3 2001/03/21 23:56:15 jsloan Exp $
/*******************************************************************/
/*    Copyright (c) 1989-2020 by Spatial Corp.                     */
/*    All rights reserved.                                         */
/*    Protected by U.S. Patents 5,257,205; 5,351,196; 6,369,815;   */
/*                              5,982,378; 6,462,738; 6,941,251    */
/*    Protected by European Patents 0503642; 69220263.3            */
/*    Protected by Hong Kong Patent 1008101A                       */
/*******************************************************************/

// Contains functionality related to the history mechanism. 

#ifndef BULLUTIL_HXX
#define BULLUTIL_HXX

#include "acis.hxx"
#include "dcl_kern.h"

#include "lists.hxx"  // ENTITY_LIST

class BULLETIN;
class BULLETIN_LIST;
class DELTA_STATE;
class HISTORY_STREAM;

struct DECL_KERN modified_entities_list
{
 ENTITY_LIST deleted;
 ENTITY_LIST created;
 ENTITY_LIST changed;

 void clear()
  {
   deleted.clear();
   created.clear();
   changed.clear();
  }

 void delete_ent(ENTITY* ptr);
 void create_ent(ENTITY* ptr);
 void change_ent(ENTITY* ptr);
};

void get_modified_entities          // adds to list
        (BULLETIN const* bull,
         modified_entities_list & mods,
         logical  rolls_back,
         logical  ignore_ATTRIB_changes=FALSE);

void get_modified_entities          // adds to list
        (BULLETIN_LIST const & blist,
         modified_entities_list & mods,
         logical  rolls_back,
         logical  ignore_ATTRIB_changes=FALSE);

void DECL_KERN get_modified_entities          // adds to list
        (DELTA_STATE const* ds,
         modified_entities_list & mods,
         logical  ignore_ATTRIB_changes=FALSE,
         logical (*tester)(ENTITY const *)=NULL);

void DECL_KERN get_modified_entities                        // adds to list
        (HISTORY_STREAM const* hs,                // use default stream if NULL
         modified_entities_list & mods,
         DELTA_STATE const* ds=NULL,              // from root to ds
         logical  ignore_ATTRIB_changes=FALSE,
         logical (*tester)(ENTITY const *)=NULL);

void get_all_user_attributes(HISTORY_STREAM const* hs,
                             int derivation_level,
                             int attrib_type_code,
                             ENTITY_LIST & attribs,     // adds to list
                             logical active_only);

void set_entity_history(DELTA_STATE const* ds);

void get_refacet_faces(
		ENTITY_LIST& deleted_faces,
		ENTITY_LIST& created_faces,
		ENTITY_LIST& changed_faces,
		DELTA_STATE* ds,
		HISTORY_STREAM* stream,
		logical use_lower_topology );

#endif
