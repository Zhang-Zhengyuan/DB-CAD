/*******************************************************************/
/*    Copyright (c) 1989-2020 by Spatial Corp.                     */
/*    All rights reserved.                                         */
/*    Protected by U.S. Patents 5,257,205; 5,351,196; 6,369,815;   */
/*                              5,982,378; 6,462,738; 6,941,251    */
/*    Protected by European Patents 0503642; 69220263.3            */
/*    Protected by Hong Kong Patent 1008101A                       */
/*******************************************************************/

// ------------------------------------------------------------------------------------------------
// Interfaces to repair the slices and hence make them more suitable for 3D Printing.
// ------------------------------------------------------------------------------------------------

#ifndef REPAIR_PSLICES_HXX
#define REPAIR_PSLICES_HXX

// ------------------------------------------------------------------------------------------------

/**
 * \defgroup REPAIRPSLICESAPI Repair Planar Slices
 * \ingroup HEALAPI
 */

/**
 * @file repair_pslices_api.hxx
 */

// ------------------------------------------------------------------------------------------------
// Includes:

#include "acis.hxx"
#include "dcl_heal.h"
#include "api.hxx"
#include "lists.hxx"
#include "boolean_handles.hxx"

// ------------------------------------------------------------------------------------------------
// Forward decls:

class repair_pslices_input_handle;
class repair_pslices_output_handle;
class repair_pslices_options;
class repair_pslices_options_data;
class AcisOptions;
class SPAunit_vector;

// ------------------------------------------------------------------------------------------------

/*! \addtogroup REPAIRPSLICESAPI
 *  \brief Declared at <repair_pslices_api.hxx>, SPAheal
 *  @{
 */

/**
 * Repairs the input planar slices and makes them more suitable for Additive Manufacturing.
 * <br><b>Technical Article:</b> <i>[Repair Planar Slices]()</i>
 * <br><br>
 * <b>Role:</b> This API identifies the anomalies in the input planar slices and repairs them.
 * The API creates and returns an <tt>repair_pslices_output_handle</tt> that can be further
 * queries using <tt>repair_pslices_output_query</tt>. By default this API runs in <i>failsafe</i>
 * mode and it can be turned off by setting "careful" option to <tt>TRUE</tt>. Under failsafe mode
 * (default) the API continues to process further slice layer if the repair process of any layer
 * fails. The list of bodies or lumps that couldn't be repaired can be obtained using
 * <tt>repair_pslices_output_query</tt>
 * <br><br>
 * <b><i>Note:</i></b> The handle to the repair planar slices output can not be directly consumed. 
 * An object of @href repair_pslices_output_query need to be instantiated with the output handle. 
 * The output handle must be released by calling <tt>remove_ref()</tt> after usage.
 * <br><br>
 * The options argument, <tt>rps_opts</tt>, allows the caller to pass additional information 
 * to this operation. Refer to @href repair_pslices_options for more details.
 * <br><br>
 * <b>Errors:</b><br>
 * <tt>rpsio</tt> is a NULL pointer or constructed with invalid input entities.<br>
 * <br><br>
 * <b>Effect:</b> Modifies the participating entities.
 * <br><br>
 * <b>Journal:</b> Available
 * <br><br>
 * <b>Product(s):</b> 3D ACIS Modeler, 3D InterOp
 * <br><br>
 * @param hrpsi
 * (in) Handle to repair planar slices. Refer to @href repair_pslices_input_handle for details.
 * @param hrpso
 * (out) Handle to the output of this operation. Refer to @href repair_pslices_output_handle for details.
 * @param rps_opts
 * (in) Repair planar slices options. Refer to @href repair_pslices_options for details.
 * @param ao
 * (in) ACIS options such as versioning and journaling. Refer to @href AcisOptions for details.
 **/

DECL_HEAL outcome
api_repair_planar_slices( repair_pslices_input_handle *hrpsi,
						  repair_pslices_output_handle *&hrpso,
						  repair_pslices_options const *rps_opts = NULL,
						  AcisOptions const *ao = NULL );

// ------------------------------------------------------------------------------------------------

/**
 * Represents a handle to the inputs given to Repair Slices operation.
 */

class DECL_HEAL repair_pslices_input_handle : public boolean_input_handle
{
public:

	/**
	 * Destructor
	 */

	virtual ~repair_pslices_input_handle();

protected:

	/**
	 * @nodoc
	 */
	// Default constructor. For internal use only!

	repair_pslices_input_handle();
};
// ------------------------------------------------------------------------------------------------

/**
 * Represents an interface to supply inputs to a Repair Slices operation.
 */

class DECL_HEAL repair_pslices_input_maker
{
public:

	/**
	 * Constructs an object of <tt>repair_pslices_input_maker</tt> with a single wire body 
	 * containing multiple slice layers. Each planar layer may contain multiple lumps but 
	 * shells in a single lump should not lie on multiple planes. The edges can have either
	 * straight or piecewise linear b-spline as geometry. The body shouldn't have a combination
	 * of both types of geometry though.
	 */

