/*******************************************************************/
/*    Copyright (c) 1989-2020 by Spatial Corp.                     */
/*    All rights reserved.                                         */
/*    Protected by U.S. Patents 5,257,205; 5,351,196; 6,369,815;   */
/*                              5,982,378; 6,462,738; 6,941,251    */
/*    Protected by European Patents 0503642; 69220263.3            */
/*    Protected by Hong Kong Patent 1008101A                       */
/*******************************************************************/

//----------------------------------------------------------------------------
#if !defined ATTRIB_HH_AGGR_GEOMBUILD_CLASS
#define		 ATTRIB_HH_AGGR_GEOMBUILD_CLASS
//----------------------------------------------------------------------------
/**
* @file hmainhdr.hxx
 * @CAA2Level L1
 * @CAA2Usage U1
 * \addtogroup HEALAPIDEP
 *
 * @{
 */
#include <stdio.h>
#include	"logical.h"
#include	"option.hxx"

#include "dcl_heal.h"
#include "bhl_stru2.hxx"
#include "bhl_stru.hxx"
#include "aggrgbld.hxx"
#include "entgmbld.hxx"

class tighten_gaps_options;

class HH_Anal_Geombld;
//----------------------------------------------------------------------------
/**
 * @nodoc
 */
ENTITY_IS_PROTOTYPE(ATTRIB_HH_AGGR_GEOMBUILD, HEAL )
#if 0
; // semicolon needed for mkman (doc tool) parsing
#endif
DECL_HEAL extern int ATTRIB_HH_AGGR_GEOMBUILD_TYPE;
/**
 * @nodoc
 */
#define ATTRIB_HH_AGGR_GEOMBUILD_LEVEL \
	(ATTRIB_HH_AGGR_GEOMBUILD_BASE_LEVEL+1)

//----------------------------------------------------------------------------

/**
 * @nodoc
 * <b>Obsolete:</b> This class has been deprecated.
 */

class DECL_HEAL hh_geombuild_options {
public:
	double min_geombuild_tol;
	double	geombuild_tol;
	logical use_geombuild;
	logical check_discontinuity;
	double bhl_tang_tol;
	double hh_min_spline_tang_tol;
	double hh_max_spline_tang_tol;

	// Constructor
	hh_geombuild_options ();

	void reset();

	void set();
};
//----------------------------------------------------------------------------
/**
 * @nodoc
 * <b>Obsolete:</b> This class has been deprecated. For healing, use API interfaces
 * and not Direct Interface.
 * <br><br>
 * Aggregates healing attribute class for the geometry building phase.
 * <br>
 * <b>Role:</b> The <tt>ATTRIB_HH_AGGR_GEOMBUILD</tt> class is attached to the body to be healed. It is used
 * by the geometry building healing phase.
 */
class DECL_HEAL ATTRIB_HH_AGGR_GEOMBUILD : public ATTRIB_HH_AGGR_GEOMBUILD_BASE {
protected:
	// Struct containing options and tols for geombuild
	hh_geombuild_options geombuild_options;

	// Result of geometry building
	bhl_geometry_results results;

	// Analysis results of input body
	bhl_anal_geometry_results anal_results;

	// Analysis results of the healed body
	bhl_anal_geometry_results output_anal_results;

	logical m_pcurves_already_cleaned;

	// RNC : 16 Jun 99
	MODULE_HEAL_STATUS m_preprocess_heal_state;
    MODULE_HEAL_STATUS m_postprocess_heal_state;

	// RNC 15 july 99: addition of option for logging changes to entities
	// in geombuild module
	logical m_do_geombuild_log;

	// HEAL_TO_TOLERANCE
	//double m_required_gap_tightness;
	tighten_gaps_options* m_tighten_gaps_options;

	// SXT, 10-Jul-2006, Bug 81621
	logical m_force_heal;

public:
/**
 * Points to the main routine which does all the analysis of the input <tt>BODY</tt>.
 * <br><br>
 * <b>Role:</b> The analysis finds all the bad geometry in the <tt>BODY</tt> and adds attributes to the bad
 * parts of the model to be used during the repair stage.
 */
	HH_Anal_Geombld* geom_analyzer ;

	// Constructor
/**
 * C++ initialize constructor requests memory for this object and populates it with the data supplied as arguments.
 * <br><br>
 * <b>Role:</b> Applications should call this constructor only with the overloaded new operator
 * inherited from the <tt>ENTITY</tt> class (for example, <tt>x=new ATTRIB_HH_AGGR_GEOMBUILD(...)</tt>), because this
 * reserves the memory on the heap, a requirement to support roll back and history management.
 * <br><br>
 * @param b
 * owning body.
 */

