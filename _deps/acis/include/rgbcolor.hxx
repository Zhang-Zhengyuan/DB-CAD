/*******************************************************************/
/*    Copyright (c) 1989-2020 by Spatial Corp.                     */
/*    All rights reserved.                                         */
/*    Protected by U.S. Patents 5,257,205; 5,351,196; 6,369,815;   */
/*                              5,982,378; 6,462,738; 6,941,251    */
/*    Protected by European Patents 0503642; 69220263.3            */
/*    Protected by Hong Kong Patent 1008101A                       */
/*******************************************************************/

// Header for rgb_color.

#ifndef RGB_COLOR_CLASS
#define RGB_COLOR_CLASS
#include "acis.hxx"
#include "safe_math.hxx"
#ifdef mac
#if !defined( macos_a64 )
#include "carbonheader.h"
#endif
#undef debug_string
#endif

#ifdef _MSC_VER
#include <windows.h>
// Disable-
// class 'rgb_color' needs to have dll-interface
// Since rgb_color is all inlined, it doesn't need to be exported.
#pragma warning( disable : 4251 )
#endif
/**
 * \defgroup ACISVISUALIZATION Visualization
 * \ingroup KERNAPI
 *
 */

/**
 * @file rgbcolor.hxx
 * @CAA2Level L1
 * @CAA2Usage U1
 * \addtogroup ACISVISUALIZATION
 *
 * @{
 */

/**
 * Defines the red, green, and blue colors for the display.
 * <br>
 * The red, green, and blue components are double values that may range from 0.0 to 1.0.
 * <br>
 * @see ATTRIB_RGB
 */
class /*DECL_RB*/ rgb_color : public ACIS_OBJECT
{
	// R,G,B values from 0.0 to 1.0
	double Red;
	double Green;
	double Blue;

public:

/**
 * C++ allocation constructor requests memory for this object but does not populate it.
 */
	rgb_color() { Red = Green = Blue = 0.0; }

	// Construct an rgb_color from three doubles.

/**
 * C++ initialize constructor requests memory for this object and populates it with the data supplied as arguments.
 * <br>
 * <b>Role</b>: Creates a rgb_color using the specified three doubles.
 * @param r
 * red color value (0.0 to 1.0)
 * @param g
 * green color value (0.0 to 1.0)
 * @param b
 * blue color value (0.0 to 1.0)
 */
	rgb_color( double r, double g, double b )
	{
		Red   = (r < 0.0) ? 0.0 : ((r > 1.0) ? 1.0 : r);
		Green = (g < 0.0) ? 0.0 : ((g > 1.0) ? 1.0 : g);
		Blue  = (b < 0.0) ? 0.0 : ((b > 1.0) ? 1.0 : b);
	}

	// Construct an rgb_color from an array of three doubles.
/**
 * C++ initialize constructor requests memory for this object and populates it with the data supplied as arguments.
 * <br>
 * <b>Role</b>: Creates a rgb_color using the specified array of three doubles. The first element of the
 * array specified red, the second green, and the third blue. Each component should be in the range of 0.0 to 1.0.
 * @param p
 * array of 3 doubles
 */
	rgb_color( double p[ 3 ] )
	{
		Red   = (p[0] < 0.0) ? 0.0 : ((p[0] > 1.0) ? 1.0 : p[0]);
		Green = (p[1] < 0.0) ? 0.0 : ((p[1] > 1.0) ? 1.0 : p[1]);
		Blue  = (p[2] < 0.0) ? 0.0 : ((p[2] > 1.0) ? 1.0 : p[2]);
	}

	// Construct an rgb_color from an integer.

#if defined( TSL ) || defined (hp700_9)
/**
 * C++ initialize constructor requests memory for this object and populates it with the data supplied as arguments.
 * @param index
 * integer color value
 */
	rgb_color( int index);
#else
/**
 * C++ initialize constructor requests memory for this object and populates it with the data supplied as arguments.
 * @param index
 * integer color value
 */
	rgb_color( int index )
	{
		LOCAL_CONST double rgb[8][3] = {	{0.0, 0.0, 0.0},	// 0 - Black
									{1.0, 0.0, 0.0},	// 1 - Red
									{0.0, 1.0, 0.0},	// 2 - Green
									{0.0, 0.0, 1.0},	// 3 - Blue
									{0.0, 1.0, 1.0},	// 4 - Cyan
									{1.0, 1.0, 0.0},	// 5 - Yellow
									{1.0, 0.0, 1.0},	// 6 - Magenta
									{1.0, 1.0, 1.0}};	// 7 - White
		// Force index into range just to be safe
		int i = ((index < 0) ? -index : index) % 8;

		Red   = rgb[i][0];
		Green = rgb[i][1];
		Blue  = rgb[i][2];
	}
#endif

