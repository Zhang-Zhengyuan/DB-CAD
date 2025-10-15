// $Id: ihl.hxx,v 1.4 2000/12/26 18:41:48 products Exp $
/*******************************************************************/
/*    Copyright (c) 1989-2020 by Spatial Corp.                     */
/*    All rights reserved.                                         */
/*    Protected by U.S. Patents 5,257,205; 5,351,196; 6,369,815;   */
/*                              5,982,378; 6,462,738; 6,941,251    */
/*    Protected by European Patents 0503642; 69220263.3            */
/*    Protected by Hong Kong Patent 1008101A                       */
/*******************************************************************/
//------------------------------------------------------------------------------
// Purpose---
//     Place to hook stuff that should be included in ALL ihl source files.
//------------------------------------------------------------------------------

#ifndef IHL_HXX
#define IHL_HXX

#include "acis.hxx"
#include "version.hxx"
#include "dcl_ihl.h"
DECL_IHL int  ProjPoint(
        double *pkt,
        double *ppt );
#endif
