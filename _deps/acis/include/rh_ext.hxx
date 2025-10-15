/* ORIGINAL: rbase2.1/rnd_husk/include/rh_ext.hxx */
//"@(#)$Id: rh_ext.hxx,v 1.4 2000/12/26 19:00:28 products Exp $
/*******************************************************************/
/*    Copyright (c) 1989-2020 by Spatial Corp.                     */
/*    All rights reserved.                                         */
/*    Protected by U.S. Patents 5,257,205; 5,351,196; 6,369,815;   */
/*                              5,982,378; 6,462,738; 6,941,251    */
/*    Protected by European Patents 0503642; 69220263.3            */
/*    Protected by Hong Kong Patent 1008101A                       */
/*******************************************************************/
#if !defined( RENDERER_EXTERN_HEADER )
#define RENDERER_EXTERN_HEADER
// 
// ***********************************************************
//
//  File name : $RCSfile: rh_ext.hxx,v $
//
//  Description :
//
//       Function prototypes for external rendering husk
//       functions. The bodies of these functions must be
//       supplied by the application via an rbase_app_callback.
//        
//		
//
//  ************************************************************
//
//  History:
//     23-June-92   PMK created
//   

#include "dcl_rb.h"

//
// Image Functions
//

DECL_RB void  rh_image_end( int frame );
DECL_RB void  rh_image_start
   ( 
      int frame, 
      int left, 
      int top, 
      int width, 
      int height,
      float scale, 
      int clear 
   );

DECL_RB void  rh_image_scanline( int frame, void *pixels );

//
// Interrupt Function called periodically by rendering husk, to
// check if image computation should continue
// If the function returns TRUE then computation will be halted
//
//
int   DECL_RB rh_check_interrupt();

#endif
