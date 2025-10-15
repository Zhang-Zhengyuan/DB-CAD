/*******************************************************************/
/*    Copyright (c) 1989-2020 by Spatial Corp.                     */
/*    All rights reserved.                                         */
/*    Protected by U.S. Patents 5,257,205; 5,351,196; 6,369,815;   */
/*                              5,982,378; 6,462,738; 6,941,251    */
/*    Protected by European Patents 0503642; 69220263.3            */
/*    Protected by Hong Kong Patent 1008101A                       */
/*******************************************************************/
// bmt 07-Feb-03  added set_tol(), get_tol() methods to acovr_edge_constraint
// bmt 10-Feb-03 Fix sentry
// bmt 11-Feb-03 Doc revision
//
#ifndef ACOVR_EDGE_CSTRN_H
#define ACOVR_EDGE_CSTRN_H

#include "dcl_adm.h"              // DECL_ADM              // DECL_ADM
#include "acovr_enum.hxx"  // acovr enumerations  // acovr enumerations

/**
* @file acovr_edge_cstrn.hxx
 * @CAA2Level L1
 * @CAA2Usage U1
 * \addtogroup ACISADVCOVR
 *
 * @{
 */

#define ACOVR_DEFAULT_POS_GAP_SPEC 1.e-3
#define ACOVR_DEFAULT_TAN_GAP_SPEC 0.00872664625997164788 // radians = 0.5 degrees
/**
 * Defines an edge constraint for advanced covering.
 * <br>
 * <b>Role:</b> This object is used to specify the continuity requirement on a
 * particular edge.
 * @see acovr_options
 */
class DECL_ADM acovr_edge_constraint
{

	private:
	acovr_continuity_level cty_level;
	double pos_tol;
	double tan_tol;

	public:

	/**
	 * Destructs a <tt>acovr_edge_constraint</tt> object.
	 */
	 ~acovr_edge_constraint();

	/**
	 * Constructs a <tt>acovr_edge_constraint</tt> object.
	 */
	acovr_edge_constraint();

/**
 * Returns the continuity level.
 * <br><br>
 * <b>Role:</b> This is the continuity (G0 or G1) that will be imposed across
 * the edge.
 */
	acovr_continuity_level get_continuity() const;

/**
 * Returns the position gap tolerance value.
 * <br><br>
 * <b>Role:</b> This is the desired position gap tolerance for
 * the edge.
 */
	double get_pos_tol() const;

/**
 * Returns the tangent gap tolerance value.
 * <br><br>
 * <b>Role:</b> This is the desired tangent gap tolerance for
 * the edge (radians).
 */
	double get_tan_tol() const;

/**
 * Returns the tolerance value, for the specified gap tolerance type (position or tangent).
 * <br><br>
 * <b>Role:</b> This is the desired gap tolerance for
 * the edge (distance or radians).
 * <br><br>
 * @param type
 *   The tolerance type to return.
 */
	double get_tol(acovr_tol_type type) const;

/**
 * Sets the continuity level.
 * <br><br>
 * <b>Role:</b> This is the continuity (G0 or G1) that will be imposed across
 * the edge.
 * <br><br>
 * @param icl
 *   The continuity level to set.
 */
	void set_continuity(acovr_continuity_level icl);

/**
 * Sets the position gap tolerance value.
 * <br><br>
 * <b>Role:</b> This is the desired position gap tolerance for
 * the edge.
 * <br><br>
 * @param gap_tol
 *   The tolerance value to set.
 */
	void set_pos_tol(double gap_tol);

/**
 * Sets the tangent gap tolerance value.
 * <br><br>
 * <b>Role:</b> This is the desired tangency gap tolerance for
 * the edge.
 * <br><br>
 * @param gap_tol
 *   The tolerance value to set (radians).
 */
	void set_tan_tol(double gap_tol);


/**
 * Sets the tolerance value, for the specified gap tolerance type (position or tangent).
 * <br><br>
 * <b>Role:</b> This is the desired gap tolerance for
 * the edge (distance or radians).
 * <br><br>
 * @param type
 *   The tolerance type to set.
 * @param gap_tol
 *   The tolerance value to set (distance or radians).
 */
	void set_tol(acovr_tol_type type, double gap_tol);


};


 /** @} */
#endif // ADVCOVR_EDGE_CSTRN_H

