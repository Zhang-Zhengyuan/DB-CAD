/*******************************************************************/
/*    Copyright (c) 1989-2020 by Spatial Corp.                     */
/*    All rights reserved.                                         */
/*    Protected by U.S. Patents 5,257,205; 5,351,196; 6,369,815;   */
/*                              5,982,378; 6,462,738; 6,941,251    */
/*    Protected by European Patents 0503642; 69220263.3            */
/*    Protected by Hong Kong Patent 1008101A                       */
/*******************************************************************/
//      Contains prototypes for advanced rendering husk api_ functions
#if !defined( RENDERER_API_HEADER_RB )
#define RENDERER_API_HEADER_RB
#include "base.hxx"
class ENTITY ;
class ENTITY_LIST ;
class BODY ;
class LUMP ;
class SHELL ;
class FACE ;
class SPAposition;
class RH_ENVIRONMENT_MAP;
class RH_TEXTURE_SPACE;
class RH_LIGHT;
class RH_BACKGROUND;
class RH_FOREGROUND;
class RH_MATERIAL ;
#include "dcl_rb.h"
#include "api.hxx"
#include "rnd_api.hxx"
#include "rh_args.hxx"
#include "rh_enty.hxx"
#include "rh_types.h"
#include "rgbcolor.hxx"
/**
 * \defgroup RENDERBASEAPI Rendering Base
 * \ingroup VISMODULE
 * Common interface from ACIS to all renderers, 
 * and is required if either interactive or batch rendering is to be performed on an ACIS model. 
 * It facilitates swapping out renderers for the requirements of different platforms. 
 * Because RBASE must support all renderers, its set of API functions may 
 * contain functionality not supported in some renderers
 */
/**
 * @file rnd_api.hxx
 * @CAA2Level L1
 * @CAA2Usage U1
 * \addtogroup RENDERBASEAPI
 *
 * @{
 */

class outcome;

//
//    Initialise renderer and bootstrap a minimum set of standard shaders
//    needed to support simple rendering
//

/**
 * Initializes the rendering library.
 * <br><br>
 * <b>Effect:</b> System routine
 * <br><br>
 * <b>Journal: </b> Not Available
 * <br><br>
 * <b>Product(s):</b> 3D ACIS Exchange, 3D Viz Exchange, 3D ACIS Modeler
 * <br><br>
**/
 DECL_RB outcome api_initialize_rendering();

/**
 * Terminates the rendering library.
 * <br><br>
 * <b>Effect:</b> System routine
 * <br><br>
 * <b>Journal: </b> Not Available
 * <br><br>
 * <b>Product(s):</b> 3D ACIS Exchange, 3D Viz Exchange, 3D ACIS Modeler
 * <br><br>
**/
 DECL_RB outcome api_terminate_rendering();

// Use api_initialize_rnd_husk_lib instead of api_rh_initialise
/**
 * Initializes the renderer.
 * <br><br>
 * <b>Role:</b> This <tt><tt>API</tt></tt> initializes the internal data structures required by the renderer.
 * Call this <tt><tt>API</tt></tt> before any other Rendering Base Component functions.
 * <br><br>
 * <b>Effect:</b> System routine
 * <br><br>
 * <b>Journal: </b> Not Available
 * <br><br>
 * <b>Product(s):</b> 3D ACIS Exchange, 3D Viz Exchange, 3D ACIS Modeler
 * <br><br>
**/
 DECL_RB outcome api_rh_initialise( ) ;

// Use api_terminate_rnd_husk_lib instead of api_rh_terminate
/**
 * Terminates the renderer.
 * <br><br>
 * <b>Role:</b> This <tt><tt>API</tt></tt> destroys the internal data structures required by the renderer.
 * It must be the final Rendering Base Component function call.
 * <br><br>
 * <b>Effect:</b> System routine
 * <br><br>
 * <b>Journal: </b> Not Available
 * <br><br>
 * <b>Product(s):</b> 3D ACIS Exchange, 3D Viz Exchange, 3D ACIS Modeler
 * <br><br>
**/
 DECL_RB outcome api_rh_terminate( ) ;

//
// Set and Get render mode
//
/**
 * Sets the current render mode.
 * <br><br>
 * <b>Role:</b>  Any subsequent rendering operations use the specified mode.
 * Refer to the chapter on Render Modes for a list of applicable modes.
 * <br><br>
 * <b>Effect:</b> System routine
 * <br><br>
 * <b>Journal: </b> Not Available
 * <br><br>
 * <b>Product(s):</b> 3D ACIS Exchange, 3D Viz Exchange, 3D ACIS Modeler
 * <br><br>
* @param render_mode
 * render mode to set.
 **/
 DECL_RB outcome api_rh_set_render_mode( Render_Mode render_mode ) ;


/**
 * Gets the current render mode.
 * <br><br>
 * <b>Role:</b> Refer to the chapter on Render Modes for a list of applicable modes.
 * <br><br>
 * <b>Effect:</b> Read-only
 * <br><br>
 * <b>Journal: </b> Not Available
 * <br><br>
 * <b>Product(s):</b> 3D ACIS Exchange, 3D Viz Exchange, 3D ACIS Modeler
 * <br><br>
* @param render_mode
 * returns render mode.
 **/
 DECL_RB outcome api_rh_get_render_mode( Render_Mode& render_mode ) ;

//
// Render control mode functions
//

/**
 * Sets a render control variable.
 * <br><br>
 * <b>Role:</b> The value given depends on the control variable being affected.
 * Refer to the chapter on Control Variables for a list of all control variable names.
 * <br><br>
 * <b>Effect:</b> System routine
 * <br><br>
 * <b>Journal: </b> Not Available
 * <br><br>
 * <b>Product(s):</b> 3D ACIS Exchange, 3D Viz Exchange, 3D ACIS Modeler
 * <br><br>
* @param var
 * control variable.
 * @param value
 * control value.
 **/
 DECL_RB outcome api_rh_set_control_variable
	(
	   Render_Control_Var var,
	   const Render_Arg    &value
	) ;

/**
 * Gets a render control variable.
 * <br><br>
 * <b>Role:</b> For a list of control variable names, refer to the chapter on Control Variables.
 * <br><br>
 * <b>Effect:</b> Read-only
 * <br><br>
 * <b>Journal: </b> Not Available
 * <br><br>
 * <b>Product(s):</b> 3D ACIS Exchange, 3D Viz Exchange, 3D ACIS Modeler
 * <br><br>
* @param var
 * control variable.
 * @param value
 * returns control variable's value.
 **/
 DECL_RB outcome api_rh_get_control_variable
	(
	    Render_Control_Var var,
	    Render_Arg    &value
	) ;
//
// Set and Get Scanline  format for image output
//
/**
 * Sets the current rendering output mode.
 * <br><br>
 * <b>Role:</b> Any subsequent rendering operations output scan lines in the format governed by
 * that mode. Refer to the chapter on Output Modes for a list of applicable output modes.
 * <br><br>
 * <b>Effect:</b> System routine
 * <br><br>
 * <b>Journal: </b> Not Available
 * <br><br>
 * <b>Product(s):</b> 3D ACIS Exchange, 3D Viz Exchange, 3D ACIS Modeler
 * <br><br>
* @param output_mode
 * output mode to set.
 **/
 DECL_RB outcome api_rh_set_output_mode( Output_Mode output_mode ) ;

/**
 * Gets the current output mode.
 * <br><br>
 * <b>Role:</b> Refer to the chapter on Output Modes for a list of applicable modes.
 * <br><br>
 * <b>Effect:</b> Read-only
 * <br><br>
 * <b>Journal: </b> Not Available
 * <br><br>
 * <b>Product(s):</b> 3D ACIS Exchange, 3D Viz Exchange, 3D ACIS Modeler
 * <br><br>
* @param output_mode
 * returns output mode.
 **/
DECL_RB outcome api_rh_get_output_mode( Output_Mode& output_mode ) ;

//
// Render one or more ACIS Geometric Entities
//
//  If clear_screen, then graphics region will be cleared before
//  render
//
/**
 * Renders a list of entities.
 * <br><br>
 * <b>Role:</b> This <tt><tt>API</tt></tt> renders one or more geometric entities of the type <tt>BODY</tt>, <tt>LUMP</tt>, <tt>SHELL</tt>, or <tt>FACE</tt>.
 * Facet the entities before rendering.
 * <br><br>
 * <b>Effect:</b> System routine
 * <br><br>
 * <b>Journal: </b> Not Available
 * <br><br>
 * <b>Product(s):</b> 3D ACIS Exchange, 3D Viz Exchange, 3D ACIS Modeler
 * <br><br>
* @param entities
 * entities to be rendered.
 * @param clear_screen
 * clear image before rendering.
 **/
DECL_RB outcome api_rh_render_entities
	(
	   ENTITY_LIST const& entities,
	   logical            clear_screen = TRUE
	) ;

//
//
//  Initialise basic shaders
//
//    This function initialises an additional set of basic shaders
//    provided as source code examples with LADS.
//

/**
 * Initializes the supplemental shaders.
 * <br><br>
 * <b>Role:</b> When the Rendering Base Component is initialized, the shaders supplied with the
 * Core Shader Library are automatically initialized.To initialize the larger set of shaders located
 * in the Supplementary Shader Library,call <tt>api_rh_initialise_supl_shaders</tt>.
 * <br><br>
 * <b>Effect:</b> System routine
 * <br><br>
 * <b>Journal: </b> Not Available
 * <br><br>
 * <b>Product(s):</b> 3D ACIS Exchange, 3D Viz Exchange, 3D ACIS Modeler
 * <br><br>
**/
DECL_RB outcome api_rh_initialise_supl_shaders( ) ;


//----------------------------------------------------------------------------
//
// RH_MATERIAL  Manipulation Functions
//
//----------------------------------------------------------------------------
/**
 * Creates a material.
 * <br><br>
 * <b>Role:</b> Each material contains four shaders, color, displacement, reflectance, and transparency,
 * and it is initialized to the default shaders specified using the <tt>api_rh_def_APIs</tt>. Once a material
 * is created, attach the material to entities using <tt>api_rh_set_material</tt>. Set a material's properties
 * by manipulating its four component shaders.
 * <br><br>
 * <b>Effect:</b> Changes model
 * <br><br>
 * <b>Journal: </b> Not Available
 * <br><br>
 * <b>Product(s):</b> 3D ACIS Exchange, 3D Viz Exchange, 3D ACIS Modeler
 * <br><br>
* @param material
 * new material returned.
 **/
DECL_RB outcome api_rh_create_material( RH_MATERIAL *& material ) ;

/**
 * Creates a copy of a material.
 * <br><br>
 * <b>Role:</b> This <tt><tt>API</tt></tt> creates a new <tt>RH_MATERIAL</tt> with the same properties as the old material.
 * <br><br>
 * <b>Effect:</b> Changes model
 * <br><br>
 * <b>Journal: </b> Not Available
 * <br><br>
 * <b>Product(s):</b> 3D ACIS Exchange, 3D Viz Exchange, 3D ACIS Modeler
 * <br><br>
* @param material
 * old material.
 * @param new_material
 * new material returned.
 **/
DECL_RB outcome api_rh_copy_material( RH_MATERIAL* material, RH_MATERIAL *& new_material) ;

/**
 * Deletes a material.
 * <br><br>
 * <b>Role:</b> Delete all materials that have been created before terminating the renderer.
 * <br><br>
 * <b>Errors:</b> Errors may occur if a material attached to one or more entities is deleted.
 * <br><br>
 * <b>Effect:</b> Changes model
 * <br><br>
 * <b>Journal: </b> Not Available
 * <br><br>
 * <b>Product(s):</b> 3D ACIS Exchange, 3D Viz Exchange, 3D ACIS Modeler
 * <br><br>
* @param material
 * material.
 **/
DECL_RB outcome api_rh_delete_material( RH_MATERIAL *material ) ;


//
// Set the Color Component of a RH_MATERIAL to a named color shader
//             eg. "plain" or "wrapped checker"
//
/**
 * Sets the color component of a material.
 * <br><br>
 * <b>Role:</b> An <tt>RH_MATERIAL</tt> comprises four component shaders: a color shader, a displacement shader,
 * a reflectance shader, and a transparency shader. When an instance of an <tt>RH_MATERIAL</tt> is created,
 * it is created with a default set of component shaders. This <tt>API</tt> changes the type of the color shader
 * to name.
 * <br><br>
 * <b>Errors:</b> The name of color component is not one of the supported types.
 * <br><br>
 * <b>Effect:</b> Changes model
 * <br><br>
 * <b>Journal: </b> Not Available
 * <br><br>
 * <b>Product(s):</b> 3D ACIS Exchange, 3D Viz Exchange, 3D ACIS Modeler
 * <br><br>
* @param material
 * material.
 * @param color_shader_name
 * name of color shader.
 **/
 DECL_RB outcome api_rh_set_color_comp
	(
	   RH_MATERIAL* material,
	   const char* color_shader_name
	);

//
// Get Attribute names, types and values of color component of a material
//

/**
 * Gets the arguments of a material's color component.
 * <br><br>
 * <b>Role:</b> Each material color component has a list of named arguments.The number and names
 * of those arguments depend on the type of color component.Access the value of each argument via
 * the <tt>Render_Arg</tt> class.
 * <br><br>
 * This <tt><tt>API</tt></tt> returns the name of the color component type with two lists containing the names and
 * values of those arguments.All returned pointer values point to internal data structures.
 * <br><br>
 * For efficiency, these values are not copied when the <tt>API</tt> is called.The application should not
 * free the memory pointed to by those pointers.In the case of <tt>Render_Arg</tt>, the internal array is
 * re-used by other inquiry functions.It is up to the application to copy those values, if required,
 * before any subsequent inquiry functions.
 * <br><br>
 * <b>Effect:</b> Read-only
 * <br><br>
 * <b>Journal: </b> Not Available
 * <br><br>
 * <b>Product(s):</b> 3D ACIS Exchange, 3D Viz Exchange, 3D ACIS Modeler
 * <br><br>
* @param material
 * material to be queried.
 * @param name
 * name of color component type.
 * @param no_of_args
 * number of arguments.
 * @param arg_names
 * array of argument names.
 * @param arg_values
 * returns array of arguments.
 **/
DECL_RB outcome api_rh_get_color_comp
	(
	   RH_MATERIAL*    material,
	   const char*       &name,
	   int         &no_of_args,
	   const char**      &arg_names,
	   Render_Arg* &arg_values
	) ;

//
// Set the Transparency Component of a RH_MATERIAL to a named transparency
// shader eg.   "plain" or "eroded"
//
/**
 * Sets a material's transparency component.
 * <br><br>
 * <b>Role:</b> An <tt>RH_MATERIAL</tt> comprises four component shaders: a color shader, a displacement
 * shader, a reflectance shader, and a transparency shader. When an instance of an <tt>RH_MATERIAL</tt> is
 * created, it is created with a default set of component shaders. This <tt><tt>API</tt></tt> changes the type of
 * the transparency shader to the specified name.
 * <br><br>
 * <b>Errors:</b> The name of the transparency component is not one of the supported types.
 * <br><br>
 * <b>Effect:</b> Changes model
 * <br><br>
 * <b>Journal: </b> Not Available
 * <br><br>
 * <b>Product(s):</b> 3D ACIS Exchange, 3D Viz Exchange, 3D ACIS Modeler
 * <br><br>
* @param material
 * material material.
 * @param transparency_shader_name
 * name of transparency shader.
 **/
 DECL_RB outcome api_rh_set_transp_comp
	(
	   RH_MATERIAL * material,
	   const char *transparency_shader_name
	) ;

