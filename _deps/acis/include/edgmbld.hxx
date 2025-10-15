/*******************************************************************/
/*    Copyright (c) 1989-2020 by Spatial Corp.                     */
/*    All rights reserved.                                         */
/*    Protected by U.S. Patents 5,257,205; 5,351,196; 6,369,815;   */
/*                              5,982,378; 6,462,738; 6,941,251    */
/*    Protected by European Patents 0503642; 69220263.3            */
/*    Protected by Hong Kong Patent 1008101A                       */
/*******************************************************************/

//----------------------------------------------------------------------------

#if !defined	ATTRIB_HH_ENT_GEOMBUILD_EDGE_CLASS
#define			ATTRIB_HH_ENT_GEOMBUILD_EDGE_CLASS
//----------------------------------------------------------------------------
/**
* @file edgmbld.hxx
 * @CAA2Level L1
 * @CAA2Usage U1
 * \addtogroup HEALAPIDEP
 *
 * @{
 */
#include	"edge.hxx"


#include	"dcl_heal.h"
#include	"entgmbld.hxx"
#include	"cegmbld.hxx"

class ATTRIB_HH_AGGR_GEOMBUILD_BASE;
class EDGE;
class CURVE;
//----------------------------------------------------------------------------
/**
 * @nodoc
 */
ENTITY_IS_PROTOTYPE(ATTRIB_HH_ENT_GEOMBUILD_EDGE, HEAL )
#if 0
; // semicolon needed for mkman (doc tool) parsing
#endif

DECL_HEAL extern int ATTRIB_HH_ENT_GEOMBUILD_EDGE_TYPE;
/**
 * @nodoc
 */
#define ATTRIB_HH_ENT_GEOMBUILD_EDGE_LEVEL (ATTRIB_HH_ENT_GEOMBUILD_BASE_LEVEL + 1 )

//----------------------------------------------------------------------------

/**
 * @nodoc
 * <b>Obsolete:</b> This class has been deprecated.
 */
class DECL_HEAL edge_data_struct
{
public:
	double max_angle;
	double min_angle;

	// Constructor
	edge_data_struct()
	{
		max_angle = -1;
		min_angle = -1;
	}
};

//----------------------------------------------------------------------------
/**
 * @nodoc
 * <b>Obsolete:</b> This class has been deprecated. For healing, use API interfaces
 * and not Direct Interface. For analysis of problems in models, use the Checker
 * API interfaces instead.
 * <br><br>
 * Individual entity-level healing attribute class attached to edges in the geometry building phase.
 * <br>
 * <b>Role:</b> <tt>ATTRIB_HH_ENT_GEOMBUILD_EDGE</tt> is the individual entity-level attribute class attached
 * to edges during the geometry building phase. Individual entity-level attributes are attached to
 * the individual entities of body being healed to store entity-specific information about each phase
 * or subphase of the healing process. The individual entity-level attributes for each phase or
 * subphase are managed by the aggregate attribute for that phase/subphase.
 * @see CURVE
 */
class DECL_HEAL ATTRIB_HH_ENT_GEOMBUILD_EDGE : public ATTRIB_HH_ENT_GEOMBUILD_BASE {
	protected:


		CURVE* m_old_geom;	// stores the old edge geometry
		REVBIT m_old_sense;	// stores the old edge sense

		// Max. deviation from faces
		double m_deviation_from_faces;

		// Max. deviation from vertices
		double m_deviation_from_vertices;

		//Data of faces linked with the edge
		edge_data_struct edge_data;

		// Whether edge deviates from face
		int m_does_not_deviate;

		// Whether the end vertices deviate from the edge curve
		int m_vertices_do_not_deviate;

		// Whether it is a tangential junction
		logical m_tangential;
		logical m_tangential_stringent;

		// Tolerance from computing tangency
		//double m_tang_tol;
		//double m_max_spline_tang_tol;	// max allowed angle for stringent checks

		// Local edge tolerance for geombuild
		double m_geombuild_tol;

