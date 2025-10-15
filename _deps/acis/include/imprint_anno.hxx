/* ORIGINAL: boolean/sg_husk/boolean/imprint_anno.hxx */
// $Id: imprint_anno.hxx,v 1.7 2000/12/26 18:26:33 products Exp $
/*******************************************************************/
/*    Copyright (c) 1989-2020 by Spatial Corp.                     */
/*    All rights reserved.                                         */
/*    Protected by U.S. Patents 5,257,205; 5,351,196; 6,369,815;   */
/*                              5,982,378; 6,462,738; 6,941,251    */
/*    Protected by European Patents 0503642; 69220263.3            */
/*    Protected by Hong Kong Patent 1008101A                       */
/*******************************************************************/

//-----------------------------------------------------------------------
// Purpose---
//	Imprint Annotations
//-----------------------------------------------------------------------
// History:
// alexl 1Aug2006: Exported IMPRINT_ANNOTATION

#ifndef __IMPRINTANNO_HXX__
#define __IMPRINTANNO_HXX__

#include "dcl_bool.h"
#include "annotation.hxx"
#include "lists.hxx"
#include "eelists.hxx"

/**
* @file imprint_anno.hxx
 * @CAA2Level L1
 * @CAA2Usage U1
 */

/**
 * @nodoc
 */
ENTITY_IS_PROTOTYPE(IMPRINT_ANNOTATION, BOOL)
#if 0
; // semicolon needed for mkman (doc tool) parsing
#endif
// We cannot use MASTER_ENTITY_DECL because that hard codes ENTITY as 
// the base.

#define IMPRINT_ANNOTATION_LEVEL (ANNOTATION_LEVEL + 1)
extern DECL_BOOL int IMPRINT_ANNOTATION_TYPE;

/**
* Defines the annotation class for the imprint operation.
* <b>Role:</b> Annotates as input the tool entity and the blank 
* entity, and as output each new edge or vertex 
* resulting from a call to <tt>api_selectively_imprint</tt>,
* when annotations are on.
*
* The tool and blank entities' intersection 
* corresponds to the new entity (which belongs to
* the blank body).
*
* When the new entity is an edge, the blank entity
* will be a face that was split and the tool entity
* will be a face or an edge.  The tool will only be
* an edge when this original edge on a tool face lies 
* in the blank face.
*
* When the new entity is a vertex, the blank and tool
* entities will be either edges or faces.  At least one
* of the these will be an edge.
* <dl>
* <dt>Inputs:</dt>
* <dd>Tool - ENTITY</dd>
* <dd>Blank - ENTITY</dd>
* <dd>New entity - ENTITY</dd>
* </dl>
* As with all inputs, the values are actually stored as
* <tt>ATTRIB_TAGs</tt> which refer to the input entity.
* @see ANNOTATION, ENTITY
*/
class DECL_BOOL IMPRINT_ANNOTATION : public ANNOTATION {
protected:
    enum {
		e_tool_entity,
        e_blank_entity,
		e_new_entity,
        e_num_datums
	};
public:
	/**
	 * @nodoc
	 */
	ANNOTATION_MAP_DECL(e_num_datums)
#if 0
; // semicolon needed for mkman (doc tool) parsing)
#endif
        /**
         * @nodoc
         */
        ENTITY_FUNCTIONS(IMPRINT_ANNOTATION, BOOL)
#if 0
; // semicolon needed for mkman (doc tool) parsing)
#endif
// Because mkman isn't aware that ENTITY_FUNCTIONS changes access to "public", do so
// explicitly here:
public:


    // initialization constructor
    /**
    * C++ initialize constructor requests memory for this object and populates it with the data supplied as arguments.
    * <br><br>
    * <b>Role:</b> Applications should call this constructor only with the overloaded new operator
    * inherited from the <tt>ENTITY</tt> class (for example, <tt>x=new ATTRIB_EFINT(...)</tt>), because this reserves
    * the memory on the heap, a requirement to support roll back and history management.
    * <br><br>
    * @param tool_entity
    * tool entity (ENTITY).
    * @param blank_entity
    * blank entity (ENTITY).
    * @param new_entity
    * new entity (ENTITY).
    */
    IMPRINT_ANNOTATION(ENTITY* tool_entity = NULL, // the tool entity 
                     ENTITY* blank_entity = NULL,  // the blank entity
                     ENTITY* new_entity = NULL)    // the new entity
	{
		ents[e_tool_entity] = copy_ATTRIB_TAG(tool_entity,this);
        ents[e_blank_entity] = copy_ATTRIB_TAG(blank_entity,this);
        ents[e_new_entity] = new_entity;
		//hook_members();
	}
	// Data Retrieval

    /**
    * Returns the ATTRIB_TAG or EE_LIST for the tool entity.
	*/
    ENTITY* get_tool_ent() const { return ents[e_tool_entity]; }
    
    /**
    * Returns the ATTRIB_TAG or EE_LIST for the blank entity.
    */
    ENTITY* get_blank_ent() const { return ents[e_blank_entity]; }

    /**
    * Returns the entity or EE_LIST for the new entity
    */
    ENTITY* get_new_ent() const { return ents[e_new_entity]; }
};

#endif //__SPLITANNO_HXX__