//
// Get Attribute names, types and values of transparency component
//
/**
 * Gets the arguments of a material's transparency component.
 * <br><br>
 * <b>Role:</b> Each material transparency component has a list of named arguments.
 * The number and names of those arguments depend on the type of transparency component.Access the
 * value of each argument via the <tt>Render_Arg</tt> class.
 * <br><br>
 * This <tt><tt>API</tt></tt> returns the name of the transparency component type with two lists containing the names
 * and values of those arguments. All returned pointer values point to internal data structures.
 * <br><br>
 * For efficiency, these values are not copied when the <tt><tt>API</tt></tt> is called. The application should not free
 * the memory pointed to by those pointers. In the case of <tt>Render_Arg</tt>, the internal array is re-used
 * by other inquiry functions. It is up to the application to copy those values, if required, before
 * any subsequent inquiry functions.
 * <br><br>
 * <b>Effect:</b> Read-only
 * <br><br>
 * <b>Journal: </b> Not Available
 * <br><br>
 * <b>Product(s):</b> 3D ACIS Exchange, 3D Viz Exchange, 3D ACIS Modeler
 * <br><br>
* @param material
 * material to be queried.
 * @param name
 * returns name of transparency component type.
 * @param no_of_args
 * returns number of arguments.
 * @param arg_names
 * returns array of argument names.
 * @param arg_values
 * returns array of arguments.
 **/
DECL_RB outcome api_rh_get_transp_comp
	(
	   RH_MATERIAL*   material,
	   const char*       &name,
	   int         &no_of_args,
	   const char**      &arg_names,
	   Render_Arg* &arg_values
	) ;

//
// Set and Get status of Transparency
//
/**
 * Sets a material's transparency status.
 * <br><br>
 * <b>Role:</b> This <tt><tt>API</tt></tt> enables or disables a material's transparency shader.
 * The transparency component is used if its status is <tt>TRUE</tt>; otherwise, it is ignored.
 * <br><br>
 * <b>Effect:</b> Changes model
 * <br><br>
 * <b>Journal: </b> Not Available
 * <br><br>
 * <b>Product(s):</b> 3D ACIS Exchange, 3D Viz Exchange, 3D ACIS Modeler
 * <br><br>
* @param material
 * material.
 * @param status
 * TRUE (transparency on) or FALSE (transparency off).
 **/
 DECL_RB outcome api_rh_set_transp_status
	(
	   RH_MATERIAL *material,
	   logical   status
	) ;

/**
 * Gets the status of a material's transparency component.
 * <br><br>
 * <b>Role:</b> If the status of a transparency shader is on (<tt>TRUE</tt>), the <tt>RH_MATERIAL</tt> renders with
 * transparency for those rendering modes that support transparency.
 * <br><br>
 * <b>Effect:</b> Read-only
 * <br><br>
 * <b>Journal: </b> Not Available
 * <br><br>
 * <b>Product(s):</b> 3D ACIS Exchange, 3D Viz Exchange, 3D ACIS Modeler
 * <br><br>
* @param material
 * shader to be queried.
 * @param status
 * returns on/off status of shader.
 **/
DECL_RB outcome api_rh_get_transp_status
	(
	   RH_MATERIAL* material,
	   logical   &status
	) ;

//
// Set and Get status of Reflection
//

/**
 * Sets a material's reflection status.
 * <br><br>
 * <b>Role:</b> This <tt><tt>API</tt></tt> enables or disables a material's reflection shader.
 * The reflection component is used if its status is <tt>TRUE</tt>; otherwise, it is ignored.
 * <br><br>
 * <b>Effect:</b> Changes model
 * <br><br>
 * <b>Journal: </b> Not Available
 * <br><br>
 * <b>Product(s):</b> 3D ACIS Exchange, 3D Viz Exchange, 3D ACIS Modeler
 * <br><br>
* @param material
 * material.
 * @param status
 * toggle on/off.
 **/
DECL_RB outcome api_rh_set_reflect_status
	(
	   RH_MATERIAL *material,
	   logical   status
	) ;

/**
 * Gets the status of a material's reflection component.
 * <br><br>
 * <b>Role:</b> This <tt>API</tt> obtains the status of a material's reflection shader.
 * If the status of a reflection shader is on (<tt>TRUE</tt>), the <tt>RH_MATERIAL</tt> renders with reflections
 * for those rendering modes that support reflection.
 * <br><br>
 * <b>Effect:</b> Read-only
 * <br><br>
 * <b>Journal: </b> Not Available
 * <br><br>
 * <b>Product(s):</b> 3D ACIS Exchange, 3D Viz Exchange, 3D ACIS Modeler
 * <br><br>
* @param material
 * shader to query.
 * @param status
 * returns on/off shader status.
 **/
 DECL_RB outcome api_rh_get_reflect_status
	(
	   RH_MATERIAL* material,
	   logical   &status
	) ;

//
//  Set the Reflectance Component of a RH_MATERIAL to a named reflectance
//  shader eg. "matte" or "mirror"
//

/**
 * Sets a material's reflectance component.
 * <br><br>
 * <b>Role:</b> An <tt>RH_MATERIAL</tt> comprises four component shaders; a color shader, a displacemen
 * t shader, a reflectance shader, and a transparency shader. When an instance of an <tt>RH_MATERIAL</tt>
 * is created, it is created with a default set of component shaders.This <tt>API</tt> changes the type of
 * the reflectance shader to the specified name.
 * <br><br>
 * <b>Errors:</b> The name of the reflectance component is not one of the supported types.
 * <br><br>
 * <b>Effect:</b> Changes model
 * <br><br>
 * <b>Journal: </b> Not Available
 * <br><br>
 * <b>Product(s):</b> 3D ACIS Exchange, 3D Viz Exchange, 3D ACIS Modeler
 * <br><br>
* @param material
 * material.
 * @param reflectance_shader_name
 * name of reflectance shader.
 **/
 DECL_RB outcome api_rh_set_reflect_comp
	(
	   RH_MATERIAL *material,
	   const char *reflectance_shader_name
	) ;

//
// Get Attribute names, types and values of reflectance component
//

/**
 * Gets the arguments of a material's reflectance component.
 * <br><br>
 * <b>Role:</b> Each material reflectance component has a list of named
 * arguments. The number and names of those arguments depend on the type of
 * reflectance component. Access the value of each argument via the <tt>Render_Arg</tt> class.
 * <br><br>
 * This <tt>API</tt> returns the name of the reflectance component type with two lists containing the names and values of those arguments.
 * All returned pointer values point to internal data structures.
 * <br><br>
 * For efficiency, these values are not copied when the <tt>API</tt> is called.
 * The application should not free the memory pointed to by those pointers.
 * In the case  of <tt>Render_Arg</tt>, the internal array is re-used by other inquiry functions.
 * It is up to the application to copy those values, if required, before any subsequent inquiry functions.
 * <br><br>
 * <b>Effect:</b> Read-only
 * <br><br>
 * <b>Journal: </b> Not Available
 * <br><br>
 * <b>Product(s):</b> 3D ACIS Exchange, 3D Viz Exchange, 3D ACIS Modeler
 * <br><br>
* @param material
 * material to be queried.
 * @param name
 * returns name of reflectance component type.
 * @param no_of_args
 * returns number of arguments.
 * @param arg_names
 * returns array of argument names.
 * @param arg_values
 * returns array of arguments.
 **/
DECL_RB outcome api_rh_get_reflect_comp
	(
	   RH_MATERIAL*    material,
	   const char*       &name,
	   int         &no_of_args,
	   const char**      &arg_names,
	   Render_Arg* &arg_values
	) ;

//
// Set the Displacement Component of a RH_MATERIAL to a given displacement
// shader type eg. "rough" or "casting"
//
/**
 * Sets a material's displacement component.
 * <br><br>
 * <b>Role:</b> An <tt>RH_MATERIAL</tt> comprises four component shaders: a color shader, a displacement shader, a reflectance shader, and a transparency shader.
 * When an instance of an <tt>RH_MATERIAL</tt> is created, it is created with a default set of component shaders.
 * This <tt>API</tt> changes the type of the displacement shader to name.
 * <br><br>
 * <b>Errors:</b> The name of the displacement component is not one of the supported types.
 * <br><br>
 * <b>Effect:</b> Changes model
 * <br><br>
 * <b>Journal: </b> Not Available
 * <br><br>
 * <b>Product(s):</b> 3D ACIS Exchange, 3D Viz Exchange, 3D ACIS Modeler
 * <br><br>
* @param material
 * material.
 * @param name
 * name of displacement shader.
 **/
DECL_RB outcome api_rh_set_displace_comp
	(
	   RH_MATERIAL* material,
	   const char*  name
	) ;
//
// Get Attribute names, types and values of displacement component
//
/**
 * Gets the arguments of a material's displacement component.
 * <br><br>
 * <b>Role:</b> Each material displacement component has a list of named arguments. The number and
 * names of those arguments depend on the type of displacement component. Access the value of each
 * argument via the <tt>Render_Arg</tt> class.
 * <br><br>
 * This <tt>API</tt> returns the name of the displacement component type with two lists containing the
 * names and values of those arguments. All returned pointer values point to internal data structures.
 * <br><br>
 * For efficiency, these values are not copied when the <tt>API</tt> is called. The application should not
 * free the memory pointed to by those pointers.In the case of <tt>Render_Arg</tt>, the internal array
 * is re-used by other inquiry functions.It is up to the application to copy those values,
 * if required, before any subsequent inquiry functions.
 * <br><br>
 * <b>Effect:</b> Read-only
 * <br><br>
 * <b>Journal: </b> Not Available
 * <br><br>
 * <b>Product(s):</b> 3D ACIS Exchange, 3D Viz Exchange, 3D ACIS Modeler
 * <br><br>
* @param material
 * material to be queried.
 * @param name
 * returns name of displacement component type.
 * @param no_of_args
 * returns number of arguments.
 * @param arg_names
 * returns array of argument names.
 * @param arg_values
 * returns array of arguments.
 **/
DECL_RB outcome api_rh_get_displace_comp
	(
	   RH_MATERIAL *material,
	   const char  *& name,
	   int         &no_of_args,
	   const char **& arg_names,
	   Render_Arg *& arg_values
	) ;

//
// Set and Get status of displacement
//
//    If on is TRUE then the displacement component a RH_MATERIAL
//    will take effect for those rendering modes where it is
//    applicable
//
/**
 * Sets the status of a material's displacement component.
 * <br><br>
 * <b>Role:</b> This <tt>API</tt> enables or disables the defined displacement component of a material.
 * The <tt>API</tt> uses the displacement component if its status is <tt>TRUE</tt>; otherwise, it is ignored.
 * <br><br>
 * <b>Effect:</b> Changes model
 * <br><br>
 * <b>Journal: </b> Not Available
 * <br><br>
 * <b>Product(s):</b> 3D ACIS Exchange, 3D Viz Exchange, 3D ACIS Modeler
 * <br><br>
* @param material
 * material to set.
 * @param status
 * TRUE (displacement on) or FALSE (displacement off).
 **/
DECL_RB outcome api_rh_set_displace_status( RH_MATERIAL *material, logical status ) ;


/**
 * Gets the status of a material's displacement component.
 * <br><br>
 * <b>Role:</b> This <tt>API</tt> obtains the status of a material's displacement shader.
 * If the status of a displacement shader is <tt>TRUE</tt>, the entity renders with displacement for those rendering modes that support displacement.
 * <br><br>
 * <b>Effect:</b> Read-only
 * <br><br>
 * <b>Journal: </b> Not Available
 * <br><br>
 * <b>Product(s):</b> 3D ACIS Exchange, 3D Viz Exchange, 3D ACIS Modeler
 * <br><br>
* @param material
 * material to be queried.
 * @param status
 * returns displacement status.
 **/
DECL_RB outcome api_rh_get_displace_status( RH_MATERIAL *material, logical &status ) ;

//
// Material component enquiry functions
//
//     These functions return a pointer to a list of names of
//     all currently available  material components for color,
//     transparency,  reflectance and  displacement, respectively.
//

/**
 * Gets a list of valid color component names.
 * <br><br>
 * <b>Role:</b> The list should not be freed by the application.
 * <br><br>
 * <b>Effect:</b> Read-only
 * <br><br>
 * <b>Journal: </b> Not Available
 * <br><br>
 * <b>Product(s):</b> 3D ACIS Exchange, 3D Viz Exchange, 3D ACIS Modeler
 * <br><br>
* @param n_colors
 * returns length of list of names.
 * @param names
 * returns list of names.
 **/
 DECL_RB outcome api_rh_get_color_comp_list
	(
	   int    &n_colors,
	   const char **&names
	) ;


/**
 * Gets a list of valid transparency component names.
 * <br><br>
 * <b>Role:</b> The list should not be freed by the application.
 * <br><br>
 * <b>Effect:</b> Read-only
 * <br><br>
 * <b>Journal: </b> Not Available
 * <br><br>
 * <b>Product(s):</b> 3D ACIS Exchange, 3D Viz Exchange, 3D ACIS Modeler
 * <br><br>
* @param n_transparencies
 * returns number of transparencies.
 * @param names
 * returns list of names.
 **/
 DECL_RB outcome api_rh_get_transp_comp_list
	(
	   int    &n_transparencies,
	   const char** &names
	) ;

/**
 * Gets a list of valid reflectance component names.
 * <br><br>
 * <b>Role:</b> This <tt>API</tt> points to a list of valid <tt>RH_MATERIAL</tt> reflectance component names.
 * The list should not be freed by the application.
 * <br><br>
 * <b>Effect:</b> Read-only
 * <br><br>
 * <b>Journal: </b> Not Available
 * <br><br>
 * <b>Product(s):</b> 3D ACIS Exchange, 3D Viz Exchange, 3D ACIS Modeler
 * <br><br>
* @param n_reflects
 * returns number of names.
 * @param names
 * returns list of names.
 **/
DECL_RB outcome api_rh_get_reflect_comp_list
	(
	   int    &n_reflects,
	   const char** &names
	) ;

/**
 * Gets a list of valid displacement component names.
 * <br><br>
 * <b>Role:</b> The list should not be freed by the application.
 * <br><br>
 * <b>Effect:</b> Read-only
 * <br><br>
 * <b>Journal: </b> Not Available
 * <br><br>
 * <b>Product(s):</b> 3D ACIS Exchange, 3D Viz Exchange, 3D ACIS Modeler
 * <br><br>
* @param n_displaces
 * returns length of list of names.
 * @param names
 * returns list of names.
 **/
DECL_RB outcome api_rh_get_displace_comp_list
	(
	   int    &n_displaces,
	   const char** &names
	) ;


//
// Material component argument functions
//
//     Functions set argument values  for color, transparency,
//     reflectanced  displacement components of material
//

