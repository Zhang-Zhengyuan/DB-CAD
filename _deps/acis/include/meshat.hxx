/*******************************************************************/
/*    Copyright (c) 1989-2020 by Spatial Corp.                     */
/*    All rights reserved.                                         */
/*    Protected by U.S. Patents 5,257,205; 5,351,196; 6,369,815;   */
/*                              5,982,378; 6,462,738; 6,941,251    */
/*    Protected by European Patents 0503642; 69220263.3            */
/*    Protected by Hong Kong Patent 1008101A                       */
/*******************************************************************/
#ifndef _MESHAT
#define _MESHAT
#include "dcl_fct.h"
#include "mesh.hxx"
#include "atteye3d.hxx"
/**
 * @file meshat.hxx
 * @CAA2Level L1
 * @CAA2Usage U1
 *! \addtogroup FCTAPI
 *  \brief Declared at <meshat.hxx>
 *  @{
 */
class law;
/**
 * @nodoc
 */
ENTITY_IS_PROTOTYPE(ATTRIB_EYE_ATTACHED_MESH, FCT )
#if 0
; // semicolon needed for mkman (doc tool) parsing
#endif

extern DECL_FCT int ATTRIB_EYE_ATTACHED_MESH_TYPE;

/*
// tbrv
*/
/**
 * @nodoc
 */
#define ATTRIB_EYE_ATTACHED_MESH_LEVEL ( ATTRIB_EYE_LEVEL + 1)

typedef uintptr_t MESH_APP_ID;
typedef uintptr_t MESH_USER_ID;

/**
 * Defines an attribute to attach facets to an entity as a <tt>MESH</tt>.
 * <br><br>
 * <b>Role:</b> The mesh manager in the faceter leaves it up to the application
 * to determine where it will store the meshes. One possibility is to attach a
 * mesh to its owning face. This class is intended to be a utility for this purpose.
 * It attaches any mesh that is derived from <tt>MESH</tt>. Typically it will be used to
 * attach the mesh (facets) of a face to the face.
 * <br><br>
 * <b>Sat Identifier:</b> fmesh
 * @see MESH
 */
class DECL_FCT ATTRIB_EYE_ATTACHED_MESH : public ATTRIB_EYE
{
	MESH *meshptr;
	MESH_APP_ID app_id;
	MESH_USER_ID user_id;
	// ywoo 20Feb01: new member variable added.
	unsigned int state_id;
 public:
 /**
  * C++ initialize constructor requests memory for this object and populates it with the data supplied as arguments.
  * <br><br>
  * <b>Role:</b> Applications should call this constructor only with the overloaded
  * new operator inherited from the <tt>ENTITY</tt> class (for example,
  * x=new <tt>ATTRIB_EYE_ATTACHED_MESH(...)</tt>), because this reserves the memory on the
  * heap, a requirement to support roll back and history management.
  * <br><br>
  * Initializes internal data. The application identification should be unique
  * to the application. The user identification allows multiple meshes to be
  * attached to the entity.
  * <br><br>
  * @param ent
  * owner of this mesh.
  * @param meshptr
  * mesh to attach.
  * @param app_id
  * application id.
  * @param user_id
  * user id.
  */
	ATTRIB_EYE_ATTACHED_MESH(
		ENTITY* ent= NULL,
		MESH *meshptr	= NULL,
		MESH_APP_ID app_id	= 0,
		MESH_USER_ID user_id = 0);

    // returns TRUE

/**
* Returns TRUE if this is pattern compatible.
*/
  virtual logical pattern_compatible() const;

/**
 * Returns the mesh of this attribute.
 */
	MESH *get_mesh() ;

/**
 * Returns the mesh constant of this attribute.
 */
	MESH const *get_mesh_const() const ;

/**
 * Returns the application identification of this attribute.
 */
	MESH_APP_ID get_app_id();

/**
 * Returns the user identification of this attribute.
 */
	MESH_USER_ID get_user_id();

/**
 * Sets the mesh of this attribute.
 * <br><br>
 * @param imesh
 * mesh to set.
 */
	void set_mesh(MESH * imesh);

/**
 * Sets the application identification of this attribute.
 * <br><br>
 * @param appid
 * application id to set.
 */
	void set_app_id(MESH_APP_ID appid);

/**
 * Sets the user identification of this attribute.
 * <br><br>
 * @param userid
 * user id to set.
 */
	void set_user_id(MESH_USER_ID userid);
	// ywoo 20Feb01: added new funtions.
/**
 * Increment the state ID.
 */

	void change_state_id();


/**
 * @nodoc
 */
	virtual void roll_notify( BULLETIN_TYPE, ENTITY * );
	// ywoo:end

	// Migration functions for modeler:

