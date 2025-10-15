/*******************************************************************/
/*    Copyright (c) 1989-2020 by Spatial Corp.                     */
/*    All rights reserved.                                         */
/*    Protected by U.S. Patents 5,257,205; 5,351,196; 6,369,815;   */
/*                              5,982,378; 6,462,738; 6,941,251    */
/*    Protected by European Patents 0503642; 69220263.3            */
/*    Protected by Hong Kong Patent 1008101A                       */
/*******************************************************************/
#ifndef SPA_ENTITY_MOD_CALLBACK_HXX
#define SPA_ENTITY_MOD_CALLBACK_HXX

#include "mmgr.hxx"

class ENTITY;
class law;

/**
 * Represents a class that provides a callback mechanism to track ENTITY modifications.
 * <br>
 * <b>Role:</b> Users can derive from the <tt>entity_modification_callback</tt> class and override
 * virtual methods to receive notifications when entities are modified, for example when they are split or merged.
 * These methods are called just prior to the normal attribute callbacks that are invoked on attached attributes.
 * Careful consideration shoud be given to the implementation of the methods since they are called frequently
 * and can impact performance.
 * @see ATTRIB, split_attrib, merge_attrib, trans_attrib, copy_attrib, reverse_attrib, warp_attrib, 
 * lop_change_attrib, replace_geometry_attrib, replace_attrib, to_tolerant_attrib, from_tolerant_attrib
 */

 // Do not use without consulting spatial kernel developers.
 /**
 * @nodoc
 */
class DECL_KERN entity_modification_callback : public ACIS_OBJECT
{
	entity_modification_callback* _previous_callback;

public:

	/**
	* Constructor installs the custom entity_modification_callback class.
	* <br>
	* <b>Role:</b> The current custom entity_modification_callback class is activated and the 
	* previous one is saved to be reinstated in the destructor. This provides push-pop
	* behaviour, to automatically maintain multiple callbacks.
	*/
	/**
	* @nodoc
	*/
	entity_modification_callback();

	/**
	* Destructor reinstates the previous entity_modification_callback class.
	*/
	/**
	* @nodoc
	*/
	virtual ~entity_modification_callback();

	/**
	* An ENTITY is being split.
	* <br><br>
	* @param old_ent
	* the entity being split.
	* @param new_ent
	* the new entity that represents "the other side" of the split.
	* @param gap_ent
	* the entity "between" the old_ent and new_ent.
	* @see split_owner
	*/
	/**
	* @nodoc
	*/
	virtual void split( ENTITY *old_ent, ENTITY *new_ent, ENTITY *gap_ent) {}

	/**
	* Two entities are being merged.
	* <br><br>
	* @param keep_ent
	* the entity being "kept".
	* @param go_ent
	* the entity "going away".
	* @see merge_owner
	*/
	/**
	* @nodoc
	*/
	virtual void merge( ENTITY *keep_ent, ENTITY *go_ent) {}

	/**
	* An ENTITY is being transformed.
	* <br><br>
	* @param owner
	* the entity being transformed.
	* @param trans
	* the transform to apply.
	* @param list
	* list of entities already transformed.
	* @see trans_owner
	*/
	/**
	* @nodoc
	*/
	virtual void trans( ENTITY *owner, SPAtransf const &trans, ENTITY_LIST *list) {}

	/**
	* An ENTITY's sense bit is reversed.
	* <br><br>
	* @param owner
	* the entity whose sense bit is reversed.
	* @see reverse_owner
	*/
	/**
	* @nodoc
	*/
	virtual void reverse( ENTITY *owner) {}

	/**
	* An ENTITY is changed according to warp law.
	* <br><br>
	* @param owner
	* the entity being warped.
	* @param warp
	* the warp law being applied.
	* @see warp_owner
	*/
	/**
	* @nodoc
	*/
	virtual void warp( ENTITY *owner, law *warp) {}

	/**
	* An ENTITY is modified during local operations.
	* <br><br>
	* @param owner
	* the owning entity.
	* @see lop_change_owner
	*/
	/**
	* @nodoc
	*/
	virtual void lop_change( ENTITY *owner) {}

	/** 
	* An ENTITY is getting new geometry.
	* @param ent
	* the owning entity.
	* @param new_geom
	* the new geometry.
	* @see replace_owner_geometry
	*/
	/**
	* @nodoc
	*/
	virtual void replace_geometry( ENTITY *owner, ENTITY *new_geometry) {}

	/** 
	* An ENTITY is being replaced by another ENTITY.
	* <br><br>
	* @param keep_ent
	* the entity that is being "kept".
	* @param replace_ent
	* the entity that is being "replaced".
	* @see replace_owner
	*/
	/**
	* @nodoc
	*/
	virtual void replace( ENTITY *keep_ent, ENTITY *replace_ent) {}

	/** 
	* An ENTITY is being copied.
	* <br><br>
	* @param source
	* the source entity.
	* @param target
	* the target entity.
	* @see copy_owner
	*/
	/**
	* @nodoc
	*/
	virtual void copy( ENTITY *source, ENTITY *target) {}

	/** 
	* An ENTITY is being copied via the COPY_DEF.
	* <br><br>
	* @param source
	* the source entity.
	* @param target
	* the target entity.
	* @see copy_owner
	*/
	/**
	* @nodoc
	*/
	virtual void copy_def( ENTITY *source, ENTITY *target) {}

	/** 
	* An ENTITY is being replaced with tolerant ENTITY.
	* @param original_ent
	* the original entity that is being deleted.
	* @param tol_ent
	* the newly created tolerant entity.
	* @see to_tolerant_owner
	*/
	/**
	* @nodoc
	*/
	virtual void to_tolerant( ENTITY *original_ent, ENTITY *tol_ent) {}

	/** 
	* A tolerant ENTITY is being replaced with non-tolerant ENTITY.
	* @param original_ent
	* the original entity that is being deleted.
	* @param non_tol_ent
	* the newly created non-tolerant entity.
	* @see from_tolerant_owner
	*/
	/**
	* @nodoc
	*/
	virtual void from_tolerant( ENTITY *original_ent, ENTITY *non_tol_ent) {}
};

// Do not use without consulting spatial kernel developers.
/**
 * @nodoc
 * Internal use.
 */
void copy_via_copy_def(	ENTITY* original_ent, ENTITY* copied_ent);

#endif //SPA_ENTITY_MOD_CALLBACK_HXX
