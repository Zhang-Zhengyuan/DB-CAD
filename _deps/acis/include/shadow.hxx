/*******************************************************************/
/*    Copyright (c) 1989-2020 by Spatial Corp.                     */
/*    All rights reserved.                                         */
/*    Protected by U.S. Patents 5,257,205; 5,351,196; 6,369,815;   */
/*                              5,982,378; 6,462,738; 6,941,251    */
/*    Protected by European Patents 0503642; 69220263.3            */
/*    Protected by Hong Kong Patent 1008101A                       */
/*******************************************************************/

#ifndef SHADOW_HXX
#define SHADOW_HXX

#include "api.hxx"
#include "dcl_covr.h"

class plane;

/**
 * @file shadow.hxx
 */

/*! \addtogroup BOOLPROJAPI 
 *  @{
 */

/**
 * Class to control results of shadow projection operation. 
 * Shadow body return type can be set through options
 * <ul>
 *	<li> SHADOW_SHEET_BODY - The output body is a sheet body. </li>
 *	<li> SHADOW_WIRE_BODY  - The output body is a wire body. </li>
 * </ul>
 * <br>
 * If computation of inner loops is set to true, holes in the shadow sheet bodies are returned.
 * If the shadow body expected is a wire body, wires corresponding to edges of hole loops are also returned. 
 * If computation of inner loops is set to false, sheet body having lumps with only periphery loops is returned. 
 * If the shadow body expected is a wire body, wires corresponding to edges of hole loops in shadow sheet body 
 * are not returned. 
 * <br> 
 * By default a sheet body with inner loops is returned. 
 */
class DECL_COVR make_shadow_options : public ACIS_OBJECT
{
public:

	/**
	 * Enumeration to specify type of body to be returned by @href api_make_shadow.
	 * @param SHADOW_SHEET_BODY
	 * The output body is a sheet body.
	 * @param SHADOW_WIRE_BODY
	 * The output body is a wire body.
	 */
	enum shadow_body_type 
	{ 
	   SHADOW_SHEET_BODY = 0,
	   SHADOW_WIRE_BODY
	};

private:
	
	shadow_body_type _shadow_type;
	bool _compute_inner_loops;  

public:

    /**
     * Default constructor. Sets shadow_body_type as SHADOW_SHEET_BODY
	 * and computation of inner loops to true.
     */
	make_shadow_options();

    /**
     * Destructor.
     */
	~make_shadow_options();
    
    /**
     * Set the type of the shadow body.
     */
    void set_shadow_body_type( shadow_body_type const &type );	

	/**
	 * Get the type of the shadow body.
	 */		
	shadow_body_type get_shadow_body_type() const;
	
	/**
     * Set the option to control computation of inner loops in shadow body.
	 * If the return body type is SHADOW_SHEET_BODY, the inner loops 
	 * are represented as holes in the sheet body. 		
     */
	void set_compute_inner_loops( bool const compute_inner_loops );	

	/**
	 * Get the option set to control computation of inner loops in shadow body.    
	 */	
	bool get_compute_inner_loops() const;	
};

/** 
 * Determines the shadow cast by an object(s) on a plane. 
 * <br><br>
 * <b>Role:</b> Given a point source of light at infinity, this API determines the shadow cast 
 * by a set of objects on the given plane. If the <tt>outcome</tt> is successful, the
 * result is returned as a sheet body or a wire body.
 * <br><br>
 * <b>Errors:</b> The input is not a valid solid body. 
 * <br><br>
 * <b>Limitations:</b> Non-manifold and sheet input bodies are not supported. 
 * <br><br>
 * <b>Effect:</b> Read-only on input. A new body is created.
 * <br><br>
 * <b>Journal:</b> Available
 * <br><br>
 * <b>Product(s):</b> 3D ACIS Modeler
 * <br><br>
 * @param bodies
 * A set of solid bodies
 * @param projection_plane
 * Shadow projection plane
 * @param shadow
 * Shadow of the input object(s) 
 * @param opts
 * Options for shadow projection. 
 * @param ao
 * ACIS options for versioning and journaling
 */
DECL_COVR outcome api_make_shadow(
							 ENTITY_LIST const &bodies,
							 plane const &projection_plane,
							 BODY *&shadow,
							 make_shadow_options const *opts = NULL,
							 AcisOptions const *ao = NULL
							 );

/** @} */

#endif
