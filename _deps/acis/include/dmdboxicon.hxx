/*******************************************************************/
/*    Copyright (c) 1989-2020 by Spatial Corp.                     */
/*    All rights reserved.                                         */
/*    Protected by U.S. Patents 5,257,205; 5,351,196; 6,369,815;   */
/*                              5,982,378; 6,462,738; 6,941,251    */
/*    Protected by European Patents 0503642; 69220263.3            */
/*    Protected by Hong Kong Patent 1008101A                       */
/*******************************************************************/
#ifndef DM_DBX_ICON_H
#define DM_DBX_ICON_H

//
// Header for the DM_dbx_icon class.
//
// Purpose: The DM_dbx_icon draws and manages an outline corresponding to
// the image of a SPAparameter sub-rectangle on a deformable surface. It is
// used in the dmicon library as a "helper" icon, owned by other icons.
//
// Description: The DM_dbx_icon is used in the
// dmicon library, to depict the image of a SPAparameter sub-rectangle on a
// deformable surface. As an owned icon, it provides an example of icon aggregation.
//
// <b>Related classes:</b> The DM_dpr_icon and the DM_act_icon both own DM_dbx_icons.
//
#include "dm_default_icon.hxx"               // base class DM_default_icon               // base class DM_default_icon
#include "dm_dbl_array.hxx"         // container class DM_dbl_array         // container class DM_dbl_array

/**
* @file dmdboxicon.hxx
 * @CAA2Level L1
 * @CAA2Usage U1
 * \addtogroup DMICONAPI
 *
 * @{
 */

class DS_dmod;
class DM_dbl_array;

/**
 * The <tt>DM_dbx_icon</tt> draws and manages an outline corresponding to the image of a parameter sub-rectangle on a deformable surface.
 *<br>
 * <b>Role:</b> The <tt>DM_dbx_icon</tt> is used in the dmicon library as a <tt>"helper"</tt> icon, to depict the
 * image of a parameter sub-rectangle on a deformable surface. As an owned icon, it provides
 * an example of icon aggregation.
 * <br><br><b>Related classes:</b> The <tt>DM_dpr_icon</tt> and the <tt>DM_act_icon</tt> both own <tt>DM_dbx_icons</tt>.
 *<br><br>
 * @see DM_dbl_array, DM_act_icon, DM_dpr_icon
 */

 class DECL_DM_ICON DM_dbx_icon : public DM_default_icon { // dbxi

 private:

// Users should not call assignment operator or copy constructor -
// use Make_copy and Set_owner
   DM_dbx_icon&
   operator=                                       // Assignment Operator
   (const DM_dbx_icon&);
//
   DM_dbl_array m_points;
   double m_uvmin[2];
   double m_uvmax[2];
   int m_num_upts;
   int m_num_vpts;
   int m_num_xyzpts;
// color state
   double m_R, m_G, m_B;
//

 protected:

   DM_dbx_icon(const DM_dbx_icon&) ;  // should not call copy constructor
                                      // call Make_copy & Set_owner
   virtual ~DM_dbx_icon();            // should call Lose

 public:

/**
 * Default constructor.
 */
   DM_dbx_icon();                                   // public constructor

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
/**
 * Sets parameter space maximum and minimum.
 *<br><br>
 * @param max
 * maximum.
 * @param min
 * minimum.
 */

   void Set_max_min(const double* max ,const double* min);   // set SPAparameter space max-min
};


 /** @} */
#endif /*DM_DBX_ICON_H*/