/**
 * Sets an argument of a material's color component.
 * <br><br>
 * <b>Role:</b> This <tt>API</tt> sets arguments controlling the color component specified by name.
 * name must correspond to one of the supported arguments for the type of color component
 * associated with the <tt>RH_MATERIAL</tt>. The underlying argument type depends on the argument set with
 * the <tt>Render_Arg</tt> class.
 * <br><br>
 * <b>Effect:</b> Changes model
 * <br><br>
 * <b>Journal: </b> Not Available
 * <br><br>
 * <b>Product(s):</b> 3D ACIS Exchange, 3D Viz Exchange, 3D ACIS Modeler
 * <br><br>
* @param material
 * material.
 * @param arg_name
 * name of argument.
 * @param value
 * argument value.
 **/
 DECL_RB outcome api_rh_set_color_arg
	(
	   RH_MATERIAL*      material,
	   const char*       arg_name,
	   const Render_Arg &value
	) ;

/**
 * Sets an argument of a material's transparency component.
 * <br><br>
 * <b>Role:</b> This <tt>API</tt> sets arguments controlling the transparency component specified by name.
 * name must correspond to one of the supported arguments for the type of transparency component
 * associated with <tt>RH_MATERIAL</tt>. The underlying argument type depends on the argument set with
 * the <tt>Render_Arg</tt> class.
 * <br><br>
 * <b>Effect:</b> Changes model
 * <br><br>
 * <b>Journal: </b> Not Available
 * <br><br>
 * <b>Product(s):</b> 3D ACIS Exchange, 3D Viz Exchange, 3D ACIS Modeler
 * <br><br>
* @param material
 * material.
 * @param arg_name
 * name of argument.
 * @param value
 * argument value.
 **/
DECL_RB outcome api_rh_set_transp_arg
	(
	   RH_MATERIAL*       material,
	   const char*        arg_name,
	   const Render_Arg  &value
	) ;

/**
 * Sets an argument of a material's reflectance component.
 * <br><br>
 * <b>Role:</b> This <tt>API</tt> sets arguments controlling the reflectance component specified by name.
 * name must correspond to one of the supported arguments for the type of reflectance component
 * associated with the <tt>RH_MATERIAL</tt>.The underlying argument type depends on the argument set with
 * the <tt>Render_Arg</tt> class.
 * <br><br>
 * <b>Effect:</b> Changes model
 * <br><br>
 * <b>Journal: </b> Not Available
 * <br><br>
 * <b>Product(s):</b> 3D ACIS Exchange, 3D Viz Exchange, 3D ACIS Modeler
 * <br><br>
* @param material
 * material.
 * @param arg_name
 * name of argument.
 * @param value
 * argument value.
 **/
DECL_RB outcome api_rh_set_reflect_arg
	(
	   RH_MATERIAL*       material,
	   const char*        arg_name,
	   const Render_Arg  &value
	) ;


/**
 * Sets an argument of a material's displacement component.
 * <br><br>
 * <b>Role:</b> This <tt>API</tt> sets arguments controlling the displacement component  specified by name.
 * name must correspond to one of the supported arguments for the type of displacement component
 * associated with the <tt>RH_MATERIAL</tt>.The underlying argument type depends on the argument set with
 * the <tt>Render_Arg</tt> class.
 * <br><br>
 * <b>Effect:</b> Changes model
 * <br><br>
 * <b>Journal: </b> Not Available
 * <br><br>
 * <b>Product(s):</b> 3D ACIS Exchange, 3D Viz Exchange, 3D ACIS Modeler
 * <br><br>
* @param material
 * material.
 * @param arg_name
 * name of argument.
 * @param value
 * argument value.
 **/
DECL_RB outcome api_rh_set_displace_arg
	(
	   RH_MATERIAL*       material,
	   const char*        arg_name,
	   const Render_Arg  &value
	) ;

//
// Set the default component names for a new RH_MATERIAL
//
// RH_MATERIALs created by subsequent calls to api_rh_create_material
// will contain the component types  set using these functions
//

/**
 * Sets a material's default color component.
 * <br><br>
 * <b>Role:</b> This <tt>API</tt> sets the default color shader to one of the known color shaders,
 * obtained using <tt>api_rh_get_color_comp_list</tt>.
 * <br><br>
 * An <tt>RH_MATERIAL</tt> is defined by four component shaders: a color shader, a displacement shader,
 * a reflectance shader, and a transparency shader. When an instance of an <tt>RH_MATERIAL</tt> is created,
 * the type of shader components that it is initially created with depends on the default values
 * set using this and other default shader component functions.
 * <br><br>
 * <b>Errors:</b> The name of the color shader is not one of the supported types.
 * <br><br>
 * <b>Effect:</b> System routine
 * <br><br>
 * <b>Journal: </b> Not Available
 * <br><br>
 * <b>Product(s):</b> 3D ACIS Exchange, 3D Viz Exchange, 3D ACIS Modeler
 * <br><br>
* @param name
 * name of color shader.
 **/
 DECL_RB outcome api_rh_def_color_comp( const char *name ) ;

/**
 * Sets a material's default transparency component.
 * <br><br>
 * <b>Role:</b> This <tt>API</tt> sets the default transparency shader to one of the known transparency shaders,
 * obtained using <tt>api_rh_get_transp_comp_list</tt>.
 * <br><br>
 * An <tt>RH_MATERIAL</tt> is defined by four component shaders: a color shader, a displacement shader,
 * a reflectance shader, and a transparency shader. When an instance of an <tt>RH_MATERIAL</tt> is created,
 * the type of shader components that it is initially created with depends on the default values
 * set using this and other  default shader component functions.
 * <br><br>
 * <b>Errors:</b> Name of transparency shader is not one of the supported types.
 * <br><br>
 * <b>Effect:</b> System routine
 * <br><br>
 * <b>Journal: </b> Not Available
 * <br><br>
 * <b>Product(s):</b> 3D ACIS Exchange, 3D Viz Exchange, 3D ACIS Modeler
 * <br><br>
* @param name
 * name of transparency shader.
 **/
 DECL_RB outcome api_rh_def_transp_comp( const char *name ) ;

/**
 * Sets a material's default displacement component.
 * <br><br>
 * <b>Role:</b> This <tt>API</tt> sets the default reflectance shader to one of the known reflectance shader
 * names, obtained using <tt>api_rh_get_reflect_comp_list</tt>.
 * <br><br>
 * An <tt>RH_MATERIAL</tt> is defined by four component shaders: a color shader, a displacement shader,
 * a reflectance shader, and a transparency shader. When an instance of an <tt>RH_MATERIAL</tt> is created,
 * the type of shader components that it is initially created with depends on the default values
 * set using this and other default shader component functions.
 * <br><br>
 * <b>Errors:</b> The name of reflectance shader is not one of the supported types.
 * <br><br>
 * <b>Effect:</b> System routine
 * <br><br>
 * <b>Journal: </b> Not Available
 * <br><br>
 * <b>Product(s):</b> 3D ACIS Exchange, 3D Viz Exchange, 3D ACIS Modeler
 * <br><br>
* @param name
 * name of displacement shader.
 **/
 DECL_RB outcome api_rh_def_reflect_comp( const char *name ) ;

/**
 * Sets a material's default displacement component.
 * <br><br>
 * <b>Role:</b> This <tt>API</tt> sets the default displacement shader to one of the known displacement shader
 * names, obtained using <tt>api_rh_get_displace_comp_list</tt>.
 * <br><br>
 * An <tt>RH_MATERIAL</tt> is defined by four component shaders: a color shader, a displacement shader,
 * a reflectance shader, and a transparency shader. When an instance of an <tt>RH_MATERIAL</tt> is created,
 * the type of shader components that it is initially created with depends on the default values
 * set using this and other  default shader component functions.
 * <br><br>
 * <b>Errors:</b> The name of displacement shader is not one of the supported types.
 * <br><br>
 * <b>Effect:</b> System routine
 * <br><br>
 * <b>Journal: </b> Not Available
 * <br><br>
 * <b>Product(s):</b> 3D ACIS Exchange, 3D Viz Exchange, 3D ACIS Modeler
 * <br><br>
* @param name
 * name of displacement shader.
 **/
 DECL_RB outcome api_rh_def_displace_comp( const char *name ) ;

//----------------------------------------------------------------------------
//
// RH_LIGHT  Manipulation Functions
//
//----------------------------------------------------------------------------

/**
 * Creates a light.
 * <br><br>
 * <b>Role:</b> This <tt>API</tt> creates a new <tt>RH_LIGHT</tt>.
 * Once it is created, set the light properties using <tt>api_rh_set_light_arg</tt>.
 * Obtain a list of supported light types by calling <tt>api_rh_get_light_types</tt>.
 * <br><br>
 * <b>Errors:</b> The name of the light is not one of the supported types.
 * <br><br>
 * <b>Effect:</b> Changes model
 * <br><br>
 * <b>Journal: </b> Not Available
 * <br><br>
 * <b>Product(s):</b> 3D ACIS Exchange, 3D Viz Exchange, 3D ACIS Modeler
 * <br><br>
* @param type
 * type of light.
 * @param light
 * new light returned.
 **/
 DECL_RB outcome api_rh_create_light( const char *type, RH_LIGHT *&light ) ;

/**
 * Creates a copy of a light.
 * <br><br>
 * <b>Role:</b> This <tt>API</tt> creates a new <tt>RH_LIGHT</tt> with the same properties as the old light.
 * <br><br>
 * <b>Effect:</b> Changes model
 * <br><br>
 * <b>Journal: </b> Not Available
 * <br><br>
 * <b>Product(s):</b> 3D ACIS Exchange, 3D Viz Exchange, 3D ACIS Modeler
 * <br><br>
* @param light
 * old light.
 * @param new_light
 * new light returned.
 **/
DECL_RB outcome api_rh_copy_light( RH_LIGHT *light, RH_LIGHT *&new_light ) ;

/**
 * Deletes a light.
 * <br><br>
 * <b>Role:</b> When a light is deleted,do not use its pointer in subsequent calls of
 * <tt>api_rh_set_light_list</tt>. If a light is deleted from the rendering library's current light list,
 * reset the light list by calling <tt>api_rh_set_light_list</tt>.
 * <br><br>
 * <b>Effect:</b> Changes model
 * <br><br>
 * <b>Journal: </b> Not Available
 * <br><br>
 * <b>Product(s):</b> 3D ACIS Exchange, 3D Viz Exchange, 3D ACIS Modeler
 * <br><br>
* @param light
 * light.
 **/
DECL_RB outcome api_rh_delete_light( RH_LIGHT* light ) ;


//
// Get a list of available light type names
//
/**
 * Gets a list of valid light type names.
 * <br><br>
 * <b>Role:</b> The list should not be freed by the application.
 * <br><br>
 * <b>Effect:</b> Read-only
 * <br><br>
 * <b>Journal: </b> Not Available
 * <br><br>
 * <b>Product(s):</b> 3D ACIS Exchange, 3D Viz Exchange, 3D ACIS Modeler
 * <br><br>
* @param n_types
 * returns number of types of lights.
 * @param names
 * returns list of names.
 **/
DECL_RB outcome api_rh_get_light_types( int& n_types, const char** &names ) ;


//
//  Create a shadow map for a particular light and list
//  of ACIS Geometric entities
//
/**
 * Creates a shadow mask for a light from a list of entities.
 * <br><br>
 * <b>Role:</b> A shadow mask is a depth buffered image computed from the view point of a light source.
 * Shadow masks help render shadows. The resolution of the shadow image depends upon the shadow
 * resolution parameter associated with the light source shader.Set the resolution
 * using <tt>api_rh_set_light_arg</tt>.
 * <br><br>
 * <b>Limitations:</b> Recompute a shadow map if certain parameters of the light, such as location,
 * are changed or if the geometry of the objects in a scene has altered.
 * <br><br>
 * <b>Effect:</b> Changes model
 * <br><br>
 * <b>Journal: </b> Not Available
 * <br><br>
 * <b>Product(s):</b> 3D ACIS Exchange, 3D Viz Exchange, 3D ACIS Modeler
 * <br><br>
* @param light
 * light.
 * @param entities
 * light entity list.
 **/
DECL_RB outcome api_rh_create_light_shadow
	(
	   RH_LIGHT *light,
	   ENTITY_LIST const &entities
	) ;

/**
 * Deletes a light's shadow.
 * <br><br>
 * <b>Role:</b> This <tt>API</tt> deletes the shadow map associated with the light. Obtain the light handle
 * by previously calling <tt>api_rh_create_light</tt> or <tt>api_rh_copy_ligh</tt>t.
 * To create a shadow map for a light, call <tt>api_rh_create_light_shadow</tt>.
 * <br><br>
 * If this <tt>API</tt> is called with an invalid light or if the light is not currently  maintaining
 * a shadow map, the call is ignored.
 * <br><br>
 * <b>Effect:</b> Changes model
 * <br><br>
 * <b>Journal: </b> Not Available
 * <br><br>
 * <b>Product(s):</b> 3D ACIS Exchange, 3D Viz Exchange, 3D ACIS Modeler
 * <br><br>
* @param light
 * light.
 **/
DECL_RB outcome api_rh_delete_light_shadow( RH_LIGHT *light ) ;

//
// Set and Get a list of current lights
//

/**
 * Sets the active light list.
 * <br><br>
 * <b>Role:</b> The Rendering Base Component maintains an internal list of  active lights that are
 * used as light sources when an image is rendered.This <tt>API</tt> sets the elements of that list.
 * At least one <tt>RH_LIGHT</tt> must reside in the active light list; otherwise, shaded entities are not
 * visible.
 * <br><br>
 * <b>Effect:</b> System routine
 * <br><br>
 * <b>Journal: </b> Not Available
 * <br><br>
 * <b>Product(s):</b> 3D ACIS Exchange, 3D Viz Exchange, 3D ACIS Modeler
 * <br><br>
* @param lights
 * list of lights.
 **/
 DECL_RB outcome api_rh_set_light_list( ENTITY_LIST const &lights ) ;


/**
 * Gets the contents of the active light list.
 * <br><br>
 * <b>Role:</b> The Rendering Base Component maintains an internal list of active lights that are
 * used as light sources when an image is rendered.This <tt>API</tt> gets a copy of the elements of that list.
 * At least one <tt>RH_LIGHT</tt> must reside in the active light list; otherwise, shaded entities are not
 * visible. The <tt>API</tt> adds entries to the <tt>ENTITY_LIST</tt> provided by the application developer.
 * It is up to the application to clean out the list.
 * <br><br>
 * <b>Effect:</b> Read-only
 * <br><br>
 * <b>Journal: </b> Not Available
 * <br><br>
 * <b>Product(s):</b> 3D ACIS Exchange, 3D Viz Exchange, 3D ACIS Modeler
 * <br><br>
* @param lights
 * returns list of light entities, expected to be empty when specified to function.
 **/
DECL_RB outcome api_rh_get_light_list( ENTITY_LIST& lights ) ;

