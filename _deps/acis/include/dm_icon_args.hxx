/*******************************************************************/
/*    Copyright (c) 1989-2020 by Spatial Corp.                     */
/*    All rights reserved.                                         */
/*    Protected by U.S. Patents 5,257,205; 5,351,196; 6,369,815;   */
/*                              5,982,378; 6,462,738; 6,941,251    */
/*    Protected by European Patents 0503642; 69220263.3            */
/*    Protected by Hong Kong Patent 1008101A                       */
/*******************************************************************/
#ifndef DM_ICON_ARGS_H
#define DM_ICON_ARGS_H

#include "dcl_ds.h"

/**
 * @file dm_icon_args.hxx
 * @CAA2Level L1
 * @CAA2Usage U2
 * \addtogroup DMAPI
 *
 * @{
 */

class DM_def_icon_draw_args;
class DM_def_icon_cmd_args;
class DM_def_icon_query_args;

// Header for the DM_icon_draw_args class.
//
// Purpose: The DM_icon_draw_args class provides a common interface between
// Customer and Spatial rendering libraries. This consists of casting methods allowing
// a concrete DM_draw_engine to decide whether to process or ignore the
// draw request.
// Note that concrete DM_icon's can also intercept and process DM_icon_draw_args.
//
//
// Description: The DM_icon_draw_args interface class is the base class for draw data objects
// which pass client data through the DM_icon::Draw method to the DM_draw_engine.
// The pass-through extends to the Deformable Modeling interface providing DS_dmod hierarchy
// broadcast capabilities.
// Typical data the DM_icon_draw_args convey are the particular views to draw onto:
// the DM_icon can tell DM_draw_engine what geometry to draw, and the DM_icon_draw_args
// can tell the DM_draw_engine what canvas to draw onto.
// Implementations overriding the DM_icon_draw_args should override the Customer_cast
// method to return a pointer to their derived object; clients must then downcast the
// returned void pointer. The Spatial_cast method is reserved for the
// example dmicon library.
//
// Related methods: DM_icon_draw_args command objects can forward through the
// DM_draw_icon and ATTRIB_DM2ACIS::Draw_icon Deformable Modeling interface methods,
// providing DS_dmod hierarchy broadcast capabilities.
//

/**
 * This interface class provides an encapsulation mechanism for passing arguments to the <tt>DM_icon::Draw method</tt>.
 * <br>
 * <b>Role:</b> The <tt>DM_icon_draw_args</tt> object can forward user specified draw routine arguments through the Deformable Modeling
 * interface to the <tt>Draw()</tt> methods of <tt>DM_icon</tt> objects. Because these arguments are passed by reference to an
 * abstract base class, the <tt>DM_icon</tt> base class can have a generic <tt>Draw()</tt> method, with no knowledge of the arguments that
 * will actually be required by the concrete icon's <tt>Draw()</tt> method.
 * <br><br>
 * Customers should derive their own concrete class, e.g. <tt>MY_icon_draw_args</tt>, from <tt>DM_icon_draw_args</tt> if they
 * have derived a concrete icon class, e.g. <tt>MY_icon</tt>, from <tt>DM_icon</tt> and want to pass parameters to its <tt>Draw()</tt> method <tt>MY_icon::Draw()</tt>
 * For example, <tt>MY_icon_draw_args</tt> may have a <tt>Get_color()</tt> method which tells the instance of <tt>MY_icon</tt> what color to use when
 * drawing itself.  To do this, the customer should first override the <tt>Customer_cast()</tt> method of <tt>MY_icon_draw_args</tt> to return
 * a <tt>(const void*)</tt> pointer to itself.  The implementation of <tt>MY_icon::Draw()</tt> can then examine the return value of the <tt>Customer_cast()</tt>
 * method to determine if the concrete type of <tt>DM_icon_draw_args</tt> object being passed in is a customer type (non-<tt>NULL</tt>) or not (<tt>NULL</tt>).
 * If it is a customer type, then it can be downcast to a customer base class which in turn can be used to determine
 * that the exact concrete type is <tt>MY_icon_draw_args</tt>. (If the customer only has one type of <tt>draw_args</tt> object this intermediate
 * customer base class is unnecessary.)  At this point, the incoming <tt>DM_icon_draw_args</tt> object can be downcast to the concrete
 * <tt>MY_icon_draw_args</tt> type, and whatever queries, like <tt>Get_color()</tt>, are needed by the implementation of <tt>MY_icon::Draw()</tt> can be called.
 * <br><br>
 * Implementations overriding the <tt>DM_icon_draw_args</tt> should override the <tt>customer_cast</tt> method to
 * return a pointer to their derived object; clients must then downcast the returned void pointer.
 * Overriding the <tt>Spatial_cast</tt> method is reserved for the example <tt>dmicon</tt> library.
 * <br><br>
 * <b>Related methods:</b> <tt>DM_icon_draw_args</tt> command objects are passed through the <tt>DM_draw_icon</tt> (SDM)
 * and <tt>ATTRIB_DM2ACIS::Draw_icon</tt> (ADM) interface methods to provide arguments to the appropriate tag
 * objects' <tt>DM_icon::Draw()</tt> methods.
 * @see DM_icon, DM_icon_cmd_args, DM_icon_query_args
 */

 class DECL_DM DM_icon_draw_args {
 public:
/**
 * Default constructor.
 */
  DM_icon_draw_args() {}                                                // public constructor

/**
 * Default destructor.
 */
   virtual ~DM_icon_draw_args() {}                                       // public destructor

/**
 * Used by customer icons to downcast to customer type.
 * <br><br>
 * <b>Role:</b> Customer subclasses should override this method as <tt>{return (void*)this;}</tt>.
 */
   virtual const void* Customer_cast() const {return 0;}                 // customer implementations should override this method
                                                                      // as {return (const void*)this;}
/**
 * Reserved for the example <tt>dmicon</tt> library.
 */
   virtual const DM_def_icon_draw_args* Spatial_cast() const {return 0;} // reserved for the example dmicon library
};