	repair_pslices_input_maker( BODY *wire_bdy );

	/**
	 * Constructs an object of <tt>repair_pslices_input_maker</tt> with list of wire-bodies 
	 * each representing a slice layer. Each body represents one planar layer and may contain
	 * multiple lumps. Lumps in a single body should not lie on multiple planes. The edges can have
	 * either straight or piecewise linear b-spline as geometry. The body shouldn't have a combination
	 * of both types of geometry though.
	 */

	repair_pslices_input_maker( ENTITY_LIST const& wire_bodies );

	/**
	 * Destructor
	 */

	~repair_pslices_input_maker();

	/**
	 * Returns a pointer of repair_pslices_input_handle that can be used to pass into 
	 * Repair Slices API. This call transfers the ownership of handle to the caller.
	 */

	repair_pslices_input_handle* unhook_handle();

private:
	
	// For internal use only!

	repair_pslices_input_handle *_h;
};
// ------------------------------------------------------------------------------------------------

/**
 * Represents a handle to the output from a Repair Planar Slices operation.
 * The user can have a shared ownership of this handle by increasing the reference count. 
 * After usage, the user must release the object by calling remove_ref().
 */

class DECL_HEAL repair_pslices_output_handle : public boolean_output_handle
{
protected:

	/**
	 * @nodoc
	 */
	// Default constructor.

	repair_pslices_output_handle();

	/**
	 * @nodoc
	 */
	// Destructor.

	virtual ~repair_pslices_output_handle();
};


/**
 * This class will provide user an interface to customize the behavior of Repair Slices operation.
 * Object of this class would be typically created by user on stack.
 */

class DECL_HEAL repair_pslices_options : public ACIS_OBJECT
{
public:

	/**
	 * Default constructor
	 */

	repair_pslices_options();

	/**
	 * Destructor
	 */

	~repair_pslices_options();

	/**
	 * Sets the resolution for the length of any linear edge or span of piecewise linear b-spline.
	 * The API will attempt to discard all segments of length smaller than this resolution.
	 * The default value is SPAresabs * 10.
	 */

	void set_res_segment_len( double res_seg_len );

	/**
	 * Sets the resolution for the area of any enclosed region. The API will attempt to discard 
	 * all closed wire shells that are smaller than this resolution.
	 * The default value is square of "SPAresabs * 10".
	 */
	
	void set_res_region_area( double res_regn_area );

	/**
	 * Sets the need to resolve the relative containment between circuits in a layer.
	 * The circuits are oriented accordingly and the output handle would contain the information
	 * about their relative containment.
	 */

	void set_need_containment( bool need_cont );

	/**
	 * Sets the choice of fixing the orientation of each slice layer. Peripheral circuits are oriented
	 * counter clockwise on the plane containing the layer, and inner circuits are oriented clockwise.
	 * The given plane normal is used to decide the orientation. It is caller's responsibility to 
	 * ensure that the given unit-vector is either parallel or antiparallel to the normals of the 
	 * planes containing the slice layers.
	 */

	void set_fix_orientation( bool fix_orient, SPAunit_vector const *pln_norm );

private:
	
	// Copy constructor and Assignment operators - to avoid duplication of objects of this class.
	
	repair_pslices_options( repair_pslices_options const & );
	repair_pslices_options& operator=( repair_pslices_options const & );

	// Data members.
	repair_pslices_options_data *_data;

	// Friends.
	friend class repair_pslices_options_data;
};
// ------------------------------------------------------------------------------------------------


/**
 * This class allows a user to query the output of Repair Planar Slices operation. It can provide the
 * repaired entities, which is either a single body or list bodies based on the inputs and the options
 * specified by user.
 */

class DECL_HEAL repair_pslices_output_query
{
public:

	/**
	 * Constructor to create an object of this class given an imprint_output_handle.
	 */

	repair_pslices_output_query( repair_pslices_output_handle const *h );

	/**
	 * Returns the list of processed slices. By default, if the input is a single body with 
	 * multiple slice layers then the output would be a single body too. 
	 * If need for containment is set to true as part of options then:
	 * - Each slice layer would be a separate body
	 * - Each body may have multiple lumps depending on the input
	 * - Each lump will have the first shell as the periphery circuit and the subsequent shells
	 *   are contained (2D) in the periphery shell.
	 * This function returns all the slices after processing and includes the ones that 
	 * couldn't be successfully repaired.
	 */

	void processed( ENTITY_LIST &ents ) const;

	/**
	 * Returns the list of slices, either bodies or lumps that could not be sucessfully processed. 
	 * These slices would have problems that the API could not successfully repair. If the output 
	 * is a single body with multiple lumps then this function returns list of lumps that 
	 * couldn't be repaired.
	 */

	void failed( ENTITY_LIST &ents ) const;


private:

	// For internal use only!
	repair_pslices_output_handle const *_hrpso;	
};
/** @} */
// ------------------------------------------------------------------------------------------------

#endif // REPAIR_SLICES_HXX

