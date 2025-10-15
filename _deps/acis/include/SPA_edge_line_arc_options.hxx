/*******************************************************************/
/*    Copyright (c) 1989-2020 by Spatial Corp.                     */
/*    All rights reserved.                                         */
/*    Protected by U.S. Patents 5,257,205; 5,351,196; 6,369,815;   */
/*                              5,982,378; 6,462,738; 6,941,251    */
/*    Protected by European Patents 0503642; 69220263.3            */
/*    Protected by Hong Kong Patent 1008101A                       */
/*******************************************************************/
/**
 * @file SPA_edge_arc_line_options.hxx
 * @CAA2Level L1
 * @CAA2Usage U1
 * \addtogroup BSGEOMETRY
 *
 * @{
 */
#if !defined(SPA_EDGE_LINE_ARC_OPTIONS_HXX)
#define SPA_EDGE_LINE_ARC_OPTIONS_HXX

#include "base.hxx"
#include "acistype.hxx"
#include "dcl_kern.h"


class SPA_edge_line_arc_opts_int;

//=============================================================================
// classes associated with approximation

/**
 *
 * This options class is used as input for @href api_edge_approx_line_arc. 
 * It lets the user tailor the behavior of <tt>api_edge_approx_line_arc</tt>.
 * The options that can be specified using a <tt>SPA_edge_line_arc_options</tt>
 * object are listed below.
 * <ul>
 * <li> <tt>Position Tolerance</tt>: The Position Tolerance parameter controls 
 * the maximum positional approximating error of a segment from the original input EDGE. 
 * The value is specified by a double value in ACIS model units. The default value 
 * is <tt>SPAresabs</tt>. This value can be set using the <tt>set_requested_tol</tt> method.</li> 
 * <li> <tt>Angle Tolerance</tt>: The Angle Tolerance parameter controls 
 * the maximum angle between two adjacent approximating edges. The value is specified in radians. 
 * The default value is the radians equivalent of five degrees. 
 * This value can be set using the <tt>set_angle_tol</tt> method.</li>
 * <li> <tt>Minimum Length</tt>: The Minimum Length parameter controls 
 * the minimum length of a segment created by the API. The value is specified by a double value 
 * in ACIS model units. The default value is <tt>1E-4</tt>. 
 * This value can be set using the <tt>set_min_length</tt> method.</li>
 * <li> <tt>Minimum Radius</tt>: The Minimum Radius parameter specifies 
 * the minimum allowable radius of a circular segment created by the API. 
 * The value is specified by a double value in ACIS model units. The default value is <tt>1E-3</tt>. 
 * This value can be set using the <tt>set_min_radius</tt> method.</li>
 * <li> <tt>Maximum Radius</tt>: The Maximum Radius parameter specifies 
 * the maximum allowable radius of a circular segment created by the API. 
 * The value is specified by a double value in ACIS model units. The default value is <tt>1E+3</tt>. 
 * This value can be set using the <tt>set_max_radius</tt> method.</li>
 * <li> <tt>Minimum Arc Angle</tt>: The Minimum Arc Angle parameter specifies 
 * the minimum allowable arc angle of a circular segment created by the API. 
 * The value is specified by a double value in radians. The default value is <tt>M_PI * 1E-6</tt>. 
 * This value can be set using the <tt>set_min_arc_angle</tt> method.</li>
 * <li> <tt>Lines Only</tt>: The Lines Only flag instructs the API to return 
 * only a sequence of linear segments instead of a mix of circular and linear segments. 
 * This value is specified by a <tt>logical</tt>. The default value is <tt>FALSE</tt>. 
 * This value can be set using the <tt>set_lines_only</tt> method.</li>
 * <li> <tt>Check Reverse</tt>: The Check Reverse flag enables or disables 
 * the reversal of the second input <tt>EDGE</tt>. When this flag is set to <tt>TRUE</tt>, 
 * the API uses the coedges (if the edges share a common face) or geometry checks 
 * (if the edges do not share a common face) to decide if it needs to reverse 
 * the approximation direction of the second input <tt>EDGE</tt>. 
 * When this flag is set to <tt>FALSE</tt>, the API assumes that the second <tt>EDGE</tt> 
 * is already oriented as you may have intended. The value is specified by a <tt>logical</tt>. 
 * The default value is <tt>TRUE</tt>. This value can be set using the <tt>set_check_reverse</tt> method.</li>
 * </ul>
 * 
 **/
class DECL_KERN SPA_edge_line_arc_options: public ACIS_OBJECT
{

public:
	/**
	* Default Constructor.
	**/
	SPA_edge_line_arc_options();