	// GSSL/RS - Moving the implementation to cpp file
	ATTRIB_HH_AGGR_GEOMBUILD( BODY * b = NULL );

/**
 * Returns <tt>TRUE</tt> if this is pattern compatible.
 */
    virtual logical pattern_compatible() const;
                                    // returns TRUE
	// Returns the analysis results on the input body
/**
 * Returns the results of geometry building analysis.
 */
	const bhl_anal_geometry_results analysis_results() const {
		return anal_results;
	}

	// Returns the analysis results on the output body
/**
 * Returns the geometry building analysis results for the output body.
 */
	const bhl_anal_geometry_results output_analysis_results() const {
		return output_anal_results;
	}

	// Returns result of geometry building
/**
 * Returns the results of the geometry building phase.
 */
	const bhl_geometry_results geom_results() const {
		return results;
	}

	// Initialize the tolerances of the various geombuild
	// modules.
/**
 * Initializes all the tolerances used in the geometry building phase (including the various geometry building subphases).
 */
	void initialize_tols();

	// Analyze the the geometry of the body and set
	// appropriate options and tols for the various
	// geometry building modules
/**
 * Performs the analyze stage of the geometry building phase.
 * <br><br>
 * <b>Role:</b> Analyzes the body and compute the best options and tolerances for the various
 * subphases of the geometry building phase.
 */
	virtual void analyze();

	// Calculate new geometry for the unhealed portions
	// the body. The new geometry is stored in individual
	// attributes.
/**
 * Performs the calculate stage of the geometry building phase.
 * <br><br>
 * <b>Role:</b> Stores all the recommended changes in individual attributes.
 * The new geometry for all the unhealed portions of the body is calculated.
 */
	virtual void calculate();

	// Validate the final geometries of the faces, edges,coedges and
	// vertices whose good geometries have been calculated.
/**
 * Applies (fixes) all the changes that are stored in individual attributes for the geometry building phase to the body.
 * <br><br>
 * <b>Role:</b> The old geometry is then stored in the attributes.
 */
	virtual void fix();

	// Check the owner body for geometric inaccuracies
/**
 * Attaches individual geometry building attributes to the body and checks the body for bad geometry.
 * <br><br>
 * @param check_results
 * struct containing results.
 */
	void check(bhl_anal_geometry_results* check_results);

	// Project: HEAL_TO_TOLERANCE
	// For internal use only
/**
 * @nodoc
 */
	void check_and_record_problem(bhl_anal_geometry_results* check_results, logical record_problem);

