/*******************************************************************/
/*    Copyright (c) 1989-2020 by Spatial Corp.                     */
/*    All rights reserved.                                         */
/*    Protected by U.S. Patents 5,257,205; 5,351,196; 6,369,815;   */
/*                              5,982,378; 6,462,738; 6,941,251    */
/*    Protected by European Patents 0503642; 69220263.3            */
/*    Protected by Hong Kong Patent 1008101A                       */
/*******************************************************************/

#ifndef DM_SYSERR_HURLER_H
#define DM_SYSERR_HURLER_H
// Header for the DM_syserr_hurler class.
//
// Purpose: The DM_syserr_hurler provides a protocol for handling exceptions
// across interfaces. Specifically, it encapsulates the DM_sys_error method.
//
// Description: Interface routines taking a Spatial_abs_hurler agree to trap all exceptions,
// translate them to an integer code, and then call the Spatial_abs_hurler rethrow_error method
// with the integer code. The DM_syserr_hurler::rethrow_error method calls DM_sys_error on
// non-zero error codes, and ignores the zero error code.
//
#include "dcl_ds.h"                         // DECL_ macros                         // DECL_ macros
#include "spatial_abs_hurler.hxx"  // base class Spatial_abs_hurler  // base class Spatial_abs_hurler

/**
* @file dm_syserr_hurler.hxx
 * @CAA2Level L1
 * @CAA2Usage U1
 * \addtogroup DMAPI
 *
 * @{
 */

/**
 * This class provides a protocol for handling exceptions across interfaces.
 * <br>
 * <b>Role:</b> This class encapsulates the <tt>DM_sys_error</tt> method. Interface routines taking a
 * <tt>Spatial_abs_hurler</tt> agree to trap all exceptions, translate them to an integer code, and
 * then call the <tt>Spatial_abs_hurler::rethrow_error</tt> method with the integer code.
 * The <tt>DM_syserr_hurler::rethrow_error</tt> method calls <tt>DM_sys_error</tt> on non-zero error codes, and
 * ignores the zero error code.
 */
  class DECL_DM DM_syserr_hurler : public Spatial_abs_hurler {

  public:
/**
 * Encapsulates <tt>DM_sys_error</tt>.
 * <br><br>
 * @param code
 * code.
 */
    virtual void rethrow_error(int code); // Encapsulate DM_sys_error
/**
 * Public constructor.
 */
  	DM_syserr_hurler();              // public constructor
/**
 * Public destructor.
 */
  	~DM_syserr_hurler();             // public destructor
};

/** @} */


#endif //DM_SYSERR_HURLER_H
