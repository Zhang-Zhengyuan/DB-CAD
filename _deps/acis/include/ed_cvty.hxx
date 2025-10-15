/* $Id: ed_cvty.hxx,v 1.7 2002/08/09 17:19:00 jeff Exp $ */
/*******************************************************************/
/*    Copyright (c) 1989-2020 by Spatial Corp.                     */
/*    All rights reserved.                                         */
/*    Protected by U.S. Patents 5,257,205; 5,351,196; 6,369,815;   */
/*                              5,982,378; 6,462,738; 6,941,251    */
/*    Protected by European Patents 0503642; 69220263.3            */
/*    Protected by Hong Kong Patent 1008101A                       */
/*******************************************************************/

#ifndef ED_CVTY_H
#define ED_CVTY_H

// Indent style: Acis

// History:
//
// 28-May-08 DM     Documentation
// 28-Jun-99 DAP	New code.

#include "logical.h"

#include "interval.hxx"

#include "dcl_intr.h"

#include "cvxty.hxx"

#include "debugmsc.hxx"

class pt_cvty_info;


// Class to represent the convexity of an edge (or equivalent), but
// where we don't necessarily want to commit ourselves to a particular
// angular tolerance at this point. When we do want the verdict given a
// particular tolerance, we can "instantiate" this into an instance of
// the cvty class.

// When instantiated it can yield the following values:

// All the same ones as for points (see pt_cvty.hxx), meaning that every
// point on the edge has those same properties. Additionally we may get:

// mixed - edge is both convex and concave in places (yikes!)
// convex_mixed - the edge is everywhere convex or smooth (according to
// 		the first order terms)
// concave_mixed - concave or smooth (to first order) everywhere
// tangent_mixed - tangent everywhere, and second order terms fluctuate
// 		between convex and concave
// tangent_convex_mixed - tangent everywhere, but 2nd order terms imply
// 		convex or flat everywhere
// tangent_concave_mixed - as above, but concave
// tangent_inflect_mixed - tangent inflection, but varied between
// 		tangent_inflect_convex and tangent_inflect_concave
// tangent_inflect_convex_mixed - everywhere tangent_inflect_convex or
// 		just tangent_inflect
// tangent_inflect_concave_mixed - as above, but concave
// knife_mixed - mixture of knife_convex and knife_concave
// knife_convex_mixed - mixture of knife_convex and just knife
// knife_concave_mixed - mixture of knife_concave and just knife

/**
* @file ed_cvty.hxx
 * @CAA2Level L1
 * @CAA2Usage U1
 */

/** \addtogroup INTRAPICONVEXITY
 *  \brief Declared at <ed_cvty.hxx>, SPAintr
 *  @{
 */

/**
 * Returns the convexity of an edge (or equivalent).
 * <br><br>
 * <b>Role:</b> This class represents the convexity of an edge (or equivalent),
 * but where you don't necessarily want to commit to a particular angular
 * tolerance. When you do want the verdict given a particular tolerance,
 * you can "instantiate" this into an instance of the cvty class.
 * <br><br>
 * When instantiated it can yield the following values:
 * <br><br>
 * All the same ones as for points (see <tt>pt_cvty.hxx</tt>), meaning that every point
 * on the edge has those same properties. Additionally we may get:
 * <br><br>
 * <tt>mixed</tt> - edge is both convex and concave in places<br>
 * <tt>convex_mixed</tt> - the edge is everywhere convex or smooth (according to the first order terms)<br>
 * <tt>concave_mixed</tt> - concave or smooth (to first order) everywhere<br>
 * <tt>tangent_mixed</tt> - tangent everywhere, and second order terms fluctuate between convex and concave<br>
 * <tt>tangent_convex_mixed</tt> - tangent everywhere, but second order terms imply convex or flat everywhere<br>
 * <tt>tangent_concave_mixed</tt> - as above, but concave<br>
 * <tt>tangent_inflect_mixed</tt> - tangent inflection, but varied between <tt>tangent_inflect_convex</tt> and <tt>tangent_inflect_concave</tt><br>
 * <tt>tangent_inflect_convex_mixed</tt> - everywhere <tt>tangent_inflect_convex</tt> or just <tt>tangent_inflect</tt><br>
 * <tt>tangent_inflect_concave_mixed</tt> - as above, but concave<br>
 * <tt>knife_mixed</tt> - mixture of <tt>knife_convex</tt> and <tt>knife_concave</tt><br>
 * <tt>knife_convex_mixed</tt> - mixture of <tt>knife_convex</tt> and just knife<br>
 * <tt>knife_concave_mixed</tt> - mixture of <tt>knife_concave</tt> and just knife
 * @see cvty, SPAinterval
 */