	// Do advance analysis of the geometry of the body
/**
 * Performs advanced analysis on the geometry of the body.
 * <br><br>
 * <b>Role:</b> It checks the body owned by the aggregate attribute and stores results in individual
 * entity-level attributes. This method executes all the various "advanced analysis" methods that
 * are defined in this class for specific entity types (vertices, edges, coedges, loops, faces,
 * shells, lumps, curves, pcurves, and surfaces).
 */
	virtual void adv_analyze();
	// Do advance analysis of the coedges of the body
/**
 * Performs advanced analysis on the coedges of the body.
 * <br><br>
 * <b>Role:</b> It checks the coedges of the body owned by the aggregate attribute and stores
 * results in individual entity-level attributes.
 * <br><br>
 * The tests performed include:
 * <ul>
 * <li>Does the coedge lie on the corresponding face surface?</li>
 * <li>If the coedge contains a pcurve, does the domain of the pcurve correspond with the edge?</li>
 * <li>Does the coedge have a partner?</li>
 * <li>If the coedge contains a pcurve, is the pcurve within tolerance of the edge?</li>
 * </ul>
 */
	virtual void adv_analyze_coedge();
	// Do advance analysis of the vertices of the body
/**
 * Performs advanced analysis on the vertices of the body.
 * <br><br>
 * <b>Role:</b> It checks the vertices of the body owned by the aggregate attribute and
 * stores results in individual entity-level attributes.
 * <br><br>
 * The tests performed include:
 * <ul>
 * <li>Does the vertex lie on the corresponding edges?</li>
 * <li>Do the edges meet at the vertex?</li>
 * <li>Does the vertex lie on the corresponding surfaces?</li>
 * </ul>
 */
	virtual void adv_analyze_vertex();
	// Do advance analysis of the edges of the body
/**
 * Performs advanced analysis on the edges of the body.
 * <br><br>
 * <b>Role:</b> It checks the edges of the body owned by the aggregate attribute and stores
 * results in individual entity-level attributes.
 * <br><br>
 * The tests performed include:
 * <ul>
 * <li>Check the curve geometry (<tt>adv_analyze_curve</tt>).</li>
 * <li>Determine convexity.</li>
 * <li>Check edge length.</li>
 * </ul>
 */
	virtual void adv_analyze_edge();
	// Do advance analysis of the faces of the body
/**
 * Performs advanced analysis on the faces of the body.
 * <br><br>
 * <b>Role:</b> It checks the faces of the body owned by the aggregate attribute and stores
 * results in individual entity-level attributes.
 * <br><br>
 * The tests performed include:
 * <ul>
 * <li>Check the loops (<tt>adv_analyze_loop</tt>).</li>
 * <li>Check the surface (<tt>adv_analyze_surface</tt>).</li>
 * <li>Check face area.</li>
 * </ul>
 */
	virtual void adv_analyze_face();
	// Do advance analysis of the loops of the body
/**
 * Performs advanced analysis on the loops of the body.
 * <br><br>
 * <b>Role:</b> It checks the loops of the body owned by the aggregate attribute and stores
 * results in individual entity-level attributes.
 * <br><br>
 * The tests performed include:
 * <ul>
 * <li>Is the loop closed?</li>
 * <li>Is the loop orientation correct?</li>
 * <li>Do the loop's coedges have gaps?</li>
 * <li>Does the loop self-intersect?</li>
 * <li>Check for correct parameter range of the coedges.</li>
 * <li>Check that the coedges lie on the face surface.</li>
 * </ul>
 */
	virtual void adv_analyze_loop();
	// Do advance analysis of the shells of the body
/**
 * Performs advanced analysis on the shells of the body.
 * <br><br>
 * <b>Role:</b> It checks the shells of the body owned by the aggregate attribute and stores
 * results in individual entity-level attributes.
 * <br><br>
 * The tests performed include:
 * <ul>
 * <li>Is the shell closed?</li>
 * <li>Check shell orientation.</li>
 * <li>Check if shell represents a single volume.</li>
 * </ul>
 */
	virtual void adv_analyze_shell();
	// Do advance analysis of the lumps of the body
/**
 * Performs advanced analysis on the lumps of the body.
 * <br><br>
 * <b>Role:</b> It checks the lumps of the body owned by the aggregate attribute and stores
 * results in individual entity-level attributes. This checks the shells for closure.
 */
	virtual void adv_analyze_lump();
	// Do advance analysis of the curves of the body
/**
 * Performs advanced analysis on the curves of the body.
 * <br><br>
 * <b>Role:</b> It checks the curves of the body owned by the aggregate attribute and stores
 * results in individual entity-level attributes.
 * <br><br>
 * The tests performed include:
 * <ul>
 * <li>Is the curve continuous?</li>
 * <li>Is the curve degenerate?</li>
 * <li>Is the curve self-intersecting?</li>
 * <li>Is the curve periodic?</li>
 * <li>Is the curve an approximate curve?</li>
 * </ul>
 */
	virtual void adv_analyze_curve();
	// Do advance analysis of the surfaces of the body
/**
 * Performs advanced analysis on the surfaces of the body.
 * <br><br>
 * <b>Role:</b> It checks the surfaces of the body owned by the aggregate attribute and stores
 * results in individual entity-level attributes.
 * <br><br>
 * The tests performed include:
 * <ul>
 * <li>Is the surface continuous?</li>
 * <li>Is the surface degenerate?</li>
 * <li>Is the surface self-intersecting?</li>
 * <li>Is the surface periodic?</li>
 * </ul>
 */
	virtual void adv_analyze_surface();
	// Do advance analysis of the pcurves of the body
/**
 * Performs advanced analysis on the pcurves of the body.
 * <br><br>
 * <b>Role:</b> It checks the pcurves of the body owned by the aggregate attribute and stores
 * results in individual entity-level attributes.
 * <br><br>
 * The tests performed include:
 * <ul>
 * <li>Is the pcurve continuous?</li>
 * <li>Is the pcurve degenerate?</li>
 * <li>Is the pcurve self-intersecting?</li>
 * <li>Does the pcurve lie on the edge?</li>
 * <li>Is the pcurve parameter range correct?</li>
 * </ul>
 */
	virtual void adv_analyze_pcurve();

	// Print the geometry analysis results of the input
	// and the output body.
/**
 * Prints the geometry building results to the specified file.
 * <br><br>
 * @param fp
 * file pointer.
 */
	virtual void print(FILE *fp);

	// Print results of analysis
/**
 * Prints the results of the analyze stage of the geometry building phase to the specified file.
 * <br><br>
 * @param fp
 * file pointer.
 */
	virtual void print_analyze(FILE *fp);

