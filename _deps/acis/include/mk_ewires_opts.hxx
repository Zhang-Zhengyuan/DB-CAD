/* ORIGINAL: acis18 */
/*******************************************************************/
/*    Copyright (c) 1989-2020 by Spatial Corp.                     */
/*    All rights reserved.                                         */
/*    Protected by U.S. Patents 5,257,205; 5,351,196; 6,369,815;   */
/*                              5,982,378; 6,462,738; 6,941,251    */
/*    Protected by European Patents 0503642; 69220263.3            */
/*    Protected by Hong Kong Patent 1008101A                       */
/*******************************************************************/
#ifndef make_ewires_opts_hxx
#define make_ewires_opts_hxx
#include "mmgr.hxx"
#include "dcl_cstr.h"
#include "tol_param.hxx"
#include "logical_state.hxx"
#include "logical.h"

/**
* @file mk_ewires_opts.hxx
 * @CAA2Level L1
 * @CAA2Usage U1
 */

/**
 * Options for <tt>make_ewires</tt>.
 * <br>
 * <b>Role:</b> 
 * Allows you to set and get the tolerance used when judging if start/end vertices
 * of edges are coincident.
 * <br><br>
 * By default, the tolerance for judging coincidence tracks SPAresabs.
 * If the <tt>set_coin_tol(tolerance)</tt> is used, the tolerance you provide 
 * is returned regardless of changes in SPAresabs. To return
 * to the default behavior, use <tt>set_coin_tol()</tt>.
 * If there are tolerant vertices (TVERTEXs) on any input edges, the tolerance used for <tt>set_coin_tol(tolerance)</tt> 
 * should be at least as big as the largest TVERTEX tolerance.
 **/
class  DECL_CSTR make_ewires_opts: public ACIS_OBJECT
{
private:
	/**
	* @nodoc
	*/
	tolerance_param coin_tol;
	logical_state single_ordered_wire;

public:
	make_ewires_opts();
	/**
	* <tt>set_edge_coin_tol</tt> called with no arguments returns the tolerance to its default state.
	*/
	void set_coin_tol();
	/**
	* <tt>set_edge_coin_tol</tt> sets the tolerance used when judging if start/end
	* vertices are coincident.
	* <br><br>
	* @param tolerance
    * Specifies the tolerance.
	*/
	void set_coin_tol(double tolerance);
	/**
	* <tt>get_edge_coin_tol</tt> retrieves the tolerance used when judging if start/end
	* vertices are coincident. Do not use <tt>get_edge_coin_tol</tt> to modify
	* tolerance.
	*/
	double get_coin_tol() const;
	/**
	* @nodoc
	*/
	// for internal use only
	// Contains logic for sorting out bad user provided values of tolerance
	// if tolerance provided is less than current SPAresabs, it returns SPAresabs.
	double get_coin_tol_internal() const;
	/**
	* returns a logical indicating whether input is expected to be a single ordered wire.
	**/
	logical get_single_ordered_wire();
	/**
	* set the logical indicating whether the input is expected to be a single ordered wire.
	**/
	void set_single_ordered_wire(logical);
};
#endif 
