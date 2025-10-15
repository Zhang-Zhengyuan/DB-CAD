/* ORIGINAL: rbase2.1/rnd_husk/entities/rh_efunc.hxx */
// $Id: rh_efunc.hxx,v 1.22 2002/08/09 17:20:13 jeff Exp $
/*******************************************************************/
/*    Copyright (c) 1989-2020 by Spatial Corp.                     */
/*    All rights reserved.                                         */
/*    Protected by U.S. Patents 5,257,205; 5,351,196; 6,369,815;   */
/*                              5,982,378; 6,462,738; 6,941,251    */
/*    Protected by European Patents 0503642; 69220263.3            */
/*    Protected by Hong Kong Patent 1008101A                       */
/*******************************************************************/

#if !defined( RENDERER_EFUNC_HEADER )
#define RENDERER_EFUNC_HEADER
//
// ****************************************************************************
//
//  File name : $RCSfile: rh_efunc.hxx,v $
//
//  Description :
//
//       Prototypes for internal advanced rendering husk
//       functions that interface to LightWork Design software
//       Many functions map directly onto a LightWorks rendering
//       function. The 'utility' functions declared at the end
//       do not.
//
//       This level of the Husk interface is intended to isolate
//       LightWorks ADS data types and functions calls.
//
//
//  ***************************************************************************
//
//  History:
//     27-March-95   JMB created by copying from rh_li.hxx of BR husk
//

// ****************************************************************************
//
//   DATA TYPES
//
// ****************************************************************************

#include "dcl_rb.h"
// ywoo 08Jan01: added the header file.
#include "base.hxx"
// ywoo: end

#include <math.h>

class SPAtransf;

#include "rh_args.hxx"
#include "rh_func.hxx"

class SPAbox;
struct PdbComponent ;
struct PdbMaterial ;
struct PdbLight ;
struct PdbBackground ;
struct PdbForeground ;
struct Shader ;
class ShaderSet ;
class RH_BACKGROUND;
class RH_FOREGROUND;
class RH_ENVIRONMENT_MAP;
class ENTITY_LIST;

// ----------------------------------------------------------------------------
// Initialise the shader table so that the user can define their own shaders
// and have them be known by the save/restore and other systems.  A shader not
// in the table has its data skipped on restore.  Further, if it is a material
// component (color, reflectance, transparency, displacement), the remaining
// components not yet read are skipped as well.
//
// The advanced rendering shader compiler has been modified to add code to
// extend the table in the create functions for the generated shaders.
// ---------------------------------------------------------------------------
DECL_RB logical rh_initialise_shader_table( void );

// ----------------------------------------------------------------------------
// Some yucky globals that communicate between the rendering base and
// a particular renderer.  These may eventually get packaged into a
// Renderer class from which each particular renderer technology can derive.
// ----------------------------------------------------------------------------

//
// Current background and Foreground entities
//
#ifdef THREAD_SAFE_ACIS
extern DECL_RB safe_object_pointer<RH_BACKGROUND> current_background;
extern DECL_RB safe_object_pointer<RH_FOREGROUND> current_foreground;
extern DECL_RB safe_object_pointer<RH_ENVIRONMENT_MAP> current_env_map;
#else
extern DECL_RB RH_BACKGROUND      * current_background;
extern DECL_RB RH_FOREGROUND      * current_foreground;
extern DECL_RB RH_ENVIRONMENT_MAP * current_env_map;
#endif

//
// RH_LIGHTs state
//   This interface maintains a local list of active RH_LIGHTs
//
#ifdef THREAD_SAFE_ACIS
extern DECL_RB safe_object_pointer<ENTITY_LIST> g_current_lights;
#else
extern DECL_RB ENTITY_LIST        * g_current_lights;
#endif

// ----------------------------------------------------------------------------
// End of yucky globals.  Now some functions dealing with them.  Each
// Rendering technology must provide these.  These functions read the
// globals and create the appropriate renderer specific internal data
// structures.
// ----------------------------------------------------------------------------
DECL_RB void rh_update_active_light_list();
DECL_RB void rh_update_current_background();
DECL_RB void rh_update_current_foreground();

//
//  ACIS equality test
//
//
inline logical fequal( double v1 , double v2 )
{
   return( fabs( v2 - v1 ) < SPAresabs );
}

