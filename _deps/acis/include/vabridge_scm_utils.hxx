/*******************************************************************/
/*    Copyright (c) 1989-2020 by Spatial Corp.                     */
/*    All rights reserved.                                         */
/*    Protected by U.S. Patents 5,257,205; 5,351,196; 6,369,815;   */
/*                              5,982,378; 6,462,738; 6,941,251    */
/*    Protected by European Patents 0503642; 69220263.3            */
/*    Protected by Hong Kong Patent 1008101A                       */
/*******************************************************************/

// ****************************************************************************
//  This file contains the declarations for the utility functions 
//  for the VAbridge Scheme extensions. Many are used in both
//  source files for the VAbridge Scheme extensions. 
// ****************************************************************************

#ifndef __VABRIDGE_SCM_UTILS_HXX__
#define __VABRIDGE_SCM_UTILS_HXX__

#include "logical.h"
#include "scmobject.h"

// Forward declarations.

class AcisOptions;
class va_base_mesh_options;
class va_surface_mesh_options;
class va_tet_mesh_options;
class outcome;

// Utility macro to add temporary, deprecated (redundant) Scheme extensions.

#define DEPRECATED_PROC( a, b ) \
ScmObject CONCAT(b,_dep) ( int argc, ScmObject *argv ) \
{ \
	acis_printf( "*** %s has been deprecated. ***\n", a ); \
	return b( argc, argv ); \
} \
SCM_PROC( 0, MANY, a, CONCAT(b,_dep) )


// ****************************************************************************
//   Temporary kludge - until VKI header files are visible
// ****************************************************************************

#if 0

// VKI includes
#include "base/basedefs.h"

#else
                   /* defines for file types */
#define SYS_ASCII       -1
#define SYS_BINARY      -2
#define SYS_NATIVE            1
#define SYS_PLOT3D_GRID       2
#define SYS_PLOT3D_SOLUTION   3
#define SYS_PATRAN_RESULT     4
#define SYS_GENERIC           5
#define SYS_PDA               6
#define SYS_LSTC_STATE        7
#define SYS_ABAQUS_ODB        8
#define SYS_ABAQUS_FIL        9
#define SYS_NASTRAN_OUTPUT2  10
#define SYS_STL              11
#define SYS_STLBIN           12
#define SYS_ABAQUS_INPUT     13
#define SYS_SDRC_UNIVERSAL   14
#define SYS_PAM_DAISY        15
#define SYS_ANSYS_RESULT     16
#define SYS_MECHANICA_STUDY  17
#define SYS_NASTRAN_BULKDATA 18
#define SYS_FDI_NEUTRAL      19
#define SYS_PATRAN_NEUTRAL   20
#define SYS_FLUENT_MESH      21
#define SYS_ANSYS_INPUT      22
#define SYS_TECPLOT          23
#define SYS_HYPERMESH_ASCII  24
#define SYS_LSTC_INPUT       25
#define SYS_CGNS             26
#define SYS_MARC_POST        27
#define SYS_ENSIGHT          28
#define SYS_STARCCM          29
#define SYS_MECHANICA_FNF    30
#define SYS_ACIS             31
#define SYS_LSTC_HISTORY     32
#define SYS_NASTRAN_XDB      33

#endif

// Get an AcisOptions object from the end of the argument list.

AcisOptions *
va_get_acis_options( int &argc, ScmObject *argv );

// Get a mesh options object (any flavor) from argument list.

va_base_mesh_options*
va_get_mesh_options( int& argc, ScmObject* argv );

// Get a surface mesh options object from argument list.

va_surface_mesh_options*
va_get_surfmesh_options( int& argc, ScmObject* argv );

// Get a tet mesh options object from argument list.

va_tet_mesh_options*
va_get_tetmesh_options( int& argc, ScmObject* argv );

// Get a mesh object from the argument list.
// It always should be the first argument in the list.

va_mesh*
va_get_mesh( int argc, ScmObject* argv );

// Convert a string to lower case.

char * 
va_strtolower( char *str );

// Parse the surface mesh options arguments.

logical
va_ParseSurfMeshOpts( va_surface_mesh_options *& p_mesh_opts, int& argc, ScmObject*& argv );

// Parse the tet mesh options arguments.

logical
va_ParseTetMeshOptions( va_tet_mesh_options *&p_mesh_opts, int& argc, ScmObject *& argv );

// Examine the contents of the error_info object within the given outcome.
// If it is a mesh_error_info object, print out the information in the object.

void 
va_examine_mesh_result( outcome &result );

#endif
