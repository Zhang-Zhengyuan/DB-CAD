/*******************************************************************/
/*    Copyright (c) 1989-2020 by Spatial Corp.                     */
/*    All rights reserved.                                         */
/*    Protected by U.S. Patents 5,257,205; 5,351,196; 6,369,815;   */
/*                              5,982,378; 6,462,738; 6,941,251    */
/*    Protected by European Patents 0503642; 69220263.3            */
/*    Protected by Hong Kong Patent 1008101A                       */
/*******************************************************************/

//----------------------------------------------------------------------------

#if !defined	ATTRIB_HH_ENT_GEOMBUILD_VERTEX_CLASS
#define			ATTRIB_HH_ENT_GEOMBUILD_VERTEX_CLASS
//----------------------------------------------------------------------------
/**
* @file vegmbld.hxx
 * @CAA2Level L1
 * @CAA2Usage U1
 * \addtogroup HEALAPIDEP
 *
 * @{
 */
#include "dcl_heal.h"
#include "entgmbld.hxx"

class ATTRIB_HH_AGGR_GEOMBUILD_BASE;
class VERTEX;
class APOINT;
class HH_GlobalNode;
/**
 * @nodoc
 */
ENTITY_IS_PROTOTYPE(ATTRIB_HH_ENT_GEOMBUILD_VERTEX, HEAL )
#if 0
; // semicolon needed for mkman (doc tool) parsing
#endif
//----------------------------------------------------------------------------

DECL_HEAL extern int ATTRIB_HH_ENT_GEOMBUILD_VERTEX_TYPE;
/**
 * @nodoc
 */
#define ATTRIB_HH_ENT_GEOMBUILD_VERTEX_LEVEL (ATTRIB_HH_ENT_GEOMBUILD_BASE_LEVEL + 1 )

//----------------------------------------------------------------------------
/**
 * @nodoc
 * <b>Obsolete:</b> This class has been deprecated. For healing, use API interfaces
 * and not Direct Interface. For analysis of problems in models, use the Checker 
 * API interfaces instead.
 * <br><br>
 * Individual entity-level healing attribute class attached to vertices in the geometry building phase.
 * <br>
 * <b>Role:</b> <tt>ATTRIB_HH_ENT_GEOMBUILD_VERTEX</tt> is the individual entity-level attribute class attached
 * to vertices during the geometry building phase. Individual entity-level attributes are attached to
 * the individual entities of body being healed to store entity-specific information about each phase
 * or subphase of the healing process. The individual entity-level attributes for each phase or
 * subphase are managed by the aggregate attribute for that phase/subphase.
 * @see APOINT
 */
class DECL_HEAL ATTRIB_HH_ENT_GEOMBUILD_VERTEX : public ATTRIB_HH_ENT_GEOMBUILD_BASE {
	protected:
/*		APOINT* pAPOINT;	// new vertex geometry

		logical geom_fixed;	// denotes if the new geometry has been fixed
*/
		APOINT* m_old_geom;	// stores the old vertex geometry
//		logical old_geom_stored;	// denotes if the old edge geometry has
//									// been stored

		double m_deviation_from_faces;	// Max. deviation from faces
		double m_deviation_from_edges;  // Max. deviation from edges

		int m_does_not_deviate_from_faces; // Whether the vertex deviates from
											// underlying the faces
		int m_does_not_deviate_from_edges; // Whether the vertex deviates from
											// underlying the edges
		// Local vertex tolerance for geombuild
		double m_geombuild_tol;

		int on_edges;		// is vertex within SPAresabs of edges?
		int edges_meet;		// are edge ends within SPAresabs of each other?
		int on_faces;		// is vertex within SPAresabs of faces?
							// 0 yes; 1 no
		double edge_dist;   // Max distance from edge
		double face_dist;   // Max distance from adjacent faces
		double meet_dist;   // Max distance between edges at vertex
		HH_GlobalNode * node; //Pointer to node on vertex
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
 * <b>Role:</b> Applications should call this constructor only with the overloaded new operator
 * inherited from the <tt>ENTITY</tt> class (for example, <tt>x=new ATTRIB_HH_ENT_GEOMBUILD_VERTEX(...)</tt>), because
 * this reserves the memory on the heap, a requirement to support roll back and history management.
 * <br><br>
 * @param e
 * owning vertex.
 */
		ATTRIB_HH_ENT_GEOMBUILD_VERTEX( VERTEX *e=NULL);

