/*******************************************************************/
/*    Copyright (c) 1989-2020 by Spatial Corp.                     */
/*    All rights reserved.                                         */
/*    Protected by U.S. Patents 5,257,205; 5,351,196; 6,369,815;   */
/*                              5,982,378; 6,462,738; 6,941,251    */
/*    Protected by European Patents 0503642; 69220263.3            */
/*    Protected by Hong Kong Patent 1008101A                       */
/*******************************************************************/
#ifndef DM_GRD_ICON_H
#define DM_GRD_ICON_H

#include "dm_default_icon.hxx"         // base class class DM_default_icon         // base class class DM_default_icon
#include "dm_dbl_array.hxx"   // container class DM_dbl_array   // container class DM_dbl_array

/**
* @file dmgridicon.hxx
 * @CAA2Level L1
 * @CAA2Usage U1
 * \addtogroup DMICONAPI
 *
 * @{
 */
//
// Header for the DM_grd_icon class.
//
// Purpose: The DM_grd_icon draws and manages a rectangular grid of lines. It is
// used in the dmicon library as a "helper" icon, owned by other icons.
//
// Description: The DM_grd_icon draws a rectangular grid of lines. It is used in the
// dmicon library, to depict spline surface element boundaries, and to depict a surface
// as an adjustable density grid of polylines. As an owned icon, it provides an example
// of icon aggregation.
//
// <br><b>Related classes:</b> The DM_srf_icon and the ADM_srf_icon both own DM_grd_icons.
//
class DS_dmod;
class Spatial_abs_hurler;
class DM_dbl_array;

/**
 * This class draws and manages a rectangular grid of lines.
 *<br>
 * <b>Role:</b> The <tt>DM_grd_icon</tt> draws a rectangular grid of lines. It is used in the dmicon
 * library as a <tt>"helper"</tt> icon, to depict spline surface element boundaries, and to depict
 * a surface as an adjustable density grid of polylines. As an owned icon, it provides an
 * example of icon aggregation.
 * <br><br><b>Related classes:</b> The <tt>DM_srf_icon</tt> and the <tt>ADM_srf_icon</tt> both own <tt>DM_grd_icons</tt>.
 * <br><br>
 * @see DM_dbl_array, DM_srf_icon
 */

 class DECL_DM_ICON DM_grd_icon : public DM_default_icon {

 private:
// Users should not call assignment operator or copy constructor -
// use Make_copy and Set_owner
   DM_grd_icon&
   operator=                                       // assignment operator
   (const DM_grd_icon&);

// points per element; only use nu for 1-d
   int m_nu_gpe, m_nv_gpe;             // number of grids lines per element
   int m_grid_u, m_grid_v;             // polyline count used when display mode != 0
   int m_nu_nelems, m_nv_nelems;       // number of elements; polyline count used for display mode 0
   int m_nu_ppe, m_nv_ppe;             // number of points per element, along grid line
   int m_image_dim;                    // usually 3
   int m_domain_dim;                   // usually 2
   int m_display_mode;                 // 0==> use m_n*_nelems, m_n*_gpe
                                       // 1==> use m_grid_u, m_grid_v
//
   DM_dbl_array m_u_lines, m_v_lines;  // storange for xyz data
//
// color state
   double m_R, m_G, m_B;
//

 protected:

   DM_grd_icon(const DM_grd_icon&) ;   // should not call copy constructor
                                       // call Make_copy & Set_owner

   virtual ~DM_grd_icon();             // should call Lose

 public:

/**
 * Default constructor.
 */
   DM_grd_icon();                                   // public constructor
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
 * array size, number of doubles.
 */
   virtual void Set_colors(const double* col_data,int arr_size);      // set color data
/**
 * Sets density for display mode <tt>0</tt>.
 * <br><br>
 * @param u_density
 * u density.
 * @param v_density
 * v density.
 */
   void Set_npts_per_elem(                          // set density for display mode 0
                        int u_density,                        // u density
                        int v_density);                       // v density
/**
 * Queries density for display mode <tt>0</tt>.
 * <br><br>
 * @param u_density
 * u density.
 * @param v_density
 * v density.
 */
   void Get_npts_per_elem(                          // query density for display mode 0
                        int& u_density,                       // u density
                        int& v_density)const;                 // v density
/**
 *Sets display mode.
 *<br><br>
 *<b>Role:</b>
 *<br><br>
 *<table border="0" width="33%">
 *<tr>
 *<td width="20%" align="right">==0</td>
 *<td width="80%" align="right">use auxiliary "element" data</td>
 *</tr>
 *<tr>
 *<td width="20%" align="right">!=0</td>
 *<td width="80%" align="right">use grid data</td>
 *</tr>
 *</table>
 * <br><br>
 * @param mode
 * mode.
 */

   void Set_display_mode(                           // set display mode; 0==> use auxilliary "element" data
                        int mode);                       // !=0 ==> use grid data
};
 /** @} */
#endif /*DM_GRD_ICON_H*/