//
// Manipulate the light arguments
//
/**
 * Gets the arguments of a light.
 * <br><br>
 * <b>Role:</b> Each light has a list of named arguments.
 * The number and names of those arguments depend on the type of light.
 * Access the value of each argument via the <tt>Render_Arg</tt> class.
 * <br><br>
 * This <tt>API</tt> returns the name of the light type with two lists containing the names and values
 * of those arguments. All returned pointer values point to internal data structures.
 * <br><br>
 * For efficiency, these values are not copied when the <tt>API</tt> is called.
 * The application should not free the memory pointed to by those pointers.
 * In the case of <tt>Render_Arg</tt>, the internal array is re-used by other inquiry functions.
 * It is up to the application to copy those values, if required, before any subsequent inquiry
 * functions.
 * <br><br>
 * <b>Effect:</b> Read-only
 * <br><br>
 * <b>Journal: </b> Not Available
 * <br><br>
 * <b>Product(s):</b> 3D ACIS Exchange, 3D Viz Exchange, 3D ACIS Modeler
 * <br><br>
* @param light
 * light to be queried.
 * @param name
 * returns name of light type.
 * @param no_of_args
 * returns number of arguments.
 * @param arg_names
 * returns array of argument names.
 * @param arg_values
 * returns array of arguments.
 **/
DECL_RB outcome api_rh_get_light_args
	(
	   RH_LIGHT*       light,
	   const char*       &name,
	   int         &no_of_args,
	   const char**      &arg_names,
	   Render_Arg* &arg_values
	) ;


/**
 * Sets an argument for a light.
 * <br><br>
 * <b>Role:</b> This <tt>API</tt> sets sets arguments controlling the light specified by name.name must
 * correspond to one of the supported arguments for the type of light associated with <tt>RH_LIGHT</tt>.
 * The underlying argument type depends on the argument set with the <tt>Render_Arg</tt> class.
 * <br><br>
 * <b>Effect:</b> Changes model
 * <br><br>
 * <b>Journal: </b> Not Available
 * <br><br>
 * <b>Product(s):</b> 3D ACIS Exchange, 3D Viz Exchange, 3D ACIS Modeler
 * <br><br>
* @param light
 * light.
 * @param arg_name
 * name of argument.
 * @param value
 * argument value.
 **/

DECL_RB outcome api_rh_set_light_arg
	(
	   RH_LIGHT*         light,
	   const char*       arg_name,
	   const Render_Arg &value
	);


//
// Manipulate the light on/off state
//

/**
 * Gets the on/off state of light.
 * <br><br>
 * <b>Role:</b> This <tt>API</tt> gets the active state of the <tt>RH_LIGHT</tt> entity.
 * <br><br>
 * <b>Effect:</b> Read-only
 * <br><br>
 * <b>Journal: </b> Not Available
 * <br><br>
 * <b>Product(s):</b> 3D ACIS Exchange, 3D Viz Exchange, 3D ACIS Modeler
 * <br><br>
* @param light
 * light to be queried.
 * @param on_off
 * returns on/off state of light.
 **/
DECL_RB outcome api_rh_get_light_state
	(
	   RH_LIGHT*         light,
	   logical*          on_off
	);


/**
 * Sets the on/off state for a light.
 * <br><br>
 * <b>Role:</b> This <tt>API</tt> sets the active state of the <tt>RH_LIGHT</tt> entity.
 * <br><br>
 * <b>Effect:</b> Changes model
 * <br><br>
 * <b>Journal: </b> Not Available
 * <br><br>
 * <b>Product(s):</b> 3D ACIS Exchange, 3D Viz Exchange, 3D ACIS Modeler
 * <br><br>
* @param light
 * light.
 * @param on_off
 * on/off state for the light.
 **/
DECL_RB outcome api_rh_set_light_state
	(
	   RH_LIGHT *         light,
	   logical           on_off
	);

//-----------------------------------------------------------------------------
//
// RH_BACKGROUND Manipulation Functions
//
//-----------------------------------------------------------------------------

/**
 * Gets a list of valid background type names.
 * <br><br>
 * <b>Role:</b> This <tt>API</tt> points to a list of valid <tt>RH_BACKGROUND</tt> type names.
 * The list should not be freed by the application.
 * <br><br>
 * <b>Effect:</b> Read-only
 * <br><br>
 * <b>Journal: </b> Not Available
 * <br><br>
 * <b>Product(s):</b> 3D ACIS Exchange, 3D Viz Exchange, 3D ACIS Modeler
 * <br><br>
* @param n_backgrounds
 * length of list of names.
 * @param names
 * list of names.
 **/
 DECL_RB outcome api_rh_get_background_types
	(
	   int    &n_backgrounds,
	   const char** &names
	) ;


/**
 * Creates a background.
 * <br><br>
 * <b>Role:</b> This <tt>API</tt> creates a new <tt>RH_BACKGROUND</tt>.
 * Once the background has been created, set the its properties using <tt>api_rh_set_background_arg</tt>.
 * Set the current background using <tt>api_rh_set_background</tt>.
 * Obtain a list of available background types by calling <tt>api_rh_get_background_types</tt>.
 * <br><br>
 * <b>Errors:</b> The name of the background is not one of the supported types.
 * <br><br>
 * <b>Effect:</b> Changes model
 * <br><br>
 * <b>Journal: </b> Not Available
 * <br><br>
 * <b>Product(s):</b> 3D ACIS Exchange, 3D Viz Exchange, 3D ACIS Modeler
 * <br><br>
* @param name
 * name of background.
 * @param background
 * new background returned.
 **/
 DECL_RB outcome api_rh_create_background
	(
	   const char*  name,
	   RH_BACKGROUND* &background
	) ;


/**
 * Creates a copy of a background.
 * <br><br>
 * <b>Role:</b> This <tt>API</tt> creates a new <tt>RH_BACKGROUND</tt> with the same properties as the old background.
 * <br><br>
 * <b>Effect:</b> Changes model
 * <br><br>
 * <b>Journal: </b> Not Available
 * <br><br>
 * <b>Product(s):</b> 3D ACIS Exchange, 3D Viz Exchange, 3D ACIS Modeler
 * <br><br>
* @param background
 * old background.
 * @param new_background
 * returns new background.
 **/
 DECL_RB outcome api_rh_copy_background( RH_BACKGROUND *background, RH_BACKGROUND *&new_background ) ;

/**
 * Deletes a background.
 * <br><br>
 * <b>Role:</b> When a background is deleted, do not use its handle in subsequent calls of
 * <tt>api_rh_set_background</tt>. If the background currently in use by the rendering library has been deleted,
 * reset it by calling <tt>api_rh_set_background</tt>.
 * <br><br>
 * <b>Effect:</b> Changes model
 * <br><br>
 * <b>Journal: </b> Not Available
 * <br><br>
 * <b>Product(s):</b> 3D ACIS Exchange, 3D Viz Exchange, 3D ACIS Modeler
 * <br><br>
* @param background
 * background.
 **/
 DECL_RB outcome api_rh_delete_background( RH_BACKGROUND* background ) ;


/**
 * Sets the current background.
 * <br><br>
 * <b>Role:</b> This <tt>API</tt> sets the current background shader used by the Rendering Base Component
 * to color pixels that are not covered by the surface of an object. If the current background
 * shader is <tt>NULL</tt>, the background is black.
 * <br><br>
 * <b>Effect:</b> Changes model
 * <br><br>
 * <b>Journal: </b> Not Available
 * <br><br>
 * <b>Product(s):</b> 3D ACIS Exchange, 3D Viz Exchange, 3D ACIS Modeler
 * <br><br>
* @param background
 * background to set.
 **/
DECL_RB outcome api_rh_set_background( RH_BACKGROUND* background ) ;

/**
 * Gets the current background.
 * <br><br>
 * <b>Role:</b> This <tt>API</tt> gets the current background shader used by the Rendering Base Component to
 * color pixels that are not covered by an object. If the current background is <tt>NULL</tt>, the background
 * is black.
 * <br><br>
 * <b>Effect:</b> Read-only
 * <br><br>
 * <b>Journal: </b> Not Available
 * <br><br>
 * <b>Product(s):</b> 3D ACIS Exchange, 3D Viz Exchange, 3D ACIS Modeler
 * <br><br>
* @param background
 * returns background to be queried.
 **/
DECL_RB outcome api_rh_get_background( RH_BACKGROUND*& background ) ;

/**
 * Gets the arguments of a background.
 * <br><br>
 * <b>Role:</b> Each background has a list of named arguments.The number and names of those
 * arguments depend on the type of the background.Access the value of each argument via the
 * <tt>Render_Arg</tt> class.
 * <br><br>
 * This <tt>API</tt> returns the name of the background type with two lists containing the names and
 * values of those arguments. All returned pointer values point to internal data structures.
 * <br><br>
 * For efficiency, these values are not copied when the <tt>API</tt> is called.The application should not
 * free the memory pointed to by those pointers.In the case of <tt>Render_Arg</tt>, the internal array
 * is re-used by other inquiry functions.It is up to the application to copy those values, if required,
 * before any subsequent inquiry functions.
 * <br><br>
 * <b>Effect:</b> Read-only
 * <br><br>
 * <b>Journal: </b> Not Available
 * <br><br>
 * <b>Product(s):</b> 3D ACIS Exchange, 3D Viz Exchange, 3D ACIS Modeler
 * <br><br>
* @param background
 * background to be queried.
 * @param name
 * returns name of background type.
 * @param no_of_args
 * returns number of arguments.
 * @param arg_names
 * returns array of argument names.
 * @param arg_values
 * returns array of arguments.
 **/
DECL_RB outcome api_rh_get_background_args
	(
	   RH_BACKGROUND*  background,
	   const char*       &name,
	   int         &no_of_args,
	   const char  **&arg_names,
	   Render_Arg *&arg_values
	) ;

/**
 * Sets an argument for a background.
 * <br><br>
 * <b>Role:</b> This <tt>API</tt> sets arguments controlling the background specified by name.
 * name must correspond to one of the supported arguments for the type of background associated
 * with <tt>RH_BACKGROUND</tt>.The underlying argument type depends on the argument set with the <tt>Render_Arg</tt> class.
 * <br><br>
 * <b>Effect:</b> Changes model
 * <br><br>
 * <b>Journal: </b> Not Available
 * <br><br>
 * <b>Product(s):</b> 3D ACIS Exchange, 3D Viz Exchange, 3D ACIS Modeler
 * <br><br>
* @param background
 * background.
 * @param arg_name
 * name of argument.
 * @param value
 * argument value.
 **/
DECL_RB outcome api_rh_set_background_arg
	(
	   RH_BACKGROUND*    background,
	   const char*       arg_name,
	   const Render_Arg &value
	) ;

//-----------------------------------------------------------------------------
//
// RH_FOREGROUND Manipulation Functions
//
//-----------------------------------------------------------------------------
/**
 * Gets a list of valid foreground type names.
 * <br><br>
 * <b>Role:</b> This <tt>API</tt> points to a list of valid <tt>RH_FOREGROUND</tt> type names.
 * The list should not be freed by the application.
 * <br><br>
 * <b>Effect:</b> Read-only
 * <br><br>
 * <b>Journal: </b> Not Available
 * <br><br>
 * <b>Product(s):</b> 3D ACIS Exchange, 3D Viz Exchange, 3D ACIS Modeler
 * <br><br>
* @param n_foregrounds
 * returns number of foregrounds.
 * @param names
 * returns list of names.
 **/
DECL_RB outcome api_rh_get_foreground_types
	(
	   int    &n_foregrounds,
	   const char** &names
	) ;

/**
 * Creates a foreground.
 * <br><br>
 * <b>Role:</b>  Once the <tt>RH_FOREGROUND</tt> is created, set the foreground properties using
 * <tt>api_rh_set_foreground_arg</tt>.Set the current foreground using <tt>api_rh_set_foreground</tt>.Obtain a
 * list of available foreground types by calling <tt>api_rh_get_foreground_types</tt>.
 * <br><br>
 * <b>Errors:</b> The name of the foreground is not one of the supported types.
 * <br><br>
 * <b>Effect:</b> Changes model
 * <br><br>
 * <b>Journal: </b> Not Available
 * <br><br>
 * <b>Product(s):</b> 3D ACIS Exchange, 3D Viz Exchange, 3D ACIS Modeler
 * <br><br>
* @param name
 * name of foreground.
 * @param foreground
 * new foreground returned.
 **/
DECL_RB outcome api_rh_create_foreground
	(
	   const char*  name,
	   RH_FOREGROUND* &foreground
	) ;



/**
 * Creates a copy of a foreground.
 * <br><br>
 * <b>Role:</b> This <tt>API</tt> creates a new <tt>RH_FOREGROUND</tt> with the same properties as the old foreground.
 * <br><br>
 * <b>Effect:</b> Changes model
 * <br><br>
 * <b>Journal: </b> Not Available
 * <br><br>
 * <b>Product(s):</b> 3D ACIS Exchange, 3D Viz Exchange, 3D ACIS Modeler
 * <br><br>
* @param foreground
 * old foreground.
 * @param new_foreground
 * returns new foreground.
 **/
 DECL_RB outcome api_rh_copy_foreground( RH_FOREGROUND* foreground, RH_FOREGROUND*& new_foreground ) ;

/**
 * Deletes a foreground.
 * <br><br>
 * <b>Role:</b> When a foreground is deleted, do not use its handle in subsequent calls of
 * <tt>api_rh_set_foreground</tt>. If the foreground currently in use by the rendering library has been deleted,
 * reset it by calling <tt>api_rh_set_foreground</tt>.
 * <br><br>
 * <b>Effect:</b> Changes model
 * <br><br>
 * <b>Journal: </b> Not Available
 * <br><br>
 * <b>Product(s):</b> 3D ACIS Exchange, 3D Viz Exchange, 3D ACIS Modeler
 * <br><br>
* @param foreground
 * foreground.
 **/
 DECL_RB outcome api_rh_delete_foreground( RH_FOREGROUND* foreground ) ;

/**
 * Sets the current foreground.
 * <br><br>
 * <b>Role:</b> This <tt>API</tt> sets the current foreground shader used by the Rendering Base Component
 * to color pixels that are not covered by the surface of an object. If the current foreground
 * shader is <tt>NULL</tt>, the foreground is black.
 * <br><br>
 * <b>Effect:</b> Changes model
 * <br><br>
 * <b>Journal: </b> Not Available
 * <br><br>
 * <b>Product(s):</b> 3D ACIS Exchange, 3D Viz Exchange, 3D ACIS Modeler
 * <br><br>
* @param foreground
 * foreground to set.
 **/
DECL_RB outcome api_rh_set_foreground( RH_FOREGROUND* foreground ) ;

/**
 * Gets the current foreground.
 * <br><br>
 * <b>Role:</b> This <tt>API</tt> gets the current foreground shader used by the Rendering Base Component
 * as a post processing step in the shader pipeline.The current foreground may be <tt>NULL</tt>, in which case,
 * no post processing is performed.
 * <br><br>
 * <b>Effect:</b> Read-only
 * <br><br>
 * <b>Journal: </b> Not Available
 * <br><br>
 * <b>Product(s):</b> 3D ACIS Exchange, 3D Viz Exchange, 3D ACIS Modeler
 * <br><br>
* @param foreground
 * returns foreground to be queried.
 **/
DECL_RB outcome api_rh_get_foreground( RH_FOREGROUND*& foreground ) ;

