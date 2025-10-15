/*******************************************************************/
/*    Copyright (c) 1989-2020 by Spatial Corp.                     */
/*    All rights reserved.                                         */
/*    Protected by U.S. Patents 5,257,205; 5,351,196; 6,369,815;   */
/*                              5,982,378; 6,462,738; 6,941,251    */
/*    Protected by European Patents 0503642; 69220263.3            */
/*    Protected by Hong Kong Patent 1008101A                       */
/*******************************************************************/
#ifndef DM_ALD_ICON_H
#define DM_ALD_ICON_H
//
// Header for the DM_ald_icon class.
//
// Purpose: The DM_ald_icon class is used for drawing area load tag
// objects in deformable modeling.
//
// Description: The DM_ald_icon is part of the dmicon library. This
// icon updates polylines to depict the current state of an
// area load. The DM_ald_icon is a light-weight icon, displaying the image of
// the area load domain boundary on the deformable model surface.
//
// Related classes: The DM_def_icon_cmd_args and the DM_def_icon_query_args
// encapsulate many of the methods inhereted from the DM_default_icon interface.
// Thus the Set_state and Query methods can be used to perform casting, when
// different libraries of icons are mixed.
// The DM_def_icon_draw_args and the DM_def_icon_cmd_args can be broadcast to icons
// using the Deformable Modeling interface methods, such as DM_draw_all_icons.
//
#include "dm_default_icon.hxx"   // base class DM_default_icon   // base class DM_default_icon
#include "dm_dbl_array.hxx"  // class DM_dbl_array  // class DM_dbl_array
/**
* @file dmaldicon.hxx
 * @CAA2Level L1
 * @CAA2Usage U1
 * \addtogroup DMICONAPI
 *
 * @{
 */

class DS_dmod;
class DM_rendering_target;
class DM_default_icon_factory;
class DM_dbl_array;

/**
 * This class is used for drawing area load tag objects in deformable modeling.
 * <br>
 * <b>Role:</b> The <tt>DM_ald_icon</tt> is part of the dmicon library. This icon depicts
 * an area load on a deformable surface. The method <tt>DM_add_area_C0_load</tt> is used
 * to create an area load.
 * @see DM_dbl_array
 */
class DECL_DM_ICON DM_ald_icon : public DM_default_icon {

 private:
// Users should not call assignment operator or copy constructor -
// use Make_copy and Set_owner
   const DM_ald_icon&
   operator=                                       // Assignment Operator
   (const DM_ald_icon&);

//
   DM_dbl_array m_xyzs;
   int m_grid;
// color state
   double m_R, m_G, m_B;
//

 protected:
   DM_ald_icon(const DM_ald_icon&) ;// should not call copy constructor
                                    // call Make_copy & Set_owner

   virtual ~DM_ald_icon();          // should call Lose

 public:
   /**
	* @nodoc
    */
   DM_ald_icon();                                   // public constructor
   /**
	* @nodoc
    */
   virtual void Draw(                               // draw this icon
                  Spatial_abs_hurler&,              // error handler
                  const DM_icon_draw_args&)const;   // draw-command object
   /**
	* @nodoc
    */
   virtual void Tag_object_changed(                 // notification of owning tag object state change
                             Spatial_abs_hurler&);  // error handler
   /**
	* @nodoc
    */
   virtual DM_icon* Make_copy(                      // clone method
                      Spatial_abs_hurler&)const;    // error handler
   /**
	* @nodoc
    */
   virtual void Validate();                         // update cached data
   /**
	* @nodoc
    */
   virtual void Get_colors(DM_dbl_array&) const;    // query color data
   /**
	* @nodoc
    */
   virtual void Set_colors(const double*,int);      // set color data
   /**
	* @nodoc
    */
   virtual void Set_grid(const int[2]);             // set discretization grid density
   /**
	* @nodoc
    */
   virtual void Get_grid(int[2])const;              // query discretization grid density
};


 /** @} */
#endif /*DM_ALD_ICON_H*/

