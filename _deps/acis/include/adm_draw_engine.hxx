/*******************************************************************/
/*    Copyright (c) 1989-2020 by Spatial Corp.                     */
/*    All rights reserved.                                         */
/*    Protected by U.S. Patents 5,257,205; 5,351,196; 6,369,815;   */
/*                              5,982,378; 6,462,738; 6,941,251    */
/*    Protected by European Patents 0503642; 69220263.3            */
/*    Protected by Hong Kong Patent 1008101A                       */
/*******************************************************************/
#ifndef ADM_DRAW_ENGINE_H
#define ADM_DRAW_ENGINE_H
//
// Header for the ADM_draw_engine class.
//
// Purpose: The ADM_draw_engine class provides provides an abstract
// draw primitive interface between the ADM icons and the view-controller.
//
// Description: The ADM_draw_engine interface classes provide abstract
// draw primitive methods to icons: Draw_point, Draw_polyline, and Draw_mesh.
// This decouples the icon library from rendering-context specifics,
// for example, OpenGL or DirectX commands.
//
// Related classes: The ADM_draw_engine extends the DM_draw_engine by providing a
// Draw_mesh method. Draw_mesh uses an ACIS INDEXED_MESH. The draw engine uses
// the data encapsulated in a DM_draw_args object for drawing. The DM_draw_args object
// is passed through from the original view manager draw request. A DM_default_icon requests
// a concrete DM_draw_engine object from the singleton DM_draw_engine_mgr.
//
//
#include "dcl_admicon.h"        // declspec def'n        // declspec def'n
#include "dm_draw_engine.hxx"    // class DM_draw_engine    // class DM_draw_engine
//

/**
* @file adm_draw_engine.hxx
 * @CAA2Level L1
 * @CAA2Usage U1
 * \addtogroup ACISDM
 *
 * @{
 */

class INDEXED_MESH;
class DS_dmod;
class Spatial_abs_hurler;
/**
 * Overrides the base class for the drawing engine.
 * <br><br><b>Role: </b>This class implements the abstract draw primitive interface
 * between the <tt>ADM</tt> icons and the view controller.
 */
class DECL_ADM_ICON ADM_draw_engine : public DM_draw_engine {

  private:
// Should not call these
    ADM_draw_engine&
    operator=                                       // assignment Operator
    (const ADM_draw_engine& );

    ADM_draw_engine(const ADM_draw_engine& ) ;      // copy constructor

  protected:
    // Destructor
		  // Should not call this. Use Lose method.
    virtual ~ADM_draw_engine() {}

  public:

/**
 * Constructs an <tt>ADM_draw_engine</tt> object.
 */
    ADM_draw_engine() {}                                  // public constructor
/**
 * Self-destructs an <tt>ADM_draw_engine</tt> object.
 * <br><br>
 * <b>Role: </b>Users should call Lose method instead of global delete (destructor
 * is protected).
 */
    virtual void Lose() = 0;                                // public destructor

/**
 * Casting operator.
 */
    virtual ADM_draw_engine* Spatial_cast() {return this;}// casting operator
    // drawing methods
/**
 * Adds the mesh.
 * <br><br>
 * @param abshurler
 *	 error handler.
 * @param icon_owner
 *	 icon owner.
 * @param tag
 *	 tag.
 * @param mesh
 *	 the mesh.
 * @param color_r
 *	 color R.
 * @param color_g
 *	 color G.
 * @param color_b
 *	 color B.
 * @param args
 *	 draw args.
 */
    virtual void Draw_mesh(
                            Spatial_abs_hurler& abshurler,                     // error handler
                            DS_dmod* icon_owner, int tag,                           // icon owner
                            INDEXED_MESH * mesh,                          // the mesh
                            double color_r, double color_g, double color_b,                 // color, R, G, B, between 0 & 1
                            const DM_def_icon_draw_args& args) const =0;     // draw args


}	;








 /** @} */
#endif //ADM_DRAW_ENGINE_H
