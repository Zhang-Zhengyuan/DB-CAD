/*******************************************************************/
/*    Copyright (c) 1989-2020 by Spatial Corp.                     */
/*    All rights reserved.                                         */
/*    Protected by U.S. Patents 5,257,205; 5,351,196; 6,369,815;   */
/*                              5,982,378; 6,462,738; 6,941,251    */
/*    Protected by European Patents 0503642; 69220263.3            */
/*    Protected by Hong Kong Patent 1008101A                       */
/*******************************************************************/
//----------------------------------------------------------------------------

// A RenderingObject may be added by reference to a SchemeView, causing the 
// inserted data to be displayed in that view.  When removed from the view,
// the displayed items will be erased.  RenderingObject is reference counted
// to facilitate using in multiple views.

#ifndef PMHUSK_GEN_OBJECT_HXX
#define PMHUSK_GEN_OBJECT_HXX

/**
 * @file gen_rendobj.hxx
 * @CAA2Level L1
 * @CAA2Usage U1
 * \addtogroup ACISVISUALIZATION
 *
 * @{
 */
#include "draw_dt.hxx"
#include "rgbcolor.hxx"
#include "param.hxx"
#include "base.hxx"
class CURVE;
class ENTITY;
class SPAtransf;
class SPAposition;
class SPAvector;

/**
 * Arrow head type
 * @param ro_double_outside
 * -1 (double arrows  -->->)
 * @param ro_simple
 * 0 (a simple arrow ---->)
 * @param ro_double_inside
 * 1 (double arrows  --<->)
 * @param ro_headless
 * 2 (obviously this -----)
 * @param ro_conic
 * 3 ("tee-pee" with 16 edges)
 */
enum ro_arrow_head_type
{
	ro_double_outside = -1,
	ro_simple         =  0,
	ro_double_inside  =  1,
	ro_headless       =  2,
	ro_conic          =  3 
};

/**
 * The drawing method in takes these values and divides them by 5 
 * to get a scaling factor for the arrow head
 * @param ro_tiny
 * 1 
 * @param ro_little
 * 3
 * @param ro_average
 * 5
 * @param ro_large
 * 10
 * @param ro_enormous
 * 25
 */
enum ro_arrow_size_type
{
	ro_tiny       =   1,
	ro_little     =   3,
	ro_average    =   5,
	ro_large      =  10,
	ro_enormous   =  25
};

/**
 * @nodoc
 * The text alignment type represents how the text will be aligned w.r.t 
 * the position of placement.
 */

enum text_alignment_type
{
	center_right, // -TEXT
	center_left	, // TEXT-
	center_center,// TE-XT
	top_left,	  // TEXT_ 
	top_right	, // _TEXT
	top_center  , // TE_XT
	bottom_left , //TEXT`
	bottom_right, //`TEXT
	bottom_center //TE`XT
};

/**
 * @nodoc
 * The text alignment array can be used to convert the enum values to the appropriate arguments for the HOOPS api.
 */

static const char* text_alignment_array[9] = { "*<", "*>", "**", "v>" , "v<" , "v*", "^>", "^<" , "^*"};

/** 
 * The RenderingObject object.
 * <br>
 * The RenderingObject can be viewed as an object which serves the specific viewing 
 * needs of the application - a way to insert entities, insert lines, insert text, etc.  
 * In the Scheme application, a RenderingObject may be added by reference to a SchemeView, 
 * causing the inserted data to be displayed in that view.  When removed from the view, 
 * the displayed items will be erased.  RenderingObject is reference counted to facilitate 
 * using in multiple views.
 * <br><br>
 * As this class is viewed as a way to acheive the described goal, how it is implemented
 * is up to the application. Hence, we leave the details of this class undocumented.
 * <br><br>
 * This object is used by the PART_CONTEXT class.
 * @see PART_CONTEXT
 */
class RenderingObject: public ACIS_OBJECT
{
protected:		
public:	
	/**
	 * Increase use count.
	 */
	virtual long AddRef() = 0;
	/**
	 * Decrease use count. Deletes when use count is zero
	 */
	virtual long Release() = 0;
	/**
	 * Returns class identifier.
	 */
	virtual const char* type_name() = 0;
	/**
	 * Sets mapping flag, only sensible for HOOPs render object
	 */
	virtual void SetMappingFlag(logical m) = 0;
	/**
	 * Get Mapping flag, only sensible for HOOPs version .1
	 */
	virtual logical GetMappingFlag() = 0;

