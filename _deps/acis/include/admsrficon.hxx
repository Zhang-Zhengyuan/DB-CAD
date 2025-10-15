/*******************************************************************/
/*    Copyright (c) 1989-2020 by Spatial Corp.                     */
/*    All rights reserved.                                         */
/*    Protected by U.S. Patents 5,257,205; 5,351,196; 6,369,815;   */
/*                              5,982,378; 6,462,738; 6,941,251    */
/*    Protected by European Patents 0503642; 69220263.3            */
/*    Protected by Hong Kong Patent 1008101A                       */
/*******************************************************************/
#ifndef ADM_SRF_ICON_H
#define ADM_SRF_ICON_H
// Header for the ADM_srf_icon class.
//
// Purpose: The ADM_srf_icon class is used for drawing deformable surface tag
// objects in ACIS deformable modeling.
//
// Description: The ADM_srf_icon is part of the admicon library. This
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
* @file admsrficon.hxx
 * @CAA2Level L1
 * @CAA2Usage U1
 * \addtogroup ACISDM
 *
 * @{
 */

#include "dcl_admicon.h"       // DECL_ macros       // DECL_ macros
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
 * <b>Role:</b> The <tt>ADM_srf_icon</tt> is part of the <tt>admicon</tt> library. This icon updates
 * the associated ACIS <tt>FACE</tt> <tt>INDEXED_MESH</tt> to depict the current state of the <tt>DS_dsurf</tt>.
 * It allows ACIS drawing to actually draw the mesh. As a consequence, the
 * deformable surface appears as if it were an ACIS <tt>FACE</tt>, using, for example, ACIS
 * colors and textures. Control points and element boundaries can also be displayed.
 * <br><br>
 * <b>Related classes:</b> The <tt>DM_def_icon_cmd_args</tt> and the <tt>DM_def_icon_query_args</tt>
 * encapsulate many of the methods inherited from the <tt>DM_default_icon</tt> interface.
 * Thus the <tt>Set_state</tt> and <tt>Query</tt> methods can be used to perform casting, when
 * different libraries of icons are mixed. The <tt>DM_def_icon_draw_args</tt> and the
 * <tt>DM_def_icon_cmd_args</tt> can be broadcast to icons using the deformable modeling
 * interface methods, such as <tt>ATTRIB_DM2ACIS::Draw_all_icons</tt>.
 * @see DM_cpt_icon, DM_grd_icon
 */
class DECL_ADM_ICON ADM_srf_icon : public DM_default_icon {

 private:
/**
 * Users should not call assignment operator or copy constructor - use Make_copy, and then Set_owner
 */
   ADM_srf_icon& operator= (const ADM_srf_icon&);
 /**
  *.
  */
   int m_image_dim;
   /**
    * Control point icon
    */
   DM_cpt_icon* m_cpt_icon;
  /**
   * Element grid icon
   */
   DM_grd_icon* m_grd_icon;
/**
 *.
 */
   int m_grid_u, m_grid_v;
 /**
  * surface color;
  */
   double m_mesh_R, m_mesh_G, m_mesh_B;
 /**
  *element bdy color
  */
   double m_elem_R, m_elem_G, m_elem_B;
/**
 * control point color
 */
   double m_cpt_R, m_cpt_G, m_cpt_B;
/**
 *.
 */
   int m_mesh_state;

 protected:
/**
 *.
 */
   ADM_srf_icon(const ADM_srf_icon&) ;  
/**
 *.
 */
   virtual ~ADM_srf_icon();

 public:
/**
 * Default constructor.
 */
   ADM_srf_icon();
/**
 * Draws this icon.
 * <br><br>
 * @param err_handler
 * error handler.
 * @param cmd
 * draw-command object.
 */
   virtual void Draw(
                  Spatial_abs_hurler& err_handler, 
                  const DM_icon_draw_args& cmd)const;
/**
 * Notification of a state change to the owning tag object.
 * <br><br>
 * @param err_handler
 * error handler.
 */
   virtual void Tag_object_changed( Spatial_abs_hurler& err_handler);
/**
 * Clones method.
 * <br><br>
 * @param err_handler
 * error handler.
 */
   virtual DM_icon* Make_copy( Spatial_abs_hurler& err_handler)const; 
/**
 * Updates cached data.
 */
   virtual void Validate();  
/**
 * Recalculates the mesh (facets).
 */
   virtual void Remesh();  
/**
 * Queries discretization grid density.
 * <br><br>
 * @param density
 * density.
 */
   virtual void Get_grid(int density[2])const;  
/**
 * Queries color data.
 * <br><br>
 * @param data
 * color data.
 */
   virtual void Get_colors(DM_dbl_array& data) const;
/**
 * Sets color data.
 * <br><br>
 * @param color
 * colors.
 * @param num
 * number of colors.
 */
   virtual void Set_colors(const double* color,int num);
/**
 * Marks cached data as bad.
 */
   virtual void Invalidate();

/**
 * Sets owner data.
 * <br><br>
 * <b>Role:</b> Also notifies icon of owner completion, for icon initialization.
 * <br><br>
 * @param err_handler
 * error handler.
 * @param new_dmod
 * icon owner ID.
 * @param new_tag
 * icon owner ID.
 */
   virtual void Set_owner( Spatial_abs_hurler& err_handler,
                   DS_dmod* new_dmod,int new_tag);
/**
 * Sets discretization grid density.
 * <br><br>
 * @param density
 * density.
 */
   virtual void Set_grid(const int density[2]);
/**
 * Sets icon draw width.
 * <br><br>
 * @param width
 * width.
 */
   virtual void Set_icon_width(double width);
/**
 * Sets draw options.
 * <br><br>
 * @param option
 * option.
 */
   virtual void Set_draw_option(int option); 
};

 /** @} */
#endif //ADM_SRF_ICON_H

