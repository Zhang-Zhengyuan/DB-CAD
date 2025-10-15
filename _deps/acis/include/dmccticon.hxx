/*******************************************************************/
/*    Copyright (c) 1989-2020 by Spatial Corp.                     */
/*    All rights reserved.                                         */
/*    Protected by U.S. Patents 5,257,205; 5,351,196; 6,369,815;   */
/*                              5,982,378; 6,462,738; 6,941,251    */
/*    Protected by European Patents 0503642; 69220263.3            */
/*    Protected by Hong Kong Patent 1008101A                       */
/*******************************************************************/
#ifndef DM_CCT_ICON_H
#define DM_CCT_ICON_H

// Header for the DM_cct_icon class.
//
// Purpose: The DM_cct_icon class is used for drawing curve constraint
// tag objects in deformable modeling.
//
// Description: The DM_cct_icon is part of the dmicon library. This
// icon updates a polyline to depict the current state of a curve constraint.
//
// Related classes: The DM_def_icon_cmd_args and the DM_def_icon_query_args
// encapsulate many of the methods inhereted from the DM_default_icon interface.
// Thus the Set_state and Query methods can be used to perform casting, when
// different libraries of icons are mixed.
// The DM_def_icon_draw_args and the DM_def_icon_cmd_args can be broadcast to icons
// using the Deformable Modeling interface methods, such as DM_draw_all_icons.
//
#include "dm_default_icon.hxx"        // base class DM_default_icon        // base class DM_default_icon
#include "dm_dbl_array.hxx"  // class DM_dbl_array  // class DM_dbl_array

/**
* @file dmccticon.hxx
 * @CAA2Level L1
 * @CAA2Usage U2
 * \addtogroup DMICONAPI
 *
 * @{
 */

class DS_dmod;
class DM_default_icon_factory;
class DM_dbl_array;

/**
 * This class is used for drawing curve constraint tag objects in deformable modeling.
 * <br>
 * <b>Role:</b> The <tt>DM_cct_icon</tt> is part of the <tt>dmicon</tt> library. This icon updates a polyline
 * to depict the current state of a curve constraint.
 * <br><br>
 * <b>Related classes</b>
 * <br>
 * The <tt>DM_def_icon_cmd_args</tt> and the <tt>DM_def_icon_query_args</tt> encapsulate
 * many of the methods inherited from the <tt>DM_default_icon</tt> interface. Thus the <tt>Set_state</tt> and
 * <tt>Query</tt> methods can be used to perform casting, when different libraries of icons are mixed.
 * The <tt>DM_def_icon_draw_args</tt> and the <tt>DM_def_icon_cmd_args</tt> can be broadcast to icons using the
 * deformable modeling interface methods, such as <tt>DM_draw_all_icons</tt>.
 * @see DM_dbl_array
 */

 class DECL_DM_ICON DM_cct_icon : public DM_default_icon {

 private:
// Users should not call assignment operator or copy constructor -
// use Make_copy and Set_owner
   DM_cct_icon&
   operator=                                       // Assignment Operator
   (const DM_cct_icon&);

   int m_image_dim;                // 3
// geometry for display
   int m_npts;                     // How many points.
   DM_dbl_array m_tgt_pts;         // Target points, for loads.
   DM_dbl_array m_param_pts;       // Parameter values.
   DM_dbl_array m_x_pts;           // The points.
   DM_dbl_array m_dx_pts;          // First derivs.
   DM_dbl_array m_ddx_pts;         // Second derivs.
// color state
   double  m_off_R, m_off_G, m_off_B;
   double  m_C0_R, m_C0_G, m_C0_B;
   double  m_C1_R, m_C1_G, m_C1_B;
   double  m_C2_R, m_C2_G, m_C2_B;
//
//
//

 protected:

   /**
    * Copy Contructor.
    * <br><br>
    * <b>Role:</b> Should not call copy contructor; call <tt>Make_copy</tt> and <tt>Set_owner</tt> instead.
    */
   DM_cct_icon(const DM_cct_icon&) ;// should not call copy constructor
                                    // call Make_copy & Set_owner

   virtual ~DM_cct_icon();          // should call Lose

 public:

/**
 * Public constructor.
 */

   DM_cct_icon();                                   // public constructor

/**
 * Draws this icon.
 * <br><br>
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
 * <br><br>
 * @param err_handler
 * error handler.
 */

   virtual void Tag_object_changed(                 // notification of owning tag object state change
                             Spatial_abs_hurler& err_handler);  // error handler

/**
 * Clone method.
 * <br><br>
 * @param err_handler
 * error handler.
 */

   virtual DM_icon* Make_copy(                      // clone method
                      Spatial_abs_hurler& err_handler)const;    // error handler
/**
 * Updates cached data.
 */

   virtual void Validate();                         // update cached data
/**
 * Sets discretization grid density.
 * <br><br>
 * @param density
 * density.
 */

   virtual void Set_grid(const int density[2]);             // set discretization grid density
/**
 * Queries discretization grid density.
 * <br><br>
 * @param density
 * density.
 */

   virtual void Get_grid(int density[2])const;              // query discretization grid density

/**
 * Queries color data.
 * <br><br>
 * @param arr_data
 * data array.
 */

   virtual void Get_colors(DM_dbl_array& arr_data) const;    // query color data
/**
 * Sets color data.
 * <br><br>
 * @param col_data
 * color data (r,g,b).
 * @param arr_size
 * array size,number of doubles.
 */

   virtual void Set_colors(const double* col_data,int arr_size);      // set color data

};
 /** @} */
#endif /*DM_CCT_ICON_H*/

