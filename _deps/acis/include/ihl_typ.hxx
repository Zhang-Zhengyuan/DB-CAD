// $Id: ihl_typ.hxx,v 1.5 2002/08/09 17:22:41 jeff Exp $
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
//    Define scheme data type for ihl data
//-----------------------------------------------------------------------------
#ifndef ihl_typ_hxx
#define ihl_typ_hxx

#include "logical.h"

#include "scheme.hxx"

#include "ihl_cam.hxx"

class ENTITY_LIST;
class SPAposition;
class GC_Ihl_Data;

class ihl_data: public ACIS_OBJECT
{
	ENTITY_LIST* draw_edge_list;
	IHL_CAMERA*  draw_camera;
 public:
	ihl_data();
	~ihl_data();
	void clear();
	void set_camera( IHL_CAMERA* c );
	IHL_CAMERA* get_camera();
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


struct S_Ihl_Data {
	ScmObject tag;
	GC_Ihl_Data *The_GC_Ihl_Data;
};

#define IHL_DATA_PTR(obj) (((struct S_Ihl_Data*)POINTER(obj))->The_GC_Ihl_Data->The_Data)


//----------------------------------------------------------------------
// Purpose---
//    Determine if a scheme object is a ihl_data
//----------------------------------------------------------------------
logical
is_Scm_Ihl_Data(
    ScmObject p
    );

//----------------------------------------------------------------------
// Purpose---
//   Get a ihl_data from a scheme ihl_data
//----------------------------------------------------------------------
ihl_data*
get_Scm_Ihl_Data(
	ScmObject ihld
	);

//----------------------------------------------------------------------
// Purpose---
//   Create a scheme ihl_data from a ihl_data
//
// NOTE:
//   This routine does not copy the ihl_data.  When the ihl_data
//   is no longer accessible from scheme, the scheme garbage collector
//   will lose it.  Since the ihl_data is not copied, it must not be
//   deleted in the procedure that calls this procedure.
//----------------------------------------------------------------------
ScmObject make_Scm_Ihl_Data(
    ihl_data* c_ihld
    );

#endif
