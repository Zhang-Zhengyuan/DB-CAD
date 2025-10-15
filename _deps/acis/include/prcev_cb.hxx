// $Id: prcev_cb.hxx,v 1.5 2000/12/26 18:48:34 products Exp $
/*******************************************************************/
/*    Copyright (c) 1989-2020 by Spatial Corp.                     */
/*    All rights reserved.                                         */
/*    Protected by U.S. Patents 5,257,205; 5,351,196; 6,369,815;   */
/*                              5,982,378; 6,462,738; 6,941,251    */
/*    Protected by European Patents 0503642; 69220263.3            */
/*    Protected by Hong Kong Patent 1008101A                       */
/*******************************************************************/

#ifndef PRCEV_CB_HXX
#define PRCEV_CB_HXX

#include "dcl_kern.h"

#include "logical.h"

typedef logical process_event_func(
			logical
			);

DECL_KERN process_event_func *set_process_event_cb(process_event_func *);
DECL_KERN process_event_func *get_process_event_cb();

#endif
