/*******************************************************************/
/*    Copyright (c) 1989-2020 by Spatial Corp.                     */
/*    All rights reserved.                                         */
/*    Protected by U.S. Patents 5,257,205; 5,351,196; 6,369,815;   */
/*                              5,982,378; 6,462,738; 6,941,251    */
/*    Protected by European Patents 0503642; 69220263.3            */
/*    Protected by Hong Kong Patent 1008101A                       */
/*******************************************************************/

#ifndef WRAPAPI_HXX
#define WRAPAPI_HXX

#include "api.hxx"
#include "dcl_hlc.h"
#include "unitvec.hxx"

class plane;
class BODY; 
class ENTITY_LIST; 		
class SPAposition;
class SPAunit_vector;
class AcisOptions;

/**
 * @file wrapapi.hxx
 */

/*! \addtogroup WARPAPI 
 *  @{
 */

/**
 * Class to define the position and orientation of wrapping solution.
 * Normal supplied is ignored for @href wrap_orientation_ref corresponding to target.
 * Providing normal is optional except for input tool wire @href BODY. 
 */
class DECL_HLC wrap_orientation_ref : public ACIS_OBJECT
{
private:
	
	SPAposition _pos;		
	SPAunit_vector _tangent_dir;		
	SPAunit_vector _normal;		
	bool _normal_set;	
	wrap_orientation_ref();    
	
public:
	
	/**
     * Constructor that takes a reference position on the body
	 * and tangent direction in the tangent plane of the body at
	 * position supplied.	
	 */
	wrap_orientation_ref( SPAposition const &pos,
						  SPAunit_vector const &tangent_dir );    

	/**
     * Constructor that takes a reference position on the body
	 * and tangent direction in the tangent plane of the body 
	 * at normal to body at position supplied.	 	 
	 */
	wrap_orientation_ref( SPAposition const &pos,
						  SPAunit_vector const &tangent_dir,
						  SPAunit_vector const &normal );  
    /**
     * Destructor.
     */
	~wrap_orientation_ref();

	/**
     * Get the reference position set. 	 
	 */
	SPAposition get_reference_pos() const;

	/**
     * Get the reference tangent direction set. 	 
	 */
	SPAunit_vector get_reference_dir() const;

	/**
     * Get the reference normal. 
	 * Returns false is the normal is not set.
	 */
	bool get_reference_normal( SPAunit_vector &n ) const;

};
/**
* @nodoc
*/
class wrap_options_impl;
/**
 * Class to control results of wrapping operation. 
 * Target @href BODY can optionally modified by using @href set_embed_in_target
 * If this option is not set, a new wire @href BODY representing wrapping solution is returned. 
 * <br> 
 * By default a new wire @href BODY is returned.  
 */

class DECL_HLC wrap_options : public ACIS_OBJECT
{
private:	

	wrap_options_impl* m_impl;

public:

    /**
     * Default constructor.	 
     */
	wrap_options();

    /**
     * Destructor.
     */
	~wrap_options();
    
   	/**
     * Set the option to embed the wrapped solution in the target @href BODY. 
	 * If option is true, target body supplied is modified and returned. 
	 * If the option is false, a new wire @href BODY is returned. The default
	 * value is false.
     */
	void set_embed_in_target( bool embed );	

	/**
	 * Get the option set to control embedding of solution in target.    
	 */	
	bool get_embed_in_target() const;	

	/**
	* Returns the wrapped solution as a sheet body, by covering  the default wire solution with the
	* tool surface definitions. 
	* If the option is true, a new sheet body @href BODY is returned. The default
	* value is false.
	* Note: if embed option is on, this option is ignored.
	*/
	void set_return_as_sheet(const bool& on);
	/**
	* Get the option to return wrapped solution as sheet body.
	*/
	bool get_return_as_sheet() const;
	/**
	 * @nodoc
	 */
	wrap_options_impl* get_impl() const;
};

