/*******************************************************************/
/*    Copyright (c) 1989-2020 by Spatial Corp.                     */
/*    All rights reserved.                                         */
/*    Protected by U.S. Patents 5,257,205; 5,351,196; 6,369,815;   */
/*                              5,982,378; 6,462,738; 6,941,251    */
/*    Protected by European Patents 0503642; 69220263.3            */
/*    Protected by Hong Kong Patent 1008101A                       */
/*******************************************************************/

//----------------------------------------------------------------------------
#if !defined ATTRIB_HH_AGGR_ANALYTIC_CLASS
#define		 ATTRIB_HH_AGGR_ANALYTIC_CLASS
//----------------------------------------------------------------------------
/**
* @file hanalsol.hxx
 * @CAA2Level L1
 * @CAA2Usage U1
 * \addtogroup HEALAPIDEP
 *
 * @{
 */
#include	"dcl_heal.h"

#include	"option.hxx"

#include	"aggrgbld.hxx"
#include	"bhl_stru.hxx"
#include	"tganasnp.hxx"
#include	"unstvert.hxx"
#include	"hsurf.hxx"
#include	"tols2.hxx"
#include    "tols.hxx"
#include	"glb_grph.hxx"

//----------------------------------------------------------------------------
/**
 * @nodoc
 */
ENTITY_IS_PROTOTYPE(ATTRIB_HH_AGGR_ANALYTIC, HEAL )
#if 0
; // semicolon needed for mkman (doc tool) parsing
#endif

DECL_HEAL extern int ATTRIB_HH_AGGR_ANALYTIC_TYPE;
/**
 * @nodoc
 */
#define ATTRIB_HH_AGGR_ANALYTIC_LEVEL \
	(ATTRIB_HH_AGGR_GEOMBUILD_BASE_LEVEL+1)

//-----------------------------------------------------------------------------

/**
 * @nodoc
 * <b>Obsolete:</b> This class has been deprecated.
 */
class DECL_HEAL hh_anal_solv_options {
public:
	logical bhl_anal_tgt ;
	double bhl_anal_tgt_tol ;
	double bhl_snap_nor_tol ;
	double bhl_scale_tol;

	// Constructor
	hh_anal_solv_options()
	{
		bhl_anal_tgt = TRUE;
		bhl_anal_tgt_tol = 0.01;
		bhl_snap_nor_tol  = 0.001;
		bhl_scale_tol  = 0.01;
	}
};

//-----------------------------------------------------------------------------
// Aggregate representating analytic solver.
/**
 * @nodoc
 * <b>Obsolete:</b> This class has been deprecated. For healing, use API interfaces
 * and not Direct Interface.
 * <br><br>
 * Aggregate healing attribute class for the analytic solver subphase of geometry building.
 * <br>
 * <b>Role:</b> The <tt>ATTRIB_HH_AGGR_ANALYTIC</tt> class is attached to the body to be healed. It is used
 * by the analytic solver subphase of the geometry building healing phase. The analytic solver
 * subphase attempts to heal all edges and vertices shared by analytic surfaces.
 */
class DECL_HEAL ATTRIB_HH_AGGR_ANALYTIC : public ATTRIB_HH_AGGR_GEOMBUILD_BASE {
protected:

		// Struct containing analytic solver results
		bhl_analytic_solver_results results;

		bhl_geombld_options geom_bld_opt;

		// Graph based tool for analytic solver.
		HH_Tangent_Analytic_Snapper snapper;

		// Unstable Vertex Solver
		Unstable_Vertex_Solver unstable_vertex_solver;

		// Global Graph for analytic solver
		HH_GlobalGraph * glob_graph;

		//New unstable vertex solver
		 HH_Unstable_Vertex_Solver uvsolver;

		// RS (Jan 29, 2002) : Remember the value from the option header.
		logical m_use_new_solver;

		// Run coincident solver
		void coincident_solver();

		// Run normal and scale solver.
		void normal_and_scale_solver();

		//clean up the code
		virtual void cleanup();

		// STI cpp begin
			logical save_sw;
		// STI cpp end