	/**
	 * Computes drawing segments for a 3D polyline from array of floats
     * @param count
     * Number of points.
     * @param points
     * Array of 3D positions (3 floats times count).
     * @param fill
     * Whether or not to fill the polygon.
     * @param color
     * Color
	 */
	virtual intptr_t insert_polyline_3d(int count, const float points[], logical fill, const rgb_color &color) = 0; // 3 floats for each point
	/**
	 * Computes drawing segments for a 3D polyline from array of SPApositions
     * @param count
     * Number of points.
     * @param points
     * Array of 3D positions
     * @param fill
     * Whether or not to fill the polygon.
     * @param color
     * Color
	 */
	virtual intptr_t insert_polyline_3d(int count, const SPAposition points[], logical fill, const rgb_color &color) = 0;
	/**
	 * Computes drawing segments for a cylinder from array of floatss
     * @param count
     * Number of positions.
     * @param points
     * Array of 3D positions (3 floats times count).
	 * @param radius
	 * Radius of cylinder
     * @param capping
     * "none" (default), "first", "second", or "both"
     * @param color
     * Color
	 */
	virtual intptr_t insert_cylinder(int count, const SPAposition points[], double radius, const char * capping = "none", const rgb_color &color =rgb_color( 0.5, 0.5, 0.5)) = 0;
	/**
	 * Computes drawing segments for a point at given position
     * @param pos
     * The position in 3D
     * @param color
     * Color
	 */
	virtual intptr_t insert_point(const SPAposition& pos, const rgb_color &color) = 0;
	/**
	 * Computes drawing segments for text string at given position
     * @param pos
     * The position in 3D
     * @param text
     * The text
     * @param color
     * Color
	 */
	virtual intptr_t insert_text(const SPAposition& pos, const char* text, const rgb_color &color) = 0;
	/**
	 * @nodoc
	 * Computes drawing segments for text string at given position
     * @param pos
     * The position in 3D
     * @param text
     * The text
     * @param color
     * Color
	 * @param text_alignment
	 * The relative positioning of pos w.r.t to text
	 */
	virtual intptr_t insert_text(const SPAposition& pos, const char* text, const rgb_color &color, text_alignment_type text_alignment);

	/**
	 * Computes drawing segments for  an ACIS ENTITY (uses defining curve or surface)
     * @param entity
     * The ACIS ENTITY to be drawn
     * @param st
     * Transformation to apply
     * @param color
     * Color
	 */
	virtual intptr_t insert_entity(ENTITY* entity, const SPAtransf *st, const rgb_color &color) = 0;
	/**
	 * Computes drawing segments for  an ACIS CURVE (uses defining curve)
     * @param curve
     * The ACIS CURVE to be drawn
     * @param start
     * Start curve parameter
     * @param end
     * End curve parameter
     * @param trans
     * Transformation to apply
     * @param color
     * Color
	 */
	virtual intptr_t insert_curve(CURVE* curve, SPAparameter start, SPAparameter end, const SPAtransf& trans, const rgb_color &color) = 0;

	/**
	 * Computes drawing segments for  3D polyline from array of floats
     * @param count
     * Number of points.
     * @param points
     * Array of 3D positions (3 floats times count).
     * @param fill
     * Whether or not to fill the polygon.
	 */
	virtual intptr_t insert_polyline_3d(int count, const float points[], logical fill) = 0; // 3 floats for each point
	/**
	 * Inserts drawing item that defines a 3D polyline from array of SPApositions
     * @param count
     * Number of points.
     * @param points
     * Array of 3D positions
     * @param fill
     * Whether or not to fill the polygon.
	 */
	virtual intptr_t insert_polyline_3d(int count, const SPAposition points[], logical fill) = 0;
	/**
	 * Inserts drawing item that defines a point at given position
     * @param pos
     * The position in 3D
	 */
	virtual intptr_t insert_point(const SPAposition& pos) = 0;
	/**
	 * Inserts drawing item that defines an ACIS ENTITY (uses defining curve or surface)
     * @param entity
     * The ACIS ENTITY to be drawn
     * @param st
     * Transformation to apply
	 */
	virtual intptr_t insert_text(const SPAposition& pos, const char* text) = 0;
	/**
	 * Inserts drawing item that defines an ACIS ENTITY (uses defining curve or surface)
     * @param entity
     * The ACIS ENTITY to be drawn
     * @param st
     * Transformation to apply
	 */
	virtual intptr_t insert_entity(ENTITY* entity, const SPAtransf *st) = 0;
	/**
	 * Inserts drawing item that defines an ACIS CURVE (uses defining curve)
     * @param curve
     * The ACIS CURVE to be drawn
     * @param start
     * Start curve parameter
     * @param end
     * End curve parameter
     * @param trans
     * Transformation to apply
	 */
	virtual intptr_t insert_curve(CURVE* curve, SPAparameter start, SPAparameter end, const SPAtransf& trans) = 0;