	// Transform the owner.
	/**
	 * @nodoc
	 */
	virtual void trans_owner( SPAtransf const & trans );

	// replace the owner geometry
	/**
	 * @nodoc
	 */
	virtual void replace_owner_geometry( ENTITY* );

	// owner is reversed
	/**
	 * @nodoc
	 */
	virtual void reverse_owner();

/**
 * @nodoc
 */
	ATTRIB_FUNCTIONS ( ATTRIB_EYE_ATTACHED_MESH, FCT )

/**
 * @nodoc
 */
	logical savable() const;

#if 0
; // semicolon needed for mkman (doc tool) parsing)
#endif
// Because mkman isn't aware that ATTRIB_FUNCTIONS changes access to "public", we do so
// explicitly here:
public:

};
//*****************************************************
// af_query(ENTITY*, MESH_APP_ID,MESH_USER_ID, MESH*&)
// Search for a mesh attached to an entity.
//*****************************************************


 
/**
* Gets the mesh attached to an entity.
* <br><br>
* <b>Role:</b> Gets the mesh attached to an entity given the application id and user id
* <br><br>
* @param entity
* given entity.
* @param app
* application id.
* @param uid
* user id.
* @param mesh
* mesh attached to the given entity.
**/

DECL_FCT logical af_query(const ENTITY *entity, MESH_APP_ID app, MESH_USER_ID uid,
		MESH *&mesh);

//****************************************************
// af_query(const ENTITY *,ATTRIB_EYE_ATTACHED_MESH*&)
// Find the first mesh attribute on an entity.
//****************************************************

/**
 * @nodoc
 */

DECL_FCT logical af_query(const ENTITY *entity, ATTRIB_EYE_ATTACHED_MESH *&meshat);

//******************************************************
// af_query(const ATTRIB_EYE_ATTACHED_MESH*,ATTRIB_EYE_ATTACHED_MESH *&)
// Find the next mesh attribute after a given one on an entity.
//******************************************************

/**
 * @nodoc
 */

DECL_FCT logical af_query(const ATTRIB_EYE_ATTACHED_MESH *meshat0,
		ATTRIB_EYE_ATTACHED_MESH *&meshat);

//*****************************************************
// af_update(ENTITY*, MESH_APP_ID,MESH_USER_ID, MESH*&)
// Add (or replace) the mesh attached to an entity.
//*****************************************************
/**
* Attaches the mesh to an entity using a mesh attribute.
* <br><br>
* <b>Role:</b> Attaches the mesh to the attribute on the given entity.
* <br><br>
* If a mesh attribute does not exist, creates and attaches a mesh attribute to the entity.
* <br><br>
* <b>Effect:</b> Changes model.
* <br><br>
* @param entity
* given entity.
* @param app
* application id.
* @param uid
* user id.
* @param mesh
* faceter's mesh class.
* @param delete_old
* delete previously attached mesh.
**/

DECL_FCT logical af_update(ENTITY *entity, MESH_APP_ID app, MESH_USER_ID uid,
		MESH *mesh, logical delete_old = TRUE);

//******************************************************
// af_delete_facets(ENTITY*, logical)
//	-- delete all facets attached to an entity
//******************************************************

/**
* Deletes all facets attached to an entity.
* <br><br>
* <b>Role:</b> Deletes all facets attached to an entity.
* In addition to meshes, this function deletes the facet information attached to the edges of the entity.
* <br><br>
* <b>Effect:</b> Changes model.
* <br><br>
* @param entity
* given entity.
* @param keep_mark
* retain the "mark" on the entity indicating it is faceted.
**/

DECL_FCT logical af_delete_facets(ENTITY *entity, logical keep_mark = FALSE);
/**
* Deletes all meshes attached to an entity.
* <br><br>
* <b>Role:</b> Deletes all meshes attached to an entity.
* <br><br>
* <b>Effect:</b> Changes model.
* @param entity
* given entity.
**/
void DECL_FCT af_delete_mesh(ENTITY *entity);

/**
* Finds the mesh attribute attached to an entity.
* <br><br>
* <b>Role:</b> Finds the mesh attribute attached to an entity with the given user id and application id.
* <br><br>
* <b>Effect:</b> Changes model.
* <br><br>
* @param entity
* given entity.
* @param app
* application id.
* @param uid
* user id.
* @param meshat
* output mesh attribute.
**/
DECL_FCT logical af_find_meshat(const ENTITY *entity,
		MESH_APP_ID app, MESH_USER_ID uid,
		ATTRIB_EYE_ATTACHED_MESH *&meshat);
/* @} */

/**
 * @nodoc
 */
#define	MASSPROPS_MESH_ID	0x0049445A

#endif