/**
 * Gets the arguments of a foreground.
 * <br><br>
 * <b>Role:</b> Each foreground has a list of named arguments. The number and names of those arguments
 * depend on the type of foreground.Access the value of each argument via the <tt>Render_Arg</tt> class.
 * <br><br>
 * This <tt>API</tt> returns the name of the foreground type with two lists containing the names and values
 * of those arguments. All returned pointer values point to internal data structures.
 * <br><br>
 * For efficiency, these values are not copied when the function is called.The application should not
 * free the memory pointed to by those pointers. In the case of <tt>Render_Arg</tt>, the internal array is
 * re-used by other inquiry functions. 	It is up to the application to copy those values, if required,
 * before any subsequent inquiry functions.
 * <br><br>
 * <b>Effect:</b> Read-only
 * <br><br>
 * <b>Journal: </b> Not Available
 * <br><br>
 * <b>Product(s):</b> 3D ACIS Exchange, 3D Viz Exchange, 3D ACIS Modeler
 * <br><br>
* @param foreground
 * foreground to be queried.
 * @param name
 * returns name of foreground type.
 * @param no_of_args
 * returns number of arguments.
 * @param arg_names
 * returns array of argument names.
 * @param arg_values
 * returns array of arguments.
 **/
DECL_RB outcome api_rh_get_foreground_args
	(
	   RH_FOREGROUND*  foreground,
	   const char*       &name,
	   int         &no_of_args,
	   const char**      &arg_names,
	   Render_Arg* &arg_values
	) ;


/**
 * Sets an argument of a foreground.
 * <br><br>
 * <b>Role:</b> This <tt>API</tt> sets arguments controlling the foreground specified by name. name must
 * correspond to one of the supported arguments for the type of foreground associated
 * with <tt>RH_FOREGROUND</tt>.The underlying argument type depends on the argument set with
 * the <tt>Render_Arg</tt> class.
 * <br><br>
 * <b>Effect:</b> Changes model
 * <br><br>
 * <b>Journal: </b> Not Available
 * <br><br>
 * <b>Product(s):</b> 3D ACIS Exchange, 3D Viz Exchange, 3D ACIS Modeler
 * <br><br>
* @param foreground
 * foreground.
 * @param arg_name
 * name of argument.
 * @param value
 * argument value.
 **/
DECL_RB outcome api_rh_set_foreground_arg
	(
	   RH_FOREGROUND*    foreground,
	   const char*       arg_name,
	   const Render_Arg &value
	) ;

//-----------------------------------------------------------------------------
//
//  TEXTURE SPACE Manipulation functions
//
//-----------------------------------------------------------------------------

/**
 * Gets a list of valid texture space type names.
 * <br><br>
 * <b>Role:</b> This <tt>API</tt> points to a list of valid <tt>RH_TEXTURE_SPACE</tt> type names. The list should
 * not be freed by the application.
 * <br><br>
 * <b>Effect:</b> Read-only
 * <br><br>
 * <b>Journal: </b> Not Available
 * <br><br>
 * <b>Product(s):</b> 3D ACIS Exchange, 3D Viz Exchange, 3D ACIS Modeler
 * <br><br>
* @param n_types
 * returns number of names.
 * @param names
 * returns list of names.
 **/
DECL_RB outcome api_rh_get_texture_space_types( int& n_types, const char** &names ) ;


/**
 * Creates a texture space.
 * <br><br>
 * <b>Role:</b> This <tt>API</tt> creates an <tt>RH_TEXTURE_SPACE</tt>.The texture space name must correspond to that
 * of an existing texture space shader prototype with that name.Set texture space properties using
 * <tt>api_rh_set_texture_space_arg</tt>.Texture spaces modify the image based on the coordinates of each pixel.
 * Texture spaces can make an object appear to be carved out of a solid material or wrapped in a
 * sheet of material.
 * <br><br>
 * <b>Errors:</b> The name of the texture space is not one of the supported types.
 * <br><br>
 * <b>Effect:</b> Changes model
 * <br><br>
 * <b>Journal: </b> Not Available
 * <br><br>
 * <b>Product(s):</b> 3D ACIS Exchange, 3D Viz Exchange, 3D ACIS Modeler
 * <br><br>
* @param name
 * name of texture space.
 * @param texture_space
 * new textures space returned.
 **/

DECL_RB outcome api_rh_create_texture_space
	(
	   const char*     name,
	   RH_TEXTURE_SPACE *& texture_space
	       ) ;
/**
 * Copies a texture space.
 * <br><br>
 * <b>Role:</b> This <tt>API</tt> creates a new <tt>RH_TEXTURE_SPACE</tt> with the same properties as the old
 * texture_space.
 * <br><br>
 * <b>Effect:</b> Changes model
 * <br><br>
 * <b>Journal: </b> Not Available
 * <br><br>
 * <b>Product(s):</b> 3D ACIS Exchange, 3D Viz Exchange, 3D ACIS Modeler
 * <br><br>
* @param  texture_space
 * original texture space.
 * @param new_texture_space
 * new texture space returned.
 **/

DECL_RB outcome api_rh_copy_texture_space
	(
	   RH_TEXTURE_SPACE*  texture_space,
	   RH_TEXTURE_SPACE*& new_texture_space
	) ;


/**
 * Deletes a texture space.
 * <br><br>
 * <b>Role:</b> Texture spaces can be attached to other entities as properties. Delete all texture
 * spaces that have been created before terminating the renderer. Do not delete a texture space
 * that may be referenced by some other entity.
 * <br><br>
 * <b>Errors:</b> Errors may occur if a texture space attached to one or more entities is deleted.
 * <br><br>
 * <b>Effect:</b> Changes model
 * <br><br>
 * <b>Journal: </b> Not Available
 * <br><br>
 * <b>Product(s):</b> 3D ACIS Exchange, 3D Viz Exchange, 3D ACIS Modeler
 * <br><br>
* @param texture_space
 * texture space.
 **/
 DECL_RB outcome api_rh_delete_texture_space( RH_TEXTURE_SPACE* texture_space ) ;


/**
 * Gets the arguments of a texture space.
 * <br><br>
 * <b>Role:</b> Each texture space has a list of named arguments.
 * The number and names of those arguments depend on the type of texture_space.
 * Access the value of each argument via the <tt>Render_Arg</tt> class.
 * <br><br>
 * This <tt>API</tt> returns the name of the texture_space type with two lists containing the names and values of those arguments.
 * All returned pointer values point to internal data structures.
 * <br><br>
 * For efficiency, these values are not copied when the function is called.
 * The application should not free the memory pointed to by those pointers.
 * In the case of <tt>Render_Arg</tt>, the internal array is re-used by other inquiry functions.
 * It is up to the application to copy those values, if required, before any subsequent inquiry functions.
 * <br><br>
 * <b>Effect:</b> Read-only
 * <br><br>
 * <b>Journal: </b> Not Available
 * <br><br>
 * <b>Product(s):</b> 3D ACIS Exchange, 3D Viz Exchange, 3D ACIS Modeler
 * <br><br>
* @param texture_space
 * texture space to be queried.
 * @param name
 * returns name of texture space type.
 * @param no_of_args
 * returns number of arguments.
 * @param arg_names
 * returns array of argument names.
 * @param arg_values
 * returns array of arguments.
 **/
DECL_RB outcome api_rh_get_texture_space_args
	(
	   RH_TEXTURE_SPACE*  texture_space,
	   const char*          &name,
	   int            &no_of_args,
	   const char**         &arg_names,
	   Render_Arg*    &arg_values
	) ;

/**
 * Sets an argument for a texture space.
 * <br><br>
 * <b>Role:</b> This <tt>API</tt> sets arguments controlling the texture space specified by name. name must correspond to one of the supported arguments for the type of texture space associated with <tt>RH_TEXTURE_SPACE</tt>.
 * The underlying argument type is depends on the argument set with the <tt>Render_Arg</tt> class.
 * <br><br>
 * <b>Effect:</b> Changes model
 * <br><br>
 * <b>Journal: </b> Not Available
 * <br><br>
 * <b>Product(s):</b> 3D ACIS Exchange, 3D Viz Exchange, 3D ACIS Modeler
 * <br><br>
* @param texture_space
 * given texture space.
 * @param arg_name
 * name of argument.
 * @param value
 * argument value.
 **/
DECL_RB outcome api_rh_set_texture_space_arg
	(
	   RH_TEXTURE_SPACE*    texture_space,
	   const char*       arg_name,
	   const Render_Arg &value
	) ;


//-----------------------------------------------------------------------------
//
// Environment mapping manipulation functions
//
//-----------------------------------------------------------------------------


//
// Create an environment map from the current entities
//
/**
 * Creates an environment map from a list of entities.
 * <br><br>
 * <b>Role:</b> Environment maps simulate reflections on objects caused by other objects in the
 * scene and by an external environment. This <tt>API</tt> supports inter-object reflections by computing an
 * environment map from a list of entities. The environment map is represented internally by six
 * square images of a given resolution. Each image represents a perspective view as seen from the
 * given <tt>view_point</tt> in each of the six orthogonal directions.
 * <br><br>
 * <b>Effect:</b> Changes model
 * <br><br>
 * <b>Journal: </b> Not Available
 * <br><br>
 * <b>Product(s):</b> 3D ACIS Exchange, 3D Viz Exchange, 3D ACIS Modeler
 * <br><br>
* @param entities
 * list of entities from which the environment map is to be computed.
 * @param resolution
 * resolution.
 * @param view_point
 * view point.
 * @param map
 * returns new environment map.
 **/
DECL_RB outcome api_rh_render_cube_environment
	(
	   ENTITY_LIST const &entities,
	   int                resolution,
	   SPAposition const     &view_point,
	   RH_ENVIRONMENT_MAP*   &map
	) ;

/**
 * Deletes an environment map.
 * <br><br>
 * <b>Role:</b> When an environment map is deleted, do not use its handle in subsequent calls of
 * <tt>api_rh_set_environment_map</tt>. If the environment map currently in use by the rendering library is
 * deleted, reset the current environment by calling <tt>api_rh_set_environment_map</tt>.
 * <br><br>
 * <b>Effect:</b> Changes model
 * <br><br>
 * <b>Journal: </b> Not Available
 * <br><br>
 * <b>Product(s):</b> 3D ACIS Exchange, 3D Viz Exchange, 3D ACIS Modeler
 * <br><br>
* @param map
 * environment map to be deleted.
 **/
 DECL_RB outcome api_rh_delete_environment_map
       (
	   RH_ENVIRONMENT_MAP* map
       ) ;

/**
 * Sets the current environment map.
 * <br><br>
 * <b>Role:</b> This <tt>API</tt> sets the renderer to use a particular cube environment map during rendering.
 * If the current mode of rendering and current shaders use reflectance, the environment map
 * generates a single level of reflections.Environment maps are generated by reading six raster
 * images supplied by the application using <tt>api_rh_create_cube_environment</tt> or generated by rendering
 * six images from the entity list using <tt>api_rh_render_cube_environment</tt>.
 * <br><br>
 * <b>Effect:</b> Changes model
 * <br><br>
 * <b>Journal: </b> Not Available
 * <br><br>
 * <b>Product(s):</b> 3D ACIS Exchange, 3D Viz Exchange, 3D ACIS Modeler
 * <br><br>
* @param map
 * environment map to set.
 **/
DECL_RB outcome api_rh_set_environment_map
       (
	   RH_ENVIRONMENT_MAP* map
       ) ;

/**
 * Gets the current environment map.
 * <br><br>
 * <b>Role:</b> The <tt>API</tt> returns <tt>NULL</tt> if the map is not set.
 * <br><br>
 * <b>Effect:</b> Read-only
 * <br><br>
 * <b>Journal: </b> Not Available
 * <br><br>
 * <b>Product(s):</b> 3D ACIS Exchange, 3D Viz Exchange, 3D ACIS Modeler
 * <br><br>
* @param map
 * returned environment map
 **/
DECL_RB outcome api_rh_get_environment_map
       (
	   RH_ENVIRONMENT_MAP* &map
       ) ;


//
// Create an environment map from 6 images
//
//   If the image  access function is NULL, then the default action
//   is to treat the  data parameters as a pointer to a raster of pixels
//   stored in row major order
//
/**
 * Creates an environment map from six raster images.
 * <br><br>
 * <b>Role:</b> This <tt>API</tt> supports external reflections by creating a cubical environment map from
 * six raster images supplied by the application.If the image access function is <tt>NULL</tt>, the default
 * treats the image data as a raster image stored in row major order; otherwise, it is up to the
 * application-supplied image access function to interpret the data.
 * <br><br>
 * <b>Limitations:</b> Replay is not supported.
 * <br><br>
 * <b>Effect:</b> System routine
 * <br><br>
 * <b>Journal: </b> Not Available
 * <br><br>
 * <b>Product(s):</b> 3D ACIS Exchange, 3D Viz Exchange, 3D ACIS Modeler
 * <br><br>
* @param data
 * array of six raster image pointers.
 * @param channels
 * number of channels in image data 1 or 3.
 * @param width
 * width of images.
 * @param height
 * height of images.
 * @param afunc
 * image access function.
 * @param env_map
 * new environment map returned.
 **/

DECL_RB outcome api_rh_create_cube_environment
       (
	  unsigned              char* data[6],
	  int                   channels,
	  int                   width[6],
	  int                   height[6],
	  Image_Access_Function afunc,
	  RH_ENVIRONMENT_MAP*      &env_map
       ) ;

//-----------------------------------------------------------------------------
//
// GEOMETRIC ENTITY Attribute Functions
//
//-----------------------------------------------------------------------------
//
/**
 * Attaches a material to a list of entities.
 * <br><br>
 * <b>Role:</b> Geometric <tt>ENTITYs</tt> must be of the type <tt>BODY</tt>, <tt>LUMP</tt>, <tt>SHELL</tt>, or <tt>FACE</tt>.
 * <br><br>
 * <b>Effect:</b> Changes model
 * <br><br>
 * <b>Journal: </b> Not Available
 * <br><br>
 * <b>Product(s):</b> 3D ACIS Exchange, 3D Viz Exchange, 3D ACIS Modeler
 * <br><br>
* @param entities
 * entities to which to attach material.
 * @param material
 * material to attach.
 **/
 DECL_RB outcome api_rh_set_material
	(
	   ENTITY_LIST  const &entities,
	   RH_MATERIAL     *material = (RH_MATERIAL *)NULL
	) ;

/**
 * Gets the material attached to an entity.
 * <br><br>
 * <b>Role:</b> This <tt>API</tt> gets the material attached to an entity.
 * Geometric entities must be of the type <tt>BODY</tt>, <tt>LUMP</tt>, <tt>SHELL</tt>, or <tt>FACE</tt>.
 * <br><br>
 * <b>Effect:</b> Read-only
 * <br><br>
 * <b>Journal: </b> Not Available
 * <br><br>
 * <b>Product(s):</b> 3D ACIS Exchange, 3D Viz Exchange, 3D ACIS Modeler
 * <br><br>
* @param entity
 * entity to be queried.
 * @param material
 * returns material attached to entity.
 * @param inherit
 * If true, traverse up the tree to find the first material attached to this entity or one of its owners.
 * If the status of the color, displacement, reflection, or transparency is not turned on, rendering will use inheritance to find the material where it is turned on.
 * See also <tt>api_rh_get_entity_rgb</tt>, <tt>api_rh_get_displace_status</tt>, <tt>api_rh_get_reflect_status</tt>, <tt>api_rh_get_transp_status</tt>.
 * See also <tt>api_rh_get_material_color</tt>, <tt>api_rh_get_material_reflection</tt>, <tt>api_rh_get_material_transp</tt>, and <tt>api_rh_get_material_texture</tt>.
 **/
