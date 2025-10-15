/*******************************************************************/
/*    Copyright (c) 1989-2020 by Spatial Corp.                     */
/*    All rights reserved.                                         */
/*    Protected by U.S. Patents 5,257,205; 5,351,196; 6,369,815;   */
/*                              5,982,378; 6,462,738; 6,941,251    */
/*    Protected by European Patents 0503642; 69220263.3            */
/*    Protected by Hong Kong Patent 1008101A                       */
/*******************************************************************/


#if !defined( CHK_FIX_HXX )
#define CHK_FIX_HXX

#include "intrapi.hxx"

/*******************************************************************/
/**
* @file chk_fix_api.hxx
 * @CAA2Level L1
 * @CAA2Usage U1
 */

/** @} */
/** \addtogroup ACISCHECKING
 *  \brief Declared at <intrapi.hxx>, SPAintr
 *  @{
 */

/**
 * Attempts to fix some check problems in model data.
 * <br><br>
 * <b>Role:</b> Given a list of problems found by <tt>api_check_entity</tt>,
 * <tt>api_check_face</tt> or <tt>api_check_edge</tt>, this function attempts
 * to alter the problematic entities to correct the problems listed. Note that
 * not all types of problem will be fixed; it is not possible to implement
 * automatic repair for all problems.
 * <br><br>
 * The API returns two new <tt>insanity_list</tt> chains. Destruction of these
 * <tt>insanity_list</tt> objects is the responsibility of the caller. The
 * input <tt>insanity_list</tt> is unaltered. The "problems_fixed" list
 * describes those problems that were successfully fixed by the API, and the
 * "problems_not_fixed" list describes those problems which could not be fixed for
 * one reason or another. A null pointer may be returned for either list,
 * depending on the degree of success of the API:
 * <br><br>
 * <table border = 1>
 * <tr>
 * <td valign = top><b>problems_fixed value&nbsp;&nbsp;&nbsp;</b></td>
 * <td valign = top><b>problems_not_fixed value&nbsp;&nbsp;&nbsp;</b></td>
 * <td valign = top><b>Interpretation&nbsp;&nbsp;&nbsp;</b></td>
 * </tr>
 * <tr>
 * <td valign = top>NULL</td>
 * <td valign = top>non-NULL</td>
 * <td valign = top>No input problems fixed by API</td>
 * </tr>
 * <tr>
 * <td valign = top>non-NULL</td>
 * <td valign = top>NULL</td>
 * <td valign = top>All input problems fixed by API</td>
 * </tr>
 * <tr>
 * <td valign = top>non-NULL</td>
 * <td valign = top>non-NULL</td>
 * <td valign = top>Some, but not all problems fixed by API</td>
 * </tr>
 * <tr>
 * <td valign = top>NULL</td>
 * <td valign = top>NULL</td>
 * <td valign = top>Special case: input problem list is NULL</td>
 * </tr>
 * </table>
 * <br><br>
 * If the input list is NULL, the API does nothing, but does not fail. A NULL
 * pointer will be returned for both lists.
 * <br><br>
 * <b>Limitations:</b> Currently, this API only attempts to fix some of the
 * check problems that were introduced in ACIS R14. Note that not all problems
 * can be repaired automatically. The <tt>insanity_data</tt> IDs for which 
 * a fix will be attempted are:
 * <tt>CRV_BAD_FITOL</tt>, <tt>SURF_BAD_FITOL</tt>, <tt>CURVE_APPROX_OVERCLAMPED</tt>, 
 * <tt>CURVE_MISSING_DISC_INFO</tt>, and <tt>SURF_MISSING_DISC_INFO</tt>.
 * <BR><BR>
 * <b>Errors:</b> None. 
 * <br><br>
 * <b>Effect:</b> Alters model if one or more problems is successfully fixed.
 * <br><br>
* <b>Journal: </b> Not Available
* <br><br>
 * <b>Product(s):</b> 3D InterOp
 * <br><br>
* @param problems_to_fix
 * list of problems for which fixing is to be attempted.
 * @param problems_fixed
 * list of input problems that were successfully fixed.
 * @param problems_not_fixed
 * list of input problems that were not successfully fixed.
 * @param ao
 * ACIS options.
 **/ 
DECL_INTR outcome api_fix_check_problems(
             const insanity_list    * problems_to_fix,
             insanity_list          *& problems_fixed,
             insanity_list          *& problems_not_fixed,
             AcisOptions            * ao = NULL);

#endif	//	CHK_FIX_HXX
