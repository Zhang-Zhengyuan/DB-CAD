/*******************************************************************/
/*    Copyright (c) 1989-2020 by Spatial Corp.                     */
/*    All rights reserved.                                         */
/*    Protected by U.S. Patents 5,257,205; 5,351,196; 6,369,815;   */
/*                              5,982,378; 6,462,738; 6,941,251    */
/*    Protected by European Patents 0503642; 69220263.3            */
/*    Protected by Hong Kong Patent 1008101A                       */
/*******************************************************************/
#ifndef DMGI_DRAW_ENGINE_H
#define DMGI_DRAW_ENGINE_H
//
// Header for the ADM_gidraw_engine class.
//
// Purpose: The ADM_gidraw_engine class provides provides a concrete
// ADM_draw_engine for drawing ADM_default_icons with the ACIS GI kernel.
//
// Description: The ADM_gidraw_engine encapsualtes the drawing methods of
// the ACIS GI kernel deformable_figure class, wrapping them in the
// ADM_draw_engine abstract interface, thus providing GI kernel drawing methods
// to the DM_default_icon's.
//
// Related classes: The ADM_giicon_draw_args provide draw data which
// passes through the DM_icon::Draw method to the ADM_gidraw_engine.
// A DM_default_icon requests a concrete DM_draw_engine object from the
// singleton DM_draw_engine_mgr. The initializer for the admgi_control library
// must load the DM_draw_engine_mgr with an ADM_gidraw_engine; classes overriding
// the ADM_draw_engine must do the same.
//
#include "adm_draw_engine.hxx"                // base class ADM_draw_engine                // base class ADM_draw_engine
#include "dcl_admgi_draweng.h"          // DECL_ macros          // DECL_ macros

class Spatial_abs_hurler;

/**
* @file admgi_draw_engine.hxx
 * @CAA2Level L1
 * @CAA2Usage U1
 * \addtogroup ADMGI
 *
 * @{
 */

/**
 *This class provides provides a concrete ADM_draw_engine for drawing ADM_default_icons with the ACIS GI kernel.
 *<b>Role</b>: The ADM_gidraw_engine encapsulates the drawing methods of the ACIS GI kernel deformable_figure class, wrapping them in the ADM_draw_engine abstract interface, thus providing GI kernel drawing methods to the DM_default_icon.
 *<b>Related Classes</b>: The ADM_giicon_draw_args provide draw data which passes through the DM_icon::Draw method to the ADM_gidraw_engine. A DM_default_icon requests a concrete DM_draw_engine object from the singleton DM_draw_engine_mgr. The initializer for the admgi_control library must load the DM_draw_engine_mgr with an ADM_gidraw_engine; classes overriding the ADM_draw_engine must do the same.
 */
class DECL_DMGI_DRAWENG ADM_gidraw_engine : public ADM_draw_engine {

  private:
    // Should not call these.
    ADM_gidraw_engine &
    operator=                                   // Assignment Operator
    (const ADM_gidraw_engine&);

    ADM_gidraw_engine(const ADM_gidraw_engine&) ; // Copy constructor


  protected:
// Should not call this - use Lose this method
    ~ADM_gidraw_engine();            // destructor


  public:

	/**
	*Default constructor.
	*/
    ADM_gidraw_engine();           // public constructor
	/**
	*Lose method for self-destruction.
	*/
    virtual void Lose();           // public destructor
	/**
	*Notify the draw engine of changes which may affect cached data.
	*@param abs_hurler
	*error handler
	*/
    virtual void Mark_dirty(Spatial_abs_hurler& abs_hurler);    // Notify the draw engine of changes which may affect cached data.

    // The drawing methods
	/**
	*Draw a mesh.
	*@param abs_hurler
	*error handler
	*@param icon_owner
	*icon owner
	*@param tag
	* owner tag
	*@param mesh
	*the mesh
	*@param colorR
	*color, between 0 and 1
	*@param colorG
	*color, between 0 and 1
	*@param colorB
	*color, between 0 and 1
	*@param args
	*draw args
	*/
	virtual void Draw_mesh(
                            Spatial_abs_hurler& abs_hurler,                     // error handler
                            DS_dmod* icon_owner, int tag,                           // icon owner
                            INDEXED_MESH * mesh,                          // the mesh
                            double colorR, double colorG, double colorB,                 // color, R, G, B, between 0 & 1
                            const DM_def_icon_draw_args& args)const;         // draw args

	/**
	*Draw a marker.
	*@param abs_hurler
	*error handler
	*@param icon_owner
	*icon owner
	*@param tag
	* owner tag
	*@param marker
	*xyz marker location
	*@param colorR
	*color, between 0 and 1
	*@param colorG
	*color, between 0 and 1
	*@param colorB
	*color, between 0 and 1
	*@param width
	*marker width
	*/
    virtual void Draw_marker(
                              Spatial_abs_hurler& abs_hurler,                   // error handler
                              DS_dmod* icon_owner,int tag,                          // icon owner
                              const double* marker,                        // xyz: marker location
                              double colorR, double colorG, double colorB,               // color, R, G, B, between 0 & 1
                              double width,                                // marker width
                              const DM_def_icon_draw_args& args) const;      // draw args
	/**
	*Draw a polyline.
	*@param abs_hurler
	*error handler
	*@param icon_owner
	*icon owner
	*@param tag
	* owner tag
	*@param point
	*number of points
	*@param pts
	*array of points
	*@param colorR
	*color, between 0 and 1
	*@param colorG
	*color, between 0 and 1
	*@param colorB
	*color, between 0 and 1
	*@param width
	*line width
	*@param args
	*draw args
	*/
    virtual void Draw_polyline(
                                Spatial_abs_hurler& abs_hurler,                 // error handler
                                DS_dmod* icon_owner, int tag,                       // icon owner
                                int point,                                 // number of points
                                const double* pts,                       // array of points
                                double colorR, double colorG, double colorB,             // color, R, G, B, between 0 & 1
                                double width,                              // line width
                                const DM_def_icon_draw_args& args) const;    // draw args


};
/** @} */
#endif //DMGI_DRAW_ENGINE_H
