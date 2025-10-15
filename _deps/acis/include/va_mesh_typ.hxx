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
//    Declare procedures for working with va_surfmesh, va_tetmesh, and
//    va_volmesh objects in scheme.
//
//----------------------------------------------------------------------

#ifndef __VA_MESH_SCM_HXX__
#define __VA_MESH_SCM_HXX__

//======================================================================

#include "scheme.hxx"
#include "logical.h"
#include "gc_obj.hxx"

class va_surfmesh;

//======================================================================

logical     is_Scm_VA_SurfMesh(ScmObject);

ScmObject   make_Scm_VA_SurfMesh(va_surfmesh*);
//ScmObject   make_Scm_VA_SurfMesh_List( int npts, VA_SURFMESH** ars );
//ScmObject   make_Scm_VA_SurfMesh_List(  VA_SURFMESH_list & ll  );

va_surfmesh*  get_Scm_VA_SurfMesh(ScmObject);
//void        get_Scm_VA_SurfMesh_List(  ScmObject scm_mdls, VA_SURFMESH_list& mdls );

//======================================================================

class va_tetmesh;

//======================================================================

logical     is_Scm_VA_TetMesh(ScmObject);

ScmObject   make_Scm_VA_TetMesh(va_tetmesh*);
//ScmObject   make_Scm_VA_TetMesh_List( int npts, VA_TETMESH** ars );
//ScmObject   make_Scm_VA_TetMesh_List(  VA_TETMESH_list & ll  );

va_tetmesh*  get_Scm_VA_TetMesh(ScmObject);
//void        get_Scm_VA_TetMesh_List(  ScmObject scm_mdls, VA_TETMESH_list& mdls );

//======================================================================

class va_volmesh;

//======================================================================

logical     is_Scm_VA_VolMesh(ScmObject);

ScmObject   make_Scm_VA_VolMesh(va_volmesh*);

va_volmesh*  get_Scm_VA_VolMesh(ScmObject);

//======================================================================

#endif // __VA_MESH_SCM_HXX__
