/*******************************************************************/
/*    Copyright (c) 1989-2020 by Spatial Corp.                     */
/*    All rights reserved.                                         */
/*    Protected by U.S. Patents 5,257,205; 5,351,196; 6,369,815;   */
/*                              5,982,378; 6,462,738; 6,941,251    */
/*    Protected by European Patents 0503642; 69220263.3            */
/*    Protected by Hong Kong Patent 1008101A                       */
/*******************************************************************/

#if !defined (ATTRIB_HH_AGGR_SIMPLIFY_CLASS)
#define ATTRIB_HH_AGGR_SIMPLIFY_CLASS

#include "agrsimbs.hxx"
#include "bhl_stru2.hxx"
#include "bhl_stru.hxx"
#include "dcl_heal.h"
#include "heal_api.hxx"
#include "heal_deprecated_api.hxx"
#include "alltop.hxx"

/**
* @file aggrsimg.hxx
 * @CAA2Level L1
 * @CAA2Usage U1
 * \addtogroup HEALAPIDEP
 *
 * @{
 */

/**
 * @nodoc
 */
ENTITY_IS_PROTOTYPE(ATTRIB_HH_AGGR_SIMPLIFY, HEAL)
#if 0
; // semicolon needed for mkman (doc tool) parsing
#endif
DECL_HEAL extern int ATTRIB_HH_AGGR_SIMPLIFY_TYPE;
/**
 * @nodoc
 */
#define ATTRIB_HH_AGGR_SIMPLIFY_LEVEL (ATTRIB_HH_AGGR_SIMPLIFY_BASE_LEVEL + 1)

//---------------------------------------------------------------------------
// Structure containing no. of each type geometry
/**
 * @nodoc
 * <b>Obsolete:</b> This class has been deprecated.
 * Structure containing the number of each type of geometry
 */
class DECL_HEAL bhl_geom_types
{
public:

	int no_splines;		// No. of splines in the entities
	int no_analytics;	// No. of analytics in the entities
	int no_planes;		// No. of planes in the entities
	int no_cylinders;	// No. of cylinders in the entities
	int no_cones;		// No. of cones in the entities
	int no_tori;		// No. of tori in the entities
	int no_spheres;		// No. of spheres in the entities

	int no_intcurves;
	int no_straights;
	int no_circles;
	int no_ellipses;

	//Constructor
	bhl_geom_types()
	{
		no_splines = 0;
		no_analytics = 0;
		no_planes = 0;
		no_cylinders = 0;
		no_cones = 0;
		no_tori = 0;
		no_spheres = 0;

		no_intcurves = 0;
		no_straights = 0;
		no_circles = 0;
		no_ellipses = 0;
	}

};
//---------------------------------------------------------------------------

/**
 * @nodoc
 * <b>Obsolete:</b> This class has been deprecated.
 * Miscellaneous statistics on simplification
 */
class DECL_HEAL bhl_geom_misc
{
public:

	double bhl_sim_doub_tol;
	double	bhl_sim_nor_tol;
    double	bhl_sim_pos_tol;

	int bhl_closed_split;
    int bhl_sim_no_ell;
    int bhl_sim_no_cir;
    int bhl_sim_no_str;
    int bhl_sim_processed_faces;
};
/**
 * @nodoc
 * <b>Obsolete:</b> This class has been deprecated. For healing, use API interfaces
 * and not Direct Interface.
 *
 * <br><br>
 * Aggregate healing attribute class for the geometry simplification phase.
 * <br>
 * <b>Role:</b> The <tt>ATTRIB_HH_AGGR_SIMPLIFY</tt> class is attached to the body to be healed. It is
 * used by the geometry simplification healing phase.
 */
class DECL_HEAL ATTRIB_HH_AGGR_SIMPLIFY : public ATTRIB_HH_AGGR_SIMPLIFY_BASE
{
protected :
		logical m_do_simplify;	// Logical to denote if
						// simplification is required

