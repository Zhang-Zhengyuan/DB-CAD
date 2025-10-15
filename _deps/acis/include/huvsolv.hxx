/*******************************************************************/
/*    Copyright (c) 1989-2020 by Spatial Corp.                     */
/*    All rights reserved.                                         */
/*    Protected by U.S. Patents 5,257,205; 5,351,196; 6,369,815;   */
/*                              5,982,378; 6,462,738; 6,941,251    */
/*    Protected by European Patents 0503642; 69220263.3            */
/*    Protected by Hong Kong Patent 1008101A                       */
/*******************************************************************/

//----------------------------------------------------------------------------
#if !defined ATTRIB_HH_AGGR_ISOSPLINE_CLASS
#define		 ATTRIB_HH_AGGR_ISOSPLINE_CLASS
//----------------------------------------------------------------------------
/**
* @file huvsolv.hxx
 * @CAA2Level L1
 * @CAA2Usage U1
 * \addtogroup HEALAPIDEP
 *
 * @{
 */
#include	"logical.h"

#include "dcl_heal.h"
#include "bhl_stru2.hxx"
#include "bhl_stru.hxx"
#include "aggrgbld.hxx"

//----------------------------------------------------------------------------
/**
 * @nodoc
 */
ENTITY_IS_PROTOTYPE(ATTRIB_HH_AGGR_ISOSPLINE, HEAL )
#if 0
; // semicolon needed for mkman (doc tool) parsing
#endif
DECL_HEAL extern int ATTRIB_HH_AGGR_ISOSPLINE_TYPE;
/**
 * @nodoc
 */
#define ATTRIB_HH_AGGR_ISOSPLINE_LEVEL \
	(ATTRIB_HH_AGGR_GEOMBUILD_BASE_LEVEL+1)

//----------------------------------------------------------------------------
// isospline module options.

/**
 * @nodoc
 * <b>Obsolete:</b> This class has been deprecated.
 * Isospline module options.
 */
class DECL_HEAL hh_isospline_options {
public:
//	logical bhl_spline_tgt = TRUE;
	logical use_spline_tgt;
//	double  isospline_tol;
	logical make_isospline_c1;
	logical do_c1_twist_correction;
	hh_isospline_options();
	void set();
	void reset();
};

/**
 * @nodoc
 */
#define hh_c1_ratio_tol 0.3
/**
 * @nodoc
 */
#define hh_c1_direction_tol 0.1


//RNC : 14 Jun 99 bhl_spline_tgt_tol defined in tols.hxx only for multi
//	  threading

//#define bhl_spline_tgt_tol 0.01

class ATTRIB_HH_ENT_ISOSPLINE_EDGE;
//----------------------------------------------------------------------------
/**
 * @nodoc
 * <b>Obsolete:</b> This class has been deprecated. For healing, use API interfaces
 * and not Direct Interface.
 * <br><br>
 * Aggregates healing attribute class for the isospline solver subphase of geometry building.
 * <br>
 * <b>Role:</b> The <tt>ATTRIB_HH_AGGR_ISOSPLINE</tt> class is attached to the body to be healed. It is used
 * by the isospline solver subphase of the geometry building healing phase. The isospline solver
 * attempts to heal all edges shared by tangential isoparametric surfaces (e.g., the intersection
 * curve is an isoparametric curve of both splines in the intersection).
 * <br>
 */
class DECL_HEAL ATTRIB_HH_AGGR_ISOSPLINE : public ATTRIB_HH_AGGR_GEOMBUILD_BASE {
protected:

	// Class containing options for isospline solver
	hh_isospline_options			uvspl_opt;

	// Struct containing results of spline solver
	bhl_spline_solver_results	results;
// STI cpp begin
		logical save_sw;
// STI cpp end

	// Functions for C1 solving
	void analyze_C1();
	void equip_attribs();
	void calculate_C1();
	void classify_C1();
	logical generate_sequence_for_C1();
	void compute_C1_ratios();
	void make_boundary_curves_C1();
	void make_strips_C1();
	void bend_splines_to_strips();
	void set_C1_seq(ENTITY_LIST& );


public:
/**
 * C++ initialize constructor requests memory for this object and populates it with the data supplied as arguments.
 * <br><br>
 * <b>Role:</b> Applications should call this constructor only with the overloaded new operator
 * inherited from the <tt>ENTITY</tt> class (for example, <tt>x=new ATTRIB_HH_AGGR_ISOSPLINE(...)</tt>), because this
 * reserves the memory on the heap, a requirement to support roll back and history management.
 * <br><br>
 * @param b
 * owning body to heal.
 */
	ATTRIB_HH_AGGR_ISOSPLINE( BODY * b = NULL );

/**
 * Returns <tt>TRUE</tt> if this is pattern compatible.
 */
    virtual logical pattern_compatible() const;
                                    // returns TRUE
	// Iso-spline solver results
/**
 * Returns the results of the isospline solver subphase.
 */
	const bhl_spline_solver_results calculation_results() const {
		return results;
	}
	//results for change
/**
 * Fills in the calculate results structure.
 */
	bhl_spline_solver_results* calculation_results_for_change()  {
		return &results;
	}

