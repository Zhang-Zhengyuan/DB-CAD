/*******************************************************************/
/*    Copyright (c) 1989-2020 by Spatial Corp.                     */
/*    All rights reserved.                                         */
/*    Protected by U.S. Patents 5,257,205; 5,351,196; 6,369,815;   */
/*                              5,982,378; 6,462,738; 6,941,251    */
/*    Protected by European Patents 0503642; 69220263.3            */
/*    Protected by Hong Kong Patent 1008101A                       */
/*******************************************************************/
#ifndef _MESH
#define _MESH
#include "dcl_fct.h"
#include "mmgr.hxx"
#include "logical.h"
#include <stdio.h>
/**
 * @file mesh.hxx
 * @CAA2Level L1
 * @CAA2Usage U2
 *! \addtogroup FCTAPI
 *  \brief Declared at <mesh.hxx>
 *  @{
 */

class SPAtransf;
class SPAbox;

/**
 * Defines a generic base mesh class that can be attached to an entity.
 * <br><br>
 * <b>Role :</b> The mesh manager in the faceter leaves it up to the application
 * to determine where it will store the meshes. One possibility is to attach a
 * mesh to its owning face. This class is intended to be a utility for this
 * purpose. If an application derives its meshes from this class, it can also
 * make use of the utility functions provided to work with the meshes.
 * @see ATTRIB_EYE_ATTACHED_MESH
 */
class DECL_FCT MESH : public ACIS_OBJECT {
	protected:
		MESH();
	public:
/**
 * C++ destructor, deleting a MESH.
 */
		virtual ~MESH();
/**
 * Write debug information to a file (stub).
 * <br><br>
 * @param fp
 * debug file stream.
 */
		virtual void debug(FILE *fp);
/**
 * Transform a MESH (stub).
 * <br><br>
 * @param trans
 * transformation.
 */
		virtual logical transform(SPAtransf const& trans);

/**
 * Return the bounding box (stub).
 */
		virtual SPAbox get_box();

/**
 * @nodoc
 */
		virtual void add();

/**
 * @nodoc
 */
		virtual void remove_ref(logical=TRUE);

/**
 * @nodoc
 */
		virtual void set_remove_vertex_template(logical);

/**
 * @nodoc
 */
		virtual logical reverse_mesh();

/**
 * @nodoc
 */
		virtual logical get_reverse_flag();

/**
 * @nodoc
 */
		virtual void set_reverse_flag(logical _reverse_flag);
};

/* @} */
#endif