DECL_RB outcome api_rh_get_material
	(
	   ENTITY   *entity,
	   RH_MATERIAL*& material,
	   logical inherit = FALSE  // Traverse up the tree to find the first material.
	) ;

/**
 * Attaches a texture space to a list of entities.
 * <br><br>
 * <b>Role:</b> This <tt>API</tt> sets the texture space associated with an entity list.
 * Each <tt>ENTITY</tt> must be of the type <tt>BODY</tt>, <tt>LUMP</tt>, <tt>SHELL</tt>, or <tt>FACE</tt>.
 * <br><br>
 * <b>Effect:</b> Changes model
 * <br><br>
 * <b>Journal: </b> Not Available
 * <br><br>
 * <b>Product(s):</b> 3D ACIS Exchange, 3D Viz Exchange, 3D ACIS Modeler
 * <br><br>
* @param entities
 * entities to have texture space attached.
 * @param texture_space
 * given texture space.
 **/

DECL_RB outcome api_rh_set_texture_space
	(
	   ENTITY_LIST   const &entities,
	   RH_TEXTURE_SPACE *texture_space = (RH_TEXTURE_SPACE *)NULL
	) ;

/**
 * Gets the texture space attached to an entity.
 * <br><br>
 * <b>Role:</b> This <tt>API</tt> gets the texture space attached to an entity.
 * Geometric entities must be of the type <tt>BODY</tt>, <tt>LUMP</tt>, <tt>SHELL</tt>, or <tt>FACE</tt>.
 * <br><br>
 * <b>Effect:</b> Read-only
 * <br><br>
 * <b>Journal: </b> Not Available
 * <br><br>
 * <b>Product(s):</b> 3D ACIS Exchange, 3D Viz Exchange, 3D ACIS Modeler
 * <br><br>
* @param entity
 * entity to be queried.
 * @param texture_space
 * returns the new texture space.
 **/

DECL_RB outcome api_rh_get_texture_space
	(
	   ENTITY         *entity ,
	   RH_TEXTURE_SPACE*& texture_space
	) ;

/**
 * @nodoc
 * <b>Obsolete:</b> This function has been deprecated. 
 * Please call the other signature of this API having 
 * the <tt><i>found</i></tt> argument.
 * Gets the material color associated with a geometric entity.
 * <br><br>
 * <b>Role:</b> This <tt>API</tt> returns the material color attached to an entity.
 * Geometric entities must be of the type <tt>BODY</tt>, <tt>LUMP</tt>, <tt>SHELL</tt>, or <tt>FACE</tt>.
 * <br><br>
 * Deprecated.   
 * <br><br>
 * <b>Effect:</b> Read-only
 * <br><br>
 * <b>Journal: </b> Not Available
 * <br><br>
 * <b>Product(s):</b> 3D ACIS Exchange, 3D Viz Exchange, 3D ACIS Modeler
 * <br><br>
* @param entity
 * entity.
 * @param red
 * returns red color value.
 * @param green
 * returns green color value.
 * @param blue
 * returns blue color value.
 **/
DECL_RB outcome api_rh_get_material_color
    (
       ENTITY *entity,
       double &red,
       double &green,
       double &blue
    );


/**
 * Gets the material color associated with a geometric entity.
 * <br><br>
 * <b>Role:</b> This <tt>API</tt> returns the material color attached to an entity.
 * Geometric entities must be of the type <tt>BODY</tt>, <tt>LUMP</tt>, <tt>SHELL</tt>, or <tt>FACE</tt>.
 * <br><br>
 * The given material might not have a color assigned. In this case, the "found" argument will be <tt>FALSE</tt>.
 * <br><br>
 * <b>Effect:</b> Read-only
 * <br><br>
 * <b>Journal: </b> Not Available
 * <br><br>
 * <b>Product(s):</b> 3D ACIS Exchange, 3D Viz Exchange, 3D ACIS Modeler
 * <br><br>
* @param entity
 * entity.
 * @param red
 * returns red color value.
 * @param green
 * returns green color value.
 * @param blue
 * returns blue color value.
 * @param found
 * returns <tt>TRUE</tt> if given material has been assigned a color, <tt>FALSE</tt> if
 * no color has been assigned.
 **/
DECL_RB outcome api_rh_get_material_color
    (
       ENTITY *entity,
       double &red,
       double &green,
       double &blue,
	   logical &found
    );



/**
 * Gets the material reflection properties associated with a geometric
 * entity.
 * <br><br>
 * <b>Role:</b> This <tt>API</tt> gets the material reflection properties (if any) associated with a geometric entity.
 * Geometric entities must be of the type <tt>BODY</tt>, <tt>LUMP</tt>, <tt>SHELL</tt>, or <tt>FACE</tt>.
 * <br><br>
 * <b>Effect:</b> Read-only
 * <br><br>
 * <b>Journal: </b> Not Available
 * <br><br>
 * <b>Product(s):</b> 3D ACIS Exchange, 3D Viz Exchange, 3D ACIS Modeler
 * <br><br>
* @param entity
 * entity.
 * @param ambient
 * returns ambient reflection.
 * @param diffuse
 * returns diffuse reflection.
 * @param specular
 * returns specular reflection.
 * @param exponent
 * returns exponent value.
 **/
 DECL_RB outcome api_rh_get_material_reflection
    (
       ENTITY *entity,
       double &ambient,
       double &diffuse,
       double &specular,
	   double &exponent
    );


/**
 * Gets the material transparency associated with a geometric entity.
 * <br><br>
 * <b>Role:</b> This <tt>API</tt> gets the material transparency associated with a geometric entity.
 * Geometric entities must be of the type <tt>BODY</tt>, <tt>LUMP</tt>, <tt>SHELL</tt>, or <tt>FACE</tt>.
 * <br><br>
 * <b>Effect:</b> Read-only
 * <br><br>
 * <b>Journal: </b> Not Available
 * <br><br>
 * <b>Product(s):</b> 3D ACIS Exchange, 3D Viz Exchange, 3D ACIS Modeler
 * <br><br>
* @param entity_list
 * list of entities.
 * @param transp
 * returns transparency value (<tt>0</tt> is opaque; <tt>1</tt> is transparent).
 * @param inherit
 * If true, traverse up the tree to find the inherited transparency that is set.
 **/

DECL_RB outcome api_rh_get_material_transp
    (
       ENTITY *entity_list,
       double &transp,
	   logical inherit = FALSE  // Traverse up the tree to find the inherited material for the transparency.
    );

/**
 * Gets the material texture associated with a geometric entity.
 * <br><br>
 * <b>Role:</b> Geometric entities must be of the type <tt>BODY</tt>, <tt>LUMP</tt>, <tt>SHELL</tt>, or <tt>FACE</tt>.
 * <br><br>
 * <b>Effect:</b> Read-only
 * <br><br>
 * <b>Journal: </b> Not Available
 * <br><br>
 * <b>Product(s):</b> 3D ACIS Exchange, 3D Viz Exchange, 3D ACIS Modeler
 * <br><br>
* @param entity
 * entity.
 * @param tex_name
 * returns type of texture.
 **/
DECL_RB outcome api_rh_get_material_texture
	(
       ENTITY *entity,
	   const char *&tex_name
    );

/**
 * Sets the color of a material associated with a geometric entity.
 * <br><br>
 * <b>Role:</b> This <tt>API</tt> sets the color of a material associated with a geometric entity.
 * Geometric entities must be of the type <tt>BODY</tt>, <tt>LUMP</tt>, <tt>SHELL</tt>, or <tt>FACE</tt>.
 * <br><br>
 * <b>Effect:</b> Changes model
 * <br><br>
 * <b>Journal: </b> Not Available
 * <br><br>
 * <b>Product(s):</b> 3D ACIS Exchange, 3D Viz Exchange, 3D ACIS Modeler
 * <br><br>
* @param entities
 * list of entities.
 * @param red
 * red color value.
 * @param green
 * green color value.
 * @param blue
 * blue color value.
 **/
DECL_RB outcome api_rh_set_material_color
    (
       ENTITY_LIST const &entities,
       double red,
       double green,
       double blue
    );


/**
 * Sets the reflection properties in the material associated with a geometric entity.
 * <br><br>
 * <b>Role:</b> This <tt>API</tt> sets the reflection properties in the material associated with a geometric
 * entity.Geometric entities must be of the type <tt>BODY</tt>, <tt>LUMP</tt>, <tt>SHELL</tt>, or <tt>FACE</tt>.
 * <br><br>
 * <b>Effect:</b> Changes model
 * <br><br>
 * <b>Journal: </b> Not Available
 * <br><br>
 * <b>Product(s):</b> 3D ACIS Exchange, 3D Viz Exchange, 3D ACIS Modeler
 * <br><br>
* @param  entity_list
 * list of entities to be set.
 * @param ambient
 * ambient light value.
 * @param diffuse
 * diffuse light value.
 * @param specular
 * specular light value.
 * @param exponent
 * exponent value.
 **/
DECL_RB outcome api_rh_set_material_reflection
    (
       ENTITY_LIST const &entity_list,
       double ambient,
       double diffuse,
       double specular,
	   double exponent
    );

/**
 * Sets the transparency properties in the material associated with a geometric entity.
 * <br><br>
 * <b>Role:</b> Geometric entities must be of the type <tt>BODY</tt>, <tt>LUMP</tt>, <tt>SHELL</tt>, or <tt>FACE</tt>.
 * <br><br>
 * <b>Effect:</b> Changes model
 * <br><br>
 * <b>Journal: </b> Not Available
 * <br><br>
 * <b>Product(s):</b> 3D ACIS Exchange, 3D Viz Exchange, 3D ACIS Modeler
 * <br><br>
* @param entity_list
 * list of entities to be set.
 * @param transp
 * transparency value ( <tt>0</tt> is opaque; <tt>1</tt> is transparent).
 **/
DECL_RB outcome api_rh_set_material_transp
    (
       ENTITY_LIST const &entity_list,
       double transp
    );

/**
 * Sets the texture file name in the material associated with a geometric entity.
 * <br><br>
 * <b>Role:</b> This <tt>API</tt> sets the texture file name in the material associated with a geometric entity.
 * Geometric entities must be of the type <tt>BODY</tt>, <tt>LUMP</tt>, <tt>SHELL</tt>, or <tt>FACE</tt>.
 * <br><br>
 * <b>Effect:</b> Changes model
 * <br><br>
 * <b>Journal: </b> Not Available
 * <br><br>
 * <b>Product(s):</b> 3D ACIS Exchange, 3D Viz Exchange, 3D ACIS Modeler
 * <br><br>
* @param entity_list
 * list of entities to be set.
 * @param tex_name
 * texture file name.
 **/
DECL_RB outcome api_rh_set_material_texture
	(
       ENTITY_LIST const &entity_list,
       const char *tex_name
    );

//
// Sidedness
//
// Define the surface of an entity to be single or double-sided
// for rendering purposes.
//
/**
 * Sets the sidedness of faces of entities.
 * <br><br>
 * <b>Role:</b> This <tt>API</tt> defines the surface of an entity to be single-sided or double-sided for
 * rendering purposes.This may be used to help visualize faces of nonmanifold objects where it
 * is desirable to view both sides of a face.Geometric entities must be of the type <tt>BODY</tt>, <tt>LUMP</tt>,
 * <tt>SHELL</tt>, or <tt>FACE</tt>. A two-sided face is always treated as double-sided.
 * <br><br>
 * <b>Effect:</b> Changes model
 * <br><br>
 * <b>Journal: </b> Not Available
 * <br><br>
 * <b>Product(s):</b> 3D ACIS Exchange, 3D Viz Exchange, 3D ACIS Modeler
 * <br><br>
* @param entities
 * entities to have sidedness set.
 * @param sides
 * 0 (sidedness undefined), 1 (single-sided), or 2 (double-sided).
 **/
DECL_RB outcome api_rh_set_sidedness
	(
	   ENTITY_LIST const &entities,
	   int     sides
	) ;


/**
 * Gets the sidedness of an entity.
 * <br><br>
 * <b>Role:</b> Geometric entities must be of the type <tt>BODY</tt>, <tt>LUMP</tt>, <tt>SHELL</tt>, or <tt>FACE</tt>.
 * If sides is <tt>2</tt>, the entity is treated as double-sided for rendering.
 * If sides is <tt>1</tt>, the entity is treated as single-sided for rendering.
 * If sides is <tt>0</tt>, the entity has no sidedness set, and it inherits sidedness from its parent.
 * <br><br>
 * <b>Effect:</b> Read-only
 * <br><br>
 * <b>Journal: </b> Not Available
 * <br><br>
 * <b>Product(s):</b> 3D ACIS Exchange, 3D Viz Exchange, 3D ACIS Modeler
 * <br><br>
* @param entity
 * entity to be queried.
 * @param sides
 * returns sidedness.
 **/
DECL_RB outcome api_rh_get_sidedness
	(
	   ENTITY  *entity,
	   int      &sides
	) ;




//-----------------------------------------------------------------------------
//
// STANDARD HUSK COMPATIBILITY FUNCTIONS
//
//-----------------------------------------------------------------------------

/**
 * @nodoc
 */

DECL_RB outcome api_rh_render_std_entities
	(
	   ENTITY_LIST const  &entities,
	   ENTITY_LIST const &lights,
	   RH_BACKGROUND         *background,
	   Render_Mode         mode,
	   logical             clear_screen = TRUE
	) ;

/**
 * @nodoc
 */


DECL_RB outcome api_rh_set_std_material
	(
	  RH_MATERIAL*   material,
	  Render_Color   const &color,
	  double         ka,
	  double         kd,
	  double         ks,
	  double         phong_exponent
	) ;

/**
 * @nodoc
 */
DECL_RB outcome api_rh_get_std_material
	(
	   RH_MATERIAL*     material,
	   Render_Color    &color,
	   double          &ka,
	   double          &kd,
	   double          &ks,
	   double          &phong_exponent
	) ;

/**
 * @nodoc
 */
DECL_RB outcome api_rh_set_std_light
	(
	   RH_LIGHT*       light,
	   double          intensity,
	   Render_Color    const &color,
	   SPAposition        const &pos
	) ;

/**
 * @nodoc
 */
DECL_RB outcome api_rh_get_std_light
	(
	   RH_LIGHT*      light,
	   const char*    &name,
	   double         &intensity,
	   Render_Color &color,
	   SPAposition     &pos
	) ;

/**
 * @nodoc
 */
DECL_RB outcome api_rh_set_std_background
	(
	   RH_BACKGROUND*         background,
	   Render_Color const &color_1,
	   Render_Color const &color_2
	) ;

/**
 * @nodoc
 */

DECL_RB outcome api_rh_get_std_background
	(
	   RH_BACKGROUND*       background,
	   const char      * &name,
	   Render_Color &color_1,
	   Render_Color &color_2
	) ;

// Viewing specification - required for refinement criterion that use image
// space.  These use to be in the faceter, to support view dependent faceting.
// There is no longer view dependent faceting, but the renderers were using
// the same info, so the api's have been moved.

// Projection types

