/* ORIGINAL: 3dt2.1/pmhusk/hashpart.hxx */
// $Id: hashpart.hxx,v 1.8 2000/12/26 18:55:16 products Exp $
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
//    Define HASHTABLE_PART class
//
//----------------------------------------------------------------------

#ifndef hashpart_hxx
#define hashpart_hxx

//======================================================================

#include "dcl_part.h"
#include "part.hxx"
#include "idtable.hxx"

class ID_ATTRIB;
class ENTITY_LIST;

//======================================================================

class DECL_PART HASHTABLE_PART : public PART
{
  private:
    entity_hash_table *EntityIdTable;
	ENTITY_LIST       *EntityList;

  public:
    HASHTABLE_PART(unsigned int initial_size = 0, logical add_to_array = TRUE);
    virtual ~HASHTABLE_PART();

    // Add an ENTITY to a PART and return its ID
    virtual void add_entity(ENTITY*, entity_id_t);

    // Add an ENTITY to a PART and return its ID
    virtual void add_entity(ENTITY*, entity_id_t, ENTITY * owner);

    // Remove an ENTITY from the part
    virtual logical remove_id(entity_id_t);

    // Find an ENTITY given its ID
    virtual ENTITY* lookup(entity_id_t);

    // Get a list of all top level ENTITIES in the part which pass
    // the given entity_filter
    virtual void get_entities(ENTITY_LIST&, entity_filter* = NULL) const;

	// Get the number of ENTITIES in the PART
	virtual unsigned long count() const;

    // Write debug information about the part
    virtual void debug(FILE*);

    // Create an ID for an ENTITY.  This method is intended to provide
    // an ID for an ENTITY which belongs to another ENTITY which is
    // already in the PART.  For example, you could call it to get an
    // ID for and EDGE of a solid which is in the PART.  If the owner of
    // the ENTITY is not already in the PART, it is added to the PART.

    virtual entity_id_t create_id(ENTITY*);

    // This version accepts the owner as second argument.

    virtual entity_id_t create_id( ENTITY*, ENTITY * );

    // Remove IDs from all ENTITIES which are not top level
    virtual void remove_ids();

};

//======================================================================

#define DEFAULT_PART_SIZE 1009

#endif