	// Print results of calculations
/**
 * Prints the results of the calculate stage of the geometry building phase to the specified file.
 * <br><br>
 * @param fp
 * file pointer.
 */
	virtual void print_calculate(FILE *fp);

	// Print results of fix
/**
 * Prints the results of the fix stage of the geometry building phase to the specified file.
 * <br><br>
 * @param fp
 * file pointer.
 */
	virtual void print_fix(FILE *fp);

	// Print results of check
/**
 * Prints the results of the check stage of the geometry building phase to the specified file.
 * <br><br>
 * @param fp
 * file pointer.
 */
	virtual void print_check(FILE *fp);


	// Print the geometry analysis results of the input
	// and the output body into the string passed
/**
 * Prints the geometry building results to the specified string.
 * <br><br>
 * @param str
 * character string.
 */
	virtual void sprint(char *str);

	// Print results of analysis into the string passed
/**
 * Prints the results of the analyze stage of the geometry building phase to the specified string.
 * <br><br>
 * @param str
 * character string.
 */
	virtual void sprint_analyze(char *str);

	// Print results of calculations into the string passed
/**
 * Prints the results of the calculate stage of the geometry building phase to the specified string.
 * <br><br>
 * @param str
 * character string.
 */
	virtual void sprint_calculate(char *str);

	// Print results of fix into the string passed
/**
 * Prints the results of the fix stage of the geometry building phase to the specified string.
 * <br><br>
 * @param str
 * character string.
 */
	virtual void sprint_fix(char *str);

	// Print results of check into the string passed
/**
 * Prints the results of the check stage of the geometry building phase to the specified string.
 * <br><br>
 * @param str
 * character string.
 */
	virtual void sprint_check(char *str);


	// Get functions for do_geombuild geombuild tol
/**
 * Gets the current value of the geometry building tolerance.
 */
	double	tol()	const { return geombuild_options.geombuild_tol;}
/**
 * Gets the minimum geombuild tolerance.
 */
	double	min_tol()	const { return geombuild_options.min_geombuild_tol;}
/**
 * Returns <tt>FALSE</tt> if the geometry building phase has been turned off; otherwise, returns <tt>TRUE</tt>.
 */
	logical do_geombuild()	const { return geombuild_options.use_geombuild;}
/**
 * Gets the option that denotes if checking edge discontinuity is on or off.
 */
	logical check_discontinuity()	const { return geombuild_options.check_discontinuity;}
/**
 * Returns the tangent tolerance.
 */
	double	tang_tol()	const { return geombuild_options.bhl_tang_tol;}
/**
 * Returns the minimum spline tangent tolerance.
 */
	double min_spline_tang_tol() const { return geombuild_options.hh_min_spline_tang_tol;}
/**
 * Returns the maximum spline tangent tolerance.
 */
	double	max_spline_tang_tol()	const { return geombuild_options.hh_max_spline_tang_tol;}

	// HEAL_TO_TOLERANCE
	// for internal use only
/**
 * @nodoc
 */
	// function to get healing_tolerance from attribute
	double get_gap_tightness_tolerance() const;


	// Set functions for do_geombuild geombuild tol
/**
 * Sets the tolerance used by the geometry building phase.
 * <br><br>
 * @param t
 * tolerance to use (radians).
 */
	void set_tol(double t) {
		backup();
		geombuild_options.geombuild_tol = t;
		if (geombuild_options.min_geombuild_tol > t)
			geombuild_options.min_geombuild_tol = t;
		geombuild_options.set();
	}
/**
 * Sets the minimum geombuild tolerance.
 * <br><br>
 * @param t
 * minimum tolerance (radians).
 */
	void set_min_tol(double t) {
		backup();
		geombuild_options.min_geombuild_tol = t;
		if (geombuild_options.geombuild_tol < t)
			geombuild_options.geombuild_tol = t;
		geombuild_options.set();
	}
/**
 * Sets a flag indicating whether or not the geometry building phase needs to be used.
 * <br><br>
 * <b>Role:</b> If it is <tt>TRUE</tt>, the geometry building phase is used; otherwise, it is not used.
 * <br><br>
 * @param use
 * use this phase flag.
 */
	void set_do_geombuild(logical use)	{
		backup();
		geombuild_options.use_geombuild = use;
		geombuild_options.set();
	}
/**
 * Sets the option that denotes if checking edge discontinuity is on or off.
 * <br><br>
 * @param val
 * on or off.
 */
	void set_check_discontinuity(logical val) {
		backup();
		geombuild_options.check_discontinuity = val;
	}
/**
 * Sets the tangent tolerance to the specified value.
 * <br><br>
 * @param tan_tol
 * tangent tolerance value to set (radians).
 */
	void set_tang_tol(double tan_tol);
/**
 * Sets the minimum global spline tangent tolerance to the specified value.
 * <br><br>
 * @param tol_val
 * tolerance value to set (radians).
 */
	void set_min_spline_tang_tol(double tol_val);
/**
 * Set the maximum global tolerance value to the input value given.
 * <br><br>
 * @param tol
 * tolerance value to set (radians).
 */
	void set_max_spline_tang_tol(double tol);