// ----------------------------------------------------------------------------
//
// Create LightWorks Shaders to support RH_ENTITYs
//
// ----------------------------------------------------------------------------

//
// Material State and Components
//
DECL_RB void* pi_create_state();
DECL_RB void  pi_delete_state( void *lihandle );
DECL_RB void* pi_copy_state( void *old );

DECL_RB void  pi_set_transparency_status( void *, logical );
DECL_RB void  pi_get_transparency_status( void *, logical & );
DECL_RB void  pi_set_reflection_status( void *, logical );
DECL_RB void  pi_get_reflection_status( void *, logical & );
DECL_RB void  pi_set_displacement_status( void *, logical );
DECL_RB void  pi_get_displacement_status( void *, logical & );

//
// Light Shader
//
DECL_RB void*   pi_create_light( const char * name );
DECL_RB void    pi_delete_light( void *lihandle );
DECL_RB void*   pi_copy_light( void *old );

//
// Background Shader
//
DECL_RB void* pi_create_background( const char * name );
DECL_RB void  pi_delete_background( void *lihandle );
DECL_RB void* pi_copy_background( void *old );

//
// Foreground Shader
//
DECL_RB void* pi_create_foreground( const char * name );
DECL_RB void  pi_delete_foreground( void *lihandle );
DECL_RB void* pi_copy_foreground( void *old );

//
//  Texture Space Shader
//
DECL_RB void* pi_create_texture_space( const char * name );
DECL_RB void  pi_delete_texture_space( void *lihandle );
DECL_RB void* pi_copy_texture_space( void *old );

//
// Environment Map Functions
//
DECL_RB void * pi_create_cube_environment
   (
       unsigned              char* data[6],
       int                   channels,
       int                   width[6],
       int                   height[6],
       Image_Access_Function afunc
   );

DECL_RB void * pi_render_cube_environment( int resolution,  double* centre );
DECL_RB void  pi_delete_environment(  void * handle );

// ****************************************************************************
//
// Data extraction routines (extract basic data from advanced shaders)
//
// ****************************************************************************

DECL_RB void pi_extract_br_color
    (
	PdbMaterial *p_mtl
    );

DECL_RB void pi_extract_br_reflectance
    (
	PdbMaterial *p_mtl
    );
	
DECL_RB void pi_extract_br_material
    (
	PdbMaterial *p_mtl,
	int clss
    );

DECL_RB void pi_extract_br_light
    (
	PdbComponent *p_comp,
	PdbLight *p_lt
    );
	
DECL_RB void pi_extract_br_background
    (
	PdbComponent *p_comp,
	PdbBackground *p_bk
    );
	
DECL_RB void pi_extract_br_foreground
    (
	PdbComponent *p_comp,
	PdbForeground *p_fr
    );
	
// ****************************************************************************
//
//  GENERAL SHADER FUNCTIONS
//
//  General functions for accessing shader arguments
//  Note that the values returned by the enquiry function
//  MUST NOT be freed. These may also be transient values
//  which the application MUST COPY if they are not used
//  before another Shader argument enquiry function call
//
//  Unlike the interface functions described above these
//  functions do not map directly onto a single ADS function
//  but instead call an appropriate ADS functions depending
//  on the Shader_Type, for instance, a Shader_Type of
//  DISPLACEMENT_COMP_SHADER will invoke the ADS function
//  'LiDefaultDisplacementComponent ' in the function
//  'pi_set_default_component', but the ADS function
//  'LiDefaultColourComponent' would  be called if the
//  Shader_Type was COLOR_COMP_SHADER
//
// ****************************************************************************

DECL_RB logical pi_set_husk_shader_argument
   (
      void*       handle,
      Shader_Type stype,
      const char* name,
      Render_Arg* arg_value
   );


DECL_RB logical pi_get_husk_shader_arguments
   (
      void*       handle,
      Shader_Type stype,
      const char* &name,
      int         &n_args,
      const char**&arg_names,
      Render_Arg* &arg_values
   );

DECL_RB logical pi_get_shader_name(
	void*         handle,
	Shader_Type   stype,
	const char*&  name
	);
	
DECL_RB void pi_set_default_component
   (
      Shader_Type stype,
      const char * name
   );

DECL_RB int pi_set_component
   (
      void*       handle,
      Shader_Type stype,
      const char * name
   );

