/* $Id: cvtycalc.hxx,v 1.7 2002/08/09 17:19:00 jeff Exp $ */
/*******************************************************************/
/*    Copyright (c) 1989-2020 by Spatial Corp.                     */
/*    All rights reserved.                                         */
/*    Protected by U.S. Patents 5,257,205; 5,351,196; 6,369,815;   */
/*                              5,982,378; 6,462,738; 6,941,251    */
/*    Protected by European Patents 0503642; 69220263.3            */
/*    Protected by Hong Kong Patent 1008101A                       */
/*******************************************************************/

#ifndef CVTYCALC_H
#define CVTYCALC_H

// Indent style: Acis

// History:
//
// 29-Jun-99 DAP	New code.

#include "acis.hxx"

#include "logical.h"

#include "dcl_intr.h"

#include "interval.hxx"

#include "cvc.hxx"
#include "svc.hxx"

#include "pt_cvty.hxx"
#include "ed_cvty.hxx"

class curve;
class pcurve;
class surface;
class COEDGE;
class EDGE;


// Class for calculating convexity information, either at a single point
// along an edge, or for regions of an edge, or for the edge as a whole.

class DECL_INTR cvty_calculator {
public:
	// Null constructor. Useful for calling the version of eval that
	// gets given the normals etc.

	cvty_calculator();

	// Given all the usual bits and pieces. Either pcurve may be a
	// NULL_REF. The left_cur and right_cur should both be given or both
	// be NULL_REFs. All curves, pcurves are assumed to have the same
	// sense. Surfaces are assumed to have the appropriate outward
	// normal direction. Additionally, cur may be the same as either the
	// left_cur or right_cur (if these were given).

	// Note that for a precise edge, the edge curve would be passed as
	// the first (main) curve the left_cur and right_cur would be passed
	// as NULL_REFs (though pcurves should be supplied where the
	// surfaces are parametric). In a tolerant edge situation, where the
	// track of the curves in the surfaces is different from the main
	// curve, then left_cur and right_cur should be given.

	cvty_calculator(
			curve const &cur,
			SPAinterval const &cur_range,
			pcurve const &left_pcur,		// req'd if left_surf parametric
			surface const &left_surf,
			curve const &left_cur,			// may be NULL_REF
			SPAinterval const &left_cur_range,	// if left_cur_given
			pcurve const &right_pcur,		// req'd if right_surf parametric
			surface const &right_surf,
			curve const &right_cur,			// may be NULL_REF
			SPAinterval const &right_cur_range	// if right_cur given
			);

	// A convenient version that takes just an edge.

	cvty_calculator( EDGE * );

	// Just like the above one, but taking a COEDGE of which the 3D
	// curve gets used as the "first curve" to drive all the angle
	// calculations. We still build all our bits and pieces in the EDGE
	// sense, however.

	cvty_calculator( COEDGE * );

	// Copy constructor and assignment. No one should want these.

	// Destructor.

	~cvty_calculator();

	// Evaluate pt_cvty_info at SPAparameter "param" on "cur". If
	// "use_curvatures" is FALSE, do not attempt to subclassify
	// tangencies (e.g. into tangent_convex etc.). Also the default_tol
	// in the point_convexity_info will not be set, and therefore cannot
	// be used.

	pt_cvty_info eval(
		double param,
		logical use_curvatures = TRUE,
		int side = 0	// just in case limit from above/below needed
		);

	// A version that doesn't need any curves or surfaces, but can still
	// be used if you've got all the necessary numeric values. Obviously
	// if curvatures not given, then tangencies do not get broken down
	// any further, and the default_tol value in the
	// point_convexity_info does not get set.

	pt_cvty_info eval(
		SPAunit_vector const &dir,		// edge direction
		SPAunit_vector const &left_N,	// left surface normal
		SPAunit_vector const &right_N,	// right surface normal
		double const &left_K = *(double const *)NULL_REF,	// left cross curvature
		double const &right_K = *(double const *)NULL_REF	// right cross curvature
		);

	// This works its way along the given edge computing the amalgamated
	// convexity information for the entire given range. The range
	// does not need to be as big as the range given to the initial
	// constructor. This version of the function uses an adaptive
	// sampling technique to try and create reasonably safe bounds for
	// the angle, and it should in general be relatively quick.

	ed_cvty_info eval_adaptive(
		SPAinterval const &range,
		logical use_curvatures = TRUE
		);

