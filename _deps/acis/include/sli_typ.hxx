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
//    declare procedures for working with skinning/lofting in scheme
//
//----------------------------------------------------------------------

#ifndef slinterface_typ_hxx
#define slinterface_typ_hxx

#include "scheme.hxx"
#include "logical.h"

class AcisSLInterface;


//======================================================================

logical is_Scm_SLInterface(ScmObject);
AcisSLInterface *get_Scm_SLInterface(ScmObject);
ScmObject make_Scm_SLInterface(AcisSLInterface *sec);
int get_Scm_SLInterface_List(ScmObject list, AcisSLInterface **&secs);
logical is_Scm_SLInterface_List(ScmObject list);
logical clear_Scm_SLInterface(ScmObject obj);

//======================================================================
#endif
