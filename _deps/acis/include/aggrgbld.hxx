/*******************************************************************/
/*    Copyright (c) 1989-2020 by Spatial Corp.                     */
/*    All rights reserved.                                         */
/*    Protected by U.S. Patents 5,257,205; 5,351,196; 6,369,815;   */
/*                              5,982,378; 6,462,738; 6,941,251    */
/*    Protected by European Patents 0503642; 69220263.3            */
/*    Protected by Hong Kong Patent 1008101A                       */
/*******************************************************************/
#if !defined ATTRIB_HH_AGGR_GEOMBUILD_BASE_CLASS
#define		 ATTRIB_HH_AGGR_GEOMBUILD_BASE_CLASS
#include	"lists.hxx"
#include	"edge.hxx"
#include "dcl_heal.h"
#include "at_aggr_stitch.hxx"
class EDGE;
class FACE;
class COEDGE;
class VERTEX;
class CURVE;
class PCURVE;
class SURFACE;
class APOINT;
/**
 * @file aggrgbld.hxx
 * @CAA2Level L1
 * @CAA2Usage U1
 * \addtogroup HEALAPIDEP
 *
 * @{
 */
/**
 * @nodoc
 */
DECL_HEAL extern int ATTRIB_HH_AGGR_GEOMBUILD_BASE_TYPE;
/**
 * @nodoc
 */
#define ATTRIB_HH_AGGR_GEOMBUILD_BASE_LEVEL (ATTRIB_HH_AGGR_LEVEL + 1)
/**
 * @nodoc
 */
ENTITY_IS_PROTOTYPE(ATTRIB_HH_AGGR_GEOMBUILD_BASE, HEAL )
#if 0
; // semicolon needed for mkman (doc tool) parsing
#endif

//----------------------------------------------------------------------------
//	Base class for the other modules of Geometry Building.
/**
 * @nodoc
 * <b>Obsolete:</b> This class has been deprecated. For healing, use API interfaces
 * and not Direct Interface.
 * <br><br>
 * Base HEAL aggregate attribute class for the geometry building phase.
 * <br>
 * <b>Role:</b> <tt>ATTRIB_HH_AGGR_GEOMBUILD_BASE</tt> is the base geometry building aggregate attribute
 * class from which other HEAL aggregate attribute classes used in the geometry building phase are
 * derived. Aggregate attributes are attached to the body being healed to store information about
 * each subphase of the geometry building phase. Aggregate attributes also manage the individual
 * attributes attached to entities of the body during geometry building.
 */
class DECL_HEAL ATTRIB_HH_AGGR_GEOMBUILD_BASE : public ATTRIB_HH_AGGR {
	public:
/**
 * C++ initialize constructor requests memory for this object and populates it with the data supplied as arguments.
 * <br><br>
 * <b>Role:</b> Applications should call this constructor only with the overloaded new operator
 * inherited from the <tt>ENTITY</tt> class (for example, <tt>x=new ATTRIB_HH_AGGR_GEOMBUILD_BASE(...)</tt>),
 * because this reserves the memory on the heap, a requirement to support roll back and history
 * management.
 * <br><br>
 * @param b
 * owning body to heal
 */
		ATTRIB_HH_AGGR_GEOMBUILD_BASE( BODY * b = NULL )
			: ATTRIB_HH_AGGR( b ){}
/**
 * Returns <tt>TRUE</tt> if this is pattern compatible.
 */
        virtual logical pattern_compatible() const;
                                    // returns TRUE
		// Attach an entity attrib to individual entity
/**
 * Attaches an individual entity-level attribute to the given entity.
 * <br><br>
 * <b>Role:</b> This method chains individual attributes.
 * <br><br>
 * @param ent
 * entity to attach to
 */
		virtual ATTRIB_HH_ENT * attach_attrib( ENTITY *ent);

		// Returns entity list of entities having entity attribs
/**
 * Returns the list of entities that have entity-level attributes attached.
 * <br><br>
 * @param ent
 * entities in the body
 */
		virtual void entity_list( ENTITY_LIST &ent ) const;

		// Returns an entity attribute for the given entity.
/**
 * Returns the entity-level attribute for the specified entity.
 * <br><br>
 * @param ent
 * entity with attribute
 */
		virtual ATTRIB_HH_ENT * get_attrib( ENTITY *ent ) const;

		// Removes the attribute from the entity.
/**
 * Removes the entity-level geometry building attributes for the specified individual entity.
 * <br><br>
 * @param ent
 * owning entity
 */
		virtual void detach_attrib( ENTITY *ent );

		// Cleans all entity attributes
/**
 * Removes the entity-level attributes from the individual entities of the owning body.
 */
		virtual void cleanup();

		// detach unused entity attributes
/**
 * Removes unused entity-level attributes from the individual entities of the owning body.
 */
		virtual void detach_redundant_attribs();

		// Attach entity-level attribs on all the entities
		// down the owner body.
/**
 * Attaches the entity-level attributes for the geometry building phase to all the individual entities of the owning body.
 */
		void attach_all_entity_attribs();

		// Miscelleneous functions
/**
 * @nodoc
 */
		virtual void init();

		// Define standard other functions.
		/**
		 * @nodoc
		 */
		ATTRIB_FUNCTIONS( ATTRIB_HH_AGGR_GEOMBUILD_BASE, HEAL )
#if 0
; // semicolon needed for mkman (doc tool) parsing)
#endif
// Because mkman isn't aware that ATTRIB_FUNCTIONS changes access to "public", we do so
// explicitly here:
public:

};
/**
 * @nodoc
 */
ATTRIB_HH_AGGR_GEOMBUILD_BASE* find_aggr_geombuild_base( BODY *);
/**
 * @nodoc
 */
logical hh_attach_healing_attribs(BODY*);
/**
 * @nodoc
 */
DECL_HEAL logical hh_detach_healing_attribs(BODY*);
/** @} */

#endif
