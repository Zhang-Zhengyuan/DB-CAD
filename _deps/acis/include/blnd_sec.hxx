/* ORIGINAL: acis2.1/sg_husk/vrbln/blnd_sec.hxx */
// $Id: blnd_sec.hxx,v 1.24 2002/08/16 19:28:36 products Exp $
/*******************************************************************/
/*    Copyright (c) 1989-2020 by Spatial Corp.                     */
/*    All rights reserved.                                         */
/*    Protected by U.S. Patents 5,257,205; 5,351,196; 6,369,815;   */
/*                              5,982,378; 6,462,738; 6,941,251    */
/*    Protected by European Patents 0503642; 69220263.3            */
/*    Protected by Hong Kong Patent 1008101A                       */
/*******************************************************************/


#include "acis.hxx"
#include "dcl_kern.h"


//
// Header file descibing the cross section of a blend surface.
//
// The cross section of a blend surface corresponds to the u-SPAparameter
// of the surface.  When evaluating the surface, a slice is taken at
// the given v-SPAparameter, and then that slice is evaluated at the u-param.
//
// During the initial construction of the blend surface geometry,
// the cross section is irrelevant.  It will first come into play when
// the blend surface is intersected with other faces.
//
// There are (or will be) at least two distinct types of cross sections,
// which unfortunately don't allow a nice generalization:
// thumbweights, which includes circular, and curvature-continuous blends.
// We can't just store a prototypical cross section shape
// in a 2d spline: the relative points and weights needed for the
// circular cross section depend on the angle between the surface normals.
// So we will need an indication of the general type of cross section,
// i.e. cross_section.form() == { CIRCULAR | THUMBWEIGHTS | G2_CONTINUOUS }.
// Specifying these will be by simply specifying one of these three forms,
// along with the necessary parameters (two thumbweights; possibly a
// fullness factor for G2, but maybe not).
//
// The explicit CHAMFER section is not used yet, but it can be achieved
// with a RND_CHAMFER with zero round height.  It could also be achieved with
// zero thumbweights, but a zero round height makes a much better surface.
//
// The rotated ellipse section does not separate nicely into radius function
// and cross section, because the offset values (ie, radius) depend on the
// shape.  The offset values are required for the first phase of evaluation,
// finding the slice at the v-SPAparameter, and are therefore handled by the
// var_radius class, not here.
//
//
// Note: var_cross_section objects don't shift, split, append or transform.
// Parameterization runs from 0 to 1, and that covers the whole section.
// Those concepts are left to higher-level entities.

//
// Modification---
//      30-Aug-07 pvc   Add method is_tangent_continuous()
//	08-Jun-99 bd	Add uncalibrate_v_params().
//	22-Feb-99 bd	New inquiry function is_circular.
//	02-Jun-97 bd	Remove the c'tor (form,double): conflict with the default.
//	29-Feb-96 bd	Change bl_geom.hxx include to bl_enum.hxx (micro-kernel).
//	28-Jan-96 bd	Remove 'range' stuff from var_radius c'tors, and ours.
//	11-Jan-96 bd	Add transform operator*=().
//	05-Jan-96 aed	Use dispatch tables for save/restore of
//		derived types of var_radius and forms of var_cross_section.
//	20-Oct-95 bd	Add value CHAMFER to cross_section_forms enumeration.
//

#ifndef BLND_SEC_DEF
#define BLND_SEC_DEF

#include <stdio.h>	// kernutil/debug/debugmsc.hxx needs this...
#include "logical.h"

#include "position.hxx"
#include "vector.hxx"
#include "transf.hxx"
#include "interval.hxx"

#include "fileio.hxx"

#include "bl_enum.hxx"
#include "var_rad.hxx"

/**
* @file blnd_sec.hxx
 * @CAA2Level L1
 * @CAA2Usage U1
 *! \addtogroup KERNAPI
 *
 * @{
 */


class SizeAccumulator;
class var_cross_section;

/*
// tbrv
*/
/**
 * @nodoc
 */

DECL_KERN var_cross_section *restore_cross_section();

/**
 * Number specifies the type of cross section.
 * @param XSECT_UNKNOWN
 * cross section is unknown.
 * @param CIRCULAR
 * cross section is circular.
 * @param THUMBWEIGHTS
 * thumbweights.
 * @param ROT_ELLIPSE
 * cross section is rotational ellipse.
 * @param RND_CHAMFER
 * cross section is round chamfer.
 * @param G2_CONTINUOUS
 * cross section is G2 continuous.
 * @param CHAMFER
 * cross section is chamfer.
 **/
enum cross_section_forms 
	{	
	XSECT_UNKNOWN = -1,
	CIRCULAR = 0,
	THUMBWEIGHTS = 1,
	ROT_ELLIPSE = 2,
	RND_CHAMFER = 3,
	G2_CONTINUOUS = 4,
	CHAMFER = 5
	};

/**
 * Evaluates the cross section of a blend surface.
 * <br>
 * <b>Role:</b> The cross section of a blend surface corresponds to the <i>u</i>-parameter of
 * the surface. When evaluating the surface, a slice is taken at the given <i>v</i>-parameter,
 * and then that slice is evaluated at the <i>u</i>-parameter.
 * <br><br>
 * During the initial construction of the blend surface geometry, the cross section
 * is irrelevant. It first comes into play when the blend surface is intersected with other faces.
 * <br><br>
 * Only circular cross sections are available in standard blending. The Advanced
 * Blending Component allows other shapes.
 * <br><br>
 * <tt>var_cross_section</tt> objects do not shift, split, append, or transform. Parameterization
 * runs from 0 to 1, and that covers the whole section.
 * @see  var_radius, blend_spl_sur
 */

