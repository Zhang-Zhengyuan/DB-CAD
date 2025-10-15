/*******************************************************************/
/*    Copyright (c) 1989-2020 by Spatial Corp.                     */
/*    All rights reserved.                                         */
/*    Protected by U.S. Patents 5,257,205; 5,351,196; 6,369,815;   */
/*                              5,982,378; 6,462,738; 6,941,251    */
/*    Protected by European Patents 0503642; 69220263.3            */
/*    Protected by Hong Kong Patent 1008101A                       */
/*******************************************************************/

#ifndef DM_DEF_ICON_CMD_ARGS_H
#define DM_DEF_ICON_CMD_ARGS_H

#include "dcl_dmicon.h"
#include "dm_icon_args.hxx"

/**
* @file dm_def_icon_args.hxx
 * @CAA2Level L1
 * @CAA2Usage U2
 * \addtogroup DMICONAPI
 *
 * @{
 */

class ADM_giicon_draw_args;
class VM_icon_draw_args;
class DM_default_icon;
class ADMHOOPS_icon_draw_args;

//
// Header for the DM_def_icon_draw_args class.
//
// Purpose: The DM_def_icon_draw_args class provides a common interface
// between Spatial rendering libraries. The interface consists of
// casting methods allowing a concrete DM_draw_engine to decide whether to
// process or ignore a draw request. Any draw engine library interfacing
// to the dmicon library must override DM_def_icon_draw_args.
//
// Description:  Concrete DM_def_icon_draw_args must provide drawing data
// when received by the DM_draw_engine::Draw method. For example, which view
// to draw into. Concrete DM_def_icon_draw_args provide draw data which
// passes through the DM_icon::Draw method to a concrete DM_draw_engine.
//
// Related classes: The ADM_giicon_draw_args in the admgi_control library are
// an example of a concrete DM_def_icon_draw_args. The DM_icon_draw_args are the
// base class for the DM_def_icon_draw_args. Each level of derivation of the
// DM_icon_draw_args introduces another level of casting methods, for RTTI.
//

/**
 * This class provides an abstract interface for passing arguments to the <tt>Draw()</tt>
 * methods of the <tt>DM</tt> default icons supplied in the dmicon library.
 * <br>
 * <b>Role:</b> The dmicon library factors icon drawing into two parts: determining
 * the combination of draw primitives (polylines and markers) that will make up
 * the icon (handled by the <tt>DM_def_icon</tt> subclasses) and passing those commands
 * on to the particular rendering engine being used (handled by the draw engine).
 * Therefore there are two types of arguments that need to be passed into the
 * <tt>Draw()</tt> method: "state" arguments (like color) that modify the choice of draw primitives
 * and rendering engine arguments that determine, for example, which view the icon
 * should be drawn into.  In the implementation of the default icons, the choice
 * was made to set the "state" of the icon by calling <tt>Set_state()</tt> prior to
 * drawing, which means that the only duty of a <tt>DM_def_icon_draw_args</tt> object is to pass configuration
 * information to the draw engine.  This duty is fulfilled by having the <tt>DM_def_icon_draw_args</tt>
 * object provide methods to downcast itself to a subclass appropriate to the draw
 * engine being used.  Note that a downcast method to a <tt>HOOPS</tt> draw engine is not required,
 * since refreshing the rendering in <tt>HOOPS</tt> does not lead to a call to the <tt>Draw()</tt> method
 * of an icon.
 * <br><br>
 * @see DM_default_icon, DM_def_icon_cmd_args, DM_def_icon_query_args
 */

class DECL_DM_ICON DM_def_icon_draw_args : public DM_icon_draw_args {

 public:
  virtual ~DM_def_icon_draw_args() {}                                    // public destructor
                                                                         // override base class cast method
/**
 * Reserved for dmicon <tt>library</tt>.
 * <br><br>
 * <b>Role:</b> Allows default icon library methods to identify this as a recognized subclass of <tt>DM_icon_draw_args</tt>.
 */
   virtual const DM_def_icon_draw_args* Spatial_cast()const {return this;}// reserved for dmicon library
                                                                         // note: Customer_cast method in base class
	                                                                     // following are new cast methods
/**
 * Allows <tt>GI</tt> draw engine to downcast <tt>draw_args</tt> object so it can be queried.
 */
  virtual const ADM_giicon_draw_args* GI_cast() const {return 0;}        // args for GI draw engine override this
/**
 * Allows <tt>GI</tt> draw engine to downcast <tt>draw_args</tt> object so it can be queried.
 */
  virtual const VM_icon_draw_args* VM_cast() const {return 0;}           // args for VM draw engine override this
  // const void* Customer_case() const {return 0;}                       // defined in the base class
/**
 * @nodoc
 */
	virtual const ADMHOOPS_icon_draw_args* HOOPS_cast() const {return 0;}
};