		//Local edge movement tol
		double edge_mov_tol;

		//STI mka
		int curve_quality; // quality of underlying curve geometry
		int vexity;        // convectity
		double length;     // length
// STI cpp begin
		logical save_sw;
// STI cpp end

		// HEAL_TO_TOLERANCE
		HH_ENTITY_GAP_CLASSIFICATION_INFO m_entity_gap_classification;

	public:

		// Constructor
/**
 * C++ initialize constructor requests memory for this object and populates it with the data supplied as arguments.
 * <br><br>
 * <b>Role:</b>  Applications should call this constructor only with the overloaded new operator
 * inherited from the <tt>ENTITY</tt> class (for example, <tt>x=new ATTRIB_HH_ENT_GEOMBUILD_EDGE(...)</tt>), because
 * this reserves the memory on the heap, a requirement to support roll back and history management.
 * <br><br>
 * @param e
 * owning coedge.
 */
		ATTRIB_HH_ENT_GEOMBUILD_EDGE( EDGE *e=NULL);

		/**
		 * @nodoc
		 */
		ATTRIB_FUNCTIONS( ATTRIB_HH_ENT_GEOMBUILD_EDGE, HEAL )
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

		// Get the new geometry of the edge. This will return
		// the old geometry iff no new geomerty has been computed
/**
 * Returns the latest geometry calculated for the edge.
 * <br><br>
 * <b>Role:</b> Returns the original geometry if no new geometry has been calculated.
 */
		CURVE* new_geometry() const;

		// Gets the original geometry of the edge even after
		// the new geometry is set.
/**
 * Returns the original geometry associated with the edge.
 * <br><br>
 * <b>Role:</b> Even after fixing the new geometry, this method continues to return the old geometry.
 */
		CURVE*	old_geometry() const;

		// Get the new sense of the edge. This will return
		// the old geometry iff no new geomerty has been computed
/**
 * Returns the latest edge sense calculated.
 * <br><br>
 * <b>Role:</b> Returns the original sense if no new sense has been calculated.
 */
		REVBIT new_sense() const;

		// Gets the original sense of the edge even after
		// the new geometry is set.
/**
 * Returns the original edge sense.
 * <br><br>
 * <b>Role:</b> Even after fixing the new geometry, this method continues to return the old sense.
 */
		REVBIT old_sense() const;

		// All the inquiry functions below operate on the new geometry

		// Gets the max. deviation of the curve from the underlying surface
		// (if maximum is passed as FALSE, then the first deviation greater
		// than SPAresabs encountered will be returned)
/**
 * Gets the deviation of the curve from the underlying surface.
 * <br><br>
 * <b>Role:</b> Gets the deviation of the curve from the underlying surface. If the flag maximum
 * is <tt>TRUE</tt>, then the maximum deviation is returned; if this flag is <tt>FALSE</tt>, then the first deviation
 * encountered that is greater than <tt>SPAresabs</tt> is returned.
 * <br><br>
 * @param maximum
 * flag whether to return max or 1st deviation.
 */
		double deviation(logical maximum = TRUE);

		// Project: HEAL_TO_TOLERANCE
		// For internal use only
/**
 * @nodoc
 */
		double deviation_with_tol(logical maximum, double dev_tol);


		// Gets the max. of deviations of the two end vertices from the edge
		// (if maximum is passed as FALSE, then the first deviation greater
		// than SPAresabs encountered will be returned)
/**
 * Gets the deviations of the two end vertices from the edge.
 * <br><br>
 * <b>Role:</b> Gets the deviations of the two end vertices from the edge. If the flag maximum is
 * <tt>TRUE</tt>, then the maximum deviation is returned; if this flag is <tt>FALSE</tt>, then the first deviation
 * encountered that is greater than <tt>SPAresabs</tt>.
 * <br><br>
 * @param maximum
 * flag whether to return max or 1st deviation.
 */
		double deviation_from_vertices(logical maximum = TRUE);

