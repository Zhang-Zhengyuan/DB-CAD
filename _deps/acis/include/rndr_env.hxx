/*******************************************************************/
/*    Copyright (c) 1989-2020 by Spatial Corp.                     */
/*    All rights reserved.                                         */
/*    Protected by U.S. Patents 5,257,205; 5,351,196; 6,369,815;   */
/*                              5,982,378; 6,462,738; 6,941,251    */
/*    Protected by European Patents 0503642; 69220263.3            */
/*    Protected by Hong Kong Patent 1008101A                       */
/*******************************************************************/
#ifndef _RNDR_ENV
#define _RNDR_ENV
#include <stdio.h>
#include "dcl_rb.h"
#include "vector.hxx"
#include "position.hxx"
#include "unitvec.hxx"
#include "wndo_dt.hxx"
#include "gen_rendobj.hxx"
/**
 * @file rndr_env.hxx
 * @CAA2Level L1
 * @CAA2Usage U1
 * \addtogroup RENDERBASEAPI
 *
 * @{
 */
/**
* Specifies the type of projection to use when rendering.
*<br>
* @param RNDR_FLAT_PROJECTION
* Flat projection.
* @param RNDR_PERSPECTIVE_PROJECTION
* Perspective projection.
**/
typedef enum {
	RNDR_FLAT_PROJECTION,
	RNDR_PERSPECTIVE_PROJECTION
 }RNDR_PROJECTION_TYPE;
/**
 *.
 */
#define DEFAULT_NEAR_CLIP 1.0
/**
 *.
 */
#define DEFAULT_FAR_CLIP 1000.0
/**
 *.
 */
#define DEFAULT_FIELD_OF_VIEW 90.0
/**
 *.
 */
#define DEFAULT_HORIZONTAL_RESOLUTION	768
/**
 *.
 */
#define DEFAULT_VERTICAL_RESOLUTION	512
/**
 *.
 */
#define SMALL_DISPLACEMENT 0.0001
/**
 *.
 */
#define SMALL_VECTOR 0.0001
/**
 *.
 */
#define BIT_LEFT  0x4
/**
 *.
 */
#define BIT_IN    0x2
/**
 *.
 */
#define BIT_RIGHT 0x1
/**
 * @nodoc
 */
class DECL_RB RNDR_ENVIRONMENT : public ACIS_OBJECT
{
  /* User-requested viewing parameters */
  SPAposition	ur_from;
  SPAposition	ur_to;
  SPAvector	ur_up;

  double	field_of_view;
  int		projection_type;
  SPAposition	from;
  SPAposition	to;
  SPAvector	up;


  SPAunit_vector	Ux,Uy,Uz;	// Unit vectors in orthgonal,
				//  left handed view space.
				// Uz is from eye point toward viewplane.

  double view_distance;		// Distance to view point.

  SPAvector	Vx,Vy,Vz;	// Vectors in the Ux,Uy,Uz
				// directions, with Vx and Vy
				// scaled to account for view plane
				// distance.

  int		perspective;
  int		z_clipping;		// 0 = no z clipping
					// 1 = z clipping on
  double	zmin,zmax;

  int		xy_clipping;	// 0 = no xy clipping
				// 1 = xy clipping
  double	xmin,xmax,ymin,ymax;
  double	view_area ; // area of window in view plane.

  int		view_changed;

  double	pixel_ar;
  double	image_scale;

  //
  int		left,right,top,bottom;
  int		ur_left,ur_right,ur_top,ur_bottom;
  int		hres,vres;
  int		width,height;
  double	half_width,half_height;

  // aspect ratio
  double	v_aspect_ratio ;

//  MESH_MANAGER *mesh_manager;

  void build_view();	// Constructs the internal view vectors
			// from the view spec.

  // Status data for testing a sequence of points
	int xstat,ystat,zstat;
 public:
  RNDR_ENVIRONMENT();
  void set_view(
	SPAposition from, SPAposition to,SPAvector up,
	int projection_type,double fov);
  void get_view(
	SPAposition &from, SPAposition &to,SPAvector &up,
	int &projection_type,double &fov);

  void set_auto_clipping( SPAposition &box_lower, SPAposition &box_higher );
  void set_clipping(double Near,double Far);
  void get_clipping(double &Near,double &Far);
  void set_sub_image(int _left,int _right,int _top,int _bottom);
  void set_resolution(int _hres,int _vres,double _pixel_ar,double _image_scale);
  void get_sub_image(int &_left,int &_right,int &_top,int &_bottom);
  void get_resolution(
	int &_hres,int &_vres,
	double &_pixel_ar,
	double &_image_scale);
//   MESH_MANAGER * get_mesh_manager();
//   void get_mesh_manager(MESH_MANAGER *&);
//   void set_mesh_manager(MESH_MANAGER *);
//
// 	//MEMBER
// 	// Compute the viewspace coordinates of the SPAvector
// 	// from the eyepoint to a given space point.
//   SPAvector transform_to_view_space(const SPAposition &);
// 	//MEMBER
// 	// Return the view plane coordinates and (unmodified)
// 	// z coordinate of a space point.
//   SPAvector transform_to_view_plane(const SPAposition &);
// 	//MEMBER
// 	// Test if a surface differential is visible.
// 	// If the normal direction has a foward component,
// 	// 	VP is the view-space SPAvector to that point.
//   logical oriented_visibility_test(
// 		const SPAposition &,const SPAvector &N,SPAvector &VP);
// 	//END
//
//   void boxtest_start();
//   void boxtest_point(const SPAposition &X);
//   void boxtest_status(int &xstat,int &ystat,int &zstat);
//   logical boxtest_all_out(); // TRUE if boxtest is clearly OUT.
//   void view_window_area(double &) ; // view window area on the view plane
//   void view_plane_window_dimensions( double &ulen, double &vlen ) ;
//   void view_window_to_screen( double u, double v, double &px, double &py ) ;
//
};

