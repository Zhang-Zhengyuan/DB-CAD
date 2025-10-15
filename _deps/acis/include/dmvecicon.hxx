/*******************************************************************/
/*    Copyright (c) 1989-2020 by Spatial Corp.                     */
/*    All rights reserved.                                         */
/*    Protected by U.S. Patents 5,257,205; 5,351,196; 6,369,815;   */
/*                              5,982,378; 6,462,738; 6,941,251    */
/*    Protected by European Patents 0503642; 69220263.3            */
/*    Protected by Hong Kong Patent 1008101A                       */
/*******************************************************************/
#ifndef DM_VEC_ICON_H
#define DM_VEC_ICON_H
//
// Header for the DM_vec_icon class.
//
// Purpose: The DM_vec_icon class is used for drawing SPAvector load tag
// objects in deformable modeling.
//
// Description: The DM_vec_icon is part of the dmicon library. This
// icon a polyline to depict the current direction of a SPAvector load.
//
// Related classes: The DM_def_icon_cmd_args and the DM_def_icon_query_args
// encapsulate many of the methods inhereted from the DM_default_icon interface.
// Thus the Set_state and Query methods can be used to perform casting, when
// different libraries of icons are mixed.
// The DM_def_icon_draw_args and the DM_def_icon_cmd_args can be broadcast to icons
// using the Deformable Modeling interface methods, such as DM_draw_all_icons.
//

#include "dm_default_icon.hxx"     // Class DM_default_icon     // Class DM_default_icon

/**
* @file dmvecicon.hxx
 * @CAA2Level L1
 * @CAA2Usage U2
 * \addtogroup DMICONAPI
 *
 * @{
 */

class DS_dmod;
class DM_default_icon_factory;
class Spatial_abs_hurler;
class DM_dbl_array;

/**
 * This class is used for drawing vector load tag objects in deformable modeling.
 * <br>
 * <b>Role:</b> The <tt>DM_vec_icon</tt> is part of the <tt>dmicon</tt> library. This icon uses a polyline
 * to depict the current direction of a vector load.
 * <br><br>
 * <b>Related classes</b>
 * <br>
 * The <tt>DM_def_icon_cmd_args</tt> and the <tt>DM_def_icon_query_args</tt> encapsulate
 * many of the methods inherited from the <tt>DM_default_icon</tt> interface. Thus the <tt>Set_state</tt> and
 * <tt>Query</tt> methods can be used to perform casting, when different libraries of icons are mixed.
 * <br><br>
 * The <tt>DM_def_icon_draw_args</tt> and the <tt>DM_def_icon_cmd_args</tt> can be broadcast to icons using
 * the deformable modeling interface methods, such as <tt>DM_draw_all_icons</tt>.
 */

 class DECL_DM_ICON DM_vec_icon : public DM_default_icon { // veci

 private:
// Users should not call assignment operator or copy constructor -
// use Make_copy and Set_owner
   DM_vec_icon&
   operator=                                       // Assignment Operator
   (const DM_vec_icon&);

//
   double m_base_pt[3];
   double m_end_pt[3];
// color state
   double m_R, m_G, m_B;
//

 protected:
   /**
    * Copy Contructor.
    * <br><br>
    * <b>Role:</b> Should not call copy contructor; call <tt>Make_copy</tt> and <tt>Set_owner</tt> instead.
    */
   DM_vec_icon(const DM_vec_icon&) ;  // should not call copy constructor
                                      // call Make_copy & Set_owner

   virtual ~DM_vec_icon();            // should call Lose

 public:

/**
 * Public constructor.
 */

   DM_vec_icon();                                   // public constructor
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
};


 /** @} */
#endif /*DM_VEC_ICON_H*/

