/*******************************************************************/
/*    Copyright (c) 1989-2020 by Spatial Corp.                     */
/*    All rights reserved.                                         */
/*    Protected by U.S. Patents 5,257,205; 5,351,196; 6,369,815;   */
/*                              5,982,378; 6,462,738; 6,941,251    */
/*    Protected by European Patents 0503642; 69220263.3            */
/*    Protected by Hong Kong Patent 1008101A                       */
/*******************************************************************/
#ifndef DM_DEFAULT_ICON_FACTORY_H
#define DM_DEFAULT_ICON_FACTORY_H

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

#include "dmicon_factory.hxx"
#include "dcl_dmicon.h"

/**
* @file dm_default_icon_factory.hxx
 * @CAA2Level L1
 * @CAA2Usage U1
 * \addtogroup DMICONAPI
 *
 * @{
 */

class Spatial_abs_hurler;
/**
 * This is an example of the icon class factory implementation.
 *<br>
 * <b>Role:</b> To replace icons, a user can replace the <tt>cxx</tt> files in the <tt>DM_icon</tt> library, or add
 * new ones and replace <tt>DM_default_icon_factory.cxx</tt>.
 *<br><br>
 * @see DM_icon
 */

class DECL_DM_ICON DM_default_icon_factory : public DM_icon_factory {

  private:
// Should not call these
    DM_default_icon_factory&
    operator=                                               // Assignment Operator
    (const DM_default_icon_factory&);

    DM_default_icon_factory(const DM_default_icon_factory&) ;// Copy constructor

  protected:
// Hold a pointer to each type of icon.
// constraints
    DM_icon* m_area_cstrn_icon;
    DM_icon* m_crv_cstrn_icon;
    DM_icon* m_link_cstrn_icon;
    DM_icon* m_pt_cstrn_icon;
    DM_icon* m_area_load_icon;
// loads
    DM_icon* m_spring_load_icon;
    DM_icon* m_spring_set_icon;
    DM_icon* m_dist_press_icon;
    DM_icon* m_attractor_icon;
    DM_icon* m_vector_load_icon;
    DM_icon* m_point_press_icon;
    DM_icon* m_crv_load_icon;
    DM_icon* m_link_load_icon;
// dmods
    DM_icon* m_srf_icon;
    DM_icon* m_crv_icon;


    virtual ~DM_default_icon_factory();

  public:
/**
 * Default constructor.
 */
    DM_default_icon_factory();                  // default constructor
/**
 * Sets up a <tt>DM_default_icon_factory</tt> with the default values.
 *<br><br>
 * @param err_handler
 * error handler.
 */
   virtual void Initialize(Spatial_abs_hurler& err_handler);       // Set default icons

/**
 * Lose method for self-destruction.
 */
    virtual void Lose();                        // delete the factory
/**
 * Deep copy this factory.
 *<br><br>
 * @param err_handler
 * error handler.
 */
    virtual DM_icon_factory* Make_copy(Spatial_abs_hurler& err_handler) const;  // Deep copy this factory

// Base class static member will construct the singleton instance of this

// Replace methods - take ownership of the new icon & use as a prototype
// for making new icons. One for each type of tag object.
// constraint icons
/**
 * Replace methods change what the factory makes.
 *<br><br>
 * @param err_handler
 * error handler.
 * @param icon_chg
 * icon to change.
 */
    virtual void Replace_area_cstrn_icon(Spatial_abs_hurler& err_handler,DM_icon*& icon_chg);
/**
 * Replace methods change what the factory makes.
 *<br><br>
 * @param err_handler
 * error handler.
 * @param icon_chg
 * icon to change.
 */

    virtual void Replace_pt_cstrn_icon(Spatial_abs_hurler& err_handler,DM_icon*& icon_chg);
/**
 * Replace methods change what the factory makes.
 *<br><br>
 * @param err_handler
 * error handler.
 * @param icon_chg
 * icon to change.
 */

    virtual void Replace_crv_cstrn_icon(Spatial_abs_hurler& err_handler,DM_icon*& icon_chg);
/**
 * Replace methods change what the factory makes.
 *<br><br>
 * @param err_handler
 * error handler.
 * @param icon_chg
 * icon to change.
 */

    virtual void Replace_link_cstrn_icon(Spatial_abs_hurler& err_handler,DM_icon*& icon_chg);
/**
 * Replace methods change what the factory makes.
 *<br><br>
 * @param err_handler
 * error handler.
 * @param icon_chg
 * icon to change.
 */

    virtual void Replace_area_load_icon(Spatial_abs_hurler& err_handler,DM_icon*& icon_chg);
//load icons
/**
 * Replace methods change what the factory makes.
 *<br><br>
 * @param err_handler
 * error handler.
 * @param icon_chg
 * icon to change.
 */

    virtual void Replace_spring_load_icon(Spatial_abs_hurler& err_handler,DM_icon*& icon_chg);
/**
 * Replace methods change what the factory makes.
 *<br><br>
 * @param err_handler
 * error handler.
 * @param icon_chg
 * icon to change.
 */

    virtual void Replace_spring_set_icon(Spatial_abs_hurler& err_handler,DM_icon*& icon_chg);
/**
 * Replace methods change what the factory makes.
 *<br><br>
 * @param err_handler
 * error handler.
 * @param icon_chg
 * icon to change.
 */