/**
 * Specifies the type of projection Required for refinement criterion that use image space.
 * <br>
 * @param PROJ_ORTHOGRAPHIC
 * Orthographic projection.
 * @param PROJ_PERSPECTIVE
 * Perspective projection.
 **/

typedef enum
{
   PROJ_ORTHOGRAPHIC = 0,
   PROJ_PERSPECTIVE

} Projection_Type ;

class SPAvector;
class SPAbox;

/**
 * Sets the view parameters that affect view-dependent faceting.
 * <br><br>
 * <b>Role:</b> In view-dependent faceting, objects outside the field of view are left unfaceted, which is
 * similar to treatment of objects outside of the clipping planes.
 * <br><br>
 * <b>Effect:</b> Changes model
 * <br><br>
 * <b>Journal: </b> Not Available
 * <br><br>
 * <b>Product(s):</b> 3D ACIS Exchange, 3D Viz Exchange, 3D ACIS Modeler
 * <br><br>
* @param from_point
 * from point.
 * @param to_point
 * to point.
 * @param view_up_vector
 * normal to view.
 * @param projection
 * given type of projection.
 * @param field_of_view
 * field of view.
 **/
DECL_RB outcome api_rh_set_view
        (
           SPAposition        from_point,
           SPAposition        to_point,
           SPAvector          view_up_vector,
           Projection_Type projection,
           double          field_of_view
        ) ;

/**
 * Sets the near and far clipping planes for faceting.
 * <br><br>
 * <b>Role:</b> When view-dependent refinements are used, objects beyond the far planes or in front of the near
 * planes are not faceted.Objects that cross the clipping planes facet correctly only in the region
 * between the planes.
 * <br><br>
 * <b>Effect:</b> Changes model
 * <br><br>
 * <b>Journal: </b> Not Available
 * <br><br>
 * <b>Product(s):</b> 3D ACIS Exchange, 3D Viz Exchange, 3D ACIS Modeler
 * <br><br>
* @param near_clipping_plane
 * near clipping plane.
 * @param far_clipping_plane
 * far clipping plane.
 **/
DECL_RB outcome api_rh_set_clipping
        (
           double near_clipping_plane,
           double far_clipping_plane
        ) ;


/**
 * Sets the default screen faceting resolution controls.
 * <br><br>
 * <b>Role:</b> This <tt>API</tt> sets the default screen faceting resolution controls.
 * <br><br>
 * <b>Effect:</b> System routine
 * <br><br>
 * <b>Journal: </b> Not Available
 * <br><br>
 * <b>Product(s):</b> 3D ACIS Exchange, 3D Viz Exchange, 3D ACIS Modeler
 * <br><br>
* @param width
 * image space width.
 * @param height
 * image space height.
 * @param pixel_aspect_ratio
 * aspect ratio in pixels.
 * @param image_scale
 * scale of the image.
 **/
DECL_RB outcome api_rh_set_resolution
        (
           int    width,
           int    height,
           double pixel_aspect_ratio,
           double image_scale
        ) ;

/**
 * Sets the region of image importance within a screen image.
 * <br><br>
 * <b>Role:</b> This <tt>API</tt> sets the region of image importance within a screen
 * image.
 * <br><br>
 * <b>Effect:</b> Changes model
 * <br><br>
 * <b>Journal: </b> Not Available
 * <br><br>
 * <b>Product(s):</b> 3D ACIS Exchange, 3D Viz Exchange, 3D ACIS Modeler
 * <br><br>
* @param left
 * left side.
 * @param right
 * right side.
 * @param top
 * top.
 * @param bottom
 * bottom.
 **/
DECL_RB outcome api_rh_set_sub_image
        (
           int left,
           int right,
           int top,
           int bottom
        ) ;

/**
 * Gets the view parameters that affect view-dependent faceting.
 * <br><br>
 * <b>Effect:</b> Read-only
 * <br><br>
 * <b>Journal: </b> Not Available
 * <br><br>
 * <b>Product(s):</b> 3D ACIS Exchange, 3D Viz Exchange, 3D ACIS Modeler
 * <br><br>
* @param from_point
 * returns from point.
 * @param to_point
 * returns to point.
 * @param view_up_vector
 * returns normal to view.
 * @param projection
 * returns type of projection.
 * @param field_of_view
 * returns field of view.
 **/
 DECL_RB outcome api_rh_get_view
        (
           SPAposition        &from_point,
           SPAposition        &to_point,
           SPAvector          &view_up_vector,
           Projection_Type &projection,
           double          &field_of_view
        ) ;

/**
 * Gets the depth clipping parameters that affect view-dependent faceting.
 * <br><br>
 * <b>Effect:</b> Read-only
 * <br><br>
 * <b>Journal: </b> Not Available
 * <br><br>
 * <b>Product(s):</b> 3D ACIS Exchange, 3D Viz Exchange, 3D ACIS Modeler
 * <br><br>
* @param near_clipping_plane
 * returns near clipping plane.
 * @param far_clipping_plane
 * returns far clipping plane.
 **/

DECL_RB outcome api_rh_get_clipping
        (
           double &near_clipping_plane,
           double &far_clipping_plane
        ) ;

/**
 * Gets the default screen faceting resolution controls.
 * <br><br>
 * <b>Effect:</b> Read-only
 * <br><br>
 * <b>Journal: </b> Not Available
 * <br><br>
 * <b>Product(s):</b> 3D ACIS Exchange, 3D Viz Exchange, 3D ACIS Modeler
 * <br><br>
* @param width
 * returns image space width.
 * @param height
 * returns image space height.
 * @param pixel_aspect_ratio
 * returns aspect ratio in pixels.
 * @param image_scale
 * returns scale of the image.
 **/
DECL_RB outcome api_rh_get_resolution
        (
           int    &width,
           int    &height,
           double &pixel_aspect_ratio,
           double &image_scale
        ) ;

/**
 * Gets the pixel coordinates of the region of interest on a screen.
 * <br><br>
 * <b>Effect:</b> Read-only
 * <br><br>
 * <b>Journal: </b> Not Available
 * <br><br>
 * <b>Product(s):</b> 3D ACIS Exchange, 3D Viz Exchange, 3D ACIS Modeler
 * <br><br>
* @param left
 * returns left side.
 * @param right
 * returns right side.
 * @param top
 * returns top.
 * @param bottom
 * returns bottom.
 **/
DECL_RB outcome api_rh_get_sub_image
        (
           int &left,
           int &right,
           int &top,
           int &bottom
        ) ;


//-----------------------------------------------------------------------------
//
// Image Utility Functions
//
//-----------------------------------------------------------------------------
//
// Lightworks Image Utility routines.

/**
 * Initializes the Image Utilities Library.
 * <br><br>
 * <b>Role:</b> This function initializes the use of the Image Format Utility Library.
 * Call this function before any others in the Image Format Utility Library. Call
 * <tt>LwTerminateImageUtilities</tt> to terminate the use of the library.Call this function when the
 * rendering session is initiated with <tt>api_rh_initialise</tt>.
 * <br><br>
 * <b>Effect:</b> System routine
 * <br><br>
 * <b>Journal: </b> Not Available
 * <br><br>
 * <b>Product(s):</b> 3D ACIS Exchange, 3D Viz Exchange, 3D ACIS Modeler
 * <br><br>
**/
 DECL_RB outcome api_rh_initialise_image_utilities();

/**
 * Terminates the use of the Image Format Utility Library.
 * <br><br>
 * <b>Role:</b> This function terminates the use of the Image Format Utility
 * Library, and it must match a corresponding call to <tt>LwInitialiseImageUtilities</tt>.
 * Any data structures that have been allocated by the library are de-allocated.
 * Call this function when terminating a rendering session with <tt>api_rh_terminate</tt>.
 * <br><br>
 * <b>Effect:</b> System routine
 * <br><br>
 * <b>Journal: </b> Not Available
 * <br><br>
 * <b>Product(s):</b> 3D ACIS Exchange, 3D Viz Exchange, 3D ACIS Modeler
 * <br><br>
**/
 DECL_RB outcome api_rh_terminate_image_utilities();


/**
 * Sets the method for conversion.
 * <br><br>
 * <b>Role:</b> This function sets the image conversion method used by <tt>LwConvertRGBScanline</tt> and
 * <tt>LwConvertRGBFloatScanline</tt> for subsequent images.method must be one of the following conversion
 * method types:
 * <br><br>
 * <tt>LW_CONVERSION_TC_DITHER</tt> adds random dither (noise) separately to each pixel color component in
 * the image data.The amplitude of the dither is half a quantization level.Use the true-color dither
 * image conversion on true-color devices.
 * <br><br>
 * <tt>LW_CONVERSION_FS_RGB</tt> distributes the color errors between the image color of a pixel and the
 * nearest color to it in the <tt>RGB</tt> cube across the image to minimize any color artifacts.The process
 * produces good color reproduction, and it is the recommended image conversion method for the
 * higher-quality rendering modes on  pseudo-color displays.This conversion method is based on
 * the Floyd-Steinberg algorithm.This is the default conversion method.
 * <br><br>
 * <tt>LW_CONVERSION_O_DITHER_RGB</tt> screens the pixel colors by a color dithering matrix.This increases
 * the color resolution of the device at the expense of reducing the spatial resolution.The ordered
 * dither image conversion method is faster than  the Floyd-Steinberg method, but it tends to
 * introduce a noticeable pattern  across the image.Use it as an alternative conversion method to
 * <tt>LW_CONVERSION_FS_RGB</tt> with simpler rendering modes, such as flat and Gouraud, where speed is more
 * important than image quality.
 * <br><br>
 * <tt>LW_CONVERSION_GREYSCALE</tt> converts the color of a pixel into a grey level using the standard <tt>NTSC</tt>
 * color to luminance conversion.This luminance value is the basis for the index assigned to the pixel.
 * <br><br>
 * <tt>luminance = 0.3r + 0.59g + 0.11b</tt>
 * <br><br>
 * This method assumes that the grey-scale ramp is defined by the red channel of the color map defined
 * by calling <tt>LwSetConversionColourMap</tt>.This conversion method applies to grey-scale display devices.
 * <br><br>
 * <tt>LW_CONVERSION_FS_MONOCHROME</tt> distributes errors in the colors between the image color of a pixel
 * and the black or white pixel value area across the image to minimize any artifacts.The conversion
 * is based on the Floyd-Steinberg algorithm.This image conversion method provides good image
 * reproduction on <i>1-</i>bit or bit-mapped display devices.
 * <br><br>
 * <tt>LW_CONVERSION_FS_DITHER_RGB</tt> augments a color Floyd-Steinberg error distribution with the addition
 * of random noise to the input colors.For some images, the standard color Floyd-Steinberg method
 * creates artifacts in the image, and the addition of noise helps to minimize them. Generally,
 * the improvements to the output image are not enough to justify the extra time spent by this method.
 * <br><br>
 * <tt>LW_CONVERSION_DITHER_RGB</tt> adds a random noise to the image before quantization into the color map.
 * The dither amplitude is equal to one quantization level.
 * This improves average image quality at the expense of some localized noise.
 * <br><br>
 * <tt>LW_CONVERSION_NEAREST_RGB</tt> quantizes pixel colors are quantized into the color map by selecting
 * the nearest color in the map.For pseudo-color displays, this usually causes color banding in the
 * image.
 * <br><br>
 * Call this function before calling <tt>LwConvertImageStart</tt>.
 * Do not call this function during the conversion of an image, which is initiated by
 * <tt>LwConvertImageStart</tt> and  terminated by <tt>LwConvertImageEnd</tt>.The default method is set to
 * <tt>LW_CONVERSION_FS_RGB</tt> when <tt>LwInitialiseImageUtilities</tt> is called.
 * <br><br>
 * <b>Effect:</b> System routine
 * <br><br>
 * <b>Journal: </b> Not Available
 * <br><br>
 * <b>Product(s):</b> 3D ACIS Exchange, 3D Viz Exchange, 3D ACIS Modeler
 * <br><br>
* @param method
 * conversion method.
 **/
  DECL_RB outcome api_rh_set_conversion_method( LwConversionMethod method );

/**
 * Sets the color map for conversion.
 * <br><br>
 * <b>Role:</b> This function describes the color map of the output device, and it takes the form
 * of an <tt>RGB</tt> cube.An <tt>RGB</tt> cube is a 3D array containing an ordered spectrum of colors.
 * The subscripts of an RGB cube are values representing the respective amounts of each red, green,
 * and blue color component of a particular color.
 * <br><br>
 * Images are displayed on a display device using a color-map containing the spectrum of colors
 * described by the <tt>RGB</tt> cube.
 * <br><br>
 * Display devices' color-maps are normally <tt>1D</tt> arrays, with the subscript being a color-map index.
 * <tt>r_mult</tt>, <tt>g_mult</tt>, and <tt>b_mult</tt> are the multipliers for the respective <tt>RGB</tt> cube subscripts used to
 * build a color-map index.The color-map index of a color within the <tt>RGB</tt> cube, with <tt>RGB</tt> cube
 * subscript levels of <i>r</i>, <i>g</i>, and <i>b</i> are <pre>base_index + r  x  r_mult + g  x g_mult + b  x  b_mult</pre>
 * When <tt>LwConvertRGBScanline</tt> or <tt>LwConvertRGBFloatScanline</tt> is called, the pixels' colors are
 * converted into color-map indexes within the <tt>RGB</tt> cube.
 * <br><br>
 * This function's parameters describe the layout of a <tt>3D</tt> array representing the <tt>RGB</tt> cube.
 * Set <tt>r_max</tt>, <tt>g_max</tt>, and <tt>b_max</tt> to the maximum number of different shades of each primary color
 * in the <tt>RGB</tt> cube.Set <tt>r_mult</tt>, <tt>g_mult</tt>, and <tt>b_mult</tt> to the multipliers for the respective components
 * within the <tt>RGB</tt> cube when converting <tt>RGB</tt> levels into color-map indexes.Set <tt>base_index</tt> to the start
 * index of the <tt>RGB</tt> cube within the color-map.For a standard <i>8-</i>bit display device, the values are
 * typically: <pre> r_max = 5, g_max = 5, b_max = 5, r_mult = 36, g_mult = 6, b_mult = 1 </pre> describing a
 * <i>216-</i>color cube with six levels of each color component.Set <tt>base_index</tt> to the index of the first
 * entry of the cube in the map.
 * <br><br>
 * For the grey-scale conversion method only <tt>r_max</tt>, <tt>r_mult</tt>, and <tt>base_index</tt> are significant.
 * <br><br>
 * For the monochrome conversion method, only <tt>base_index</tt> is significant.
 * <br><br>
 * Set the conversion color-map for an image before calling <tt>LwConvertImageStart</tt>.
 * This function is ignored if it is called between calls to <tt>LwConvertImageStart</tt> and <tt>LwConvertImageEnd</tt>.
 * <br><br>
 * The default conversion color-map set by <tt>LwInitialiseImageUtilities</tt> is a  <tt>6 x 6 x 6</tt> <tt>RGB</tt> cube with
 * parameters of <pre> r_max = 5, g_max = 5, b_max = 5, r_mult = 36, g_mult = 6, b_mult = 1, and
 * base_index = 0</pre>.
 * <br><br>
 * <b>Effect:</b> System routine
 * <br><br>
 * <b>Journal: </b> Not Available
 * <br><br>
 * <b>Product(s):</b> 3D ACIS Exchange, 3D Viz Exchange, 3D ACIS Modeler
 * <br><br>
* @param r_max
 * red color map index.
 * @param g_max
 * green color map index.
 * @param b_max
 * blue color map index.
 * @param r_mult
 * red color multiplier.
 * @param g_mult
 * green color multiplier.
 * @param b_mult
 * blue color multiplier.
 * @param base_index
 * base index.
 **/
 DECL_RB outcome api_rh_set_conversion_colour_map
	(
		LwInt32 r_max, LwInt32 g_max, LwInt32 b_max,
		LwInt32 r_mult, LwInt32 g_mult, LwInt32 b_mult,
		LwInt32 base_index
	);