		/**
		 * @nodoc
		 */
		ATTRIB_FUNCTIONS( ATTRIB_HH_ENT_GEOMBUILD_VERTEX, HEAL )
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

		// Get the new geometry of the vertex. This will return
		// the old geometry iff no new geomerty has been computed
/**
 * Returns the latest geometry calculated for the vertex.
 * <br><br>
 * <b>Role:</b> Returns the original geometry if no new geometry has been calculated.
 */
		APOINT*	new_geometry() const;

		// Gets the original geometry of the face even after
		// the new geometry is set.
/**
 * Returns the original geometry associated with the vertex.
 * <br><br>
 * <b>Role:</b> Even after fixing the new geometry, this method continues to return the old geometry.
 */
		APOINT*	old_geometry() const;

		// Gets the max. of the deviations of the vertex from the
		// underlying surfaces and edges
		// (if maximum is passed as FALSE, then the first deviation greater
		// than SPAresabs encountered will be returned)
		// GSSL/RS (Jun 21,2002) : Fix for 68168, 68662 . Added a logical argument to
		// ignore_unshared_edges
// isospline module options.
/**
 * Gets the deviations of the vertex from the underlying surfaces and edges.
 * <br><br>
 * <b>Role:</b> If the flag maximum is <tt>TRUE</tt>, then the maximum deviation is returned. If this flag is
 * <tt>FALSE</tt>, then the first deviation encountered that is greater than <tt>SPAresabs</tt> is returned.
 * <br><br>
 * @param maximum
 * flag indicating to get max deviation.
 * @param ignore_unshared_edges
 * if ignore_unshared_edges set to TRUE, then the routine will not consider sheet
 * edges for deviation calculation.
 */
		double deviation(logical maximum = TRUE, logical ignore_unshared_edges = TRUE);

		// Project: HEAL_TO_TOLERANCE
		// For internal use only
/**
 * @nodoc
 */
		double deviation_with_tol(logical maximum, logical ignore_unshared_edges, double dev_tol);

		// Gets the max. deviation of the vertex from the faces
		// (if maximum is passed as FALSE, then the first deviation greater
		// than SPAresabs encountered will be returned)
/**
 * Gets the deviations of the vertex from the faces.
 * <br><br>
 * <b>Role:</b> If the flag maximum is <tt>TRUE</tt>, then the maximum deviation is returned. If this flag is
 * <tt>FALSE</tt>, then the first deviation encountered that is greater than <tt>SPAresabs</tt> is returned.
 * <br><br>
 * @param maximum
 * flag indicating to get max deviation.
 */
		double deviation_from_faces(logical maximum = TRUE);

		// Project: HEAL_TO_TOLERANCE
		// For internal use only
/**
 * @nodoc
 */
		double deviation_from_faces_with_tol(logical maximum, double dev_tol);

		// Gets the max. deviation of the vertex from the edges
		// (if maximum is passed as FALSE, then the first deviation greater
		// than SPAresabs encountered will be returned)
		// GSSL/RS (Jun 21,2002) : Fix for 68168, 68662 . Added a logical argument to
		// ignore_unshared_edges
/**
 * Gets the deviations of the vertex from the edges.
 * <br><br>
 * <b>Role:</b> If the flag maximum is <tt>TRUE</tt>, then the maximum deviation is returned. If this flag is
 * <tt>FALSE</tt>, then the first deviation encountered that is greater than <tt>SPAresabs</tt> is returned.
 * <br><br>
 * @param maximum
 * flag indicating to get max deviation.
 * @param ignore_unshared_edges
 * if ignore_unshared_edges set to TRUE, then the routine will not consider sheet
 * edges for deviation calculation.
 */
		double deviation_from_edges(logical maximum = TRUE, logical ignore_unshared_edges = TRUE);

		// Project: HEAL_TO_TOLERANCE
		// For internal use only
/**
 * @nodoc
 */
		double deviation_from_edges_with_tol(logical maximum, logical ignore_unshared_edges, double dev_tol);

