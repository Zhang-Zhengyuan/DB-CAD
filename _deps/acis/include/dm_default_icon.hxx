/*******************************************************************/
/*    Copyright (c) 1989-2020 by Spatial Corp.                     */
/*    All rights reserved.                                         */
/*    Protected by U.S. Patents 5,257,205; 5,351,196; 6,369,815;   */
/*                              5,982,378; 6,462,738; 6,941,251    */
/*    Protected by European Patents 0503642; 69220263.3            */
/*    Protected by Hong Kong Patent 1008101A                       */
/*******************************************************************/
#ifndef DM_DEFAULT_ICON_H
#define DM_DEFAULT_ICON_H
//
// Header for the DM_default_icon class.
//
// Purpose: The DM_default_icon class is used for drawing DM tag objects.
// The DM_default_icon class provides an example of a common
// command/query interface base class, which derives from the DM_icon
// pure interface class.
//
// Description: The dmicon library provides concrete examples of icons
// for all DM tag objects. The icons in the dmicon libarary derive from
// the DM_default_icon base class.
//
// Related classes: Examples of concrete DM_default_icon objects are found
// in the dmicon library.
// A DM tag object only creates a concrete icon via the DM_icon_factory.
// Many DM_default_icon methods are encapsulated in either
// DM_def_cmd_args, or DM_def_query_args, e.g., Set_icon_width or Get_icon_width.
// Base class (DM_icon) methods take a Spatial_abs_hurler for error handling.
#include "dmicon.hxx"     // base class DM_icon     // base class DM_icon
#include "dcl_dmicon.h"            // DECL macros            // DECL macros

/**
* @file dm_default_icon.hxx
 * @CAA2Level L1
 * @CAA2Usage U2
 * \addtogroup DMICONAPI
 *
 * @{
 */

class Spatial_abs_hurler;
class DM_dbl_array;

