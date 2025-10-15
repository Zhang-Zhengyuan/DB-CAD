/*******************************************************************/
/*    Copyright (c) 1989-2020 by Spatial Corp.                     */
/*    All rights reserved.                                         */
/*    Protected by U.S. Patents 5,257,205; 5,351,196; 6,369,815;   */
/*                              5,982,378; 6,462,738; 6,941,251    */
/*    Protected by European Patents 0503642; 69220263.3            */
/*    Protected by Hong Kong Patent 1008101A                       */
/*******************************************************************/

#if !defined (BHL_SPLINE_UTL)
#define BHL_SPLINE_UTL

#include "sp3crtn.hxx"
#include "bs3cutil.hxx"
#include "sp2crtn.hxx"

#include "splutil2.hxx"
// This header must be included on mac (instead of declaring class COEDGE_LIST)
// if using a function prototype with an array operator 
// (see bhl_get_degenerate_spot).
#include "coechain.hxx"


class surface;
class COEDGE;
class curve;
class pcurve;
class SPAposition;

logical bhl_bs3_make_degree_compat(bs3_curve& , bs3_curve& );

logical bhl_bs3_make_rat_compat( bs3_curve& , bs3_curve& );

logical bhl_bs3_make_param_compat(bs3_curve& , bs3_curve& );

logical bhl_bs3_make_knots_compat(bs3_curve& , bs3_curve& );

logical bhl_bs3_interpolate_curve(bs3_curve& , bs3_curve& ,
										double , bs3_curve& );

logical bhl_bs3_interpolate_curve_degen(bs3_curve& , curve* , double , bs3_curve& );

int bhl_get_degenerate_spot(COEDGE_CHAIN [4] , SPAposition& );

logical bhl_fill_2dgaps(COEDGE_CHAIN*, int, SURFACE*, int&);

logical bhl_make_points_for_fit(bs3_curve& , SPAposition* , SPApar_pos* , int ,
	const surface& , SPAposition*& , SPApar_pos*&,  int& , SPAunit_vector& , SPAunit_vector& );

logical bhl_fit_curve( SPAposition*, int, SPAunit_vector&, SPAunit_vector&, bs3_curve&);

void bhl_make_intcurve_from_pcurve(const pcurve& , intcurve*& );

bs3_curve bhl_extend_bs3_curve_to_range(bs3_curve& , const surface& );

void bhl_extend_curve_to_range(curve* , const surface& , curve*& );

struct bhl_surface_deviation  {
	double ext_pos_dev;
	double ext_nor_dev;
	double in_pos_dev;
	double in_nor_dev;
	double ext_pos_dev_avg;
	double ext_nor_dev_avg;
	double in_pos_dev_avg;
	double in_nor_dev_avg;
};

logical bhl_intersect_two_curves(curve* , curve* , SPAposition& );

logical bhl_compare_two_surfaces(const surface& , const surface& ,
					bhl_surface_deviation* );

logical
bhl_get_bs3_from_bs2_using_coedge_chain(bs2_curve& , const surface& , bs3_curve& , const COEDGE_CHAIN* const = NULL);


// Functional Class for Converting bs2_curve to bs3_curve using coedge chain
class bs3_from_bs2_curve_using_coedge_chain : public bs3_from_bs2_curve
{

// Attributes
protected:

	const COEDGE_CHAIN *m_chain; // Coedge chain for projection

// Exposed Functions
public:

	// Constructor
	bs3_from_bs2_curve_using_coedge_chain(const bs2_curve& bs2, const surface &surf, const COEDGE_CHAIN *chain) : bs3_from_bs2_curve(bs2, surf), m_chain(chain)
	{ /* do nothing here */	};

	// Destructor
	virtual ~bs3_from_bs2_curve_using_coedge_chain(){/* do nothing here */};

// Overridables
protected:

	// Project point (chain project point)
	virtual SPAposition project_point(SPApar_pos);

}; //  end of class bs3_from_bs2_curve_using_coedge_chain


#endif

// ---------------------------------------------------------------------------
// ---------------------------------------------------------------------------
