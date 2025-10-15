/*******************************************************************/
/*    Copyright (c) 1989-2020 by Spatial Corp.                     */
/*    All rights reserved.                                         */
/*    Protected by U.S. Patents 5,257,205; 5,351,196; 6,369,815;   */
/*                              5,982,378; 6,462,738; 6,941,251    */
/*    Protected by European Patents 0503642; 69220263.3            */
/*    Protected by Hong Kong Patent 1008101A                       */
/*******************************************************************/
#ifndef ADM_REGOBJ_FACTORY_MGR_H
#define ADM_REGOBJ_FACTORY_MGR_H
//
// Header for the DM_draw_engine_mgr class.
//
// Purpose: The ADM_regobj_factory_mgr is a ADM_regobj_factory server.
// The ADM_regobj_factory_mgr manages the singelton ADM_regobj_factory,
// providing global access to the unique instance.
//
// Description: The ADM_regobj_factory_mgr is used by the ADM kernel
// as its access to the singleton ADM_regobj_factory. When the admgi_control
// library is initialized, it creates a concrete ADM_regobj_factory and loads
// it into the ADM_regobj_factory_mgr using the Replace_factory method.
// Users replacing the admgi_control library should do the same.
// If no concrete ADM_regobj_factory is loaded into the ADM_regobj_factory_mgr,
// the dmicon library will not draw.
//
// Related classes: Access to a concrete ADM_regobj's is provided by the
// ADM_regobj_factory_mgr::Make_regobj method.
//

/**
* @file admregobj_factory_mgr.hxx
 * @CAA2Level L1
 * @CAA2Usage U1
 * \addtogroup ACISDM
 *
 * @{
 */

class ADM_regobj_factory;
class ATTRIB_DM2ACIS;
class Spatial_abs_hurler;
class ADM_regobj;

#include "dcl_adm.h"
/**
 * Manages the singleton <tt>DM_regobj_factory</tt> providing global access to the unique instance.
 * <br>
 * <b>Role:</b> The <tt>ADM_regobj_factory_mgr</tt> is used by the ADM kernel as its access
 * to the singleton <tt>ADM_regobj_factory</tt>. When the <tt>admgi_control</tt> library is
 * initialized, it creates a concrete <tt>ADM_regobj_factory</tt> and loads it into the
 * <tt>ADM_regobj_factory_mgr</tt> using the Replace_factory <tt>method</tt>. Users replacing the
 * <tt>admgi_control</tt> library should do the same. If no concrete <tt>ADM_regobj_factory</tt>
 * is loaded into the <tt>ADM_regobj_factory_mgr</tt>, the <tt>dmicon</tt> library will not draw.
 * <br><br>
 * <b>Related classes:</b> Access to a concrete <tt>ADM_regobjs</tt> is provided by the
 * <tt>ADM_regobj_factory_mgr::Make_regobj</tt> method.
 */
class ADM_regobj_factory_mgr {

  protected:
    static ADM_regobj_factory* m_factory;

  public:
/**
 * Default constructor.
 * <br><br>
 * <b>Role:</b> Users should not call a single static instance.
 */
    ADM_regobj_factory_mgr();                             // constructor - users should not call; the draw_eng library creates
                                                          // a single static instance
/**
 * Default destructor.
 */
    ~ADM_regobj_factory_mgr();                            // destructor - users should not call
/**
 * Returns a concrete <tt>ADM_regobj</tt>.
 * <br><br>
 * <b>Role:</b> <tt>ATTRIB_DM2ACIS</tt> specifies what to register with the view controller.
 * <br><br>
 * @param err_handler
 * error handler.
 * @param att
 * attribute.
 */
    static DECL_ADM ADM_regobj* Make_regobj(              // return a concrete ADM_regobj
                                   Spatial_abs_hurler& err_handler,   // error handler
                                   ATTRIB_DM2ACIS* att);      // ATTRIB_DM2ACIS to register with the view-controller
/**
 * Loads a concrete <tt>ADM_regobj_factory</tt>.
 * <br><br>
 * @param err_handler
 * error handler.
 * @param owner
 * take ownership.
 */
    static DECL_ADM void Replace_factory(                 // load a concrete ADM_regobj_factory
                                   Spatial_abs_hurler& err_handler,   // error handler
                                   ADM_regobj_factory*& owner); // take ownership

};


 /** @} */
#endif /*ADM_REGOBJ_FACTORY_MGR_H*/
