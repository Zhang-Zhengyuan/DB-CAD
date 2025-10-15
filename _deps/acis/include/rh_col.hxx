/*******************************************************************/
/*    Copyright (c) 1989-2020 by Spatial Corp.                     */
/*    All rights reserved.                                         */
/*    Protected by U.S. Patents 5,257,205; 5,351,196; 6,369,815;   */
/*                              5,982,378; 6,462,738; 6,941,251    */
/*    Protected by European Patents 0503642; 69220263.3            */
/*    Protected by Hong Kong Patent 1008101A                       */
/*******************************************************************/
#if !defined( RENDERER_COL_HEADER )
#define RENDERER_COL_HEADER
//      Contains class and types definitions for the Render_Color Class
#include "dcl_rb.h"
#include <stdio.h>
/**
 * @file rh_col.hxx
 * @CAA2Level L1
 * @CAA2Usage U1
 * \addtogroup RENDERBASEAPI
 *
 * @{
 */

/**
 * Indices for accessing members of RGB color triples
 */
#define  RC_RED   0
/**
 * Indices for accessing members of RGB color triples
 */
#define  RC_GREEN 1
/**
 * Indices for accessing members of RGB color triples
 */
#define  RC_BLUE  2
/**
 * Represents an RGB color.
 * <br>
 * <b>Role:</b> Colors are represented in terms of an RGB triple, where each color
 * component ranges from 0 to 1, with 1 representing the maximum intensity.
 * The array of three values is indexed with 0 corresponding to red, 1 to green
 * and 2 to blue.
 */
class DECL_RB Render_Color
{
   private:
      double rgb[3];
   public:

      //
      // Base Color ( Black )
      //
/**
 * C++ allocation constructor requests memory for this object but does not populate it.
 */
      Render_Color( )
      {
         rgb[ RC_RED ] = rgb[ RC_GREEN ] = rgb[ RC_BLUE ] = 0.0;
      };

      //
      // Construct from RGB values
      //
/**
 * C++ initialize constructor requests memory for this object and populates it with the data supplied as arguments.
 * <br><br>
 * @param r
 * red.
 * @param g
 * green.
 * @param b
 * blue.
 */
      Render_Color( double r, double g, double b )
      {
         rgb[ RC_RED ] = r;
         rgb[ RC_GREEN ] = g;
         rgb[ RC_BLUE ] = b;
      };

      //
      // set red green and blue components
      //
/**
 * Sets the red color value to a double value between 0 and 1.
 * <br><br>
 * @param r
 * red.
 */
      void set_red( double r )   {   rgb[ RC_RED] = r ; };
/**
 * Sets the green color value to a double value between 0 and 1.
 * <br><br>
 * @param g
 * green.
 */
      void set_green( double g ) {  rgb[ RC_GREEN]= g ; };
/**
 * Sets the blue color value to a double value between 0 and 1.
 * <br><br>
 * @param b
 * blue.
 */
      void set_blue( double b )  {  rgb[ RC_BLUE] = b ; };

      //
      // get red green and blue components
      //
/**
 * Returns the normalized red component of the color value.
 */
      double red() const { return( rgb[ RC_RED] ); };
/**
 * Returns the normalized green component of the color value.
 */
      double green() const { return( rgb[ RC_GREEN] ); };
/**
 * Returns the normalized blue component of the color value.
 */
      double blue() const { return( rgb[ RC_BLUE] ); };

      //
      // print out rgb values of color
      //
/**
 * Prints the RGB color values, where each value is separated from the next by the separator character.
 * <br><br>
 * @param sep
 * separator character.
 * @param output
 * output stream.
 */
      void print( const char *sep = " ", FILE *output = stdout )
      {
         fprintf( output, "%g%s%g%s%g",
                  rgb[ RC_RED ], sep, rgb[ RC_GREEN ], sep, rgb[ RC_BLUE ] ) ;
      }
};


/** @} */

#endif