		// Project: HEAL_TO_TOLERANCE
		// For internal use only
/**
 * @nodoc
 */
		double deviation_from_vertices_with_tol(logical maximum, double dev_tol);

		// Returns TRUE if the curve deviation from
		// the faces is less than SPAresabs
/**
 * Returns <tt>TRUE</tt> if the curve deviation from the faces is less than <tt>SPAresabs</tt>.
 */
		logical does_not_deviate();

		// Project: HEAL_TO_TOLERANCE
		// For internal use only
/**
 * @nodoc
 */
		logical does_not_deviate_with_tol(double tol);


		// Returns TRUE iff none of the end vertices deviate from the curve.
/**
 * Returns <tt>TRUE</tt> if none of the end vertices deviate from the curve.
 */
		logical vertices_do_not_deviate();

		// Project: HEAL_TO_TOLERANCE
		// For internal use only
/**
 * @nodoc
 */
		logical vertices_do_not_deviate_with_tol(double dev_tol);

		// Returns TRUE if either the curve deviation from the face
		// is more than SPAresabs or if the end vertices dont lie on the
		// all surrounding faces or if the any of the end vertices
		// doesn't lie on the edge.
/**
 * Returns <tt>TRUE</tt> if either the curve deviation from the face is more than <tt>SPAresabs</tt>, or if the end vertices do not lie on the surrounding faces, or if any of the end vertices do not lie on the edge.
 */
		 logical is_bad();

		 // Project: HEAL_TO_TOLERANCE
		 // For internal use only
/**
 * @nodoc
 */
		 logical is_bad_with_tol(double tol);

		// Returns TRUE if the edge forms a tangential junction
		// (within a specific tolerance)
/**
 * Returns <tt>TRUE</tt> if the edge forms a tangential junction (within a specific tolerance).
 * <br><br>
 * @param stringent
 * whether it is a tangential junction.
 */
		void get_tangency_details(logical& tangential, logical& stringent_tangential);

		logical is_tangential( logical stringent = FALSE );

		// Gets the tolerance at which the tangency is calculated
/**
 * Returns the tolerance at which the tangency is calculated (<tt>m_tang_tol</tt>).
 */
		double tang_tol() const;
/**
 * Gets the smallest allowed difference for the tangent angles between two spline faces which will still be considered tangent.
 */
		double min_spline_tang_tol() const;
/**
 * Gets the largest allowed difference (tolerance) for the tangent angles between two spline faces which will still be considered tangent.
 */
		double max_spline_tang_tol() const;

		// Returns TRUE if both the adjacent faces are analytic
/**
 * Determines if the edge forms a junction of two analytic surfaces.
 */
		logical is_analytic_junction();

		// Returns TRUE if the adjacent faces form an isospline
		// junction. (i.e. uv-uv boun-boun complete range)
/**
 * Returns a flag indicating whether the isospline edge is within the domain of the isospline solver.
 * <br><br>
 * <b>Role:</b> Returns <tt>TRUE</tt> if the adjacent faces form an isospline junction (i.e., <i>uv</i> boundary/<i>uv</i>
 * boundary complete range).
 */
		logical is_isospline();

		// Return TRUE if its a uv-uv junction
/**
 * Returns <tt>TRUE</tt> if it is a <i>uv/uv</i> junction.
 */
		logical is_uv_uv();

		// Returns TRUE if its a uv-nonuv junction
/**
 * Returns <tt>TRUE</tt> if it is a <i>uv</i>/non-<i>uv</i> junction.
 */
		logical is_uv_nonuv();

		// Returns TRUE if its a uv boundary-uv boundary junction
/**
 * Returns TRUE if it is a <i>uv</i> boundary/<i>uv</i> boundary junction.
 */
		logical is_uv_uv_boun_boun();

