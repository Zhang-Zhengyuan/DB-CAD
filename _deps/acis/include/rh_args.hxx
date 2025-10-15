/*******************************************************************/
/*    Copyright (c) 1989-2020 by Spatial Corp.                     */
/*    All rights reserved.                                         */
/*    Protected by U.S. Patents 5,257,205; 5,351,196; 6,369,815;   */
/*                              5,982,378; 6,462,738; 6,941,251    */
/*    Protected by European Patents 0503642; 69220263.3            */
/*    Protected by Hong Kong Patent 1008101A                       */
/*******************************************************************/
#if !defined( RENDERER_ARG_HEADER )
#define RENDERER_ARG_HEADER
//      Contains class and types definitions for the Advanced Rendering
//      Husk api functions
//
// Rendering Mode
//
//    A Render_Mode argument is given to the function
//    'api_rh_render_entities', to select how the facets of a
//    geometric entity should be rendered. Each mode trades off
//    computation time for quality. Flat mode is the speediest and
//    renders the facets a constant color. Gouraud and Phong are
//    'well-known' shading modes that 'smooth' out the appearance
//    of facets through interpolation. Full details of these modes
//    can be found in any standard graphics text book. Gouraud mode
//    interpolates color intensity only and is faster than Phong
//    shading in which the surface normals of the vertices of a facet
//    are interpolated across the pixels of its interior.
//
//    Photo-realistic rendering is provided by the Full and Ray Trace Full
//    modes. These modes support effects, such as, transparency and
//    mirrored surfaces, and support a wide range of surface materials.
//    The Preview and Ray Trace Preview modes provide a means of
//    generating images with a slightly reduced range of effects and
//    quality than their full counterparts but at greater speed.
//

#include "dcl_rb.h"
#include "mmgr.hxx"
#include "logical.h"
#include "rh_col.hxx"
/**
 * @file rh_args.hxx
 * @CAA2Level L1
 * @CAA2Usage U1
 * \addtogroup RENDERBASEAPI
 *
 * @{
 */
/**
 * Specifies the mode of rendering.
 * @param RENDER_MODE_FLAT
 * Constant Color across facet.
 * @param RENDER_MODE_LAMBERT
 * Illumination model based on the diffuse reflection of asurface.
 * @param RENDER_MODE_GOURAUD
 * Interpolated intensities across facet.
 * @param RENDER_MODE_PHONG
 * Interpolated normals across facet.
 * @param RENDER_MODE_PREVIEW
 * Anti-aliasing and transparency.
 * @param RENDER_MODE_FULL
 * All Visual attributes.
 * @param RENDER_MODE_RAYTRACE_PREVIEW
 * Same as raytrace full but without anti-aliasing.
 * @param RENDER_MODE_RAYTRACE_FULL
 * Same as full but with improved anti-aliasing on secondary effects.
 **/
 typedef enum
 {
   RENDER_MODE_FLAT,              //   Constant Color across facet
   RENDER_MODE_LAMBERT,           //   Illumination model based on the diffuse reflection of a surface
   RENDER_MODE_GOURAUD,           //   Interpolated intensities across facet
   RENDER_MODE_PHONG,             //   Interpolated normals across facet
   RENDER_MODE_PREVIEW,           //   Anti-aliasing and transparency
   RENDER_MODE_FULL,              //   All Visual attributes
   RENDER_MODE_RAYTRACE_PREVIEW,  //   as below  without anti-aliasing
   RENDER_MODE_RAYTRACE_FULL      //   As for Full but with improved
                                  //   anti-aliasing on secondary effects
 } Render_Mode ;


//
// Rendering Control Variables ( for ray-tracing )
//


