/*******************************************************************/
/*    Copyright (c) 1989-2020 by Spatial Corp.                     */
/*    All rights reserved.                                         */
/*    Protected by U.S. Patents 5,257,205; 5,351,196; 6,369,815;   */
/*                              5,982,378; 6,462,738; 6,941,251    */
/*    Protected by European Patents 0503642; 69220263.3            */
/*    Protected by Hong Kong Patent 1008101A                       */
/*******************************************************************/

#ifndef TEDREP_API_HEADER
#define TEDREP_API_HEADER

/**
 * @file tedrepapi.hxx
 * @CAA2Level L1
 * @CAA2Usage U1
 * \addtogroup STITCHAPI
 *
 * @{
 */

// Base includes
#include "logical.h"

// Kern includes
#include "api.hxx"
#include "acis_options.hxx"
#include "mmgr.hxx"

// Stitch includes
#include "dcl_stitch.h"


// Forward declarations
class outcome;
class TEDGE;

/**
 * Specifies the options for @href api_check_and_fix_tedge.<br>
 * <b>Role:</b> At the moment, this is a blank class that has been defined and added from the 
 * futuristic point of view in order to make it easier to introduce options that 
 * can be passed to the API. 
 */
class DECL_STITCH tedge_repair_options : public ACIS_OBJECT {

};

/**
 * Attempts to repair level 30 check errors in TEDGEs.
 * <br><br>
 * <b>Role: </b> <tt>api_check_and_fix_tedge</tt> attempts to fix level 30 check errors in TEDGEs 
 * but does not guarantee that these errors will be fixed. The repairing operation will be 
 * limited to modifying the ingredients of the TEDGE, its TCOEDGEs and its TVERTEXes. 
 * After the repairing operation, if the API detects one or more level 30 check errors in 
 * the modified TEDGE, then the API will reinstate the original state of the TEDGE, 
 * its TCOEDGEs and its TVERTEXes. 
 * <br>
 * Presently, the API has the capability to fix the following types of errors. <br>
 * <pre>
 * 1.	TCOEDGE and TEDGE geometry appear to be incompatible.		TOL_GEOM_INCOMPATIBLE
 * 2.	Geometry at start of TCOEDGE and TEDGE appear inconsistent.	START_INCONS_TOL_GEOM
 * 3.	Geometry at end of TCOEDGE and TEDGE appear inconsistent.	END_INCONS_TOL_GEOM
 * </pre>
 * <br>
 * The first argument <tt>tedge</tt> is the pointer to the TEDGE that is to be repaired.
 * <br><br>
 * The second argument <tt>fixed</tt> is a <tt>logical</tt> (by reference). The API puts <tt>TRUE</tt> in this 
 * <tt>logical</tt> if the resultant TEDGE has no check errors at level 30 (this could be <tt>TRUE</tt> either 
 * because the incoming TEDGE was already good at level 30 or because the API successfully 
 * repaired the TEDGE). The API puts <tt>FALSE</tt> in this logical if the API could not repair the level 30 check error in the TEDGE.
 * <br><br>
 * The third argument <tt>already_good</tt> is a <tt>logical</tt> (by reference). The API puts <tt>TRUE</tt> in this <tt>logical</tt> if the 
 * incoming TEDGE was already good at level 30.
 * <br><br>
 * The fourth argument takes a pointer to an object of class <tt>tedge_repair_options</tt>. At the moment, 
 * this is a blank class that has been defined and added from the futuristic point of view in order 
 * to make it easier to introduce options that can be passed to the API. 
 * <br><br>
 * The fifth argument is a pointer to an <tt>AcisOptions</tt> class object. This has a default value of <tt>NULL</tt>.
  * <br><br>
 * <b>Product(s):</b> 3D InterOp
 * <br><br>
 * <br><br>
 * <b>Journal: </b> Available
 * <br><br>
 * @param tedge
 * TEDGE to repair
 * @param fixed
 * logical to denote if tedge was repaired.
 * @param already_good
 * logical to denote TEDGE already good at level 30.
 * @param tedrepopts
 * configuration information
 * @param aopts
 * ACIS options
 * @see tedge_repair_options
 */
DECL_STITCH outcome api_check_and_fix_tedge (
        TEDGE* tedge,							// TEDGE to repair
		logical& fixed,							// logical to denote if tedge got repaired or not.
		logical& already_good,					// logical to denote TEDGE already good.
        tedge_repair_options* tedrepopts=NULL,	// configuration information.
		AcisOptions* aopts=NULL					// required by ACIS
		);
/** @} */
#endif // (TEDREP_API_HEADER)
