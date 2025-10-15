/*******************************************************************/
/*    Copyright (c) 1989-2020 by Spatial Corp.                     */
/*    All rights reserved.                                         */
/*    Protected by U.S. Patents 5,257,205; 5,351,196; 6,369,815;   */
/*                              5,982,378; 6,462,738; 6,941,251    */
/*    Protected by European Patents 0503642; 69220263.3            */
/*    Protected by Hong Kong Patent 1008101A                       */
/*******************************************************************/

#ifndef SPATIAL_ABS_HURLER_H
#define SPATIAL_ABS_HURLER_H
//
// Header for the Spatial_abs_hurler class.
//
// Purpose: The Spatial_abs_hurler provides a protocol for handling exceptions
// across interfaces.
//
// Description: Exception handling across interfaces is problematic, because different libraries
// may have different exception handling methods, e.g., C++ try-catch or C setjmp-longjmp.
// Interface routines taking a Spatial_abs_hurler agree to trap all exceptions,
// translate them to an integer code, and then call the Spatial_abs_hurler rethrow_error method
// with the integer code. IMPORTANT: an error code of 0 (zero) is reserved as a no-error condition,
// and may be passed to the rethrow_error method by the Deformable modeling kernel.
//
#include "dcl_ds.h"

/**
 * @file spatial_abs_hurler.hxx
 * @CAA2Level L1
 * @CAA2Usage U2
 * \addtogroup DMAPI
 *
 * @{
 */

/**
 * This abstract base class class provides a protocol for handling exceptions across interfaces.
 * <br><br>
 * <b>Role:</b> Exception handling across interfaces is problematic, because different libraries
 * may have different exception handling methods, e.g., C++ try-catch or C setjmp-longjmp.
 * Interface routines taking a <tt>Spatial_abs_hurler</tt> agree to trap all exceptions, translate
 * them to an integer code, and then call the <tt>Spatial_abs_hurler</tt><tt>rethrow_error</tt> method with
 * the integer code. To use such an interface routine, clients must pass in a concrete hurler object,
 * such as a <tt>DM_rtnerr_hurler</tt>, that will resignal the error using the client code's error signalling
 * mechanism.
 * <br><br>
 * <b>IMPORTANT:</b> An error code of 0 (zero) is reserved as a no-error condition,
 * and may be passed to the <tt>rethrow_error</tt> method by the Deformable modeling kernel.
 * Therefore the rethrow_error method of subclasses overriding the <tt>Spatial_abs_hurler</tt> must
 * not signal an error when passed a 0 argument.
 * @see DM_rtnerr_hurler, DM_syserr_hurler
 */

  class DECL_DM Spatial_abs_hurler {

  public:
/**
 * A pure virtual method for resignaling an exception.
 * <br><br>
 * <b>Role:</b> This pure virtual method is overriden by a concrete hurler
 * subclass to implement a specific error resignalling strategy (such as C++ exception
 * or integer error code). It must treat a 0 argument as a no-error condition.<br><br>
 * @param error_no
 * error code that will be resignaled by the hurler.
 */

 	virtual void rethrow_error(int error_no)=0;  // method for resignaling an exception; must no-op
                                        // when passed a 0 argument.
/**
 * Default destructor.
 */

 	virtual ~Spatial_abs_hurler() {}
/**
 * Default constructor.
 */

 	Spatial_abs_hurler() {}

};

/** @} */
#endif //SPATIAL_ABS_HURLER_H
