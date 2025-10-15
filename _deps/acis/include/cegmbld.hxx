/*******************************************************************/
/*    Copyright (c) 1989-2020 by Spatial Corp.                     */
/*    All rights reserved.                                         */
/*    Protected by U.S. Patents 5,257,205; 5,351,196; 6,369,815;   */
/*                              5,982,378; 6,462,738; 6,941,251    */
/*    Protected by European Patents 0503642; 69220263.3            */
/*    Protected by Hong Kong Patent 1008101A                       */
/*******************************************************************/

//----------------------------------------------------------------------------

#if !defined	ATTRIB_HH_ENT_GEOMBUILD_COEDGE_CLASS
#define			ATTRIB_HH_ENT_GEOMBUILD_COEDGE_CLASS
//----------------------------------------------------------------------------
#include        "acis.hxx"
#include	"coedge.hxx"


#include	"dcl_heal.h"
#include	"entgmbld.hxx"
/**
* @file cegmbld.hxx
 * @CAA2Level L1
 * @CAA2Usage U1
 * \addtogroup HEALAPIDEP
 *
 * @{
 */

class ATTRIB_HH_AGGR_GEOMBUILD_BASE;
class COEDGE;
class PCURVE;
class CURVE;
/**
 * @nodoc
 */
ENTITY_IS_PROTOTYPE(ATTRIB_HH_ENT_GEOMBUILD_COEDGE, HEAL )
#if 0
; // semicolon needed for mkman (doc tool) parsing
#endif

// Enums and struct declarations (used for storing coedge details)

/**
 * @nodoc
 */
enum PAR_LINE
{
	NO_LINE,
	U_LINE,
	V_LINE
};

/**
 * @nodoc
 */
enum PAR_LOC
{
	MID,
	MAX,
	MIN
};

/**
 * @nodoc
 * <b>Obsolete:</b> This class has been deprecated.
 */
class DECL_HEAL hh_coedge_details
{
public:
	PAR_LINE partype;
	double parval;
	PAR_LOC parloc;
	logical end_to_end;
	logical rev;

	int face_type;		// identity of the face (SPLINE_TYPE, PLANE_TYPE, etc..)
	int n_sided_face;

	// Constructor
	hh_coedge_details()
	{
		init();
	}

	void init();
	void reset_param_details();

	logical is_uv();
	logical is_uv_boun();
	logical is_end_to_end();
	logical is_analytic();
};


//----------------------------------------------------------------------------
DECL_HEAL extern int ATTRIB_HH_ENT_GEOMBUILD_COEDGE_TYPE;
/**
 * @nodoc
 */
#define ATTRIB_HH_ENT_GEOMBUILD_COEDGE_LEVEL (ATTRIB_HH_ENT_GEOMBUILD_BASE_LEVEL + 1 )
//----------------------------------------------------------------------------

//----------------------------------------------------------------------------
/**
 * @nodoc
 * <b>Obsolete:</b> This class has been deprecated. For healing, use API interfaces
 * and not Direct Interface. For analysis of problems in models, use the Checker
 * API interfaces instead.
 * <br><br>
 * Individual entity-level healing attribute class attached to coedges in the geometry building phase.
 * <br>
 * <b>Role:</b> <tt>ATTRIB_HH_ENT_GEOMBUILD_COEDGE</tt> is the individual entity-level attribute class
 * attached to coedges during the geometry building phase. Individual entity-level attributes are
 * attached to the individual entities of body being healed to store entity-specific information
 * about each phase or subphase of the healing process. The individual entity-level attributes for
 * each phase or subphase are managed by the aggregate attribute for that phase/subphase.
 * @see CURVE, PCURVE
 */
class DECL_HEAL ATTRIB_HH_ENT_GEOMBUILD_COEDGE : public ATTRIB_HH_ENT_GEOMBUILD_BASE {
	protected:

/*		PCURVE * pPCU;			// new coedge geometry

		logical geom_fixed;		// denotes if the new geometry has been fixed
*/
		PCURVE* m_old_geom;		// stores the old coedge geometry
		REVBIT m_old_sense;
		CURVE* m_old_edge_curve; // stores the geometry of the old edge
								// (this is useful information in case
								// the old edge gets deleted during stitching

		REVBIT m_old_face_sense; // stores the sense of the face when old coedge
							// geometry was stored

		double m_deviation_from_face;	// Max. deviation from face
		int m_geom_is_bad;				// denotes if the pcurve is bad

