/*******************************************************************/
/*    Copyright (c) 1989-2020 by Spatial Corp.                     */
/*    All rights reserved.                                         */
/*    Protected by U.S. Patents 5,257,205; 5,351,196; 6,369,815;   */
/*                              5,982,378; 6,462,738; 6,941,251    */
/*    Protected by European Patents 0503642; 69220263.3            */
/*    Protected by Hong Kong Patent 1008101A                       */
/*******************************************************************/
#if !defined( VECTOR_UTILS )
#define VECTOR_UTILS

#include "base.hxx"
#include "logical.h"
#include "dcl_base.h"

/**
 * @file vector_utils.hxx
 * @CAA2Level L1
 * @CAA2Usage U1
 * \addtogroup ACISGEOMOPERATORS
 *
 * @{
 */

class SPAposition;
class SPAunit_vector;
class SPAbox;
class SPApar_pos;
class SPApar_box;

/**
 * Determines the distance between two points.
 * <br><br>
 * @param pt1
 * first point.
 * @param pt2
 * second point.
 */
DECL_BASE double distance_to_point(
    const SPAposition&  pt1,
    const SPAposition&  pt2
    );


DECL_BASE double gme_distance_to_point(
    const SPAposition&	pt1,
    const SPAposition&	pt2
    );

/**
 * Computes the squared distance between two positions.
 * <br><br>
 * <b>Role:</b> This is much faster than <tt>distance_to_point</tt>,
 * and can often be used in place of it.  You may test results with <tt>is_zero_squared(n)</tt>.
 * <br><br>
 * @param pt1
 * first point.
 * @param pt2
 * second point.
 */
DECL_BASE double distance_to_point_squared(
    const SPAposition&	pt1,
    const SPAposition&	pt2
    );

DECL_BASE double gme_distance_to_point_squared(
    const SPAposition&	pt1,
    const SPAposition&	pt2
    );
/**
 * Determines the distance from a <tt>SPAposition</tt> to a line.
 * <br><br>
 * @param pt
 * position to test.
 * @param line_pt
 * position on line.
 * @param line_dir
 * direction of line.
 */
DECL_BASE double distance_to_line(
    const SPAposition&		pt,
    const SPAposition&		line_pt,
    const SPAunit_vector&	line_dir
    );

DECL_BASE double gme_distance_to_line(
    const SPAposition&		pt,
    const SPAposition&		line_pt,
    const SPAunit_vector&	line_dir
    );


/**
 * Computes the squared distance from a <tt>SPAposition</tt> to a line.
 * <br><br>
 * <b>Role:</b> This is much faster than <tt>distance_to_line</tt>,
 * and can often be used in place of it. You may test results with
 * <tt>is_zero_squared(n)</tt>.
 * <br><br>
 * @param pt
 * position to test.
 * @param line_pt
 * position on line.
 * @param line_dir
 * direction of line.
 */
DECL_BASE double distance_to_line_squared(
    const SPAposition&		pt,
    const SPAposition&		line_pt,
    const SPAunit_vector&	line_dir
    );

DECL_BASE double gme_distance_to_line_squared(
    const SPAposition&		pt,
    const SPAposition&		line_pt,
    const SPAunit_vector&	line_dir
    );

/**
 * Determines the distance from a <tt>SPAposition</tt> to a plane.
 * <br><br>
 * @param pt
 * position to test.
 * @param plane_pt
 * position on plane.
 * @param normal
 * normal to plane.
 */
DECL_BASE double distance_to_plane(
    const SPAposition&		pt,
    const SPAposition&		plane_pt,
    const SPAunit_vector&	normal
    );

DECL_BASE double gme_distance_to_plane(
    const SPAposition&		pt,
    const SPAposition&		plane_pt,
    const SPAunit_vector&	normal
    );

/**
 * Determines if a <tt>SPAposition</tt> is on a line within <tt>SPAresabs</tt>.
 * <br><br>
 * <b>Effect:</b> Read-only
 * <br><br>
 * @param pt
 * position to test.
 * @param line_pt
 * position on line.
 * @param line_dir
 * direction of line.
 */
DECL_BASE logical is_on_line(
    const SPAposition&		pt,
    const SPAposition&		line_pt,
    const SPAunit_vector&	line_dir
    );

DECL_BASE logical gme_is_on_line(
    const SPAposition&		pt,
    const SPAposition&		line_pt,
    const SPAunit_vector&	line_dir
    );
/**
 * Determines if a <tt>SPAposition</tt> is on a plane within <tt>SPAresabs</tt>.
 * <br><br>
 * @param pt
 * position to test.
 * @param plane_pt
 * position on.
 * @param normal
 * plane normal.
 **/