		// Gets the parametric details and other misc. info. related to the
		// two underlying coedges. Note that this function is invalid
		// for open edges.
/**
 * Gets the parametric details and other miscellaneous information related to the two underlying coedges.
 * <br><br>
 * <b>Role:</b> This method is invalid for open edges.
 * <br><br>
 * @param coedge1
 * details for 1st coedge.
 * @param coedge2
 * details for 2nd coedge.
 */
		void get_coedge_details(hh_coedge_details& coedge1, hh_coedge_details& coedge2);

/**
 * Gets the edge_data_struct which contains two angle values:
 * <br>
 * min_angle - minimum tangent angle (based on local edge size)
 * <br>
 * max_angle - maximum tangent angle (based on local edge size)
 */
		const edge_data_struct get_edge_data();


		// For internal use only
/**
 * @nodoc
 */
		// Method to get entity_gap_classification info. HEAL_TO_TOLERANCE
		HH_ENTITY_GAP_CLASSIFICATION_INFO get_entity_gap_classification_info() const;

/**
 * Sets the <tt>edge_datamax_angle</tt> to the input value.
 * <br><br>
 * @param max_angle
 * new max_angle to set (radians).
 */
		void set_max_angle(double max_angle);
/**
 * Sets the <tt>edge_datamin_angle</tt> to the input value.
 * <br><br>
 * @param min_angle
 * new min_angle to set (radians).
 */
		void set_min_angle(double min_angle);
/**
 * Initializes the tangency information for this edge, specifying whether the edge is a tangent edge or not.
 */
		void reset_tangency_details();
		//void update_tang_tol();
/**
 * Updates the <tt>min_angle</tt> and <tt>max_angle</tt>.
 */
		logical update_edge_data();


		// For internal use only
/**
 * @nodoc
 */
		// Method to set entity gap classification info. HEAL_TO_TOLERANCE
		void set_entity_gap_classification_info(HH_ENTITY_GAP_CLASSIFICATION_INFO entity_gap_info);



		// Returns TRUE if the two surfaces are G1 continuous at the edge
/**
 * Returns <tt>TRUE</tt> if the two surfaces are <tt>G1</tt> continuous at the edge.
 */
		logical is_edge_G1_healed();

		// Returns TRUE if the two analytic tangential surfaces are
		//G1 continuous at the edge
/**
 * Returns <tt>TRUE</tt> if the two surfaces are <tt>G1</tt> continuous at the edge.
 */
		logical is_analytic_edge_G1_healed();

		///////////////////////////
		// Modification Functions
		///////////////////////////

		// Set a new geometry for the edge. This function
		// will also reset all the geometry related info.
		// stored in the attributes.
/**
 * Sets the geometry in the attribute to the given curve.
 * <br><br>
 * @param pC
 * new curve.
 */
		void set_geometry(CURVE* pC);

		// Set a new sense for the edge. This function
		// will also reset all the geometry related info.
		// stored in the attributes.
/**
 * Sets the edge sense.
 * <br><br>
 * @param pS
 * new sense.
 */
		void set_sense (REVBIT pS);

/*		// Fix the geometry and sense of the owner edge to whatever
		// is stored in the attribute as the new geometry and new sense
		// by the member functions set_geometry() and set_sense().
		// In case nothing is set, the old geometry and sense are restored.
		virtual void fix();
*/
		// Set the tolerance for calculating tangency
		//void set_tang_tol(double);

		// Mark the edge as bad
/**
 * Mark the edge as bad.
 * <br><br>
 * @param val
 * new value.
 */
		virtual void set_bad(logical val);

		/////////////////////////////////////
		// Utility functions
		/////////////////////////////////////

		// Intersect surfaces and compute the intersection curve
/**
 * Intersects surfaces and computes the intersection curve.
 */
		logical do_intersect();

		// Compute sharp intersections and vertices around.
/*
// tbrv
*/
/**
 * @nodoc
 */
		void do_sharp();

		/////////////////////////////////////
		// Functions for internal usage only
		/////////////////////////////////////

		// Draw the edge choosing a proper color according to the
		// type of the edge. (only for GSSL Eval UIF)
/*
// tbrv
*/
/**
 * @nodoc
 */
		void draw();

