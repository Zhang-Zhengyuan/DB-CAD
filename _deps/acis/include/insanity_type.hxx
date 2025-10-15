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
//   declare insanity objects for Scheme
//
//----------------------------------------------------------------------

#ifndef insanity_type_hxx
#define insanity_type_hxx

#include "scheme.hxx"
#include "logical.h"

class insanity_data;
class insanity_list;

// Extract the C++ insanity_data object from a Scheme object
insanity_data* get_Scm_Insanity(ScmObject r);

// Return whether the given Scheme object is in fact an insanity object
logical is_Scm_Insanity(ScmObject r);

// Construct a Scheme Insanity object from the given C++ insanity_data object
// insanity_data objects are reference-counted, so no need to worry about
// ownership.
ScmObject make_Scm_Insanity(insanity_data *insanity);

// Construct a Scheme list of Insanity objects from the given insanity_list.
ScmObject make_Scm_Insanity_List(insanity_list* list);

#endif
