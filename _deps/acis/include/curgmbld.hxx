/*******************************************************************/
/*    Copyright (c) 1989-2020 by Spatial Corp.                     */
/*    All rights reserved.                                         */
/*    Protected by U.S. Patents 5,257,205; 5,351,196; 6,369,815;   */
/*                              5,982,378; 6,462,738; 6,941,251    */
/*    Protected by European Patents 0503642; 69220263.3            */
/*    Protected by Hong Kong Patent 1008101A                       */
/*******************************************************************/

//----------------------------------------------------------------------------

#if !defined	ATTRIB_HH_ENT_GEOMBUILD_CURVE_CLASS
#define			ATTRIB_HH_ENT_GEOMBUILD_CURVE_CLASS
//----------------------------------------------------------------------------
/**
* @file curgmbld.hxx
 * @CAA2Level L1
 * @CAA2Usage U1
 * \addtogroup HEALAPIDEP
 *
 * @{
 */
#include	"dcl_heal.h"
#include	"entgmbld.hxx"

class ATTRIB_HH_AGGR_GEOMBUILD_BASE;
class CURVE;
//----------------------------------------------------------------------------
/**
 * @nodoc
 */
ENTITY_IS_PROTOTYPE(ATTRIB_HH_ENT_GEOMBUILD_CURVE, HEAL )
#if 0
; // semicolon needed for mkman (doc tool) parsing
#endif

DECL_HEAL extern int ATTRIB_HH_ENT_GEOMBUILD_CURVE_TYPE;
/**
 * @nodoc
 */
#define ATTRIB_HH_ENT_GEOMBUILD_CURVE_LEVEL (ATTRIB_HH_ENT_GEOMBUILD_BASE_LEVEL + 1 )

//----------------------------------------------------------------------------
/**
 * @nodoc
 * <b>Obsolete:</b> This class has been deprecated. For healing, use API interfaces
 * and not Direct Interface. For analysis of problems in models, use the Checker 
 * API interfaces instead.
 * <br><br>
 * Individual entity-level healing attribute class attached to curves in the geometry building phase.
 * <br>
 * <b>Role:</b> <tt>ATTRIB_HH_ENT_GEOMBUILD_CURVE</tt> is the individual entity-level attribute class attached
 * to curves during the geometry building phase. Individual entity-level attributes are attached to
 * the individual entities of body being healed to store entity-specific information about each phase
 * or subphase of the healing process. The individual entity-level attributes for each phase or
 * subphase are managed by the aggregate attribute for that phase/subphase.
 */
class DECL_HEAL ATTRIB_HH_ENT_GEOMBUILD_CURVE : public ATTRIB_HH_ENT_GEOMBUILD_BASE {
	protected:
	int continuity;  // = 0 -> continuious, = 1 -> discontinuous
	int degeneracy;  // = 0 -> non-degenerate, = 1 -> degenerate
	int selfint;	 // = 0 -> not selfintersecting, = 1 -> self-intersection
	int closure;	 // = 0 -> not closed, = 1 -> closed
	int approx_fit;  // = 0 -> exact, = 1 -> approximate
// STI cpp begin
	logical save_sw;
// STI cpp end



	public:
/**
 * C++ initialize constructor requests memory for this object and populates it with the data supplied as arguments.
 * <br><br>
 * <b>Role:</b> Applications should call this constructor only with the overloaded <tt>new</tt> operator
 * inherited from the <tt>ENTITY</tt> class (for example, <tt>x=new ATTRIB_HH_ENT_GEOMBUILD_CURVE(...)</tt>), because
 * this reserves the memory on the heap, a requirement to support roll back and history management.
 * <br><br>
 * @param e
 * owning curve.
 */
		ATTRIB_HH_ENT_GEOMBUILD_CURVE( CURVE *e=NULL);

