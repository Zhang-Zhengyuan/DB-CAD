/*******************************************************************/
/*    Copyright (c) 1989-2020 by Spatial Corp.                     */
/*    All rights reserved.                                         */
/*    Protected by U.S. Patents 5,257,205; 5,351,196; 6,369,815;   */
/*                              5,982,378; 6,462,738; 6,941,251    */
/*    Protected by European Patents 0503642; 69220263.3            */
/*    Protected by Hong Kong Patent 1008101A                       */
/*******************************************************************/

//----------------------------------------------------------------------------

#if !defined	ATTRIB_HH_ENT_GEOMBUILD_SURFACE_CLASS
#define			ATTRIB_HH_ENT_GEOMBUILD_SURFACE_CLASS
//----------------------------------------------------------------------------
/**
* @file surgmbld.hxx
 * @CAA2Level L1
 * @CAA2Usage U1
 * \addtogroup HEALAPIDEP
 *
 * @{
 */
#include	"dcl_heal.h"
#include	"entgmbld.hxx"

class ATTRIB_HH_AGGR_GEOMBUILD_BASE;
class SURFACE;
//----------------------------------------------------------------------------
/**
 * @nodoc
 */
ENTITY_IS_PROTOTYPE(ATTRIB_HH_ENT_GEOMBUILD_SURFACE, HEAL )
#if 0
; // semicolon needed for mkman (doc tool) parsing
#endif

DECL_HEAL extern int ATTRIB_HH_ENT_GEOMBUILD_SURFACE_TYPE;
/**
 * @nodoc
 */
#define ATTRIB_HH_ENT_GEOMBUILD_SURFACE_LEVEL (ATTRIB_HH_ENT_GEOMBUILD_BASE_LEVEL + 1 )

//----------------------------------------------------------------------------
/**
 * @nodoc
 * <b>Obsolete:</b> This class has been deprecated. For healing, use API interfaces
 * and not Direct Interface. For analysis of problems in models, use the Checker 
 * API interfaces instead.
 * <br><br>
 * Individual entity-level healing attribute class attached to surfaces in the geometry building phase.
 * <br>
 * <b>Role:</b> <tt>ATTRIB_HH_ENT_GEOMBUILD_SURFACE</tt> is the individual entity-level attribute class
 * attached to surfaces during the geometry building phase. Individual entity-level attributes are
 * attached to the individual entities of body being healed to store entity-specific information
 * about each phase or subphase of the healing process. The individual entity-level attributes for
 * each phase or subphase are managed by the aggregate attribute for that phase/subphase.
 */
class DECL_HEAL ATTRIB_HH_ENT_GEOMBUILD_SURFACE : public ATTRIB_HH_ENT_GEOMBUILD_BASE {
	protected:


	int continuity;			// = 1 -> surface discontiuities
	int degeneracy;			// = 1 -> surface degeneracies
	int selfint;			// = 1 -> surface self-intersect
	int closure;			// = 1 -> surface closed
	int approx_fit;			// = 1 -> approximate fitted surface
// STI cpp begin
		logical save_sw;
// STI cpp end


	public:
/**
 * C++ initialize constructor requests memory for this object and populates it with the data supplied as arguments.
 * <br><br>
 * <b>Role:</b> Applications should call this constructor only with the overloaded new operator
 * inherited from the <tt>ENTITY</tt> class (for example, <tt>x=new ATTRIB_HH_ENT_GEOMBUILD_SURFACE(...)</tt>), because
 * this reserves the memory on the heap, a requirement to support roll back and history management.
 * <br><br>
 * @param e
 * owning surface.
 */
		ATTRIB_HH_ENT_GEOMBUILD_SURFACE( SURFACE *e=NULL);
		/**
		 * @nodoc
		 */
		ATTRIB_FUNCTIONS( ATTRIB_HH_ENT_GEOMBUILD_SURFACE, HEAL )
#if 0
; // semicolon needed for mkman (doc tool) parsing)
#endif
// Because mkman isn't aware that ATTRIB_FUNCTIONS changes access to "public", we do so
// explicitly here:
public:

/**
 * Returns <tt>TRUE</tt> if this is pattern compatible.
 */
    virtual logical pattern_compatible() const;
                                    // returns TRUE
// Set functions
/**
 * Sets the value of the continuity indicator.
 * <br><br>
 * @param q
 * new indicator.
 */
	void set_continuity (int q) {continuity = q;}
/**
 * Sets the value of the degeneracy indicator.
 * <br><br>
 * @param q
 * new indicator.
 */
	void set_degeneracy (int q) {degeneracy = q;}
/**
 * Sets the value of the self-intersection indicator.
 * <br><br>
 * @param q
 * new indicator.
 */
	void set_selfint (int q) {selfint = q;}
/**
 * Sets the value of the closure indicator.
 * <br><br>
 * @param q
 * new indicator.
 */
	void set_closure (int q) {closure = q;}
/**
 * Sets the value of the approximate fit indicator.
 * <br><br>
 * @param q
 * new indicator.
 */
	void set_approx_fit (int q) {approx_fit = q;}

	// Get functions
/**
 * Gets the value of the indicator of whether this is a continuous surface.
 */
	int get_continuity () const {return continuity;}
/**
 * Gets the value of the indicator of whether this is a degenerate surface.
 */
	int get_degeneracy () const {return degeneracy;}
/**
 * Gets the value of the indicator of whether this is a self-intersecting surface.
 */
	int get_selfint () const {return selfint;}
/**
 * Gets the value of the indicator of whether this is a closed surface.
 */
	int get_closure () const {return closure;}
/**
 * Gets the value of the indicator of whether this is an exact or approximate fit surface.
 */
	int get_approx_fit () const {return approx_fit;}
/**
 * Returns the surface quality based on values set in the attribute.
 * <br><br>
 * <b>Role:</b> Possible values are
 * <br><br>
 * <table width = 100%>
 * <tr width = 100%>
 * <td valign = "top" width = 5%>0</td>
 * <td valign = "top">Good quality. The surface is continuous, <i>and</i> the surface is not degenerate, <i>and</i> the surface
 * does not self-intersect.</td>
 * </tr>
 * <tr>
 * <td valign = "top">1</td>
 * <td valign = "top">Poor quality. The surface is not continuous, <i>or</i> the surface is degenerate, <i>or</i> the surface
 * self-intersects.</td>
 * </tr>
 * </table>
 * <br>
 * Returns <tt>UNSET</tt> if neither of these conditions is met.
 */
	int get_quality () const;

	// Reset attribute
/**
 * Resets the attribute.
 * <br><br>
 * <b>Role:</b> Sets the continuity, degeneracy, self-intersection, closure, and fit indicators to <tt>UNSET</tt>.
 */
	void reset() ;
/**
 * Performs advanced checks on the owning surface.
 * <br><br>
 * <b>Role:</b> Checks for fit, closure, continuity, and self-intersection.
 */
	int adv_check();


};
/** @} */
#endif  //ATTRIB_HH_ENT_GEOMBUILD_SURFACE_CLASS
