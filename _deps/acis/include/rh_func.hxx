/*******************************************************************/
/*    Copyright (c) 1989-2020 by Spatial Corp.                     */
/*    All rights reserved.                                         */
/*    Protected by U.S. Patents 5,257,205; 5,351,196; 6,369,815;   */
/*                              5,982,378; 6,462,738; 6,941,251    */
/*    Protected by European Patents 0503642; 69220263.3            */
/*    Protected by Hong Kong Patent 1008101A                       */
/*******************************************************************/
#if !defined( RENDERER_FUNC_HEADER )
#define RENDERER_FUNC_HEADER
//       Function prototypes for internal advanced rendering husk
//       functions to support 'api' functions. All functions return
//       a logical instead of outcome and have the same name,
//       bar the 'api_' pre-fix. Most functions are mirror functions
//       of 'api' functions. Some utility functions that are useful to
//       the api are also declared here.

#include <math.h>
#include "dcl_rb.h"
#include "rh_args.hxx"
/**
 * @file rh_func.hxx
 * @CAA2Level L1
 * @CAA2Usage U1
 * \addtogroup RENDERBASEAPI
 *
 * @{
 */

#ifndef NO_RENDERING

#ifndef ANSI
#define ANSI
#endif
#include "rh_types.h"
#endif

//
//
// Acis entities
//
//

class ENTITY;
class ENTITY_LIST ;
class BODY ;
class LUMP ;
class SHELL ;
class FACE ;
class SPAposition;
class RH_MATERIAL;
class RH_ENTITY_PROPS;
class SPAtransf;

//
// Rendering entities - derived from ENTITY
//

class RH_ENTITY;

//
//  Rendering entities provide the basis for manipulating the appearance
//  geometric entities, image backgrounds, foregrounds  and lighting
//  conditions within the rendering husk.  Rendering entities are derived
//  from class ENTITY.  There are currently five rendering entities, known
//  as, RH_LIGHT, RH_MATERIAL, RH_FOREGROUND, RH_BACKGROUND, RH_TEXTURE_SPACE
//  and RH_ENVIRONMENT_MAP.  Materials and texture spaces can be associated
//  with geometric entities.  Backgrounds and lights are independent of
//  geometry. RH_ENVIRONMENT_MAP is essentially a utility class that is used
//  in conjunction with other rendering entities.
//
//
//
//       class RH_LIGHT
//
//           Entities of class RH_LIGHT are used  to define a light source
//           within the rendering husk. Five types of lights sources are
//           currently supported; ambient, spot, distant, point and eye.
//           Lights spot, distant and point support shadows.
//           A shadow map must be computed for those lights
//           supporting shadowing prior to rendering.  A shadow map is view
//           independent and may be reused, but must be recomputed if there
//           is a change in geometry of the light source or entities which it
//           illuminates.

class RH_LIGHT ;

//
//        class RH_BACKGROUND
//
//           A background defines the color of a pixel at any point in
//           the image not covered by an entity surface. A background
//           may comprise a simple uniform color, pattern or be composed
//           of a previously generated image generated, for example,
//           from scanning  a photograph.

class RH_BACKGROUND ;

//
//        class RH_FOREGROUND
//
//           A foreground modifies to colors of all pixels in an image and
//           can be used to simulate fog, depth cue, dirty windows, etc.

class RH_FOREGROUND ;

//
//        class RH_MATERIAL
//
//           A material defines the appearance of an entity surface in
//           terms of a color source, reflectance model, transparency
//           source and displacement function.
//
//              A color source defines the color for any point on the
//              surface of an entity to  which it applies and may be a plain
//              single color or complex  pattern, such as, a marble effect.
//
//              The reflectance model  governs how the surface behaves
//              visually in the prescence of light. This can be a result of
//              any light sources and material reflection properties.
//              It may be thought of as defining the 'surface finish' of
//              an entity and can model  effects such as matte, metal and
//              plastic surfaces.
//
//              Transparency sources define how transparent or opaque a
//              surface is and hence how much light passes through it.
//
//              Displacement functions simulate small surface perturbations
//              by modifying the surface normal SPAvector. This provides an
//              efficient method of simulating surface features, such as,
//              regular indentations which would be inefficient or difficult
//              to model geometrically.
//

class RH_MATERIAL ;

//
//       class RH_TEXTURE_SPACE ;
//
//           RH_TEXTURE_SPACE entities are be used to produce a shading effect
//           known as a wrapped texture. As the name implies, a wrapped
//           texture produces an effect as though a sheet of paper had been
//           shrink wrapped onto the surface of a solid object.
//           A texture space may use one of several texture space shaders
//           to map between the the coordinate system of the sheet and
//           that of the surface of the solid object.

class RH_TEXTURE_SPACE ;

