/*******************************************************************/
/*    Copyright (c) 1989-2020 by Spatial Corp.                     */
/*    All rights reserved.                                         */
/*    Protected by U.S. Patents 5,257,205; 5,351,196; 6,369,815;   */
/*                              5,982,378; 6,462,738; 6,941,251    */
/*    Protected by European Patents 0503642; 69220263.3            */
/*    Protected by Hong Kong Patent 1008101A                       */
/*******************************************************************/

//----------------------------------------------------------------------------
#if !defined ATTRIB_HH_AGGR_WRAPUP_CLASS
#define		 ATTRIB_HH_AGGR_WRAPUP_CLASS
//----------------------------------------------------------------------------
/**
* @file hsecndry.hxx
 * @CAA2Level L1
 * @CAA2Usage U1
 * \addtogroup HEALAPIDEP
 *
 * @{
 */
#include	"logical.h"

#include "dcl_heal.h"
#include "tols2.hxx"
#include "tols.hxx"
#include "bhl_stru2.hxx"
#include "bhl_stru.hxx"
#include "aggrgbld.hxx"

//----------------------------------------------------------------------------
/**
 * @nodoc
 */
ENTITY_IS_PROTOTYPE(ATTRIB_HH_AGGR_WRAPUP, HEAL )
#if 0
; // semicolon needed for mkman (doc tool) parsing
#endif
DECL_HEAL extern int ATTRIB_HH_AGGR_WRAPUP_TYPE;
/**
 * @nodoc
 */
#define ATTRIB_HH_AGGR_WRAPUP_LEVEL \
	(ATTRIB_HH_AGGR_GEOMBUILD_BASE_LEVEL+1)

//----------------------------------------------------------------------------

/**
 * @nodoc
 * <b>Obsolete:</b> This class has been deprecated.
 */
// Class containing opts for the wrapup module
class DECL_HEAL hh_secondary_solver_options {
public:
	logical use_secondary_solver;
	logical bhl_wrapup ;

	hh_secondary_solver_options () {
		use_secondary_solver = TRUE;
		bhl_wrapup = TRUE;
	}
/*
// For internal use only
*/
/**
 * @nodoc
 */
	void reset() { use_secondary_solver = bhl_wrapup ;}
/*
// For internal use only
*/
/**
 * @nodoc
 */
	void set() {bhl_wrapup = use_secondary_solver;}
};
//----------------------------------------------------------------------------

/**
 * @nodoc
 * <b>Obsolete:</b> This class has been deprecated. For healing, use API interfaces
 * and not Direct Interface.
 * <br><br>
 * Aggregate healing attribute class for the wrap-up subphase of geometry building.
 * <br>
 * <b>Role:</b> The <tt>ATTRIB_HH_AGGR_WRAPUP</tt> class is attached to the body to be healed. Is is used
 * by the wrap-up subphase of the geometry building healing phase. The wrap-up subphase recomputes
 * the pcurve geometry of unhealed coedges (also referred to as "secondary geometries"). It trims
 * curves, fixes pcurves, and orients normals.
 */
class DECL_HEAL ATTRIB_HH_AGGR_WRAPUP : public ATTRIB_HH_AGGR_GEOMBUILD_BASE {
protected:

	// struct containing opts for the wrapup module
	hh_secondary_solver_options options;

	// Results of wrapup module
	bhl_wrapup_results results;
// STI cpp begin
		logical save_sw;
// STI cpp end

	// New option added for setting enums in edges to tangent

	logical fix_enum;

public:
/**
 * C++ initialize constructor requests memory for this object and populates it with the data supplied as arguments.
 * <br><br>
 * <b>Role:</b> Applications should call this constructor only with the overloaded new operator
 * inherited from the <tt>ENTITY</tt> class (for example, <tt>x=new ATTRIB_HH_AGGR_WRAPUP(...)</tt>), because this
 * reserves the memory on the heap, a requirement to support roll back and history management.
 * <br><br>
 * @param b
 * owning body.
 */