	// Extract a color value.
/**
 * Returns the red color value.
 */
	double red() const { return Red; }
/**
 * Returns the green color value.
 */
	double green() const { return Green; }
/**
 * Returns the blue color value.
 */
	double blue() const { return Blue; }

#if defined( TSL ) || defined (hp700_9)
/**
 * Extracts a color value.
 */
	operator int();
#else
/**
 * Extracts a color value.
 */
	operator int()
	{
		LOCAL_CONST int index[2][2][2] = {{{0, 3}, {2, 4}}, {{1, 6}, {5, 7}}};

		return index[Red >= 0.5][Green >= 0.5][Blue >= 0.5];
	}
#endif
/**
 * Determines whether a color is equal to another color.
 * @param c
 * rgb color value
 */
	int operator==(const rgb_color &c) const
	{
		return	safe_fabs(c.Red   - Red  ) < 1e-4 &&
				safe_fabs(c.Green - Green) < 1e-4 &&
				safe_fabs(c.Blue  - Blue ) < 1e-4;
	}

/**
 * Determines whether the color is not equal to another color.
 * @param c
 * rgb color value
 */
	int operator!=(const rgb_color &c) const
	{
		return	safe_fabs(c.Red   - Red  ) >= 1e-4 ||
				safe_fabs(c.Green - Green) >= 1e-4 ||
				safe_fabs(c.Blue  - Blue ) >= 1e-4;
	}


 	// Set color values.

/**
 * Sets the red component.
 * @param r
 * red component
 */
 	void set_red( double r )
	{
		Red   = (r < 0.0) ? 0.0 : ((r > 1.0) ? 1.0 : r);
	}
/**
 * Sets the green component.
 * @param g
 * green component
 */
 	void set_green( double g )
	{
		Green = (g < 0.0) ? 0.0 : ((g > 1.0) ? 1.0 : g);
	}
/**
 * Sets the blue component.
 * @param b
 * blue component
 */
 	void set_blue( double b )
	{
		Blue  = (b < 0.0) ? 0.0 : ((b > 1.0) ? 1.0 : b);
	}

	// Output details of an rgb_color.

//	void debug( FILE * = debug_file_ptr ) const;


//===================================================================
// When we are running in the WIN32 environment, then we define these
// additional methods for converting to or from a Windows GDI color

#ifdef _MSC_VER


// When we are running in the WIN32 environment, then we define these
// additional methods for converting to or from a Windows GDI color

#define RB_COLOR_MAP(r)  ((r < 0.0) ? 0 : ((r >= 1.0) ? 255 : (int) (r * 255.0)))

// NOTE:
//  We should really include windows.h to get the definition of COLORREF,
//  but this causes lots of problems because of naming conflicts between
//  ACIS and windows, so we just use unsigned long

/**
 * Convert to a Windows COLORREF.
 * <br>
 * <b>Role</b>: For NT platforms only.
 */
	unsigned long windows_color() const
	{
		return (unsigned long)RGB(RB_COLOR_MAP(Red), RB_COLOR_MAP(Green), RB_COLOR_MAP(Blue));
	}

/**
 * Set the color values from a Windows COLORREF.
 * <br>
 * <b>Role</b>: For NT platforms only.
 * @param from
 * Windows GDI color
 */
	void set_windows_color(unsigned long from)
	{
		Red = (double)GetRValue((COLORREF)from) / 255.0;
		Green = (double)GetGValue((COLORREF)from) / 255.0;
		Blue = (double)GetBValue((COLORREF)from) / 255.0;
	}

#endif

//===================================================================
// When we are running in the Macintosh Environment, then we define this
// additional method for converting to an RGBColor record

#if defined( mac ) && !defined( macos_a64 )
/**
 * When running in the Macintosh Environment, this defines an additional method for converting to an RGBColor record.
 * @param mac_rgb
 * mac color
 */
	RGBColor *mac_color(RGBColor *mac_rgb)
	{
		mac_rgb->red = (int)(Red * 0xFFFF);
		mac_rgb->green = (int)(Green * 0xFFFF);
		mac_rgb->blue = (int)(Blue * 0xFFFF);

		return mac_rgb;
	}

/**
 * When running in the Macintosh Environment, this defines an additional method for converting to an RGBColor record.
 * @param mac_rgb
 * color for mac
 */
	void set_mac_color(RGBColor mac_rgb)
	{
		Red = (double)mac_rgb.red / 0xFFFF;
		Green = (double)mac_rgb.green / 0xFFFF;
		Blue = (double)mac_rgb.blue / 0xFFFF;
	}


#endif

};

/** @} */
#endif
