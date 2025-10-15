/*******************************************************************/
/*    Copyright (c) 1989-2020 by Spatial Corp.                     */
/*    All rights reserved.                                         */
/*    Protected by U.S. Patents 5,257,205; 5,351,196; 6,369,815;   */
/*                              5,982,378; 6,462,738; 6,941,251    */
/*    Protected by European Patents 0503642; 69220263.3            */
/*    Protected by Hong Kong Patent 1008101A                       */
/*******************************************************************/

#if !defined (ATTRIB_HH_ENT_SIMPLIFY_FACE_CLASS)
#define ATTRIB_HH_ENT_SIMPLIFY_FACE_CLASS
#include "surface.hxx"
#include "allsurf.hxx"
#include "dcl_heal.h"
#include "att_hh.hxx"
#include "entsimbs.hxx"
#include "entity_simplify.hxx"
/**
 * @file entsimg.hxx
 * @CAA2Level L1
 * @CAA2Usage U1
 * \addtogroup HEALAPIDEP
 *
 * @{
 */
/**
 * @nodoc
 */
ENTITY_IS_PROTOTYPE(ATTRIB_HH_ENT_SIMPLIFY_FACE, HEAL)
#if 0
; // semicolon needed for mkman (doc tool) parsing
#endif

DECL_HEAL extern int ATTRIB_HH_ENT_SIMPLIFY_FACE_TYPE;

/**
 * @nodoc
 */
#define ATTRIB_HH_ENT_SIMPLIFY_FACE_LEVEL (ATTRIB_HH_ENT_SIMPLIFY_BASE_LEVEL + 1)

class ATTRIB_HH_AGGR_SIMPLIFY;
/**
 * @nodoc
 * <b>Obsolete:</b> This class has been deprecated. For healing, use API interfaces
 * and not Direct Interface.
 * <br><br>
 * Individual entity-level healing attribute class attached to faces in the geometry simplification phase.
 * <br>
 * <b>Role:</b> <tt>ATTRIB_HH_ENT_SIMPLIFY_FACE</tt> is the individual entity-level attribute class attached
 * to faces during the geometry simplification phase. Individual entity-level attributes are attached
 * to the individual entities of body being healed to store entity-specific information about each
 * phase or subphase of the healing process. The individual entity-level attributes for each phase
 * are managed by the aggregate attribute for that phase.
 * @see SURFACE
 */
class DECL_HEAL ATTRIB_HH_ENT_SIMPLIFY_FACE : public ATTRIB_HH_ENT_SIMPLIFY_BASE
{
protected:

	SURFACE* m_surf;	// Simplified surface
	double  m_tol;	// Tolerance at which surface
				// is simplified.

	SURFACE* m_old_geom; // Old geometry of face

	// Lose the simplified surface

	void lose_surf();
// STI cpp begin
		logical save_sw;
// STI cpp end

public:
/**
 * C++ initialize constructor requests memory for this object and populates it with the data supplied as arguments.
 * <br><br>
 * <b>Role:</b> Applications should call this constructor only with the overloaded new operator
 * inherited from the <tt>ENTITY</tt> class (for example, <tt>x=new ATTRIB_HH_ENT_SIMPLIFY_FACE(...)</tt>), because
 * this reserves the memory on the heap, a requirement to support roll back and history management.
 * <br><br>
 * @param own_face
 * owning face.
 */
	ATTRIB_HH_ENT_SIMPLIFY_FACE (FACE *own_face = NULL);
/**
 * Returns <tt>TRUE</tt> if this is pattern compatible.
 */
    virtual logical pattern_compatible() const;
                                    // returns TRUE
	// Analyze the face at a given tolerance. This would be
	// called by the aggregate analyze function. This could
	// also be directly from UI with user-defined tolerance.
/**
 * Analyzes the face at a given tolerance.
 * <br><br>
 * <b>Role:</b> This is called by the aggregate analyze function, but it could also be called
 * directly from a user interface with a user-defined tolerance.
 */
	SURFACE* simplify();

	// Get the simplified surface
/**
 * Gets the simplified surface.
 */
 	SURFACE* get_surf() const;