//
//      class RH_ENVIRONMENT_MAP
//
//          Environment maps can be used to simulate inter object reflections
//          both between bodies in a scene and between a body and its
//          external environment, say through the use of scanned images
//          An entity of class RH_ENVIRONMENT_MAP would in practice, be
//          used in conjunction with one of the component shaders of
//          the other rendering entities.
//
//
//

class RH_ENVIRONMENT_MAP;


//
// Internal enumerated type used to select a shader type in
// some functions. Note its convenient to treat an environment
// as a shader for the sake of some generic functions.
//
/**
 * Specifies the type of a shader.
 *<br>
 * @param RH_BACKGROUND_SHADER
 * background.
 * @param RH_FOREGROUND_SHADER
 * foreground.
 * @param RH_TEXTURE_SPACE_SHADER
 * texture space.
 * @param RH_LIGHT_SHADER
 * light.
 * @param RH_ENVIRONMENT_SHADER
 * environment map.
 * @param REFLECTANCE_COMP_SHADER
 * reflectance.
 * @param DISPLACEMENT_COMP_SHADER
 * displacement (bump mapping).
 * @param COLOR_COMP_SHADER
 * color.
 * @param TRANSPARENCY_COMP_SHADER
 * transparency.
 **/

enum Shader_Type
{
   RH_BACKGROUND_SHADER,
   RH_FOREGROUND_SHADER,
   RH_TEXTURE_SPACE_SHADER,
   RH_LIGHT_SHADER,
   RH_ENVIRONMENT_SHADER,
   REFLECTANCE_COMP_SHADER,
   DISPLACEMENT_COMP_SHADER,
   COLOR_COMP_SHADER,
   TRANSPARENCY_COMP_SHADER

} ;
//
//-----------------------------------------------------------------------------
//
// API FUNCTION DESCRIPTIONS
//
//-----------------------------------------------------------------------------
//
//
//  Rendering Control
//
//    Initialise renderer and bootstrap a minimum set of standard shaders
//    needed to support simple rendering
//
/*
// tbrv
*/
/**
 * @nodoc
 */
DECL_RB logical rh_initialise( void ) ;

/*
// tbrv
*/
/**
 * @nodoc
 */
DECL_RB logical rh_terminate( void ) ;

//
// Set and Get render mode
//
/*
// tbrv
*/
/**
 * @nodoc
 */
DECL_RB logical rh_set_render_mode( Render_Mode render_mode ) ;

/*
// tbrv
*/
/**
 * @nodoc
 */
DECL_RB logical rh_get_render_mode( Render_Mode& render_mode ) ;

//
// Render control mode functions
//
/*
// tbrv
*/
/**
 * @nodoc
 */
DECL_RB logical rh_set_control_variable
	(
	   Render_Control_Var   mode,
	   const Render_Arg    &value
	) ;

/*
// tbrv
*/
/**
 * @nodoc
 */
DECL_RB logical rh_get_control_variable
	(
	    Render_Control_Var mode,
	    Render_Arg    &value
	) ;
//
// Set and Get Scanline  format for image output
//
/*
// tbrv
*/
/**
 * @nodoc
 */
logical rh_set_output_mode( Output_Mode output_mode ) ;

/*
// tbrv
*/
/**
 * @nodoc
 */
logical rh_get_output_mode( Output_Mode& output_mode ) ;

//
// Render one or more ACIS Geometric Entities
//
//  If clear_screen, then graphics region will be cleared before
//  render
//
/*
// tbrv
*/
/**
 * @nodoc
 */
DECL_RB logical rh_render_entities
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
/*
// tbrv
*/
/**
 * @nodoc
 */
DECL_RB logical rh_initialise_supl_shaders( void ) ;

//----------------------------------------------------------------------------
//
// RH_MATERIAL comp manipulation functions
//
//     Components are refered to by name ( const char * )
//     and set via a Render_Arg ( see the Render_Arg ) definition
//     for a fuller explanation on Render_Args
//
//       eg. rh_set_reflect_comp( a_material, "plastic" );
//
//     defines the relectance comp of a_material to be of
//     type "plastic".
//
//      Render_Arg ka( ( double ) 0.5 );
//      rh_set_reflect_arg( a_material,"ambient factor", ka );
//
//     would set the ambient attribute of the reflectance comp "plastic"
//     to 0.5.
//
//     Available comps depend on what has been created using
//     the shader initialisation functions. Their names can obtained
//     by calling a list function for each comp type.
//
//       eg.
//           rh_get_reflect_comp_list( &rnum, rlist );
//
//     would return a list of names of available reflectance comps
//
//----------------------------------------------------------------------------
/*
// tbrv
*/
/**
 * @nodoc
 */
DECL_RB logical rh_create_material( RH_MATERIAL*& material ) ;
/*
// tbrv
*/
/**
 * @nodoc
 */
DECL_RB logical rh_copy_material( RH_MATERIAL* orig, RH_MATERIAL*& copy ) ;
/*
// tbrv
*/
/**
 * @nodoc
 */