		hh_coedge_details m_coedge_details;		// stores coedge details
		int m_coedge_details_updated;		// Whether coedge details have been updated

		//STI mka
		int on_face;		        // = 1 -> co-edge not on face
		double off_face;			// max distance off face
		int within_domain;
		int partner;				// = 1 -> no partner coedge
		int p_curve;				// = 0 -> no pcurve, = 1 -> pcurve present
	                            // = 2 -> no pcurve present but required (spline surface)
	                            // = 3 -> missing defining geometry
	                            // = 4 -> direction inconsistent with coedge
	                            // = 5 -> distance from edge > SPAresfit
		double p_curve_max_dist;	// max distance of pcurve from edge
// STI cpp begin
		logical save_sw;
// STI cpp end

	public:

		// Constructor
/**
 * C++ initialize constructor requests memory for this object and populates it with the data supplied as arguments.
 * <br><br>
 * <b>Role:</b> Applications should call this constructor only with the overloaded new operator
 * inherited from the <tt>ENTITY</tt> class (for example, <tt>x=new ATTRIB_HH_ENT_GEOMBUILD_COEDGE(...)</tt>), because
 * this reserves the memory on the heap, a requirement to support roll back and history management.
 * <br><br>
 * @param e
 * owning coedge.
 */
		ATTRIB_HH_ENT_GEOMBUILD_COEDGE( COEDGE *e=NULL);

		/**
		 * @nodoc
		 */
		ATTRIB_FUNCTIONS( ATTRIB_HH_ENT_GEOMBUILD_COEDGE, HEAL )
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
/**
 * Returns <tt>TRUE</tt> if this is pattern compatible.
 */
        virtual logical pattern_compatible() const;
                                    // returns TRUE
		/////////////////////
		// Inquiry functions
		/////////////////////

		// Get the new geometry of the coedge. This will return
		// the old geometry iff no new geomerty has been computed
/**
 * Returns the new geometry (pcurve) associated with the coedge, if the coedge has been fixed.
 */
		PCURVE* new_geometry() const;

		// Gets the original geometry of the coedge even after
		// the new geometry is set.
/**
 * Returns the old geometry (pcurve) associated with the coedge, if the coedge has not been fixed.
 */
		PCURVE* old_geometry();

		// Added similarly for sense
/**
 * Returns the old sense of the coedge.
 */
		REVBIT old_sense() const;
/**
 * Gets the new sense of the coedge and this returns the old sense if no new sense has been computed.
 */
 		REVBIT new_sense() const;

		// All the inquiry functions below operate on the new geometry

		// Gets the max. deviation of the pcurve from the underlying surface
		// (if maximum is passed as FALSE, then the function will return
		// immediately after the first instance of inaccurate coedge geometry
		// is encountered)
/**
 * Returns the maximum deviation of the pcurve from the underlying surface.
 * <br><br>
 * <b>Role:</b> If maximum is passed as <tt>FALSE</tt>, then the method returns immediately after the
 * first instance of inaccurate coedge geometry is encountered.
 * <br><br>
 * @param is_bad
 * flag indicating bad geometry.
 * @param maximum
 * flag to return after 1st problem.
 */
		double deviation(logical& is_bad = *(logical*)NULL_REF,
							logical maximum = TRUE);

		// Returns TRUE if the pcurve deviation is more than SPAresabs
/**
 * Returns <tt>TRUE</tt> if the pcurve deviation is more than <tt>SPAresabs</tt>.
 */
		logical does_not_deviate();

		// Returns TRUE if either the pcurve deviation is more than SPAresabs
		// or if the parent edge geometry is bad.
/**
 * Returns <tt>TRUE</tt> if either the pcurve deviation is more than <tt>SPAresabs</tt> or if the parent edge geometry is bad.
 * <br><br>
 * <b>Role:</b> In both cases, the coedge geometry may be required to be computed and healed.
 */
		virtual logical is_bad();

		// Project: HEAL_TO_TOLERANCE
		// For internal use only
/**
 * @nodoc
 */
		virtual logical is_bad_with_tol(double tol);

		// Gets the parametric details and other misc. info.
/**
 * Returns a structure containing the parametric details and other information about the coedge.
 */
		hh_coedge_details get_coedge_details();

		///////////////////////////
		// Modification Functions
		///////////////////////////

		// Set a new geometry for the coedge. This function
		// will also reset all the geometry related info.
		// stored in the attributes.
/**
 * Sets the pcurve, <tt>pPCU</tt>, associated with the owning coedge to the given pcurve.
 * <br><br>
 * @param p
 * new pcurve.
 */
		void set_geometry( PCURVE * p );

