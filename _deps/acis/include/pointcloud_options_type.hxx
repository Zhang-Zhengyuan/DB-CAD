// $Id: acisoptions_type.hxx,v 1.1 2001/12/03 16:35:22 rocon Exp $
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
//   declare acis_options object for scheme
//
//----------------------------------------------------------------------

#ifndef pointcloud_options_type_hxx
#define pointcloud_options_type_hxx

#include "scheme.hxx"
#include "point_cloud.hxx"
#include "logical.h"

class SPApoint_cloud_options;


//======================================================================

logical is_Scm_SPApoint_cloud_options(ScmObject);
SPApoint_cloud_options* get_Scm_SPApoint_cloud_options(ScmObject);
ScmObject make_Scm_SPApoint_cloud_options(SPApoint_cloud_options *Options_info);
logical clear_Scm_SPApoint_cloud_options(ScmObject obj);

//======================================================================
#endif