/**
 * Rendering control variables for ray-tracing.
 * @param CVAR_OCTTREE_DEPTH
 * octtree depth is the maximum depth of the octtree data structure constructed
 * during ray tracing. An octtree is constructed when rendering using ray tracing,
 * either in a ray tracing mode, or when ray tracing is required by a shader in
 * preview or full rendering modes. The default is 8.
 * @param CVAR_OCTTREE_OCCUPANCY
 * octtree occupancy is the maximum occupancy of a leaf cell of the octtree data
 * structure constructed for ray tracing. The occupancy corresponds to the number
 * of facets contained in an octtree leaf cell. An octtree is constructed when
 * rendering using ray tracing, either in a ray tracing mode, or when ray tracing
 * is required by a shader in the preview or full rendering modes. The default is 8.
 * @param CVAR_RAYTRACE_LEVEL
 * raytrace level is the maximum level of recursion that is enforced when tracing
 * secondary rays. The default is 16.
 * @param CVAR_MIN_PIXEL_CONTRIB
 * min pixel contrib is used for determining whether the contribution made by a
 * secondary ray can be ignored. A combination of this control variable and the
 * level variable define the recursive ray tracing termination criteria. The range
 * is 0.0 to 1.0. The default is 0.05.
 * @param CVAR_PIXEL_THRESHOLD
 * pixel threshold controls anti-aliasing. It determines when to adaptively
 * super-sample the image. The image is sampled until adjacentcolor samples differ
 * in the largest of their red, green and blue components by an amount not
 * exceeding the pixel threshold. Range is 0.0 to 1.0. The default is 0.1.
 **/
typedef enum
{
    CVAR_OCTTREE_DEPTH = 0,		//    octtree depth is the maximum depth of the octtree data structure constructed
								//    during ray tracing. An octtree is constructed when rendering using ray tracing,
								//    either in a ray tracing mode, or when ray tracing is required by a shader in
								//    preview or full rendering modes. The default is 8.

    CVAR_OCTTREE_OCCUPANCY,		//    octtree occupancy is the maximum occupancy of a leaf cell of the octtree data
								//    structure constructed for ray tracing. The occupancy corresponds to the number
								//    of facets contained in an octtree leaf cell. An octtree is constructed when
								//    rendering using ray tracing, either in a ray tracing mode, or when ray tracing
								//    is required by a shader in the preview or full rendering modes. The default is 8.

    CVAR_RAYTRACE_LEVEL,		//    raytrace level is the maximum level of recursion that is enforced when tracing
								//    secondary rays. The default is 16.

    CVAR_MIN_PIXEL_CONTRIB,		//    min pixel contrib is used for determining whether the contribution made by a
								//    secondary ray can be ignored. A combination of this control variable and the
								//    level variable define the recursive ray tracing termination criteria. The range
								//    is 0.0 to 1.0. The default is 0.05.

    CVAR_PIXEL_THRESHOLD		//    pixel threshold controls anti-aliasing. It determines when to adaptively
								//    super-sample the image. The image is sampled until adjacent color samples differ
								//    in the largest of their red, green and blue components by an amount not
								//    exceeding the pixel threshold. Range is 0.0 to 1.0. The default is 0.1.
} Render_Control_Var;


//
// Light Control Values
//
//    Fall_Off_Type
//        selects how the intensity of a light at some SPAposition in space
//        varies with the distance from its source
//
//
/*
// tbrv
*/
/**
 * @nodoc
 */
typedef enum
{
   FALL_OFF_CONSTANT = 0,
   FALL_OFF_INVERSE,
   FALL_OFF_ISL,
   FALL_OFF_INVERSE_CLAMP,
   FALL_OFF_ISL_CLAMP,
   FALL_OFF_INVERSE_SQUARE

} Fall_Off_Type ;

//
//
// Output modes
//
//  The output mode is used to set the format of scanlines output
//  by subsequent rendering calls to application graphics drivers
//
/**
 * Specifies the format of scanlines output by subsequent rendering calls to application graphics drivers.
 * @param OUTPUT_MODE_RGB
 * RGB triples in range (unsigned char ) 0-255.
 * @param OUTPUT_MODE_Z
 * World space values (real) specifying distance from visible
 * surface to eye-point.
 * @param OUTPUT_MODE_RGB_FLOAT
 * RGB triples (real).
 * @param OUTPUT_MODE_RGB_FLOAT_CLAMP
 * RGB triples in range (real) 0 - 1.0.
 **/
 typedef enum
 {
   OUTPUT_MODE_RGB = 0,        // RGB triples in range ( unsigned char ) 0-255
   OUTPUT_MODE_Z,              // World space values (double) specifying
                               // distance from visible surface to eye-point
   OUTPUT_MODE_RGB_FLOAT,      // RGB triples (double)
   OUTPUT_MODE_RGB_FLOAT_CLAMP // RGB triples in range (double) 0 - 1.0

 } Output_Mode ;