	ATTRIB_HH_AGGR_WRAPUP( BODY * b = NULL );

/**
 * Returns <tt>TRUE</tt> if this is pattern compatible.
 */
    virtual logical pattern_compatible() const;
                                    // returns TRUE
	// Results of gen-spline solver
/**
 * Returns the results of the wrap-up subphase.
 */
	const bhl_wrapup_results calculation_results() const {
		return results;
	}

	// Results of gen-spline solver
/**
 * Gets the structure which contains the results for wrapup, such as the number of pcurves calculated etc.
 */
	bhl_wrapup_results& results_for_change() {
		return results;
	}
/**
 * Returns the number of bad coedges.
 */
	int num_bad_coedges();
/**
 * Returns <tt>FALSE</tt> if the <tt>fix_enum</tt> option has been turned off, otherwise, returns <tt>TRUE</tt>.
 */
	logical do_enum() const  {
		return fix_enum;
	}

	// analyze and calculate methods
/**
 * Performs the analyze stage of the wrap-up subphase of geometry building.
 * <br><br>
 * <b>Role:</b> Analyzes the body and computes the best options for the wrap-up subphase.
 */
	virtual void analyze();
/**
 * Performs the calculate stage of the wrap-up subphase of geometry building.
 * <br><br>
 * <b>Role:</b> Stores all the recommended changes in individual attributes.
 */
	virtual void calculate();
	/*virtual void fix();*/

	//void init();

	// Print results
/**
 * Prints statistics of the results of the wrap-up subphase to the specified file.
 * <br><br>
 * @param fp
 * file pointer.
 */
	void print(FILE *fp);
/**
 * Prints statistics of the results of the analyze stage of the wrap-up subphase to the specified file.
 * <br><br>
 * @param fp
 * file pointer.
 */
	void print_analyze(FILE *fp);
/**
 * Prints statistics of the results of the calculate stage of the wrap-up subphase to the specified file.
 * <br><br>
 * @param fp
 * file pointer.
 */
	void print_calculate(FILE *fp);
/**
 * Prints statistics of the results of the wrap-up subphase to the specified string.
 * <br><br>
 * @param str
 * character string.
 */
	void sprint(char *str);
/**
 * Prints statistics of the results of the analyze stage of the wrap-up subphase to the specified string.
 * <br><br>
 * @param str
 * character string.
 */
	void sprint_analyze(char *str);
/**
 * Prints statistics of the results of the calculate stage of the wrap-up subphase to the specified string.
 * <br><br>
 * @param str
 * character string.
 */
	void sprint_calculate(char *str);

	// options
/**
 * Returns <tt>FALSE</tt> if the wrap-up subphase has been turned off, otherwise, returns <tt>TRUE</tt>.
 */
	logical do_wrapup()	{ return options.use_secondary_solver;}

/**
 * Sets a flag indicating whether or not the wrap-up subphase needs to be used.
 * <br><br>
 * <b>Role:</b> If <tt>TRUE</tt>, the wrap-up subphase is used; otherwise, it is not used.
 * <br><br>
 * @param use
 * new flag value.
 */
	void set_do_wrapup(logical use)	{
		backup();
		options.use_secondary_solver = use;
		options.set();
	}
/**
 * Sets a flag indicating whether or not the <tt>fix_enum</tt> option needs to be used.
 * <br><br>
 * <b>Role:</b> If it is <tt>TRUE</tt>, the <tt>fix_enum</tt> option is used; otherwise, it is not used.
 * <br><br>
 * @param use
 * new flag value.
 */
	void set_enum(logical use) {
		backup();
		fix_enum = use;
	}

	/**
	 * @nodoc
	 */
	ATTRIB_FUNCTIONS( ATTRIB_HH_AGGR_WRAPUP, HEAL )
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
DECL_HEAL ATTRIB_HH_AGGR_WRAPUP* find_aggr_wrapup( BODY * );

/** @} */
#endif
