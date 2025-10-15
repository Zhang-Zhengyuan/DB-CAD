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
//    Declare procedures for working with va_mesh_options object in scheme
//
//----------------------------------------------------------------------

#ifndef VA_MESH_OPTIONS_SCM_HXX_
#define  VA_MESH_OPTIONS_SCM_HXX_

//======================================================================

#include "scheme.hxx"
#include "logical.h"
#include "gc_obj.hxx"

class va_base_mesh_options;
class va_surface_mesh_options;
class va_tet_mesh_options;


//======================================================================

logical     is_Scm_VA_Mesh_Opts(ScmObject);

ScmObject   make_Scm_va_Mesh_Opts(va_base_mesh_options*);

va_base_mesh_options*  get_Scm_VA_Mesh_Opts(ScmObject);

//======================================================================

logical     is_Scm_VA_Surface_Mesh_Opts(ScmObject);

ScmObject   make_Scm_va_Surface_Mesh_Opts(va_surface_mesh_options*);

va_surface_mesh_options*  get_Scm_VA_Surface_Mesh_Opts(ScmObject);

//======================================================================

logical     is_Scm_VA_Tet_Mesh_Opts(ScmObject);

ScmObject   make_Scm_va_Tet_Mesh_Opts(va_tet_mesh_options*);

va_tet_mesh_options*  get_Scm_VA_Tet_Mesh_Opts(ScmObject);

//======================================================================

#endif /*VA_MESH_OPTIONS_HXX_*/
