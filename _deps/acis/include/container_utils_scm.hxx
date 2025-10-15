/*******************************************************************/
/*    Copyright (c) 1989-2020 by Spatial Corp.                     */
/*    All rights reserved.                                         */
/*    Protected by U.S. Patents 5,257,205; 5,351,196; 6,369,815;   */
/*                              5,982,378; 6,462,738; 6,941,251    */
/*    Protected by European Patents 0503642; 69220263.3            */
/*    Protected by Hong Kong Patent 1008101A                       */
/*******************************************************************/

#ifndef CONTAINER_UTILS_SCM_HXX
#define CONTAINER_UTILS_SCM_HXX
//#include "base_containers_prot.hxx"
#include "container_utils.hxx"
#include "intrapi.hxx"
#include <vector>
class param_info;

enum scm_pointer_type
{
	void_star_type, 
	position_vector_type,
	double_array_type,
	param_info_vector_type,
	int_vector_type
};

ScmObject make_Scm_Pointer( void* p, scm_pointer_type type); 

logical is_Scm_Pointer( ScmObject arg); //

SPAposition_vector* get_Scm_position_vector( ScmObject arg); //

void clear_Scm_position_vector( ScmObject r);

void* get_void_ptr( ScmObject r );

SPAdouble_vector* get_Scm_double_vector( ScmObject arg); //

param_info_vector* get_Scm_param_info_vector( ScmObject arg);//
SPAint_vector* get_Scm_int_vector( ScmObject arg);//

void append_Scm_List( ScmObject & list, ScmObject & tail, ScmObject & item );

ScmObject make_Scm_List( int count, ...);

ScmObject make_Scm_List( std::vector<ScmObject> const& objs );

std::vector<ScmObject> get_Scm_List( ScmObject arg );
#endif