//
//
// Environment Map Field Access Values
//
//  The following values are used for accessing the fields
//  of a cubic environment map
//
/**
 * Values used for accessing the fields of a cubic environment map.
 * @param ENVIRONMENT_POS_X
 * the positive X field.
 * @param ENVIRONMENT_NEG_X
 * the negative X field.
 * @param ENVIRONMENT_POS_Y
 * the positive Y field.
 * @param ENVIRONMENT_NEG_Y
 * the negative Y field.
 * @param ENVIRONMENT_POS_Z
 * the positive Z field.
 * @param ENVIRONMENT_NEG_Z
 * the negative Z field.
 */
typedef enum
{
   ENVIRONMENT_POS_X,
   ENVIRONMENT_NEG_X,
   ENVIRONMENT_POS_Y,
   ENVIRONMENT_NEG_Y,
   ENVIRONMENT_POS_Z,
   ENVIRONMENT_NEG_Z

} Environment_Field;

//
// Image Access Function
//
//   An image access function, is an application supplied function
//   which given an x-y SPAposition and pointer to some application
//   data, returns a color value for that positiion.
//   The result is a pointer to an array of char's whose length
//   depends upon the the no. of color channels in use. So that
//   for 3 channels the result would be hold 3 RGB values.
//
//   Image access functions are used fo creating cube environment maps
//


/**
 * Image Access Function.
 * <br>
 * <b>Role:</b> An image access function, is an application supplied function which given an <i>x</i>-<i>y</i>
 * position and pointer to some application data, returns a color value for that position. The
 * result is a pointer to an array of char's whose length depends upon the the no. of color
 * channels in use. So that for 3 channels the result would be hold 3 RGB values. Image access
 * functions are used for creating cube environment maps.
 */
typedef void (*Image_Access_Function)
   (
       unsigned char *result,
       void *data,
       int x,
       int y
   );


//
// Argument types
//
//   The argument type is used to define the type of data held by a
//   Render_Arg
//
/**
 * Defines the type of data held by a <tt>Render_Arg</tt>.
 * @param ARG_UNDEF
 * Undefined argument.
 * @param ARG_INT
 * Integer argument.
 * @param ARG_REAL
 * Real argument.
 * @param ARG_STRING
 * String argument.
 * @param ARG_COLOR
 * Color argument.
 * @param ARG_VECTOR
 * Vector argument.
 * @param ARG_ON_OFF
 * On/off argument.
 * @param ARG_FALL_OFF
 * Light fall off type argument.
 **/
 typedef enum
 {
   ARG_UNDEF  = 0,
   ARG_INT    = -1,
   ARG_REAL  = -2,
   ARG_STRING = -3,
   ARG_COLOR = -4,
   ARG_VECTOR = -5,
   ARG_ON_OFF = -6,
   ARG_FALL_OFF = -7
 } Arg_Type ;

//
// On_Off_Type
//
//  Used primarily to set and get ON_OFF arguments
//
/**
 * Used primarily to set and get <tt>ON_OFF</tt> arguments.
 * @param OFF
 * Integer argument.
 * @param ON
 * Integer argument.
 */
typedef enum
{
   OFF = FALSE,
   ON = TRUE
} On_Off_Type;


//   The following value sets the upper limit on no. of render_args per shader
#define RENDER_ARG_MAX 50

