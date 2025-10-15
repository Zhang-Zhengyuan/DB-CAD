/* $Id: hsharped.hxx,v 1.21 2002/01/21 17:35:39 jsloan Exp $ */
/*******************************************************************/
/*    Copyright (c) 1989-2020 by Spatial Corp.                     */
/*    All rights reserved.                                         */
/*    Protected by U.S. Patents 5,257,205; 5,351,196; 6,369,815;   */
/*                              5,982,378; 6,462,738; 6,941,251    */
/*    Protected by European Patents 0503642; 69220263.3            */
/*    Protected by Hong Kong Patent 1008101A                       */
/*******************************************************************/

//----------------------------------------------------------------------------
#if !defined ATTRIB_HH_AGGR_SHARP_EDGE_CLASS
#define		 ATTRIB_HH_AGGR_SHARP_EDGE_CLASS
//----------------------------------------------------------------------------
/**
* @file hsharped.hxx
 * @CAA2Level L1
 * @CAA2Usage U1
 * \addtogroup HEALAPIDEP
 *
 * @{
 */
#include	"logical.h"

#include "dcl_heal.h"

#include "bhl_stru2.hxx"
#include "aggrgbld.hxx"
#include "tols2.hxx"
#include "tols.hxx"

//----------------------------------------------------------------------------
/**
 * @nodoc
 */
ENTITY_IS_PROTOTYPE(ATTRIB_HH_AGGR_SHARP_EDGE, HEAL )
#if 0
; // semicolon needed for mkman (doc tool) parsing
#endif
DECL_HEAL extern int ATTRIB_HH_AGGR_SHARP_EDGE_TYPE;
/**
 * @nodoc
 */
#define ATTRIB_HH_AGGR_SHARP_EDGE_LEVEL \
	(ATTRIB_HH_AGGR_GEOMBUILD_BASE_LEVEL+1)

//----------------------------------------------------------------------------

/**
 * @nodoc
 * <b>Obsolete:</b> This class has been deprecated.
 * Struct for holding the sharp edge solver options.
 */
class DECL_HEAL hh_sharped_options {
public:
	logical	fix_geometry;

	hh_sharped_options();
	void set();
	void reset();
};

//----------------------------------------------------------------------------
/**
 * @nodoc
 * <b>Obsolete:</b> This class has been deprecated. For healing, use API interfaces
 * and not Direct Interface.
 * <br><br>
 * Aggregates healing attribute class for the sharp edge solver subphase of geometry building.
 * <br>
 * <b>Role:</b> The <tt>ATTRIB_HH_AGGR_SHARP_EDGE</tt> class is attached to the body to be healed. It is used
 * by the sharp edge solver subphase of the geometry building healing phase. The sharp edge solver
 * attempts to heal all edges and vertices that are shared by surfaces that intersect sharply. This
 * includes nontangential surface junctions.
 */
class DECL_HEAL ATTRIB_HH_AGGR_SHARP_EDGE : public ATTRIB_HH_AGGR_GEOMBUILD_BASE {
protected:

	// Class containing opts sharp-edge solver
	hh_sharped_options				sharped_opt;

	// Struct containing results of sharp-edge solver
	bhl_transversal_solver_results	results;

	bhl_geombld_options opts;

	// Results of analysis done in analyze()
	bhl_anal_geometry_results anal_results;
// STI cpp begin
		logical save_sw;
// STI cpp end

public:
/**
 * C++ initialize constructor requests memory for this object and populates it with the data supplied as arguments.
 * <br><br>
 * <b>Role:</b> Applications should call this constructor only with the overloaded new operator
 * inherited from the <tt>ENTITY</tt> class (for example, <tt>x=new ATTRIB_HH_AGGR_SHARP_EDGE(...)</tt>), because this
 * reserves the memory on the heap, a requirement to support roll back and history management.
 * <br><br>
 * @param b
 * owning body.
 */
/*	ATTRIB_HH_AGGR_SHARP_EDGE( BODY * b = NULL )
		: ATTRIB_HH_AGGR_GEOMBUILD_BASE(b){
	save_sw = FALSE;
	}
*/
	ATTRIB_HH_AGGR_SHARP_EDGE( BODY * b = NULL );
/**
 * Returns <tt>TRUE</tt> if this is pattern compatible.
 */
    virtual logical pattern_compatible() const;
                                    // returns TRUE
	// analyze and calculate methods.
/**
 * Performs the analyze stage of the sharp edge solver subphase of geometry building.
 * <br><br>
 * <b>Role:</b> Analyzes the body and computes the best options and tolerances for the sharp edge
 * solver subphase.
 */
	virtual void analyze();
/**
 * Performs the calculate stage of the sharp edge solver subphase of geometry building.
 * <br><br>
 * <b>Role:</b> Stores all the recommended changes in individual attributes.
 */
	virtual void calculate();
	/*virtual void fix();*/

	// options
/**
 * Returns <tt>FALSE</tt> if the sharp edge solver subphase has been turned off, otherwise, returns <tt>TRUE</tt>.
 */
	logical	do_sharp_edge()	const { return sharped_opt.fix_geometry;}
/**
 * Sets a flag indicating whether or not the sharp edge solver subphase needs to be used.
 * <br><br>
 * <b>Role:</b> If <tt>TRUE</tt>, the sharp edge solver subphase is used; otherwise, it is not used.
 * <br><br>
 * @param l
 * use this subphase flag.
 */
	void set_do_sharp_edge( logical l)	{
		backup();
		sharped_opt.fix_geometry=l;
		sharped_opt.set();
	}

	// Sharp-edge solver results
/**
 * Returns the results of the sharp edge solver subphase.
 */
	const bhl_transversal_solver_results calculation_results() const {
		return results;
	}
/**
 * Returns the current option results.
 */
	bhl_geombld_options * get_current_results(){return &opts;}

	// Input body geometry analysis results
/**
 * Returns the results of geometry analysis (from analyze stage) of the input body (before healing).
 */
	const bhl_anal_geometry_results anal_geom_results() const {
		return anal_results;
	}

	// messages and global data setting.
	//void init();

	// Print results
/**
 * Prints statistics of the results of the sharp edge solver subphase to the specified file.
 * <br><br>
 * @param fp
 * file pointer.
 */
	void print(FILE *fp);
/**
 * Prints statistics of the results of the calculate stage of the sharp edge solver subphase to the specified file.
 * <br><br>
 * @param fp
 * file pointer.
 */
	void print_calculate(FILE *fp);
/**
 * Prints statistics of the results of the sharp edge solver subphase to the specified string.
 * <br><br>
 * @param str
 * character string.
 */
	void sprint(char *str);
/**
 * Prints statistics of the results of the calculate stage of the sharp edge solver subphase to the specified string.
 * <br><br>
 * @param str
 * character string.
 */
	void sprint_calculate(char *str);

	/**
	 * @nodoc
	 */
	ATTRIB_FUNCTIONS( ATTRIB_HH_AGGR_SHARP_EDGE, HEAL )
#if 0
; // semicolon needed for mkman (doc tool) parsing)
#endif
// Because mkman isn't aware that ATTRIB_FUNCTIONS changes access to "public", we do so
// explicitly here:
public:

};
//----------------------------------------------------------------------------
// Miscelleneous.
/*
// For internal use only
*/
/**
 * @nodoc
 */
DECL_HEAL ATTRIB_HH_AGGR_SHARP_EDGE* find_aggr_sharp_edge( BODY * );

/** @} */
#endif