	// HEAL_TO_TOLERANCE
	// for internal use only
/**
 * @nodoc
 */
	// function to set healing_tolerance in attribute
	//void set_gap_tightness_tolerance(double gap_tighten_tol);

	// for internal use only
	/**
	* @nodoc
	*/
	// function to set healing_tolerance in attribute
	void set_tighten_gaps_options( tighten_gaps_options* tgopts);
	tighten_gaps_options* get_tighten_gaps_options();

/**
 * @nodoc
 */
	// SXT, 10-Jul-2006, Bug 81621, functions for setting and getting value of flag force_heal
	void set_force_heal(logical force_heal);
	logical get_force_heal();



/**
 * Sets the preprocess stage flag.
 * <br><br>
 * @param state
 * status.
 */
    void set_preprocess_heal_status(MODULE_HEAL_STATUS state);
/**
 * Sets the postprocessor stage flag.
 * <br><br>
 * @param state
 * status.
 */
    void set_postprocess_heal_status(MODULE_HEAL_STATUS state);
/**
 * Gets the value of the flag which denotes the preprocess stage.
 */
	MODULE_HEAL_STATUS preprocess_heal_state() {return m_preprocess_heal_state;}
/**
 * Gets the value of the flag which denotes the postprocess stage.
 */
    MODULE_HEAL_STATUS postprocess_heal_state() {return m_postprocess_heal_state;}

	// RNC 15 July 99 log list functions added.
	// Get/Set functions for logging simplification module changes
/**
 * Toggles module change logging.
 * <br><br>
 * <b>Role:</b> Returns <tt>FALSE</tt> if logging has been turned off; otherwise, returns <tt>TRUE</tt>.
 */
	logical do_geombuild_log() const;
/**
 * Used for logging simplification module changes.
 * <br><br>
 * @param tog_log
 * toggle logging.
 */
	void set_do_geombuild_log(logical tog_log);
/**
 * Initializes the edge tangency information that specifies whether an edge is a tangent edge or not.
 */
	void reset_edges_tangency_details();
/**
 * Finds all the edges in the model and computes the <tt>min_angle</tt> and <tt>max_angle</tt> data for the edge.
 */
	void update_all_edge_data();
/**
 * Computes the rotation tolerance value.
 */
	void compute_analytic_rot_tol();
/**
 * Computes the spline tangent tolerances.
 */
	void compute_spline_tang_tols();
/**
 * Computes the maximum spline tangent tolerance.
 */
	double compute_max_spline_tang_tol();
/**
 * Computes the minimum spline tangent tolerance.
 */
	double compute_min_spline_tang_tol();

	// MA (21 Oct 99) : Function to cleanup pcurves and making approx intcurves
/**
 * Cleans up pcurves and make approximate intcurves.
 */
	void cleanup_pcurves();

	// Attach the aggregate attributes of
	// all the geombuild modules to the owner body.
/**
 * Attaches the aggregate attributes for all the geometry building subphases to the body being healed.
 */
	void attach_all_aggr_attribs();

	/**
	 * @nodoc
	 */
	ATTRIB_FUNCTIONS( ATTRIB_HH_AGGR_GEOMBUILD, HEAL )
#if 0
; // semicolon needed for mkman (doc tool) parsing)
#endif
// Because mkman isn't aware that ATTRIB_FUNCTIONS changes access to "public", we do so
// explicitly here:
public:

};
//----------------------------------------------------------------------------
/*
// For internal use only
*/
/**
 * @nodoc
 */
DECL_HEAL ATTRIB_HH_AGGR_GEOMBUILD*
find_aggr_geombuild( BODY * );
//----------------------------------------------------------------------------
/*
// For internal use only
*/
/**
 * @nodoc
 */
ATTRIB_HH_ENT_GEOMBUILD_BASE*
attach_ent_geombuild_attrib(ENTITY* );
//----------------------------------------------------------------------------
/*
// For internal use only
*/
/**
 * @nodoc
 */
ATTRIB_HH_ENT_GEOMBUILD_BASE* get_ent_geombuild_attrib(ENTITY* );
/** @} */
#endif