	public:
		//tolerances  and options for solver . Only for internal use.
/**
 * Option to denote if the simple snap correction at the end of the analytic solver is on or off.
 */
		logical bhl_snap_check ;
/**
 * Option to denote if the main part of the analytic solver is on/off.
 */
		logical bhl_snap_main ;
/**
 * Number of unstable vertices fixed.
 */
		int bhl_anal_sol_unst_vert;
/**
 * Number of coincident faces unresolved.
 */
		int	bhl_anal_sol_coin_unresolved;
/**
 * Number of analytic tangencies un-resolved.
 */
		int	bhl_anal_sol_tang_unresolved;
/**
 * Number of coincident faces resolved.
 */
		int	bhl_anal_sol_coin_resolved ;
/**
 * Number of analytic tangencies resolved.
 */
		int	bhl_anal_sol_tang_resolved;
/**
 * A flag to denote the current stage of the analytic solver.
 */
		int bhl_anal_solver_stage ;
/**
 * The degree of the analytic tangency graph.
 */
		int bhl_anal_sol_degree ;
/*
// For internal use only
*/
/**
 * @nodoc
 */
		int bhl_anal_sol_resolved_edges ;
/**
 * Option for setting <tt>uv_tan_grph</tt> solver.
 */
		logical bhl_do_uv_tan_graph;	//option for setting uv_tang. grph solver
/**
 * Minimum scale deviation for the analytic solver.
 */
		double bhlmch ;
/**
 * Minimum rotation deviation for the analytic solver.
 */
		double bhlnor ;
/**
 * Minimum translation deviation for the analytic solver.
 */
		double bhlabs ;
//		double bhl_snap_nor_tol ;
/**
 * The maximum tolerance for scaling beyond which no analytic face is scaled.
 */
		double bhl_snap_doub_tol;
/**
 * The maximum tolerance for translation beyond which no analytic face is translated.
 */
		double bhl_snap_pos_tol ;
/**
 * Identifies node of type <tt>HH_Unstable_SurfSnap</tt>.
 */
		HH_SurfSnap_Node_Type HH_Unstable_Snap_Type;
/**
 * Identifies node of type <tt>HH_SurfSnap</tt>.
 */
		HH_SurfSnap_Node_Type HH_SurfSnap_Type;
		//logical bhl_anal_tgt ;
		//double bhl_anal_tgt_tol ;
/**
 * Option to denote if the unstable vertex part of the analytic solver is on or off.
 */
		logical bhl_unstable_vert ;
/**
 * Option to denote if the geometry has worsened.
 */
		int m_analytic_geom_worsened;


	public:
		// Struct containing options and tols for the
		// analytic solver module.
/**
 * A Structure which contains options and tolerances for their analytic solver module.
 */
		hh_anal_solv_options		anal_options;

	public:
/**
 * C++ initialize constructor requests memory for this object and populates it with the data supplied as arguments.
 * <br><br>
 * <b>Role:</b> Applications should call this constructor only with the overloaded new operator
 * inherited from the <tt>ENTITY</tt> class (for example, <tt>x=new ATTRIB_HH_AGGR_ANALYTIC(...)</tt>), because this
 * reserves the memory on the heap, a requirement to support roll back and history management.
 * <br><br>
 * @param b
 * owning body.
 */
		ATTRIB_HH_AGGR_ANALYTIC( BODY * b = NULL );
/**
 * Returns <tt>TRUE</tt> if this is pattern compatible.
 */
        virtual logical pattern_compatible() const;
                                    // returns TRUE
		// analysis and calculate stage.
/**
 * Performs the analyze stage of the analytic solver subphase of geometry building.
 * <br><br>
 * <b>Role:</b> Analyzes the body and computes the best options and tolerances for the analytic solver
 * subphase.
 */
		virtual void	analyze();
/**
 * Performs the calculate stage of the analytic solver subphase of geometry building.
 * <br><br>
 * <b>Role:</b> Stores all the recommended changes in individual attributes.
 * This method finds how each face should be snapped so as to make all the tangencies
 * valid, and stores the results in the attributes.
 */
		virtual void	calculate();

		// Get functions for opts and tols
/**
 * Gets the current value of the analytic solver subphase tolerance.
 */
		double tol() const { return anal_options.bhl_anal_tgt_tol;}
/**
 * Returns <tt>FALSE</tt> if the analytic solver subphase has been turned off, otherwise, returns <tt>TRUE</tt>.
 */
		logical do_analytic() const { return anal_options.bhl_anal_tgt;}
/**
 * Returns the rotation tolerance value.
 */
		double get_rot_tol() const{ return anal_options.bhl_snap_nor_tol ;}
/**
 * Returns the scale tolerance value.
 */
		double get_scale_tol() const{ return anal_options.bhl_scale_tol ;}

