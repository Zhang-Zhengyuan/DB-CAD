/* ORIGINAL: acis2.1/agspline/bs2_crv/bs2c_def.hxx */
/* $Id: bs2c_def.hxx,v 1.1.1.1 2001/06/05 17:28:41 jeff Exp $ */
/*******************************************************************/
/*    Copyright (c) 1989-2020 by Spatial Corp.                     */
/*    All rights reserved.                                         */
/*    Protected by U.S. Patents 5,257,205; 5,351,196; 6,369,815;   */
/*                              5,982,378; 6,462,738; 6,941,251    */
/*    Protected by European Patents 0503642; 69220263.3            */
/*    Protected by Hong Kong Patent 1008101A                       */
/*******************************************************************/

// Definition of a 2D B-spline curve, used to approximate to true
// curves in the SPAparameter space of a surface. This provides insulation
// between the modeller and the underlying curve package.

// The great majority of Acis simply requires to know that a
// "bs2_curve" is a pointer to a class object of type "bs2_curve_def",
// and so only needs to include the file bs2_curve.hxx. Generally only
// the bs2/3 interface will need this file.

#if !defined( BS2_CURVE_DEF )
#define BS2_CURVE_DEF

#include "mmgr.hxx"

// Haul in the typedef for bs2_curve - it will always be wanted by
// anyone who uses this file.

#include "bs2curve.hxx"
#include "dcl_spl.h"
#include "base.hxx"

// Now the object to which it points.

struct ag_spline;

class DECL_SPLINE bs2_curve_def : public ACIS_OBJECT {

	ag_spline *cur;			// the curve itself
	bs2_curve_form form;	// (object-space) end conditions.
	size_t state;				// the paged state
    int m_initial_seam_knot_mult;
    int m_start_mult;

#ifdef GEOMPAGE_DEBUG
public:
	bs2_curve_def *prev;
	bs2_curve_def *next;
#endif

public:

	bs2_curve_def( ag_spline *, 
                   bs2_curve_form = bs2_curve_unknown_ends,
                   int = -1,
                   int = 0
                   );
	~bs2_curve_def();

	void trash();

	ag_spline * get_cur();
	ag_spline ** get_cur_ref();
	void set_cur( ag_spline *c);

	bs2_curve_form get_form();
	void set_form( bs2_curve_form f);

	void page();
	void unpage();
	int paged() { return state != 0; }

    void set_initial_seam_multiplicity(int seam_knot_mult, int start_mult);
    int  initial_seam_multiplicity(int &start_mult = *(int*)NULL_REF);
};

#endif