	// Get the old surface
/**
 * Returns the old surface.
 */
	SURFACE* old_surf() const {return m_old_geom;}

	// Get the simplification tolerance used for calculating
	// the simplified surface
/**
 * Gets the simplification tolerance used for calculating the simplified surface.
 */
	double tol() const;

	// Set the simplification tolerance used for calculating
	// the simplified surface
/**
 * Sets the simplification tolerance used for calculating the simplified surface.
 * <br><br>
 * @param tol
 * new tolerance.
 */
	void set_tol(double tol);

	// Set function for simplified surface. Note that the
	// old surface will get lost.
/**
 * Sets the new simplified surface and the old surface is lost.
 * <br><br>
 * @param surf
 * simplified surface.
 */
	void set_surf(SURFACE *surf);

	// Get the surface identity of the owner surface
/**
 * Gets the surface identity of the owner surface.
 */
	int surf_identity();

	// Get the identity of the expected surface
/**
 * Gets the identity of the expected surface.
 */
	int expected_surf_identity();

	// Set the surface geometry of the owner face
	// to the simplified surface

/**
 * Sets the surface geometry of the owner face to the simplified geometry.
 * <br><br>
 * <b>Role:</b> Applies (fixes) all the changes for the geometry simplification phase that are
 * stored in this individual attribute to the body. The old geometry is then stored in the attribute.
 */
	void fix();

	// RNC : 5 july 99
	// RNC : functions for force simplification of surfaces
/**
 * Simplifies the spline to a plane.
 */
	PLANE* force_simplify_to_plane();
/**
 * Simplifies the spline to a cylinder.
 */
	CONE* force_simplify_to_cylinder();
/**
 * Simplifies the spline to a cone.
 */
	CONE* force_simplify_to_cone();
/**
 * Simplifies the spline to a sphere.
 */
	SPHERE* force_simplify_to_sphere();
/**
 * Simplifies the spline to a torus.
 */
	TORUS* force_simplify_to_torus();

	// Standard Attrib functions

	//*****************************************************************

	// RNC : 8 july 99 for logging in changes in attributes to entities.

	// Functions added for getting surface type.
/**
 * Returns whether the surface type is a plane.
 */
	logical is_plane_type();
/**
 * Returns whether the surface type is a cone.
 */
	logical is_cone_type();
/**
 * Returns whether the surface type is a cylinder.
 */
	logical is_cylinder_type();
/**
 * Returns whether the surface type is a sphere.
 */
	logical is_sphere_type();
/**
 * Returns whether the surface type is a torus.
 */
	logical is_torus_type();

/**
 * Set logging details.
 */
    void set_log_details();
/**
 * Returns the surface data from the log.
 * <br><br>
 * @param surf_data
 * surface data.
 */
    void sprint_log_details(char *surf_data);

	//*****************************************************************

	/**
	 * @nodoc
	 */
	ATTRIB_FUNCTIONS (ATTRIB_HH_ENT_SIMPLIFY_FACE, HEAL)
#if 0
; // semicolon needed for mkman (doc tool) parsing)
#endif
// Because mkman isn't aware that ATTRIB_FUNCTIONS changes access to "public", we do so
// explicitly here:
public:

	/**
	 * @nodoc
	 */
	ROLL_NOTIFY_DECL
#if 0
; // semicolon needed for mkman (doc tool) parsing)
#endif
};
/**
 * @nodoc
 */
DECL_HEAL void hh_force_simplify_to_cylinder(FACE* inp_face);

/**
 * @nodoc
 */
DECL_HEAL void hh_force_simplify_to_cone(FACE* inp_face);

/**
 * @nodoc
 */
DECL_HEAL void hh_force_simplify_to_plane(FACE* inp_face);

/**
 * @nodoc
 */
DECL_HEAL void hh_force_simplify_to_sphere(FACE* inp_face);

/**
 * @nodoc
 */
DECL_HEAL void hh_force_simplify_to_torus(FACE* inp_face);

/**
 * @nodoc
 */
DECL_HEAL void hh_fix_simgeom(FACE* inp_face);

/** @} */

#endif // ATTRIB_HH_ENT_SIMPLIFY_FACE_CLASS