/**
 * Provides the mechanism for getting and setting shader parameters.
 * <br>
 * <b>Role:</b> <tt>Render_Arg</tt> objects are used for setting and getting shader
 * parameters. Shader parameters are identified by an enumerated type, and are
 * accessed by a name in the form of a string. Because the parameter types are
 * not known at compile time it is necessary to be able to set the parameters at
 * run time according to their types. <tt>Render_Arg</tt> types include:
 * <br><dl>
 * <dt><tt>ARG_UNDEF</tt></dt>  <dd>Undefined argument</dd>
 * <dt><tt>ARG_INT</tt></dt>  <dd>Integer argument</dd>
 * <dt><tt>ARG_REAL</tt></dt>  <dd>Real argument</dd>
 * <dt><tt>ARG_STRING</tt></dt>  <dd>String argument</dd>
 * <dt><tt>ARG_COLOR</tt></dt>  <dd>Three real values specifying the RGB color</dd>
 * <dt><tt>ARG_VECTOR</tt></dt>  <dd>Three real values for x, y, and z</dd>
 * <dt><tt>ARG_ON_OFF</tt></dt>  <dd>ON or OFF</dd>
 * <dt><tt>ARG_FALL_OFF</tt></dt>  <dd>One of the values:<br>
 * <tt>&nbsp;&nbsp;FALL_OFF_CONSTANT</tt>,<br>
 * <tt>&nbsp;&nbsp;FALL_OFF_INVERSE</tt>,<br>
 * <tt>&nbsp;&nbsp;FALL_OFF_ISL</tt>,<br>
 * <tt>&nbsp;&nbsp;FALL_OFF_INVERSE_CLAMP</tt>,<br>
 * <tt>&nbsp;&nbsp;FALL_OFF_ISL_CLAMP</tt>.</dd>
 * </dl>
 */
class DECL_RB Render_Arg : public ACIS_OBJECT
{
   private:

      union
      {
         double        float_arg;
         int           int_arg;
         const char *  string_arg;
         double        color_arg[3];
         double        vector_arg[3];
         On_Off_Type   on_off_arg;
         Fall_Off_Type fall_off_arg;
      };

      Arg_Type  arg_type;

   public:
/**
 * C++ allocation constructor requests memory for this object but does not populate it.
 */
      Render_Arg()
      {
         arg_type  = ARG_UNDEF;
		 string_arg   = 0;
		 float_arg = 0.0;
      }

/**
 * C++ initialize constructor requests memory for this object and populates it with the data supplied as arguments, creating a <tt>Render_Arg</tt> of type <tt>ARG_STRING</tt>.
 * <br><br>
 * @param val
 * integer.
 */
      Render_Arg( const int &val )
      {
         arg_type  = ARG_INT;
         int_arg   = val;
      }
/**
 * Returns the <tt>Render_Arg</tt>'s integer value.
 * <br><br>
 * @param val
 * integer value.
 */
      Render_Arg operator=( const int &val ) // Each member of the union has an
      {                                      // overloaded = operator that
         arg_type  = ARG_INT;                // looks like the constructor
         int_arg   = val;
         return *this;
      }

/**
 * C++ initialize constructor requests memory for this object and populates it with the data supplied as arguments, creating a <tt>Render_Arg</tt> of type <tt>ARG_STRING</tt>.
 * <br><br>
 * @param val
 * double.
 */
      Render_Arg( const double &val )
      {
         arg_type  = ARG_REAL;
         float_arg = ( double ) val;
      }
/**
 * Returns the <tt>Render_Arg</tt>'s double value.
 * <br><br>
 * @param val
 * double value.
 */
      Render_Arg operator=( const double &val )
      {
         arg_type  = ARG_REAL;
         float_arg = ( double ) val;
         return *this;
      }

/**
 * C++ initialize constructor requests memory for this object and populates it with the data supplied as arguments, creating a <tt>Render_Arg</tt> of type <tt>ARG_STRING</tt>.
 * <br><br>
 * @param val
 * string.
 */
      Render_Arg( const char *  val )
      {
         arg_type  = ARG_STRING;
         string_arg = val;
      }
/**
 * Returns the <tt>Render_Arg</tt>'s string value.
 * <br><br>
 * @param val
 * string value.
 */
      Render_Arg operator=( const char *val )
      {
         arg_type  = ARG_STRING;
         string_arg = val;
         return *this;
      }

/**
 * C++ initialize constructor requests memory for this object and populates it with the data supplied as arguments, creating a <tt>Render_Arg</tt> of type <tt>ARG_COLOR</tt>.
 * <br><br>
 * <b>Role:</b> This value is composed of three doubles, corresponding to red, green and blue.
 * <br><br>
 * @param val
 * color.
 */
      Render_Arg( const Render_Color &val )
      {
         arg_type  = ARG_COLOR;
         color_arg[ RC_RED ] = val.red();
         color_arg[ RC_GREEN ] = val.green();
         color_arg[ RC_BLUE ] = val.blue();
      }
/**
 * Returns the <tt>Render_Arg</tt>'s <tt>Render_Color</tt> value.
 * <br><br>
 * @param val
 * render color.
 */
      Render_Arg operator=( const Render_Color &val )
      {
         arg_type  = ARG_COLOR;
         color_arg[ RC_RED ] = val.red();
         color_arg[ RC_GREEN ] = val.green();
         color_arg[ RC_BLUE ] = val.blue();
         return *this;
      }

/**
 * C++ initialize constructor requests memory for this object and populates it with the data supplied as arguments, creating a <tt>Render_Arg</tt> of type <tt>ARG_VECTOR</tt>.
 * <br><br>
 * @param val
 * vector.
 */
      Render_Arg( const double *val )
      {
         arg_type  = ARG_VECTOR;
         vector_arg[0] = val[0];
         vector_arg[1] = val[1];
         vector_arg[2] = val[2];
      }
/**
 * Returns the <tt>Render_Arg</tt>'s vector value.
 * <br><br>
 * @param val
 * vector value.
 */
      Render_Arg operator=( const double *val )
      {
         arg_type  = ARG_VECTOR;
         vector_arg[0] = val[0];
         vector_arg[1] = val[1];
         vector_arg[2] = val[2];
         return *this;
      }

/**
 * C++ initialize constructor requests memory for this object and populates it with the data supplied as arguments, creating a <tt>Render_Arg</tt> of type <tt>ARG_ON_OFF</tt>.
 * <br><br>
 * @param val
 * on or off.
 */
      Render_Arg( const On_Off_Type &val )
      {
         arg_type  = ARG_ON_OFF;
         on_off_arg = val;
      }
/**
 * Returns the <tt>Render_Arg</tt>'s <tt>Fall_Off_Type</tt> value.
 * <br><br>
 * @param val
 * on off type.
 */
      Render_Arg operator=( const On_Off_Type &val )
      {
         arg_type  = ARG_ON_OFF;
         on_off_arg = val;
         return *this;
      }

/**
 * C++ initialize constructor requests memory for this object and populates it with the data supplied as arguments, creating a <tt>Render_Arg</tt> of type <tt>ARG_FALL_OFF</tt>.
 * <br><br>
 * @param val
 * value.
 */
      Render_Arg( const Fall_Off_Type &val )
      {
         arg_type  = ARG_FALL_OFF;
         fall_off_arg = val;
      }
/**
 * Returns the <tt>Render_Arg</tt>'s <tt>Fall_Off_Type</tt> value.
 * <br><br>
 * @param val
 * fall off type.
 */
      Render_Arg operator=( const Fall_Off_Type &val )
      {
         arg_type  = ARG_FALL_OFF;
         fall_off_arg = val;
         return *this;
      }