/** 
 * This API wraps a planar wire or sheet tool @href BODY on an target sheet 
 * or solid @href BODY. 
 * <br><br>
 * <b>Role:</b> This API determines wrapped body such that the stretch is minimum. 
 * Reference points and directions on the tool and target bodies define 
 * a mapping between the target and tool bodies. Face normal is internally used
 * to define the unique map. For wire tool body, normal must be supplied.  
 * API can optionally embed the wrapped body in the target body or return a separate wire body. 
 * The default behavior is to return the wrapped body without embedding.
 * To get best results, only the target faces on which solution is likely to be 
 * wrapped should be provided. Giving more or less faces than needed can affect quality and 
 * performance.
 * <br><br>
 * <b>Errors:</b> 
 * <ul> 
 * <li><tt>WRAP_POS_NOT_ON_TOOL</tt>: Tool position is not on the tool body</li>
 * <li><tt>WRAP_POS_NOT_ON_TARGET</tt>: Target position is not on the target body</li>
 * <li><tt>WRAP_WIRE_NORMAL_NOT_SUPPLIED</tt>: Normal vector is not supplied for wire tool body</li>
 * <li><tt>WRAP_INVALID_TOOL_DIR</tt>: Tool direction is not tangential to a tool face</li>
 * <li><tt>WRAP_INVALID_TARGET_DIR</tt>: Target direction is not tangential to a target body face</li>
 * <li><tt>WRAP_INVALID_TOOL_DIR_NOR</tt>: Tool direction and normal are not perpendicular</li>
 * <li><tt>WRAP_TOPOLOGY_COMPLEX</tt>: Solution region of target body makes a topologically higher genus surface</li>
 * <li><tt>WRAP_SHARP_EDGE</tt>: Wrapping across sharp edge attempted but failed</li>
 * <li><tt>WRAP_ENGULFS_TARGET</tt>: Tool body engulfs target body, a hole or a boundary in target body</li> 
 * <li><tt>WRAP_DIFFICULT_GEOMETRY</tt>: Difficult geometric conditions encountered. This typically happens because of high curvature regions</li> 
 * </ul>
 * <br><br>
 * <b>Limitations:</b> 
 * <ul>
 * <li>The solution region (set of faces) of the target body should not make a topological torus
 * or higher genus surface. 
 * The target body on which the tool is wrapped may be of higher genus. 
 * The wrapping solution is first attempted on the input faces supplied. 
 * The solution region is automatically grown if needed. However if the solution region makes 
 * a closed surface with genus greater than equal to 1, @href api_wrap fails. 
 * </li>
 * <li>Wrapping across sharp edges with angles between face normals more than 10 degrees
 * is not supported. The API attempts the wrapping across sharp edges, 
 * but if it encounters difficulties, it may fail.
 * </li> 
 * <li>The wrapped body should not hit an external boundary of target sheet body. </li>
 * <li>If the tool body is not planar, the results can be unexpected. 
 * The algorithm does not check the planarity of the tool body entities. 
 * </li>
 * <li>If the target body has more than one @href SHELL, a solution is attempted only on the
 * @SHELL determined by the face closest to the target position supplied.   
 * </li>
 * <li>The target faces supplied must be connected. </li>
 * <li>A non-manifold target body is not supported. </li> 
 * </ul>
 * <br><br>
 * <b>Effect:</b> Optionally modifies the input body or returns a new wire body. 
 * Input bodies are read only in case latter. 
 * <br><br>
 * <b>Journal:</b> Available
 * <br><br>
 * <b>Product(s):</b> 3D ACIS Modeler
 * <br><br>
 * @param tool
 * A sheet or wire tool body to be wrapped. It must be planar.  
 * @param tool_ref
 * This defines a reference position and orientation on the tool body. 
 * For a wire tool body, the tool position must be on the wire edges 
 * For a sheet tool body, the tool position must be a point in one of the tool faces.
 * Reference tool direction is a direction in the tangent plane to the 
 * sheet tool body at the tool position.
 * In the case of a wire tool body, the tool normal must be provided.
 * Tool direction and normal define a plane on which the tool body must lie. 
 * @param target_faces
 * Initial set of faces to start wrapping the tool on to.
 * @param target_ref
 * This defines the reference position and orientation on the target body. 
 * Target position must be on the target faces supplied. 
 * Target direction is a direction in the tangent plane to the target body 
 * at the target reference position. 
 * Normal, if provided, is ignored for target reference. It is always internally computed. 
 * @param wrapped_body
 * Result of wrapping. It is either the target body with the wrapping solution embedded or
 * a separate wire body. Use @href wrap_options to control the behavior.
 * @param opts
 * Used to control wrap solution embedding behavior.
 * @param ao
 * ACIS options for versioning and journaling
 */

DECL_HLC outcome api_wrap( BODY* tool, 
						   wrap_orientation_ref const &tool_ref,		
						   ENTITY_LIST const &target_faces, 								   
						   wrap_orientation_ref const &target_ref,						
						   BODY *&wrapped_body,
						   wrap_options *opts = NULL,
						   AcisOptions const *ao = NULL );


/** @} */

#endif
