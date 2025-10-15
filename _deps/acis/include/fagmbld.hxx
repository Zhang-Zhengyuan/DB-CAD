/*******************************************************************/
/*    Copyright (c) 1989-2020 by Spatial Corp.                     */
/*    All rights reserved.                                         */
/*    Protected by U.S. Patents 5,257,205; 5,351,196; 6,369,815;   */
/*                              5,982,378; 6,462,738; 6,941,251    */
/*    Protected by European Patents 0503642; 69220263.3            */
/*    Protected by Hong Kong Patent 1008101A                       */
/*******************************************************************/

//----------------------------------------------------------------------------

#if !defined	ATTRIB_HH_ENT_GEOMBUILD_FACE_CLASS
#define			ATTRIB_HH_ENT_GEOMBUILD_FACE_CLASS
//----------------------------------------------------------------------------
#include	"edge.hxx"
#include    "dcl_heal.h"
#include	"entgmbld.hxx"
#include    "splutil.hxx"
#include "all_chk.hxx"
class ATTRIB_HH_AGGR_GEOMBUILD_BASE;
class FACE;
class SURFACE;
class HH_GlobalNode;

/**
 * @file fagmbld.hxx
 * @CAA2Level L1
 * @CAA2Usage U1
 * \addtogroup HEALAPIDEP
 *
 * @{
 */
/**
 * @nodoc
 */
ENTITY_IS_PROTOTYPE(ATTRIB_HH_ENT_GEOMBUILD_FACE, HEAL )
#if 0
; // semicolon needed for mkman (doc tool) parsing
#endif


DECL_HEAL extern int ATTRIB_HH_ENT_GEOMBUILD_FACE_TYPE;
/**
 * @nodoc
 */
#define ATTRIB_HH_ENT_GEOMBUILD_FACE_LEVEL (ATTRIB_HH_ENT_GEOMBUILD_BASE_LEVEL + 1 )

//----------------------------------------------------------------------------
/**
 * @nodoc
 * <b>Obsolete:</b> This class has been deprecated. For healing, use API interfaces
 * and not Direct Interface. For analysis of problems in models, use the Checker 
 * API interfaces instead.
 * <br><br>
 * Individual entity-level healing attribute class attached to faces in the geometry building phase.
 * <br>
 * <b>Role:</b> <tt>ATTRIB_HH_ENT_GEOMBUILD_FACE</tt> is the individual entity-level attribute class attached
 * to faces during the geometry building phase. Individual entity-level attributes are attached to
 * the individual entities of body being healed to store entity-specific information about each phase
 * or subphase of the healing process. The individual entity-level attributes for each phase or
 * subphase are managed by the aggregate attribute for that phase/subphase.
 * @see SURFACE
 */
class  DECL_HEAL ATTRIB_HH_ENT_GEOMBUILD_FACE : public ATTRIB_HH_ENT_GEOMBUILD_BASE {
	protected:

		SURFACE* m_old_geom;	// stores the old face geometry
		REVBIT m_old_sense;		// stores the old face sense
//		logical old_geom_stored;	// denotes if the old edge geometry has
//									// been stored
//		logical old_sense_stored;	// denotes if the old edge sense has
//									// been stored

		int m_discontinuous;		// Whether the surface is discontinuous

		double m_face_box_size;

		//STI mka
		int lolo_inter;			// = 0 -> no loop/loop intersections, = 1 -> intersections
		int loop_inter;			// = 0 -> no lopp self-intersections, = 1 -> intersections
		int loops;				// loop quality
		int surface_quality;	// surface quality
		int face_narrow;		// = 1 -> face width < minimum
		int face_area;			// = 1 -> face area < minimum or negative
		logical surf_extended;  // True if surface has been extended else FALSE
		double narrow;			// width
		double area;			// area
		double face_mov_tol;
		HH_GlobalNode * node; //Pointer to node on face
// STI cpp begin
		logical save_sw;
// STI cpp end


	public:

		// Constructor
/**
 * C++ initialize constructor requests memory for this object and populates it with the data supplied as arguments.
 * <br><br>
 * <b>Role:</b> Applications should call this constructor only with the overloaded new operator
 * inherited from the <tt>ENTITY</tt> class (for example, <tt>x=new ATTRIB_HH_ENT_GEOMBUILD_FACE(...)</tt>), because
 * this reserves the memory on the heap, a requirement to support roll back and history management.
 * <br><br>
 * @param e
 * owning coedge.
 */
		ATTRIB_HH_ENT_GEOMBUILD_FACE( FACE *e=NULL);

		/**
		 * @nodoc
		 */
		ATTRIB_FUNCTIONS( ATTRIB_HH_ENT_GEOMBUILD_FACE, HEAL )
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

		// Get the new geometry of the face. This will return
		// the old geometry iff no new geomerty has been computed
/**
 * Returns the latest geometry calculated for the face.
 * <br><br>
 * <b>Role:</b> Returns the original geometry if no new geometry has been calculated.
 */
		SURFACE* new_geometry() const;


		// Gets the original geometry of the face even after
		// the new geometry is set.
/**
 * Returns the original geometry associated with the face.
 * <br><br>
 * <b>Role:</b> Returns the original geometry associated with the face. Even after fixing the
 * new geometry, this method continues to return the old geometry.
 */
		SURFACE* old_geometry() const;

		// Get the new sense of the face. This will return
		// the old geometry iff no new geomerty has been computed
/**
 * Returns the latest face sense calculated.
 * <br><br>
 * <b>Role:</b> Returns the original sense if no new sense has been calculated.
 */
		REVBIT new_sense() const;

		// Gets the original sense of the edge even after
		// the new geometry is set.
/**
 * Returns the original face sense.
 * <br><br>
 * <b>Role:</b> Returns the original face sense. Even after fixing the new geometry, this method
 * continues to return the old sense.
 */
		REVBIT old_sense() const;

		// Get face tol for geombuild, max of all the edge tols
/**
 * Gets the face tolerance for geombuild (the maximum of all the edge tolerances).
 */
		double geombuild_tol() const;

		//Get the tol for movement of face
/**
 * Gets the tolerance for movement of the face.
 */
		double get_face_mov_tol();

		//Get node

/**
 * @nodoc
 */
		HH_GlobalNode * get_node_for_change();

		//Set node

/**
 * @nodoc
 */
		void set_node(HH_GlobalNode * n);

		// Returns TRUE if the surface C1/G1 discontinuous at multiple knots
/**
 * Returns <tt>TRUE</tt> if the surface is C1/G1 discontinuous at multiple knots.
 */
		logical is_discontinuous();

		//Returns true if the face has a blend attrib
/**
 * Determines whether or not the face has a blend attribute.
 */
// GSSL/RS - Reblend project
	///	logical is_blend();

		// Returns the approximate SPAbox size of the face
/**
 * Returns the approximate box size of the face.
 */
		double face_box_size();

		///////////////////////////
		// Modification Functions
		///////////////////////////

		// Set a new geometry for the face. This function
		// will also reset all the geometry related info.
		// stored in the attributes.
/**
 * Sets the geometry in the attribute to the given surface.
 * <br><br>
 * @param pS
 * new surface.
 */
		void set_geometry( SURFACE * pS);

		// Set a old geometry for the face.
/**
 * @nodoc
 */
		void set_old_geometry( SURFACE * pS);

		// Set a new sense for the face. This function
		// will also reset all the geometry related info.
		// stored in the attributes.
/**
 * Sets the face sense.
 * <br><br>
 * @param pS
 * new sense.
 */
		void set_sense (REVBIT pS);


		/////////////////////////////////////
		// Utility functions
		/////////////////////////////////////

/**
 * @nodoc
 */
		void do_sharp();

		/////////////////////////////////////
		// Functions for internal usage only
		/////////////////////////////////////

		// Reset the attribute. This function will reset badness related
		// info. of the face stored in the attrib.
/**
 * Resets the attribute.
 * <br><br>
 * <b>Role:</b> Resets the attribute.
 * <br>
 * &nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;-  Sets the face width to 0.0.<br>
 * &nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;-  Sets the face area to 0.0.<br>
 * &nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;-  Sets the various indicators
 *                                 (loop/loop intersection, loop self-intersection, loop quality,<br>
 * &nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;surface quality, face area status,
 *                                 and face width status) to <tt>UNSET</tt>.
 */
		void reset();

