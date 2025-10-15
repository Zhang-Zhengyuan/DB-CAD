/*******************************************************************/
/*    Copyright (c) 1989-2020 by Spatial Corp.                     */
/*    All rights reserved.                                         */
/*    Protected by U.S. Patents 5,257,205; 5,351,196; 6,369,815;   */
/*                              5,982,378; 6,462,738; 6,941,251    */
/*    Protected by European Patents 0503642; 69220263.3            */
/*    Protected by Hong Kong Patent 1008101A                       */
/*******************************************************************/
#ifndef ADM_HOOPS_ICON_FACTORY_H
#define ADM_HOOPS_ICON_FACTORY_H

// This is the icon class factory SDM implementation.
// To replace icons, a user can replace the cxx files in the DM_icon library,
// or add new ones and replace DM_default_icon_factory.cxx.
//
// Header for the DM_default_icon_factory class.
//
// Purpose: The DM_default_icon_factory class provides an implementation of the DM_icon_factory
// for the dmicon library. The DM_icon_factory interface is used by the Deformable Modeling
// kernel for creating and destroying concrete DM_icon objects - the kernel only knows about
// the abstract DM_icon interface.
//
// Description: For each type of Deformable Modeling tag object (DS_dmod, DS_load, or DS_cstrn),
// the DM_default_icon_factory owns a prototype DM_default_icon. The DM_default_icon
// factory makes new icons by cloning the prototype icon, i.e, using the DM_icon::Make_copy method.
// Changing the prototype, using the DM_default_icon_factory::Replace_xxx_icon method, changes
// the type of icon subsequent calls to DM_default_icon_factory::Make_xxx_icon will return.
//
// Related classes: The DM_default_icon_factory_mgr is instantiated as a singleton by the dmicon
// library, providing unique global access to a DM_default_icon factory. The
// DM_default_icon_factory_mgr can be initialized to hold a pointer to a DM_default_icon_factory,
// providing access to the DM_default_icon_factory public  methods.
//

#include "dcl_admhoops.h"
#include "dm_default_icon_factory.hxx"

/**
* @file admhoops_icon_factory.hxx
 * @CAA2Level L1
 * @CAA2Usage U1
 * \addtogroup ADMHOOPS
 *
 * @{
 */

class Spatial_abs_hurler;
/**
 * This is an example of the icon class factory implementation.
 * <b>Role</b>:To replace icons, a user can replace the cpp files in the DM_icon library, or add
 * new ones and replace DM_default_icon_factory.cpp.
 * @see DM_icon
 */

class DECL_ADMHOOPS ADM_hoops_icon_factory : public DM_default_icon_factory {

  private:
// Should not call these
    ADM_hoops_icon_factory&
    operator=                                               // Assignment Operator
    (const ADM_hoops_icon_factory&);

    ADM_hoops_icon_factory(const ADM_hoops_icon_factory&) ;// Copy constructor

  protected:

    virtual ~ADM_hoops_icon_factory();

  public:
/**
 * Default constructor.
 */
    ADM_hoops_icon_factory();                  // default constructor
/**
 * Lose method for self-destruction.
 */
    virtual void Lose();                        // delete the factory
/**
 * Deep copy this factory.
 * @param err_handler
 * error handler
 */
    virtual DM_icon_factory* Make_copy(Spatial_abs_hurler& err_handler) const;  // Deep copy this factory

// Base class static member will construct the singleton instance of this

// Replace methods - take ownership of the new icon & use as a prototype
// for making new icons. One for each type of tag object.
// constraint icons
/**
 * Replace methods change what the factory makes.
 * @param err_handler
 * error handler
 * @param icon_chg
 * icon to change
 */
    virtual void Replace_area_cstrn_icon(Spatial_abs_hurler& err_handler,DM_icon*& icon_chg);
/**
 * Replace methods change what the factory makes.
 * @param err_handler
 * error handler
 * @param icon_chg
 * icon to change
 */

    virtual void Replace_pt_cstrn_icon(Spatial_abs_hurler& err_handler,DM_icon*& icon_chg);
/**
 * Replace methods change what the factory makes.
 * @param err_handler
 * error handler
 * @param icon_chg
 * icon to change
 */

    virtual void Replace_crv_cstrn_icon(Spatial_abs_hurler& err_handler,DM_icon*& icon_chg);
/**
 * Replace methods change what the factory makes.
 * @param err_handler
  * error handler
  * @param icon_chg
 * icon to change
 */

    virtual void Replace_link_cstrn_icon(Spatial_abs_hurler& err_handler,DM_icon*& icon_chg);
/**
 * Replace methods change what the factory makes.
 * @param err_handler
 * error handler
 * @param icon_chg
 * icon to change
 */

    virtual void Replace_area_load_icon(Spatial_abs_hurler& err_handler,DM_icon*& icon_chg);
//load icons
/**
 * Replace methods change what the factory makes.
 * @param err_handler
 * error handler
 * @param icon_chg
 * icon to change
 */

    virtual void Replace_spring_load_icon(Spatial_abs_hurler& err_handler,DM_icon*& icon_chg);
/**
 * Replace methods change what the factory makes.
 * @param err_handler
 * error handler
 * @param icon_chg
 * icon to change
 */

    virtual void Replace_spring_set_icon(Spatial_abs_hurler& err_handler,DM_icon*& icon_chg);
/**
 * Replace methods change what the factory makes.
 * @param err_handler
 * error handler
 * @param icon_chg
 * icon to change
 */

    virtual void Replace_dist_press_icon(Spatial_abs_hurler& err_handler,DM_icon*& icon_chg);
/**
 * Replace methods change what the factory makes.
 * @param err_handler
 * error handler
 * @param icon_chg
 * icon to change
 */

    virtual void Replace_attractor_icon(Spatial_abs_hurler& err_handler,DM_icon*& icon_chg);
/**
 * Replace methods change what the factory makes.
 * @param err_handler
 * error handler
 * @param icon_chg
 * icon to change
 */

   virtual void Replace_vector_load_icon(Spatial_abs_hurler& err_handler,DM_icon*& icon_chg);
/**
 * Replace methods change what the factory makes.
 * @param err_handler
 * error handler
 * @param icon_chg
 * icon to change
 */

    virtual void Replace_point_press_icon(Spatial_abs_hurler& err_handler,DM_icon*& icon_chg);
/**
 * Replace methods change what the factory makes.
 * @param err_handler
 * error handler
 * @param icon_chg
 * icon to change
 */

    virtual void Replace_crv_load_icon(Spatial_abs_hurler& err_handler,DM_icon*& icon_chg);
/**
 * Replace methods change what the factory makes.
 * @param err_handler
 * error handler
 * @param icon_chg
 * icon to change
 */
    virtual void Replace_link_load_icon(Spatial_abs_hurler& err_handler,DM_icon*& icon_chg);
// dmod icons
/**
 * Replace methods change what the factory makes.
 * @param err_handler
 * error handler
 * @param icon_chg
 * icon to change
 */

    virtual void Replace_srf_icon(Spatial_abs_hurler& err_handler,DM_icon*& icon_chg);
/**
 * Replace methods change what the factory makes.
 * @param err_handler
 * error handler
 * @param icon_chg
 * icon to change
 */

    virtual void Replace_crv_icon(Spatial_abs_hurler& err_handler,DM_icon*& icon_chg);

};
/** @} */
#endif /*ADM_HOOPS_ICON_FACTORY_H*/
