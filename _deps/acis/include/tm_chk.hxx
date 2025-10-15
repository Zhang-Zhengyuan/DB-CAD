// $Id: tm_chk.hxx,v 1.4 2002/08/09 17:19:04 jeff Exp $
/*******************************************************************/
/*    Copyright (c) 1989-2020 by Spatial Corp.                     */
/*    All rights reserved.                                         */
/*    Protected by U.S. Patents 5,257,205; 5,351,196; 6,369,815;   */
/*                              5,982,378; 6,462,738; 6,941,251    */
/*    Protected by European Patents 0503642; 69220263.3            */
/*    Protected by Hong Kong Patent 1008101A                       */
/*******************************************************************/

#ifndef TM_CHK_HEADER
#define TM_CHK_HEADER

// This file defines all the functions which can be used to check
// tolerant edges and coedges. The functions all return tm_chk_info
// (see tm_chk_info.hxx) objects to indicate any problems. None of these
// functions consume or alter their arguments. The head of the returned
// list always emerges with a use count of 0, so it could be passed
// straight into a sys_error. (Any subsequent list elements emerge with
// a use count of 1, by virtue of being referenced by the previous
// list item.) If it was wanted to pass a particular error somewhere in
// the list to sys_error, other than the first, more care would have to
// be taken, for example:
#if 0
tm_chk_info *all_errors = tm_check_all( my_edge );
if ( all_errors ) all_errors->add(); // now all have use count 1
tm_chk_info *error_to_pass_on = (tm_chk_info *)
                find_error_list_info( my_error_type_id );
if ( error_to_pass_on ) // up its use count as the "global error info"
       set_global_error_info( error_to_pass_on );
if ( error_to_pass_on ) // do this to pass on *only* this one error
       error_to_pass_on->set_next( NULL );
if ( all_errors ) // clean up all the rest of the list
       all_errors->remove();
// Now call sys_error, this won't up the use count again
sys_error( MY_ERROR_NUMBER, error_to_pass_on );
#endif

// At the top of this file are the functions which perform each of the
// individual basic tests. These can be given topology, but there are
// also versions that take raw geometry as arguments, thereby allowing
// applications to check prospective geometry before making tolerant
// edges and coedges out of them. A complete set of checks can be
// performed by calling each test in the order listed, and where a
// coedge is involved, for each coedge.

// At the bottom of the file some functions that conveniently group some
// or all of these tests together in the correct order. For the function
// that performs *all* the tests, the source code is given as an example
// of how to use the basic tests.

class EDGE;
class COEDGE;
class TEDGE;
class TCOEDGE;
class tm_chk_info;
class curve;
class SPAinterval;
class surface;

#include "dcl_intr.h"

#include "bs2curve.hxx"


// So first, the basic tests.

// Every basic check has a function that performs it. For convenience a
// function is provided that performs the check given just the
// appropriate piece of topology, but there is also an overloaded
// function for each that takes just raw geometry. None of these checks
// actually perform the preceding checks that would be required to
// ensure it is sensible to continue - it is assumed the caller will
// take care of this.

// Each of these functions will only ever return NULL (if all is well)
// or an error object of type given in the function name (i.e. the xxx
// bit in tm_check_xxx).


// This first test is the only exception in that there is no "geometry"
// version. It merely checks for the topology being basically sensible
// (i.e. no missing bits, or non-tolerant bits).

DECL_INTR tm_chk_info *tm_check_tm_bad_topology( EDGE * );

// Check the edge curve for being non-G1.

DECL_INTR tm_chk_info *tm_check_tedge_crv_non_G1( TEDGE * );
DECL_INTR tm_chk_info *tm_check_tedge_crv_non_G1(
							 curve const &edge_curve,
							 SPAinterval const &edge_range
							 );

// Run the regular curve checks over the edge curve.

DECL_INTR tm_chk_info *tm_check_tedge_bad_crv( TEDGE * );
DECL_INTR tm_chk_info *tm_check_tedge_bad_crv(
						  curve const &edge_curve,
						  SPAinterval const &edge_range
						  );

// Check the coedge SPAparameter-space curve is G1 in SPAparameter-space.

DECL_INTR tm_chk_info *tm_check_tcoedge_bs2_non_G1( TCOEDGE * );
DECL_INTR tm_chk_info *tm_check_tcoedge_bs2_non_G1(
							   bs2_curve coedge_bs2,
							   SPAinterval const &bs2_range
							   );

// Check the coedge SPAparameter-space curve lies in its surface.

DECL_INTR tm_chk_info *tm_check_tcoedge_bs2_outside_sf( TCOEDGE * );
DECL_INTR tm_chk_info *tm_check_tcoedge_bs2_outside_sf(
								   bs2_curve coedge_bs2,
								   SPAinterval const &bs2_range,
								   surface const &coedge_srf
								   );

// Check the coedge 3D curve is G1 everywhere. If a curve and range are
// passed directly, then these should be passed in the EDGE sense.

DECL_INTR tm_chk_info *tm_check_tcoedge_crv_non_G1( TCOEDGE * );
DECL_INTR tm_chk_info *tm_check_tcoedge_crv_non_G1(
							   curve const &coedge_curve,
							   SPAinterval const &coedge_range
							   );

