/*******************************************************************/
/*    Copyright (c) 1989-2020 by Spatial Corp.                     */
/*    All rights reserved.                                         */
/*    Protected by U.S. Patents 5,257,205; 5,351,196; 6,369,815;   */
/*                              5,982,378; 6,462,738; 6,941,251    */
/*    Protected by European Patents 0503642; 69220263.3            */
/*    Protected by Hong Kong Patent 1008101A                       */
/*******************************************************************/
#ifndef DM_ICON_FACTORY_H
#define DM_ICON_FACTORY_H
// Header for the DM_icon_factory class.
//
// Purpose: The DM_icon_factory class provides an abstract interface to the Deformable Modeling
// kernel for creating and destroying concrete DM_icon objects - the kernel only knows about
// the abstract DM_icon interface.
//
// Description: For each type of Deformable Modeling tag object (DS_dmod, DS_load, or DS_cstrn),
// the DM_icon_factory has a Replace_xxx_icon and a Make_xxx_icon method. The Deformable Modeling
// kernel uses the Make_xxx_icon methods to attach an icon to a tag object,
// upon tag object creation. The Replace_xxx_icon methods allows concrete icon creation to be changed
// at runtime.
//
// Related classes: The class DM_icon_factory_mgr provides the Deformable Modeling kernel with
// a singleton DM_icon_factory object. Implementations overriding the DM_icon_factory must
// initialize the DM_icon_factory_mgr. A sample implementation of the DM_icon_factory is the
// class DM_default_icon_factory in the dmicon library.
//
class DM_icon;
class DS_dmod;
class DM_icon_factory_mgr;
class Spatial_abs_hurler;

#include "dcl_ds.h"
#include "mmgr.hxx" // class ACIS_OBJECT

/**
* @file dmicon_factory.hxx
 * @CAA2Level L1
 * @CAA2Usage U2
 * \addtogroup DMAPI
 *
 * @{
 */

#pragma warning(push)
#pragma warning(disable: 4100)

/**
 * The <tt>DM_icon_factory</tt> class provides an abstract interface to the deformable modeling kernel for creating and destroying <tt>DM_icon</tt> objects.
 * <br>
 * <b>Role:</b> For each type of deformable modeling tag object (<tt>DS_dmod</tt>, <tt>DS_load</tt>, or <tt>DS_cstrn</tt>),
 * the <tt>DM_icon_factory</tt> has a <tt>Replace_XXX_icon</tt> and a <tt>Make_XXX_icon</tt> method. The deformable modeling
 * kernel uses the <tt>Make_XXX_icon</tt> methods to attach an icon to a tag object, upon tag object creation.
 * The <tt>Replace_XXX_icon</tt> methods are intended to determine what subsequent calls to the <tt>Make_XXX_icon</tt>
 * method will return. Thus tag object icons can be changed at runtime.
 * <br><br>
 * <b>Related classes:</b> The class <tt>DM_icon_factory_mgr</tt> provides the deformable modeling kernel with
 * a singleton <tt>DM_icon_factory</tt> object. Implementations overriding the <tt>DM_icon_factory</tt> must initialize
 * the <tt>DM_icon_factory_mgr</tt>. A sample implementation of the <tt>DM_icon_factory</tt> is the class
 * <tt>DM_default_icon_factory</tt> in the <tt>dmicon</tt> library.
 */
class DECL_DM DM_icon_factory : public ACIS_OBJECT {

  private:
// Users should always call the Make_copy method,
// instead of operator= or the copy constructor.
    DM_icon_factory&
    operator=                                  // Assignment Operator
    (const DM_icon_factory&);

    DM_icon_factory(const DM_icon_factory&) ;  // Copy constructor

  protected:

    virtual ~DM_icon_factory(){}
/**
 * Default constructor.
 */
    DM_icon_factory() {}