DECL_BASE logical is_on_plane(
    const SPAposition&		pt,
    const SPAposition&		plane_pt,
    const SPAunit_vector&	normal
    );
DECL_BASE logical gme_is_on_plane(
    const SPAposition&		pt,
    const SPAposition&		plane_pt,
    const SPAunit_vector&	normal
    );

/**
 * Gets a plane defined by 3 positions.
 * <br><br>
 * <b>Role:</b> Returns TRUE if the plane could be computed
 * or FALSE if the positions are colinear.
 * <br><br>
 * @param pt1
 * first position.
 * @param pt2
 * second position.
 * @param pt3
 * third position.
 * @param plane_pt
 * returned position on the plane.
 * @param plane_normal
 * returned plane normal.
 */
DECL_BASE logical get_plane_from_points(
    const SPAposition& pt1,
    const SPAposition& pt2,
    const SPAposition& pt3,
    SPAposition&       plane_pt,
    SPAunit_vector&    plane_normal
    );

DECL_BASE logical gme_get_plane_from_points(
    const SPAposition& pt1,
    const SPAposition& pt2,
    const SPAposition& pt3,
    SPAposition&       plane_pt,
    SPAunit_vector&    plane_normal
    );

/**
 * Given an array of positions, tries to find a plane on which they all lie.
 * <br><br>
 * <table>
 * <tr>
 * <td align="top"><b>Role:</b> Returns FALSE if:</td>
 * <td>&nbsp;the points do not span a plane OR</td>
 * </tr>
 * <tr>
 * <td>&nbsp;</td>
 * <td>&nbsp;the points do not all fit in the same plane OR</td>
 * </tr>
 * <tr>
 * <td>&nbsp;</td>
 * <td>&nbsp;&lt;3 points are given OR </td>
 * </tr>
 * <tr>
 * <td>&nbsp;</td>
 * <td>&nbsp;3 points are given and they are colinear.</td>
 * </tr>
 * </table>
 * <br>
 * @param npts
 * number of positions in the pts array.
 * @param pts
 * array of positions.
 * @param plane_pt
 * returned position on plane.
 * @param plane_normal
 * returned plane normal.
 */
DECL_BASE logical get_plane_from_points(
    int					npts,
    const SPAposition *	pts,
    SPAposition&		plane_pt,
    SPAunit_vector&		plane_normal);

DECL_BASE logical gme_get_plane_from_points(
    int					npts,
    const SPAposition *	pts,
    SPAposition&		plane_pt,
    SPAunit_vector&		plane_normal);

/**
 * Given an array of positions, tries to find a line on which they all lie.
 * <br><br>
 * <table>
 * <tr>
 * <td><b>Role:</b> Returns FALSE</td>
 * <td>&nbsp;if the points do not span a line, OR </td>
 * </tr>
 * <tr>
 * <td>&nbsp;</td>
 * <td>&nbsp;if they do not all fit in the same line.</td>
 * </tr>
 * </table>
 * <br>
 * @param npts
 * number of positions in the pts array.
 * @param pts
 * array of positions.
 * @param line_point
 * returned point on line.
 * @param line_direction
 * returned line direction.
 */
DECL_BASE logical get_line_from_points(
    int					npts,
    const SPAposition *	pts,
    SPAposition&		line_point,
    SPAunit_vector&		line_direction);

DECL_BASE logical gme_get_line_from_points(
    int					npts,
    const SPAposition *	pts,
    SPAposition&		line_point,
    SPAunit_vector&		line_direction);
/**
* Returns minimim distance between box and point.
**/
DECL_BASE double min_distance_to_box( 
					SPAposition const& pt,
					SPAbox const& box );

DECL_BASE double gme_min_distance_to_box( 
					SPAposition const& pt,
					SPAbox const& box );
/**
* Returns distance from point to furthest point on box.
**/
DECL_BASE double max_distance_to_box(const SPAposition& p,const SPAbox& b);
DECL_BASE double gme_max_distance_to_box(const SPAposition& p,const SPAbox& b);

/**
* Returns minimim distance between par_box and par_pos.
**/
DECL_BASE double min_distance_to_box( 
					SPApar_pos const& pt,
					SPApar_box const& box );

DECL_BASE double gme_min_distance_to_box( 
					SPApar_pos const& pt,
					SPApar_box const& box );
/**
* Returns distance from point to furthest par_pos on par_box.
**/
DECL_BASE double max_distance_to_box(const SPApar_pos& p,const SPApar_box& b);
DECL_BASE double gme_max_distance_to_box(const SPApar_pos& p,const SPApar_box& b);

/** @} */
#endif