	// Analyze and calculate methods
/**
 * Performs the analyze stage of the isospline solver subphase of geometry building.
 * <br><br>
 * <b>Role:</b> Analyzes the body and computes the best options and tolerances for the isospline
 * solver subphase.
 */
	virtual void analyze();
/**
 * Performs the calculate stage of the isospline solver subphase of geometry building.
 * <br><br>
 * <b>Role:</b> Stores all the recommended changes in individual attributes.
 */
	virtual void calculate();
	/*virtual void fix();*/

	// options query
	//double	tol() const { return uvspl_opt.isospline_tol;}
/**
 * Returns <tt>FALSE</tt> if the isospline solver subphase has been turned off, otherwise, returns <tt>TRUE</tt>.
 */
	logical do_isospline() const { return  uvspl_opt.use_spline_tgt;}
/**
 * Gets the option to denote if C1 calculations are on or off.
 */
	logical make_c1() const { return uvspl_opt.make_isospline_c1;}
/**
 * Gets the option to denote if twist correction calculation is on or off.
 */
	logical do_twist_correction() const { return uvspl_opt.do_c1_twist_correction;}

	// options set
/*	void set_tol( double t ){
		backup();
		uvspl_opt.isospline_tol = t;
		uvspl_opt.set();
	}
*/
/**
 * Sets a flag indicating whether or not the isospline solver subphase needs to be used.
 * <br><br>
 * <b>Role:</b> If it is <tt>TRUE</tt>, the isospline solver subphase is used; otherwise, it is not used.
 * <br><br>
 * @param l
 * use this subphase flag.
 */
	void set_do_isospline( logical l){
		backup();
		uvspl_opt.use_spline_tgt = l;
		uvspl_opt.set();
	}

	// Set TRUE if isospline junctions are to be made C1
/**
 * Sets the <tt>make_c1</tt> option.
 * <br><br>
 * @param l
 * make_c1.
 */
	void set_make_c1( logical l){
		backup();
		uvspl_opt.make_isospline_c1 = l;
	}

	// Set TRUE if twist correction is to be applied at spline corners.
	// NOTE: Twist correction needs to be applied when both junctions at
	// a spline corner are to be made c1. When one is made C1, its effect
	// goes to the other junction in the form of twist correction.
	// This option can be switched off if the propagative nature of this
	// correction causes distortions.

/**
 * Sets the <tt>twist_correction</tt> option on or off.
 * <br><br>
 * @param l
 * twist correction.
 */
	void set_twist_correction( logical l){
		backup();
		uvspl_opt.make_isospline_c1 = l;
	}

	//void init();

	// Print the results of the iso-spline solver
/**
 * Prints statistics of the results of the isospline solver subphase to the specified file.
 * <br><br>
 * @param fp
 * file pointer.
 */
	void print(FILE *fp);
/**
 * Prints statistics of the results of the calculate stage of the isospline solver subphase to the specified file.
 * <br><br>
 * @param fp
 * file pointer.
 */
	void print_calculate(FILE *fp);
/**
 * Prints the results of the analyze stage of the isospline solver subphase.
 * <br><br>
 * @param fp
 * file pointer.
 */
	void print_analyze(FILE *fp);
/**
 * Prints statistics of the results of the isospline solver subphase to the specified string.
 * <br><br>
 * @param str
 * character string.
 */
	void sprint(char *str);
/**
 * Prints statistics of the results of the calculate stage of the isospline solver subphase to the specified string.
 * <br><br>
 * @param str
 * character string.
 */
	void sprint_calculate(char *str);
/**
 * Prints statistics of the results of the analyze stage of the isospline solver subphase to the specified string.
 * <br><br>
 * @param str
 * character string.
 */
	void sprint_analyze(char *str);

	// Functions for actual Isospline Healing
/**
 * Used for isospline healing.
 * <br><br>
 * <b>Role:</b> Locates inconsistencies in the topology, and should be used before other healing
 * subprocesses.
 */
	void preprocess();
/**
 * Used for isospline healing.
 * <br><br>
 * <b>Role:</b> Places all the vertices in the owner body into the best possible location. Bends
 * the faces to the iso vertices (i.e., vertices of the isospline edges).
 */
	void heal_isospline_vertices();
/**
 * Used for isospline healing.
 * <br><br>
 * <b>Role:</b> Bends the entity faces to the isospline vertices.
 */
	void bend_faces_to_iso_vertices();
/**
 * Used for isospline healing.
 * <br><br>
 * <b>Role:</b> Makes the procedural geometry exact, wherever necessary.
 */
	void fix_procedural_geometry();
/**
 * Attaches the isospline attributes for the isospline solver subphase to the body being healed.
 */
	void attach_isospline_attribs();
/**
 * Used for isospline healing.
 * <br><br>
 * <b>Role:</b> One-by-one, heals all of the isospline edges in the body, using member functions of
 * isospline edge attributes.
 */
	void heal_isospline_edges();
/**
 * Removes the isospline attributes from the entity.
 */
	void detach_isospline_attribs();

	/**
	 * @nodoc
	 */
	ATTRIB_FUNCTIONS( ATTRIB_HH_AGGR_ISOSPLINE, HEAL )
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
DECL_HEAL ATTRIB_HH_AGGR_ISOSPLINE* find_aggr_isospline( BODY * );

/** @} */

#endif
