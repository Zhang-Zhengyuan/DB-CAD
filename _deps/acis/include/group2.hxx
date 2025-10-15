/*******************************************************************/
/*    Copyright (c) 1989-2020 by Spatial Corp.                     */
/*    All rights reserved.                                         */
/*    Protected by U.S. Patents 5,257,205; 5,351,196; 6,369,815;   */
/*                              5,982,378; 6,462,738; 6,941,251    */
/*    Protected by European Patents 0503642; 69220263.3            */
/*    Protected by Hong Kong Patent 1008101A                       */
/*******************************************************************/

/*******************************************************************/
// amt 02Nov02 Changing GROUP to SPAGROUP.
/*******************************************************************/


#ifndef SOLVER_GROUP2_HXX
#define SOLVER_GROUP2_HXX

#include "dcl_stitch.h"
#include "logical.h"

class SPAGROUP;
class HH_Trans;
class ENTITY;
class BODY;

DECL_STITCH void
reverse_group(BODY* );

DECL_STITCH void
add_group(ENTITY*, ENTITY*);

DECL_STITCH void
add_group(ENTITY* ent1, ENTITY* ent2, ENTITY_LIST& new_added_attribs);

DECL_STITCH logical
same_group(ENTITY*, ENTITY*);

#endif
