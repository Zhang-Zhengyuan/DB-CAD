/*******************************************************************/
/*    Copyright (c) 1989-2020 by Spatial Corp.                     */
/*    All rights reserved.                                         */
/*    Protected by U.S. Patents 5,257,205; 5,351,196; 6,369,815;   */
/*                              5,982,378; 6,462,738; 6,941,251    */
/*    Protected by European Patents 0503642; 69220263.3            */
/*    Protected by Hong Kong Patent 1008101A                       */
/*******************************************************************/
#ifndef DM_ICON_CMD_ARGS_H
#define DM_ICON_CMD_ARGS_H

#include "dcl_dmicon.h"          // DECL_ macros          // DECL_ macros
#include "dm_def_icon_args.hxx"  // base class DM_def_query_args  // base class DM_def_query_args

/**
* @file dm_def_icon_cmd_args.hxx
 * @CAA2Level L1
 * @CAA2Usage U1
 * \addtogroup DMICONAPI
 *
 * @{
 */

class DM_icon;

// Header for the DM_ica_grid class.
//
// Purpose: The DM_ica_grid class is a DM_def_icon_cmd_args that encapsulates a command
// to set a DM_default_icon draw grid density.
//
// Description: The DM_ica_grid class is a concrete DM_def_icon_cmd_args, providing an
// exmample of command encapsulation for the dmicon library.
//
// Related classes: The base class DM_def_icon_cmd_args derive from DM_icon_cmd_args.
// The DM_icon_cmd_args have a cast interface, allowing icons to decide whether to
// process or ignore the command; this allows an application to mix icons from different libraries.
//
/**
 * This class is a <tt>DM_def_icon_cmd_args</tt> that encapsulates a command to set a <tt>DM_default_icon</tt> draw grid density.
 *<br>
 * <b>Role:</b> The <tt>DM_ica_grid</tt> class is a concrete <tt>DM_def_icon_cmd_args</tt>, providing an
 * example of command encapsulation for the dmicon library.
 * <br><br>
 * <b>Related classes:</b> The base class <tt>DM_def_icon_cmd_args</tt> derive from <tt>DM_icon_cmd_args</tt>.
 * The <tt>DM_icon_cmd_args</tt> have a cast interface, allowing icons to decide whether to process
 * or ignore the command. This allows an application to mix icons from different libraries.
 * <br><br>
 * @see DM_def_icon_cmd_args, DM_icon_cmd_args
 */

class DECL_DM_ICON DM_ica_grid : public DM_def_icon_cmd_args {
  private:
    int m_grid[2];
  public:
/**
 * Constructor for <tt>1D</tt> or <tt>2D</tt> isotropic.
 *<br><br>
 * @param grid
 * density.
 */
    DM_ica_grid (int grid=5);                     // constructor for 1-d, or 2-d isotropic
/**
 * Constructor for <tt>2D</tt> anisotropic.
 *<br><br>
 * @param grid1
 * density.
 * @param grid2
 * density.
 */
    DM_ica_grid (int grid1,int grid2);            // constructor for 2-d anisotropic

/**
 * Process this icon.
 *<br><br>
 * @param d_icon
 * icon.
 */
     virtual void Execute(DM_default_icon* d_icon)const ; // process this icon
};


// Header for the DM_ica_width class.
//
// Purpose: The DM_ica_width class is a DM_def_icon_cmd_args that encapsulates a command
// to set a DM_default_icon draw width.
//
// Description: The DM_ica_grid class is a concrete DM_def_icon_cmd_args, providing an
// exmample of command encapsulation for the dmicon library.
//
// Related classes: The base class DM_def_icon_cmd_args derive from <tt>DM_icon_cmd_args</tt>.
// The <tt>DM_icon_cmd_args</tt> have a cast interface, allowing icons to decide whether to
// process or ignore the command; this allows an application to mix icons from different libraries.
//
/**
 * This class is a <tt>DM_def_icon_cmd_args</tt> that encapsulates a command to set a <tt>DM_default_icon</tt> draw width.
 *<br>
 * <b>Role:</b> The <tt>DM_ica_grid</tt> class is a concrete <tt>DM_def_icon_cmd_args</tt>, providing
 * an example of command encapsulation for the dmicon library.
 * <br><br>
 * <b>Related classes:</b> The base class <tt>DM_def_icon_cmd_args</tt> derive from <tt>DM_icon_cmd_args</tt>.
 * The <tt>DM_icon_cmd_args</tt> have a cast interface, allowing icons to decide whether to process
 * or ignore the command. This allows an application to mix icons from different libraries.
 * <br><br>
 * @see DM_def_icon_cmd_args, DM_icon_cmd_args
 */

class DECL_DM_ICON DM_ica_width : public DM_def_icon_cmd_args {
  private:
    double m_width;
  public:
/**
 * Public constructor.
 *<br><br>
 * @param width
 * width.
 */