// Header for the DM_def_icon_cmd_args class.
//
// Purpose: The DM_def_icon_cmd_args class provides a common interface to the
// DM_default_icons for encapsulating state change commands.
// This consists of an Execute method.
//
// Description: Concrete DM_def_icon_cmd_args encapsulate a state change command by holding
// "input" data and implementing the Execute method. The Execute method is called by the
// DM_default_icon::Set_state method.
//
/**
 * This class provides an abstract interface (an <tt>Execute</tt> method) to the <tt>DM_default_icons</tt> for encapsulating state change commands.
 * <br>
 * <b>Role:</b> A concrete <tt>DM_def_icon_cmd_args</tt> object encapsulates a state change command for an icon by holding
 * "input" data and implementing the <tt>Execute</tt> method. When a <tt>DM_icon_cmd_args</tt> object is passed into the <tt>Set_state()</tt> method of a
 * <tt>DM_default_icon</tt> object, the method attempts to downcast the <tt>DM_icon_cmd_args</tt> object into a <tt>DM_def_icon_cmd_args</tt> object by using
 * its <tt>Spatial_cast()</tt> method.  If the downcast is succesful (non-NULL) then the method passes its icon pointer into the <tt>Execute()</tt>
 * method of the command object.  This <tt>Execute()</tt> method in turn calls the appropriate state changing methods of the icon, using
 * command specific data members of the command object.  For example, the <tt>Execute()</tt> method of a <tt>DM_ica_width</tt> command argument calls
 * <tt>DM_default_icon::Set_icon_width()</tt> method of the icon using its <tt>m_width</tt> data member.  By emulating this pattern in their icon
 * implementations, customers can use <tt>DM</tt> to broadcast commands to their icons without requiring <tt>DM</tt> to have knowledge of the specific
 * commands which may be implemented.
 * <br><br>
 * @see DM_default_icon, DM_def_icon_query_args, DM_def_icon_draw_args
 */

class DECL_DM_ICON DM_def_icon_cmd_args : public DM_icon_cmd_args {

 public:
 /**
  * public destructor
  */
  virtual ~DM_def_icon_cmd_args() {}                                    // public destructor

/**
 * Concrete args objects override the <tt>Execute</tt> method with command specific method calls on the input icon.
 * <br><br>
 * @param d_icon
 * icon.
 */
   virtual void Execute(DM_default_icon* d_icon)const =0;                       // concrete args must implement
                                                                         // override base class cast method
/**
 * Reserved for dmicon library.
 */

  virtual const DM_def_icon_cmd_args* Spatial_cast()const {return this;}// reserved for dmicon library
                                                                        // note: Customer_cast method in base class
};

// Header for the DM_def_icon_query_args class.
//
// Purpose: The DM_def_query_cmd_args class provides a common interface to the
// DM_default_icons for encapsulating query commands.
// This consists of an Execute method.
//
// Description: Concrete DM_def_icon_query_args encapsulate a state change command by holding
// "input" & "output" data and implementing the Execute method. The Execute method is called
// by the DM_default_icon::Query method.
//
/**
 * This class provides an abstract interface to the <tt>DM_default_icons</tt> for encapsulating query commands.
 * <br>
 * <b>Role:</b> A concrete <tt>DM_def_icon_query_args</tt> object encapsulates a query command for an icon by holding
 * "output" data and implementing the <tt>Execute</tt> method. When a <tt>DM_icon_query_args</tt> object is passed into the <tt>Query()</tt> method of a
 * <tt>DM_default_icon</tt> object, the method attempts to downcast the <tt>DM_icon_query_args</tt> object into a <tt>DM_def_icon_query_args</tt> object by using
 * its <tt>Spatial_cast()</tt> method.  If the downcast is succesful (non-NULL) then the method passes its icon pointer into the <tt>Execute()</tt>
 * method of the query object.  This <tt>Execute()</tt> method in turn calls the appropriate query methods of the icon, storing the result in
 * query specific data members of the query object.  For example, the Execute() method of a <tt>DM_icq_draw_grid</tt> query argument calls
 * <tt>DM_default_icon::Get_grid()</tt> method of the icon and stores the result in its <tt>m_grid</tt> data member.  By emulating this pattern in their icon
 * implementations, customers can use <tt>DM</tt> to send queries to their icons without requiring <tt>DM</tt> to have knowledge of the specific
 * queries which may be implemented.
 * <br><br>
 * @see DM_default_icon, DM_def_icon_cmd_args, DM_def_icon_draw_args.
 */

class DECL_DM_ICON DM_def_icon_query_args : public DM_icon_query_args {
 public:
 /**
  * public destructor
  */
  virtual ~DM_def_icon_query_args() {}                         // public destructor
/**
 * Concrete args must implement override base class cast method.
 * <br><br>
 * @param d_icon
 * icon
 */
  virtual void Execute(const DM_default_icon* d_icon)=0;              // concrete args must implement
                                                               // override base class cast method
/**
 * Reserved for dmicon library.
 */
  virtual DM_def_icon_query_args* Spatial_cast() {return this;}// reserved for dmicon library
                                                               // note: Customer_cast method in base class
};
 /** @} */
#endif // DM_DEF_ICON_CMD_ARGS_H