		// Set the old geometry for the coedge.
/*
// tbrv
*/
/**
 * @nodoc
 */
		void set_old_geometry( PCURVE * p );

		// Set a new sense for the coedge. This function
		// will also reset all the geometry related info.
		// stored in the attributes.
/**
 * Sets a new sense for the coedge.
 * <br><br>
 * <b>Role:</b> This function will also reset all the geometry related info stored in the attributes.
 * <br><br>
 * @param pS
 * new sense.
 */
		void set_sense (REVBIT pS);

		/////////////////////////////////////
		// Functions for internal usage only
		/////////////////////////////////////

		// Update the coedge_detailts struct in the attribute
/**
 * Updates the coedge details structure (<tt>m_coedge_details</tt>) with new information and sets the <tt>m_coedge_details_updated</tt> flag.
 */
		void update_coedge_details ();

		// Set the coedge_detailts struct in the attribute
/**
 * Stores the input coedge details structure in the <tt>m_coedge_details</tt> data member.
 * <br><br>
 * @param in_str
 * input structure.
 */
		void set_coedge_details (hh_coedge_details* in_str);

		// Reset coedge details so that they get freshly calculated
		// when asked for.
/**
 * Resets the coedge details so that they are recalculated the next time they are requested.
 */
		void reset_coedge_details();

		// RS (Sep 29,2001):

/**
 * @nodoc
 */
		virtual void notify_geometry_change(void);

		// RS (Sep 29,2001):

/**
 * @nodoc
 */
		virtual void replace_owner_geometry(ENTITY *  // "new_geometry"
								 );

		// Gets the original geometry of the old edge of the owner coedge.
		// This geometry can be used even after the edge gets deleted
		// during stitching
/**
 * Returns the old edge curve.
 */
		CURVE* old_edge_geometry() const;
/**
 * Notifies the <tt>ATTRIB_HH_ENT_GEOMBUILD_COEDGE</tt> that its owner is about to be split into two parts.
 * <br><br>
 * <b>Role:</b> The application has the chance to duplicate or otherwise modify the attribute. The
 * default action is to do nothing. This function is supplied by the application whenever it defines
 * a new attribute, and is called when a split occurs.
 * <br><br>
 * @param ent
 * new entity.
 */
		virtual void split_owner(ENTITY* ent);
/**
 * Notifies the <tt>ATTRIB_HH_ENT_GEOMBUILD_COEDGE</tt> class that its owning <tt>ENTITY</tt> is about to be merged with the given entity.
 * <br><br>
 * <b>Role:</b> The application has the chance to delete or otherwise modify the attribute. After
 * the merge, this owner will be deleted if the deleting owner logical is <tt>TRUE</tt>, otherwise it will
 * be retained and the other entity will be deleted. The default action is to do nothing. This
 * function is supplied by the application whenever it defines a new attribute, and is called when a
 * merge occurs.
 * <br><br>
 * @param ent
 * given entity.
 * @param del_own
 * deleting owner.
 */
		virtual void merge_owner(ENTITY* ent, logical del_own);

		// Reset the attribute. This function will reset all the
		// info. of the coedge stored in the attrib. Hence any
		// call to an inquiry function will result in a recalculation
		// all details w.r.t. the latest geometry. This function is
		// typically called whenever a new geomtry is set.
/**
 * Resets the attribute.
 * <br><br>
 * <dl>
 * <b>Role:</b> It resets the attribute in following way
 * <dd>- Sets the maximum distance from the coedge to the edge to 0.0.</dd>
 * <dd>- Sets the maximum distance from the face surface to 0.0.</dd>
 * <dd>- Sets the on face, within domain, partner, and pcurve status indicators to <tt>UNSET</tt>.</dd>
 * </dl>
 */
		void reset();