// Header for the DM_icon_cmd_args class.
//
// Purpose:
// The DM_icon_cmd_args interface class provides a common interface for encapsulating
// state change commands between icon libraries. This consists of casting methods
// allowing a concrete DM_icon to decide whether to process or ignore a command.
//
// Description: Concrete DM_icon_cmd_args encapsulate commands for changing an icon's state.
// The DM_icon_cmd_args command object passes through the Deformable Modeling
// interface, providing broadcast capabilities.
// Implementations overriding the DM_icon_cmd_args should override the Customer_cast
// method to return a pointer to their derived object; clients must then downcast the
// returned void pointer.
// Overriding the Spatial_cast method is reserved for the example dmicon library.
//
// Related methods: DM_icon_cmd_args command objects can pass through the
// DM_setstate_icon and ATTRIB_DM2ACIS::Setstate_icon Deformable Modeling interface methods,
// providing DS_dmod hierarchy broadcast capabilities.
//
/**
 * This interface class provides a command object to forward client requests through the <tt>DM_icon::Set_state</tt> method to the <tt>DM_icon</tt>.
 * <br>
 * <b>Role:</b> The <tt>DM_icon_cmd_args</tt> command object can be forwarded through the deformable modeling
 * interface, providing the capability to broadcast user commands to <tt>DM_icon</tt> objects throughout
 * the <tt>DS_dmod</tt> hierarchy.  To do this, customers should override the <tt>Set_state()</tt> method of their
 * concrete class, eg. <tt>MY_icon</tt>, derived from <tt>DM_icon</tt>.  This method should first call the <tt>Customer_cast()</tt> method
 * to determine whether the concrete type of the <tt>DM_icon_cmd_args</tt> object being passed in is a customer
 * type (non-<tt>NULL</tt>) or not (<tt>NULL</tt>). If it is a customer type, then it can be downcast to a customer base class
 * which in turn can be used to determine the exact concrete type of the customer command object. The <tt>Set_state()</tt>
 * method of the icon can then further downcast the command object to its exact type and process the command.
 * <br><br>
 * Implementations overriding
 * the <tt>DM_icon_cmd_args</tt> should override the <tt>customer_cast</tt> method to return a pointer to their
 * derived object; clients must then downcast the returned void pointer. Overriding the
 * <tt>Spatial_cast</tt> method is reserved for the example <tt>dmicon</tt> library.
 * <br><br>
 * <b>Related methods:</b> <tt>DM_icon_cmd_args</tt> command objects are passed into the <tt>DM_setstate_icon</tt> (SDM)
 * and <tt>ATTRIB_DM2ACIS::Setstate_icon</tt> (ADM) interface methods to broadcast commands to the apropriate
 * tag objects' <tt>DM_icon</tt> objects in the <tt>DS_dmod</tt> hierarchy.
 * @see DM_icon, DM_icon_draw_args, DM_icon_query_args
 */

 class DECL_DM DM_icon_cmd_args {
 public:
/**
 * Default constructor.
 */
   DM_icon_cmd_args() {}                                               // public constructor
/**
 * Default destructor.
 */

  virtual ~DM_icon_cmd_args() {}                                      // public destructor
/**
 * Used by customer icons to downcast to customer type.
 * <br><br>
 * <b>Role:</b> Customer subclasses should override this method as <tt>{return (void*)this;}</tt>.
 */

  virtual const void* Customer_cast() const {return 0;}               // customer implementations should override this method
                                                                      // as {return (const void*)this;}
/**
 * Reserved for the example <tt>dmicon</tt> library.
 */
   virtual const DM_def_icon_cmd_args* Spatial_cast() const {return 0;}// reserved for the example dmicon library
};