DECL_RB logical rh_delete_material( RH_MATERIAL* material ) ;


//
// Set the Color Component of a RH_MATERIAL to a named color shader
//             eg. "plain" or "wrapped checker"
//
/*
// tbrv
*/
/**
 * @nodoc
 */
DECL_RB logical rh_set_color_comp
	(
	   RH_MATERIAL* material,
	   const char* color_shader_name
	);

//
// Get Attribute names, types and values of color comp of a material
//
/*
// tbrv
*/
/**
 * @nodoc
 */
DECL_RB logical rh_get_color_comp
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
/*
// tbrv
*/
/**
 * @nodoc
 */
DECL_RB logical rh_set_transp_comp
	(
	   RH_MATERIAL*   material,
	   const char* transparency_shader_name
	) ;
//
// Get Attribute names, types and values of transparency comp
//
/*
// tbrv
*/
/**
 * @nodoc
 */
DECL_RB logical rh_get_transp_comp
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
//    If on is TRUE then the transparency comp of a RH_MATERIAL
//    will take effect for those rendering modes where it is
//    applicable
//
/*
// tbrv
*/
/**
 * @nodoc
 */
DECL_RB logical rh_set_transp_status
	(
	   RH_MATERIAL* material,
	   logical   on
	) ;
/*
// tbrv
*/
/**
 * @nodoc
 */
DECL_RB logical rh_get_transp_status
	(
	   RH_MATERIAL* material,
	   logical   &on
	) ;

//
// Set and Get status of Reflection
//
//    If on is TRUE then the reflection part of a RH_MATERIAL
//    will take effect for those rendering modes where it is
//    applicable
//
/*
// tbrv
*/
/**
 * @nodoc
 */
DECL_RB logical rh_set_reflect_status
	(
	   RH_MATERIAL* material,
	   logical   on
	) ;
/*
// tbrv
*/
/**
 * @nodoc
 */
DECL_RB logical rh_get_reflect_status
	(
	   RH_MATERIAL* material,
	   logical   &on
	) ;

//
//  Set the Reflectance Component of a RH_MATERIAL to a named reflectance
//  shader eg. "matte" or "mirror"
//
/*
// tbrv
*/
/**
 * @nodoc
 */
DECL_RB logical rh_set_reflect_comp
	(
	   RH_MATERIAL* material,
	   const char* reflectance_shader_name
	) ;

//
// Get Attribute names, types and values of reflectance comp
//
/*
// tbrv
*/
/**
 * @nodoc
 */
DECL_RB logical rh_get_reflect_comp
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
/*
// tbrv
*/
/**
 * @nodoc
 */
DECL_RB logical rh_set_displace_comp
	(
	   RH_MATERIAL* material,
	   const char*  name
	) ;
//
// Get Attribute names, types and values of displacement comp
//
/*
// tbrv
*/
/**
 * @nodoc
 */
DECL_RB logical rh_get_displace_comp
	(
	   RH_MATERIAL*    material,
	   const char*       &name,
	   int         &no_of_args,
	   const char**      &arg_names,
	   Render_Arg* &arg_values
	) ;

//
// Set and Get status of displacement
//
//    If on is TRUE then the displacement comp a RH_MATERIAL
//    will take effect for those rendering modes where it is
//    applicable
//
/*
// tbrv
*/
/**
 * @nodoc
 */
DECL_RB logical rh_set_displace_status( RH_MATERIAL* material, logical on ) ;
/*
// tbrv
*/
/**
 * @nodoc
 */
DECL_RB logical rh_get_displace_status( RH_MATERIAL* material, logical& on ) ;
//
// Material comp enquiry functions
//
//     These functions return a pointer to a list of names of
//     all currently available  material comps for color,
//     transparency,  reflectance and  displacement, respectively.
//
/*
// tbrv
*/
/**
 * @nodoc
 */
DECL_RB logical rh_get_color_comp_list
	(
	   int    &n_colors,
	   const char** &names
	) ;

/*
// tbrv
*/
/**
 * @nodoc
 */
DECL_RB logical rh_get_transp_comp_list
	(
	   int    &n_transparencies,
	   const char** &names
	) ;

/*
// tbrv
*/
/**
 * @nodoc
 */
DECL_RB logical rh_get_reflect_comp_list
	(
	   int    &n_reflects,
	   const char** &names
	) ;
/*
// tbrv
*/
/**
 * @nodoc
 */
DECL_RB logical rh_get_displace_comp_list
	(
	   int    &n_displaces,
	   const char** &names
	) ;

//
// Material comp argument functions
//
//     Functions set argument values  for color, transparency,
//     reflectanced  displacement comps of material
//
/*
// tbrv
*/
/**
 * @nodoc
 */
DECL_RB logical rh_set_color_arg
	(
	   RH_MATERIAL*        material,
	   const char*      arg_name,
	   const Render_Arg &value
	) ;
