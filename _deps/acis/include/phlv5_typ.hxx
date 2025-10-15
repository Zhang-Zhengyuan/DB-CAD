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
//    Define scheme data type for phlv5 data
//-----------------------------------------------------------------------------
#ifndef phlv5_typ_hxx
#define phlv5_typ_hxx
#include "base.hxx"
#include "logical.h"
#include "scheme.hxx"
#include "phlv5_cam.hxx"

class ENTITY_LIST;
class SPAposition;
class GC_Phlv5_Data;
class CATHLRComputerACIS;

class phlv5_data ACIS_BASE_CLASS
{
	ENTITY_LIST* draw_edge_list;
	PHLV5_CAMERA*  draw_camera;
	CATHLRComputerACIS* computer;


 public:
	phlv5_data();
	~phlv5_data();

	void set_computer(CATHLRComputerACIS* _computer) {computer = _computer;};
	CATHLRComputerACIS* get_computer() {return computer; };
 
	void clear();
	void set_camera( PHLV5_CAMERA* c );
	PHLV5_CAMERA* get_camera();

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


struct S_Phlv5_Data {
	ScmObject tag;
	GC_Phlv5_Data *The_GC_Phlv5_Data;
};

#define PHLV5_DATA_PTR(obj) (((struct S_Phlv5_Data*)POINTER(obj))->The_GC_Phlv5_Data->The_Data)


//----------------------------------------------------------------------
// Purpose---
//    Determine if a scheme object is a phlv5_data
//----------------------------------------------------------------------
logical
is_Scm_Phlv5_Data(
    ScmObject p
    );

//----------------------------------------------------------------------
// Purpose---
//   Get a phlv5_data from a scheme phlv5_data
//----------------------------------------------------------------------
phlv5_data*
get_Scm_Phlv5_Data(
	ScmObject phlv5d
	);

//----------------------------------------------------------------------
// Purpose---
//   Create a scheme phlv5_data from a phlv5_data
//
// NOTE:
//   This routine does not copy the phlv5_data.  When the phlv5_data
//   is no longer accessible from scheme, the scheme garbage collector
//   will lose it.  Since the phlv5_data is not copied, it must not be
//   deleted in the procedure that calls this procedure.
//----------------------------------------------------------------------
ScmObject make_Scm_Phlv5_Data(
    phlv5_data* c_phlv5d
    );

#endif
