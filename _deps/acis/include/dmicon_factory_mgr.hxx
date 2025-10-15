/*******************************************************************/
/*    Copyright (c) 1989-2020 by Spatial Corp.                     */
/*    All rights reserved.                                         */
/*    Protected by U.S. Patents 5,257,205; 5,351,196; 6,369,815;   */
/*                              5,982,378; 6,462,738; 6,941,251    */
/*    Protected by European Patents 0503642; 69220263.3            */
/*    Protected by Hong Kong Patent 1008101A                       */
/*******************************************************************/
#ifndef DM_ICON_FACTORY_MGR_H
#define DM_ICON_FACTORY_MGR_H

// Header for the DM_icon_factory_mgr class.
//
// Purpose: The DM_icon_factory_mgr is a DM_icon_factory server.
// The DM_icon_factory_mgr manages the singelton DM_icon_factory,
// providing global access to the unique instance.
//
// Description: The DM_icon_factory_mgr is used by the deformable modeling
// kernel as its access to the singleton DM_icon_factory. If the dmicon
// library is initialized, it creates a DM_default_icon_factory and loads
// it into the DM_icon_factory_mgr using the Replace_factory method.
// Users replacing the dmicon library should do the same.
// If no concrete DM_icon_factory is loaded into the DM_icon_factory_mgr,
// no icons will be created by the deformable modeling kernel.
//
// Related classes: Access to a concrete DM_icon_factory is provided by the
// DM_icon_factory_mgr::Instance method.
//
class DM_icon_factory;
class Spatial_abs_hurler;

#include "dcl_ds.h"       // DECL_ macros       // DECL_ macros
#include "mmgr.hxx" // class ACIS_OBJECT
/**
* @file dmicon_factory_mgr.hxx
 * @CAA2Level L1
 * @CAA2Usage U2
 * \addtogroup DMAPI
 *
 * @{
 */

// Destructs the singleton DM_icon_factory
// We create a static instance to clean up the DM_icon_factory on program termination

/**
 * This class manages the singleton <tt>DM_icon_factory</tt>, providing global access to the unique instance.
 * <br>
 * <b>Role:</b> The <tt>DM_icon_factory_mgr</tt> is used by the deformable modeling kernel as its access
 * to the singleton <tt>DM_icon_factory</tt>. If the <tt>dmicon</tt> library is initialized, it creates a
 * <tt>DM_default_icon_factory</tt> and loads it into the <tt>DM_icon_factory_mgr</tt> using the <tt>Replace_factory</tt>
 * method. Users replacing the <tt>dmicon</tt> library should do the same. If no concrete <tt>DM_icon_factory</tt>
 * is loaded into the <tt>DM_icon_factory_mgr</tt>, no icons will be created by the deformable modeling kernel.
 * <br><br>
 * <b>Related classes:</b> Access to a concrete <tt>DM_icon_factory</tt> is provided by the
 * <tt>DM_icon_factory_mgr::Instance</tt> method.
 */
class DM_icon_factory_mgr : public ACIS_OBJECT {

  protected:

/**
 * A static instance to clean up the <tt>DM_icon_factory</tt> on program termination.
 */
   static DM_icon_factory* m_factory;

  public:

/**
 * Public constructor.
 */
     DM_icon_factory_mgr();                                   // constructor - should not be called
/**
 * Destructor; should not be called.
 */
    ~DM_icon_factory_mgr();                                  // destructor - should not be called

/**
 * Takes ownership and nulls the pointer.
 * <br><br>
 * @param err_handler
 * error handler.
 * @param icon
 * icon factory.
 */

  static DECL_DM void Replace_factory(Spatial_abs_hurler& err_handler, // error handler
                                        DM_icon_factory*& icon);  // take ownership and null the pointer
/**
 * Provides global access to the singleton icon factory.
 */
     static DECL_DM DM_icon_factory* Instance();              // provide global access to the singleton
                                                             // icon factory
};
/** @} */
#endif /*DM_ICON_FACTORY_MGR_H*/
