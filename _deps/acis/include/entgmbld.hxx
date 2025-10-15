/*******************************************************************/
/*    Copyright (c) 1989-2020 by Spatial Corp.                     */
/*    All rights reserved.                                         */
/*    Protected by U.S. Patents 5,257,205; 5,351,196; 6,369,815;   */
/*                              5,982,378; 6,462,738; 6,941,251    */
/*    Protected by European Patents 0503642; 69220263.3            */
/*    Protected by Hong Kong Patent 1008101A                       */
/*******************************************************************/

//----------------------------------------------------------------------------

#if !defined	ATTRIB_HH_ENT_GEOMBUILD_BASE_CLASS
#define			ATTRIB_HH_ENT_GEOMBUILD_BASE_CLASS
//----------------------------------------------------------------------------

/**
* @file entgmbld.hxx
 * @CAA2Level L1
 * @CAA2Usage U1
 * \addtogroup HEALAPIDEP
 *
 * @{
 */
#include	"at_enty_stitch.hxx"
#include	"at_aggr_stitch.hxx"

#include "dcl_heal.h"

class ATTRIB_HH_AGGR_GEOMBUILD_BASE;
//----------------------------------------------------------------------------
/**
 * @nodoc
 */
ENTITY_IS_PROTOTYPE(ATTRIB_HH_ENT_GEOMBUILD_BASE, HEAL )
#if 0
; // semicolon needed for mkman (doc tool) parsing
#endif


DECL_HEAL extern int ATTRIB_HH_ENT_GEOMBUILD_BASE_TYPE;
/**
 * @nodoc
 */
#define ATTRIB_HH_ENT_GEOMBUILD_BASE_LEVEL (ATTRIB_HH_ENT_LEVEL + 1 )
/**
 * @nodoc
 */
#define HH_UNSET -999


/**
 * @nodoc
 */
enum HH_COMPUTED_FLAG
{
	MARKED_FOR_FORCE_COMPUTE = -1,
	UNCOMPUTED = 0,
	COMPUTED = 1
};

// HEAL_TO_TOLERANCE
/**
 * @nodoc
 */
enum HH_ENTITY_GAP_CLASSIFICATION_INFO
{
	HH_GAP_UNINITIALIZED = -1,
	HH_GAP_ALREADY_BAD = 0,
	HH_GAP_ALREADY_GOOD = 1,
	HH_GAP_ALREADY_GOOD_BUT_TOUCHED = 2,
	HH_COULD_NOT_TIGHTEN_GAP = 3
};


//----------------------------------------------------------------------------
/**
 * @nodoc
 * <b>Obsolete:</b> This class has been deprecated. For healing, use API interfaces
 * and not Direct Interface. For analysis of problems in models, use the Checker 
 * API interfaces instead.
 * <br><br>
 * Base HEAL individual entity-level attribute class for the geometry building phase.
 * <br>
 * <b>Role:</b> <tt>ATTRIB_HH_ENT_GEOMBUILD_BASE</tt> is the base geometry building individual entity-level
 * attribute class from which other HEAL individual entity-level attribute classes used in the
 * geometry building phase are derived. Individual entity-level attributes are attached to the
 * individual entities of body being healed to store entity-specific information about each phase
 * or subphase of the healing process. The individual entity-level attributes for each phase or
 * subphase are managed by the aggregate attribute for that phase/subphase.
 */
class DECL_HEAL ATTRIB_HH_ENT_GEOMBUILD_BASE : public ATTRIB_HH_ENT {
	protected:
		// whether this attrib has been used for storing geometry
		logical m_unused;

		// whether the new geometry (if existing,
		// otherwise old geometry) is bad
		int m_bad;

		// whether the geometry is godd in the beginning
		int m_good_incoming;

		// whether a new geometry has been computed for the
		// owning entity
		HH_COMPUTED_FLAG m_computed;


