/* ORIGINAL: acis2.1/sg_husk/pcurve/pcu_intp.hxx */
/* $Id: pcu_intp.hxx,v 1.4 2000/12/26 18:30:55 products Exp $ */
/*******************************************************************/
/*    Copyright (c) 1989-2020 by Spatial Corp.                     */
/*    All rights reserved.                                         */
/*    Protected by U.S. Patents 5,257,205; 5,351,196; 6,369,815;   */
/*                              5,982,378; 6,462,738; 6,941,251    */
/*    Protected by European Patents 0503642; 69220263.3            */
/*    Protected by Hong Kong Patent 1008101A                       */
/*******************************************************************/


// Define an interpolated curve which represents a SPAparameter space
// curve.

#if !defined( pcurve_interp_CLASS )
#define  pcurve_interp_CLASS

#include "acis.hxx"
#include "dcl_cstr.h"

#include "logical.h"

#include "fit.hxx"

#include "intdef.hxx"

#include "debugmsc.hxx"

/**
* @file pcu_intp.hxx
 * @CAA2Level L1
 * @CAA2Usage U1
 */

// The definition of a class to fit a SPAparameter space curve.  The SPAparameter
// space curve will be constructed by treating it as a 3D curve having
// a zero z-value.

/**
 * Fits a parameter space curve.
 * <br>
 * <b>Role:</b> This class defines an interpolated curve that represents a parameter space curve.
 * The parameter space curve is constructed by treating it as a 3D curve having a zero z-value.
 */
class DECL_CSTR pcurve_interp: public curve_interp {

private:
	const curve*	object_curve;	// edge being blended along
	const surface*	surf;			// spine of pipe

public:
	// Constructor providing all the information required.

/**
 * C++ initialize constructor requests memory for this object and populates it with the data supplied as arguments.
 * <br><br>
 * @param scurve
 * object space curve.
 * @param sface
 * surface definition.
 * @param ntangents
 * number of optional tangents.
 * @param space_pos
 * array of param. space positions.
 * @param space_tang
 * array of param. space tangents.
 * @param tol
 * fit tolerance.
 */
	pcurve_interp(
			const curve * scurve,		// object space curve
			const surface * sface,	// surface definition
			int ntangents,				// number of initial data points
			const SPAposition * space_pos,	// array of initial pcurve positions
			const SPAvector *space_tang,		// array of initial pcurve tangents
			double	tol			// fit tolerance
		);

	// Virtual destructor.

/**
 * @nodoc
 */
	~pcurve_interp();

	// Find the true-point for the SPAparameter space curve

/**
 * Finds the point in 3D for a given parameter value.
 * <br><br>
 * <b>Role:</b> Input and output information is contained in the <tt>point_data</tt> argument.
 * The input parameter position and direction are approximate; the exact values for parameter
 * position and direction are provided as output.
 * <br><br>
 * @param tol
 * tolerance.
 * @param pdata
 * point data.
 */
	void true_point(
			double tol,			// tolerance
			point_data & pdata	// approximate SPAposition, direction,
							// SPAparameter values on input, exact values
							// on output
		) const;

	// Construct the appropriate int_cur subclass object (in this
	// case an exact_int_cur) from the data in this object after curve
	// interpolation.


/**
 * Constructs the appropriate <tt>int_cur</tt> subclass object from the data in this object after curve interpolation.
 */
	int_cur *make_int_cur();
};

#endif
