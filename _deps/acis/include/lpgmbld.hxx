/*******************************************************************/
/*    Copyright (c) 1989-2020 by Spatial Corp.                     */
/*    All rights reserved.                                         */
/*    Protected by U.S. Patents 5,257,205; 5,351,196; 6,369,815;   */
/*                              5,982,378; 6,462,738; 6,941,251    */
/*    Protected by European Patents 0503642; 69220263.3            */
/*    Protected by Hong Kong Patent 1008101A                       */
/*******************************************************************/

//----------------------------------------------------------------------------

#if !defined	ATTRIB_HH_ENT_GEOMBUILD_LOOP_CLASS
#define			ATTRIB_HH_ENT_GEOMBUILD_LOOP_CLASS
//----------------------------------------------------------------------------
/**
* @file lpgmbld.hxx
 * @CAA2Level L1
 * @CAA2Usage U1
 * \addtogroup HEALAPIDEP
 *
 * @{
 */
#include	"dcl_heal.h"

#include	"entgmbld.hxx"

class ATTRIB_HH_AGGR_GEOMBUILD_BASE;
class LOOP;
//----------------------------------------------------------------------------
/**
 * @nodoc
 */
ENTITY_IS_PROTOTYPE(ATTRIB_HH_ENT_GEOMBUILD_LOOP, HEAL )
#if 0
; // semicolon needed for mkman (doc tool) parsing
#endif

DECL_HEAL extern int ATTRIB_HH_ENT_GEOMBUILD_LOOP_TYPE;
/**
 * @nodoc
 */
#define ATTRIB_HH_ENT_GEOMBUILD_LOOP_LEVEL (ATTRIB_HH_ENT_GEOMBUILD_BASE_LEVEL + 1 )

//----------------------------------------------------------------------------
/**
 * @nodoc
 * <b>Obsolete:</b> This class has been deprecated. For healing, use API interfaces
 * and not Direct Interface. For analysis of problems in models, use the Checker 
 * API interfaces instead.
 * <br><br>
 * Individual entity-level healing attribute class attached to loops in the geometry building phase.
 * <br>
 * <b>Role:</b> <tt>ATTRIB_HH_ENT_GEOMBUILD_LOOP</tt> is the individual entity-level attribute class attached
 * to loops during the geometry building phase. Individual entity-level attributes are attached to
 * the individual entities of body being healed to store entity-specific information about each phase
 * or subphase of the healing process. The individual entity-level attributes for each phase or
 * subphase are managed by the aggregate attribute for that phase/subphase.
 */
class DECL_HEAL ATTRIB_HH_ENT_GEOMBUILD_LOOP : public ATTRIB_HH_ENT_GEOMBUILD_BASE {
	protected:

	int closure;			// = 1 -> loop not closed
	int oriented;			// = 1 -> bad loop orientation
	int gaps;				// = 1 -> loop has gaps between coedges
	int selfint;			// = 1 -> loop self-intersect
	int on_face;			// = 1 -> loop not on face
	int within_domain;      // = 1 -> loop co-edge parameters not correct
	double max_gap;         // = maximum co-edge gap
// STI cpp begin
		logical save_sw;
// STI cpp end



	public:
/**
 * C++ initialize constructor requests memory for this object and populates it with the data supplied as arguments.
 * <br><br>
 * <b>Role:</b> Applications should call this constructor only with the overloaded new operator
 * inherited from the <tt>ENTITY</tt> class (for example, <tt>x=new ATTRIB_HH_ENT_GEOMBUILD_LOOP(...)</tt>), because
 * this reserves the memory on the heap, a requirement to support roll back and history management.
 * <br><br>
 * @param e
 * owning loop.
 */
		ATTRIB_HH_ENT_GEOMBUILD_LOOP( LOOP *e=NULL);
		/**
		 * @nodoc
		 */
		ATTRIB_FUNCTIONS( ATTRIB_HH_ENT_GEOMBUILD_LOOP, HEAL )
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
 * Sets the value of the loop closure indicator.
 * <br><br>
 * @param q
 * new indicator.
 */
	void set_closure (logical q) {closure = q;}
/**
 * Sets the value of the loop orientation indicator.
 * <br><br>
 * @param q
 * new indicator.
 */
	void set_oriented (logical q) {oriented = q;}
/**
 * Sets the value of the coedge gaps indicator.
 * <br><br>
 * @param q
 * new indicator.
 */
	void set_gaps (logical q) {gaps = q;}
/**
 * Sets the value of the loop self-intersection indicator.
 * <br><br>
 * @param q
 * new indicator.
 */
	void set_selfint (logical q) {selfint = q;}
/**
 * Sets the value of the "on face" indicator.
 * <br><br>
 * @param q
 * new indicator.
 */
	void set_on_face (logical q) {on_face = q;}
/**
 * Sets the value of the within domain indicator.
 * <br><br>
 * @param q
 * new indicator.
 */
	void set_within_domain (logical q) {within_domain = q;}
/**
 * Sets the maximum gap.
 * <br><br>
 * @param q
 * new gap value.
 */
	void set_max_gap (double q) {max_gap = q;}

	// Get functions
/**
 * Gets the value of the loop closure indicator.
 */
	int get_closure () const {return closure;}
/**
 * Gets the value of the loop orientation indicator.
 */
	int get_oriented () const {return oriented;}
/**
 * Gets the value of the coedge gaps indicator.
 */
	int get_gaps () const {return gaps;}
/**
 * Gets the value of the loop self-intersection indicator.
 */
	int get_selfint () const {return selfint;}
/**
 * Gets the value of the "on face" indicator.
 */
	int get_on_face () const {return on_face;}
/**
 * Gets the value of the within domain indicator.
 */
	int get_within_domain () const {return within_domain;}
/**
 * Gets the maximum gap.
 */
	double get_max_gap () const {return max_gap;}
/**
 * Returns the loop quality based on values set in the attribute.
 * <br><br>
 * <b>Role:</b> Possible values are
 * <br><br>
 * <table width = 100%>
 * <tr width = 100%>
 * <td valign = "top" width = 5%>0</td>
 * <td valign = "top">Good quality. Loop is closed, <i>and</i> the orientation is good, <i>and</i> there are no coedge gaps,
 * <i>and</i> there are no self-intersections, <i>and</i> the loop is on the face, <i>and</i> the loop is within
 * the domain.</td>
 * </tr>
 * <tr>
 * <td valign = "top">1
 * <td valign = "top">Poor quality. Loop is not closed, <i>or</i> the orientation is bad, <i>or</i> there are coedge gaps, <i>or</i>
 * there are self-intersections, <i>or</i> the loop is not on the face, <i>or</i> the loop is not within the
 * domain.</td>
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
 * <dl>
 * <b>Role:</b> It resets the attribute in the following way:
 * <dd>- Sets the maximum coedge gap to 0.0.</dd>
 * <dd>- Sets the closure, orientation, coedge gaps,self-intersections, "on face", and within domain
 * indicators to <tt>UNSET</tt>.</dd>
 * </dl>
 */
	void reset() ;
/**
 * Performs advanced checks on the owning loop.
 * <br><br>
 * <b>Role:</b> Checks loop closure, loop orientation, coedge gaps, loop self-intersections, whether
 * the loop is on the face, and whether the loop is within domain.
 */
    int adv_check();

};
/** @} */
#endif  //ATTRIB_HH_ENT_GEOMBUILD_LOOP_CLASS