	/**
	* Constructor that takes as input all initial values.
	* <br><br>
	* @param positionTolerance
	* value of the position tolerance parameter
	* @param angularTolerance
	* value of the angle tolerance parameter
	* @param minLength
	* value of the minimum length parameter
	* @param minRadius
	* value of the minimum radius parameter
	* @param maxRadius
	* value of the maximum radius parameter
	* @param minArcAngle
	* value of the minimum arc angle parameter
	* @param useLinesOnly
	* value of the lines only flag
	* @param inputCheckReverse
	* value of the check reverse flag
	**/
	SPA_edge_line_arc_options(double positionTolerance, 
							  double angularTolerance, 
							  double minLength, 
							  double minRadius, 
							  double maxRadius, 
							  double minArcAngle, 
							  logical useLinesOnly, 
							  logical inputCheckReverse);

	/**
	* Destructor
	**/
	~SPA_edge_line_arc_options();

	/**
	* Sets the Position Tolerance parameter, which controls the maximum positional 
	* approximating error of a segment from the original input EDGE. 
	* The default value is <tt>SPAresabs</tt>. 
	* <br><br>
	* @param tol
	* new value of the position tolerance parameter
	**/
    void set_requested_tol(double tol);

	/**
	* Sets the Minimum Length parameter, which controls the minimum length of 
	* a segment created by the API. 
	* <br><br>
	* @param minLength
	* new value of the minimum length parameter
	**/
	void set_min_length(double minLength);

	/**
	* Sets the mininum radius allowable for an arc segment. 
	* If this cannot be achieved, a line segment is created instead.
	* <br><br>
	* @param minRadius
	* new value of the minimum radius parameter
	**/
	void set_min_radius(double minRadius);

	/**
	* Sets the angular tolerance (in radians) allowable between two segments.
	* <br><br>
	* @param angleTol
	* new value of the angle tolerance parameter
	**/
	void set_angle_tol(double angleTol);
	
	/**
	* Sets the maximum radius allowable for an arc segment. 
	* If this cannot be achieved a line segment is created instead.
	* <br><br>
	* @param maxRadius
	* new value of the maximum radius parameter
	**/
	void set_max_radius(double maxRadius);
	
	/**
	* The Minimum Arc Angle parameter specifies the minimum allowable arc angle of a circular segment created by the API.
	* The value is specified by a double value in radians. The default value is <tt>M_PI*1E-6</tt>.
	* <br><br>
	* @param minArcAngle
	* new value of the minimum arc angle parameter
	**/
	void set_min_arc_angle(double minArcAngle);

	/**
	* Sets the flag to indicate that the API should create only lines 
	* instead of a combination of lines and circular arcs.
	* <br><br>
	* @param linesOnly
	* new value of the lines only flag
	**/
	void set_lines_only(logical linesOnly);

	/**
	* The Check Reverse flag enables or disables the reversal of the second input EDGE. 
	* When this flag is set to <tt>TRUE</tt>, the API uses the coedges (if the edges share a common face) 
	* or geometry checks (if the edges do not share a common face) to decide if it needs to 
	* reverse the approximation direction of the second input EDGE. When this flag is set to <tt>FALSE</tt>,
	* the API assumes that the second EDGE is already oriented as the user intended. 
	* The default value is <tt>TRUE</tt>. 
	* <br><br>
	* @param checkReverse
	* new value of the check reverse flag
	**/
	void set_check_reverse(logical checkReverse);
	
	/**
	* Gets the current angular tolerance value of this Options Object.
	**/
	double get_angle_tol() const;

	/**
	* Gets the current positional tolerance value of this Options Object.
	**/
    double get_requested_tol() const;

	/**
	* Gets the current minimum segment length value of this Options Object.
	**/
	double get_min_length() const;

	/**
	* Gets the current minimum allowable radius value of this Options Object.
	**/
	double get_min_radius() const;

	/**
	* Gets the current maximum allowable radius value of this Options Object.
	**/
	double get_max_radius() const;

	/**
	* Gets the current minimum allowable arc angle value (in radians) of this Options Object.
	**/
	double get_min_arc_angle() const;

	/**
	* Gets the current lines only flag of this Options Object.
	**/
	logical get_lines_only() const;

	/**
	* Gets the current check reverse flag of this Options Object.
	**/
	logical get_check_reverse() const;
	

private:
	SPA_edge_line_arc_opts_int *internalOpts;
};

/** @} */
#endif
