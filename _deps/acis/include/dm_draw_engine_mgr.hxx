/*******************************************************************/
/*    Copyright (c) 1989-2020 by Spatial Corp.                     */
/*    All rights reserved.                                         */
/*    Protected by U.S. Patents 5,257,205; 5,351,196; 6,369,815;   */
/*                              5,982,378; 6,462,738; 6,941,251    */
/*    Protected by European Patents 0503642; 69220263.3            */
/*    Protected by Hong Kong Patent 1008101A                       */
/*******************************************************************/
#ifndef DM_DRAW_ENGINE_MGR_H
#define DM_DRAW_ENGINE_MGR_H

// Header for the DM_draw_engine_mgr class.
//
// Purpose: The DM_draw_engine_mgr is a DM_draw_engine server.
// The DM_draw_engine_mgr manages the singelton DM_draw_engine,
// providing global access to the unique instance.
//
// Description: The DM_draw_engine_mgr is used by the dmicon library
// as its access to the singleton DM_draw_engine. If the admgi_draweng
// library is initialized, it creates a concrete DM_draw_engine and loads
// it into the DM_draw_engine_mgr using the Replace_draw_engine method.
// Users replacing the dmicon library should do the same.
// If no concrete DM_draw_engine is loaded into the DM_draw_engine_mgr,
// DM_default_icon's will not draw.
//
// Related classes: Access to a concrete DM_draw_engine is provided by the
// DM_draw_engine_mgr::Instance method.
//
#include "dcl_dmicon.h"
#include "mmgr.hxx" 
/** 
 * \defgroup DMICONAPI Interactive Graphics Interface
 * \ingroup DMAPI
 *
 * (surfaces, curves, constraints, and loads).
 * A common command/query interface is provided by the  
 * DM_default_icon base class, which is derived from the  DM_icon interface class.
 */
/**
 * @file dm_draw_engine_mgr.hxx
 * @CAA2Level L1
 * @CAA2Usage U1
 * \addtogroup DMICONAPI
 *
 * @{
 */

class DM_draw_engine;
class Spatial_abs_hurler;

/**
 * This class manages the singleton <tt><tt>DM_draw_engine</tt></tt>, providing global access to the unique instance.
 *<br>
 * <b>Role:</b> The <tt>DM_draw_engine_mgr</tt> is used by the dmicon library as its access to the
 * singleton <tt>DM_draw_engine</tt>. If the <tt>admgi_draweng</tt> library is initialized, it creates a
 * concrete <tt>DM_draw_engine</tt> and loads it into the <tt>DM_draw_engine_mgr</tt> using the
 * <tt>Replace_draw_engine</tt> method. Users replacing the dmicon library should do the same.
 * <br><br>If no concrete <tt>DM_draw_engine</tt> is loaded into the <tt>DM_draw_engine_mgr</tt>, <tt>DM_default_icons</tt>
 * will not draw.
 * <br><br><b>Related classes:</b> Access to a concrete <tt>DM_draw_engine</tt> is provided by the
 * <tt>DM_draw_engine_mgr::Instance</tt> method.
 *@see DM_draw_engine
 */
class DM_draw_engine_mgr : public ACIS_OBJECT {

  private:

    static DM_draw_engine* m_draw_engine;

  public:

/**
 * Public constructor.
 *<br><br>
 * <b>Role:</b> Users should not call this; the icon library creates a single static instance.
 */
    DM_draw_engine_mgr();                             // constructor - users should not call; the icon library creates
                                                      // a single static instance

/**
 * Road a draw engine.
 *<br><br>
 * <b>Role:</b>  The owner argument takes ownership of a concrete <tt>DM_draw_engine</tt>.
 * This is the prototype for future requests.
 *<br><br>
 * @param  err_handler
 *  error handler.
 * @param owner
 * owner.
 */
    static DECL_DM_ICON void Replace_draw_engine(     // load a draw engine
                                Spatial_abs_hurler& err_handler,  // error handler
                                DM_draw_engine*& owner);    // take ownership of a concrete DM_draw_engine;
                                                      // prototype for future requests

/**
 * Requests a concrete <tt>DM_draw_engine</tt> object.
 */
     static DECL_DM_ICON DM_draw_engine* Instance();   // request a concrete DM_draw_engine object
/**
 * Destructor; use the <tt>Lose</tt> method instead.
 */
    ~DM_draw_engine_mgr();                            // destructor - users should not call
};

 /** @} */
#endif /*DM_DRAW_ENGINE_MGR_H*/
