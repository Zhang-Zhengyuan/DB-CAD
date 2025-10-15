/*******************************************************************/
/*    Copyright (c) 1989-2020 by Spatial Corp.                     */
/*    All rights reserved.                                         */
/*    Protected by U.S. Patents 5,257,205; 5,351,196; 6,369,815;   */
/*                              5,982,378; 6,462,738; 6,941,251    */
/*    Protected by European Patents 0503642; 69220263.3            */
/*    Protected by Hong Kong Patent 1008101A                       */
/*******************************************************************/
#if ! defined ( PE_DEFINES_H )
#define PE_DEFINES_H

  // If you wants to get information about the PE_MESH - Data, set this define.
  // You will find the functions:  PE_xxx->Debug() in  "pe_mesh.cxx",
  // and you can change the implementation for your own use.
  //#define DEBUG_MESH

#if defined ( OLD_APP )

  #define ACIS_MESH
  #define ACIS_FACETER

#else

  // set this define, if the pointer attached to PE_NODE, PE_EDGE, PE_POLY
  // should be recognized as ENTITY pointer
  #define ACIS_MESH

  // set this define, if the faceter calls the PE_MESH_MANAGER with
  // ENTITY SPAparameter
  #define ACIS_FACETER

  // Types and Defines    (used by POLYEDGE_MESH, Standard-Defines in OLD_APP)
  #define IBIT(n) (1 << (n))

  #define SetTo0(str, siz) memset(str, 0, siz)

  typedef unsigned  int   uint;
  typedef unsigned  short ushort;

#endif // OLD_APP / SPATIAL

#endif // PE_DEFINES_H
