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
//    S_PointsSegmentHandle utility functions
//
//----------------------------------------------------------------------

#ifndef PTS_LINE_ARC_TYP_HXX
#define PTS_LINE_ARC_TYP_HXX

#include "scheme.hxx"
#include "base.hxx"
#include "points_segment_hndl.hxx"
/**
* @nodoc
*/
ScmObject
make_Scm_points_segment_handle ( points_segment_handle* p );

/**
* @nodoc
*/
logical is_Scm_PointsSegmentHandle ( ScmObject r );

/**
* @nodoc
*/
points_segment_handle * get_Scm_points_segment_handle ( ScmObject r );

#endif //PTS_LINE_ARC_TYP_HXX