		// MA (17 Feb 2000) Commented since it is no longer being used anywhere
		// and we don't want to expose it to the user

		// Mark forcefully as tangential
		//void set_tangential(logical);

		// Get edge tol for geombuild (based on stitch gap)
/**
 * Gets the edge tolerance for geometry building (based on the stitch gap).
 */
		double geombuild_tol();

		//Get edge tol for its movement
/**
 * Gets edge tolerance for its movement.
 */
		double edge_movemnt_tol();

		//Edge tolerance if the input edges was excluded from its neighbours
/**
 * Returns the edge tolerance if the input edge was excluded from its neighbors.
 * <br><br>
 * @param ent_list
 * list.
 */
		double get_specific_mov_tol(ENTITY_LIST ent_list);


		// Reset the geombuild tol so that it gets recalculated
		// the next time it is asked for.
/**
 * Resets the geometry building tolerance so that it is recalculated the next time it is requested.
 */
		void reset_geombuild_tol();

		// Reset the attribute. This function will reset all the
		// deviation related info. of the edge stored in the attrib. Hence any
		// call to an inquiry function for edge goodness will result
		// in a recalculation all details w.r.t. the latest geometry.
		// This function is typically called whenever a new
		// geometry/sense is set.
/**
 * Resets the attribute.
 * <br><br>
 * <dl>
 * <b>Role:</b> It resets the attribute in following way:
 * <dd>- Sets the edge length to -100.0.</dd>
 * <dd>- Sets the curve quality and convexity indicators to <tt>UNSET</tt>.</dd>
 * </dl>
 */
		void reset();

		// Calls the reset member function of all those
		// attributes whose goodness depends on the geometry of
		// this edge (i.e. associated vertices and coedges)
/**
 * Resets the attribute on the owning edge's dependent entities (such as its coedges, vertices, etc).
 */
		void reset_dependents();

		// Resets the param_range of the owner edge and
		// also its bounding SPAbox
/**
 * Resets the parameter range of the owner edge and also its bounding box.
 */
		void reset_box_and_param_range();

		// RS (Sep 29,2001):
/*
// tbrv
*/
/**
 * @nodoc
 */
		virtual void notify_geometry_change(void);

		// RS (Sep 29,2001):
/*
// tbrv
*/
/**
 * @nodoc
 */
		virtual void replace_owner_geometry(ENTITY *  // "new_geometry"
								 );
/**
 * Notifies the <tt>ATTRIB_HH_ENT_GEOMBUILD_EDGE</tt> that its owner is about to be split into two parts.
 * <br><br>
 * <b>Role:</b> The application has the chance to duplicate or otherwise modify the attribute.
 * The default action is to do nothing. This function is supplied by the application whenever it
 * defines a new attribute, and is called when a split occurs.
 * <br><br>
 * @param ent
 * given entity.
 */
		virtual void split_owner(ENTITY* ent);
/**
 * Notifies the <tt>ATTRIB_HH_ENT_GEOMBUILD_EDGE</tt> that its owning <tt>ENTITY</tt> is about to be merged with given entity.
 * <br><br>
 * <b>Role:</b> The application has the chance to delete or otherwise modify the attribute. After
 * the merge, this owner will be deleted if the logical deleting owner is <tt>TRUE</tt>, otherwise it will be
 * retained and other entity will be deleted. The default action is to do nothing. This function is
 * supplied by the application whenever it defines a new attribute, and is called when a merge occurs.
 * <br><br>
 * @param ent
 * given entity.
 * @param del_owner
 * deleting owner.
 */
		virtual void merge_owner(ENTITY* ent, logical del_owner);

		// STI mka
		//Begin
	// Set functions
/**
 * Sets the indicator of underlying curve quality.
 * <br><br>
 * @param q
 * new value.
 */
		void set_curve_quality (int q) {curve_quality = q;}
/**
 * Sets the value of the convexity indicator.
 * <br><br>
 * @param v
 * new value.
 */
		void set_vexity (int v) {vexity = v;}
/**
 * Sets the length of the edge.
 * <br><br>
 * @param l
 * new length.
 */
		void set_length (double l) {length = l;}

