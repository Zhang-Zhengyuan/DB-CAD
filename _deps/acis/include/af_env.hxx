/* ORIGINAL: acis2.1/faceter/meshing/af_env.hxx */
// $Id: af_env.hxx,v 1.10 2002/08/09 17:21:51 jeff Exp $
/*******************************************************************/
/*    Copyright (c) 1989-2020 by Spatial Corp.                     */
/*    All rights reserved.                                         */
/*    Protected by U.S. Patents 5,257,205; 5,351,196; 6,369,815;   */
/*                              5,982,378; 6,462,738; 6,941,251    */
/*    Protected by European Patents 0503642; 69220263.3            */
/*    Protected by Hong Kong Patent 1008101A                       */
/*******************************************************************/
#ifndef _AF_ENV
#define _AF_ENV


#include "dcl_fct.h"

#include "vector.hxx"
#include "position.hxx"
#include "unitvec.hxx"

#include "meshmg.hxx"
typedef enum {
	AF_FLAT_PROJECTION,
	AF_PERSPECTIVE_PROJECTION
 }AF_PROJECTION_TYPE;

#define DEFAULT_NEAR_CLIP 1.0
#define DEFAULT_FAR_CLIP 1000.0
#define DEFAULT_FIELD_OF_VIEW 90.0
#define DEFAULT_HORIZONTAL_RESOLUTION	768
#define DEFAULT_VERTICAL_RESOLUTION	512
#define SMALL_DISPLACEMENT 0.0001
#define SMALL_VECTOR 0.0001

#define BIT_LEFT  0x4
#define BIT_IN    0x2
#define BIT_RIGHT 0x1

class DECL_FCT AF_ENVIRONMENT : public ACIS_OBJECT {
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

  MESH_MANAGER *mesh_manager;

  void build_view();	// Constructs the internal view vectors
			// from the view spec.

  // Status data for testing a sequence of points
	int xstat,ystat,zstat;
 public:
  AF_ENVIRONMENT();
  void set_view(
	const SPAposition & from, const SPAposition & to,const SPAvector & up,
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
  MESH_MANAGER * get_mesh_manager();
  void get_mesh_manager(MESH_MANAGER *&);
  void set_mesh_manager(MESH_MANAGER *);

	//MEMBER
	// Compute the viewspace coordinates of the SPAvector
	// from the eyepoint to a given space point.
  SPAvector transform_to_view_space(const SPAposition &);
	//MEMBER
	// Return the view plane coordinates and (unmodified)
	// z coordinate of a space point.
  SPAvector transform_to_view_plane(const SPAposition &);
	//MEMBER
	// Test if a surface differential is visible.
	// If the normal direction has a foward component,
	// 	VP is the view-space SPAvector to that point.
  logical oriented_visibility_test(
		const SPAposition &,const SPAvector &N,SPAvector &VP);
	//END

  void boxtest_start();
  void boxtest_point(const SPAposition &iX);
  void boxtest_status(int &xstat,int &ystat,int &zstat);
  logical boxtest_all_out(); // TRUE if boxtest is clearly OUT.
  void view_window_area(double &) ; // view window area on the view plane
  void view_plane_window_dimensions( double &ulen, double &vlen ) ;
  void view_window_to_screen( double u, double v, double &px, double &py ) ;
 
};



#endif