		// Returns TRUE if the vertex deviation from
		// the faces and edges is less than SPAresabs
/**
 * Returns <tt>TRUE</tt> if the vertex deviation from the faces and edges is less than <tt>SPAresabs</tt>.
 */
		logical does_not_deviate();

		// Project: HEAL_TO_TOLERANCE
		// For internal use only
/**
 * @nodoc
 */
		logical does_not_deviate_with_tol(double dev_tol);

		// Returns TRUE if the vertex deviation from
		// the faces is less than SPAresabs
/**
 * Returns <tt>TRUE</tt> if the vertex deviation from the faces is less than <tt>SPAresabs</tt>.
 */
		logical does_not_deviate_from_faces();

		// Project: HEAL_TO_TOLERANCE
		// For internal use only
/**
 * @nodoc
 */
		logical does_not_deviate_from_faces_with_tol(double tol);

		// Returns TRUE if the vertex deviation from
		// the faces is less than SPAresabs
		// GSSL/RS (Jun 21,2002) : Fix for 68168, 68662 . Added a logical argument to
		// ignore_unshared_edges
/**
 * Returns <tt>TRUE</tt> if the vertex deviation from the edges is less than <tt>SPAresabs</tt>.
 * <br><br>
 * @param ignore_unshared_edges
 * if ignore_unshared_edges set to TRUE, then the routine will not consider sheet
 * edges for deviation calculation.
 */
		logical does_not_deviate_from_edges(logical ignore_unshared_edges = TRUE);

		// Project: HEAL_TO_TOLERANCE
		// For internal use only
/**
 * @nodoc
 */
		logical does_not_deviate_from_edges_with_tol(logical ignore_unshared_edges, double dev_tol);

		// Returns TRUE if either the vertex deviation from the faces
		// and edges is more than SPAresabs
/**
 * Returns <tt>TRUE</tt> if either the vertex deviation from the faces and edges is more than <tt>SPAresabs</tt> or if any edges around the vertex are bad.
 */
		virtual logical is_bad();

		// Project: HEAL_TO_TOLERANCE
		// For internal use only
/**
 * @nodoc
 */
		virtual logical is_bad_with_tol(double tol);

		///////////////////////////
		// Modification Functions
		///////////////////////////

		// Set a new geometry for the vertex. This function
		// will also reset all the geometry related info.
		// stored in the attributes.
/**
 * Sets the geometry in the attribute to the given point.
 * <br><br>
 * @param pA
 * new point.
 */
		void set_geometry(APOINT* pA);


		/////////////////////////////////////
		// Functions for internal usage only
		/////////////////////////////////////

		// Reset the attribute. This function will reset all the
		// info. of the vertex stored in the attrib. Hence any
		// call to an inquiry function will result in a recalculation
		// all details w.r.t. the latest geometry. This function is
		// typically called whenever a new geomtry is set.

 /**
  * Resets the attribute.
  * <br><br>
  * <dl>
  * <b>Role:</b> Resets the attribute in following way:
  * <dd>&nbsp;- Sets the maximum distance to edges, maximum distance to faces, and maximum distance between edges to 0.0.</dd>
  * <dd>&nbsp;- Sets the on edge, edges meet, and on faces indicators to <tt>UNSET</tt>.</dd>
  * </dl>
 */
		void reset() ;

		// Calls the reset member function of all those
		// attributes whose goodness depends on the geometry of
		// this vertex (i.e. associated edges and coedges)
/**
 * Resets the attribute on the vertex's dependent entities (such as its edges, coedges).
 */
		void reset_dependents() ;

		// RS (Sep 29,2001):
/*
// For internal use only
*/
/**
 * @nodoc
 */
		virtual void notify_geometry_change(void);

		// RS (Sep 29,2001):
/*
// For internal use only
*/
/**
 * @nodoc
 */
		virtual void replace_owner_geometry(ENTITY *);

		// Get vertex tol for geombuild, max of vertex deviation
		// and all the edge tols of edges around
/**
 * Gets the vertex tolerance for geometry building (the maximum of the vertex deviation and all the edge tolerances).
 */
		double geombuild_tol();