// Header for the DM_icon_query_args class.
//
// Purpose: The DM_icon_query_args interface class provides a common interface
// between icon libraries. This consists of casting methods allowing a concrete
// DM_icon to decide whether to process or ignore a command. DM_icon_query_args
// are for querying an icon's state.
//
// Description: Concrete DM_icon_query_args encapsulate commands for querying an icon's state.
// The DM_icon_query_args query-command object passes through the Deformable Modeling
// interface, giving a common look and feel to icon draw, set_state, and query commands.
// Implementations overriding the DM_icon_query_args should override the Customer_cast
// method to return a pointer to their derived object; clients must then downcast the
// returned void pointer.
//
// Derived classes can implement cast methods, allowing icons to decide whether to process or
// ignore the args. Overriding the Spatial_cast method is reserved for the example dmicon library.
//
// Related methods: DM_icon_query_args query-command objects can pass through the
// DM_query_icon and ATTRIB_DM2ACIS::Query_icon Deformable Modeling interface methods,
// giving a common look and feel to icon draw, set_state, and query commands.
//
/**
 * This interface class encapsulates queries that can be sent to <tt>DM_icon</tt> objects.
 * <br>
 * <b>Role:</b> A <tt>DM_icon_query_args</tt> object can be passed to the the <tt>Query()</tt> method of
 * a customer icon object (derived from <tt>DM_icon</tt>) to perform a customer-specified query
 * on the state of that icon.  To do this, the icon's override of the <tt>Query()</tt> method
 * should call <tt>Customer_cast()</tt> on the incoming <tt>DM_icon_query_args</tt> object to determine if
 * it is a customer query (non-<tt>NULL</tt> return value) or not (<tt>NULL</tt> return value). If it
 * is a customer query object, it can then be downcast to a customer base class, which
 * in turn can be used to determine the specific query type.  The <tt>Query()</tt> method should then
 * call methods of the (downcast) query object to satisfy the query.
 * <br><br>
 * Implementations overriding the <tt>DM_icon_query_args</tt> should override the <tt>Customer_cast</tt> method to return a pointer
 * to their derived object; clients must then downcast the returned void pointer.
 * Overriding the <tt>Spatial_cast</tt> method is reserved for the example <tt>dmicon</tt> library.
 * <br><br>
 * <b>Related methods:</b> <tt>DM_icon_query_args</tt> query-command objects can pass through the
 * <tt>DM_query_icon</tt> (SDM) and <tt>ATTRIB_DM2ACIS::Query_icon</tt> (ADM) deformable modeling interface methods, giving a
 * common look and feel to icon draw, set state, and query commands.
 * @see DM_icon, DM_icon_cmd_args, DM_icon_draw_args
 */
 class DECL_DM DM_icon_query_args {
 public:
/**
 * Public constructor.
 */
   DM_icon_query_args() {}                                   // public constructor
/**
 * Public destructor.
 */
  virtual ~DM_icon_query_args() {}                          // public destructor
/**
 * Used by customer icons to downcast to customer type.
 * <br><br>
 * <b>Role:</b> Customer subclasses should override this method as <tt>{return (void*)this;}</tt>.
 */
   virtual void* Customer_cast() {return 0;}                 // customer implementations should override this method
                                                            // as {return (void*)this;}
/**
 * Reserved for the example <tt>dmicon</tt> library.
 */
   virtual DM_def_icon_query_args* Spatial_cast() {return 0;}// reserved for the example dmicon library
};

/** @} */

#endif //DM_ICON_ARGS_H