		// RNC 8 july 99: addition of option for logging changes to entities
		// in simplification module
		logical m_do_simplify_log;

		logical m_planes_only;	// Logical to denote if
							// only planes should be simplified

		double m_tol;		// Simplification Tolerance

		bhl_geom_types input_geom_stats;
		bhl_geom_types output_geom_stats;
		bhl_geom_misc m_geom_misc_stats;

		logical m_do_curve_simplify;
		double m_max_radius;
		logical save_sw;


public:
/**
 * C++ initialize constructor requests memory for this object and populates it with the data supplied as arguments.
 * <br><br>
 * <b>Role:</b> Applications should call this constructor only with the overloaded new operator
 * inherited from the <tt>ENTITY</tt> class (for example, <tt>x=new ATTRIB_HH_AGGR_SIMPLIFY(...)</tt>), because
 * this reserves the memory on the heap, a requirement to support roll back and history management.
 * <br><br>
 * @param owner
 * owning body.
 */
		ATTRIB_HH_AGGR_SIMPLIFY (BODY* owner = NULL);
/**
 * Returns <tt>TRUE</tt> if this is pattern compatible.
 */
        virtual logical pattern_compatible() const;
                                    // returns TRUE
 		// Chain individual attributes
/**
 * Attaches an individual entity-level attribute to the given entity.
 * <br><br>
 * <b>Role:</b> This method chains individual attributes.
 * <br><br>
 * @param ent
 * entity to attach to.
 */
		virtual ATTRIB_HH_ENT*  attach_attrib( ENTITY* ent);

		// Get all the entities chained to the body. These are
		// entities to which individual attributes are attached.
/**
 * Gets all the entities chained to the owning body and these are entities to which individual attributes are attached.
 * <br><br>
 * @param ent
 * list of entities.
 */
		virtual void entity_list(ENTITY_LIST& ent) const;

		// Get the attribute for the corresponding entity
/**
 * Gets the entity-level attribute for the corresponding entity.
 * <br><br>
 * @param ent
 * owning entity.
 */
		virtual ATTRIB_HH_ENT* get_attrib( ENTITY* ent) const;

 		// Remove the attribute for the entity
/**
 * Removes the entity-level attribute from the entity.
 * <br><br>
 * @param ent
 * owning entity.
 */
		virtual void detach_attrib( ENTITY* ent);

		// Remove all individual attributes.
		// LOSE_DEF calls it as well
/**
 * Removes all individual entity-level attributes from the entities of the owning body.
 */
		virtual void  cleanup();

		// Attach individual attributes to all spline faces
/**
 * Attaches individual entity-level attributes to all spline faces.
 */
		void attach_attribs_to_splines();

		// Detach all empty individual attribs
/**
 * Detaches all empty individual entity-level attributes; that is, those attributes that do not carry any simplified geometry.
 */
		void detach_empty_attribs();

		// Reset the data members based on heuristics
/**
 * Analyzes all the faces of the owning body for geometry simplification.
 * <br><br>
 * <b>Role:</b> Sets appropriate value for the geometry simplification tolerance based on this
 * analysis.
 */
		virtual void analyze();

		// Traverse all faces and add entity attributes. Also
		// call the analyze member function of entity attribute
		// Then analysis is performed for each face of body at
		// the tolerance.
/**
 * Traverses all faces and adds individual entity-level attributes to faces having spline geometry.
 * <br><br>
 * <b>Role:</b> Then, for every spline surface, calculates (if possible) the simplified geometry
 * within the specified tolerance. Stores the simplified geometry in the individual entity-level
 * attributes.
 */
		virtual void calculate();

		// Fix all the attributes to individual entities.
/**
 * Applies (fixes) all the changes that are stored in individual attributes for the geometry simplification phase to the body and the old geometry is then stored in the attributes.
 */
		virtual void fix();

		// Print overall results
/**
 * Prints the geometry simplification analysis results of both the input and the output body to the specified file.
 * <br><br>
 * @param fp
 * file pointer.
 */
		virtual void print(FILE *fp);

