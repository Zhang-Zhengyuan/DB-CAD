// $Id: curextnd.hxx,v 1.9 2002/08/16 17:33:12 products Exp $
/*******************************************************************/
/*    Copyright (c) 1989-2020 by Spatial Corp.                     */
/*    All rights reserved.                                         */
/*    Protected by U.S. Patents 5,257,205; 5,351,196; 6,369,815;   */
/*                              5,982,378; 6,462,738; 6,941,251    */
/*    Protected by European Patents 0503642; 69220263.3            */
/*    Protected by Hong Kong Patent 1008101A                       */
/*******************************************************************/

#if !defined(CUREXTND_HXX)
#define CUREXTND_HXX

#include "dcl_intr.h"
#include "logical.h"

#include "polyadic.hxx"
#include "interval.hxx"

/**
* @file curextnd.hxx
 * @CAA2Level L1
 * @CAA2Usage U1
 */

/** \addtogroup ACISGEOMETRY
 *  \brief Declared at <curextnd.hxx>, SPAintr
 *  @{
 */

class curve;
class extension_info;

typedef SPAinterval (*curve_extend_function)(
		curve &,
		SPAinterval const &,
		logical,
		extension_info &,
		logical
	);
DECL_INTR
curve_extend_function find_curve_extend_function(
			curve const &
		);

// Class for registering specific curve extension functions

class DECL_INTR curve_extend_calculator : public monadic_function_entry {
	curve_extend_function fn;
public:
	curve_extend_calculator(int, curve_extend_function);

	friend DECL_INTR curve_extend_function find_curve_extend_function(
			curve const &
		);
};

// Generic curve extension function
/**
* In place curve extension.
* <br><br>
* <b>Role:</b> Extend a curve in place, returning the extension that was actually achieved. 
* <br><br>
* If possible, the input curve is extended out to the requested parameter range. However, if the
* extended curve is found to be improper (e.g., self-intersecting), the requested extension will not be completed.
* In this case, if the logical flag partial_ok is TRUE, a smaller extension, which is proper, 
* will be performed.
* In either case, the achieved pamameter range will be returned. Clients should check the returned parameter range to determine the success of
* extend_curve.
* <br><br>
* <b>Limitations:</b> Closed and periodic curves cannot be extended. extend_curve will not shrink a parameter range.
* <br><br>
* <br><br>
* @param cur
* curve to be extended.
* @param requested_param_range
* requested parameter range of the extended curve.
* @param partial_ok
* if the requested parameter range cannot be achieved, controls whether a partial extension is performed; TRUE for yes, FALSE for no extension
* @param ei
* Clients should not use.
* @param f
* Clients should not use.
**/
DECL_INTR SPAinterval
extend_curve(
		curve & cur,
		SPAinterval const & requested_param_range,
		logical partial_ok = TRUE,
		extension_info& ei = *(extension_info*)NULL_REF,
		logical f = FALSE
	);

DECL_INTR void 
extend_curve_short(
		curve &in_curve,
		SPAinterval const &in_interval);
		
		
/*! @} */

/** @} */

#endif
