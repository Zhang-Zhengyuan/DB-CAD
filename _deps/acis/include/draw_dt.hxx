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
//    Graphics system dependent data types
//
//----------------------------------------------------------------------

#ifndef draw_dt_hxx
#define draw_dt_hxx

/**
* @file draw_dt.hxx
 * @CAA2Level L1
 * @CAA2Usage U1
 * \addtogroup ACISVISUALIZATION
 *
 * @{
 */

//======================================================================

// Currently defined colors are:
//	0 - Black
//	1 - Red
//	2 - Green
//	3 - Blue
//	4 - Cyan
//	5 - Yellow
//	6 - Magenta
//	7 - White
// For internal use only
/**
 * @nodoc
 */
typedef unsigned int color_value;

// There should actually be 16 drawing modes, but these are the only
// two that we are interested in for now.
// For internal use only
/**
 * @nodoc
 */
typedef enum {
    SPA_DRAW_MODE,          // set pixels to the foreground color
    SPA_XOR_MODE
} drawing_mode;

// For internal use only
/**
 * @nodoc
 */
typedef enum {
    SPA_SOLID_LINE=0,
    SPA_DASHED_LINE,
    SPA_DOTTED_LINE,
	SPA_NO_LINE
} line_style;

// we probably do not need this for release 1
// For internal use only
/**
 * @nodoc
 */
typedef enum 
{
    SPA_X_POINT,
    SPA_CROSS_POINT,
    SPA_SQUARE_POINT,
	SPA_DOT_POINT,
	SPA_CIRCLE_POINT,
	SPA_SPLAT_POINT,
	SPA_SOLID_CIRCLE
} point_style;
// For internal use only
/**
 * @nodoc
 */
typedef enum 
{
	SPA_NO_SHADING=0,			// Lighting OFF
	SPA_FLAT_SHADING,			// Lighting ON, poly normals
	SPA_SMOOTH_SHADING			// Lighting ON, averaged normals
} shade_model;
// For internal use only
/**
 * @nodoc
 */
typedef enum 
{
	SPA_CULL_NONE=0,		// Disable Culling
	SPA_FRONT,			// Front-facing polygons culled
	SPA_BACK,			// Back-facing polygons culled
	SPA_FRONT_AND_BACK	// Front and back-facing polygons culled
}	cull_type;

//======================================================================
/** @} */
#endif