      // We also need an overloaded operator for Render_Arg = Render_Arg
/**
 * Returns the <tt>Render_Arg</tt>'s value.
 * <br><br>
 * @param rarg
 * render argument.
 */
      Render_Arg operator=( const Render_Arg &rarg )
      {
          arg_type = rarg.arg_type;
          switch( rarg.type() ) {
              case ARG_UNDEF:
              break;

              case ARG_INT:
              int_arg   = rarg.int_arg;
              break;

              case ARG_REAL:
              float_arg = ( double ) rarg.float_arg;
              break;

              case ARG_STRING:
              string_arg = rarg.string_arg;
              break;

              case ARG_COLOR:
              color_arg[ RC_RED ] = rarg.color_arg[ RC_RED ];
              color_arg[ RC_GREEN ] = rarg.color_arg[ RC_GREEN ];
              color_arg[ RC_BLUE ] = rarg.color_arg[ RC_BLUE ];
              break;

              case ARG_VECTOR:
              vector_arg[0] = rarg.vector_arg[0];
              vector_arg[1] = rarg.vector_arg[1];
              vector_arg[2] = rarg.vector_arg[2];
              break;

              case ARG_ON_OFF:
              on_off_arg = rarg.on_off_arg;
              break;

              case ARG_FALL_OFF:
              fall_off_arg = rarg.fall_off_arg;
              break;
          }
          return *this;
      }

/**
 * Returns the type of <tt>Render_Arg</tt>.
 * <br><br>
 * <b>Role:</b> If the value is not a <tt>Render_Arg</tt> type value, the result is undefined.
 */
      Arg_Type type() const
      {
          return ( arg_type );
      }
/**
 * Returns the <tt>Render_Arg</tt>'s <tt>Render_Color</tt> value.
 * <br><br>
 * <b>Role:</b> If the value is not a <tt>Render_Color</tt> value, the result is undefined.
 */
      operator Render_Color()  const
      {
          return Render_Color( color_arg[ RC_RED ],
                               color_arg[ RC_GREEN ],
                               color_arg[ RC_BLUE ]);
      };

