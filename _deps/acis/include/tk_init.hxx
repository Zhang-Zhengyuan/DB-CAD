/* ORIGINAL: 3dt2.1/main/tk_init.hxx */
// $Id: tk_init.hxx,v 1.5 2000/12/26 19:05:39 products Exp $
/*******************************************************************/
/*    Copyright (c) 1989-2020 by Spatial Corp.                     */
/*    All rights reserved.                                         */
/*    Protected by U.S. Patents 5,257,205; 5,351,196; 6,369,815;   */
/*                              5,982,378; 6,462,738; 6,941,251    */
/*    Protected by European Patents 0503642; 69220263.3            */
/*    Protected by Hong Kong Patent 1008101A                       */
/*******************************************************************/
//----------------------------------------------------------------------
// purpose---
//   Define procedures which shold be called to initialise and terminate
//   the 3D Toolkit.
//
//   A lot of the initialisation and termination code in the 3D Toolkit
//   is handled by the constructors and destructors of static objects.
//   For some things however, this does not work because you cannot control
//   the order in which things are constructed or destroyed, so you might
//   try using things that do not exist yet, or which have already been
//   destroyed.
//----------------------------------------------------------------------

#ifndef tk_init_hxx
#define tk_init_hxx

//======================================================================

void toolkit_init();
void toolkit_close();

//======================================================================

#endif
