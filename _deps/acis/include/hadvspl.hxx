/*******************************************************************/
/*    Copyright (c) 1989-2020 by Spatial Corp.                     */
/*    All rights reserved.                                         */
/*    Protected by U.S. Patents 5,257,205; 5,351,196; 6,369,815;   */
/*                              5,982,378; 6,462,738; 6,941,251    */
/*    Protected by European Patents 0503642; 69220263.3            */
/*    Protected by Hong Kong Patent 1008101A                       */
/*******************************************************************/

//----------------------------------------------------------------------------
#if !defined ATTRIB_HH_AGGR_GEN_SPLINE_CLASS
#define		 ATTRIB_HH_AGGR_GEN_SPLINE_CLASS
//----------------------------------------------------------------------------
#include "logical.h"
#include "dcl_heal.h"
#include "bhl_stru.hxx"
#include "aggrgbld.hxx"
/**
 * @file hadvspl.hxx
 * @CAA2Level L1
 * @CAA2Usage U1
 * \addtogroup HEALAPIDEP
 *
 * @{
 */
/**
 * @nodoc
 */
ENTITY_IS_PROTOTYPE(ATTRIB_HH_AGGR_GEN_SPLINE, HEAL )
#if 0
; // semicolon needed for mkman (doc tool) parsing
#endif
DECL_HEAL extern int ATTRIB_HH_AGGR_GEN_SPLINE_TYPE;
/**
 * @nodoc
 */
#define ATTRIB_HH_AGGR_GEN_SPLINE_LEVEL \
	(ATTRIB_HH_AGGR_GEOMBUILD_BASE_LEVEL+1)

//----------------------------------------------------------------------------

/**
 * @nodoc
 * <b>Obsolete:</b> This class has been deprecated.
 */
class DECL_HEAL hh_advspl_options {
public:
	logical use_advspl;

	// Constructor
	hh_advspl_options();
};
//----------------------------------------------------------------------------
/**
 * @nodoc
 * <b>Obsolete:</b> This class has been deprecated. For healing, use API interfaces
 * and not Direct Interface.
 * <br><br>
 * Aggregate healing attribute class for the generic spline solver subphase of geometry building.
 * <br>
 * <b>Role:</b> The <tt>ATTRIB_HH_AGGR_GEN_SPLINE</tt> class is attached to the body to be healed. It is
 * used by the generic spline solver subphase of the geometry building healing phase. The generic
 * spline solver attempts to heal generic tangential spline junctions, (e.g., the intersection curve
 * is <i>not</i> an isoparametric curve of both splines in the intersection).
 */
class DECL_HEAL ATTRIB_HH_AGGR_GEN_SPLINE : public ATTRIB_HH_AGGR_GEOMBUILD_BASE {
protected:

	// Struct containing opts for gen-spline module
	hh_advspl_options advspl_opt;

	// Results of gen-spline module
	bhl_advanced_spline_solver_results results;
// STI cpp begin
		logical save_sw;
// STI cpp end

public:
	// Constructor
/**
 * @nodoc
 * <b>Obsolete:</b> This class has been deprecated.
 * C++ initialize constructor requests memory for this object and populates it with the data supplied as arguments.
 * <br><br>
 * <b>Role:</b> Applications should call this constructor only with the overloaded new operator
 * inherited from the <tt>ENTITY</tt> class (for example, <tt>x=new ATTRIB_HH_AGGR_GEN_SPLINE(...)</tt>), because
 * this reserves the memory on the heap, a requirement to support roll back and history management.
 * <br><br>
 * @param b
 * owning body to heal.
 */
/*	ATTRIB_HH_AGGR_GEN_SPLINE( BODY * b = NULL )
		: ATTRIB_HH_AGGR_GEOMBUILD_BASE(b){
// STI cpp begin
	save_sw = FALSE;
// STI cpp begin
	}
*/
	ATTRIB_HH_AGGR_GEN_SPLINE( BODY * b = NULL );
/**
 * Returns <tt>TRUE</tt> if this is pattern compatible.
 */
    virtual logical pattern_compatible() const;
                                    // returns TRUE
	// Results of gen-spline solver
/**
 * Returns the results of the generic spline solver subphase.
 */
	const bhl_advanced_spline_solver_results calculation_results() const {
		return results;
	}

	// Results of gen-spline solver
/**
 * Fills in the calculate results structure.
 */
	bhl_advanced_spline_solver_results * calculation_results_for_change() {
		return &results;
	}

	// analyze and calculate methods
/**
 * Performs the analyze stage of the generic spline solver subphase of geometry building.
 * <br><br>
 * <b>Role:</b> Analyzes the body and compute the best options and tolerances for the generic
 * spline solver subphase.
 */
	virtual void analyze();
/**
 * Performs the calculate stage of the generic spline solver subphase of geometry building.
 * <br><br>
 * <b>Role:</b> Stores all the recommended changes in individual attributes.
 */
	virtual void calculate();

	// options
/**
 * Returns <tt>FALSE</tt> if the generic spline solver subphase has been turned off, otherwise, returns <tt>TRUE</tt>.
 */
	logical do_gen_spline() const { return advspl_opt.use_advspl;}
/**
 * Sets a flag indicating whether or not the generic spline solver subphase needs to be used.
 * <br><br>
 * <b>Role:</b> if the flag is <tt>TRUE</tt>, the generic spline solver subphase is used; otherwise, it is not used.
 * <br><br>
 * @param t
 * use this subphase flag.
 */
	void	set_do_gen_spline(logical t) { backup(); advspl_opt.use_advspl = t;}

	//void init();

	// Print results
/**
 * Prints statistics of the results of the generic spline solver subphase to the specified file.
 * <br><br>
 * @param fp
 * file pointer.
 */
	void print(FILE *fp);
/**
 * Prints statistics of the results of the calculate stage of the generic spline solver subphase to the specified file.
 * <br><br>
 * @param fp
 * file pointer.
 */
	void print_calculate(FILE *fp);
/**
 * Prints statistics of the results of the generic spline solver subphase to the specified string.
 * <br><br>
 * @param str
 * character string.
 */
	void sprint(char *str);
/**
 * Prints statistics of the results of the calculate stage of the generic spline solver subphase to the specified string.
 * <br><br>
 * @param str
 * character string.
 */
	void sprint_calculate(char *str);

	/**
	 * @nodoc
	 */
	ATTRIB_FUNCTIONS( ATTRIB_HH_AGGR_GEN_SPLINE, HEAL )
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
DECL_HEAL ATTRIB_HH_AGGR_GEN_SPLINE* find_aggr_gen_spline( BODY * );

/** @} */
#endif