		// Reset the geombuild tol so that it gets recalculated
		// the next time it is asked for.
/**
 * Resets the geometry building tolerance so that it is recalculated the next time it is requested.
 */
		void reset_geombuild_tol();

		// Draw bad vertex (only for GSSL Eval UIF)
/*
// For internal use only
*/
/**
 * @nodoc
 */
		void draw();

		//Get node
/*
// For internal use only
*/
/**
 * @nodoc
 */
		HH_GlobalNode * get_node_for_change();

		//Set node
/*
// For internal use only
*/
/**
 * @nodoc
 */
		void set_node(HH_GlobalNode * n);


		// Set functions
/**
 * Gets the indicator of whether or not the vertex is on the edges.
 * <br><br>
 * @param q
 * new indicator.
 */
		void set_on_edges (int q)		{on_edges = q;}
/**
 * Gets the indicator of whether or not edges meet.
 * <br><br>
 * @param q
 * new indicator.
 */
		void set_edges_meet (int q)		{edges_meet = q;}
/**
 * Gets the indicator of whether or not the vertex is on the faces.
 * <br><br>
 * @param q
 * new indicator.
 */
		void set_on_faces (int q)		{on_faces = q;}
/**
 * Gets the maximum distance from the edges.
 * <br><br>
 * @param q
 * new distance.
 */
		void set_edge_dist (double q)		{edge_dist = q;}
/**
 * Gets the maximum distance between the edges.
 * <br><br>
 * @param q
 * new distance.
 */
		void set_meet_dist (double q)	{meet_dist = q;}
/**
 * Gets the maximum distance from the faces.
 * <br><br>
 * @param q
 * new distance.
 */
		void set_face_dist (double q)		{face_dist = q;}


		// For internal use only
/**
 * @nodoc
 */
		// Method to set entity gap classification info. HEAL_TO_TOLERANCE
		void set_entity_gap_classification_info(HH_ENTITY_GAP_CLASSIFICATION_INFO entity_gap_info);


		// Get functions
/**
 * Gets the indicator of whether or not the vertex is on the edges.
 */
		int get_on_edges () const		{return on_edges;}
/**
 * Gets the indicator of whether or not edges meet.
 */
		int get_edges_meet () const		{return edges_meet;}
/**
 * Gets the indicator of whether or not the vertex is on the faces.
 */
		int get_on_faces () const		{return on_faces;}
/**
 * Gets the maximum distance from the edges.
 */
		double get_edge_dist () const		{return edge_dist;}
/**
 * Gets the maximum distance between the edges.
 */
		double get_meet_dist () const		{return meet_dist;}
/**
 * Gets the maximum distance from the faces.
 */
		double get_face_dist () const		{return face_dist;}
/**
 * Returns the vertex quality based on values set in the attribute.
 * <br><br>
 * <b>Role:</b> Possible values are
 * <br><br>
 * <table width = 100%>
 * <tr width = 100%>
 * <td valign = "top" width = 5%>0</td>
 * <td valign = "top">Good quality. The vertex is on the edges, <i>and</i> the edges meet <i>and</i> the vertex is on the faces.</td>
 * </tr>
 * <tr>
 * <td valign = "top">1</td>
 * <td valign = "top">Poor quality. The vertex is not on the edges, <i>or</i> the edges do not meet, <i>or</i> the vertex is not on the faces.</td>
 * </tr>
 * </table>
 */
		int get_quality () const;
/**
 * Performs advanced checks on the vertex.
 * <br><br>
 * <b>Role:</b> Checks if vertex is on edges, maximum distance from the edges, if edges meet,
 * maximum distance between edges, if vertex lies on faces, and maximum distance from the faces.
 */
		int adv_check();

		// For internal use only
/**
 * @nodoc
 */
		// Method to get entity_gap_classification info. HEAL_TO_TOLERANCE
		HH_ENTITY_GAP_CLASSIFICATION_INFO get_entity_gap_classification_info() const;
};

//----------------------------------------------------------------------------
/*
// For internal use only
*/
/**
 * @nodoc
 */
ATTRIB_HH_ENT_GEOMBUILD_VERTEX * find_att_vertex_geombuild( VERTEX * );

/** @} */
#endif
