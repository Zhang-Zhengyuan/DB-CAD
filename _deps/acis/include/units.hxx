/*******************************************************************/
/*    Copyright (c) 1989-2020 by Spatial Corp.                     */
/*    All rights reserved.                                         */
/*    Protected by U.S. Patents 5,257,205; 5,351,196; 6,369,815;   */
/*                              5,982,378; 6,462,738; 6,941,251    */
/*    Protected by European Patents 0503642; 69220263.3            */
/*    Protected by Hong Kong Patent 1008101A                       */
/*******************************************************************/
//----------------------------------------------------------------------
// purpose---
//   Define the UNITS_SCALE class.  A UNITS_SCALE ENTITY is used to
//   specify what units a model is defined in.  It contains a scale
//   factor which defines how to convert length units in the model into
//   millimeters.
//----------------------------------------------------------------------

#ifndef UNITS_SCALE_hxx
#define UNITS_SCALE_hxx
#include "dcl_part.h"
#include "entity.hxx"
/**
 * @file units.hxx
 * @CAA2Level L1
 * @CAA2Usage U1
 * \addtogroup PARTAPI
 *
 * @{
 */
/**
 * @nodoc
 */
ENTITY_IS_PROTOTYPE(UNITS_SCALE, PART )
#if 0
; // semicolon needed for mkman (doc tool) parsing
#endif
// Identifier used to find out (via identity() defined below) to what
// an entity pointer refers.

// For internal use only
/**
 * @nodoc
 */
extern DECL_PART int UNITS_SCALE_TYPE;

// Identifier that gives number of levels of derivation of this class
// from ENTITY.

#define UNITS_SCALE_LEVEL 1

// UNITS_SCALE declaration proper.
/**
 * Defines the <tt>UNITS_SCALE</tt> class.
 * <br>
 * <b>Role:</b> This class defines the <tt>UNITS_SCALE</tt> class.
 * A <tt>UNITS_SCALE</tt> <tt>ENTITY</tt> specifies the units in which a model is defined.
 * It contains a scale factor that defines how to convert length units in the model into millimeters.
 * <br><br>
 * <i><b>Note</b> The input and output scale factors are not currently used by ACIS.</i>
 * <br>
 * <dl>
 * <dt>This class uses the following scale factors:</dt>
 * <dd>model is the factor that converts from model units to millimeters.
 * The conversion is defined as:
 * <br>
 * (size in MM) = (model units size) * m_modelScale </dd>
 * <dd>input is the factor that enables the user to define a separate scale
 * that is used to convert units entered by the user into model units.
 * This value is normally 1. By using this value, the user could allow other
 * uses to enter values in inches, but they would be stored in meters.</dd>
 * <dd>output is the factor that enables the user to display values in units
 * that are different than those in which the model is stored.
 * This value is normally the inverse of the input scale factor.</dd>
 * </dl>
 */
class DECL_PART UNITS_SCALE: public ENTITY {

	// m_modelScale is used to convert from model units to millimeters.
	// The conversion is defined as:
	// (size in MM) = (size in model units) * m_modelScale
	double m_modelScale;

	// NOTE: The following two values are only provided for possible
	// future extensions.  Neither ACIS nor the 3D Toolkit currently
	// use these values.

	// This allows you to define a separate scale which can be used to
	// convert units entered by the user into model units.  This value
	// will normally be 1.0.  By using this value, you could do something
	// like let the user enter values in inches, but store them in meters.
	double m_inputScale;

	// This value lets you display values to the user in units that are
	// different than those in which the model is stored.  This value will
	// normally be the inverse of m_inputScale.
	double m_outputScale;

	/**
	 * @nodoc
	 */
	ENTITY_FUNCTIONS( UNITS_SCALE, PART )
#if 0
; // semicolon needed for mkman (doc tool) parsing)
#endif
// Because mkman isn't aware that ENTITY_FUNCTIONS changes access to "public", we do so
// explicitly here:
public:


public:

    //----------------------------------------
    // Constructors

	// The default constructor sets all scale factors to 1.0.  This means
	// that the model is stored in millimeters
/**
 * C++ allocation constructor requests memory for this object but does not populate it.
 * <br><br>
 * <b>Role:</b> The allocation constructor is used primarily by restore.
 * Applications should call this constructor only with the overloaded new operator,
 * because this reserves the memory on the heap, a requirement to support roll back
 * and history management.
 */
	UNITS_SCALE();

	// Create a UNITS_SCALE and specify the scale factors
/**
 * C++ initialize constructor requests memory for this object and populates it with the data supplied as arguments.
 * <br><br>
 * <b>Role:</b> Applications should call this constructor only with the
 * overloaded new operator, because this reserves the memory on the heap,
 * a requirement to support roll back and history management.
 * <br><br>
 * @param modelscale
 * scale of the model.
 * @param inputScale
 * input scale factor.
 * @param outputScale
 * output scale factor.
 */
	UNITS_SCALE(
		double modelScale,
		double inputScale = 1.0,
		double outputScale = 1.0
		);

    //----------------------------------------
    // Inquiry Functions

	// Get the various scale factors
/**
 * Returns the model scale factor.
 */
	double model_scale() const { return m_modelScale; }
/**
 * Returns the input scale factor.
 */

	double input_scale() const { return m_inputScale; }
/**
 * Returns the output scale factor.
 */
	double output_scale() const { return m_outputScale; }

    //----------------------------------------
    // Change Functions

	// Set the model units scale.  Note that this does not actually
	// change any of the model data.  It only changes how the data is
	// interpreted.  If you want the model to remain the same size,
	// you must also transform all of the ENTITIES in the model.
/**
 * Sets the model scale factor.
 * <br><br>
 * <b>Role:</b> This does not actually change any of the model data;
 * it only changes how the data is interpreted. If the user wants the
 * model to remain the same size, the user must also transform all of the entities in the model.
 * <br><br>
 * @param scale
 * model scale factor.
 */
	void set_model_scale(double scale);

	// The values for the input and output scales are relative to the
	// model units scale.  This means that in the normal case in which
	// the input and output units are the same as the model units, these
	// values do not have to be changed when the model units are changed.
	// If the input or output scale are not 1.0, then they will need to
	// be changed when the model units are changed.
/**
 * Sets the input scale factor.
 * <br><br>
 * <b>Role:</b>  This factor is relative to the model scale factor.
 * This means that in the normal case in which the input units are the same
 * as the model units, this value does not have to be changed when
 * the model units are changed. If the input scale is not 1,
 * then it needs to be changed when the model units are changed.
 * <br><br>
 * @param scale
 * input scale factor.
 */
	void set_input_scale(double scale);
/**
 * Sets the output scale factor.
 * <br><br>
 * <b>Role:</b>  This factor is relative to the model scale factor.
 * This means that in the normal case in which the output units are the
 * same as the model units, this value does not have to be changed when the
 * model units are changed. If the output scale is not 1, then it needs to be
 * changed when the model units are changed.
 * <br><br>
 * @param scale
 * output scale factor.
 */
	void set_output_scale(double scale);
};

//----------------------------------------------------------------------
/**
* Determines if the entity is an <tt>UNITS_SCALE</tt>.
* <br><br>
* <b>Effect:</b> Read-only
* <br><br>
* @param ent
* entity to test.
**/
DECL_PART logical is_UNITS_SCALE(const ENTITY* ent);

/** @} */

#endif