		// Reset the coedge details of all the coedges of the owner face.
/**
 * Resets the coedge details of all the coedges of the face.
 */
		void reset_coedge_details();

		// Calls the reset member function of all those
		// attributes whose goodness depends on the geometry of
		// this face (i.e. vertices, edges and coedges)
/**
 * Resets the attribute on the face's dependent entities (such as its edges, coedges, vertices, etc).
 */
		void reset_dependents();

		// RS (Sep 29,2001):

/**
 * @nodoc
 */
		virtual void notify_geometry_change(void);

		// RS (Sep 29,2001):

/**
 * @nodoc
 */
		virtual void replace_owner_geometry(ENTITY *);

		// Reset the local tolerances of all the edges and vertices
		// around the face.
/**
 * Resets the local tolerances of all the edges and vertices around the face.
 */
		void reset_geombuild_tol();

		// Get/Set the radius of a blend if the face has a
		//blend  attrib attached to it.
/**
 * Sets the radius for the given blend.
 * <br><br>
 * @param blnd_rad
 * new blend radius.
 */
	// GSSL/RS - Reblend project
	// Function commented to remove dependency on blend
	///	void set_blend_radius(double blnd_rad);
/**
 * Gets the radius of the given blend.
 */
	/// GSSL/RS - Reblend project
	// Function commented to remove dependency on blend
	///	double get_blend_radius();

		//STI mka
/**
 * Sets the loop/loop intersection indicator.
 * <br><br>
 * @param q
 * new indicator.
 */
		void set_lolo_inter (int q) {lolo_inter = q;}
/**
 * Sets the loop self-intersection indicator.
 * <br><br>
 * @param q
 * new self-int indicator.
 */
		void set_loop_inter (int q) {loop_inter = q;}
/**
 * Sets the loop quality indicator.
 * <br><br>
 * @param q
 * new quality indicator.
 */
		void set_loops (int q) {loops = q;}
/**
 * Sets the indicator of the underlying surface quality.
 * <br><br>
 * @param q
 * new value.
 */
		void set_surface_quality (int q) {surface_quality = q;}
/**
 * Sets the face width.
 * <br><br>
 * @param d
 * new width value.
 */
		void set_narrow (double d) {narrow = d;}
/**
 * Sets the face area.
 * <br><br>
 * @param d
 * new area value.
 */
		void set_area (double d) {area = d;}
/**
 * Sets the face area status indicator.
 * <br><br>
 * @param q
 * new area indicator.
 */
		void set_face_area (int q) {face_area = q;}
/**
 * Sets the face width status indicator.
 * <br><br>
 * @param q
 * new width indicator.
 */
		void set_face_narrow (int q) {face_narrow = q;}
/**
 * Sets surface extension on or off.
 * <br><br>
 * @param a
 * new value.
 */
		void set_surf_extended(logical a) { surf_extended = a;}

		// Get functions
/**
 * Gets the loop/loop intersection indicator.
 */
		int get_lolo_inter () const {return lolo_inter;}
/**
 * Gets the loop self-intersection indicator.
 */
		int get_loop_inter () const {return loop_inter;}
/**
 * Gets the loop quality indicator.
 */
		int get_loops () const {return loops;}
/**
 * Gets the face area status indicator.
 */
		int get_face_area () const {return face_area;}
/**
 * Gets the face width status indicator.
 */
		int get_face_narrow () const {return face_narrow;}
/**
 * Gets the indicator of the underlying surface quality.
 */
		int get_surface_quality () const {return surface_quality;}
/**
 * Gets the face width.
 */
		double get_narrow () const {return narrow;}
/**
 * Returns <tt>TRUE</tt> if the surface is extended.
 */
		logical get_surf_extended() const {return surf_extended;}
/**
 * Gets the face area.
 */
		double get_area () const {return area;}
/**
 * Returns the face quality based on values set in the attribute.
 * <br><br>
 * <b>Role:</b> Returns the face quality based on values set in the attribute.
 * <br><br>
 * Possible values are
 * <table width="75%">
 * <tr>
 * <td valign="top" width="5%">&nbsp;&nbsp;0 </td>	<td>Good quality. There are no loop/loop intersections,
 *      <i>and</i> there are no loop self-intersections, <i>and</i> the underlying surface quality is good
 *      (0), <i>and</i> the loop quality is good (0), <i>and</i> the face width is greater than the minimum,
 *      <i>and</i> the face area is greater than the minimum.</td>
 * </tr>
 * <tr>
 * <td valign="top" width="5%">&nbsp;&nbsp;1 </td>	<td>Poor quality. There are loop/loop intersections,
 *      <i>or</i> there are loop self-intersections, <i>or</i> the underlying surface quality is poor (1),
 *      <i>or</i> the loop quality is poor (1), <i>or</i> the face width is less than the minimum, <i>or</i>
 *      the face area is less than the minimum.</td>
 * </tr>
 * </table>
 * <br>
 * Returns <tt>UNSET</tt> if neither of these conditions is met.
 */
		int get_quality () const;
/**
 * Performs advanced checks on the face.
 * <br><br>
 * <b>Role:</b> Checks for loop/loop intersections, loop self-intersections, loop quality,
 * surface quality, face width status, and face area status.
 */
		int adv_check();