	// The same again, but doing it precisely, the hard way. Finding
	// actual extreme of the angle function. On complex edges,
	// particularly ones where the surfaces are waving to and fro a lot
	// you can expect it to be slower than the above version.

	ed_cvty_info eval_precise(
					  SPAinterval const &range,
					  logical use_curvatures = TRUE
					  );

	// The same again, but using n equally spaced points. Use at your
	// peril.

	ed_cvty_info eval_n_pts(
					int n,
					SPAinterval const &range,
					logical use_curvatures = TRUE
					);

	// Return (compute and cache if necessary) the range of the first
	// curve for which it overlaps sensibly with the others. Only
	// significant really for data from tolerant type edges.

	SPAinterval const &common_range();

	// The functions below here are not in principle expected to be used
	// much by application code, however, they are made available in
	// case any other algorithms happen to find them useful.

	// These get set by any evaluation of the curves and surfaces, and
	// are available should anyone want them. We provide: a CVEC on the
	// each curve, and an SVEC on each of the surfaces.

	CVEC &cvec() { return cvec_data; }
	CVEC &left_cvec() { return left_cvec_data; }
	CVEC &right_cvec() { return right_cvec_data; }
	SVEC &left_svec() { return left_svec_data; }
	SVEC &right_svec() { return right_svec_data; }

	// No harm owning up to these either, in case anyone wants to know:

	curve const &cur() const { return *cur_data; }
	curve const &left_cur() const { return *left_cur_data; }
	curve const &right_cur() const { return *right_cur_data; }
	surface const &left_surf() const { return *left_surf_data; }
	surface const &right_surf() const { return *right_surf_data; }

	// Basic geometric evaluator used by the other functions. Evaluates
	// nd_cur derivs for the first cvec, and then at the corresponding
	// points on the left/right surface, evaluates nd_curs derivs for
	// the left and right cvecs (if these curves were specified), and
	// nd_surfs derivs for the surfaces. The return argument indicates
	// whether all the necessary relaxations worked (FALSE if they did
	// not). If TRUE, the cvecs and svecs can be asked explicitly
	// whether they yielded the necessary derivatives.

	logical eval(
	  double param,	// main curve SPAparameter
	  int nd_cur,	// requested number of main curve derivs
	  int nd_curs,	// requested number of surface curve derivs
	  int nd_surfs,	// requested number of surface derivs
	  int side = 0	// just in case limit from above/below needed
	  );

	// Compute up to 2 derivs of angle, left and right normals, and main
	// curve tangent derivs (where nd=0 means angle, normals etc. only).
	// Return what we found.

	int eval(
	 double t,			// main curve SPAparameter
	 double *angle,		// pointer for return of angle and up to 2 derivs
	 SPAvector *left_N,	// pointer for return of left normal, and up to 2 derivs
	 SPAvector *right_N,	// pointer for return of right normal, and up to 2 derivs
	 SPAvector *T,			// pointer for return of curve tange, and up to 2 derivs
	 int nd				// number of derivs required (0 <= nd <=2 )
	 );

	// An amusing member function to calculate the 3-space tolerance
	// between the positions on the surfaces, for the given range of the
	// main curve (which shouldn't really exceed the common_range), and
	// using a point_perp kind of definition for corresponding points.

	double eval_tol( SPAinterval const & );

	// Quick and dirty version of the above, which returns an
	// approximate (upper bound) for the tolerance, based on an adaptive
	// kind of sampling and fudging technique.

	double eval_tol_adaptive( SPAinterval const & );

	// Debug. We print out all the grisly curves, pcurves, and surfaces.

	void debug( FILE * = debug_file_ptr ) const;

private:
	// The given curves and surfaces. Things actually work out easiest
	// if we own a copy of these, which is a bit of a pain, but there
	// you go.

	curve *cur_data, *left_cur_data, *right_cur_data;
	pcurve *left_pcur_data, *right_pcur_data;
	surface *left_surf_data, *right_surf_data;

	// For storing the results of curve and surface evaluations. I
	// thought long and hard before deciding to use CVECs and SVECs, but
	// they do provide a convenient way of caching evaluation results,
	// and work much better at singularities in computing directions,
	// limiting curvatures with the correct sign etc. Also, the
	// algorithms that analyze entire ranges along the edge are likely
	// to be very grateful for them, so it probably makes sense to use
	// them from the ground up.

	CVEC cvec_data, left_cvec_data, right_cvec_data;
	SVEC left_svec_data, right_svec_data;

	// To store the intervals over which the first curve overaps the
	// others (if there are any).

	SPAinterval common_range_data;
};

#endif
