/*******************************************************************/
/*    Copyright (c) 1989-2020 by Spatial Corp.                     */
/*    All rights reserved.                                         */
/*    Protected by U.S. Patents 5,257,205; 5,351,196; 6,369,815;   */
/*                              5,982,378; 6,462,738; 6,941,251    */
/*    Protected by European Patents 0503642; 69220263.3            */
/*    Protected by Hong Kong Patent 1008101A                       */
/*******************************************************************/
#ifndef ADMHOOPS_DRAW_ENGINE_H
#define ADMHOOPS_DRAW_ENGINE_H
// bmt 16-Apr-03 Make float_array an ADMHOOPS_draw_engine data member
//               instead of static object in Draw_polyline method.
//               Fixes hp_700_11 crashes. Add limit_memory method to
//               float_array to avoid hogging memory until program exit.
//
// Header for the ADMHOOPS_draw_engine class.
//
// Purpose: The ADMHOOPS_draw_engine class provides provides a concrete
// ADM_draw_engine for drawing ADM_default_icons with the ACIS GI kernel.
//
// Description: The ADMHOOPS_draw_engine encapsualtes the drawing methods of
// the ACIS GI kernel deformable_figure class, wrapping them in the
// ADM_draw_engine abstract interface, thus providing GI kernel drawing methods
// to the DM_default_icon's.
//
// Related classes: The ADMHOOPS_icon_draw_args provide draw data which
// passes through the DM_icon::Draw method to the ADMHOOPS_draw_engine.
// A DM_default_icon requests a concrete DM_draw_engine object from the
// singleton DM_draw_engine_mgr. The initializer for the admhoops_control library
// must load the DM_draw_engine_mgr with an ADMHOOPS_draw_engine; classes overriding
// the ADM_draw_engine must do the same.
//
#include "adm_draw_engine.hxx"                // base class ADM_draw_engine                // base class ADM_draw_engine
#include "dcl_admhoops.h"          // DECL_ macros          // DECL_ macros

/**
* @file admhoops_draw_engine.hxx
 * @CAA2Level L1
 * @CAA2Usage U1
 * \addtogroup ADMHOOPS
 *
 * @{
 */

class Spatial_abs_hurler;
/**
 * @nodoc
 */
class float_array  // growable float array utility
{
	private:
		int m_NumPoints;
		int m_MaxNumPoints;
		float *m_Points;
		const int MAX_PTS;  // limit_memory() will NULL out if storage > MAX_PTS

	public:
		float_array();
		~float_array();
		float *get_points() const;
		int get_num_points() const;

		void limit_memory();  // kill memory after use, if > MAX_PTS
		void set_points(int num_pts,const double *points);
};

/**
 * This class provides provides a concrete <tt>DM_draw_engine</tt> for drawing <tt>DM_default_icons</tt> with Hoops.
 * <br>
 * <b>Role:</b> The <tt>ADMHOOPS_draw_engine</tt> encapsulates the drawing methods of Hoops, wrapping them in 
 * the <tt>DM_draw_engine</tt> abstract interface, thus
 * providing HOOPS drawing methods to <tt>DM_default_icon</tt>s.
 * <br><br>
 * <b>Related Classes:</b> The <tt>ADMHOOPS_icon_draw_args</tt> object can provide draw data which passes through the
 * <tt>DM_icon::Draw</tt> method to the <tt>ADMHOOPS_draw_engine</tt>. A <tt>DM_default_icon</tt> requests a
 * concrete <tt>DM_draw_engine</tt> object from the singleton <tt>DM_draw_engine_mgr</tt>. The initializer for
 * the admhoops library must load the <tt>DM_draw_engine_mgr</tt> with an <tt>ADMHOOPS_draw_engine</tt>; classes
 * overriding the <tt>DM_draw_engine</tt> must do the same.
 */
class DECL_ADMHOOPS ADMHOOPS_draw_engine : public ADM_draw_engine {

  private:
    // Should not call these.
    ADMHOOPS_draw_engine &operator=(const ADMHOOPS_draw_engine&); // Assignment Operator
    ADMHOOPS_draw_engine(const ADMHOOPS_draw_engine&) ; // Copy constructor
		/*mutable*/ float_array m_pts;  // float cache, so Draw_polyline needn't new every time


  protected:
// Should not call this - use Lose this method
    ~ADMHOOPS_draw_engine();            // destructor

  public:
   /**
	*Default constructor.
	*/
    ADMHOOPS_draw_engine();           // public constructor
	/**
	*Lose method for self-destruction.
	*/
    void Lose();           // public destructor
	/**
	*Notify the draw engine of changes which may affect cached data.
	* <br><br>
	*@param abs_hurler
	*error handler.
	*/
    void Mark_dirty(Spatial_abs_hurler& abs_hurler);    // Notify the draw engine of changes which may affect cached data.

    // The drawing methods
	/**
	*Draw a mesh.
	* <br><br>
	*@param abs_hurler
	*error handler.
	*@param icon_owner
	*icon owner dmod.
	*@param tag
	*icon owner tag ID.
	*@param mesh
	*the mesh.
	*@param colorR
	*color, between 0 and 1.
	*@param colorG
	*color, between 0 and 1.
	*@param colorB
	*color, between 0 and 1.
	*@param args
	*draw args.
	*/
    void Draw_mesh(    Spatial_abs_hurler& abs_hurler,                     // error handler
                       DS_dmod* icon_owner, int tag,                           // icon owner
                       INDEXED_MESH * mesh,                          // the mesh
                       double colorR, double colorG, double colorB,                 // color, R, G, B, between 0 & 1
                       const DM_def_icon_draw_args& args)const;         // draw args

	/**
	*Draw a marker.
	* <br><br>
	*@param abs_hurler
	*error handler.
	*@param icon_owner
	*icon owner dmod.
	*@param tag
	*icon owner tag ID.
	*@param marker
	*xyz marker location.
	*@param colorR
	*color, between 0 and 1.
	*@param colorG
	*color, between 0 and 1.
	*@param colorB
	*color, between 0 and 1.
	*@param width
	*marker width.
	*@param args
	*draw args.
	*/
    void Draw_marker(  Spatial_abs_hurler& abs_hurler,                   // error handler
                       DS_dmod* icon_owner,int tag,                          // icon owner
                       const double* marker,                        // xyz: marker location
                       double colorR, double colorG, double colorB,               // color, R, G, B, between 0 & 1
                       double width,                                // marker width
                       const DM_def_icon_draw_args& args) const;      // draw args
	/**
	*Draw a polyline.
	* <br><br>
	*@param abs_hurler
	*error handler.
	*@param icon_owner
	*icon owner dmod.
	*@param tag
	*icon owner tag ID.
	*@param point
	*number of points.
	*@param pts
	*array of points.
	*@param colorR
	*color, between 0 and 1.
	*@param colorG
	*color, between 0 and 1.
	*@param colorB
	*color, between 0 and 1.
	*@param width
	*line width.
	*@param args
	*draw args.
	*/
    void Draw_polyline(Spatial_abs_hurler& abs_hurler,                 // error handler
                       DS_dmod* icon_owner, int tag,                       // icon owner
                       int point,                                 // number of points
                       const double* pts,                       // array of points
                       double colorR, double colorG, double colorB,             // color, R, G, B, between 0 & 1
                       double width,                              // line width
                       const DM_def_icon_draw_args& args) const;    // draw args


};
/** @} */
#endif //ADMHOOPS_DRAW_ENGINE_H