		// STI mka
		//Begin
		// Set functions
/**
 * Sets the value of "on the face" indicator, <tt>on_face</tt>.
 * <br><br>
 * <b>Role:</b> A value of 1 means the coedge is not on the face; 0 means the coedge is on the face.
 * <br><br>
 * @param q
 * new value.
 */
		void set_on_face (int q) {on_face = q;}
/**
 * Sets the value of the maximum distance off the face.
 * <br><br>
 * @param q
 * new value.
 */
		void set_off_face (double q) {off_face = q;}
/**
 * Sets the value of partner indicator, <tt>partner</tt>.
 * <br><br>
 * <b>Role:</b> A value of 1 means there is no partner coedge; 0 means there is a partner.
 * @param q
 * new value.
 */
		void set_partner (int q) {partner = q;}
/**
 * Sets the value of the within domain indicator (<tt>within_domain</tt>).
 * <br><br>
 * <b>Role:</b> A value of 0 means the coedge parameters are within the edge range.
 * <br><br>
 * @param q
 * new value
 */
		void set_within_domain (int q) {within_domain = q;}
/**
 * Sets the value of the maximum distance off the face.
 * <br><br>
 * @param q
 * new distance.
 */
		void set_p_curve_max_dist (double q) {p_curve_max_dist = q;}
/**
 * Sets the value of pcurve status indicator, <tt>p_curve</tt>.
 * <br><br>
 * @param q
 * new value.
 */
		void set_p_curve (int q) {p_curve = q;}

		// Get functions
/**
 * Gets the value of "on the face" indicator, <tt>on_face</tt>.
 * <br><br>
 * <b>Role:</b> A value of 1 means the coedge is not on the face; 0 means the coedge is on the face.
 */
		int get_on_face () const {return on_face;}
/**
 * Gets the value of the maximum distance off the face.
 */
		double get_off_face () const {return off_face;}
/**
 * Gets the value of partner indicator, <tt>partner</tt>.
 * <br><br>
 * <b>Role:</b> A value of 1 means there is no partner coedge; 0 means there is a partner.
 */
		int get_partner () const {return partner;}
/**
 * Returns the value of the within domain indicator (<tt>within_domain</tt>).
 * <br><br>
 * <b>Role:</b> A value of 0 means the coedge parameters are within the edge range.
 */
		int get_within_domain () const {return within_domain;}
/**
 * Gets the value of pcurve status indicator, <tt>p_curve</tt>.
 * <br><br>
 * <b>Role:</b> The pcurve status indicator can take following values:
 * <br><br>
 * <table width = 100%>
 * <tr width = 100%>
 * <td valign = "top" width = 10%>0</td>
 * <td valign = "top">No pcurve</td>
 * </tr>
 * <tr>
 * <td valign = "top">1</td>
 * <td valign = "top">Pcurve present</td>
 * </tr>
 * <tr>
 * <td valign = "top">2</td>
 * <td valign = "top">No pcurve present but required (spline surface)</td>
 * </tr>
 * <tr>
 * <td valign = "top">3</td>
 * <td valign = "top">Missing defining geometry</td>
 * </tr>
 * <tr>
 * <td valign = "top">4</td>
 * <td valign = "top">Direction is inconsistent with coedge</td>
 * </tr>
 * <tr>
 * <td valign = "top">5</td>
 * <td valign = "top">Distance from the edge is greater than <tt>SPAresfit</tt></td>
 * </tr>
 * </table>
 */
		int get_p_curve () const {return p_curve;}
/**
 * Gets the value of maximum distance of the pcurve from the edge.
 */
		double get_p_curve_max_dist () const {return p_curve_max_dist;}
/**
 * Returns the coedge quality based on values set in the attribute.
 * <br><br>
 * <b>Role:</b> Returns the coedge quality based on values set in the attribute.
 * Possible values are
 * <br><br>
 * <table width = 100%>
 * <tr width = 100%>
 * <td valign = "top" width = 10%>0</td>
 * <td valign = "top">Good quality. The coedge is on the face, <i>and</i> the coedge has a partner, <i>and</i> the pcurve
 * status (<tt>p_curve</tt>) is less than 2.</td>
 * </tr>
 * <tr>
 * <td valign = "top">1</td>
 * <td valign = "top">Poor quality. The coedge is not on the face, <i>or</i> the coedge has no partner, <i>or</i> the pcurve
 * status (<tt>p_curve</tt>) is greater than or equal to 2.</td>
 * </tr>
 * </table>
 * <br>
 * Returns <tt>UNSET</tt> if neither of these conditions is met.
 */
		int get_quality () const;
/**
 * Performs advanced check of coedge.
 * <br><br>
 * <b>Role:</b> Checks for pcurve status, partner coedge, whether the coedge is within the range
 * of the edge, and to see if the coedge is on the face.
 */
		virtual int adv_check() ;
		//End
};

//----------------------------------------------------------------------------

/**
 * @nodoc
 */
ATTRIB_HH_ENT_GEOMBUILD_COEDGE * find_att_coedge_geombuild( COEDGE * );

/** @} */
#endif
