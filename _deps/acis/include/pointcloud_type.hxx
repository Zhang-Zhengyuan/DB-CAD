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

#ifndef pointcloud_type_hxx
#define pointcloud_type_hxx

#include "scheme.hxx"
#include "logical.h"
#include "point_cloud.hxx"
#include "rgbcolor.hxx"
#include "transf.hxx"


class scm_point_cloud : public SPApoint_cloud
{
	SPAtransf _tr;
	rgb_color _color;
	int use_count;

protected:
	virtual SPApoint_cloud* make_object( SPApoint_cloud* target_pc, SPApoint_cloud_options* pc_opts,
		position_list_stream* pos_list, SPApoint_cloud_copy_type copy_type );

public:
	scm_point_cloud( SPApoint_cloud_options* pc_opts = NULL, position_list_stream* pos_list = NULL );

	virtual const SPAtransf* get_transform() const { return &_tr; }
	// concatenates with the existing transform
	void transform( const SPAtransf& tr );

	rgb_color get_color() const { return _color; };
	void set_color( const rgb_color& color );

	void add_ref();
	void remove_ref( logical keep = FALSE );
};


//======================================================================

logical is_Scm_Point_Cloud(ScmObject);
scm_point_cloud* get_Scm_Point_Cloud(ScmObject);
ScmObject make_Scm_Point_Cloud(scm_point_cloud *point_cloud);
logical clear_Scm_Point_Cloud(ScmObject obj);

//======================================================================
#endif
