/* $Id: pt_cvty.hxx,v 1.6 2000/12/26 18:42:49 products Exp $ */
/*******************************************************************/
/*    Copyright (c) 1989-2020 by Spatial Corp.                     */
/*    All rights reserved.                                         */
/*    Protected by U.S. Patents 5,257,205; 5,351,196; 6,369,815;   */
/*                              5,982,378; 6,462,738; 6,941,251    */
/*    Protected by European Patents 0503642; 69220263.3            */
/*    Protected by Hong Kong Patent 1008101A                       */
/*******************************************************************/

#ifndef PT_CVTY_H
#define PT_CVTY_H

// Indent style: Acis

// History:
//
// 27-May-08 DM		Added grouping for documentation
// 28-Jun-99 DAP	New code.

#include "logical.h"

#include "dcl_intr.h"

#include "cvxty.hxx"

/**
* @file pt_cvty.hxx
 * @CAA2Level L1
 * @CAA2Usage U1
 */

/** \addtogroup INTRAPICONVEXITY
 *  \brief Declared at <pt_cvty.hxx>, SPAintr
 *  @{
 */

// Class to represent the convexity of a single point along an edge (or
// equivalent), but where we don't necessarily want to commit ourselves
// to a particular angular tolerance at this point. When we do want the
// verdict given a particular tolerance, we can "instantiate" this into
// an instance of the cvty class.

// When instantiated, the convexity values that can be produced for a
// single point are:

// convex - edge is convex at this point
// concave - edge is concave at this point
// tangent_convex - edge is smooth here, but 2nd order terms imply
// 		convex (i.e. both surfaces have convex curvature here)
// tangent_concave - smooth here, 2nd order terms imply concave
// tangent_inflect - smooth here, 2nd order terms imply one surface is
//		convex, the other concave, and curvatures are the same
// tangent_inflect_convex - as above, but the convex surface is more
// 		curved than the concave one
// tangent_inflect_concave - as above, but the concave surface is more
// 		curved
// knife - surfaces antiparallel here, could not tell from curvatures
// 	whether the "knife" is made of material or air
// knife_convex - a "knife" made of material
// knife_concave - a "knife" made of air
/**
 * Returns the convexity of a single point along an edge (or equivalent).
 * <br>
 * <b>Role:</b> When instantiated, the convexity values that can be produced for a single point are:
 * <br><br>
 * <tt>convex</tt> - edge is convex at this point<br>
 * <tt>concave</tt> - edge is concave at this point<br>
 * <tt>tangent_convex</tt> - edge is smooth here, but 2nd order terms imply convex (both surfaces have convex curvature here)<br>
 * <tt>tangent_concave</tt> - smooth here, second order terms imply concave<br>
 * <tt>tangent_inflect</tt> - smooth here, second order terms imply one surface is convex, the other concave, and curvatures are the same<br>
 * <tt>tangent_inflect_convex</tt> - as above, but the convex surface is more curved than the concave one<br>
 * <tt>tangent_inflect_concave</tt> - as above, but the concave surface is more curved<br>
 * <tt>knife</tt> - surfaces antiparallel here, could not tell from curvatures whether the "knife" is made of material or air<br>
 * <tt>knife_convex</tt> - a "knife" made of material<br>
 * <tt>knife_concave</tt> - a "knife" made of air
 * @see cvty
 */
class DECL_INTR pt_cvty_info {
public:
	// Generic constructor.
/**
 * Generic constructor.
 * <br><br>
 * <b>Role:</b> Make an "unset" or "unknown" one. The default argument makes an
 * unset one. Pass <tt>cvty_unknown</tt> to make an unknown one.
 * <br><br>
 * @param ang
 * sine of the angle being returned.
 * @param tangent_cvty
 * convexity.
 * @param default_tol
 * tolerance.
 */
	pt_cvty_info(
		 double ang,
		 const cvty& tangent_cvty,
		 double default_tol
		 );

	// Make an "unset" or "unknown" one. Default arg makes an unset one,
	// pass cvty_unknown to make an unknown one.
/**
 * Default copy constructor.
 * <br><br>
 * @param c
 * convexity.
 */
	pt_cvty_info( int c = cvty_unset );

	// Default copy constructor, assignment operator and destructor are
	// all fine.

	// The angle between surface normals at this point. We use positive
	// to indicate convex, negative to mean concave.

/**
 * The convexity of this point, if using an angle tolerance such that this point is regarded as tangent.
 */
	double angle() const { return angle_data; }

	// The convexity of this point, if we are using an angle tolerance
	// such that we would regard this point as tangent.

/**
 * The convexity of this point, if the angle tolerance is such that this point would be regarded as tangent.
 */
	cvty tangent_convexity() const { return tangent_convexity_data; }

	// Whether unset or unknown. "unset" means just that - that this is
	// not the result of any evaluation, while "unknown" means that an
	// evaluation happened but failed.
/**
 * This is not the result of any evaluation.
 */
	logical unset() const { return angle_data == -99; }
/**
 * An evaluation happened, but failed.
 */
	logical unknown() const { return angle_data == 99; }

	// The actual convexity of this point, as it appears when viewed
	// using the given angle tolerance. tol may be passed as -1, meaning
	// to use the "default tolerance", which is a value derived from the
	// surface curvatures at this point (if default_tol is not set, the
	// result will be "unknown" cvty).
/**
 * The actual convexity of this point.
 * <br><br>
 * <b>Role:</b> The actual convexity of this point, as it appears when viewed
 * using the given angle tolerance. <tt>tol</tt> may be passed as -1, meaning to use
 * the "default tolerance", which is a value derived from the surface curvatures
 * at this point (if <tt>default_tol</tt> is not set, the result will be
 * "unknown" convexity)
 * <br><br>
 * @param tol
 * angle tolerance.
 */
	cvty instantiate( double tol ) const;

	// Test for equality. Just invoke "==" on all the bits.
/**
 * Tests for equality.
 * <br><br>
 * <b>Role:</b> Just invoke "==" on all the bits.
 * <br><br>
 * @param other
 * pointer to what's tested.
 */
	logical operator==( pt_cvty_info const &other ) const;

	// Debug. Prints the "string representation" to the file.
/**
 * Debugs.
 * <br><br>
 * <b>Role:</b> Prints the "string representation" to the file.
 * <br><br>
 * @param fp
 * file to print to.
 */
	void debug( FILE * fp = debug_file_ptr ) const;

	// The readable "string representation" of this thing, in case you
	// want to do other things with it than send it to a file. The
	// passed "str" must be big enough. Returns the given arg for
	// convenience.
/**
 * The readable "string representation" of this thing.
 * <br><br>
 * <b>Role:</b> In case you want to do other things with it than send it to a
 * file. The passed <tt>str</tt> must be big enough. Returns the given argument for convenience.
 * <br><br>
 * @param str
 * string.
 */
	char *string( char *str ) const;

private:
	double angle_data;
	cvty tangent_convexity_data;
	double default_tol_data;
};

/** @} */

#endif
