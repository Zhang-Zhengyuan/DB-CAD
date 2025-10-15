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
//    Define PART_CONTEXT class
//
//   The PART_CONTEXT class creates an association between a PART and
//   the display of the PART.  It contains a PART which keeps track of
//   a collection of ENTITIES which define the PART.  It is derived from
//	 rendering_manager in order to manage the display.
//
//   When you add a PART to a PART_CONTEXT, the PART_CONTEXT takes over
//		ownership for it.  That is, when the PART_CONTEXT is deleted,
//		it will also delete its PART.
//
//   When you use the routine api_pm_create_part, it creates a new PART,
//   and also creates a PART_CONTEXT which owns that PART.  The routine
//   api_pm_delete_part checks to see if the PART belongs to a PART_CONTEXT,
//   and if so, deletes the PART_CONTEXT.
//
//----------------------------------------------------------------------

#ifndef part_context_hxx
#define part_context_hxx

#include "logical.h"
#include "mmgr.hxx"
class PART_CONTEXT;
#include "scmapp.hxx"

/**
 * @file part_ctx.hxx
 * @CAA2Level L1
 * @CAA2Usage U2
 * \addtogroup SCMAPI
 * \Declared at <part_ctx.hxx>
 * @{ 
 */

class PART;
class ENTITY;
class component_entity_handle;
class PART_CONTEXT_CB;
class display_entity_handler;
class defer_roll_cb;
class SchemeView;
class RenderingObject;
class scm_pick_manager;

/**
 * The PART_CONTEXT object.
 * <br>
 * <b>Role:</b> This is a pure virtual base class. Hence, it can only be derived from.
 * Since this is within our Scheme code, it is meant as an example of how to provide
 * a means of connecting a PART with the underlying rendering component.
 * <br><br>
 * The PART_CONTEXT class creates an association between a PART and the display of the PART.  It
 * contains a PART which keeps track of a collection of ENTITIES which define the PART.  The PART_CONTEXT
 * class is an example of a way to manage PART with the display.
 * <br><br>
 * For example, Scheme was originally written based on GI.  For this, we implemented
 * the GI_PART_CONTEXT class, which is derived from PART_CONTEXT.  More recently, Scheme
 * added support for HOOPS.  For this, we implemented the HA_PART_CONTEXT, which is
 * derived from PART_CONTEXT. (Note that GI_PART_CONTEXT and HA_PART_CONTEXT are not documented,
 * as they are simply implementations of this pure virtual base class, which is documented.)
 * <br><br>
 * While a PART_CONTEXT is not required for interfacing the PART to the underlying rendering
 * component, the PART_CONTEXT class is a good example of how to do so.  The PART_CONTEXT has the entity callback
 * functions to the PART, so the rendering component gets notified of entity events, such as
 * entity creation, deletion, updating, and rollback, etc. When an entity is added to the PART,
 * it gets passed to the PART_CONTEXT, which in turn passes it on to all of its rendering contexts
 * and views. PART_CONTEXT maintains a flag regarding whether or not to display entities from the
 * PART.
 * @see active_part_context, get_part_context, PART, RenderingObject, scm_pick_manager
 */
class PART_CONTEXT: public ACIS_OBJECT
{
public:
	/**
	 * Virtual destructor.
	 */
	virtual ~PART_CONTEXT(){}

	/**
	 * Returns the PART associated with this PART_CONTEXT.
	 */
	virtual PART* Part() =0;

	/**
	 * Make this the active PART_CONTEXT
	 */
	virtual void Activate() =0;

	/**
	 * Display entities when they are added to the PART_CONTEXT?
	 */
	virtual logical get_auto_display() =0;

	/**
	 * Control if entities are displayed when they are added to the PART_CONTEXT.
	 */
	virtual void set_auto_display(logical onoff) =0;

	/**
	 * Does this PART_CONTEXT contain the given SchemeView?
	 */
	virtual logical contains_view(const SchemeView&)=0;

	/**
	 * Add an ENTITY to this PART_CONTEXT.
	 */
	virtual void add( ENTITY* ent, logical displayable = TRUE) =0;

	/**
	 * Remove an ENTITY from this PART_CONTEXT.
	 */
	virtual void remove( ENTITY* ent)=0;

	/**
	 * Update an ENTITY to this PART_CONTEXT when it has been modified.
	 */
	virtual void modified( ENTITY* ent, logical)=0;

	/**
	 * Add the given RenderingObject to every rendering manager's view.
	 */
	virtual void AddRenderingObject(RenderingObject *)=0;

	/**
	 * Remove the given RenderingObject from every rendering manager's view.
	 */
	virtual void RemoveRenderingObject(RenderingObject *)=0;

	/**
	 * Get the pick manager associated with this PART_CONTEXT.
	 */
	virtual scm_pick_manager* GetPickManager()=0;

	/**
	 * Refresh the view(s) associated with this PART_CONTEXT.
	 */
	virtual void refresh(logical update_sils=FALSE)=0;
};


/**
 * @nodoc
 */
PART_CONTEXT* create_part_context();

// Get the PART_CONTEXT from an ENTITY

/**
 * Gets the <tt>PART_CONTEXT</tt> from an <tt>ENTITY</tt>.
 * <br><br>
 * <b>Effect:</b> Read-only
 * <br><br>
 * @param ent
 * given entity.
 **/

PART_CONTEXT* get_part_context(const ENTITY* ent);


/**
 * @nodoc
 */
int count_part_contexts();

/*
// tbrv
*/
/**
 * @nodoc
 */
void add_part_context(PART_CONTEXT *);

/**
 * @nodoc
 */
void remove_part_context(PART_CONTEXT *);


// The proceudre get_next_part_context is used to iterate through all of the
// PART_CONTEXTS that are defined.  The argument is used to keep track of the
// SPAposition in the list of PARTS 0 it is both an input and output argument.
// To iterate though all of the PARTs, you can use code similar to the
// following example.
//   int pos = 0;
//   PART_CONTEXT* part = NULL;
//   while( (part = get_next_part(pos)) != NULL ) {
//     do something with the part
//   }

/**
 * @nodoc
 */
PART_CONTEXT * get_next_part_context(int&);

// Delete all PART_CONTEXTs that are defined.  This is normally only called to
// clean up from toolkit_terminate.

/**
 * @nodoc
 */
void delete_all_part_contexts();

// This procedure lets you lookup a PART_CONTEXT if you know the ID of the
// PART it manages.
/**
 * @nodoc
 */
PART_CONTEXT* find_part_context(unsigned int id);

// This procedure lets you lookup a PART_CONTEXT if you know the PART it
// managers
/**
 * @nodoc
 */
PART_CONTEXT* find_part_context( PART *part);

// lookup a PART_CONTEXT if you know a view
/**
 * @nodoc
 */
 PART_CONTEXT* find_part_context(const SchemeView&);


/**
 * Refreshes all views.
 * <br><br>
 * <b>Effect:</b> System routine
 **/

void refresh_all(logical update_sils=FALSE);

/**
 * @nodoc
 */
void SelectEntity(ENTITY *e, logical on);

/**
 * @nodoc
 */
void SelectComponentEntity(component_entity_handle *ce, logical on);

/**
 * @nodoc
 */
void GetSelected(ENTITY *e, logical &on);

/** @} */
#endif