// Check the coedge 3D curve passes the regular curve checks. Again, if
// a curve and range are passed directly, then they should have the EDGE
// sense.

DECL_INTR tm_chk_info *tm_check_tcoedge_bad_crv( TCOEDGE * );
DECL_INTR tm_chk_info *tm_check_tcoedge_bad_crv(
							curve const &coedge_curve,
							SPAinterval const &coedge_range
							);

// Check that the common range can be computed for these edge and curve
// curves. The coedge_curve and range should be passed in EDGE sense too
// so as to match the edge. The latter function additionally returns 2
// ranges, one for each of the edge and coedge, which are the ranges
// over which the 2 curves overlap sensibly.

DECL_INTR tm_chk_info *tm_check_tedge_tcoedge_ranges( TCOEDGE * );
DECL_INTR tm_chk_info *tm_check_tedge_tcoedge_ranges(
								 curve const &edge_curve,
								 SPAinterval const &edge_range,
								 curve const &coedge_curve,
								 SPAinterval const &coedge_range,
								 SPAinterval &common_edge_range,
								 SPAinterval &common_coedge_range
								 );

// Check the edge and coedge curves are everywhere sympathetic. The
// coedge curve and range should have the same sense as the EDGE.

DECL_INTR tm_chk_info *tm_check_tedge_tcoedge_bad_geom( TCOEDGE * );
DECL_INTR tm_chk_info *tm_check_tedge_tcoedge_bad_geom(
								  curve const &edge_curve,
								  SPAinterval const &edge_range,
								  curve const &coedge_curve,
								  SPAinterval const &coedge_range
								  );

// Check the edge curve does not have local self-intersections at the
// edge tolerance.

DECL_INTR tm_chk_info *tm_check_tedge_local_self_int( TEDGE * );
DECL_INTR tm_chk_info *tm_check_tedge_local_self_int(
									 curve const &edge_curve,
									 SPAinterval const &edge_range,
									 double tol
									 );

// Check the edge curve does not have remote self-intersections at the
// edge tolerance. (Note that "tol" represents the edge tolerance, so
// the self-intersection test looks for self-intersections to within
// *twice* this value.)

DECL_INTR tm_chk_info *tm_check_tedge_remote_self_int( TEDGE * );
DECL_INTR tm_chk_info *tm_check_tedge_remote_self_int(
									 curve const &edge_curve,
									 SPAinterval const &edge_range,
									 double tol
									 );

// Check the given coedge does not stray further than the edge tolerance
// away from the edge curve. Coedge curve and range should be passed
// with the same sense as the EDGE.

DECL_INTR tm_chk_info *tm_check_tedge_tcoedge_bad_tol( TCOEDGE * );
DECL_INTR tm_chk_info *tm_check_tedge_tcoedge_bad_tol(
								  curve const &edge_curve,
								  SPAinterval const &edge_range,
								  double tol,
								  curve const &coedge_curve,
								  SPAinterval const &coedge_range
								  );


// Now some convenient functions that group some or all of these tests
// together:

// Do all the checks that pertain to the edge and not the coedges. This
// performs the tedge_crv_non_G1 and tedge_bad_crv checks. Assumes any
// previously necessary checks have been performed (tm_bad_topology).

DECL_INTR tm_chk_info *tm_check_tedge( TEDGE * );

// Do all the checks that pertain to a given coedge, namely
// tcoedge_bs2_non_G1, tcoedge_bs2_outside_sf, tcoedge_crv_non_G1 and
// tcoedge_bad_crv. Assumes any previously necessary checks have been
// performed (tm_bad_topology).

DECL_INTR tm_chk_info *tm_check_tcoedge( TCOEDGE * );

// Do all the extra checks for this coedge given that we know the edge
// to have passed its checks (tm_check_tedge). These tests are
// tedge_tcoedge_ranges and tedge_tcoedge_bad_geom.

DECL_INTR tm_chk_info *tm_check_tedge_tcoedge( TCOEDGE * );

// Check for local and global self-intersections. Assumes all the
// precondition checks (tm_check_tedge, tm_check_tcoedge,
// tm_check_tedge_tcoedge) are ok. This is the first check that needs
// the edge tolerance.

DECL_INTR tm_chk_info *tm_check_tedge_self_int( TEDGE * );

// Do the tolerance check tm_check_tedge_tcoedge_bad_tol for every
// coedge on the edge. It is provided for convenience but does not form
// any of the other precondition checks (tm_check_tedge,
// tm_check_tcoedge, tm_check tedge_tcoedge).

DECL_INTR tm_chk_info *tm_check_tedge_tol( TEDGE * );

// This function performs *all* the basic tests in the correct order.
// In general, once some of the earlier tests, e.g. tm_check_tedge or
// tm_check_tcoedge, have failed it will tend to skip the later ones.
// The tedge system is only guaranteed to be at all legal if NULL is
// returned.

DECL_INTR tm_chk_info *tm_check_all( EDGE * );


#endif