	public:
/**
 * C++ initialize constructor requests memory for this object and populates it with the data supplied as arguments.
 * <br><br>
 * <b>Role:</b> Applications should call this constructor only with the overloaded new operator
 * inherited from the <tt>ENTITY</tt> class (for example, <tt>x=new ATTRIB_HH_ENT_GEOMBUILD_BASE(...)</tt>), because
 * this reserves the memory on the heap, a requirement to support roll back and history management.
 * <br><br>
 * @param e
 * owning entity.
 */
		ATTRIB_HH_ENT_GEOMBUILD_BASE( ENTITY *e=NULL);
		
/**
 * Returns <tt>TRUE</tt> if this is pattern compatible.
 */
        virtual logical pattern_compatible() const;
                                    // returns TRUE
		// get/set functions for logicals
/**
 * Gets the value of the <tt>m_unused</tt> flag, which indicates whether this attribute has been used for storing geometry.
 */
		virtual logical unused() const {return m_unused;}
/**
 * Sets the value of the <tt>m_unused</tt> flag, which indicates whether this attribute has been used for storing geometry.
 */
		virtual void got_used() {
			if (m_unused) backup();
			m_unused = FALSE;
		}
/**
 * Returns the value of the <tt>m_bad</tt> flag.
 */
		virtual logical is_bad();

/**
 * @nodoc
 */
		// Project: HEAL_TO_TOLERANCE
		virtual logical is_bad_with_tol(double tol);

/**
 * Sets the value of the <tt>m_bad</tt> flag, which indicates whether the geometry (new if present; otherwise, old) is bad.
 * <br><br>
 * @param val
 * new flag value.
 */
		virtual void set_bad(logical val) {backup(); m_bad = val;}
/**
 * Returns the value of the <tt>m_computed</tt> flag.
 */
		virtual logical is_computed() {return m_computed == COMPUTED;}
/**
 * Sets the value of the <tt>m_computed</tt> flag.
 * <br><br>
 * @param val
 * new flag value.
 */
		virtual void set_computed(logical val)
		{
			got_used();
			backup();
			if (val == TRUE)
				m_computed = COMPUTED;
			else
				m_computed = UNCOMPUTED;
		}

		// return
		//		1 if good incoming geometry
		//		0 if bad incoming geometry
		//		-1 if not determined
/**
 * Checks the geometry.
 * <br><br>
 * <b>Role:</b> Checks the geometry and returns
 * <table>
 * <tr>
 * <td valign = "top">1&nbsp;&nbsp;</td>
 * <td valign = "top">if good incoming geometry.</td>
 * </tr>
 * <tr>
 * <td valign = "top">0&nbsp;&nbsp;</td>
 * <td valign = "top">if bad incoming geometry.</td>
 * </tr>
 * <tr>
 * <td valign = "top">-1&nbsp;&nbsp;</td>
 * <td valign = "top">if not determined.</td>
 * </tr>
 * </table>
 */
 		virtual int is_good_incoming() {return m_good_incoming;}
/**
 * This instance is marked for a force compute.
 */
		virtual void mark_for_force_compute()
			{backup(); m_computed = MARKED_FOR_FORCE_COMPUTE;}
/**
 * Returns a flag indicating whether or not this instance is marked for a force compute.
 */
		virtual logical is_marked_for_force_compute()
			{return m_computed == MARKED_FOR_FORCE_COMPUTE;}

/**
 * @nodoc
 */
		virtual logical does_not_deviate() {return TRUE;}
/**
 * Returns a flag indicating whether or not healing is required.
 */
		virtual logical is_healing_required();

/**
 * @nodoc
 */
		// Project : HEAL_TO_TOLERANCE
		virtual logical is_healing_required_with_tol(double tol);

/**
 * Virtual function that must be implemented by classes derived from this one for performing advanced checks and for this class, always returns <tt>UNSET</tt>.
 */
		virtual int adv_check() { return HH_UNSET;}
/**
 * Resets the value of the <tt>m_bad</tt> flag (to -1).
 */
		virtual void reset() {backup(); m_bad = -1;}


	/**
	 * @nodoc
	 */
	ATTRIB_FUNCTIONS( ATTRIB_HH_ENT_GEOMBUILD_BASE, HEAL )
#if 0
; // semicolon needed for mkman (doc tool) parsing)
#endif
// Because mkman isn't aware that ATTRIB_FUNCTIONS changes access to "public", we do so
// explicitly here:
public:

};

//----------------------------------------------------------------------------

/**
 * @nodoc
 */
ATTRIB_HH_ENT_GEOMBUILD_BASE * find_att_entity_geombuild( ENTITY * );

/** @} */
#endif