		// Print overall results in a string
/**
 * Prints statistics of the results of the geometry simplification phase to the specified string.
 * <br><br>
 * @param str
 * character string.
 */
		virtual void sprint(char *str);

		//Print the results of analysis into a string passed.
/**
 * Prints statistics of the results of the analyze stage of the geometry simplification phase to the specified string.
 * <br><br>
 * @param str
 * character string.
 */
		virtual void sprint_analyze(char *str);

		// Print results of analysis
/**
 * Prints statistics of the results of the analyze stage of the geometry simplification phase to the specified file.
 * <br><br>
 * @param fp
 * file pointer.
 */
		virtual void print_analyze(FILE *fp);

		// Print results of calculations
/**
 * Prints statistics of the results of the calculate stage of the geometry simplification phase to the specified file.
 * <br><br>
 * @param fp
 * file pointer.
 */
		virtual void print_calculate(FILE *fp);

		//Print the calculation results in to the string passed
/**
 * Prints statistics of the results of the calculate stage of the geometry simplification phase to the specified string.
 * <br><br>
 * @param str
 * character string.
 */
		virtual void sprint_calculate(char *str );

		// Print results of fix
/**
 * Prints statistics of the results of the fix stage of the geometry simplification phase to the specified file.
 * <br><br>
 * @param fp
 * file pointer.
 */
		virtual void print_fix(FILE *fp);

		//Print the fix results in to the string passed
/**
 * Prints statistics of the results of the fix stage of the geometry simplification phase to the specified string.
 * <br><br>
 * @param str
 * character string.
 */
		virtual void sprint_fix(char *str );

		// Get/Set functions for simgeom tolerance
/**
 * Returns <tt>FALSE</tt> if the geometry simplification phase has been turned off, otherwise, returns <tt>TRUE</tt>.
 */
		logical do_simplify() const;
/**
 * Returns the value of the <tt>m_planes_only</tt> flag, which indicates whether or not only planes should be simplified and If <tt>TRUE</tt>, only plane surface types are simplified.
 */
		logical planes_only() const;
/**
 * Gets the current value of the geometry simplification tolerance.
 */
		double tol() const;

		// RNC : 8 july 99.
		// Get/Set functions for logging simplification module changes
/**
 * Gets the option to denote if simplify log generation is on or off.
 */
		logical do_simplify_log() const;
/**
 * Sets a flag indicating whether or not the geometry simplification phase needs to be used and if <tt>TRUE</tt>, the geometry simplification phase is used; otherwise, it is not used.
 * <br><br>
 * @param logical phase_flag
 * use this phase flag.
 */
		void set_do_simplify(logical phase_flag);
		// RNC : 8 july 99. void set_do_simplify_log(logical); added.
/**
 * Sets the option to denote if simplify log generation is on or off.
 * <br><br>
 * @param sim_log
 * simplify log.
 */
		void set_do_simplify_log(logical sim_log);

/**
 * Sets the value of the m_planes_only flag, which indicates whether or not only planes should be simplified and If <tt>TRUE</tt>, only plane surface types are simplified.
 * <br><br>
 * @param plane_flag
 * planes only flag value.
 */
		void set_planes_only(logical plane_flag);
/**
 * Sets the tolerance used by the geometry simplification phase.
 * <br><br>
 * @param tol
 * tolerance to use.
 */
		void set_tol(double tol);

		// GSSL/RA - EDGE SIMPLPIFICATION PROJECT
/**
 * Sets the simplify options used by simplification.
 * <br><br>
 * @param simp_opts
 * simplify options to set.
 */
		void set_simplification_options(hh_simplify_options *simp_opts);

/**
 * Returns whether curve simplification is to be done.
 */
		int to_do_curve_simplify();

/**
 * Does curve simplification.
 */
		void simplify_curves();

/**
 * Returns the maximum permissible radius of a circle to which a curve can be simplified.
 */
		double get_max_radius();