		/**
		 * @nodoc
		 */
		ATTRIB_FUNCTIONS( ATTRIB_HH_ENT_GEOMBUILD_CURVE, HEAL )
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
 * Sets the value of the indicator of whether this is a continuous curve.
 * <br><br>
 * <b>Role:</b> A value of 0 means continuous; 1 means discontinuous.
 * <br><br>
 * @param q
 * new indicator value.
 */
	void set_continuity (int q) {continuity = q;}
/**
 * Sets the value of the indicator of whether this is a degenerate curve.
 * <br><br>
 * <b>Role:</b> A value of 0 means not degenerate; 1 means degenerate.
 * <br><br>
 * @param q
 * new indicator value.
 */
	void set_degeneracy (int q) {degeneracy = q;}
/**
 * Sets the value of the indicator of whether this is a self-intersecting curve.
 * <br><br>
 * <b>Role:</b> A value of 0 means not self-intersecting; 1 means self-intersecting.
 * <br><br>
 * @param q
 * new indicator value.
 */
	void set_selfint (int q) {selfint = q;}
/**
 * Sets the value of the indicator of whether this is a closed curve.
 * <br><br>
 * <b>Role:</b> A value of 0 means not closed; 1 means closed.
 * <br><br>
 * @param c
 * new indicator value.
 */
	void set_closure (int c) {closure = c;}
/**
 * Sets the value of the indicator of whether this is an exact or approximate fit curve.
 * <br><br>
 * <b>Role:</b> A value of 0 means exact fit; 1 means approximate fit.
 * <br><br>
 * @param q
 * new indicator value.
 */
	void set_approx_fit (int q) {approx_fit = q;}

	// Get functions
/**
 * Gets the value of the indicator of whether this is a continuous curve.
 * <br><br>
 * <b>Role:</b> A value of 0 means continuos; 1 means discontinuous.
 */
	int get_continuity () const {return continuity;}
/**
 * Gets the value of the indicator of whether this is a degenerate curve.
 * <br><br>
 * <b>Role:</b> A value of 0 means not degenerate; 1 means degenerate.
 */
	int get_degeneracy () const {return degeneracy;}
/**
 * Gets the value of the indicator of whether this is a self-intersecting curve.
 * <br><br>
 * <b>Role:</b> A value of 0 means not self-intersecting; 1 means self-intersecting.
 */
	int get_selfint () const {return selfint;}
/**
 * Gets the value of the indicator of whether this is a closed curve.
 * <br><br>
 * <b>Role:</b> A value of 0 means not closed; 1 means closed.
 */
	int get_closure () const {return closure;}
/**
 * Gets the value of the indicator of whether this is an exact or approximate fit curve.
 * <br><br>
 * <b>Role:</b> A value of 0 means exact fit; 1 means approximate fit.
 */
	int get_approx_fit () const {return approx_fit;}
/**
 * Returns the curve quality based on values set in the attribute.
 * <br><br>
 * <b>Role:</b> Possible values are
 * <br><br>
 * <table>
 * <tr>
 * <td valign = "top">0&nbsp;&nbsp;</td>
 * <td valign = "top">Good quality. The curve is continuous, and the curve is not degenerate, and the curve does
 * not self-intersect, and the curve is an exact fit.</td>
 * </tr>
 * <tr>
 * <td valign = "top">1&nbsp;&nbsp;</td>
 * <td valign = "top">Poor quality. The curve is not continuous, or the curve is degenerate, or the curve
 * self-intersects, or the curve is an approximate fit.</td>
 * </tr>
 * </table>
 * <br>
 * Returns <tt>UNSET</tt> if neither of these conditions is met.
 */
	int get_quality () const;
/**
 * Performs advanced checks on the owning curve.
 * <br><br>
 * <b>Role:</b> Checks for fit, closure, continuity, and self-intersection.
 */
	int adv_check () ;

	// Reset attribute
/**
 * Resets the attribute.
 * <br><br>
 * <b>Role:</b> Sets the continuity, degeneracy, self-intersection, closure, and fit indicators to
 * <tt>UNSET</tt>.
 */
	void reset() ;


};
/** @} */
#endif  //ATTRIB_HH_ENT_GEOMBUILD_CURVE_CLASS
