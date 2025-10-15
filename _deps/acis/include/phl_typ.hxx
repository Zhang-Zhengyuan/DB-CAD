/* ORIGINAL: phl1.3/scmext/phl_typ.hxx */
// $Id: phl_typ.hxx,v 1.14 2002/08/09 17:20:58 jeff Exp $
/*******************************************************************/
/*    Copyright (c) 1989-2020 by Spatial Corp.                     */
/*    All rights reserved.                                         */
/*    Protected by U.S. Patents 5,257,205; 5,351,196; 6,369,815;   */
/*                              5,982,378; 6,462,738; 6,941,251    */
/*    Protected by European Patents 0503642; 69220263.3            */
/*    Protected by Hong Kong Patent 1008101A                       */
/*******************************************************************/
// ----------------------------------------------------------------------------
// purpose---
//    Define scheme data type for phl data
//-----------------------------------------------------------------------------
#ifndef phl_typ_hxx
#define phl_typ_hxx

#include "logical.h"

#include "scheme.hxx"

#include "phl_cam.hxx"

class ENTITY_LIST;
class SPAposition;
class GC_Phl_Data;

class phl_data : public ACIS_OBJECT
{
	ENTITY_LIST* draw_edge_list;
	PHL_CAMERA*  draw_camera;
 public:
	phl_data();
	~phl_data();
#ifndef PHL_USE_MMGR
	void clear();
#endif
	void set_camera( PHL_CAMERA* c );
	ENTITY_LIST& get_list();
	SPAposition get_eye() {
		return draw_camera ?
			draw_camera->eyepos()
				:
		    SPAposition(0,0,0);
	}
	SPAposition get_target() {
		return draw_camera ?
			draw_camera->target()
				:
		    SPAposition(0,0,0);
	}
	logical  get_perspective() {
		return draw_camera ?
			draw_camera->perspective()
				:
		    FALSE;
	}
};


struct S_Phl_Data {
	ScmObject tag;
	GC_Phl_Data *The_GC_Phl_Data;
};

#define PHL_DATA_PTR(obj) (((struct S_Phl_Data*)POINTER(obj))->The_GC_Phl_Data->The_Data)


//----------------------------------------------------------------------
// Purpose---
//    Determine if a scheme object is a phl_data
//----------------------------------------------------------------------
logical
is_Scm_Phl_Data(
    ScmObject p
    );

//----------------------------------------------------------------------
// Purpose---
//   Get a phl_data from a scheme phl_data
//----------------------------------------------------------------------
phl_data*
get_Scm_Phl_Data(
	ScmObject phld
	);

//----------------------------------------------------------------------
// Purpose---
//   Create a scheme phl_data from a phl_data
//
// NOTE:
//   This routine does not copy the phl_data.  When the phl_data
//   is no longer accessible from scheme, the scheme garbage collector
//   will lose it.  Since the phl_data is not copied, it must not be
//   deleted in the procedure that calls this procedure.
//----------------------------------------------------------------------
ScmObject make_Scm_Phl_Data(
    phl_data* c_phld
    );

#endif