		// Query functions for analysis
/**
 * Returns the number of input splines.
 */
		int num_input_splines() const;
/**
 * Returns the number of input analytics.
 */
		int num_input_analytics() const;
/**
 * Returns the number of input planes.
 */
		int num_input_planes() const;
/**
 * Returns the number of input cylinders.
 */
		int num_input_cylinders() const;
/**
 * Returns the number of input cones.
 */
		int num_input_cones() const;
/**
 * Returns the number of input spheres.
 */
		int num_input_spheres() const;
/**
 * Returns the number of input tori.
 */
		int num_input_tori() const;
/**
 * Returns the number of input intcurves.
 */
		int num_input_intcurves() const;
/**
 * Returns the number of input straights.
 */
		int num_input_straights() const;
/**
 * Returns the number of input tori.
 */
		int num_input_circles() const;
/**
 * Returns the number of input tori.
 */
		int num_input_ellipses() const;
/**
 * Returns the number of expected spline-to-analytic conversions.
 */
		int num_expected_analytics() const;
/**
 * Returns the number of expected spline-to-plane conversions.
 */
		int num_expected_planes() const;
/**
 * Returns the number of expected spline-to-cylinder conversions.
 */
		int num_expected_cylinders() const;
/**
 * Returns the number of expected spline-to-cone conversions.
 */
		int num_expected_cones() const;
/**
 * Returns the number of expected spline-to-sphere conversions.
 */
		int num_expected_spheres() const;
/**
 * Returns the number of expected spline-to-tori conversions.
 */
		int num_expected_tori() const;
/**
 * Returns the number of spline surfaces remaining in the final body.
 */
		int num_final_splines() const;
/**
 * Returns the number of of splines converted to analytics.
 */
		int num_converted_analytics() const;
/**
 * Returns the number of splines converted to planes.
 */
		int num_converted_planes() const;
/**
 * Returns the number of of splines converted to cylinders.
 */
		int num_converted_cylinders() const;
/**
 * Returns the number of of splines converted to cones.
 */
		int num_converted_cones() const;
/**
 * Returns the number of of splines converted to spheres.
 */
		int num_converted_spheres() const;
/**
 * Returns the number of of splines converted to tori.
 */
		int num_converted_tori() const;
/**
 * Returns the number of of intcurves converted to straights.
 */
		int num_converted_straights() const;
/**
 * Returns the number of of intcurves converted to circles.
 */
		int num_converted_circles() const;
/**
 * Returns the number of of intcurves converted to ellipses.
 */
		int num_converted_ellipses() const;
/**
 * Returns the number of of intcurves converted to ellipses.
 */
		int num_final_intcurves() const;
/**
 * Retrieves the <tt>bhl_geom_misc_stats</tt> structure.
 */
		bhl_geom_misc& bhl_geom_misc_stats() {backup(); return m_geom_misc_stats;}
/**
 * Retrieves the <tt>bhl_geom_types_func</tt> structure.
 */
		bhl_geom_types& bhl_geom_types_func(){ backup(); return output_geom_stats;}

		/**
		 * @nodoc
		 */
		ATTRIB_FUNCTIONS (ATTRIB_HH_AGGR_SIMPLIFY, HEAL)
#if 0
; // semicolon needed for mkman (doc tool) parsing)
#endif
// Because mkman isn't aware that ATTRIB_FUNCTIONS changes access to "public", we do so
// explicitly here:
public:

};
/**
 * @nodoc
 */
DECL_HEAL ATTRIB_HH_AGGR_SIMPLIFY* find_aggr_simplify (BODY* inp_body);
/**
 * @nodoc
 */
void hh_simplify_set_geometry(FACE* face, SURFACE* new_surf, logical use_attrib);

/*
// For internal use only
*/
/**
 * @nodoc
 */
SURFACE* hh_simplify_get_geometry(FACE* face, logical use_attrib);
/** @} */
#endif // ATTRIB_HH_AGGR_SIMPLIFY_CLASS
