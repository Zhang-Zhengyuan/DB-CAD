/*******************************************************************/
/*    Copyright (c) 1989-2020 by Spatial Corp.                     */
/*    All rights reserved.                                         */
/*    Protected by U.S. Patents 5,257,205; 5,351,196; 6,369,815;   */
/*                              5,982,378; 6,462,738; 6,941,251    */
/*    Protected by European Patents 0503642; 69220263.3            */
/*    Protected by Hong Kong Patent 1008101A                       */
/*******************************************************************/
// bmt 11-Feb-03 Doc revision
//
#ifndef ADVCOVR_GAPREPORT_H
#define ADVCOVR_GAPREPORT_H

#include "dcl_adm.h"           // DECL_ADM           // DECL_ADM
#include "acovr_enum.hxx"
/**
* @file acovr_gap_report.hxx
 * @CAA2Level L1
 * @CAA2Usage U1
 * \addtogroup ACISADVCOVR
 *
 * @{
 */

/**
 * Boundary gap reporting.
 * <br>
 * <b>Role:</b> This object is used to store the results of queries that
 * measure gaps across an edge or edges.
 * @see acovr_options
 */
class DECL_ADM acovr_gap_report
{

	private:
		double pos_gap;
		double tan_gap;


	public:
	/**
	 * Destructs an <tt>acovr_gap_report</tt> object.
	 */
	~acovr_gap_report();

	/**
	 * Constructs an <tt>acovr_gap_report</tt> object.
	 */
	acovr_gap_report();

	/**
	 * Returns the position gap.
	 * <br><br>
	 * <b>Role:</b> This is the maximum distance measured across the edge.
	 */
	double get_pos_gap() const;

	/**
	 * Returns the tangent gap.
	 * <br><br>
	 * <b>Role:</b> This is the angular deviation, in radians.
	 */
	double get_tan_gap() const;

	/**
	 * Returns the value of the specified gap (position, tangent, or curvature).
	 * <br><br>
	 * @param type
	 *   Gap type.
	 */
	double get_gap(acovr_tol_type type) const;

	// for internal use only
/**
 * @nodoc
 */
	void set_pos_gap(double gap_size);

/**
 * @nodoc
 */
	void set_tan_gap(double gap_size);

/**
 * @nodoc
 */
	void set_gap(acovr_tol_type type, double gap_size);
};

 /** @} */
#endif // ADVCOVR_GAPREPORT_H
