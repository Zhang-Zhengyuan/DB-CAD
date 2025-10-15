/*******************************************************************/
/*    Copyright (c) 1989-2020 by Spatial Corp.                     */
/*    All rights reserved.                                         */
/*    Protected by U.S. Patents 5,257,205; 5,351,196; 6,369,815;   */
/*                              5,982,378; 6,462,738; 6,941,251    */
/*    Protected by European Patents 0503642; 69220263.3            */
/*    Protected by Hong Kong Patent 1008101A                       */
/*******************************************************************/
#ifndef DM_DRAW_ENGINE_H
#define DM_DRAW_ENGINE_H
//
// Header for the DM_draw_engine class.
//
// Purpose: The DM_draw_engine class provides provides an abstract
// draw primitive interface between the DM icons and the view-controller.
//
// Description: The DM_draw_engine interface classes provide abstract
// draw primitive methods to icons: Draw_point and Draw_polyline.
// This decouples the icon library from rendering-context specifics,
// for example, OpenGL or DirectX commands.
//
// Related classes: The draw engine uses the data encapsulated in a
// DM_draw_args object for drawing. The DM_draw_args object is passed through
// from the original view manager draw request. Icon's only obtain
// concrete DM_draw_engine objects from the (global) DM_draw_engine_mgr.
//
#include "dcl_dmicon.h"
#include "mmgr.hxx"
#include "dsstatic.hxx"    // LOCAL_PROC    // LOCAL_PROC

/**
* @file dm_draw_engine.hxx
 * @CAA2Level L1
 * @CAA2Usage U2
 * \addtogroup DMICONAPI
 *
 * @{
 */
//
class DM_def_icon_draw_args;
class DM_draw_engine;
class DS_dmod;
class Spatial_abs_hurler;
class ADM_draw_engine;

/**
 * This class provides an abstract draw primitive interface between the deformable modeling icons and the view-controller.
 * <br>
 * <b>Role:</b> The <tt>DM_draw_engine</tt> interface classes provide abstract draw primitive methods
 * to icons <tt>Draw_point</tt> and <tt>Draw_polyline</tt>. This decouples the icon library from rendering-context
 * specifics, for example, <tt>OpenGL</tt> or <tt>DirectX</tt> commands.
 * <br><br> <b>Related classes:</b> The draw engine uses the data encapsulated in a <tt>DM_draw_args</tt> object for
 * drawing. The <tt>DM_draw_args</tt> object is passed through from the original view manager draw request.
 * Icons only obtain concrete <tt>DM_draw_engine</tt> objects from the (global) <tt>DM_draw_engine_mgr</tt>.
 *<br><br>
 *@see DM_draw_args, DM_draw_engine_mgr
 */

class DECL_DM_ICON DM_draw_engine : public ACIS_OBJECT {
  private:
// Should not call these
    DM_draw_engine&
    operator=                               // Assignment Operator
    (const DM_draw_engine&);

    DM_draw_engine(const DM_draw_engine&) ; // Copy constructor

  protected:
// Should not call this - use the Release_this method.
	virtual ~DM_draw_engine(){}
/**
 *Default constructor.
 */
    DM_draw_engine(){}
  public:

/**
 * Casting operator; reserved for <tt>DM_default_icons</tt>.
 */
   virtual ADM_draw_engine* Spatial_cast() {return 0;} // casting operator; reserved for DM_default_icons

/**
 * Casting operator; used for overriding.
 */
  virtual void* Customer_cast() { return 0;}          // casting operator; used for overriding
/**
 * Lose method for self-destruction.
 */
  virtual void Lose()=0;                              // public destructor
/**
 * Notifies the draw engine of changes which may affect cached data.
 *<br><br>
 * @param err_handler
 * error handler.
 */
   virtual void Mark_dirty(Spatial_abs_hurler& err_handler)=0;     // Notify the draw engine of changes which may affect cached data.

 // Drawing methods
/**
 * Adds a marker.
 *<br><br>
 * @param err_handler
 * error handler.
 * @param own_id1
 * icon owner ID.
 * @param tag
 * icon owner ID.
 * @param marker
 * marker location.
 * @param color1
 * color (r,g,b).
 * @param color2
 * color (r,g,b).
 * @param color3
 * color (r,g,b).
 * @param width
 * line width.
 * @param draw_arg
 * draw args.
 */

  	virtual void Draw_marker(
                             Spatial_abs_hurler& err_handler,                 // error handler
                             DS_dmod* own_id1, int tag,                   // icon owner
                             const double* marker ,                      // marker location (xyz)
                             double color1, double color2, double color3,              // color, R, G, B, between 0 & 1
                             double width,                              // line width
                             const DM_def_icon_draw_args& draw_arg )const =0;  // draw args



/**
 * Adds a polyline. The color values must be between <tt>0</tt> and <tt>1</tt>.
 *<br><br>
 * @param err_handler
 * error handler.
 * @param own_id1
 * icon owner ID.
 * @param tag
 * icon owner ID.
 * @param npoints
 * number of points.
 * @param arr_points
 * array of points.
 * @param color1
 * color (r,g,b).
 * @param color2
 * color (r,g,b).
 * @param color3
 * color (r,g,b).
 * @param width
 * line width.
 * @param draw_arg
 * draw args.
 */

	  virtual void Draw_polyline(
                                 Spatial_abs_hurler& err_handler,              // error handler
                                 DS_dmod* own_id1, int tag,                // icon owner
                                 int npoints ,                             // number of points
                                 const double* arr_points,                    // array of points
                                 double color1, double color2, double color3,           // color, R, G, B, between 0 & 1
                                 double width,                           // line width
                                 const DM_def_icon_draw_args&  draw_arg)const=0;// draw args


}	;



 /** @} */
#endif /*DM_DRAW_ENGINE_H*/