/*
// tbrv
*/
/**
 * @nodoc
 */
DECL_RB logical rh_set_transp_arg
	(
	   RH_MATERIAL*          material,
	   const char*        arg_name,
	   const Render_Arg  &value
	) ;
/*
// tbrv
*/
/**
 * @nodoc
 */
DECL_RB logical rh_set_reflect_arg
	(
	   RH_MATERIAL*         material,
	   const char*       arg_name,
	   const Render_Arg &value
	) ;
/*
// tbrv
*/
/**
 * @nodoc
 */
DECL_RB logical rh_set_displace_arg
	(
	   RH_MATERIAL*          material,
	   const char*        arg_name,
	   const Render_Arg  &value
	) ;

//
// Set the def comp names for a new RH_MATERIAL
//
// RH_MATERIALs created by subsequent calls to rh_create_material
// will contain the comp types  set using these functions
//
/*
// tbrv
*/
/**
 * @nodoc
 */
DECL_RB logical rh_def_color_comp( const char *name ) ;
/*
// tbrv
*/
/**
 * @nodoc
 */
DECL_RB logical rh_def_transp_comp( const char *name ) ;
/*
// tbrv
*/
/**
 * @nodoc
 */
DECL_RB logical rh_def_reflect_comp( const char *name ) ;
/*
// tbrv
*/
/**
 * @nodoc
 */
DECL_RB logical rh_def_displace_comp( const char *name ) ;

//----------------------------------------------------------------------------
//
// RH_LIGHT  Manipulation Functions
//
//        Light arguments are handled in a similar manner to the
//        comp arguments of materials
//
//----------------------------------------------------------------------------
/*
// tbrv
*/
/**
 * @nodoc
 */
DECL_RB logical rh_create_light( const char* type, RH_LIGHT*& light ) ;

/*
// tbrv
*/
/**
 * @nodoc
 */
DECL_RB logical rh_copy_light( RH_LIGHT* orig, RH_LIGHT*& copy ) ;

/*
// tbrv
*/
/**
 * @nodoc
 */
DECL_RB logical rh_delete_light( RH_LIGHT* light ) ;

// STI DJP Begin...
/*
// tbrv
*/
/**
 * @nodoc
 */
DECL_RB logical rh_roll_add_light( RH_LIGHT *light ) ;

/*
// tbrv
*/
/**
 * @nodoc
 */
DECL_RB logical rh_roll_remove_light( RH_LIGHT *light ) ;

// STI DJP End.
/*
// tbrv
*/
/**
 * @nodoc
 */
DECL_RB logical rh_set_light_state( RH_LIGHT* light, logical on_off );

/*
// tbrv
*/
/**
 * @nodoc
 */
DECL_RB logical rh_get_light_state( RH_LIGHT* light );


//
// Get a list of available light type names
//
/*
// tbrv
*/
/**
 * @nodoc
 */
DECL_RB logical rh_get_light_types( int& n_types, const char** &names ) ;

//
//  Create a shadow map for a particular light and list
//  of ACIS Geometric entities
//
/*
// tbrv
*/
/**
 * @nodoc
 */
DECL_RB logical rh_create_light_shadow
	(
	   RH_LIGHT *light,
	   ENTITY_LIST const &entities
	) ;

/*
// tbrv
*/
/**
 * @nodoc
 */
DECL_RB logical rh_delete_light_shadow( RH_LIGHT *light ) ;

//
// Set and Get a list of current lights
//
/*
// tbrv
*/
/**
 * @nodoc
 */
DECL_RB logical rh_set_light_list( ENTITY_LIST const &lights ) ;

/*
// tbrv
*/
/**
 * @nodoc
 */
DECL_RB logical rh_get_light_list( ENTITY_LIST& lights ) ;

//
// Manipulate the light arguments
//
/*
// tbrv
*/
/**
 * @nodoc
 */
DECL_RB logical rh_get_light_args
	(
	   RH_LIGHT*       light,
	   const char*       &type,
	   int         &no_of_args,
	   const char**      &arg_names,
	   Render_Arg* &arg_values
	) ;
/*
// tbrv
*/
/**
 * @nodoc
 */
DECL_RB logical rh_set_light_arg
	(
	   RH_LIGHT*            light,
	   const char*       arg_name,
	   const Render_Arg &value
	);


//-----------------------------------------------------------------------------
//
// RH_BACKGROUND Manipulation Functions
//
//   Backgrounds are treated as for RH_MATERIAL comps and RH_LIGHTs.
//   However  only  one RH_BACKGROUND may exist for the current
//   rendering environment.
//
//-----------------------------------------------------------------------------

/*
// tbrv
*/
/**
 * @nodoc
 */
DECL_RB logical rh_get_background_types
	(
	   int    &n_backgrounds,
	   const  char** &names
	) ;

/*
// tbrv
*/
/**
 * @nodoc
 */
