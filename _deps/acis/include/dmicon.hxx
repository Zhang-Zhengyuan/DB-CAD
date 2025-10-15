/*******************************************************************/
/*    Copyright (c) 1989-2020 by Spatial Corp.                     */
/*    All rights reserved.                                         */
/*    Protected by U.S. Patents 5,257,205; 5,351,196; 6,369,815;   */
/*                              5,982,378; 6,462,738; 6,941,251    */
/*    Protected by European Patents 0503642; 69220263.3            */
/*    Protected by Hong Kong Patent 1008101A                       */
/*******************************************************************/
#ifndef DM_ICON_H
#define DM_ICON_H
// Header for the DM_icon class.
//
// Purpose: The DM_icon class provides abstract notify methods to the Deformable Modeling kernel
// for drawing services.
//
// Description:  The contract with the DM_icon class gives the DM kernel 3 responsibilities.
// First, DM_icon objects are owned by tag objects: they are created by the tag object CTOR
// and destroyed by the tag object DTOR. This is supported by the DM_icon Make_copy
// and Lose methods.
// Second, the DM kernel calls the icon Set_owner method when the tag object is fully constructed.
// This allows the icon to initialize itself, and retain knowledge of its owner.
// Third, the DM kernel calls the icon Tagobject_changed method to notify the icon when
// tag object state has changed,  e.g., geometry or behavior. The icon can then e.g. redraw,
// or set a flag for lazy update.
// Additionally, the DM kernel defines a command object interface which allow an application
// to broadcast commands to all or selected icons in a deformable modeling hierarchy.
// These are supported by the DM_icon Draw and Set_state methods.
//
// Related classes: Implementations derived from the DM_icon class must support the
// DM_icon_factory class by overriding the Lose and Make_copy methods. Implementations
// overriding the Draw method, Set_state method, and the Query method must also
// override the DM_icon_draw_args, the DM_icon_cmd_args, and the DM_icon_query_args
// classes respectively.
// Methods take a Spatial_abs_hurler for error handling.
//
#include "dcl_ds.h"

#include "dm_icon_args.hxx"       // icon args base classes       // icon args base classes
#include "spatial_abs_hurler.hxx" // error handler base class  // error handler base class
#include "mmgr.hxx" // class ACIS_OBJECT

/**
* @file dmicon.hxx
 * @CAA2Level L1
 * @CAA2Usage U2
 * \addtogroup DMAPI
 *
 * @{
 */
class DS_dmod;
/**
 * This class provides abstract notify methods to the deformable modeling kernel for drawing services.
 * <br>
 * <b>Role:</b> The contract with the <tt>DM_icon</tt> class gives the deformable modeling kernel three
 * responsibilities. First, <tt>DM_icon</tt> objects are owned by tag objects: they are created by the tag
 * object <tt>CTOR</tt> and destroyed by the tag object <tt>DTOR</tt>. This is supported by the <tt>DM_iconMake_copy</tt>
 * and <tt>Lose</tt> mehods.
 * <br><br>
 * Second, the deformable modeling kernel calls the icon <tt>Set_owner</tt> method when the tag object
 * is fully constructed. This allows the icon to initialize itself, and retain knowledge of its
 * owner. Third, the kernel calls the icon <tt>Tagobject_changed</tt> method to notify the icon when the
 * tag object state has changed, e.g., geometry or behavior. The icon can then redraw, or set a
 * flag for lazy update.
 * <br><br>
 * The deformable modeling kernel also defines a command object interface which allows an
 * application to broadcast commands to all or selected icons in a deformable modeling hierarchy.
 * These are supported by the <tt>DM_iconDraw</tt> and <tt>Set_state</tt> methods.
 * <br><br>
 * <b>Related classes:</b> Implementations derived from the <tt>DM_icon</tt> class must support the
 * <tt>DM_icon_factory</tt> class by overriding the <tt>Lose</tt> and <tt>Make_copy</tt> methods. Implementations overriding
 * the <tt>Draw</tt> method, <tt>Set_state</tt> method, and the <tt>Query</tt> method must also override the <tt>DM_icon_draw_args</tt>,
 * the <tt>DM_icon_cmd_args</tt>, and the <tt>DM_icon_query_args</tt> classes respectively.
 * @see DM_default_icon_factory, DM_icon_draw_args, DM_icon_cmd_args, DM_icon_query_args, DS_dmod
 */

