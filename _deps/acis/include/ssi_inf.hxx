/*******************************************************************/
/*    Copyright (c) 1989-2020 by Spatial Corp.                     */
/*    All rights reserved.                                         */
/*    Protected by U.S. Patents 5,257,205; 5,351,196; 6,369,815;   */
/*                              5,982,378; 6,462,738; 6,941,251    */
/*    Protected by European Patents 0503642; 69220263.3            */
/*    Protected by Hong Kong Patent 1008101A                       */
/*******************************************************************/
//			Surface - Surface Intersection
/*******************************************************************/
#ifndef SSI_INF_H
#define SSI_INF_H
/**
* @file ssi_inf.hxx
 * @CAA2Level L1
 * @CAA2Usage U1
 */
#include <stdio.h>
#include "dcl_intr.h"
#include "logical.h"
#include "pos.hxx"
#include "fn2.hxx"
class surface;
class SPAbox;
class surf_surf_int;
#if defined D3_STANDALONE || defined D3_DEBUG
    class D3_ostream;
    class D3_istream;
#endif
/**
  * @nodoc
  * Help point types.
  * <br><br>
  * <b>Role :<b> Except for the 'Flat' type (which indicates either a terminator
  * or an  isolated point), these categories do not overlap and should not be confused. 
  * For example, classifying a point as 'Help' when it should be an entry point 
  * will lead to errors in the intersection algorithm. 
  * @param Entry
  * Is an entry point.
  * @param Exit
  * Is an exit point.
  * @param Help
  * It is a point on the intersection which is not an entry, exit or isolated point
  * or a terminator.
  * @param Terminator
  * Is a terminal point.
  * @param Isolated
  *
  * @param Flat
  * Either a terminator or an isolated point.
  * @param Turning_point
  * 
  */
enum help_point_type
{
	Entry,
	Exit,
 	Help,
	Terminator,		
	Isolated,	
	Flat,
	Turning_point		
	};
/**
  * @nodoc
  * Help point class.
  * <br><br>
  * <b>Role :<b> 
  * Help points are supplied to d3_sf_sf_int in a linked list of the following 
  * structures. 
  * STIPORT aw changed this from struct to class and so had to also add public
  * there was a redeclaration of it as class and it has a constructor
  * Note that on_boundary should always be TRUE if the type is an entry or exit 
  * point. 
  */
class DECL_INTR help_point : public ACIS_OBJECT
{
public:
    /**
     * Help point position
     */
	SPAposition	P;
    /**
     * First surface's u-v parameter. Set if the surface is parametric
     */
	SPApar_pos		uv0;
    /**
     * Second surface's u-v parameter. Set if the surface is parametric
     */
	SPApar_pos		uv1;
    /**
     * Help point type.
     */
	help_point_type type;
    /**
     * TRUE if the help point is known to be on a boundary of either
     * surface.
     */
	logical		on_boundary;
    /**
     * Next help point
     */
	help_point*	next;
    /**
     * Constructor
     */
    help_point( const SPAposition& aP, 
                const SPApar_pos&  auv0, 
                const SPApar_pos&  auv1, 
                help_point_type    atype, 
                logical            aon_boundary,
                help_point*        anext );
    /**
     * Direction of the curve at the 
     * help point position
     */
	SPAvector      tangent;
    /**
     * Constructor. Considers the tangent direction.
     */
	help_point( const SPAposition& aP,
                const SPApar_pos&  auv0, 
                const SPApar_pos&  auv1, 
                const SPAvector&   tan,
                help_point_type    atype, 
                logical            aon_boundary,
                help_point*        anext );
    /**
     * Add help points to the end of the concatenated list.
     * @param pHelpPoint
     */
     void add(help_point* pHelpPoint);

	};
#ifdef D3_DEBUG 
    DECL_INTR D3_ostream& operator<<( D3_ostream& s, const help_point& hpt );
