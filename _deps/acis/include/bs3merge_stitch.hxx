/*******************************************************************/
/*    Copyright (c) 1989-2020 by Spatial Corp.                     */
/*    All rights reserved.                                         */
/*    Protected by U.S. Patents 5,257,205; 5,351,196; 6,369,815;   */
/*                              5,982,378; 6,462,738; 6,941,251    */
/*    Protected by European Patents 0503642; 69220263.3            */
/*    Protected by Hong Kong Patent 1008101A                       */
/*******************************************************************/

#if !defined (BHL_MERGE)
#define BHL_MERGE

#include "sp3crtn.hxx"
#include "sp2crtn.hxx"

#include "dcl_stitch.h"

#define bs3_tol 0.001

class bhl_stitch_options;

#ifdef THREAD_SAFE_ACIS
DECL_STITCH extern GLOBAL_VAR safe_integral_type<logical> bhl_snap_curves;
#else
DECL_STITCH extern GLOBAL_VAR logical bhl_snap_curves;
#endif

logical
bhl_merge_bs3_curves_small( bs3_curve&, bs3_curve&,
									bs3_curve&, const SPAposition& );

logical
bhl_merge_bs3_curves_big( bs3_curve&, bs3_curve&,
			bs3_curve&, const SPAposition&, double merge_tol);

DECL_STITCH logical
bhl_join_two_bs3_curves( bs3_curve&, bs3_curve&,
			bs3_curve&, const SPAposition&);

DECL_STITCH logical
bhl_average_two_bs3_curves(bs3_curve&, bs3_curve&, bs3_curve& );

DECL_STITCH logical
bhl_fix_bs3_curve_end(	bs3_curve&, bs3_curve&, const SPAposition& );

// GSSL/RA - start ( June 12, 2002) : Fix for Bug #68651.
// Added a logical.
DECL_STITCH logical
bhl_snap_bs3_curve_end(	bs3_curve&, bs3_curve&, const SPAposition&, logical , double tol = -1.0 );


//sumanta
///////////

enum DECL_STITCH cur_pt_status
{
	END_PT,
	START_PT,
	NONE
};

class DECL_STITCH cur_snap_data{

private:
SPAposition pos;
double tolerance;
cur_pt_status st;

public:
	cur_snap_data() {};
	~cur_snap_data() {};
	cur_snap_data(SPAposition p);
	cur_snap_data(SPAposition p,double toler,cur_pt_status sta);
	SPAposition get_position()  ;
	double get_tolerance() ;
	cur_pt_status get_status();
	void set_tolerance(double t);
	void set_status(cur_pt_status sta);
	void set_position(SPAposition p);
};
/////////////
DECL_STITCH 
logical bhl_snap_bs3_curve_to_points(	bs3_curve&  ,bs3_curve& ,  cur_snap_data* ,const int );

DECL_STITCH bs2_curve bhl_bs2_curve_from_bs3(bs3_curve& );

logical
bhl_bs3_curve_connect (	bs3_curve&, bs3_curve& cur2,
					   bs3_curve&, const SPAposition&);


#endif

