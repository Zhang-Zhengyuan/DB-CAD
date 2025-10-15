/*******************************************************************/
/*    Copyright (c) 1989-2020 by Spatial Corp.                     */
/*    All rights reserved.                                         */
/*    Protected by U.S. Patents 5,257,205; 5,351,196; 6,369,815;   */
/*                              5,982,378; 6,462,738; 6,941,251    */
/*    Protected by European Patents 0503642; 69220263.3            */
/*    Protected by Hong Kong Patent 1008101A                       */
/*******************************************************************/
#ifndef ADM_srf_icon3_H
#define ADM_srf_icon3_H
// Header for the ADM_srf_icon3 class.
//
// Purpose: The ADM_srf_icon3 class is used for drawing deformable surface tag
// objects in ACIS deformable modeling.
//
// Description: The ADM_srf_icon3 is part of the admicon library. This
// icon updates the associated ACIS FACE INDEXED_MESH to depict the current
// state of the DS_dsurf. It allows ACIS drawing to actually draw the mesh.
// As a consequence, the deformable surface appears as if it were
// an ACIS FACE, using, for example, ACIS colors and textures.
// Control points and element boundaries can also be displayed
//
// Related classes: The DM_def_icon_cmd_args and the DM_def_icon_query_args
// encapsulate many of the methods inhereted from the DM_default_icon interface.
// Thus the Set_state and Query methods can be used to perform casting, when
// different libraries of icons are mixed.
// The DM_def_icon_draw_args and the DM_def_icon_cmd_args can be broadcast to icons
// using the Deformable Modeling interface methods, such as
// ATTRIB_DM2ACIS::Draw_all_icons.
//

/**
* @file admsrficon3.hxx
 * @CAA2Level L1
 * @CAA2Usage U1
 * \addtogroup ADMHOOPS
 *
 * @{
 */

#include "dcl_admhoops.h"            // DECL macros            // DECL macros
#include "dm_default_icon.hxx"  // base class DM__default_icon  // base class DM__default_icon

class DS_dmod;
class DM_grd_icon;
class DM_cpt_icon;
class DM_default_icon_factory;
class Spatial_abs_hurler;
class DM_dbl_array;
/**
 * This class is used for drawing deformable surface tag objects in ACIS deformable modeling.
 * <br>
 * <b>Role</b>: The ADM_srf_icon3 is part of the admicon library. This icon updates
 * the associated ACIS FACE INDEXED_MESH to depict the current state of the DS_dsurf.
 * It allows ACIS drawing to actually draw the mesh. As a consequence, the
 * deformable surface appears as if it were an ACIS FACE, using, for example, ACIS
 * colors and textures. Control points and element boundaries can also be displayed.
 * <br>
 * <b>Related classes:<b> The DM_def_icon_cmd_args and the DM_def_icon_query_args
 * encapsulate many of the methods inherited from the DM_default_icon interface.
 * Thus the Set_state and Query methods can be used to perform casting, when
 * different libraries of icons are mixed. The DM_def_icon_draw_args and the
 * DM_def_icon_cmd_args can be broadcast to icons using the deformable modeling
 * interface methods, such as ATTRIB_DM2ACIS::Draw_all_icons.
 * @see DM_cpt_icon, DM_grd_icon
 */
class DECL_ADMHOOPS ADM_srf_icon3 : public DM_default_icon {

 private:

// Users should not call assignment operator or copy constructor -
// use Make_copy, and then Set_owner
   ADM_srf_icon3&
   operator=                                       // Assignment Operator
   (const ADM_srf_icon3&);


   int m_image_dim;

// Control point icon
   DM_cpt_icon* m_cpt_icon;
// Element grid icon
   DM_grd_icon* m_grd_icon;

   int m_grid_u, m_grid_v;

// surface color;
   double m_mesh_R, m_mesh_G, m_mesh_B;
// element bdy color
   double m_elem_R, m_elem_G, m_elem_B;
// cpt color
   double m_cpt_R, m_cpt_G, m_cpt_B;
//
   int m_mesh_state;

 protected:

   ADM_srf_icon3(const ADM_srf_icon3&) ;               // Copy constructor

   virtual ~ADM_srf_icon3();

 public:
/**
 * Default constructor.
 */
   ADM_srf_icon3();                                  // public constructor
/**
 * Draw this icon.
 * @param err_handler
 * error handler
 * @param cmd
 * draw-command object
 */
   virtual void Draw(                               // draw this icon
                  Spatial_abs_hurler& err_handler,              // error handler
                  const DM_icon_draw_args& cmd)const;   // draw-command object
/**
 * Notification of a state change to the owning tag object.
 * @param err_handler
 * error handler
 */
   virtual void Tag_object_changed(                 // notification of owning tag object state change
                             Spatial_abs_hurler& err_handler);  // error handler
/**
 * Clone method.
 * @param err_handler
 * error handler
 */
   virtual DM_icon* Make_copy(                      // clone method
                      Spatial_abs_hurler& err_handler)const;    // error handler
/**
 * Update cached data.
 */
   virtual void Validate();                         // update cached data
/**
 * Recalculate the mesh (facets).
 */
   virtual void Remesh();                           // recalculate mesh (facets)
/**
 * Query discretization grid density.
 * @param density
 * density
 */
   virtual void Get_grid(int density[2])const;              // query discretization grid density
/**
 * Query color data.
 * @param data
 * color data
 */
   virtual void Get_colors(DM_dbl_array& data) const;    // query color data
/**
 * Set color data.
 * @param color
 * colors
 * @param num
 * number of colors
 */
   virtual void Set_colors(const double* color,int num);      // set color data

// Override these to propagate to owned icons.
/**
 * Mark cached data as bad.
 */
   virtual void Invalidate();                       // mark cahced data bad

/**
 * Set owner data.
 * <br>
 * <b>Role</b>: Also notifies icon of owner completion, for icon initialization.
 * @param err_handler
 * error handler
 * @param new_dmod
 * icon owner ID
 * @param new_tag
 * icon owner ID
 */
                                                    // set owner data; also notifies icon
   virtual void Set_owner(                          // of owner completion, for icon initialization
                   Spatial_abs_hurler& err_handler,             // error handler
                   DS_dmod* new_dmod,int new_tag);  // icon owner pair
/**
 * Set discretization grid density.
 * @param density
 * density
 */
   virtual void Set_grid(const int density[2]);             // set discretization grid density
/**
 * Set icon draw width.
 * @param width
 * width
 */
   virtual void Set_icon_width(double width);             // set icon draw width
/**
 * Set draw options.
 * @param option
 * option
 */
   virtual void Set_draw_option(int option);               // set draw options




};
/** @} */
#endif //ADM_srf_icon3_H

