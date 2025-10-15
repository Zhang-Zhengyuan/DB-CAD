/*******************************************************************/
/*    Copyright (c) 1989-2020 by Spatial Corp.                     */
/*    All rights reserved.                                         */
/*    Protected by U.S. Patents 5,257,205; 5,351,196; 6,369,815;   */
/*                              5,982,378; 6,462,738; 6,941,251    */
/*    Protected by European Patents 0503642; 69220263.3            */
/*    Protected by Hong Kong Patent 1008101A                       */
/*******************************************************************/
#ifndef ADMHOOPS_DRAW_ARGS_H
#define ADMHOOPS_DRAW_ARGS_H
//
// Header for the ADMHOOPS_icon_draw_args class.
//
// Purpose: The ADMHOOPS_icon_draw_args class provides a concrete draw arguments class for
// drawing DM_default_icons with Hoops.
//
// Description: ADMHOOPS_icon_draw_args provide draw data which
// passes through the DM_icon::Draw method to a concrete DM_draw_engine. A concrete
// DM_draw_engine will process (or ignore, based on information from casting methods)
// the draw request using the draw data.
//
// Related classes: The ADMHOOPS_draw_engine is the only concrete DM_draw_engine class
// which processes ADMHOOPS_icon_draw_args; casting methods allow other concrete DM_draw_engine's
// to ignore the ADMHOOPS_icon_draw_args. Each level of derivation of the DM_icon_draw_args
// introduces another level of casting methods, for RTTI.
//
#include "dm_def_icon_args.hxx"           // base class DM_def_icon_draw_args           // base class DM_def_icon_draw_args
#include "dcl_admhoops.h"            // DECL macros            // DECL macros

/**
* @file admhoops_draw_args.hxx
 * @CAA2Level L1
 * @CAA2Usage U1
 * \addtogroup ADMHOOPS
 *
 * @{
 */

/**
 * This class provides concrete draw arguments for drawing <tt>DM_default_icons</tt> with the HOOPS kernel.
 * <br>
 * <b>Role:</b> <tt>ADMHOOPS_icon_draw_args</tt> provides draw data, which passes through the <tt>DM_icon::Draw</tt> method
 * to a concrete <tt>DM_draw_engine</tt>. A concrete <tt>DM_draw_engine</tt> will process (or ignore, based on information
 * from casting methods) the draw request using the draw data.
 * <br><br>
 * <b>Related Classes:</b> The <tt>ADMHOOPS_draw_engine</tt> is the only concrete <tt>DM_draw_engine</tt> class
 * which processes <tt>ADMHOOPS_icon_draw_args</tt>; casting methods allow other concrete <tt>DM_draw_engines</tt> to
 * ignore the <tt>ADMHOOPS_icon_draw_args</tt>. Each level of derivation of the <tt>DM_icon_draw_args</tt> introduces
 * another level of casting methods, for RTTI.
 */
class DECL_ADMHOOPS ADMHOOPS_icon_draw_args : public DM_def_icon_draw_args
{
 public:
  /**
  *Default constructor.
  */
  ADMHOOPS_icon_draw_args() {}					// public constructor
  /**
  *Default destructor.
  */
  virtual ~ADMHOOPS_icon_draw_args() {};                        // public destructor

	virtual const ADMHOOPS_icon_draw_args* HOOPS_cast() const {return this;}

};
/** @} */
#endif //ADMHOOPS_DRAW_ARGS_H