DECL_RB logical rh_create_background
	(
	   const char*  type,
	   RH_BACKGROUND* &background
	) ;

/*
// tbrv
*/
/**
 * @nodoc
 */
DECL_RB logical rh_copy_background( RH_BACKGROUND* orig, RH_BACKGROUND*& copy ) ;

/*
// tbrv
*/
/**
 * @nodoc
 */
DECL_RB logical rh_delete_background( RH_BACKGROUND* background ) ;

/*
// tbrv
*/
/**
 * @nodoc
 */
DECL_RB logical rh_set_background( RH_BACKGROUND* background ) ;

/*
// tbrv
*/
/**
 * @nodoc
 */
DECL_RB logical rh_get_background( RH_BACKGROUND*& background ) ;

/*
// tbrv
*/
/**
 * @nodoc
 */
DECL_RB logical rh_get_background_args
	(
	   RH_BACKGROUND*  background,
	   const char*       &name,
	   int         &no_of_args,
	   const char**      &arg_names,
	   Render_Arg* &arg_values
	) ;

/*
// tbrv
*/
/**
 * @nodoc
 */
DECL_RB logical rh_set_background_arg
	(
	   RH_BACKGROUND*       background,
	   const char*       arg_name,
	   const Render_Arg &value
	) ;

// STI DJP Begin...

/*
// tbrv
*/
/**
 * @nodoc
 */
DECL_RB logical rh_roll_background( RH_BACKGROUND * bg ) ;
// STI DJP End.

//-----------------------------------------------------------------------------
//
// RH_FOREGROUND Manipulation Functions
//
//   Foregrounds are treated as for RH_MATERIAL comps and RH_LIGHTs.
//   However  only  one RH_FOREGROUND may exist for the current
//   rendering environment.
//
//-----------------------------------------------------------------------------

/*
// tbrv
*/
/**
 * @nodoc
 */
DECL_RB logical rh_get_foreground_types
	(
	   int    &n_foregrounds,
	   const  char** &names
	) ;

/*
// tbrv
*/
/**
 * @nodoc
 */
DECL_RB logical rh_create_foreground
	(
	   const char*  type,
	   RH_FOREGROUND* &foreground
	) ;

/*
// tbrv
*/
/**
 * @nodoc
 */
DECL_RB logical rh_copy_foreground( RH_FOREGROUND* orig, RH_FOREGROUND*& copy ) ;

/*
// tbrv
*/
/**
 * @nodoc
 */
DECL_RB logical rh_delete_foreground( RH_FOREGROUND* foreground ) ;

/*
// tbrv
*/
/**
 * @nodoc
 */
DECL_RB logical rh_set_foreground( RH_FOREGROUND* foreground ) ;

/*
// tbrv
*/
/**
 * @nodoc
 */
DECL_RB logical rh_get_foreground( RH_FOREGROUND*& foreground ) ;

/*
// tbrv
*/
/**
 * @nodoc
 */
DECL_RB logical rh_get_foreground_args
	(
	   RH_FOREGROUND*  foreground,
	   const char*       &name,
	   int         &no_of_args,
	   const char**      &arg_names,
	   Render_Arg* &arg_values
	) ;

/*
// tbrv
*/
/**
 * @nodoc
 */
DECL_RB logical rh_set_foreground_arg
	(
	   RH_FOREGROUND*       foreground,
	   const char*       arg_name,
	   const Render_Arg &value
	) ;

// STI DJP Begin...
/*
// tbrv
*/
/**
 * @nodoc
 */
DECL_RB logical rh_roll_foreground( RH_FOREGROUND * bg ) ;
// STI DJP End.

//-----------------------------------------------------------------------------
//
//  TEXTURE SPACE Manipulation functions
//
//      Texture spaces are treated in a similar fashion to
//      RH_MATERIAL comps.
//
//-----------------------------------------------------------------------------
/*
// tbrv
*/
/**
 * @nodoc
 */
DECL_RB logical rh_get_texture_space_types( int& n_types, const char** &names ) ;

/*
// tbrv
*/
/**
 * @nodoc
 */
DECL_RB logical rh_create_texture_space
	(
	   const char*     name,
	   RH_TEXTURE_SPACE*& texture_space
	       ) ;

/*
// tbrv
*/
/**
 * @nodoc
 */
DECL_RB logical rh_copy_texture_space
	(
	   RH_TEXTURE_SPACE*  orig,
	   RH_TEXTURE_SPACE*& copy
	) ;

/*
// tbrv
*/
/**
 * @nodoc
 */
DECL_RB logical rh_delete_texture_space( RH_TEXTURE_SPACE* texture_space ) ;

/*
// tbrv
*/
/**
 * @nodoc
 */
DECL_RB logical rh_get_texture_space_args
	(
	   RH_TEXTURE_SPACE*  texture_space,
	   const char*          &name,
	   int            &no_of_args,
	   const char**         &arg_names,
	   Render_Arg*    &arg_values
	) ;

