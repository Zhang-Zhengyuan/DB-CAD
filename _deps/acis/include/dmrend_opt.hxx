/*******************************************************************/
/*    Copyright (c) 1989-2020 by Spatial Corp.                     */
/*    All rights reserved.                                         */
/*    Protected by U.S. Patents 5,257,205; 5,351,196; 6,369,815;   */
/*                              5,982,378; 6,462,738; 6,941,251    */
/*    Protected by European Patents 0503642; 69220263.3            */
/*    Protected by Hong Kong Patent 1008101A                       */
/*******************************************************************/
#ifndef DM_REND_OPT_H
#define DM_REND_OPT_H
//
// Header for the DM_rend_options class.
//
// Purpose: The DM_rend_options class is used internally by the dmicon library
// for encapsulating draw options.
//
// Description: The DM_rend_options is part of the dmicon library, and is only used
// internally by the dmicon library. DM_rend_options are constructed from a bitfield
// defined by the DM_DRAW_XXX macros defined in the file dmapinum.hxx.
//
// Related classes: DM_ica_draw_option and DM_icq_draw_option encapsulate set and query draw
// option commands using the DM_rend_options class. These derive from the DM_def_icon_cmd_args
// and DM_def_icon_query_args base classes.
// The DM_def_icon_cmd_args and the DM_def_icon_query_args
// encapsulate many of the methods inhereted from the DM_default_icon interface.
// Thus the Set_state and Query methods can be used to perform casting, when
// different libraries of icons are mixed.
// The DM_def_icon_draw_args and the DM_def_icon_cmd_args can be broadcast to icons
// using the Deformable Modeling interface methods, such as DM_draw_all_icons.
//
#include "dcl_dmicon.h"        // DECL_ macros        // DECL_ macros

/**
* @file dmrend_opt.hxx
 * @CAA2Level L1
 * @CAA2Usage U1
 * \addtogroup DMICONAPI
 *
 * @{
 */

/**
 * This class is used internally by the dmicon library for encapsulating draw options.
 * <b>Role</b>: The DM_rend_options is part of the dmicon library, and is only used internally by
 * the dmicon library. DM_rend_options are constructed from a bitfield defined by the
 * DM_DRAW_XXX macros defined in the file dmapinum.hxx.
 * <br> Related classes: DM_ica_draw_option and DM_icq_draw_option encapsulate set and query
 * draw option commands using the DM_rend_options class. These derive from the
 * DM_def_icon_cmd_args and DM_def_icon_query_args base classes.
 * <br> The DM_def_icon_cmd_args and the DM_def_icon_query_args encapsulate many of the methods
 * inherited from the DM_default_icon interface. Thus the Set_state and Query methods can be used
 * to perform casting, when different libraries of icons are mixed.
 * <br> The DM_def_icon_draw_args and the DM_def_icon_cmd_args can be broadcast to icons using
 * the deformable modeling interface methods, such as DM_draw_all_icons.
 */

  class DECL_DM_ICON DM_rend_options {

  private:

    int rop_draw_cpts;
    int rop_draw_seams;
    int rop_draw_cstrns;
    int rop_draw_loads;
    int rop_draw_curve_comb;
    int rop_draw_torque_comb;
    int rop_draw_log_of_comb;
    int rop_draw_elems;
    int rop_draw_seg_bnds;
    int rop_draw_cstrn_norms;
    int rop_draw_cstrn_combs;
    int rop_draw_gauss_pts;

  public:

/**
 * Query draw control points option.
 */

    int Draw_cpts( )const;           // query draw control points option
/**
 * Query draw seams option.
 */

 	int Draw_seams( )const;          // query draw seams option
/**
 * Query draw constraints option.
 */

  	int Draw_cstrns( )const;         // query draw constraints option
/**
 * Query draw loads option.
 */

    int Draw_loads( )const;          // query draw loads option
/**
 * Query draw curvature comb option.
 */

    int Draw_curve_comb( )const;     // query draw curvature comb option
/**
 * Query draw element boundary option.
 */

    int Draw_elems( )const;          // query draw element boundary option

/**
 * Set draw control points option on.
 */

    void Set_cpts_on( );             // set draw control points option on
/**
 * Set draw seams option on.
 */

	void Set_seams_on( );            // set draw seams option on
/**
 * Set draw constraints option on.
 */

	void Set_cstrns_on( );           // set draw constraints option on
/**
 * Set draw loads option on.
 */

	void Set_loads_on( );            // set draw loads option on
/**
 * Set draw curvature comb option on.
 */

	void Set_curve_comb_on( );       // set draw curvature comb option on
/**
 * Set draw element boundary option on.
 */

	void Set_elems_on( );            // set draw element boundary option on
/**
 * Set draw control points option off.
 */

    void Set_cpts_off( );            // set draw control points option off
/**
 * Set draw seams option off.
 */

    void Set_seams_off( );           // set draw seams option off
/**
 * Set draw constraints option off.
 */

    void Set_cstrns_off( );          // set draw constraints option off
/**
 * Set draw loads option off.
 */

    void Set_loads_off( );           // set draw loads option off
/**
 * Set draw curvature comb option off.
 */

    void Set_curve_comb_off( );      // set draw curvature comb option off
/**
 * Set draw element boundary option off.
 */

    void Set_elems_off( );           // set draw element boundary option off

/**
 * Public constructor.
 */

    DM_rend_options();
/**
 * Copy constructor; call Make_copy and Set_owner instead.
 * @param bitfield
 * bitfield
 */

    DM_rend_options(int bitfield);


};
 /** @} */
#endif /*DM_REND_OPT_H*/