  public:

/**
 * Public destructor.
 */
    virtual void Lose()=0;                   // public destructor
/**
 * Makes a deep copy of this factory.
 * <br><br>
 * @param err_handler
 * error handler.
 */
    virtual DM_icon_factory* Make_copy(      // Deep copy this factory
                                        Spatial_abs_hurler& err_handler)const=0; // error handler

// Replace methods - Not used by the DM kernel.
// These methods provide hooks for a prototype-pattern factory in which Replace_xxx methods
// change what the factory makes.
//
// constraint icons
/**
 * Replace methods change what the factory makes.
 * <br><br>
 * @param err_handler
 * error handler.
 * @param icon
 * icon.
 */

    virtual void Replace_area_cstrn_icon(Spatial_abs_hurler& err_handler,DM_icon*& icon){}
/**
 * Replace methods change what the factory makes.
 * <br><br>
 * @param err_handler
 * error handler.
 * @param icon
 * icon.
 */
    virtual void Replace_pt_cstrn_icon(Spatial_abs_hurler& err_handler,DM_icon*& icon){}
/**
 * Replace methods change what the factory makes.
 * <br><br>
 * @param err_handler
 * error handler.
 * @param icon
 * icon.
 */
    virtual void Replace_crv_cstrn_icon(Spatial_abs_hurler& err_handler,DM_icon*& icon){}
/**
 * Replace methods change what the factory makes.
 * <br><br>
 * @param err_handler
 * error handler.
 * @param icon
 * icon.
 */
    virtual void Replace_link_cstrn_icon(Spatial_abs_hurler& err_handler,DM_icon*& icon){}
//load icons
/**
 * Replace methods change what the factory makes.
 * <br><br>
 * @param err_handler
 * error handler.
 * @param icon
 * icon.
 */
    virtual void Replace_area_load_icon(Spatial_abs_hurler& err_handler,DM_icon*& icon) {}
/**
 * Replace methods change what the factory makes.
 * <br><br>
 * @param err_handler
 * error handler.
 * @param icon
 * icon.
 */
    virtual void Replace_spring_load_icon(Spatial_abs_hurler& err_handler,DM_icon*& icon){}
/**
 * Replace methods change what the factory makes.
 * <br><br>
 * @param err_handler
 * error handler.
 * @param icon
 * icon.
 */
    virtual void Replace_spring_set_icon(Spatial_abs_hurler& err_handler,DM_icon*& icon){}
/**
 * Replace methods change what the factory makes.
 * <br><br>
 * @param err_handler
 * error handler.
 * @param icon
 * icon.
 */
    virtual void Replace_dist_press_icon(Spatial_abs_hurler& err_handler,DM_icon*& icon){}
/**
 * Replace methods change what the factory makes.
 * <br><br>
 * @param err_handler
 * error handler.
 * @param icon
 * icon.
 */
    virtual void Replace_attractor_icon(Spatial_abs_hurler& err_handler,DM_icon*& icon){}
/**
 * Replace methods change what the factory makes.
 * <br><br>
 * @param err_handler
 * error handler.
 * @param icon
 * icon.
 */
    virtual void Replace_vector_load_icon(Spatial_abs_hurler& err_handler,DM_icon*& icon){}
/**
 * Replace methods change what the factory makes.
 * <br><br>
 * @param err_handler
 * error handler.
 * @param icon
 * icon.
 */
    virtual void Replace_point_press_icon(Spatial_abs_hurler& err_handler,DM_icon*& icon){}
/**
 * Replace methods change what the factory makes.
 * <br><br>
 * @param err_handler
 * error handler.
 * @param icon
 * icon.
 */
    virtual void Replace_crv_load_icon(Spatial_abs_hurler& err_handler,DM_icon*& icon){}
/**
 * Replace methods change what the factory makes.
 * <br><br>
 * @param err_handler
 * error handler.
 * @param icon
 * icon.
 */
    virtual void Replace_link_load_icon(Spatial_abs_hurler& err_handler,DM_icon*& icon){}
// dmod icons
/**
 * Replace methods change what the factory makes.
 * <br><br>
 * @param err_handler
 * error handler.
 * @param icon
 * icon.
 */
    virtual void Replace_srf_icon(Spatial_abs_hurler& err_handler,DM_icon*& icon){}
/**
 * Replace methods change what the factory makes.
 * <br><br>
 * @param err_handler
 * error handler.
 * @param icon
 * icon.
 */
    virtual void Replace_crv_icon(Spatial_abs_hurler& err_handler,DM_icon*& icon){}

// Make methods are used by the Deformable Modeling kernel to create
// concrete DM_icon objects.
// constraint icons
/**
 * Makes a new icon of this type.
 * <br><br>
 * @param err_handler
 * error handler.
 */
    virtual DM_icon* Make_area_cstrn_icon(Spatial_abs_hurler& err_handler)const{return 0;}
/**
 * Makes a new icon of this type.
 * <br><br>
 * @param err_handler
 * error handler.
 */
   	virtual DM_icon* Make_pt_cstrn_icon(Spatial_abs_hurler& err_handler)const{return 0;}
/**
 * Makes a new icon of this type.
 * <br><br>
 * @param err_handler
 * error handler.
 */
    virtual DM_icon* Make_crv_cstrn_icon(Spatial_abs_hurler& err_handler)const{return 0;}
/**
 * Makes a new icon of this type.
 * <br><br>
 * @param err_handler
 * error handler.
 */
    virtual DM_icon* Make_link_cstrn_icon(Spatial_abs_hurler& err_handler)const{return 0;}
//load icons
/**
 * Makes a new icon of this type.
 * <br><br>
 * @param err_handler
 * error handler.
 */
   virtual DM_icon* Make_spring_load_icon(Spatial_abs_hurler& err_handler)const{return 0;}
/**
 * Makes a new icon of this type.
 * <br><br>
 * @param err_handler
 * error handler.
 */
    virtual DM_icon* Make_spring_set_icon(Spatial_abs_hurler& err_handler)const{return 0;}
/**
 * Makes a new icon of this type.
 * <br><br>
 * @param err_handler
 * error handler.
 */
    virtual DM_icon* Make_dist_press_icon(Spatial_abs_hurler& err_handler)const{return 0;}
/**
 * Makes a new icon of this type.
 * <br><br>
 * @param err_handler
 * error handler.
 */
    virtual DM_icon* Make_attractor_icon(Spatial_abs_hurler& err_handler)const{return 0;}
/**
 * Makes a new icon of this type.
 * <br><br>
 * @param err_handler
 * error handler.
 */
    virtual DM_icon* Make_vector_load_icon(Spatial_abs_hurler& err_handler)const{return 0;}
/**
 * Makes a new icon of this type.
 * <br><br>
 * @param err_handler
 * error handler.
 */
    virtual DM_icon* Make_point_press_icon(Spatial_abs_hurler& err_handler)const{return 0;}
/**
 * Makes a new icon of this type.
 * <br><br>
 * @param err_handler
 * error handler.
 */
    virtual DM_icon* Make_crv_load_icon(Spatial_abs_hurler& err_handler)const{return 0;}
/**
 * Makes a new icon of this type.
 * <br><br>
 * @param err_handler
 * error handler.
 */
    virtual DM_icon* Make_link_load_icon(Spatial_abs_hurler& err_handler)const{return 0;}
// dmod icons
/**
 * Makes a new icon of this type.
 * <br><br>
 * @param err_handler
 * error handler.
 */
    virtual DM_icon* Make_srf_icon(Spatial_abs_hurler& err_handler)const{return 0;}
/**
 * Makes a new icon of this type.
 * <br><br>
 * @param err_handler
 * error handler.
 */
    virtual DM_icon* Make_crv_icon(Spatial_abs_hurler& err_handler)const{return 0;}

    // area loads
/**
 * Makes an area load icon.
 * <br><br>
 * @param err_handler
 * error handler.
 */
    virtual DM_icon* Make_area_load_icon(Spatial_abs_hurler& err_handler)const {return 0;}
};

#pragma warning(pop)
/** @} */
#endif //DM_ICON_FACTORY_H