/*
// tbrv
*/
/**
 * @nodoc
 */
DECL_RB logical rh_set_texture_space_arg
	(
	   RH_TEXTURE_SPACE*    texture_space,
	   const char*       arg_name,
	   const Render_Arg &value
	) ;


//-----------------------------------------------------------------------------
//
// Environment mapping functions
//
//     Environment mapping is supported for all shading modes but
//     FLAT and SIMPLE. Inter-object reflections may be simulated
//     by computing an environment map for the currently defined
//     entities.  More commonly a scanned image may form the basis
//     for a environment map.
//
//-----------------------------------------------------------------------------


//
// Create an environment map from a list of entities
//
/*
// tbrv
*/
/**
 * @nodoc
 */

DECL_RB logical rh_render_cube_environment
	(
	   ENTITY_LIST const &entities,
	   int               resolution,
	   SPAposition const    &centre,
	   RH_ENVIRONMENT_MAP*  &map
	) ;

/*
// tbrv
*/
/**
 * @nodoc
 */
DECL_RB logical rh_delete_environment_map
       (
	   RH_ENVIRONMENT_MAP* map
       ) ;

/*
// tbrv
*/
/**
 * @nodoc
 */
DECL_RB logical rh_set_environment_map
       (
	   RH_ENVIRONMENT_MAP* map
       ) ;

/*
// tbrv
*/
/**
 * @nodoc
 */
DECL_RB logical rh_get_environment_map
       (
	   RH_ENVIRONMENT_MAP* &map
       ) ;


//
// Create an environment map from 6 images
//
//   If the image  access function is NULL, then the def action
//   is to treat the  data parameters as a pointer to a raster of pixels
//   stored in row major order
//

/*
// tbrv
*/
/**
 * @nodoc
 */

DECL_RB logical rh_create_cube_environment
       (
	  unsigned              char* data[6],
	  int                   channels,
	  int                   width[6],
	  int                   height[6],
	  Image_Access_Function afunc,
	  RH_ENVIRONMENT_MAP*      &map
       ) ;

//-----------------------------------------------------------------------------
//
// GEOMETRIC ENTITY Attribute Functions
//
//-----------------------------------------------------------------------------
//

//
//   Set and get the a texture space or matrial attribute for a
//   geometric entity, where an ENTITY may be one of the following
//   BODY, LUMP, SHELL, FACE
//
//

/*
// tbrv
*/
/**
 * @nodoc
 */
DECL_RB logical rh_set_material
	(
	   ENTITY_LIST  const &entitities,
	   RH_MATERIAL     *material = (RH_MATERIAL *)NULL
	) ;
/*
// tbrv
*/
/**
 * @nodoc
 */
DECL_RB logical rh_get_material
	(
	   ENTITY   *entity,
	   RH_MATERIAL*& material,
	   logical inherit = FALSE  // Traverse up the tree to find the inherited material for the transparency.
	   );
/*
// tbrv
*/
/**
 * @nodoc
 */
DECL_RB logical rh_set_texture_space
	(
	   ENTITY_LIST   const &entities,
	   RH_TEXTURE_SPACE *texture_space = (RH_TEXTURE_SPACE *)NULL
	) ;

/*
// tbrv
*/
/**
 * @nodoc
 */
DECL_RB logical rh_get_texture_space
	(
	   ENTITY         *entity ,
	   RH_TEXTURE_SPACE*& texture_space
	) ;


//
// Sidedness
//
// Define the surface of an entity to be single or double-sided
// for rendering pruposes. This is neccessary to support transparency
// and may be useful for say rendering faces of non-manifold objects
// where it is desirable to view both sides of a face.
//

/*
// tbrv
*/
/**
 * @nodoc
 */
DECL_RB logical rh_set_sidedness
	(
	   ENTITY_LIST const &entities,
	   int     sides
	) ;

/*
// tbrv
*/
/**
 * @nodoc
 */
DECL_RB logical rh_get_sidedness
	(
	   ENTITY  *entity,
	   int     &sides
	) ;




// ************************************************************************
//
//  Utility functions
//
// ************************************************************************

//
//
//  Function: rh_set_entity_material
//
//  Description:
//
//    Creates an internal Husk copy of a light list
//

/*
// tbrv
*/
/**
 * @nodoc
 */
DECL_RB logical rh_save_light_list( ENTITY_LIST const &lights ) ;

//
//
//  Function: rh_set_entity_material
//
//  Description:
//
//    Attaches a material to an entity
//

/*
// tbrv
*/
/**
 * @nodoc
 */
DECL_RB logical rh_set_entity_material( ENTITY *entity, RH_MATERIAL *mtl);


//
//  Function: rh_set_sides
//
//  Description:
//
//    Sets the sidedness of an entity
//

/*
// tbrv
*/
/**
 * @nodoc
 */
DECL_RB logical rh_set_sides( ENTITY *entity, int sd );