/**
 * Starts conversion of an image.
 * <br><br>
 * <b>Role:</b> This function initializes the image format process for an image of the given size.
 * The image to be converted has dimensions of width pixels horizontally and height pixels vertically.
 * <br><br>
 * <b>Effect:</b> System routine
 * <br><br>
 * <b>Journal: </b> Not Available
 * <br><br>
 * <b>Product(s):</b> 3D ACIS Exchange, 3D Viz Exchange, 3D ACIS Modeler
 * <br><br>
* @param width
 * image width.
 * @param height
 * image height.
 **/
DECL_RB outcome api_rh_convert_image_start( LwInt32 width, LwInt32 height );

/**
 * Terminates conversion of an image.
 * <br><br>
 * <b>Role:</b> Call this function to end the conversion of an image.
 * It must match a corresponding call to <tt>LwConvertImageStart</tt>.
 * Call this function after the last scanline of the image has been converted using
 * <tt>LwConvertRGBFloatScanline</tt> or <tt>LwConvertRGBScanline</tt>.
 * <br><br>
 * <b>Effect:</b> System routine
 * <br><br>
 * <b>Journal: </b> Not Available
 * <br><br>
 * <b>Product(s):</b> 3D ACIS Exchange, 3D Viz Exchange, 3D ACIS Modeler
 * <br><br>
**/
DECL_RB outcome api_rh_convert_image_end();

/**
 * Converts a scanline of LwNat8 values.
 * <br><br>
 * <b>Role:</b> This function converts an input array of pixel colors into a corresponding array
 * of color map indexes, using the image conversion method previously set with <tt>LwSetConversionMethod</tt>.
 * Call this function from the application-supplied function, <tt>rh_image_scanline</tt>.
 * <br><br>
 * <b>Effect:</b> System routine
 * <br><br>
 * <b>Journal: </b> Not Available
 * <br><br>
 * <b>Product(s):</b> 3D ACIS Exchange, 3D Viz Exchange, 3D ACIS Modeler
 * <br><br>
* @param input
 * array of pixel colors.
 * @param output
 * returns array of color map indexes.
 **/
DECL_RB outcome api_rh_convert_rgb_scanline
	(
		LwNat8 *input, LwInt32 *output
	);



/**
 * Converts a scanline of <tt>LwFloat</tt> values.
 * <br><br>
 * <b>Role:</b> This function converts an input array of pixel colors into a corresponding array of
 * color map indexes, using the image conversion method previously set by <tt>LwSetConversionMethod</tt>.
 * Call this function from the application-supplied function, <tt>rh_image_scanline</tt>.
 * <br><br>
 * <b>Effect:</b> System routine
 * <br><br>
 * <b>Journal: </b> Not Available
 * <br><br>
 * <b>Product(s):</b> 3D ACIS Exchange, 3D Viz Exchange, 3D ACIS Modeler
 * <br><br>
* @param input
 * array of pixel colors.
 * @param output
 * returns array of color map indexes.
 **/
 DECL_RB outcome api_rh_convert_rgb_float_scanline
	(
		LwFloat *input, LwInt32 *output
	);

/**
 * Gets the LightWorks image size.
 * <br><br>
 * <b>Role:</b> Gets the LightWorks image size.
 * <br><br>
 * <b>Effect:</b> System routine
 * <br><br>
 * <b>Journal: </b> Not Available
 * <br><br>
 * <b>Product(s):</b> 3D ACIS Exchange, 3D Viz Exchange, 3D ACIS Modeler
 * <br><br>
* @param fp
 * filename.
 * @param width
 * image width.
 * @param height
 * image height.
 **/
DECL_RB outcome api_rh_read_lightworks_image_size
	(
		FILE *fp,
		LwInt32 *width,
		LwInt32 *height
	);

/**
 * Gets the LightWorks image for display.
 * <br><br>
 * <b>Effect:</b> System routine
 * <br><br>
 * <b>Journal: </b> Not Available
 * <br><br>
 * <b>Product(s):</b> 3D ACIS Exchange, 3D Viz Exchange, 3D ACIS Modeler
 * <br><br>
* @param fp
 * filename.
 * @param channels
 * color channel.
 * @param width
 * image width.
 * @param height
 * image height.
 * @param data
 * image data.
 **/
DECL_RB outcome api_rh_read_lightworks_image
	(
		FILE *fp,
		LwInt32 channels,
		LwInt32 width,
		LwInt32 height,
		LwNat8 *data
	);


/**
 * Sets the display image parameters.
 * <br><br>
 * <b>Effect:</b> System routine
 * <br><br>
 * <b>Journal: </b> Not Available
 * <br><br>
 * <b>Product(s):</b> 3D ACIS Exchange, 3D Viz Exchange, 3D ACIS Modeler
 * <br><br>
* @param width
 * image width.
 * @param height
 * image height.
 * @param method
 * render mode.
 * @param n_colours
 * number of colors.
 * @param y
 * number of scan lines.
 * @param scan
 * input display.
 * @param y
 * number of scan lines.
 * @param scan
 * the output.
 * @param colours
 * set the color map for the output.
 **/
 DECL_RB outcome api_rh_display_image
	(
		LwInt32 width, LwInt32 height,
		LwDisplayMethod method, LwInt32 n_colours,
		LwVoid (*input_scanline)( LwInt32 y, LwNat8 **scan ),
		LwVoid (*output_scanline)( LwInt32 y, LwInt32 *scan ),
		LwVoid (*set_colour_map)( LwNat8 *colours )
	);

// Change the display color of an ENTITY

/**
 * Modifies the <tt>RGB</tt> color index of an <tt>ENTITY</tt>.
 * <br><br>
 * <b>Role:</b> Change the entity color by adding or changing an <tt>ATTRIB_RGB</tt>. Also, if the entity is
 * in the  display list, its color is changes. Any <tt>ATTRIB_COL</tt> is removed from the entity.
 * <br><br>
 * <b>Effect:</b> Changes model
 * <br><br>
 * <b>Journal: </b> Not Available
 * <br><br>
 * <b>Product(s):</b> 3D ACIS Exchange, 3D Viz Exchange, 3D ACIS Modeler, 3D ACIS Polyhedral
 * <br><br>
* @param ent
 * entity to modify.
 * @param color
 * new color index.
 **/
DECL_RB outcome api_rh_set_entity_rgb(
    ENTITY*           ent,           // ENTITY to modify
    rgb_color         color          // new color
    );

// Get the display color of an ENTITY
/**
 * Gets the <tt>RGB</tt> color of an entity.
 * <br><br>
 * <b>Role:</b> Searches the entity for one of four color attributes. Color precedence is as follows:<br>
 * <tt>ATTRIB_RGB</tt><br>
 * <tt>ATTRIB_COL</tt><br>
 * <tt>ATTRIB_RENDER</tt><br>
 * <tt>ATTRIB_IGES_COLOR</tt><br>
 * <br>
 * If none of these attributes are found on the given entity and the inherit flag is set, the given entity's
 * owner is examined in the same manner and so on up the hierarchy.
 * <br><br>
 * If no color attribute is found, the 'found' flag will be FALSE and the default color is returned (see <tt>api_rh_get_default_rgb</tt>).
 * <br><br>
 * <b>Effect:</b> Read-only
 * <br><br>
 * <b>Journal: </b> Not Available
 * <br><br>
 * <b>Product(s):</b> 3D ACIS Exchange, 3D Viz Exchange, 3D ACIS Modeler, 3D ACIS Polyhedral
 * <br><br>
* @param ent
 * entity to change.
 * @param color
 * color returned.
 * @param inherit
 * inherit color from owner?
 * @param found
 * Flag indicating that the color was explicitly set.
 **/
 DECL_RB outcome api_rh_get_entity_rgb(
    ENTITY*           ent,           // ENTITY to look at
    rgb_color&        color,         // color of entity
	logical			  inherit,  // Traverse up the tree to find the inherited color.
	logical&		  found     // Flag to indicate that the color was explicitly set.
    );

// Set the default color for newly created ENTITIES
/**
 * Modifies the default <tt>RGB</tt> color for newly-created entities.
 * <br><br>
 * <b>Role:</b> Use the changed color to display entities with no color attribute.
 * It is also used when color is <tt>-1</tt> in <tt>api_rh_set_entity_color</tt>.
 * <br><br>
 * <b>Effect:</b> Read-only
 * <br><br>
 * <b>Journal: </b> Not Available
 * <br><br>
 * <b>Product(s):</b> 3D ACIS Exchange, 3D Viz Exchange, 3D ACIS Modeler
 * <br><br>
* @param color
 * color to set.
 **/
DECL_RB outcome api_rh_set_default_rgb(
    rgb_color         color         // color to use when ENTITIES are displayed
    );

// Get the default color
/**
 * Gets the default <tt>RGB</tt> color for newly-created entities.
 * <br><br>
 * <b>Effect:</b> Read-only
 * <br><br>
 * <b>Journal: </b> Not Available
 * <br><br>
 * <b>Product(s):</b> 3D ACIS Exchange, 3D Viz Exchange, 3D ACIS Modeler
 * <br><br>
* @param color
 * returned color.
 **/
 DECL_RB outcome api_rh_get_default_rgb(
    rgb_color&         color        // the default color
    );
/**
 * Gets the default background <tt>RGB</tt> color for new toolkit graphics windows.
 * <br><br>
 * <b>Effect:</b> Read-only
 * <br><br>
 * <b>Journal: </b> Not Available
 * <br><br>
 * <b>Product(s):</b> 3D ACIS Exchange, 3D Viz Exchange, 3D ACIS Modeler
 * <br><br>
* @param color
 * returned color.
 **/
DECL_RB outcome api_rh_set_default_background_rgb(
    rgb_color         color
    );
/**
 * Gets the background default <tt>RGB</tt> color for new toolkit graphics windows.
 * <br><br>
 * <b>Effect:</b> Read-only
 * <br><br>
 * <b>Journal: </b> Not Available
 * <br><br>
 * <b>Product(s):</b> 3D ACIS Exchange, 3D Viz Exchange, 3D ACIS Modeler
 * <br><br>
* @param color
 * returned color.
 **/
DECL_RB outcome api_rh_get_default_background_rgb(
    rgb_color&         color
    );
/**
 * Gets the default <tt>RGB</tt> color for new face entities.
 * <br><br>
 * <b>Effect:</b> Read-only
 * <br><br>
 * <b>Journal: </b> Not Available
 * <br><br>
 * <b>Product(s):</b> 3D ACIS Exchange, 3D Viz Exchange, 3D ACIS Modeler
 * <br><br>
* @param color
 * returned color.
 **/
DECL_RB outcome api_rh_set_default_face_rgb(
    rgb_color         color
    );
/**
 * Gets the default <tt>RGB</tt> color for new face entities.
 * <br><br>
 * <b>Effect:</b> Read-only
 * <br><br>
 * <b>Journal: </b> Not Available
 * <br><br>
 * <b>Product(s):</b> 3D ACIS Exchange, 3D Viz Exchange, 3D ACIS Modeler
 * <br><br>
* @param color
 * returned color.
 **/
DECL_RB outcome api_rh_get_default_face_rgb(
    rgb_color&         color
    );
/**
 * Gets the default  <tt>RGB</tt> color for edge entities.
 * <br><br>
 * <b>Effect:</b> Read-only
 * <br><br>
 * <b>Journal: </b> Not Available
 * <br><br>
 * <b>Product(s):</b> 3D ACIS Exchange, 3D Viz Exchange, 3D ACIS Modeler
 * <br><br>
* @param color
 * returned color.
 **/
DECL_RB outcome api_rh_set_default_edge_rgb(
    rgb_color         color
    );
/**
 * Gets the default <tt>RGB</tt> color for new edge entities.
 * <br><br>
 * <b>Effect:</b> Read-only
 * <br><br>
 * <b>Journal: </b> Not Available
 * <br><br>
 * <b>Product(s):</b> 3D ACIS Exchange, 3D Viz Exchange, 3D ACIS Modeler
 * <br><br>
* @param color
 * returned color.
 **/
DECL_RB outcome api_rh_get_default_edge_rgb(
    rgb_color&         color
    );
/**
 * Gets the default  <tt>RGB</tt> color for new vertex entities.
 * <br><br>
 * <b>Effect:</b> Read-only
 * <br><br>
 * <b>Journal: </b> Not Available
 * <br><br>
 * <b>Product(s):</b> 3D ACIS Exchange, 3D Viz Exchange, 3D ACIS Modeler
 * <br><br>
* @param color
 * returned color.
 **/
DECL_RB outcome api_rh_set_default_vertex_rgb(
    rgb_color         color
    );
/**
 * Gets the default <tt>RGB</tt> color for new vertex entities.
 * <br><br>
 * <b>Effect:</b> Read-only
 * <br><br>
 * <b>Journal: </b> Not Available
 * <br><br>
 * <b>Product(s):</b> 3D ACIS Exchange, 3D Viz Exchange, 3D ACIS Modeler
 * <br><br>
* @param color
 * returned color.
 **/
DECL_RB outcome api_rh_get_default_vertex_rgb(
    rgb_color&         color
    );
// Set the rgb_color used to highlight ENTITIES
/**
 * Modifies the <tt>RGB</tt> color used to highlight entities.
 * <br><br>
 * <b>Role:</b> This <tt>API</tt> changes the color index used to display highlighted entities.
 * <br><br>
 * <b>Effect:</b> Changes model
 * <br><br>
 * <b>Journal: </b> Not Available
 * <br><br>
 * <b>Product(s):</b> 3D ACIS Exchange, 3D Viz Exchange, 3D ACIS Modeler
 * <br><br>
* @param color
 * new color index.
 **/
DECL_RB outcome api_rh_set_highlight_rgb(
    rgb_color          color        // the highlight color
    );

// Get the rgb_color used to highlight ENTITIES
/**
 * Gets the <tt>RGB</tt> color used to highlight entities.
 * <br><br>
 * <b>Role:</b> This <tt>API</tt> returns the <tt>RGB</tt> color used to highlight entities.
 * <br><br>
 * <b>Effect:</b> Read-only
 * <br><br>
 * <b>Journal: </b> Not Available
 * <br><br>
 * <b>Product(s):</b> 3D ACIS Exchange, 3D Viz Exchange, 3D ACIS Modeler
 * <br><br>
* @param color
 * highlight_rgb.
 **/
DECL_RB outcome api_rh_get_highlight_rgb(
    rgb_color&         color        // the highlight color
    );
/** @} */
#endif
