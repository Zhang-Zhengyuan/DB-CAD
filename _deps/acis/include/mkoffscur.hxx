/*******************************************************************/
/*    Copyright (c) 1989-2020 by Spatial Corp.                     */
/*    All rights reserved.                                         */
/*    Protected by U.S. Patents 5,257,205; 5,351,196; 6,369,815;   */
/*                              5,982,378; 6,462,738; 6,941,251    */
/*    Protected by European Patents 0503642; 69220263.3            */
/*    Protected by Hong Kong Patent 1008101A                       */
/*******************************************************************/
//            Function to make up an offset curve
/*******************************************************************/
#if !defined( MKOFFSCUR)
#define MKOFFSCUR

#include "logical.h"
#include "acis.hxx"
#include "dcl_kern.h"
#include "intdef.hxx"
#include "svc.hxx"

class COEDGE;
 /**
   * @nodoc
   */
DECL_KERN curve* make_offset_curve( const   surface* base_sf,
                                    const   curve*   base_cu,
                                    const   double&  offset, 
                                    const   pcurve*  pcur1      = NULL,
                                    const   surface* base_sf2   = NULL,
                                    const   double&  sf2_offset = 0.0,
                                    const   pcurve*  pcur2      = NULL,
                                    const   surface* offs_sf1   = NULL,
                                    const   surface* offs_sf2   = NULL,
                                            COEDGE*  coed       = NULL,
											logical& pcurve_invalid = *(logical*)NULL_REF );



#endif