 	DM_ica_width(double width=1.0);                // constructor
/**
 * Process this icon.
 *<br><br>
 * @param d_icon
 * icon
 */
     virtual void Execute(DM_default_icon* d_icon)const ;  // process this icon
};

// Header for the DM_ica_width class.
//
// Purpose: The DM_ica_draw_option class is a DM_def_icon_cmd_args that encapsulates a command
// to set a DM_default_icon draw draw options, e.g., which tag objects to draw.
//
// Description: The DM_ica_draw_option class is a concrete DM_def_icon_cmd_args, providing an
// exmample of command encapsulation for the dmicon library.
//
// Related classes: The base class DM_def_icon_cmd_args derive from DM_icon_cmd_args.
// The DM_icon_cmd_args have a cast interface, allowing icons to decide whether to
// process or ignore the command; this allows an application to mix icons from different libraries.
// The class DM_rend_options is used by the DM_default_icons to encapsulate draw options.
//
/**
 * This class is a <tt>DM_def_icon_cmd_args</tt> that encapsulates a command to set a <tt>DM_default_icon</tt> draw options.
 *<br>
 * <b>Role:</b>  The <tt>DM_ica_draw_option</tt> class is a concrete <tt>DM_def_icon_cmd_args</tt>,
 * providing an example of command encapsulation for the dmicon library.
 * <br><br>
 * <b>Related classes:</b> The base class <tt>DM_def_icon_cmd_args</tt> derive from <tt>DM_icon_cmd_args</tt>.
 * The <tt>DM_icon_cmd_args</tt> have a cast interface, allowing icons to decide whether to process
 * or ignore the command; this allows an application to mix icons from different libraries.
 * The class <tt>DM_rend_options</tt> is used by the <tt>DM_default_icon</tt>s to encapsulate draw options.
 * <br><br>
 * @see DM_def_icon_cmd_args, DM_icon_cmd_args
 */

class DECL_DM_ICON DM_ica_draw_option : public DM_def_icon_cmd_args {
  private:
    int m_option;
  public:
/**
 * Constructor, defaults to null option.
 *<br><br>
 * @param opt
 * option.
 */

    DM_ica_draw_option(int opt=0);                // constructor, defaults to null option
/**
 * Process this icon.
 *<br><br>
 * @param d_icon
 * icon.
 */
    virtual void Execute(DM_default_icon* d_icon)const ; // process this icon
};

// Header for the DM_ica_on_off class.
//
// Purpose: The DM_ica_on_off class is a DM_def_icon_cmd_args that encapsulates a command
// to switch an icon on or off, i.e, draw or don't draw.
//
// Description: The DM_ica_on_off class is a concrete DM_def_icon_cmd_args, providing an
// exmample of command encapsulation for the dmicon library.
//
// Related classes: The base class DM_def_icon_cmd_args derive from DM_icon_cmd_args.
// The DM_icon_cmd_args have a cast interface, allowing icons to decide whether to
// process or ignore the command; this allows an application to mix icons from different libraries.
//
/**
 * This class is a <tt>DM_def_icon_cmd_args</tt> that encapsulates a command to switch an icon on or off (draw or don't draw).
 *<br>
 * <b>Role:</b> The <tt>DM_ica_on_off</tt> class is a concrete <tt>DM_def_icon_cmd_args</tt>, providing an
 * example of command encapsulation for the dmicon library.
 * <br><br>
 * <b>Related classes:</b> The base class <tt>DM_def_icon_cmd_args</tt> derive from <tt>DM_icon_cmd_args</tt>.
 * The <tt>DM_icon_cmd_args</tt> have a cast interface, allowing icons to decide whether to process
 * or ignore the command. This allows an application to mix icons from different libraries.
 * <br><br>
 * @see DM_def_icon_cmd_args, DM_icon_cmd_args
 */

class DECL_DM_ICON DM_ica_on_off : public DM_def_icon_cmd_args {
  private:
    int m_on;
  public:
/**
 * Constructor, defaults to off.
 *<br><br>
 * @param on_off
 * on or off.
 */

    DM_ica_on_off(int on_off=0);                  // constructor, defaults to off
/**
 * Process this icon.
 *<br><br>
 * @param d_icon
 * icon.
 */
     virtual void Execute(DM_default_icon* d_icon)const ; // process this icon
};

// ********************************************************************
// Class:
//    DM_ica_change_notification
//
// Access:
//    do not document
// ********************************************************************
// tbrv
/**
 * @nodoc
 */

class DECL_DM_ICON DM_ica_change_notification : public DM_def_icon_cmd_args {
  public:
    DM_ica_change_notification();
    virtual void Execute(DM_default_icon*)const ;
};


 /** @} */
#endif /*DM_ICON_CMD_ARGS_H*/
