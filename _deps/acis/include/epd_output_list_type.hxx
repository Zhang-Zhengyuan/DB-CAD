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

#ifndef epd_output_list_type_hxx
#define epd_output_list_type_hxx

#include "float.h"
#include "scheme.hxx"
#include "logical.h"
#include "intrapi.hxx"
#include "list_stream.hxx"


class epd_output : public ACIS_OBJECT
{
public:
	epd_output():distance( -DBL_MAX ) {}
	epd_output( SPAposition _closest_pos, double _distance, param_info _ent_info ):closest_pos( _closest_pos ),
		distance( _distance ), ent_info ( _ent_info ) {}

	inline SPAposition closest_point() const { return closest_pos; }
	inline double least_distance() const { return distance; }
	inline param_info entity_info() const { return ent_info; } 

private:
	SPAposition closest_pos;
	double distance;
	param_info ent_info;
};

class epd_output_list : public list_stream<epd_output>
{
public:
	epd_output next_from( int& iter );
	epd_output operator[](int index);
};



//======================================================================

logical is_Scm_epd_output_list(ScmObject);
epd_output_list* get_Scm_epd_output_list(ScmObject);
ScmObject make_Scm_epd_output_list(epd_output_list *epd_out_list);
logical clear_Scm_epd_output_list(ScmObject obj);

//======================================================================
#endif
