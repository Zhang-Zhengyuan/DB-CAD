// $Id: processevent.hxx,v 1.3 2000/12/26 19:05:38 products Exp $
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
// Declare process_event(logical wait), so people can quit externing it
//----------------------------------------------------------------------

#ifndef processevent_hxx
#define processevent_hxx

#include "logical.h"

//======================================================================

//======================================================================

extern "C" logical process_event(logical wait);

//======================================================================
#endif
