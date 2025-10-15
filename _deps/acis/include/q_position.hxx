// $Id: q_position.hxx,v 1.5 2002/08/09 17:15:37 jeff Exp $
/*******************************************************************/
/*    Copyright (c) 1989-2020 by Spatial Corp.                     */
/*    All rights reserved.                                         */
/*    Protected by U.S. Patents 5,257,205; 5,351,196; 6,369,815;   */
/*                              5,982,378; 6,462,738; 6,941,251    */
/*    Protected by European Patents 0503642; 69220263.3            */
/*    Protected by Hong Kong Patent 1008101A                       */
/*******************************************************************/


#ifndef Q_POSITION_HXX
#define Q_POSITION_HXX

#include "dcl_kern.h"
// ywoo 08Jan01: added the header file.
#include "logical.h"
// ywoo: end

class ENTITY;
class SPAposition;

DECL_KERN
SPAposition get_position_on_entity(ENTITY *, logical=TRUE);

#endif
