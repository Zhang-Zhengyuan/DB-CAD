/*******************************************************************/
/*    Copyright (c) 1989-2020 by Spatial Corp.                     */
/*    All rights reserved.                                         */
/*    Protected by U.S. Patents 5,257,205; 5,351,196; 6,369,815;   */
/*                              5,982,378; 6,462,738; 6,941,251    */
/*    Protected by European Patents 0503642; 69220263.3            */
/*    Protected by Hong Kong Patent 1008101A                       */
/*******************************************************************/

#ifndef SPATIAL_SYSERR_HURLER_H
#define SPATIAL_SYSERR_HURLER_H

// Header for the Spatial_syserr_hurler class.
//
// Purpose: The Spatial_syserr_hurler provides a protocol for handling exceptions
// across interfaces. Specifically, it encapsulates the ACIS sys_error method.
//
// Description: Interface routines taking a Spatial_abs_hurler agree to trap all exceptions,
// translate them to an integer code, and then call the Spatial_abs_hurler rethrow_error method
// with the integer code. The Spatial_syserr_hurler::rethrow_error method calls ACIS sys_error
// on non-zero error codes, and ignores the zero error code.
//
#include "dcl_adm.h"                        // DECL_ macros                        // DECL_ macros
#include "spatial_abs_hurler.hxx"   // base class Spatial_abs_hurler   // base class Spatial_abs_hurler

/**
* @file spatial_syserr_hurler.hxx
 * @CAA2Level L1
 * @CAA2Usage U1
 * \addtogroup ACISDM
 *
 * @{
 */
/**
 * Provides a protocol for handling exceptions across interfaces.
 * <br>
 * <b>Role:</b> Interface routines taking a <tt>Spatial_abs_hurler</tt> agree to trap all exceptions,
 * translate them to an integer code, and then call the <tt>Spatial_abs_hurler::rethrow_error</tt>
 * method with the integer code. The <tt>Spatial_syserr_hurler::rethrow_error</tt> method calls
 * ACIS <tt>sys_error</tt> on non-zero error codes, and ignores the zero error code. Specifically,
 * it encapsulates the ACIS <tt>sys_error</tt> method.
 */
class DECL_ADM Spatial_syserr_hurler : public Spatial_abs_hurler {
  public:
/**
 * Calls ACIS <tt>sys_error</tt> on non-zero error codes, and ignores the zero error code.
 * <br><br>
 * @param code
 * error code.
 */
     virtual void rethrow_error(int code); // Encapsulate ACIS sys_error
/**
 * Default constructor.
 */
     Spatial_syserr_hurler();
/**
 * Default destructor.
 */
    ~Spatial_syserr_hurler();
};

 /** @} */
#endif //SPATIAL_SYSERR_HURLER_H
