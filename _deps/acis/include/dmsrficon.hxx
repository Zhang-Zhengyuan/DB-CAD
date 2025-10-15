/*******************************************************************/
/*    Copyright (c) 1989-2020 by Spatial Corp.                     */
/*    All rights reserved.                                         */
/*    Protected by U.S. Patents 5,257,205; 5,351,196; 6,369,815;   */
/*                              5,982,378; 6,462,738; 6,941,251    */
/*    Protected by European Patents 0503642; 69220263.3            */
/*    Protected by Hong Kong Patent 1008101A                       */
/*******************************************************************/
#ifndef DM_SRF_ICON_H
#define DM_SRF_ICON_H

// Header for the DM_srf_icon class.
//
// Purpose: The DM_srf_icon class is used for drawing deformable surface tag
// objects in standalone deformable modeling.
//
// Description: The DM_srf_icon is part of the dmicon library. This
// icon updates a grid of polylines to depict the current
// state of a deformable surface. Control points and element boundaries can also be displayed
//
// <br><b>Related classes:</b> The DM_def_icon_cmd_args and the DM_def_icon_query_args
// encapsulate many of the methods inhereted from the DM_default_icon interface.
// Thus the Set_state and Query methods can be used to perform casting, when
// different libraries of icons are mixed.
// The DM_def_icon_draw_args and the DM_def_icon_cmd_args can be broadcast to icons
// using the Deformable Modeling interface methods, such as DM_draw_all_icons.
// A DM_srf_icon owns a DM_grd_icon and a DM_cpt_icon, providing an example of icon aggregation.
//
#include "dm_default_icon.hxx"       // base class DM_default_icon       // base class DM_default_icon

/**
* @file dmsrficon.hxx
 * @CAA2Level L1
 * @CAA2Usage U1
 * \addtogroup DMICONAPI
 *
 * @{
 */

class DS_dmod;
class DM_grd_icon;
class DM_cpt_icon;
class DM_default_icon_factory;
class Spatial_abs_hurler;
class DM_dbl_array;

/**
 * This class is used for drawing deformable surface tag objects in standalone deformable modeling.
 *<br>
 * <b>Role:</b> The <tt>DM_srf_icon</tt> is part of the dmicon library. This icon updates a grid of
 * polylines to depict the current state of a deformable surface. Control points and element
 * boundaries can also be displayed
 * <br><br><b>Related classes:</b> <tt>DM_ica_draw_option</tt> and <tt>DM_icq_draw_option</tt> encapsulate set and query
 * draw option commands using the <tt>DM_rend_options</tt> class. These derive from the
 * <tt>DM_def_icon_cmd_args</tt> and <tt>DM_def_icon_query_args</tt> base classes.
 * <br><br>
 * The <tt>DM_def_icon_cmd_args</tt> and the <tt>DM_def_icon_query_args</tt> encapsulate many of the methods
 * inherited from the <tt>DM_default_icon</tt> interface. Thus the <tt>Set_state</tt> and <tt>Query</tt> methods can be
 * used to perform casting, when different libraries of icons are mixed.
 *<br><br>
 * @see DM_cpt_icon, DM_grd_icon
 */

 class DECL_DM_ICON DM_srf_icon : public DM_default_icon {

 private:

// Users should not call assignment operator or copy constructor -
// use Make_copy and Set_owner
   DM_srf_icon&
   operator=                                       // Assignment Operator
   (const DM_srf_icon&);

   DM_srf_icon(const DM_srf_icon&) ;  // should not call copy constructor
                                      // call Make_copy & Set_owner

   int m_image_dim;


// Control point icon
   DM_cpt_icon* m_cpt_icon;
// Element grid icon
   DM_grd_icon* m_grd_icon;
// Surface grid
   DM_grd_icon* m_mesh_icon;

// surface color;
   double m_mesh_R, m_mesh_G, m_mesh_B;
// element bdy color
   double m_elem_R, m_elem_G, m_elem_B;
// cpt color
   double m_cpt_R, m_cpt_G, m_cpt_B;
//
 protected:

   virtual ~DM_srf_icon();            // should call Lose

 public:

/**
 * Public constructor.
 */

   DM_srf_icon();                                   // public constructor
/**
 * Draws this icon.
 *<br><br>
 * @param err_handler
 * error handler.
 * @param draw_obj
 * draw-command object.
 */
   virtual void Draw(                               // draw this icon
                  Spatial_abs_hurler& err_handler,              // error handler
                  const DM_icon_draw_args& draw_obj)const;   // draw-command object

/**
 * Notification of owning tag object state change.
 *<br><br>
 * @param err_handler
 * error handler.
 */
   virtual void Tag_object_changed(                 // notification of owning tag object state change
                             Spatial_abs_hurler& err_handler);  // error handler

/**
 * Clone method.
 *<br><br>
 * @param err_handler
 * error handler.
 */
   virtual DM_icon* Make_copy(                      // clone method
                      Spatial_abs_hurler& err_handler)const;    // error handler
/**
 * Updates cached data.
 */
   virtual void Validate();                         // update cached data

// Override these to propagate to owned icons.
/**
 * Notification of owner completion, for icon initialization.
 *<br><br>
 * @param err_handler
 * error handler.
 * @param new_dmod
 * icon owner dmod.
 * @param new_tag
 * icon owner tag ID.
 */
   virtual void Set_owner(                          // of owner completion, for icon initialization
                   Spatial_abs_hurler& err_handler,             // error handler
                   DS_dmod* new_dmod,int new_tag);  // icon owner pair
/**
 * Sets discretization grid density.
 *<br><br>
 * @param density
 * density.
 */
   virtual void Set_grid(const int density[2]);             // set discretization grid density
/**
 * Queries discretization grid density.
 *<br><br>
 * @param density
 * density.
 */
   virtual void Get_grid(int density[2])const;              // query discretization grid density
/**
 * Sets icon draw width.
 *<br><br>
 * @param width
 * width.
 */
   virtual void Set_icon_width(double width);             // set icon draw width
/**
 * Sets draw options.
 *<br><br>
 * @param option
 * option.
 */

   virtual void Set_draw_option(int option);               // set draw options
/**
 * Marks cached data bad.
 */
   virtual void Invalidate();                       // mark cahced data bad
/**
 * Queries color data.
 *<br><br>
 * @param arr_data
 * data array.
 */
   virtual void Get_colors(DM_dbl_array& arr_data) const;    // query color data
/**
 * Sets color data.
 *<br><br>
 * @param col_data
 * color data (r,g,b).
 * @param arr_size
 * array size, number of doubles.
 */
   virtual void Set_colors(const double* col_data,int arr_size);      // set color data

};

 /** @} */
#endif /*DM_SRF_ICON_H*/

