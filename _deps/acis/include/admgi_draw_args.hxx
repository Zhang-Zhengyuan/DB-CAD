/*******************************************************************/
/*    Copyright (c) 1989-2020 by Spatial Corp.                     */
/*    All rights reserved.                                         */
/*    Protected by U.S. Patents 5,257,205; 5,351,196; 6,369,815;   */
/*                              5,982,378; 6,462,738; 6,941,251    */
/*    Protected by European Patents 0503642; 69220263.3            */
/*    Protected by Hong Kong Patent 1008101A                       */
/*******************************************************************/
#ifndef DMGI_DRAW_ARGS_H
#define DMGI_DRAW_ARGS_H
//
// Header for the ADM_giicon_draw_args class.
//
// Purpose: The ADM_giicon_draw_args class provides concrete draw args for
// drawing ADM_default_icons with the ACIS GI kernel.
//
// Description: ADM_giicon_draw_args provide draw data which
// passes through the DM_icon::Draw method to a concrete DM_draw_engine. A concrete
// DM_draw_engine will process (or ignore, based on information from casting methods)
// the draw request using the draw data. For example, the draw data specifies which
// view to draw into.
//
// Related classes: The ADM_gidraw_engine is the only concrete DM_draw_engine class
// which processes ADM_giicon_draw_args; casting methods allow other concrete DM_draw_engine's
// to ignore the ADM_giicon_draw_args. Each level of derivation of the DM_icon_draw_args
// introduces another level of casting methods, for RTTI.
//
#include "dm_def_icon_args.hxx"           // base class DM_def_icon_draw_args           // base class DM_def_icon_draw_args
#include "dcl_admgi_draweng.h"     // DECL_ macros     // DECL_ macros

/**
* @file admgi_draw_args.hxx
 * @CAA2Level L1
 * @CAA2Usage U1
 * \addtogroup ADMGI
 *
 * @{
 */

class deformable_figure;

/**
 * This class provides concrete draw arguments for drawing ADM_default_icons with the ACIS GI kernel.
 * <b>Role</b>: ADM_giicon_draw_args provide draw data which passes through the DM_icon::Draw method to a concrete DM_draw_engine. A concrete DM_draw_engine will process (or ignore, based on information from casting methods) the draw request using the draw data. For example, the draw data specifies which view to draw into.
 * <b>Related Classes</b>: The ADM_gidraw_engine is the only concrete DM_draw_engine class which processes ADM_giicon_draw_args; casting methods allow other concrete DM_draw_engines to ignore the ADM_giicon_draw_args. Each level of derivation of the DM_icon_draw_args introduces another level of casting methods, for RTTI.
 * @see deformable_figure
 */
class DECL_DMGI_DRAWENG ADM_giicon_draw_args : public DM_def_icon_draw_args {

 private:
  deformable_figure* m_fig;
 public:
 /**
  *Access method for Graphic Interaction deformable_figure.
  */
  deformable_figure* Fig() const {return m_fig;}             // access to drawing data, for icon drawing
  /**
  *Default constructor.
  *@param fig
  * figure
  */
  ADM_giicon_draw_args(deformable_figure* fig=0) {m_fig=fig;}// public constructor
 /**
  *Default destructor.
  */
  virtual ~ADM_giicon_draw_args() {};                        // public destructor
 /**
  *Override GI_cast, providing RTTI to icons.
  */
  virtual const ADM_giicon_draw_args*  GI_cast()             // override GI_cast, providing RTTI to icons
                                    const{return this;}


};

/** @} */

#endif //DMGI_DRAW_ARGS_H