//
//  Function: rh_set_entity_texture_space
//
//  Description:
//
//    Sets the texture space associated with an entity
//

/*
// tbrv
*/
/**
 * @nodoc
 */
DECL_RB logical rh_set_entity_texture_space
   (
      ENTITY         *entity,
      RH_TEXTURE_SPACE *tsp
   );

// ************************************************************************
//
// STANDARD HUSK COMPATIBILITY FUNCTIONS
//
// ************************************************************************

/*
// tbrv
*/
/**
 * @nodoc
 */
logical rh_render_std_entities
	(
	   ENTITY_LIST const  &entities,
	   ENTITY_LIST const &lights,
	   RH_BACKGROUND         *background,
	   Render_Mode         mode,
	   logical             clear_screen = TRUE
	) ;

/*
// tbrv
*/
/**
 * @nodoc
 */
logical rh_set_std_material
	(
	  RH_MATERIAL*     material,
	  Render_Color const &color,
	  double         ka,
	  double         kd,
	  double         ks,
	  double         phong_exponent
	) ;

/*
// tbrv
*/
/**
 * @nodoc
 */
logical rh_get_std_material
	(
	   RH_MATERIAL*     material,
	   Render_Color  &color,
	   double          &ka,
	   double          &kd,
	   double          &ks,
	   double          &phong_exponent
	) ;

/*
// tbrv
*/
/**
 * @nodoc
 */
logical rh_set_std_light
	(
	   RH_LIGHT*              light,
	   double                intensity,
	   Render_Color  const &color,
	   SPAposition      const &pos
	) ;

/*
// tbrv
*/
/**
 * @nodoc
 */
DECL_RB logical rh_get_std_light
	(
	   RH_LIGHT*        light,
	   const char*        &name,
	   double         &intensity,
	   Render_Color &color,
	   SPAposition     &pos
	) ;

/*
// tbrv
*/
/**
 * @nodoc
 */
logical rh_set_std_background
	(
	   RH_BACKGROUND*         background,
	   Render_Color const &color_1,
	   Render_Color const &color_2
	) ;

/*
// tbrv
*/
/**
 * @nodoc
 */
DECL_RB logical rh_get_std_background
	(
	   RH_BACKGROUND*       background,
	   const char*        &name,
	   Render_Color &color_1,
	   Render_Color &color_2
	) ;


// --------------------------------------------------------------------------
//
// Utility Functions to support journalling and replay of Render Args.
//
//   Although not defined in rh_func.cxx, they are declared here
//   because they are useful internally to api functions. They are defined
//   in rh_util.cxx.
//
// --------------------------------------------------------------------------

// rh_cvt_from_render_args
//
//   Converts an array of render arg values into a form
//   suitable for journalling.
//

/*
// tbrv
*/
/**
 * @nodoc
 */
DECL_RB void rh_cvt_from_render_args

   (
      int         arg_length,
      Render_Arg *arg_arr,
      int        *type_arr,
      int        &value_length,
      double     *value_arr
   );

//
// rh_cvt_to_render_args
//
//   Convert data in journal format to Render_Args.
//
/*
// tbrv
*/
/**
 * @nodoc
 */
DECL_RB void rh_cvt_to_render_args
   (
      int         type_length,
      int        *type_arr,
      double     *value_arr ,
      Render_Arg *arg_arr
   );


//
//  rh_cvt_to_cstring
//
//    A utility function  which creates a null terminated string
//    from a non-null terminated string of known length.
//    Used mainly by api functions to change 'replayed' string
//    arguments to the null terminated strings required by several
//    api functions.
//
/*
// tbrv
*/
/**
 * @nodoc
 */
DECL_RB char * rh_cvt_to_cstring( int len, char *oldtsr );

// --------------------------------------------------------------------------
//
// Utility Functions to argument checking in api functions
//
//   Although not defined in rh_func.cxx, they are declared here
//   because they are useful internally to api functions. They are defined
//   in rh_autil.cxx.
//
// --------------------------------------------------------------------------

//
// rh_check_shader
//
//  Check that the entity identity matches the
//  expected render entity type for a function
//
/*
// tbrv
*/
/**
 * @nodoc
 */
DECL_RB void rh_check_shader
   (
      ENTITY * ent,
      Shader_Type expected_type
   );

//
// rh_check_shader_type
//
//   Check that a shader type exists for a given class
//
/*
// tbrv
*/
/**
 * @nodoc
 */
DECL_RB void rh_check_shader_type
   (
      Shader_Type its_type,
      const char *type_name
   );

//
// rh_check_shader_arg
//
//   Check that a shader type exists for a given class and
//   if it does then optionally check that a requested shader
//   argument actually exists for a given shader.
//   If it does return its current value
//
/*
// tbrv
*/
/**
 * @nodoc
 */
