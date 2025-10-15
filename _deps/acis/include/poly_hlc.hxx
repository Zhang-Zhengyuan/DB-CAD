/*******************************************************************/
/*    Copyright (c) 2017-2020 by Spatial Corp.                     */
/*    All rights reserved.                                         */
/*    Protected by U.S. Patents 5,257,205; 5,351,196; 6,369,815;   */
/*                              5,982,378; 6,462,738; 6,941,251    */
/*    Protected by European Patents 0503642; 69220263.3            */
/*    Protected by Hong Kong Patent 1008101A                       */
/*******************************************************************/
// Error code definitions for module "poly_hlc_husk/src"

#ifndef POLY_HLC_HUSK_POLY_HLC_ERRMOD_INCLUDED
#define POLY_HLC_HUSK_POLY_HLC_ERRMOD_INCLUDED

#include "errorbase.hxx"
#include "errmsg.hxx"

#include "importexport.h"
#ifdef __poly_hlc_husk
extern EXPORT_SYMBOL message_module poly_hlc_husk_poly_hlc_errmod;

#else
extern IMPORT_SYMBOL message_module poly_hlc_husk_poly_hlc_errmod;

#endif
#define POLY_NOT_SUPPORTED poly_hlc_husk_poly_hlc_errmod.message_code(0)
#define POLY_MIX_NOT_SUPPORTED poly_hlc_husk_poly_hlc_errmod.message_code(1)
#define NOT_POLY_BODY poly_hlc_husk_poly_hlc_errmod.message_code(2)
#define POLY_OFFSET_FAILED poly_hlc_husk_poly_hlc_errmod.message_code(3)
#define POLY_OFFSET_OPENFACE poly_hlc_husk_poly_hlc_errmod.message_code(4)
#define POLY_ALGO_FAILED poly_hlc_husk_poly_hlc_errmod.message_code(5)
#define BREP_TO_POLY poly_hlc_husk_poly_hlc_errmod.message_code(6)
#define POLY_OUT_OF_BOUNDS poly_hlc_husk_poly_hlc_errmod.message_code(7)
#define POLY_OPTION_NOT_SUPPORTED poly_hlc_husk_poly_hlc_errmod.message_code(8)
#define POLY_INTERNAL_ERROR poly_hlc_husk_poly_hlc_errmod.message_code(9)
#define POLY_STITCH_NON_BODY poly_hlc_husk_poly_hlc_errmod.message_code(10)
#define POLY_STITCH_NO_BODIES poly_hlc_husk_poly_hlc_errmod.message_code(11)
#define POLY_TRANSF_MISMATCH poly_hlc_husk_poly_hlc_errmod.message_code(12)
#define POLY_COVR_CIRCUIT_PARTIAL poly_hlc_husk_poly_hlc_errmod.message_code(13)
#define POLY_COVR_CIRCUIT_FAILED poly_hlc_husk_poly_hlc_errmod.message_code(14)
#define POLY_MEMORY_ERROR poly_hlc_husk_poly_hlc_errmod.message_code(15)
#define POLY_FATAL_ERROR_DATA poly_hlc_husk_poly_hlc_errmod.message_code(16)
#define POLY_BAD_VERTEX poly_hlc_husk_poly_hlc_errmod.message_code(17)
#define POLY_BAD_EDGE poly_hlc_husk_poly_hlc_errmod.message_code(18)
#define POLY_BAD_FACE poly_hlc_husk_poly_hlc_errmod.message_code(19)
#define POLY_BOUNDING_BOX_ERROR poly_hlc_husk_poly_hlc_errmod.message_code(20)
#define POLY_VERTEX_IN_INTERIOR_OF_EDGE poly_hlc_husk_poly_hlc_errmod.message_code(21)
#define POLY_NON_MANIFOLD_VERTEX poly_hlc_husk_poly_hlc_errmod.message_code(22)
#define POLY_VERTEX_MISMATCHED_EDGE_ORIENTATION poly_hlc_husk_poly_hlc_errmod.message_code(23)
#define POLY_EDGE_NON_MATCHING_SURFACE_VERTEX_LINE poly_hlc_husk_poly_hlc_errmod.message_code(24)
#define POLY_EDGE_NON_MATCHING_GEOMETRY poly_hlc_husk_poly_hlc_errmod.message_code(25)
#define POLY_VERTEX_NO_GEOMETRY poly_hlc_husk_poly_hlc_errmod.message_code(26)
#define POLY_EDGE_INVALID_VTX_CNT poly_hlc_husk_poly_hlc_errmod.message_code(27)
#define POLY_EDGE_INVALID_SURF_VERT poly_hlc_husk_poly_hlc_errmod.message_code(28)
#define POLY_EDGE_SELF_INTERSECTING poly_hlc_husk_poly_hlc_errmod.message_code(29)
#define POLY_EDGE_HAS_INTERIOR_VERTEX poly_hlc_husk_poly_hlc_errmod.message_code(30)
#define POLY_EDGE_OUTSIDE_BB poly_hlc_husk_poly_hlc_errmod.message_code(31)
#define POLY_FACE_NON_ORIENTABLE poly_hlc_husk_poly_hlc_errmod.message_code(32)
#define POLY_FACE_NON_MATCHING_SURF_VERT_LINE poly_hlc_husk_poly_hlc_errmod.message_code(33)
#define POLY_FACE_INCONSIST_ORIENTATION poly_hlc_husk_poly_hlc_errmod.message_code(34)
#define POLY_FACE_OUTSIDE_BB poly_hlc_husk_poly_hlc_errmod.message_code(35)
#define POLY_EDGE_MISMATCHED_FACE_ORIENTATIONS poly_hlc_husk_poly_hlc_errmod.message_code(36)
#define POLY_FACE_NON_CONTIGUOUS_SURF poly_hlc_husk_poly_hlc_errmod.message_code(37)
#define POLY_FACE_BAD_TRIANGLES poly_hlc_husk_poly_hlc_errmod.message_code(38)
#define POLY_FACE_UNREFERENCED_VERTS poly_hlc_husk_poly_hlc_errmod.message_code(39)
#define POLY_FACE_DUPLICATE_TRIANGLES poly_hlc_husk_poly_hlc_errmod.message_code(40)
#define POLY_FACE_DEGEN_TRIANGLES poly_hlc_husk_poly_hlc_errmod.message_code(41)
#define POLY_FACE_REVERSED_FACETS poly_hlc_husk_poly_hlc_errmod.message_code(42)
#define POLY_FACE_OVERLAPPING_FACETS poly_hlc_husk_poly_hlc_errmod.message_code(43)
#define POLY_FACET_FACET_INTERSECTION poly_hlc_husk_poly_hlc_errmod.message_code(44)
#define POLY_FACE_SELF_CONTACTS poly_hlc_husk_poly_hlc_errmod.message_code(45)
#define POLY_FACE_SMALL_FACET_EDGE poly_hlc_husk_poly_hlc_errmod.message_code(46)
#define POLY_FACE_WIDE_ANGLE poly_hlc_husk_poly_hlc_errmod.message_code(47)
#define POLY_FACE_FACET_SELF_INTERSECTION poly_hlc_husk_poly_hlc_errmod.message_code(48)
#define POLY_FACE_FACET_REMOTE_INTERSECTION poly_hlc_husk_poly_hlc_errmod.message_code(49)
#define POLY_NO_SHALLOW_COPY poly_hlc_husk_poly_hlc_errmod.message_code(50)
#define POLY_NO_BACK_SAVE poly_hlc_husk_poly_hlc_errmod.message_code(51)
#define POLY_CANONIC_RECOGNIZE_FAILED poly_hlc_husk_poly_hlc_errmod.message_code(52)
#define POLY_ZERO_FACETS poly_hlc_husk_poly_hlc_errmod.message_code(53)
#define POLY_MISSING_FACETS poly_hlc_husk_poly_hlc_errmod.message_code(54)
#define POLY_BAD_CANONIC_HYBRID_CREATION poly_hlc_husk_poly_hlc_errmod.message_code(55)
#define POLY_BAD_GEOFORM poly_hlc_husk_poly_hlc_errmod.message_code(56)
#define POLY_HYB_MASSPROP_APPROX poly_hlc_husk_poly_hlc_errmod.message_code(57)
#define POLY_NO_COPY_DATA poly_hlc_husk_poly_hlc_errmod.message_code(58)
#define POLY_BAD_CANONIC_HYBRID_INPUT poly_hlc_husk_poly_hlc_errmod.message_code(59)
#define HYBRID_POLY_FACE_BAD_INPUT poly_hlc_husk_poly_hlc_errmod.message_code(60)
#define POLY_SHEET_BODY_BAD_MASSPROP poly_hlc_husk_poly_hlc_errmod.message_code(61)
#define POLY_VOID_OFFSET poly_hlc_husk_poly_hlc_errmod.message_code(62)
#define POLY_OFFSET_VOXEL_SIZE_TOO_BIG poly_hlc_husk_poly_hlc_errmod.message_code(63)
#define POLY_MAX_DECIMATION poly_hlc_husk_poly_hlc_errmod.message_code(64)
#define POLY_USER_INTERRUPTED poly_hlc_husk_poly_hlc_errmod.message_code(65)
#define POLY_3DPM_UNAVAILABLE poly_hlc_husk_poly_hlc_errmod.message_code(66)

#endif /* POLY_HLC_HUSK_POLY_HLC_ERRMOD_INCLUDED */