class DECL_INTR ed_cvty_info {
public:
	// Generic constructor.
/**
 * Generic constructor.<br><br>
 * @param angs
 * angles.
 * @param tangent_cvty
 * tangent convexity.
 */
	ed_cvty_info(
		 SPAinterval const &angs,
		 const cvty& tangent_cvty
		 );

	// Handy default constructor to make an "unset" ed_cvty_info. Pass
	// cvty_unknown to make an "unknown" one.
/**
 * Default constructor to make an "unset" <tt>ed_cvty_info</tt>.
 * <br><br>
 * <b>Role:</b> Pass <tt>cvty_unknown</tt> to make an "unknown" one.<br><br>
 * @param c
 * convexity.
 */
	ed_cvty_info( int c = cvty_unset );

	// Make an ed_cvty_info for an infinitessimal portion of edge around
	// the given point, using that point's pt_cvty_info.
/**
 * Make an <tt>ed_cvty_info</tt> for an infinitesimal portion of edge around the given point, using that point's <tt>pt_cvty_info</tt>.<br><br>
 * @param pt
 * point to use.
 */
	ed_cvty_info( pt_cvty_info const & pt);

	// Default copy constructor, assignment operator and destructor are
	// all fine.

	// By convention we use an empty SPAinterval to indicate an "unset"
	// ed_cvty_info, an infinite one to indicate an "unknown" one. As
	// usual, an evaluation can never result in an "unset" ed_cvty_info,
	// but if it fails completely it can result in an "unknown" one.

/**
 * By convention, an empty interval indicates an "unset" <tt>ed_cvty_info</tt>.
 * <br><br>
 * <b>Role:</b> An evaluation can never result in an "unset" <tt>ed_cvty_info</tt>, but
 * if it fails completely it can result in an "unknown" one.
 */
	logical unset() const { return angles_data.empty(); }
/**
 * By convention an infinite interval indicates an "unknown" <tt>ed_cvty_info</tt>.
 * <br><br>
 * <b>Role</b>: An evaluation can never result in an "unset" <tt>ed_cvty_info</tt>, but
 * if it fails completely it can result in an "unknown" one.
 */
	logical unknown() const { return angles_data.infinite(); }

	// The maximum and mimum angles between surface normals along this
	// edge. We use positive to indicate convex, negative to mean
	// concave.

/**
 * The maximum and minimum sine of the angles between surface normals along this edge. For example, 
 * the edge of a cube returns the pair 1,1, and a tangent edge returns the pair 0,0. 
 * <br><br>
 * <b>Role:</b> Positive indicates convex and negative indicates concave.
 */
	SPAinterval const &angles() const { return angles_data; }

	// The convexity of this edge, if we are using an angle tolerance
	// such that we would regard this whole edge as tangent.
/**
 * The convexity of this edge, if the angle tolerance is such that the whole edge would be regarded as tangent.
 */
	cvty tangent_convexity() const { return tangent_convexity_data; }

	// The actual convexity of this edge, as it appears when viewed
	// using the given angle tolerance.
/**
 * The actual convexity of this edge, as it appears when viewed using the given angle tolerance.<br><br>
 * @param tol
 * angle tolerance.
 */
	cvty instantiate( double tol ) const;

	// Merge the information currently in this object with information
	// about a other points on the edge. Returns itself.

/**
 * Merges the information present currently in this object with information about other points on the edge.
 * <br><br>
 * <b>Role:</b> Returns itself.<br><br>
 * @param eci
 * info to merge.
 */
	ed_cvty_info &merge( ed_cvty_info const &eci );

	// Test for equality. Just invoke "==" on all the bits.

/**
 * Test for equality.
 * <br><br>
 * <b>Role:</b> Just invoke "==" on all the bits.<br><br>
 * @param other
 * what to test.
 */
	logical operator==( ed_cvty_info const &other ) const;

	// Debug. Prints the "string representation" to the file.

/**
 * Prints out debug data on all the curves, pcurves, and surfaces to the specified file.
 * <br><br>
 * <b>Role:</b> Prints the "string representation" to the file.<br><br>
 * @param fp
 * file to print to.
 */
	void debug( FILE *fp = debug_file_ptr ) const;

	// The readable "string representation" of this thing, in case you
	// want to do other things with it than send it to a file. The
	// passed "str" must be big enough. Returns the given arg for
	// convenience.
/**
 * The readable "string representation" of this.
 * <br><br>
 * <b>Role:</b> In case you want to do other things with it than send it to a file.
 * The passed "str" must be big enough. Returns the given argument for convenience.<br><br>
 * @param str
 * string.
 */
	char *string( char *str ) const;

private:
	SPAinterval angles_data;
	cvty tangent_convexity_data;
};

/** @} */

#endif