// A function to return a static copy of the above;
/**
 * @nodoc
 */
DECL_RB RNDR_ENVIRONMENT* rndr_modal_environment();
/** @} */

//MMMMMMMMMMMMMMMMMMMMMMMMMMMMMMMMMMMMMMMMMMMMMMMMMMMMMMMMMMMMMMMMMMMMMMMMMMMMMMMMMMMMMMMMMMMMMMMMMMMMMMM
//MMMMMMMMMMMMMMMMMMMMMMMMMMMMMMMMMMMMMMMMMMMMMMMMMMMMMMMMMMMMMMMMMMMMMMMMMMMMMMMMMMMMMMMMMMMMMMMMMMMMMMM

//#ifdef OS_Linux
////#include <X11/X.h>
//typedef Widget window_handle;
////window_handle Guyz_Window_Widget;
//#endif

class DECL_RB view_base : public ACIS_OBJECT
{
public:
	virtual ~view_base() {};
	virtual	void				clear() = 0;
	virtual	RenderingObject*	CreateRenderingObject() = 0;
	virtual	RenderingObject*	CreateRenderingObject( RenderingObject * ) = 0;
	virtual	void				destroy() = 0;
	virtual	int					draw_point_3d( const SPAposition & ) = 0;
	virtual	logical				draw_polyline_3d( int, const float[], logical ) = 0;
	virtual	logical				draw_polyline_3d( int, const SPAposition[], logical ) = 0;
	virtual	int					draw_text_3d( const SPAposition &, const char * ) = 0;
	virtual	logical				drawable_to_world( int, int, SPAposition & ) = 0;
	virtual	void				flush(int flags = 0) = 0;
	virtual	rgb_color			get_background_rgb() = 0;
	virtual	logical				get_DisplayFacets() = 0;
	virtual	logical				get_DisplayParamLines() = 0;
	virtual	logical				get_DisplaySurfacePolys() = 0;
	virtual	SPAposition			get_eye() = 0;
	virtual	rgb_color			get_foreground_rgb() = 0;
	virtual	double				get_height() = 0;
	virtual	double				get_hither() = 0;
	virtual	unsigned char * 	get_image() = 0;
	virtual	line_style			get_line_style() = 0;
	virtual	int					get_line_width() = 0;
	virtual	SPAunit_vector		get_out() = 0;
	virtual	logical				get_perspective() = 0;
	virtual	int					get_point_size() = 0;
	virtual	point_style			get_point_style() = 0;
	virtual	SPAunit_vector		get_right() = 0;
	virtual	SPAposition			get_target() = 0;
	virtual	SPAunit_vector		get_up() = 0;
	virtual	void				get_viewport( int &, int &, int &, int & ) = 0;
	virtual	double				get_width() = 0;
	virtual	void *				get_window_handle() = 0;
	virtual	double				get_yon() = 0;
	virtual	logical				is_shaded() = 0;
	virtual	logical				is_valid() = 0;
	virtual	void				make_current() = 0;
	virtual	void				name_window( const char * ) = 0;
	virtual	void				refresh( logical ) = 0;
	virtual	void				RemoveRenderingObject( RenderingObject * ) = 0;
	virtual	void				set_background_rgb( const rgb_color & ) = 0;
	virtual	void				set_DisplayFacets( logical ) = 0;
	virtual	void				set_DisplayParamLines( logical ) = 0;
	virtual	void				set_DisplaySurfacePolys( logical ) = 0;
	virtual	void				set_drawing_mode( drawing_mode ) = 0;
	virtual	void				set_eye( SPAposition const & ) = 0;
	virtual	void				set_foreground_rgb( const rgb_color & ) = 0;
	virtual	void				set_hither( double ) = 0;
	virtual	void				set_line_style( line_style ) = 0;
	virtual	void				set_line_width( int ) = 0;
	virtual	void				set_perspective( logical ) = 0;
	virtual	void				set_point_size( int ) = 0;
	virtual	void				set_point_style( point_style ) = 0;
	virtual	void				set_rubberband_mode( logical ) = 0;
	virtual	void				set_size( double, double ) = 0;
	virtual	void				set_target( SPAposition const & ) = 0;
	virtual	logical				set_text_font( const char * ) = 0;
	virtual	void				set_text_size( int ) = 0;
	virtual	void				set_up( SPAvector const & ) = 0;
	virtual	void				set_viewport( int, int, int, int ) = 0;
	virtual	void				set_yon( double ) = 0;
	virtual	logical				world_to_drawable( const SPAposition &, float &, float & ) = 0;
	virtual const char *		get_engine_full_name() = 0;
	virtual const char *		get_engine_abbrev() = 0;
	virtual int					get_engine_version_major() = 0;
	virtual int					get_engine_version_minor() = 0;
	virtual int					get_engine_version_point() = 0;	
};

#endif