#endif
  /**
   * @nodoc
   * Intersects two surfaces.
   * <br><br>
   * <b>Role:</b> The surface intersection function is d3_sf_sf_int. This 
   * takes two surfaces and a SPAbox, and finds all intersection curves,
   * isolated intersection points and coincidencent regions. The intersections
   * are returned as ACIS surf_surf_int objects.
   * <br><br>
   * Although the algorithm can work on any types of surface, it is designed 
   * for use on parametric surfaces with boundaries, and treats all surfaces 
   * as though they have boundaries, or are periodic. The boundaries of an  
   * implicit surface in a non-periodic direction are SPAparameter curves at the 
   * limit of the SPApar_box returned by the surface param_range function, given the 
   * intersection SPAbox. 
   * <br><br>
   * The full algorithm finds all intersections on the surface boundaries first 
   * to obtain points to march from, and generate intersection curves. It also 
   * performs subdivision, to find internal points from which to march. The 
   * application can request that either the boundary processing stage or the 
   * subdivision (or both) be missed, so that only the marching and creation of 
   * result curves remains. In this case, the application must supply the missing 
   * information. 
   * <br><br>
   * Three flags are supplied to d3_sf_sf_int, to control what the algorithm does 
   * and what data it should expect:
   * <br><br>
   * enum completeness :
   * <br>This enum controls the number of interection curves that 
   * are returned. It may have one of six values:
   * <br><br>	
   *	SINGLE_CURVE - Only a single intersection curve is required, and a
   *		help point of some sort has been supplied for it. 
   * <br><br>
   *	COMPOSITE_CURVE - All the bits of one intersection curve are
   *		required, even if they lie on either side of a terminator.
   * <br><br>
   *	INPUT_DATA - All curves corresponding to input data will be found. 
   *		Again, help point(s) must be supplied or no results will be 
   *		returned. Curves will only be returned if they lie on one or 
   *		more of the help points, or if they are connected to such a
   *		curve through a terminator (so, for example, both parts of a 
   *		figure-of-eight curve will be returned if a single help point on
   *		one part is supplied). This is an important difference between 
   *		completeness = 1 and completeness = 3. 
   * <br><br>
   *	PROCESS_BOUNDARIES - Curves that lie on one or more of the supplied help
   *		points, or that are connected to such a curve through a
   *		terminator are returned, and also any curves that are found
   *		by the boundary processing.
   * <br><br>
   *	DO_SUBDIVISION - Curves that lie on one or more of the supplied help
   *		points, or that are connected to such a curve through a
   *		terminator are returned, and also any curves that are found
   *		by the subdivision.
   * <br><br>
   *	ALL_CURVES (the default) - All curves will be found, using the 
   *		full algorithm. There is no need to supply any help point
   *		information. 
   * <br><br>
   *   	Note that using INPUT_DATA  will find all possible curves if the 
   * 	input data is 'complete', in the sense that the following help
   * 	point data is supplied:
   * <br><br>
   *	1.  All entry points (points at which an intersection curve enters 
   *	    any boundary of either surface).
   * <br><br>
   *	2.  All exit points (points at which an intersection curve leaves 
   *	    any boundary of either surface).
   * <br><br>
   *	3.  A help point (including a terminator) on each intersection curve 
   *	    which does not reach the boundary. 
   * <br><br>
   *	3.  All terminators which happen to be on any boundary of either 
   *	    surface.
   * <br><br>
   *	4.  All isolated points which happen to be on any boundary of either 
   *	    surface. This includes tangent isolated points, at which the 
   *	    surfaces just touch, or points that are isolated simply because 
   *	    they are on the boundary. 
   * <br><br>
   *	If isolated points are not provided then they will not be included in 
   * 	the results structure, but there is no other adverse effect. 
   * <br><br><br>
   *
   *   logical safe_area
   * <br><br>
   *	If this flag is true, then it indicates that all terminators, turning 
   *	points and isolated points have been supplied. It does not affect the 
   *	results returned by the algorithm but enables it to work much faster. 
   * 	If incomplete results are sufficient (for example only a single curve
   * 	has been requested) then it is only necessary to supply the turning
   * 	points, isolated points and terminators on or near these results. 
   * <br><br>
   * A tangent curve starts and ends at terminators, not entry and exit points. 
   * Also, a help point on a tangent curve should be supplied as a terminator. 
   * <br><br>
   * The various entry, exit, isolated and help points and terminators that might 
   * be supplied to d3_sf_sf_int are collectively called help_points. 
   * There follows a classification for the help points. 
   * <br><br>
   * The algorithm can also be used on offset surfaces, defined by supplying the 
   * offsets specifically. However, help points are required and complete results 
   * cannot be guaranteed in this case so the completeness flag cannot be set to.
   *
   * @param sf0
   * First surface.
   * @param sf1
   * Second surface.
   * @param b_in
   * Region of interest.
   * @param fitol
   * Fitting tolerance for return curves.
   * @param list
   * List of entry, exit, help and isolated points and terminators.
   * @param completeness
   * Specifies the completeness of the results required. (See above).
   * @param safe_area
   * TRUE if all terminators, turning points and isolated points have been 
   * supplied, or none exists.
   * @param off0
   * Offset of first surface.
   * @param off1
   * Offset of second surface.
   * @param abort_on_illegal_surf
   * Exit function if illegal surface found.
   */
extern DECL_INTR surf_surf_int* d3_sf_sf_int( 
                                const surface&    sf0,
                                const surface&    sf1,
                                const SPAbox&     b_in,
                                double            fitol                 = SPAresfit,
                                const help_point* list                  = 0,
                                COMPLETENESS_TYPE completeness          = ALL_CURVES,
                                logical	          safe_area             = FALSE,
                                const double      off0                  = 0.0,
                                const double      off1                  = 0.0,
                                logical           abort_on_illegal_surf = FALSE
                                );

#endif