		//************************************************************************
		// RNC : 13 july 99 for logging in changes in attributes to entities.

		// logging of surf snap details

/**
 * @nodoc
 */
		void set_analytic_log_details(const SPAtransf&, FACE * = NULL);

/**
 * @nodoc
 */
		void sprint_analytic_log_details(const SPAtransf&, char * = NULL);

		// logging of surface extension details

/**
 * @nodoc
 */
		void set_geombuild_log_details(double dist[2][2]);

/**
 * @nodoc
 */
		void sprint_geombuild_log_details(char *, double dist[2][2]);

		// logging of surface netting details

/**
 * @nodoc
 */
		// GSSL\SXT 8.6.2004 UPVA mkchksrc errors fixed
		// arguments passed by value changed to arguments passed by reference

		void set_gen_spline_log_details(const spline & new_spl, int num_u_grid_line, int num_v_grid_line);

/**
 * @nodoc
 */
		void sprint_gen_spline_log_details(char *, spline, int num_u_grid_line, int num_v_grid_line);

		// logging of surface knot insertion details
/**
 * Logs surface knot insertion details.
 * <br><br>
 * <b>Role:</b> Logs surface knot insertion details. If the <tt>FACES</tt> was healed in the iso-spline
 * solver, this function adds information to the <tt>ATTRIB_HH_ENT_GEOMBUILD_FACE</tt> attribute associated
 * with the <tt>FACE</tt>. The information marks the faces as being healed using the isospline solver.
 * <br><br>
 * @param knots_add_inu
 * u knots.
 * @param knots_add_inv
 * v knots.
 * @param old_nv
 * old nv.
 * @param old_nu
 * old nu.
 * @param new_nv
 * new nv.
 * @param new_nu
 * new nu.
 */
		void set_iso_spline_log_details(int knots_add_inu, int knots_add_inv, int old_nv, int old_nu,
										int new_nv, int new_nu);
/**
 * Returns the surface knot insertion details from the log.
 * <br><br>
 * <b>Role:</b> This function prints out the information stored in the iso_spline_log_details if
 * the information exists in the <tt>ATTRIB_HH_ENT_GEOMBUILD_FACE</tt> attribute associated with the
 * given <tt>FACE</tt>.
 * <br><br>
 * @param str
 * string.
 * @param knots_add_inu
 * u knots.
 * @param knots_add_inv
 * v knots.
 * @param old_nv
 * old nv.
 * @param old_nu
 * old nu.
 * @param new_nv
 * new nv.
 * @param new_nu
 * new nu.
 */
		void sprint_iso_spline_log_details(char *str, int knots_add_inu, int knots_add_inv, int old_nv,
										   int old_nu, int new_nv, int new_nu);


		//************************************************************************

};
/**
 * @nodoc
 */
const double MIN_FACE_WIDTH = 1e-4;
/**
 * @nodoc
 */
const double MIN_FACE_AREA = 1e-5;
/**
 * @nodoc
 */
DECL_HEAL ATTRIB_HH_ENT_GEOMBUILD_FACE * find_att_face_geombuild( FACE * );

/** @} */
#endif
