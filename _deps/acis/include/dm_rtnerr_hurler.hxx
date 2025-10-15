/*******************************************************************/
/*    Copyright (c) 1989-2020 by Spatial Corp.                     */
/*    All rights reserved.                                         */
/*    Protected by U.S. Patents 5,257,205; 5,351,196; 6,369,815;   */
/*                              5,982,378; 6,462,738; 6,941,251    */
/*    Protected by European Patents 0503642; 69220263.3            */
/*    Protected by Hong Kong Patent 1008101A                       */
/*******************************************************************/

#ifndef DM_RTNERR_HURLER_H
#define DM_RTNERR_HURLER_H
// Header for the DM_rtnerr_hurler class.
//
// Purpose: The DM_rtnerr_hurler provides a protocol for handling exceptions
// across interfaces. Specifically, it returns an integer error code.
//
// Description: Interface routines taking a Spatial_abs_hurler agree to trap all exceptions,
// translate them to an integer code, and then call the Spatial_abs_hurler rethrow_error method
// with the integer code. The DM_rtnerr_hurler::rethrow_error method simply stores the
// error code, which can later be queried; the int cast operator allows us to treat
// DM_rtnerr_hurler objects as int's.
//
#include "dcl_ds.h"
#include "spatial_abs_hurler.hxx"        // base class        // base class
/**
* @file dm_rtnerr_hurler.hxx
 * @CAA2Level L1
 * @CAA2Usage U1
 * \addtogroup DMAPI
 *
 * @{
 */

/**
 * This class provides a protocol for handling exceptions across interfaces.
 * <br>
 * <b>Role:</b> Interface routines taking a <tt>Spatial_abs_hurler</tt> agree to trap all exceptions,
 * translate them to an integer code, and then call the <tt>Spatial_abs_hurler::rethrow_error</tt> method
 * with the integer code. The <tt>DM_rtnerr_hurler::rethrow_error</tt> method simply stores the error code,
 * which can later be queried; the int cast operator allows treating <tt>DM_rtnerr_hurler</tt> objects
 * as <tt>int</tt>s.
 */

 class DECL_DM DM_rtnerr_hurler : public Spatial_abs_hurler {
  private:
    int m_err;
  public:
/**
 * Encapsulates the error handling method.
 * <br><br>
 * @param code
 * code.
 */
    virtual void rethrow_error(int code); // Encapsulate our error handling method
/**
 * <tt>int</tt> cast; this method can treat a <tt>DM_rtnerr_hurler</tt> object as an <tt>int</tt>.
 */
   operator int&();                 // int cast; can treat DM_rtnerr_hurler object as int
/**
 * Public constructor.
 */
     DM_rtnerr_hurler();              // public constructor
/**
 * Public destructor.
 */
    ~DM_rtnerr_hurler();             // public destructor
};
/** @} */

#endif //DM_RTNERR_HURLER_H