	/**
	 * Inserts drawing item that defines an arrow
     * @param arrow_base
     * 3D position where arrow starts
     * @param arrow_direction
     * Vector defining arrow direction
     * @param arrow_length
     * Size of arrow
     * @param arrow_type_o_head
     * Type of head, default to conic.
     * @param arrow_size_o_head
     * Size of arry head, default to average
	 */
	virtual intptr_t insert_arrow(SPAposition  const &arrow_base,  
								  SPAvector const &arrow_direction, 
								  double  const  arrow_length = -1,
								  ro_arrow_head_type const  arrow_type_o_head = ro_conic,  
								  ro_arrow_size_type const  arrow_size_o_head = ro_average) = 0;

	/**
	 * Remove the previously inserted item or all items
     * @param key
     * Drawing item key
	 */
	virtual void remove_item(intptr_t key) = 0;
	/**
	 * Remove all drawing items.
	 */
	virtual void remove_all_items() = 0;
	/**
	 * Remove this RO from the view
	 */
	virtual void remove() = 0;
	/**
	 * Rubberband mode
     * @param mode
     * Mode, if false uses normal key.
	 */
	virtual void set_rubberband_mode(logical mode) = 0;
	/**
	 * Transform segments
     * @param trans
     * transformation
	 */
	virtual void set_Transform(const SPAtransf& trans)=0;
	/**
	 * Get transformation
	 */
	virtual SPAtransf get_Transform() const =0;
	/**
	 * Set rgb color
	 */
	virtual void		set_color(rgb_color const& color) = 0;
	/**
	 * Get Color
	 */
	virtual rgb_color	get_color() const = 0;

	/**
	 * Set line style
	 */
	virtual void		set_line_style( line_style style ) = 0;
	/**
	 * Get line style
	 */
	virtual line_style	get_line_style() const = 0;
	/**
	 * Get line width
	 */
	virtual void		set_line_width( float width ) = 0;
	/**
	 * Set line width
	 */
	virtual float		get_line_width() const = 0;
	/**
	 * Set point style
	 */
	virtual void		set_point_style( point_style style ) = 0;
	/**
	 * Get line style
	 */
	virtual point_style get_point_style() const = 0;
	/**
	 * Set point size
	 */
	virtual void		set_point_size( int size ) = 0;
	/**
	 * Get point size
	 */
	virtual int			get_point_size() const = 0;
	/**
	 * Get font type. Returns FALSE if font not found
	 */
    virtual logical		set_text_font(const char* font_name) = 0;
	/**
	 * Set font length. 
	 */
	virtual size_t		get_text_font_length() const = 0;
	/**
	 * Set font name. Caller must provide "big enough" buffer
	 */
	virtual void		get_text_font(char* font_name) const = 0;
	/**
	 * Set text size in points (default is 10 pts)
	 */
	virtual void		set_text_size(int size) = 0;
	/**
	 * Get text size in points
	 */
	virtual int			get_text_size() const = 0;


	/**
	*@nodoc
	**/
	template<typename vertexCoordinateType>
	struct shaded_triangle_set
	{
		rgb_color color;
		rgb_color edge_color;
		double alpha;
		const char* renderOptsString;
		int ntriangles;
		// triangles should point to an array of integers of size
		// 3*ntriangles.  Each triangle is represented by a block of 
		// 3 elements in the array.
		int* triangles;

		int npoints;
		// points should point to an array of floats of size 3*points.
		// each point is represented by a block of 3 consecutive floats.
		vertexCoordinateType* points;

		// normals should point to NULL or an array of 3*points doubles,
		// with each normal represented by 3 consecutive doubles.
		// the normal vectors should be normalized.
		double* normals;

		shaded_triangle_set(): alpha(0), ntriangles(0), triangles(0), npoints(0), points(0), normals(0),renderOptsString(NULL){}
	};

	/**
	*@nodoc
	**/
	virtual void		insert_shaded_triangle_set( shaded_triangle_set<float> const& triangles) =0;
	virtual void		insert_shaded_triangle_set( shaded_triangle_set<double> const& triangles) =0;
};

/**
 * Create a rendering object
 */
RenderingObject* CreateRenderingObject(logical allviews);
/**
 * Create copy of rendering object
 */
RenderingObject* CreateRenderingObject(RenderingObject *parent_ro, logical allviews);

/** @} */
#endif /* PMHUSK_GEN_OBJECT_HXX */