    virtual void Replace_dist_press_icon(Spatial_abs_hurler& err_handler,DM_icon*& icon_chg);
/**
 * Replace methods change what the factory makes.
 *<br><br>
 * @param err_handler
 * error handler.
 * @param icon_chg
 * icon to change.
 */

    virtual void Replace_attractor_icon(Spatial_abs_hurler& err_handler,DM_icon*& icon_chg);
/**
 * Replace methods change what the factory makes.
 *<br><br>
 * @param err_handler
 * error handler.
 * @param icon_chg
 * icon to change.
 */

   virtual void Replace_vector_load_icon(Spatial_abs_hurler& err_handler,DM_icon*& icon_chg);
/**
 * Replace methods change what the factory makes.
 *<br><br>
 * @param err_handler
 * error handler.
 * @param icon_chg
 * icon to change.
 */

    virtual void Replace_point_press_icon(Spatial_abs_hurler& err_handler,DM_icon*& icon_chg);
/**
 * Replace methods change what the factory makes.
 *<br><br>
 * @param err_handler
 * error handler.
 * @param icon_chg
 * icon to change.
 */

    virtual void Replace_crv_load_icon(Spatial_abs_hurler& err_handler,DM_icon*& icon_chg);
/**
 * Replace methods change what the factory makes.
 *<br><br>
 * @param err_handler
 * error handler.
 * @param icon_chg
 * icon to change.
 */
    virtual void Replace_link_load_icon(Spatial_abs_hurler& err_handler,DM_icon*& icon_chg);
// dmod icons
/**
 * Replace methods change what the factory makes.
 *<br><br>
 * @param err_handler
 * error handler.
 * @param icon_chg
 * icon to change.
 */

    virtual void Replace_srf_icon(Spatial_abs_hurler& err_handler,DM_icon*& icon_chg);
/**
 * Replace methods change what the factory makes.
 *<br><br>
 * @param err_handler
 * error handler.
 * @param icon_chg
 * icon to change.
 */

    virtual void Replace_crv_icon(Spatial_abs_hurler& err_handler,DM_icon*& icon_chg);

// constraints
/**
 * Makes an area constraint icon.
 *<br><br>
 * @param err_handler
 * error handler.
 */
    virtual DM_icon* Make_area_cstrn_icon(Spatial_abs_hurler& err_handler)const;
/**
 * Makes a curve constraint icon.
 *<br><br>
 * @param err_handler
 * error handler.
 */
    virtual DM_icon* Make_crv_cstrn_icon(Spatial_abs_hurler& err_handler)const;
/**
 * Makes a link constraint icon.
 *<br><br>
 * @param err_handler
 * error handler.
 */
    virtual DM_icon* Make_link_cstrn_icon(Spatial_abs_hurler& err_handler)const;
/**
 * Makes a point constraint icon.
 *<br><br>
 * @param err_handler
 * error handler.
 */
    virtual DM_icon* Make_pt_cstrn_icon(Spatial_abs_hurler& err_handler)const;
/**
 * Makes an area load icon.
 *<br><br>
 * @param err_handler
 * error handler.
 */
    virtual DM_icon* Make_area_load_icon(Spatial_abs_hurler& err_handler)const;
// loads
/**
 * Makes a spring load icon.
 *<br><br>
 * @param err_handler
 * error handler.
 */
    virtual DM_icon* Make_spring_load_icon(Spatial_abs_hurler& err_handler)const;

/**
 * Makes a spring set icon.
 *<br><br>
 * @param err_handler
 * error handler.
 */
    virtual DM_icon* Make_spring_set_icon(Spatial_abs_hurler& err_handler)const;
/**
 * Makes a distributed pressure icon.
 *<br><br>
 * @param err_handler
 * error handler.
 */
    virtual DM_icon* Make_dist_press_icon(Spatial_abs_hurler& err_handler)const;
/**
 * Makes an attractor icon.
 *<br><br>
 * @param err_handler
 * error handler.
 */
    virtual DM_icon* Make_attractor_icon(Spatial_abs_hurler& err_handler)const;
/**
 * Makes a vector load icon.
 *<br><br>
 * @param err_handler
 * error handler.
 */

    virtual DM_icon* Make_vector_load_icon(Spatial_abs_hurler& err_handler)const;
/**
 * Makes a point pressure icon.
 *<br><br>
 * @param err_handler
 * error handler.
 */
    virtual DM_icon* Make_point_press_icon(Spatial_abs_hurler& err_handler)const;
/**
 * Makes a curve load icon.
 *<br><br>
 * @param err_handler
 * error handler.
 */
    virtual DM_icon* Make_crv_load_icon(Spatial_abs_hurler& err_handler)const;
/**
 * Makes a link load icon.
 *<br><br>
 * @param err_handler
 * error handler.
 */
     virtual DM_icon* Make_link_load_icon(Spatial_abs_hurler& err_handler)const;
// dmods
/**
 * Makes a surface icon.
 *<br><br>
 * @param err_handler
 * error handler.
 */
    virtual DM_icon* Make_srf_icon(Spatial_abs_hurler& err_handler)const;
/**
 * Makes a curve icon.
 *<br><br>
 * @param err_handler
 * error handler.
 */
    virtual DM_icon* Make_crv_icon(Spatial_abs_hurler& err_handler)const;

};

 /** @} */
#endif /*dm_default_ICON_FACTORY_H*/
