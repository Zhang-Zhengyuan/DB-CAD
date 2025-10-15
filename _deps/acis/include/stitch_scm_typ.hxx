/*******************************************************************/
/*    Copyright (c) 1989-2020 by Spatial Corp.                     */
/*    All rights reserved.                                         */
/*    Protected by U.S. Patents 5,257,205; 5,351,196; 6,369,815;   */
/*                              5,982,378; 6,462,738; 6,941,251    */
/*    Protected by European Patents 0503642; 69220263.3            */
/*    Protected by Hong Kong Patent 1008101A                       */
/*******************************************************************/
#ifndef STITCH_SCM_TYP_HXX
#define STITCH_SCM_TYP_HXX

#include "scheme.hxx"
#include "logical.h"

template <typename> class scm_shared_ptr;

template <typename stitch_handle_type> bool is_Scm_Stitch_Handle(ScmObject);
template <typename stitch_handle_type> scm_shared_ptr<stitch_handle_type> get_Scm_Stitch_Handle(ScmObject);
template <typename stitch_handle_type> ScmObject make_Scm_Stitch_Handle(scm_shared_ptr<stitch_handle_type>);

#endif // STITCH_SCM_TYP_HXX