/**
 * Base class for the icon objects in the dmicon and admicon libraries.
 *<br>
 * <b>Role:</b> This class provides an interface for common queries and commands shared
 * by the example icons in the dmicon library.  By deriving all of the icons in the library
 * from a common base class, icon-specific implementations of the commands and queries
 * supported by the library can be called through a virtual function mechanism.  In addition,
 * the default icons cache the geometry data needed for drawing so that expensive geometry
 * queries only need to be performed when the tag objects actually change, rather than every
 * time a redraw is requested.
 * <br><br>A <tt>Tag_object_changed</tt> notification sets a validation flag, marking the cache as
 * invalid. When a draw request is received, the validation flag is checked, and if
 * marked invalid, the geometry cache is refreshed before drawing (via tag object queries).
 * <br><br> Because of data caching, clients must request redraws after executing certain
 * <tt>DM_*</tt> interface functions which change the geometry of the tag objects.<br>
 * These are the "add tag <tt>object"</tt> commands, <tt>"set tag object
 * behavior"</tt> commands (e.g., toggle a constraint), <tt>solve</tt>, and the <tt>"change domain"</tt> commands:
 * <tt>split domain</tt>, <tt>extrapolate</tt>, and <tt>change degree</tt>.
 *<br><br>
 * @see DS_dmod
 */
 class DECL_DM_ICON DM_default_icon : public DM_icon {

  private:
// Users should not call assignment operator or copy constructor - use Make_copy
    DM_default_icon&
    operator=                                 // Assignment Operator
    (const DM_default_icon&);


  protected:
/**
 * @nodoc
 */

    DM_default_icon(const DM_default_icon&) ; // Copy constructor

// Users should not create these!
    DM_default_icon();
// Call Lose to delte.
    virtual ~DM_default_icon(){}

    int m_tag;
    DS_dmod* m_dmod;
    int m_valid;

    int m_on;
    double m_icon_width;
    int m_draw_option;

  public:

/**
 * Tells icon to execute the command encapsulated in <tt>cmd_obj</tt> on itself, if applicable.
 *<br><br>
 * @param err_handler
 * error handler.
 * @param cmd_obj
 * command object.
 */
    virtual void Set_state(                                   // execute command
                            Spatial_abs_hurler& err_handler,              // error handler
                            const DM_icon_cmd_args& cmd_obj);         // command object

/**
 * Queries and returns the result in <tt>query_obj</tt>.
 *<br><br>
 * @param err_handler
 * error handler.
 * @param query_obj
 * query object.
 */
    virtual void Query(                                       // query & return result in args
                        Spatial_abs_hurler& err_handler,                  // error handler
                        DM_icon_query_args& query_obj)const;            // query object
/**
 * This routine is called by <tt>DM</tt> to inform the icon of the <tt>DS_dmod</tt> pointer and tag <tt>ID</tt>
 * of its owning tag object.  These can then be used in <tt>DM</tt> interface query functions to
 * allow the icon to determine the state of its owning tag object.  During creation
 * of a new tag object, this routine will not be called until the owning tag object has
 * been completely initialized and is safe to be queried.  This routine should not be called
 * by customer code.
 *<br><br>
 * @param err_handler
 * error handler.
 * @param new_dmod
 * icon owner dmod.
 * @param new_tag
 * icon owner tag ID.
 */
    virtual void Set_owner(                                   // notification of owner completion
                            Spatial_abs_hurler& err_handler,              // error handler
                            DS_dmod* new_dmod,int new_tag);   // icon owner pair
/**
 * Icon owner query.
 * <br><br>
 * @param dmod
 * icon owner dmod.
 * @param tag
 * icon owner tag ID.
 */
	virtual void Get_owner(                                   // icon owner query
                            DS_dmod*& dmod, int& tag)const;   // icon owner pair

                                            // Note that the following methods do not take a Spatial_abs_hurler
                                            // since they use DM_sys_error throwing.
                                            // They should be wrapped in DM_EXCEPTION blocks.
/**
 * Updates cached data.
 */
    virtual void Validate()=0;              // update cached data
/**
 * Marks cached data bad.
 */
 	virtual void Invalidate();              // mark cached data bad
/**
 * Public destructor.  To ensure that the same library allocates and deallocates the memory for icons,
 * icons are destructed by calling their lose method, which in turn should call the C++ destructor.
 */
    virtual void Lose();                    // public destructor

// Query/Setstate pairs
/**
 * Returns the current draw option filter settings.
 */
    virtual int Get_draw_option() const;   // query draw options
/**
 * Sets filter to draw seams, constraints and/or loads.
 *<br><br>
 * @param option
 * option.
 */
    virtual void Set_draw_option(int option );     // set draw options
/**
 * Returns the icon width.
 */
    virtual double Get_icon_width() const; // query icon draw width
/**
 * Sets the icon draw width.
 *<br><br>
 * @param width
 * width.
 */

   virtual void Set_icon_width(double width);   // set icon draw width

/**
 * Returns the current on or off state.
 */
   virtual int Get_on_off() const;        // query on-off flag
/**
 * Sets the on or off state.
 *<br><br>
 * @param on_off
 * state.
 */
    virtual void Set_on_off(int on_off);          // set on-off flag
/**
 * Queries discretization grid density.
 *<br><br>
 * @param den
 * density.
 */

    virtual void Get_grid(int den[2])const;    // query discretization grid density
/**
 * Sets discretization grid density, Does nothing by default.
 *<br><br>
 * @param density
 * density.
 */

    virtual void Set_grid(const int density[2]);   // set discretization grid density

/**
 * Returns <tt>TRUE</tt> if the cache has not been invalidated.
 */
    virtual int Is_valid() const;          // query data cache state

/**
 * Queries color data.  Does nothing by default.
 * <br><br>
 * @param data_array
 * data array.
 */
     virtual void Get_colors(DM_dbl_array& data_array) const; // query color data
/**
 * Sets color data.
 * <br><br>
 * @param col_data
 * color data (r,g,b).
 * @param narr
 * array size,number of doubles.
 */
    virtual void Set_colors(const double* col_data,int narr);   // set color data

};



 /** @} */
#endif //DM_DEFAULT_ICON_H
