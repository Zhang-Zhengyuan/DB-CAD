/*******************************************************************/
/*    Copyright (c) 1989-2020 by Spatial Corp.                     */
/*    All rights reserved.                                         */
/*    Protected by U.S. Patents 5,257,205; 5,351,196; 6,369,815;   */
/*                              5,982,378; 6,462,738; 6,941,251    */
/*    Protected by European Patents 0503642; 69220263.3            */
/*    Protected by Hong Kong Patent 1008101A                       */
/*******************************************************************/
#ifndef cds_type_hxx
#define cds_type_hxx

#include "scheme.hxx"
#include "logical.h"
#include "ca_bridge.hxx"

//class cds_constraint;
//class workspace_handle;

logical is_Scm_CDS_Constraint(ScmObject);
cds_constraint_handle get_Scm_CDS_Constraint(ScmObject);
ScmObject make_Scm_CDS_Constraint(   cds_constraint_handle constraint_data   );

logical is_Scm_CDS_Workspace(ScmObject);
cds_workspace_handle get_Scm_CDS_Workspace(ScmObject);
ScmObject make_Scm_CDS_Workspace(	cds_workspace_handle pWorkspace	);

logical is_Scm_CDS_Constraint_Status(ScmObject);
cds_constraint_status get_Scm_CDS_Constraint_Status(ScmObject);
ScmObject make_Scm_CDS_Constraint_Status(	cds_constraint_status	);

//logical is_Scm_CDS_Geometry_Status(ScmObject);
//cds_geometry_status get_Scm_CDS_Geometry_Status(ScmObject);
ScmObject make_Scm_CDS_Geometry_Status( cds_geometry_status );

ScmObject make_Scm_Chirality_Status( cds_constraint_alignment , cds_surface_orientation , cds_half_space , cds_half_space );

#endif
