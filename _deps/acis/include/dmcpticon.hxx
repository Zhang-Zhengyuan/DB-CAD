/*******************************************************************/
/*    Copyright (c) 1989-2020 by Spatial Corp.                     */
/*    All rights reserved.                                         */
/*    Protected by U.S. Patents 5,257,205; 5,351,196; 6,369,815;   */
/*                              5,982,378; 6,462,738; 6,941,251    */
/*    Protected by European Patents 0503642; 69220263.3            */
/*    Protected by Hong Kong Patent 1008101A                       */
/*******************************************************************/
#ifndef DM_CPT_ICON_H
#define DM_CPT_ICON_H

//
// Header for the DM_cpt_icon class.
//
// Purpose: The DM_cpt_icon draws an array of control points. It is
// used in the dmicon library as a "helper" icon, owned by other icons.
//
// Description: The DM_cpt_icon draws an array of control points. It is used in the
// dmicon library, to depict spline surface and curve control points. As an owned icon,
// it provides an example of icon aggregation.
//
//<br><b>Related classes:</b> The DM_srf_icon, DM_crv_icon, and the ADM_srf_icon each own DM_cpt_icons.
//
#include "dm_default_icon.hxx"          // base class DM_default_icon          // base class DM_default_icon

/**
* @file dmcpticon.hxx
 * @CAA2Level L1
 * @CAA2Usage U1
 * \addtogroup DMICONAPI
 *
 * @{
 */

class DS_dmod;
class Spatial_abs_hurler;
class DM_dbl_array;

/**
 * The <tt>DM_cpt_icon</tt> draws an array of control points.
 *<br>
 * <b>Role:</b> The <tt>DM_cpt_icon</tt> draws an array of control points. It is used in the dmicon
 * library as a <tt>"helper"</tt> icon, owned by other icons. It is used to depict spline surface
 * and curve control points. As an owned icon, it provides an example of icon aggregation.
 * <br><br><b>Related classes:</b> The <tt>DM_srf_icon</tt>, <tt>DM_crv_icon</tt>, and the <tt>ADM_srf_icon</tt> each own <tt>DM_cpt_icons</tt>.
 *<br><br>
 * @see DM_crv_icon, DM_srf_icon
 */
 class DECL_DM_ICON DM_cpt_icon : public DM_default_icon {

 private:

// Users should not call assignment operator or copy constructor -
// use Make_copy and Set_owner
   DM_cpt_icon&
   operator=                                    // Assignment Operator
   (const DM_cpt_icon&);

   int m_num_upts;
//
// color state
   double m_R, m_G, m_B;

 protected:

   DM_cpt_icon(const DM_cpt_icon&) ;  // should not call copy constructor
                                      // call Make_copy & Set_owner

   virtual ~DM_cpt_icon();            // should call Lose

 public:

/**
 * Default constructor.
 */
   DM_cpt_icon();                                   // public constructor
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
#endif /*DM_CPT_ICON_H*/