	  // STI let (7/98): Removed "real" and "real*" casting operators and
	  // replaced with "double" and "double*" operators.

/**
 * Returns the <tt>Render_Arg</tt>'s <tt>Fall_Off_Type</tt> value.
 * <br><br>
 * <b>Role:</b> If the value is not a <tt>Fall_Off_Type</tt> value, the result is undefined.
 */
      operator Fall_Off_Type() const { return fall_off_arg; };
/**
 * Returns the <tt>Render_Arg</tt>'s <tt>On_Off_Type</tt> value.
 * <br><br>
 * <b>Role:</b> If the value is not a <tt>On_Off_Type</tt> value, the result is undefined.
 */
      operator On_Off_Type()   const { return on_off_arg;   };
/**
 * Returns the <tt>Render_Arg</tt>'s integer value.
 * <br><br>
 * <b>Role:</b> If the value is not an integer value, the result is undefined.
 */
      operator int()           const { return int_arg;      };
/**
 * Returns the <tt>Render_Arg</tt>'s string value.
 * <br><br>
 * <b>Role:</b> If the value is not a string value, the result is undefined.
 */
      operator const char*()   const { return string_arg;   };
//    operator real()          const { return float_arg;    };
//    operator real*()         const { return (real *)vector_arg;   };
/**
 * Returns the <tt>Render_Arg</tt>'s real value.
 * <br><br>
 * <b>Role:</b> If the value is not a real value, the result is undefined.
 */
      operator double()        const { return float_arg;    };
/**
 * Returns the Render_Arg's vector value.
 * <br><br>
 * <b>Role:</b> If the value is not a vector value, the result is undefined.
 */
      operator double*()       const { return (double *)vector_arg;   };
	  // STI let: end

/**
 * Supports debugging.
 * <br><br>
 * @param fp
 * debug file.
 */
      void debug( FILE * fp );

      //
      // Support functions for render entity save restore
      //

/**
 * Saves the arg type, followed by the arg value, which can vary by type.
 */
      void save() const;
/**
 * Restores the <tt>Render_Arg</tt>.
 * <br><br>
 * <b>Role:</b> The <tt>restore</tt> function does the actual work. It calls the base class,
 * then reads the selector, if the save file is new enough.
 * <br><pre>
 * read_int                Argument Type
 * switch( arg_type )
 *  case ARG_INT:
 *     read_int            integer value
 *     break
 *  case ARG_REAL:
 *     read_real           real value
 *     break
 *  case ARG_STRING:
 *     rh_restore_string   string to read in
 *     break
 *  case ARG_COLOR:
 *     read_real           First color value
 *     read_real           Second color value
 *     read_real           Third color value
 *     break
 *  case ARG_VECTOR:
 *     read_real           x value
 *     read_real           y value
 *     read_real           z value
 *     break
 *  case ARG_ON_OFF:
 *     read_int            on or off value
 *     break
 *  case ARG_FALL_OFF:
 *     read_int            fall off value
 *     break
 *  case ARG_UNDEF:
 *     break</pre>
 */
      logical  restore();

      //
      // Free the string pointed to by a Render Arg of type
      // ARG_STRING.
      //
      //   NB: Be careful with this function. Be sure that you
      //       are confident that the memory can be safely freed
      //
/**
 * Frees the string memory pointed to by a <tt>Render_Arg</tt> of type <tt>ARG_STRING</tt>.
 * <br><br>
 * <b>Role:</b> Use this method to free the memory when completed, such as when
 * the <tt>Render_Arg</tt> is destroyed and no other objects reference the string.
 */
      void free_string();
};

/** @} */
#endif