		// Get functions
/**
 * Gets the indicator of underlying curve quality.
 */
		int get_curve_quality () const {return curve_quality;}
/**
 * Returns the value of the convexity indicator.
 */
		int get_vexity () const {return vexity;}
/**
 * Gets the length of the edge.
 */
		double get_length () const {return length;}
/**
 * Returns the edge quality based on values set in the attribute.
 * <br><br>
 * <b>Role:</b> Possible values are
 * <br><br>
 * <table width = 100%>
 * <tr width = 100%>
 * <td valign = "top" width = 10%>0</td>
 * <td valign = "top">Good quality. The underlying curve quality is good (0), and the edge length is greater than
 * the minimum edge length.</td>
 * </tr>
 * <tr>
 * <td valign = "top">1</td>
 * <td valign = "top">Poor quality. The underlying curve quality is poor (1), or the edge length is less than the
 * minimum edge length (but greater than zero).</td>
 * </tr>
 * </table>
 * <br>
 * Returns <tt>UNSET</tt> if neither of these conditions is met.
 */
		int get_quality () const;
/**
 * Performs advanced checks on the edge and checks the curve, the edge length, and the convexity.
 */
		int adv_check() ;


		// RNC: 3 Aug 99 Function for log list addition for iso_spline junction
/**
 * Function for log list addition for <tt>iso_spline</tt> junction.
 * <br><br>
 * @param data
 * logged data.
 */
		void set_iso_spline_edge_log_details(char *data = NULL);
/**
 * Returns the log list addition for <tt>iso_spline</tt> junction.
 * <br><br>
 * @param str
 * temp string.
 * @param det
 * details.
 */
		void sprint_iso_spline_edge_log_details(char *str = NULL, char *det = NULL);

		// RNC: 3 Aug 99 Function for log list addition for stitch_split
/**
 * Function for log list addition for <tt>stitch_split</tt>.
 * <br><br>
 * @param data
 * logged data.
 */
		void set_stitch_split_edge_log_details(char *data = NULL);
/**
 * Returns the log list addition for <tt>stitch_split</tt>.
 * <br><br>
 * @param str
 * temp string.
 * @param det
 * details.
 */
		void sprint_stitch_split_edge_log_details(char *str = NULL, char *det = NULL);

		// RNC: 3 Aug 99 Function for log list addition for edge geometry computation using intersection.
/**
 * Function for log list addition for edge geometry computation using intersection.
 * <br><br>
 * @param data
 * logged data.
 */
		void set_analytic_edge_intersect_log_details(char *data = NULL);
/**
 * Returns the log list addition for edge geometry computation using intersection.
 * <br><br>
 * @param data1
 * log data.
 * @param data2
 * log data.
 */
		void sprint_analytic_edge_intersect_log_details(char *data1 = NULL, char *data2 = NULL);

		// RNC: 3 Aug 99 Function for log list addition for edge geometry computation using projection.
/**
 * Function for log list addition for edge geometry computation using projection.
 * <br><br>
 * @param data
 * logged data.
 */
		void set_analytic_edge_project_log_details(char *data = NULL);
/**
 * Returns the log list addition for edge geometry computation using projection.
 * <br><br>
 * @param data1
 * log data.
 * @param data2
 * log data.
 */
		void sprint_analytic_edge_project_log_details(char *data1 = NULL, char *data2 = NULL);

		//End
};

//----------------------------------------------------------------------------

/**
 * @nodoc
 */
DECL_HEAL ATTRIB_HH_ENT_GEOMBUILD_EDGE * find_att_edge_geombuild( EDGE * );
//----------------------------------------------------------------------------

/**
 * @nodoc
 */
logical hh_edge_does_not_deviate(EDGE*);
//----------------------------------------------------------------------------

/** @} */

#endif