		// Set functions for opts and tols
/**
 * Sets the tolerance used by the analytic solver subphase.
 * <br><br>
 * @param t
 * tolerance to use (radians).
 */
		void set_tol(double t) {
			anal_options.bhl_anal_tgt_tol = t;
		}
/**
 * Sets the rotation tolerance to the specified value.
 * <br><br>
 * @param r
 * tolerance value to set (radians).
 */
		void set_rot_tol(double r){
			anal_options.bhl_snap_nor_tol = r;
		}
/**
 * Sets the scale tolerance to the specified value.
 * <br><br>
 * @param s
 * tolerance to use (radians).
 */
		void set_scale_tol(double s){
			anal_options.bhl_scale_tol = s;
		}
/**
 * Sets a flag indicating whether or not the analytic solver subphase needs to be used.
 * <br><br>
 * <b>Role:</b> If <tt>TRUE</tt>, the analytic solver subphase is used; otherwise, it is not used.
 * <br><br>
 * @param use
 * use this subphase flag.
 */
		void set_do_analytic(logical use) {
			anal_options.bhl_anal_tgt = use;
		}
/**
 * Initializes various global variables to zero.
 */
		void	set_globals();
/**
 * Sets the tolerance used by the analytic solver subphase.
 */
		void	set_tolerances();
/*
// For internal use only
*/
/**
 * @nodoc
 */
		void	set_global_graph(HH_GlobalGraph*);
/**
 * Gathers various healing results from globals and options into the results structure.
 * <br><br>
 * @param results
 * results.
 * @param opts
 * options.
 */
		void	fill_results( bhl_analytic_solver_results& results,bhl_geombld_options& opts);
/*
// For internal use only
*/
/**
 * @nodoc
 */
		HH_GlobalGraph * get_global_graph(){
			return glob_graph;
		}

		// Analytic solver results.
/**
 * Returns the results of the analytic solver subphase.
 */
		const bhl_analytic_solver_results & calculation_results() const {
			return results;
		}
			// Analytic solver results.
/**
 * Fills in the calculate results structure.
 */
		bhl_analytic_solver_results * calculation_results_for_change()  {
			return &results;
		}

		// Analytic solver results.
/**
 * Returns analytic solver results.
 */
		bhl_geombld_options * geombld_results_for_change()  {
			return &geom_bld_opt;;
		}


		// Print the results of analytic solver
/**
 * Prints statistics of the results of the analytic solver subphase to the specified string.
 * <br><br>
 * @param str
 * character string.
 */
		void sprint(char *str);
/**
 * Prints statistics of the results of the analytic solver subphase to the specified file.
 * <br><br>
 * @param fp
 * file pointer.
 */
		void print(FILE *fp);
/**
 * Prints statistics of the results of the calculate stage of the analytic solver subphase to the specified file.
 * <br><br>
 * @param fp
 * file pointer.
 */
		void print_calculate(FILE *fp);
/**
 * Prints statistics of the results of the calculate stage of the analytic solver subphase to the specified string.
 * <br><br>
 * @param str
 * character string.
 */
		void sprint_calculate(char *str);
/**
 * Checks analytic tangencies of body.
 * <br><br>
 * <b>Role:</b> Returns <tt>TRUE</tt> if all analytic tangencies are good.
 */
		logical is_analytic_tangency_good();
/**
 * Checks unstable vertices of body.
 * <br><br>
 * <b>Role:</b> Returns <tt>TRUE</tt> if all unstable vertices are good.
 */
		logical is_unstable_vertex_good();
/**
 * Checks the geometry of the body.
 * <br><br>
 * <b>Role:</b> Returns <tt>TRUE</tt> if the geometry has worsened.
 */
		logical is_analytic_geom_worsened();

		// RNC : 16 July 99 function added for log list addition.
/**
 * Allows additions to the log list.
 */
		void log_analytic_details();

		/**
		 * @nodoc
		 */
		ATTRIB_FUNCTIONS( ATTRIB_HH_AGGR_ANALYTIC, HEAL )
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
};

//----------------------------------------------------------------------------
/*
// For internal use only
*/
/**
 * @nodoc
 */
DECL_HEAL ATTRIB_HH_AGGR_ANALYTIC *
find_aggr_analytic( BODY * );

/*
// For internal use only
*/
/**
 * @nodoc
 */
void check_face_modification_and_set_geom(BODY *bf, int);

/** @} */

#endif