class DECL_DM DM_icon : public ACIS_OBJECT{

  private:
// Users should always call the Make_copy method, then the Set_owner method,
// instead of operator= or the copy constructor.
    DM_icon&
    operator=                                  // Assignment Operator
    (const DM_icon&);

    DM_icon(const DM_icon&) ;                  // Copy constructor

  protected:
/**
 * Public constructor.
 */
    DM_icon(){}
    virtual ~DM_icon(){}

  public:

/**
 * Public destructor.
 * <br><br>
 * <b>Role:</b> To ensure that the same library allocates and deallocates the memory for icons,
 * icons are destructed by calling their <tt>lose</tt> method, which in turn should call the C++ destructor.
 */
    virtual void Lose()=0;                                    // public destructor
/**
 * This routine is called by DM to inform the icon of the <tt>DS_dmod</tt> pointer and tag <tt>ID</tt> of its owning tag object.
 * <br><br>
 * <b>Role:</b> These can then be used in DM interface query functions to
 * allow the icon to determine the state of its owning tag object.  During creation
 * of a new tag object, this routine will not be called until the owning tag object has
 * been completely initialized and is safe to be queried.  This routine should not be called
 * by customer code.
 * <br><br>
 * @param err_handler
 * error handler.
 * @param new_dmod
 * icon owner dmod.
 * @param new_tag
 * icon owner tag ID.
 */
// set owner data; also notifies icon of owner completion, for icon initialization
    virtual void Set_owner(
                            Spatial_abs_hurler& err_handler,  // error handler
                            DS_dmod* new_dmod,int new_tag)=0; // icon owner pair
/**
 * Icons owner query.
 * <br><br>
 * @param dmod
 * icon owner dmod.
 * @param tag
 * icon owner tag ID.
 */

    virtual void Get_owner(                                   // icon owner query
                            DS_dmod*& dmod, int& tag)const=0; // icon owner pair

/**
 * This routine is called by DM to notify the icon that the state of the owning tag object has changed.
 * <br><br>
 * <b>Role:</b> This routine should not be called by customer code.
 * <br><br>
 * @param err_handler
 * error handler.
 */
    virtual void Tag_object_changed(                          // notification of owning tag object state change
                                     Spatial_abs_hurler& err_handler)=0;  // error handler

/**
 * Tells icon to draw itself using arguments found in <tt>draw_obj</tt>.
 * <br><br>
 * @param err_handler
 * error handler.
 * @param draw_obj
 * draw-command object.
 */
    virtual void Draw(                                        // draw this icon
                       Spatial_abs_hurler& err_handler,                   // error handler
                       const DM_icon_draw_args& draw_obj)const=0;      // draw-command object

/**
 * Tells icon to execute the command encapsulated in <tt>cmd_obj</tt> on itself, if applicable.
 * <br><br>
 * @param err_handler
 * error handler.
 * @param cmd_obj
 * command object.
 */
   virtual void Set_state(                                   // execute command
                            Spatial_abs_hurler& err_handler,              // error handler
                            const DM_icon_cmd_args& cmd_obj)=0;       // command object

/**
 * Queries and returns the result in <tt>query_obj</tt>.
 * <br><br>
 * @param err_handler
 * error handler.
 * @param query_obj
 * query object, concrete type determines query to be performed.
 */
    virtual void Query(                                       // query & return result in args
                        Spatial_abs_hurler& err_handler,                  // error handler
                        DM_icon_query_args& query_obj)const=0;          // query object

/**
 * Clone method.
 * <br><br>
 * <b>Role:</b> Performs a deep copy.
 * <br><br>
 * @param err_handler
 * error handler.
 */
    virtual DM_icon* Make_copy(                               // clone method
                                Spatial_abs_hurler& err_handler)const=0;  // error handler

};
/** @} */
#endif /*DM_ICON_H*/
