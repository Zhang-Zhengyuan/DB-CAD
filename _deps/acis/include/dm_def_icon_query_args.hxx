/*******************************************************************/
/*    Copyright (c) 1989-2020 by Spatial Corp.                     */
/*    All rights reserved.                                         */
/*    Protected by U.S. Patents 5,257,205; 5,351,196; 6,369,815;   */
/*                              5,982,378; 6,462,738; 6,941,251    */
/*    Protected by European Patents 0503642; 69220263.3            */
/*    Protected by Hong Kong Patent 1008101A                       */
/*******************************************************************/
#ifndef DM_ICON_QUERY_ARGS_H
#define DM_ICON_QUERY_ARGS_H

#include "dcl_dmicon.h"          // DECL_ macros          // DECL_ macros
#include "dm_def_icon_args.hxx"  // base class DM_def_query_args  // base class DM_def_query_args

/**
* @file dm_def_icon_query_args.hxx
 * @CAA2Level L1
 * @CAA2Usage U1
 * \addtogroup DMICONAPI
 *
 * @{
 */
//
// Header for the DM_icq_draw_option class.
//
// Purpose: The DM_icq_draw_option class is a DM_def_icon_query_args that
// encapsulates a command to query DM_default_icon draw draw options,
// e.g., which tag objects to draw.
//
// Description: The DM_icq_draw_option class is a concrete DM_def_icon_query_args, providing an
// exmample of command encapsulation for the dmicon library.
//
// <b>Related classes:</b>  The base class DM_def_icon_query_args derive from DM_icon_query_args.
// The DM_icon_cmd_args have a cast interface, allowing icons to decide whether to
// process or ignore the command; this allows an application to mix icons from different libraries.
// The class DM_rend_options is used by the DM_default_icons to encapsulate draw options.
//
/**
 * This class is a <tt>DM_def_icon_query_args</tt> that encapsulates a command to query <tt>DM_default_icon</tt> draw options.
 *<br>
 * <b>Role:</b> The <tt>DM_icq_draw_option</tt> class is a concrete <tt>DM_def_icon_query_args</tt>, providing
 * an example of command encapsulation for the dmicon library.
 * <br><br> <b>Related classes:</b>  The base class <tt>DM_def_icon_query_args</tt> derives from <tt>DM_icon_query_args</tt>.
 * The <tt>DM_icon_cmd_args</tt> have a cast interface, allowing icons to decide whether to process or
 * ignore the command; this allows an application to mix icons from different libraries. The class
 * <tt>DM_rend_options</tt> is used by the <tt>DM_default_icons</tt> to encapsulate draw options.
 * <br><br>
 * @see DM_def_icon_query_args, DM_icon_query_args, DM_icon_cmd_args, DM_rend_options, DM_default_icons
 */

class DECL_DM_ICON DM_icq_draw_option : public DM_def_icon_query_args {
  private:
    int m_option;
  public:
/**
 * Public constructor.
 */
    DM_icq_draw_option();                        // constructor
/**
 * Returns stored data; used after processing a query.
 */
    int Get_option() const;                      // return stored data - used after processing a query
/**
 * Query this icon.
 *<br><br>
 * @param d_icon
 * icon.
 */
 	virtual void Execute(const DM_default_icon* d_icon);// query this icon
};

//
// Header for the DM_icq_draw_grid class.
//
// Purpose: The DM_icq_draw_grid class is a DM_def_icon_query_args that
// encapsulates a command to query DM_default_icon draw grid density.
//
// Description: The DM_icq_draw_grid class is a concrete DM_def_icon_query_args, providing an
// exmample of command encapsulation for the dmicon library.
//
// <b>Related classes:</b>  The base class DM_def_icon_query_args derive from DM_icon_query_args.
// The DM_icon_cmd_args have a cast interface, allowing icons to decide whether to
// process or ignore the command; this allows an application to mix icons from different libraries.
//
/**
 * This class is a <tt>DM_def_icon_query_args</tt> that encapsulates a command to query <tt>DM_default_icon</tt> draw grid density.
 *<br>
 * <b>Role:</b> The <tt>DM_icq_draw_grid</tt> class is a concrete <tt>DM_def_icon_query_args</tt>,
 * providing an example of command encapsulation for the dmicon library.
 * <br><br><b>Related classes:</b>  The base class <tt>DM_def_icon_query_args</tt> derives from <tt>DM_icon_query_args</tt>.
 * The <tt>DM_icon_cmd_args</tt> have a cast interface, allowing icons to decide whether to process or
 * ignore the command; this allows an application to mix icons from different libraries.
 * <br><br>
 * @see DM_def_icon_query_args, DM_icon_query_args
 */

class DECL_DM_ICON DM_icq_draw_grid : public DM_def_icon_query_args {
  private:
    int m_grid[2];
  public:
/**
 * Public constructor.
 */
    DM_icq_draw_grid();                          // constructor
/**
 * Returns stored data - used after processing a query.  Pointer is to member data and becomes invalid when object is destructed.
 */
  	const int* Get_grid() const;                 // return stored data - used after processing a query
/**
 * Query this icon.
 *<br><br>
 * @param d_icon
 * icon.
 */
    virtual void Execute(const DM_default_icon* d_icon);// query this icon
};


 /** @} */
#endif /*DM_ICON_QUERY_ARGS_H*/