DECL_RB void rh_check_shader_arg
   (
      RH_ENTITY  *ent,
      Shader_Type its_type,
      const char *arg_name,
      Render_Arg  *exp_arg  = NULL
   );

//
// rh_check_shader_value
//
//  Check that a shader arg is valid and is passed the correct type
//
/*
// tbrv
*/
/**
 * @nodoc
 */
DECL_RB void rh_check_shader_value
   (
      RH_ENTITY  *ent,
      Shader_Type its_type,
      const char *arg_name,
      const Render_Arg &arg_value
   );


//
// rh_check_material
//
//  Check that a RH_ENTITY is a valid MATERIAL
//
/*
// tbrv
*/
/**
 * @nodoc
 */
DECL_RB void rh_check_material
   (
      RH_MATERIAL  *mtl
   );

// --------------------------------------------------------------------------
//
// Debug functions for render entity details
//
// --------------------------------------------------------------------------
/*
// tbrv
*/
/**
 * @nodoc
 */
DECL_RB void rh_debug_light( RH_LIGHT * , FILE * );

/*
// tbrv
*/
/**
 * @nodoc
 */
DECL_RB void rh_debug_material( RH_MATERIAL * , FILE * );

/*
// tbrv
*/
/**
 * @nodoc
 */
DECL_RB void rh_debug_texture_space( RH_TEXTURE_SPACE * , FILE * );

/*
// tbrv
*/
/**
 * @nodoc
 */
DECL_RB void rh_debug_background( RH_BACKGROUND * , FILE * );

/*
// tbrv
*/
/**
 * @nodoc
 */
DECL_RB void rh_debug_foreground( RH_FOREGROUND * , FILE * );

/*
// tbrv
*/
/**
 * @nodoc
 */
DECL_RB void rh_debug_environment_map( RH_ENVIRONMENT_MAP * , FILE * );

//
//  Function: rh_get_render_attributes
//
//  Description:
//   A utility function which tests if two ACIS transfs are
//   the same ( within ACIS tolerances )
//
//  Note:  This function may be generally useful and could be moved into
//         the kernel.
/*
// tbrv
*/
/**
 * @nodoc
 */
DECL_RB logical rh_equal_transf( SPAtransf const &tran1, SPAtransf const & tran2 );

//
// rh_calc_primitive_transf
//
//    Determine if any transform needs to be applied to the
//    primitive facets generated for an entity
//
/*
// tbrv
*/
/**
 * @nodoc
 */
DECL_RB logical  rh_calc_primitive_transf
   (
      ENTITY                 * entity,
      RH_ENTITY_PROPS const  & props,
      SPAtransf                 & prim_tran
   );

//
// Lightworks Image Utility routines.
/*
// tbrv
*/
/**
 * @nodoc
 */
DECL_RB void rh_initialise_image_utilities();

/*
// tbrv
*/
/**
 * @nodoc
 */
DECL_RB void rh_terminate_image_utilities();

/*
// tbrv
*/
/**
 * @nodoc
 */
DECL_RB void rh_set_conversion_method( LwConversionMethod method );

/*
// tbrv
*/
/**
 * @nodoc
 */
DECL_RB void rh_set_conversion_colour_map
	(
		LwInt32 r_max, LwInt32 g_max, LwInt32 b_max,
		LwInt32 r_mult, LwInt32 g_mult, LwInt32 b_mult,
		LwInt32 base_index
	);

/*
// tbrv
*/
/**
 * @nodoc
 */
DECL_RB void rh_convert_image_start( LwInt32 width, LwInt32 height );

/*
// tbrv
*/
/**
 * @nodoc
 */
DECL_RB void rh_convert_image_end( LwVoid );

/*
// tbrv
*/
/**
 * @nodoc
 */
DECL_RB void rh_convert_rgb_scanline
	(
		LwNat8 *input, LwInt32 *output
	);

/*
// tbrv
*/
/**
 * @nodoc
 */
DECL_RB void rh_convert_rgb_float_scanline
	(
		LwFloat *input, LwInt32 *output
	);

/*
// tbrv
*/
/**
 * @nodoc
 */
DECL_RB void rh_read_lightworks_image_size
	(
		FILE *fp,
		LwInt32 *width,
		LwInt32 *height
	);

/*
// tbrv
*/
/**
 * @nodoc
 */
DECL_RB void rh_read_lightworks_image
	(
		FILE *fp,
		LwInt32 channels,
		LwInt32 width,
		LwInt32 height,
		LwNat8 *data
	);

/*
// tbrv
*/
/**
 * @nodoc
 */
DECL_RB void rh_display_image
	(
		LwInt32 width, LwInt32 height,
		LwDisplayMethod method, LwInt32 n_colours,
		LwVoid (*input_scanline)( LwInt32 y, LwNat8 **scan ),
		LwVoid (*output_scanline)( LwInt32 y, LwInt32 *scan ),
		LwVoid (*set_colour_map)( LwNat8 *colours )
	);

/** @} */

#endif