class DECL_KERN var_cross_section : public ACIS_OBJECT {

protected:

	cross_section_forms	form_data;

	double	left_thumb_data;		//  for form "THUMBWEIGHTS"
	double	right_thumb_data;

	var_radius *rnd_height;			//	for form "RND_CHAMFER"

public:

// constructors:

// This one causes the null constructor to default to a circular section.
/**
 * C++ initialize constructor requests memory for this object and populates it with the data supplied as arguments.
 * <br><br>
 * @param new_form
 * cross section form.
 */

	var_cross_section(cross_section_forms const new_form = CIRCULAR);

// Make a thumoweights or rounded-chamfer section.
// If form is thumbweights, the two arguments are the left and right thumbweighs.
// If rounded-chamfer, the arguments are the start and end round-height values.
// In both cases, the default value for the second argument is the same
// as the first argument.  Since it's not legal to declare the default
// as "= arg1", we set it as a flag.  Negative values are illegal for both
// forms, so -1 will be the flag.
/**
 * C++ initialize constructor requests memory for this object and populates it with the data supplied as arguments.
 * <br><br>
 * <b>Role:</b> Creates a <tt>var_cross_section</tt> using the specified parameters.
 * <br><br>
 * @param form
 * cross section form.
 * @param arg1
 * new left thumbweight.
 * @param arg2
 * new right thumbweight.
 */

	var_cross_section(
		cross_section_forms form,
		double arg1,
		double arg2 = -1.0	// would say "= arg1", but that's not allowed.
	);

// copy constructor:
/**
 * C++ copy constructor requests memory for this object and populates it with the data from the object supplied as an argument.
 * <br><br>
 * @param old_section
 * old section.
 */

	var_cross_section(var_cross_section const &old_section);

// Destructor:
/**
 * C++ destructor, deleting a <tt>var_cross_section</tt>.
 */

	~var_cross_section();


// member data retrieval:
/**
 * Returns the <tt>form_data</tt> about the <tt>var_cross_section</tt>.
 */

	cross_section_forms	form() const		{ return form_data; }
/**
 * Returns the left thumbweight of the <tt>var_cross_section</tt>.
 */

	double left_thumb()  const { return left_thumb_data; }
/**
 * Returns the right thumbweight of the <tt>var_cross_section</tt>.
 */

	double right_thumb() const { return right_thumb_data; }
/**
 * Returns the round height function of a rounded chamfer cross section.
 * <br><br>
 * <b>Role:</b> This is stored as a <tt>var_radius</tt> object. It just returns the pointer, without copying.
 */

	var_radius *chamfer_round_height() const
		{ return form_data == RND_CHAMFER ? rnd_height : NULL; }

// conveniences:
/**
 * Determines if the <tt>var_cross_section</tt> is circular.
 */

	logical is_circular() const;

/**
 * @nodoc
 */
// Determines if the var_cross_section is flat.

	logical is_flat() const;

/**
 * @nodoc
 */
// Determines if the var_cross_section is tangent continuous.

	logical is_tangent_continuous() const;

/**
 * @nodoc
 */
// Determines if the var_cross_section is curvature continuous.

	logical is_curvature_continuous() const;

/**
 * @nodoc
 */
// Determines if the var_cross_section is symmetric about its midpoint

	logical is_symmetric() const;

// utilities:

	// Equality operators.
/**
 * Implements an assignment operator, which makes a copy of a <tt>var_cross_section</tt>.
 * <br><br>
 * @param other
 * var_cross_sec.
 */

	logical operator==( var_cross_section const &other ) const;
/**
 * Determines if two variable cross sections are unequal.
 * <br><br>
 * @param other
 * var_cross_sec.
 */

	logical operator!=( var_cross_section const &other ) const
		{ return !(*this==other); }

	// Transform operator.
/**
 * Performs the transform operation.
 * <br><br>
 * @param transf
 * transform.
 */

	virtual void operator*=( SPAtransf const &transf );

	// Return a measure of continuity at the ends.
/**
 * Returns a measure of continuity at the ends.
 */

	bl_continuity continuity() const;

	// Reverse the left-to-right properties of an asymmetric blend.
/**
 * Reverses the left-to-right properties of an asymmetric blend.
 */

	void reverse();

	// Calibrate (or un-calibrate) v-parameters, for a variable section.
/**
 * Calibrates the <i>v</i>-parameters of a <tt>var_cross_section</tt> object to the given parameter range.
 * <br><br>
 * <b>Role:</b> This only applies to cross-sections that vary with the <i>v</i>-parameter of the blend.
 * <br><br>
 * @param t0
 * start parameter.
 * @param t1
 * end parameter.
 */

	void calibrate_v_params( double t0, double t1 );
/**
 * Un-calibrates <i>v</i>-parameters, for a variable section.
 */

	void uncalibrate_v_params();

// save / restore:
/**
 * Saves the form data and thumb data.
 */
 	void save() const;
/*
// tbrv
*/
/**
 * @nodoc
 */
	friend DECL_KERN var_cross_section * restore_cross_section();
/**
 * Restores the data for a <tt>var_cross_section</tt> from a save file.
 */

	void restore_data();

	// STI ROLL
/*
// tbrv
*/
/**
 * @nodoc
 */

	virtual void full_size(SizeAccumulator&, logical = TRUE) const;
	// STI ROLL

// debug:
/**
 * Writes information about the <tt>var_cross_section</tt> to the debug file or to the specified file.
 * <br><br>
 * @param leader
 * title line.
 * @param fp
 * file pointer.
 */

	void debug( char const *leader, FILE *fp = debug_file_ptr ) const;
};

/*! @} */
#endif