//
// get a list of available shader types
//
DECL_RB void pi_get_shader_list
   (
      Shader_Type   stype,
      int         	&ncol,
      const char ** &names
   );

DECL_RB void pi_set_standard_background( void *, float*, float *);
DECL_RB void pi_get_standard_background( void *, char * &, float*, float *);

DECL_RB void pi_set_standard_foreground( void *, float*, float *);
DECL_RB void pi_get_standard_foreground( void *, char * &, float*, float *);

DECL_RB void pi_get_standard_state( void *, float*, float &, float&, float&, float&);
DECL_RB void pi_set_standard_state( void *, float*, float, float, float, float );


DECL_RB void pi_set_standard_light( void *, float, float *, float * );
DECL_RB void pi_get_standard_light( void *, char * &, float&, float *, float * );

//  Function: rh_set_entity_list
//
//  Description:
//     A utility function which sets the internal lightworks prim
//     list to contain a list of primitives derived from the facets
//     attached to the entities in the ENTITY_LIST. Also returns
//     list of generated primitives handles.
//
DECL_RB logical rh_set_entity_list
   (
      ENTITY_LIST const & entities,
      int               & n_prims,
      void           ** & prim_handles,
      logical            set_box = TRUE
   ) ;


//
// Get a SPAtransf for a BODY
//
//   Used primarily by boxing functions
//
DECL_RB logical rh_get_body_transf( BODY * body, SPAtransf & new_tran );


// 
// Convenience funtions for setting and getting material properties
//
//
// get functions
//
//
// rh_get_material_color - 	gets the color value of the current color shader. The material may not
// have any color assigned, in which case, found will return FALSE.
//
DECL_RB logical rh_get_material_color(RH_MATERIAL *mat, double &r, double &g, double &b, logical &found );

//
// Legacy, for those folks who don't care if the color is found.
//
DECL_RB logical rh_get_material_color(RH_MATERIAL *mat, double &r, double &g, double &b );

//
// rh_get_material_reflection - gets the reflectance values of the current 
// reflectance shader. The function returns TRUE if successful, otherwise FALSE.
//
DECL_RB logical rh_get_material_reflection(RH_MATERIAL *mat , 
								   double &ambent,
								   double &diffuse,
								   double &specular,
								   double &exponent);
//
// rh_get_material_transparency - gets the transparency if stored as"color" arg 
// of the current transparency shader. The function returns TRUE if successful, 
// otherwise FALSE.
//
DECL_RB logical rh_get_material_transparency(RH_MATERIAL *mat,double &transp);

//
// rh_get_material_texure name - gets the texure name  if stored as "file name" arg 
// of the current color shader. The function returns TRUE if successful, 
// otherwise FALSE.
//
DECL_RB logical rh_get_material_texture_name(RH_MATERIAL *mat,const char *&name);

//
// set functions
//
// rh_set_material_color - sets the "color" arg of the material in the current color 
//  shader, if that fails the color shader is set to "plain" and the color 
//  value is set. Returns TRUE if set was successful FALSE otherwise.
//
DECL_RB logical rh_set_material_color(RH_MATERIAL *mat, double r, double g, double b);


//
// rh_set_material_reflection - sets the reflection args ("ambient factor", 
//      "diffuse factor", "specular factor", "exponent") in the current 
//      reflection shader for the material.	If that fails the reflection shader
//      is changed to "phong" and then the properties are set.  Returns TRUE if 
//      set was successful FALSE otherwise.
//
DECL_RB logical rh_set_material_reflection(RH_MATERIAL *mat , 
								   double ambent,
								   double diffuse,
								   double specular,
								   double exponent);

//
// rh_set_material_transparency - sets the transparency factor in "color" arg
//  in the current transparecy shader of the material if that fails the 
//  transparency shader is set to "plain" and the "color arg" is set to the
//  transparency value. Returns TRUE if set was	successful FALSE otherwise.
//
DECL_RB logical rh_set_material_transparency(RH_MATERIAL *mat,double transp);

//
// rh_set_material_texture_name - sets the texture file name in "file name" arg
//  in the current color  shader of the material, If that fails the color shader 
//  is set to "wrapped image" and the "file name" value is set to texture name.
//  Returns TRUE if set was successful FALSE otherwise.
//
DECL_RB logical rh_set_material_texture_name(RH_MATERIAL *mat,const char *tex_name);

#endif
